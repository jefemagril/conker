#include <ultra64.h>

#include "functions.h"
#include "variables.h"

typedef struct {
    f32 v[9];
} F9;

void func_150AAD98(F9 arg0);

void func_15049260(F9 arg0) {
    func_150AAD98(arg0);
}

void func_150492CC(f32 arg0, f32 arg1, f32 arg2) {
    D_800CC220 = arg0;
    D_800CC224 = arg1;
    D_800CC228 = arg2;
    D_800CC22C = arg0 / (1.0f + 1.0f);
    D_800CC230 = arg1 / (1.0f + 1.0f);
    D_800CC234 = arg2 / (1.0f + 1.0f);
    if (arg0 == 0.0f) {
        arg0 = D_80099080;
    }
    D_800CC238 = arg1 / arg0;
    D_800CC23C = arg2 / arg0;
}

// NON-MATCHING: length/frame vs schedule thrash — frame-ok 0xd4/0xe4 or len-ok 2/57;
// F32s-in-GPR (a0-a3) vs f32 (f12/f14) and IDO stack homes for temps fight each other.
// typedef union { f32 f; s32 i; } F32s;
// void func_15049350(F32s arg0, F32s arg1, F32s arg2, F32s arg3, F32s arg4, F32s arg5, F32s arg6, F32s arg7, F32s arg8) {
//     f32 sp0 = arg0.f;
//     f32 sp4 = arg1.f;
//     f32 f0 = arg0.f - arg3.f;
//     f32 f2 = arg1.f - arg4.f;
//     f32 f12 = arg2.f - arg5.f;
//     f32 f14 = arg0.f - arg6.f;
//     f32 f16 = arg1.f - arg7.f;
//     f32 f18 = arg2.f - arg8.f;
//     f32 sp10 = (f2 * f18) - (f12 * f16);
//     f32 spC;
//     f32 sp8;
//     D_800CC210 = sp10;
//     spC = ((-f0) * f18) + (f12 * f14);
//     D_800CC214 = spC;
//     sp8 = (f0 * f16) - (f2 * f14);
//     D_800CC218 = sp8;
//     D_800CC21C = (sp10 * sp0) + (spC * sp4) + (arg2.f * sp8);
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_76710/func_15049350.s")
