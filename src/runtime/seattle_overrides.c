/**
 * Function overrides for CarnEvil runtime.
 * These replace NOP/BREAK stubs in the recompiled output with actual implementations.
 * They must have the same symbol names as the recompiled functions to override at link time.
 */

#include "recomp.h"
#include "rtos_scheduler.h"
#include "voodoo.h"
#include <stdio.h>
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

    /* Device I/O polling loop breaker: if we see > 1000 I/O calls
     * without yielding, force responses to non-zero to break loops. */
    {
        static int io_total = 0;
        io_total++;
        if (io_total > 1000 && io_total % 1000 == 0) {
            if (data_phys > 0 && data_phys + 4 <= 0x00800000)
                *(int32_t*)(rdram + data_phys) = (int32_t)0xFFFFFFFF;
            ctx->r2 = (gpr)(int32_t)0xFFFFFFFF;
            return;
        }
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
                /* DCS2 sound data read - return "ready" with all status bits set */
                static int dcs_read_count = 0;
                dcs_read_count++;
                if (resp) {
                    *resp = 0x55EA | 0xFF; /* handshake + all status bits */
                }
                if (dcs_read_count <= 5) {
                    fprintf(stderr, "[dcs2] READ #%d resp@0x%08X = 0x%08X\n",
                            dcs_read_count, data, resp ? *resp : 0);
                }
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
            case 0x7405:
                /* IOASIC write / DCS command — always return "done" */
                if (resp) *resp = 0x0001;
                ctx->r2 = 0;
                return; /* Return immediately, don't continue to other handling */
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
            case 0x6301: if (resp) *resp = 0x0000; break; /* PIC challenge */
            case 0x6302: if (resp) *resp = 0x0000; break; /* PIC verify */
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
        msg_ctx.r4 = 6;              /* channel = 6 */
        msg_ctx.r5 = (gpr)task_id;   /* value = task_id */
        extern RECOMP_FUNC void func_80145DE0(uint8_t*, recomp_context*);
        func_80145DE0(rdram, &msg_ctx);
    }

    rtos_log_count++;
    if (rtos_log_count <= 3) {
        fprintf(stderr, "[rtos] create_task(%d, func_%08X)\n", task_id, callback);
    } else if (rtos_log_count == 4) {
        fprintf(stderr, "[rtos] create_task ... (suppressing)\n");
    }
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
    if (dispatch_count <= 3) {
        int active = 0;
        for (int i = 0; i < g_scheduler.fiber_count; i++)
            if (g_scheduler.fibers[i].active) active++;
        fprintf(stderr, "[rtos] process_dispatch #%d: %d fibers, %d active\n",
                dispatch_count, g_scheduler.fiber_count, active);
    }
    rtos_sched_run_frame(&g_scheduler, ctx);
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

    /* For high-numbered channels (rendering sync), return immediately.
     * These are waited on from non-fiber code (our test render call). */
    if (channel > 100) {
        /* Store return value at buf_addr */
        if (buf_phys < 0x00800000 - 4)
            *(uint32_t*)(rdram + buf_phys) = 181; /* echo the channel as the value */
        ctx->r2 = 0;
        return;
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
    rtos_sched_wake(&g_scheduler, (int)channel);
    ctx->r2 = 0;
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
    int idx = g_scheduler.current_fiber;
    if (idx >= 0 && idx < g_scheduler.fiber_count) {
        /* Mark done for this frame, yield to scheduler */
        g_scheduler.fibers[idx].blocked = 1;
        rtos_sched_yield(&g_scheduler, -1);
        /* When we resume next frame, we'll re-enter the callback from
         * after func_80144EB8. The infinite loop (pause_self) follows,
         * but pause_self will yield again. */
    }
    ctx->r2 = 0;
}

/* func_80151618: sync/yield function called in the PIC check infinite loop.
 * The loop at 0x800C515C-0x800C5164 calls this repeatedly until
 * the PIC serial at 0x801E6504 matches 528, 469, or 486.
 * We force-set the PIC serial to 486 each call to break the loop. */
int g_yield_counter = 0;
uint32_t g_ram_poll_addr = 0;
int g_ram_poll_count = 0;

RECOMP_FUNC void func_80151618(uint8_t* rdram, recomp_context* ctx) {
    static int c = 0; c++;
    g_yield_counter++;
    *(uint32_t*)(rdram + 0x001E6504) = 486; /* force PIC serial */

    /* Advance frame counters */
    uint32_t* vbl = (uint32_t*)(rdram + 0x001A35CC);
    uint32_t* tick = (uint32_t*)(rdram + 0x001A35C8);
    uint32_t* sync1 = (uint32_t*)(rdram + 0x001A1AB0);
    uint32_t* sync2 = (uint32_t*)(rdram + 0x001A1AB8);
    (*vbl)++;
    (*tick) += 16667;
    (*sync1)++;
    (*sync2) = (*sync1) - 1;

    /* Set DCS/sound ready flag */
    *(uint32_t*)(rdram + 0x001DDDE0) |= 0x04;

    /* If yield is called too many times (infinite loop detection),
     * force-set various flags that polling loops check. */
    if (g_yield_counter > 500) {
        /* Set all common polling flags to break out of loops */
        *(uint32_t*)(rdram + 0x001DDDE0) |= 0xFF;  /* all DCS flags */
        *(uint32_t*)(rdram + 0x002122E0) |= 0x10000; /* zone loaded flag */
        *(uint32_t*)(rdram + 0x001E50AC) = 0;  /* clear loading trigger */
    }

    ctx->r2 = 0;
}

/* Override func_80143A40 (heap malloc) to trace allocation failures.
 * Original renamed to func_80143A40_real in funcs_30.c */
extern RECOMP_FUNC void func_80143A40_real(uint8_t* rdram, recomp_context* ctx);

/* Track the large render buffer allocation for fixing GP */
uint32_t g_render_buffer_addr = 0;

RECOMP_FUNC void func_80143A40(uint8_t* rdram, recomp_context* ctx) {
    uint32_t size = (uint32_t)ctx->r4;
    uint32_t heap_head = *(uint32_t*)(rdram + 0x001A1E90);
    uint32_t hp = heap_head & 0x1FFFFFFF;
    uint32_t free_sz = (hp < 0x00800000 - 4) ? (*(uint32_t*)(rdram + hp) & ~1u) : 0;

    func_80143A40_real(rdram, ctx);

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
extern RECOMP_FUNC void func_80161140_real(uint8_t* rdram, recomp_context* ctx);
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

    func_80161140_real(rdram, ctx);

    if (c <= 3)
        fprintf(stderr, "[pci_drv] func_80161140: r2=0x%08X base=0x%08X render=0x%08X\n",
                (uint32_t)ctx->r2,
                *(uint32_t*)(rdram + 0x001AA660),
                *(uint32_t*)(rdram + 0x0022A454));
}

/* Override func_800F25E0 (CMOS validation) to return success.
 * The original returns error code -3 because our CMOS data doesn't
 * match the expected checksum format. Returning 0 = valid. */
RECOMP_FUNC void func_800F25E0(uint8_t* rdram, recomp_context* ctx) {
    static int c = 0; c++;
    if (c <= 3) fprintf(stderr, "[cmos] Bypassing CMOS validation (returning 0=OK)\n");
    ctx->r2 = 0; /* success - CMOS is valid */
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
    if (gfx_cmd_count <= 100) {
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
    if (call_count == 1) {
        extern recomp_func_t* get_function(int32_t);

        /* Set DCS ready flag so scene functions don't block on polls */
        *(uint32_t*)(rdram + 0x001DDDE0) |= 0x04;

        /* Call the attract mode scene functions in order */
        uint32_t scene_funcs[] = { 0x800CAFD0, 0x800CAF24, 0x800CB19C, 0x800CB31C };
        for (int i = 0; i < 4; i++) {
            recomp_func_t* sfn = get_function((int32_t)scene_funcs[i]);
            if (sfn) {
                fprintf(stderr, "[attract] Calling scene func 0x%08X...\n", scene_funcs[i]);
                ctx->r4 = (gpr)i;
                sfn(rdram, ctx);
                fprintf(stderr, "[attract] Scene func 0x%08X returned\n", scene_funcs[i]);
            }
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
            *(uint32_t*)(rdram + 0x002122E0) &= ~0x10000; /* clear "loaded" flag */
            *(uint32_t*)(rdram + 0x00212354) = 0;          /* clear delay counter */

            /* Force display state so zone parser proceeds */
            *(uint32_t*)(rdram + 0x001E0444) |= 0x44;
            *(uint32_t*)(rdram + 0x001E055C) |= 0x44;

            /* Create test scene nodes */
            {
                /* Position data at 0x005F0000 */
                float* pos = (float*)(rdram + 0x005F0000);
                pos[0] = 100.0f;  /* X */
                pos[1] = 100.0f;  /* Y */
                pos[2] = -500.0f; /* Z (into screen) */

                float* scale = (float*)(rdram + 0x005F0010);
                scale[0] = 50.0f;
                scale[1] = 50.0f;
                scale[2] = 50.0f;

                /* Create multiple nodes at different positions */
                recomp_func_t* create_node = get_function(0x800D7600);
                if (create_node) {
                    for (int n = 0; n < 5; n++) {
                        pos[0] = 50.0f + n * 80.0f;
                        pos[1] = 192.0f;
                        pos[2] = -200.0f - n * 100.0f;

                        uint32_t sp_phys = (uint32_t)ctx->r29 & 0x1FFFFFFF;
                        if (sp_phys >= 0x20 && sp_phys < 0x00800000 - 0x20)
                            *(uint32_t*)(rdram + sp_phys + 0x10) = 1;

                        ctx->r4 = 1;
                        ctx->r5 = (gpr)(int32_t)(0x80000000 | 0x005F0000);
                        ctx->r6 = (gpr)(int32_t)(0x80000000 | 0x005F0010);
                        ctx->r7 = (gpr)0x3F800000; /* 1.0f */
                        create_node(rdram, ctx);
                    }
                    fprintf(stderr, "[attract] Created 5 scene nodes\n");
                }
            }
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
    if (call_count > 1 && call_count < 500 && !zone_done && call_count >= zone_skip_until) {
        extern recomp_func_t* get_function(int32_t);
        static int zone_cmd_count = 0;
        extern int g_yield_counter;  /* declared below in func_80151618 */

        /* Set flags for zone processing */
        *(uint32_t*)(rdram + 0x002122E0) &= ~0x10000;
        *(uint32_t*)(rdram + 0x001E4FF4) = 1;
        *(uint32_t*)(rdram + 0x001E0444) |= 0x44;
        *(uint32_t*)(rdram + 0x001E055C) |= 0x44;
        *(uint32_t*)(rdram + 0x00212354) = 0;
        *(uint32_t*)(rdram + 0x001DDDE0) |= 0x04;

        recomp_func_t* process_zone = get_function(0x8010FE90);
        if (process_zone) {
            uint32_t cfg_before = *(uint32_t*)(rdram + 0x001E0C4C);

            g_yield_counter = 0;

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
            process_zone(rdram, ctx);
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

RECOMP_FUNC void func_8014A488(uint8_t* rdram, recomp_context* ctx) {
    /* Read the registered mode entry from 0x001A25F0 */
    uint32_t entry_ptr = *(uint32_t*)(rdram + 0x001A25F0);
    uint32_t entry_phys = entry_ptr & 0x1FFFFFFF;

    if (entry_phys == 0 || entry_phys + 0x30 >= 0x00800000) {
        return;
    }

    uint32_t mode_func_addr = *(uint32_t*)(rdram + entry_phys + 0x24);
    uint32_t data_buf = *(uint32_t*)(rdram + entry_phys + 0x10);

    static int dispatch_count = 0;
    dispatch_count++;
    if (dispatch_count <= 10 || dispatch_count % 500 == 0) {
        fprintf(stderr, "[state_dispatch] #%d: entry=0x%08X mode_func=0x%08X data=0x%08X\n",
                dispatch_count, entry_ptr, mode_func_addr, data_buf);
    }

    /* Validate mode_func_addr looks like a MIPS virtual address */
    if (mode_func_addr < 0x80000000 || mode_func_addr >= 0x80800000) {
        if (dispatch_count <= 5) {
            fprintf(stderr, "[state_dispatch] Invalid mode_func 0x%08X (not a MIPS vaddr)\n",
                    mode_func_addr);
        }
        return;
    }

    /* Look up and call the mode function */
    recomp_func_t* mode_func = get_function((int32_t)mode_func_addr);
    if (mode_func) {
        ctx->r4 = (gpr)data_buf;
        mode_func(rdram, ctx);
    } else if (dispatch_count <= 5) {
        fprintf(stderr, "[state_dispatch] Mode function 0x%08X not found!\n", mode_func_addr);
    }

    /* After mode function returns, call yield */
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
    /* Cycle the display mode state machine:
     * 0x40 = prepare, 0x20 = render, 0x1000 = swap, 0x2000 = done
     * Alternate between 0x40 (setup) and 0x20 (render) each frame. */
    {
        uint32_t cur_mode = *(uint32_t*)(rdram + 0x002122D4);
        if (cur_mode == 0x40 || cur_mode == 0x2000 || cur_mode == 0) {
            /* After prepare/done, switch to render mode */
            *(uint32_t*)(rdram + 0x002122D4) = 0x20;
        } else if (cur_mode == 0x20) {
            /* After render, keep in render mode (swap happens naturally) */
            *(uint32_t*)(rdram + 0x002122D4) = 0x20;
        }
    }
    *(uint32_t*)(rdram + 0x001DDD80) = 1;    /* voodoo active */
    *(uint32_t*)(rdram + 0x001AA660) = 0x08100000; /* Voodoo PCI base - CRITICAL */
    /* Display channel active flags: 1=active, checked at *(0x1E0490+channel*0x118) */
    *(uint32_t*)(rdram + 0x001E0490) = 1;           /* channel 0 state */
    *(uint32_t*)(rdram + 0x001E0490 + 0x118) = 1;   /* channel 1 state */
    /* Display channel initialized flag at entry[+4], needed by zone parser.
     * func_8010FE90 checks *(0x1E0444) & 0x44 and *(0x1E055C) & 0x44.
     * Value 4 = "rendering active". Set 0x44 = bits 2,6. */
    *(uint32_t*)(rdram + 0x001E0444) |= 0x44;       /* channel 0 display flag */
    *(uint32_t*)(rdram + 0x001E055C) |= 0x44;       /* channel 1 display flag */

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
