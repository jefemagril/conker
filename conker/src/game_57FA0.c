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
// NON-MATCHING: dump stall — simple fill 0x15C vs 0x184; cleaned m2c remainder/x4 to &D_800C3EA0 compiled 0x34C/0x360.
// ROM: bcopy then rem loop (lw/sw via v0+8) then x4 until &D_800C3EA0. Resume from m2c pointer-chasing, not a simple for.
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
// NON-MATCHING: dump stall — naive 16-slot walk 0x190 vs 0x27C; cleaned m2c memmove rem/x4 to D_800C3D68+0x130 compiled 0x308.
// ROM: bnel miss, rem copy, x4 until D_800C3E98, last slot D_800C3E58 / unkF4=D_800C3D60. Resume m2c unroll, not simple for.
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
// NON-MATCHING: dump stall — simple for 0x128 vs 0x11C; cleaned m2c rem then x4 (load 8/10/18/0, store reverse) compiled 0x284–0x29C.
// ROM remainder: v1 byte-off, a1=base once; x4: sll end=n*8. C remainder/x4 is not IDO's compact unroll. Resume m2c, not simple for.
// s32 func_1502AF04(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
//     s32 src = arg0 + (arg2 * 8), dst = (arg1 + 8) & ~0xF, i, *p;
//     func_10004514(src & ~0xF, (void *) dst, ((src & 0xE) + (arg3 * 8) + 0xF) & ~0xF, 1);
//     p = (s32 *) (dst + (src & 0xF));
//     for (i = 0; i != arg3; i++) p[i * 2] += arg0;
//     return (s32) p;
// }

#pragma GLOBAL_ASM("asm/nonmatchings/game_57FA0/func_1502B020.s")
// NON-MATCHING: dump stall 0xE0 vs 0xF0 — &arg1 gets frame -0x48 and a1-home, but first test is lw 0x4C;beqz not beqz $a1;
// missing dup sw a1,0x4C and stacked more sw 0x38 (IDO keeps 1 in $v0). if(arg1)+&arg1 still lw-first. Two-basin like B8E0.
// s32 *count = &arg1; *count = *count - 1 in loop. First test still lw+beqz.
// void *func_1502B020(s32 *arg0, s32 arg1, s32 arg2, s32 arg3) {
//     s32 sp38 = 1, *cursor = &arg2, *count = &arg1, offset = (s32) &D_AB1950;
//     if (*count != 0) {
//         do {
//             cursor = (s32 *) (((s32) cursor + 3) & ~3) + 1;
//             if (sp38 != 0) offset += func_1502AC88(offset, cursor[-1], &sp38);
//             *count = *count - 1; sp38 &= 0x0FFFFFFF;
//         } while (*count != 0);
//     }
//     if (arg0 != NULL) *arg0 = sp38 & 0x0FFFFFFF;
//     return sp38 == 0 ? NULL : (void *) offset;
// }

#pragma GLOBAL_ASM("asm/nonmatchings/game_57FA0/func_1502B110.s")
// NON-MATCHING: dump stall 0xF8 vs 0x114 — missing early sw $s2 / move $s2,$a0, sltiu $a3,2 + dup sw a3,0x54, s5=-4.
// &arg3 still 7 insns short; same two-basin counter as B8E0 plus AF04 ALIGN epilogue.
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
// NON-MATCHING: dump stall — sz[2] dual sw (0x30/0x34) but frame -0x38 vs ROM -0x30, len 0x144 vs 0x158;
// m2c gotos 0x15C/0x154, often no $s0 save. ROM: dual sw size 0x20/0x2C, save ra+s0, jr/nop epilogue. Not pad-guess.
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
// NON-MATCHING: dump confirmed JUSTREG 70/72 exact 29/72 len 0x120 — sltiu imm 2 vs ROM 1 (words 7,14). True park; do not thrash < 1.
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
// NON-MATCHING: exact 53/61 len 0xF4 — dest/flag slots 0x4C/0x48 vs ROM 0x40/0x38; ALIGN +4 in beqz delay vs before branch.
// s32 func_1502B5C8(s32 *arg0, s32 arg1, s32 arg2, s32 arg3) {
//     s32 sp40, sp38, offset, *cursor, *dest, *count, var_v1;
//     dest = &sp40;
//     if (arg0 != NULL) dest = arg0;
//     *dest = 1;
//     offset = (s32) &D_AB1950;
//     cursor = &arg2;
//     count = &arg1;
//     if (*count != 0) {
//         do {
//             cursor = (s32 *) (((s32) cursor + 3) & ~3) + 1;
//             if (*dest != 0) offset += func_1502AC88(offset, cursor[-1], &sp38);
//             *dest = sp38 & 0x0FFFFFFF;
//             *count = *count - 1;
//         } while (*count != 0);
//     }
//     if (*dest != 0) var_v1 = (s32) func_1502B350(offset, sp38, dest);
//     else var_v1 = 0;
//     return var_v1;
// }

#pragma GLOBAL_ASM("asm/nonmatchings/game_57FA0/func_1502B6BC.s")
// NON-MATCHING: dump stall — &arg3 home: frame -0x50 OK, lw 0x5C;beqz count, dest $s1@0x4C vs ROM $s2@0x44, flag 0x48 vs 0x38,
// ALIGN +4 in beqz delay vs before branch, len 0x138 vs 0x134. Bare arg3: 2/77 justreg 15/77 at 0x134, count in $s1. Same B5C8 slot/schedule.
// s32 *count = &arg3. dest $s1@0x4C vs ROM $s2@0x44.
// s32 func_1502B6BC(s32 *arg0, s32 arg1, s32 *arg2, s32 arg3, s32 arg4) {
//     s32 sp44, sp38, var_s0, *cursor = &arg4, *dest = &sp44, *count = &arg3, temp_v0;
//     if (arg0 != NULL) dest = arg0;
//     *dest = 1; var_s0 = (s32) &D_AB1950;
//     if (*count != 0) {
//         do {
//             cursor = (s32 *) (((s32) cursor + 3) & ~3) + 1;
//             if (*dest != 0) var_s0 += func_1502AC88(var_s0, cursor[-1], &sp38);
//             *dest = sp38 & 0x0FFFFFFF; *count = *count - 1;
//         } while (*count != 0);
//     }
//     if (*dest != 0) {
//         temp_v0 = (s32) func_1502B350(var_s0, sp38, dest);
//         var_s0 = temp_v0;
//         arg1 = (*dest != 0 && temp_v0 != 0) ? func_1502B4A8(temp_v0, arg1) : 0;
//         if (arg2 != NULL) *arg2 = arg1;
//     } else var_s0 = 0;
//     return var_s0;
// }

#pragma GLOBAL_ASM("asm/nonmatchings/game_57FA0/func_1502B7F0.s")
// NON-MATCHING: dump stall 0xE4 vs 0xF0 — &arg1+if(arg1) gets frame -0x48, a1-home, s2=-4; first test lw 0x4C;beqz not beqz $a1;
// more in $v1 not sw 0x38. 5-arg proto in functions.h. Two-basin like B8E0.
// s32 *count = &arg1. Frame -0x48. Still lw-first not beqz $a1.
// s32 func_1502B7F0(s32 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
//     s32 sp38 = 1, sp34, *cursor = &arg2, *count = &arg1, offset = (s32) &D_AB1950;
//     if (*count != 0) {
//         do {
//             cursor = (s32 *) (((s32) cursor + 3) & ~3) + 1;
//             if (sp38 != 0) offset += func_1502AC88(offset, cursor[-1], &sp34);
//             *count = *count - 1; sp38 = sp34 & 0x0FFFFFFF;
//         } while (*count != 0);
//     }
//     *arg0 = sp38 ? (s32) func_1502B350(offset, sp34, &sp38) : 0;
//     return sp38;
// }

#pragma GLOBAL_ASM("asm/nonmatchings/game_57FA0/func_1502B8E0.s")
// NON-MATCHING: length 0xD0 vs 0xD4 — Close dump stall, not ±1 guess.
// ROM: frame -0x48, beqz $a2, delay+extra sw $a2,0x50, loop lw/addiu/sw a2-home, s2=-4.
// IDO basins: (1) m2c below — correct frame, move $s2,$a2, extra $s5, no a2-home.
// (2) s32 *c=&arg2 / volatile arg2 — a2-home loop + s2=-4, but frame -0x50 and first test lw+beqz not beqz $a2.
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
// NON-MATCHING: dump 40/69 justreg 51/69 len 0x114 — unused f32 pad0..pad3 grows frame to ROM -0x68;
// first test still lw 0x68;beqz not beqz $a0 + dup sw a0,0x68. Two-basin counter; pads only fix the frame.
// f32 pad0,pad1,pad2,pad3; s32 *count = &arg0. Frame -0x68. Still lw-first not beqz $a0.
// s32 func_1502B9B4(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
//     f32 pad0, pad1, pad2, pad3;
//     s32 sp54, sp40, sp38, *cursor = &arg1, *count = &arg0, offset = (s32) &D_AB1950, more = 1, *p;
//     if (*count != 0) {
//         do {
//             cursor = (s32 *) (((s32) cursor + 3) & ~3) + 1;
//             if (more != 0) offset += func_1502AC88(offset, cursor[-1], &sp54);
//             *count = *count - 1; more = sp54 & 0x0FFFFFFF;
//         } while (*count != 0);
//     }
//     if (more != 0) {
//         more = ((sp54 & 0x0FFFFFFF) + 1) & ~1;
//         if ((sp54 & 0x70000000) == 0x10000000) {
//             p = &sp38;
//             if ((s32) p & 8) p = &sp40;
//             func_10004514(offset, p, 0x10, 1);
//             more = *p;
//         }
//     }
//     return more;
// }
