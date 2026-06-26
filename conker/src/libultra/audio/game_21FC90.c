#include <n_libaudio.h>

typedef void (*N_ALStreamDoneCallback)(s32);
typedef s32 (*N_ALStreamReadCallback)(void *state, void *dst, s32 len, s32 offset);

extern N_ALStreamDoneCallback D_800E0E00;
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
extern N_ALStreamReadCallback D_800E0DFC;
extern s16  D_8002BC10[];
extern s16  D_8002BD0E[];
extern u8   D_800428C1;
extern u8   D_800428C2;

typedef struct N_ALStreamState {
    u8 pad0[0xC];
    s16 pan;
    s16 volume;
    s16 currentLeft;
    s16 currentRight;
    s16 pad14;
    s16 pad16;
    u16 leftRateFrac;
    s16 leftRate;
    s16 targetLeft;
    u16 rightRateFrac;
    s16 rightRate;
    s16 targetRight;
    s16 needsEnvelopeUpdate;
    s16 pad26;
    s32 rampSamplesDone;
    s32 rampSamplesTotal;
    u8 pad30[0x58];
    s32 targetVolume;
    u8 pad8C[0x4];
    u32 requestedRampSamples;
    s16 targetPan;
} N_ALStreamState;

#define STREAM_VOLUME_LIMIT 0x8000
#define STREAM_VOLUME_MAX   0x7FFF

#define STREAM_PREFETCH_SIZE 0x810
#define STREAM_DMA_CACHED_BASE 0x80000000
#define STREAM_AUDIO_FRAME_SAMPLES 0xB8

#define STREAM_STATE_PLAYING       1
#define STREAM_STATE_STOPPING      2
#define STREAM_STATE_TEARDOWN      3
#define STREAM_STATE_START_PENDING 5
#define STREAM_STATE_FADE_OUT      6
#define STREAM_STATE_FADE_IN       7

#define STREAM_TRANSITION_DELAY 5
#define STREAM_LOADING_STATE    4
#define STREAM_PAN_MIN          0
#define STREAM_PAN_CENTER       0x40
#define STREAM_PAN_LIMIT        0x80
#define STREAM_PAN_MAX          0x7F
#define STREAM_DATA_BUFFER_SIZE  0x8000
#define STREAM_BANK_ID           0x17

#define gStreamState             D_800E0E04
#define gStreamTargetVolume      D_800E0E08
#define gStreamVolumeRampSamples D_800E0E10
#define gStreamCurrentPan        D_800E0E14
#define gStreamTargetPan         D_800E0E16
#define gStreamTransitionDelay   D_800E0E18
#define gStreamInitialized       D_800E0E2C
#define gStreamVoice0            D_800E0E20
#define gStreamVoice1            D_800E0E24
#define gStreamVoice2            D_800E0E28
#define gStreamDataBuffer        D_800E0E30
#define gStreamLoadCommandCount  D_800E0DB2
#define gStreamLoadSampleRate    D_800E0DB0
#define gStreamLoadMode          D_800E0DD8
#define gStreamDoneCallback      D_800E0E00
#define gStreamDataStart         D_800E0D80
#define gStreamDataLength        D_800E0DE0
#define gStreamReadOffset        D_800E0DE4
#define gStreamReadCallback      D_800E0DFC
#define gStreamStateBlock        (*(N_ALStreamState *) &D_800E0D80)

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

void n_alStreamStart(s32 dataStart, s32 dataLength) {
    if (gStreamReadCallback == 0) {
        return;
    }
    gStreamState = STREAM_LOADING_STATE;
    if (gStreamInitialized == 0) {
      gStreamInitialized = 1;
        gStreamDataBuffer = allocate_memory(STREAM_DATA_BUFFER_SIZE, 0xFF, 2, 1);
        if (gStreamDataBuffer == 0) {
            gStreamInitialized = 0;
            return;
        }
        gStreamVoice0 = func_1502B5C8(0, 2, STREAM_BANK_ID, 4);
        if (gStreamVoice0 != 0) {
            func_100043B4(gStreamVoice0, 0xFF);
        }
        gStreamVoice1 = func_1502B5C8(0, 2, STREAM_BANK_ID, 5);
        if (gStreamVoice1 != 0) {
            func_100043B4(gStreamVoice1, 0xFF);
        }
        gStreamVoice2 = func_1502B5C8(0, 2, STREAM_BANK_ID, 6);
        if (gStreamVoice2 != 0) {
            func_100043B4(gStreamVoice2, 0xFF);
        }
        if ((gStreamVoice0 == 0) || (gStreamVoice1 == 0) || (gStreamVoice2 == 0)) {
            if (gStreamVoice0 != 0) {
                func_10004074(gStreamVoice0);
            }
            if (gStreamVoice1 != 0) {
                func_10004074(gStreamVoice1);
            }
            if (gStreamVoice2 != 0) {
                func_10004074(gStreamVoice2);
            }
            func_10004074(gStreamDataBuffer);
            gStreamDataBuffer = 0;
            gStreamInitialized = 0;
            return;
        }
        func_151F3DE0();
    }
    gStreamDataStart = dataStart;
    gStreamDataLength = dataLength;
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

void n_alStreamSetPan(s32 pan, s32 immediate) {
    if (pan >= STREAM_PAN_LIMIT) {
        pan = (u16) STREAM_PAN_MAX;
    } else {
        if (pan < STREAM_PAN_MIN) {
            pan = (u16) STREAM_PAN_MIN;
        }
    }
    gStreamTargetPan = pan;
    if (immediate != 0) {
        gStreamCurrentPan = (s16) gStreamTargetPan;
    }
}

void n_alStreamSetLoadInfo(s32 commandCount, s32 sampleRate) {
    gStreamLoadCommandCount = commandCount;
    gStreamLoadSampleRate = sampleRate;
    if (gStreamLoadMode == 0) {
        gStreamLoadMode = 2;
    }
}

#pragma GLOBAL_ASM("asm/nonmatchings/libultra/audio/game_21FC90/func_151F2E88.s")

void n_alStreamUpdateEnvelope(N_ALStreamState *state) {
    if ((state->volume != state->targetVolume) || (state->pan != state->targetPan)) {
        if (state->rampSamplesDone >= state->rampSamplesTotal) {
            state->targetLeft = (D_8002BC10[state->pan] * state->volume) >> 15;
            state->targetRight = (D_8002BD0E[-state->pan] * state->volume) >> 15;
            state->rampSamplesDone = state->rampSamplesTotal;
            state->currentLeft = state->targetLeft;
            state->currentRight = state->targetRight;
        } else {
            state->currentLeft = _getVol(state->currentLeft, state->rampSamplesDone, state->leftRate,
                                         state->leftRateFrac);
            state->currentRight = _getVol(state->currentRight, state->rampSamplesDone, state->rightRate,
                                          state->rightRateFrac);
        }
        if (state->currentLeft == 0) {
            state->currentLeft = 1;
        }
        if (state->currentRight == 0) {
            state->currentRight = 1;
        }
        state->volume = state->targetVolume;
        if ((state->volume == 0) && (state->requestedRampSamples != 0)) {
            n_alStreamForceStop();
        }
        if (state->pan != state->targetPan) {
            if (D_800428C2 != 0) {
                state->pan = (state->targetPan >> 1) + 0x20;
            } else if (D_800428C1 != 0) {
                state->pan = 0x40;
            } else {
                state->pan = state->targetPan;
            }
        }
        state->rampSamplesDone = 0;
        state->rampSamplesTotal =
            ((state->requestedRampSamples + (STREAM_AUDIO_FRAME_SAMPLES - 1)) / STREAM_AUDIO_FRAME_SAMPLES) *
            STREAM_AUDIO_FRAME_SAMPLES;
        state->needsEnvelopeUpdate = 1;
    }
}

void n_alStreamSetDoneCallback(N_ALStreamDoneCallback callback) {
    gStreamDoneCallback = callback;
}

void n_alStreamSetReadCallback(N_ALStreamReadCallback readCallback) {
    gStreamReadCallback = readCallback;
}

s32 n_alStreamRead(void *state, void *dst, s32 len, s32 offset) {
    void *src;
    ALDMAproc dmaProc;

    if (offset != -1) {
        gStreamReadOffset = offset;
    }
    if (gStreamReadOffset + len > gStreamDataLength) {
        len = gStreamDataLength - gStreamReadOffset;
    }

    dmaProc = n_alStreamDmaNew()(&src);
    src = (void *) dmaProc(gStreamDataStart + gStreamReadOffset, len, 0);
    if (src == 0) {
        return 0;
    }
    src = (void *) ((s32) src + STREAM_DMA_CACHED_BASE);
    osInvalDCache(src, len);
    bcopy(src, dst, len);
    gStreamReadOffset += len;
    return len;
}

void n_alStreamPrefetch(void) {
    void *dmaState;
    ALDMAproc dmaProc;

    dmaProc = n_alStreamDmaNew()(&dmaState);
    dmaProc(gStreamDataStart + gStreamReadOffset, STREAM_PREFETCH_SIZE, 0);
}
