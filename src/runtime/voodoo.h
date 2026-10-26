/* 3dfx Voodoo 1 (SST-1) on PCI, as hw.c drives it. Implemented in voodoo_sst1.c. */

#ifndef CARNEVIL_VOODOO_H
#define CARNEVIL_VOODOO_H

#include <stdint.h>

/* Opaque: each implementation defines its own state. */
typedef struct voodoo_state voodoo_state_t;

/* Interface used by hw.c. offset is relative to PCI BAR0 (16 MB window). */
voodoo_state_t* voodoo_create(void);
uint32_t voodoo_read(voodoo_state_t* voodoo, uint32_t offset);
void     voodoo_write(voodoo_state_t* voodoo, uint32_t offset, uint32_t value);
/* Partial write: only the bytes set in mem_mask (16-bit LFB writes). */
void     voodoo_write_masked(voodoo_state_t* voodoo, uint32_t offset, uint32_t value, uint32_t mem_mask);
/* PCI configuration write (byte offset): 0x40 is initEnable. */
void     voodoo_pci_config_write(voodoo_state_t* voodoo, uint32_t offset, uint32_t value);
/* Vertical blank begins (1) / ends (0). */
void     voodoo_vblank(voodoo_state_t* voodoo, int state);
/* The buffer being scanned out: RGB565, *stride pixels per row. */
const uint16_t* voodoo_display(voodoo_state_t* voodoo, int* width, int* height, int* stride);


#endif
