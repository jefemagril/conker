#include <ultra64.h>

#include "functions.h"
#include "variables.h"


void func_150064E0(void) {
    s32 i = 0;
    func_15017790();

    do {
        D_800C3A60[i++] = 0;
    } while (i < 69);

    D_800BE3DF = 24;
    D_800BE3E8 = 0;
    D_800D2E45 = 1;
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_33990/func_15006590.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_33990/func_15006BEC.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_33990/func_1500707C.s")
// requires jump table
#pragma GLOBAL_ASM("asm/nonmatchings/game_33990/func_15007168.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_33990/func_1500727C.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_33990/func_15007360.s")
// NON-MATCHING: 0xdc vs 0xe0  tip unroll_hash_jal_home
// IDO unroll of `for (i=4; i!=0x68; i++) hash += *p++ << (i&3)` is the 4-wide `andi 3; sllv`.
// Missing `sw $v0, 0x18($sp)` after `jal func_151DCEF0` (frame 0x18 vs 0x20). Named `s32 ret`
// and `(void)&ret` still drop the dead home. `s32 n=0x68` as the limit peels remainder (0x114).
// p/i land in `$v1`/`$a0` vs ROM `$a0`/`$v1`. Skip siblings 1500727C / 0707C / 07558.
// void func_15007360(void) {
//     extern u16 D_800BE2F0;
//     extern u8 D_800BE2F2, D_800BE2F3, D_800BE2F4[];
//     extern s32 func_151DCEF0(OSMesgQueue *, s32, u16 *, s32);
//     u8 *p = D_800BE2F4;
//     s32 i, hash;
//
//     hash = D_800BE2F2 << 2;
//     hash += 0xCC;
//     hash += D_800BE2F3 << 3;
//     hash &= 0xFFFF;
//     for (i = 4; i != 0x68; i++) {
//         hash += *p++ << (i & 3);
//         hash &= 0xFFFF;
//     }
//     D_800BE2F0 = hash;
//     if (D_8002AC5C == 0) {
//         func_151DCEF0(&D_800BE900, 0x44, &D_800BE2F0, 0x70);
//     }
// }

#pragma GLOBAL_ASM("asm/nonmatchings/game_33990/func_15007440.s")
#pragma GLOBAL_ASM("asm/nonmatchings/game_33990/func_15007558.s")

void func_15007644(void) {
}

void func_1500764C(void) {
    D_80082BB4 = (u8)6;
    D_800BE3EC = (u8)0;
}

void func_15007668(void) {
    D_80082BB4 = (u8)7;
    D_800BE3EC = (u8)0;
}

void func_15007684(void) {
    D_80082BB4 = (u8)4;
    D_800BE3EC = (u8)0;
}

void func_150076A0(void) {
    D_80082BB4 = (u8)5;
    D_800BE3EC = (u8)0;
}

void func_150076BC(s32 arg0) {
    if (arg0 < 0) {
        func_150064E0();
    }
    if ((arg0 >= 0) && (arg0 < 4) && (D_800BE616 == 0)) {
        D_80082BB4 = (u8)1;
        D_800BE3EC = arg0;
    }
}

void func_15007718(s32 arg0) {
    if ((arg0 >= 0) && (arg0 < 3) && (D_800BE616 == 0)) {
        D_80082BB4 = (u8)2;
        D_800BE3EC = arg0;
    }
}

void func_15007750(s32 arg0) {
    if ((arg0 >= 0) && (arg0 < 3)) {
        D_80082BB4 = (u8)3;
        D_800BE3EC = arg0;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/game_33990/func_15007778.s")
// NON-MATCHING: should probably start over..
// void func_15007778(void) {
//     s32 phi_v0;
//
//     D_800BE3F8->unk8 = -1;
//     D_800BE3F8->unk18 = -1;
//     D_800BE3F8->unk28 = -1;
//     D_800BE3F8->unkE = -1;
//     D_800BE3F8->unk1E = -1;
//     D_800BE3F8->unk2E = -1;
//     D_800BE3DC = -1;
//     D_80082BC0 = 1;
//     D_800BE3DE = 0;
//
//     phi_v0 = 0;
//     do {
//         ((u8*)D_800D2E4C)[phi_v0++] = 0;
//     } while (phi_v0 < 27);
//
//     for (phi_v0 = 0; phi_v0 < 9; phi_v0++) {
//         D_800D2E60[phi_v0] = 0;
//     }
//
//     D_800BE3DB = -1;
//     D_800BE3DA = -1;
//     D_800BE3D9 = -1;
//     D_800BE3D8 = -1;
// }
