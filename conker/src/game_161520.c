#include <ultra64.h>
#include "functions.h"
#include "variables.h"

extern f32 D_800A45B0;
extern s32 D_800BE9E4;
extern void *func_15152190(void *arg0, void *arg1, void *arg2, s32 arg3, f32 arg4, s32 arg5, u8 arg6, s32 arg7);
extern void func_151D9014(f32 *arg0, f32 *arg1, s32 arg2, f32 arg3, s16 arg4, u8 arg5, f32 arg6, s32 arg7, f32 arg8, f32 arg9, s32 argA, s32 argB, s32 argC, s32 argD, u8 argE, u8 argF);
extern void func_15153F18(void *arg0, void *arg1, s32 arg2, u8 arg3, s32 arg4);
extern f32 D_800A4624;
extern f32 D_800A4628;
extern f32 D_800A462C;
extern f32 D_800A482C;
extern f32 D_800A4830;
extern f32 D_800A4960;
extern f32 D_800A4964;
extern f32 D_800A4968;
extern f32 D_800A496C;
extern f32 D_800A4950;
extern f32 D_800A4954;
extern f32 D_800A4958;
extern f32 D_800A495C;
extern u8 D_800A4268[];
extern u8 D_800A4270[];
extern u8 D_800A4260[];
extern u8 D_800A4264[];
extern void *func_15167A68(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, s32 arg5);
extern f32 D_800A461C;
extern f32 D_800A4620;
extern void (*D_80089B70[6])(struct102 *arg0);
extern void (*D_80089B88[6])(struct102 *arg0);
extern void func_15145EA4(s32 *arg0, s32 *arg1, s32 arg2, s32 arg3);
extern void func_15143134(void *arg0, void *arg1, s32 arg2);
extern u8 D_800A3FD8[];
extern f32 D_800BE9A4;
extern f32 D_800A4828;
extern void func_151BC5A4(struct102 *arg0, void *arg1, u8 arg2);

// NON-MATCHING: jtbl jtbl_800A4350 / jtbl_800A4554 — defer Wave 7
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15134070.s")

s32 func_1513416C(struct102 *arg0) {
    s16 temp_v0 = arg0->unk1C;
    if (temp_v0 < 32) {
        arg0->unk28 = temp_v0 * 8;
    }
    return 1;
}

// NON-MATCHING: JUSTREG 75/76 — tip justreg_park; addu operand order only
// void *func_1513418C(void *arg0, s32 arg1, u8 arg2, s32 arg3) {
//     void *temp_ret;
//     u8 temp_a0;
//     void *temp_v0_2;
//     s32 temp_v1;
// 
//     temp_ret = func_15167A68(0x28, arg3, arg1 + 0x58, 1, arg2, 1);
//     if (temp_ret == NULL) {
//         return NULL;
//     }
//     memcpy((void *)((s32)temp_ret + 0x10), arg0, 0x30);
//     temp_a0 = *(u8 *)((s32)temp_ret + 0x3A);
//     if ((temp_a0 & 2) != 0) {
//         temp_v0_2 = *(void **)((s32)temp_ret + 0x1C);
//         if ((*(s32 *)temp_v0_2 == 0) ||
//             (*(u8 *)((s32)temp_ret + 0x18) != *(u8 *)((s32)temp_v0_2 + 0x3B))) {
//             func_1516972C(temp_ret);
//             return NULL;
//         }
//         temp_v1 = *(s32 *)((s32)temp_v0_2 + 0x1D4);
//         if ((temp_v1 != 0) && ((*(u8 *)((s32)temp_v0_2 + 0x74) & 0xF) != 0xF)) {
//             func_15143134((void *)((s32)temp_ret + 0x24), (void *)((s32)temp_ret + 0x40),
//                           temp_v1 + (*(u8 *)((s32)temp_ret + 0x20) << 6));
//         } else {
//             *(u8 *)((s32)temp_ret + 0x3A) = temp_a0 | 8;
//         }
//     } else {
//         *(u8 *)((s32)temp_ret + 0x3A) = temp_a0 | 0x18;
//     }
//     *(f32 *)((s32)temp_ret + 0x50) = 0.0f;
//     *(f32 *)((s32)temp_ret + 0x4C) = 1.0f / (2.0f * *(f32 *)((s32)temp_ret + 0x30));
//     return temp_ret;
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_1513418C.s")

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_151342BC.s")

s32 func_151346D0(s32 arg0, struct102 *arg1, s32 arg2) {
    arg1->unk3A &= 0xFFEF;
    return arg0;
}

void func_151346EC(struct102 *arg0) {
    func_15169804(arg0);
}

void func_1513470C(struct102 *arg0) {
    func_15169824(arg0);
}

void func_1513472C(struct102 *arg0) {
    s32 idx = arg0->unk3D;
    if (idx < 0) {
        idx = 0;
    }
    if (idx >= 10) {
        idx = 0;
    }
    D_80089AAC[idx]();
}

void func_1513477C(struct102 *arg0) {
  s32 idx = arg0->unk3D;
  if (idx < 0) {
      idx = 0;
  }
  if (idx >= 10) {
      idx = 0;
  }
    D_80089AD4[idx]();
}

// NON-MATCHING: JUSTREG 68/73 — tip justreg_park; do not thrash
// void func_151347CC(struct102 *arg0, s32 *arg1, u8 arg2) {
//     if ((arg2 == 0) || (arg2 == 3)) {
//         if ((*(s32 *)((s32)arg0 + 0x1C) == arg1[0]) ||
//             (*(u8 *)((s32)arg0 + 0x18) == *(u8 *)((s32)arg1 + 4))) {
//             func_1516972C(arg0);
//         }
//     } else if (arg2 == 0x11) {
//         if (*(u8 *)((s32)arg0 + 0x3D) == 5) {
//             if ((*(s32 *)((s32)arg0 + 0x1C) == arg1[0]) ||
//                 (*(u8 *)((s32)arg0 + 0x18) == *(u8 *)((s32)arg1 + 4))) {
//                 func_1516972C(arg0);
//             }
//         }
//     } else if (arg2 == 0x16) {
//         if ((s32)arg1 == *(s32 *)((s32)arg0 + 0x1C)) {
//             func_1516972C(arg0);
//         }
//     } else if (arg2 == 0x2D) {
//         if (arg1[0] == *(s32 *)((s32)arg0 + 0x1C)) {
//             *(s32 *)((s32)arg0 + 0x1C) = arg1[1];
//             *(u8 *)((s32)arg0 + 0x18) = *(u8 *)((s32)arg1 + 9);
//         } else if (arg1[1] == *(s32 *)((s32)arg0 + 0x1C)) {
//             *(s32 *)((s32)arg0 + 0x1C) = arg1[0];
//             *(u8 *)((s32)arg0 + 0x18) = *(u8 *)((s32)arg1 + 8);
//         }
//     }
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_151347CC.s")

void func_151348F0(f32 arg0, f32 arg1, s32 arg2, s32 arg3) {
}

// NON-MATCHING: schedule 39/50 len=0xc8 — float div vs zero remat CSE; tip thrash
// void *func_15134908(void *arg0, s32 arg1, u8 arg2, s32 arg3) {
//     void *v1;
//     void *v0;
//     f32 *t0;
//     f32 *t1;
//     f32 *t2;
//     f32 f0;
//     f32 f18;
// 
//     v1 = func_15167A68(0x2A, arg3, arg1 + 0x40, 1, arg2, 1);
//     if (v1 == NULL) {
//         return NULL;
//     }
//     *(u8 *)((s32)arg0 + 0x16) |= 2;
//     memcpy((void *)((s32)v1 + 0x10), arg0, 0x1C);
//     v0 = v1;
//     f0 = *(f32 *)((s32)v0 + 0x1C);
//     t0 = *(f32 **)((s32)v0 + 0x10);
//     t1 = *(f32 **)((s32)v0 + 0x14);
//     f18 = 1.0f / (f0 + f0);
//     t2 = *(f32 **)((s32)v0 + 0x18);
//     *(f32 *)((s32)v0 + 0x2C) = *t0;
//     *(f32 *)((s32)v0 + 0x30) = *t1;
//     *(f32 *)((s32)v0 + 0x3C) = 0.0f;
//     *(f32 *)((s32)v0 + 0x34) = *t2;
//     *(f32 *)((s32)v0 + 0x38) = f18;
//     return v0;
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15134908.s")

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_151349D0.s")

void func_15134C98(struct102 *arg0, void *arg1, u8 arg2) {
    if (arg0->unk28 == 1) {
        func_151BC5A4(arg0, arg1, arg2);
    }
}

void func_15134CD4(f32 arg0, f32 arg1, s32 arg2, s32 arg3) {
}

s32 func_15134CEC(void *arg0) {
    s32 temp_v1;

    temp_v1 = *(u8 *)((s32)arg0 + 0x2E);
    *(f32 *)((s32)arg0 + 0x70) += 0.125f * D_800BE9A4;
    *(f32 *)((s32)arg0 + 0x74) += D_800A45B0 * D_800BE9A4;
    *(f32 *)((s32)arg0 + 0x14) += *(f32 *)((s32)arg0 + 0x70) * D_800BE9A4;
    *(f32 *)((s32)arg0 + 0x1C) += *(f32 *)((s32)arg0 + 0x74) * D_800BE9A4;
    if (130.0f < *(f32 *)((s32)arg0 + 0x14)) {
        return 0;
    }
    temp_v1 = temp_v1 - (D_800BE9E4 * 2);
    if (temp_v1 < 0) {
        return 0;
    }
    *(u8 *)((s32)arg0 + 0x2E) = temp_v1;
    return 1;
}

// NON-MATCHING: JUSTREG 35/39 — tip justreg_park; do not thrash
// void *func_15134DAC(void *arg0, s32 arg1) {
//     void *v1;
//     void *v0;
//     void *t8;
//     s16 t9;
// 
//     v1 = func_15167A68(0x29, 0, arg1 + 0x80, 1, 0xFF, 1);
//     if (v1 == 0) {
//         return 0;
//     }
//     memcpy((void *)((s32)v1 + 0x18), arg0, 0x3C);
//     t8 = arg0;
//     v0 = v1;
//     t9 = *(s16 *)((s32)t8 + 0x28);
//     *(s32 *)((s32)v0 + 0x10) = 1;
//     *(s16 *)((s32)v0 + 0x54) = -t9;
//     *(s32 *)((s32)v0 + 0x14) = 0;
//     *(f32 *)((s32)v0 + 0x70) = 0.0f;
//     *(f32 *)((s32)v0 + 0x74) = 0.0f;
//     *(f32 *)((s32)v0 + 0x78) = 0.0f;
//     return v0;
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15134DAC.s")

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15134E48.s")

void func_151352EC(struct102 *arg0) {
    func_15169804(arg0);
}

void func_1513530C(struct102 *arg0) {
    func_15169824(arg0);
}

void func_1513532C(struct102 *arg0) {
    void (**tbl)(struct102 *) = D_80089B70;
    s32 idx = *(u8 *)((s32)arg0 + 0x50);
    u16 tmp;

    if (idx < 0) {
        idx = 0;
    } else if (idx >= 6) {
        idx = 0;
    }
    tmp = *(u16 *)((s32)arg0 + 0x44);
    if (tmp != 0) {
        func_100111C8(tmp);
        *(u16 *)((s32)arg0 + 0x44) = 0;
    }
    tbl[idx](arg0);
}


void func_151353A8(struct102 *arg0) {
    void (**tbl)(struct102 *) = D_80089B88;
    s32 idx = *(u8 *)((s32)arg0 + 0x50);
    u16 tmp;

    if (idx < 0) {
        idx = 0;
    } else if (idx >= 6) {
        idx = 0;
    }
    tmp = *(u16 *)((s32)arg0 + 0x44);
    if (tmp != 0) {
        func_100111C8(tmp);
        *(u16 *)((s32)arg0 + 0x44) = 0;
    }
    tbl[idx](arg0);
}


void func_15135424(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
    s32 sp20[2];
    s32 sp18[2];

    sp20[0] = arg1;
    sp20[1] = arg2;
    sp18[0] = arg3;
    sp18[1] = arg4;
    func_15145EA4(sp20, sp18, arg0, 2);
}


// NON-MATCHING: length 0xd8/0xdc — missing insn vs driver shape; tip length_schedule
// void func_15135480(struct102 *arg0, s32 *arg1, u8 arg2) {
//     u8 v0;
// 
//     if (arg2 == 0x2D) {
//         if (arg1[0] == *(s32 *)((s32)arg0 + 0x1C)) {
//             *(s32 *)((s32)arg0 + 0x1C) = arg1[1];
//             *(u8 *)((s32)arg0 + 0x18) = *(u8 *)((s32)arg1 + 9);
//         } else if (arg1[1] == *(s32 *)((s32)arg0 + 0x1C)) {
//             *(s32 *)((s32)arg0 + 0x1C) = arg1[0];
//             *(u8 *)((s32)arg0 + 0x18) = *(u8 *)((s32)arg1 + 8);
//         }
//     }
//     v0 = *(u8 *)((s32)arg0 + 0x50);
//     if (v0 == 1) {
//         func_151355B8(arg0, arg1, arg2);
//     } else if (v0 == 2) {
//         func_1513555C(arg0, arg1, arg2);
//     } else if (arg2 == 0) {
//         if ((*(s32 *)((s32)arg0 + 0x1C) == arg1[0]) ||
//             (*(u8 *)((s32)arg1 + 4) == *(u8 *)((s32)arg0 + 0x18))) {
//             func_1516972C(arg0);
//         }
//     }
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15135480.s")

void func_1513555C(struct102 *arg0, s32 *arg1, u8 arg2) {
    if ((arg2 == 0) || (arg2 == 0x12)) {
        if ((arg1[0] == *(s32 *)((s32)arg0 + 0x1C)) ||
            (*(u8 *)((s32)arg1 + 4) == *(u8 *)((s32)arg0 + 0x18))) {
            func_1516972C(arg0);
        }
    }
}


// NON-MATCHING: schedule 10/40 len=0xa0 — beqz/beql vs else-if; tip thrash
// void func_151355B8(struct102 *arg0, s32 *arg1, u8 arg2) {
//     if (arg2 == 0) {
//         if ((arg1[0] == *(s32 *)((s32)arg0 + 0x1C)) ||
//             (*(u8 *)((s32)arg1 + 4) == *(u8 *)((s32)arg0 + 0x18))) {
//             func_1516972C(arg0);
//         }
//     } else if (arg2 == 3) {
//         if ((arg1[0] == *(s32 *)((s32)arg0 + 0x1C)) ||
//             (*(u8 *)((s32)arg1 + 4) == *(u8 *)((s32)arg0 + 0x18))) {
//             *(s32 *)((s32)arg0 + 0x10) &= ~1;
//             *(s32 *)((s32)arg0 + 0x10) &= ~1;
//         }
//     }
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_151355B8.s")

s32 func_15135658(struct259 *arg0) {
    s32 ret = 1;

    arg0->unk74 = 1.0f;
    return ret;
}

f32 func_15135670(s32 arg0) {
    // "power", "../Effects/Blood/blood.c"
    return func_151422DC(0, &D_800A3FB4, 0, 2000, 1000, &D_800A3FBC, 2938) * D_800A45B4;
}

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_151356D4.s")

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
// s32 func_15135BF8(void *arg0) {
//     s32 ret = 1;
//     void *ptr;
//     f32 sp8;
//     f32 spC;
//     f32 temp;
// 
//     if (*(s16 *)((s32)arg0 + 0x70) > 0) {
//         ptr = (void *)((s32)arg0 + 0x70);
//         *(s16 *)ptr = *(s16 *)ptr - D_800BE9E4;
//         *(f32 *)((s32)arg0 + 0x10) += *(f32 *)((s32)ptr + 8) * D_800BE9A4;
//         *(f32 *)((s32)arg0 + 0x14) += *(f32 *)((s32)ptr + 0xC) * D_800BE9A4;
//         *(f32 *)((s32)arg0 + 0x18) += *(f32 *)((s32)ptr + 4) * D_800BE9A4;
//         *(f32 *)((s32)arg0 + 0x1C) += *(f32 *)((s32)ptr + 4) * D_800BE9A4;
//     } else {
//         ptr = (void *)((s32)arg0 + 0x70);
//         sp8 = *(f32 *)((s32)ptr + 8);
//         spC = *(f32 *)((s32)ptr + 0xC);
//         *(f32 *)((s32)arg0 + 0x14) +=
//             (*(f32 *)((s32)ptr + 0xC) * D_800BE9A4) +
//             (((*(f32 *)((s32)ptr + 0x10) * D_800BE9A4) * D_800BE9A4) * 0.5f);
//         *(f32 *)((s32)ptr + 0xC) += *(f32 *)((s32)ptr + 0x10) * D_800BE9A4;
//         sp8 = sp8 + *(f32 *)((s32)ptr + 8);
//         spC = spC + *(f32 *)((s32)ptr + 0xC);
//         sp8 = sp8 * 0.5f;
//         spC = spC * 0.5f;
//         temp = fabsf(sp8) * *(f32 *)((s32)ptr + 0x14);
//         *(f32 *)((s32)arg0 + 0x18) += temp * D_800BE9A4;
//         *(f32 *)((s32)arg0 + 0x1C) += temp * D_800BE9A4;
//     }
// 
//     temp = *(f32 *)((s32)arg0 + 0x10);
//     if ((200.0f < temp) || (temp < -200.0f)) {
//         ret = 0;
//     } else {
//         temp = *(f32 *)((s32)arg0 + 0x14);
//         if ((200.0f < temp) || (temp < -200.0f)) {
//             ret = 0;
//         }
//     }
//     return ret;
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15135BF8.s")

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15135DD0.s")

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15136404.s")

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15136698.s")

s32 func_151368A8(struct102 *arg0) {
    s16 tmp = *(s16 *)((s32)arg0 + 0x1C);
    f32 *ptr = (f32 *)((s32)arg0 + 0x128);

    if (tmp < 0x20) {
        s32 value = tmp * 8;
        if (value < *(u8 *)((s32)arg0 + 0x5C)) {
            *(u8 *)((s32)arg0 + 0x5C) = value;
        }
    }
    *(f32 *)((s32)arg0 + 0x2C) += *ptr * D_800BE9A4;
    *(f32 *)((s32)arg0 + 0x30) += *ptr * D_800BE9A4;
    return 1;
}


// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
// extern struct210 *func_1513C73C(s32 arg0, u8 arg1, u8 arg2, s32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7, f32 arg8, u8 arg9, u8 argA, s32 argB, u8 argC, s32 argD);
// 
// void func_15136918(f32 *arg0, u8 arg1, u8 arg2, s16 arg3, s16 arg4, u8 arg5, s32 arg6) {
//     u8 sp4E;
//     s32 sp48;
//     s32 sp50;
//     s32 sp54;
//     u8 sp58;
//     u8 sp59;
//     u8 sp5A;
//     u8 sp5B;
//     u8 sp5C;
//     u8 sp5D;
//     s32 sp60;
//     u8 sp5E;
//     u8 sp5F;
//     s16 sp4C;
//     f32 sp10;
//     f32 sp14;
//     f32 sp18;
//     f32 sp20;
//     f32 sp1C;
//     s32 sp2C;
//     s32 sp28;
//     s32 sp24;
// 
//     sp4E = 0x55;
//     sp48 = 0x300;
//     sp50 = 0;
//     sp54 = 0;
//     sp58 = arg1;
//     sp59 = arg2;
//     sp5A = 0;
//     sp5B = 0;
//     sp5C = 0;
//     sp5D = 0;
//     sp60 = 0x280001;
//     sp5E = 1;
//     sp5F = 1;
//     if (arg3 == -1) {
//         sp4C = 0x12C;
//     } else {
//         sp48 = 0x301;
//         sp4C = arg3 + 0x20;
//     }
//     sp10 = arg0[0];
//     sp14 = arg0[1];
//     sp18 = arg0[2];
//     sp20 = 0.0f;
//     sp1C = 0.0f;
//     sp2C = 0;
//     sp28 = 0;
//     sp24 = arg4;
//     func_1513C73C((s32) &sp48, 0xD, 0, arg6, sp10, sp14, sp18, sp1C, sp20, arg5, 0, 0, arg4, arg6);
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15136918.s")

s32 func_15136A1C(struct102 *arg0) {
    s16 idx = arg0->unk1C;

    if (idx < 32) {
        s32 value = idx * 8;
        if (value < arg0->unk28) {
            arg0->unk28 = value;
        }
    }
    return 1;
}

// NON-MATCHING: JUSTREG 21/37 — tip justreg_park; do not thrash
// typedef struct {
//     s32 unk0;
//     s32 unk4;
//     s32 unk8;
//     f32 unkC;
//     f32 unk10;
//     s16 unk14;
//     u8 unk16;
//     u8 unk17;
//     u8 unk18;
//     s8 unk19;
// } Sp36A50;
// 
// void *func_15136A50(s32 arg0, s32 arg1, s32 arg2, s16 arg3, u8 arg4, s32 arg5) {
//     Sp36A50 sp1C;
// 
//     sp1C.unk16 = 5;
//     sp1C.unk17 = 5;
//     sp1C.unk18 = 2;
//     sp1C.unk19 = -1;
//     sp1C.unkC = D_800A461C;
//     sp1C.unk0 = arg0;
//     sp1C.unk4 = arg1;
//     sp1C.unk8 = arg2;
//     sp1C.unk10 = D_800A4620;
//     sp1C.unk14 = arg3;
//     return func_15134908(&sp1C, 0, arg4, arg5);
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15136A50.s")

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
// extern f32 D_800A4624;
// extern f32 D_800A4628;
// extern f32 D_800A462C;
// extern void func_151D9014(void);
// 
// void func_15136AE4(f32 arg0, f32 arg1, f32 arg2, f32 arg3, struct102 *arg4, f32 arg4f) {
//     f32 sp74;
//     f32 sp6C;
//     f32 sp70;
//     f32 sp60[3];
//     f32 sp4C;
//     s32 sp50;
//     s32 sp54;
//     f32 sp18;
//     f32 sp20;
//     f32 sp24;
//     s32 sp30;
//     s32 sp28;
//     s32 sp1C;
//     s32 sp2C;
//     s32 sp34;
//     s32 sp10;
//     s32 sp14;
//     u8 sp38;
//     u8 sp3C;
//     f32 tmp;
// 
//     sp74 = arg0;
//     sp6C = arg1;
//     sp70 = arg2;
//     func_150ADA68();
//     tmp = (func_150ADA68() * 112.0f) + 247.0f;
//     sp60[0] = -arg3 * tmp * D_800A4624;
//     sp60[1] = -arg1 * tmp * D_800A4624;
//     sp60[2] = -arg2 * tmp * D_800A4624;
//     func_150ADA68();
//     func_150ADA20();
//     sp50 = func_150ADA20();
//     func_150ADA68();
//     sp54 = func_150ADA20();
//     sp30 = 1;
//     sp28 = 1;
//     sp1C = 0;
//     sp2C = 0;
//     sp34 = 0;
//     sp20 = 1.0f;
//     sp24 = 1.0f;
//     sp4C = func_150ADA20();
//     sp18 = (sp54 / 0x65) * D_800A4628 * sp4C + D_800A462C;
//     sp10 = (sp50 & 0xF) + 0x19;
//     sp14 = (sp54 % 0x65) + 0x9B;
//     sp38 = *(u8 *)((s32) arg4 + 0xC);
//     sp3C = *(u8 *)((s32) arg4 + 1);
//     func_151D9014();
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15136AE4.s")

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15136C3C.s")

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15136F50.s")

// NON-MATCHING: jtbl jtbl_800A4650 — defer Wave 7
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15137610.s")

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_1513783C.s")

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
// s32 func_15137C64(f32 *arg0, f32 *arg1, f32 *arg2, f32 *arg3, void *arg4, f32 *arg5, f32 *arg6, f32 *arg7) {
//     s32 sp2C;
//     s32 sp28;
// 
//     if ((arg5 != NULL) && (arg6 != NULL)) {
//         arg0[0] = arg5[0];
//         arg0[1] = arg5[1];
//         arg0[2] = arg5[2];
//         arg1[0] = arg6[0];
//         arg1[1] = arg6[1];
//         arg1[2] = arg6[2];
//     } else if (arg5 != NULL) {
//         arg0[0] = arg5[0];
//         arg0[1] = arg5[1];
//         arg0[2] = arg5[2];
//         arg1[0] = arg5[0];
//         arg1[1] = arg5[1];
//         arg1[2] = arg5[2];
//     } else if (arg6 != NULL) {
//         arg0[0] = arg6[0];
//         arg0[1] = arg6[1];
//         arg0[2] = arg6[2];
//         arg1[0] = arg6[0];
//         arg1[1] = arg6[1];
//         arg1[2] = arg6[2];
//     } else {
//         return 0;
//     }
// 
//     if (arg7 != NULL) {
//         arg2[0] = arg7[0];
//         arg2[1] = arg7[1];
//         arg2[2] = arg7[2];
//     } else {
//         arg2[0] = arg1[0] - arg0[0];
//         arg2[1] = arg1[1] - arg0[1];
//         arg2[2] = arg1[2] - arg0[2];
//         if (func_15145128(arg2, arg2, &sp2C, &sp28) == 0) {
//             return 0;
//         }
//     }
// 
//     if (func_15146078(arg2, arg3, arg4) != 0) {
//         return 1;
//     }
//     return 2;
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15137C64.s")

s32 func_15137E10(struct259 *arg0) {
    arg0->unk74 = ((func_150ADA68() * 50.0f) + 580.0f) * D_800A4828;
    return 1;
}

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
// void func_15137F30(f32 *arg0, f32 *arg1, f32 *arg2, struct102 *arg3, f32 *arg4, s16 *arg5, u8 *arg6, f32 *arg7);
// 
// void func_15137E60(f32 *arg0, f32 *arg1, f32 *arg2, struct102 *arg3, f32 arg4, struct102 *arg5) {
//     f32 sp10;
//     f32 sp48;
//     f32 sp6C[3];
//     f32 sp54[3];
//     s16 sp4E;
//     u8 sp4D;
//     s32 sp30;
//     s32 sp28;
//     s32 sp1C;
//     s32 sp2C;
//     s32 sp34;
//     f32 sp20;
//     f32 sp24;
//     f32 sp18;
//     u8 sp38;
//     u8 sp3C;
// 
//     sp10 = arg4;
//     func_15137F30(arg0, arg1, arg2, arg3, arg0, &sp4E, &sp4D, &sp48);
//     sp30 = 1;
//     sp28 = 1;
//     sp1C = 0;
//     sp2C = 0;
//     sp34 = 0;
//     sp20 = 1.0f;
//     sp24 = 1.0f;
//     sp18 = sp48;
//     sp38 = *(u8 *)((s32) arg5 + 0xC);
//     sp3C = *(u8 *)((s32) arg5 + 1);
//     func_151D9014(sp6C, sp54, 0, arg5, sp4E, sp4D, sp18, sp30, sp28, sp1C, sp2C, sp34, sp20, sp24, sp38, sp3C);
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15137E60.s")

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
// extern f32 D_800A482C;
// extern void func_151D9014(void);
// 
// void func_15137F30(f32 *arg0, f32 *arg1, f32 *arg2, struct102 *arg3, f32 *arg4, s16 *arg5, u8 *arg6, f32 *arg7) {
//     f32 scale;
//     f32 sp74;
//     f32 a;
//     f32 b;
//     f32 c;
//     f32 d;
//     f32 e;
//     f32 f;
//     f32 r0;
//     f32 r1;
//     f32 r2;
//     s32 t0;
//     s32 t1;
// 
//     scale = arg7[0];
//     sp74 = arg3->unk74;
//     a = arg0[0] + (arg2[0] * scale);
//     arg4[0] = a;
//     b = arg0[1] + (arg2[1] * scale);
//     arg4[1] = b;
//     c = arg0[2] + (arg2[2] * scale);
//     arg4[2] = c;
//     d = arg1[0] + (arg3[0] * scale);
//     arg5[0] = d;
//     e = arg1[1] + (arg3[1] * scale);
//     arg5[1] = e;
//     f = arg1[2] + (arg3[2] * scale);
//     arg5[2] = f;
//     r0 = (d - a) * sp74;
//     arg6[0] = r0;
//     r1 = (e - b) * sp74;
//     arg6[1] = r1;
//     r2 = (f - c) * sp74;
//     arg6[2] = r2;
//     func_150ADA68();
//     arg7[0] = ((func_150ADA68() * 217.0f) - 456.0f) * D_800A482C;
//     func_150ADA20();
//     t0 = func_150ADA20();
//     arg5[0] = (t0 % 0x1F) + 0x1E;
//     t1 = func_150ADA68();
//     arg6[0] = (t1 % 0x9C) + 0x64;
//     arg7[0] = (func_150ADA68() * 34.0f) + 36.0f;
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15137F30.s")

s32 func_151380B4(struct102 *arg0, s32 arg1, s32 arg2) {
    s32 v0 = *(s32 *)((s32)arg0 + 0x1D4);

    if (v0 == 0) {
        return 0;
    }
    if ((*(u8 *)((s32)arg0 + 0x74) & 0xF) == 0xF) {
        return 0;
    }
    func_15143134(&D_800A3FD8[arg1 * 0x10], (void *)arg2, v0 + 0x300);
    return 1;
}


// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
// extern u8 D_800A3FD8[];
// extern u8 D_800A4058[];
// extern u8 D_800A4068[];
// extern void *func_15134DAC(void *arg0, s32 arg1);
// 
// void func_15138120(struct102 *arg0, s32 arg1, u8 arg2) {
//     u8 *tbl;
//     u8 sp4C[0x40];
//     s16 sp78;
//     u8 sp54;
//     u8 sp70;
//     s16 sp72;
//     s16 sp74;
//     s16 sp76;
//     f32 sp58;
//     f32 sp5C;
//     f32 sp60;
//     f32 sp64;
//     f32 sp6C;
//     f32 sp68;
//     f32 sp80;
//     u8 sp7A;
//     u8 sp7B;
//     u8 sp7C;
//     u8 sp84;
//     u8 sp85;
//     s32 sp20;
//     s32 sp14;
//     s32 sp18;
//     s32 sp10;
//     void *sp1C;
//     s16 x;
//     s16 y;
//     s16 z;
// 
//     tbl = &D_800A3FD8[arg1 * 0x10];
//     if (tbl[0xE] == 2) {
//         return;
//     }
//     if (arg2 != 0) {
//         sp54 = 0xC;
//     } else {
//         sp54 = 1;
//     }
//     sp70 = 2;
//     sp72 = 0x28;
//     sp74 = 0x10;
//     sp58 = 0.0f;
//     sp5C = 0.0f;
//     sp60 = 0.0f;
//     sp64 = 0.0f;
//     sp6C = 0.0f;
//     sp68 = 20.0f;
//     if (tbl == D_800A4058) {
//         if ((*(s32 *)((s32) arg0 + 0x94) & 0xE) != 0) {
//             sp76 = 0x78;
//         } else {
//             sp76 = 0xF0;
//         }
//     } else if (tbl == D_800A4068) {
//         sp76 = 0x258;
//     } else {
//         sp76 = 0x258;
//     }
//     if (tbl[0xE] != 0) {
//         sp7B = 6;
//     } else {
//         sp7B = 5;
//     }
//     sp7C = -1;
//     sp84 = 0;
//     sp85 = -1;
//     sp80 = 1.0f;
//     x = (s16) *(f32 *)((s32) arg0 + 0x14);
//     y = (s16) *(f32 *)((s32) arg0 + 0x1C);
//     z = (s16) *(f32 *)((s32) arg0 + 0x18);
//     sp20 = 0x78;
//     sp14 = 0x258;
//     sp18 = 0x12C;
//     sp10 = 0x5DC0;
//     sp1C = func_1000EBC4;
//     sp78 = func_1000FA64(0x4FE, x, y, z, sp10, sp20, sp14, sp18, sp1C, sp28, sp24, sp1C);
//     func_15134DAC(sp4C, 0);
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15138120.s")

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
// extern f32 D_800A4830;
// extern f32 D_800A4834;
// extern f32 D_800A4838;
// extern f32 D_800A483C;
// extern f32 D_800A4840;
// extern u8 D_800A3FE6[];
// extern void func_15153F18(void *arg0, void *arg1, s32 arg2, u8 arg3, s32 arg4);
// 
// void func_151382E0(s32 *arg0, s32 arg1, s32 arg2, u8 arg3, s32 arg4) {
//     u8 idx;
//     s32 sp2C[3];
//     s16 sp24;
//     s16 sp26;
//     s16 sp28;
//     s16 sp2A;
//     s16 sp50;
//     s16 sp52;
//     s16 sp54;
//     s16 sp56;
//     s16 sp58;
//     s16 sp5A;
//     s16 sp5C;
//     s16 sp5E;
//     s16 sp68;
//     s16 sp6A;
//     f32 sp38;
//     f32 sp3C;
//     f32 sp40;
//     f32 sp44;
//     f32 sp48;
//     f32 sp4C;
//     f32 sp64;
//     u8 sp60;
//     s32 sp6C;
//     s32 sp10;
// 
//     idx = D_800A3FE6[arg1 * 0x10 + 6];
//     if (idx == 2) {
//         return;
//     }
//     sp2C[0] = arg0[0];
//     sp2C[1] = arg0[1];
//     sp2C[2] = arg0[2];
//     sp24 = 0;
//     sp26 = 0xFF;
//     sp28 = -0x3F;
//     sp2A = 0x4E;
//     sp50 = 0x12;
//     sp52 = 7;
//     sp38 = D_800A4830;
//     sp3C = D_800A4834;
//     sp40 = D_800A4838;
//     sp44 = D_800A483C;
//     sp48 = 4.0f;
//     sp4C = 9.0f;
//     sp64 = D_800A4840;
//     sp54 = 3;
//     sp56 = 3;
//     sp58 = 0x14;
//     sp5A = 0x1E;
//     sp5C = 0x9B;
//     sp5E = 0x64;
//     sp68 = 0x10;
//     sp6A = 0xF;
//     sp6C = 0;
//     sp60 = (idx == 1) ? 1 : 0;
//     sp10 = arg4;
//     func_15153F18(&sp24, sp2C, arg2, arg3, arg4);
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_151382E0.s")

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15138424.s")

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
// s32 func_151389A8(void *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5) {
//     f32 sp54;
//     f32 sp58;
//     f32 sp5C;
//     f32 temp_f0;
//     s32 sp4C;
//     s32 sp44;
//     f32 temp;
//     s32 temp_v0;
// 
//     temp = *(f32 *)((s32)arg0 + 0x14);
//     *(f32 *)((s32)arg0 + 0x48) = *(f32 *)((s32)arg0 + 0x48) * -temp;
//     *(f32 *)((s32)arg0 + 0x3C) = *(f32 *)((s32)arg0 + 0x10) + arg4;
//     *(f32 *)((s32)arg0 + 0x44) = *(f32 *)((s32)arg0 + 0x44) * temp;
//     *(f32 *)((s32)arg0 + 0x4C) = *(f32 *)((s32)arg0 + 0x4C) * temp;
//     *(f32 *)((s32)arg0 + 0x50) = *(f32 *)((s32)arg0 + 0x50) * temp;
//     *(f32 *)((s32)arg0 + 0x54) = *(f32 *)((s32)arg0 + 0x54) * temp;
//     *(f32 *)((s32)arg0 + 0x58) = *(f32 *)((s32)arg0 + 0x58) * temp;
// 
//     if (fabsf(*(f32 *)((s32)arg0 + 0x48)) < 4.0f) {
//         *(f32 *)((s32)arg0 + 0x44) = 0.0f;
//         *(s32 *)((s32)arg0 + 0x60) &= ~0x69;
//         *(f32 *)((s32)arg0 + 0x48) = 0.0f;
//         *(f32 *)((s32)arg0 + 0x4C) = 0.0f;
//         *(f32 *)((s32)arg0 + 0x50) = 0.0f;
//         *(f32 *)((s32)arg0 + 0x54) = 0.0f;
//         *(f32 *)((s32)arg0 + 0x58) = 0.0f;
//         *(f32 *)((s32)arg0 + 0x5C) = 0.0f;
//     }
// 
//     if (*(f32 *)((s32)arg0 + 0x30) == 2.0f) {
//         return 1;
//     }
// 
//     sp54 = *(f32 *)((s32)arg0 + 0x38);
//     sp58 = arg4;
//     sp5C = *(f32 *)((s32)arg0 + 0x40);
//     if (*(f32 *)((s32)arg0 + 0x30) == 0.0f) {
//         sp44 = 0;
//     } else {
//         sp44 = 1;
//     }
// 
//     temp_f0 = func_150ADA68();
//     sp4C = func_150ADA20();
//     temp_v0 = func_150ADA20();
//     temp =
//         ((temp_f0 * D_800A486C) + D_800A4870) *
//         ((*(f32 *)((s32)arg0 + 0x18) + *(f32 *)((s32)arg0 + 0x1C)) * 0.5f);
//     func_151D9B8C(
//         sp44,
//         temp,
//         (sp4C % 0x65U) + 0x64,
//         arg5,
//         (struct17 *)&sp54,
//         (temp_v0 % 0x65U) + 0x50,
//         1,
//         1,
//         0,
//         *(u8 *)((s32)arg0 + 0xC),
//         *(u8 *)((s32)arg0 + 1));
//     *(f32 *)((s32)arg0 + 0x2C) = 0.0f;
//     return 1;
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_151389A8.s")

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
// void func_15138BC0(void *arg0, u8 arg1, s32 arg2) {
//     s32 code;
//     u8 flag;
//     u8 sp28[0x28];
//     f32 sp50[3];
// 
//     code = func_15134070(arg0);
//     if (code != 0x63) {
//         flag = func_151380B4(arg0, code, (s32)sp50);
//         func_15138120(arg0, code, 1);
//         if (flag != 0) {
//             func_1504715C(sp28, arg0);
//             func_151382E0(sp50, code, (s32)sp28, arg1, arg2);
//             func_15138424(arg0, sp50, code, sp28, arg1, arg2);
//         }
//     }
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15138BC0.s")

// NON-MATCHING: jtbl jtbl_800A4874 — defer Wave 7
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15138C80.s")

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15138E98.s")

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
// s32 func_15139578(void *arg0, s32 arg1, s32 arg2, s32 arg3, f32 arg4, s32 arg5) {
//     f32 sp54;
//     f32 sp58;
//     f32 sp5C;
//     f32 temp_f0;
//     s32 sp4C;
//     s32 sp44;
//     f32 temp;
//     s32 temp_v0;
// 
//     temp = *(f32 *)((s32)arg0 + 0x14);
//     *(f32 *)((s32)arg0 + 0x48) = *(f32 *)((s32)arg0 + 0x48) * -temp;
//     *(f32 *)((s32)arg0 + 0x3C) = *(f32 *)((s32)arg0 + 0x10) + arg4;
//     *(f32 *)((s32)arg0 + 0x44) = *(f32 *)((s32)arg0 + 0x44) * temp;
//     *(f32 *)((s32)arg0 + 0x4C) = *(f32 *)((s32)arg0 + 0x4C) * temp;
//     *(f32 *)((s32)arg0 + 0x50) = *(f32 *)((s32)arg0 + 0x50) * temp;
//     *(f32 *)((s32)arg0 + 0x54) = *(f32 *)((s32)arg0 + 0x54) * temp;
//     *(f32 *)((s32)arg0 + 0x58) = *(f32 *)((s32)arg0 + 0x58) * temp;
// 
//     if (fabsf(*(f32 *)((s32)arg0 + 0x48)) < 4.0f) {
//         *(f32 *)((s32)arg0 + 0x44) = 0.0f;
//         *(s32 *)((s32)arg0 + 0x60) &= ~0x69;
//         *(f32 *)((s32)arg0 + 0x48) = 0.0f;
//         *(f32 *)((s32)arg0 + 0x4C) = 0.0f;
//         *(f32 *)((s32)arg0 + 0x50) = 0.0f;
//         *(f32 *)((s32)arg0 + 0x54) = 0.0f;
//         *(f32 *)((s32)arg0 + 0x58) = 0.0f;
//         *(f32 *)((s32)arg0 + 0x5C) = 0.0f;
//     }
// 
//     if (*(f32 *)((s32)arg0 + 0x30) == 2.0f) {
//         return 1;
//     }
// 
//     sp54 = *(f32 *)((s32)arg0 + 0x38);
//     sp58 = arg4;
//     sp5C = *(f32 *)((s32)arg0 + 0x40);
//     if (*(f32 *)((s32)arg0 + 0x30) == 0.0f) {
//         sp44 = 0;
//     } else {
//         sp44 = 1;
//     }
// 
//     temp_f0 = func_150ADA68();
//     sp4C = func_150ADA20();
//     temp_v0 = func_150ADA20();
//     temp =
//         ((temp_f0 * D_800A48F8) + D_800A48FC) *
//         ((*(f32 *)((s32)arg0 + 0x18) + *(f32 *)((s32)arg0 + 0x1C)) * 0.5f);
//     func_151D9B8C(
//         sp44,
//         temp,
//         (sp4C % 0x65U) + 0x64,
//         arg5,
//         (struct17 *)&sp54,
//         (temp_v0 % 0x90U) + 0x50,
//         1,
//         1,
//         0,
//         *(u8 *)((s32)arg0 + 0xC),
//         *(u8 *)((s32)arg0 + 1));
//     return 1;
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15139578.s")

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15139768.s")

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_15139D74.s")

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_1513A24C.s")

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
// extern f32 D_800A4950;
// extern f32 D_800A4954;
// extern f32 D_800A4958;
// extern f32 D_800A495C;
// extern u8 D_800A4260[];
// extern u8 D_800A4264[];
// extern void func_15152190(void *arg0, void *arg1, void *arg2, s32 arg3, f32 arg4, u8 arg5, s32 arg6);
// 
// void func_1513A48C(s32 *arg0, u8 arg1, s32 arg2) {
//     s32 sp2C;
//     s32 sp30;
//     s32 sp34;
//     s32 sp38;
//     s32 sp3C;
//     s16 sp40;
//     s16 sp42;
//     s16 sp44;
//     s16 sp46;
//     f32 sp48;
//     f32 sp4C;
//     f32 sp50;
//     f32 sp54;
//     s16 sp58;
//     s16 sp5A;
//     f32 sp5C;
//     f32 sp60;
//     f32 sp64;
//     s32 sp14;
//     f32 sp10;
//     u8 sp18;
//     s32 sp1C;
// 
//     sp2C = 8;
//     sp30 = 4;
//     sp34 = arg0[0];
//     sp38 = arg0[1];
//     sp3C = arg0[2];
//     sp40 = 0;
//     sp42 = 0xFF;
//     sp44 = -0x37;
//     sp46 = 0x20;
//     sp48 = 10.0f;
//     sp4C = 9.0f;
//     sp50 = D_800A4954;
//     sp54 = D_800A4958;
//     sp58 = 0x28;
//     sp5A = 0x14;
//     sp5C = D_800A4950;
//     sp60 = D_800A4950;
//     sp64 = D_800A495C;
//     sp14 = 1;
//     sp10 = 0.0f;
//     sp18 = arg1;
//     sp1C = arg2;
//     func_15152190(&sp2C, D_800A4260, D_800A4264, 1, sp10, sp18, sp1C);
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_1513A48C.s")

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
// extern void func_1513A5E0(s32 *arg0, u8 arg1, s32 arg2);
// 
// void func_1513A594(struct102 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4) {
//     s32 t6;
//     s32 a3c;
// 
//     t6 = arg3 & 0xFF;
//     a3c = t6;
//     func_1513A5E0((s32 *) arg1, a3c, arg4);
//     if (*(s32 *)((s32) arg0 + 0x1D4) == 0) {
//         return;
//     }
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_1513A594.s")

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
// extern f32 D_800A4960;
// extern f32 D_800A4964;
// extern f32 D_800A4968;
// extern f32 D_800A496C;
// extern u8 D_800A4268[];
// extern u8 D_800A4270[];
// extern void func_15152190(void *arg0, void *arg1, void *arg2, s32 arg3, f32 arg4, u8 arg5, s32 arg6);
// 
// void func_1513A5E0(s32 *arg0, u8 arg1, s32 arg2) {
//     s32 sp2C;
//     s32 sp30;
//     s32 sp34;
//     s32 sp38;
//     s32 sp3C;
//     s16 sp40;
//     s16 sp42;
//     s16 sp44;
//     s16 sp46;
//     f32 sp48;
//     f32 sp4C;
//     f32 sp50;
//     f32 sp54;
//     s16 sp58;
//     s16 sp5A;
//     f32 sp5C;
//     f32 sp60;
//     f32 sp64;
//     s32 sp14;
//     f32 sp10;
//     u8 sp18;
//     s32 sp1C;
// 
//     sp2C = 7;
//     sp30 = 7;
//     sp34 = arg0[0];
//     sp38 = arg0[1];
//     sp3C = arg0[2];
//     sp40 = 0;
//     sp42 = 0xFF;
//     sp44 = -0x32;
//     sp46 = 0x1B;
//     sp48 = 4.0f;
//     sp4C = 4.0f;
//     sp50 = D_800A4964;
//     sp54 = D_800A4968;
//     sp58 = 0x19;
//     sp5A = 0x28;
//     sp5C = D_800A4960;
//     sp60 = D_800A4960;
//     sp64 = D_800A496C;
//     sp14 = 1;
//     sp10 = 0.0f;
//     sp18 = arg1;
//     sp1C = arg2;
//     func_15152190(&sp2C, D_800A4268, D_800A4270, 2, sp10, sp18, sp1C);
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_1513A5E0.s")

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_1513A6E0.s")

// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_1513ABB8.s")

void func_1513B0B8(void *arg0, s32 arg1, u8 arg2) {
    s32 *p = (s32 *)((s32)arg0 + 0x170);

    if (arg2 == 0x45) {
        if (--(*p) < 0) {
            *(s32 *)((s32)arg0 + 0x60) |= 0x80;
        }
    }
}


// NON-MATCHING: parked — schedule/JUSTREG/length; leave asm
#pragma GLOBAL_ASM("asm/nonmatchings/game_161520/func_1513B0F8.s")
