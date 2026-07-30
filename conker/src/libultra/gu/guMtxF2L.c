#include <ultra64.h>

#pragma GLOBAL_ASM("asm/nonmatchings/libultra/gu/guMtxF2L/guMtxF2L.s")
/*
 * NON-MATCHING: Conker asm is fully unrolled handwritten-style halfword stores
 * (cvt.w.s + srl + sh with pair-interleaved mul/cvt), not the libreultra/PD loop
 * with packed word stores. Semantically: FTOFIX32 each float, write hi/lo halves
 * into Mtx int/frac parts. Matched twin is guMtxF2L2 (trunc.w.s + packed sw).
 *
 * Literal unrolled C with s16 ai/af stores at -O2/-O3 emits trunc.w.s + sra and
 * a different mul/cvt interleave ($f0 scale, both muls then both truncs) — cannot
 * match the original cvt.w.s/$f18/srl schedule from IDO.
 */
