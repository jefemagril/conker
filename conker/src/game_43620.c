#include <ultra64.h>

#include "functions.h"
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_43620/func_15016170.s")
// NON-MATCHING: 45/77 justreg=61/77 len=0x134  tip s0_bzero_global_macro. Park.
// `St *s = (St *)&D_800E0D80` remats bzero (no s0, 0x34 long). Macro +
// bzero(&D_800E0D80) hits s0/frame. Leftover: one li 0x7FFF CSE vs ROM two;
// pan reload in a1 vs v1 hoists sll before table lui.
// void func_15016170(s32 arg0) {
//     typedef struct {
//         u8 pad0[8];
//         void *unk8;
//         s16 unkC;
//         s16 unkE;
//         s16 unk10;
//         s16 unk12;
//         s16 unk14;
//         s16 unk16;
//         u8 pad18[0xC];
//         s16 unk24;
//         u8 pad26[0xA];
//         s16 unk30;
//         s16 unk32;
//         u8 pad34[0x28];
//         void *unk5C;
//         u8 pad60[0x14];
//         void *unk74;
//         u8 pad78[0xC];
//         s32 unk84;
//         s32 unk88;
//         u8 pad8C[0x8];
//         s16 unk94;
//         s16 unk96;
//         u8 pad98[2];
//         u8 unk9A;
//     } St;
// #define ST (*(St *)&D_800E0D80)
//     bzero(&D_800E0D80, 0xA0);
//     ST.unk8 = alHeapDBAlloc(0, 0, arg0, 1, 0x50);
//     ST.unk74 = alHeapDBAlloc(0, 0, arg0, 1, 0x440);
//     ST.unk5C = alHeapDBAlloc(0, 0, arg0, 1, 8);
//     ST.unkC = 0x40;
//     ST.unk96 = 0x40;
//     ST.unk88 = 0x7FFF;
//     ST.unkE = 0x7FFF;
//     ST.unk84 = 0;
//     ST.unk9A = 0;
//     ST.unk32 = 0;
//     ST.unk30 = 0;
//     ST.unk94 = ST.unk96;
//     ST.unk10 = (D_8002BC10[ST.unkC] * ST.unkE) >> 15;
//     ST.unk12 = (D_8002BD0E[-ST.unkC] * ST.unkE) >> 15;
//     ST.unk24 = 1;
//     ST.unk14 = 0x7FFC;
//     ST.unk16 = 0;
//     n_alStreamSetReadCallback(n_alStreamRead);
// #undef ST
// }
