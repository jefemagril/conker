#include <ultra64.h>

#include "functions.h"
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_EF410/func_150C1F60.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_EF410/func_150C2290.s")
// TODO: figure out this struct!
// void func_150C2290(s32 arg0) {
//     s16 sp9A;
//     s16 sp98;
//     s8 sp95;
//     s8 sp94;
//     ?32 sp90;
//     ?32 sp8C;
//     ?32 sp88;
//     ?32 sp84;
//     ?32 sp80;
//     ?32 sp7C;
//     ?32 sp78;
//     ?32 sp74;
//     ?32 sp70;
//     s8 sp6E;
//     s8 sp6D;
//     s8 sp6C;
//     ?32 sp68;
//     ?32 sp64;
//     ?32 sp60;
//     ?32 sp5C;
//     ?32 sp58;
//     ?32 sp54;
//     s8 sp52;
//     s8 sp51;
//     s8 sp50;
//     s16 sp4E;
//     s16 sp4C;
//     s16 sp4A;
//     s16 sp48;
//     s16 sp46;
//     s16 sp44;
//     s16 sp42;
//     s16 sp40;
//     s16 sp3E;
//     s16 sp3C;
//     f32 sp38;
//     f32 sp34;
//     f32 sp30;
//     f32 sp2C;
//     f32 sp28;
//     f32 sp24;
//     f32 sp20;
//     f32 sp1C;
//     f32 sp18;
//
//     sp18 = *(void *)0x800A0258;
//     sp1C = 40.0f;
//     sp3E = 3;
//     sp40 = -0x29;
//     sp44 = -0x16;
//     sp42 = 0x15;
//     sp46 = 0x16;
//     sp48 = 7;
//     sp4E = 0x15;
//     sp50 = 0xB;
//     sp20 = *(void *)0x800A025C;
//     sp3C = 2;
//     sp4C = 0x30;
//     sp52 = 0x28;
//     sp54 = 1;
//     sp58 = 4;
//     sp6C = 0xFF;
//     sp6D = 0xFF;
//     sp70 = 3;
//     sp4A = 0;
//     sp51 = 1;
//     sp5C = 0;
//     sp60 = 0;
//     sp64 = 0;
//     sp68 = 0;
//     sp6E = 0;
//     sp74 = 0xFF;
//     sp78 = 0;
//     sp7C = 0x220005;
//     sp80 = 0x1D0600;
//     sp84 = (?32) (u8)1;
//     sp88 = 0x3B;
//     sp8C = 0x80;
//     sp90 = 0x20;
//     sp94 = 0;
//     sp95 = 7;
//     sp98 = 0xC;
//     sp9A = 0x15;
//     sp34 = 100.0f;
//     sp38 = 57.5f;
//     sp24 = *(void *)0x800A0260;
//     sp28 = 39.0f;
//     sp2C = *(void *)0x800A0264;
//     sp30 = 1.0f;
//     func_15151A38(&sp18, arg0 & 0xFF, 1);
// }

void func_1514C470(f32, f32, f32, f32, f32, f32, f32, s32, s32, f32, s32, u8);
void func_150C2424(u8 arg0);

void func_150C2424(u8 arg0) {
    f32 *p = &D_800A0268;

    func_1514C470(
        *p,
        -490.0f,
        -328.0f,
        *p,
        -490.0f,
        328.0f,
        (func_150ADA68() * 8.0f) + 8.0f,
        1,
        0,
        0.0f,
        0,
        arg0
    );
    func_1514C470(
        D_800A026C,
        -560.0f,
        -580.0f,
        8117.0f,
        -560.0f,
        -580.0f,
        (func_150ADA68() * 3.0f) + 4.0f,
        3,
        0,
        0.0f,
        0,
        arg0
    );
}

s32 func_150C251C(void *arg0) {
    typedef struct {
        u8 pad0[0x1C];
        s16 unk1C;
        u8 pad1E[0x7A];
        void *unk98;
    } Local;
    typedef struct {
        u8 pad[0x1B];
        u8 unk1B;
    } Inner;
    Local *a = arg0;
    Inner *p = a->unk98;
    s32 v = a->unk1C * 8;

    if (v >= 0x100) {
        v = 0xFF;
    }
    p->unk1B = v;
    if ((u8)v < 0) {
        return 0;
    }
    return 1;
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_EF410/func_150C2558.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_EF410/func_150C2700.s")
// NON-MATCHING: 1 insn long (0x108 vs 0x104). Park. Frame 0x68, ra 0x1C,
// pack at 0x20, a1 = &p.unk8, lbu 0xA3 from a 15-arg proto (dummy s32s),
// jal delay swc1 D_800A027C. Extra is an s16 `li` in the lui D_800A027C
// delay vs ROM `lwc1` arg4. Comma / delaying unk14=6 / CSE of 6 for
// unk14+unk39 still hoist the next s16 immediate. tip s16_li_hoist_vs_lwc1.
// s32 func_150C2700(s32 arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, s32 a5, s32 a6, s32 a7, s32 a8, s32 a9, s32 a10, s32 a11, s32 a12, s32 a13, u8 arg5) {
//     typedef struct {
//         s16 unk0, unk2, unk4, unk6;
//         f32 unk8, unkC, unk10;
//         s16 unk14, unk16;
//         f32 unk18, unk1C;
//         s16 unk20, unk22;
//         f32 unk24, unk28;
//         u8 unk2C, unk2D, pad2E[2];
//         f32 unk30, unk34;
//         u8 unk38, unk39, pad3A[2];
//         f32 unk3C;
//         u8 unk40, pad41[3];
//         f32 unk44;
//     } Pack;
//     void func_15150178(void *, void *, s32, u8, s32);
//     extern f32 D_800A0274;
//     extern f32 D_800A0278;
//     extern f32 D_800A027C;
//     Pack p;
//
//     p.unk18 = 30.0f;
//     p.unk1C = 35.0f;
//     p.unk24 = D_800A0274;
//     p.unk28 = D_800A0278;
//     p.unk16 = 5;
//     p.unk0 = 0x6B;
//     p.unk10 = arg4;
//     p.unk30 = D_800A027C;
//     p.unk8 = arg2;
//     p.unkC = arg3;
//     p.unk2 = 0x46;
//     p.unk4 = -0x1F;
//     p.unk6 = 0x2E;
//     p.unk14 = 6;
//     p.unk20 = 0x64;
//     p.unk22 = 0;
//     p.unk2C = 0x9B;
//     p.unk2D = 0x64;
//     p.unk34 = 0.0f;
//     p.unk38 = 1;
//     p.unk39 = 6;
//     p.unk3C = 0.0f;
//     p.unk40 = 1;
//     p.unk44 = 0.0f;
//     func_15150178(&p, &p.unk8, 0, arg5, 1);
//     return 1;
// }
void *func_150C2804(s32 arg0, s32 arg1, s32 arg2, s16 arg3, u8 arg4, s32 arg5) {
    typedef struct {
        s32 unk0;
        s32 unk4;
        s32 unk8;
        f32 unkC;
        f32 unk10;
        s16 unk14;
        u8 unk16;
        u8 unk17;
        u8 unk18;
        s8 unk19;
    } Pack;
    void *func_15134908(void *, s32, u8, s32);
    extern f32 D_800A0280;
    extern f32 D_800A0284;
    Pack p;

    p.unk0 = arg0;
    p.unk4 = arg1;
    p.unk8 = arg2;
    p.unkC = D_800A0280;
    p.unk10 = D_800A0284;
    p.unk14 = arg3;
    p.unk16 = 5;
    p.unk17 = 6;
    p.unk18 = 3;
    p.unk19 = -1;
    return func_15134908(&p, 0, arg4, arg5);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_EF410/func_150C2898.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_EF410/func_150C29F0.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_EF410/func_150C2C00.s")
typedef struct {
    u8 pad0[0x2C];
    f32 unk2C;
    f32 unk30;
    f32 unk34;
    f32 unk38;
    u8 pad3C[0x10];
    f32 unk4C;
    f32 unk50;
    u8 pad54[0xFC];
    f32 unk150;
} T150C;

s32 func_150C2FCC(T150C *arg0) {
    f32 *dt = &D_800BE9A4;

    arg0->unk2C = arg0->unk2C - (arg0->unk2C * arg0->unk150);
    arg0->unk30 = arg0->unk30 - (arg0->unk30 * arg0->unk150);
    arg0->unk38 += (arg0->unk50 * *dt) + (((0.5f * arg0->unk4C) * *dt) * *dt);
    arg0->unk50 += arg0->unk4C * *dt;
    if ((arg0->unk2C < 10.0f) || (arg0->unk30 < 10.0f)) {
        return 0;
    }
    return 1;
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_EF410/func_150C308C.s")
// ? func_150C308C(void *arg0) {
//     u8 sp1F;
//     f32 temp_f0;
//     f32 temp_f2;
//     u8 temp_a1;
//     void *temp_v0;
//     u8 phi_a1;
//
//     phi_a1 = (u8)0U;
//     if ((s32) arg0->unk1C >= 6) {
//         sp1F = 0;
//         temp_v0 = func_15144B34(D_80082FA4, (u8)0U);
//         temp_f0 = temp_v0->unk0;
//         temp_f2 = temp_v0->unk8;
//         temp_a1 = sp1F;
//         if (((temp_f0 * temp_f0) + (temp_f2 * temp_f2)) < D_800A0310) {
// block_4:
//             phi_a1 = (u8)1U;
//         } else {
//             sp1F = temp_a1;
//             phi_a1 = temp_a1;
//             if (D_800A0314 < func_15144C8C(func_150484A0(temp_v0->unk0, temp_v0->unk8, temp_a1), arg0->unk160)) {
//                 goto block_4;
//             }
//         }
//     }
//     if (phi_a1 != 0) {
//         arg0->unk1C = (u16)5;
//     }
//     return 1;
// }

// NON-MATCHING: 25/52 at 0xd0. tip beql_zero_f32_else_div
// `t = (d==0) ? 1.0f : n/d` then `gDPSetTileSize(gdl++, 4, uls, ult, 0x1FE, 0x3E)` is the right length; IDO `bnezl` to div vs ROM `beql` + delay mtc1 1.0f.
// Gfx *func_150C3160(Gfx *gdl, T3160 *arg1) {
//     f32 t;
//     s32 v1;
//     s32 a0;
//     if (arg1->unk2E8 == 0) {
//         t = 1.0f;
//     } else {
//         t = (f32)arg1->unk2E4 / (f32)arg1->unk2E8;
//     }
//     t = 1.0f - t;
//     a0 = 2 - arg1->unk2EC;
//     v1 = (s32)((500.0f * t) + 2.0f);
//     arg1->unk2EC = v1 / 3;
//     while (a0 < 0) {
//         a0 += 0x40;
//     }
//     gDPSetTileSize(gdl++, 4, v1 & 0xFFF, a0 & 0xFFF, 0x1FE, 0x3E);
//     return gdl;
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_EF410/func_150C3160.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_EF410/func_150C3230.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_EF410/func_150C3574.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_EF410/func_150C3994.s")

s32 func_150C3D48(s32 arg0) {
    return arg0 + 0xEDCBA988;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_EF410/func_150C3D5C.s")
