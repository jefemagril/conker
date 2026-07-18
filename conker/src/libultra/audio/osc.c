#include <libaudio.h>
#include <os_internal.h>

#include "functions.h"
#include "variables.h"

#define OSCTYPE_TREM             1
#define OSCTYPE_VIB              128
#define OSC_RATE_BASE            0x103
#define OSC_DELAY_SHIFT          14
#define OSC_TREM_BASE_VOLUME     0x7F
#define OSC_TWO_PI               6.2831854820251465f
#define OSC_STATE_SIZE           0x2C

typedef struct124 ConkerOscState;

#define oscNext                  unk0
#define oscType                  unk4
#define oscPeriod                unk22
#define oscPhase                 unk24
#define tremoloDepth             unk0
#define tremoloBaseVolume        unk1
#define vibratoDepthCents        unk0

f32 n_alOscDepthToCents(u8 depth) {
    f32 mult = 1.0309929847717285f;
    f32 ratio = 1.0f;

    while (depth)
    {
        if (depth & 1) {
            ratio *= mult;
        }
        mult *= mult;
        depth = (u32)depth >> 1;
    }
    return ratio;
}

s32 n_alOscInit(s32 *oscState, f32 *initValue, u8 oscType, u8 oscRate, u8 oscDepth, u8 oscDelay) {
    ConkerOscState *state;
    s32 ret;

    ret = 0;
    if (oscDelay == 0) {
        return 0;
    }
    if (D_80042800 != 0) {
        state = D_80042800;
        D_80042800 = D_80042800->oscNext;
        state->oscType = oscType;
        *oscState = state;
        ret = oscDelay << OSC_DELAY_SHIFT;
        switch (oscType) {
            case OSCTYPE_TREM:
                state->oscPhase = 0;
                state->oscPeriod = OSC_RATE_BASE - oscRate;
                state->data.i.tremoloDepth = oscDepth >> 1;
                state->data.i.tremoloBaseVolume = OSC_TREM_BASE_VOLUME - state->data.i.tremoloDepth;
                *initValue = state->data.i.tremoloBaseVolume;
                break;
            case OSCTYPE_VIB:
                state->data.f.vibratoDepthCents = n_alOscDepthToCents(oscDepth);
                state->oscPhase = 0;
                state->oscPeriod = OSC_RATE_BASE - oscRate;
                *initValue = 1.0f;
                break;
            default:
                break;
        }
    }
    return ret;
}

s32 n_alOscUpdate(ConkerOscState *oscState, f32 *updateValue) {
    f32 phase;
    ConkerOscState *state;
    s32 ret;

    state = oscState;
    ret = AL_USEC_PER_FRAME;

    switch (state->oscType)
    {
        case OSCTYPE_TREM:
            state->oscPhase = (u16)state->oscPhase + 1;
            if ((u16)state->oscPhase >= (u16)state->oscPeriod) {
                state->oscPhase = 0;
            }
            phase = (f32)(u16)state->oscPhase / (f32) (u16)state->oscPeriod;
            phase = sinf(phase * OSC_TWO_PI);
            phase = state->data.i.tremoloDepth * phase;
            *updateValue = state->data.i.tremoloBaseVolume + phase;
            break;

        case OSCTYPE_VIB:
            state->oscPhase = (u16)state->oscPhase + 1;
            if ((u16)state->oscPhase >= (u16)state->oscPeriod) {
                state->oscPhase = 0;
            }
            phase = (f32)(u16) state->oscPhase / (f32)(u16) state->oscPeriod;
            phase = sinf(phase * OSC_TWO_PI) * state->data.f.vibratoDepthCents;
            *updateValue = alCents2Ratio(phase);
            break;
        default:
            break;
    }

    return ret;
}

void n_alOscStop(s32 *oscState) {
    *oscState = (s32*)D_80042800;
    D_80042800 = oscState;
}

void n_alOscSetCallbacks(ALSeqpConfig *config, s32 oscStateCount) {
    s32 *link;
    s32 i;

    D_80042804 = alHeapDBAlloc(0, 0, config->heap, oscStateCount, OSC_STATE_SIZE);
    D_80042800 = (s32) D_80042804;
    D_80042800 = (s32) D_80042804; // ???
    link = D_80042804;

    for (i = 0; i < (oscStateCount - 1); i++) {
        *link = (i * OSC_STATE_SIZE) + D_80042804 + OSC_STATE_SIZE; // is this an array?
        link = *link;
    }
    *link = 0;
    config->initOsc = n_alOscInit;
    config->updateOsc = n_alOscUpdate;
    config->stopOsc = n_alOscStop;
}
