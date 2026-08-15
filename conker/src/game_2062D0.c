#include <ultra64.h>

#include "functions.h"
#include "variables.h"


u8 func_151D8E20(void) {
    if ((D_800BE9F0 == 0) && (func_150A29C8(0, 0x1C) == 0)) {
        return 10;
    }
    return D_800E0A10;
}

u8 func_151D8E6C(void) {
    u8 tmp[3] = D_800AB340;
    return tmp[(func_150ADA20() % 3U)];
}

u8 func_151D8EB0(void) {
    return 117;
}

u8 func_151D8EBC(void) {
    return 29;
}

u8 func_151D8EC8(void) {
    s32 tmp;

    if (func_150ADA20() & 1) {
        tmp = 17;
    } else {
        tmp = 147;
    }
    return tmp;
}

u8 func_151D8EFC(void) {
    s32 tmp;

    if (func_150ADA20() & 1) {
        tmp = 90;
    } else {
        tmp = 91;
    }
    return tmp;
}

u8 func_151D8F30(void) {
    u8 tmp[5] = D_800AB344;
    return tmp[func_150ADA20() % 5U];
}

u8 func_151D8F7C(void) {
    s32 tmp;

    if ((func_150ADA20() & 1) != 0) {
        tmp = 102;
    } else {
        tmp = 103;
    }
    return tmp;
}

u8 func_151D8FB0(void) {
    return 149;
}

u8 func_151D8FBC(void) {
    return 159;
}

u8 func_151D8FC8(void) {
    return 179;
}

u8 func_151D8FD4(void) {
    return 117;
}

u8 func_151D8FE0(void) {
    u8 tmp[4] = D_800AB34C;
    return tmp[func_150ADA20() & 3];
}

// big struct definition
// void func_151D9014(void *arg0, f32 *arg1, u8 arg2, f32 arg3, s16 arg4, u8 arg5, f32 arg6, u8 arg7, f32 arg8, f32 arg9, u8 argA, s32 argB, u8 argC, u8 argD, u8 argE, s32 argF);
#pragma GLOBAL_ASM("asm/nonmatchings/game_2062D0/func_151D9014.s")

s32 func_151D9450(void *arg0, void *arg1);

s32 func_151D93F4(void *arg0, void *arg1) {
    s32 res;

    if (func_151D9450(arg0, arg1) != 0) {
        if (func_151D9534(arg0, arg1) != 0) {
            res = 1;
        } else {
            res = 0;
        }
    } else {
        res = 0;
    }
    return res;
}

s32 func_151D9450(void *arg0, void *arg1) {
    typedef struct {
        f32 unk0;
        u8 unk4;
        u8 unk5;
        s8 unk6;
        s8 unk7;
        f32 unk8;
        f32 unkC;
    } Mid;
    typedef struct {
        u8 pad[0x38];
        f32 unk38;
        f32 unk3C;
        u8 pad40[0x68];
        Mid unkA8;
        u8 padB8[9];
        u8 unkC1;
    } Local;
    Mid *m;

    arg1 = arg0;
    m = &((Local *) arg1)->unkA8;
    if (((Local *) arg1)->unkC1 & 1) {
        return 1;
    }
    m->unk4 += m->unk6 * D_800BE9E4;
    m->unk5 += m->unk7 * D_800BE9E4;
    ((Local *) arg1)->unk38 = func_151423D8((u8) (m->unk4 - 0x40)) * m->unk8 + m->unk0;
    ((Local *) arg1)->unk3C = func_151423D8((u8) (m->unk5 - 0x40)) * m->unkC + m->unk0;
    return 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_2062D0/func_151D9534.s")

u8 func_151D97A8(void) {
    s32 tmp[7] = D_800AB350;
    return tmp[func_150ADA20() % 7U];
}

u8 func_151D9820(void) {
    s32 tmp[3] = D_800AB36C;
    return tmp[func_150ADA20() % 3U];
}

u8 func_151D9878(void) {
    s32 tmp[3] = D_800AB378;
    return tmp[func_150ADA20() % 3U];
}

u8 func_151D98D0(void) {
    s32 tmp[2] = D_800AB384;
    return tmp[func_150ADA20() & 1];
}

u8 func_151D9918(void) {
    s32 tmp[2] = D_800AB38C;
    return tmp[func_150ADA20() & 1];
}

u8 func_151D9960(void) {
    s32 tmp[5] = D_800AB394;
    return tmp[func_150ADA20() % 5U];
}

u8 func_151D99C8(void) {
    s32 tmp[3] = D_800AB3A8;
    return tmp[func_150ADA20() % 3U];
}

u8 func_151D9A20(void) {
    s32 tmp[2] = D_800AB3B4;
    return tmp[func_150ADA20() & 1];
}

u8 func_151D9A68(void) {
    s32 tmp[3] = D_800AB3BC;
    return tmp[func_150ADA20() % 3U];
}

u8 func_151D9AC0(void) {
    s32 tmp[1] = D_800AB3C8;
    return tmp[0];
}

u8 func_151D9ADC(void) {
    s32 tmp[3] = D_800AB3CC;
    return tmp[func_150ADA20() % 3U];
}

u8 func_151D9B34(void) {
    s32 tmp[4] = D_800AB3D8;
    return tmp[func_150ADA20() & 3];
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_2062D0/func_151D9B8C.s")

extern void func_151D9014(f32 *arg0, f32 *arg1, s32 arg2, f32 arg3, s16 arg4, u8 arg5, f32 arg6, s32 arg7, f32 arg8, f32 arg9, s32 argA, s32 argB, s32 argC, s32 argD, u8 argE, u8 argF);


void func_151D9EB0(void *arg0) {
    typedef struct {
        s16 unk0;
        s16 pad2;
        f32 unk4;
        f32 unk8;
        u8 padC[8];
        u8 unk14;
        u8 unk15;
        u8 unk16;
    } Mid;
    typedef struct {
        u8 unk0;
        u8 unk1;
        u8 pad2[0xA];
        u8 unkC;
        u8 padD[0x1B];
        Mid unk28;
    } Local;
    Mid *m = &((Local *) arg0)->unk28;

    m->unk0 -= D_800BE9E4;
    if (m->unk0 >= 0) {
        return;
    }
    func_151D9014(
        &m->unk8,
        &D_800A5480,
        m->unk16,
        func_150ADA68() * D_800AB464 + D_800AB468,
        func_150ADA20() % 0x29U + 0x23,
        m->unk14,
        m->unk4,
        0,
        1.0f,
        1.0f,
        m->unk15,
        0,
        1,
        0,
        ((Local *) arg0)->unkC,
        ((Local *) arg0)->unk1
    );
    m->unk0 = func_150ADA20() % 0x6FU + 0x1E;
}


void func_151D9FC0(u8 arg0, f32 arg1, u8 arg2, s32 arg3, s32 arg4, u8 arg5, s32 arg6) {
    func_151DBCBC(arg0, arg1 * 0.5f, arg2, arg3, arg4, arg5, arg6);
    if ((arg0 != 5) && (arg0 != 2)) {
        func_151DA08C(arg0, arg1 * D_800AB46C, 1.0099999904632568f, arg2, 100, arg3, arg4, arg5, arg6);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_2062D0/func_151DA08C.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_2062D0/func_151DA368.s")
// TODO when we know what arg0 is...
// NON-MATCHING: 0x54 vs 0x50 — while(n--) is right shape (beql flag skip); do-while
// --n unrolls andi 3. tip countdown_while_ndec_vs_unroll
// s32 func_151DA6A8(void *arg0) {
//     typedef struct {
//         u8 pad[0x10];
//         f32 unk10;
//         f32 unk14;
//     } Mid;
//     typedef struct {
//         u8 pad[0x58];
//         s32 unk58;
//         u8 pad5C[0xCC];
//         Mid unk128;
//     } Local;
//     Local *a = arg0;
//     Mid *m;
//     s32 n;
//     f32 f;
//
//     if ((a->unk58 & 1) != 0) {
//         n = D_800BE9E4;
//         m = &a->unk128;
//         if (n != 0) {
//             f = m->unk14;
//             while (n--) {
//                 m->unk10 = m->unk10 * f;
//             }
//         }
//     }
//     return 1;
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_2062D0/func_151DA6A8.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_2062D0/func_151DA6F8.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_2062D0/func_151DA938.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_2062D0/func_151DAA88.s")
// NON-MATCHING: 37/52 at 0xd0. tip stride_multu_vs_ptr_home
// `s32 n=0x14` + reload `a->unk2D` twice emits both `li; multu`. Named table
// pointer CSEs `$v0` but frame 0x50 vs 0x48; reloading unk94 is 4 long.
// s32 func_151DAA88(void *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5) {
//     typedef struct {
//         f32 unk0;
//         u8 pad4[0x17];
//         u8 unk1B;
//         u8 pad1C[4];
//         u8 unk20;
//         u8 pad21[0x2B];
//         f32 unk4C;
//         u8 unk50;
//     } Inner;
//     typedef struct {
//         u8 pad0;
//         u8 unk1;
//         u8 pad2[0xA];
//         u8 unkC;
//         u8 padD[0x20];
//         s8 unk2D;
//         u8 pad2E[0x66];
//         void *unk94;
//         Inner *unk98;
//     } Local;
//     Local *a = arg0;
//     Inner *m = a->unk98;
//     f32 vec[3];
//     s32 n = 0x14;
//     u8 *base = a->unk94;
//
//     vec[0] = *(f32 *)(base + a->unk2D * n);
//     vec[1] = arg4;
//     vec[2] = *(f32 *)(base + a->unk2D * n + 8);
//     func_151D9FC0(m->unk50, (m->unk0 * 11.0f) * m->unk4C, m->unk1B, arg5, (s32) vec, a->unkC, a->unk1);
//     m->unk20 = 4;
//     return 1;
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_2062D0/func_151DAB58.s")
s32 func_151DADA0(void *arg0) {
    typedef struct {
        u8 unk0;
        s8 unk1;
        u8 pad2[0x2];
        f32 unk4;
        f32 unk8;
    } Mid;
    typedef struct {
        u8 pad[0x4C];
        f32 unk4C;
        f32 unk50;
        u8 pad54[0xBC];
        Mid unk110;
    } Local;
    Local *a = arg0;
    Mid *m = &a->unk110;
    f32 s;

    s = func_151423D8((u8)((m->unk0 += m->unk1 * D_800BE9E4) - 0x40));
    a->unk4C = m->unk4 * s + 1.0f;
    a->unk50 = D_800AB4B0 - m->unk8 * s;
    return 1;
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_2062D0/func_151DAE28.s")

void func_151DB004(struct218 *arg0) {
    arg0->unk14 = (func_150ADA20() % 0x38U) + 80;
    arg0->unk15 = 0;
    arg0->unk16 = 0;
    arg0->unk18 = (func_150ADA20() % 0x2EU) + 180;
    arg0->unk19 = 0;
    arg0->unk1A = 0;
}

void func_151DB068(struct218 *arg0) {
    arg0->unk14 = arg0->unk15 = (func_150ADA20() % 0x38U) + 100;
    arg0->unk16 = 0;
    arg0->unk18 = arg0->unk19 = (func_150ADA20() % 0x2EU) + 180;
    arg0->unk1A = 0;
}

void func_151DB0CC(struct218 *arg0) {
    arg0->unk14 = (func_150ADA20() % 0x38U) + 80;
    arg0->unk15 = (func_150ADA20() % 0x38U) + 80;
    arg0->unk16 = 0;
    arg0->unk18 = (func_150ADA20() % 0x2EU) + 180;
    arg0->unk19 = (func_150ADA20() % 0x2EU) + 180;
    arg0->unk1A = 0;
}

void func_151DB15C(struct218 *arg0) {
    arg0->unk14 = (func_150ADA20() % 0x38U) + 80;
    arg0->unk15 = (func_150ADA20() % 0x38U) + 80;
    arg0->unk16 = 0;
    arg0->unk18 = (func_150ADA20() % 0x2EU) + 180;
    arg0->unk19 = (func_150ADA20() % 0x2EU) + 180;
    arg0->unk1A = 0;
}

void func_151DB1EC(struct218 *arg0) {
    arg0->unk14 = (func_150ADA20() % 0x38U) + 80;
    arg0->unk15 = (func_150ADA20() % 0x38U) + 80;
    arg0->unk16 = 0;
    arg0->unk18 = (func_150ADA20() % 0x2EU) + 180;
    arg0->unk19 = (func_150ADA20() % 0x2EU) + 180;
    arg0->unk1A = 0;
}

void func_151DB27C(struct218 *arg0) {
    arg0->unk14 = 0xFF;
    arg0->unk15 = 0xFF;
    arg0->unk16 = 0xFF;
    arg0->unk18 = 0xB4;
    arg0->unk19 = 0xC8;
    arg0->unk1A = 0xC8;
}

void func_151DB2A8(struct218 *arg0) {
    arg0->unk14 = 0;
    arg0->unk15 = 200;
    arg0->unk16 = 0;
    arg0->unk18 = 0;
    arg0->unk19 = 200;
    arg0->unk1A = 0;
}

void func_151DB2CC(struct218 *arg0) {
    arg0->unk14 = 0;
    arg0->unk15 = (func_150ADA20() % 0x38U) + 80;
    arg0->unk16 = 0;
    arg0->unk18 = 0;
    arg0->unk19 = (func_150ADA20() % 0x2EU) + 180;
    arg0->unk1A = 0;
}

void func_151DB330(struct218 *arg0) {
    arg0->unk14 = (func_150ADA20() % 0x15U) + 95;
    arg0->unk15 = (func_150ADA20() % 0x15U) + 95;
    arg0->unk16 = (func_150ADA20() % 0xBU) + 45;
    arg0->unk18 = (func_150ADA20() & 0xF) + 58;
    arg0->unk19 = (func_150ADA20() & 0xF) + 60;
    arg0->unk1A = (func_150ADA20() % 0xBU) + 25;
}

void func_151DB3D8(struct218 *arg0) {
    arg0->unk14 = 0;
    arg0->unk15 = arg0->unk16 = (func_150ADA20() % 0x38U) + 80;
    arg0->unk18 = 0;
    arg0->unk19 = arg0->unk1A = (func_150ADA20() % 0x2EU) + 180;
}

void func_151DB43C(struct218 *arg0) {
    arg0->unk14 = (func_150ADA20() % 0x38U) + 80;
    arg0->unk15 = 0;
    arg0->unk16 = (func_150ADA20() % 0x38U) + 80;
    arg0->unk18 = (func_150ADA20() % 0x2EU) + 180;
    arg0->unk19 = 0;
    arg0->unk1A = (func_150ADA20() % 0x2EU) + 180;
}

void func_151DB4CC(struct218 *arg0) {
    arg0->unk14 = (func_150ADA20() % 56U) + 200;
    arg0->unk15 = (func_150ADA20() % 56U) + 200;
    arg0->unk16 = (func_150ADA20() % 56U) + 200;
    arg0->unk18 = (func_150ADA20() % 56U) + 200;
    arg0->unk19 = (func_150ADA20() % 56U) + 200;
    arg0->unk1A = (func_150ADA20() % 56U) + 200;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_2062D0/func_151DB5D0.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_2062D0/func_151DB97C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_2062D0/func_151DBAA8.s")
// NON-MATCHING: 40/75 justreg=44/75 len=0x12c  tip pack_vec_li_interleave. Park.
// Frame 0x70, a1=&tmp.unk8, Vec word-copy of arg0 matches the first 10 words.
// ROM interleaves `li -0x40, 0x2E, 0xFF, 3, 2, 0x1E…` with that copy; IDO
// hoists 0xFF/3 first. Delay unk3C=arg2 until the jal. Keep 5-arg proto.
// void func_151DBAA8(struct17 *arg0, s32 arg1, u8 arg2, u8 arg3, s32 arg4) {
//     typedef struct {
//         s32 unk0, unk4, unk8;
//     } Vec;
//     typedef struct {
//         s16 unk0, unk2, unk4, unk6;
//         Vec unk8;
//         f32 unk14, unk18, unk1C, unk20, unk24, unk28;
//         s16 unk2C, unk2E, unk30, unk32, unk34, unk36, unk38, unk3A;
//         s8 unk3C;
//         u8 pad3D[3];
//         f32 unk40;
//         s16 unk44, unk46;
//         s32 unk48;
//     } Pack;
//     void func_15153F18(void *, void *, s32, u8, s32);
//     Pack tmp;
//
//     tmp.unk8 = *(Vec *) arg0;
//     tmp.unk4 = -0x40;
//     tmp.unk6 = 0x2E;
//     tmp.unk2 = 0xFF;
//     tmp.unk30 = 3;
//     tmp.unk32 = 2;
//     tmp.unk34 = 0x1E;
//     tmp.unk36 = 0x1E;
//     tmp.unk38 = 0x9B;
//     tmp.unk14 = 5.5f;
//     tmp.unk18 = D_800AB4C0;
//     tmp.unk1C = D_800AB4C4;
//     tmp.unk20 = D_800AB4C8;
//     tmp.unk24 = 10.0f;
//     tmp.unk2C = arg1;
//     tmp.unk28 = D_800AB4CC;
//     tmp.unk0 = 0;
//     tmp.unk2E = 0;
//     tmp.unk3A = 0x64;
//     tmp.unk44 = 0x10;
//     tmp.unk46 = 0xF;
//     tmp.unk48 = 0;
//     tmp.unk40 = 0.5f;
//     tmp.unk3C = arg2;
//     func_15153F18(&tmp, &tmp.unk8, 0, arg3, arg4);
// }

void func_151DBBD4(struct17 *arg0, s32 arg1, u8 *arg2, u8 arg3, s32 arg4) {
    struct17 tmp;
    struct217 tmp2;

    tmp.unk0 = arg0->unk0;
    tmp.unk4 = arg0->unk4 + 5.0f;
    tmp.unk8 = arg0->unk8;

    tmp2.unkF = *arg2;
    tmp2.unk0 = func_150ADA68();
    tmp2.unk4 = func_150ADA20();

    func_151D9B8C(tmp2.unkF, (tmp2.unk0 * 25.0f) + 10.0f, ((tmp2.unk4 % 0x38U) + 200), arg1 + 4, &tmp, (func_150ADA20() % 0x97U) + 150, 0, 1, 0, arg3, arg4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_2062D0/func_151DBCBC.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_2062D0/func_151DBE80.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_2062D0/func_151DC034.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_2062D0/func_151DC260.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_2062D0/func_151DC484.s")

// typedef struct {
//     s16 unk10; // sp24
//     s16 unk12; // sp26
//     s16 unk14;
//     s16 unk16; // sp2A
//     s32 unk18[3];
//     s16 unk24; // sp38;
//     s16 unk26; // sp3A
//     f32 unk28; // sp3C
//     f32 unk2C; // sp40
//     s16 unk30; // sp44
//     s16 unk32; // sp46
//     f32 unk34; // sp48;
//     f32 unk38; // sp4C;
//     s8  unk3C; // sp50;
//     s8  unk3D; // sp51;
//     u8  unk3E[2]; //
//     f32 unk40; // sp54
//     f32 unk44; // sp58
//     s8  unk48; // sp5C
//     u8  unk49; // sp5D
//     u8  unk4A[2];
//     f32 unk4C; // sp60
//     s8  unk50; // sp64
//     u8  pad51[3];
//     f32 unk54; // sp68
// } struct217;
//
// typedef struct {
//     s16 unk58; // sp6C
//     s16 unk5A; // sp6E
//     s16 unk5C; // sp70
//     s16 unk5E; // sp72
//     s32 unk60[3]; // sp74
//     f32 unk6C; // sp80
//     f32 unk70; // sp84
//     f32 unk74; // sp88
//     f32 unk78; // sp8C
//     f32 unk7C; // sp90
//     f32 unk80; // sp94
//     s16 unk84; // sp98
//     s16 unk86; // sp9A
//     s16 unk88; // sp9C
//     s16 unk8A; // sp9E
//     s16 unk8C; // spA0
//     s16 unk8E; // spA2
//     s16 unk90; // spA4
//     s16 unk92; // spA6
//     u8  unk94; // spA8;
//     u8  pad95[3];
//     f32 unk98; // spAC;
//     s16 unk9C; // spB0
//     s16 unk9E; // spB2
//     s32 unkA0; // spB4
// } struct218;
//
// void func_151DC484(struct00 *arg0, s32 arg1, u8 arg2, u8 arg3, s32 arg4) {
//     struct218 tmp2;
//     struct217 tmp;
//
//     tmp2.unk60[0] = arg0->unk0; // sp74.unk0 = (s32) arg0->unk0;
//     tmp2.unk60[1] = arg0->unk4; // sp74.unk4 = (s32) arg0->unk4;
//     tmp2.unk60[2] = arg0->unk8; // sp74.unk8 = (s32) arg0->unk8;
//
//     tmp2.unk5A = 0xFF;   // sp6E = 0xFF;
//     tmp2.unk5C = -0x40;  // sp70 = -0x40;
//     tmp2.unk84 = 8;      // sp98 = 8;
//     tmp2.unk86 = 6;      // sp9A = 6;
//     tmp2.unk58 = 0;      // sp6C = 0;
//     tmp2.unk88 = 3;      // sp9C = 3;
//     tmp2.unk8A = 0;      // sp9E = 0;
//     tmp2.unk5E = 0x28;   // sp72 = 0x28;
//     tmp2.unk8C = 0x3C;   // spA0 = 0x3C;
//     tmp2.unk8E = 0x28;   // spA2 = 0x28;
//
//     tmp2.unk6C = 3.0f;   // sp80 = 3.0f;
//     tmp2.unk90 = 100;     // spA4 = 0x64;
//     tmp2.unk92 = 100;     // spA6 = 0x64;
//     tmp2.unk9C = 16;      // spB0 = 0x10;
//     tmp2.unk9E = 15;      // spB2 = 0xF;
//     tmp2.unk70 = 2.0f;       // sp84 = 2.0f;
//     tmp2.unk74 = D_800AB4E4; // sp88 = D_800AB4E4;
//     tmp2.unk78 = D_800AB4E8; // sp8C = D_800AB4E8;
//     tmp2.unk7C = 8.0f;       // sp90 = 8.0f;
//     tmp2.unk80 = 5.0f;       // sp94 = 5.0f;
//     tmp2.unk98 = 1.0f;       // spAC = 1.0f; 0x3f80
//     tmp2.unk94 = arg2;       // spA8 = arg2;
//     tmp2.unkA0 = 0;      // spB4 = 0;
//
//     // func_15153F18(&sp6C, &sp74, arg1, arg3, arg4);
//     func_15153F18(&tmp2, &tmp2, arg1, arg3, arg4);
//
//     tmp.unk18[0] = arg0->unk0; // sp2C.unk0 = (s32) arg0->unk0;
//     tmp.unk18[1] = arg0->unk4; // sp2C.unk4 = (s32) arg0->unk4;
//     tmp.unk18[2] = arg0->unk8; // sp2C.unk8 = (s32) arg0->unk8;
//
//     tmp.unk24 = 0xC; // sp38 = 0xC;
//     tmp.unk12 = 0xFF;   // sp26 = 0xFF;
//     tmp.unk26 = 6;      // sp3A = 6;
//     tmp.unk10 = 0;      // sp24 = 0;
//     tmp.unk14 = -0x40;  // sp28 = -0x40;
//     tmp.unk16 = 0x1A;   // sp2A = 0x1A;
//     tmp.unk30 = 0x23;   // sp44 = 0x23;
//     tmp.unk32 = 0xF;    // sp46 = 0xF;
//     tmp.unk3C = 0x9B;   // sp50 = 0x9B;
//     tmp.unk3D = 0x64;   // sp51 = 0x64;
//     tmp.unk40 = 59.0f;  // sp54 = 59.0f;
//     tmp.unk44 = 59.0f;  // sp58 = 59.0f;
//     tmp.unk48 = 1;      // sp5C = 1;
//     tmp.unk50 = 1;      // sp64 = 1;
//     tmp.unk54 = 0.0f;   // sp68 = 0.0f;
//     tmp.unk28 = 7.0f;   // sp3C = 7.0f;
//     tmp.unk2C = 3.0f;   // sp40 = 3.0f;
//     tmp.unk34 = D_800AB4EC; // sp48 = D_800AB4EC;
//     tmp.unk38 = D_800AB4F0; // sp4C = D_800AB4F0;
//     tmp.unk49 = arg2;       // sp5D = arg2;
//     tmp.unk4C = D_800AB4F4; // sp60 = D_800AB4F4;
//     // func_15150178(&sp24, &sp2C, arg1, arg3, arg4);
//     func_15150178(&tmp, &tmp.unk18, arg1, arg3, arg4);
// }
