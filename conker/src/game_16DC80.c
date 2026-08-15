#include <ultra64.h>

#include "functions.h"
#include "variables.h"

#pragma GLOBAL_ASM("asm/nonmatchings/game_16DC80/func_151407D0.s")
// NON-MATCHING: spawn wrapper; jal func_1513D524 then memcpy(v0+0x110). length_schedule on
// beqz-delay addiu dest vs early return; inner D_800DC9F0++. tip length_schedule

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

// NON-MATCHING: 10/15 at right 0x3c — addiu $v0,$a0,0x110 hoisted before beqz
// (and first lwc1/lw swapped). tip substruct_addiu_vs_nullcheck; do not thrash
// void func_1514143C(void *arg0) {
//     typedef struct {
//         f32 unk0;
//         f32 unk4;
//         f32 unk8;
//     } Vec;
//     typedef struct {
//         u8 pad[0x44];
//         Vec * volatile unk44;
//     } Mid;
//     typedef struct {
//         u8 pad0[0x34];
//         f32 unk34;
//         f32 unk38;
//         f32 unk3C;
//         u8 pad40[0xD0];
//         Mid unk110;
//     } Local;
//     Local *a = arg0;
//     Mid *m = &a->unk110;
//
//     if (m->unk44 != NULL) {
//         m->unk44->unk0 = a->unk34;
//         m->unk44->unk4 = a->unk38;
//         m->unk44->unk8 = a->unk3C;
//     }
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_16DC80/func_1514143C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_16DC80/func_15141478.s")
// NON-MATCHING: 53/59 justreg=53/59 len=0xec  tip ptr_home_20_vs_24_frame
// Mid *m jal-spill is `sw $v1, 0x20($sp)` vs ROM `0x24`. Unused s32/f32 or `s32 ret=1`
// moves the spill to 0x24 but grows the frame 0x30→0x38 (57/59, only addiu sp).
// `{Part *p; Mid *m}` wrap keeps 0x30/0x20. Sibling of matched func_15141564.
// s32 func_15141478(void *arg0) {
//     typedef struct {
//         f32 unk0, unk4, unk8, unkC, unk10, unk14, unk18;
//     } Mid;
//     typedef struct {
//         u8 pad[0x48];
//         f32 unk48;
//     } Part;
//     typedef struct {
//         u8 pad[0x110];
//         Part unk110;
//         u8 pad15C[0x14];
//         Mid unk170;
//     } Local;
//     Local *a = arg0;
//     Mid *m = &a->unk170;
//     Part *p = &a->unk110;
//
//     m->unk10 = m->unk10 - D_800BE9A4;
//     if (m->unk10 < 0) {
//         m->unk10 = func_150ADA68() * m->unk14;
//         if ((func_150ADA20() & 3) != 0) {
//             m->unkC = m->unk4 + func_150ADA68() * (m->unk0 - m->unk4);
//         } else {
//             m->unkC = m->unk0 + func_150ADA68() * (m->unk8 - m->unk0);
//         }
//     }
//     p->unk48 += (m->unkC - p->unk48) * m->unk18;
//     return 1;
// }

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
// NON-MATCHING: 1 insn short (0x110 vs 0x114). Park. First 6 words match (lwc1
// unk17C/180 from $a0, addiu $v0,$a0,0x170, c.lt.s, nop, bc1fl). Missing
// remat `addiu $v0,$a0,0x170` in the unkC<unk10 arm (ROM then lwc1 4($v0)).
// Reassigning m / nested q / a->unk170.unk4 CSE to lwc1 0x174($a0) or add
// move $v0,$v1. Named d/s temps get the second-arm load order (unk10, unk20,
// unk8) at 0x114 only with the extra v1 copy (22/69). Sibling of matched
// func_15141564. Leave asm.
// s32 func_151415D4(void *arg0) {
//     typedef struct {
//         f32 unk0, unk4, unk8, unkC, unk10, unk14, unk18, unk1C, unk20;
//     } Mid;
//     typedef struct {
//         u8 pad[0x158];
//         f32 unk158;
//         u8 pad15C[0x14];
//         Mid unk170;
//     } Local;
//     Local *a = arg0;
//     Mid *m = &a->unk170;
//     f32 d, s;
//
//     if (m->unkC < m->unk10) {
//         a->unk158 = m->unk4;
//     } else if (m->unkC < m->unk14) {
//         d = m->unkC - m->unk10;
//         s = d * m->unk20;
//         a->unk158 = m->unk4 + (m->unk8 * s);
//     } else if (m->unkC < m->unk18) {
//         a->unk158 = m->unk0;
//     } else {
//         d = m->unkC - m->unk18;
//         a->unk158 = m->unk4 + (m->unk8 * (1.0f - (d * m->unk20)));
//     }
//     m->unkC = m->unkC + D_800BE9A4;
//     while (m->unk1C < m->unkC) {
//         m->unkC = m->unkC - m->unk1C;
//     }
//     return 1;
// }

#pragma GLOBAL_ASM("asm/nonmatchings/game_16DC80/func_151416E8.s")

// NON-MATCHING: 7/21 at right 0x54 — needs `sw $a0` home then `addiu $a0,$sp,0x1C`
// before `sw $ra`, copy of D_8008A074 via $t6, u8 at 0x24. Array/struct copies
// load the global first. Leave asm.
#pragma GLOBAL_ASM("asm/nonmatchings/game_16DC80/func_151417C4.s")

s32 func_15141818(s32 arg0, s32 arg1) {
    return 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_16DC80/func_1514182C.s")
// NON-MATCHING: 37/63 justreg=40/63 len=0xfc  tip lerp_500_lui_interleave. Park.
// Keep the matched caller's proto (s32 arg3, f32 arg4/arg5). Unused f32 pad0
// puts mtx at 0x34 / frame 0x80. 8050 gets addiu $a2,0; 7960 mfc1 zeros + lw
// arg2. Prologue through the second jal is exact. Named f32 sc = *(f32*)&arg3
// is right length but lui 500.0f immediately after lwc1 scale (f0 not f6) and
// finishes the first swc1 early vs ROM interleaving unk38/v->unk4/unk3C then
// lui. Bare *(f32*)&arg3 is 2 long (0x104). Comma preloads / Vec copies do not
// delay the lui. Do not return the jal leftover. Do not change func_15141928.
// s32 func_1514182C(void *arg0, void *arg1, s32 arg2, s32 arg3, f32 arg4, f32 arg5) {
//     typedef struct {
//         u8 pad[0x34];
//         f32 unk34, unk38, unk3C, unk40, unk44, unk48;
//     } Local;
//     typedef struct {
//         f32 unk0, unk4, unk8;
//     } Vec;
//     void func_150A7960(f32 (*)[4], f32, s32, f32, f32 *, f32 *, f32 *);
//     Local *a = arg0;
//     Vec *v = arg1;
//     f32 pad0;
//     f32 mtx[4][4];
//     f32 sc;
//
//     func_150A8050(mtx, arg4, 0, arg5);
//     mtx[3][0] = v->unk0;
//     mtx[3][1] = v->unk4;
//     mtx[3][2] = v->unk8;
//     func_150A7960(mtx, 0.0f, arg2, 0.0f, &a->unk34, &a->unk38, &a->unk3C);
//     sc = *(f32 *) &arg3;
//     a->unk40 = a->unk34 + ((a->unk34 - v->unk0) * sc * 500.0f);
//     a->unk44 = a->unk38 + ((a->unk38 - v->unk4) * sc * 500.0f);
//     a->unk48 = a->unk3C + ((a->unk3C - v->unk8) * sc * 500.0f);
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
