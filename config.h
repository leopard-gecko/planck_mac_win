#ifndef CONFIG_USER_H
#define CONFIG_USER_H

#include "../../config.h"

#define NO_DEBUG
#define NO_PRINT

#define BACKLIGHT_BREATHING
#define TAPPING_TERM 135

#define SWAP_LCTR_LGUI

#ifdef AUDIO_ENABLE

    #define LAYER_LOCK_ON_SOUND \
    E__NOTE(_C4),         \
    E__NOTE(_G4),
    #define LAYER_LOCK_OFF_SOUND \
    E__NOTE(_G4),         \
    E__NOTE(_C4),
    #define PUSH_SOUND \
    E__NOTE(_G4),
    #define ADJUST_ON_SOUND \
    E__NOTE(_C4 ),
    #define ADJUST_OFF_SOUND \
    E__NOTE(_A4 ),

    #define STARTUP_SONG SONG(PLANCK_SOUND)
    #define DEFAULT_LAYER_SONGS { SONG(QWERTY_SOUND), \
                                  SONG(ONE_UP_SOUND) \
                                }
#endif
#endif
