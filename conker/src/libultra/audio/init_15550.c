#include <n_libaudio.h>

typedef struct N_ALSndpSoundState {
    /* 0x00 */ ALLink node;
    /* 0x08 */ u8 pad8[0x4];
    /* 0x0C */ ALSound *sound;
    /* 0x10 */ u8 voice[0x20]; /* N_ALVoice storage; local header layout is still incomplete */
    /* 0x30 */ f32 basePitch;
    /* 0x34 */ f32 pitch;
    /* 0x38 */ struct N_ALSndpSoundState **handle;
    /* 0x3C */ ALBank *bank;
    /* 0x40 */ s32 retryCount;
    /* 0x44 */ u16 vol;
    /* 0x46 */ u8 pad46[0x6];
    /* 0x4C */ s16 soundNum;
    /* 0x4E */ s8 priority;
    /* 0x4F */ u8 pan;
    /* 0x50 */ u8 fxmix;
    /* 0x51 */ u8 fxbus;
    /* 0x52 */ u8 pad52;
    /* 0x53 */ u8 flags;
    /* 0x54 */ u8 state;
} N_ALSndpSoundState;

typedef struct {
    /* 0x00 */ ALPlayer node;
    /* 0x14 */ ALEventQueue evtq;
    /* 0x28 */ N_ALEvent nextEvent;
    /* 0x38 */ N_ALSynth *drvr;
    /* 0x3C */ s32 target;
    /* 0x40 */ N_ALSndpSoundState *sndState;
    /* 0x44 */ s32 maxSounds;
    /* 0x48 */ ALMicroTime frameTime;
    /* 0x4C */ ALMicroTime nextDelta;
    /* 0x50 */ ALMicroTime curTime;
    /* 0x54 */ s32 soundTableCount;
} N_ALSndPlayerExtended;

extern N_ALUnknownStruct1 *D_8002BA20;
extern N_ALUnknownStruct1 *D_8002BA24;
extern N_ALUnknownStruct1 *D_8002BA28;
extern N_ALSndPlayer *D_8002BA2C;
extern s16 D_8002BA30;
extern u16 *D_800428B8;

s32 _n_sndpVoiceHandler(N_ALSndPlayer *sndp);
N_ALUnknownStruct1 *sndp_alloc_state(s32 bank, s16 soundIndex);
void sndp_free_state2(N_ALUnknownStruct1 *state);
void n_alSndpFlushVoiceEvents(ALEventQueue *evtq, N_ALUnknownStruct1 *voice, u16 typeMask);

#define SNDP_VOICE_UPDATE_EVT        0x400
#define SNDP_VOICE_CHANNEL_EVT       0x800
#define SNDP_ACTIVE_FLAG             0x01
#define SNDP_STATE_READY_MASK        0x03
#define SNDP_CLEAR_RESTART_FLAG_MASK -0x11
#define SNDP_CHANNEL_MASK            0x1F
#define SNDP_ALL_EVENT_TYPES         0xFFFF
#define SNDP_API_EVT                 0x20
#define SNDP_PLAY_SOUND_EVT          0x4000

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

    D_8002BA2C->maxSounds = config->maxSounds;
    D_8002BA2C->target = 0;
    D_8002BA2C->drvr = n_syn;
    D_8002BA2C->frameTime = 16000;

    ptr = alHeapDBAlloc(0, 0, config->heap, config->maxStates, sizeof(N_ALSndpSoundState));
    D_8002BA2C->sndState = ptr;
    ((N_ALSndPlayerExtended *) D_8002BA2C)->soundTableCount = config->soundTableCount;

    ptr = alHeapDBAlloc(0, 0, config->heap, config->maxEvents, sizeof(N_ALEventListItem));
    n_alEvtqNew(&D_8002BA2C->evtq, ptr, config->maxEvents);

    D_8002BA28 = (N_ALUnknownStruct1 *) D_8002BA2C->sndState;

    for (stateIndex = 1; stateIndex < (u32) config->maxStates; stateIndex++) {
        states = (N_ALSndpSoundState *) D_8002BA2C->sndState;
        state = &states[stateIndex];
        prevState = &states[stateIndex - 1];
        state->node.next = prevState->node.next;
        state->node.prev = &prevState->node;
        if (prevState->node.next != 0) {
            prevState->node.next->prev = &state->node;
        }
        prevState->node.next = &state->node;
    }

    D_800428B8 = alHeapDBAlloc(0, 0, config->heap, sizeof(u16), config->maxVolumes);
    for (stateIndex = 0; stateIndex < config->maxVolumes; stateIndex++) {
        D_800428B8[stateIndex] = 0x7FFF;
    }

    D_8002BA2C->node.next = 0;
    D_8002BA2C->node.handler = (ALVoiceHandler) _n_sndpVoiceHandler;
    D_8002BA2C->node.clientData = D_8002BA2C;
    n_alSynAddPlayer(&D_8002BA2C->node);

    event.type = SNDP_API_EVT;
    n_alEvtqPostEvent(&D_8002BA2C->evtq, &event, D_8002BA2C->frameTime, 3);
    D_8002BA2C->nextDelta = n_alEvtqNextEvent(&D_8002BA2C->evtq, &D_8002BA2C->nextEvent);
}

s32 _n_sndpVoiceHandler(N_ALSndPlayer *sp) {
    N_ALSndPlayer *alsp;
    N_ALEvent event;

    alsp = sp;
    do {
        switch (alsp->nextEvent.type) {
        case 32:
            event.type = 32;
            n_alEvtqPostEvent(&alsp->evtq, &event, alsp->frameTime, 3);
            break;
        default:
            _n_handleEvent(&alsp->nextEvent);
            break;
        }
        alsp->nextDelta = n_alEvtqNextEvent(&alsp->evtq, &alsp->nextEvent);
    } while (alsp->nextDelta == 0);

    alsp->curTime += alsp->nextDelta;
    return alsp->nextDelta;
}

// jump table
#pragma GLOBAL_ASM("asm/nonmatchings/libultra/audio/init_15550/_n_handleEvent.s")

void sndp_free_state(N_ALUnknownStruct1 *arg0) {
    if ((arg0->unk53 & 4) != 0) {
        n_alSynStopVoice(&arg0->unk10);
        n_alSynFreeVoice(&arg0->unk10);
    }
    sndp_free_state2(arg0);
    n_alSndpFlushVoiceEvents(&D_8002BA2C->evtq, arg0, SNDP_ALL_EVENT_TYPES);
}

void sndp_apply_detune_pitch(N_ALSndpSoundState *state) {
    N_ALEvent event;
    f32 res;

    res = alCents2Ratio(state->sound->keyMap->detune) * state->pitch;

    event.type = 16;
    /* TODO: check if this is the right struct */
    event.msg.vol.voice = (N_ALVoice *) state;
    event.msg.vol.delta = *(s32*)&res;

    n_alEvtqPostEvent(&D_8002BA2C->evtq, &event, 33333, 2);
}

void n_alSndpFlushVoiceEvents(ALEventQueue *evtq, N_ALUnknownStruct1 *voice, u16 typeMask) {
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
            if ((event->msg.unknown1.unk0 == voice) && (((u16) event->type & typeMask) != 0)) {
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

N_ALUnknownStruct1 *sndp_alloc_state(s32 bank, s16 soundIndex) {
    N_ALUnknownStruct1 *sp24;
    u32 mask;
    N_ALUnknownStruct1 *sp1C;

    mask = osSetIntMask(1);
    sp24 = D_8002BA28;
    if (sp24 != 0) {
        D_8002BA28 = sp24->node.next;
        sp1C = sp24;
        if (sp1C->node.next) {
            sp1C->node.next->prev = sp1C->node.prev;
        }
        if (sp1C->node.prev) {
            sp1C->node.prev->next = sp1C->node.next;
        }
        if (D_8002BA20) {
            sp24->node.next = D_8002BA20;
            sp24->node.prev = NULL;
            D_8002BA20->node.prev = sp24;
            D_8002BA20 = sp24;
        } else {
            sp24->node.prev = 0;
            sp24->node.next = sp24->node.prev;
            D_8002BA20 = sp24;
            D_8002BA24 = sp24;
        }
        osSetIntMask(mask);
        sp24->unkC = 0;
        sp24->unk4C = soundIndex;
        sp24->unk3C = bank;
        sp24->unk4E = 64;
        sp24->unk54 = 5;
        sp24->unk40 = 2;
        sp24->unk53 = 0;
        sp24->unk38 = 0;
        sp24->unk30 = 1.0f;
    } else {
        osSetIntMask(mask);
    }
    return sp24;
}

void sndp_free_state2(N_ALUnknownStruct1 *arg0) {
    N_ALUnknownStruct1 *sp4;

    if (D_8002BA20 == arg0) {
        D_8002BA20 = arg0->node.next;
    }
    if (D_8002BA24 == arg0) {
        D_8002BA24 = arg0->node.prev;
    }

    sp4 = arg0;
    if (sp4->node.next) {
        sp4->node.next->prev = sp4->node.prev;
    }

    if (sp4->node.prev) {
        sp4->node.prev->next = sp4->node.next;
    }

    if (D_8002BA28) {
        arg0->node.next = D_8002BA28;
        arg0->node.prev = NULL;
        D_8002BA28->node.prev = arg0;
        D_8002BA28 = arg0;
    } else {
        arg0->node.prev = NULL;
        arg0->node.next = arg0->node.prev;
        D_8002BA28 = arg0;
    }
    if (arg0->unk53 & 4) {
        D_8002BA30 -= 1;
    }
    arg0->unk54 = 0;
    if (arg0->unk38) {
        if (*arg0->unk38 == (s32)arg0) {
            *arg0->unk38 = 0;
        }
        arg0->unk38 = NULL;
    }
}

s32 sndp_get_state(N_ALUnknownEvent2 *arg0) {
    s32 ret;
    s32 mask;

    ret = 0;
    if (arg0->unk0) {
        mask = __osDisableInt();
        if (arg0->unk0) {
            ret = arg0->unk0->unk54;
        }
        __osRestoreInt(mask);
    }
    return ret;
}

N_ALSndpSoundState *n_alSndpPlaySound(ALBank *bank, s16 soundNum, u16 vol, ALPan pan, f32 pitch, u8 fxmix, u8 fxbus,
                                      N_ALSndpSoundState **handle) {
    typedef struct N_ALSndpEventFragment {
        s16 type;
        u8 pad2[2];
        N_ALSndpSoundState *state;
        u8 pad8[8];
    } N_ALSndpEventFragment;

    N_ALSndpSoundState *state;
    N_ALSndpSoundState *leafState;
    s16 done;
    s32 delay;
    N_ALSndpEventFragment event;

    leafState = NULL;
    done = 0;
    if (soundNum != 0) {
        do {
            state = (N_ALSndpSoundState *) sndp_alloc_state((s32) bank, soundNum - 1);
            if (state != 0) {
                D_8002BA2C->target = (s32) state;
                event.type = SNDP_PLAY_SOUND_EVT;
                event.state = state;
                state->pan = pan;
                state->vol = vol;
                state->pitch = pitch;
                state->fxmix = fxmix;
                state->fxbus = fxbus;
                delay = 0;
                n_alEvtqPostEvent(&D_8002BA2C->evtq, (N_ALEvent *) &event, delay + 1, 2);
                leafState = state;
            }
            soundNum = 0;
        } while ((soundNum != 0) && (state != 0));
        if (leafState != 0) {
            leafState->flags |= SNDP_ACTIVE_FLAG;
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

void sndp_post_end_event(N_ALUnknownStruct1 *arg0) {
    N_ALEvent event;

    if (arg0) {
        event.type = SNDP_VOICE_UPDATE_EVT;
        event.msg.unknown1.unk0 = arg0;
        event.msg.unknown1.unk0->unk53 &= SNDP_CLEAR_RESTART_FLAG_MASK;
        n_alEvtqPostEvent(&D_8002BA2C->evtq, &event, 0, 2);
    }
}

void n_alSndpPostVoiceUpdateEvents(u8 flags) {
    typedef struct N_ALVoiceEventFragment {
        s16 type;
        u8 pad2[2];
        N_ALUnknownStruct1 *voice;
        u8 pad8[8];
    } N_ALVoiceEventFragment;

    s32 mask;
    N_ALVoiceEventFragment event;
    N_ALUnknownStruct1 *voice;

    mask = osSetIntMask(1);
    voice = D_8002BA20;
    if (voice != 0) {
        do {
            event.type = SNDP_VOICE_UPDATE_EVT;
            event.voice = voice;
            if ((voice->unk53 & flags) == flags) {
                event.voice->unk53 &= SNDP_CLEAR_RESTART_FLAG_MASK;
                n_alEvtqPostEvent(&D_8002BA2C->evtq, (N_ALEvent *) &event, 0, 2);
            }
            voice = voice->node.next;
        } while (voice != 0);
    }
    osSetIntMask(mask);
}

void n_alSndpPostActiveVoiceUpdates(void) {
    n_alSndpPostVoiceUpdateEvents(SNDP_ACTIVE_FLAG);
}

void n_alSndpPostReadyVoiceUpdates(void) {
    n_alSndpPostVoiceUpdateEvents(SNDP_STATE_READY_MASK);
}

void sndp_post_event(N_ALUnknownStruct1 *state, s16 type, s32 data) {
    N_ALEvent event;

    if (state != 0) {
        event.type = type;
        event.msg.vol.voice = (N_ALVoice *) state;
        event.msg.vol.delta = data;
        n_alEvtqPostEvent(&D_8002BA2C->evtq, &event, 0, 2);
    }
}

void n_alSndpSetChannelValue(u8 channel, u16 value) {
    typedef struct N_ALVoiceEventFragment {
        s16 type;
        u8 pad2[2];
        N_ALUnknownStruct1 *voice;
        u8 pad8[8];
    } N_ALVoiceEventFragment;

    s32 mask;
    N_ALUnknownStruct1 *voice;
    s32 voiceIndex;
    N_ALVoiceEventFragment event;

    mask = osSetIntMask(1);
    voice = D_8002BA20;
    D_800428B8[channel] = value;
    for (voiceIndex = 0; voice != 0; voiceIndex++, voice = voice->node.next) {
        if ((voice->unkC != 0) && ((((ALSound *) voice->unkC)->keyMap->keyMin & SNDP_CHANNEL_MASK) == channel)) {
            event.type = SNDP_VOICE_CHANNEL_EVT;
            event.voice = voice;
            n_alEvtqPostEvent(&D_8002BA2C->evtq, (N_ALEvent *) &event, 0, 2);
        }
    }
    osSetIntMask(mask);
}
