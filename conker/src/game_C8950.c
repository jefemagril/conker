#include <ultra64.h>
#include "functions.h"
#include "variables.h"


struct249 *func_1509B704(s16 arg0);
void func_1509C120(void);
void func_1509C3A0(void);


void func_1509B4A0(s32 arg0, s32 arg1) {
    s32 i;

    if(0) {};

    D_8003C8E0 = 0x5000000;
    func_1509C120();
    func_15096970();

    for (i = 0; i < D_800D2F3C; i++) {
        if (func_1509CBD4(D_800D2F40[i])) {
            func_1509B5AC(D_800D2F40[i], arg1);
        }
    }

    func_1509C3A0();
    D_8003C8E0 = 0;
}

u16 *func_1509B570(s32 arg0) {
    struct248 *temp_v0;
    u16 res;

    temp_v0 = func_1509B704(arg0);
    if (temp_v0 != 0) {
        res = temp_v0->unkA;
        return (s32)temp_v0 + (res & 0xFFFF);
    }
    return NULL;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_C8950/func_1509B5AC.s")

struct249 *func_1509B704(s16 arg0) {
    struct249 *tmp;
    s32 mask = 0xFFFF03FF;
    s32 i;

    tmp = D_800D2F48.unk4;
    for (i = 0; i < D_800D2F48.length; i++) {
        if (arg0 == (tmp->unk0 & mask)) {
            return tmp;
        }
        tmp = tmp->next;
    }
    return NULL;
}


// NON-MATCHING: 0xb4 vs 0xac — extra lui remat of &D_800D2F48 on head=next.
// struct250 *g still remats; volatile g is longer. tip live_global_pointer
// void func_1509B764(void *arg0) {
//     typedef struct Node {
//         u8 pad[0x18];
//         struct Node *next;
//         struct Node *prev;
//     } Node;
//     Node *n = arg0;
//
//     if (D_800D2F48.length == 1) {
//         D_800D2F48.unk4 = 0;
//         D_800D2F48.unk8 = 0;
//     } else {
//         if (arg0 == D_800D2F48.unk4) {
//             D_800D2F48.unk4 = n->next;
//             n->next->prev = 0;
//         } else {
//             n->prev->next = n->next;
//         }
//         if (arg0 == (void *)D_800D2F48.unk8) {
//             D_800D2F48.unk8 = (s32)n->prev;
//             n->prev->next = 0;
//         } else {
//             n->next->prev = n->prev;
//         }
//     }
//     func_10004074(arg0);
//     D_800D2F48.length--;
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_C8950/func_1509B764.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_C8950/func_1509B810.s")

void func_1509B8FC(s16 arg0) {
    struct248 *temp_v0;
    s16 sp18[2];

    temp_v0 = func_1502B5C8(&sp18, 2, 20, arg0);
    temp_v0->unk0 |= arg0;
    temp_v0->unk2 = D_800BE9F0;
    func_1509B950(temp_v0);
}

// NON-MATCHING: JUSTREG 39/45 at 0xb4. tip justreg_park, u64_imm_vs_add_reassoc
// s32 `8 - ((p+x)&7)` then `x+pad` reassociates to `(x-mask)+8` (missing li $t8,8 and the dead sh of end).
// `u64 eight=8` keeps li $t8,8 + dead sh via field reload, but subu dest is $t9 not $v1.
// int func_1509B950(struct248 *arg0) {
//     u64 eight = 8;
//     s32 unk4 = arg0->unk4;
//     s32 unk6 = arg0->unk6;
//     s32 pad = (s32)eight - (((s32)arg0 + unk4) & 7);
//     s32 mid = unk4 + pad;
//     void *mem;
//
//     arg0->unk4 = mid + unk6;
//     arg0->unkA = mid;
//     arg0->unk4 = (arg0->unk4 - (((s32)arg0 + arg0->unk4) & 7)) + 8;
//     mem = (void *)allocate_memory((u16)arg0->unk4, 0xFF, 2, 0);
//     if (mem == 0) {
//         for (;;) {}
//     }
//     bcopy(arg0, mem, arg0->unk4);
//     bzero((u8 *)mem + ((struct248 *)mem)->unkA, ((struct248 *)mem)->unk6);
//     func_10004074(arg0);
//     return (int)mem;
// }
#pragma GLOBAL_ASM("asm/nonmatchings/game_C8950/func_1509B950.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_C8950/func_1509BA04.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_C8950/func_1509BBA0.s")

//
#pragma GLOBAL_ASM("asm/nonmatchings/game_C8950/func_1509BE40.s")

#pragma GLOBAL_ASM("asm/nonmatchings/game_C8950/func_1509BFB0.s")

// need a bigger brain
#pragma GLOBAL_ASM("asm/nonmatchings/game_C8950/func_1509C120.s")

void func_1509C228(void) {
    if ((D_800D2E68 & 8) || (D_800D2E4C->unkF & 1)) {
        func_1509BFB0(2, 0x2000, 0x36, 0, 1);
        return;
    }
    func_1509BFB0(2, 0x2000, 0x36, 1, 1);
}

s32 func_1509C2A4(void) {
    s32 temp_v0 = D_800BE9F0;
    if ((temp_v0 == 3) ||
        (temp_v0 == 5) ||
        (temp_v0 == 9) ||
        (temp_v0 == 13) ||
        (temp_v0 == 15) ||
        (temp_v0 == 17) ||
        (temp_v0 == 21) ||
        (temp_v0 == 22) ||
        (temp_v0 == 24) ||
        (temp_v0 == 26) ||
        (temp_v0 == 29) ||
        (temp_v0 == 31) ||
        (temp_v0 == 32) ||
        (temp_v0 == 33) ||
        (temp_v0 == 34) ||
        (temp_v0 == 36) ||
        (temp_v0 == 37) ||
        (temp_v0 == 42) ||
        (temp_v0 == 43) ||
        (temp_v0 == 45) ||
        (temp_v0 == 48) ||
        (temp_v0 == 51) ||
        (temp_v0 == 52) ||
        (temp_v0 == 56) ||
        (temp_v0 == 62) ||
        (temp_v0 == 63) ||
        (D_800D2E44 != 0)
      ) {
        return 0;
    }
    return 1;
}

void func_1509C3A0(void) {
    if ((D_800D2E43 != 0) && (D_800D2E44 == 0) && (func_1509C2A4() != 0)) {
        D_800BE3DF = (s8) D_800BE9F4;
        if (D_800C35C4 == 0) {
            func_15007718(D_8008FDA8);
        }
        D_800D2E43 = 0;
    }
}


s32 func_1509C414(s32 arg0) {
    return ((D_800D2E4C->unk3 & 1) << 0xA) + arg0 + 0x1400;
}
