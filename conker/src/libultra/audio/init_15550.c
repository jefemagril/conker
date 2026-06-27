#include <n_libaudio.h>


typedef struct {
    s32 pad0;
    u8  unk4;
    s8  unk5; // used
} struct152;

typedef struct {
    s32 pad0;
    struct152 *unk4;
} struct153;

typedef struct {
    u8  pad0[0xC];
    struct153 *unkC;
    u8  pad10[0x24];
    f32 unk34;  // used
} struct154;

extern N_ALUnknownStruct1 *D_8002BA20;
extern N_ALUnknownStruct1 *D_8002BA24;
extern N_ALUnknownStruct1 *D_8002BA28;
extern N_ALSndPlayer *D_8002BA2C;
extern s16 D_8002BA30;
extern u16 *D_800428B8;

void func_10017298(N_ALUnknownStruct1 *arg0);

#define SNDP_VOICE_UPDATE_EVT        0x400
#define SNDP_VOICE_CHANNEL_EVT       0x800
#define SNDP_ACTIVE_FLAG             0x01
#define SNDP_STATE_READY_MASK        0x03
#define SNDP_CLEAR_RESTART_FLAG_MASK -0x11
#define SNDP_CHANNEL_MASK            0x1F

void func_10015550(N_ALCSPlayer *csp, s32 arg1) {
    N_ALEvent event;

    event.type = 14;
    event.msg.midi.ticks = arg1;

    n_alEvtqPostEvent(&csp->evtq, &event, 0, 2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/libultra/audio/init_15550/func_100155A0.s")

s32 func_10015878(N_ALSndPlayer *sp) {
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

void func_10016E90(N_ALUnknownStruct1 *arg0) {
    if ((arg0->unk53 & 4) != 0) {
        n_alSynStopVoice(&arg0->unk10);
        n_alSynFreeVoice(&arg0->unk10);
    }
    func_10017298(arg0);
    func_10016F80(&D_8002BA2C->evtq, arg0, 0xFFFF);
}

void func_10016F00(struct154 *arg0) {
    N_ALEvent event;
    f32 res;

    res = alCents2Ratio(arg0->unkC->unk4->unk5) * arg0->unk34;

    event.type = 16;
    /* TODO: check if this is the right struct */
    event.msg.vol.voice = arg0;
    event.msg.vol.delta = *(s32*)&res;

    n_alEvtqPostEvent(&D_8002BA2C->evtq, &event, 33333, 2);
}

#pragma GLOBAL_ASM("asm/nonmatchings/libultra/audio/init_15550/func_10016F80.s")
// void func_10016F80(void *arg0, s32 arg1, u16 arg2) {
//     void *sp3C;
//     void *sp38;
//     void *sp34;
//     void *sp30;
//     void *sp2C;
//     s32 sp28;
//     void *sp24;
//     void *sp20;
//     void *sp1C;
//
//     sp28 = osSetIntMask(1);
//     sp3C = arg0->unk8;
//     if (sp3C != 0) {
// loop_1:
//         sp38 = *sp3C;
//         sp34 = sp3C;
//         sp30 = sp38;
//         sp2C = sp34 + 0xC;
//         if ((sp2C->unk4 == arg1) && ((sp2C->unk0 & arg2) != 0)) {
//             if (sp30 != 0) {
//                 sp30->unk8 = (s32) (sp30->unk8 + sp34->unk8);
//             }
//             sp24 = sp3C;
//             if (sp24->unk0 != 0) {
//                 sp24->unk0->unk4 = (s32) sp24->unk4;
//             }
//             if (sp24->unk4 != 0) {
//                 *sp24->unk4 = (s32) sp24->unk0;
//             }
//             sp20 = sp3C;
//             sp1C = arg0;
//             sp20->unk0 = (s32) *sp1C;
//             sp20->unk4 = sp1C;
//             if (*sp1C != 0) {
//                 (*sp1C)->unk4 = sp20;
//             }
//             *sp1C = sp20;
//         }
//         sp3C = sp38;
//         if (sp3C != 0) {
//             goto loop_1;
//         }
//     }
//     osSetIntMask(sp28);
// }

N_ALUnknownStruct1 *func_10017100(s32 arg0, s16 arg1) {
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
        sp24->unk4C = arg1;
        sp24->unk3C = arg0;
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

void func_10017298(N_ALUnknownStruct1 *arg0) {
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

s32 func_100173C4(N_ALUnknownEvent2 *arg0) {
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

#pragma GLOBAL_ASM("asm/nonmatchings/libultra/audio/init_15550/func_10017438.s")
// void *func_10017438(s32 arg0, s16 arg1, u16 arg2, u8 arg3, f32 arg4, u8 arg5, u8 arg6, void *arg7) {
//     void *sp34;
//     void *sp30;
//     s16 sp2E;
//     s32 sp28;
//     void *sp1C;
//     s16 sp18;
//
//     sp30 = NULL;
//     sp2E = 0;
//     if (arg1 != 0) {
// loop_1:
//         sp34 = func_10017100(arg0, arg1 - 1);
//         if (sp34 != 0) {
//             (*(void *)0x8002BA2C)->unk3C = sp34;
//             sp18 = 0x4000;
//             sp1C = sp34;
//             sp34->unk4F = arg3;
//             sp34->unk44 = arg2;
//             sp34->unk34 = arg4;
//             sp34->unk50 = arg5;
//             sp34->unk51 = arg6;
//             sp28 = 0;
//             n_alEvtqPostEvent(D_8002BA2C + 20, &sp18, sp28 + 1, 2);
//             sp30 = sp34;
//         }
//         arg1 = (u16)0;
//         if (arg1 != 0) {
//             if (sp34 != 0) {
//                 goto loop_1;
//             }
//         }
//         if ((sp30 == 0) || (sp30->unk53 = (u8) (sp30->unk53 | 1), sp30->unk38 = arg7, (sp2E != 0))) {
//
//         }
//     }
//     if (arg7 != 0) {
//         *arg7 = sp30;
//     }
//     return sp30;
// }

void func_10017594(N_ALUnknownStruct1 *arg0) {
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

void func_10017714(s32 arg0, s16 type, s32 arg2) {
    N_ALEvent event;

    if (arg0 != 0) {
        event.type = type;
        event.msg.vol.voice = arg0;
        event.msg.vol.delta = arg2;
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
        if ((voice->unkC != 0) &&
            ((((N_ALUnknownEvent3 *) ((struct153 *) voice->unkC)->unk4)->unk2 & SNDP_CHANNEL_MASK) == channel)) {
            event.type = SNDP_VOICE_CHANNEL_EVT;
            event.voice = voice;
            n_alEvtqPostEvent(&D_8002BA2C->evtq, (N_ALEvent *) &event, 0, 2);
        }
    }
    osSetIntMask(mask);
}
