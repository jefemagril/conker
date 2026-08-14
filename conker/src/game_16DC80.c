#include <ultra64.h>

#include "functions.h"
#include "variables.h"

#pragma GLOBAL_ASM("asm/nonmatchings/game_16DC80/func_151407D0.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_16DC80/func_151408A4.s")

void func_151411A4(struct210 *arg0) {
    func_1513CA6C(arg0);
}

void func_151411C4(struct210 *arg0) {
    func_1513CAA0(arg0);
}

void func_1517E134(void *);

void func_151411E4(void *arg0) {
    typedef struct {
        u8 pad0[0x154];
        void * volatile unk154;
        u8 pad158[0x10];
        u8 unk168;
    } Local;
    Local *a = arg0;
    extern s32 D_800DC9F0;
    extern void (*D_80089F9C[])(void *);

    if (a->unk154 != NULL) {
        func_1517E134(a->unk154);
    }
    D_800DC9F0 = D_800DC9F0 - 1;
    D_80089F9C[a->unk168](a);
}


void func_15141250(void *arg0) {
    typedef struct {
        u8 pad0[0x154];
        void * volatile unk154;
        u8 pad158[0x10];
        u8 unk168;
    } Local;
    Local *a = arg0;
    extern s32 D_800DC9F0;
    extern void (*D_80089FE4[])(void *);

    if (a->unk154 != NULL) {
        func_1517E134(a->unk154);
    }
    D_800DC9F0 = D_800DC9F0 - 1;
    D_80089FE4[a->unk168](a);
}


#pragma GLOBAL_ASM("asm/nonmatchings/game_16DC80/func_151412BC.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_16DC80/func_1514143C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_16DC80/func_15141478.s")

s32 func_15141564(void *arg0) {
    typedef struct {
        f32 unk0;
        f32 unk4;
        f32 unk8;
        f32 unkC;
    } Mid;
    typedef struct {
        u8 pad0[0x158];
        f32 unk158;
        u8 pad15C[0x14];
        Mid unk170;
    } Local;
    Local *a = arg0;
    Mid *m = &a->unk170;
    f32 s;

    s = sinf(m->unk8);
    a->unk158 = m->unk0 + (m->unk4 * s);
    m->unk8 = m->unk8 + (m->unkC * D_800BE9A4);
    m->unk8 = func_15144B68(m->unk8);
    return 1;
}


#pragma GLOBAL_ASM("asm/nonmatchings/game_16DC80/func_151415D4.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_16DC80/func_151416E8.s")

// NON-MATCHING: 7/21 at right 0x54 — needs `sw $a0` home then `addiu $a0,$sp,0x1C`
// before `sw $ra`, copy of D_8008A074 via $t6, u8 at 0x24. Array/struct copies
// load the global first. Leave asm.
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

s32 func_1514182C(void *arg0, void *arg1, s32 arg2, s32 arg3, f32 arg4, f32 arg5);

s32 func_15141928(void *arg0) {
    typedef struct {
        f32 unk0;
        f32 unk4;
        f32 unk8;
    } Vec;
    typedef struct {
        u8 pad[0x170];
        s32 unk170;
        s32 unk174;
        Vec *unk178;
        u8 unk17C;
    } Local;
    Local *a = arg0;
    Vec *v0 = a->unk178;

    func_1514182C(a, &a->unk17C, a->unk170, a->unk174, v0->unk0, v0->unk8);
    return 1;
}
