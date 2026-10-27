/* Two-player netplay, lockstep over TCP. See netplay.h.
 *
 * Adapted from model3recomp's netplay.c (MIT, the same author's Model 3
 * toolkit, as used by lostworld). Every message is 24 bytes, little-endian:
 *   byte 0 type, 4..7 frame, then four 32-bit words. */
#define WIN32_LEAN_AND_MEAN
#include <winsock2.h>
#include <ws2tcpip.h>
#include "netplay.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NP_VERSION    1u
#define NP_MSG        24
#define NP_RING       256u       /* frames of input held */
#define NP_TIMEOUT_MS 15000

enum { MSG_HELLO = 1, MSG_INPUT = 2, MSG_HASH = 3, MSG_SETUP = 4, MSG_BYE = 5 };

static SOCKET   g_sock = INVALID_SOCKET;
static int      g_role;              /* 0 none, 1 host, 2 joiner */
static unsigned g_delay = 3;
static char     g_status[128];
static int      g_desync;
static uint64_t g_first = UINT64_MAX;
static netplay_pump_fn g_pump;

static struct { uint64_t f; int valid; cabinet_inputs in; } g_remote[NP_RING];
static struct { uint64_t f; uint32_t h; int valid; } g_hash_mine, g_hash_peer;

int netplay_active(void) { return g_role != 0; }
int netplay_role(void) { return g_role; }
const char* netplay_status(void) { return g_status; }

/* ---- wire --------------------------------------------------------------- */
static void put32(uint8_t* p, uint32_t v) { p[0] = (uint8_t)v; p[1] = (uint8_t)(v >> 8); p[2] = (uint8_t)(v >> 16); p[3] = (uint8_t)(v >> 24); }
static uint32_t get32(const uint8_t* p) { return p[0] | ((uint32_t)p[1] << 8) | ((uint32_t)p[2] << 16) | ((uint32_t)p[3] << 24); }

static int send_all(const void* data, size_t n) {
    const char* p = data;
    while (n) {
        int k = send(g_sock, p, (int)(n > 65536 ? 65536 : n), 0);
        if (k <= 0) return 0;
        p += k; n -= (size_t)k;
    }
    return 1;
}

static int send_msg(unsigned type, uint64_t f, uint32_t a, uint32_t b, uint32_t c, uint32_t d) {
    uint8_t m[NP_MSG] = { 0 };
    m[0] = (uint8_t)type;
    put32(m + 4, (uint32_t)f);
    put32(m + 8, a); put32(m + 12, b); put32(m + 16, c); put32(m + 20, d);
    return send_all(m, sizeof m);
}

/* Wait for data, keeping the window alive; closing it ends the program. */
static int wait_readable(int ms) {
    fd_set rs;
    struct timeval tv = { ms / 1000, (ms % 1000) * 1000 };
    FD_ZERO(&rs);
    FD_SET(g_sock, &rs);
    return select(0, &rs, NULL, NULL, &tv) > 0;
}

static int recv_all(void* data, size_t n) {
    char* p = data;
    int waited = 0;
    while (n) {
        if (!wait_readable(20)) {
            if (g_pump && !g_pump()) { netplay_shutdown(); exit(0); }
            if ((waited += 20) > NP_TIMEOUT_MS) return 0;
            continue;
        }
        int k = recv(g_sock, p, (int)(n > 65536 ? 65536 : n), 0);
        if (k <= 0) return 0;
        p += k; n -= (size_t)k; waited = 0;
    }
    return 1;
}

static void lost(const char* why) {
    fprintf(stderr, "[netplay] %s -- playing on alone\n", why);
    snprintf(g_status, sizeof g_status, "netplay: %s", why);
    if (g_sock != INVALID_SOCKET) closesocket(g_sock);
    g_sock = INVALID_SOCKET;
    g_role = 0;
}

/* ---- inputs ------------------------------------------------------------- */
static int send_input(uint64_t f, const cabinet_inputs* in) {
    const gun_state* g = &in->gun[0];
    return send_msg(MSG_INPUT, f, in->system | ((uint32_t)in->dips << 16),
                    g->x | ((uint32_t)g->y << 16), g->buttons, in->cheats);
}

static int pump_one(void) {
    uint8_t m[NP_MSG];
    if (!recv_all(m, sizeof m)) return 0;
    uint64_t f = get32(m + 4);
    switch (m[0]) {
        case MSG_INPUT: {
            unsigned k = (unsigned)(f % NP_RING);
            cabinet_inputs* in = &g_remote[k].in;
            memset(in, 0, sizeof *in);
            in->system = (uint16_t)get32(m + 8);
            in->dips = (uint16_t)(get32(m + 8) >> 16);
            in->gun[0].x = (uint16_t)get32(m + 12);
            in->gun[0].y = (uint16_t)(get32(m + 12) >> 16);
            in->gun[0].buttons = (uint8_t)get32(m + 16);
            in->cheats = get32(m + 20);
            g_remote[k].f = f;
            g_remote[k].valid = 1;
            break;
        }
        case MSG_HASH:
            g_hash_peer.f = f; g_hash_peer.h = get32(m + 8); g_hash_peer.valid = 1;
            break;
        case MSG_BYE:
            return 0;
    }
    return 1;
}

static void check_hashes(void) {
    if (!g_hash_mine.valid || !g_hash_peer.valid || g_hash_mine.f != g_hash_peer.f) return;
    if (g_hash_mine.h != g_hash_peer.h && !g_desync) {
        g_desync = 1;
        fprintf(stderr, "[netplay] DESYNC at frame %llu: %08X here, %08X there\n",
                (unsigned long long)g_hash_mine.f, g_hash_mine.h, g_hash_peer.h);
        snprintf(g_status, sizeof g_status, "netplay: DESYNC at frame %llu", (unsigned long long)g_hash_mine.f);
    }
    g_hash_peer.valid = 0;
}

/* Each machine's own player is in its player-1 slots (gun 0, coin 1,
 * start 1); the joiner's are moved over to player 2. The host also owns
 * the operator controls: service, test, tilt, the DIPs and the cheats. */
static void merge(const cabinet_inputs* host, const cabinet_inputs* join, cabinet_inputs* out) {
    uint16_t host_bits = SYS_COIN1 | SYS_START1 | SYS_SERVICE | SYS_TEST | SYS_TILT;
    uint16_t pressed = (uint16_t)~host->system & host_bits;          /* active low */
    uint16_t j = (uint16_t)~join->system;
    if (j & (SYS_COIN1 | SYS_COIN2)) pressed |= SYS_COIN2;
    if (j & (SYS_START1 | SYS_START2)) pressed |= SYS_START2;
    out->system = (uint16_t)~pressed;
    out->dips = host->dips;
    out->gun[0] = host->gun[0];
    out->gun[1] = join->gun[0];
    out->cheats = host->cheats;
}

/* The first `delay` frames of a session, before any input can have crossed:
 * the same on both machines. */
static void neutral(cabinet_inputs* in) {
    memset(in, 0, sizeof *in);
    in->system = 0xFFFF;
    in->dips = 0xBDFE;
    in->gun[0].x = in->gun[0].y = in->gun[1].x = in->gun[1].y = 32768;
}

int netplay_exchange(const cabinet_inputs* local, cabinet_inputs* out, uint64_t f) {
    static cabinet_inputs mine[NP_RING];
    cabinet_inputs me, remote;
    if (!g_role) { *out = *local; return 0; }
    if (g_first == UINT64_MAX) g_first = f;

    /* Mine for f + delay goes out now. */
    mine[(f + g_delay) % NP_RING] = *local;
    if (!send_input(f + g_delay, local)) { lost("the other player went away"); *out = *local; return -1; }

    if (f < g_first + g_delay) {
        neutral(&me);
        neutral(&remote);
    } else {
        unsigned k = (unsigned)(f % NP_RING);
        while (!(g_remote[k].valid && g_remote[k].f == f))
            if (!pump_one()) { lost("the other player went away"); *out = *local; return -1; }
        remote = g_remote[k].in;
        g_remote[k].valid = 0;
        me = mine[k];
    }
    check_hashes();
    if (g_role == 1) merge(&me, &remote, out); else merge(&remote, &me, out);
    return 1;
}

void netplay_check(uint64_t f, uint32_t ram_hash) {
    if (!g_role) return;
    g_hash_mine.f = f; g_hash_mine.h = ram_hash; g_hash_mine.valid = 1;
    if (f % 600 == 0) fprintf(stderr, "[netplay] frame %llu ram %08X\n", (unsigned long long)f, ram_hash);
    send_msg(MSG_HASH, f, ram_hash, 0, 0, 0);
    check_hashes();
}

/* ---- connecting --------------------------------------------------------- */
static int net_up(void) {
    WSADATA w;
    return WSAStartup(MAKEWORD(2, 2), &w) == 0;
}

static SOCKET listen_on(int port) {
    char ps[16];
    struct addrinfo hints = { 0 }, *ai = NULL;
    snprintf(ps, sizeof ps, "%d", port);
    hints.ai_family = AF_INET6;              /* dual-stack: v4 and v6 */
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    if (getaddrinfo(NULL, ps, &hints, &ai) != 0) {
        hints.ai_family = AF_INET;
        if (getaddrinfo(NULL, ps, &hints, &ai) != 0) return INVALID_SOCKET;
    }
    SOCKET ls = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
    if (ls == INVALID_SOCKET) { freeaddrinfo(ai); return INVALID_SOCKET; }
    int one = 1, zero = 0;
    setsockopt(ls, SOL_SOCKET, SO_REUSEADDR, (const char*)&one, sizeof one);
    if (ai->ai_family == AF_INET6) setsockopt(ls, IPPROTO_IPV6, IPV6_V6ONLY, (const char*)&zero, sizeof zero);
    if (bind(ls, ai->ai_addr, (int)ai->ai_addrlen) != 0 || listen(ls, 1) != 0) {
        freeaddrinfo(ai); closesocket(ls); return INVALID_SOCKET;
    }
    freeaddrinfo(ai);
    snprintf(g_status, sizeof g_status, "netplay: hosting on port %d, waiting for player 2", port);
    fprintf(stderr, "[netplay] %s\n", g_status);
    SOCKET s = INVALID_SOCKET;
    for (;;) {
        fd_set rs;
        struct timeval tv = { 0, 50000 };
        FD_ZERO(&rs); FD_SET(ls, &rs);
        if (select(0, &rs, NULL, NULL, &tv) > 0) { s = accept(ls, NULL, NULL); break; }
        if (g_pump && !g_pump()) { closesocket(ls); exit(0); }
    }
    closesocket(ls);
    return s;
}

static SOCKET connect_to(const char* host, const char* port) {
    struct addrinfo hints = { 0 }, *ai = NULL;
    snprintf(g_status, sizeof g_status, "netplay: joining %s:%s", host, port);
    fprintf(stderr, "[netplay] %s\n", g_status);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    /* The host may still be starting up: keep trying for half a minute
     * (by the clock: a refused connect alone can take seconds on Windows). */
    ULONGLONG deadline = GetTickCount64() + 30000;
    while (GetTickCount64() < deadline) {
        if (getaddrinfo(host, port, &hints, &ai) == 0) {
            for (struct addrinfo* p = ai; p; p = p->ai_next) {
                SOCKET s = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
                if (s == INVALID_SOCKET) continue;
                if (connect(s, p->ai_addr, (int)p->ai_addrlen) == 0) { freeaddrinfo(ai); return s; }
                closesocket(s);
            }
            freeaddrinfo(ai);
        }
        for (int i = 0; i < 10 && GetTickCount64() < deadline; i++) {
            if (g_pump && !g_pump()) exit(0);
            Sleep(50);
        }
    }
    return INVALID_SOCKET;
}

/* Hello both ways: version, role and the host's input delay. */
static int hello(void) {
    uint8_t m[NP_MSG];
    int one = 1;
    setsockopt(g_sock, IPPROTO_TCP, TCP_NODELAY, (const char*)&one, sizeof one);
    if (!send_msg(MSG_HELLO, NP_VERSION, (uint32_t)g_role, g_delay, 0, 0) || !recv_all(m, sizeof m) || m[0] != MSG_HELLO) {
        lost("handshake failed"); return 0;
    }
    if (get32(m + 4) != NP_VERSION) { lost("the other side runs a different version"); return 0; }
    if ((int)get32(m + 8) == g_role) { lost("both sides chose the same role"); return 0; }
    if (g_role == 2) g_delay = get32(m + 12);
    return 1;
}

static void connected(void) {
    snprintf(g_status, sizeof g_status, "netplay: %s, delay %u",
             g_role == 1 ? "host (player 1)" : "joined (player 2)", g_delay);
    fprintf(stderr, "[netplay] %s\n", g_status);
}

int netplay_host(int port, int delay, const void* setup, size_t setup_size, netplay_pump_fn pump) {
    g_pump = pump;
    g_delay = delay < 1 ? 1 : delay > 30 ? 30 : (unsigned)delay;
    if (!net_up()) return 0;
    g_role = 1;
    g_sock = listen_on(port);
    if (g_sock == INVALID_SOCKET) { lost("could not listen on that port"); return 0; }
    if (!hello()) return 0;
    if (!send_msg(MSG_SETUP, 0, (uint32_t)setup_size, 0, 0, 0) || !send_all(setup, setup_size)) {
        lost("could not send the settings"); return 0;
    }
    connected();
    return 1;
}

int netplay_join(const char* addr_port, void** setup, size_t* setup_size, netplay_pump_fn pump) {
    char buf[256];
    g_pump = pump;
    snprintf(buf, sizeof buf, "%s", addr_port);
    char* port = strrchr(buf, ':');
    char* host = buf;
    if (!port) { snprintf(g_status, sizeof g_status, "netplay: join needs ADDRESS:PORT"); return 0; }
    *port++ = 0;
    if (*host == '[') {                         /* [v6]:port */
        host++;
        size_t n = strlen(host);
        if (n && host[n - 1] == ']') host[n - 1] = 0;
    }
    if (!net_up()) return 0;
    g_role = 2;
    g_sock = connect_to(host, port);
    if (g_sock == INVALID_SOCKET) { lost("could not connect"); return 0; }
    if (!hello()) return 0;
    uint8_t m[NP_MSG];
    if (!recv_all(m, sizeof m) || m[0] != MSG_SETUP) { lost("no settings from the host"); return 0; }
    size_t n = get32(m + 8);
    void* b = malloc(n ? n : 1);
    if (!b || !recv_all(b, n)) { free(b); lost("could not receive the settings"); return 0; }
    *setup = b;
    *setup_size = n;
    connected();
    return 1;
}

void netplay_shutdown(void) {
    if (g_sock != INVALID_SOCKET) {
        send_msg(MSG_BYE, 0, 0, 0, 0, 0);
        closesocket(g_sock);
    }
    g_sock = INVALID_SOCKET;
    g_role = 0;
}
