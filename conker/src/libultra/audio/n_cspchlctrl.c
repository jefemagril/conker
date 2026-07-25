#include <ultra64.h>
#include <n_libaudio.h>
#include "structs.h"
#include "n_sndp.h"
#include "n_seqp.h"

void n_alSynFilter13(N_ALVoice *voice, f32 pitch);
f32 alSemitones2Ratio(s32 semitones);
void n_alCSPStepChlFade(N_ALCSPlayer *seqp, s32 event, s32 chan, s32 unused);
extern f32 fabsf(f32);
#pragma intrinsic (fabsf)

#define CSP_MIN_RELEASE_TIME 0x3E80
#define CSP_DEFAULT_CHL_FADE_SPEED 0x88
#define CSP_CHL_FADE_DURATION_MASK 0x7F
#define CSP_CHL_FADE_STEP_EVENT 0xFE

void n_alCSPHandleChlVolCtrl(N_ALCSPlayer *seqp, s32 unused, s32 chan, s32 vol) {
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

void n_alCSPHandleChlPanCtrl(N_ALCSPlayer *seqp, s32 unused, s32 chan, s32 pan) {
    N_ALSoundState *voiceState;
    u8 voicePan;
    seqp->chanState[chan].pan = pan;
    for (voiceState = seqp->vAllocHead; voiceState != NULL; voiceState = voiceState->voice.node.next) {
        if ((voiceState->chan == chan)) {
            voicePan = __n_vsPan(voiceState, seqp);
            n_alSynSetPan(&voiceState->voice.node.prev, voicePan);
        }
    }
}

void n_alCSPHandleChlPriorityCtrl(N_ALCSPlayer *seqp, s32 unused, s32 chan, s32 priority) {
    seqp->chanState[chan].priority = priority;
}

void n_alCSPPostOsMesg(N_ALCSPlayer *seqp, s32 unused, s32 unused2, s32 msgValue) {
    if (seqp->queue != 0) {
        osSendMesg(seqp->queue, (msgValue & 7) | 0x10 | ((seqp->node.samplesLeft << 5) & -0x100), 0);
    }
}

void n_alCSPSetChlFxId(N_ALCSPlayer *seqp, s32 unused, s32 chan, s32 fxId) {
    seqp->chanState[chan].instmajor = fxId;
}

void n_alCSPApplyChlFilterPitch(N_ALCSPlayer *seqp, u8 chan) {
    N_ALVoiceState *voiceState;
    s16 filter12;
    s8 pitchOffset;
    f32 pitchBend;

    pitchOffset = seqp->chanState[chan].filterPitch - 0x40;
    pitchBend = seqp->chanState[chan].pitchBend;

    for (voiceState = seqp->vAllocHead; voiceState != NULL; voiceState = voiceState->next) {
        if (voiceState->channel == chan) {
            filter12 = seqp->chanState[chan].filter12;
            n_alSynFilter12(&voiceState->voice, filter12);
            if (filter12 != 0) {
                n_alSynFilter13(&voiceState->voice,
                              alSemitones2Ratio((voiceState->key - voiceState->sound->keyMap->keyBase) + pitchOffset) *
                                  440.0f * pitchBend);
            }
        }
    }
}

void n_alCSPSetChlFilter12(N_ALCSPlayer *seqp, s32 unused, s32 chan, s32 filter12) {
    seqp->chanState[chan].filter12 = filter12;
    n_alCSPApplyChlFilterPitch(seqp, chan);
}

void n_alCSPSetChlFilter13Pitch(N_ALCSPlayer *seqp, s32 unused, s32 chan, s32 pitchOffset) {
    seqp->chanState[chan].filterPitch = pitchOffset;
    n_alCSPApplyChlFilterPitch(seqp, chan);
}

void n_alCSPSetChlFilter11(N_ALCSPlayer *seqp, s32 unused, s32 chan, s32 filter11) {
    N_ALSoundState *voiceState;
    seqp->chanState[chan].filter11 = filter11;
    for (voiceState = seqp->vAllocHead; voiceState != NULL; voiceState = voiceState->voice.node.next) {
        if (voiceState->chan == chan) {
            n_alSynFilter11(&voiceState->voice.node.prev, filter11);
        }
    }
}

void n_alCSPSetChlSustain(N_ALCSPlayer *seqp, s32 unused, s32 chan, u32 sustain) {
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

void n_alCSPSetChlFXMix80(N_ALCSPlayer *seqp, s32 unused, s32 chan, s32 fxmix80) {
    N_ALSoundState *voiceState;

    seqp->chanState[chan].fxmix = (seqp->chanState[chan].fxmix & 0x7F) | (fxmix80 << 7);
    for (voiceState = seqp->vAllocHead; voiceState != NULL; voiceState = voiceState->voice.node.next) {
        if ((voiceState->chan == chan) && (voiceState->unk38 != 3)) {
            n_alSynSetFXMix(&voiceState->voice.node.prev, seqp->chanState[chan].fxmix);
        }
    }
}

void n_alCSPSetChlFXMix7F(N_ALCSPlayer *seqp, s32 unused, s32 chan, s32 fxmix7F) {
    fxmix7F = fxmix7F & 0x7F;
    seqp->chanState[chan].fxmix = (seqp->chanState[chan].fxmix & 0x80) | fxmix7F;
    n_alCSPSetChlFXMix80(seqp, unused, chan, seqp->chanState[chan].fxmix >> 7);
}

void n_alCSPSetChlFXBus(N_ALCSPlayer *seqp, s32 unused, s32 chan, u32 fxbus) {
    if (fxbus < n_syn->maxAuxBusses) {
        seqp->chanState[chan].fxbus = fxbus;
    }
}

void n_alCSPSetStreamFileGroup(struct24 *state, s32 unused, s32 unused2, s32 group) {
    state->streamFileGroup = group;
}

void n_alCSPPlayStreamFile(struct24 *state, s32 unused, s32 unused2, s32 fileIndex) {
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

    if (seqp->chanState[chan].fadevolinc == 0) {
        seqp->chanState[chan].fadevolinc = CSP_DEFAULT_CHL_FADE_SPEED;
    }
    if (seqp->chanState[chan].fadevoltarget != target) {
        fadeDelta = target - seqp->chanState[chan].fadevolcurrent;
        seqp->chanState[chan].fadevolstep = fadeDelta / (seqp->chanState[chan].fadevolinc & CSP_CHL_FADE_DURATION_MASK);
        seqp->chanState[chan].fadevolstep = fabsf(seqp->chanState[chan].fadevolstep);
        if (seqp->chanState[chan].fadevoltarget == seqp->chanState[chan].fadevolcurrent) {
            seqp->chanState[chan].fadevoltarget = target;
        } else {
            seqp->chanState[chan].fadevoltarget = target;
            return;
        }
    } else {
        return;
    }
    event->msg.midi.byte1 = CSP_CHL_FADE_STEP_EVENT;
    n_alCSPStepChlFade(seqp, (s32) event, chan, target);
}

void n_alCSPStepChlFade(N_ALCSPlayer *seqp, s32 event, s32 chan, s32 unused) {
    u8 currentFadeVol;
    u8 targetFadeVol;
    s32 fadeDelta;
    s32 fadeStep;
    f32 fadeStepFloat;
    f32 eventDelayScale;

    currentFadeVol = seqp->chanState[chan].fadevolcurrent;
    targetFadeVol = seqp->chanState[chan].fadevoltarget;
    fadeStepFloat = seqp->chanState[chan].fadevolstep;
    fadeDelta = targetFadeVol - currentFadeVol;
    if (fadeDelta > 0) {
        if (seqp->chanState[chan].fadevolinc & 0x80) {
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
    seqp->chanState[chan].fadevolcurrent = currentFadeVol;
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

void n_alCSPSetChlFadeSpeed(N_ALCSPlayer *seqp, s32 unused, s32 chan, s32 fadeSpeed) {
    seqp->chanState[chan].fadevolinc = fadeSpeed;
}

void n_alCSPSetChlFadeEnd(N_ALCSPlayer *seqp, s32 unused, s32 chan, s32 fadeVol) {
    seqp->chanState[chan].fadevolcurrent = fadeVol;
    seqp->chanState[chan].fadevoltarget = fadeVol;
    if (fadeVol == 0) {
        seqp->chanMask &= (1 << chan) ^ 0xFFFF; // disable
    } else {
        seqp->chanMask |= 1 << chan;
    }
    n_alCSPApplyChlVol(seqp, chan);
}
