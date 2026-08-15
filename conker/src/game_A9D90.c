#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_1507C8E0(struct127 *arg0, s32 arg1) {
    arg0->unk31C->unk120 = (u8)2;
    arg0->unk31C->unk124 = arg1;
}

// "goto" hell
#pragma GLOBAL_ASM("asm/nonmatchings/game_A9D90/func_1507C8FC.s")

void func_1507CD0C(struct127 *arg0) {
    s32 temp_lo = ((s32)arg0 - (s32)&D_800CC2D0) / (s32)sizeof(struct127);

    arg0->unk31C->unk120 = 3;

    if (temp_lo <= D_80082FA0) {
        func_15181D70(temp_lo);
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_A9D90/func_1507CD64.s")
void func_1507D158(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    u8 v0 = D_800CC2D0[arg0].unk13F;
    func_1509BFB0(3, v0 | 0x2000, arg1, arg2, arg3, arg4);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_A9D90/func_1507D1D8.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_A9D90/func_1507D4F8.s")
// NON-MATCHING: pretty far away!
// void func_1507D4F8(s16 arg0) { // struct126 *
//     // ?32 sp24;
//     struct127 *temp_a0;
//
//     if ((D_8008FDBC & 1) == 0) {
//         func_15085710(arg0, 4, 1);
//     }
//     // sp24 = (?32) arg0;
//     if (func_150859AC(arg0, 3) != 0) {
//         func_15085710(arg0, 5, D_8008726C); //temp_ret =
//         temp_a0 = &D_800CC2D0[arg0];
//         temp_a0->unkB2 = (u16)0;
//         if (D_800BE616 == 0) {
//             D_800D18A8 = (u8)1;
//             if (((D_800D2E4C->unk19 & 4) != 0) || (D_8008FDA8 < 0)) {
//                 func_1501C730(2, D_800BE3DF, D_800BE3E0, 0, 0);
//                 return;
//             }
//             func_1501C730(1, 0x22, 0, 0, 0);
//             return;
//         }
//         if (D_800E0C20 != 0) {
//             temp_a0->unk31C->unk120 = (u8)0xA;
//             return; // temp_ret;
//         }
//         func_1507D1D8(temp_a0);
//         return;
//     }
//     if (D_800BE616 == 0) {
//         D_800D2E43 = (u8)1;
//         func_1509C3A0();
//         D_800D18A8 = (u8)1;
//         func_15085710(arg0, 5, D_8008726C);
//         func_15085710(arg0, 2, D_80087260);
//         func_1501C730(1, 0x18, 0, 0, 0);
//     } else {
//         D_800D18A0 = (u16) (D_800D18A0 | (1 << (s32) arg0));
//     }
//     // temp_a0 = &D_800CC2D0[arg0];
//     if (temp_a0->unk31C->unk84 == 0) {
//         D_8008FD94 -= 1; //(s8) (D_8008FD94 - 1);
//     }
//     temp_a0->unk31C->unk120 = (u8)0xA;
//     D_800BE618 -= 1; //(s8) (D_800BE618 - 1);
//     //return temp_a0_2->unk31C;
// }

#pragma GLOBAL_ASM("asm/nonmatchings/game_A9D90/func_1507D754.s")

void func_1507DB44(s32 arg0, s32 arg1) {
    func_1503DE70(arg0, arg1, -1);
}

void func_1507DB64(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_A9D90/func_1507DB6C.s")

void func_1507DE4C(struct127 *arg0) {

    if (arg0->interaction_state == 1) {
        func_150836CC(arg0, 0x44);
        func_150836CC(arg0, 0x23);
        arg0->unk9C |= 0xF000;
        func_150836CC(arg0, 0x44);
        func_150836CC(arg0, 0x23);
        return;
    }

    switch(arg0->id) {
        case 0x9F:
        case 0xa0:
            arg0->unk9C |= 0xF000;
            break;
        case 0x5A:
        case 0x74:
        case 0x7A:
            arg0->unk9C |= 0xFF8;
            break;
    }
}


#pragma GLOBAL_ASM("asm/nonmatchings/game_A9D90/func_1507DF10.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_A9D90/func_1507DFE4.s")
// NON-MATCHING: 2 long at frame 0x68 (save); named f32 ang exact 0x130 but frame
// 0x70. tip f32_ang_home_vs_f0. Park. `save` then Stk hits out@0x38 / vec@0x5C.
// 84D70 2nd arg s32 (u8 lbu 0x6B). ROM hoists (u8-0x40)*1.40625f+180 before the
// out branch. Widened 7E114's extern to s32; still 47/47.
// void func_1507DFE4(s32 arg0, s32 arg1) {
//     extern s32 func_15084D70(s32, s32, s32, void *, void *, void *, void *, void *, void *, s32, s32 *);
//     extern void func_15022190(s16, s16, s16, f32);
//     extern void func_1501D348(s32, s32, s32, s32, s32);
//     typedef struct {
//         s32 out;
//         s32 mode;
//         s32 pad40;
//         s32 unk44;
//         s32 unk48;
//         u8 pad4C[2];
//         u8 unk4E;
//         u8 unk4F;
//         u8 unk50[0xC];
//         s16 unk5C;
//         s16 unk5E;
//         s16 unk60;
//     } Stk;
//     s32 save;
//     Stk s;
//
//     if (D_800C35EA == 1) {
//         return;
//     }
//     func_15084D70(0, arg0, 1, &s.unk5C, &s.unk50, &s.unk4F, &s.unk4E, &s.unk44, &s.unk48, 1, &s.out);
//     if (s.out == 0) {
//         s.mode = 1;
//     } else if (s.out != 1) {
//         return;
//     } else {
//         s.mode = 3;
//     }
//     func_15022190(s.unk5C, s.unk5E, s.unk60, ((f32) (s.unk4E - 0x40) * 1.40625f) + 180.0f);
//     save = D_800BE9F0;
//     D_800BE9F0 = 0x25;
//     D_800C3671 = 1;
//     func_1501D348(0x25, s.mode, 0, 0, 0);
//     D_800C3670 = 1;
//     D_800BE9F0 = save;
// }
s32 func_1507E114(s32 arg0) {
    /* Stk before id: id-first shifts the pack 0x3C→0x38 (decl_order_stack). */

    extern s32 func_15084D70(s32, s32, s32, void *, void *, void *, void *, void *, void *, s32, s32 *);
    typedef struct {
        s32 sp3C;
        u8 pad40[2];
        u8 sp42;
        u8 sp43;
        s32 sp44;
        s32 sp48;
        u8 pad4C[8];
        s32 sp54;
        s32 pad58;
        s32 out;
    } Stk;

    Stk s;
    s32 id;

    if (D_800D18A8 == 0) {
        return 0;
    }
    id = (s32) D_800BE9F4;
    if ((id == 0x22) || (id == 0x18)) {
        return 0;
    }
    D_800BE9F4 = (u16 *) (s32) (s8) D_800BE3DF;
    func_15084D70(0, D_800BE3E0, 1, &s.sp54, &s.sp48, &s.sp43, &s.sp42, &s.sp44, &s.sp3C, 1, &s.out);
    return s.out + 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_A9D90/func_1507E1D0.s")
// NON-MATCHING: 0xd0 vs 0xd4. Right frame 0x38. IDO `beql` + else `lwc1` delay vs ROM
// `lui 30.0; addiu $a1,&out; beqz; lw a1home`. `f32 thirty=30.0f` hoists the lui but
// grows the frame. Arrays beat named scalars (those CSE to 0xb8). tip beql_else_vs_beqz_a1home
// extern void func_15143134(void *arg0, void *arg1, s32 arg2);
// void func_1507E1D0(struct127 *arg0, f32 *arg1, f32 *arg2, f32 *arg3) {
//     f32 out[3];
//     f32 in[3];
//     s32 mtx;
//
//     if (arg0->unk1D4 != 0) {
//         in[0] = 0.0f;
//         in[2] = 0.0f;
//         in[1] = arg0->y_scale * 30.0f;
//         mtx = (s32) arg0->unk1D4;
//         if (arg0->interaction_state == 1) {
//             mtx += 0x300;
//         } else if (arg0->interaction_state == 0x1E) {
//             mtx += 0xC0;
//         }
//         func_15143134(in, out, mtx);
//         *arg1 = out[0];
//         *arg2 = out[1];
//         *arg3 = out[2];
//     } else {
//         *arg1 = arg0->x_position;
//         *arg2 = arg0->y_position;
//         *arg3 = arg0->z_position;
//     }
// }
