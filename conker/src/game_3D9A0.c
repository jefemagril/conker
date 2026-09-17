#include <ultra64.h>
#include "functions.h"
#include "variables.h"

s32 func_151149AC(u32);

// PARKED zero_temp_sb_vs_move: exact -1/18 justreg -1/18 len 0x44/0x48 score 175
// ROM: lui/addiu $v1=&D_800D9950; move $v0,$zero; sb $v0,1/0($v1); sb $zero,2($v1).
// Proven-zero temps fold to sb $zero with base in $v0 (4 short). Pointer+temp, chain,
// !1, &p&0 all still fold. dummy_label is length-equal 0x48 but wrong 3x lui/sb $at shape
// (score 825) — do not thrash volatile/dummy remat.
// void func_150104F0(void) {
//     s32 temp_v0 = 0;
//
//     D_800D9950.unk1 = temp_v0;
//     D_800D9950.unk0 = temp_v0;
//     D_800D9950.unk2 = 0;
//     temp_v0 = func_151149AC(0xF6);
//     *(f32 *)(temp_v0 + 0x7C) = 2.0f;
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
