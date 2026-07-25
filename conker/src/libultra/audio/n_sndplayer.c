#include <n_libaudio.h>

extern N_ALSndpSoundState *D_8002BA20;
extern N_ALSndpSoundState *D_8002BA24;
extern N_ALSndpSoundState *D_8002BA28;
extern N_ALSndPlayerExtended *D_8002BA2C;
extern s16 D_8002BA30;
extern s16 *D_800428B8;

typedef struct {
    u16 type;
    u16 pad;
    N_ALSndpSoundState *state;
    union {
        s32 i;
        f32 f;
    } data;
    s32 data2;
} N_ALSndpEvent;

s32 _n_sndpVoiceHandler(N_ALSndPlayer *sndp);
void _n_handleEvent(N_ALSndpEvent *event);
void sndp_free_state(N_ALSndpSoundState *state);
void sndp_free_state2(N_ALSndpSoundState *state);
void sndp_apply_detune_pitch(N_ALSndpSoundState *state);
void n_alSndpFlushVoiceEvents(ALEventQueue *evtq, N_ALSndpSoundState *voice, u16 typeMask);
N_ALSndpSoundState *n_alSndpPlaySound(ALBank *bank, s16 soundNum, u16 vol, ALPan pan, f32 pitch,
                                      u8 fxmix, u8 fxbus, N_ALSndpSoundState **handle);
ALSound *__n_synBankLoad(N_ALSynth *syn, void *bankInstruments, s32 soundNum);
void __n_synBankFree(N_ALSynth *syn, void *state, s32 index);
void func_10002088(const char *fmt, ...);
void _bnkfPatchSound(void *s, s32 offset, s32 table);

#define g_SndpAllocStatesHead        D_8002BA20
#define g_SndpAllocStatesTail        D_8002BA24
#define g_SndpFreeStatesHead         D_8002BA28
#define g_SndPlayer                  D_8002BA2C
#define g_SndpNumPlaying             D_8002BA30
#define g_SndpVolumeTable            D_800428B8

#define SNDP_PLAY_EVT                0x0001
#define SNDP_STOP_EVT                0x0002
#define SNDP_PAN_EVT                 0x0004
#define SNDP_VOL_EVT                 0x0008
#define SNDP_PITCH_EVT               0x0010
#define SNDP_API_EVT                 0x0020
#define SNDP_DECAY_EVT               0x0040
#define SNDP_END_EVT                 0x0080
#define SNDP_FX_EVT                  0x0100
#define SNDP_PLAYNEXT_EVT            0x0200
#define SNDP_STOPALL_EVT             0x0400
#define SNDP_VOLTBL_EVT              0x0800
#define SNDP_STOP2_EVT               0x1000
#define SNDP_FXBUS_EVT               0x2000
#define SNDP_PLAY_SOUND_EVT          0x4000
#define SNDP_FILTER_EVT              0x8000

#define SNDP_CHAIN_BREAK_MASK        0x42D1
#define SNDP_LEAF_FLAG               0x01
#define SNDP_NO_DECAY_FLAG           0x02
#define SNDP_STATE_READY_MASK        0x03
#define SNDP_HAS_VOICE_FLAG          0x04
#define SNDP_RELATIVE_DELAY_FLAG     0x10
#define SNDP_PARENT_OF_LEAF_FLAG     0x10
#define SNDP_HAS_DETUNE_PITCH_FLAG   0x20
#define SNDP_CLEAR_PARENT_FLAG_MASK  (~SNDP_PARENT_OF_LEAF_FLAG)
#define SNDP_CHANNEL_MASK            0x1F
#define SNDP_ALL_EVENT_TYPES         0xFFFF
#define SNDP_VOLUME_TABLE_FULL       0x7FFF
#define SNDP_MAX_PRIORITY            0x40
#define SNDP_INITIAL_RETRY_COUNT     2
#define SNDP_STATE_READY             5
#define SNDP_STATE_WAITING           4
#define SNDP_PITCH_UPDATE_DELAY      33333
#define SNDP_RETRY_DELAY             0x8235
#define SNDP_STATE_VOICE(state)      ((N_ALVoice *) (state)->voice)

#define KEYMAP_FLAGS(m)              ((m)->keyMax & 0xf0)
#define KEYMAP_FXMIX(m)              (((m)->keyMax & 0x0f) * 8)
#define KEYMAP_PITCHCENTS_EXACT(m)   ((m)->keyBase * 100 + (m)->detune - 6000)
#define KEYMAP_PITCHCENTS_ROUGH(m)   ((m)->keyBase * 100 - 6000)
#define KEYMAP_VOLINDEX(m)           ((m)->keyMin & 0x1f)

#undef MIN
#undef MAX
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

void n_alCSPSetBank(N_ALCSPlayer *csp, ALBank *bank) {
    N_ALEvent event;

    event.type = AL_SEQP_BANK_EVT;
    event.msg.spbank.bank = bank;

    n_alEvtqPostEvent(&csp->evtq, &event, 0, 2);
}

void n_alSndpNew(N_ALSndpConfig *config) {
    u32 stateIndex;
    void *ptr;
    N_ALEvent event;
    N_ALSndpSoundState *states;
    N_ALSndpSoundState *state;
    N_ALSndpSoundState *prevState;

    g_SndPlayer->maxSounds = config->maxSounds;
    g_SndPlayer->target = 0;
    g_SndPlayer->drvr = n_syn;
    g_SndPlayer->frameTime = AL_USEC_PER_FRAME;

    ptr = alHeapDBAlloc(0, 0, config->heap, config->maxStates, sizeof(N_ALSndpSoundState));
    g_SndPlayer->sndState = ptr;
    g_SndPlayer->soundTableCount = config->soundTableCount;

    ptr = alHeapDBAlloc(0, 0, config->heap, config->maxEvents, sizeof(N_ALEventListItem));
    n_alEvtqNew(&g_SndPlayer->evtq, ptr, config->maxEvents);

    g_SndpFreeStatesHead = g_SndPlayer->sndState;

    for (stateIndex = 1; stateIndex < (u32) config->maxStates; stateIndex++) {
        states = g_SndPlayer->sndState;
        state = &states[stateIndex];
        prevState = &states[stateIndex - 1];
        state->node.next = prevState->node.next;
        state->node.prev = &prevState->node;
        if (prevState->node.next != 0) {
            prevState->node.next->prev = &state->node;
        }
        prevState->node.next = &state->node;
    }

    g_SndpVolumeTable = alHeapDBAlloc(0, 0, config->heap, sizeof(u16), config->maxVolumes);
    for (stateIndex = 0; stateIndex < config->maxVolumes; stateIndex++) {
        g_SndpVolumeTable[stateIndex] = SNDP_VOLUME_TABLE_FULL;
    }

    g_SndPlayer->node.next = 0;
    g_SndPlayer->node.handler = (ALVoiceHandler) _n_sndpVoiceHandler;
    g_SndPlayer->node.clientData = g_SndPlayer;
    n_alSynAddPlayer(&g_SndPlayer->node);

    event.type = SNDP_API_EVT;
    n_alEvtqPostEvent(&g_SndPlayer->evtq, &event, g_SndPlayer->frameTime, 3);
    g_SndPlayer->nextDelta = n_alEvtqNextEvent(&g_SndPlayer->evtq, &g_SndPlayer->nextEvent);
}

s32 _n_sndpVoiceHandler(N_ALSndPlayer *sp) {
    N_ALSndPlayer *alsp;
    N_ALEvent event;

    alsp = sp;
    do {
        switch (alsp->nextEvent.type) {
        case SNDP_API_EVT:
            event.type = SNDP_API_EVT;
            n_alEvtqPostEvent(&alsp->evtq, &event, alsp->frameTime, 3);
            break;
        default:
            _n_handleEvent((N_ALSndpEvent *) &alsp->nextEvent);
            break;
        }
        alsp->nextDelta = n_alEvtqNextEvent(&alsp->evtq, &alsp->nextEvent);
    } while (alsp->nextDelta == 0);

    alsp->curTime += alsp->nextDelta;
    return alsp->nextDelta;
}

void _n_handleEvent(N_ALSndpEvent *event)
{
    ALVoiceConfig config;
    ALSound *sound;
    ALKeyMap *keymap;
    ALPan pan;
    N_ALSndpEvent sp94;
    N_ALSndpEvent sp84;
    ALMicroTime delta;
    s32 fxmix;
    s32 vol;
    s32 tmppan;
    s32 isfull;
    s32 isspecial;
    s32 done = 1;
    s32 hasvoice = 0;
    N_ALSndpSoundState *state = NULL;
    N_ALSndpSoundState *nextstate = NULL;
    s32 isUnity;

    do {
        if (nextstate != NULL) {
            sp84.state = state;
            sp84.type = event->type;
            sp84.data.i = event->data.i;
            event = &sp84;
        }

        state = event->state;
        if (!state) {
            /* empty — original emits bnez-to-next */
        }

        sound = state->sound;
        nextstate = (N_ALSndpSoundState *) state->node.next;

        if (sound == NULL) {
            if (event->type != SNDP_PLAY_SOUND_EVT) {
                if (state->retryCount > 0) {
                    if (event->type != SNDP_PAN_EVT
                            && event->type != SNDP_VOL_EVT
                            && event->type != SNDP_FX_EVT
                            && event->type != SNDP_PITCH_EVT
                            && event->type != SNDP_VOLTBL_EVT
                            && event->type != SNDP_FXBUS_EVT) {
                        state->retryCount--;
                    }
                    n_alEvtqPostEvent(&g_SndPlayer->evtq, (N_ALEvent *) event, SNDP_RETRY_DELAY, 2);
                } else {
                    sndp_free_state(state);
                }
                goto loop_tail;
            }
        }

        switch (event->type) {
        case SNDP_PLAY_SOUND_EVT:
            if (sound == NULL) {
                sound = __n_synBankLoad(g_SndPlayer->drvr, (u8 *) state->bank + 0xC, state->soundNum);
                state->sound = sound;
                if (sound == NULL) {
                    event->type = SNDP_PLAY_SOUND_EVT;
                    state->state = SNDP_STATE_READY;
                    state->retryCount--;
                    n_alEvtqPostEvent(&g_SndPlayer->evtq, (N_ALEvent *) event, SNDP_RETRY_DELAY, 2);
                    break;
                }
            }

            if (sound != NULL) {
                if ((u32) sound->envelope < 0x1000000) {
                    _bnkfPatchSound(sound, (s32) sound, g_SndPlayer->soundTableCount);
                }
                if (((u32) sound->envelope & 0xFF000003) != 0x80000000) {
                    break;
                }

                keymap = sound->keyMap;
                state->sound = sound;

                isUnity = (u32) sound->envelope->decayTime + 1 == 0;
                state->priority = (s8) (isUnity + SNDP_MAX_PRIORITY);

                state->flags = KEYMAP_FLAGS(keymap) | SNDP_LEAF_FLAG;

                if (state->flags & SNDP_HAS_DETUNE_PITCH_FLAG) {
                    state->basePitch = alCents2Ratio(KEYMAP_PITCHCENTS_ROUGH(keymap));
                } else {
                    state->basePitch = alCents2Ratio(KEYMAP_PITCHCENTS_EXACT(keymap));
                }
                if (isUnity) {
                    state->flags |= SNDP_NO_DECAY_FLAG;
                }
            }

            event->type = SNDP_PLAY_EVT;
            /* fallthrough */

        case SNDP_PLAY_EVT:
            if (state->state != SNDP_STATE_READY && state->state != SNDP_STATE_WAITING) {
                return;
            }

            keymap = sound->keyMap;

            config.fxBus = state->fxbus;
            config.priority = (u8) state->priority;
            config.unityPitch = 0;
            config.unk8 = *(s32 *) ((u8 *) state->bank->instArray[0] + state->soundNum * 4 + 0x10);

            isfull = g_SndpNumPlaying >= g_SndPlayer->maxSounds;

            if (!isfull || (state->flags & SNDP_PARENT_OF_LEAF_FLAG)) {
                hasvoice = n_alSynAllocVoice(SNDP_STATE_VOICE(state), &config);
            }

            if (!hasvoice) {
                if ((state->flags & (SNDP_NO_DECAY_FLAG | SNDP_PARENT_OF_LEAF_FLAG))
                        || state->retryCount > 0) {
                    state->state = SNDP_STATE_WAITING;
                    state->retryCount--;
                    n_alEvtqPostEvent(&g_SndPlayer->evtq, (N_ALEvent *) event, SNDP_RETRY_DELAY, 2);
                } else {
                    sndp_free_state(state);
                }
                return;
            }

            state->flags |= SNDP_HAS_VOICE_FLAG;
            state->envvol = sound->envelope->attackVolume;
            state->fxbus = config.fxBus;

            delta = sound->envelope->attackTime / state->pitch / state->basePitch;
            state->endtime = g_SndPlayer->curTime + delta;

            vol = MAX(0, (g_SndpVolumeTable[KEYMAP_VOLINDEX(keymap)]
                    * (state->envvol * state->vol * sound->sampleVolume / 0x3F01))
                    / SNDP_VOLUME_TABLE_FULL - 1);

            tmppan = state->pan + sound->samplePan - AL_PAN_CENTER;
            pan = MIN(MAX(tmppan, 0), 127);

            fxmix = (state->fxmix & 0x7f) + KEYMAP_FXMIX(keymap);
            fxmix = MIN(127, MAX(0, fxmix));
            fxmix |= state->fxmix & 0x80;

            __n_synBankFree(g_SndPlayer->drvr, state->bank->instArray[0], state->soundNum);

            n_alSynStartVoiceParams(SNDP_STATE_VOICE(state), sound->wavetable,
                    state->pitch * state->basePitch, vol, pan, fxmix,
                    0, 0.0f, 0, delta);

            state->state = AL_PLAYING;
            g_SndpNumPlaying++;

            if ((state->flags & SNDP_NO_DECAY_FLAG) == 0) {
                if (delta == 0) {
                    state->envvol = sound->envelope->decayVolume;

                    vol = MAX(0, (g_SndpVolumeTable[KEYMAP_VOLINDEX(keymap)]
                            * (state->envvol * state->vol * sound->sampleVolume / 0x3F01))
                            / SNDP_VOLUME_TABLE_FULL - 1);

                    delta = sound->envelope->decayTime / state->basePitch / state->pitch;
                    state->endtime = g_SndPlayer->curTime + delta;

                    n_alSynSetVol(SNDP_STATE_VOICE(state), vol, delta);

                    sp94.type = SNDP_STOP_EVT;
                    sp94.state = state;
                    n_alEvtqPostEvent(&g_SndPlayer->evtq, (N_ALEvent *) &sp94, delta, 2);

                    if (state->flags & SNDP_HAS_DETUNE_PITCH_FLAG) {
                        sndp_apply_detune_pitch(state);
                    }
                } else {
                    sp94.type = SNDP_DECAY_EVT;
                    sp94.state = state;
                    delta = sound->envelope->attackTime / state->pitch / state->basePitch;
                    n_alEvtqPostEvent(&g_SndPlayer->evtq, (N_ALEvent *) &sp94, delta, 2);
                }
            }
            break;

        case SNDP_STOP_EVT:
        case SNDP_STOP2_EVT:
        case SNDP_STOPALL_EVT:
            if (event->type != SNDP_STOP2_EVT || (state->flags & SNDP_NO_DECAY_FLAG)) {
                switch (state->state) {
                case AL_PLAYING:
                    n_alSndpFlushVoiceEvents(&g_SndPlayer->evtq, state, SNDP_DECAY_EVT);
                    if (0) {
                        func_10002088("snd %d has been freed too early - %d\n\n\n\n\n",
                                state->state, state->state);
                        func_10002088("-2 sound has been freed too early - %d\n\n\n\n\n",
                                state->state);
                    }
                    delta = sound->envelope->releaseTime / state->basePitch / state->pitch;
                    n_alSynSetVol(SNDP_STATE_VOICE(state), 0, delta);
                    if (delta) {
                        sp94.type = SNDP_END_EVT;
                        sp94.state = state;
                        n_alEvtqPostEvent(&g_SndPlayer->evtq, (N_ALEvent *) &sp94, delta, 2);
                        state->state = AL_STOPPING;
                    } else {
                        sndp_free_state(state);
                    }
                    break;
                case SNDP_STATE_WAITING:
                case SNDP_STATE_READY:
                    sndp_free_state(state);
                    break;
                default:
                    break;
                }
                if (event->type == SNDP_STOP_EVT) {
                    event->type = SNDP_STOP2_EVT;
                }
            }
            break;

        case SNDP_PAN_EVT:
            state->pan = event->data.i;
            if (state->state == AL_PLAYING) {
                tmppan = state->pan + sound->samplePan - AL_PAN_CENTER;
                pan = MIN(MAX(tmppan, AL_PAN_LEFT), AL_PAN_RIGHT);
                n_alSynSetPan(SNDP_STATE_VOICE(state), pan);
            }
            break;

        case SNDP_PITCH_EVT:
            state->pitch = event->data.f;
            if (state->state == AL_PLAYING) {
                n_alSynSetPitch(SNDP_STATE_VOICE(state), state->pitch * state->basePitch);
                if (state->flags & SNDP_HAS_DETUNE_PITCH_FLAG) {
                    sndp_apply_detune_pitch(state);
                }
            }
            break;

        case SNDP_FX_EVT:
            keymap = sound->keyMap;
            state->fxmix = event->data.i;
            if (state->state == AL_PLAYING) {
                fxmix = (state->fxmix & 0x7f) + KEYMAP_FXMIX(keymap);
                fxmix = MIN(127, MAX(0, fxmix));
                fxmix |= state->fxmix & 0x80;
                n_alSynSetFXMix(SNDP_STATE_VOICE(state), fxmix);
            }
            break;

        case SNDP_FXBUS_EVT:
            state->fxbus = event->data.i;
            if (state->fxbus >= n_syn->maxAuxBusses) {
                state->fxbus = 0;
            }
            if (state->state == AL_PLAYING) {
                SNDP_STATE_VOICE(state)->fxBus = state->fxbus;
            }
            break;

        case SNDP_VOL_EVT:
            keymap = sound->keyMap;
            state->vol = event->data.i;
            if (state->state == AL_PLAYING) {
                vol = MAX(0, (g_SndpVolumeTable[KEYMAP_VOLINDEX(keymap)]
                        * (state->envvol * state->vol * sound->sampleVolume / 0x3F01))
                        / SNDP_VOLUME_TABLE_FULL - 1);
                n_alSynSetVol(SNDP_STATE_VOICE(state), vol,
                        MAX(1000, state->endtime - g_SndPlayer->curTime));
            }
            break;

        case SNDP_VOLTBL_EVT:
            keymap = sound->keyMap;
            if (state->state == AL_PLAYING) {
                delta = sound->envelope->releaseTime / state->basePitch / state->pitch;
                vol = MAX(0, (g_SndpVolumeTable[KEYMAP_VOLINDEX(keymap)]
                        * (state->envvol * state->vol * sound->sampleVolume / 0x3F01))
                        / SNDP_VOLUME_TABLE_FULL - 1);
                n_alSynSetVol(SNDP_STATE_VOICE(state), vol, delta);
            }
            break;

        case SNDP_DECAY_EVT:
            if ((state->flags & SNDP_NO_DECAY_FLAG) == 0) {
                keymap = sound->keyMap;
                state->envvol = sound->envelope->decayVolume;
                vol = MAX(0, (g_SndpVolumeTable[KEYMAP_VOLINDEX(keymap)]
                        * (state->envvol * state->vol * sound->sampleVolume / 0x3F01))
                        / SNDP_VOLUME_TABLE_FULL - 1);
                delta = sound->envelope->decayTime / state->basePitch / state->pitch;
                state->endtime = g_SndPlayer->curTime + delta;
                n_alSynSetVol(SNDP_STATE_VOICE(state), vol, delta);

                sp94.type = SNDP_STOP_EVT;
                sp94.state = state;
                n_alEvtqPostEvent(&g_SndPlayer->evtq, (N_ALEvent *) &sp94, delta, 2);

                if (state->flags & SNDP_HAS_DETUNE_PITCH_FLAG) {
                    sndp_apply_detune_pitch(state);
                }
            }
            break;

        case SNDP_END_EVT:
            sndp_free_state(state);
            break;

        case SNDP_PLAYNEXT_EVT:
            if (state->flags & SNDP_PARENT_OF_LEAF_FLAG) {
                n_alSndpPlaySound((ALBank *) event->data2, event->data.i, state->vol, state->pan,
                        state->pitch, state->fxmix, state->fxbus, state->handle);
            }
            break;

        case SNDP_FILTER_EVT:
            state->filter = event->data.i;
            if (state->state == AL_PLAYING) {
                n_alSynFilter11(SNDP_STATE_VOICE(state), state->filter);
            }
            break;

        default:
            break;
        }

loop_tail:
        isspecial = event->type & SNDP_CHAIN_BREAK_MASK;

        if ((state = nextstate) && !isspecial) {
            done = state->flags & SNDP_LEAF_FLAG;
        }
    } while (!done && state && !isspecial);
}

void sndp_free_state(N_ALSndpSoundState *state) {
    if ((state->flags & SNDP_HAS_VOICE_FLAG) != 0) {
        n_alSynStopVoice(SNDP_STATE_VOICE(state));
        n_alSynFreeVoice(SNDP_STATE_VOICE(state));
    }
    sndp_free_state2(state);
    n_alSndpFlushVoiceEvents(&g_SndPlayer->evtq, state, SNDP_ALL_EVENT_TYPES);
}

void sndp_apply_detune_pitch(N_ALSndpSoundState *state) {
    N_ALEvent event;
    f32 res;

    res = alCents2Ratio(state->sound->keyMap->detune) * state->pitch;

    event.type = SNDP_PITCH_EVT;
    event.msg.vol.voice = (N_ALVoice *) state;
    event.msg.vol.delta = *(s32*)&res;

    n_alEvtqPostEvent(&g_SndPlayer->evtq, &event, SNDP_PITCH_UPDATE_DELAY, 2);
}

void n_alSndpFlushVoiceEvents(ALEventQueue *evtq, N_ALSndpSoundState *voice, u16 typeMask) {
    N_ALEventListItem *current;
    N_ALEventListItem *next;
    N_ALEventListItem *item;
    N_ALEventListItem *nextEvent;
    N_ALEvent *event;
    s32 mask;
    ALLink *unlink;
    ALLink *freeItem;
    ALEventQueue *queue;

    mask = osSetIntMask(1);
    current = (N_ALEventListItem *) evtq->allocList.next;
    if (current != 0) {
        do {
            next = (N_ALEventListItem *) current->node.next;
            item = current;
            nextEvent = next;
            event = &item->evt;
            if ((event->msg.sndp.state == (N_ALUnknownStruct1 *) voice) && (((u16) event->type & typeMask) != 0)) {
                if (nextEvent != 0) {
                    nextEvent->delta += item->delta;
                }
                unlink = (ALLink *) current;
                if (unlink->next != 0) {
                    unlink->next->prev = unlink->prev;
                }
                if (unlink->prev != 0) {
                    unlink->prev->next = unlink->next;
                }
                freeItem = (ALLink *) current;
                queue = evtq;
                freeItem->next = queue->freeList.next;
                freeItem->prev = &queue->freeList;
                if (queue->freeList.next != 0) {
                    queue->freeList.next->prev = freeItem;
                }
                queue->freeList.next = freeItem;
            }
            current = next;
        } while (current != 0);
    }
    osSetIntMask(mask);
}

N_ALSndpSoundState *sndp_alloc_state(s32 bank, s16 soundIndex) {
    N_ALSndpSoundState *state;
    u32 mask;
    N_ALSndpSoundState *unlink;

    mask = osSetIntMask(1);
    state = g_SndpFreeStatesHead;
    if (state != 0) {
        g_SndpFreeStatesHead = (N_ALSndpSoundState *) state->node.next;
        unlink = state;
        if (unlink->node.next) {
            unlink->node.next->prev = unlink->node.prev;
        }
        if (unlink->node.prev) {
            unlink->node.prev->next = unlink->node.next;
        }
        if (g_SndpAllocStatesHead) {
            state->node.next = &g_SndpAllocStatesHead->node;
            state->node.prev = NULL;
            g_SndpAllocStatesHead->node.prev = &state->node;
            g_SndpAllocStatesHead = state;
        } else {
            state->node.prev = 0;
            state->node.next = state->node.prev;
            g_SndpAllocStatesHead = state;
            g_SndpAllocStatesTail = state;
        }
        osSetIntMask(mask);
        state->sound = 0;
        state->soundNum = soundIndex;
        state->bank = (ALBank *) bank;
        state->priority = SNDP_MAX_PRIORITY;
        state->state = SNDP_STATE_READY;
        state->retryCount = SNDP_INITIAL_RETRY_COUNT;
        state->flags = 0;
        state->handle = 0;
        state->basePitch = 1.0f;
    } else {
        osSetIntMask(mask);
    }
    return state;
}

void sndp_free_state2(N_ALSndpSoundState *state) {
    N_ALSndpSoundState *unlink;

    if (g_SndpAllocStatesHead == state) {
        g_SndpAllocStatesHead = (N_ALSndpSoundState *) state->node.next;
    }
    if (g_SndpAllocStatesTail == state) {
        g_SndpAllocStatesTail = (N_ALSndpSoundState *) state->node.prev;
    }

    unlink = state;
    if (unlink->node.next) {
        unlink->node.next->prev = unlink->node.prev;
    }

    if (unlink->node.prev) {
        unlink->node.prev->next = unlink->node.next;
    }

    if (g_SndpFreeStatesHead) {
        state->node.next = &g_SndpFreeStatesHead->node;
        state->node.prev = NULL;
        g_SndpFreeStatesHead->node.prev = &state->node;
        g_SndpFreeStatesHead = state;
    } else {
        state->node.prev = NULL;
        state->node.next = state->node.prev;
        g_SndpFreeStatesHead = state;
    }
    if (state->flags & SNDP_HAS_VOICE_FLAG) {
        g_SndpNumPlaying -= 1;
    }
    state->state = AL_STOPPED;
    if (state->handle) {
        if (*state->handle == state) {
            *state->handle = 0;
        }
        state->handle = NULL;
    }
}

s32 sndp_get_state(N_ALSndpSoundState **handle) {
    s32 ret;
    s32 mask;

    ret = 0;
    if (*handle) {
        mask = __osDisableInt();
        if (*handle) {
            ret = (*handle)->state;
        }
        __osRestoreInt(mask);
    }
    return ret;
}

N_ALSndpSoundState *n_alSndpPlaySound(ALBank *bank, s16 soundNum, u16 vol, ALPan pan, f32 pitch, u8 fxmix, u8 fxbus,
                                      N_ALSndpSoundState **handle) {
    N_ALSndpSoundState *state;
    N_ALSndpSoundState *leafState;
    s16 done;
    s32 delay;
    N_ALSndpEventPayload event;

    leafState = NULL;
    done = 0;
    if (soundNum != 0) {
        do {
            state = sndp_alloc_state((s32) bank, soundNum - 1);
            if (state != 0) {
                g_SndPlayer->target = (s32) state;
                event.type = SNDP_PLAY_SOUND_EVT;
                event.state = state;
                state->pan = pan;
                state->vol = vol;
                state->pitch = pitch;
                state->fxmix = fxmix;
                state->fxbus = fxbus;
                delay = 0;
                n_alEvtqPostEvent(&g_SndPlayer->evtq, (N_ALEvent *) &event, delay + 1, 2);
                leafState = state;
            }
            soundNum = 0;
        } while ((soundNum != 0) && (state != 0));
        if (leafState != 0) {
            leafState->flags |= SNDP_LEAF_FLAG;
            leafState->handle = handle;
            if (done != 0) {
            }
        }
    }
    if (handle != 0) {
        *handle = leafState;
    }
    return leafState;
}

void sndp_post_stopall_event(N_ALSndpSoundState *state) {
    N_ALEvent event;

    if (state) {
        event.type = SNDP_STOPALL_EVT;
        event.msg.sndp.state = (N_ALUnknownStruct1 *) state;
        ((N_ALSndpSoundState *) event.msg.sndp.state)->flags &= SNDP_CLEAR_PARENT_FLAG_MASK;
        n_alEvtqPostEvent(&g_SndPlayer->evtq, &event, 0, 2);
    }
}

void sndp_post_stopall_event_bulk(u8 flags) {
    s32 mask;
    N_ALSndpEventPayload event;
    N_ALSndpSoundState *state;

    mask = osSetIntMask(1);
    state = g_SndpAllocStatesHead;
    if (state != 0) {
        do {
            event.type = SNDP_STOPALL_EVT;
            event.state = state;
            if ((state->flags & flags) == flags) {
                event.state->flags &= SNDP_CLEAR_PARENT_FLAG_MASK;
                n_alEvtqPostEvent(&g_SndPlayer->evtq, (N_ALEvent *) &event, 0, 2);
            }
            state = (N_ALSndpSoundState *) state->node.next;
        } while (state != 0);
    }
    osSetIntMask(mask);
}

void sndp_stop_all(void) {
    sndp_post_stopall_event_bulk(SNDP_LEAF_FLAG);
}

void sndp_stop_nodecays(void) {
    sndp_post_stopall_event_bulk(SNDP_STATE_READY_MASK);
}

void sndp_post_event(N_ALSndpSoundState *state, s16 type, s32 data) {
    N_ALEvent event;

    if (state != 0) {
        event.type = type;
        event.msg.vol.voice = (N_ALVoice *) state;
        event.msg.vol.delta = data;
        n_alEvtqPostEvent(&g_SndPlayer->evtq, &event, 0, 2);
    }
}

void n_alSndpSetChannelValue(u8 channel, u16 value) {
    s32 mask;
    N_ALSndpSoundState *state;
    s32 voiceIndex;
    N_ALSndpEventPayload event;

    mask = osSetIntMask(1);
    state = g_SndpAllocStatesHead;
    g_SndpVolumeTable[channel] = value;
    for (voiceIndex = 0; state != 0; voiceIndex++, state = (N_ALSndpSoundState *) state->node.next) {
        if ((state->sound != 0) && ((state->sound->keyMap->keyMin & SNDP_CHANNEL_MASK) == channel)) {
            event.type = SNDP_VOLTBL_EVT;
            event.state = state;
            n_alEvtqPostEvent(&g_SndPlayer->evtq, (N_ALEvent *) &event, 0, 2);
        }
    }
    osSetIntMask(mask);
}
