/* Register every function of the recompiled game binary for address lookup. */
#include "hw.h"
#define num_sections game_num_sections
#include "recomp_overlays.inl"

void lookup_add_game(void) {
    for (size_t s = 0; s < ARRLEN(section_table); s++)
        for (size_t i = 0; i < section_table[s].num_funcs; i++)
            lookup_add(section_table[s].ram_addr + section_table[s].funcs[i].offset, section_table[s].funcs[i].func);
}
