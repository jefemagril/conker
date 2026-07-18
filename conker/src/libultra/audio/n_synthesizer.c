#include "n_synthInternals.h"

extern f32 D_8002C750;
Acmd *func_1001FB40(s32 sampleOffset, Acmd *p);
Acmd *func_1001E530(s32 sampleOffset, Acmd *p);
s32 func_1001E4A0(s16 bus, ALSynConfig *c, ALHeap *hp);
s32 __n_nextSampleTime(ALPlayer **client);
s32 _n_timeToSamplesNoRound(s32 micros);

#ifndef MIN
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#endif

/* Conker free-list ALParam nodes are 0x24 bytes (larger than vanilla ALParam). */
#define N_ALPARAM_SIZE 0x24

void n_alSynNew(ALSynConfig *c)
{
    s32 i;
    s32 j;
    N_PVoice *pv;
    N_PVoice *pvoices;
    ALHeap *hp = c->heap;
    ALParam *params;
    ALParam *paramPtr;
    ALLink *element;
    ALLink *after;

    n_syn->head = NULL;
    n_syn->numPVoices = c->maxPVoices;
    n_syn->curSamples = 0;
    n_syn->paramSamples = 0;
    n_syn->outputRate = c->outputRate;
    n_syn->maxOutSamples = FIXED_SAMPLE;
    n_syn->dma = (ALDMANew)c->dmaproc;
    n_syn->unk28 = c->unk14;
    n_syn->unk2C = c->unk18;
    n_syn->unk30 = c->unk1C;
    n_syn->unk34 = c->unk20;
    n_syn->unk38 = c->unk24;

    if (c->maxFXbusses > 2) {
        n_syn->maxAuxBusses = 2;
    } else if (c->maxFXbusses < 1) {
        n_syn->maxAuxBusses = 1;
    } else {
        n_syn->maxAuxBusses = c->maxFXbusses;
    }

    n_syn->auxBus = (N_ALAuxBus *)alHeapDBAlloc(0, 0, hp, n_syn->maxAuxBusses, sizeof(N_ALAuxBus));

    for (i = 0; i < n_syn->maxAuxBusses; i++) {
        n_syn->auxBus[i].sourceCount = 0;
        n_syn->auxBus[i].maxSources = 0;

        if (c->fxTypes[i]) {
            /* func_1001E4A0 / n_alFxNew write FX at +0x20; return also stored at +0x1C */
            n_syn->auxBus[i].sources = (N_PVoice **)func_1001E4A0(i, c, hp);
        } else {
            n_syn->auxBus[i].sources = 0;
        }

        n_syn->auxBus[i].fx_array[7] =
            (ALFx *)alHeapDBAlloc(0, 0, hp, 1, sizeof(struct auxbus44));
        ((struct auxbus44 *)n_syn->auxBus[i].fx_array[7])->unk02 = 0;
        ((struct auxbus44 *)n_syn->auxBus[i].fx_array[7])->unk2c =
            alHeapDBAlloc(0, 0, hp, 1, sizeof(POLEF_STATE));
        ((struct auxbus44 *)n_syn->auxBus[i].fx_array[7])->unk30 =
            alHeapDBAlloc(0, 0, hp, 1, sizeof(POLEF_STATE));
    }

    n_syn->mainBus = (N_ALMainBus *)alHeapDBAlloc(0, 0, hp, 1, sizeof(N_ALMainBus));
    n_syn->mainBus->filter.handler = (N_ALCmdHandler)func_1001E530;

    n_syn->pFreeList.next = 0;
    n_syn->pFreeList.prev = 0;
    n_syn->pLameList.next = 0;
    n_syn->pLameList.prev = 0;
    n_syn->pAllocList.next = 0;
    n_syn->pAllocList.prev = 0;

    pvoices = alHeapDBAlloc(0, 0, hp, c->maxPVoices, sizeof(N_PVoice));

    for (i = 0; i < c->maxPVoices; i++) {
        pv = &pvoices[i];

        /* inlined alLink(pv, &n_syn->pFreeList) */
        element = (ALLink *)pv;
        after = &n_syn->pFreeList;
        element->next = after->next;
        element->prev = after;
        if (after->next != 0) {
            after->next->prev = element;
        }
        after->next = element;

        pv->vvoice = 0;
        alN_PVoiceNew(pv, n_syn->dma, hp);
    }

    /* Reset aux-bus voice-list heads (no sources[] pre-registration like PD) */
    for (j = 0; j < n_syn->maxAuxBusses; j++) {
        n_syn->auxBus[j].sourceCount = 0;
        n_syn->auxBus[j].maxSources = 0;
    }

    params = alHeapDBAlloc(0, 0, hp, c->maxUpdates, N_ALPARAM_SIZE);
    n_syn->paramList = 0;

    for (i = 0; i < c->maxUpdates; i++) {
        paramPtr = (ALParam *)((i * N_ALPARAM_SIZE) + (s32)params);
        paramPtr->next = n_syn->paramList;
        n_syn->paramList = paramPtr;
    }

    n_syn->heap = hp;
}

Acmd *n_alAudioFrame(Acmd *cmdList, s32 *cmdLen, s16 *outBuf, s32 outLen) {
    ALPlayer *client;
    Acmd *cmdlEnd = cmdList;
    s32 nOut;
    s16 *lOutBuf = outBuf;
    Acmd *cmdPtr;
    Acmd *cmdPtr2;

    if (n_syn->head == 0) {
        *cmdLen = 0;
        return cmdList;
    }

    for (n_syn->paramSamples = __n_nextSampleTime(&client);
         (u32)(n_syn->paramSamples - n_syn->curSamples) < (u32)outLen;
         n_syn->paramSamples = __n_nextSampleTime(&client)) {
        n_syn->paramSamples &= ~0xf;
        client->samplesLeft += _n_timeToSamplesNoRound((*client->handler)(client));
    }

    n_syn->paramSamples &= ~0xf;

    while (outLen > 0) {
        nOut = MIN(n_syn->maxOutSamples, outLen);

        cmdlEnd = func_1001FB40(n_syn->curSamples, cmdlEnd);
        cmdPtr = cmdlEnd++;
        cmdPtr->words.w0 = 0x0D000000;

        cmdPtr2 = cmdlEnd++;
        cmdPtr2->words.w0 = 0x062E0000;
        cmdPtr2->words.w1 = (u32)lOutBuf;

        outLen -= nOut;
        lOutBuf += nOut << 1;
        n_syn->curSamples += nOut;
    }

    *cmdLen = (s32)(cmdlEnd - cmdList);
    _n_collectPVoices();
    return cmdlEnd;
}

ALParam *__n_allocParam(void) {
    ALParam *update = 0;

    if (n_syn->paramList) {
        update = n_syn->paramList;
        n_syn->paramList = n_syn->paramList->next;
        update->next = 0;
    }
    return update;
}


void _n_freeParam(ALParam *param)
{
  param->next = n_syn->paramList;
  n_syn->paramList = param;
}

void _n_collectPVoices(void) {
    ALLink *dl;
    ALLink *element;
    ALLink *linkElement;
    ALLink *after;
    ALLink *voiceLink;

    while ((dl = n_syn->pLameList.next) != 0) {
        element = dl;

        if (element->next != 0) {
            element->next->prev = element->prev;
        }

        if (element->prev != 0) {
            element->prev->next = element->next;
        }

        linkElement = dl;
        after = &n_syn->pFreeList;
        linkElement->next = after->next;
        linkElement->prev = after;

        if (after->next != 0) {
            after->next->prev = linkElement;
        }

        after->next = linkElement;

        voiceLink = &((N_PVoice *)dl)->voiceLink;

        if (voiceLink->next != 0) {
            voiceLink->next->prev = voiceLink->prev;
        }

        if (voiceLink->prev != 0) {
            voiceLink->prev->next = voiceLink->next;
        }

        ((N_PVoice *)dl)->voiceLink.next = 0;
        ((N_PVoice *)dl)->voiceLink.prev = 0;
    }
}

void _n_freePVoice(N_PVoice *pvoice) {
    N_PVoice *voice = pvoice;
    ALLink *element;
    ALLink *after;

    if (((ALLink *)voice)->next != 0) {
        ((ALLink *)voice)->next->prev = ((ALLink *)voice)->prev;
    }

    if (((ALLink *)voice)->prev != 0) {
        ((ALLink *)voice)->prev->next = ((ALLink *)voice)->next;
    }

    element = (ALLink *)pvoice;
    after = &n_syn->pLameList;
    element->next = after->next;
    element->prev = after;

    if (after->next != 0) {
        after->next->prev = element;
    }

    after->next = element;
}

s32 _n_timeToSamplesNoRound(s32 micros) {
    f32 tmp = (((f32) micros * (f32) n_syn->outputRate) / D_8002C750) + 0.5f; // 1000000.0f
    return (s32) tmp;
}

s32 _n_timeToSamples( s32 micros)
{
  return _n_timeToSamplesNoRound( micros) & ~0xf;
}

s32 __n_nextSampleTime(ALPlayer **client) {
    u32 delta = 0x7fffffff;
    ALPlayer *cl;

    *client = 0;

    for (cl = n_syn->head; cl != 0; cl = cl->next) {
        if ((cl->samplesLeft - n_syn->curSamples) < delta) {
            *client = cl;
            delta = cl->samplesLeft - n_syn->curSamples;
        }
    }

    return (*client)->samplesLeft;
}

// s32 func_10019A04(void *arg0) {
//     u32 sp4;
//     void *sp0;
//     void *temp_t2;
//     void *temp_t8;
//
//     sp4 = 0x7FFFFFFF;
//     *arg0 = NULL;
//     temp_t8 = *D_8002BA44;
//     sp0 = temp_t8;
//     if (temp_t8 != 0) {
// loop_1:
//         if ((u32) (sp0->unk10 - D_8002BA44->unk20) < sp4) {
//             *arg0 = sp0;
//             sp4 = sp0->unk10 - D_8002BA44->unk20;
//         }
//         temp_t2 = sp0->unk0;
//         sp0 = temp_t2;
//         if (temp_t2 != 0) {
//             goto loop_1;
//         }
//     }
//     return (*arg0)->unk10;
// }
