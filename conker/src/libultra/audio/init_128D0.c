// #include <ultra64.h>

#include <libaudio.h>
#include <os_internal.h>
#include <ultraerror.h>

#include "functions.h"
#include "variables.h"

#define OSCTYPE_TREM             1
#define OSCTYPE_VIB              128
#define OSC_RATE_BASE            0x103
#define OSC_DELAY_SHIFT          14
#define OSC_TREM_BASE_VOLUME     0x7F
#define OSC_TWO_PI               6.2831854820251465f
#define OSC_STATE_SIZE           0x2C

typedef struct124 ConkerOscState;
typedef struct121 ConkerBank;
typedef struct11 ConkerWaveBaseList;

#define BNKF_SAMPLE_BANK_BASE(sampleBankId) (((sampleBankId) & -8) << 5)

#define oscNext                  unk0
#define oscType                  unk4
#define oscPeriod                unk22
#define oscPhase                 unk24
#define tremoloDepth             unk0
#define tremoloBaseVolume        unk1
#define vibratoDepthCents        unk0

#define bankInstrumentCount      unk0
#define bankPatched              unk2
#define bankPercussion           unk8
#define bankInstruments          unkC

#define waveListPatched          unk3
#define waveBaseCount            unkE
#define waveBases                unk10

void n_alBnkfPatchBank(ConkerBank *bank, s32 bankFileOffset, s32 sampleTableOffset, s32 sampleBankBase);
void n_alBnkfPatchSingleWaveBaseList(ConkerWaveBaseList *waveList, s32 sampleBankBase);
void n_alBnkfPatchWaveBaseList(ConkerWaveBaseList *waveList, s32 sampleBankBase);

void alSeqFileNew(ALSeqFile *arg0, u8 *base) {
    s32 offset = base;
    s32 i;

    for (i = 0; i < arg0->seqCount; i++)
    {
        arg0->seqArray[i].offset += offset;
    }
}

// Conker's bank files can point at sample banks outside the loaded bank file.
void n_alBnkfNew(ALBankFile *file, u8 *sampleTable, s32 sampleBankId) {
    s32 bankFileOffset = file;
    s32 sampleTableOffset = sampleTable;
    s32 sampleBankBase = sampleBankId;

    s32 i;

    ALFailIf(file->revision != AL_BANK_VERSION, ERR_ALBNKFNEW);

    for (i = 0; i < file->bankCount; i++) {
        file->bankArray[i] = (ALBank *)((u8 *)file->bankArray[i] + bankFileOffset);
        if (file->bankArray[i] != 0) {
            n_alBnkfPatchBank((ConkerBank *) file->bankArray[i], bankFileOffset, sampleTableOffset, sampleBankBase);
        }
    }
}

void n_alBnkfPatchBank(ConkerBank *bank, s32 bankFileOffset, s32 sampleTableOffset, s32 sampleBankBase) {
    s32 i;
    s32 baseOffset;

    if (bank->bankPatched != 0) {
        return;
    }

    bank->bankPatched = 1;

    if (bank->bankPercussion != 0) {
        bank->bankPercussion += bankFileOffset;
        n_alBnkfPatchSingleWaveBaseList((ConkerWaveBaseList *) bank->bankPercussion, sampleBankBase);
    }

    for (i = 0; i < bank->bankInstrumentCount; i ++) {
        if (bank->bankInstruments[i] != 0) {
            if (i == 0) {
                bank->bankInstruments[i] += bankFileOffset;
                n_alBnkfPatchWaveBaseList((ConkerWaveBaseList *) bank->bankInstruments[i], sampleBankBase);
            } else {
                baseOffset = BNKF_SAMPLE_BANK_BASE(sampleBankBase);
                bank->bankInstruments[i] += baseOffset;
            }
        }
    }
}

void n_alBnkfPatchSingleWaveBaseList(ConkerWaveBaseList *waveList, s32 sampleBankBase) {
    if (waveList->waveListPatched) {
        return;
    }
    waveList->waveListPatched = 1;
    sampleBankBase = BNKF_SAMPLE_BANK_BASE(sampleBankBase);
    waveList->waveBases[0] += sampleBankBase;
}

void n_alBnkfPatchWaveBaseList(ConkerWaveBaseList *waveList, s32 sampleBankBase) {
    s32 i;

    if (waveList->waveListPatched != 0) {
        return;
    }
    waveList->waveListPatched = 1;

    sampleBankBase = BNKF_SAMPLE_BANK_BASE(sampleBankBase);

    for (i = 0; i < waveList->waveBaseCount; i++)
    {
        waveList->waveBases[i] += sampleBankBase;
    }
}

void func_10012C5C(struct9 *arg0, s32 arg1, s32 arg2) {
    if (arg0->unkE) {
        return;
    }
    arg0->unkE = 1;
    arg0->unk0 += arg1;
    arg0->unk4 += arg1;
    arg0->unk8 += arg1;
    func_10012CFC(arg0->unk8, arg1, arg2);
}

void func_10012CFC(struct12 *arg0, s32 arg1, s32 arg2) {
    arg0->unk9 = 1;
    arg0->unk0 += arg2;
    if (arg0->unk8 == 0) {
        arg0->unk10 += arg1;
        if (arg0->unkC != 0) {
            arg0->unkC += arg1;
        }
        arg0->unk14 = 0;
    } else if ((arg0->unk8 == 1) && (arg0->unkC != 0)) {
        arg0->unkC = (s32) (arg0->unkC + arg1);
    }
}

f32 n_alOscDepthToCents(u8 depth) {
    f32 sp4 = 1.0309929847717285f;
    f32 sp0 = 1.0f;

    while (depth)
    {
        if (depth & 1) {
            sp0 *= sp4;
        }
        sp4 *= sp4;
        depth = (u32)depth >> 1;
    }
    return sp0;
}

s32 n_alOscInit(s32 *oscState, f32 *initValue, u8 oscType, u8 oscRate, u8 oscDepth, u8 oscDelay) {
    ConkerOscState *state;
    s32 ret;

    ret = 0;
    if (oscDelay == 0) {
        return 0;
    }
    if (D_80042800 != 0) {
        state = D_80042800;
        D_80042800 = D_80042800->oscNext;
        state->oscType = oscType;
        *oscState = state;
        ret = oscDelay << OSC_DELAY_SHIFT;
        switch (oscType) {
            case OSCTYPE_TREM:
                state->oscPhase = 0;
                state->oscPeriod = OSC_RATE_BASE - oscRate;
                state->data.i.tremoloDepth = oscDepth >> 1;
                state->data.i.tremoloBaseVolume = OSC_TREM_BASE_VOLUME - state->data.i.tremoloDepth;
                *initValue = state->data.i.tremoloBaseVolume;
                break;
            case OSCTYPE_VIB:
                state->data.f.vibratoDepthCents = n_alOscDepthToCents(oscDepth);
                state->oscPhase = 0;
                state->oscPeriod = OSC_RATE_BASE - oscRate;
                *initValue = 1.0f;
                break;
            default:
                break;
        }
    }
    return ret;
}

s32 n_alOscUpdate(ConkerOscState *oscState, f32 *updateValue) {
    f32 sp2C;
    ConkerOscState *state;
    s32 ret;

    state = oscState;
    ret = AL_USEC_PER_FRAME;

    switch (state->oscType)
    {
        case OSCTYPE_TREM:
            state->oscPhase = (u16)state->oscPhase + 1;
            if ((u16)state->oscPhase >= (u16)state->oscPeriod) {
                state->oscPhase = 0;
            }
            sp2C = (f32)(u16)state->oscPhase / (f32) (u16)state->oscPeriod;
            sp2C = sinf(sp2C * OSC_TWO_PI);
            sp2C = state->data.i.tremoloDepth * sp2C;
            *updateValue = state->data.i.tremoloBaseVolume + sp2C;
            break;

        case OSCTYPE_VIB:
            state->oscPhase = (u16)state->oscPhase + 1;
            if ((u16)state->oscPhase >= (u16)state->oscPeriod) {
                state->oscPhase = 0;
            }
            sp2C = (f32)(u16) state->oscPhase / (f32)(u16) state->oscPeriod;
            sp2C = sinf(sp2C * OSC_TWO_PI) * state->data.f.vibratoDepthCents;
            *updateValue = alCents2Ratio(sp2C);
            break;
        default:
            break;
    }

    return ret;
}

void n_alOscStop(s32 *oscState) {
    *oscState = (s32*)D_80042800;
    D_80042800 = oscState;
}

void n_alOscSetCallbacks(struct13 *arg0, s32 arg1) {
    s32 *sp24;
    s32 i;

    D_80042804 = alHeapDBAlloc(0, 0, arg0->unkC, arg1, OSC_STATE_SIZE);
    D_80042800 = (s32) D_80042804;
    D_80042800 = (s32) D_80042804; // ???
    sp24 = D_80042804;

    for (i = 0; i < (arg1 - 1); i++) {
        *sp24 = (i * OSC_STATE_SIZE) + D_80042804 + OSC_STATE_SIZE; // is this an array?
        sp24 = *sp24;
    }
    *sp24 = 0;
    arg0->unk10 = n_alOscInit;
    arg0->unk14 = n_alOscUpdate;
    arg0->unk18 = n_alOscStop;
}
