#include <ultra64.h>

#include "functions.h"
#include "variables.h"


typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
} Game16DC80Vec3f;

typedef struct {
    u8 pad0[0x154];
    volatile s32 unk154;
    u8 pad158[0x10];
    u8 unk168;
    u8 pad169[0x7];
    s32 unk170;
    f32 unk174;
    Game16DC80Vec3f *unk178;
    Game16DC80Vec3f unk17C;
} Game16DC80Obj;

f32 func_1514182C(Game16DC80Obj *arg0, Game16DC80Vec3f *arg1, s32 arg2, f32 arg3, f32 arg4, f32 arg5);
void func_1517E134(s32 arg0);
extern s32 D_800DC9F0;
extern void (*D_80089F9C[])(void *);
extern void (*D_80089FE4[])(void *);

#pragma GLOBAL_ASM("asm/nonmatchings/game_16DC80/func_151407D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_16DC80/func_151408A4.s")

void func_151411A4(struct210 *arg0) {
    func_1513CA6C(arg0);
}

void func_151411C4(struct210 *arg0) {
    func_1513CAA0(arg0);
}

void func_151411E4(Game16DC80Obj *arg0) {
    if (arg0->unk154 != 0) {
        func_1517E134(arg0->unk154);
    }
    D_800DC9F0--;
    D_80089F9C[arg0->unk168](arg0);
}

void func_15141250(Game16DC80Obj *arg0) {
    if (arg0->unk154 != 0) {
        func_1517E134(arg0->unk154);
    }
    D_800DC9F0--;
    D_80089FE4[arg0->unk168](arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_16DC80/func_151412BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_16DC80/func_1514143C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_16DC80/func_15141478.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_16DC80/func_15141564.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_16DC80/func_151415D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_16DC80/func_151416E8.s")

// ???
#pragma GLOBAL_ASM("asm/nonmatchings/game_16DC80/func_151417C4.s")

s32 func_15141818(s32 arg0, s32 arg1) {
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_16DC80/func_1514182C.s")
// f32 func_1514182C(void *arg0, void *arg1, s32 arg2, f32 arg3, s32 arg4, s32 arg5) {
//     f32 sp6C;
//     f32 sp68;
//     f32 sp64;
//     ? sp34;
//     f32 temp_f0;
//     f32 temp_f12;
//     f32 temp_f2;
//
//     func_150A8050(&sp34, arg4, 0, arg5);
//     sp64 = arg1->unk0;
//     sp68 = arg1->unk4;
//     sp6C = arg1->unk8;
//     func_150A7960(&sp34, 0, arg2, 0, arg0 + 0x34, arg0 + 0x38, arg0 + 0x3C);
//     temp_f0 = arg0->unk34;
//     temp_f2 = arg0->unk38;
//     temp_f12 = arg0->unk3C;
//     arg0->unk40 = (f32) (temp_f0 + ((temp_f0 - arg1->unk0) * arg3 * 500.0f));
//     arg0->unk44 = (f32) (temp_f2 + ((temp_f2 - arg1->unk4) * arg3 * 500.0f));
//     arg0->unk48 = (f32) (temp_f12 + ((temp_f12 - arg1->unk8) * arg3 * 500.0f));
//     return temp_f0;
// }

s32 func_15141928(Game16DC80Obj *arg0) {
    Game16DC80Vec3f *temp_v0;

    temp_v0 = arg0->unk178;
    func_1514182C(arg0, &arg0->unk17C, arg0->unk170, arg0->unk174, temp_v0->unk0, temp_v0->unk8);
    return 1;
}
