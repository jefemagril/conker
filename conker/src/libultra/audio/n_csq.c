#include <n_libaudio.h>

#define CONKER_AL_TRACK_END 0x13
#define CONKER_AL_CSP_LOOPSTART 0x14
#define CONKER_AL_CSP_LOOPEND 0x15

u8 __getTrackByte(ALCSeq *seq, s32 track);
u32 __readVarLen(ALCSeq *seq, s32 track);
u32 __n_alCSeqGetTrackEvent(ALCSeq *seq, u32 track, N_ALEvent *event, s32 allowLoop);

void n_alCSeqNew(ALCSeq *seq, u8 *ptr) {
    u32 i;
    u32 tmpOff;
    u32 flagTmp;

    seq->base = (ALCMidiHdr *)ptr;
    seq->validTracks = 0;
    seq->lastDeltaTicks = 0;
    seq->lastTicks = 0;
    seq->deltaFlag = 1;

    for (i = 0; i < 16; i++) {
        seq->lastStatus[i] = 0;
        seq->curBUPtr[i] = 0;
        seq->curBULen[i] = 0;
        tmpOff = seq->base->trackOffset[i];

        if (tmpOff != 0) {
            flagTmp = 1 << i;
            seq->validTracks |= flagTmp;
            seq->curLoc[i] = ptr + tmpOff;
            seq->evtDeltaTicks[i] = __readVarLen(seq, i);
        } else {
            seq->curLoc[i] = 0;
        }
    }

    seq->qnpt = 1.0f / (f32)seq->base->division;
}
void n_alCSeqNextEvent(ALCSeq *seq, N_ALEvent *evt, s32 allowLoop) {
    u32 i;
    u32 firstTime = -1;
    u32 firstTrack = -1;
    u32 lastTicks = seq->lastDeltaTicks;

    for (i = 0; i < 16; i++) {
        if ((seq->validTracks >> i) & 1) {
            if (seq->deltaFlag != 0) {
                seq->evtDeltaTicks[i] -= lastTicks;
            }

            if (seq->evtDeltaTicks[i] < firstTime) {
                firstTime = seq->evtDeltaTicks[i];
                firstTrack = i;
            }
        }
    }

    if (firstTrack != -1) {
        __n_alCSeqGetTrackEvent(seq, firstTrack, evt, allowLoop);
    } else {
        evt->type = CONKER_AL_TRACK_END;
    }

    evt->msg.midi.ticks = firstTime;
    seq->lastTicks += firstTime;
    seq->lastDeltaTicks = firstTime;

    if (evt->type != CONKER_AL_TRACK_END) {
        seq->evtDeltaTicks[firstTrack] += __readVarLen(seq, firstTrack);
    }

    seq->deltaFlag = 1;
}
u32 __n_alCSeqGetTrackEvent(ALCSeq *seq, u32 track, N_ALEvent *event, s32 allowLoop) {
    u32 offset;
    u8 status;
    u8 loopCt;
    u8 curLpCt;
    u8 *tmpPtr;

    status = __getTrackByte(seq, track);

    if (status == AL_MIDI_Meta) {
        u8 type = __getTrackByte(seq, track);

        if (type == AL_MIDI_META_TEMPO) {
            event->type = AL_TEMPO_EVT;
            event->msg.tempo.status = status;
            event->msg.tempo.type = type;
            event->msg.tempo.byte1 = __getTrackByte(seq, track);
            event->msg.tempo.byte2 = __getTrackByte(seq, track);
            event->msg.tempo.byte3 = __getTrackByte(seq, track);
            seq->lastStatus[track] = 0;
        } else if (type == AL_MIDI_META_EOT) {
            u32 flagMask = 1 << track;

            seq->validTracks ^= flagMask;
            if (seq->validTracks != 0) {
                event->type = CONKER_AL_TRACK_END;
            } else {
                event->type = AL_SEQ_END_EVT;
            }
        } else if (type == AL_CMIDI_LOOPSTART_CODE) {
            status = __getTrackByte(seq, track);
            event->msg.loop.count = status << 8;
            status = __getTrackByte(seq, track);
            event->msg.loop.count += status;
            seq->lastStatus[track] = 0;
            event->type = CONKER_AL_CSP_LOOPSTART;
        } else if (type == AL_CMIDI_LOOPEND_CODE) {
            tmpPtr = seq->curLoc[track];
            loopCt = *tmpPtr++;
            curLpCt = *tmpPtr;
            if (curLpCt == 0 || allowLoop == 0) {
                *tmpPtr = loopCt;
                seq->curLoc[track] = tmpPtr + 5;
            } else {
                if (curLpCt != 0xFF) {
                    *tmpPtr = curLpCt - 1;
                }
                tmpPtr++;
                offset = *tmpPtr++ << 24;
                offset += *tmpPtr++ << 16;
                offset += *tmpPtr++ << 8;
                offset += *tmpPtr++;
                seq->curLoc[track] = tmpPtr - offset;
            }
            seq->lastStatus[track] = 0;
            event->type = CONKER_AL_CSP_LOOPEND;
        }
    } else {
        event->type = AL_SEQ_MIDI_EVT;
        if (status & 0x80) {
            event->msg.midi.status = (status & 0xF0) | track;
            event->msg.midi.byte1 = __getTrackByte(seq, track);
            seq->lastStatus[track] = event->msg.midi.status;
        } else {
            event->msg.midi.status = seq->lastStatus[track];
            event->msg.midi.byte1 = status;
        }

        if (((event->msg.midi.status & 0xF0) != AL_MIDI_ProgramChange) &&
            ((event->msg.midi.status & 0xF0) != AL_MIDI_ChannelPressure)) {
            event->msg.midi.byte2 = __getTrackByte(seq, track);
            if ((event->msg.midi.status & 0xF0) == AL_MIDI_NoteOn) {
                event->msg.midi.duration = __readVarLen(seq, track);
            }
        } else {
            event->msg.midi.byte2 = 0;
        }
    }

    return 1;
}
void alCSeqSetLoc(ALCSeq *seq, ALCSeqMarker *m) {
    s32 i;

    seq->validTracks = m->validTracks;
    seq->lastTicks = m->lastTicks;
    seq->lastDeltaTicks = m->lastDeltaTicks;

    for (i = 0; i < 16; i++) {
        seq->curLoc[i] = m->curLoc[i];
        seq->curBUPtr[i] = m->curBUPtr[i];
        seq->curBULen[i] = m->curBULen[i];
        seq->lastStatus[i] = m->lastStatus[i];
        seq->evtDeltaTicks[i] = m->evtDeltaTicks[i];
    }
}

void n_alCSeqNewLoopStartMarkers(ALCSeq *seq, ALCSeqMarker *m, u32 markerCount, u32 firstLoopIndex) {
    N_ALEvent evt;
    ALCSeq tempSeq;
    s32 i;
    s32 j;
    ALCSeqMarker m2;

    n_alCSeqNew(&tempSeq, (u8 *)seq->base);

    for (j = 0; j < markerCount; j++) {
        m[j].lastTicks = 0;
    }

    do {
        m2.validTracks = tempSeq.validTracks;
        m2.lastTicks = tempSeq.lastTicks;
        m2.lastDeltaTicks = tempSeq.lastDeltaTicks;

        for (i = 0; i < 16; i++) {
            m2.curLoc[i] = tempSeq.curLoc[i];
            m2.curBUPtr[i] = tempSeq.curBUPtr[i];
            m2.curBULen[i] = tempSeq.curBULen[i];
            m2.lastStatus[i] = tempSeq.lastStatus[i];
            m2.evtDeltaTicks[i] = tempSeq.evtDeltaTicks[i];
        }

        n_alCSeqNextEvent(&tempSeq, &evt, 0);

        if (evt.type == CONKER_AL_CSP_LOOPSTART) {
            if (((evt.msg.loop.count >> 8) >= firstLoopIndex) && ((evt.msg.loop.count >> 8) < firstLoopIndex + markerCount)) {
                if (m[(evt.msg.loop.count >> 8) - firstLoopIndex].lastTicks == 0) {
                    m[(evt.msg.loop.count >> 8) - firstLoopIndex] = m2;

                    if (--j <= 0) {
                        return;
                    }
                }
            }
        }
    } while (evt.type != AL_SEQ_END_EVT);
}

u8 __getTrackByte(ALCSeq *seq, s32 track) {
    u8 theByte;

    if (seq->curBULen[track]) {
        theByte = *seq->curBUPtr[track];
        seq->curBUPtr[track]++;
        seq->curBULen[track]--;
    } else {
        theByte = *seq->curLoc[track];
        seq->curLoc[track]++;
        if (theByte == AL_CMIDI_BLOCK_CODE) {
            u8 loBackUp, hiBackUp, theLen, nextByte;
            u32 backup;

            nextByte = *seq->curLoc[track];
            seq->curLoc[track]++;
            if (nextByte != AL_CMIDI_BLOCK_CODE) {
                hiBackUp = nextByte;
                loBackUp = *seq->curLoc[track];
                seq->curLoc[track]++;
                theLen = *seq->curLoc[track];
                seq->curLoc[track]++;
                backup = (u32) hiBackUp;
                backup <<= 8;
                backup += loBackUp;
                seq->curBUPtr[track] = seq->curLoc[track] - (backup + 4);
                seq->curBULen[track] = (u32)theLen;
                theByte = *seq->curBUPtr[track];
                seq->curBUPtr[track]++;
                seq->curBULen[track]--;
            }
        }
    }
    return theByte;
}

u32 __readVarLen(ALCSeq *seq, s32 track) {
    u32 value;
    u32 c;

    value = __getTrackByte(seq, track);
    if (value & 0x80) {
        value &= 0x7F;
        do
        {
            c = __getTrackByte(seq, track);
            value = (value << 7) + (c & 0x7F);
        } while (c & 0x80);
    }
    return value;
}
