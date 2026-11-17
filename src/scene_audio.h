#ifndef SCENE_AUDIO_H
#define SCENE_AUDIO_H

#include "psx.h"
#include "audio_player.h"
#include "mission_spawn.h"

enum { SCENE_AUDIO_SLOT_COUNT = 4, SCENE_AUDIO_PRESET_COUNT = 5, SCENE_AUDIO_KIND_COUNT = 48 };

typedef struct
{
    uint32 flags;
    uint32 preset_idx;
    uint32 object_idx;
    uint32 position[3];
} SCENE_AUDIO_GROUP;

typedef struct
{
    uint32 count;
    SCENE_AUDIO_GROUP group[SCENE_AUDIO_SLOT_COUNT];
    uint8 voices[3][SCENE_AUDIO_SLOT_COUNT];
    uint32 listener_pos[4];
    AUDIO_SPATIAL spatial[SCENE_AUDIO_SLOT_COUNT];
    AUDIO_AUX_CONTROL aux[SCENE_AUDIO_SLOT_COUNT];
    AUDIO_MODULATION modulation[SCENE_AUDIO_SLOT_COUNT][2];
    SpuVolume previous_volume[SCENE_AUDIO_SLOT_COUNT][3];
} SCENE_AUDIO_ROUTING;

typedef struct
{
    uint8 sounds[3];
    uint8 aux_sounds[2];
    uint8 speed_threshold;
    sint16 distance_threshold;
    uint8 volume_period[2];
    uint8 note_period[2];
    uint8 volume_depth[2];
    uint8 note_depth[2];
} SCENE_AUDIO_PRESET;

extern const SCENE_AUDIO_PRESET scene_audio_presets[SCENE_AUDIO_PRESET_COUNT];
extern const uint8 scene_audio_preset_for_kind[SCENE_AUDIO_KIND_COUNT];

void scene_audio_select_preset(uint32 slot, uint32 object_index);

extern SCENE_AUDIO_ROUTING scene_audio_routing;

void scene_audio_set_distance(uint32 index, uint32 value);
void scene_audio_map_groups(const uint32 *native_selected, uint32 *native_mapping);
void scene_audio_update_groups(const uint32 *native_position, const uint16 *native_listener_angles);
void scene_audio_update_actors(void);

void scene_audio_init_groups(sint32 count);
void scene_audio_stop_groups(void);
void scene_audio_init_listener(void);
void scene_audio_slew_volume(uint32 voice, const uint16 *volume, const SpuVolume *previous, uint32 step);

void scene_audio_apply_frame(const uint8 *data, uint32 bytes);

void scene_audio_quit_spu(void);

void scene_audio_stop_all(void);

void scene_audio_start_transition(void);

#endif
