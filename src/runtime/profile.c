/*
 * Sampling profiler: with CARNEVIL_PROFILE set, a thread samples the main
 * thread's instruction pointer every millisecond and prints the hottest
 * functions at exit, resolved from carnevil.map (written next to the exe).
 */
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SYMS 200000
typedef struct { unsigned long long rva; char name[96]; } sym;
static sym* g_syms;
static int g_nsyms;
static unsigned long long* g_samples;
static volatile LONG g_nsamples;
static HANDLE g_main;
static unsigned long long g_base, g_image_size;

static DWORD WINAPI sampler(LPVOID arg) {
    (void)arg;
    for (;;) {
        Sleep(1);
        if (SuspendThread(g_main) == (DWORD)-1) continue;
        CONTEXT c = { 0 };
        c.ContextFlags = CONTEXT_CONTROL;
        if (GetThreadContext(g_main, &c) && g_nsamples < 4000000)
            g_samples[InterlockedIncrement(&g_nsamples) - 1] = c.Rip - g_base;
        ResumeThread(g_main);
    }
}

static int cmp_sym(const void* a, const void* b) {
    unsigned long long x = ((const sym*)a)->rva, y = ((const sym*)b)->rva;
    return x < y ? -1 : x > y;
}

static void load_map(void) {
    char path[MAX_PATH];
    GetModuleFileNameA(NULL, path, sizeof(path));
    char* dot = strrchr(path, '.');
    if (dot) strcpy(dot, ".map");
    FILE* f = fopen(path, "r");
    if (!f) { fprintf(stderr, "[profile] no %s\n", path); return; }
    g_syms = malloc(sizeof(sym) * MAX_SYMS);
    char line[512];
    unsigned long long pref = 0x140000000ull;
    while (fgets(line, sizeof(line), f) && g_nsyms < MAX_SYMS) {
        char seg[32], name[256];
        unsigned long long va;
        if (strstr(line, "Preferred load address is")) sscanf(strstr(line, "is") + 2, "%llx", &pref);
        if (sscanf(line, " %31s %255s %llx", seg, name, &va) == 3 && strchr(seg, ':') && va > pref) {
            g_syms[g_nsyms].rva = va - pref;
            strncpy(g_syms[g_nsyms].name, name, sizeof(g_syms[0].name) - 1);
            g_nsyms++;
        }
    }
    fclose(f);
    qsort(g_syms, g_nsyms, sizeof(sym), cmp_sym);
}

static const char* lookup(unsigned long long rva) {
    if (rva >= g_image_size) {
        static char names[32][MAX_PATH];
        static HMODULE mods[32];
        static int nmods;
        HMODULE m = NULL;
        GetModuleHandleExA(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
                           (LPCSTR)(g_base + rva), &m);
        for (int i = 0; i < nmods; i++) if (mods[i] == m) return names[i];
        if (nmods == 32) return "[system]";
        mods[nmods] = m;
        strcpy(names[nmods], "[system] ");
        if (!m || !GetModuleFileNameA(m, names[nmods] + 9, MAX_PATH - 9)) strcat(names[nmods], "?");
        return names[nmods++];
    }
    int lo = 0, hi = g_nsyms - 1, best = -1;
    while (lo <= hi) {
        int mid = (lo + hi) / 2;
        if (g_syms[mid].rva <= rva) { best = mid; lo = mid + 1; } else hi = mid - 1;
    }
    return best >= 0 ? g_syms[best].name : "?";
}

typedef struct { const char* name; int count; } hit;
static int cmp_hit(const void* a, const void* b) { return ((const hit*)b)->count - ((const hit*)a)->count; }

static void report(void) {
    load_map();
    if (!g_nsyms) return;
    hit* hits = calloc(g_nsamples + 1, sizeof(hit));
    int nh = 0;
    for (LONG i = 0; i < g_nsamples; i++) {
        const char* n = lookup(g_samples[i]);
        int j;
        for (j = 0; j < nh && hits[j].name != n; j++) {}
        if (j == nh) { hits[nh].name = n; nh++; }
        hits[j].count++;
    }
    qsort(hits, nh, sizeof(hit), cmp_hit);
    fprintf(stderr, "[profile] %ld samples\n", g_nsamples);
    for (int i = 0; i < nh && i < 25; i++)
        fprintf(stderr, "[profile] %5.1f%%  %s\n", 100.0 * hits[i].count / g_nsamples, hits[i].name);
}

void profile_start(void) {
    if (!getenv("CARNEVIL_PROFILE")) return;
    g_samples = malloc(sizeof(unsigned long long) * 4000000);
    g_base = (unsigned long long)GetModuleHandleA(NULL);
    IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(g_base + ((IMAGE_DOS_HEADER*)g_base)->e_lfanew);
    g_image_size = nt->OptionalHeader.SizeOfImage;
    DuplicateHandle(GetCurrentProcess(), GetCurrentThread(), GetCurrentProcess(), &g_main, 0, FALSE, DUPLICATE_SAME_ACCESS);
    CreateThread(NULL, 0, sampler, NULL, 0, NULL);
    atexit(report);
}
