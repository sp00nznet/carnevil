/* The two types recomp_overlays.inl needs; MidwayRecomp emits it for librecomp. */
#ifndef CARNEVIL_SECTIONS_H
#define CARNEVIL_SECTIONS_H
#include <stddef.h>
typedef struct { recomp_func_t* func; uint32_t offset; uint32_t rom_size; } FuncEntry;
typedef struct {
    uint32_t rom_addr, ram_addr, size;
    FuncEntry* funcs; size_t num_funcs;
    void* relocs; size_t num_relocs;
    size_t index;
} SectionTableEntry;
#define ARRLEN(x) (sizeof(x) / sizeof((x)[0]))
#define nullptr NULL
#endif
