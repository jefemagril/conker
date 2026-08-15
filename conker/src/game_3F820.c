#include <ultra64.h>

#include "functions.h"
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_3F820/func_15012370.s")
// NON-MATCHING: guOrtho loop. live &D_80082FA0 / &D_800BE628, s32 n=0x180, Mtx *mtxs.
// u8 i matches frame 0x58 (s4 live) but 0x10c vs 0x100; s32 i remats 0x180 and drops s4 (0x50 vs 0x58).
// void func_15012370(void) {
//     f32 temp_f0;
//     f32 temp_f2;
//     u8 i;
//
//     f32 foo = D_80096560; // 20000.0f
//     f32 **tmp = D_800BE628;
//
//     for (i = 0; i <= D_80082FA0; i++) {
//         temp_f0 = tmp[i][1] * 0.5f;
//         temp_f2 = tmp[i][2] * 0.5f;
//         guOrtho(D_800DCC10[i], -temp_f0, temp_f0, -temp_f2, temp_f2, 1.0f, foo, 1.0f);
//     }
// }

void func_15012470(void) {
    D_80088750 = func_1518AADC(4, 300, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_3F820/func_150124A0.s")
