#include <n_libaudio.h>

extern s32  D_800E0E00;
extern s32  D_800E0E04;
extern s32  D_800E0E08;
extern s32  D_800E0E10;
extern s16  D_800E0E14;
extern s16  D_800E0E16;
extern u8   D_800E0E18;
extern u8   D_800E0E2C;
extern s32  D_800E0E20;
extern s32  D_800E0E24;
extern s32  D_800E0E28;
extern s32  *D_800E0E30; // 0x8000
extern s32  D_800E0D80; // libaudio struct?
extern s16  D_800E0DB0;
extern s16  D_800E0DB2;
extern s32  D_800E0DD8;
extern s32  D_800E0DE0;
extern s32  D_800E0DE4;
extern s32  D_800E0DFC;

#define STREAM_VOLUME_LIMIT 0x8000
#define STREAM_VOLUME_MAX   0x7FFF

#define STREAM_PREFETCH_SIZE 0x810

#define STREAM_STATE_PLAYING       1
#define STREAM_STATE_STOPPING      2
#define STREAM_STATE_TEARDOWN      3
#define STREAM_STATE_START_PENDING 5
#define STREAM_STATE_FADE_OUT      6
#define STREAM_STATE_FADE_IN       7

#define STREAM_TRANSITION_DELAY 5

#define gStreamState             D_800E0E04
#define gStreamTargetVolume      D_800E0E08
#define gStreamVolumeRampSamples D_800E0E10
#define gStreamTransitionDelay   D_800E0E18
#define gStreamDataStart         D_800E0D80
#define gStreamReadOffset        D_800E0DE4

typedef ALDMAproc (*N_ALStreamDMANew)(void *state);

/* Conker stores a stream DMA-new callback in N_ALSynth's custom pad at 0x24. */
#define n_alStreamDmaNew() (((N_ALStreamDMANew *) n_syn->pad24)[0])


#pragma GLOBAL_ASM("asm/nonmatchings/libultra/audio/game_21FC90/func_151F27E0.s")
// NON-MATCHING: This is a CRC-5-style helper over a 16-bit word using polynomial
// 0x15. Straight C matches the semantics, but IDO schedules the input-bit branch
// and return epilogue differently than the original.
//
// u8 n_alStreamCrc5Word(u16 value) {
//     u8 crc;
//     u8 feedback;
//     s32 bit;
//     s32 i;
//
//     crc = 0;
//     for (i = 0; i < 16; i++) {
//         if (crc & 0x10) {
//             feedback = 0x15;
//         } else {
//             feedback = 0;
//         }
//         if (value & 0x400) {
//             bit = 1;
//         } else {
//             bit = 0;
//         }
//         crc = (crc << 1) | bit;
//         value <<= 1;
//         crc ^= feedback;
//     }
//     return crc & 0x1F;
// }

#pragma GLOBAL_ASM("asm/nonmatchings/libultra/audio/game_21FC90/func_151F2890.s")
// NON-MATCHING: CRC-8-style helper over 32 data bytes plus one final zero-byte
// round using polynomial 0x85. Straight C matches the semantics, but IDO does
// not schedule the shift and loop-counter update into the same delay slots as
// the original asm.
//
// u8 n_alStreamCrc8Block(u8 *data) {
//     u8 crc;
//     u8 feedback;
//     s32 i;
//     s32 bit;
//
//     crc = 0;
//     for (i = 0; i < 0x21; i++) {
//         for (bit = 7; bit >= 0; bit--) {
//             if (crc & 0x80) {
//                 feedback = 0x85;
//             } else {
//                 feedback = 0;
//             }
//             crc <<= 1;
//             if (i != 0x20) {
//                 if (*data & (1 << bit)) {
//                     crc |= 1;
//                 }
//             }
//             crc ^= feedback;
//         }
//         data++;
//     }
//     return crc;
// }

void func_151F2960(s32 arg0, s32 arg1) {
    if (D_800E0DFC == 0) {
        return;
    }
    gStreamState = 4;
    if (D_800E0E2C == 0) {
      D_800E0E2C = 1;
        D_800E0E30 = allocate_memory(0x8000, 0xFF, 2, 1);
        if (D_800E0E30 == 0) {
            D_800E0E2C = 0;
            return;
        }
        D_800E0E20 = func_1502B5C8(0, 2, 0x17, 4);
        if (D_800E0E20 != 0) {
            func_100043B4(D_800E0E20, 0xFF);
        }
        D_800E0E24 = func_1502B5C8(0, 2, 0x17, 5);
        if (D_800E0E24 != 0) {
            func_100043B4(D_800E0E24, 0xFF);
        }
        D_800E0E28 = func_1502B5C8(0, 2, 0x17, 6);
        if (D_800E0E28 != 0) {
            func_100043B4(D_800E0E28, 0xFF);
        }
        if ((D_800E0E20 == 0) || (D_800E0E24 == 0) || (D_800E0E28 == 0)) {
            if (D_800E0E20 != 0) {
                func_10004074(D_800E0E20);
            }
            if (D_800E0E24 != 0) {
                func_10004074(D_800E0E24);
            }
            if (D_800E0E28 != 0) {
                func_10004074(D_800E0E28);
            }
            func_10004074(D_800E0E30);
            D_800E0E30 = 0;
            D_800E0E2C = 0;
            return;
        }
        func_151F3DE0();
    }
    gStreamDataStart = arg0;
    D_800E0DE0 = arg1;
    gStreamReadOffset = 0;
    gStreamVolumeRampSamples = 0;
    gStreamTransitionDelay = STREAM_TRANSITION_DELAY;
    gStreamState = STREAM_STATE_START_PENDING;
}

void n_alStreamForceStop(void) {
    u32 mask = osSetIntMask(1);
    gStreamState = STREAM_STATE_TEARDOWN;
    osSetIntMask(mask);
}

void n_alStreamStop(void) {
    u32 mask;

    mask = osSetIntMask(1);
    if (gStreamState == STREAM_STATE_START_PENDING) {
        gStreamState = STREAM_STATE_FADE_OUT;
    } else {
        gStreamState = STREAM_STATE_STOPPING;
    }
    osSetIntMask(mask);
}

void n_alStreamResume(void) {
    u32 mask;

    mask = osSetIntMask(1);
    if (gStreamState == STREAM_STATE_STOPPING) {
        gStreamTransitionDelay = STREAM_TRANSITION_DELAY;
        gStreamState = STREAM_STATE_FADE_IN;
    } else if (gStreamState == STREAM_STATE_FADE_OUT) {
        gStreamTransitionDelay = STREAM_TRANSITION_DELAY;
        gStreamState = STREAM_STATE_START_PENDING;
    }
    osSetIntMask(mask);
}

s32 n_alStreamGetState(void) {
    s32 state;
    u32 mask;

    state = 0;
    mask = osSetIntMask(1);
    if ((gStreamState == STREAM_STATE_PLAYING) ||
        (gStreamState == STREAM_STATE_START_PENDING) ||
        (gStreamState == STREAM_STATE_FADE_OUT) ||
        (gStreamState == STREAM_STATE_FADE_IN) ||
        (gStreamState == STREAM_STATE_STOPPING)) {
        state = gStreamState;
    }
    osSetIntMask(mask);
    return state;
}

void n_alStreamSetVolumeRamp(s32 volume, s32 rampSamples) {
    u32 mask;

    mask = osSetIntMask(1);
    if (volume < 0) {
        gStreamTargetVolume = 0;
    } else if (volume >= STREAM_VOLUME_LIMIT) {
        gStreamTargetVolume = STREAM_VOLUME_MAX;
    } else {
        gStreamTargetVolume = volume;
    }
    gStreamVolumeRampSamples = rampSamples;
    osSetIntMask(mask);
}

void func_151F2DFC(s32 arg0, s32 arg1) {
    if (arg0 >= 0x80) {
        arg0 = (u16)0x7F;
    } else {
        if (arg0 < 0) {
            arg0 = (u16)0;
        }
    }
    D_800E0E16 = arg0;
    if (arg1 != 0) {
        D_800E0E14 = (s16) D_800E0E16;
    }
}

void func_151F2E4C(s32 arg0, s32 arg1) {
    D_800E0DB2 = arg0;
    D_800E0DB0 = arg1;
    if (D_800E0DD8 == 0) {
        D_800E0DD8 = 2;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/libultra/audio/game_21FC90/func_151F2E88.s")

#pragma GLOBAL_ASM("asm/nonmatchings/libultra/audio/game_21FC90/func_151F39E4.s")

void func_151F3C1C(s32 arg0) {
    D_800E0E00 = arg0;
}

void func_151F3C34(s32 arg0) {
    D_800E0DFC = arg0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/libultra/audio/game_21FC90/func_151F3C4C.s")

void n_alStreamPrefetch(void) {
    void *dmaState;
    ALDMAproc dmaProc;

    dmaProc = n_alStreamDmaNew()(&dmaState);
    dmaProc(gStreamDataStart + gStreamReadOffset, STREAM_PREFETCH_SIZE, 0);
}
