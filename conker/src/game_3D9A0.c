#include <ultra64.h>
#include "functions.h"
#include "variables.h"

s32 func_151149AC(u32);

// PARKED zero_temp_sb_vs_move: i=D_80088980&0 fold score 150 len 0x44/0x48 (base $v1).
// 2026-09-18: and0_ptr/split/structp still 150; DKR Jaccard twins are loops, not move+sb.
// ROM: lui/addiu $v1; sw $ra; move $v0,$zero; sb $v0,1/0($v1); sb $zero,2($v1).
// Bare i=0 colors dest $v0 (175). Do not record dummy 825, between_if0f 430, nested
// struct26 335 (nop epilogue), or and0_if0f 410/0x48 (justreg 16/18, $v1+lui remat).
// Banjo midichannel_func_8024AE74 move+$v0 sb is `call()?TRUE:FALSE` phi after jal —
// constant FALSE is sb $zero (func_8024AED8). 1-trip !i loop still 3 extra.
// void func_150104F0(void) {
//     s32 i = D_80088980 & 0;
//     s32 temp_v0;
//
//     D_800D9950.unk1 = i;
//     D_800D9950.unk0 = i;
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
