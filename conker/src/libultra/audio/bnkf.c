// #include <ultra64.h>

#include <libaudio.h>
#include <os_internal.h>
#include <ultraerror.h>

#include "functions.h"
#include "variables.h"

typedef struct121 ConkerBank;
typedef struct11 ConkerWaveBaseList;

#define BNKF_SAMPLE_BANK_BASE(sampleBankId) (((sampleBankId) & -8) << 5)

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
void _bnkfPatchWaveTable(struct12 *w, s32 offset, s32 table);

void alSeqFileNew(ALSeqFile *file, u8 *base) {
    s32 offset = (s32)base;
    s32 i;

    for (i = 0; i < file->seqCount; i++)
    {
        file->seqArray[i].offset += offset;
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

/* Vanilla ALSound layout; Conker opaque typedef is struct9. */
void _bnkfPatchSound(struct9 *s, s32 offset, s32 table) {
    if (s->unkE) { /* flags */
        return;
    }
    s->unkE = 1;
    s->unk0 += offset; /* envelope */
    s->unk4 += offset; /* keyMap */
    s->unk8 += offset; /* wavetable */
    _bnkfPatchWaveTable((struct12 *)s->unk8, offset, table);
}

/*
 * Conker wave-table patcher. Layout differs from vanilla ALWaveTable:
 * ADPCM book is at +0x10 and loop at +0xC (swapped vs libreultra).
 */
void _bnkfPatchWaveTable(struct12 *w, s32 offset, s32 table) {
    w->unk9 = 1; /* flags */
    w->unk0 += table; /* base */
    if (w->unk8 == 0) { /* AL_ADPCM_WAVE */
        w->unk10 += offset; /* book */
        if (w->unkC != 0) {
            w->unkC += offset; /* loop */
        }
        w->unk14 = 0;
    } else if ((w->unk8 == 1) && (w->unkC != 0)) { /* AL_RAW16_WAVE */
        w->unkC = (s32) (w->unkC + offset);
    }
}
