/*
 * Recovered game data structures (via m2c type/struct inference -- see
 * tools/m2c_decompile.py and ghidra_export/entity_m2c.c).
 *
 * Entity dispatch table @ 0x801E3880: a 128-entry array of EntityEntry (stride
 * 0x28), organised as a priority-sorted doubly-linked free/active list. This is
 * the table the attract render handlers register into (see the render-dispatch
 * investigation). m2c recovered the field offsets below from func_800CC218
 * (alloc/insert) and func_800CC47C (process/destroy).
 */
#ifndef CARNEVIL_GAME_STRUCTS_H
#define CARNEVIL_GAME_STRUCTS_H

#include <stdint.h>

#define ENTITY_TABLE_VADDR   0x801E3880u   /* entry[0] */
#define ENTITY_TABLE_PHYS    0x001E3880u
#define ENTITY_STRIDE        0x28u          /* 40 bytes per entry */
#define ENTITY_MAX           0x80u          /* 128 slots (cursor & 0x7F) */

#define ENTITY_HEAD_SENTINEL 0x801E3882u    /* head priority; ==-2 when empty */
#define ENTITY_ALLOC_CURSOR  0x801E6360u    /* rolling free-slot cursor */
#define ENTITY_LIVE_COUNT    0x80212250u    /* alloc if <128; dispatch if >=2;
                                               ++ on alloc, -- on destroy      */
#define ENTITY_CUR_INDEX     0x80212252u

/* Per-entry layout. Free slot is marked by priority == 0xFFFF. The list is
 * sorted by `priority` and linked by `next`/`prev` slot indices (-2 = end). */
typedef struct {
    uint16_t priority;   /* +0x00  sort key; 0xFFFF=free, 0xFFFE=sentinel      */
    uint16_t slot_index; /* +0x02  this entry's own index                      */
    int16_t  next;       /* +0x04  next slot index in priority list (-2=end)   */
    int16_t  prev;       /* +0x06  prev slot index                             */
    uint32_t unk8;       /* +0x08  cleared on alloc (a pointer field)          */
    uint32_t unkC;       /* +0x0C                                              */
    uint32_t handler;    /* +0x10  REGISTERED handler fn ptr (from register a1)*/
    uint32_t unk14;      /* +0x14                                              */
    uint32_t unk18;      /* +0x18  cleared on alloc                            */
    uint32_t unk1C;      /* +0x1C  cleared on alloc                            */
    uint32_t on_process; /* +0x20  per-entry hook(entry,index); cleared on
                                     alloc; called+freed by func_800CC47C      */
    uint32_t unk24;      /* +0x24                                              */
} EntityEntry;

/*
 * Semantics (recovered):
 *   func_800CC1E4(id, handler, a2, a3)  -> build payload, call func_800CC218
 *   func_800CC218 -> func_800CC220      : alloc free slot at cursor, copy
 *                                          payload (handler lands at +0x10),
 *                                          clear +8/+18/+1C/+20, insert into the
 *                                          priority-sorted list, ++LIVE_COUNT.
 *   func_800CC47C(index)                : if LIVE_COUNT>=2 and slot occupied,
 *                                          call entry->on_process (+0x20) if set,
 *                                          unlink from list, free slot (=0xFFFF),
 *                                          --LIVE_COUNT.  (== process + destroy)
 *
 * NOTE: func_800CC47C dispatches the +0x20 hook, NOT the registered +0x10
 * handler. The +0x10 handler is invoked by a separate per-frame list-walker
 * (one of func_800CC728 / func_800CC890 / func_800CCA00), which is the next
 * thread to pull for the render path.
 */

#endif /* CARNEVIL_GAME_STRUCTS_H */
