#include <ultra64.h>
#include "functions.h"
#include "variables.h"

s32 func_151149AC(u32);

// NON-MATCHING: length 0x44/0x48 — IDO -O2 folds zero-temps to sb $zero with
// base in $v0; target wants move $v0,$zero + base in $v1 then sb; tip length_schedule
// void func_150104F0(void) {
//     s32 temp_v0;
//     u8 *p;
//
//     p = &D_800D9950.unk0;
//     temp_v0 = 0;
//     p[1] = temp_v0;
//     p[0] = temp_v0;
//     p[2] = 0;
//     temp_v0 = func_151149AC(0xF6);
//     *(f32 *)((u8 *)temp_v0 + 0x7C) = 2.0f;
//     D_80088980 = 0;
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_3D9A0/func_150104F0.s")

void func_15010538(struct127 *arg0) {
    struct175 tmp;
    struct37 *temp_v0;

    func_15161E24(arg0, 2, 2, 300, 30, 100, 200, 255, 255, 1);
// FAKEMATCH but works...
dummy_label_927029:
    tmp.unk0 = arg0;
    tmp.unk4 = arg0->unique_id;
    tmp.unk6 = 0;
    tmp.unk8 = 0;
    tmp.unkA = 0;

    temp_v0 = func_15149130(300, -1, 80, -1, 0, 61, 12, 255, 1);
    if (temp_v0 != NULL) {
        memcpy(&temp_v0->unk28, &tmp, 12); // memcpy
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_3D9A0/func_15010600.s")
// NON-MATCHING: addresses are wrong :(
// void func_15010600(void) {
//     s32 i;
//
//     for (i = 0; i < 11; i++) {
//         D_800D9930[i] = D_800D9920[i] = 0;
//     }
// }
