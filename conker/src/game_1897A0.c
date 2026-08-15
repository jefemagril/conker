#include <ultra64.h>

#include "functions.h"
#include "variables.h"

typedef struct {
    u8 pad0[0x25];
    u8 unk25;
    u8 pad26[0x6];
    s8 unk2C;
    u8 pad2D;
    s8 unk2E;
} Game1897A0Ring;


void *func_1515C2F0(void *arg0, void *arg1, void *arg2, s32 arg3, u8 arg4, s32 arg5) {
    typedef struct {
        u8 pad[0x10];
        s32 unk10;
    } A0;
    typedef struct {
        u8 pad[0x98];
        void *unk98;
    } Ret;
    void *func_15147A80(void *, void *, s32, s32, s32, s32, s32, s32, s32, u8, s32);
    Ret *v0;

    ((A0 *)arg0)->unk10 = 0xA;
    v0 = func_15147A80(arg0, (u8 *)arg1 + 0x40, 0x10, 8, 8, 8, 0, 0, arg3, arg4, arg5);
    if (v0 == NULL) {
        return NULL;
    }
    memcpy(v0->unk98, arg2, 0x3C);
    return v0;
}


#pragma GLOBAL_ASM("asm/nonmatchings/game_1897A0/func_1515C388.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_1897A0/func_1515C534.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_1897A0/func_1515C6F4.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_1897A0/func_1515CF9C.s")
// NON-MATCHING: 11/37 at right 0x94 — extra lw unk94 vs ROM $v1 CSE; wrap
// `sb` not in delay. Named Slot *base lengthens. Do not thrash.
// s32 func_1515CF9C(void *arg0, void *arg1) {
//     s32 temp_v1;
//     s8 temp_v0;
//     void *temp_t1;
//
//     temp_v0 = arg0->unk2C;
//     if ((s32) temp_v0 < (arg0->unk25 - 1)) {
//         temp_v1 = arg0->unk94;
//         arg0->unk2C = (s8) (temp_v0 + 1);
//         temp_t1 = temp_v1 + (arg0->unk2E * 0x10);
//         temp_t1->unk0 = (s32) arg0->unk10;
//         temp_t1->unk4 = (s32) arg0->unk14;
//         temp_t1->unk8 = (s32) arg0->unk18;
//         (temp_v1 + (arg0->unk2E * 0x10))->unkC = (f32) arg1->unk8;
//         arg0->unk2E = (s8) (arg0->unk2E + 1);
//         if (arg0->unk25 == arg0->unk2E) {
//             arg0->unk2E = (u8)0;
//         }
//     } else {
//         arg1->unk39 = (u8)-1;
//     }
//     return 1;
// }

s32 func_1515D030(Game1897A0Ring *arg0, s32 arg1) {
    s32 ret;

    ret = 1;
    if (arg0->unk2C >= 3) {
        arg0->unk2C = arg0->unk2C - 1;
        arg0->unk2E = arg0->unk2E - 1;
        if (arg0->unk2E < 0) {
            arg0->unk2E = arg0->unk25 - 1;
        }
    } else {
        ret = 0;
    }
    return ret;
}


#pragma GLOBAL_ASM("asm/nonmatchings/game_1897A0/func_1515D088.s")
// NON-MATCHING: 23/42 at right 0xa8 — Pack at 0x34 vs ROM 0x30; andi $v1 vs $v0. tip justreg_park / stack
// void *func_1515D088(void *arg0) {
//     typedef struct {
//         void *unk0;
//         f32 unk4;
//         s8 unk8;
//     } Pack;
//     Pack sp30;
//     s32 v0;
//     void *ret;
//
//     v0 = *(s32 *)((u8 *)arg0 + 0x18) & 0xFF;
//     sp30.unk8 = *(s32 *)((u8 *)arg0 + 0x18);
//     if ((v0 < 0) || (v0 >= 2)) {
//         return NULL;
//     }
//     sp30.unk0 = arg0;
//     sp30.unk4 = 0.0f;
//     ret = func_151491F4(0x12C, -1, 0x11, 0, 0xD, 0xC, 0xFF, 1);
//     if (ret != NULL) {
//         memcpy((u8 *)ret + 0x28, &sp30, 0xC);
//     }
//     return ret;
// }

// fat struct
#pragma GLOBAL_ASM("asm/nonmatchings/game_1897A0/func_1515D130.s")
