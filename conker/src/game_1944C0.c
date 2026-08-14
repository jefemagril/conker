#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_15168B10(s32 arg0, s32 arg1);
void func_15168A4C(void *arg0, s32 arg1);
void *func_15167A68(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, u8 arg5);
void func_15169070(s32 arg0, s32 arg1, void *arg2, u8 arg3);
extern u8 D_800D2DAB;

typedef struct {
    u8 pad0[0xE4];
    u8 unkE4;
} Game1944C0Dispatch;

extern void (*D_8008CA20[])(Game1944C0Dispatch *);

typedef struct {
    u8 pad0[0x40];
    s32 unk40;
} Game1944C0CopySrc;

typedef struct {
    u8 pad0[0x90];
    u8 unk90[0x60];
} Game1944C0CopyDst;

typedef struct {
    u8 pad0[0x3B];
    u8 unk3B;
} Game1944C0Payload;

typedef struct {
    void *payload;
    u8 unk4;
} Game1944C0StackPayload;

typedef struct {
    u8 pad0[0xC];
    u8 unkC;
} Game1944C0EventObject;

typedef struct {
    u8 unk0;
} Game1944C0EventPayloadHeader;

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

void *func_15167A68(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4, u8 arg5) {
    void *v0 = (void *)func_10003C6C(arg2, 1, arg3, 0, arg5);

    if (v0 != NULL) {
        ((u8 *)v0)[1] = arg1;
        func_15168A4C(v0, arg0);
        ((u8 *)v0)[0xC] = arg4;
    }
    return v0;
}

void func_15167AD8(void *arg0, u8 arg1, s32 arg2) {
    void *v0 = func_15167A68(3, arg2, 0x28, 0, arg1, 1);

    if (v0 != NULL) {
        bcopy(arg0, (u8 *)v0 + 0x10, 0x18);
        ((u8 *)v0)[0x23] = 0xFF;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15167B44.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15167C58.s")
void *func_15167D84(void *arg0, s32 arg1, s32 arg2, s8 arg3, u8 arg4, s32 arg5) {
    void *v0 = func_15167A68(arg1 == 0 ? 5 : 0x42, arg5, arg2 + 0x50, 0, arg4, 1);

    if (v0 == NULL) {
        return v0;
    }
    bcopy(arg0, (u8 *)v0 + 0x10, 0x38);
    ((s8 *)v0)[0x48] = arg3;
    return v0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15167E0C.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15168118.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_1516865C.s")
void *func_15168800(void *arg0, u8 arg1, s32 arg2) {
    void *v0 = func_15167A68(0xE, arg2, 0xB8, 1, arg1, 1);

    if (v0 == NULL) {
        return NULL;
    }
    bcopy(arg0, (u8 *)v0 + 0x10, 0xA8);
    return v0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15168870.s")
void func_15168A2C(s32 arg0) {
    func_15168B10(arg0, 0);
}

// NON-MATCHING: JUSTREG 4/20 — opcodes match; list-insert into D_800DCE50[unk1]*0x1A0 + arg1; tip justreg_park
// void func_15168A4C(void *arg0, s32 arg1) {
//     typedef struct {
//         u8 unk0;
//         u8 unk1;
//         u8 pad2[2];
//         void *unk4;
//         void *unk8;
//     } Node;
//     Node *a = arg0;
//     Node **slot = (Node **)((u8 *)D_800DCE50 + (a->unk1 * 0x1A0) + (arg1 * 4));
//     Node *t0 = *slot;
//
//     a->unk8 = t0;
//     if (t0 != NULL) {
//         t0->unk4 = a;
//     }
//     a->unk0 = arg1;
//     a->unk4 = NULL;
//     *slot = a;
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15168A4C.s")
// NON-MATCHING: JUSTREG 20/29 (justreg 28/29) — unlink from D_800DCE50[unk1]*0x1A0 +
// unk0*4; prefix matches with `s32 v0 = unk1; s32 v1 = unk0`; list walk stays in
// $v1 plus extra `move $v0,$v1` vs ROM nop. Same family as func_15168A4C. Leave asm.
// void *func_15168A9C(void *arg0) {
//     typedef struct {
//         u8 unk0;
//         u8 unk1;
//         u8 pad2[2];
//         void *unk4;
//         void *unk8;
//     } Node;
//     Node *a = arg0;
//     s32 v0 = a->unk1;
//     s32 v1 = a->unk0;
//     Node **slot = (Node **)((u8 *)D_800DCE50 + (v0 * 0x1A0) + (v1 * 4));
//     Node *n;
//
//     if (a == *slot) {
//         *slot = a->unk8;
//     }
//     n = a->unk8;
//     if (n != NULL) {
//         n->unk4 = a->unk4;
//     }
//     n = a->unk4;
//     if (n != NULL) {
//         n->unk8 = a->unk8;
//     }
//     return n;
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15168A9C.s")


void func_15168B10(s32 arg0, s32 arg1) {
    func_15168A9C(arg0);
    func_15168A4C(arg0, arg1);
}

// NON-MATCHING: packed u16-in-word decrement. Right-length vol_field is 6/26
// (justreg 22/26): IDO DSE's the dead `sw` of (word & 0xFFFF0000) before
// `or`/`sw` of the combined value (4 short without volatile); volatile keeps
// both stores but swaps `lui 0xFFFF` ahead of `addiu 0x1E` and puts `and`
// in the `beqz` delay instead of `addiu lo-1`. Leave asm.
// void func_15168B44(void *arg0) {
//     typedef struct {
//         u8 pad0[0x14];
//         s32 unk14;
//         u8 pad18[0x20];
//         s16 unk38;
//         u8 pad3A[5];
//         u8 unk3F;
//     } Local;
//     Local *a = arg0;
//     s32 v1 = a->unk14;
//     s16 t8 = 0x1E;
//     u16 t6 = v1;
//
//     if (t6 != 0) {
//         s32 t9 = v1 & 0xFFFF0000;
//
//         a->unk14 = t9;
//         a->unk38 = t8;
//         a->unk14 = t9 | (u16)(t6 - 1);
//         return;
//     }
//     {
//         u16 hi = v1 >> 16;
//         u8 a2 = a->unk3F;
//
//         if (hi < a2) {
//             a->unk3F = a2 - hi;
//             a->unk38 = 0x1E;
//         } else {
//             a->unk38 = 0;
//         }
//     }
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15168B44.s")

void func_15168BAC(Game1944C0Dispatch *arg0) {
    u8 idx = arg0->unkE4;

    if (idx != 0) {
        D_8008CA20[idx](arg0);
    }
}

void func_15168BE4(Game1944C0CopySrc *arg0, u8 arg1, s32 arg2) {
    Game1944C0CopyDst *temp_v0;

    if (arg0->unk40 != 0) {
        temp_v0 = func_15167A68(0x10, arg2, 0xF0, 1, arg1, 1);
        if (temp_v0 != NULL) {
            bcopy(arg0, temp_v0->unk90, 0x60);
        }
    }
}

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

void func_15169040(void *arg0, u8 arg1) {
    func_15169070(0, 0x68, arg0, arg1);
}
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15169070.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_15169260.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_1516944C.s")

void func_151695F0(Game1944C0Payload *arg0, u8 arg1) {
    Game1944C0StackPayload sp18;

    sp18.payload = arg0;
    sp18.unk4 = arg0->unk3B;
    func_15169040(&sp18, arg1);
}

void func_1516962C(s32 arg0, Game1944C0Payload *arg1, u8 arg2) {
    Game1944C0StackPayload sp18;

    sp18.payload = arg1;
    sp18.unk4 = arg1->unk3B;
    func_1516944C(arg0, (s32)&sp18, arg2);
}

s32 func_15169668(s32 arg0, s32 arg1, s32 arg2, s32 arg3) {
    D_800D2DAB = 1;
    return arg0;
}

// NON-MATCHING: 18/20 — ROM lbu arg1->unk0 then arg0->unkC; C `arg1->unk0 == arg0->unkC` still loads $a0 first. u8 temp from arg1 is JUSTREG 17/20 (v0/t7 vs t7/t8); tip justreg_park
// void func_1516968C(Game1944C0EventObject *arg0, Game1944C0EventPayloadHeader *arg1, u8 arg2) {
//     if ((arg2 == 0xF) || (arg2 == 0x10)) {
//         if (arg1->unk0 == arg0->unkC) {
//             func_1516972C(arg0);
//         }
//     }
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_1944C0/func_1516968C.s")

void func_151696DC(struct102 *arg0) {
    s32 i;

    for (i = 0; i < D_800DD190; i = (s8)(i + 1)) {
        if (arg0 == ((void **)D_800DD198)[i]) {
            ((void **)D_800DD198)[i] = (void *)arg0->unk8;
        }
    }
}

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
