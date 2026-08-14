#include <ultra64.h>

#include "functions.h"
#include "variables.h"

#include "macros.h"

s32  allocate_memory(s32, s32, s32, s32);
s32  rzip_uncompress_file(void *, void *, s32);
void func_10004074(void *);
s32  func_1502AC88(s32, s32, s32 *);
s32  func_1502AF04(s32, s32, s32, s32);
s32  func_1502B4A8(s32, s32);
void *func_1502B350(s32, s32, s32 *);

extern u32 D_AB1950;
extern s32 D_800C3D60;
extern s32 D_800C3D68[];
extern s32 D_800C3EA0;

void func_1502AAF0(void) {
}

void func_1502AAF8(s32 arg0) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_57FA0/func_1502AB04.s")
// NON-MATCHING: length 0x15C vs 0x184 — IDO unroll of 16-slot D_800C3D68 fill; tip length_schedule
// void func_1502AB04(s32 arg0, s32 *src, s32 arg2, s32 arg3) {
//     s32 i, *dst, keep = arg0;
//     if (keep != 0) bcopy((u8 *) D_800C3D68 + (keep * 0x10), D_800C3D68, 0x100 - (keep * 0x10));
//     for (i = 0x10 - keep; i < 0x10; i++) {
//         dst = &D_800C3D68[i * 4];
//         dst[2] = src[0]; dst[3] = src[1]; dst[0] = arg3; dst[1] = arg2;
//         src += 2; arg3 += 8;
//     }
// }

#pragma GLOBAL_ASM("asm/nonmatchings/game_57FA0/func_1502AC88.s")
// NON-MATCHING: length 0x190 vs 0x27C — 16×0x10 cache at D_800C3D68, miss DMA + func_1502AB04; tip length_schedule
// s32 func_1502AC88(s32 arg0, s32 arg1, s32 *arg2) {
//     s32 tmp[4], buf[24], *cache = D_800C3D68, *p, key, i, result;
//     key = (arg0 + (arg1 * 8)) | 0x80000000;
//     for (i = 0; i < 0x10; i++) {
//         if (cache[i * 4] == key) {
//             tmp[0] = cache[i*4]; tmp[1] = cache[i*4+1]; tmp[2] = cache[i*4+2]; tmp[3] = cache[i*4+3];
//             for (; i < 0xF; i++) { cache[i*4] = cache[(i+1)*4]; cache[i*4+1] = cache[(i+1)*4+1]; cache[i*4+2] = cache[(i+1)*4+2]; cache[i*4+3] = cache[(i+1)*4+3]; }
//             cache[0xF*4] = tmp[0]; cache[0xF*4+1] = D_800C3D60; cache[0xF*4+2] = tmp[2]; cache[0xF*4+3] = tmp[3];
//             *arg2 = cache[0xF*4+3]; return cache[0xF*4+2];
//         }
//     }
//     D_800C3D60 += 1;
//     p = (s32 *) (((s32) buf + 0xF) & ~0xF);
//     func_10004514(key & 0x7FFFFFF0, p, ((key & 0xE) + 0x1F) & ~0xF, 1);
//     p = (s32 *) ((s32) p + (key & 0xF));
//     result = p[0]; *arg2 = p[1];
//     func_1502AB04(2, p, D_800C3D60, key);
//     return result;
// }

#pragma GLOBAL_ASM("asm/nonmatchings/game_57FA0/func_1502AF04.s")
// NON-MATCHING: length 0x128 vs 0x11C — DMA then p[i*2] += base; IDO unroll is 3 insns long
// s32 func_1502AF04(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
//     s32 src = arg0 + (arg2 * 8), dst = (arg1 + 8) & ~0xF, i, *p;
//     func_10004514(src & ~0xF, (void *) dst, ((src & 0xE) + (arg3 * 8) + 0xF) & ~0xF, 1);
//     p = (s32 *) (dst + (src & 0xF));
//     for (i = 0; i != arg3; i++) p[i * 2] += arg0;
//     return (s32) p;
// }

#pragma GLOBAL_ASM("asm/nonmatchings/game_57FA0/func_1502B020.s")
// NON-MATCHING: length 0xE0 vs 0xF0 — TOC walk; tip arg_counter_sreg_vs_ahome
// void *func_1502B020(s32 *arg0, s32 arg1, s32 arg2, s32 arg3) {
//     s32 sp38 = 1, *var_s0 = &arg2, var_s1 = (s32) &D_AB1950;
//     if (arg1 != 0) {
//         do {
//             var_s0 = (s32 *) (((s32) var_s0 + 3) & ~3) + 1;
//             if (sp38 != 0) var_s1 += func_1502AC88(var_s1, var_s0[-1], &sp38);
//             arg1--; sp38 &= 0x0FFFFFFF;
//         } while (arg1 != 0);
//     }
//     if (arg0 != NULL) *arg0 = sp38 & 0x0FFFFFFF;
//     return sp38 == 0 ? NULL : (void *) var_s1;
// }

#pragma GLOBAL_ASM("asm/nonmatchings/game_57FA0/func_1502B110.s")
// NON-MATCHING: length 0xF8 vs 0x114 — TOC walk then func_1502AF04; tip arg_counter_sreg_vs_ahome
// s32 func_1502B110(s32 *arg0, s32 arg1, s32 arg2, u32 arg3, s32 arg4) {
//     s32 sp3C = 1, sp38 = 0, *var_s0 = &arg4, var_s2 = (s32) arg0;
//     if (var_s2 == 0) var_s2 = (s32) &D_AB1950;
//     if (arg3 >= 2U) {
//         do {
//             var_s0 = (s32 *) (((s32) var_s0 + 3) & ~3) + 1;
//             if (sp3C != 0) var_s2 += func_1502AC88(var_s2, var_s0[-1], &sp3C);
//             arg3--; sp3C &= 0x0FFFFFFF;
//         } while (arg3 >= 2U);
//     }
//     if (sp3C != 0) {
//         var_s0 = (s32 *) (((s32) var_s0 + 3) & ~3) + 1;
//         sp38 = func_1502AF04(var_s2, arg2, var_s0[-1], arg1);
//     }
//     return sp38;
// }

s32 func_1502B224(s32 arg0, void *arg1, s32 arg2, u32 arg3) {
    f32 pad0;
    f32 pad1;
    s32 sp24;
    void *temp_v0;
    u32 var_s1;
    s32 temp_v0_2;

    var_s1 = ((arg2 & 0x0FFFFFFF) + 1) & ~1;
    if ((arg3 != 0) && (arg3 < var_s1)) {
        var_s1 = arg3;
    }
    if ((arg2 & 0x70000000) == 0x10000000) {
        temp_v0 = (void *) allocate_memory(var_s1, 1, 2, 2);
        if (temp_v0 == NULL) {
            return 0;
        }
        func_10004514(arg0, temp_v0, (var_s1 + 0xF) & ~0xF, 1);
        sp24 = *(s32 *) temp_v0 & 0x7FFFFFFF;
        temp_v0_2 = rzip_uncompress_file(temp_v0, arg1, D_8003809C);
        var_s1 = temp_v0_2;
        if (temp_v0_2 != sp24) {
            D_8003C8E0 = 0x0C000036;
            func_150AD770();
        }
        func_10004074(temp_v0);
        return var_s1;
    }
    func_10004514(arg0, arg1, (var_s1 + 0xF) & ~0xF, 1);
    return var_s1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_57FA0/func_1502B350.s")
// NON-MATCHING: length 0x150 vs 0x158 — B224 sibling that allocs dest; sz[2] dual sw, still -2 insns
// void *func_1502B350(s32 arg0, s32 arg1, s32 *arg2) {
//     s32 sz[2]; void *sp28, *var_s0; s32 var_t0, temp_t4;
//     sz[0] = ((arg1 & 0x0FFFFFFF) + 1) & ~1; sz[1] = sz[0];
//     var_s0 = (void *) allocate_memory(sz[0], 1, 2, 2);
//     if (var_s0 == NULL) return NULL;
//     sp28 = var_s0;
//     func_10004514(arg0, var_s0, (sz[0] + 0xF) & ~0xF, 1);
//     var_t0 = sz[1];
//     if ((arg1 & 0x70000000) == 0x10000000) {
//         temp_t4 = *(s32 *) sp28 & 0x7FFFFFFF; *arg2 = temp_t4;
//         var_s0 = NULL; var_t0 = 0;
//         if (temp_t4 != 0 && (u32) temp_t4 < 0xF4240U) {
//             var_s0 = (void *) allocate_memory(temp_t4, 1, 2, 2);
//             var_t0 = var_s0 ? rzip_uncompress_file(sp28, var_s0, D_8003809C) : 0;
//         }
//         func_10004074(sp28);
//     }
//     *arg2 = var_t0; return var_s0;
// }

#pragma GLOBAL_ASM("asm/nonmatchings/game_57FA0/func_1502B4A8.s")
// NON-MATCHING: JUSTREG 70/72 exact 35/72 — sltiu 2 vs 1; tip hibit_and_sltiu_vs_bltz
// typedef struct { s32 ptr; s32 flags; } Rec8;
// s32 func_1502B4A8(Rec8 *arg0, s32 arg1) {
//     Rec8 *var_v1; u32 bit; s32 i, flags;
//     if (arg1 == 0) {
//         var_v1 = (Rec8 *) ((u8 *) arg0 + arg1 * 8);
//         flags = var_v1->flags; var_v1 += 1; arg1 += 1;
//         bit = (u32) flags & 0x80000000;
//         if (bit < 2) {
//             do {
//                 flags = var_v1->flags; arg1 += 1; var_v1 += 1;
//                 bit = (u32) flags & 0x80000000;
//             } while (bit < 2);
//         }
//     }
//     for (i = 0; i < arg1; i++) {
//         arg0[i].flags &= 0x0FFFFFFF;
//         if (arg0[i].ptr == -1 || arg0[i].flags == 0) arg0[i].ptr = 0;
//         else arg0[i].ptr += (s32) arg0;
//     }
//     return arg1;
// }

#pragma GLOBAL_ASM("asm/nonmatchings/game_57FA0/func_1502B5C8.s")
// NON-MATCHING: length 0xF0 vs 0xF4 — TOC walk + B350; tip arg_counter_sreg_vs_ahome
// s32 func_1502B5C8(s32 *arg0, s32 arg1, s32 arg2, s32 arg3) {
//     s32 sp40, sp38, var_s0, *var_s1, *var_s2 = &sp40;
//     if (arg0 != NULL) var_s2 = arg0;
//     *var_s2 = 1; var_s0 = (s32) &D_AB1950; var_s1 = &arg2;
//     if (arg1 != 0) {
//         do {
//             var_s1 = (s32 *) (((s32) var_s1 + 3) & ~3) + 1;
//             if (*var_s2 != 0) var_s0 += func_1502AC88(var_s0, var_s1[-1], &sp38);
//             *var_s2 = sp38 & 0x0FFFFFFF; arg1--;
//         } while (arg1 != 0);
//     }
//     return *var_s2 ? (s32) func_1502B350(var_s0, sp38, var_s2) : 0;
// }

#pragma GLOBAL_ASM("asm/nonmatchings/game_57FA0/func_1502B6BC.s")
// NON-MATCHING: length 0x138 vs 0x134 — TOC walk + B350 + B4A8; one insn long
// s32 func_1502B6BC(s32 *arg0, s32 arg1, s32 *arg2, s32 arg3, s32 arg4) {
//     s32 sp44, sp38, var_s0, *var_s1, *var_s2 = &sp44, temp_v0;
//     if (arg0 != NULL) var_s2 = arg0;
//     *var_s2 = 1; var_s0 = (s32) &D_AB1950; var_s1 = &arg4;
//     if (arg3 != 0) {
//         do {
//             var_s1 = (s32 *) (((s32) var_s1 + 3) & ~3) + 1;
//             if (*var_s2 != 0) var_s0 += func_1502AC88(var_s0, var_s1[-1], &sp38);
//             *var_s2 = sp38 & 0x0FFFFFFF; arg3--;
//         } while (arg3 != 0);
//     }
//     if (*var_s2 != 0) {
//         temp_v0 = (s32) func_1502B350(var_s0, sp38, var_s2);
//         var_s0 = temp_v0;
//         arg1 = (*var_s2 != 0 && temp_v0 != 0) ? func_1502B4A8(temp_v0, arg1) : 0;
//         if (arg2 != NULL) *arg2 = arg1;
//     } else var_s0 = 0;
//     return var_s0;
// }

#pragma GLOBAL_ASM("asm/nonmatchings/game_57FA0/func_1502B7F0.s")
// NON-MATCHING: length 0xE4 vs 0xF0 — B020 clone + B350 epilogue; tip arg_counter_sreg_vs_ahome; 5-arg proto in functions.h
// s32 func_1502B7F0(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
//     s32 sp38 = 1, sp34, *var_s1 = &arg2, var_s0 = (s32) &D_AB1950;
//     if (arg1 != 0) {
//         do {
//             var_s1 = (s32 *) (((s32) var_s1 + 3) & ~3) + 1;
//             if (sp38 != 0) var_s0 += func_1502AC88(var_s0, var_s1[-1], &sp34);
//             arg1--; sp38 = sp34 & 0x0FFFFFFF;
//         } while (arg1 != 0);
//     }
//     *arg0 = sp38 ? (s32) func_1502B350(var_s0, sp34, &sp38) : 0;
//     return sp38;
// }

#pragma GLOBAL_ASM("asm/nonmatchings/game_57FA0/func_1502B8E0.s")
// NON-MATCHING: length 0xD0 vs 0xD4 — TOC walk + B224; tip arg_counter_sreg_vs_ahome
// s32 func_1502B8E0(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
//     s32 sp34, var_s0 = (s32) &D_AB1950, *var_s1 = &arg3, more = 1, temp;
//     if (arg2 != 0) {
//         do {
//             var_s1 = (s32 *) (((s32) var_s1 + 3) & ~3) + 1;
//             if (more != 0) var_s0 += func_1502AC88(var_s0, var_s1[-1], &sp34);
//             temp = arg2 - 1; arg2 = temp; more = sp34 & 0x0FFFFFFF;
//         } while (temp != 0);
//     }
//     if (more != 0) more = func_1502B224(var_s0, (void *) arg0, sp34, arg1);
//     return more;
// }

#pragma GLOBAL_ASM("asm/nonmatchings/game_57FA0/func_1502B9B4.s")
// NON-MATCHING: exact 4/69 len 0x114 — TOC walk + optional header DMA
// s32 func_1502B9B4(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
//     s32 sp54, sp40, sp38, *tmp = &arg1, offset = (s32) &D_AB1950, more = 1;
//     for (; arg0 != 0; arg0--) {
//         tmp = (s32 *) (((s32) tmp + 3) & ~3) + 1;
//         if (more != 0) offset += func_1502AC88(offset, tmp[-1], &sp54);
//         more = sp54 & 0x0FFFFFFF;
//     }
//     if (more != 0) {
//         more = ((sp54 & 0x0FFFFFFF) + 1) & ~1;
//         if ((sp54 & 0x70000000) == 0x10000000) {
//             s32 *p = &sp38;
//             if ((s32) p & 8) p = &sp40;
//             func_10004514(offset, p, 0x10, 1);
//             more = *p;
//         }
//     }
//     return more;
// }
