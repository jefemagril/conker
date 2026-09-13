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

void func_15004A4C(void) {
    s32 i;

    for (i = 0; i < D_800DBEF0; i++) {
        ((s32 *)D_800DBEF8[0])[i] = 0;
        ((s8 *)*((s32 *)D_800DBEFC))[i] = 0;
    }
}


// NON-MATCHING: raw m2c draft already scores exact 41/81 justreg 75/81 at the
// correct length 0x144 with no hand edits. Close/regalloc — permuter territory.
// See drafts/func_15004AAC.c (harness draft).
#pragma GLOBAL_ASM("asm/nonmatchings/game_30E90/func_15004AAC.s")

s32 func_15004BF0(s32 arg0) {
    s32 n;
    s32 id;
    s32 i;
    s32 count;

    if (arg0 == 0) {
        i = arg0;
        id = 1;
        arg0 = D_800DBF00, count = D_800DBEF0;
        if (arg0 < count) {
            i = arg0;
            n = 0xA0;
            do {
                if (id == D_800DBEF4[i].unk72) {
                    id++;
                    i = arg0 - 1;
                }
                i++;
                if (id >= 0x100) {
                    id = 0xFF;
                    goto done;
                }
            } while (i < count);
            goto ret;
        }
    } else {
        arg0 = 0xFF - D_800DBF00;
        count = D_800DBEF0;
        id = arg0;
        i = 0;
        if (count > 0) {
            n = 0xA0;
            do {
                if (id == D_800DBEF4[i].unk72) {
                    id--;
                    i = -1;
                }
                i++;
                if (id <= 0) {
                    id = arg0;
                    goto done;
                }
            } while (i < count);
        }
    }
done:
ret:
    return id;
}

// NON-MATCHING: JUSTREG 27/27 exact 14/27 — tip justreg_park; do not thrash
// Indexed Gfx walker relocates G_MOVEMEM / G_MV_MATRIX when w1 < 0x80000000.
// Opcodes+length match; cmd latch is $a2 (reloc $a3) vs ROM $a1/$a2.
// void func_15004CE0(Gfx *arg0, s32 arg1) {
//     s32 i;
//     Gfx *g;
//
//     for (i = 0, g = arg0; *(s8 *)&arg0[i] != -0x21; g = arg0 + ++i) {
//         if (*(s8 *)&arg0[i] == -0x24) {
//             if (((u8 *)g)[3] == 0xE) {
//                 if (g->words.w1 < 0x80000000U) {
//                     g->words.w1 += arg1;
//                 }
//             }
//         }
//     }
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_30E90/func_15004CE0.s")
