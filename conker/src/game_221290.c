#include <ultra64.h>

#include "functions.h"
#include "variables.h"

/*
 * ASI MP3 main + decoder — Perfect Dark src/lib/mp3/ twin.
 * Stream player/CRC lives in n_streamplayer.c / n_streamcrc.c (already matched).
 */

#define LAYER_3 1
#define LAYER_2 2
#define LAYER_1 3

#define CRC_PROTECTED   0
#define CRC_UNPROTECTED 1

#define CHANNELMODE_STEREO      0
#define CHANNELMODE_JOINTSTEREO 1
#define CHANNELMODE_DUALMONO    2
#define CHANNELMODE_SINGLEMONO  3

#define VERSION_2 0x0
#define VERSION_1 0x1

struct asistream_scalefac {
    /*0x3d08*/ u32 l[22];
    /*0x3d60*/ u32 unk3d60;
    /*0x3d64*/ u32 s[3][13];
};

struct asistream_4f64 {
    f32 unk00[18];
};

struct asistream {
    /*0x0000*/ s32 unk00;
    /*0x0004*/ s32 (*dmafunc)(s32 arg0, void *arg1, s32 arg2, s32 arg3);
    /*0x0008*/ s32 filesize;
    /*0x000c*/ s32 unk0c;
    /*0x0010*/ s32 unk10;
    /*0x0014*/ s32 unk14;
    /*0x0018*/ s32 unk18;
    /*0x001c*/ u8 unk1c[0x2000];
    /*0x201c*/ s32 unk201c;
    /*0x2020*/ s32 unk2020;
    /*0x2024*/ u8 buffer[0x40];
    /*0x2064*/ s32 offset;
    /*0x2068*/ u32 unk2068;
    /*0x206c*/ u32 unk206c;
    /*0x2070*/ u16 unk2070[6][580];
    /*0x3ba0*/ s32 unk3ba0;
    /*0x3ba4*/ u32 version;
    /*0x3ba8*/ u32 layer;
    /*0x3bac*/ u32 crctype;
    /*0x3bb0*/ u32 bitrateindex;
    /*0x3bb4*/ u32 samplerateindex;
    /*0x3bb8*/ u32 haspadding;
    /*0x3bbc*/ u32 privatebit;
    /*0x3bc0*/ u32 channelmode;
    /*0x3bc4*/ u32 channelmodeext;
    /*0x3bc8*/ u32 copyright;
    /*0x3bcc*/ u32 isoriginal;
    /*0x3bd0*/ u32 emphasis;
    /*0x3bd4*/ s32 doneinitial;
    /*0x3bd8*/ u32 initialversion;
    /*0x3bdc*/ u32 initiallayer;
    /*0x3be0*/ u32 initialcrctype;
    /*0x3be4*/ u32 initialsamplerateindex;
    /*0x3be8*/ u32 initialchannelmode;
    /*0x3bec*/ u32 initialcopyright;
    /*0x3bf0*/ u32 initialisoriginal;
    /*0x3bf4*/ u32 main_data_begin;
    /*0x3bf8*/ u32 scfsi[1][32];
    /*0x3c78*/ u32 part2_3_length[2][1];
    /*0x3c80*/ u32 big_value[2][1];
    /*0x3c88*/ s32 global_gain[2][1];
    /*0x3c90*/ u32 scalefac_compress[2][1];
    /*0x3c98*/ u32 window_switching[2][1];
    /*0x3ca0*/ u32 block_type[2][1];
    /*0x3ca8*/ u32 mixed_block_flag[2][1];
    /*0x3cb0*/ u32 table_select[2][1][3];
    /*0x3cc8*/ u32 subblock_gain[2][1][3];
    /*0x3ce0*/ u32 region0_count[2][1];
    /*0x3ce8*/ u32 region1_count[2][1];
    /*0x3cf0*/ u32 preflag[2][1];
    /*0x3cf8*/ u32 scalefac_scale[2][1];
    /*0x3d00*/ u32 count1table_select[2][1];
    struct asistream_scalefac scalefac[2][1];
    /*0x3ef8*/ u32 unk3ef8;
    /*0x3efc*/ u32 unk3efc[6];
    /*0x3f14*/ u32 unk3f14[26];
    /*0x3f7c*/ s32 bitrate;
    /*0x3f80*/ s32 samplerate;
    /*0x3f84*/ s32 unk3f84;
    /*0x3f88*/ s32 unk3f88;
    /*0x3f8c*/ s32 numchannels;
    /*0x3f90*/ s32 numgranules;
    /*0x3f94*/ s16 unk3f94[1][578];
    /*0x4418*/ u8 unk4418[1][578];
    /*0x465c*/ s32 unk465c[1];
    /*0x4660*/ s32 unk4660[1];
    /*0x4664*/ struct asistream_4f64 unk4664[1][32];
    /*0x4f64*/ struct asistream_4f64 unk4f64[2][32];
    /*0x6164*/ u8 unk6164[0x900];
    /*0x6a64*/ struct asistream_4f64 unk6a64[2][32];
    /*0x7c64*/ u8 unk7c64[0x810];
    /*0x8474*/ s32 unk8474;
    /*0x8478*/ s32 (*decodeframefunc)(struct asistream *stream);
    /*0x847c*/ s32 (*setsideinfofunc)(struct asistream *stream);
};

/* Stream object lives at a fixed BSS address (not a pointer like PD g_AsiStream). */
extern struct asistream D_800E1880;
extern s32 D_800E0E04;
/* Copyright-string / done callback — Conker continues with (0, buf, len). */
extern void (*D_800E0E00)(s32, char *, s32);

extern s32 mp3util_get_bits(u8 *buffer, s32 *offset, s32 numbits);
extern s32 mp3util000462f8(u8 *buffer, s32 *bitoffset, u32 table, s32 sp48, s32 linbits, s32 end,
                           s16 **out, u8 **signout);
extern s32 mp3util000464a8(u8 *buffer, s32 *bitoffset, u32 table, s32 sp48, s32 endbit,
                           s16 **out, u8 **signout);
extern s32 mp3dec_set_side_info(struct asistream *stream);
extern s32 mp3dec_decode_frame(struct asistream *stream);
extern void func00046650(struct asistream_4f64 *in, s32 i, struct asistream_4f64 *out,
                          struct asistream_4f64 *overlap, f32 *window);
extern void func00047550(struct asistream_4f64 *in, s32 i, struct asistream_4f64 *out,
                          struct asistream_4f64 *overlap);

/* Reorder table: PD var70057b9c[version][samplerateindex][576] */
extern s16 D_800AEB7C[2][3][576];
/* Alias-reduction coeffs: PD var7005969c / var700596bc */
extern f32 D_800B067C[];
extern f32 D_800B069C[];
/* PD g_BitRateTable / g_SampleRateTable */
extern u32 D_800B06BC[2][15];
extern u32 D_800B0734[2][4];
/* mp3dec_init float constants / tables (PD sine_block + pow tables) */
extern f32 D_800B0C2C;
extern f32 D_800B0C30;
extern f32 D_800B0C34;
extern f32 D_800B0C38;
extern f32 D_800B0C3C;
extern f32 D_800B0C40;
extern f32 D_800B0C44;
extern f32 D_800B0C48;
extern void *D_800E0E20; /* pointer to huffman table ptrs (PD var8009c650) */
extern s32 D_800E0E28;  /* relocation bias */
extern f32 *D_800E0E30; /* pow buffer source (PD var8009c644) */
extern f32 D_800E0E38[4][36]; /* PD sine_block — also D_800E0EC8 / D_800E0FE8 windows */
extern f32 D_800E0EC8[36];
extern f32 D_800E0FE8[36];
extern f32 *D_800E1078; /* PD var8009c648 */
extern f32 D_800E1080[256]; /* PD var8009c6d8 */
extern f32 D_800E1480[256]; /* PD var8009c6dc */
/* Decoder tables */
extern s32 D_800AE7B8[34]; /* PD var700577d8 linbits */
extern s16 D_800AE840[2][3][22]; /* PD var70057860 */
extern u8 D_800AE948[2][3][13]; /* PD var70057968 */
extern u8 D_800AEB54[]; /* PD var70057b74 */
extern u8 D_800AEB55[]; /* var70057b74 + 1 */
extern u8 D_800AEB5A[]; /* var70057b74 + 6 */
extern u8 D_800AEB5C[]; /* PD var70057b7c[0] */
extern u8 D_800AEB6C[]; /* PD var70057b7c[1] */

s32 func_151F3DE0(void);
s32 func_151F42E8(struct asistream *stream, u32 gr, u32 ch);
s32 func_151F4F38(struct asistream *stream, u32 gr, u32 ch);
s32 func_151F578C(struct asistream *stream, u32 gr, u32 ch);
s32 func_151F63C4(struct asistream *stream, u32 gr, u32 ch);
s32 mp3main00043dd0(struct asistream *stream);
s32 mp3main_read_frame(struct asistream *stream, s32 arg1);
struct asistream *mp3main_start_file(s32 arg0, void *dmafunc, s32 filesize);
s32 mp3main_continue_file(struct asistream *streamptr, u16 **arg1, s32 *numchannels);
s32 mp3dec000427d8(struct asistream *stream, u32 gr);
s32 mp3dec_reduce_aliases(struct asistream *stream, u32 gr, u32 ch);
s32 mp3dec_set_side_info(struct asistream *stream);

/* NON-MATCHING: twin of Perfect Dark mp3dec_init.
 * Conker relocates prebuilt Huffman pointers (D_800E0E20 += D_800E0E28) and fills
 * pow tables inline with Newton (no tree build / no func00045ed0). Same length as
 * original (322 words) with do-while Newton + successive 2^(-0.5/ -2) tables, but
 * IDO -g will not byte-match: c.lt.s / mtc1 / mul.s float-reg allocation around the
 * Newton exit test and D_800E1080/1480 fill differs (~13 reg-only ops). */
#if 0
s32 func_151F3DE0(void) {
    s32 i;
    f32 sp30;
    f32 sp2c;
    f32 sp28;
    f32 sp24;
    f32 sp20;
    f32 sp1c;
    f32 sp18;

    for (i = 0; i < 36; i++) {
        D_800E0E38[0][i] = sinf((i + 0.5f) * D_800B0C2C);
    }

    for (i = 0; i < 18; i++) {
        D_800E0EC8[i] = sinf((i + 0.5f) * D_800B0C30);
    }

    for (i = 18; i < 24; i++) {
        D_800E0EC8[i] = 1.0f;
    }

    for (i = 24; i < 30; i++) {
        D_800E0EC8[i] = sinf((i + 0.5f - 18.0f) * D_800B0C34);
    }

    for (i = 30; i < 36; i++) {
        D_800E0EC8[i] = 0.0f;
    }

    for (i = 0; i < 6; i++) {
        D_800E0FE8[i] = 0.0f;
    }

    for (i = 6; i < 12; i++) {
        D_800E0FE8[i] = sinf((i + 0.5f - 6.0f) * D_800B0C38);
    }

    for (i = 12; i < 18; i++) {
        D_800E0FE8[i] = 1.0f;
    }

    for (i = 18; i < 36; i++) {
        D_800E0FE8[i] = sinf((i + 0.5f) * D_800B0C3C);
    }

    for (i = 1; i < 0x22; i++) {
        ((s32 *)D_800E0E20)[i] += D_800E0E28;
    }

    D_800E1078 = D_800E0E30;

    if (D_800E1078 == NULL) {
        return 0;
    }

    sp30 = 1.0f;
    D_800E1078[0] = 0.0f;

    for (i = 1; i < 0x2000; i++) {
        sp24 = i;
        sp24 = sp24 * sp24;
        sp20 = sp24 * D_800B0C40;
        sp24 = sp24 * sp24;

        do {
            sp2c = sp30 * sp30;
            sp28 = (sp2c * sp30 - sp24) / (2.0f * sp2c);
            sp30 = sp30 - sp28;
        } while (sp20 < sp28 || sp28 < -sp20);

        D_800E1078[i] = sp30;
    }

    sp1c = D_800B0C44;
    sp18 = 0.25f;
    D_800E1080[0] = 1.0f;
    D_800E1480[0] = 1.0f;

    for (i = 1; i < 0x100; i++) {
        D_800E1080[i] = sp1c;
        D_800E1480[i] = sp18;
        sp18 *= 0.25f;
        sp1c *= D_800B0C48;
    }

    return 1;
}
#endif
#pragma GLOBAL_ASM("asm/nonmatchings/game_221290/func_151F3DE0.s")

/* NON-MATCHING: twin of Perfect Dark mp3dec00040164 (requantize / scale apply).
 * Large (0xCE8) with local pretab, pow tables D_800E1078/1080/1480, and
 * var700579b8-style band tables — left for a dedicated pass after scale-fac. */
#pragma GLOBAL_ASM("asm/nonmatchings/game_221290/func_151F42E8.s")

/* NON-MATCHING: twin of Perfect Dark mp3dec_unpack_scale_fac (MPEG1).
 * Closest shape is PD's for-loop twin (533 words). Remaining mismatch is Conker's
 * dual end-bound symbols: loop setup compares via D_800AEB54[5+i]/[6+i] (or
 * [i]/[i+1]), but the sfb++ continue test reloads D_800AEB5A[i] / D_800AEB55[i]
 * (same values, different lo16). Rewriting as do-while to force those symbols
 * blows the s0/s1 / stack frame and diverges further. */
#if 0
s32 func_151F4F38(struct asistream *stream, u32 gr, u32 ch) {
    s32 i;
    s32 sfb;
    s32 window;

    if (stream->window_switching[gr][ch] != 0 && stream->block_type[gr][ch] == 2) {
        if (stream->mixed_block_flag[gr][ch] != 0) {
            for (sfb = 0; sfb < 8; sfb++) {
                stream->scalefac[gr][ch].l[sfb] =
                    D_800AEB5C[stream->scalefac_compress[gr][ch]]
                    ? mp3util_get_bits(stream->unk1c, &stream->unk2020, D_800AEB5C[stream->scalefac_compress[gr][ch]])
                    : 0;
            }

            for (sfb = 3; sfb < 6; sfb++) {
                for (window = 0; window < 3; window++) {
                    stream->scalefac[gr][ch].s[window][sfb] =
                        D_800AEB5C[stream->scalefac_compress[gr][ch]]
                        ? mp3util_get_bits(stream->unk1c, &stream->unk2020, D_800AEB5C[stream->scalefac_compress[gr][ch]])
                        : 0;
                }
            }

            for (sfb = 6; sfb < 12; sfb++) {
                for (window = 0; window < 3; window++) {
                    stream->scalefac[gr][ch].s[window][sfb] =
                        D_800AEB6C[stream->scalefac_compress[gr][ch]]
                        ? mp3util_get_bits(stream->unk1c, &stream->unk2020, D_800AEB6C[stream->scalefac_compress[gr][ch]])
                        : 0;
                }
            }
        } else {
            for (i = 0; i < 2; i++) {
                for (sfb = D_800AEB54[5 + i]; sfb < D_800AEB54[6 + i]; sfb++) {
                    for (window = 0; window < 3; window++) {
                        stream->scalefac[gr][ch].s[window][sfb] =
                            D_800AEB5C[i * 16 + stream->scalefac_compress[gr][ch]]
                            ? mp3util_get_bits(stream->unk1c, &stream->unk2020, D_800AEB5C[i * 16 + stream->scalefac_compress[gr][ch]])
                            : 0;
                    }
                }
            }
        }

        for (window = 0; window < 3; window++) {
            stream->scalefac[gr][ch].s[window][12] = 0;
        }
    } else {
        for (i = 0; i < 4; i++) {
            if (stream->scfsi[ch][i] == 0 || gr == 0) {
                for (sfb = D_800AEB54[i]; sfb < D_800AEB54[i + 1]; sfb++) {
                    stream->scalefac[gr][ch].l[sfb] =
                        D_800AEB5C[(i < 2 ? 0 : 16) + stream->scalefac_compress[gr][ch]]
                        ? mp3util_get_bits(stream->unk1c, &stream->unk2020, D_800AEB5C[(i < 2 ? 0 : 16) + stream->scalefac_compress[gr][ch]])
                        : 0;
                }
            } else {
                for (sfb = D_800AEB54[i]; sfb < D_800AEB54[i + 1]; sfb++) {
                    stream->scalefac[1][ch].l[sfb] = stream->scalefac[0][ch].l[sfb];
                }
            }
        }

        stream->scalefac[gr][ch].unk3d60 = 0;
    }

    return 1;
}
#endif
#pragma GLOBAL_ASM("asm/nonmatchings/game_221290/func_151F4F38.s")

/* NON-MATCHING: twin of Perfect Dark mp3dec00041600 (MPEG2 scale-fac).
 * Large (0xC38) with a big local slen table; not attempted beyond identifying the
 * PD split (Conker calls this from func_151F63C4 when version == VERSION_2). */
#pragma GLOBAL_ASM("asm/nonmatchings/game_221290/func_151F578C.s")

/* Twin of Perfect Dark mp3dec00042238. */
s32 func_151F63C4(struct asistream *stream, u32 gr, u32 ch) {
    s32 sp64 = stream->unk2020;
    s32 sp60;
    s32 sp54[3];
    const s16 *sp50;
    const u8 *sp4c;
    s32 sp48;
    s16 *sp44;
    u8 *sp40;
    s32 i;
    u32 sp38;
    s32 sp34;
    s32 sp30;
    s32 sp2c;
    s32 sp28;

    if (stream->version != VERSION_2) {
        func_151F4F38(stream, gr, ch);
    } else {
        func_151F578C(stream, gr, ch);
    }

    sp60 = stream->big_value[gr][ch] * 2;
    sp50 = D_800AE840[stream->version][stream->samplerateindex];
    sp4c = D_800AE948[stream->version][stream->samplerateindex];

    if (stream->window_switching[gr][ch] == 0 && stream->block_type[gr][ch] == 0) {
        sp54[0] = sp50[stream->region0_count[gr][ch]] + 1;

        if (sp54[0] > sp60) {
            sp54[0] = sp54[1] = sp60;
        } else {
            sp54[1] = sp50[stream->region0_count[gr][ch] + stream->region1_count[gr][ch] + 1] + 1;

            if (sp54[1] > sp60) {
                sp54[1] = sp60;
            }
        }
    } else {
        if (stream->block_type[gr][ch] == 2 && stream->mixed_block_flag[gr][ch] == 0) {
            sp54[0] = sp4c[2] * 3 + 3;
        } else {
            sp54[0] = sp50[7] + 1;
        }

        if (sp54[0] > sp60) {
            sp54[0] = sp60;
        }

        sp54[1] = sp60;
    }

    sp54[2] = sp60;
    sp48 = 0;
    sp44 = stream->unk3f94[ch];
    sp40 = stream->unk4418[ch];

    for (i = 0; i < 3; i++) {
        sp38 = stream->table_select[gr][ch][i];
        sp34 = D_800AE7B8[sp38];
        sp30 = sp54[i];

        if (((void **)D_800E0E20)[sp38] == NULL) {
            sp2c = sp30 - sp48;

            bzero(sp44, sp2c * 2);

            sp44 += sp2c;
            sp40 += sp2c;
            sp48 = sp30;
        } else {
            sp48 = mp3util000462f8(stream->unk1c, &stream->unk2020, sp38, sp48, sp34, sp30, &sp44, &sp40);
        }
    }

    sp38 = stream->count1table_select[gr][ch] + 32;
    sp28 = stream->part2_3_length[gr][ch] + sp64;
    sp48 = mp3util000464a8(stream->unk1c, &stream->unk2020, sp38, sp48, sp28, &sp44, &sp40);
    stream->unk2020 = sp28;

    if (sp48 > 576) {
        stream->unk465c[ch] = 576;
    } else {
        stream->unk465c[ch] = sp48;
    }

    if (sp48 < 576) {
        stream->unk4660[ch] = 576 - sp48;
        bzero(sp44, stream->unk4660[ch] * 2);
    } else {
        stream->unk4660[ch] = 0;
    }

    return 1;
}

/* Twin of Perfect Dark mp3dec000427d8. */
s32 mp3dec000427d8(struct asistream *stream, u32 gr) {
    const s16 *sp14 = D_800AEB7C[stream->version][stream->samplerateindex];
    f32 *sp10 = stream->unk4f64[0][0].unk00;
    f32 *sp0c = stream->unk4664[0][0].unk00;
    s32 i = 0;

    if (stream->window_switching[gr][0] && stream->block_type[gr][0] == 2) {
        if (stream->mixed_block_flag[gr][0]) {
            while (i++ < 36) {
                *(sp10++) = *(sp0c++);
            }
        }

        while (i < 576) {
            sp10[sp14[i++]] = *(sp0c++);
        }
    } else {
        while (i++ < 576) {
            *(sp10++) = *(sp0c++);
        }
    }

    return 1;
}

/* Twin of Perfect Dark mp3dec_reduce_aliases. */
s32 mp3dec_reduce_aliases(struct asistream *stream, u32 gr, u32 ch) {
    s32 i;

    if (stream->window_switching[gr][ch] && stream->block_type[gr][ch] == 2) {
        return 1;
    }

    for (i = 1; i < 32; i++) {
        struct asistream_4f64 *sp08 = &stream->unk4f64[ch][i];
        f32 sp04;
        f32 sp00;

        sp04 = sp08->unk00[0];
        sp00 = sp08->unk00[-1];
        sp08->unk00[-1] = sp00 * D_800B069C[0] - D_800B067C[0] * sp04;
        sp08->unk00[0] = sp04 * D_800B069C[0] + D_800B067C[0] * sp00;

        sp04 = sp08->unk00[1];
        sp00 = sp08->unk00[-2];
        sp08->unk00[-2] = sp00 * D_800B069C[1] - D_800B067C[1] * sp04;
        sp08->unk00[1] = sp04 * D_800B069C[1] + D_800B067C[1] * sp00;

        sp04 = sp08->unk00[2];
        sp00 = sp08->unk00[-3];
        sp08->unk00[-3] = sp00 * D_800B069C[2] - D_800B067C[2] * sp04;
        sp08->unk00[2] = sp04 * D_800B069C[2] + D_800B067C[2] * sp00;

        sp04 = sp08->unk00[3];
        sp00 = sp08->unk00[-4];
        sp08->unk00[-4] = sp00 * D_800B069C[3] - D_800B067C[3] * sp04;
        sp08->unk00[3] = sp04 * D_800B069C[3] + D_800B067C[3] * sp00;

        sp04 = sp08->unk00[4];
        sp00 = sp08->unk00[-5];
        sp08->unk00[-5] = sp00 * D_800B069C[4] - D_800B067C[4] * sp04;
        sp08->unk00[4] = sp04 * D_800B069C[4] + D_800B067C[4] * sp00;

        sp04 = sp08->unk00[5];
        sp00 = sp08->unk00[-6];
        sp08->unk00[-6] = sp00 * D_800B069C[5] - D_800B067C[5] * sp04;
        sp08->unk00[5] = sp04 * D_800B069C[5] + D_800B067C[5] * sp00;

        sp04 = sp08->unk00[6];
        sp00 = sp08->unk00[-7];
        sp08->unk00[-7] = sp00 * D_800B069C[6] - D_800B067C[6] * sp04;
        sp08->unk00[6] = sp04 * D_800B069C[6] + D_800B067C[6] * sp00;

        sp04 = sp08->unk00[7];
        sp00 = sp08->unk00[-8];
        sp08->unk00[-8] = sp00 * D_800B069C[7] - D_800B067C[7] * sp04;
        sp08->unk00[7] = sp04 * D_800B069C[7] + D_800B067C[7] * sp00;
    }

    return 1;
}

/* Twin of Perfect Dark mp3dec_set_side_info. */
s32 mp3dec_set_side_info(struct asistream *stream) {
    s32 sp34;
    s32 ch;
    s32 scfsi_band;
    s32 sp28;
    s32 gr;
    s32 region;
    s32 window;

    if (stream->version != VERSION_2) {
        stream->unk206c = stream->channelmode == CHANNELMODE_SINGLEMONO ? 17 : 32;
    } else {
        stream->unk206c = stream->channelmode == CHANNELMODE_SINGLEMONO ? 9 : 17;
    }

    sp34 = stream->dmafunc(stream->unk00, &stream->buffer[stream->unk2068], stream->unk206c, -1);

    if (stream->unk206c != sp34) {
        return 0;
    }

    stream->unk18 += stream->unk206c;

    stream->numchannels = stream->channelmode == CHANNELMODE_SINGLEMONO ? 1 : 2;
    stream->numgranules = stream->version != VERSION_2 ? 2 : 1;

    if (stream->version != VERSION_2) {
        stream->main_data_begin = mp3util_get_bits(stream->buffer, &stream->offset, 9);

        stream->numchannels == 1
            ? mp3util_get_bits(stream->buffer, &stream->offset, 5)
            : mp3util_get_bits(stream->buffer, &stream->offset, 3);
    } else {
        stream->main_data_begin = mp3util_get_bits(stream->buffer, &stream->offset, 8);

        stream->numchannels == 1
            ? mp3util_get_bits(stream->buffer, &stream->offset, 1)
            : mp3util_get_bits(stream->buffer, &stream->offset, 2);
    }

    if (stream->version != VERSION_2) {
        for (ch = 0; ch < stream->numchannels; ch++) {
            for (scfsi_band = 0; scfsi_band < 4; scfsi_band++) {
                stream->scfsi[ch][scfsi_band] = mp3util_get_bits(stream->buffer, &stream->offset, 1);
            }
        }
    }

    if (stream->version != VERSION_2) {
        sp28 = 4;
    } else {
        sp28 = 9;
    }

    for (gr = 0; gr < stream->numgranules; gr++) {
        for (ch = 0; ch < stream->numchannels; ch++) {
            stream->part2_3_length[gr][ch] = mp3util_get_bits(stream->buffer, &stream->offset, 12);
            stream->big_value[gr][ch] = mp3util_get_bits(stream->buffer, &stream->offset, 9);
            stream->global_gain[gr][ch] = mp3util_get_bits(stream->buffer, &stream->offset, 8);
            stream->scalefac_compress[gr][ch] = sp28 ? mp3util_get_bits(stream->buffer, &stream->offset, sp28) : 0;
            stream->window_switching[gr][ch] = mp3util_get_bits(stream->buffer, &stream->offset, 1);

            if (stream->window_switching[gr][ch]) {
                stream->block_type[gr][ch] = mp3util_get_bits(stream->buffer, &stream->offset, 2);
                stream->mixed_block_flag[gr][ch] = mp3util_get_bits(stream->buffer, &stream->offset, 1);

                for (region = 0; region < 2; region++) {
                    stream->table_select[gr][ch][region] = mp3util_get_bits(stream->buffer, &stream->offset, 5);
                }

                stream->table_select[gr][ch][2] = 0;

                for (window = 0; window < 3; window++) {
                    stream->subblock_gain[gr][ch][window] = mp3util_get_bits(stream->buffer, &stream->offset, 3);
                }
            } else {
                stream->block_type[gr][ch] = 0;
                stream->mixed_block_flag[gr][ch] = 0;

                for (region = 0; region < 3; region++) {
                    stream->table_select[gr][ch][region] = mp3util_get_bits(stream->buffer, &stream->offset, 5);
                }

                stream->region0_count[gr][ch] = mp3util_get_bits(stream->buffer, &stream->offset, 4);
                stream->region1_count[gr][ch] = mp3util_get_bits(stream->buffer, &stream->offset, 3);
            }

            if (stream->version != VERSION_2) {
                stream->preflag[gr][ch] = mp3util_get_bits(stream->buffer, &stream->offset, 1);
            }

            stream->scalefac_scale[gr][ch] = mp3util_get_bits(stream->buffer, &stream->offset, 1);
            stream->count1table_select[gr][ch] = mp3util_get_bits(stream->buffer, &stream->offset, 1);
        }
    }

    stream->bitrate = D_800B06BC[stream->version][stream->bitrateindex];
    stream->samplerate = D_800B0734[stream->version][stream->samplerateindex];

    if (stream->version != VERSION_2) {
        stream->unk3f84 = stream->bitrate * 144 / stream->samplerate;
    } else {
        stream->unk3f84 = stream->bitrate * 72 / stream->samplerate;
    }

    stream->unk3f88 = (stream->unk3f84 + stream->haspadding) - (stream->unk2068 + stream->unk206c);

    return 1;
}

/* Twin of Perfect Dark mp3dec_decode_frame.
 * Conker treats mp3main00043dd0 failure as 0 (PD checks == -1). */
s32 mp3dec_decode_frame(struct asistream *stream) {
    s32 sp954;
    s32 gr = 0;
    s32 ch;
    s32 sp948;
    s32 sp944;
    s32 sp940;
    s32 i;
    s32 j;
    u16 *sp934;
    struct asistream_4f64 sp34[32];
    f32 sp30;
    f32 sp2c;
    f32 sp28;
    s32 sp24;
    s32 sp20;

    sp934 = stream->unk2070[stream->unk3ba0];
    sp954 = mp3main00043dd0(stream);

    if (sp954 == 0) {
        return 0;
    }

    stream->unk2020 = (sp954 - stream->main_data_begin) * 8;

    if (stream->unk2020 < 0) {
        return 1;
    }

    for (ch = 0; ch < stream->numchannels; ch++) {
        func_151F63C4(stream, gr, ch);
        func_151F42E8(stream, gr, ch);
    }

    mp3dec000427d8(stream, gr);

    if (stream->window_switching[gr][0] != 0 && stream->block_type[gr][0] == 2) {
        sp948 = 32;
    } else {
        sp944 = (stream->unk465c[0] - 1) / 18 + 1;
        sp948 = sp944;
    }

    for (ch = 0; ch < stream->numchannels; ch++) {
        mp3dec_reduce_aliases(stream, gr, ch);

        if (stream->window_switching[gr][ch] != 0
                && stream->block_type[gr][ch] == 2
                && stream->mixed_block_flag[gr][ch] != 0) {
            sp940 = 0;
        } else if (stream->window_switching[gr][ch] == 0) {
            sp940 = 0;
        } else {
            sp940 = stream->block_type[gr][ch];
        }

        if (sp940 == 2) {
            for (i = 0; i < 2; i++) {
                func00047550(&stream->unk4f64[ch][i], i, &sp34[i], &stream->unk6a64[ch][i]);
            }
        } else {
            for (i = 0; i < 2; i++) {
                func00046650(&stream->unk4f64[ch][i], i, &sp34[i], &stream->unk6a64[ch][i], D_800E0E38[sp940]);
            }
        }

        if (stream->window_switching[gr][ch]
                    && stream->block_type[gr][ch] == 2
                    && stream->mixed_block_flag[gr][ch]) {
            sp940 = 2;
        }

        if (sp940 == 2) {
            for (i = 2; i < sp948; i++) {
                func00047550(&stream->unk4f64[ch][i], i, &sp34[i], &stream->unk6a64[ch][i]);
            }
        } else {
            for (i = 2; i < sp948; i++) {
                func00046650(&stream->unk4f64[ch][i], i, &sp34[i], &stream->unk6a64[ch][i], D_800E0E38[sp940]);
            }
        }

        while (i < 32) {
            bcopy(&stream->unk6a64[ch][i], &sp34[i], sizeof(struct asistream_4f64));
            bzero(&stream->unk6a64[ch][i], sizeof(struct asistream_4f64));
            i++;
        }

        sp30 = 65536;
        sp28 = 1;
        sp20 = sp30 * sp28 * 16;
        *sp934 = sp20 >> 16;
        sp934++;

        *sp934 = sp20 & 0xffff;
        sp934++;

        sp20 = -sp20;
        *sp934 = sp20 >> 16;
        sp934++;

        *sp934 = sp20 & 0xffff;
        sp934++;

        sp2c = 2048 / sp28;

        for (j = 0; j < 18; j++) {
            for (i = 0; i < 32; i++) {
                sp24 = sp34[i].unk00[j] * sp2c;
                *sp934 = sp24;
                sp934++;
            }
        }
    }

    return 1;
}

/* Twin of Perfect Dark mp3main00043dd0. */
s32 mp3main00043dd0(struct asistream *stream) {
    s32 sp1c = 0x1000;
    s32 sp18;

    if (stream->unk201c + stream->unk3f88 > 0x1ffb) {
        bcopy(&stream->unk1c[sp1c], &stream->unk1c, sp1c);
        stream->unk201c -= sp1c;
        stream->unk2020 -= sp1c * 8;
    }

    sp18 = stream->dmafunc(stream->unk00, &stream->unk1c[stream->unk201c], stream->unk3f88, -1);

    if (sp18 < stream->unk3f88) {
        bzero(&stream->unk1c[sp18], stream->unk3f88 - sp18);
    }

    stream->unk18 += stream->unk3f88;
    stream->unk201c += stream->unk3f88;

    return stream->unk201c - stream->unk3f88;
}

/* Twin of Perfect Dark mp3main_read_frame (Conker rejects non-0xFF/0xF3 bytes). */
s32 mp3main_read_frame(struct asistream *stream, s32 arg1) {
    s32 sp24;
    s32 sp20;
    s32 sp1c;
    u8 mask;

    if (arg1 != -1) {
        stream->unk18 = arg1;
    }

    sp24 = arg1;
    sp20 = 0;
    mask = 0xff;

    while (1) {
        sp1c = stream->dmafunc(stream->unk00, &stream->buffer[sp20], 1, sp24);

        if (sp1c <= 0) {
            return 0;
        }

        sp24 = -1;
        stream->unk18++;

        /* Conker-only: only 0xFF / 0xF3 continue into the mask sync test. */
        if (!(stream->buffer[sp20] == 0xff || stream->buffer[sp20] == 0xf3)) {
            return 0;
        }

        if ((stream->buffer[sp20] & mask) != mask) {
            mask = 0xff;
            sp20 = 0;
            continue;
        }

        sp20++;

        if (mask == 0xf0) {
            break;
        }

        mask = 0xf0;
    }

    sp1c = stream->dmafunc(stream->unk00, &stream->buffer[2], 2, -1);

    if (sp1c <= 0) {
        return 0;
    }

    stream->unk18 += 2;

    stream->offset = 12;

    stream->version = mp3util_get_bits(stream->buffer, &stream->offset, 1);
    stream->layer = mp3util_get_bits(stream->buffer, &stream->offset, 2);
    stream->crctype = mp3util_get_bits(stream->buffer, &stream->offset, 1);
    stream->bitrateindex = mp3util_get_bits(stream->buffer, &stream->offset, 4);
    stream->samplerateindex = mp3util_get_bits(stream->buffer, &stream->offset, 2);
    stream->haspadding = mp3util_get_bits(stream->buffer, &stream->offset, 1);
    stream->privatebit = mp3util_get_bits(stream->buffer, &stream->offset, 1);
    stream->channelmode = mp3util_get_bits(stream->buffer, &stream->offset, 2);
    stream->channelmodeext = mp3util_get_bits(stream->buffer, &stream->offset, 2);
    stream->copyright = mp3util_get_bits(stream->buffer, &stream->offset, 1);
    stream->isoriginal = mp3util_get_bits(stream->buffer, &stream->offset, 1);
    stream->emphasis = mp3util_get_bits(stream->buffer, &stream->offset, 2);

    if (stream->bitrateindex == 15 || stream->samplerateindex == 3) {
        return mp3main_read_frame(stream, -1);
    }

    if (!stream->doneinitial) {
        stream->doneinitial = 1;

        stream->initialversion = stream->version;
        stream->initiallayer = stream->layer;
        stream->initialcrctype = stream->crctype;
        stream->initialsamplerateindex = stream->samplerateindex;
        stream->initialchannelmode = stream->channelmode;
        stream->initialcopyright = stream->copyright;
        stream->initialisoriginal = stream->isoriginal;
    } else if (stream->version != stream->initialversion
            || stream->layer != stream->initiallayer
            || stream->crctype != stream->initialcrctype
            || stream->samplerateindex != stream->initialsamplerateindex
            || stream->channelmode != stream->initialchannelmode
            || stream->isoriginal != stream->initialisoriginal) {
        return mp3main_read_frame(stream, -1);
    }

    stream->unk2068 = 4;

    if (stream->crctype == CRC_PROTECTED) {
        sp1c = stream->dmafunc(stream->unk00, &stream->buffer[4], 2, -1);

        if (sp1c <= 0) {
            return 0;
        }

        stream->unk18 += 2;
        stream->offset += 16;
        stream->unk2068 = 6;
    }

    if (stream->layer == LAYER_3) {
        stream->decodeframefunc = mp3dec_decode_frame;
        stream->setsideinfofunc = mp3dec_set_side_info;
    } else if (stream->layer == LAYER_2) {
        return 0;
    } else if (stream->layer == LAYER_1) {
        return 0;
    }

    if (!stream->setsideinfofunc(stream)) {
        return 0;
    }

    return 1;
}

/* Twin of Perfect Dark mp3main_start_file (Conker checks read_frame result). */
struct asistream *mp3main_start_file(s32 arg0, void *dmafunc, s32 filesize) {
    struct asistream *stream = &D_800E1880;

    if (stream == NULL) {
        return NULL;
    }

    stream->unk0c = -1;
    stream->unk10 = -1;
    stream->unk14 = -1;

    stream->unk00 = arg0;
    stream->dmafunc = dmafunc;
    stream->filesize = filesize;

    stream->unk201c = 0;
    stream->unk2020 = 0;
    stream->unk3ba0 = 0;

    if (mp3main_read_frame(stream, 0) == 0) {
        return NULL;
    }

    stream->unk8474 = 0;

    bzero(&stream->unk6a64, 0x900);

    return stream;
}

/* Twin of Perfect Dark mp3main_continue_file (+ Conker copyright DMA / done cb). */
s32 mp3main_continue_file(struct asistream *streamptr, u16 **arg1, s32 *numchannels) {
    struct asistream *stream = streamptr;
    s32 result;
    u8 copyrightBuf[0x100];
    s32 i;

    stream->unk3ba0++;

    if (stream->unk3ba0 >= 6) {
        stream->unk3ba0 = 0;
    }

    if (!mp3main_read_frame(stream, stream->unk8474)) {
        D_800E0E04 = 3;
        return 0;
    }

    stream->unk8474 = -1;

    result = stream->decodeframefunc(stream);

    if (!result) {
        /* empty */
    } else {
        *arg1 = stream->unk2070[stream->unk3ba0];
        *numchannels = stream->numchannels;

        if (stream->copyright != 0) {
            i = 0;
            do {
                if (stream->dmafunc(stream->unk00, &copyrightBuf[i], 1, -1) == 0) {
                    break;
                }
            } while (copyrightBuf[i++]);
            if (D_800E0E00 != 0) {
                D_800E0E00(0, (char *) copyrightBuf, (s32) strlen((char *) copyrightBuf) + 1);
            }
        }
    }

    return result;
}
