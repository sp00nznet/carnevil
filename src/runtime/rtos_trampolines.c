/* Auto-generated RTOS vector trampolines */
#include "recomp.h"
#include <stdio.h>
#include <string.h>

extern RECOMP_FUNC void rtos_80004E3C(uint8_t* rdram, recomp_context* ctx);
extern RECOMP_FUNC void rtos_80005048(uint8_t* rdram, recomp_context* ctx);
extern RECOMP_FUNC void rtos_800060BC(uint8_t* rdram, recomp_context* ctx);
extern RECOMP_FUNC void rtos_8000619C(uint8_t* rdram, recomp_context* ctx);
extern RECOMP_FUNC void rtos_80006200(uint8_t* rdram, recomp_context* ctx);
extern RECOMP_FUNC void rtos_80006210(uint8_t* rdram, recomp_context* ctx);
extern RECOMP_FUNC void rtos_80006220(uint8_t* rdram, recomp_context* ctx);
extern RECOMP_FUNC void rtos_80006754(uint8_t* rdram, recomp_context* ctx);
extern RECOMP_FUNC void rtos_80009A80(uint8_t* rdram, recomp_context* ctx);
extern RECOMP_FUNC void rtos_80009C9C(uint8_t* rdram, recomp_context* ctx);
extern RECOMP_FUNC void rtos_80009CBC(uint8_t* rdram, recomp_context* ctx);
extern RECOMP_FUNC void rtos_8000B1D0(uint8_t* rdram, recomp_context* ctx);
extern RECOMP_FUNC void rtos_8000B240(uint8_t* rdram, recomp_context* ctx);
extern RECOMP_FUNC void rtos_8000B70C(uint8_t* rdram, recomp_context* ctx);
extern RECOMP_FUNC void rtos_8000BB68(uint8_t* rdram, recomp_context* ctx);
extern RECOMP_FUNC void rtos_8000BCB0(uint8_t* rdram, recomp_context* ctx);
extern RECOMP_FUNC void rtos_8000BE10(uint8_t* rdram, recomp_context* ctx);
extern RECOMP_FUNC void rtos_8000E7D0(uint8_t* rdram, recomp_context* ctx);
extern RECOMP_FUNC void rtos_8000E80C(uint8_t* rdram, recomp_context* ctx);
extern RECOMP_FUNC void rtos_8000E930(uint8_t* rdram, recomp_context* ctx);
extern RECOMP_FUNC void rtos_8000EE30(uint8_t* rdram, recomp_context* ctx);
extern RECOMP_FUNC void rtos_8000F930(uint8_t* rdram, recomp_context* ctx);
extern RECOMP_FUNC void rtos_8000F95C(uint8_t* rdram, recomp_context* ctx);
extern RECOMP_FUNC void rtos_80010368(uint8_t* rdram, recomp_context* ctx);
extern RECOMP_FUNC void rtos_800103BC(uint8_t* rdram, recomp_context* ctx);
extern RECOMP_FUNC void rtos_800112F4(uint8_t* rdram, recomp_context* ctx);
extern RECOMP_FUNC void rtos_80012718(uint8_t* rdram, recomp_context* ctx);

/* vec[3] static_0_800C40A4 -> rtos_8000B1D0 (1 calls) */
RECOMP_FUNC void static_0_800C40A4(uint8_t* rdram, recomp_context* ctx) {
    rtos_8000B1D0(rdram, ctx);
}

/* vec[5] static_0_800C40B4 -> rtos_8000B240 (1 calls) */
RECOMP_FUNC void static_0_800C40B4(uint8_t* rdram, recomp_context* ctx) {
    rtos_8000B240(rdram, ctx);
}

/* vec[7] static_0_800C40C4 -> rtos_8000B70C (1 calls) */
RECOMP_FUNC void static_0_800C40C4(uint8_t* rdram, recomp_context* ctx) {
    rtos_8000B70C(rdram, ctx);
}

/* vec[17] static_0_800C4114 -> rtos_8000BB68 (1 calls) */
RECOMP_FUNC void static_0_800C4114(uint8_t* rdram, recomp_context* ctx) {
    rtos_8000BB68(rdram, ctx);
}

/* vec[18] static_0_800C411C -> rtos_8000BCB0 (4 calls)
 * Device read: a0=device_handle, a1=dest_buf, a2=byte_count
 * For file devices, copy pre-loaded file data into dest buffer. */
RECOMP_FUNC void static_0_800C411C(uint8_t* rdram, recomp_context* ctx) {
    uint32_t dev_id = (uint32_t)ctx->r4;
    uint32_t dest_virt = (uint32_t)ctx->r5;
    uint32_t count = (uint32_t)ctx->r6;
    uint32_t dest_phys = dest_virt & 0x1FFFFFFF;

    /* Check if this is a file device (index 0x10-0x3F) */
    {
        struct file_dev_entry {
            char name[64];
            uint32_t data_phys;
            uint32_t size;
            int active;
        };
        extern struct file_dev_entry g_file_devs[];
        extern int g_file_dev_count;

        if (dev_id >= 0x10 && dev_id < 0x40) {
            int fdev_idx = dev_id - 0x10;
            if (fdev_idx < g_file_dev_count && g_file_devs[fdev_idx].active) {
                uint32_t data_phys = g_file_devs[fdev_idx].data_phys;
                uint32_t file_size = g_file_devs[fdev_idx].size;
                if (count == 0 || count > file_size) count = file_size;

                static int vec18_log = 0;
                vec18_log++;
                if (vec18_log <= 20) {
                    fprintf(stderr, "[vec18_read] \"%s\" -> dest=0x%08X count=%u\n",
                            g_file_devs[fdev_idx].name, dest_virt, count);
                }

                if (dest_phys > 0 && dest_phys + count <= 0x00800000 &&
                    data_phys + count <= 0x00800000 && count > 0) {
                    memcpy(rdram + dest_phys, rdram + data_phys, count);
                }

                ctx->r2 = (gpr)(int32_t)count; /* return bytes read */
                return;
            }
        }
    }

    /* Not a file device - use original RTOS handler */
    rtos_8000BCB0(rdram, ctx);
}

/* vec[24] static_0_800C414C -> rtos_8000BE10 (4 calls)
 * Device read: a0=device_id, a1=dest_buf, a2=byte_count
 * For file devices (opened via device_open), a0 is our descriptor pointer.
 * We intercept these and copy file data directly. */
RECOMP_FUNC void static_0_800C414C(uint8_t* rdram, recomp_context* ctx) {
    uint32_t dev_id = (uint32_t)ctx->r4;
    uint32_t dest_virt = (uint32_t)ctx->r5;
    uint32_t count = (uint32_t)ctx->r6;
    uint32_t dest_phys = dest_virt & 0x1FFFFFFF;

    /* Check if dev_id is one of our file device indices (0x10-0x3F) */
    struct file_dev_entry {
        char name[64];
        uint32_t data_phys;
        uint32_t size;
        int active;
    };
    extern struct file_dev_entry g_file_devs[];
    extern int g_file_dev_count;
    if (dev_id >= 0x10 && dev_id < 0x40) {
        int fdev_idx = dev_id - 0x10;
        if (fdev_idx < g_file_dev_count && g_file_devs[fdev_idx].active) {
            uint32_t data_phys = g_file_devs[fdev_idx].data_phys;
            uint32_t file_size = g_file_devs[fdev_idx].size;
            if (count > file_size) count = file_size;

            static int devread_log = 0;
            devread_log++;
            if (devread_log <= 10) {
                fprintf(stderr, "[dev_read] \"%s\" dest=0x%08X count=%u\n",
                        g_file_devs[fdev_idx].name, dest_virt, count);
            }

            if (dest_phys > 0 && dest_phys + count <= 0x00800000 &&
                data_phys + count <= 0x00800000 && count > 0) {
                memcpy(rdram + dest_phys, rdram + data_phys, count);
            }

            ctx->r2 = 0;
            return;
        }
    }

    /* Not a file device - use original RTOS handler */
    rtos_8000BE10(rdram, ctx);
}

/* vec[27] static_0_800C4164 -> rtos_80005048 (1 calls) */
RECOMP_FUNC void static_0_800C4164(uint8_t* rdram, recomp_context* ctx) {
    rtos_80005048(rdram, ctx);
}

/* vec[28] static_0_800C416C -> rtos_80006754 (1 calls) */
RECOMP_FUNC void static_0_800C416C(uint8_t* rdram, recomp_context* ctx) {
    rtos_80006754(rdram, ctx);
}

/* vec[29] static_0_800C4174 -> rtos_80004E3C (3 calls) */
RECOMP_FUNC void static_0_800C4174(uint8_t* rdram, recomp_context* ctx) {
    rtos_80004E3C(rdram, ctx);
}

/* vec[30] static_0_800C417C -> rtos_8000F95C (20 calls) */
RECOMP_FUNC void static_0_800C417C(uint8_t* rdram, recomp_context* ctx) {
    rtos_8000F95C(rdram, ctx);
}

/* vec[31] static_0_800C4184 -> rtos_8000F930 (42 calls) */
RECOMP_FUNC void static_0_800C4184(uint8_t* rdram, recomp_context* ctx) {
    rtos_8000F930(rdram, ctx);
}

/* vec[33] static_0_800C4194 -> rtos_8000E7D0 (1 calls) */
RECOMP_FUNC void static_0_800C4194(uint8_t* rdram, recomp_context* ctx) {
    rtos_8000E7D0(rdram, ctx);
}

/* vec[34] static_0_800C419C -> rtos_8000E80C (2 calls) */
RECOMP_FUNC void static_0_800C419C(uint8_t* rdram, recomp_context* ctx) {
    rtos_8000E80C(rdram, ctx);
}

/* vec[35] static_0_800C41A4 -> rtos_80010368 (4 calls) */
RECOMP_FUNC void static_0_800C41A4(uint8_t* rdram, recomp_context* ctx) {
    rtos_80010368(rdram, ctx);
}

/* vec[37] static_0_800C41B4 -> rtos_800103BC (2 calls) */
RECOMP_FUNC void static_0_800C41B4(uint8_t* rdram, recomp_context* ctx) {
    rtos_800103BC(rdram, ctx);
}

/* vec[43] static_0_800C41E4 -> rtos_800112F4 (4 calls) */
RECOMP_FUNC void static_0_800C41E4(uint8_t* rdram, recomp_context* ctx) {
    rtos_800112F4(rdram, ctx);
}

/* vec[50] static_0_800C421C -> rtos_80009A80 (1 calls) */
RECOMP_FUNC void static_0_800C421C(uint8_t* rdram, recomp_context* ctx) {
    rtos_80009A80(rdram, ctx);
}

/* vec[51] static_0_800C4224 -> rtos_8000E930 (1 calls) */
RECOMP_FUNC void static_0_800C4224(uint8_t* rdram, recomp_context* ctx) {
    rtos_8000E930(rdram, ctx);
}

/* vec[53] static_0_800C4234 -> rtos_8000EE30 (1 calls) */
RECOMP_FUNC void static_0_800C4234(uint8_t* rdram, recomp_context* ctx) {
    rtos_8000EE30(rdram, ctx);
}

/* vec[55] static_0_800C4244 -> rtos_80009C9C (1 calls) */
RECOMP_FUNC void static_0_800C4244(uint8_t* rdram, recomp_context* ctx) {
    rtos_80009C9C(rdram, ctx);
}

/* vec[56] static_0_800C424C -> rtos_80009CBC (1 calls) */
RECOMP_FUNC void static_0_800C424C(uint8_t* rdram, recomp_context* ctx) {
    rtos_80009CBC(rdram, ctx);
}

/* vec[59] static_0_800C4264 -> rtos_80012718 (4 calls) */
RECOMP_FUNC void static_0_800C4264(uint8_t* rdram, recomp_context* ctx) {
    rtos_80012718(rdram, ctx);
}

/* vec[60] static_0_800C426C -> rtos_800060BC (5 calls) */
RECOMP_FUNC void static_0_800C426C(uint8_t* rdram, recomp_context* ctx) {
    rtos_800060BC(rdram, ctx);
}

/* vec[61] static_0_800C4274 -> rtos_8000619C (2 calls) */
RECOMP_FUNC void static_0_800C4274(uint8_t* rdram, recomp_context* ctx) {
    rtos_8000619C(rdram, ctx);
}

/* vec[62] static_0_800C427C -> rtos_80006200 (1 calls) */
RECOMP_FUNC void static_0_800C427C(uint8_t* rdram, recomp_context* ctx) {
    rtos_80006200(rdram, ctx);
}

/* vec[63] static_0_800C4284 -> rtos_80006210 (1 calls) */
RECOMP_FUNC void static_0_800C4284(uint8_t* rdram, recomp_context* ctx) {
    rtos_80006210(rdram, ctx);
}

/* vec[64] static_0_800C428C -> rtos_80006220 (2 calls) */
extern void rtos_80006220_wrapper(uint8_t* rdram, recomp_context* ctx);
RECOMP_FUNC void static_0_800C428C(uint8_t* rdram, recomp_context* ctx) {
    rtos_80006220_wrapper(rdram, ctx);
}

