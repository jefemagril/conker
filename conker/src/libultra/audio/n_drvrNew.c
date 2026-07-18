#include "n_synthInternals.h"

#if 0
f32 PI = 3.1415927410125732;
f32 D_8002C774 = 1.4142136573791504;
f32 D_8002C778 = 1.0594631433486938;
f32 D_8002C77C = 0.9438742995262146;
f32 D_8002C780 = 26768.0;
f32 D_8002C784 = 26768.0;
f32 D_8002C788 = 173123.40625;
f32 D_8002C78C = 0.0;
#endif

extern f32 D_8002C770;
extern f32 D_8002C774;
extern f32 D_8002C778;
extern f32 D_8002C77C;
extern f32 D_8002C780;
extern f32 D_8002C784;
extern f32 D_8002C788;
extern f32 D_8002C78C;

f32 func_150484A0(f32, f32);

/* RBJ-style low-pass biquad coefficients (PD: func0003b710). */
void n_alFxCalcBiquadCoefs(f32 outputRate, f32 cutoff, f32 q, f32 b[3], f32 a[3]) {
    f32 w;
    f32 w2;
    f32 alpha;

    if (cutoff >= (outputRate - 200.0f)) {
        cutoff = outputRate - 200.0f;
    }

    w = func_150484A0(cutoff * D_8002C770, outputRate);
    w2 = w * w;
    alpha = (w * D_8002C774) / q;
    b[0] = w2 / (1.0f + w2 + alpha);
    b[1] = b[0] * 2.0f;
    b[2] = b[0];
    a[1] = ((w2 - 1.0f) * 2.0f) / (1.0f + w2 + alpha);
    a[2] = ((1.0f + w2) - alpha) / (1.0f + w2 + alpha);
}

void init_lpfilter(ALLowPass *lp) {
    s32 i;
    s32 temp;
    s16 fc;
    f32 ffc;
    f32 fcoef;
    f32 scale;

    temp = lp->fc * 16384.0f;
    fc = temp >> 15;
    lp->fgain = 16384.0f - fc;
    lp->first = 0;

    for (i = 0; i < 8; i++)
    {
        lp->fcvec.fccoef[i] = 0;
    }
    lp->fcvec.fccoef[i++] = fc;

    scale = 16384.0f;
    fcoef = ffc = fc / scale;

    for (;i < 16; i++)
    {
        fcoef *= ffc;
        lp->fcvec.fccoef[i] = fcoef * scale;
    }
}

/* Pitch ratio from semitone offset: 2^(semitones/12). Not alCents2Ratio. */
f32 alSemitones2Ratio(s32 semitones) {
    f32 mult;
    f32 ratio = 1.0f;

    if (semitones >= 0) {
        mult = D_8002C778;
    } else {
        mult = D_8002C77C;
        semitones = -semitones;
    }

    while (semitones != 0) {
        if ((semitones & 1) != 0) {
            ratio = ratio * mult;
        }
        mult *= mult;
        semitones = semitones >> 1;
    };

    return ratio;
}

void n_alFxInitlpfilter_mono(ALLowPass *lp, f32 outputRate) {
    s32 i;
    f32 b[3];
    f32 a[3];

    if (lp->fgain == 0) {
        return;
    }
    if (lp->fgain < 10) {
        lp->fgain = 10;
    }
    n_alFxCalcBiquadCoefs(outputRate, lp->fc + 10.0f, lp->fgain / 10.0f, b, a);

    for(i = 3; i < 8; i++) {
        lp->fcvec.fccoef[i] = 0;
    }
    lp->fcvec.fccoef[0] = b[0] * (D_8002C780 - (lp->fgain * 128.0f));
    lp->fcvec.fccoef[1] = b[1] * (D_8002C784 - (lp->fgain * 128.0f));
    lp->fcvec.fccoef[2] = 0;

    lp->fcvec.fccoef[8] = a[1] * -16384.0f;
    lp->fcvec.fccoef[9] = a[2] * -16384.0f;

    for (i = 10; i < 16; i++) {
        lp->fcvec.fccoef[i] = 0;
    }
}

extern s32 D_8002BBE0[];

void n_alFxNew(ALFx **fx_ar, ALSynConfig *c, s16 bus, ALHeap *hp)
{
    u16 i;
    u16 j;
    u16 k;
    s32 *param;
    ALDelay *d;
    ALFx *r;

    param = 0;
    *fx_ar = r = (ALFx *)alHeapDBAlloc(0, 0, hp, 1, sizeof(ALFx));

    switch (c->fxTypes[bus]) {
    case AL_FX_CUSTOM:
        param = c->params[bus];
        break;
    default:
        param = D_8002BBE0;
        break;
    }

    j = 0;

    r->section_count = param[j++];
    r->length = param[j++];

    r->delay = alHeapDBAlloc(0, 0, hp, r->section_count, sizeof(ALDelay));
    r->base[0] = alHeapDBAlloc(0, 0, hp, r->length, sizeof(s16));
    r->input[0] = r->base[0];
    r->base[1] = alHeapDBAlloc(0, 0, hp, r->length, sizeof(s16));
    r->input[1] = r->base[1];

    for (k = 0; k < r->length; k++) {
        r->base[1][k] = 0;
        r->base[0][k] = r->base[1][k];
    }

    for (i = 0; i < r->section_count; i++) {
        d = &r->delay[i];
        d->input = param[j++];
        d->output = param[j++];
        d->fbcoef = param[j++];
        d->ffcoef = param[j++];
        d->gain = param[j++];

        if (param[j]) {
#define RANGE 2.0f
            d->rsinc = ((((f32)param[j++]) / 1000) * RANGE) / c->outputRate;
            /* D_8002C788 ≈ 120000/ln(2) for hundredths-of-a-cent conversion */
#define LENGTH (d->output - d->input)
            d->rsgain = (((f32)param[j++]) / D_8002C788) * LENGTH;
            d->rsval = 1.0f;
            d->rsdelta = 0;
            d->rs = alHeapDBAlloc(0, 0, hp, 1, sizeof(ALResampler));
            d->rs->state[0] = alHeapDBAlloc(0, 0, hp, 1, sizeof(RESAMPLE_STATE));
            d->rs->state[1] = alHeapDBAlloc(0, 0, hp, 1, sizeof(RESAMPLE_STATE));
            d->rs->delta = 0.0f;
            d->rs->first = 1;
        } else {
            d->rs = 0;
            j++;
            j++;
        }

        if (param[j]) {
            d->lp = alHeapDBAlloc(0, 0, hp, 1, sizeof(ALLowPass));
            d->lp->fstate[0] = alHeapDBAlloc(0, 0, hp, 1, sizeof(POLEF_STATE));
            d->lp->fstate[1] = alHeapDBAlloc(0, 0, hp, 1, sizeof(POLEF_STATE));
            d->lp->fc = param[j++];
            init_lpfilter(d->lp);
        } else {
            d->lp = 0;
            j++;
        }
    }
}

void alN_PVoiceNew(N_PVoice *mv, ALDMANew dmaNew, ALHeap *hp) {
    mv->dc_state = alHeapDBAlloc(0, 0, hp, 1, sizeof(ADPCM_STATE));
    mv->dc_lstate = alHeapDBAlloc(0, 0, hp, 1, sizeof(ADPCM_STATE));
    mv->dc_dma = dmaNew(&mv->dc_dmaState);
    mv->dc_lastsam = 0;
    mv->dc_first = 1;
    mv->dc_memin = 0;

    mv->rs_state = alHeapDBAlloc(0, 0, hp, 1, sizeof(RESAMPLE_STATE));
    mv->rs_delta  = 0.0;
    mv->rs_first  = 1;
    mv->rs_ratio = 1.0;
    mv->rs_upitch = 0;

    mv->em_state = alHeapAlloc(hp, 1, sizeof(ENVMIX_STATE));
    mv->em_first = 1;
    mv->em_motion = AL_STOPPED;
    mv->em_volume = 1;
    mv->em_ltgt = 1;
    mv->em_rtgt = 1;
    mv->em_cvolL = 1;
    mv->em_cvolR = 1;
    mv->em_dryamt = 0;
    mv->em_wetamt = 0;
    mv->em_lratm = 1;
    mv->em_lratl = 0;
    mv->em_lratm = 1;
    mv->em_lratl = 0;
    mv->em_delta = 0;
    mv->em_segEnd = 0;
    mv->em_pan = 0;
    mv->em_ctrlList = 0;
    mv->em_ctrlTail = 0;
    /* Rare extras: per-voice LP / filter11 (PD: unk8c / fx / unkb8 / unkbc) */
    mv->unk99 = 0;
    mv->unkA2 = 0;
    mv->unkA0 = 0;
    mv->unkCC = alHeapDBAlloc(0, 0, hp, 1, 8);
    mv->unkC8 = 0;
}
