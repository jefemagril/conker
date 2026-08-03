#include <ultra64.h>
#include "functions.h"
#include "variables.h"

// loops and loops

// NON-MATCHING: JUSTREG 37/48 — tip justreg_park; IDO -O2 unroll matches opcodes, regs only (a2/a3/t0)
// void func_15008870(s32 arg0) {
//     s32 start = 0;
//     s32 end = 0x68;
//     s32 i;
//     s32 j;
//
//     if (arg0 == 1) {
//         end = 0x65;
//     } else if (arg0 == 2) {
//         start = 0x65;
//     }
//     for (i = 0; i < 2; i++) {
//         for (j = start; j < end; j++) {
//             *(&D_800DCE50[0] + i * 0x68 + j) = 0;
//         }
//     }
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_35D20/func_15008870.s")

void func_15008930(s32 arg0) {
    void (**tbl)(void);
    s32 i;
    s32 count;

    if (arg0 == 1) {
        tbl = &D_80082BD0;
        count = 1;
    } else if (arg0 == 2) {
        tbl = &D_80082BD4;
        count = 1;
    } else {
        count = 0;
    }
    for (i = 0; i < count; i++) {
        tbl[i]();
    }
    D_800DD1B0 = -1;
}
