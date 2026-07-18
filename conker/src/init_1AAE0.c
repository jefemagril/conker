#include <n_libaudio.h>
#include "n_seqp.h"

void __n_resetPerfChanState(N_ALSeqPlayer *seqp, s32 chan);

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

    vs = (N_ALVoiceState *)voice->unk10;

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

#pragma GLOBAL_ASM("asm/nonmatchings/init_1AAE0/func_1001ADA4.s")

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
        vs->voice.unk10         = vs; // this isn't right

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
#pragma GLOBAL_ASM("asm/nonmatchings/init_1AAE0/func_1001B07C.s")

s16 __n_vsVol(N_ALVoiceState *vs, N_ALSeqPlayer *seqp)
{
    u32     t1,t2;

    t1 = (vs->tremelo*vs->velocity*vs->envGain) >> 6;
    t2 = (vs->sound->sampleVolume*seqp->vol*
          seqp->chanState[vs->channel].vol) >> 14;

    if (seqp->chanState[vs->channel].unkD != 0xFF) {
        t2 = ((seqp->chanState[vs->channel].unkD * t2) + 1) >> 8;
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
    fxmix = ((seqp->chanState[vs->channel].fxmix & 0x7f) + (s32)(seqp->unk7C * 127.0f)) * seqp->unk80;

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
    seqp->chanState[chan].unk17 = 0;
    seqp->chanState[chan].unkD = 255;
    seqp->chanState[chan].unkE = 255;
    seqp->chanState[chan].unkF = 0;
    seqp->chanState[chan].unkB = 0;
    seqp->chanState[chan].unk16 = 0;
    seqp->chanState[chan].unk15 = 0;
    seqp->chanState[chan].unk14 = 0;
    seqp->chanState[chan].unk8 = 0;
}

#pragma GLOBAL_ASM("asm/nonmatchings/init_1AAE0/func_1001B7D0.s")
#pragma GLOBAL_ASM("asm/nonmatchings/init_1AAE0/func_1001BD34.s")
// s32 func_1001BD34(void *arg0, void *arg1, s32 arg2) {
//     void *sp1C;
//     s32 sp18;
//     s32 phi_v0;
//
//     sp18 = 0;
//     sp1C = arg0->unk28();
//     if (sp1C == 0) {
//         return 0;
//     }
//     if (arg2 == -1) {
//         phi_v0 = sp1C(arg1, 1);
//     } else {
//         phi_v0 = sp1C(*arg1 + (arg2 * 4) + 0x10, 0);
//     }
//     sp18 = phi_v0;
//     if ((sp18 != 0) && ((sp18 & 0xFF000003) != 0x80000000)) {
//         return 0;
//     }
//     return sp18;
// }

/* Conker synth helper: invoke n_syn->unk30 (config unk1C) as a free/stop
 * callback. index == -1 passes state directly; otherwise state+index*4+0x10. */
void func_1001BE1C(N_ALSynth *syn, void *state, s32 index)
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
