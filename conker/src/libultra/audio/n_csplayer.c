#include <libaudio.h>
#include <os_internal.h>
#include <ultraerror.h>
#include <assert.h>
#include "n_libaudio.h"
#include "n_seqp.h"
#include "cseq.h"
#include "n_cseqp.h"

void n_alSynFilter13(N_ALVoice *voice, f32 pitch);
f32 alSemitones2Ratio(s32 semitones);

/* CC dispatch (asm/data/2AB50.data.s). D_8002BFC0[-cc] aliases BA50 fade slots. */
extern void (*D_8002BA50[])(N_ALCSPlayer *seqp, N_ALEvent *event, s32 chan, s32 byte2);
extern void (*D_8002BFC0[])(N_ALCSPlayer *seqp, N_ALEvent *event, s32 chan, s32 byte2);
extern ALMicroTime D_80042810[];

ALSound *func_1001B07C(N_ALSeqPlayer *seqp, u8 key, u8 vel, u8 chan);
s32      func_1001B7D0(N_ALCSPlayer *seqp, s32 prog, s32 chan);

/* initOsc is called with a 7th timeindex arg; Conker ALOscInit typedef is 6-arg. */
typedef ALMicroTime (*ALOscInit7)(void **oscState, f32 *initVal, u8 oscType,
                                  u8 oscRate, u8 oscDepth, u8 oscDelay, u8 timeindex);

       ALMicroTime      __n_CSPVoiceHandler(void *node);
static void              __n_CSPHandleNextSeqEvent(N_ALCSPlayer *seqp);
       void             __n_CSPHandleMIDIMsg(N_ALCSPlayer *seqp, N_ALEvent *event);
static void             __n_CSPHandleMetaMsg(N_ALCSPlayer *seqp, N_ALEvent *event);
       void             __n_CSPRepostEvent(ALEventQueue *evtq, N_ALEventListItem *item);
       void              __n_setUsptFromTempo(N_ALCSPlayer *seqp, f32 tempo);


void n_alCSPNew(N_ALCSPlayer *seqp, ALSeqpConfig *c)
{
    s32                 i;
    N_ALEventListItem  *items;
    N_ALVoiceState     *vs;
    N_ALVoiceState     *voices;

    ALHeap *hp = c->heap;

    /*
     * initialize member variables
     */
    seqp->bank          = 0;
    seqp->target        = NULL;
    seqp->drvr          = n_syn;
    seqp->chanMask      = 0xffff;
    n_alCSPAllChanOn(seqp);
    seqp->uspt          = 488;
    seqp->nextDelta     = 0;
    seqp->state         = AL_STOPPED;
    seqp->vol           = 0x7FFF;              /* full volume  */
    seqp->debugFlags    = c->debugFlags;
    seqp->frameTime     = AL_USEC_PER_FRAME;   /* should get this from driver */
    seqp->curTime       = 0;
    seqp->initOsc       = c->initOsc;
    seqp->updateOsc     = c->updateOsc;
    seqp->stopOsc       = c->stopOsc;

#if 1
    seqp->unk7C = 0.0f;
    seqp->unk80 = 1.0f;
    seqp->unk84 = 0;
    seqp->unk8D = 0;
    seqp->unk8C = c->maxVoices;
#endif

    seqp->nextEvent.type = AL_SEQP_API_EVT;  /* this will start the voice handler "spinning" */

    /*
     * init the channel state
     */
    seqp->maxChannels = c->maxChannels;
    seqp->chanState = alHeapAlloc(hp, c->maxChannels, sizeof(ALChanState) );
    __n_initChanState((N_ALSeqPlayer*)seqp);  /* sct 11/6/95 */

    /*
     * init the voice state array
     */
    voices = alHeapAlloc(hp, c->maxVoices, sizeof(N_ALVoiceState));
    seqp->vFreeList = 0;
    for (i = 0; i < c->maxVoices; i++) {
      vs = &voices[i];
      vs->next = seqp->vFreeList;
      seqp->vFreeList = vs;
    }

    seqp->vAllocHead = 0;
    seqp->vAllocTail = 0;

    /*
     * init the event queue
     */
    items = alHeapAlloc(hp, c->maxEvents, sizeof(N_ALEventListItem));
    n_alEvtqNew(&seqp->evtq, items, c->maxEvents);


    /*
     * add ourselves to the driver
     */
    seqp->node.next       = NULL;
    seqp->node.handler    = __n_CSPVoiceHandler;
    seqp->node.clientData = seqp;
#if 1
    n_alSynAddSndPlayer (&seqp->node);
#endif
#if 0
    n_alSynAddSeqPlayer( &seqp->node);
#endif
}

/* Conker twin of PD __n_CSPVoiceHandler. Live event IDs after LOOPEND are
 * CONKER_* (NOTEOFF/TREM/VIB/FX shifted +1 vs PD). Stop API remapped:
 *  STOP(0x10) soft→state 3; STOPPING(0x11) hard free; TRACK_END(0x12) begin-stop.
 */
char func_1001ADA4(N_ALSeqPlayer *seqp, N_ALVoice *voice, ALMicroTime killTime);

ALMicroTime __n_CSPVoiceHandler(void *node)
{
    N_ALCSPlayer   *seqp = (N_ALCSPlayer *) node;
    N_ALEvent       evt;
    N_ALVoice      *voice;
    ALMicroTime     delta;
    N_ALVoiceState *vs;
    void           *oscState;
    f32             oscValue;
    u8              chan;
    void           *fx;
    void           *lpfx;
    s32             oldState;
    N_ALEvent       playEvt;
    s32             deltaTicks;
    register s16    volume;
    register s32    volDelta;
    register s32    mix;

    do {
        switch (seqp->nextEvent.type) {
        case (AL_SEQ_REF_EVT):
            __n_CSPHandleNextSeqEvent(seqp);
            break;

        case (AL_SEQP_API_EVT):
            evt.type = AL_SEQP_API_EVT;
            n_alEvtqPostEvent(&seqp->evtq, &evt, seqp->frameTime, 1);
            break;

        case (AL_NOTE_END_EVT):
            voice = seqp->nextEvent.msg.note.voice;

            n_alSynStopVoice(voice);
            n_alSynFreeVoice(voice);
            vs = (N_ALVoiceState *) voice->unk10;

            if (vs->flags) {
                __n_seqpStopOsc((N_ALSeqPlayer *) seqp, vs);
            }

            __n_unmapVoice((N_ALSeqPlayer *) seqp, voice);
            break;

        case (AL_SEQP_ENV_EVT):
            voice = seqp->nextEvent.msg.vol.voice;
            vs = (N_ALVoiceState *) voice->unk10;

            if (vs->envPhase == AL_PHASE_ATTACK) {
                vs->envPhase = AL_PHASE_DECAY;
            }

            delta = seqp->nextEvent.msg.vol.delta;
            vs->envEndTime = seqp->curTime + delta;
            vs->envGain = seqp->nextEvent.msg.vol.vol;
            volume = __n_vsVol(vs, (N_ALSeqPlayer *) seqp),
            n_alSynSetVol(voice, volume, delta);
            break;

        case (CONKER_TREM_OSC_EVT):
            vs = seqp->nextEvent.msg.osc.vs;
            oscState = seqp->nextEvent.msg.osc.oscState;
            delta = (*seqp->updateOsc)(oscState, &oscValue);
            vs->tremelo = (u8) oscValue;
            volume = __n_vsVol(vs, (N_ALSeqPlayer *) seqp),
            volDelta = __n_vsDelta(vs, seqp->curTime),
            n_alSynSetVol(&vs->voice, volume, volDelta);
            evt.type = CONKER_TREM_OSC_EVT;
            evt.msg.osc.vs = vs;
            evt.msg.osc.oscState = oscState;
            n_alEvtqPostEvent(&seqp->evtq, &evt, delta, 0);
            break;

        case (CONKER_VIB_OSC_EVT):
            vs = seqp->nextEvent.msg.osc.vs;
            oscState = seqp->nextEvent.msg.osc.oscState;
            chan = seqp->nextEvent.msg.osc.chan;
            delta = (*seqp->updateOsc)(oscState, &oscValue);
            vs->vibrato = oscValue;
            n_alSynSetPitch(&vs->voice,
                            vs->pitch * vs->vibrato * seqp->chanState[chan].pitchBend);

            if (seqp->chanState[chan].unk14) {
                n_alSynFilter13(
                    &vs->voice,
                    440 * alSemitones2Ratio((u8) seqp->chanState[chan].unk15
                                            + (vs->key - vs->sound->keyMap->keyBase)
                                            - 64)
                        * seqp->chanState[chan].pitchBend * vs->vibrato);
            }

            evt.type = CONKER_VIB_OSC_EVT;
            evt.msg.osc.vs = vs;
            evt.msg.osc.oscState = oscState;
            evt.msg.osc.chan = chan;
            n_alEvtqPostEvent(&seqp->evtq, &evt, delta, 0);
            break;

        case (AL_SEQP_MIDI_EVT):
        case (CONKER_CSP_NOTEOFF_EVT):
            __n_CSPHandleMIDIMsg(seqp, &seqp->nextEvent);
            break;

        case (AL_SEQP_META_EVT):
            __n_CSPHandleMetaMsg(seqp, &seqp->nextEvent);
            break;

        case (AL_SEQP_VOL_EVT):
            seqp->vol = seqp->nextEvent.msg.spvol.vol;

            for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
                volume = __n_vsVol(vs, (N_ALSeqPlayer *) seqp),
                volDelta = __n_vsDelta(vs, seqp->curTime),
                n_alSynSetVol(&vs->voice, volume, volDelta);
            }
            break;

        case (CONKER_SEQP_FXMIX_EVT):
            seqp->unk7C = seqp->nextEvent.msg.unknown0.unk0;
            seqp->unk80 = seqp->nextEvent.msg.unknown0.unk4;

            for (vs = seqp->vAllocHead; vs != 0;) {
                if (vs->envPhase != AL_PHASE_RELEASE) {
                    mix = __n_vsMix(vs, seqp) & 0xFF,
                    n_alSynSetFXMix(&vs->voice, mix);
                }
                vs = vs->next;
            }
            break;

        case (CONKER_SEQP_FXPARAM_EVT):
            if (seqp->nextEvent.msg.unknown2.unk1 < 8) {
                fx = (void *) n_alSynGetFXRef(seqp->nextEvent.msg.unknown2.unk0);

                if (fx) {
                    n_alSynSetFXParam(fx,
                                      (seqp->nextEvent.msg.unknown2.unk2 << 3)
                                          | (seqp->nextEvent.msg.unknown2.unk1 & 7),
                                      &seqp->nextEvent.msg.unknown2.unk4);
                }
            } else {
                lpfx = (void *) n_alSynGetOutputLPRef(seqp->nextEvent.msg.unknown2.unk0);

                if (lpfx) {
                    n_alSynSetOutputLPParam(lpfx,
                                            seqp->nextEvent.msg.unknown2.unk1,
                                            &seqp->nextEvent.msg.unknown2.unk4);
                }
            }
            break;

        case (AL_SEQP_PLAY_EVT):
            /* Conker: resume via __alCSeqNextDelta; soft-stop reuses unk88. */
            if (seqp->state != AL_PLAYING) {
                oldState = seqp->state;
                if (seqp->target != NULL) {
                    seqp->state = AL_PLAYING;
                    if (__alCSeqNextDelta(seqp->target, &deltaTicks)) {
                        playEvt.type = AL_SEQ_REF_EVT;
                        if (oldState == AL_SOFT_STOPPING) {
                            deltaTicks = seqp->unk88;
                        }
                        n_alEvtqPostEvent(&seqp->evtq, &playEvt, deltaTicks, 0);
                    }
                }
            }
            break;

        case (AL_SEQP_STOP_EVT):
            /* Conker soft-stop: park in state 3, flush REF, save delta. */
            if (seqp->state == AL_PLAYING) {
                seqp->state = AL_SOFT_STOPPING;
                seqp->unk88 = n_alEvtqFlushType(&seqp->evtq, AL_SEQ_REF_EVT);
            }
            break;

        case (AL_SEQP_STOPPING_EVT):
            /* Conker hard-stop body (PD STOP) plus channel instrument cleanup. */
            if (seqp->state == AL_STOPPING) {
                for (vs = seqp->vAllocHead; vs != 0; vs = seqp->vAllocHead) {
                    n_alSynStopVoice(&vs->voice);
                    n_alSynFreeVoice(&vs->voice);

                    if (vs->flags) {
                        __n_seqpStopOsc((N_ALSeqPlayer *) seqp, vs);
                    }

                    __n_unmapVoice((N_ALSeqPlayer *) seqp, &vs->voice);
                }

                seqp->state = AL_STOPPED;

                for (chan = 0; chan < 16; chan++) {
                    if (seqp->chanState[chan].unk36) {
                        /* empty — matches original load/beqz trampoline */
                    }
                    if (seqp->chanState[chan].instrument != 0) {
                        ((void (*)(ALInstrument *)) seqp->drvr->unk34)(
                            seqp->bank->instArray[seqp->chanState[chan].unk38]);
                        seqp->chanState[chan].instrument = 0;
                    }
                }
            }
            break;

        case (AL_TRACK_END):
            /* Conker begin-stop (PD STOPPING), also runs from soft-stop state. */
            if (seqp->state == AL_PLAYING || seqp->state == AL_SOFT_STOPPING) {
                n_alEvtqFlushType(&seqp->evtq, AL_SEQ_REF_EVT);
                n_alEvtqFlushType(&seqp->evtq, CONKER_CSP_NOTEOFF_EVT);
                n_alEvtqFlushType(&seqp->evtq, AL_SEQP_MIDI_EVT);

                for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
                    if (func_1001ADA4((N_ALSeqPlayer *) seqp, &vs->voice, KILL_TIME)) {
                        __n_seqpReleaseVoice((N_ALSeqPlayer *) seqp, &vs->voice, KILL_TIME);
                    }
                }

                for (chan = 0; chan < 16; chan++) {
                    seqp->chanState[chan].unkD = seqp->chanState[chan].unkE;

                    if (seqp->chanState[chan].unkD == 0) {
                        seqp->chanMask &= (1 << chan) ^ 0xffff;
                    } else {
                        seqp->chanMask |= 1 << chan;
                    }
                }

                seqp->state = AL_STOPPING;
                evt.type = AL_SEQP_STOPPING_EVT;
                n_alEvtqPostEvent(&seqp->evtq, &evt, AL_EVTQ_END, 0);
            }
            break;

        case (AL_SEQP_PRIORITY_EVT):
            chan = seqp->nextEvent.msg.sppriority.chan;
            seqp->chanState[chan].priority = seqp->nextEvent.msg.sppriority.priority;
            break;

        case (AL_SEQP_SEQ_EVT):
            seqp->target = seqp->nextEvent.msg.spseq.seq;
            seqp->chanMask = 0xffff;

            if (seqp->bank) {
                __n_initFromBank((N_ALSeqPlayer *) seqp, seqp->bank);
            }
            break;

        case (AL_SEQP_BANK_EVT):
            seqp->bank = seqp->nextEvent.msg.spbank.bank;
            __n_initFromBank((N_ALSeqPlayer *) seqp, seqp->bank);
            break;

        /* Empty cases share one trampoline; unlisted IDs jump straight to join
         * (no default — a default: creates a second trampoline and breaks jtbl). */
        case (AL_SEQ_END_EVT):
        case (AL_TEMPO_EVT):
        case (AL_SEQ_MIDI_EVT):
            break;
        }

        seqp->nextDelta = n_alEvtqNextEvent(&seqp->evtq, &seqp->nextEvent);
    } while (seqp->nextDelta == 0);

    seqp->curTime += seqp->nextDelta;
    return seqp->nextDelta;
}

static void __n_CSPHandleNextSeqEvent(N_ALCSPlayer *seqp)
{
    N_ALEvent evt;

    /* Conker: also bail if state == 3 (custom stop-pending). */
    if (seqp->target == NULL || seqp->state == 3) {
        return;
    }

    n_alCSeqNextEvent(seqp->target, &evt, 1);

    switch (evt.type) {
    case AL_SEQ_MIDI_EVT:
        __n_CSPHandleMIDIMsg(seqp, &evt);
        __n_CSPPostNextSeqEvent(seqp);
        break;

    case AL_TEMPO_EVT:
        __n_CSPHandleMetaMsg(seqp, &evt);
        __n_CSPPostNextSeqEvent(seqp);
        break;

    case AL_SEQ_END_EVT:
        /* Conker posts AL_SEQP_STOPPING_EVT (siblings use AL_SEQP_STOP_EVT). */
        seqp->state = AL_STOPPING;
        evt.type = AL_SEQP_STOPPING_EVT;
        n_alEvtqPostEvent(&seqp->evtq, &evt, AL_EVTQ_END, 0);
        break;

    case AL_CSP_LOOPSTART:
    case AL_CSP_LOOPEND:
    case AL_CSP_NOTEOFF_EVT:
        __n_CSPPostNextSeqEvent(seqp);
        break;

    default:
        break;
    }
}

void __n_CSPHandleMIDIMsg(N_ALCSPlayer *seqp, N_ALEvent *event)
{
    N_ALVoice          *voice;
    s32                 status;
    u8                  chan;
    u8                  key;
    u8                  byte1;
    u8                  byte2;
    ALMIDIEvent         *midi = &event->msg.midi;
    N_ALEvent           evt;
    ALMicroTime         deltaTime;
    N_ALVoiceState     *vstate;
    ALChanState        *chanstate;
    s32                 sp90;
    ALVoiceConfig       config;
    ALSound            *sound;
    s16                 cents;
    f32                 pitch, oscValue;
    u8                  fxmix;
    u8                  sp76;
    ALPan               pan;
    s16                 vol;
    f32                 sp70;
    void               *oscState;
    ALInstrument       *inst;
    N_ALVoiceState     *vs;
    void              (*handler)(N_ALCSPlayer *, N_ALEvent *, s32, s32);
    s32                 bendVal;
    f32                 bendRatio;
    s32                 bendCents;
    N_ALVoiceState     *vsBend;
    register s32        volDelta;
    register s16        volume;
    register f32        filterRatio;

    status = midi->status & AL_MIDI_StatusMask;
    chan   = midi->status & AL_MIDI_ChannelMask;
    byte1  = key = midi->byte1;
    byte2  = midi->byte2;

    /* Conker: defer MIDI while channel muted (unk36), except ProgramChange */
    if (seqp->chanState[chan].unk36 && status != AL_MIDI_ProgramChange) {
        evt.type = AL_SEQP_MIDI_EVT;
        evt.msg.midi = *midi;
        n_alEvtqPostEvent(&seqp->evtq, &evt, 0x8235, 0);
        return;
    }

    switch (status) {
    case (AL_MIDI_NoteOn):

        if (byte2 != 0) {
            oscState = 0;

            if (seqp->state != AL_PLAYING || (seqp->chanMask & (1 << chan)) == 0) {
                if (midi->duration) {
                    /* Muted/not-playing path posts type 2, not CONKER_CSP_NOTEOFF_EVT */
                    evt.type            = AL_SEQP_MIDI_EVT;
                    evt.msg.midi.status = chan | AL_MIDI_NoteOff;
                    evt.msg.midi.byte1  = key;
                    evt.msg.midi.byte2  = 0;

                    deltaTime = seqp->uspt * midi->duration;
                    D_80042810[chan] = deltaTime;

                    n_alEvtqPostEvent(&seqp->evtq, &evt, deltaTime, 0);
                }

                break;
            }

            chanstate = &seqp->chanState[chan];

            sound = func_1001B07C((N_ALSeqPlayer *)seqp, key, byte2, chan);
            if (!sound) {
                break;
            }
            ALFlagFailIf(!sound, seqp->debugFlags & NO_SOUND_ERR_MASK,
                         ERR_ALSEQP_NO_SOUND);

            config.priority   = chanstate->priority;
            config.fxBus      = chanstate->unkB;
            config.unityPitch = 0;
            config.unk8       = 0;

            vstate = __n_mapVoice((N_ALSeqPlayer *)seqp, key, byte2, chan);
            ALFlagFailIf(!vstate, seqp->debugFlags & NO_VOICE_ERR_MASK,
                         ERR_ALSEQP_NO_VOICE);

            voice = &vstate->voice;

            n_alSynAllocVoice(voice, &config);

            vstate->sound    = sound;
            vstate->envPhase = AL_PHASE_ATTACK;

            if (chanstate->sustain > AL_SUSTAIN) {
                vstate->phase = AL_PHASE_SUSTAIN;
            } else {
                vstate->phase = AL_PHASE_NOTEON;
            }

            cents = (key - sound->keyMap->keyBase) * 100
                    + sound->keyMap->detune;

            if (chanstate->usechanparams) {
                cents += chanstate->pitch;
            }

            vstate->pitch = alCents2Ratio(cents);

            if (chanstate->usechanparams) {
                vstate->envGain    = chanstate->attackVolume;
                vstate->envEndTime = seqp->curTime + chanstate->attackTime;
            } else {
                vstate->envGain    = sound->envelope->attackVolume;
                vstate->envEndTime = seqp->curTime + sound->envelope->attackTime;
            }

            vstate->flags = 0;

            if (chanstate->usechanparams) {
                sp90 = chanstate->tremType;
            } else {
                inst = seqp->chanState[chan].instrument;
                sp90 = inst->tremType;
            }

            oscValue = (f32)AL_VOL_FULL;

            if (sp90) {
                if (seqp->initOsc) {
                    if (chanstate->usechanparams) {
                        deltaTime = ((ALOscInit7)seqp->initOsc)(
                            &oscState, &oscValue, chanstate->tremType,
                            chanstate->tremRate, chanstate->tremDepth,
                            chanstate->tremDelay, chanstate->timeindex);
                    } else {
                        deltaTime = ((ALOscInit7)seqp->initOsc)(
                            &oscState, &oscValue, inst->tremType,
                            inst->tremRate, inst->tremDepth, inst->tremDelay,
                            chanstate->timeindex);
                    }

                    if (deltaTime) {
                        evt.type             = CONKER_TREM_OSC_EVT;
                        evt.msg.osc.vs       = vstate;
                        evt.msg.osc.oscState = oscState;
                        n_alEvtqPostEvent(&seqp->evtq, &evt, deltaTime, 0);
                        vstate->flags |= 0x01;
                        vstate->oscState = oscState;
                    }
                }
            }

            vstate->tremelo = (u8)oscValue;

            oscValue = 1.0f;

            if (chanstate->usechanparams) {
                sp90 = chanstate->vibType;
            } else {
                sp90 = inst->vibType;
            }

            if (sp90) {
                if (seqp->initOsc) {
                    if (chanstate->usechanparams) {
                        deltaTime = ((ALOscInit7)seqp->initOsc)(
                            &oscState, &oscValue, chanstate->vibType,
                            chanstate->vibRate, chanstate->vibDepth,
                            chanstate->vibDelay, chanstate->timeindex);
                    } else {
                        deltaTime = ((ALOscInit7)seqp->initOsc)(
                            &oscState, &oscValue, inst->vibType,
                            inst->vibRate, inst->vibDepth, inst->vibDelay,
                            chanstate->timeindex);
                    }

                    if (deltaTime) {
                        evt.type             = CONKER_VIB_OSC_EVT;
                        evt.msg.osc.vs       = vstate;
                        evt.msg.osc.oscState = oscState;
                        evt.msg.osc.chan     = chan;
                        n_alEvtqPostEvent(&seqp->evtq, &evt, deltaTime, 0);
                        vstate->flags |= 0x02;
                        vstate->oscState2 = oscState;
                    }
                }
            }

            vstate->vibrato = oscValue;

            pitch = vstate->pitch * chanstate->pitchBend * vstate->vibrato;

            fxmix = __n_vsMix(vstate, seqp);

            sp76 = chanstate->unk14;

            if (sp76) {
                sp70 = 440 * alSemitones2Ratio(
                           cents / 100 + (u8)chanstate->unk15 - 64)
                       * chanstate->pitchBend;
            } else {
                sp70 = 127.0f;
            }

            pan = __n_vsPan(vstate, (N_ALSeqPlayer *)seqp);
            vol = __n_vsVol(vstate, (N_ALSeqPlayer *)seqp);

            if (chanstate->usechanparams) {
                deltaTime = chanstate->attackTime;
            } else {
                deltaTime = sound->envelope->attackTime;
            }

            n_alSynStartVoiceParams(voice, sound->wavetable,
                                    pitch, vol, pan, fxmix, sp76, sp70,
                                    chanstate->unk16, deltaTime);

            evt.type          = AL_SEQP_ENV_EVT;
            evt.msg.vol.voice = voice;

            if (chanstate->usechanparams) {
                evt.msg.vol.vol   = chanstate->decayVolume;
                evt.msg.vol.delta = chanstate->decayTime;
            } else {
                evt.msg.vol.vol   = sound->envelope->decayVolume;
                evt.msg.vol.delta = sound->envelope->decayTime;
            }

            n_alEvtqPostEvent(&seqp->evtq, &evt, deltaTime, 0);

            if (midi->duration) {
                evt.type            = CONKER_CSP_NOTEOFF_EVT;
                evt.msg.midi.status = chan | AL_MIDI_NoteOff;
                evt.msg.midi.byte1  = key;
                evt.msg.midi.byte2  = 0;
                deltaTime = seqp->uspt * midi->duration;
                D_80042810[chan] = deltaTime;

                n_alEvtqPostEvent(&seqp->evtq, &evt, deltaTime, 0);
            }

            if ((chanstate->unk17 & 1) && seqp->unk84) {
                osSendMesg((OSMesgQueue *)seqp->unk84,
                           (OSMesg)((D_80042810[chan] & 0xffffff00)
                                    | (chanstate->unk17 >> 2)),
                           OS_MESG_NOBLOCK);
            }

            break;
        }

        /* intentional fall-through: NoteOn vel == 0 */

    case (AL_MIDI_NoteOff):
        vstate = __n_lookupVoice((N_ALSeqPlayer *)seqp, key, chan);
        ALFlagFailIf(!vstate, seqp->debugFlags & NOTE_OFF_ERR_MASK,
                     ERR_ALSEQP_OFF_VOICE);

        chanstate = &seqp->chanState[chan];

        if (vstate->phase == AL_PHASE_SUSTAIN) {
            vstate->phase = AL_PHASE_SUSTREL;
        } else {
            vstate->phase = AL_PHASE_RELEASE;

            if (chanstate->usechanparams) {
                __n_seqpReleaseVoice((N_ALSeqPlayer *)seqp, &vstate->voice,
                                     chanstate->releaseTime);
            } else {
                __n_seqpReleaseVoice((N_ALSeqPlayer *)seqp, &vstate->voice,
                                     vstate->sound->envelope->releaseTime);
            }
        }

        if ((chanstate->unk17 & 2) && seqp->unk84) {
            osSendMesg((OSMesgQueue *)seqp->unk84,
                       (OSMesg)(key << 16 | 8 | chanstate->unk17 >> 2),
                       OS_MESG_NOBLOCK);
        }

        break;

    case (AL_MIDI_PolyKeyPressure):
        vstate = __n_lookupVoice((N_ALSeqPlayer *)seqp, key, chan);
        ALFailIf(!vstate, ERR_ALSEQP_POLY_VOICE);

        vstate->velocity = byte2;
        volume = __n_vsVol(vstate, (N_ALSeqPlayer *)seqp),
        volDelta = __n_vsDelta(vstate, seqp->curTime),
        n_alSynSetVol(&vstate->voice, volume, volDelta);
        break;

    case (AL_MIDI_ChannelPressure):
        if ((vs = seqp->vAllocHead) != 0) {
            do {
                if (vs->channel == chan) {
                    vs->velocity = byte1;
                    volume = __n_vsVol(vs, (N_ALSeqPlayer *)seqp),
                    volDelta = __n_vsDelta(vs, seqp->curTime),
                    n_alSynSetVol(&vs->voice, volume, volDelta);
                }
            } while ((vs = vs->next) != 0);
        }
        break;

    case (AL_MIDI_ControlChange):
        if (byte1 < 0x5D) {
            handler = D_8002BA50[byte1];
        } else if (byte1 >= 0xFC) {
            handler = D_8002BFC0[-byte1];
        } else {
            handler = NULL;
        }

        if (handler) {
            if (0) {
                if (chan == 2) {
                }
            }
            handler(seqp, event, chan, byte2);
        }
        break;

    case (AL_MIDI_ProgramChange):
        sp90 = (seqp->chanState[chan].unk8 << 7) + key;

        if (sp90 < seqp->bank->instCount) {
            if (func_1001B7D0(seqp, sp90, chan)) {
                evt.type            = AL_SEQP_MIDI_EVT;
                evt.msg.midi.ticks  = 0;
                evt.msg.midi.status = chan | AL_MIDI_ProgramChange;
                evt.msg.midi.byte1  = key;
                evt.msg.midi.byte2  = 0;
                n_alEvtqPostEvent(&seqp->evtq, &evt, 0x8235, 0);
            }
        } else {
            /* empty */
        }
        break;

    case (AL_MIDI_PitchBendChange):
        bendVal = ((byte2 << 7) + byte1) - 8192;
        bendCents = seqp->chanState[chan].bendRange * bendVal / 8192;
        bendRatio = alCents2Ratio(bendCents);
        seqp->chanState[chan].pitchBend = bendRatio;

        if ((vsBend = seqp->vAllocHead) != 0) {
            do {
                if (vsBend->channel == chan) {
                    n_alSynSetPitch(&vsBend->voice,
                                    vsBend->pitch * bendRatio * vsBend->vibrato);

                    if (seqp->chanState[chan].unk14) {
                        filterRatio = alSemitones2Ratio(
                            (u8)seqp->chanState[chan].unk15
                            + (vsBend->key - vsBend->sound->keyMap->keyBase)
                            - 64),
                        n_alSynFilter13(
                            &vsBend->voice,
                            filterRatio * 440 * bendRatio * vsBend->vibrato);
                    }
                }
            } while ((vsBend = vsBend->next) != 0);
        }
        break;

    default:
        break;
    }
}

static void __n_CSPHandleMetaMsg(N_ALCSPlayer *seqp, N_ALEvent *event)
{
  ALTempoEvent    *tevt = &event->msg.tempo;
  s32             tempo;
  s32             oldUspt;
  u32             ticks;
  ALMicroTime         tempDelta,curDelta = 0;
  N_ALEventListItem     *thisNode,*nextNode,*firstTemp = 0;
  N_ALEventListItem     *temp0,*temp1,*temp2;

  if (event->msg.tempo.status == AL_MIDI_Meta) {
    if (event->msg.tempo.type == AL_MIDI_META_TEMPO) {
      oldUspt = seqp->uspt;
      tempo = (tevt->byte1 << 16) | (tevt->byte2 <<  8) | (tevt->byte3 <<  0);
      __n_setUsptFromTempo (seqp, (f32)tempo);    /* sct 1/8/96 */

      thisNode = (N_ALEventListItem*)seqp->evtq.allocList.next;
      while (thisNode) {
          curDelta += thisNode->delta;
          nextNode = (N_ALEventListItem*)thisNode->node.next;
          if (thisNode->evt.type == 0x16 ) { // AL_CSP_NOTEOFF_EVT
              // custom
              temp0 = thisNode;
              if (temp0->node.next) {
                  temp0->node.next->prev = temp0->node.prev;
              }
              if (temp0->node.prev) {
                  temp0->node.prev->next = temp0->node.next;
              }
              if (firstTemp != 0) {
                  temp1 = thisNode;
                  if (1) {
                      temp2 = firstTemp;

                      temp1->node.next = temp2->node.next;
                      temp1->node.prev = temp2;

                      if (temp2->node.next != 0) {
                          temp2->node.next->prev = temp1;
                      }
                      temp2->node.next = temp1;
                  }
              } else {
                    thisNode->node.next = 0;
                    thisNode->node.prev = 0;
                    firstTemp = thisNode;
              }

              tempDelta = curDelta;                   /* record the current delta */
              if (nextNode)                           /* don't do this if no nextNode */ {
                  curDelta -= thisNode->delta;        /* subtract out this delta */
                  nextNode->delta += thisNode->delta; /* add it to next event */
              }
              thisNode->delta = tempDelta;            /* set this event delta from current */
          }
          thisNode = nextNode;
      }

      thisNode = firstTemp;
      while (thisNode) {
          nextNode = (N_ALEventListItem*)thisNode->node.next;
          ticks = thisNode->delta/oldUspt;
          thisNode->delta = ticks * seqp->uspt;
          __n_CSPRepostEvent(&seqp->evtq,thisNode);
          thisNode = nextNode;
      }
    }
  }
}

void __n_CSPRepostEvent(ALEventQueue *evtq, N_ALEventListItem *item)
{
    ALLink *node;
    N_ALEventListItem *nextItem;
    ALLink *element;
    ALLink *after;
    ALLink *element2;
    ALLink *after2;

    for (node = &evtq->allocList; node != 0; node = node->next) {
        if (!node->next) {
            element = (ALLink *)item;
            after = node;
            element->next = after->next;
            element->prev = after;
            if (after->next) {
                after->next->prev = element;
            }
            after->next = element;
            break;
        } else {
            nextItem = (N_ALEventListItem *)node->next;
            if (item->delta < nextItem->delta) {
                nextItem->delta -= item->delta;
                element2 = (ALLink *)item;
                after2 = node;
                element2->next = after2->next;
                element2->prev = after2;
                if (after2->next) {
                    after2->next->prev = element2;
                }
                after2->next = element2;
                break;
            }
            item->delta -= nextItem->delta;
        }
    }
}

void __n_setUsptFromTempo (N_ALCSPlayer *seqp, f32 tempo)
{
  if (seqp->target)
    seqp->uspt = (s32)((f32)tempo * seqp->target->qnpt);
  else
    seqp->uspt = 488;    /* This is the initial value set by alSeqpNew. */
}

void __n_CSPPostNextSeqEvent(N_ALCSPlayer *seqp)
{
  N_ALEvent   evt;
  s32    deltaTicks;

  if (seqp->state != AL_PLAYING || seqp->target == NULL)
    return;

  /* Get the next event time in ticks. */
  /* If false is returned, then there is no next delta (ie. end of sequence reached). */
  if (!__alCSeqNextDelta(seqp->target, &deltaTicks))
    return;

  evt.type = AL_SEQ_REF_EVT;
  n_alEvtqPostEvent(&seqp->evtq, &evt, deltaTicks * seqp->uspt, 0);
}
