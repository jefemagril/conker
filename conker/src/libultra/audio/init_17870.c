#include <ultra64.h>

#include "functions.h"
#include "variables.h"

#define AUDIO_OUTPUT_MODE_MONO        1
#define AUDIO_OUTPUT_MODE_STEREO      2
#define AUDIO_OUTPUT_MODE_HEADPHONES  3
#define AUDIO_OUTPUT_MODE_SURROUND    4

#define AUDIO_OUTPUT_ROUTE_STEREO     2
#define AUDIO_OUTPUT_ROUTE_SURROUND   3
#define AUDIO_OUTPUT_ROUTE_CENTER     4
#define AUDIO_OUTPUT_ROUTE_MONO       5

#define AUDIO_OUTPUT_SLOT_COUNT       2

#define gAudioOutputSurroundEnabled   D_800428C0
#define gAudioOutputMonoEnabled       D_800428C1
#define gAudioOutputNarrowPanEnabled  D_800428C2
#define gAudioOutputLeftMixFlags      D_800428C4
#define gAudioOutputRightMixFlags     D_800428C6
#define gAudioOutputRoutes            D_800428C8

void n_alSndpSetOutputRoute(s32 slot, u32 route);

void n_alSndpSetOutputMode(u8 outputMode) {
    s32 slot;

    gAudioOutputSurroundEnabled = 0;
    gAudioOutputMonoEnabled = 0;
    gAudioOutputNarrowPanEnabled = 0;

    switch (outputMode) {

        case AUDIO_OUTPUT_MODE_MONO:
            gAudioOutputMonoEnabled = 1;
            break;
        case AUDIO_OUTPUT_MODE_HEADPHONES:
            gAudioOutputNarrowPanEnabled = 1;
            break;
        case AUDIO_OUTPUT_MODE_SURROUND:
            gAudioOutputSurroundEnabled = 1;
            break;
    }

    for(slot = 0; slot < AUDIO_OUTPUT_SLOT_COUNT; slot++) {
        n_alSndpSetOutputRoute(slot, 0);
    }
}

void n_alSndpSetOutputRoute(s32 slot, u32 route) {
    if (route == 0) {
        route = gAudioOutputRoutes[slot];
    }

    gAudioOutputLeftMixFlags[slot] = 0;
    gAudioOutputRightMixFlags[slot] = 0;

    switch (route)
    {
        case AUDIO_OUTPUT_ROUTE_STEREO:
            if (gAudioOutputSurroundEnabled != 0) {
                gAudioOutputRightMixFlags[slot] = 1;
            }
            break;
        case AUDIO_OUTPUT_ROUTE_SURROUND:
            if (gAudioOutputSurroundEnabled != 0) {
                gAudioOutputLeftMixFlags[slot] = 1;
            }
            break;
        case AUDIO_OUTPUT_ROUTE_CENTER:
            if (gAudioOutputMonoEnabled == 0) {
                gAudioOutputLeftMixFlags[slot] = 1;
            }
            break;
        case AUDIO_OUTPUT_ROUTE_MONO:
            if (gAudioOutputMonoEnabled == 0) {
                gAudioOutputLeftMixFlags[slot] = 1;
                gAudioOutputRightMixFlags[slot] = 1;
            }
            break;
    }

    gAudioOutputRoutes[slot] = route;
}
