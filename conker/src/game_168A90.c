#include <ultra64.h>

#include "functions.h"
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_168A90/func_1513B5E0.s")
void func_1513B798(void *arg0) {
    typedef struct {
        u8 pad0[0x10];
        u8 unk10;
        s8 unk11;
        u8 pad12[2];
        s16 unk14;
    } Local;
    Local *a0 = arg0;
    u8 ret = 0;
    extern s32 (*D_80089C18[])(void);

    if ((a0->unk10 & 1) != 0) {
        a0->unk14 -= D_800BE9E4;
        if (a0->unk14 < 0) {
            ret = 1;
        }
    }
    if ((ret == 0) && (a0->unk11 != -1) && (D_80089C18[a0->unk11]() == 0)) {
        ret = 1;
    }
    if (ret) {
        func_1516972C(arg0);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_168A90/func_1513B83C.s")

s32 func_1513B968(s32 arg0, s32 arg1) {
    // FIXME: &arg0->unk_120[D_800BE9C0]
    func_150A7B80(arg0 + 120 + (D_800BE9C0 << 6));
    return 1;
}

void func_1513B9A8(struct132 *arg0) {
    func_100043B4(arg0->unk4C, 4);
    func_15169804(arg0);
}

void func_1513B9DC(struct132 *arg0) {
    func_100043B4(arg0->unk4C, 4);
    func_15169824(arg0);
}

void func_1513BA10(struct132 *arg0) {
    D_80089C44[arg0->unk48]();
}

void func_1513BA44(struct132 *arg0) {
    D_80089C54[arg0->unk48]();
}

void func_15109064(struct132 *, s32, u8);
void func_151BA468(struct132 *, s32, u8);

void func_1513BA78(struct132 *arg0, s32 arg1, u8 arg2) {
    switch (arg0->unk48) {
        case 1:
            func_15109064(arg0, arg1, arg2);
            break;
        case 2:
            func_151BA468(arg0, arg1, arg2);
            break;
    }
}


s32 func_1513BAD4(s32 arg0, s32 arg1) {
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_168A90/func_1513BAE8.s")
// NON-MATCHING: 65/69 justreg=65/69 len=0x114  tip spawn_v0_home_20_vs_74. Park.
// Unused s32 pad0 before Pack/Vec puts the pack at 0x38 and the 5-float blob
// at 0x24 (frame 0x78). `if (v0->unk50 != 0x1180) 6972C; else memcpy(v0+unk50+0xF8)`
// matches the beq polarity. The four misses are `sw/lw $v0/$a3, 0x20($sp)` vs
// ROM 0x74 — pad0 coalesces with the jal home. f32/u8/s16 pads and `(void)&v0`
// still home at 0x20; a struct prefix shifts the pack +4. 1-arg 6972C (a1 from
// the beq delay). Do not re-proto 6972C vs functions.h.
// void *func_1513BAE8(void) {
//     typedef struct {
//         u8 unk0, unk1, unk2, pad3;
//         s16 unk4;
//         u8 pad6[2];
//         s32 unk8, unkC, unk10, unk14, unk18, unk1C, unk20;
//         u8 unk24, unk25, pad26[0xA];
//         s32 unk30, unk34;
//         u8 unk38;
//     } Pack;
//     typedef struct {
//         f32 unk0, unk4, unk8, unkC, unk10;
//     } Vec;
//     typedef struct {
//         u8 pad[0x50];
//         s32 unk50;
//     } Obj;
//     void *func_1513B5E0(Pack *, u8, s32, u8, s32);
//     s32 pad0;
//     Pack p;
//     Vec v;
//     Obj *v0;
//
//     p.unk1 = 2;
//     p.unk2 = 5;
//     p.unk4 = 0x12C;
//     p.unk30 = 9;
//     v.unk0 = 0.0f;
//     v.unk4 = 0.0f;
//     v.unkC = 0.0f;
//     v.unk10 = 0.0f;
//     v.unk8 = 0.0f;
//     p.unk0 = 0;
//     p.unk34 = 0x1AE;
//     p.unk8 = 1;
//     p.unkC = 0x220205;
//     p.unk10 = 0x40600;
//     p.unk24 = 0;
//     p.unk25 = 0;
//     p.unk14 = 1;
//     p.unk18 = 0x36;
//     p.unk1C = 0x80;
//     p.unk20 = 0x20;
//     p.unk38 = 3;
//     v0 = func_1513B5E0(&p, 1, 0x14, 0xFF, 1);
//     if (v0 != NULL) {
//         if (v0->unk50 != 0x1180) {
//             func_1516972C(v0);
//         } else {
//             memcpy((u8 *) v0 + v0->unk50 + 0xF8, &v, 0x14);
//         }
//     }
//     return v0;
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_168A90/func_1513BBFC.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_168A90/func_1513BEB0.s")
