#ifndef SCENE_SCRIPT_H
#define SCENE_SCRIPT_H

#include "psx.h"

typedef struct
{
    uint32 frames;
    const uint8 *commands;
    uint32 bytes;
} SCENE_RESTORE_RECORD;

void scene_script_reset_frames(void);
void scene_script_reset_restore(void);
void scene_script_bind_restore(const void *buffer, uint32 bytes);
const SCENE_RESTORE_RECORD *scene_script_get_restore(uint32 index);
const uint8 *scene_script_execute(const uint8 *script, uint32 bytes);

void scene_script_playback_ready(void);
void scene_script_playback_begin(void);
uint32 scene_script_playback_end(void);
void scene_script_playback_update(void);
uint32 scene_script_playback_draw(void);

void scene_script_tick_mission72(uint32 event);

void scene_script_select_playback(uint8 index, uint8 offset);
void scene_script_select_movie(uint8 index, uint8 offset);

#endif
