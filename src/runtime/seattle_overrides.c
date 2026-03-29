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

#define MAX_RTOS_FILES 8
static struct {
    int active;
    char name[64];
    const uint8_t* data;
    uint32_t size;
    uint32_t pos;
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
    for (int i = 0; i < MAX_RTOS_FILES; i++) {
        if (!rtos_files[i].active) {
            rtos_files[i].active = 1;
            strncpy(rtos_files[i].name, name, 63);
            rtos_files[i].pos = 0;

            if (strcmp(name, "game.env") == 0 || strcmp(name, "GAME.ENV") == 0) {
                rtos_files[i].data = (const uint8_t*)game_env_data;
                rtos_files[i].size = sizeof(game_env_data);
            } else {
                /* Unknown file - try loading from extracted/files/ */
                char path[256];
                snprintf(path, sizeof(path), "extracted/files/%s", name);
                FILE* f = fopen(path, "rb");
                if (f) {
                    fseek(f, 0, SEEK_END);
                    long sz = ftell(f);
                    fseek(f, 0, SEEK_SET);
                    uint8_t* buf = (uint8_t*)malloc(sz);
                    if (buf) {
                        fread(buf, 1, sz, f);
                        rtos_files[i].data = buf;
                        rtos_files[i].size = (uint32_t)sz;
                    }
                    fclose(f);
                    fprintf(stderr, "[file] Loaded '%s' from disk (%ld bytes)\n", path, sz);
                } else {
                    rtos_files[i].data = NULL;
                    rtos_files[i].size = 0;
                    fprintf(stderr, "[file] '%s' not found on disk\n", name);
                }
            }
            return i + 1; /* handle = 1-based index */
        }
    }
    return -1;
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

/* func_80142C58: RTOS fopen(device_ptr, mode_string)
 * Returns file handle (pointer) or 0 on error.
 * Called after device_open to open a file for reading. */
RECOMP_FUNC void func_80142C58(uint8_t* rdram, recomp_context* ctx) {
    uint32_t dev_ptr = (uint32_t)ctx->r4;
    uint32_t mode_phys = (uint32_t)ctx->r5 & 0x1FFFFFFF;
    char mode[16] = {0};
    if (mode_phys < 0x00800000 - 16) {
        for (int i = 0; i < 15; i++) {
            char c = rdram[mode_phys + i];
            if (!c) break;
            mode[i] = c;
        }
    }
    fprintf(stderr, "[file] fopen(dev=0x%08X, mode=\"%s\") -> handle\n", dev_ptr, mode);
    /* Return a non-zero "handle" (just use the device ptr as the handle) */
    ctx->r2 = dev_ptr ? dev_ptr : 1;
}

/* func_80142F10: RTOS fread(dest_buf, element_size, count_or_size, handle)
 * Reads data from an opened file into the destination buffer.
 * Returns number of bytes read. */
RECOMP_FUNC void func_80142F10(uint8_t* rdram, recomp_context* ctx) {
    uint32_t dest_virt = (uint32_t)ctx->r4;
    uint32_t dest_phys = dest_virt & 0x1FFFFFFF;
    uint32_t elem_size = (uint32_t)ctx->r5;
    uint32_t count = (uint32_t)ctx->r6;
    uint32_t total = elem_size * count;

    /* Find which file is open - check all handles */
    int bytes_read = 0;
    for (int i = 0; i < MAX_RTOS_FILES; i++) {
        if (rtos_files[i].active && rtos_files[i].data) {
            if (dest_phys < 0x00800000 - total && total > 0) {
                bytes_read = rtos_file_read(i + 1, rdram + dest_phys, total);
                if (bytes_read > 0) break;
            }
        }
    }

    fprintf(stderr, "[file] fread(dest=0x%08X, %u, %u, total=%u) -> %d bytes\n",
            dest_virt, elem_size, count, total, bytes_read);

    ctx->r2 = (gpr)(int32_t)(bytes_read > 0 ? bytes_read : total);
}

/* func_80142850: RTOS fclose(handle) */
RECOMP_FUNC void func_80142850(uint8_t* rdram, recomp_context* ctx) {
    uint32_t handle = (uint32_t)ctx->r4;
    fprintf(stderr, "[file] fclose(0x%08X)\n", handle);

    /* Mark all matching files as closed */
    for (int i = 0; i < MAX_RTOS_FILES; i++) {
        if (rtos_files[i].active) {
            rtos_files[i].active = 0;
        }
    }
    ctx->r2 = 0;
}

/* func_80143E74: RTOS free(ptr) - from heap */
RECOMP_FUNC void func_80143E74(uint8_t* rdram, recomp_context* ctx) {
    /* Just ignore frees for now */
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
            /* [+0] = device ID */
            *(uint32_t*)(rdram + entry + 0) = dev_idx;
            /* [+16] = flags (bit 2 set = active) */
            *(uint32_t*)(rdram + entry + 16) = 0x00000004;

            /* [+24] = handler table pointer.
             * vec[25] does: lw handler, 24(entry); lw func, 24(handler); jalr func
             * We need a fake handler struct in RAM with a function pointer at offset 24.
             * Use a region at 0x6F0000+ for these fake handler structs. */
            uint32_t handler_phys = 0x6F0000 + dev_idx * 64;
            uint32_t handler_virt = 0x806F0000 + dev_idx * 64;
            memset(rdram + handler_phys, 0, 64);

            /* Put a "PCI device handler" function pointer at handler[+24].
             * We'll use a known function address that we can override.
             * Use 0x800C40A4 (vec[3]) which we have a trampoline for. */
            /* Actually, the handler func gets called via jalr which means
             * LOOKUP_FUNC is used. Let's use a known registered address. */
            *(uint32_t*)(rdram + handler_phys + 24) = 0x800C40A4; /* vec[3] = RTOS render func */

            *(uint32_t*)(rdram + entry + 24) = handler_virt;
            fprintf(stderr, "[rtos] Registered device \"%s\" at index %d, handler at 0x%08X\n",
                    devname, dev_idx, handler_virt);
        }
    }

    /* Return the device handle (1-based index or positive value) */
    ctx->r2 = (dev_idx >= 0) ? (gpr)(dev_idx + 1) : 1;
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

    if ((cmd & 0xFF00) == 0x6900) {
        /* DCS2 Sound commands */
        switch (cmd) {
            case 0x6900: if (resp) *resp = 0x0000; break; /* Sound status */
            case 0x6901: if (resp) *resp = 0x0003; break; /* DCS version */
            case 0x6902: if (resp) *resp = 0x0001; break; /* Sound ready */
            case 0x6903: if (resp) *resp = 0x0001; break; /* Sound OK */
            case 0x6905: break;                             /* Sound command (play) */
            case 0x6907: if (resp) *resp = 0x0000; break; /* Volume */
            case 0x690E: if (resp) *resp = 0x0000; break; /* Sound test */
            case 0x690F: break;                             /* Sound data */
            case 0x6910: if (resp) *resp = 0x0000; break; /* Sound param */
            default: if (resp) *resp = 0x0000; break;
        }
    } else if ((cmd & 0xFF00) == 0x7400 || (cmd & 0xFF00) == 0x7000 || (cmd & 0xFF00) == 0x6300) {
        /* IOASIC / PIC / system commands */
        switch (cmd) {
            case 0x7000: if (resp) *resp = 0x0001; break; /* System OK */
            case 0x7001: if (resp) *resp = 0x0001; break; /* IOASIC version */
            case 0x740B: if (resp) *resp = 0x0000; break; /* PIC status */
            case 0x7403: if (resp) *resp = 0x0000; break; /* PIC query */
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
        /* Unknown command - log it */
        if (dev_io_count <= 50) {
            fprintf(stderr, "[dev_io] UNKNOWN cmd=0x%04X dev=%d data=0x%08X\n", cmd, dev_id, data);
        }
        if (resp) *resp = 0x0000;
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

    if (rtos_event_counter <= 5) {
        fprintf(stderr, "[rtos] event_wait(channel=%d) -> counter=%u\n", channel, rtos_event_counter);
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
    if (msg_send_count <= 5) {
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

/* func_80145020: yield timeslice */
RECOMP_FUNC void func_80145020(uint8_t* rdram, recomp_context* ctx) {
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
