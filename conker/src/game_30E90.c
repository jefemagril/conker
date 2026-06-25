#include <ultra64.h>

#include "functions.h"
#include "variables.h"


#pragma GLOBAL_ASM("asm/nonmatchings/game_30E90/func_150039E0.s")

// FIXME: matches but something isnt right
void func_15004574(void) {
    if (D_800DBF88 != 0xFF) {
        D_800DBF8C = D_800DBEF4[D_800DBF88].unk1C;
        D_800DBF90 = D_800DBEF4[D_800DBF88].unk28;
    }
}

void func_150045BC(void) {
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_30E90/func_150045C4.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_30E90/func_150049A4.s")

// void func_15004A4C(void) {
//     s32 i;
//
//     for (i = 0; i < D_800DBEF0; i++) {
//         ((s32 *)D_800DBEF8[0])[i] = 0;
//         ((s8 *)*((s32 *)D_800DBEFC))[i] = 0;
//     }
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_30E90/func_15004A4C.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_30E90/func_15004AAC.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_30E90/func_15004BF0.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_30E90/func_15004CE0.s")
