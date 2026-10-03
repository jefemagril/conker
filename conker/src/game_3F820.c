#include <ultra64.h>

#include "functions.h"
#include "variables.h"


typedef struct {
    f32 unk0;
    f32 unk4;
    f32 unk8;
    char pad[0x174];
} Cam180;

void func_15012370(void) {
    s32 i;
    f32 foo;

    i = 0;
    if (D_80082FA0 >= 0) {
        foo = D_80096560;
        do {
            guOrtho((Mtx *)((u8 *)D_800DCC10 + (i << 6)),
                    -(((Cam180 *)D_800BE628)[i].unk4 * 0.5f),
                    (((Cam180 *)D_800BE628)[i].unk4 * 0.5f),
                    -(((Cam180 *)D_800BE628)[i].unk8 * 0.5f),
                    ((Cam180 *)D_800BE628)[i].unk8 * 0.5f,
                    1.0f, foo, 1.0f);
            i = (i + 1) & 0xFF;
        } while (D_80082FA0 >= i);
    }
}

void func_15012470(void) {
    D_80088750 = func_1518AADC(4, 300, 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_3F820/func_150124A0.s")
