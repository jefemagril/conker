#include <ultra64.h>

#pragma GLOBAL_ASM("asm/nonmatchings/libultra/gu/guMtxF2L/guMtxF2L.s")
/*
 * NON-MATCHING: Conker asm is fully unrolled handwritten-style halfword stores
 * (cvt.w.s + srl + sh), not the libreultra/PD loop with packed word stores.
 * Semantically: FTOFIX32 each float, write hi/lo halves into Mtx int/frac parts.
 */
