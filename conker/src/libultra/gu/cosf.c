#include <ultra64.h>
#include "variables.h"

#pragma GLOBAL_ASM("asm/nonmatchings/libultra/gu/cosf/cosf.s")
/*
 * NON-MATCHING near-miss (-O2 -g3): float-adapted SGI cosf (same algorithm as
 * libreultra/DK64 cosf.c, but single-precision with Conker rodata). ABS control
 * flow matches with `#define ABS(x) ((x) > 0.0f ? (x) : -(x))`, but IDO picks
 * different FP regs ($f14/$f8 vs original $f4/$f6).
 *
 * Closest shape:
 * #define ROUND(d) ((s32)(((d) >= 0.0f) ? ((d) + 0.5f) : ((d) - 0.5f)))
 * #define ABS(x) ((x) > 0.0f ? (x) : -(x))
 * f32 cosf(f32 x) {
 *     f32 absx, dx, xsq, poly, dn;
 *     s32 n, ix, xpt;
 *     ix = *(s32 *)&x;
 *     xpt = (ix >> 22) & 0x1ff;
 *     if (xpt < 0x136) {
 *         absx = ABS(x);
 *         dx = absx;
 *         dn = dx * D_80085EA0 + 0.5f;
 *         n = ROUND(dn);
 *         dn = n;
 *         dn -= 0.5f;
 *         dx = dx - dn * D_80085EA4;
 *         dx = dx - dn * D_80085EA8;
 *         xsq = dx * dx;
 *         poly = ((D_80098D70[4] * xsq + D_80098D70[3]) * xsq + D_80098D70[2]) * xsq
 *                + D_80098D70[1];
 *         if ((n & 1) == 0)
 *             return dx + (dx * xsq) * poly;
 *         return -(dx + (dx * xsq) * poly);
 *     }
 *     if (x != x)
 *         return D_8002C920;
 *     return D_80085EAC;
 * }
 */
