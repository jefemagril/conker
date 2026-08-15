#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15042D50(void) {
    D_800CBD64 = 0;
    func_15043384(0);
}

void func_15042D78(u8 arg0) {
    D_800CBD74 = arg0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_70200/func_15042D94.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_70200/func_15042E3C.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_70200/func_15042ECC.s")

void func_150432BC(f32 arg0) {
    D_800CBD80 = arg0;
}

void func_150432CC(s32 arg0, s32 arg1) {
    D_800CBD74 = D_800CBD74 | 1;
    D_800CBD74 = D_800CBD74;
    D_800CBD7C = arg1;
    D_800CBD78 = arg0;
}

void func_150432FC(s16 arg0, s16 arg1) {
    D_800CBD70 = arg0;
    D_800CBD72 = arg1;
}

void func_1504332C(u8 arg0, u8 arg1, u8 arg2, u8 arg3) {
    D_800CBD60 = arg0;
    D_800CBD61 = arg1;
    D_800CBD62 = arg2;
    D_800CBD63 = arg3;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_70200/func_15043384.s")

void func_15043A00(struct105 *arg0, s32 arg1, s32 arg2) {
    if (arg0 != 0) {
        arg0->unk0 = arg1; // are these structs?
        arg0->unk4 = arg2;
        arg0->unkC = 0;
        arg0->unk8 = 0;
    }
}

s32 func_15043A20(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 n;

    while (arg4 != 0) {
        if (arg1 < (arg2 + arg4)) {
            n = arg1 - arg2;
        } else {
            n = arg4;
        }
        memcpy((void *) (arg0 + arg2), (void *) arg3, n);
        arg2 += n;
        arg3 += n;
        arg4 -= n;
        if (arg2 >= arg1) {
            arg2 = 0;
        }
    }
    return arg2;
}

s32 func_15043AC8(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 n;

    while (arg4 != 0) {
        if (arg1 < (arg2 + arg4)) {
            n = arg1 - arg2;
        } else {
            n = arg4;
        }
        memcpy((void *) arg3, (void *) (arg0 + arg2), n);
        arg2 += n;
        arg3 += n;
        arg4 -= n;
        if (arg2 >= arg1) {
            arg2 = 0;
        }
    }
    return arg2;
}


s32 func_15043B70(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    s32 v0;

    while (arg3 != 0) {
        if (arg1 < (arg2 + arg3)) {
            v0 = arg1 - arg2;
        } else {
            v0 = arg3;
        }
        arg2 += v0;
        arg3 -= v0;
        if (arg2 >= arg1) {
            arg2 = 0;
        }
    }
    return arg2;
}

// NON-MATCHING: 3-arg ring write matches in isolation, but functions.h must stay
// 1-arg for already-matched init_12560 (`func_15043BB8(&D_800427A0)`). A 3-arg
// prototype breaks that caller; a 1-arg prototype disagrees with this definition.
// tip: do not change shared header arity to ship a callee
// s32 func_15043BB8(struct105 *arg0, void *arg1, s32 arg2) {
//     s32 off;
//
//     if (arg2 != 0) {
//         if (arg1 != 0) {
//             arg2 += 4;
//             arg2 = (arg2 + 3) & -4;
//             off = arg0->unkC;
//             if (off < arg0->unk8) {
//                 if ((off + arg2) >= arg0->unk8) {
//                     return 1;
//                 }
//             } else if ((off + arg2 - arg0->unk4) >= arg0->unk8) {
//                 return 1;
//             }
//             arg2 -= 4;
//             off = func_15043A20(arg0->unk0, arg0->unk4, off, (s32)&arg2, 4);
//             arg0->unkC = func_15043A20(arg0->unk0, arg0->unk4, off, (s32)arg1, arg2);
//         }
//     }
//     return 0;
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_70200/func_15043BB8.s")


#pragma GLOBAL_ASM("asm/nonmatchings/game_70200/func_15043CA4.s")
// NON-MATCHING: 4 long (0xec vs 0xe8) — extra `off` home puts `len` at 0x34;
// `beq`+nop vs ROM `bnel`+delay `lw $a0`. Reusing the offset local across jals
// adds $s2 (8 long). Nested/comma did not recover `bnel`. tip justreg_park
// s32 func_15043CA4(struct105 *arg0, void *arg1, s32 arg2) {
//     s32 len;
//     s32 off;
//
//     len = 0;
//     off = arg0->unk8;
//     if (off == arg0->unkC) {
//         return 0;
//     }
//     off = func_15043AC8(arg0->unk0, arg0->unk4, off, (s32)&len, 4);
//     if (arg2 < len) {
//         arg2--;
//         off = func_15043AC8(arg0->unk0, arg0->unk4, off, (s32)arg1, arg2);
//         ((u8 *)arg1)[arg2] = 0;
//         off = func_15043B70(arg0->unk0, arg0->unk4, off, len - arg2);
//     } else if (len != 0) {
//         off = func_15043AC8(arg0->unk0, arg0->unk4, off, (s32)arg1, len);
//     }
//     arg0->unk8 = off;
//     return len;
// }
