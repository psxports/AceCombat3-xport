#ifndef AUDIO_PLAYER_H
#define AUDIO_PLAYER_H

#include "psx.h"
#include "mission_vm.h"
#include "resource_queue.h"

enum { SOUND_ID_COUNT = 136 };

typedef struct
{
    const uint8 *data;
    uint32 bytes;
    uint32 offset;
} SOUND_BANK_INPUT;


typedef struct
{
    uint8 sound;
    uint8 flags;
    uint8 key_delay;
    uint8 active;
    uint16 volume[2];
    uint16 base_volume[2];
    uint16 note;
    uint16 base_note;
    uint8 fade_target;
    uint8 fade_level;
    uint16 fade_ticks;
} SOUND_VOICE;

void sound_reset_storage(void);
SOUND_VOICE *sound_voice_at(uint32 index);

typedef struct
{
    uint8 mode;
    uint8 phrase;
    sint16 command_offset;
} SPEECH_ENTRY;

typedef char SPEECH_ENTRY_LAYOUT_CHECK[
    sizeof(SPEECH_ENTRY) == 4 && offsetof(SPEECH_ENTRY, command_offset) == 2 ? 1 : -1];

typedef struct
{
    const uint8 *data;
    const uint8 *end;
    const uint8 *commands;
    const SPEECH_ENTRY *entries;
    uint32 count;
} SPEECH_CATALOG;

typedef struct
{
    uint32 cur_item;
    uint32 phrase_phase;
    SPEECH_CATALOG catalog;
    uint16 elapsed[4];
    uint16 duration[4];
    uint8 pending[4];
    uint32 mode;
} SPEECH_QUEUE;

extern SPEECH_QUEUE speech_queue;

void speech_queue_phrase_complete(void);
void speech_queue_enqueue_phrase(uint32 mission, uint32 index);
void audio_play_main_track(uint32 index);
uint32 speech_queue_update_phrase(void);

void speech_queue_init(const uint8 *root_data, uint32 root_bytes, uint32 variant_offset);
void speech_queue_push(uint32 id, sint32 value);
void speech_queue_update(void);
void speech_queue_stop(void);
uint32 speech_queue_mode(void);
void speech_queue_play_phrase(uint32 index);

typedef struct
{
    const uint8 *cursor;
    const uint8 *end;
    uint16 ticks;
    uint8 sound;
    uint8 voice;
    uint8 control[2];
    uint32 speaker;
} SOUND_SCRIPT;

extern SOUND_SCRIPT sound_script;

uint32 sound_script_init(void);
uint32 sound_script_update(void);

typedef struct
{
    uint32 played_mask;
    uint16 phase;
    uint16 ticks;
    uint8 voice;
    uint8 sound;
    uint8 speaker;
} MISSION_PROMPT;

extern MISSION_PROMPT mission_prompt;

uint32 mission_prompt_permission(uint32 id, uint32 mission, uint32 kind);
void mission_prompt_request(uint32 kind, uint32 value, uint32 id, uint32 code);
void mission_prompt_init(void);
void mission_prompt_update(void);

typedef struct
{
    uint8 sound;
    uint16 ticks;
} SOUND_DELAYED;

typedef struct
{
    uint8 sound;
    uint8 voice;
    uint16 ticks;
} SOUND_QUEUED;

typedef struct
{
    uint8 head;
    uint8 tail;
    SOUND_DELAYED delayed[8];
    SOUND_QUEUED queued[32];
} SOUND_SCHEDULE;

extern SOUND_SCHEDULE sound_schedule;

void sound_load_bank(SOUND_BANK_INPUT bank);
uint32 sound_get_note(uint32 sound);
uint32 sound_play_spatial(uint32 sound, uint32 mode, const uint32 *native_position);
void sound_play_spatial_reuse(uint32 sound, uint32 mode, const uint32 *native_position);
uint32 sound_play(uint32 sound);
uint32 sound_play_reuse(uint32 sound);
void sound_play_on_voice(uint32 sound, uint32 voice);
uint32 sound_play_allocated(uint32 sound);
uint32 sound_find_voice(uint32 sound);
uint32 sound_reserve_voice(void);
uint32 sound_voice_status(uint32 voice);
uint32 sound_stop_voice(uint32 voice);
void sound_stop_all(void);
uint32 sound_set_voice_note(uint32 index, uint32 note);
void sound_set_voice_volume(uint32 voice, uint32 left, uint32 right);
uint32 sound_set_voice_fade(uint32 index, uint32 percent, uint32 steps);
void sound_publish_voice(uint32 index);
void sound_publish_key_on(uint32 index);
void sound_update(void);
void sound_start_voice(uint32 sound, uint32 index);
void sound_reset_voice(uint32 index);
void sound_init_voices(void);
uint32 sound_offset_note(uint32 note, uint32 offset);
void sound_restore_voice_defaults(uint32 voice);
void sound_apply_voice_dir(uint32 voice, uint32 direction);
uint32 sound_offset_voice_note(uint32 voice, uint32 offset);

extern SpuReverbAttr sound_reverb_attr;

sint32 sound_init_reverb(void);
void sound_enable_reverb(void);
sint32 sound_disable_reverb(void);
uint32 sound_reset_reverb_mode(void);

void sound_reset_slot(uint32 slot);
void sound_init_slots(void);
void sound_schedule_delayed(uint32 kind, uint32 value);
void sound_update_slots(void);
void sound_init_queue(void);
void sound_queue_push(uint32 value, uint32 kind);
void sound_update_queue(void);

typedef struct
{
    uint32 distance;
    uint32 speed;
    uint16 pan;
    sint16 dir;
} AUDIO_SPATIAL;

void audio_spatial_reset(AUDIO_SPATIAL *state);
void audio_spatial_update(AUDIO_SPATIAL *state, const uint32 *native_source, const uint32 *native_listener, const uint16 *native_listener_angles);
void audio_spatial_attenuate(uint32 voice, uint32 mode, const AUDIO_SPATIAL *state);
void audio_spatial_pan(uint32 voice, const AUDIO_SPATIAL *state);
void audio_spatial_pitch(uint32 voice, const AUDIO_SPATIAL *state);
void audio_spatial_crossfade(uint32 voice, uint32 other_voice, const AUDIO_SPATIAL *state);

typedef struct
{
    uint8 flags;
    uint8 voice;
    uint8 volume_period;
    uint8 note_period;
    uint16 volume_depth;
    uint16 note_depth;
    uint16 volume_ticks;
    uint16 note_ticks;
} AUDIO_MODULATION;

void audio_modulation_init(AUDIO_MODULATION *state, uint32 voice);
void audio_modulation_set_volume(uint32 a, uint32 b, AUDIO_MODULATION *state);
void audio_modulation_set_note(uint32 a, uint32 b, AUDIO_MODULATION *state);
void audio_modulation_update(AUDIO_MODULATION *state);

typedef struct
{
    uint8 phase;
    uint8 sounds[2];
    uint8 voice;
    sint16 distance_threshold;
    uint8 speed_threshold;
} AUDIO_AUX_CONTROL;

void audio_aux_init(AUDIO_AUX_CONTROL *control, uint32 a, uint32 b, uint32 c, uint32 d);
void audio_aux_update(AUDIO_AUX_CONTROL *control, const AUDIO_SPATIAL *spatial);

enum
{
    AUDIO_PCM_CAPTURE_BYTES = 2048
};

typedef struct
{
    uint8 bytes[AUDIO_PCM_CAPTURE_BYTES];
} AUDIO_PCM_CAPTURE;

typedef struct
{
    const RESOURCE_AUDIO_TRACK *records;
    uint32 count;
} AUDIO_TRACK_TABLE;

typedef struct
{
    uint32 flags;
    uint32 cur_idx;
    uint32 requested_idx;
    const AUDIO_TRACK_TABLE *tracks;
    AUDIO_PCM_CAPTURE capture;
} AUDIO_PLAYER;

typedef struct
{
    uint32 enabled;
    RESOURCE_AUDIO_TRACK track;
} DIALOGUE_AUDIO;

extern const AUDIO_TRACK_TABLE audio_tracks_options;
extern const AUDIO_TRACK_TABLE audio_tracks_sound_test;
extern const AUDIO_TRACK_TABLE audio_tracks_debrief;
extern const AUDIO_TRACK_TABLE audio_tracks_aircraft;
extern const AUDIO_TRACK_TABLE audio_tracks_mission;
extern const AUDIO_TRACK_TABLE audio_tracks_events;
extern const AUDIO_TRACK_TABLE audio_tracks_transition;

sint16 audio_mission_track_id(uint32 mission);
uint8 audio_mission_uses_speech(uint32 mission);
uint8 audio_mission_music_index(uint32 mission);
const RESOURCE_AUDIO_TRACK *audio_main_track(uint32 index);
const RESOURCE_AUDIO_TRACK *audio_mission_phrase(uint32 mission, uint32 index);
const RESOURCE_AUDIO_TRACK *audio_fallback_track(uint32 index);

typedef struct
{
    uint8 volume[2];
    uint8 flags;
} AUDIO_SETTINGS;

typedef struct
{
    uint8 voice;
    uint8 target_idx;
    uint32 distance;
    AUDIO_SPATIAL spatial;
} PROXIMITY_AUDIO;

typedef struct
{
    uint8 active;
    uint8 voices[3];
    uint8 sounds[3];
} ENGINE_AUDIO;

typedef struct
{
    uint32 voice1_pitch;
    uint32 voice0_level;
    uint16 voice2_modulation;
    uint16 voice1_level;
    uint16 voice0_modulation;
} ENGINE_AUDIO_INPUT;

extern ENGINE_AUDIO engine_audio;

void engine_audio_init(void);
void engine_audio_stop(void);
void engine_audio_update(const ENGINE_AUDIO_INPUT *native_input);
void engine_audio_apply(void);

extern SpuCommonAttr sound_common_attr;

void sound_init_cd(uint32 enabled);
void sound_set_master_volume(uint32 volume);
void sound_set_cd_volume(uint32 volume);
void sound_set_cd_mix(uint32 mode);
void sound_set_cd_reverb(uint32 enabled);

extern PROXIMITY_AUDIO proximity_audio;

void proximity_audio_reset(void);
void proximity_audio_update(void);

extern AUDIO_SETTINGS audio_settings;
extern AUDIO_PLAYER mission_audio_player;
extern RESOURCE_AUDIO_TRACK menu_audio_track;
extern DIALOGUE_AUDIO dialogue_audio;
extern AUDIO_PCM_CAPTURE menu_audio_capture;

void sound_initialize_mix(void);
void sound_initialize(void);

void mission_audio_initialize_groups(void);

void mission_audio_stop_groups(void);

void mission_audio_stop_scene_groups(void);

void mission_audio_reset_sequence(void);
void mission_audio_bind_tracks(const AUDIO_TRACK_TABLE *tracks);

uint32 mission_audio_current_track_id(void);

uint32 mission_audio_control_track(sint32 mode, uint32 resource);

uint32 mission_audio_music_index(uint32 index);

#endif
