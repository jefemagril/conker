#include <abi.h>
#include <n_libaudio.h>
#include "n_synthInternals.h"

#define aMp3SetAddr(pkt, a)                       \
    {                                             \
        Acmd *_a = (Acmd *) (pkt);                \
        _a->words.w0 = 0x08000000;                \
        _a->words.w1 = (u32) (a);                 \
    }

#define aMp3ExecDma(pkt, a, b)                    \
    {                                             \
        Acmd *_a = (Acmd *) (pkt);                \
        _a->words.w0 = 0x07000000 | ((a) &0xFFFF); \
        _a->words.w1 = (u32) (b);                 \
    }

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
extern s32  D_800E0D80; // First word of N_ALStreamState.
extern s16  D_800E0DB0;
extern s16  D_800E0DB2;
extern s32  D_800E0DD8;
extern s32  D_800E0DE0;
extern s32  D_800E0DE4;
extern N_ALStreamReadCallback D_800E0DFC;
extern void *D_800E0D84;
extern void *D_800E0D88;
extern s16  D_800E0D8C;
extern s16  D_800E0D8E;
extern s16  D_800E0D90;
extern s16  D_800E0D92;
extern s16  D_800E0D94;
extern s16  D_800E0D96;
extern u16  D_800E0D98;
extern s16  D_800E0D9A;
extern s16  D_800E0D9C;
extern u16  D_800E0D9E;
extern s16  D_800E0DA0;
extern s16  D_800E0DA2;
extern s16  D_800E0DA4;
extern s32  D_800E0DA8;
extern s32  D_800E0DAC;
extern void *D_800E0DDC;
extern void *D_800E0DE8;
extern s32  D_800E0DEC;
extern s32  D_800E0DF0;
extern void *D_800E0DF4[];
extern s32  D_800E0DF8;
extern u8   D_800E0E19;
extern s32  D_800BE9F0;
extern s16  D_8002BC10[];
extern s16  D_8002BD0E[];
extern u8   D_800428C1;
extern u8   D_800428C2;

extern s16 _getRate(f32, f32, s32, u16 *);
extern s16 _getVol(s16, s32, s16, u16);
extern void *func_151F85C4(s32, N_ALStreamReadCallback, s32);
extern s32 func_151F86B0(void *, u16 **, s32 *);
extern void func_10004074(s32);

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

void n_alStreamUpdateEnvelope(N_ALStreamState *state);
void n_alStreamPrefetch(void);

/* Conker stores a stream DMA-new callback at N_ALSynth.dma (0x24). */
#define n_alStreamDmaNew() (((N_ALStreamDMANew *) &n_syn->dma)[0])


/* The CRC-5/CRC-8 stream helpers (func_151F27E0, func_151F2890) live in their
 * own -O1 translation unit (src/libultra/audio/n_streamcrc.c); the rest of this
 * file is -g/-O0. */

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

#if 0
/* NON-MATCHING: idiomatic C from PD mp3_make_samples + Conker asm (builds, logic OK, no byte match).
 * First mismatch at prologue stack locals (0x6C vs 0x70); full match needs asm-order stack temps + gotos. */
s32 func_151F2E88(s32 samples, Acmd **cmd) {
    s32 numchannels;
    u16 *sp54;
    s32 sp4c;
    s32 sp48;
    s32 diff;
    s32 i;
    u16 *sp58;
    s32 sp50;

    numchannels = 0;
    sp54 = NULL;
    sp4c = N_AL_MAIN_L_OUT;
    sp48 = N_AL_MAIN_R_OUT;

    if (gStreamCurrentPan != gStreamTargetPan) {
        diff = gStreamTargetPan - gStreamCurrentPan;
        if (diff >= -2 && diff < 3) {
            gStreamCurrentPan += diff;
        } else if (diff >= 3) {
            gStreamCurrentPan += 2;
        } else {
            gStreamCurrentPan -= 2;
        }
    }

    if (gStreamState == STREAM_STATE_START_PENDING) {
        n_alStreamPrefetch();
        if (gStreamTransitionDelay == 0) {
            D_800E0D84 = func_151F85C4(0, gStreamReadCallback, gStreamDataLength);
            if (D_800E0D84 != NULL) {
                gStreamState = STREAM_STATE_PLAYING;
                D_800E0DA4 = 1;
                D_800E0DE8 = NULL;
                D_800E0DEC = 0;
                D_800E0DF0 = 0;
                D_800E0DF8 = 0;
                bzero((void *) ((s8 *) &D_800E0D80 + 0x74), 0x440);
            } else {
                gStreamReadOffset = 0;
                gStreamVolumeRampSamples = 0;
                gStreamTransitionDelay = STREAM_TRANSITION_DELAY;
                n_alStreamPrefetch();
                gStreamState = STREAM_STATE_START_PENDING;
                return 0;
            }
        } else {
            gStreamTransitionDelay--;
        }
    }

    if (gStreamState == STREAM_STATE_FADE_IN) {
        n_alStreamPrefetch();
        if (gStreamTransitionDelay == 0) {
            gStreamState = STREAM_STATE_PLAYING;
        } else {
            gStreamTransitionDelay--;
        }
    }

    if (gStreamState == STREAM_STATE_PLAYING) {
        if (samples + D_800E0DF0 > D_800E0DEC) {
            D_800E0DEC -= D_800E0DF0;
            if ((D_800E0DEC != 0) && (D_800E0DE8 != NULL)) {
                sp54 = (u16 *) ((s8 *) D_800E0DE8 + (D_800E0DF0 * 2));
                sp50 = D_800E0DEC;
            }
            D_800E0DF0 = 0;
            D_800E0DEC = 0;
            if (func_151F86B0(D_800E0D84, &sp58, &numchannels) != 0) {
                D_800E0DE8 = sp58;
                for (i = 0; i < numchannels; i++) {
                    aMp3SetAddr((*cmd)++, osVirtualToPhysical(D_800E0DF4[i]));
                    aMp3ExecDma((*cmd)++, D_800E0DF8, osVirtualToPhysical(sp58));
                    sp58 = (u16 *) ((s8 *) sp58 + 0x488);
                }
                D_800E0DF8 = (D_800E0DF8 - 0x24) & 0x1E;
                D_800E0DEC = 0x240;
                D_800E0E19 = (numchannels ^ 2) < 1;
            }
        }

        n_alStreamPrefetch();

        if (D_800E0E19 == 0) {
            sp4c = 0;
        }

        if (sp54 != NULL) {
            n_aLoadBuffer((*cmd)++, sp50 + sp50, sp4c, osVirtualToPhysical(sp54));
            if (D_800E0E19 != 0) {
                sp54 = (u16 *) ((s8 *) sp54 + 0x488);
                n_aLoadBuffer((*cmd)++, sp50 + sp50, sp48, osVirtualToPhysical(sp54));
            }
            sp4c += sp50 + sp50;
            sp48 += sp50 + sp50;
            samples -= sp50;
        }

        if ((samples > 0) && (D_800E0DE8 != NULL)) {
            sp54 = (u16 *) ((s8 *) D_800E0DE8 + (D_800E0DF0 * 2));
            n_aLoadBuffer((*cmd)++, samples + samples, sp4c, osVirtualToPhysical(sp54));
            if (D_800E0E19 != 0) {
                sp54 = (u16 *) ((s8 *) sp54 + 0x488);
                n_aLoadBuffer((*cmd)++, samples + samples, sp48, osVirtualToPhysical(sp54));
            }
            D_800E0DF0 += samples;
        }

        n_alStreamUpdateEnvelope(&gStreamStateBlock);

        if (D_800E0E19 == 0) {
            aClearBuffer((*cmd)++, N_AL_MAIN_L_OUT, N_AL_TEMP_2);
            aClearBuffer((*cmd)++, N_AL_AUX_L_OUT, N_AL_TEMP_2);

            if (gStreamLoadCommandCount > 0) {
                Acmd *loadCmd;
                u32 lpRateBits;
                f32 lpFilterRate;

                if (gStreamLoadMode != 0) {
                    lpRateBits = 0x46AC0800;
                    lpFilterRate = *(f32 *) &lpRateBits;
                    n_alFxInitlpfilter_mono((ALLowPass *) ((s8 *) &D_800E0D80 + 0x30), lpFilterRate);
                }
                loadCmd = (*cmd)++;
                loadCmd->words.w0 = 0x0B000020;
                loadCmd->words.w1 = osVirtualToPhysical((void *) ((s8 *) &D_800E0D80 + 0x38)) & 0xFFFFFF;
                if (gStreamLoadMode == 2) {
                    gStreamLoadMode = 0;
                }
                loadCmd = (*cmd)++;
                loadCmd->words.w0 = 0x0E000000 | ((gStreamLoadMode & 0xFF) << 16);
                loadCmd->words.w1 = osVirtualToPhysical(D_800E0DDC) & 0xFFFFFF;
                gStreamLoadMode = 0;
            }

            if (D_800E0DA4 != 0) {
                D_800E0DA4 = 0;
                D_800E0D9C = (D_8002BC10[D_800E0D8C] * D_800E0D8E) >> 15;
                D_800E0D9A = _getRate((f32) D_800E0D90, (f32) D_800E0D9C, D_800E0DAC, &D_800E0D98);
                D_800E0DA2 = (D_8002BD0E[-D_800E0D8C] * D_800E0D8E) >> 15;
                D_800E0DA0 = _getRate((f32) D_800E0D92, (f32) D_800E0DA2, D_800E0DAC, &D_800E0D9E);
                n_aSetVolume((*cmd)++, A_VOL | A_LEFT, D_800E0D90, D_800E0D94, D_800E0D96);
                n_aSetVolume((*cmd)++, A_VOL | A_RIGHT, D_800E0DA2, D_800E0DA0, D_800E0D9E);
                n_aSetVolume((*cmd)++, A_RATE, D_800E0D9C, D_800E0D9A, D_800E0D98);
                n_aEnvMixer((*cmd)++, A_INIT, D_800E0D92, osVirtualToPhysical(D_800E0D88));
            } else {
                n_aEnvMixer((*cmd)++, A_CONTINUE, 0, osVirtualToPhysical(D_800E0D88));
            }

            D_800E0DA8 += SAMPLES;
            if (D_800E0DA8 > D_800E0DAC) {
                D_800E0DA8 = D_800E0DAC;
            }
        }

        return 1;
    }

    if (gStreamState == STREAM_STATE_TEARDOWN) {
        if ((D_800BE9F0 != 0x35) && (gStreamInitialized != 0)) {
            if (gStreamVoice0 != 0) {
                func_10004074(gStreamVoice0);
            }
            if (gStreamVoice1 != 0) {
                func_10004074(gStreamVoice1);
            }
            if (gStreamVoice2 != 0) {
                func_10004074(gStreamVoice2);
            }
            gStreamInitialized = 0;
        }
        if (gStreamDataBuffer != 0) {
            func_10004074((s32) gStreamDataBuffer);
            gStreamDataBuffer = 0;
        }
        gStreamState = 0;
        return 0;
    }

    return 0;
}
#endif

#pragma GLOBAL_ASM("asm/nonmatchings/libultra/audio/n_streamplayer/func_151F2E88.s")

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
