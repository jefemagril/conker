#include <ultra64.h>
#include "functions.h"
#include "variables.h"

s32 func_151149AC(u32);

void func_150104F0(void) {
    typedef struct {
        u8 unk[3];
    } Col;
    Col *c = (Col *)&D_800D9950;

    c->unk[0] = c->unk[1] = c->unk[2] = 0;
    *(f32 *)(func_151149AC(0xF6) + 0x7C) = 2.0f;
    D_80088980 = 0;
}


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
