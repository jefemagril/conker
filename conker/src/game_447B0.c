#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15017300(s16 arg0, s16 arg1) {
    s32 i;
    s16 tmp;

    tmp = arg0;
    bzero(&D_800D2138, 524);
    D_800D2138 = tmp;

    for (i = 0; i < 16; i++) {
        if ((1 << i) & arg1) {
            func_15085710(i, 0, D_80087270[i]);
            D_800D2457 = D_800D2456 = 6;
            func_15085710(i, 5, D_8008726C);
            func_15085710(i, 2, D_80087260);
            if (D_800BE616 != 0) {
                func_15085710(i, 9, D_80087264);
            }
        }
        func_1501748C(arg1);
    }
    D_800D2340 = arg1;
    D_800D2132 = 0;
}

void func_1501748C(s16 arg0) {
}

void func_15017498(void) {
    bzero(&D_800D2138, 524);
}

// NON-MATCHING: schedule 2/46 justreg=11/46 len=0xb8 — IDO wants remat lui+lw of D_800D23C0
// (base in $a1, const 2 in $t3, early blezl) but two loads CSE to address-form ($t5) + $s1 spill.
// Without mid-loop reload, prologue matches (frame 8 / blezl / $t3=2) but size 0xa4 (pointer-walk inner).
// void func_150174C0(s32 arg0) {
//     s32 i, j, off, k, count;
//     u8 *base, *row;
//     u16 val;
//
//     i = 0;
//     if ((s32)D_80087380 <= 0) {
//         return;
//     }
//     base = (u8 *)D_800D23C0;
//     off = 0;
//     row = base;
//     do {
//         count = *(u16 *)(row + 2);
//         j = 0;
//         if (count > 0) {
//             k = 0;
//             do {
//                 val = *(u16 *)(base + (i * 24) + k + 8);
//                 j++;
//                 if ((val >> 12) == 2) {
//                     *(u16 *)(base + (i * 24) + k + 8) = val + arg0;
//                     base = (u8 *)D_800D23C0;
//                     row = base + off;
//                     count = *(u16 *)(row + 2);
//                 }
//                 k += 2;
//             } while (j < count);
//         }
//         i++;
//         off += 0x18;
//         row += 0x18;
//     } while (i < (s32)D_80087380);
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_447B0/func_150174C0.s")

void func_15017578(s32 arg0) {
    u32 tmp = 0;
    D_800D23C0 = func_1502B5C8(&tmp, 3, 12, arg0, 4);
    D_80087380 = tmp / 24;
    func_150174C0(D_800DBF00);
}
