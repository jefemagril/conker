#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15168B10(s32 arg0, s32 arg1);
void *func_15167A68(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s32 arg5);
extern u8 D_800D2DAB;

#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15167010.s")
// NON-MATCHING: not hugely far away
// void func_15167010(void) {
//     void (*func)(void);
//     s32 i;
//
//     for (i = 0; i < 24; i++)
//     {
//         func = D_8008B4A8[i].unk18;
//         if (func != NULL) {
//             func();
//         }
//     }
// }

#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_1516706C.s")
// NEAR-MATCHING: loop body matches, but IDO schedules the setup as
// `addiu s0, D_8008CB64` before `addiu s1, D_8008CB70`; target asm needs
// the low-half setup in `s1` then `s0` order. Reordering declarations flips
// both high-half and low-half setup instead of only the low-half instructions.
// extern void (*D_8008CB64[])(void);
// extern void (*D_8008CB70[])(void);
// void func_1516706C(void) {
//     void (**table)(void) = D_8008CB64;
//     void (**end)(void) = D_8008CB70;
//     void (*func)(void);
//
//     do {
//         func = *table;
//         if (func != NULL) {
//             func();
//         }
//         table++;
//     } while (table != end);
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_151670C0.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_151671E8.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15167310.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_151674F8.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15167A68.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15167AD8.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15167B44.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15167C58.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15167D84.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15167E0C.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15168118.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_1516865C.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15168800.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15168870.s")
void func_15168A2C(s32 arg0) {
    func_15168B10(arg0, 0);
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15168A4C.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15168A9C.s")
// void *func_15168A9C(struct12 *arg0) {
//     void *temp_a1;
//     void *temp_v0;
//     void *temp_v0_2;
//
//     temp_a1 = (arg0->unk1 * 0x1A0) + (arg0->unk0 * 4) + 0x800DCE50;
//     if (arg0 == *temp_a1) {
//         *temp_a1 = (void *) arg0->unk8;
//     }
//     temp_v0_2 = arg0->unk8;
//     if (temp_v0_2 != 0) {
//         temp_v0_2->unk4 = (void *) arg0->unk4;
//     }
//     temp_v0 = arg0->unk4;
//     if (temp_v0 != 0) {
//         temp_v0->unk8 = (void *) arg0->unk8;
//     }
//     return temp_v0;
// }


void func_15168B10(s32 arg0, s32 arg1) {
    func_15168A9C(arg0);
    func_15168A4C(arg0, arg1);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15168B44.s")
// void func_15168BAC(void *arg0_raw) {
//     typedef struct {
//         u8 pad0[0xE4];
//         u8 unkE4;
//     } Game1944C0Dispatch;
//
//     Game1944C0Dispatch *arg0 = (Game1944C0Dispatch *)arg0_raw;
//     u8 idx = arg0->unkE4;
//
//     if (idx != 0) {
//         D_8008CA20[idx](arg0);
//     }
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15168BAC.s")

// void func_15168BE4(void *arg0_raw, u8 arg1, s32 arg2) {
//     typedef struct {
//         u8 pad0[0x40];
//         s32 unk40;
//     } Game1944C0CopySrc;
//
//     typedef struct {
//         u8 pad0[0x90];
//         u8 unk90[0x60];
//     } Game1944C0CopyDst;
//
//     Game1944C0CopySrc *arg0 = (Game1944C0CopySrc *)arg0_raw;
//     Game1944C0CopyDst *temp_v0;
//
//     if (arg0->unk40 != 0) {
//         temp_v0 = func_15167A68(0x10, arg2, 0xF0, 1, arg1, 1);
//         if (temp_v0 != NULL) {
//             bcopy(arg0, temp_v0->unk90, 0x60);
//         }
//     }
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15168BE4.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15168C4C.s")
void func_15168E34(u32 *arg0, u32 arg1) {
    u32 value = *arg0;

    if ((value & 0x0F000000) == 0) {
        *arg0 = value + arg1;
    }
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15168E54.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15168F08.s")
void func_15168F84(s32 arg0, s32 *arg1, s32 *arg2) {
    if (arg0 == 0) {
        *arg1 = 1;
        *arg2 = 0x41;
        return;
    }
    if (arg0 == 1) {
        *arg1 = 0x42;
        *arg2 = 0x4F;
        return;
    }
    if (arg0 == 2) {
        *arg1 = 0x50;
        *arg2 = 0x58;
        return;
    }
    if (arg0 == 3) {
        *arg1 = 0x59;
        *arg2 = 0x5C;
        return;
    }
    if (arg0 == 5) {
        *arg1 = 0x61;
        *arg2 = 0x63;
        return;
    }
    if (arg0 == 6) {
        *arg1 = 0x64;
        *arg2 = 0x65;
        return;
    }
    *arg1 = 0x5D;
    *arg2 = 0x60;
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15169040.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15169070.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15169260.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_1516944C.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_151695F0.s")
// void func_1516962C(s32 arg0, void *arg1, u8 arg2) {
//     typedef struct {
//         u8 pad0[0x3B];
//         u8 unk3B;
//     } Game1944C0Payload;
//
//     struct {
//         void *unk0;
//         u8 unk4;
//     } sp18;
//
//     sp18.unk0 = arg1;
//     sp18.unk4 = ((Game1944C0Payload *)arg1)->unk3B;
//     func_1516944C(arg0, (s32)&sp18, arg2);
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_1516962C.s")
s32 func_15169668(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    D_800D2DAB = 1;
    return arg0;
}
// void func_1516968C(void *arg0, u8 *arg1, u8 arg2) {
//     if ((arg2 == 0xF) || (arg2 == 0x10)) {
//         if (*arg1 == ((u8 *)arg0)[0xC]) {
//             func_1516972C(arg0);
//         }
//     }
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_1516968C.s")

// void func_151696DC(struct102 *arg0) {
//     s32 i;
//
//     for (i = 0; i < D_800DD190; i = (s8)(i + 1)) {
//         if (arg0 == ((void **)D_800DD198)[i]) {
//             ((void **)D_800DD198)[i] = (void *)arg0->unk8;
//         }
//     }
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_151696DC.s")

void func_1516972C(struct102 *arg0) {
    void (*func)(struct102 *arg0);
    func_151696DC(arg0);

    if (arg0->unk0 >= 2) {
        func = D_8008B4D0[arg0->unk0].unk0;
        if (func != NULL) {
            func(arg0);
            return;
        }
        func_15169804(arg0);
    }
}

void func_1516979C(struct102 *arg0) {
    void (*func)(struct102 *arg0);

    func_151696DC(arg0);
    func = D_8008B4D4[arg0->unk0].unk0;
    if (func != NULL) {
        func(arg0);
        return;
    }
    func_15169824(arg0);
}

void func_15169804(struct102 *arg0) {
    func_15168B10(arg0, 1);
}

void func_15169824(struct102 *arg0) {
    func_15168A9C(arg0);
    func_10004074(arg0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15169850.s")
