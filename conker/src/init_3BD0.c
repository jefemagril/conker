#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/* Heap init — single free block from &D_800E9D10 to D_80038098 (BK heap_init analogue).
 * NON-MATCHING: idiomatic C CSEs D_800380B4 into $a0; original reloads via $v1 each
 * field store and keeps &$D_800380BC in $a1 from the prologue. Volatile/tmp reload
 * shapes get the right size/structure but wrong temp regs (v0 vs t7/t1/t2/t3) and
 * miss early a1. Compiler-shape thrash — leave asm.
 *
 * void func_10003BD0(void) {
 *     struct54 * volatile *pB4 = (struct54 * volatile *)&D_800380B4;
 *     s32 **pBC = &D_800380BC;
 *     struct54 *heap = (struct54 *)&D_800E9D10;
 *     struct54 *tmp;
 *
 *     *pB4 = heap;
 *     ((volatile struct54 *)heap)->unk0 = NULL;
 *     tmp = *pB4;
 *     tmp->unk4 = NULL;
 *     tmp = *pB4;
 *     tmp->unk8 = D_80038098 - (u32)heap - 0x14;
 *     tmp = *pB4;
 *     tmp->unkC = 0;
 *     tmp = *pB4;
 *     tmp->unk10 = 0;
 *     tmp = *pB4;
 *     *pBC = (s32 *)tmp;
 *     D_800380B8 = tmp;
 *     D_800380B0 = tmp;
 * }
 */
#pragma GLOBAL_ASM("asm/nonmatchings/init_3BD0/func_10003BD0.s")
