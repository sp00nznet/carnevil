/**
 * Function overrides for CarnEvil runtime.
 * These replace NOP/BREAK stubs in the recompiled output with actual implementations.
 * They must have the same symbol names as the recompiled functions to override at link time.
 */

#include "recomp.h"
#include "rtos_scheduler.h"
#include "voodoo.h"
#include <stdio.h>
#include <stdlib.h>
#include <setjmp.h>

extern uint8_t* seattle_rdram;
extern voodoo_state_t g_voodoo;
extern uint32_t voodoo_get_write_count(void);

/* DCS2 sound system state */
uint32_t g_dcs_last_write = 0x55AA;
uint32_t g_render_counter = 0;

/* File device table for RTOS device I/O.
 * Indices 0x10-0x3F are file devices opened via device_open. */
struct file_dev_entry {
    char name[64];
    uint32_t data_phys;
    uint32_t size;
    int active;
};
struct file_dev_entry g_file_devs[32];
int g_file_dev_count = 0;
uint32_t g_file_next_data_phys = 0x600000;

/* Forward declaration for heap check utility */
static void override_check_heap(const uint8_t* rdram, const char* when);

/* ======================================================================
 * IOASIC Command Handler
 * Replaces: static_0_800C4154 (NOP pad in original binary)
 * Called as: ioasic_cmd(r4=type, r5=cmd_id, r6=response_ptr)
 * ====================================================================== */

/* ======================================================================
 * RTOS varargs/device open -- replaces static_0_801451F0
 * This stub was originally just varargs_save, but the game uses it as
 * an RTOS open_device() call. Return a valid device handle.
 * ====================================================================== */

static int open_count = 0;

/* func_80144E70: RTOS device open wrapper
 * Called during sys_init_2 with a0=device_name_ptr, a1=output_buf, a2=flags
 * Return 0 = success, non-zero = error. If this returns error,
 * sys_init_2 bails out at 0x80140E5C and the game never inits. */
/* vec[7] at 0x800C40C4 -> rtos_8000B70C: RTOS device I/O handler.
 * This is the actual hardware I/O dispatch. On real hardware it talks to
 * the Galileo/Voodoo/IOASIC. We stub it to return success. */
/* Generic RTOS vector call tracer - logs all calls to help identify I/O sequence */
static int vec_trace_count = 0;
static void trace_vec_call(const char* name, int vec_idx, uint8_t* rdram, recomp_context* ctx) {
    vec_trace_count++;
    if (vec_trace_count <= 100) {
        fprintf(stderr, "[vec] %s vec[%d] a0=0x%08X a1=0x%08X a2=0x%08X a3=0x%08X\n",
                name, vec_idx, (uint32_t)ctx->r4, (uint32_t)ctx->r5,
                (uint32_t)ctx->r6, (uint32_t)ctx->r7);
    }
}

/* Vector call tracing removed - using function lookup logging instead */

/* The RTOS file I/O model:
 * - device_open (func_80144E70): opens a named device/file, returns handle
 * - device_read (vec entry): reads data from handle into buffer
 * - device_close (vec entry): closes handle
 *
 * We implement a simple file handle system that serves game.env with
 * hardware configuration environment strings. */

#define MAX_RTOS_FILES 32
static struct {
    int active;
    char name[64];
    uint8_t* data;       /* file data (malloc'd from host heap, not game heap) */
    uint32_t size;
    uint32_t pos;
    int owns_data;       /* 1 = we malloc'd data, 0 = static data */
} rtos_files[MAX_RTOS_FILES];

/* CarnEvil game.env: hardware config environment strings.
 * Format: null-terminated "KEY=VALUE" strings, double-null terminated.
 * The RTOS creates this from detected hardware configuration. */
/* Environment file: NEWLINE-separated key=value pairs.
 * The game counts newlines to determine number of entries,
 * then parses each line. Must use \n not \0 as separator! */
/* Environment strings from the Seattle RTOS boot.
 * Must match what the Mother GOOSE RTOS expects.
 * Newline-separated, game counts \n chars to determine entry count. */
static const char game_env_data[] =
    "GAME=carnevil\n"
    "CLOCK_SPEED=150\n"     /* R5000 @ 150MHz */
    "SDRAM_SIZE=8\n"        /* 8MB SDRAM */
    "PROCESSORS=1\n"        /* single CPU */
    "DISPLAY=norml\n"       /* display config */
    "RESOLUTION=norml\n"    /* resolution setting */
    "FBI_MEM=2\n"           /* 2MB FBI framebuffer */
    "TMU_MEM=4\n"           /* 4MB TMU0 texture memory */
    "TMU0_MEM=4\n"          /* explicit TMU0 */
    "TMU1_MEM=0\n"          /* no TMU1 */
    "MONITOR=norml\n"       /* monitor type */
    "VIDEO=norml\n"         /* video output */
    "SERIAL=486\n"          /* PIC serial number */
    "GFX_SPEED=norml\n"     /* Voodoo clock speed */
    "SOUND=dcs2\n"          /* sound system */
    ;                       /* C string auto-null terminates */

static int rtos_file_open(const char* name) {
    /* Find a free slot */
    int slot = -1;
    for (int i = 0; i < MAX_RTOS_FILES; i++) {
        if (!rtos_files[i].active) { slot = i; break; }
    }
    if (slot < 0) {
        /* Try to reclaim oldest slot */
        slot = 0;
        if (rtos_files[slot].owns_data && rtos_files[slot].data)
            free(rtos_files[slot].data);
    }

    rtos_files[slot].active = 1;
    strncpy(rtos_files[slot].name, name, 63);
    rtos_files[slot].name[63] = 0;
    rtos_files[slot].pos = 0;
    rtos_files[slot].owns_data = 0;
    rtos_files[slot].data = NULL;
    rtos_files[slot].size = 0;

    if (strcmp(name, "game.env") == 0 || strcmp(name, "GAME.ENV") == 0) {
        rtos_files[slot].data = (uint8_t*)game_env_data;
        rtos_files[slot].size = sizeof(game_env_data);
        rtos_files[slot].owns_data = 0;
    } else {
        /* Try loading from extracted/files/ directory */
        char path[256];
        snprintf(path, sizeof(path), "extracted/files/%s", name);
        FILE* f = fopen(path, "rb");
        if (!f) {
            /* Try uppercase */
            char upper_name[64];
            for (int i = 0; name[i] && i < 63; i++) {
                upper_name[i] = (name[i] >= 'a' && name[i] <= 'z') ? name[i] - 32 : name[i];
                upper_name[i+1] = 0;
            }
            snprintf(path, sizeof(path), "extracted/files/%s", upper_name);
            f = fopen(path, "rb");
        }
        if (f) {
            fseek(f, 0, SEEK_END);
            long sz = ftell(f);
            fseek(f, 0, SEEK_SET);
            if (sz > 0 && sz < 16 * 1024 * 1024) { /* max 16MB per file */
                uint8_t* buf = (uint8_t*)malloc((size_t)sz);
                if (buf) {
                    fread(buf, 1, (size_t)sz, f);
                    /* WMS texture files carry a 512-byte boot-thumbnail header
                     * (RGB preview) before the real payload -- the version word
                     * 0x8005 lives at +0x200. The game's file layer skips it; our
                     * extraction keeps it, so strip it here. After stripping, the
                     * stat size and the data both reflect the payload, so the
                     * texture loader's version + size + CRC checks all line up.
                     * (.EXE files get their 0x208 header stripped at extraction;
                     * .WMS were left intact.) */
                    {
                        size_t nl = strlen(name);
                        int is_wms = (nl >= 4 &&
                            (name[nl-4]=='.') &&
                            (name[nl-3]=='W'||name[nl-3]=='w') &&
                            (name[nl-2]=='M'||name[nl-2]=='m') &&
                            (name[nl-1]=='S'||name[nl-1]=='s'));
                        if (is_wms && sz > 0x200) {
                            memmove(buf, buf + 0x200, (size_t)sz - 0x200);
                            sz -= 0x200;
                        }
                    }
                    rtos_files[slot].data = buf;
                    rtos_files[slot].size = (uint32_t)sz;
                    rtos_files[slot].owns_data = 1;
                }
            }
            fclose(f);
            static int load_log = 0;
            load_log++;
            if (load_log <= 20) {
                fprintf(stderr, "[file] Loaded '%s' (%ld bytes)\n", path, sz);
            }
        } else {
            static int miss_log = 0;
            miss_log++;
            if (miss_log <= 20) {
                fprintf(stderr, "[file] NOT FOUND: '%s'\n", name);
            }
        }
    }
    return slot + 1; /* handle = 1-based index */
}

static int rtos_file_read(int handle, uint8_t* dest, uint32_t count) {
    if (handle < 1 || handle > MAX_RTOS_FILES) return -1;
    struct { int active; char name[64]; const uint8_t* data; uint32_t size; uint32_t pos; }
        *fp = &rtos_files[handle - 1];
    if (!fp->active || !fp->data) return -1;

    uint32_t avail = fp->size - fp->pos;
    if (count > avail) count = avail;
    if (count > 0) {
        memcpy(dest, fp->data + fp->pos, count);
        fp->pos += count;
    }
    return (int)count;
}

RECOMP_FUNC void rtos_vec7_device_io(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = 0;
}

/* Map fopen handles to rtos_file slots.
 * fopen returns a "file pointer" that we encode as 0xF1000000 | slot.
 * This lets us distinguish our handles from game RAM pointers. */
#define FOPEN_MAGIC 0xF1000000
#define FOPEN_SLOT(h) ((int)((h) & 0xFF))
#define IS_FOPEN_HANDLE(h) (((h) & 0xFF000000) == FOPEN_MAGIC)

/* func_80142C58: RTOS fopen(name_ptr, mode_string)
 * The first argument is a pointer to a filename string in game RAM.
 * Returns file handle or 0 on error. */
RECOMP_FUNC void func_80142C58(uint8_t* rdram, recomp_context* ctx) {
    uint32_t name_virt = (uint32_t)ctx->r4;
    uint32_t name_phys = name_virt & 0x1FFFFFFF;
    uint32_t mode_phys = (uint32_t)ctx->r5 & 0x1FFFFFFF;

    /* Read filename string */
    char filename[64] = {0};
    if (name_phys < 0x00800000 - 64) {
        for (int i = 0; i < 63; i++) {
            char c = rdram[name_phys + i];
            if (!c) break;
            filename[i] = c;
        }
    }

    /* Read mode string */
    char mode[16] = {0};
    if (mode_phys < 0x00800000 - 16) {
        for (int i = 0; i < 15; i++) {
            char c = rdram[mode_phys + i];
            if (!c) break;
            mode[i] = c;
        }
    }

    /* Reject empty filenames (directory table not populated) */
    if (filename[0] == '\0') {
        ctx->r2 = 0; /* NULL = failed */
        return;
    }

    /* Open the file */
    int handle = rtos_file_open(filename);

    static int fopen_log = 0;
    fopen_log++;
    if (fopen_log <= 30 || (handle > 0 && rtos_files[handle-1].data)) {
        if (fopen_log <= 30)
            fprintf(stderr, "[file] fopen(\"%s\", \"%s\") -> handle %d (%u bytes)\n",
                    filename, mode, handle,
                    handle > 0 ? rtos_files[handle-1].size : 0);
    }

    if (handle > 0 && rtos_files[handle-1].data) {
        /* Reset read position for new open */
        rtos_files[handle-1].pos = 0;
        ctx->r2 = (gpr)(FOPEN_MAGIC | (handle - 1));
    } else {
        ctx->r2 = 0; /* failed */
    }
}

/* func_80142F10: RTOS fread(dest_buf, element_size, count, handle)
 * Returns number of elements read. */
RECOMP_FUNC void func_80142F10(uint8_t* rdram, recomp_context* ctx) {
    uint32_t dest_virt = (uint32_t)ctx->r4;
    uint32_t dest_phys = dest_virt & 0x1FFFFFFF;
    uint32_t elem_size = (uint32_t)ctx->r5;
    uint32_t count = (uint32_t)ctx->r6;
    uint32_t handle = (uint32_t)ctx->r7;
    uint32_t total = elem_size * count;

    int bytes_read = 0;

    if (IS_FOPEN_HANDLE(handle)) {
        int slot = FOPEN_SLOT(handle);
        if (slot >= 0 && slot < MAX_RTOS_FILES && rtos_files[slot].active && rtos_files[slot].data) {
            if (dest_phys > 0 && dest_phys < 0x00800000 && total > 0 &&
                dest_phys + total <= 0x00800000) {
                bytes_read = rtos_file_read(slot + 1, rdram + dest_phys, total);
            }
        }
    } else {
        /* Legacy: try all active files */
        for (int i = 0; i < MAX_RTOS_FILES; i++) {
            if (rtos_files[i].active && rtos_files[i].data) {
                if (dest_phys > 0 && dest_phys < 0x00800000 && total > 0 &&
                    dest_phys + total <= 0x00800000) {
                    bytes_read = rtos_file_read(i + 1, rdram + dest_phys, total);
                    if (bytes_read > 0) break;
                }
            }
        }
    }

    static int fread_log = 0;
    fread_log++;
    if (fread_log <= 20 || bytes_read == 0) {
        if (fread_log <= 30)
            fprintf(stderr, "[file] fread(dest=0x%08X, %u, %u, total=%u, h=0x%X) -> %d bytes\n",
                    dest_virt, elem_size, count, total, handle, bytes_read);
    }

    /* Return number of ELEMENTS read (like C fread) */
    if (bytes_read > 0 && elem_size > 0) {
        ctx->r2 = (gpr)(int32_t)(bytes_read / elem_size);
    } else {
        ctx->r2 = 0;
    }
}

/* func_80142850: RTOS fclose(handle) */
RECOMP_FUNC void func_80142850(uint8_t* rdram, recomp_context* ctx) {
    uint32_t handle = (uint32_t)ctx->r4;

    if (IS_FOPEN_HANDLE(handle)) {
        int slot = FOPEN_SLOT(handle);
        if (slot >= 0 && slot < MAX_RTOS_FILES) {
            static int fclose_log = 0;
            fclose_log++;
            if (fclose_log <= 10)
                fprintf(stderr, "[file] fclose(\"%s\")\n", rtos_files[slot].name);
            rtos_files[slot].active = 0;
            /* Don't free data - might be reopened. Host memory is cheap. */
        }
    }
    ctx->r2 = 0;
}

/* func_80143E74: RTOS free(ptr) - call real implementation.
 * Original renamed to func_80143E74_original in funcs_30.c.
 * MUST work for attract mode to load/free/reload game assets. */
extern RECOMP_FUNC void func_80143E74_original(uint8_t* rdram, recomp_context* ctx);
RECOMP_FUNC void func_80143E74(uint8_t* rdram, recomp_context* ctx) {
    uint32_t ptr = (uint32_t)ctx->r4;
    if (ptr != 0) {
        func_80143E74_original(rdram, ctx);
    }
    ctx->r2 = 0;
}

RECOMP_FUNC void func_80144E70(uint8_t* rdram, recomp_context* ctx) {
    uint32_t name_ptr = (uint32_t)ctx->r4 & 0x1FFFFFFF;
    char name_buf[64] = {0};
    if (name_ptr < 0x00800000 - 64) {
        for (int i = 0; i < 63; i++) {
            char c = *(char*)(rdram + name_ptr + i);
            if (c == 0) break;
            name_buf[i] = c;
        }
    }
    /* Skip BNK (sound bank) files — DCS2 hardware not emulated.
     * Return success with handle 1 and size 0 to prevent retry loops.
     * The caller expects a valid handle + size in the output descriptor. */
    {
        int len = (int)strlen(name_buf);
        if (len >= 4 && (strcmp(name_buf + len - 4, ".BNK") == 0 ||
                         strcmp(name_buf + len - 4, ".bnk") == 0)) {
            static int bnk_skip = 0;
            bnk_skip++;
            if (bnk_skip <= 5) {
                fprintf(stderr, "[rtos] device_open(\"%s\") -> DUMMY (sound bank)\n", name_buf);
            }
            /* Fill output descriptor with small dummy size so caller
             * can malloc successfully and doesn't retry.
             * Size at offset 0x1A-0x1D in big-endian format. */
            uint32_t out_phys = (uint32_t)ctx->r5 & 0x1FFFFFFF;
            if (out_phys > 0 && out_phys + 0x20 < 0x00800000) {
                memset(rdram + out_phys, 0, 0x20);
                /* Set size = 64 bytes (big-endian at offset 0x1A) */
                rdram[out_phys + 0x1A] = 0;
                rdram[out_phys + 0x1B] = 0;
                rdram[out_phys + 0x1C] = 0;
                rdram[out_phys + 0x1D] = 64;
            }
            ctx->r2 = 1; /* success handle */
            return;
        }
    }

    fprintf(stderr, "[rtos] device_open(\"%s\", out=0x%08X, flags=0x%X) -> success\n",
            name_buf, (uint32_t)ctx->r5, (uint32_t)ctx->r6);

    /* Open the file and get a handle */
    int handle = rtos_file_open(name_buf);
    if (handle < 0) {
        ctx->r2 = (gpr)(int32_t)-1;
        return;
    }

    /* Fill the output buffer (device descriptor struct).
     * Layout (from sys_init_2 disassembly):
     * [+0x00..0x19]: device handle info
     * [+0x1A..0x1D]: file size (big-endian 32-bit) - read by game to alloc buffer
     * The game does: lbu sp+0x2A..0x2D, combines big-endian, calls malloc(size) */
    uint32_t out_phys = (uint32_t)ctx->r5 & 0x1FFFFFFF;
    uint32_t safe_size = 64; /* only write the essential fields */
    if (out_phys + safe_size < 0x00800000) {
        memset(rdram + out_phys, 0, safe_size);

        /* Store file handle at +0x00 */
        *(uint32_t*)(rdram + out_phys + 0x00) = (uint32_t)handle;

        /* Store file size at +0x1A as big-endian 32-bit
         * The game reads bytes at buf+0x1A, 0x1B, 0x1C, 0x1D and
         * combines as: (b[0x1B] << 8) | b[0x1A] | (b[0x1C] << 16) | (b[0x1D] << 24) */
        uint32_t fsize = rtos_files[handle - 1].size;
        rdram[out_phys + 0x1A] = (uint8_t)(fsize & 0xFF);
        rdram[out_phys + 0x1B] = (uint8_t)((fsize >> 8) & 0xFF);
        rdram[out_phys + 0x1C] = (uint8_t)((fsize >> 16) & 0xFF);
        rdram[out_phys + 0x1D] = (uint8_t)((fsize >> 24) & 0xFF);

        fprintf(stderr, "[rtos] device_open: handle=%d, size=%u bytes, out_phys=0x%06X\n",
                handle, fsize, out_phys);
    } else {
        fprintf(stderr, "[rtos] device_open: out_phys=0x%06X is out of RDRAM range!\n", out_phys);
        /* The output buffer is on the stack near top of RAM.
         * We can still write the essential fields if in range. */
        if (out_phys + 0x1E <= 0x00800000) {
            rdram[out_phys + 0x00] = (uint8_t)handle;
            uint32_t fsize2 = rtos_files[handle - 1].size;
            rdram[out_phys + 0x1A] = (uint8_t)(fsize2 & 0xFF);
            rdram[out_phys + 0x1B] = (uint8_t)((fsize2 >> 8) & 0xFF);
            rdram[out_phys + 0x1C] = (uint8_t)((fsize2 >> 16) & 0xFF);
            rdram[out_phys + 0x1D] = (uint8_t)((fsize2 >> 24) & 0xFF);
        }
    }

    ctx->r2 = 0; /* success */
}

RECOMP_FUNC void static_0_801451F0(uint8_t* rdram, recomp_context* ctx) {
    MEM_W(0x4, ctx->r29) = ctx->r5;
    MEM_W(0x8, ctx->r29) = ctx->r6;
    MEM_W(0xC, ctx->r29) = ctx->r7;

    /* Read the device name string from a0 */
    uint32_t name_phys = (uint32_t)ctx->r4 & 0x1FFFFFFF;
    char devname[64] = {0};
    if (name_phys < 0x00800000 - 64) {
        for (int i = 0; i < 63; i++) {
            char c = *(char*)(rdram + name_phys + i);
            if (!c) break;
            devname[i] = c;
        }
    }

    open_count++;
    if (open_count <= 10) {
        fprintf(stderr, "[rtos] device_open_801451F0(\"%s\", flags=0x%X) #%d\n",
                devname, (uint32_t)ctx->r5, open_count);
    }

    /* Register the device in the RTOS device table at 0x800B83E8.
     * Each entry is 36 bytes. The device index depends on the device type.
     * The game expects:
     *   dev 0-3: PCI devices (Voodoo)
     *   dev 4: DCS sound
     *   dev 5: IOASIC */
    uint32_t dev_table_phys = 0x000B83E8;
    int dev_idx = -1;

    if (strstr(devname, "pci") || strstr(devname, "voodoo") || strstr(devname, "3dfx")) {
        /* PCI/Voodoo device - use index based on open order */
        static int pci_dev_count = 0;
        dev_idx = pci_dev_count++;
        if (dev_idx > 3) dev_idx = 3;
    } else if (strstr(devname, "wdog") || strstr(devname, "watchdog")) {
        dev_idx = -1; /* watchdog doesn't need device table entry */
    }

    if (dev_idx >= 0 && dev_idx < 16) {
        uint32_t entry = dev_table_phys + dev_idx * 36;
        if (entry + 36 <= 0x00800000) {
            *(uint32_t*)(rdram + entry + 0) = dev_idx;
            *(uint32_t*)(rdram + entry + 16) = 0x00000004;

            uint32_t handler_phys = 0x6F0000 + dev_idx * 64;
            uint32_t handler_virt = 0x806F0000 + dev_idx * 64;
            memset(rdram + handler_phys, 0, 64);
            *(uint32_t*)(rdram + handler_phys + 24) = 0x800C40A4;
            *(uint32_t*)(rdram + entry + 24) = handler_virt;
            fprintf(stderr, "[rtos] Registered device \"%s\" at index %d, handler at 0x%08X\n",
                    devname, dev_idx, handler_virt);
        }
    }

    /* For file-type device opens (names with extensions like .ZM, .WMS, etc.),
     * load the file from disk and return a small positive device index.
     * The game checks bgez (>= 0 signed) on the return value.
     * We use indices 0x10-0x3F to avoid collision with PCI device indices (0-5). */
    if (strchr(devname, '.') && !strstr(devname, "pci") && !strstr(devname, "pic") &&
        !strstr(devname, "wdog")) {
        /* Check cache */
        for (int i = 0; i < g_file_dev_count; i++) {
            if (strcmp(g_file_devs[i].name, devname) == 0) {
                int idx = 0x10 + i; /* file device index */
                ctx->r2 = (gpr)(int32_t)idx;
                return;
            }
        }

        /* New file: load from disk */
        int handle = rtos_file_open(devname);
        if (getenv("CARNEVIL_FILEDBG") && (handle <= 0 || !rtos_files[handle-1].data || g_file_dev_count >= 32)) {
            fprintf(stderr, "[filefail] '%s': handle=%d data=%p dev_count=%d/32 next_phys=0x%X size=%u\n",
                    devname, handle, handle>0?(void*)rtos_files[handle-1].data:NULL,
                    g_file_dev_count, g_file_next_data_phys, handle>0?rtos_files[handle-1].size:0);
        }
        if (handle > 0 && rtos_files[handle - 1].data && g_file_dev_count < 32) {
            uint32_t fsize = rtos_files[handle - 1].size;
            uint32_t data_phys = g_file_next_data_phys;

            if (data_phys + fsize < 0x00780000 && fsize > 0) {
                memcpy(rdram + data_phys, rtos_files[handle - 1].data, fsize);
                g_file_next_data_phys = (data_phys + fsize + 0xF) & ~0xF;
            }

            strncpy(g_file_devs[g_file_dev_count].name, devname, 63);
            g_file_devs[g_file_dev_count].data_phys = data_phys;
            g_file_devs[g_file_dev_count].size = fsize;
            g_file_devs[g_file_dev_count].active = 1;

            int idx = 0x10 + g_file_dev_count;
            g_file_dev_count++;

            if (open_count <= 20) {
                fprintf(stderr, "[rtos] File device \"%s\": idx=%d size=%u data@0x%08X\n",
                        devname, idx, fsize, 0x80000000 | data_phys);
            }

            /* Set up device table entry so func_80145020 can find it.
             * Device table base is at *(0x001A1EEC). If the table exists,
             * set the flag byte for our index. */
            uint32_t dtable_ptr = *(uint32_t*)(rdram + 0x001A1EEC);
            uint32_t dtable_phys = dtable_ptr & 0x1FFFFFFF;
            if (dtable_phys > 0 && dtable_phys + idx + 1 < 0x00800000) {
                rdram[dtable_phys + idx] |= 0x04; /* set active flag */
            }

            ctx->r2 = (gpr)(int32_t)idx;
            return;
        }
    }

    /* Return the device handle.
     * For "pic:" device: the handle value gets stored at 0x801E6504 and
     * compared against PIC serial numbers (528, 469, 486). The game loops
     * forever if it doesn't match. Return 486 (39" cabinet serial). */
    if (strstr(devname, "pic")) {
        ctx->r2 = 486;
    } else {
        ctx->r2 = (dev_idx >= 0) ? (gpr)(dev_idx + 1) : 1;
    }
}

/* vec[25] = static_0_800C4154 -> rtos_8000BEB0
 * This is a general RTOS device I/O function (160 calls).
 * Redirect to the actual RTOS function. */
/* vec[25] = static_0_800C4154: RTOS device I/O dispatch (160 calls/frame)
 * On real hardware: looks up device in table, dispatches to driver handler.
 * Since our device table is empty (RTOS boot didn't run), we implement
 * the device I/O directly.
 *
 * Arguments: a0=device_id, a1=command, a2=data
 * Commands (from disassembly of rtos_8000BEB0):
 *   0x6601: device read
 *   0x6603: device write (most common - rendering commands)
 *   0x6604: device ioctl (with indirect handler call)
 */
extern voodoo_state_t g_voodoo;
RECOMP_FUNC void static_0_800C4154(uint8_t* rdram, recomp_context* ctx) {
    uint32_t dev_id = (uint32_t)ctx->r4;
    uint32_t cmd = (uint32_t)ctx->r5;
    uint32_t data = (uint32_t)ctx->r6;

    static int dev_io_count = 0;
    dev_io_count++;
    if (dev_io_count <= 20) {
        fprintf(stderr, "[dev_io] dev=%d cmd=0x%04X data=0x%08X\n", dev_id, cmd, data);
    }

    /* Handle commands by device/type */
    uint32_t data_phys = data & 0x1FFFFFFF;
    int32_t* resp = NULL;
    if (data_phys > 0 && data_phys + 4 <= 0x00800000)
        resp = (int32_t*)(rdram + data_phys);

    /* IO call counter for diagnostics */
    {
        static int io_total = 0;
        io_total++;
    }

    /* Handle all commands regardless of device ID */
    if ((cmd & 0xFF00) == 0x6900) {
        /* DCS2 Sound commands */
        switch (cmd) {
            case 0x6900: if (resp) *resp = 0x0000; break; /* Sound status */
            case 0x6901: if (resp) *resp = 0x0003; break; /* DCS version */
            case 0x6902: if (resp) *resp = 0x0001; break; /* Sound ready */
            case 0x6903: if (resp) *resp = 0x0001; break; /* Sound OK */
            case 0x6905: break;                             /* Sound command (play) */
            case 0x6907: if (resp) *resp = 0x0000; break; /* Volume */
            case 0x6909: {
                /* DCS2 sound status read.
                 * Bit 0x40: data ready (loop exit condition in polling loops)
                 * Bits 0x700: status code — must be 0x200 for success
                 *   (func_8016C630 checks (resp & 0x700) == 0x200)
                 * Bits 0x7F00: must NOT be 0x1900 (that triggers error check)
                 * Value 0x0240 = bit 0x40 set + status 0x200 = success + ready */
                if (resp) *resp = 0x0240;
                break;
            }
            case 0x690E: if (resp) *resp = 0x0000; break; /* Sound test */
            case 0x690F: break;                             /* Sound data */
            case 0x6910: if (resp) *resp = 0x0000; break; /* Sound param */
            default: if (resp) *resp = 0x0000; break;
        }
    } else if ((cmd & 0xFF00) == 0x7400 || (cmd & 0xFF00) == 0x7000 || (cmd & 0xFF00) == 0x6300) {
        /* IOASIC / PIC / system commands */
        switch (cmd) {
            case 0x7000: if (resp) *resp = 0x0001; break; /* System OK */
            case 0x7001: if (resp) *resp = 486;   break; /* IOASIC upper = PIC serial (486=39") */
            case 0x7002: if (resp) *resp = 487;   break; /* Board ID (game subtracts 1 → 486) */
            case 0x7403: if (resp) *resp = 486; break; /* PIC query - return serial */
            case 0x7405: {
                /* IOASIC write / DCS command — always return a value above the
                 * DCS2 polling timeout threshold (0x17D7840 = ~25M).
                 * This forces the loop to exit via the timer path immediately. */
                if (resp) *resp = 0x7FFFFFFF; /* max positive int32 — always exceeds threshold */
                ctx->r2 = 0;
                return; /* Return immediately, don't continue to other handling */
            }
            case 0x7406: {
                /* IOASIC data transfer / counter read.
                 *
                 * func_8016A540 polls this in a tight loop:
                 *   dev_io(5, 0x7406, &counter)
                 *   if (counter < target) goto loop
                 * The comparison is SIGNED (slt). The target is computed from
                 * the function argument * large constant (typically millions).
                 *
                 * On real hardware, this is a timing counter that increments
                 * rapidly. We simulate it by returning a large incrementing value.
                 * Must be positive (not 0xFFFF which is -1 signed). */
                static int ip = 0;
                ip += 100000; /* increment fast to exceed any target quickly */
                if (resp) *resp = ip;
                break;
            }
            case 0x740B: if (resp) *resp = 486; break; /* PIC serial number (486=39") */
            case 0x6300: if (resp) *resp = 0x0000; break; /* PIC init */
            /* For device 3 (NVRAM/CMOS) these are the CMOS-header queries the
             * layout init (sub_8014586C) issues: 0x6301 = CMOS region base, 0x6302
             * = CMOS size. Returning 0 (the old PIC-challenge default) left
             * csize=0 -> bound dword_801E65C0=0 -> every adjustment read skipped
             * -> diagnostic mode. Return base=0, size=8000 (the layout the game
             * itself writes: "cmos size 8000"). For other devices keep the old
             * PIC-challenge/verify behaviour. */
            case 0x6301: if (resp) *resp = (dev_id == 3) ? 0u : 0x0000; break;
            case 0x6302: if (resp) *resp = (dev_id == 3) ? 8000u : 0x0000; break;
            default: if (resp) *resp = 0x0000; break;
        }
    } else if ((cmd & 0xFF00) == 0x6600) {
        /* Voodoo rendering commands! */
        if (dev_io_count <= 50) {
            fprintf(stderr, "[dev_io] *** RENDER CMD 0x%04X data=0x%08X ***\n", cmd, data);
        }
        if (data_phys > 0 && data_phys + 8 <= 0x00800000) {
            uint32_t reg = *(uint32_t*)(rdram + data_phys);
            uint32_t val = *(uint32_t*)(rdram + data_phys + 4);
            if (reg < 0x400) {
                voodoo_write(&g_voodoo, reg, val);
            }
        }
    } else {
        /* Unknown command - return success/ready */
        if (resp) *resp = 0x0000;
    }

    /* Anti-spin protection: if dev_io is called too many times in a burst,
     * force the game to progress. Polling loops stall without this. */
    {
        static int last_cmd = 0;
        static int repeat_count = 0;
        if ((int)cmd == last_cmd) {
            repeat_count++;
            if (repeat_count > 50) {
                /* Force function to return via longjmp-like mechanism.
                 * Since we can't longjmp, instead make the attract mode's
                 * inner loops break by varying the IOASIC responses. */
                if (resp) {
                    if (*resp == 0) *resp = 0x55AA;
                    else *resp = (int32_t)(repeat_count & 1 ? 0xFFDF : 0xFFFF);
                }
                /* After 500 repeats, the loop won't break on its own.
                 * Force a return by setting a "done" flag. */
                if (repeat_count > 200) {
                    repeat_count = 0; /* prevent infinite loop */
                    return; /* bail out of dev_io entirely */
                }
            }
        } else {
            repeat_count = 0;
            last_cmd = cmd;
        }
    }

    ctx->r2 = 0;
}

/* ======================================================================
 * RTOS Task Creation
 * Replaces: func_801A2A3C (BREAK trap in original binary)
 * Called as: create_task(r4=task_id, r5=callback_vram)
 * ====================================================================== */

/* Task table shared with seattle_runtime.c */
typedef struct { int id; uint32_t callback_vram; } seattle_task_t;
extern seattle_task_t task_table[];
extern int task_count;

static int rtos_log_count = 0;
static int heap_checked = 0;

static void override_check_heap(const uint8_t* rdram, const char* when) {
    if (heap_checked > 5) return;
    heap_checked++;
    uint32_t head = *(uint32_t*)(rdram + 0x001A1E90);
    uint32_t total_free = 0;
    uint32_t block_count = 0;
    uint32_t cur = head;
    while (cur != 0 && block_count < 1000) {
        uint32_t phys = cur & 0x1FFFFFFF;
        if (phys >= 0x00800000 - 8) break;
        uint32_t size = *(uint32_t*)(rdram + phys);
        uint32_t next = *(uint32_t*)(rdram + phys + 4);
        total_free += (size & ~1);  /* mask off allocated flag */
        block_count++;
        cur = next;
    }
    fprintf(stderr, "[heap] %s: %d free blocks, %u KB free, head=0x%08X\n",
            when, block_count, total_free / 1024, head);
}

RECOMP_FUNC void func_801A2A3C(uint8_t* rdram, recomp_context* ctx) {
    int task_id = (int)ctx->r4;
    uint32_t callback = (uint32_t)ctx->r5;

    if (task_count < 16) {
        task_table[task_count].id = task_id;
        task_table[task_count].callback_vram = callback;
        task_count++;
    }

    /* Each frame creates new tasks. Create or reactivate fibers. */
    if (g_scheduler.main_fiber != NULL) {
        /* Try to find an existing inactive fiber to reactivate */
        int found = 0;
        for (int i = 0; i < g_scheduler.fiber_count; i++) {
            if (g_scheduler.fibers[i].callback_vram == callback && !g_scheduler.fibers[i].active) {
                g_scheduler.fibers[i].active = 1;
                g_scheduler.fibers[i].blocked = 0;
                found = 1;
                break;
            }
        }
        /* If no existing fiber, create a new one */
        if (!found) {
            rtos_sched_create_task(&g_scheduler, task_id, callback);
        }
    }

    /* Send task_id as a message to channel 6 (the task dispatch channel).
     * The fibers running func_800C47E0 wait on channel 6 and use the
     * message value as a jump table index to select the handler.
     * We call func_80145DE0 (msg_send) to do this properly. */
    {
        recomp_context msg_ctx = *ctx;
        recomp_ctx_init_fodd(&msg_ctx);
        msg_ctx.r4 = 6;              /* channel = 6 */
        msg_ctx.r5 = (gpr)task_id;   /* value = task_id */
        extern RECOMP_FUNC void func_80145DE0(uint8_t*, recomp_context*);
        func_80145DE0(rdram, &msg_ctx);
    }

    rtos_log_count++;
    fprintf(stderr, "[rtos] create_task(%d, func_%08X)\n", task_id, callback);
    if (rtos_log_count == 1) {
        override_check_heap(rdram, "at first create_task");
    }
    ctx->r2 = 0;
}

/* ======================================================================
 * NOP I/O register stubs called from main_loop
 * These are in the RTOS vector table area (0x800C426C-0x800C428C)
 * On real hardware, the RTOS patches these with I/O accessor functions.
 * For now, implement them as safe no-ops that don't crash.
 * ====================================================================== */

/* These functions are called via direct name, so they need matching symbols.
 * The recompiled versions are just NOPs. Since these are auto-generated
 * "static" functions, they use the static_N_ naming convention. */

/* ======================================================================
 * Game printf -- replaces static_0_80142DF8
 * Called as: game_printf(callback_struct*, format_string, ...)
 * The varargs are already saved to stack by the recompiled code.
 * We just read the format string and print it.
 * ====================================================================== */

static int printf_count = 0;

RECOMP_FUNC void static_0_80142DF8(uint8_t* rdram, recomp_context* ctx) {
    printf_count++;
    if (printf_count == 1) {
        override_check_heap(rdram, "at first printf");
    }
    /* r4 = callback struct pointer (0x801A1DF8)
     * r5 = format string
     * r6, r7 = first two args (a2, a3)
     * Stack: additional args at sp+16, sp+20, etc. (saved by varargs_save)
     */
    uint32_t fmt_addr = (uint32_t)ctx->r5;
    uint32_t fmt_phys = fmt_addr & 0x1FFFFFFF;

    if (fmt_phys >= 0x00800000 - 1) { ctx->r2 = 0; return; }

    const char* fmt = (const char*)(rdram + fmt_phys);
    int len = 0;
    while (len < 200 && fmt[len] != 0 && fmt[len] >= 0x0A && fmt[len] < 0x7F) len++;
    if (len == 0 || len >= 200) { ctx->r2 = 0; return; }

    /* Trace DMA buffer allocation failures */
    static int dma_err_log = 0;
    if (len > 10 && strstr(fmt, "DMA buffer") && dma_err_log < 3) {
        dma_err_log++;
        fprintf(stderr, "[DMA_ERR] fmt_addr=0x%08X fmt=\"%.60s\" a2=0x%08X\n",
                fmt_addr, fmt, (uint32_t)ctx->r6);
        /* Dump the stack to find the caller */
        uint32_t sp_phys = (uint32_t)ctx->r29 & 0x1FFFFFFF;
        fprintf(stderr, "[DMA_ERR] SP=0x%08X, stack:\n", (uint32_t)ctx->r29);
        for (int si = 0; si < 8; si++) {
            uint32_t sv = (sp_phys + si*4 < 0x00800000) ?
                *(uint32_t*)(rdram + sp_phys + si*4) : 0;
            if (sv >= 0x800C0000 && sv < 0x801C0000)
                fprintf(stderr, "  SP[%+d] = 0x%08X (code ptr!)\n", si*4, sv);
        }
    }

    /* Simple printf: handle %s, %d, %x, %08x, skip unknown formats */
    uint32_t args[8];
    args[0] = (uint32_t)ctx->r6;  /* a2 */
    args[1] = (uint32_t)ctx->r7;  /* a3 */
    /* Read remaining args from stack (varargs area) */
    for (int i = 2; i < 8; i++) {
        uint32_t sp = (uint32_t)ctx->r29;
        uint32_t phys = (sp + 16 + (i-2)*4) & 0x1FFFFFFF;
        if (phys < 0x00800000 - 4)
            args[i] = *(uint32_t*)(rdram + phys);
        else
            args[i] = 0;
    }

    fprintf(stderr, "[game] ");
    int arg_idx = 0;
    for (int i = 0; i < len; i++) {
        if (fmt[i] == '%' && i+1 < len) {
            i++;
            /* Skip width/precision specifiers */
            while (i < len && (fmt[i] == '0' || fmt[i] == '-' || (fmt[i] >= '1' && fmt[i] <= '9'))) i++;
            if (i >= len) break;
            if (fmt[i] == 's' && arg_idx < 8) {
                uint32_t str_phys = args[arg_idx++] & 0x1FFFFFFF;
                if (str_phys < 0x00800000 - 1) {
                    const char* s = (const char*)(rdram + str_phys);
                    fprintf(stderr, "%s", s);
                } else {
                    fprintf(stderr, "(null)");
                }
            } else if (fmt[i] == 'd' && arg_idx < 8) {
                fprintf(stderr, "%d", (int32_t)args[arg_idx++]);
            } else if ((fmt[i] == 'x' || fmt[i] == 'X') && arg_idx < 8) {
                fprintf(stderr, "%x", args[arg_idx++]);
            } else if (fmt[i] == '%') {
                fputc('%', stderr);
            } else {
                fprintf(stderr, "%%%c", fmt[i]);
                if (fmt[i] != 'l' && fmt[i] != 'h') arg_idx++;
            }
        } else {
            fputc(fmt[i], stderr);
        }
    }
    if (len > 0 && fmt[len-1] != '\n') fprintf(stderr, "\n");

    ctx->r2 = 0;
}

/* ======================================================================
 * RTOS Process Dispatcher (func_80151718)
 * This is the heart of the RTOS - it runs all registered tasks using
 * the fiber scheduler until all tasks have completed their frame work.
 * Called from main_loop. On real hardware, this never returns.
 * In our runtime, it runs one scheduling round and returns.
 * ====================================================================== */

static int dispatch_count = 0;

RECOMP_FUNC void func_80151718(uint8_t* rdram, recomp_context* ctx) {
    dispatch_count++;
    if (dispatch_count <= 3 || dispatch_count % 100 == 0) {
        int active = 0, blocked = 0;
        for (int i = 0; i < g_scheduler.fiber_count; i++) {
            if (g_scheduler.fibers[i].active) active++;
            if (g_scheduler.fibers[i].blocked) blocked++;
        }
        fprintf(stderr, "[rtos] process_dispatch #%d: %d fibers, %d active, %d blocked\n",
                dispatch_count, g_scheduler.fiber_count, active, blocked);
    }
    /* Experimented with pulsing ch6 every frame here (tried sending 14
     * task-id values per frame to keep task fibers awake) -- net slightly
     * worse rendering. Removed; the real game's ch6 event source is
     * something more specific than "post all tids each frame". */
    extern const char* g_floop_step;
    g_floop_step = "dispatch.sched";
    rtos_sched_run_frame(&g_scheduler, ctx);

    /* Drain the per-frame callback list. The original RTOS scheduler loop
     * (IDA decomp of sub_80151718) calls sub_80148714 every iteration; our
     * override previously skipped it. sub_80148714 walks the doubly-linked
     * callback list at dword_801A25D8 and invokes each entry's +0x1C handler
     * (skipping entries whose +0x16 flag has bit 0 set). func_800CA148 (run
     * during attract init via func_800C50AC) registers func_800CC728 into this
     * list via static_0_80148138 -> func_80148140, and func_800CC728 in turn
     * walks the entity list at 0x801E3880 and ticks each entity's +0x14
     * handler -- the bridge to per-frame entity dispatch and rendering.
     *
     * STATUS (2026-06): once func_800C50AC is allowed to return (see the
     * yield-escape wrap in seattle_runtime.c), registration sticks and this
     * walker reads a populated head (0x806FAE7C). func_800CC728 then fires and
     * real geometry flows -- ~20 triangles submitted to the Voodoo per frame
     * (the first time entity render has ever run). It currently faults
     * (access violation) deeper in the entity tick after submitting those
     * triangles, and the triangle vertices decode as (0,0) -- the next two
     * bugs to chase. The frame loop's SEH guard keeps the run alive. */
    g_floop_step = "dispatch.walker";
    if (getenv("CARNEVIL_ENTDBG")) {
        static int n=0; static int dumped=0; n++;
        /* Detailed one-shot dump of the entity table after the scene funcs run
         * (CARNEVIL_FORCESCENE fires at frame 40): show every non-empty entry's
         * id + handler(+0x10) + tick(+0x14) + process(+0x20) so we can see whether
         * the render handlers were stored and survive. */
        if (!dumped && n >= 45) {
            dumped = 1;
            int ents = 0;
            for (int i = 0; i < 128; i++) {
                uint32_t e = 0x001E3880 + i*0x28;
                uint16_t id = *(uint16_t*)(rdram + e);
                uint32_t h10 = *(uint32_t*)(rdram + e + 0x10);
                uint32_t h14 = *(uint32_t*)(rdram + e + 0x14);
                uint32_t h20 = *(uint32_t*)(rdram + e + 0x20);
                if (id != 0xFFFF && (h10 || h14 || h20)) {
                    ents++;
                    if (ents <= 16)
                        fprintf(stderr, "[entdump] idx%d id=0x%04X +10=0x%08X +14=0x%08X +20=0x%08X\n",
                                i, id, h10, h14, h20);
                }
            }
            fprintf(stderr, "[entdump] total populated=%d, cb_head=0x%08X\n",
                    ents, *(uint32_t*)(rdram + 0x001A25D8));
        }
    }
    {
        extern RECOMP_FUNC void func_80148714(uint8_t* rdram, recomp_context* ctx);
        func_80148714(rdram, ctx);
    }

    ctx->r2 = 0;
}

/* Watchdog task stub */
RECOMP_FUNC void func_801515C8(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = 0;
}

/* ======================================================================
 * RTOS Primitives
 *
 * func_80145CE4: Event/semaphore wait
 *   r4 = event channel, r5 = pointer to store received data
 *   Returns: r2 = received value (we return a frame counter to simulate ticks)
 *
 * func_80145F98: Event/semaphore signal
 *   r4 = event channel
 *   Returns immediately
 * ====================================================================== */

static uint32_t rtos_event_counter = 0;

/* Yield-escape globals (defined later in this file) -- forward-declared here
 * so event_wait (func_80145CE4) can participate in the same escape budget. */
extern int g_yield_counter;
extern jmp_buf g_yield_escape_buf;
extern int g_yield_escape_armed;
extern int g_yield_escape_threshold;

/* Forward declare message queue (defined below) */
#define RTOS_MAX_CHANNELS 256
#define RTOS_QUEUE_SIZE   32
static struct rtos_queue_s {
    uint32_t buffer[RTOS_QUEUE_SIZE];
    int head;
    int tail;
    int count;
} rtos_queues[RTOS_MAX_CHANNELS];

/* Hook to check heap before main_loop's malloc */
static int event_heap_checked = 0;

RECOMP_FUNC void func_80145CE4(uint8_t* rdram, recomp_context* ctx) {
    /* sub_80145CE4 is NOT event_wait -- it's the NVRAM AUDIT READ (paired with
     * sub_80145DE0 audit-write): reads audit a1 from io.cmos[dword_801DDD90 +
     * 32*a1] via a device-3 seek+read and validates a checksum, returning 0 on
     * success / -6 bad-checksum / -7 out-of-range. The subsystem-readiness check
     * sub_800F16D0 passes only when these reads return 0. Our legacy "event_wait"
     * interpretation returns channel-queue values (e.g. channel-6's task-ID
     * flood) -> non-zero -> the check fails -> the game stays in diagnostic mode.
     * Read the real audit from the loaded NVRAM and report success. Gated by
     * CARNEVIL_AUDIT while we confirm it doesn't disturb the attract fiber's
     * per-frame advance (which should fall to the func_80151618 yield instead). */
    {
        extern uint8_t* seattle_nvram_ptr(void);
        extern uint32_t seattle_nvram_size(void);
        if (getenv("CARNEVIL_AUDIT")) {
            int ch = (int)ctx->r4;
            uint32_t bufp = (uint32_t)ctx->r5 & 0x1FFFFFFF;
            /* Channel 6 inside a task fiber is the render-task dispatch slot:
             * sub_800C47E0 reads it to decide which slot (0-9) to render. On
             * hardware the dispatcher posts each task's id before waking it; we
             * model that by returning THIS fiber's id, so every render task gets
             * its own slot instead of all reading the last create_task post. */
            if (ch == 6 && g_scheduler.current_fiber >= 0 &&
                g_scheduler.current_fiber < g_scheduler.fiber_count) {
                uint32_t slot = (uint32_t)g_scheduler.fibers[g_scheduler.current_fiber].id;
                if (bufp + 4 <= 0x00800000) *(uint32_t*)(rdram + bufp) = slot;
                ctx->r2 = 0;
                return;
            }
            uint32_t naud = *(uint32_t*)(rdram + 0x23662C);   /* dword_8023662C audits=182 */
            if (ch < 0 || (uint32_t)ch >= naud) { ctx->r2 = (gpr)(int32_t)-7; return; }
            uint32_t abase = *(uint32_t*)(rdram + 0x1DDD90);  /* dword_801DDD90 audit base */
            uint8_t* nv = seattle_nvram_ptr(); uint32_t nvsz = seattle_nvram_size();
            uint32_t off = (abase + 32u * (uint32_t)ch) & (nvsz - 1);
            uint32_t val = (off + 4 <= nvsz)
                ? (nv[off] | (nv[off+1]<<8) | (nv[off+2]<<16) | ((uint32_t)nv[off+3]<<24)) : 0;
            if (bufp + 4 <= 0x00800000) *(uint32_t*)(rdram + bufp) = val;
            ctx->r2 = 0;   /* audit valid */
            if (getenv("CARNEVIL_AUDITDBG") && ch == 6) {
                static int n=0;   /* sub_800C47E0's dispatch read (slot index) */
                if (n++ < 40) fprintf(stderr, "[dispatch6] slot=%u (renders if <10)\n", val);
            }
            return;
        }
    }
    if (event_heap_checked < 3) {
        event_heap_checked++;
        override_check_heap(rdram, event_heap_checked == 1 ? "first event_wait (task callback)" : "event_wait");
    }
    uint32_t channel = (uint32_t)ctx->r4;
    uint32_t buf_addr = (uint32_t)ctx->r5;
    uint32_t buf_phys = buf_addr & 0x1FFFFFFF;

    rtos_event_counter++;

    if (rtos_event_counter <= 20 || (channel < 100 && rtos_event_counter <= 200)) {
        fprintf(stderr, "[rtos] event_wait(channel=%d, buf=0x%08X) -> counter=%u\n",
                channel, buf_addr, rtos_event_counter);
    }

    /* Yield-escape for main-thread event_wait spins. When a per-frame mode
     * function (dispatched cold from the C frame loop via func_8014A488, so
     * current_fiber < 0) busy-waits on event_wait for a subsystem-ready
     * barrier that our stubbed RTOS never signals, it spins forever: it never
     * calls func_80151618 (yield), so the yield-escape that normally bounds
     * mode functions can't fire. Count these main-thread waits against the
     * same yield budget and longjmp out once we cross the threshold, turning
     * the infinite barrier wait into "one frame's worth of work" -- exactly
     * the treatment func_80151618 gives yield-loops. Only applies when armed
     * (inside a mode dispatch) and off-fiber; fiber-context waits below still
     * yield to the scheduler normally. */
    if (g_yield_escape_armed && g_scheduler.current_fiber < 0 && channel < 100) {
        g_yield_counter++;
        if (g_yield_counter >= g_yield_escape_threshold) {
            g_yield_escape_armed = 0;  /* disarm to avoid re-entry */
            longjmp(g_yield_escape_buf, 2);
        }
    }

    /* For high-numbered channels (rendering sync), return immediately.
     * These are waited on from non-fiber code (our test render call). */
    if (channel > 100) {
        /* Store return value at buf_addr */
        if (buf_phys < 0x00800000 - 4)
            *(uint32_t*)(rdram + buf_phys) = 181; /* echo the channel as the value */
        ctx->r2 = 0;
        return;
    }

    /* Attract-fiber blocking semantics. Inside the attract fiber, a wait on an
     * EMPTY channel means "block until something arrives" -- on real hardware
     * the task suspends and the RTOS resumes it on a later frame. We model that
     * by suspending the fiber for one frame (the frame loop resumes it next
     * frame, having advanced the vblank/tick clock that drives the attract
     * timers). A wait on a NON-empty channel (e.g. the send-then-wait round
     * trips in func_800F1550) returns the queued value immediately, no block.
     * This lets the attract state machine's timer waits advance the clock
     * instead of spinning forever. */
    {
        extern int attract_fiber_in_context(void);
        extern int attract_fiber_yield(void);
        if (attract_fiber_in_context() && channel < RTOS_MAX_CHANNELS) {
            struct rtos_queue_s *q = &rtos_queues[channel];
            if (q->count == 0) {
                attract_fiber_yield();          /* block one frame */
                if (buf_phys < 0x00800000 - 4)
                    *(uint32_t*)(rdram + buf_phys) = 0;
                ctx->r2 = 0;
                return;
            }
            /* non-empty: fall through to dequeue below */
        }
    }

    /* Yield the current fiber to let other tasks run */
    if (g_scheduler.current_fiber >= 0) {
        rtos_sched_yield(&g_scheduler, (int)channel);
    }

    /* Check message queue for this channel.
     * The game expects values 0-9 (command IDs). */
    uint32_t value = 1; /* Default: command 1 (safe) */

    if (channel < RTOS_MAX_CHANNELS) {
        struct rtos_queue_s *q = &rtos_queues[channel];
        if (q->count > 0) {
            value = q->buffer[q->head];
            q->head = (q->head + 1) % RTOS_QUEUE_SIZE;
            q->count--;
        }
    }

    /* Clamp to valid range (0-9) */
    if (value >= 10) value = value % 10;

    if (buf_phys < 0x00800000 - 4) {
        *(uint32_t*)(rdram + buf_phys) = value;
    }

    static int evt_val_log = 0;
    if (channel == 6) {
        evt_val_log++;
        if (evt_val_log <= 30) {
            fprintf(stderr, "[rtos] event_wait(ch=6) -> value=%u (queue had %d items)\n",
                    value, (channel < RTOS_MAX_CHANNELS) ? rtos_queues[channel].count + 1 : -1);
        }
    }

    ctx->r2 = (gpr)(int32_t)value;
}

RECOMP_FUNC void func_80145F98(uint8_t* rdram, recomp_context* ctx) {
    /* Signal event -- wake any fiber waiting on this channel */
    uint32_t channel = (uint32_t)ctx->r4;
    if (getenv("CARNEVIL_AUDITDBG") && channel == 6) {
        static int n=0;  /* sub_800C47E0 (render task) reached here after its slot read */
        if (n++ < 20) fprintf(stderr, "[rendertask] sub_800C47E0 ran (signal ch6)\n");
    }
    rtos_sched_wake(&g_scheduler, (int)channel);
    ctx->r2 = 0;
}

/* func_800CC1E4(id, handler, a3, a4): registers an entity render handler at
 * entry+0x10 in the table 0x801E3880 (via the inserter). The registered +0x10
 * handler (e.g. the demo render funcs func_800CB5AC/CB42C, which submit triangles
 * via func_80167848) is normally never invoked: func_800CC47C only jalrs +0x20
 * (which the inserter clears to 0), and the entity is cleared shortly after. So
 * invoke the +0x10 handler HERE, right after registration while the entity still
 * exists -- the long-missing entity render dispatch. Gated CARNEVIL_INVOKE10; a
 * re-entrancy guard stops the handler's own registrations from recursing. */
extern RECOMP_FUNC void func_800CC1E4_original(uint8_t*, recomp_context*);
static int g_in_invoke10 = 0;
RECOMP_FUNC void func_800CC1E4(uint8_t* rdram, recomp_context* ctx) {
    uint32_t handler = (uint32_t)ctx->r5;   /* a2 = handler, saved before the call */
    func_800CC1E4_original(rdram, ctx);
    if (getenv("CARNEVIL_INVOKE10") && !g_in_invoke10 && handler >= 0x80000000) {
        for (int i = 0; i < 128; i++) {
            uint32_t e = 0x001E3880 + i*0x28;
            if (*(uint16_t*)(rdram + e) != 0xFFFF &&
                *(uint32_t*)(rdram + e + 0x10) == handler) {
                extern recomp_func_t* get_function(int32_t);
                recomp_func_t* h = get_function((int32_t)handler);
                if (h) {
                    static int lg = 0;
                    if (lg++ < 12) fprintf(stderr, "[invoke10] render handler 0x%08X (entity idx%d)\n", handler, i);
                    g_in_invoke10 = 1;
                    recomp_context c2 = *ctx;
                    recomp_ctx_init_fodd(&c2);
                    c2.r4 = (gpr)(int32_t)(0x80000000u | e);   /* a0 = entity slot */
                    h(rdram, &c2);
                    g_in_invoke10 = 0;
                }
                break;
            }
        }
    }
}

static int msg_send_count = 0;
static int msg_recv_count = 0;

/* func_80145DE0: message send (post to queue) */
RECOMP_FUNC void func_80145DE0(uint8_t* rdram, recomp_context* ctx) {
    int channel = (int)ctx->r4;
    msg_send_count++;
    if (msg_send_count <= 30 || (channel < 100 && msg_send_count <= 200)) {
        fprintf(stderr, "[rtos] msg_send(ch=%d, val=0x%X) [#%d]\n",
                channel, (uint32_t)ctx->r5, msg_send_count);
    }
    uint32_t value = (uint32_t)ctx->r5;

    /* func_80145DE0 is the NVRAM AUDIT/message WRITE (paired with func_80145CE4
     * read). sub_800C47E0 (the render task) reads its task-id from "channel" 6
     * and writes render params to 133-162; create_task posts the task-id to 6.
     * For the audit-read (func_80145CE4) to observe these writes, back them with
     * the same io.cmos store the read uses (idx -> io.cmos[dword_801DDD90+32*idx]).
     * The render shortcut (0x85-0xA2 -> voodoo) below still runs. Gated by
     * CARNEVIL_AUDIT alongside the read side. */
    if (getenv("CARNEVIL_AUDIT")) {
        extern uint8_t* seattle_nvram_ptr(void); extern uint32_t seattle_nvram_size(void);
        uint32_t naud = *(uint32_t*)(rdram + 0x23662C);          /* audits=182 */
        if (channel >= 0 && (uint32_t)channel < naud) {
            uint32_t abase = *(uint32_t*)(rdram + 0x1DDD90);     /* audit base */
            uint8_t* nv = seattle_nvram_ptr(); uint32_t nvsz = seattle_nvram_size();
            uint32_t off = (abase + 32u * (uint32_t)channel) & (nvsz - 1);
            if (off + 4 <= nvsz) {
                nv[off] = (uint8_t)value; nv[off+1] = (uint8_t)(value>>8);
                nv[off+2] = (uint8_t)(value>>16); nv[off+3] = (uint8_t)(value>>24);
            }
        }
        if (getenv("CARNEVIL_AUDITDBG") && channel >= 133 && channel <= 162) {
            static int n=0;  /* sub_800C47E0's render-param writes */
            if (n++ < 40) fprintf(stderr, "[renderparam] idx=%d val=0x%08X\n", channel, value);
        }
    }

    if (channel >= 0 && channel < RTOS_MAX_CHANNELS) {
        struct rtos_queue_s *q = &rtos_queues[channel];
        if (q->count < RTOS_QUEUE_SIZE) {
            q->buffer[q->tail] = value;
            q->tail = (q->tail + 1) % RTOS_QUEUE_SIZE;
            q->count++;
        }
        rtos_sched_wake(&g_scheduler, channel);

        /* RTOS DMA consumer: channels 0x85-0xA2 carry rendering data.
         * On real hardware, the RTOS DMA tasks receive these values and
         * write them to Voodoo registers. We implement this directly.
         *
         * Channel-to-Voodoo register mapping (derived from RTOS analysis):
         * The values are direct Voodoo register values, not pointers. */
        if (channel >= 0x85 && channel <= 0xA2) {
            extern voodoo_state_t g_voodoo;
            extern void voodoo_write(voodoo_state_t*, uint32_t, uint32_t);

            /* Map channels to Voodoo register byte offsets.
             * These are the standard Voodoo 1 rendering state registers. */
            /* Map channels to actual Voodoo register offsets (from MAME voodoo_regs.h).
             * The RTOS DMA service writes these values directly to the Voodoo. */
            static const uint32_t channel_to_reg[] = {
                /* 0x85 */ VOODOO_FBZCOLORPATH,  /* 0x104 */
                /* 0x86 */ VOODOO_FOGMODE,       /* 0x108 */
                /* 0x87 */ VOODOO_ALPHAMODE,     /* 0x10C */
                /* 0x88 */ VOODOO_FBZMODE,       /* 0x110 */
                /* 0x89 */ VOODOO_LFBMODE,       /* 0x114 */
                /* 0x8A */ VOODOO_CLIPLR,        /* 0x118 */
                /* 0x8B */ VOODOO_CLIPLOHI,      /* 0x11C */
                /* 0x8C */ VOODOO_NOPCOMMAND,    /* 0x120 */
                /* 0x8D */ VOODOO_FOGCOLOR,      /* 0x12C */
                /* 0x8E */ VOODOO_ZACOLOR,       /* 0x130 */
                /* 0x8F */ VOODOO_CHROMAKEY,      /* 0x134 */
                /* 0x90 */ VOODOO_CHROMARANGE,    /* 0x138 */
                /* 0x91 */ VOODOO_STIPPLE,        /* 0x140 */
                /* 0x92 */ VOODOO_COLOR0,         /* 0x144 */
                /* 0x93 */ VOODOO_COLOR1,         /* 0x148 */
                /* 0x94 */ VOODOO_FBIPIXELSIN,    /* 0x14C */
                /* 0x95 */ VOODOO_TEXMODE,        /* 0x300 */
                /* 0x96 */ VOODOO_TLOD,           /* 0x304 */
                /* 0x97 */ VOODOO_TDETAIL,        /* 0x308 */
                /* 0x98 */ VOODOO_TEXBASE,        /* 0x30C */
                /* 0x99 */ 0x310,                 /* texBaseAddr_1 */
                /* 0x9A */ 0x314,                 /* texBaseAddr_2 */
                /* 0x9B */ 0x318,                 /* texBaseAddr_3_8 */
                /* 0x9C */ VOODOO_TREXINIT0,      /* 0x31C */
                /* 0x9D */ VOODOO_TREXINIT1,      /* 0x320 */
                /* 0x9E */ VOODOO_NOPCOMMAND,     /* nop */
                /* 0x9F */ VOODOO_NOPCOMMAND,     /* nop */
                /* 0xA0 */ VOODOO_NOPCOMMAND,     /* dynamic ch 0 */
                /* 0xA1 */ VOODOO_NOPCOMMAND,     /* dynamic ch 1 */
                /* 0xA2 */ VOODOO_SWAPBUFCMD,     /* 0x128 */
            };

            int idx = channel - 0x85;
            if (idx >= 0 && idx < (int)(sizeof(channel_to_reg)/sizeof(channel_to_reg[0]))) {
                uint32_t reg = channel_to_reg[idx];
                voodoo_write(&g_voodoo, reg, value);
            }

            /* Log rendering messages for debug */
            static int render_msg_count = 0;
            static int render_nonzero_count = 0;
            render_msg_count++;
            if (value != 0) {
                render_nonzero_count++;
                if (render_nonzero_count <= 30) {
                    int idx2 = channel - 0x85;
                    uint32_t reg2 = (idx2 >= 0 && idx2 < (int)(sizeof(channel_to_reg)/sizeof(channel_to_reg[0])))
                        ? channel_to_reg[idx2] : 0;
                    fprintf(stderr, "[render] ch=0x%02X -> reg 0x%03X val=0x%08X\n",
                            channel, reg2, value);
                }
            }
            if (render_msg_count == 1000) {
                fprintf(stderr, "[render] After 1000 msgs: %d non-zero\n", render_nonzero_count);
            }
        }

        ctx->r2 = 0;
    } else {
        ctx->r2 = (gpr)(int32_t)-1;
    }
}

/* func_80145E84: message receive (get from queue, non-blocking) */
RECOMP_FUNC void func_80145E84(uint8_t* rdram, recomp_context* ctx) {
    int channel = (int)ctx->r4;
    uint32_t buf_addr = (uint32_t)ctx->r5;
    uint32_t buf_phys = buf_addr & 0x1FFFFFFF;

    if (channel >= 0 && channel < RTOS_MAX_CHANNELS) {
        struct rtos_queue_s *q = &rtos_queues[channel];
        if (q->count > 0) {
            uint32_t value = q->buffer[q->head];
            q->head = (q->head + 1) % RTOS_QUEUE_SIZE;
            q->count--;
            if (buf_phys < 0x00800000 - 4) {
                *(uint32_t*)(rdram + buf_phys) = value;
            }
            ctx->r2 = (gpr)(int32_t)value;
        } else {
            ctx->r2 = 0; /* No message (not an error) */
        }
    } else {
        ctx->r2 = (gpr)(int32_t)-1;
    }
}

/* func_80145020: RTOS device read / yield.
 * Original: reads data from device handle into buffer via device table.
 * For file devices, we intercept and copy file data directly.
 * a0=device_handle, a1=output_buf_ptr, a2=sector_count */
RECOMP_FUNC void func_80145020(uint8_t* rdram, recomp_context* ctx) {
    uint32_t handle = (uint32_t)ctx->r4;
    uint32_t handle_phys = handle & 0x1FFFFFFF;

    static int f45020_log = 0;
    f45020_log++;
    if (f45020_log <= 30) {
        fprintf(stderr, "[func_80145020] #%d a0=0x%08X a1=0x%08X a2=0x%08X\n",
                f45020_log, handle, (uint32_t)ctx->r5, (uint32_t)ctx->r6);
    }

    /* Check if this is a file device handle (index 0x10-0x3F) */
    if (handle >= 0x10 && handle < 0x40) {
        int fdev_idx = handle - 0x10;
        if (fdev_idx >= 0 && fdev_idx < g_file_dev_count && g_file_devs[fdev_idx].active) {
            uint32_t data_phys = g_file_devs[fdev_idx].data_phys;
            uint32_t file_size = g_file_devs[fdev_idx].size;
            uint32_t buf_info = (uint32_t)ctx->r5;
            uint32_t sector_count = (uint32_t)ctx->r6;

            /* buf_info points to a stack struct. The destination address
             * may be at *(buf_info) or buf_info itself may be the dest. */
            uint32_t buf_info_phys = buf_info & 0x1FFFFFFF;
            uint32_t dest_virt = 0;
            if (buf_info_phys > 0 && buf_info_phys + 4 <= 0x00800000) {
                dest_virt = *(uint32_t*)(rdram + buf_info_phys);
            }
            uint32_t dest_phys = dest_virt & 0x1FFFFFFF;

            uint32_t read_size = sector_count * 512;
            if (read_size > file_size) read_size = file_size;
            if (read_size == 0) read_size = file_size; /* sector_count=0 means read all */

            static int fdev_read_log = 0;
            fdev_read_log++;
            if (fdev_read_log <= 20) {
                fprintf(stderr, "[fdev_read] #%d \"%s\" dest=0x%08X sectors=%u size=%u/%u\n",
                        fdev_read_log, g_file_devs[fdev_idx].name, dest_virt,
                        sector_count, read_size, file_size);
            }

            /* Copy file data to destination */
            if (dest_phys > 0 && dest_phys + read_size <= 0x00800000 &&
                data_phys + read_size <= 0x00800000 && read_size > 0) {
                memcpy(rdram + dest_phys, rdram + data_phys, read_size);
                if (fdev_read_log <= 5) {
                    fprintf(stderr, "[fdev_read] Copied %u bytes to 0x%08X\n", read_size, dest_virt);
                }
            }

            ctx->r2 = 0;
            return;
        }
    }

    /* Not a file device - original yield behavior for fiber operations */
    if (g_scheduler.current_fiber >= 0) {
        rtos_sched_yield(&g_scheduler, -1);
    }
    ctx->r2 = 0;
}

/* func_80144EB8: exit/terminate task -- never returns on real RTOS.
 * Mark the fiber as blocked (done for this frame) and yield. */
RECOMP_FUNC void func_80144EB8(uint8_t* rdram, recomp_context* ctx) {
    (void)rdram;
    int idx = g_scheduler.current_fiber;
    if (idx >= 0 && idx < g_scheduler.fiber_count) {
        /* exit_task: the task body is done for this frame. The recompiled task
         * (e.g. func_800C47E0) has a while(1); after this call, so we must NOT
         * return into it. Unwind to fiber_entry's per-frame loop via longjmp;
         * fiber_entry then blocks the fiber and re-runs the body next frame. */
        extern jmp_buf g_task_exit_jmp[];
        g_scheduler.fibers[idx].blocked = 1;
        longjmp(g_task_exit_jmp[idx], 1);   /* does not return */
    }
    ctx->r2 = 0;
}

/* func_800F16D0(count, channel_list): subsystem-readiness check. For each
 * channel it event_waits (sub_80145CE4); if a channel still has a pending event
 * after an ack (msg_send 0), it returns 0 = "subsystem NOT ready". The attract
 * state machine (func_800C50AC) ANDs these via sub_800F181C; if any fails it sets
 * dword_801DFE70=2 and shows the DIAGNOSTIC/error screen (sub_800E4B1C) instead of
 * the normal attract (sub_800E4C84). In our RTOS event model these channels carry
 * stale/garbage queued values, so the check fails and the game is stuck on the
 * diagnostic screen (the flat colored blocks). Inside the attract fiber, report
 * "all ready" (1) so the game boots into the normal attract path. Non-fiber
 * callers fall through to the original. EXPERIMENT to validate the diagnosis. */
extern RECOMP_FUNC void func_800F16D0_original(uint8_t*, recomp_context*);
RECOMP_FUNC void func_800F16D0(uint8_t* rdram, recomp_context* ctx) {
    extern int attract_fiber_in_context(void);
    if (getenv("CARNEVIL_FORCE_NORMAL") && attract_fiber_in_context()) { ctx->r2 = 1; return; }
    uint32_t cnt = (uint32_t)ctx->r4;
    uint32_t listp = (uint32_t)ctx->r5 & 0x1FFFFFFF;
    func_800F16D0_original(rdram, ctx);
    if (getenv("CARNEVIL_EVTDBG")) {
        static int n=0;
        if (n++ < 12) {
            /* dump the channel ids this check covers + pass/fail */
            char chs[160]=""; int p=0;
            for (uint32_t i=0;i<cnt && i<16 && listp+i*4+4<=0x800000;i++)
                p+=snprintf(chs+p,sizeof(chs)-p,"%u,", *(uint32_t*)(rdram+listp+i*4));
            fprintf(stderr, "[evtchk] count=%u ret=%d channels=[%s]\n", cnt, (int)(int32_t)ctx->r2, chs);
        }
    }
}

/* func_80146168 (read adjustment value from NVRAM device 3).
 * The original reads via the device-3 path (seek 0x6300 + sub_80144EF8) and
 * validates a CRC of a RAM shadow at dword_801DFED8 -- but in our build the
 * shadow base is a low NVRAM offset (0x16C0, since the CMOS region base is 0),
 * not a valid RAM address, so the device read never positions correctly and the
 * checksum can't match: every read fails -> all adjustments "invalid" -> the
 * game boots into DIAGNOSTIC mode. The real per-channel values DO live in the
 * loaded MAME NVRAM at io.cmos[base + 16*ch] (verified: sensible in-range
 * values). Read them directly and report success (checksum OK). This makes the
 * operator adjustments validate -> dword_801DFE70 stays 0 -> normal attract. */
extern uint8_t* seattle_nvram_ptr(void);
extern uint32_t seattle_nvram_size(void);
RECOMP_FUNC void func_80146168(uint8_t* rdram, recomp_context* ctx) {
    int ch = (int)ctx->r4;
    uint32_t buf_phys = (uint32_t)ctx->r5 & 0x1FFFFFFF;
    uint32_t count = *(uint32_t*)(rdram + 0x236630);   /* dword_80236630 */
    if (ch < 0 || (uint32_t)ch >= count) { ctx->r2 = (gpr)(int32_t)-9; return; }
    uint32_t base = *(uint32_t*)(rdram + 0x1DFED8);     /* dword_801DFED8 (0x16C0) */
    uint8_t* nv = seattle_nvram_ptr();
    uint32_t nvsz = seattle_nvram_size();
    uint32_t off = (base + 16u * (uint32_t)ch) & (nvsz - 1);
    uint32_t val = 0;
    if (off + 4 <= nvsz)
        val = nv[off] | (nv[off+1] << 8) | (nv[off+2] << 16) | ((uint32_t)nv[off+3] << 24);
    if (buf_phys + 4 <= 0x00800000)
        *(uint32_t*)(rdram + buf_phys) = val;
    ctx->r2 = 0;   /* success: value valid (skip the broken shadow checksum) */
    if (getenv("CARNEVIL_ADJDBG")) {
        static int n=0;
        if (n++ < 40)
            fprintf(stderr, "[adjread] ch=%d val=%u (nvoff=0x%X) count=%u base=0x%X\n",
                    ch, val, off, count, base);
    }
}

/* func_800F1BA8 (check_adjustments): validates operator adjustments against
 * their min/max (sub_80150098). Returns non-zero if any is out of range. Our
 * emulated CMOS/NVRAM has invalid adjustment values, so it fails -> at
 * func_800C50AC LABEL_19, dword_801DFE70=2 -> diagnostic/error screen. Force 0
 * (adjustments OK) in the attract fiber so the game boots normal attract.
 * EXPERIMENT (with func_800F16D0) to validate the diagnostic-mode diagnosis. */
extern RECOMP_FUNC void func_800F1BA8_original(uint8_t*, recomp_context*);
RECOMP_FUNC void func_800F1BA8(uint8_t* rdram, recomp_context* ctx) {
    extern int attract_fiber_in_context(void);
    if (getenv("CARNEVIL_FORCE_NORMAL") && attract_fiber_in_context()) { ctx->r2 = 0; return; }
    func_800F1BA8_original(rdram, ctx);
}

/* func_80143788: a cooperative yield (-> static_0_800C421C -> the RTOS
 * scheduler rtos_80009A80). The game's ASYNC file layer calls it from file_copy
 * (func_800E5E78, "could_not_open_readfile_in_file_copy") to let a file-server
 * TASK service the read while the caller suspends. In our model file I/O is
 * serviced synchronously by overrides and that task never runs, so the yield
 * idles the game scheduler forever (rtos_80008094 scans 170 tasks, none
 * runnable) -- the attract DEADLOCKS here while loading the 3D-demo assets
 * (BABY.ZM2, the Big Baby model). Diagnosed via the hang watchdog
 * (CARNEVIL_WATCHDOG) -> spin in rtos_8000884C <- file_copy <- hstddf_wms.
 * Fix: mirror the func_80151618 hook -- inside the attract fiber, suspend back
 * to the frame loop and return; next frame file_copy resumes and reads the
 * already-loaded data. This unblocked the attract (swap 759 -> 6975) and made
 * the game bind textures for render (texBaseAddr != 0) for the first time.
 * Non-fiber callers fall through to the original recompiled body. */
extern RECOMP_FUNC void func_80143788_original(uint8_t*, recomp_context*);
RECOMP_FUNC void func_80143788(uint8_t* rdram, recomp_context* ctx) {
    extern int attract_fiber_in_context(void);
    extern int attract_fiber_yield(void);
    if (attract_fiber_in_context()) { attract_fiber_yield(); ctx->r2 = 0; return; }
    func_80143788_original(rdram, ctx);
}

/* func_80151618: sync/yield function called in the PIC check infinite loop.
 * The loop at 0x800C515C-0x800C5164 calls this repeatedly until
 * the PIC serial at 0x801E6504 matches 528, 469, or 486.
 * We force-set the PIC serial to 486 each call to break the loop. */
int g_yield_counter = 0;
int g_zone_processing_active = 0;
uint32_t g_ram_poll_addr = 0;
int g_ram_poll_count = 0;
extern jmp_buf g_yield_escape_buf;
extern int g_yield_escape_armed;
extern int g_yield_escape_threshold;

RECOMP_FUNC void func_80151618(uint8_t* rdram, recomp_context* ctx) {
    static int c = 0; c++;
    *(uint32_t*)(rdram + 0x001E6504) = 486; /* force PIC serial */

    /* Attract-fiber path: func_80151618 is the attract state machine's per-frame
     * yield. When running inside the attract fiber, suspend it back to the frame
     * loop here; the C loop advances the frame counters and resumes us next
     * frame, so the state machine progresses one iteration per frame with
     * consistent state (instead of being longjmp-escaped mid-traversal). */
    {
        extern int attract_fiber_in_context(void);
        extern int attract_fiber_yield(void);
        if (attract_fiber_in_context()) {
            attract_fiber_yield();   /* returns next frame */
            ctx->r2 = 0;
            return;
        }
    }

    g_yield_counter++;

    /* Yield-escape: when armed (we're inside a per-frame mode call), after
     * threshold yields, longjmp out to the dispatcher. This turns infinite
     * yield-loops in fiber-resume mid-entries into per-frame iteration. */
    if (g_yield_escape_armed && g_yield_counter >= g_yield_escape_threshold) {
        g_yield_escape_armed = 0;  /* disarm to avoid re-entry */
        longjmp(g_yield_escape_buf, 1);
    }

    /* Advance frame counters */
    uint32_t* vbl = (uint32_t*)(rdram + 0x001A35CC);
    uint32_t* tick = (uint32_t*)(rdram + 0x001A35C8);
    uint32_t* sync1 = (uint32_t*)(rdram + 0x001A1AB0);
    uint32_t* sync2 = (uint32_t*)(rdram + 0x001A1AB8);
    (*vbl)++;
    (*tick) += 16667;
    (*sync1)++;
    (*sync2) = (*sync1) - 1;

    /* DCS state simulation: only when yield-escape is armed (i.e. we're
     * inside an attract scene func or per-frame mode call), set bit 0x4
     * at 0x001DDDE0 after a few yields. func_800CAFD0/CB19C set that
     * address to 0x1 (a state transition) then wait for the DCS driver
     * to OR-in bit 0x4 as the "ready" signal. We stub DCS, so we have
     * to set the bit ourselves — but only when the function is genuinely
     * blocking on it (a few yields in), not preemptively (which would
     * make brand-new wait-loops skip on their first iteration). */
    if (g_yield_escape_armed && g_yield_counter == 10) {
        *(uint32_t*)(rdram + 0x001DDDE0) |= 0x04;
    }

    ctx->r2 = 0;
}

/* Override func_80143A40 (heap malloc) to trace allocation failures.
 * Original renamed to func_80143A40_original in funcs_30.c */
extern RECOMP_FUNC void func_80143A40_original(uint8_t* rdram, recomp_context* ctx);

/* Track the large render buffer allocation for fixing GP */
uint32_t g_render_buffer_addr = 0;

RECOMP_FUNC void func_80143A40(uint8_t* rdram, recomp_context* ctx) {
    uint32_t size = (uint32_t)ctx->r4;
    uint32_t heap_head = *(uint32_t*)(rdram + 0x001A1E90);
    uint32_t hp = heap_head & 0x1FFFFFFF;
    uint32_t free_sz = (hp < 0x00800000 - 4) ? (*(uint32_t*)(rdram + hp) & ~1u) : 0;

    func_80143A40_original(rdram, ctx);

    uint32_t result = (uint32_t)ctx->r2;
    static int alloc_log = 0;
    alloc_log++;
    if (result == 0 || alloc_log <= 5 || size > 100000) {
        fprintf(stderr, "[malloc] size=%u (0x%X) heap=0x%08X free=%u -> 0x%08X%s\n",
                size, size, heap_head, free_sz, result, result == 0 ? " FAILED!" : "");
    }

    /* Save the 1.75MB render buffer address (malloc(0x1C0020)) */
    if (size == 0x1C0020 && result != 0 && g_render_buffer_addr == 0) {
        /* Align to 32 bytes (matching main_loop's alignment loop) */
        uint32_t phys = result & 0x1FFFFFFF;
        g_render_buffer_addr = (phys + 0x1F) & ~0x1F;
        fprintf(stderr, "[malloc] Render buffer: raw=0x%08X aligned=0x%08X\n",
                phys, g_render_buffer_addr);
    }
}

/* Override func_80161140 (Voodoo PCI driver init).
 * Populate the device descriptor table BEFORE calling the real function
 * so it can find the Voodoo during PCI scan. */
extern RECOMP_FUNC void func_80161140_original(uint8_t* rdram, recomp_context* ctx);
RECOMP_FUNC void func_80161140(uint8_t* rdram, recomp_context* ctx) {
    /* Populate full descriptor entry including float values.
     * entry[+24] and [+28] are floats read by func_800C42D8 for display init. */
    uint32_t e = 0x001E52B0;
    memset(rdram + e, 0, 152);
    *(uint32_t*)(rdram + e + 0)  = 0x08100000;
    *(uint32_t*)(rdram + e + 8)  = 0x00800000;
    *(uint32_t*)(rdram + e + 20) = 0x00200000;
    *(uint32_t*)(rdram + e + 24) = 0x40000000; /* 2.0f (FBI_MEM as float) */
    *(uint32_t*)(rdram + e + 28) = 0x40800000; /* 4.0f (TMU0_MEM as float) */
    *(uint32_t*)(rdram + e + 32) = 512;
    *(uint32_t*)(rdram + e + 36) = 384;
    *(uint32_t*)(rdram + e + 40) = 60;
    *(uint32_t*)(rdram + 0x001A3354) = 1;
    *(uint32_t*)(rdram + 0x001E0350) = 0x801E52B0;
    *(uint32_t*)(rdram + 0x00236810) = 0x00800000;

    static int c = 0; c++;
    if (c <= 3) fprintf(stderr, "[pci_drv] func_80161140: populated descriptor, calling real...\n");

    func_80161140_original(rdram, ctx);

    if (c <= 3)
        fprintf(stderr, "[pci_drv] func_80161140: r2=0x%08X base=0x%08X render=0x%08X\n",
                (uint32_t)ctx->r2,
                *(uint32_t*)(rdram + 0x001AA660),
                *(uint32_t*)(rdram + 0x0022A454));
}

/* Override func_800F25E0 (CMOS init/validation).
 * The original RUNS THE REAL CMOS LAYOUT INIT (-> sub_8014586C), which copies the
 * layout block and sets the adjustment COUNT dword_80236630=100 plus the NVRAM
 * base/bound globals (dword_801DFED8, dword_801E65C0) that every adjustment
 * read/write depends on. The previous override skipped all of this and just
 * returned 0 -- so the count stayed 0, every sub_80146168 returned -9 (out of
 * bounds), all adjustments read as "invalid", and the game booted into DIAGNOSTIC
 * mode (dword_801DFE70 != 0) instead of normal attract. Now: run the real init
 * for its side effects, then ignore its -3 return (our NVRAM doesn't fully
 * validate) and report success so the cmos-err path isn't taken. */
extern RECOMP_FUNC void func_800F25E0_original(uint8_t*, recomp_context*);
RECOMP_FUNC void func_800F25E0(uint8_t* rdram, recomp_context* ctx) {
    func_800F25E0_original(rdram, ctx);
    if (getenv("CARNEVIL_ADJDBG")) {
        static int c = 0;
        if (c++ < 3) fprintf(stderr, "[cmos] real CMOS init ran (ret=%d), forcing OK. count=%u csize=%u base=0x%X bound=0x%X\n",
                        (int)(int32_t)ctx->r2, *(uint32_t*)(rdram+0x236630), *(uint32_t*)(rdram+0x236628),
                        *(uint32_t*)(rdram+0x1DFED8), *(uint32_t*)(rdram+0x1E65C0));
    }
    ctx->r2 = 0; /* ignore -3; the layout globals (count/base) are set above */
}

/* ======================================================================
 * DCS2 Sound Manager Override -- func_80138954
 * The original is a state machine that loads sound banks from the filesystem
 * and transfers them to the DCS2 sound board. Since we don't have real
 * DCS2 hardware, the transfers fail and cause infinite retry loops.
 * Override to no-op: skip all bank loading. */
RECOMP_FUNC void func_80138954(uint8_t* rdram, recomp_context* ctx) {
    ctx->r2 = 0; /* success — no bank loading needed */
}

/* ======================================================================
 * Voodoo Command Buffer Builder Override
 *
 * func_80167848 is the core DMA command builder. Every rendering function
 * calls it to append Voodoo register writes to a GP-relative command buffer.
 *
 * Original behavior:
 *   GP[0] = a1 (byte count of data)
 *   GP[4] = GP+0x10 (pointer to data start)
 *   GP[8] = a0 (PCI destination address, e.g. 0x0800010C = Voodoo reg 0x10C)
 *   GP[C] = next_ptr (aligned to 32 bytes)
 *   GP[0x10..] = data words (written by caller before this function)
 *   GP = next_ptr (advance cursor)
 *
 * Our override: replicate the buffer building AND immediately execute
 * the Voodoo register writes so we get actual pixel output.
 *
 * PCI address mapping:
 *   0x08000000 + offset = Voodoo registers (PCI bus address)
 *   0x00800000 + offset = Voodoo LFB (PCI bus address)
 * ====================================================================== */
RECOMP_FUNC void func_80167848(uint8_t* rdram, recomp_context* ctx) {
    uint32_t dest_pci = (uint32_t)ctx->r4;   /* PCI destination address */
    uint32_t byte_count = (uint32_t)ctx->r5;  /* data size in bytes */
    uint32_t gp = (uint32_t)ctx->r28;
    uint32_t gp_phys = gp & 0x1FFFFFFF;

    /* Replicate original buffer-building logic */
    uint32_t t0 = ((byte_count >> 2) << 2) + 0x10;
    uint32_t t0_abs = gp_phys + t0;

    /* Write command header to GP buffer */
    if (gp_phys > 0 && gp_phys + 0x20 < 0x00800000) {
        *(uint32_t*)(rdram + gp_phys + 0x00) = byte_count;
        *(uint32_t*)(rdram + gp_phys + 0x04) = gp + 0x10;
        *(uint32_t*)(rdram + gp_phys + 0x08) = dest_pci;
    }

    /* Align to 32-byte boundary (original loop: while (t0 & 0x1C) t0 += 4) */
    if (t0_abs & 0x1C) {
        t0_abs = (t0_abs + 0x1F) & ~0x1F;
    }

    /* Write next pointer */
    if (gp_phys > 0 && gp_phys + 0x0C < 0x00800000) {
        *(uint32_t*)(rdram + gp_phys + 0x0C) = (gp & 0xE0000000) | t0_abs;
    }

    /* Advance GP */
    ctx->r28 = (gpr)((gp & 0xE0000000) | t0_abs);

    /* --- Intercept and execute Voodoo writes --- */
    static int gfx_cmd_count = 0;
    gfx_cmd_count++;
    if (gfx_cmd_count <= 100 || gfx_cmd_count % 10000 == 0) {
        fprintf(stderr, "[gfx_cmd] #%d dest=0x%08X bytes=%d GP=0x%08X\n",
                gfx_cmd_count, dest_pci, byte_count, gp);
    }

    /* Determine Voodoo register offset from PCI address */
    uint32_t voodoo_off = 0xFFFFFFFF; /* invalid */
    if (dest_pci >= 0x08000000 && dest_pci < 0x08100000) {
        /* PCI bus address for Voodoo registers: 0x08000000 + reg_offset */
        voodoo_off = dest_pci - 0x08000000;
    } else if (dest_pci >= 0x08100000 && dest_pci < 0x09000000) {
        /* CPU MMIO address for Voodoo registers */
        voodoo_off = dest_pci - 0x08100000;
    } else if (dest_pci >= 0x00800000 && dest_pci < 0x01800000) {
        /* LFB writes */
        voodoo_off = dest_pci - 0x00800000;
    }

    if (voodoo_off != 0xFFFFFFFF && byte_count > 0 && byte_count <= 4096) {
        uint32_t data_phys = gp_phys + 0x10;
        if (data_phys > 0 && data_phys + byte_count <= 0x00800000) {
            for (uint32_t i = 0; i < byte_count; i += 4) {
                uint32_t val = *(uint32_t*)(rdram + data_phys + i);
                voodoo_write(&g_voodoo, voodoo_off + i, val);
            }

            if (gfx_cmd_count <= 30) {
                uint32_t first_val = *(uint32_t*)(rdram + data_phys);
                fprintf(stderr, "[gfx_cmd]   -> voodoo reg 0x%03X = 0x%08X\n",
                        voodoo_off, first_val);
            }
        }
    }
}

/* ======================================================================
 * Attract Mode Camera Update -- func_800CAE2C
 *
 * This is a mid-function entry point inside func_800CADD4.
 * The original code reads input bits from 0x80236790 and adjusts
 * camera float values at 0x80180000-based addresses.
 * Each bit controls a different camera axis.
 *
 * Since this is a mid-function entry (fiber resume point), we replicate
 * the camera update logic as a callable function.
 * ====================================================================== */
RECOMP_FUNC void func_800CAE2C(uint8_t* rdram, recomp_context* ctx) {
    static int call_count = 0;
    call_count++;

    /* On first call: run the attract mode scene functions.
     * These are func_800CAFD0, func_800CAF24, func_800CB19C, func_800CB31C
     * which load zones, create scene objects, and set up animations.
     * Each function: yield, setup zone, init scene, create objects. */
    /* Scene SETUP runs exactly once. (It was previously re-run every frame under
     * CARNEVIL_PERFRAME, but the setup funcs re-initialise the zone-config pointer
     * back to the stream start (0x80112DA4) -- so re-running them each frame reset
     * the zone parser to command 0 every frame, wedging it on the first "wait 428"
     * and starving the scene graph. Per-frame work now happens only in the zone-
     * processing block below; PERFRAME just keeps func_800CAE2C being called.) */
    if (call_count == 1) {
        extern recomp_func_t* get_function(int32_t);

        /* Pre-set bit 0x4 at 0x001DDDE0 ("DCS ready" flag).
         * func_800CAFD0 and func_800CB19C have wait-loops polling this
         * bit; in the real game it gets set asynchronously by the DCS
         * audio chip's "ready" signal. Our DCS is stubbed so nothing ever
         * sets it naturally. Without this preempt the functions yield-loop
         * for nothing — no other code runs in the meantime because our
         * scheduler doesn't preempt during yields.
         *
         * Earlier, this set lived inside func_80151618 (yield), which had
         * the bad side-effect of also unblocking *new* polls created later
         * in the run. Pre-setting here keeps it scoped to attract init. */
        *(uint32_t*)(rdram + 0x001DDDE0) |= 0x04;

        /* Call the attract mode scene functions in order.
         * Arm yield-escape so they can progress past internal yield-loops
         * (they call func_80151618 expecting fiber suspension). */
        uint32_t scene_funcs[] = { 0x800CAFD0, 0x800CAF24, 0x800CB19C, 0x800CB31C };
        extern jmp_buf g_yield_escape_buf;
        extern int g_yield_escape_armed;
        for (int i = 0; i < 4; i++) {
            recomp_func_t* sfn = get_function((int32_t)scene_funcs[i]);
            if (sfn) {
                fprintf(stderr, "[attract] Calling scene func 0x%08X...\n", scene_funcs[i]);
                ctx->r4 = (gpr)i;
                int yc_save = g_yield_counter;
                int thresh_save = g_yield_escape_threshold;
                g_yield_counter = 0;
                g_yield_escape_threshold = 50000;  /* high threshold for init */
                g_yield_escape_armed = 1;
                int reason = setjmp(g_yield_escape_buf);
                if (reason == 0) {
                    sfn(rdram, ctx);
                    fprintf(stderr, "[attract] Scene func 0x%08X returned (yields=%d)\n",
                            scene_funcs[i], g_yield_counter);
                } else {
                    fprintf(stderr, "[attract] Scene func 0x%08X yield-escaped after %d yields\n",
                            scene_funcs[i], g_yield_counter);
                }
                g_yield_escape_armed = 0;
                g_yield_counter = yc_save;
                g_yield_escape_threshold = thresh_save;
            }
        }

        /* EXPERIMENT (CARNEVIL_INVOKE10): right after the scene funcs register the
         * render handlers at entry+0x10 (and before the scaffolding below clears
         * them), invoke each populated entity's +0x10 handler -- the long-missing
         * spawn/render dispatch -- and force the gate >=2 so the per-frame
         * func_800CC47C dispatch will run. See if triangles flow. */
        if (getenv("CARNEVIL_INVOKE10")) {
            extern recomp_func_t* get_function(int32_t);
            *(uint16_t*)(rdram + 0x00212250) = 2;   /* gate -> dispatch enabled */
            int inv = 0;
            for (int i = 0; i < 128; i++) {
                uint32_t e = 0x001E3880 + i*0x28;
                uint16_t id = *(uint16_t*)(rdram + e);
                uint32_t h10 = *(uint32_t*)(rdram + e + 0x10);
                if (id != 0xFFFF && h10 >= 0x80000000) {
                    recomp_func_t* h = get_function((int32_t)h10);
                    if (h) {
                        fprintf(stderr, "[invoke10] idx%d invoking +0x10=0x%08X\n", i, h10);
                        ctx->r4 = (gpr)(int32_t)(0x80000000u | e);   /* entity slot vaddr */
                        h(rdram, ctx);
                        inv++;
                    }
                }
            }
            fprintf(stderr, "[invoke10] invoked %d render handlers\n", inv);
        }

        /* After scene functions set up zone config, try calling the zone builder.
         * func_8010FE90 processes zone data from the config at 0x001DFEB8.
         * It should parse zone files and create scene objects. */
        {
            uint32_t zone_cfg = *(uint32_t*)(rdram + 0x001DFEB8);
            uint32_t zone_cfg2 = *(uint32_t*)(rdram + 0x001E0C4C);
            fprintf(stderr, "[attract] Zone config: 0x%08X / 0x%08X\n", zone_cfg, zone_cfg2);

            /* Set the zone loading trigger flags */
            *(uint32_t*)(rdram + 0x001E50AC) = 1;  /* func_8010FE60 sets this */
            *(uint32_t*)(rdram + 0x001E4FF4) = 1;  /* func_8010FE74 sets this */

            /* Call the zone processor to parse zone data and create scene objects.
             * Clear the "already loaded" flag and set r2=0 for normal entry. */
            /* Don't clear bit 0x10000 — it's the DCS2 ready flag that prevents
             * the sound manager from endlessly retrying bank loading. */
            *(uint32_t*)(rdram + 0x00212354) = 0;          /* clear delay counter */

            /* Force display state so zone parser proceeds */
            *(uint32_t*)(rdram + 0x001E0444) = 4;
            *(uint32_t*)(rdram + 0x001E055C) = 4;

            /* Phase A: test-fixture scene-node creation disabled.
             * This block manually invoked func_800D7600 with synthetic args
             * to populate the scene graph with placeholder nodes for
             * diagnostic purposes. It masked the real game's failure to
             * produce scene nodes, so we removed it to expose the actual
             * zone-parser stall. Real nodes should come from the zone
             * processing chain (func_8010FE90 -> ... -> func_800D7600). */
        }

        /* Check if scene graph now has objects.
         * List head is at 0x0017B6D8 + 0x44 = 0x0017B71C (next ptr in root node).
         * Each node links via offset +0x44. */
        uint32_t sg_head = *(uint32_t*)(rdram + 0x0017B71C);
        int sg_count = 0;
        uint32_t cur = sg_head;
        while (cur != 0 && sg_count < 200) {
            uint32_t phys = cur & 0x1FFFFFFF;
            if (phys + 0x48 >= 0x00800000) break;
            cur = *(uint32_t*)(rdram + phys + 0x44);
            sg_count++;
        }
        fprintf(stderr, "[attract] Scene graph (at +0x44): head=0x%08X count=%d\n", sg_head, sg_count);
    }

    /* Process zone commands. Use a global yield counter to detect infinite loops.
     * If func_80151618 is called > 1000 times during a single zone command,
     * the command is blocking — force-skip it by advancing the config pointer. */
    /* Zone processing: skip past the blocking region.
     * Frames 2-50 work (cfg 0x80112DAC-0x80113274). Frame 51+ blocks.
     * Try jumping the config pointer past the trouble zone. */
    static int zone_done = 0;
    static int zone_skip_until = 0;
    if (call_count > 1 && !zone_done && call_count >= zone_skip_until) {
        extern recomp_func_t* get_function(int32_t);
        static int zone_cmd_count = 0;
        extern int g_yield_counter;  /* declared below in func_80151618 */

        /* Set flags for zone processing */
        /* Don't clear bit 0x10000 — DCS2 ready flag */
        *(uint32_t*)(rdram + 0x001E4FF4) = 1;
        *(uint32_t*)(rdram + 0x001E0444) = 4;
        *(uint32_t*)(rdram + 0x001E055C) = 4;
        *(uint32_t*)(rdram + 0x00212354) = 0;
        *(uint32_t*)(rdram + 0x001DDDE0) |= 0x04;

        recomp_func_t* process_zone = get_function(0x8010FE90);
        if (process_zone) {
            uint32_t cfg_before = *(uint32_t*)(rdram + 0x001E0C4C);

            g_yield_counter = 0;

            /* Don't clear bit 0x10000 globally — func_8010FE90 is patched to
             * bypass its own check. Other functions still need the bit set. */

            /* Check config pointer. The config stream has 413 command words
             * from 0x80112DA4 to 0x80113A20. Some commands read multi-word
             * arguments. If the pointer enters a region where the parser
             * gets misaligned (reads data words as commands), skip to end. */
            uint32_t cfg_cur = *(uint32_t*)(rdram + 0x001E0C4C);
            uint32_t cfg_phys_pre = cfg_cur & 0x1FFFFFFF;

            /* Read the next command value to check validity */
            uint32_t next_cmd = 0;
            if (cfg_phys_pre > 0 && cfg_phys_pre + 4 < 0x00800000)
                next_cmd = *(uint32_t*)(rdram + cfg_phys_pre);

            /* If the value is not a valid command (1-29) AND not a known
             * data value used by multi-word commands, we're misaligned.
             * Force the config pointer past the end. */
            if (cfg_phys_pre >= 0x00113270 && (next_cmd == 0 || next_cmd > 29)) {
                *(uint32_t*)(rdram + 0x001E0C4C) = 0x80113A24; /* past end */
                fprintf(stderr, "[zone] Config misaligned at 0x%08X (val=0x%X), marking done\n",
                        cfg_cur, next_cmd);
                zone_done = 1;
                goto zone_done_skip; /* skip the process_zone call */
            }

            ctx->r2 = 0;
            ctx->r4 = 0;
            g_zone_processing_active = 1;
            /* func_8010FE90 gates ALL zone-command parsing behind
             * (dword_802122E0 & 0x10000) == 0 (loc_8010FEB4: lui $v1,1; and;
             * bnez -> exit). Bit 0x10000 is the DCS2 "ready" flag we keep set so
             * the sound manager stops retrying bank loads -- but with it set the
             * zone processor bails immediately (ret=0, config pointer never
             * advances, scene graph stays empty -> no demo, frozen frame).
             * Clear it just for this synchronous call, then restore, so zone
             * parsing runs without disturbing the DCS state seen elsewhere. */
            uint32_t dcs_save = *(uint32_t*)(rdram + 0x002122E0);
            *(uint32_t*)(rdram + 0x002122E0) = dcs_save & ~0x10000u;
            process_zone(rdram, ctx);
            *(uint32_t*)(rdram + 0x002122E0) = dcs_save;
            g_zone_processing_active = 0;
            int ret = (int)(int32_t)ctx->r2;

            uint32_t cfg_after = *(uint32_t*)(rdram + 0x001E0C4C);

            if (call_count <= 50 || call_count % 200 == 0) {
                fprintf(stderr, "[zone] frame=%d ret=%d cfg=0x%08X yields=%d\n",
                        call_count, ret, cfg_after, g_yield_counter);
            }

            /* Check for end of config stream (config ends at 0x80113A20) */
            uint32_t cfg_phys = cfg_after & 0x1FFFFFFF;
            if (cfg_phys >= 0x00113A20 || cfg_phys == 0) {
                zone_done = 1;
                uint32_t sg_head = *(uint32_t*)(rdram + 0x0017B71C);
                int sg_n = 0;
                uint32_t c2 = sg_head;
                while (c2 && sg_n < 200) {
                    c2 = *(uint32_t*)(rdram + (c2 & 0x1FFFFFFF) + 0x44);
                    sg_n++;
                }
                fprintf(stderr, "[zone] DONE: %d cmds, sg_head=0x%08X count=%d\n",
                        zone_cmd_count, sg_head, sg_n);
            }
        }

        if (call_count <= 100 || call_count % 200 == 0) {
            uint32_t sg_head = *(uint32_t*)(rdram + 0x0017B71C);
            fprintf(stderr, "[zone_frame] frame=%d cmds=%d sg_head=0x%08X\n",
                    call_count, zone_cmd_count, sg_head);
        }
    }

    zone_done_skip: ;
    /* CARNEVIL_TRI3D: validation hook for the 3D triangle path. Drives the
     * depth-tested 3D emitter sub_800D2654 directly with three hand-built
     * 9-float vertices {screenX,screenY,W,S,T,Z,R,G,B}. Emitted here so it
     * rides the same DMA/flush context as the INVOKE10 2D triangles. S/T point
     * at a bright font texel so the triangle is visible whether the current
     * render state is textured (rgbselect=1) or Gouraud (rgbselect=0). */
    if (getenv("CARNEVIL_TRI3D")) {
        extern recomp_func_t* get_function(int32_t);
        recomp_func_t* emit = get_function(0x800D2654);
        if (emit) {
            static const float vtx[3][9] = {
                {200.f,150.f, 1.f, 188.f,205.f, 0.5f, 255.f, 32.f, 32.f},
                {440.f,150.f, 1.f, 188.f,205.f, 0.5f,  32.f,255.f, 32.f},
                {320.f,360.f, 1.f, 188.f,205.f, 0.5f,  32.f, 32.f,255.f},
            };
            uint32_t base = 0x007CE000;
            for (int i=0;i<3;i++) for(int j=0;j<9;j++)
                *(float*)(rdram + base + i*0x24 + j*4) = vtx[i][j];
            ctx->r4 = (gpr)(int32_t)(0x80000000u | (base + 0*0x24));
            ctx->r5 = (gpr)(int32_t)(0x80000000u | (base + 1*0x24));
            ctx->r6 = (gpr)(int32_t)(0x80000000u | (base + 2*0x24));
            emit(rdram, ctx);
            static int once=0; if(!once){once=1; fprintf(stderr,"[tri3d] emitted test triangle via sub_800D2654\n");}
        }
    }
    /* CARNEVIL_MESH3D: render the BABY model mesh with a hand-rolled ortho
     * projection (bypassing the game camera/T&L), driving the 3D emitter
     * sub_800D2654 per face. We read BABY.ZM ourselves.
     *
     * .ZM layout (verified against the recompiled mesh walker func_800E1EF0,
     * which strides r18 by 0x28 and reads six halfwords at +4..+0xF):
     *   0x000  512B thumbnail header  -- same preview header .EXE files carry
     *   0x200  942 verts   x 12B float xyz
     *   0x2C48 946 normals x 12B float xyz
     *   0x5A80 1693 faces  x 40B: u32 surfaceId, u16 vert idx @+4/+6/+8,
     *                             u16 normal idx @+0xA/+0xC/+0xE,
     *                             6x float S/T @+0x10
     * 512 + 942*12 + 946*12 + 1693*40 = 90888, 8B slack in a 90896B file.
     *
     * The earlier "assets are a different build" conclusion was wrong: the
     * layout was simply being read from offset 0 instead of 0x200, which put
     * every index 512B out of phase (46/1706 in-range). Read at the right
     * offset, 1693/1693 faces and 942/942 verts validate, from the .ZM we
     * already had. BABY.ZM is byte-identical on both the v1.0.1 and v1.0.3
     * disks, which is what ruled out the wrong-revision theory. */
    if (getenv("CARNEVIL_MESH3D")) {
        extern recomp_func_t* get_function(int32_t);
        static int ready = 0;
        static float scale=1.f, cx=320.f, cy=240.f;
        const int vcount=942, fcount=1693, ccount=946;
        const uint32_t zmhdr = 512;                     /* thumbnail header */
        const uint32_t modbase = 0x00600000;            /* raw .ZM in RDRAM */
        const uint32_t vp = modbase + zmhdr;
        const uint32_t cp = vp + (uint32_t)vcount*12;
        const uint32_t fp = cp + (uint32_t)ccount*12;
        const uint32_t xf = 0x007B0000;                 /* transformed verts */
        if (!ready) {
            FILE* zf = fopen("extracted/files/BABY.ZM", "rb");
            if (zf) {
                size_t need = zmhdr + (size_t)vcount*12 + (size_t)ccount*12 + (size_t)fcount*40;
                size_t n = fread(rdram + modbase, 1, need, zf);
                fclose(zf);
                /* ponytail: all 942 verts are finite once the header offset is
                 * right; this stays as a cheap guard against a bad .ZM. */
                float mnx=1e9f,mxx=-1e9f,mny=1e9f,mxy=-1e9f,mnz=1e9f,mxz=-1e9f;
                for (int i=0;i<vcount;i++){
                    float x=*(float*)(rdram+vp+i*12+0), y=*(float*)(rdram+vp+i*12+4), z=*(float*)(rdram+vp+i*12+8);
                    if (!(x>-1e4f&&x<1e4f) || !(y>-1e4f&&y<1e4f) || !(z>-1e4f&&z<1e4f)) continue;
                    if(x<mnx)mnx=x; if(x>mxx)mxx=x; if(y<mny)mny=y; if(y>mxy)mxy=y; if(z<mnz)mnz=z; if(z>mxz)mxz=z;
                }
                float w=mxx-mnx, h=mxy-mny, ext=(w>h?w:h); if(ext<1e-3f)ext=1.f;
                scale = 280.f/ext;
                cx = 320.f - 0.5f*(mnx+mxx)*scale;
                cy = 240.f + 0.5f*(mny+mxy)*scale;   /* +: flip Y for screen */
                fprintf(stderr,"[mesh3d] read %zu/%zu bytes; range x[%.1f,%.1f] y[%.1f,%.1f] z[%.1f,%.1f] scale=%.3f\n",
                        n,need,mnx,mxx,mny,mxy,mnz,mxz,scale);
                ready = 1;
            } else { fprintf(stderr,"[mesh3d] fopen BABY.ZM failed\n"); ready = -1; }
        }
        if (ready == 1) {
            /* xf stride is 16B: screen x, screen y, W, Z (depth 0.1..0.9). */
            float mnz=1e9f, mxz=-1e9f;
            for (int i=0;i<vcount;i++){
                float z=*(float*)(rdram+vp+i*12+8);
                if (z<mnz) mnz=z; if (z>mxz) mxz=z;
            }
            float zr = (mxz-mnz); if (zr<1e-4f) zr=1.f;
            for (int i=0;i<vcount;i++){
                float x=*(float*)(rdram+vp+i*12+0), y=*(float*)(rdram+vp+i*12+4),
                      z=*(float*)(rdram+vp+i*12+8);
                *(float*)(rdram+xf+i*16+0) = cx + x*scale;
                *(float*)(rdram+xf+i*16+4) = cy - y*scale;
                *(float*)(rdram+xf+i*16+8) = 1.0f;                    /* W (ortho) */
                *(float*)(rdram+xf+i*16+12) = 0.1f + 0.8f*(mxz-z)/zr; /* Z, nearer = smaller */
            }
            /* Emit the real surface: one triangle per face record, indices from
             * +4/+6/+8, normals from +0xA/+0xC/+0xE (Lambert against a fixed
             * light for a readable shade). S/T is pinned to the same bright font
             * texel the validated tri3d path uses, so this stays visible whether
             * the current render state is textured or Gouraud.
             * Vertex layout for sub_800D2654 = {x, y, W, S*W, T*W, Z, R, G, B}. */
            recomp_func_t* emit = get_function(0x800D2654);
            if (emit) {
                gpr r4=ctx->r4,r5=ctx->r5,r6=ctx->r6;
                const uint32_t tb = 0x007C0000;          /* 3-vertex scratch */
                int drawn=0, skipped=0;
                for (int f=0;f<fcount;f++){
                    const uint32_t fr = fp + (uint32_t)f*40;
                    uint16_t vi[3], ni[3];
                    for (int k=0;k<3;k++){
                        vi[k] = *(uint16_t*)(rdram + fr + 4 + k*2);
                        ni[k] = *(uint16_t*)(rdram + fr + 0xA + k*2);
                    }
                    if (vi[0]>=vcount||vi[1]>=vcount||vi[2]>=vcount) { skipped++; continue; }
                    int off=0;
                    for (int k=0;k<3;k++){
                        float X=*(float*)(rdram+xf+vi[k]*16+0), Y=*(float*)(rdram+xf+vi[k]*16+4);
                        if (!(X>=-2048.f&&X<2048.f) || !(Y>=-2048.f&&Y<2048.f)) off=1;
                    }
                    if (off) { skipped++; continue; }
                    for (int k=0;k<3;k++){
                        float* o = (float*)(rdram + tb + (uint32_t)k*36);
                        o[0]=*(float*)(rdram+xf+vi[k]*16+0);
                        o[1]=*(float*)(rdram+xf+vi[k]*16+4);
                        o[2]=1.0f;
                        o[3]=188.f; o[4]=205.f;                        /* S,T (W=1) */
                        o[5]=*(float*)(rdram+xf+vi[k]*16+12);          /* Z */
                        /* Lambert: light from (0,0,1), normal z drives shade. */
                        float nz = (ni[k]<ccount) ? *(float*)(rdram+cp+ni[k]*12+8) : 1.f;
                        float l = 0.30f + 0.70f*(nz<0.f?-nz:nz);
                        if (l>1.f) l=1.f;
                        o[6]=235.f*l; o[7]=225.f*l; o[8]=215.f*l;
                    }
                    ctx->r4=(gpr)(int32_t)(0x80000000u|(tb+0*36));
                    ctx->r5=(gpr)(int32_t)(0x80000000u|(tb+1*36));
                    ctx->r6=(gpr)(int32_t)(0x80000000u|(tb+2*36));
                    emit(rdram, ctx);
                    drawn++;
                }
                ctx->r4=r4; ctx->r5=r5; ctx->r6=r6;
                static int once=0; if(!once){once=1;
                    fprintf(stderr,"[mesh3d] surface: %d tris drawn, %d skipped\n",drawn,skipped);}
            }
        }
    }
    /* Camera update: call func_800CADD4 */
    {
        extern recomp_func_t* get_function(int32_t);
        recomp_func_t* parent = get_function(0x800CADD4);
        if (parent) {
            ctx->r1 = (gpr)(int32_t)0x80180000;
            parent(rdram, ctx);
        }
    }

    if (call_count <= 5 || call_count % 1000 == 0) {
        fprintf(stderr, "[attract_cam] call #%d\n", call_count);
    }

    ctx->r2 = 0;
}

/* ======================================================================
 * State Machine Mode Dispatcher -- func_8014A488
 *
 * The RTOS mode system stores registered mode entries at 0x001A25F0.
 * func_80148AFC populates: entry[+0x24] = mode function, entry[+0x10] = data.
 * This dispatcher reads the registered entry and calls the mode function.
 *
 * NOTE: 0x001A25F8 is the "current task" pointer, NOT the mode entry.
 * It often points to the display global (0x80710100) which has garbage
 * at offset +0x24. Always use 0x001A25F0 for mode dispatch.
 * ====================================================================== */
extern RECOMP_FUNC void func_80151618(uint8_t* rdram, recomp_context* ctx);

/* Yield-escape: when a mode function is called per-frame, its body often
 * contains infinite loops that call yield() expecting to be suspended.
 * Our func_80151618 yield is a no-op, so those loops spin.
 *
 * Set up a setjmp here before each mode call; func_80151618 longjmps out
 * when called too many times, returning control to the dispatcher. The
 * mode function effectively becomes "do one frame's worth of work."
 *
 * The jmp_buf is held in TLS-style globals; func_80151618 reads them. */
#include <setjmp.h>
jmp_buf g_yield_escape_buf;
int g_yield_escape_armed = 0;
int g_yield_escape_threshold = 200;  /* yields before escape */

RECOMP_FUNC void func_8014A488(uint8_t* rdram, recomp_context* ctx) {
    /* Read the current mode entry from 0x001A25F0 */
    uint32_t entry_ptr = *(uint32_t*)(rdram + 0x001A25F0);
    uint32_t entry_phys = entry_ptr & 0x1FFFFFFF;

    if (entry_phys == 0 || entry_phys + 0x30 >= 0x00800000) {
        return;
    }

    static int dispatch_count = 0;
    dispatch_count++;

    /* Walk the mode-entry doubly-linked list to head, then forward.
     * Mode entries are linked via [+0]=prev and [+4]=next; mode_fn at [+24].
     * Multiple modes register (attract, camera, render layer, etc.) and the
     * game's real per-frame dispatcher calls them all in order. We were only
     * calling one, which is why entity dispatch was dormant. */
    uint32_t head = entry_ptr;
    for (int safety = 0; safety < 16; safety++) {
        uint32_t hp = head & 0x1FFFFFFF;
        if (hp == 0 || hp + 0x30 >= 0x00800000) break;
        uint32_t prev = *(uint32_t*)(rdram + hp + 0x00);
        if (prev == 0 || prev == head) break;
        head = prev;
    }

    int called = 0, skipped = 0;
    uint32_t cur = head;
    for (int safety = 0; safety < 16 && cur; safety++) {
        uint32_t cp = cur & 0x1FFFFFFF;
        if (cp == 0 || cp + 0x30 >= 0x00800000) break;
        uint32_t mfn = *(uint32_t*)(rdram + cp + 0x24);
        uint32_t dat = *(uint32_t*)(rdram + cp + 0x10);

        /* Attract-fiber path owns func_800C50AC; don't also dispatch it here
         * (that would run the state machine twice per frame and corrupt it). */
        {
            extern int g_use_attract_fiber;
            if (g_use_attract_fiber && mfn == 0x800C50AC) {
                uint32_t next = *(uint32_t*)(rdram + cp + 0x04);
                if (next == cur || next == 0) break;
                cur = next;
                continue;
            }
        }

        if (mfn >= 0x80000000 && mfn < 0x80800000) {
            extern void seattle_null_stub(uint8_t*, recomp_context*);
            recomp_func_t* fn = get_function((int32_t)mfn);
            int is_stub = (fn == seattle_null_stub);
            /* Mid-function split entries the recompiler missed.
             * Fall back to the containing function — it'll run from the
             * start and pass through the mid-entry point. The intervening
             * yield calls return benignly under our scheduler. */
            if (is_stub || !fn) {
                uint32_t fallback = 0;
                if (mfn == 0x800C6D08) fallback = 0x800C6AA8;
                else if (mfn == 0x800C78E4) fallback = 0x800C78BC;
                else if (mfn == 0x800E79C0) fallback = 0x800E7968;
                /* NOTE: per-frame parent calls keep boot working (each parent
                 * has a quick early-exit when its state preconditions aren't
                 * met). Direct calls to the split-entry recompiled bodies
                 * hang because they assume the prefix already ran in a
                 * yielded fiber. Fiber-resume support is the real fix. */
                if (fallback) {
                    recomp_func_t* pf = get_function((int32_t)fallback);
                    if (pf && pf != seattle_null_stub) {
                        fn = pf;
                        is_stub = 0;
                        if (dispatch_count <= 3) {
                            fprintf(stderr, "[state_dispatch] mid-entry 0x%08X -> parent 0x%08X\n",
                                    mfn, fallback);
                        }
                    }
                }
            }
            if (fn && !is_stub) {
                ctx->r4 = (gpr)dat;
                /* Arm yield-escape so func_80151618 can longjmp out of any
                 * infinite yield-loop in this mode function. */
                g_yield_escape_armed = 1;
                int yield_count_save = g_yield_counter;
                g_yield_counter = 0;
                if (setjmp(g_yield_escape_buf) == 0) {
                    fn(rdram, ctx);
                }
                /* whether we returned normally or via longjmp, disarm */
                g_yield_escape_armed = 0;
                g_yield_counter = yield_count_save;
                called++;
            } else {
                skipped++;
                if (dispatch_count <= 3) {
                    fprintf(stderr, "[state_dispatch] skipped mode_fn=0x%08X (entry 0x%08X)\n",
                            mfn, cur);
                }
            }
        }
        uint32_t next = *(uint32_t*)(rdram + cp + 0x04);
        if (next == cur || next == 0) break;
        cur = next;
    }

    if (dispatch_count <= 5 || dispatch_count % 500 == 0) {
        fprintf(stderr, "[state_dispatch] #%d: walked list, called=%d skipped=%d\n",
                dispatch_count, called, skipped);
    }

    /* After mode functions return, call yield */
    ctx->r4 = 1;
    func_80151618(rdram, ctx);
}

/* ======================================================================
 * Scene Graph Render Wrapper -- func_800D4C24
 * Traces the rendering pipeline to understand why no output is produced.
 * ====================================================================== */
extern RECOMP_FUNC void func_800D4C24_original(uint8_t* rdram, recomp_context* ctx);
RECOMP_FUNC void func_800D4C24(uint8_t* rdram, recomp_context* ctx) {
    static int render_call = 0;
    render_call++;

    /* Log the scene graph pointer list that func_800D4C24 iterates.
     * The display entry at 0x001E0440 has a linked list of scene objects.
     * Each display channel entry is 0x118 bytes. */
    if (render_call <= 5 || render_call == 50 || render_call == 500) {
        /* Check the REAL scene graph list head at 0x0017B71C (base+0x44) */
        uint32_t sg_head = *(uint32_t*)(rdram + 0x0017B71C);
        int sg_count = 0;
        uint32_t cur = sg_head;
        while (cur != 0 && sg_count < 100) {
            uint32_t phys = cur & 0x1FFFFFFF;
            if (phys + 0x48 >= 0x00800000) break;
            uint32_t next = *(uint32_t*)(rdram + phys + 0x44);
            sg_count++;
            cur = next;
        }
        fprintf(stderr, "[scene_render] #%d: sg_head=0x%08X sg_count=%d\n",
                render_call, sg_head, sg_count);

        /* Also check display entry at 0x1E0440 */
        for (int ch = 0; ch < 2; ch++) {
            uint32_t base = 0x001E0440 + ch * 0x118;
            uint32_t ch_state = *(uint32_t*)(rdram + base + 0x50);
            fprintf(stderr, "[scene_render] ch%d state=0x%X\n", ch, ch_state);
        }
    }

    /* Dump scene graph node contents to understand their type/data */
    if (render_call == 50 || render_call == 500) {
        uint32_t sg_head = *(uint32_t*)(rdram + 0x0017B71C);
        uint32_t cur = sg_head;
        int idx = 0;
        while (cur != 0 && idx < 10) {
            uint32_t phys = cur & 0x1FFFFFFF;
            if (phys + 0x60 >= 0x00800000) break;
            /* Dump first 24 words of each node */
            fprintf(stderr, "[sg_node] #%d @0x%08X:", idx, cur);
            for (int w = 0; w < 24 && phys + w*4 + 4 <= 0x00800000; w++) {
                uint32_t val = *(uint32_t*)(rdram + phys + w*4);
                if (val != 0) fprintf(stderr, " [%02X]=0x%08X", w*4, val);
            }
            fprintf(stderr, "\n");
            cur = *(uint32_t*)(rdram + phys + 0x44);
            idx++;
        }
    }

    /* Check critical rendering gates before calling original */
    if (render_call <= 5 || render_call == 50 || render_call == 500) {
        /* Float comparison gate: f12 (0x1E65A8) vs f0 (0x16F6C8) */
        uint32_t f12_raw = *(uint32_t*)(rdram + 0x001E65A8);
        uint32_t f0_raw  = *(uint32_t*)(rdram + 0x0016F6C8);
        /* Display mode at 0x2122D4 and render context at 0x22A444 */
        uint32_t disp_mode = *(uint32_t*)(rdram + 0x002122D4);
        uint32_t rctx_idx  = *(uint32_t*)(rdram + 0x0022A444);
        /* Scene active flags */
        uint32_t ddd80 = *(uint32_t*)(rdram + 0x001DDD80);
        uint32_t w6580 = *(uint32_t*)(rdram + 0x001E6580);
        uint32_t w65A4 = *(uint32_t*)(rdram + 0x001E65A4);
        uint32_t frame_ctr = *(uint32_t*)(rdram + 0x001A24CC);
        /* Additional check: *(0x1E02F0) — display resolution init state. -1 = not init. */
        uint32_t res_init = *(uint32_t*)(rdram + 0x001E02F0);
        /* Check *(0x1E0490) — display channel 0 active state */
        uint32_t ch0_active = *(uint32_t*)(rdram + 0x001E0490);
        /* Check render context entry[1] ready flag */
        uint32_t entry1_off = 0x001E6A20 + 70316;
        uint32_t entry1_ready = (entry1_off + 0x11178 + 4 < 0x00800000) ?
            *(uint32_t*)(rdram + entry1_off + 0x11178) : 0xDEAD;
        /* Check func_8015E300 index computation: idx * 70316 */
        fprintf(stderr, "[render_diag] #%d: mode=0x%X ctx=%u active=%u w6580=0x%X res_init=0x%X ch0=%u e1rdy=%u\n",
                render_call, disp_mode, rctx_idx, ddd80, w6580, res_init, ch0_active, entry1_ready);
    }

    func_800D4C24_original(rdram, ctx);

    if (render_call <= 10 || render_call == 50 || render_call == 500) {
        uint32_t writes_after = voodoo_get_write_count();
        fprintf(stderr, "[scene_render] #%d complete, total_voodoo=%u\n", render_call, writes_after);
    }
}

/* ======================================================================
 * VEC[64] Slot 0 Callback -- func_800C5FE4
 *
 * This is the MAIN per-frame rendering trigger. The recompiler missed it
 * because it's a split entry point (mid-function code at 0x800C5FE4 that
 * falls through to func_800C602C).
 *
 * Original code computes: frame_counter++ % display_list_count
 * Then calls func_800C602C which triggers the scene graph traversal.
 * Without this, no rendering happens during the frame loop.
 * ====================================================================== */
extern RECOMP_FUNC void func_800C602C(uint8_t* rdram, recomp_context* ctx);

RECOMP_FUNC void func_800C5FE4(uint8_t* rdram, recomp_context* ctx) {
    static int render_cb_count = 0;
    render_cb_count++;

    /* Replicate the prefix code at 0x800C5FE4-0x800C6028:
     * v1 = MEM[0x1A24CC] + 1   (increment frame counter)
     * v0 = MEM[0x177604]       (display list count / divisor)
     * a0 = v1 % v0             (frame index modulo)
     * v0 = MEM[0x1A24C8]       (load secondary counter)
     */
    uint32_t frame_ctr = *(uint32_t*)(rdram + 0x001A24CC);
    uint32_t divisor   = *(uint32_t*)(rdram + 0x00177604);
    frame_ctr++;

    /* Sanity check: divisor should be a small display list count (1-4).
     * It often gets corrupted by Voodoo PCI base address writes (0x08100000).
     * If corrupted, default to 2 (double buffering). */
    if (divisor == 0 || divisor > 16) {
        divisor = 2;
        *(uint32_t*)(rdram + 0x00177604) = divisor;
    }

    uint32_t remainder = frame_ctr % divisor;

    /* Set registers as the original code would before falling through.
     * The original prefix at 0x800C5FE4 sets these registers:
     *   v1 (r3) = frame_ctr (incremented)
     *   a0 (r4) = remainder (frame_ctr % divisor)
     *   v0 (r2) = *0x001A24C8 (secondary counter)
     *   at (r1) = 0x801A0000 (needed by func_800C602C for store instructions!) */
    ctx->r1 = (gpr)(int32_t)0x801A0000;
    ctx->r3 = (gpr)(int32_t)frame_ctr;
    ctx->r4 = (gpr)(int32_t)remainder;
    ctx->r2 = (gpr)(int32_t)(*(uint32_t*)(rdram + 0x001A24C8));

    if (render_cb_count <= 5 || render_cb_count % 500 == 0) {
        fprintf(stderr, "[vec64_cb] frame_ctr=%u divisor=%u remainder=%u (call #%d)\n",
                frame_ctr, divisor, remainder, render_cb_count);
    }

    /* Log scene graph gate conditions */
    if (remainder == 0) {
        uint32_t render_flag = *(uint32_t*)(rdram + 0x00212320);
        uint32_t display_init = *(uint32_t*)(rdram + 0x00179258);
        static int scene_log = 0;
        scene_log++;
        if (scene_log <= 10) {
            fprintf(stderr, "[vec64_cb] remainder==0: render_flag@0x212320=%u display_init@0x179258=%u\n",
                    render_flag, display_init);
        }
        /* Force display_init flag to 1 (Voodoo display ready).
         * On real hardware, the display init sequence sets this.
         * Our Voodoo emulation handles init registers but may not set this flag. */
        if (display_init != 1) {
            *(uint32_t*)(rdram + 0x00179258) = 1;
        }
    }

    /* Force gate values right before calling the render callback */
    *(uint32_t*)(rdram + 0x00179258) = 1;    /* display_init */
    /* Display mode state machine:
     * func_800D4C24: mode==0x40 → setup (resolution init), mode!=0x40 → channel setup
     * func_800D5D04: mode==2 → scene graph rendering (calls scene node handlers)
     * On real hardware, VBLANK interrupt transitions: 0x40 → 2 → render → swap
     * Simulate: first 2 calls do setup (0x40), then switch to render mode (2). */
    if (render_cb_count <= 2) {
        *(uint32_t*)(rdram + 0x002122D4) = 0x40;  /* setup mode */
    } else {
        *(uint32_t*)(rdram + 0x002122D4) = 2;     /* render mode */
    }
    /* Don't force voodoo active — the renderer manages it via *(0x1DDD80) */
    *(uint32_t*)(rdram + 0x001AA660) = 0x08100000; /* Voodoo PCI base - CRITICAL */
    /* Display channel active flags: 1=active, checked at *(0x1E0490+channel*0x118) */
    *(uint32_t*)(rdram + 0x001E0490) = 1;           /* channel 0 state */
    *(uint32_t*)(rdram + 0x001E0490 + 0x118) = 1;   /* channel 1 state */
    /* Display channel state at entry[+4]:
     * func_8010FE90 (zone processor) checks *(0x1E0444) & 0x44 (bits 2,6)
     * func_800D5D04 (scene renderer) checks *(0x1E0444) == 4 exactly
     * Set to 4 = bit 2 only. This satisfies both checks (4 & 0x44 = 4 != 0). */
    *(uint32_t*)(rdram + 0x001E0444) = 4;            /* channel 0: render mode */
    *(uint32_t*)(rdram + 0x001E055C) = 4;            /* channel 1: render mode */
    /* Clear render-busy flags at entry[+0x5C]. func_800D5D04 checks these and
     * skips rendering if non-zero (indicates render-in-progress). On real hardware,
     * VBLANK clears these. Force-clear every frame for our single-pass rendering. */
    *(uint32_t*)(rdram + 0x001E0440 + 0x5C) = 0;    /* channel 0 render sync */
    *(uint32_t*)(rdram + 0x001E0558 + 0x5C) = 0;    /* channel 1 render sync */

    /* Log the render buffer / scene graph state on first calls */
    if (render_cb_count <= 3) {
        uint32_t scene_count = *(uint32_t*)(rdram + 0x00212320);
        uint32_t render_ptr = *(uint32_t*)(rdram + 0x0022A444);
        uint32_t display_mode = *(uint32_t*)(rdram + 0x002122D4);
        uint32_t disp_init = *(uint32_t*)(rdram + 0x00179258);
        uint32_t vdoo_active = *(uint32_t*)(rdram + 0x001DDD80);
        uint32_t widget_flag = *(uint32_t*)(rdram + 0x001E6580);
        uint32_t pci_base = *(uint32_t*)(rdram + 0x001AA660);
        fprintf(stderr, "[render_gate] scene=%u render_ptr=0x%08X mode=0x%X init=%u active=%u widget=0x%X pci=0x%08X\n",
                scene_count, render_ptr, display_mode, disp_init, vdoo_active, widget_flag, pci_base);
    }

    /* Fall through to the real callback body */
    func_800C602C(rdram, ctx);
}
