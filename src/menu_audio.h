#ifndef MENU_AUDIO_H
#define MENU_AUDIO_H

#include "psx.h"

enum { MENU_VOICE_COUNT = 5 };

void menu_voices_reset(void);
uint32 *menu_voice_slot(uint32 index);
uint32 menu_voices_restart(uint32 adjustment);

void menu_audio_reset(void);
void menu_audio_begin(void);
uint32 menu_audio_frame(void);

const uint8 *title_audio_capture_channel(uint32 channel);
sint32 title_audio_mean_magnitude(void);

const uint8 *mission_audio_capture_channel(uint32 channel);
uint32 mission_audio_mean_magnitude(void);

uint32 mission_sound_test_begin(void);
uint32 mission_sound_test_tick(void);

#endif
