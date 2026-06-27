#include <n_libaudio.h>
#include "n_sndp.h"
#include "n_seqp.h"

void n_alSynFilter13(N_ALVoice *voice, f32 pitch);
f32 func_1001CEA4(s32 cents);
void n_alCSPStepChlFade(N_ALCSPlayer *seqp, s32 event, s32 chan, s32 arg3);
extern f32 fabsf(f32);
#pragma intrinsic (fabsf)

#define CSP_MIN_RELEASE_TIME 0x3E80
#define CSP_DEFAULT_CHL_FADE_SPEED 0x88
#define CSP_CHL_FADE_DURATION_MASK 0x7F
#define CSP_CHL_FADE_STEP_EVENT 0xFE

typedef struct N_ALCSPExtraChanState {
    u8 pad0[0x24];
    s32 releaseTime;
    u8 useCustomReleaseTime;
} N_ALCSPExtraChanState;

void n_alCSPHandleChlVolCtrl(N_ALCSPlayer *seqp, s32 arg1, s32 chan, s32 vol) {
    N_ALSoundState *voiceState;
    s16 voiceVol;
    seqp->chanState[chan].vol = vol;
    for(voiceState = seqp->vAllocHead; voiceState != NULL; voiceState = voiceState->voice.node.next) {
        if ((voiceState->chan == chan) && (voiceState->unk38 != 3)) {
            voiceVol = __n_vsVol(voiceState, seqp);
            n_alSynSetVol(&voiceState->voice.node.prev, voiceVol, __n_vsDelta(voiceState, seqp->curTime));
        }
    }
}

void n_alCSPHandleChlPanCtrl(N_ALCSPlayer *seqp, s32 arg1, s32 chan, s32 pan) {
    N_ALSoundState *voiceState;
    u8 voicePan;
    seqp->chanState[chan].pan = pan;
    for (voiceState = seqp->vAllocHead; voiceState != NULL; voiceState = voiceState->voice.node.next) {
        if ((voiceState->chan == chan)) {
            voicePan = __n_vsPan(voiceState, seqp);
            func_1001E2A0(&voiceState->voice.node.prev, voicePan);
        }
    }
}

void n_alCSPHandleChlPriorityCtrl(N_ALCSPlayer *seqp, s32 arg1, s32 chan, s32 priority) {
    seqp->chanState[chan].priority = priority;
}

void n_alCSPPostOsMesg(N_ALCSPlayer *seqp, s32 arg1, s32 arg2, s32 msgValue) {
    if (seqp->unk84 != 0) {
        osSendMesg(seqp->unk84, (msgValue & 7) | 0x10 | ((seqp->node.samplesLeft << 5) & -0x100), 0);
    }
}

void n_alCSPSetChlFxId(N_ALCSPlayer *seqp, s32 arg1, s32 chan, s32 fxId) {
    seqp->chanState[chan].unk8 = fxId;
}

void n_alCSPApplyChlFilterPitch(N_ALCSPlayer *seqp, u8 chan) {
    N_ALVoiceState *voiceState;
    s16 filter12;
    s8 pitchOffset;
    f32 pitchBend;

    pitchOffset = seqp->chanState[chan].unk15 - 0x40;
    pitchBend = seqp->chanState[chan].pitchBend;

    for (voiceState = seqp->vAllocHead; voiceState != NULL; voiceState = voiceState->next) {
        if (voiceState->channel == chan) {
            filter12 = seqp->chanState[chan].unk14;
            n_alSynSetPan(&voiceState->voice, filter12);
            if (filter12 != 0) {
                n_alSynFilter13(&voiceState->voice,
                              func_1001CEA4((voiceState->key - voiceState->sound->keyMap->keyBase) + pitchOffset) *
                                  440.0f * pitchBend);
            }
        }
    }
}

void n_alCSPSetChlFilter12(N_ALCSPlayer *seqp, s32 arg1, s32 chan, s32 filter12) {
    seqp->chanState[chan].unk14 = filter12;
    n_alCSPApplyChlFilterPitch(seqp, chan);
}

void n_alCSPSetChlFilter13Pitch(N_ALCSPlayer *seqp, s32 arg1, s32 chan, s32 pitchOffset) {
    seqp->chanState[chan].unk15 = pitchOffset;
    n_alCSPApplyChlFilterPitch(seqp, chan);
}

void n_alCSPSetChlFilter11(N_ALCSPlayer *seqp, s32 arg1, s32 chan, s32 filter11) {
    N_ALSoundState *voiceState;
    seqp->chanState[chan].unk16 = filter11;
    for (voiceState = seqp->vAllocHead; voiceState != NULL; voiceState = voiceState->voice.node.next) {
        if (voiceState->chan == chan) {
            n_alSynFilter11(&voiceState->voice.node.prev, filter11);
        }
    }
}

void n_alCSPSetChlSustain(N_ALCSPlayer *seqp, s32 arg1, s32 chan, u32 sustain) {
    N_ALVoiceState *state;
    register s32 releaseTime;

    seqp->chanState[chan].sustain = sustain;
    for (state = seqp->vAllocHead; state != NULL; state = state->next) {
        if ((state->channel == chan) && (state->phase != AL_PHASE_RELEASE)) {
            if (sustain >= (AL_SUSTAIN + 1)) {
                if (state->phase == AL_PHASE_NOTEON) {
                    state->phase = AL_PHASE_SUSTAIN;
                }
            } else {
                if (state->phase == AL_PHASE_SUSTAIN) {
                    state->phase = AL_PHASE_NOTEON;
                } else if (state->phase == AL_PHASE_SUSTREL) {
                    state->phase = AL_PHASE_RELEASE;
                    if (((N_ALCSPExtraChanState *) &seqp->chanState[chan])->useCustomReleaseTime) {
                        if (((N_ALCSPExtraChanState *) &seqp->chanState[chan])->releaseTime < CSP_MIN_RELEASE_TIME) {
                            releaseTime = CSP_MIN_RELEASE_TIME;
                        } else {
                            releaseTime = ((N_ALCSPExtraChanState *) &seqp->chanState[chan])->releaseTime;
                        }
                        __n_seqpReleaseVoice((N_ALSeqPlayer *) seqp, &state->voice, releaseTime);
                    } else {
                        if (state->sound->envelope->releaseTime < CSP_MIN_RELEASE_TIME) {
                            releaseTime = CSP_MIN_RELEASE_TIME;
                        } else {
                            releaseTime = state->sound->envelope->releaseTime;
                        }
                        __n_seqpReleaseVoice((N_ALSeqPlayer *) seqp, &state->voice, releaseTime);
                    }
                }
            }
        }
    }
}

void n_alCSPSetChlFXMix80(N_ALCSPlayer *seqp, s32 arg1, s32 chan, s32 fxmix80) {
    N_ALSoundState *voiceState;

    seqp->chanState[chan].fxmix = (seqp->chanState[chan].fxmix & 0x7F) | (fxmix80 << 7);
    for (voiceState = seqp->vAllocHead; voiceState != NULL; voiceState = voiceState->voice.node.next) {
        if ((voiceState->chan == chan) && (voiceState->unk38 != 3)) {
            n_alSynSetFXMix(&voiceState->voice.node.prev, seqp->chanState[chan].fxmix);
        }
    }
}

void n_alCSPSetChlFXMix7F(N_ALCSPlayer *seqp, s32 arg1, s32 chan, s32 fxmix7F) {
    fxmix7F = fxmix7F & 0x7F;
    seqp->chanState[chan].fxmix = (seqp->chanState[chan].fxmix & 0x80) | fxmix7F;
    n_alCSPSetChlFXMix80(seqp, arg1, chan, seqp->chanState[chan].fxmix >> 7);
}

void n_alCSPSetChlFXBus(N_ALCSPlayer *seqp, s32 arg1, s32 chan, u32 fxbus) {
    if (fxbus < n_syn->maxAuxBusses) {
        seqp->chanState[chan].unkB = fxbus;
    }
}

typedef struct {
    u8 pad0[0x36];
    u8 streamFileGroup;
} struct24;

void n_alCSPSetStreamFileGroup(struct24 *state, s32 arg1, s32 arg2, s32 group) {
    state->streamFileGroup = group;
}

void n_alCSPPlayStreamFile(struct24 *state, s32 arg1, s32 arg2, s32 fileIndex) {
    func_1001263C(state->streamFileGroup * 100 + fileIndex, 0x7FFF, 0x40);
}

void n_alCSPApplyChlVol(N_ALCSPlayer *seqp, u8 chan) {
    N_ALSoundState *voiceState;
    s16 vol;

    for (voiceState = seqp->vAllocHead; voiceState != NULL; voiceState = voiceState->voice.node.next) {
        if ((voiceState->chan == chan) && (voiceState->unk38 != 3)) {
            vol = __n_vsVol(voiceState, seqp);
            n_alSynSetVol(&voiceState->voice.node.prev, vol, __n_vsDelta(voiceState, seqp->curTime));
        }
    }
}

void n_alCSPStartChlFade(N_ALCSPlayer *seqp, N_ALEvent *event, s32 chan, s32 target) {
    f32 fadeDelta;

    if (seqp->chanState[chan].unkF == 0) {
        seqp->chanState[chan].unkF = CSP_DEFAULT_CHL_FADE_SPEED;
    }
    if (seqp->chanState[chan].unkE != target) {
        fadeDelta = target - seqp->chanState[chan].unkD;
        seqp->chanState[chan].unk10 = fadeDelta / (seqp->chanState[chan].unkF & CSP_CHL_FADE_DURATION_MASK);
        seqp->chanState[chan].unk10 = fabsf(seqp->chanState[chan].unk10);
        if (seqp->chanState[chan].unkE == seqp->chanState[chan].unkD) {
            seqp->chanState[chan].unkE = target;
        } else {
            seqp->chanState[chan].unkE = target;
            return;
        }
    } else {
        return;
    }
    event->msg.midi.byte1 = CSP_CHL_FADE_STEP_EVENT;
    n_alCSPStepChlFade(seqp, (s32) event, chan, target);
}

void n_alCSPStepChlFade(N_ALCSPlayer *seqp, s32 event, s32 chan, s32 arg3) {
    u8 currentFadeVol;
    u8 targetFadeVol;
    s32 fadeDelta;
    s32 fadeStep;
    f32 fadeStepFloat;
    f32 eventDelayScale;

    currentFadeVol = seqp->chanState[chan].unkD;
    targetFadeVol = seqp->chanState[chan].unkE;
    fadeStepFloat = seqp->chanState[chan].unk10;
    fadeDelta = targetFadeVol - currentFadeVol;
    if (fadeDelta > 0) {
        if (seqp->chanState[chan].unkF & 0x80) {
            fadeStepFloat = fadeStepFloat * 2.0f;
        }
        fadeStep = fadeStepFloat;
        if (fadeStep == 0) {
            fadeStep = 1;
            eventDelayScale = 1.0f / fadeStepFloat;
        } else {
            eventDelayScale = fadeStep / fadeStepFloat;
        }
        if (fadeDelta > fadeStep) {
            fadeDelta = fadeStep;
        }
    } else {
        fadeStep = fadeStepFloat;
        if (fadeStep == 0) {
            fadeStep = 1;
            eventDelayScale = fadeStepFloat;
        } else {
            eventDelayScale = fadeStep / fadeStepFloat;
        }
        fadeStep = -fadeStep;
        if (fadeDelta < fadeStep) {
            fadeDelta = fadeStep;
        }
    }
    currentFadeVol = fadeDelta + currentFadeVol;
    seqp->chanState[chan].unkD = currentFadeVol;
    if (currentFadeVol != targetFadeVol) {
        n_alEvtqPostEvent(&seqp->evtq, event, seqp->uspt * 100 * eventDelayScale, 2);
    }
    if (currentFadeVol != 0) {
        seqp->chanMask |= (1 << chan);
    } else {
        seqp->chanMask &= ~(1 << chan);
    }
    n_alCSPApplyChlVol(seqp, chan);
}

void n_alCSPSetChlFadeSpeed(N_ALCSPlayer *seqp, s32 arg1, s32 chan, s32 fadeSpeed) {
    seqp->chanState[chan].unkF = fadeSpeed;
}

void n_alCSPSetChlFadeEnd(N_ALCSPlayer *seqp, s32 arg1, s32 chan, s32 fadeVol) {
    seqp->chanState[chan].unkD = fadeVol;
    seqp->chanState[chan].unkE = fadeVol;
    if (fadeVol == 0) {
        seqp->chanMask &= (1 << chan) ^ 0xFFFF; // disable
    } else {
        seqp->chanMask |= 1 << chan;
    }
    n_alCSPApplyChlVol(seqp, chan);
}
