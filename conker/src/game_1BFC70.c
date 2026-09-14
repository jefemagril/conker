#include <ultra64.h>
#include "functions.h"
#include "variables.h"


// NON-MATCHING: exact 13/15 justreg 15/15 len 0x3c/0x3c score 10
// permuter-plateau: 85 -> 10; remaining addu/sw $t2 vs ROM $t3
// void func_151927C0(s8 *arg0) {
//     int new_var;
//     new_var = (s16) (((s32) ((*((s32 *) (((s8 *) arg0) + 0x14))) & 0xFF)) >> 1);
//     if (1) {
//         *((s16 *) (((s8 *) arg0) + 0x38)) = (float) 0x12C;
//         *((s8 *) (((s8 *) arg0) + 0x3A)) = 0xA;
//     }
//     *((s32 *) (((s8 *) arg0) + 0x14)) = (s32) (new_var * 0x10001);
//     *((s8 *) (((s8 *) arg0) + 0x3B)) = 0;
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_1BFC70/func_151927C0.s")
