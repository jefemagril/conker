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
// NON-MATCHING: join list was unrelated goto→for. ASM twin is func_15004CE0.
// 04CE0 Gfx-index is 0xb4 vs 0xa8 (score 1939, was 20 short). Remaining miss
// is extra `sll …,3` in `bnel`/`beql` delay slots (likely-branch).
#pragma GLOBAL_ASM("asm/nonmatchings/game_30E90/func_150049A4.s")

void func_15004A4C(void) {
    s32 i;

    for (i = 0; i < D_800DBEF0; i++) {
        ((s32 *)D_800DBEF8[0])[i] = 0;
        ((s8 *)*((s32 *)D_800DBEFC))[i] = 0;
    }
}


extern u16 D_800BE2A0;
extern u16 D_800BE2A2;
extern u16 D_800BE2A4;

void func_15004AAC(void *arg0, s32 arg1) {
    f32 sz;
    f32 sx;
    f32 sy;
    f32 x;
    f32 y;
    f32 z;
    f32 root;
    f32 new_var;
    s32 maxv = 0;
    s32 off;
    s32 len;
    s32 val;
    s16 *p;
    u16 new_var2;
    u16 cur;
    u16 count;

    count = *((u16 *)(((s8 *)arg0) + 0x16));
    new_var2 = count;
    if (0 < ((s32)new_var2)) {
        if ((arg0 && arg0) && arg0) {
        }
        off = 0;
        p = *((s16 **)(((s8 *)arg0) + 0x28));
        sz = *((f32 *)(((s8 *)arg0) + 0x34));
        z = ((f32)p[2]) * sz;
        sx = *((f32 *)(((s8 *)arg0) + 0x2C));
        x = ((f32)p[0]) * sx;
        sy = *((f32 *)(((s8 *)arg0) + 0x30));
        if (!arg0) {
        }
        len = ((s32)new_var2) * 0x10;
        do {
            off += 0x10;
            z = ((f32)p[2]) * sz;
            x = ((f32)p[0]) * sx;
            new_var = sy;
            y = ((f32)p[1]) * new_var;
            val = (s32)((z * z) + ((x * x) + (y * y)));
            if (maxv < val) {
                maxv = val;
            }
            p = p + 8;
        } while (off < len);
    }
    if (maxv != 0) {
        cur = *((u16 *)(((s8 *)arg0) + 0x50));
        root = sqrtf((f32)maxv);
        if ((*((u16 *)(((s8 *)arg0) + 0x50))) == 0) {
            val = (s32)root;
            *((u16 *)(((s8 *)arg0) + 0x50)) = (u16)val;
            *((u16 *)(((s8 *)arg0) + 0x52)) = (u16)val;
            D_800BE2A2 += 1;
            return;
        }
        val = (s32)root;
        if ((((s32)cur) < val) || (((s32)(*((u16 *)(((s8 *)arg0) + 0x52)))) < val)) {
            D_800BE2A0 += 1;
            return;
        }
        D_800BE2A4 += 1;
    }
}

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

// NON-MATCHING: JUSTREG 27/27 exact 14/27 score 75 — tip justreg_park; do not thrash
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
