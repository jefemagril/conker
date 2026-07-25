#include <n_libaudio.h>
#include "n_seqp.h"

void __n_resetPerfChanState(N_ALSeqPlayer *seqp, s32 chan);
void *__n_synBankLoad(N_ALSynth *syn, void *slot, s32 index);
void _bnkfPatchSound(void *s, s32 offset, s32 table);

void __n_unmapVoice(N_ALSeqPlayer *seqp, N_ALVoice *voice)
{
    N_ALVoiceState *prev = 0;
    N_ALVoiceState *vs;

    /*
     * we could use doubly linked lists here and save some code and
     * execution time, but time spent here in negligible, so it won't
     * make much difference.
     */
    for (vs = seqp->vAllocHead; vs != 0; vs = vs->next) {
        if (&vs->voice == voice) {
            if (prev) {
                prev->next = vs->next;
            } else {
                seqp->vAllocHead = vs->next;
            }

            if (vs == seqp->vAllocTail) {
                seqp->vAllocTail = prev;
            }

            vs->next = seqp->vFreeList;
            seqp->vFreeList = vs;
            seqp->usedVoices--;
            return;
        }
        prev = vs;
    }
}

void __n_seqpReleaseVoice(N_ALSeqPlayer *seqp, N_ALVoice *voice, ALMicroTime deltaTime)
{
    N_ALEvent evt;
    N_ALVoiceState *vs;
    ALLink *thisNode;
    ALLink *nextNode;
    N_ALEventListItem *thisItem;
    N_ALEventListItem *nextItem;
    ALLink *element;
    ALLink *linkElement;
    ALLink *after;

    vs = (N_ALVoiceState *)voice->clientPrivate;

    /*
     * if in attack phase, remove all pending volume
     * events for this voice from the queue
     */
    if (vs->envPhase == AL_PHASE_ATTACK) {
        thisNode = seqp->evtq.allocList.next;

        while (thisNode != 0) {
            nextNode = thisNode->next;
            thisItem = (N_ALEventListItem *)thisNode;
            nextItem = (N_ALEventListItem *)nextNode;

            if (thisItem->evt.type == AL_SEQP_ENV_EVT) {
                if (thisItem->evt.msg.vol.voice == voice) {
                    if (nextItem) {
                        nextItem->delta += thisItem->delta;
                    }

                    /* inlined alUnlink(thisNode) */
                    element = thisNode;
                    if (element->next != 0) {
                        element->next->prev = element->prev;
                    }
                    if (element->prev != 0) {
                        element->prev->next = element->next;
                    }

                    /* inlined alLink(thisNode, &seqp->evtq.freeList) */
                    linkElement = thisNode;
                    after = &seqp->evtq.freeList;
                    linkElement->next = after->next;
                    linkElement->prev = after;
                    if (after->next != 0) {
                        after->next->prev = linkElement;
                    }
                    after->next = linkElement;
                }
            }

            thisNode = nextNode;
        }
    }

    vs->velocity = 0;
    vs->envPhase = AL_PHASE_RELEASE;
    vs->envGain = 0;
    vs->envEndTime = seqp->curTime + deltaTime;

    n_alSynSetPriority(voice, 0); /* make candidate for stealing */
    n_alSynSetVol(voice, 0, deltaTime);

    evt.type = AL_NOTE_END_EVT;
    evt.msg.note.voice = voice;

    deltaTime += AL_USEC_PER_FRAME * 2;

    n_alEvtqPostEvent(&seqp->evtq, &evt, deltaTime, 0);
}

u8 __n_voiceNeedsNoteKill(N_ALSeqPlayer *seqp, N_ALVoice *voice, ALMicroTime killTime)
{
    ALLink *thisNode;
    ALLink *nextNode;
    N_ALEventListItem *thisItem;
    ALMicroTime itemTime = 0;
    u8 needsNoteKill = TRUE;
    ALLink *element;
    ALLink *linkElement;
    ALLink *after;

    thisNode = seqp->evtq.allocList.next;

    while (thisNode != 0) {
        nextNode = thisNode->next;
        thisItem = (N_ALEventListItem *)thisNode;
        itemTime += thisItem->delta;

        if (thisItem->evt.type == AL_NOTE_END_EVT) {
            if (thisItem->evt.msg.note.voice == voice) {
                if (itemTime > killTime) {
                    if ((N_ALEventListItem *)nextNode) {
                        ((N_ALEventListItem *)nextNode)->delta += thisItem->delta;
                    }

                    /* inlined alUnlink(thisNode) */
                    element = thisNode;
                    if (element->next != 0) {
                        element->next->prev = element->prev;
                    }
                    if (element->prev != 0) {
                        element->prev->next = element->next;
                    }

                    /* inlined alLink(thisNode, &seqp->evtq.freeList) */
                    linkElement = thisNode;
                    after = &seqp->evtq.freeList;
                    linkElement->next = after->next;
                    linkElement->prev = after;
                    if (after->next != 0) {
                        after->next->prev = linkElement;
                    }
                    if (1) {
                        after->next = linkElement;
                    }
                } else {
                    needsNoteKill = FALSE;
                }

                break;
            }
        }

        thisNode = nextNode;
    }

    return needsNoteKill;
}

N_ALVoiceState *__n_mapVoice(N_ALSeqPlayer *seqp, u8 key, u8 vel, u8 channel)
{
    N_ALVoiceState  *vs = seqp->vFreeList;

    if (seqp->usedVoices > seqp->maxVoices) {
        return NULL;
    }

    if (vs) {

        seqp->vFreeList = vs->next;

        vs->next = 0;

        if (!seqp->vAllocHead)
            seqp->vAllocHead = vs;
        else
            seqp->vAllocTail->next = vs;

        seqp->vAllocTail = vs;

        vs->channel             = channel;
        vs->key                 = key;
        vs->velocity            = vel;
        vs->voice.clientPrivate         = vs; // clientPrivate

        seqp->usedVoices++;
    }

    return vs;
}

N_ALVoiceState *__n_lookupVoice(N_ALSeqPlayer *seqp, u8 key, u8 channel)
{
    N_ALVoiceState *vs = seqp->vAllocHead;

    while (vs != 0) {
        if ((vs->key == key)
                && (vs->channel == channel)
                && (vs->phase != AL_PHASE_RELEASE)
                && (vs->phase != AL_PHASE_SUSTREL)) {
            return vs;
        }
        vs = vs->next;
    }

    return 0;
}
ALSound *__n_lookupSoundQuick(N_ALSeqPlayer *seqp, u8 key, u8 vel, u8 chan)
{
    ALInstrument *inst = seqp->chanState[chan].instrument;
    s32 l = 1;
    s32 r;
    s32 i;
    ALKeyMap *keymap;

    if (inst == 0) {
        return 0;
    }

    r = inst->soundCount;

    while (r >= l) {
        i = (l + r) / 2;

        keymap = inst->soundArray[i - 1]->keyMap;

        if (key >= keymap->keyMin && key <= keymap->keyMax
                && vel >= keymap->velocityMin && vel <= keymap->velocityMax) {
            return inst->soundArray[i - 1];
        } else if (key < keymap->keyMin
                || (vel < keymap->velocityMin && key <= keymap->keyMax)) {
            r = i - 1;
        } else {
            l = i + 1;
        }
    }

    return 0;
}

s16 __n_vsVol(N_ALVoiceState *vs, N_ALSeqPlayer *seqp)
{
    u32     t1,t2;

    t1 = (vs->tremelo*vs->velocity*vs->envGain) >> 6;
    t2 = (vs->sound->sampleVolume*seqp->vol*
          seqp->chanState[vs->channel].vol) >> 14;

    if (seqp->chanState[vs->channel].fadevolcurrent != 0xFF) {
        t2 = ((seqp->chanState[vs->channel].fadevolcurrent * t2) + 1) >> 8;
    }

    t1 *= t2;
    t1 >>= 15;

    return t1;
}

s32 __n_vsMix(N_ALVoiceState *vs, N_ALCSPlayer *seqp)
{
    s32 sign;
    s32 fxmix;

    sign = seqp->chanState[vs->channel].fxmix & 0x80;
    fxmix = ((seqp->chanState[vs->channel].fxmix & 0x7f) + (s32)(seqp->fxmixmajor * 127.0f)) * seqp->fxmixmega;

    return (MAX(0, MIN(127, fxmix)) | sign) & 0xff;
}

ALMicroTime __n_vsDelta(N_ALVoiceState *vs, ALMicroTime t) {
  /*
   * If we are interrupting a previously set envelope segment, we
   * need to recalculate the segment end time given the current
   * time. Note: this routine assumes that the voice is currently
   * playing.
   */

  s32 delta = vs->envEndTime - t;

  if (delta >= 0) {
      return delta;
  } else {
      return AL_GAIN_CHANGE_TIME;
  }
}

ALPan __n_vsPan(N_ALVoiceState *vs, N_ALSeqPlayer *seqp)
{
    s32 tmp;

    tmp = seqp->chanState[vs->channel].pan - AL_PAN_CENTER +
        vs->sound->samplePan;
    tmp = MAX(tmp, AL_PAN_LEFT);
    tmp = MIN(tmp, AL_PAN_RIGHT);

    return (ALPan) tmp;
}

/* Conker: find first instrument from instArray[1], reset channels only
 * (no __n_setInstChanState). PD/libreultra also setInst from the found
 * instrument and percussion. */
void __n_initFromBank(N_ALSeqPlayer *seqp, ALBank *b)
{
    s32 i;
    ALInstrument *inst = 0;

    for (i = 1; !inst; i++) {
        inst = b->instArray[i];
    }

    for (i = 0; i < seqp->maxChannels; i++) {
        __n_resetPerfChanState(seqp, i);
    }

    if (b->percussion) {
        __n_resetPerfChanState(seqp, i);
    }
}

void __n_initChanState(N_ALSeqPlayer *seqp)
{
    int i;

    for (i = 0; i < seqp->maxChannels; i++)
    {
        seqp->chanState[i].instrument = 0;
        __n_resetPerfChanState (seqp, i);
    }
}

void __n_resetPerfChanState(N_ALSeqPlayer *seqp, s32 chan) {
    seqp->chanState[chan].fxmix = 0;
    seqp->chanState[chan].pan = AL_PAN_CENTER;
    seqp->chanState[chan].vol = AL_VOL_FULL;
    seqp->chanState[chan].priority = AL_DEFAULT_PRIORITY;
    seqp->chanState[chan].sustain = 0;
    seqp->chanState[chan].bendRange = 200;
    seqp->chanState[chan].pitchBend = 1.0f;
    seqp->chanState[chan].notemesgflags = 0;
    seqp->chanState[chan].fadevolcurrent = 255;
    seqp->chanState[chan].fadevoltarget = 255;
    seqp->chanState[chan].fadevolinc = 0;
    seqp->chanState[chan].fxbus = 0;
    seqp->chanState[chan].filter11 = 0;
    seqp->chanState[chan].filterPitch = 0;
    seqp->chanState[chan].filter12 = 0;
    seqp->chanState[chan].instmajor = 0;
}

/*
 * Conker async ProgramChange: load instrument by bank slot, free prior,
 * patch sounds, copy into chanState. Returns 1 if load still pending.
 */
s32 __n_cspLoadInstChanState(N_ALCSPlayer *seqp, s32 prog, s32 chan)
{
    ALSound *sound;
    ALInstrument *inst;
    s32 i;

    inst = __n_synBankLoad(seqp->drvr, (u8 *)seqp->bank + prog * 4 + 0xC, -1);

    if (seqp->chanState[chan].instrument != 0) {
        ((void (*)(ALInstrument *))seqp->drvr->unk34)(
            seqp->bank->instArray[seqp->chanState[chan].instIndex]);
        seqp->chanState[chan].instrument = 0;
    }

    if (inst != 0) {
        for (i = 0; i < inst->soundCount; i++) {
            sound = inst->soundArray[i];
            if ((u32)sound->envelope < 0x100000) {
                _bnkfPatchSound(sound, (s32)inst, (s32)seqp->drvr->unk38);
            }
        }
        sound = inst->soundArray[0];
    }

    if (inst != 0) {
        if (inst->soundCount == 0) {
            return 0;
        }

        if (sound != 0) {
            seqp->chanState[chan].attackTime = sound->envelope->attackTime;
            seqp->chanState[chan].decayTime = sound->envelope->decayTime;
            seqp->chanState[chan].releaseTime = sound->envelope->releaseTime;
            seqp->chanState[chan].attackVolume = sound->envelope->attackVolume;
            seqp->chanState[chan].decayVolume = sound->envelope->decayVolume;
        }

        seqp->chanState[chan].pan = inst->pan;
        seqp->chanState[chan].vol = inst->volume;
        seqp->chanState[chan].priority = inst->priority;
        seqp->chanState[chan].bendRange = inst->bendRange;
        seqp->chanState[chan].tremType = inst->tremType;
        seqp->chanState[chan].tremRate = inst->tremRate;
        seqp->chanState[chan].tremDepth = inst->tremDepth;
        seqp->chanState[chan].tremDelay = inst->tremDelay;
        seqp->chanState[chan].vibType = inst->vibType;
        seqp->chanState[chan].vibRate = inst->vibRate;
        seqp->chanState[chan].vibDepth = inst->vibDepth;
        seqp->chanState[chan].vibDelay = inst->vibDelay;
        seqp->chanState[chan].muted = 0;
        seqp->chanState[chan].instrument = inst;
    } else {
        seqp->chanState[chan].muted = 1;
    }

    seqp->chanState[chan].pitch = 0;
    seqp->chanState[chan].usechanparams = 0;
    seqp->chanState[chan].timeindex = 0;
    seqp->chanState[chan].instIndex = prog;

    if (inst == 0) {
        return 1;
    }
    return 0;
}

/* Resolve a bank instrument (index == -1) or sound slot via syn->unk28 loader. */
void *__n_synBankLoad(N_ALSynth *syn, void *slot, s32 index)
{
    void *(*loader)(void *, s32);
    u32 result = 0;

    loader = ((void *(*)(void))syn->unk28)();
    if (loader != 0) {
        if (index == -1) {
            result = (u32)loader(slot, 1);
        } else {
            result = (u32)loader((void *)((u8 *)*(void **)slot + index * 4 + 0x10), 0);
        }
        if ((result != 0) && ((result & 0xFF000003) != 0x80000000)) {
            return 0;
        }
        goto return_result;
    }
    return 0;
return_result:
    return (void *)result;
}

/* Free counterpart: syn->unk30 callback. index == -1 passes state directly. */
void __n_synBankFree(N_ALSynth *syn, void *state, s32 index)
{
    if (index == -1) {
        ((void (*)(void *))syn->unk30)(state);
    } else {
        ((void (*)(void *))syn->unk30)(*(void **)((u8 *)state + (index * 4) + 0x10));
    }
}

/* Conker osc event ids (header AL_TREM/VIB_OSC_EVT are off by one). */
#define CONKER_AL_TREM_OSC_EVT 0x17
#define CONKER_AL_VIB_OSC_EVT  0x18

void __n_seqpStopOsc(N_ALSeqPlayer *seqp, N_ALVoiceState *vs)
{
    N_ALEventListItem *thisNode;
    N_ALEventListItem *nextNode;
    s16 evtType;
    ALLink *element;
    ALLink *linkElement;
    ALLink *after;

    thisNode = (N_ALEventListItem *)seqp->evtq.allocList.next;

    while (thisNode) {
        nextNode = (N_ALEventListItem *)thisNode->node.next;
        evtType = thisNode->evt.type;

        if (evtType == CONKER_AL_TREM_OSC_EVT || evtType == CONKER_AL_VIB_OSC_EVT) {
            if (thisNode->evt.msg.osc.vs == vs) {
                (*seqp->stopOsc)(thisNode->evt.msg.osc.oscState);

                /* inlined alUnlink */
                element = (ALLink *)thisNode;
                if (element->next != 0) {
                    element->next->prev = element->prev;
                }
                if (element->prev != 0) {
                    element->prev->next = element->next;
                }

                if (nextNode) {
                    nextNode->delta += thisNode->delta;
                }

                /* inlined alLink to freeList */
                linkElement = (ALLink *)thisNode;
                after = &seqp->evtq.freeList;
                linkElement->next = after->next;
                linkElement->prev = after;
                if (after->next != 0) {
                    after->next->prev = linkElement;
                }
                after->next = linkElement;

                if (evtType == CONKER_AL_TREM_OSC_EVT) {
                    vs->flags &= 0xfe;
                } else { /* must be a AL_VIB_OSC_EVT */
                    vs->flags &= 0xfd;
                }

                if (!vs->flags) {
                    return;  /* there should be no more events */
                }
            }
        }

        thisNode = nextNode;
    }
}
