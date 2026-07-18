#include <ultra64.h>

#pragma GLOBAL_ASM("asm/nonmatchings/libultra/gu/guMtxIdentF/guMtxIdentF.s")
/*
 * NON-MATCHING near-miss (-O2 -g3): store order and int-zero stores match, but
 * IDO emits $f0 and puts the final swc1 before jr; original uses $f4 with the
 * final swc1 in the jr delay slot.
 *
 * Closest shape:
 * void guMtxIdentF(f32 mf[4][4]) {
 *     if (0) {}
 *     *(s32 *)&mf[0][1] = 0;
 *     mf[0][0] = 1.0f;
 *     dummy_label:;
 *     *(s32 *)&mf[0][2] = 0;
 *     *(s32 *)&mf[0][3] = 0;
 *     *(s32 *)&mf[1][0] = 0;
 *     mf[1][1] = 1.0f;
 *     *(s32 *)&mf[1][2] = 0;
 *     *(s32 *)&mf[1][3] = 0;
 *     *(s32 *)&mf[2][0] = 0;
 *     *(s32 *)&mf[2][1] = 0;
 *     mf[2][2] = 1.0f;
 *     *(s32 *)&mf[2][3] = 0;
 *     *(s32 *)&mf[3][0] = 0;
 *     *(s32 *)&mf[3][1] = 0;
 *     *(s32 *)&mf[3][2] = 0;
 *     mf[3][3] = 1.0f;
 * }
 */
