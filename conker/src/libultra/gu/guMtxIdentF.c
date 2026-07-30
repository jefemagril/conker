#include <ultra64.h>

#pragma GLOBAL_ASM("asm/nonmatchings/libultra/gu/guMtxIdentF/guMtxIdentF.s")
/*
 * NON-MATCHING near-miss: interleaved int-zero / float-one store order matches
 * at -O2 -g3 (with if(0){} / label so the first sw fills the mtc1 delay), but
 * IDO always uses $f0 and places the final swc1 before jr. Plain -O2 puts the
 * final swc1 in the jr delay slot (matching original) but groups all zero
 * stores first. -O1 can emit $f4 but duplicates mtc1 per diagonal and groups
 * zeros. Could not get $f4 + interleaved stores + delay-slot swc1 together;
 * original likely handwritten-asm schedule.
 *
 * Closest shape (-O2 -g3):
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
