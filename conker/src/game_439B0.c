#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15016500(void) {
    D_800D1940 = (u8)0;
    D_800D1941 = (u8)0;
    D_800D1944 = 0;
    D_800D1948 = 0;
    D_800D194C = 0;
    D_800D1950 = 0;
    bzero(D_800D1958, 48); // bzero
    D_800D1988 = 0.0f;
    D_800D198C = 0.0f;
    D_800D1990 = 0.0f;
    D_800D1994 = (u8)0;
    D_800D1995 = (u8)0;
    D_800D1998 = 0;
}

// grim looking loop
#pragma GLOBAL_ASM("asm/nonmatchings/game_439B0/func_15016588.s")
// NON-MATCHING: 53/55 justreg=53/55 len=0xdc  tip lui_lo_vs_zero_fill
// After bzero, ROM is `lui $s5,%hi(D_800BEAAB); addiu $s1,-1; addiu $s5,%lo; or $s0,$zero`.
// IDO fills `or $s0,$zero` (`i=0`) before completing the lo. Named `u8 *flag` drops `$s6`
// (0xd0 vs 0xdc). `u8 *buf = D_800BE580` grows the frame 0x50→0x58. Comma/`(addr, 0)` no-ops.
// void func_15016588(void) {
//     extern u8 D_800BE580[];
//     extern void *func_1502B020(s32 *, s32, s32, u8, s32);
//     s32 i, byte, bit, out;
//
//     bzero(D_800BE580, 8);
//     byte = -1;
//     i = 0;
//     do {
//         if ((i & 7) == 0) {
//             bit = 1;
//             byte++;
//         } else {
//             bit <<= 1;
//         }
//         func_1502B020(&out, 3, 0x1A, D_800BEAAB, i);
//         i++;
//         if (out != 0) {
//             D_800BE580[byte] |= bit;
//         }
//     } while (i != 0x43);
// }
