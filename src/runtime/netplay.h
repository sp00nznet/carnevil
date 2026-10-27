/*
 * Two players over the network, lockstep over TCP.
 *
 * The runtime is deterministic: emulated time comes from the work the
 * recompiled code does, not from the host clock, so two machines that boot
 * the same build from power-on with the same CMOS and settings, and are fed
 * the same inputs on the same frames, stay identical. Netplay only trades
 * inputs. Each side sends its own a few frames ahead (the input delay) and,
 * before every frame, waits for the other's. The host plays player 1, the
 * joiner player 2.
 *
 * A session starts at power-on, so the menu's Host and Join relaunch the
 * program with --host PORT / --join ADDR:PORT.
 */
#ifndef CARNEVIL_NETPLAY_H
#define CARNEVIL_NETPLAY_H

#include <stddef.h>
#include <stdint.h>
#include "hw.h"

/* Called while waiting for the peer; return 0 to give up (window closed). */
typedef int (*netplay_pump_fn)(void);

/* Host: listen on port, wait for a joiner, send it the setup blob (the CMOS
 * image and settings both machines boot with). delay is the input delay in
 * frames; the host decides it for both. Returns 0 on failure. */
int netplay_host(int port, int delay, const void* setup, size_t setup_size, netplay_pump_fn pump);

/* Join "address:port" (a LAN address, a Tailscale name, a forwarded public
 * address). On success *setup / *setup_size hold the host's blob (malloc'd).
 * Returns 0 on failure. */
int netplay_join(const char* addr_port, void** setup, size_t* setup_size, netplay_pump_fn pump);

void netplay_shutdown(void);

int netplay_active(void);
int netplay_role(void);              /* 1 host (player 1), 2 joiner (player 2) */
const char* netplay_status(void);    /* one line for the title bar */

/* Trade inputs for frame f. local is this machine's controls; out gets the
 * merged inputs both machines run: the host's gun 0, coin 1, start 1,
 * service, test switch, DIPs and cheats; the joiner's gun 1, coin 2 and
 * start 2. Returns 0 when there is no session (out = *local). Returns -1 if
 * the peer went away. */
int netplay_exchange(const cabinet_inputs* local, cabinet_inputs* out, uint64_t f);

/* The machine calls this every frame with a hash of main RAM; the two sides
 * compare them now and then and the status line says if they ever differ. */
void netplay_check(uint64_t f, uint32_t ram_hash);

#endif
