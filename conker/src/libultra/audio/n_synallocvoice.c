#include <os_internal.h>
#include <ultraerror.h>
#include "n_synthInternals.h"


s32 _allocatePVoice(N_PVoice **pvoice, s16 priority);

s32 n_alSynAllocVoice(N_ALVoice *voice, ALVoiceConfig *vc)
{
    N_PVoice *pvoice = 0;
    ALParam *update;
    s32 stolen;
    ALLink *element;
    ALLink *linkElement;
    ALLink *after;

    voice->priority   = vc->priority;
    voice->unityPitch = vc->unityPitch;
    voice->table      = 0;
    voice->fxBus      = vc->fxBus;
    voice->state      = AL_STOPPED;
    voice->pvoice     = 0;
    voice->unk14      = (void *)vc->unk8;

    stolen = _allocatePVoice(&pvoice, vc->priority);

    if (pvoice) {    /* if we were able to allocate a voice */
        if (stolen) {
            pvoice->offset = 0x228;
            pvoice->vvoice->pvoice = 0; /* zero stolen voice */
            pvoice->vvoice = voice;
            voice->pvoice = pvoice;

            /*
             * ramp down stolen voice
             */
            update = __n_allocParam();
            if (update) {
                update->delta      = n_syn->paramSamples;
                update->type       = AL_FILTER_SET_VOLUME;
                update->data.i     = 0;
                update->moredata.i = 0x170;

                n_alEnvmixerParam(voice->pvoice, AL_FILTER_ADD_UPDATE, update);
            } else {
                /* empty */
            }

            /*
             * stop stolen voice
             */
            update = __n_allocParam();
            if (update) {
                update->delta = n_syn->paramSamples + pvoice->offset;
                update->type  = AL_FILTER_STOP_VOICE;
                update->next  = 0;
                n_alEnvmixerParam(voice->pvoice, AL_FILTER_ADD_UPDATE, update);
            } else {
                /* empty */
            }
        } else {
            pvoice->offset = 0;
            pvoice->vvoice = voice;
            voice->pvoice  = pvoice;

            /* unlink from any prior list, then link onto auxBus[fxBus] voice list */
            element = &pvoice->voiceLink;
            if (element->next != 0) {
                element->next->prev = element->prev;
            }
            if (element->prev != 0) {
                element->prev->next = element->next;
            }

            linkElement = &pvoice->voiceLink;
            /* sourceCount/maxSources are the ALLink head at auxBus+0x14 */
            after = (ALLink *)&n_syn->auxBus[vc->fxBus].sourceCount;
            linkElement->next = after->next;
            linkElement->prev = after;
            if (after->next != 0) {
                after->next->prev = linkElement;
            }
            after->next = linkElement;

            pvoice->dc_table = 0;
        }
    }

    return (pvoice != 0);
}

#pragma GLOBAL_ASM("asm/nonmatchings/libultra/audio/n_synallocvoice/_allocatePVoice.s")
// NON-MATCHING: sibling if/else if/else (PD/Banjo/libreultra) with Conker's inlined
// alUnlink/alLink and duplicate locals per expansion is the intended shape. At -g,
// IDO delay-slot-fills the arm-exit branch (`b; sw`) instead of the original
// `sw; b; nop`. Do not "fix" that with goto / early returns — keep hunting a
// sibling-faithful block shape (see matched __n_CSPRepostEvent `break` after link).
//
// s32 _allocatePVoice(N_PVoice **pvoice, s16 priority)
// {
//     ALLink *dl;
//     N_PVoice *pv;
//     s32 stolen;
//     ALLink *element;
//     ALLink *linkElement;
//     ALLink *after;
//     ALLink *element2;
//     ALLink *linkElement2;
//     ALLink *after2;
//
//     stolen = 0;
//
//     if ((dl = n_syn->pLameList.next) != 0) {
//         *pvoice = (N_PVoice *)dl;
//         element = dl;
//         if (element->next != 0) {
//             element->next->prev = element->prev;
//         }
//         if (element->prev != 0) {
//             element->prev->next = element->next;
//         }
//         linkElement = dl;
//         after = &n_syn->pAllocList;
//         linkElement->next = after->next;
//         linkElement->prev = after;
//         if (after->next != 0) {
//             after->next->prev = linkElement;
//         }
//         after->next = linkElement;
//     } else if ((dl = n_syn->pFreeList.next) != 0) {
//         *pvoice = (N_PVoice *)dl;
//         element2 = dl;
//         if (element2->next != 0) {
//             element2->next->prev = element2->prev;
//         }
//         if (element2->prev != 0) {
//             element2->prev->next = element2->next;
//         }
//         linkElement2 = dl;
//         after2 = &n_syn->pAllocList;
//         linkElement2->next = after2->next;
//         linkElement2->prev = after2;
//         if (after2->next != 0) {
//             after2->next->prev = linkElement2;
//         }
//         after2->next = linkElement2;
//     } else {
//         for (dl = n_syn->pAllocList.next; dl != 0; dl = dl->next) {
//             pv = (N_PVoice *)dl;
//             if ((pv->vvoice->priority <= priority) && (pv->offset == 0)) {
//                 *pvoice = pv;
//                 priority = pv->vvoice->priority;
//                 stolen = 1;
//             }
//         }
//     }
//
//     return stolen;
// }
