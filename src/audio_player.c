#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "audio_player.h"
#include "game_functions.h"

static uint32 sound_script_start(const uint8 *data, uint32 bytes);

#if !defined(XPORT_SPU_WRITE_MAX_BYTES) || XPORT_SPU_WRITE_MAX_BYTES != 0x7eff0u
    #error AC3 requires the reviewed late-PsyQ SPU write limit
#endif

#pragma pack(push, 1)

typedef struct
{
    uint8 first_sample, sample_count;
    sint16 source_word_offset;
    uint32 spu_address, transfer_bytes;
} SOUND_BANK_HEADER;

typedef struct
{
    uint8 voice;
    uint8 unknown[3];
    uint16 volume[2];
    uint16 note, sample_note;
    uint16 adsr[2];
    uint32 address;
} SOUND_BANK_SAMPLE;

#pragma pack(pop)

typedef char sound_bank_wire_layout_check[sizeof(SOUND_BANK_HEADER) == 12 && offsetof(SOUND_BANK_HEADER, source_word_offset) == 2 && offsetof(SOUND_BANK_HEADER, spu_address) == 4 && offsetof(SOUND_BANK_HEADER, transfer_bytes) == 8 && sizeof(SOUND_BANK_SAMPLE) == 20 && offsetof(SOUND_BANK_SAMPLE, volume) == 4 && offsetof(SOUND_BANK_SAMPLE, note) == 8 && offsetof(SOUND_BANK_SAMPLE, sample_note) == 10 && offsetof(SOUND_BANK_SAMPLE, adsr) == 12 && offsetof(SOUND_BANK_SAMPLE, address) == 16 ? 1 : -1];

typedef struct
{
    uint32 address;
    uint16 volume[2];
    uint16 note;
    uint16 sample_note;
    uint16 adsr[2];
    uint8 voice;
} SOUND_SAMPLE;

static sint32 sound_upload(uint32 address, const uint8 *source, uint32 bytes, uint32 source_bytes);

static SOUND_VOICE sound_voices[24];

SOUND_VOICE *sound_voice_at(uint32 index)
{
    if (index >= 24)
        abort();
    return &sound_voices[index];
}

static SOUND_SAMPLE sound_samples[SOUND_ID_COUNT];

void sound_reset_storage(void)
{
    memset(sound_samples, 0, sizeof(sound_samples));
    memset(sound_voices, 0, sizeof(sound_voices));
}

static const SOUND_SAMPLE *sound_sample_at(uint32 sound)
{
    if (sound >= SOUND_ID_COUNT)
        abort();
    return &sound_samples[sound];
}

ENGINE_AUDIO engine_audio;
SpuCommonAttr sound_common_attr;
PROXIMITY_AUDIO proximity_audio;
SOUND_SCHEDULE sound_schedule;
SpuReverbAttr sound_reverb_attr;
MISSION_PROMPT mission_prompt;
SOUND_SCRIPT sound_script;
SPEECH_QUEUE speech_queue;

AUDIO_SETTINGS audio_settings;
AUDIO_PLAYER mission_audio_player;
RESOURCE_AUDIO_TRACK menu_audio_track;
DIALOGUE_AUDIO dialogue_audio;

#define AUDIO_TRACK_RECORD(id, fade, duration, offset, next) {(sint16)(id), (sint16)(fade), (sint16)(duration), (sint16)(offset), next}

static const RESOURCE_AUDIO_TRACK audio_options_records[] = {
    AUDIO_TRACK_RECORD(0x083Du, 0x0000u, 0x0000u, 0x0000u, NULL),
};

const AUDIO_TRACK_TABLE audio_tracks_options = {audio_options_records, 1};

static const RESOURCE_AUDIO_TRACK audio_sound_test_records[] = {
    AUDIO_TRACK_RECORD(0x083Du, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x0843u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x0838u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x0836u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x0835u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x0844u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x083Bu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x0833u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x0826u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x0841u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x0831u, 0x0000u, 0x0000u, 0x0000u, NULL),
    AUDIO_TRACK_RECORD(0x0839u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x0829u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x0834u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x082Du, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x0832u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x082Eu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x0845u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x082Bu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x0837u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x0842u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x0827u, 0x0000u, 0x0000u, 0x0000u, NULL),
    AUDIO_TRACK_RECORD(0x082Fu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x0825u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x0828u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x083Fu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x082Cu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x0840u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x082Au, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x0830u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x0824u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x0846u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x083Eu, 0x0000u, 0x0000u, 0x0000u, NULL),
};

const AUDIO_TRACK_TABLE audio_tracks_sound_test = {audio_sound_test_records, 33};

static const RESOURCE_AUDIO_TRACK audio_debrief_records[] = {
    AUDIO_TRACK_RECORD(0x0000u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x193Au, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x193Bu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x193Cu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x193Du, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x193Eu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x193Fu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1940u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1941u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1942u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1943u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1944u, 0x0000u, 0x0000u, 0x0000u, NULL),
    AUDIO_TRACK_RECORD(0x1945u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1946u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1947u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1948u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1949u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x194Au, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x194Bu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x194Cu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x194Du, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x194Eu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x194Fu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1950u, 0x0000u, 0x0000u, 0x0000u, NULL),
    AUDIO_TRACK_RECORD(0x1951u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1952u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1953u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1954u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1955u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1956u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1957u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1958u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1959u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x195Au, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x195Bu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x195Du, 0x0000u, 0x0000u, 0x0000u, NULL),
};

const AUDIO_TRACK_TABLE audio_tracks_debrief = {audio_debrief_records, 36};

static const RESOURCE_AUDIO_TRACK audio_aircraft_records[] = {
    AUDIO_TRACK_RECORD(0x083Eu, 0x0000u, 0x0000u, 0x0000u, NULL),
};

const AUDIO_TRACK_TABLE audio_tracks_aircraft = {audio_aircraft_records, 1};

static const RESOURCE_AUDIO_TRACK audio_mission_records[] = {
    AUDIO_TRACK_RECORD(0x1919u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1919u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x191Au, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x191Bu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x191Cu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x191Du, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x191Eu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x191Fu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1920u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1921u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1922u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1923u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1924u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1925u, 0x0000u, 0x0000u, 0x0000u, NULL),
    AUDIO_TRACK_RECORD(0x1926u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1927u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1928u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1929u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x192Au, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x192Bu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x192Cu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x192Du, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x192Eu, 0x0000u, 0x0000u, 0x0000u, NULL),
    AUDIO_TRACK_RECORD(0x192Fu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1930u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1931u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1932u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1933u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1939u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1934u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1935u, 0x0000u, 0x0000u, 0x0000u, NULL),
    AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1936u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1937u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1938u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL),
};

const AUDIO_TRACK_TABLE audio_tracks_mission = {audio_mission_records, 60};

static const RESOURCE_AUDIO_TRACK audio_events_records[] = {
    AUDIO_TRACK_RECORD(0x0909u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x090Au, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x090Bu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x090Cu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x090Du, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x090Eu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x090Fu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x0910u, 0x0000u, 0x0000u, 0x0000u, NULL),
};

const AUDIO_TRACK_TABLE audio_tracks_events = {audio_events_records, 8};

static const RESOURCE_AUDIO_TRACK audio_transition_records[] = {
    AUDIO_TRACK_RECORD(0x08E7u, 0x0000u, 0x0000u, 0x0000u, NULL),
};

const AUDIO_TRACK_TABLE audio_tracks_transition = {audio_transition_records, 1};

static const uint8 mission_music_indices[] = {
    0, 9, 7, 5, 38, 13, 31, 36, 20, 0, 10, 24, 3, 30, 7, 1, 31, 39, 18, 0, 13, 30, 8, 2, 0, 10, 9, 24, 15, 38, 40, 20, 32, 27, 23, 5, 31, 20, 34, 27, 10, 9, 22, 3, 30, 35, 18, 29, 19, 0, 1, 31, 39, 33, 23, 32, 22, 40, 37, 0, 0, 0, 0, 0,
};

static const RESOURCE_AUDIO_TRACK main_track_records[] = {
    AUDIO_TRACK_RECORD(0x0831u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 0),  AUDIO_TRACK_RECORD(0x0832u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 1),  AUDIO_TRACK_RECORD(0x0833u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 2),  AUDIO_TRACK_RECORD(0x0834u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 3),  AUDIO_TRACK_RECORD(0x0831u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 4),  AUDIO_TRACK_RECORD(0x0835u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 5),  AUDIO_TRACK_RECORD(0x0831u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 6),  AUDIO_TRACK_RECORD(0x0836u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 7),  AUDIO_TRACK_RECORD(0x0837u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 8),  AUDIO_TRACK_RECORD(0x0838u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 9),  AUDIO_TRACK_RECORD(0x0839u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 10),
    AUDIO_TRACK_RECORD(0x0831u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 11), AUDIO_TRACK_RECORD(0x083Au, 0x0000u, 0x0000u, 0x0000u, main_track_records + 12), AUDIO_TRACK_RECORD(0x083Bu, 0x0000u, 0x0000u, 0x0000u, main_track_records + 13), AUDIO_TRACK_RECORD(0x0831u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 14), AUDIO_TRACK_RECORD(0x083Cu, 0x0000u, 0x0000u, 0x0000u, main_track_records + 15), AUDIO_TRACK_RECORD(0x083Du, 0x0000u, 0x0000u, 0x0000u, main_track_records + 16), AUDIO_TRACK_RECORD(0x083Eu, 0x0000u, 0x0000u, 0x0000u, main_track_records + 17), AUDIO_TRACK_RECORD(0x083Fu, 0x0000u, 0x0000u, 0x0000u, main_track_records + 18), AUDIO_TRACK_RECORD(0x0840u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 20), AUDIO_TRACK_RECORD(0x0828u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 20), AUDIO_TRACK_RECORD(0x0841u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 21),
    AUDIO_TRACK_RECORD(0x0831u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 22), AUDIO_TRACK_RECORD(0x0827u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 23), AUDIO_TRACK_RECORD(0x0828u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 24), AUDIO_TRACK_RECORD(0x0829u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 25), AUDIO_TRACK_RECORD(0x0827u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 26), AUDIO_TRACK_RECORD(0x0827u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 27), AUDIO_TRACK_RECORD(0x082Au, 0x0000u, 0x0000u, 0x0000u, main_track_records + 28), AUDIO_TRACK_RECORD(0x082Bu, 0x0000u, 0x0000u, 0x0000u, main_track_records + 29), AUDIO_TRACK_RECORD(0x082Cu, 0x0000u, 0x0000u, 0x0000u, main_track_records + 30), AUDIO_TRACK_RECORD(0x082Du, 0x0000u, 0x0000u, 0x0000u, main_track_records + 31), AUDIO_TRACK_RECORD(0x082Eu, 0x0000u, 0x0000u, 0x0000u, main_track_records + 32),
    AUDIO_TRACK_RECORD(0x082Fu, 0x0000u, 0x0000u, 0x0000u, main_track_records + 33), AUDIO_TRACK_RECORD(0x0830u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 34), AUDIO_TRACK_RECORD(0x0824u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 35), AUDIO_TRACK_RECORD(0x0825u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 36), AUDIO_TRACK_RECORD(0x0826u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 38), AUDIO_TRACK_RECORD(0x0826u, 0x0001u, 0x005Au, 0x0168u, main_track_records + 38), AUDIO_TRACK_RECORD(0x0846u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 39), AUDIO_TRACK_RECORD(0x0844u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 40), AUDIO_TRACK_RECORD(0x0845u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 41), AUDIO_TRACK_RECORD(0x0842u, 0x0000u, 0x0000u, 0x0000u, main_track_records + 42),
};

static const RESOURCE_AUDIO_TRACK mission_phrase_records[] = {
    AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18ADu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18AEu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18AFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18B0u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18B1u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18B2u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18B3u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18B4u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18B5u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18B6u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x189Au, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18B7u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x189Bu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18B8u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18B9u, 0x0000u, 0x0000u, 0x0000u, NULL),
    AUDIO_TRACK_RECORD(0x18BAu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18BBu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18BCu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18BDu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x189Cu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18BEu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18BFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18C0u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18C1u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18C2u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x189Du, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18C3u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18C4u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18C5u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18C6u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18C7u, 0x0000u, 0x0000u, 0x0000u, NULL),
    AUDIO_TRACK_RECORD(0x189Eu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x189Fu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18C8u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18C9u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18CAu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18CBu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18A0u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18CCu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18CDu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18CEu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18CFu, 0x0000u, 0x0000u, 0x0000u, NULL),
    AUDIO_TRACK_RECORD(0x18D0u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18D1u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18D2u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18D3u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18D4u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18D5u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18D6u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18D7u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18D8u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18A1u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18D9u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18A2u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18DAu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18DBu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18DCu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18DDu, 0x0000u, 0x0000u, 0x0000u, NULL),
    AUDIO_TRACK_RECORD(0x18DEu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18DFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18E0u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18E1u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18E2u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18A3u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18A4u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18E3u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18E4u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18E5u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18E6u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18E8u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18A5u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18E9u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18EAu, 0x0000u, 0x0000u, 0x0000u, NULL),
    AUDIO_TRACK_RECORD(0x18EBu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18ECu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18EDu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18A6u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18EEu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18EFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18F0u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18F1u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18F2u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18F3u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18F4u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18F5u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18A7u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18F6u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18F7u, 0x0000u, 0x0000u, 0x0000u, NULL),
    AUDIO_TRACK_RECORD(0x18A8u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18F8u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18A9u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18F9u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18FAu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18FBu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18AAu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18FCu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18ABu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18FDu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18FEu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x18FFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1900u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1901u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1902u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL),
    AUDIO_TRACK_RECORD(0x18ACu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1903u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1904u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1905u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1906u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1907u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0x1908u, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL), AUDIO_TRACK_RECORD(0xFFFFu, 0x0000u, 0x0000u, 0x0000u, NULL),
};

static const RESOURCE_AUDIO_TRACK fallback_track_records[] = {
    AUDIO_TRACK_RECORD(0x0843u, 0x0000u, 0x0000u, 0x0000u, fallback_track_records + 0), AUDIO_TRACK_RECORD(0x0836u, 0x0000u, 0x0000u, 0x0000u, fallback_track_records + 1), AUDIO_TRACK_RECORD(0x0838u, 0x0000u, 0x0000u, 0x0000u, fallback_track_records + 2), AUDIO_TRACK_RECORD(0x0839u, 0x0000u, 0x0000u, 0x0000u, fallback_track_records + 3), AUDIO_TRACK_RECORD(0x0829u, 0x0000u, 0x0000u, 0x0000u, fallback_track_records + 4), AUDIO_TRACK_RECORD(0x082Bu, 0x0000u, 0x0000u, 0x0000u, fallback_track_records + 5), AUDIO_TRACK_RECORD(0x082Eu, 0x0000u, 0x0000u, 0x0000u, fallback_track_records + 6),
};

static const RESOURCE_AUDIO_TRACK main_track_disc2 = AUDIO_TRACK_RECORD(0x0831u, 0x0000u, 0x0000u, 0x0000u, &main_track_disc2);

static const RESOURCE_AUDIO_TRACK *const main_track_roots[] = {
    main_track_records + 0,  main_track_records + 1,  main_track_records + 2,  main_track_records + 3,  main_track_records + 4,  main_track_records + 5,  main_track_records + 6,  main_track_records + 7,  main_track_records + 8,  main_track_records + 9,  main_track_records + 10, main_track_records + 11, main_track_records + 12, main_track_records + 13, main_track_records + 14, main_track_records + 15, main_track_records + 16, main_track_records + 17, main_track_records + 18, main_track_records + 19, main_track_records + 21,
    main_track_records + 22, main_track_records + 23, main_track_records + 24, main_track_records + 25, main_track_records + 26, main_track_records + 27, main_track_records + 28, main_track_records + 29, main_track_records + 30, main_track_records + 31, main_track_records + 32, main_track_records + 33, main_track_records + 34, main_track_records + 35, main_track_records + 36, main_track_records + 37, main_track_records + 39, main_track_records + 40, main_track_records + 41, main_track_records + 42,
};

static const AUDIO_TRACK_TABLE mission_phrase_tables[] = {
    {mission_phrase_records + 0, 125}, {mission_phrase_records + 1, 124}, {mission_phrase_records + 2, 123},  {mission_phrase_records + 3, 122},  {mission_phrase_records + 6, 119},  {mission_phrase_records + 14, 111}, {mission_phrase_records + 15, 110}, {mission_phrase_records + 20, 105}, {mission_phrase_records + 27, 98},  {mission_phrase_records + 28, 97},  {mission_phrase_records + 34, 91}, {mission_phrase_records + 35, 90}, {mission_phrase_records + 36, 89}, {mission_phrase_records + 37, 88}, {mission_phrase_records + 39, 86}, {mission_phrase_records + 42, 83}, {mission_phrase_records + 43, 82}, {mission_phrase_records + 44, 81}, {mission_phrase_records + 45, 80}, {mission_phrase_records + 46, 79}, {mission_phrase_records + 47, 78},
    {mission_phrase_records + 49, 76}, {mission_phrase_records + 51, 74}, {mission_phrase_records + 54, 71},  {mission_phrase_records + 56, 69},  {mission_phrase_records + 58, 67},  {mission_phrase_records + 60, 65},  {mission_phrase_records + 62, 63},  {mission_phrase_records + 65, 60},  {mission_phrase_records + 67, 58},  {mission_phrase_records + 71, 54},  {mission_phrase_records + 76, 49}, {mission_phrase_records + 78, 47}, {mission_phrase_records + 79, 46}, {mission_phrase_records + 83, 42}, {mission_phrase_records + 84, 41}, {mission_phrase_records + 85, 40}, {mission_phrase_records + 87, 38}, {mission_phrase_records + 89, 36}, {mission_phrase_records + 91, 34}, {mission_phrase_records + 92, 33}, {mission_phrase_records + 93, 32},
    {mission_phrase_records + 97, 28}, {mission_phrase_records + 99, 26}, {mission_phrase_records + 101, 24}, {mission_phrase_records + 105, 20}, {mission_phrase_records + 107, 18}, {mission_phrase_records + 108, 17}, {mission_phrase_records + 109, 16}, {mission_phrase_records + 111, 14}, {mission_phrase_records + 112, 13}, {mission_phrase_records + 114, 11}, {mission_phrase_records + 116, 9}, {mission_phrase_records + 117, 8}, {mission_phrase_records + 118, 7}, {mission_phrase_records + 119, 6}, {mission_phrase_records + 120, 5}, {mission_phrase_records + 121, 4}, {mission_phrase_records + 122, 3}, {mission_phrase_records + 123, 2}, {mission_phrase_records + 124, 1},
};

#undef AUDIO_TRACK_RECORD

sint16 audio_mission_track_id(uint32 mission)
{
    if (mission >= audio_tracks_mission.count)
        abort();
    return audio_tracks_mission.records[mission].id;
}

uint8 audio_mission_uses_speech(uint32 mission)
{
    if (mission >= GAME_MISSION_COUNT)
        abort();
    return mission == 2 || mission == 14 || mission == 31;
}

uint8 audio_mission_music_index(uint32 mission)
{
    if (mission >= sizeof(mission_music_indices))
        abort();
    return mission_music_indices[mission];
}

const RESOURCE_AUDIO_TRACK *audio_main_track(uint32 index)
{
    const RESOURCE_AUDIO_TRACK *record;
    if (index >= sizeof(main_track_roots) / sizeof(main_track_roots[0]))
        abort();
    record = main_track_roots[index];
    if (record == main_track_records + 15 && game_current_data_disc() == 2)
        return &main_track_disc2;
    return record;
}

const RESOURCE_AUDIO_TRACK *audio_mission_phrase(uint32 mission, uint32 index)
{
    const AUDIO_TRACK_TABLE *table;
    if (mission >= sizeof(mission_phrase_tables) / sizeof(mission_phrase_tables[0]))
        abort();
    table = mission_phrase_tables + mission;
    if (index >= table->count)
        abort();
    return table->records + index;
}

const RESOURCE_AUDIO_TRACK *audio_fallback_track(uint32 index)
{
    if (index >= sizeof(fallback_track_records) / sizeof(fallback_track_records[0]))
        abort();
    return fallback_track_records + index;
}

void proximity_audio_reset(void)
{
    FUNCTION_MARKER(0x80051824, "ACE3.BIN");
    proximity_audio.voice = 255;
    proximity_audio.target_idx = 255;
    proximity_audio.distance = 0x100000;
    return;
}

void proximity_audio_update(void)
{
    MISSION_ACTOR *pose_owner_source;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_slots;
    uint32 actor_index_table;

    uint32 voice, index, kind, distance, value, selected;
    MISSION_ACTOR_TARGET *target;
    FUNCTION_MARKER(0x80051840, "ACE3.BIN");
    voice = proximity_audio.voice;
    if (voice != 255)
    {
        if ((game_runtime.mission_control.mode & 4) == 0)
        {
            sound_stop_voice(proximity_audio.voice);
            proximity_audio.voice = 255;
        }
    }
    actor_index_slots = 32;
    proximity_audio.target_idx = 255;
    proximity_audio.distance = 0x100000;
    for (index = 0; index < 32; ++index)
    {
        actor_traits_object = mission_actor_traits((actor_index_slots + ((uint32)((sint32)(index * 4) >> 2))));
        if ((~actor_traits_object->flags & 0xC000) != 0)
            continue;
        kind = actor_traits_object->kind;
        if ((kind - 35) >= 2 && (sint16)kind != 43)
            continue;
        target = mission_actor_target(index + 32);
        if (target == 0)
            continue;
        distance = proximity_audio.distance;
        value = (target->distance);
        if ((sint32)value < (sint32)distance)
        {
            value = (target->distance);
            proximity_audio.target_idx = index + 32;
            proximity_audio.distance = value;
        }
    }
    voice = proximity_audio.voice;
    if (voice != 255)
    {
        selected = proximity_audio.target_idx;
        if (selected == 255 || (sint32)proximity_audio.distance >= 0x2000)
        {
            value = sound_stop_voice(proximity_audio.voice);
            proximity_audio.voice = 255;
            {
                (void)(uint32)(value);
                return;
            }
        }
    }
    selected = proximity_audio.target_idx;
    if (selected == 255)
        return;
    voice = proximity_audio.voice;
    if (voice == 255)
    {
        if ((sint32)proximity_audio.distance < 0x2000)
            proximity_audio.voice = sound_play_allocated(92);
        voice = proximity_audio.voice;
        if (voice == 255)
            return;
    }
    selected = proximity_audio.target_idx;
    actor_index_table = 0;
    pose_owner_source = mission_actor((actor_index_table + ((uint32)((sint32)(selected * 4) >> 2))));
    audio_spatial_update(&proximity_audio.spatial, pose_owner_source->pose.position, game_runtime.camera_view.pose.position, game_runtime.camera_view.pose.angles);
    sound_restore_voice_defaults(proximity_audio.voice);
    audio_spatial_attenuate(proximity_audio.voice, 2, &proximity_audio.spatial);
    audio_spatial_pan(proximity_audio.voice, &proximity_audio.spatial);
    {
        (void)(audio_spatial_pitch(proximity_audio.voice, &proximity_audio.spatial));
        return;
    }
}

void sound_set_cd_mix(uint32 mode)
{
    FUNCTION_MARKER(0x8002D880, "ACE3.BIN");
    sound_common_attr.mask = 0x200;
    sound_common_attr.cd.mix = mode != 0;
    SpuSetCommonAttr(&sound_common_attr);
}

void sound_init_cd(uint32 enabled)
{
    uint32 volume;

    FUNCTION_MARKER(0x8002D79C, "ACE3.BIN");
    sound_common_attr.mask = 0x3C3;
    sound_common_attr.mvol.left = (sint16)(0x3FFF);
    sound_common_attr.mvol.right = (sint16)(0x3FFF);
    sound_common_attr.cd.mix = enabled != 0;
    sound_common_attr.cd.reverb = 0;
    volume = audio_settings.volume[1] >> 1;
    sound_common_attr.cd.volume.right = (sint16)(volume);
    sound_common_attr.cd.volume.left = (sint16)(volume);
    SpuSetCommonAttr(&sound_common_attr);
}

void sound_set_master_volume(uint32 volume)
{
    FUNCTION_MARKER(0x8002D80C, "ACE3.BIN");
    sound_common_attr.mask = 3;
    sound_common_attr.mvol.left = (sint16)(volume);
    sound_common_attr.mvol.right = (sint16)(volume);
    SpuSetCommonAttr(&sound_common_attr);
}

void sound_set_cd_volume(uint32 volume)
{
    FUNCTION_MARKER(0x8002D844, "ACE3.BIN");
    sound_common_attr.mask = 0xC0;
    sound_common_attr.cd.volume.right = (sint16)(volume << 6);
    sound_common_attr.cd.volume.left = (sint16)(volume << 6);
    SpuSetCommonAttr(&sound_common_attr);
}

void sound_set_cd_reverb(uint32 enabled)
{
    FUNCTION_MARKER(0x8002D8C8, "ACE3.BIN");
    sound_common_attr.mask = 256;
    sound_common_attr.cd.reverb = enabled != 0;
    SpuSetCommonAttr(&sound_common_attr);
}

void engine_audio_stop(void)
{
    FUNCTION_MARKER(0x800301A8, "ACE3.BIN");
    sound_stop_voice(engine_audio.voices[0]);
    sound_stop_voice(engine_audio.voices[1]);
    sound_stop_voice(engine_audio.voices[2]);
    engine_audio.voices[2] = 255;
    engine_audio.voices[1] = 255;
    engine_audio.voices[0] = 255;
    return;
}

void engine_audio_update(const ENGINE_AUDIO_INPUT *native_input)
{
    uint32 result, next, value, left, right, voice, offset, magnitude;
    const SOUND_SAMPLE *descriptor;
    FUNCTION_MARKER(0x800301F0, "ACE3.BIN");
    if (engine_audio.active == 0)
    {
        result = sound_play(engine_audio.sounds[0]);
        next = engine_audio.sounds[1];
        engine_audio.voices[0] = result;
        result = sound_play(next);
        next = engine_audio.sounds[2];
        engine_audio.voices[1] = result;
        result = sound_play(next);
        value = engine_audio.active;
        engine_audio.voices[2] = result;
        engine_audio.active = value + 1;
    }
    if (engine_audio.voices[2] != 255)
    {
        descriptor = sound_sample_at(engine_audio.sounds[2]);
        value = ((native_input->voice2_modulation));
        result = sound_offset_note((sint16)descriptor->note, (uint32)((sint32)(value << 16) >> 20));
        sound_set_voice_note(engine_audio.voices[2], result & 0xFFFF);
        voice = engine_audio.voices[2];
        value = ((native_input->voice2_modulation));
        descriptor = sound_sample_at(engine_audio.sounds[2]);
        offset = ((sint32)(value << 16) >> 18);
        left = descriptor->volume[0];
        right = descriptor->volume[1];
        sound_set_voice_volume(voice, (uint32)(sint32)(sint16)(left + offset), (uint32)(sint32)(sint16)(right + offset));
    }
    if (engine_audio.voices[1] != 255)
    {
        descriptor = sound_sample_at(engine_audio.sounds[1]);
        value = ((native_input->voice1_pitch));
        offset = ((sint32)((value + 0xFFFDC000) << 9) >> 16);
        result = sound_offset_note((sint16)descriptor->note, offset);
        sound_set_voice_note(engine_audio.voices[1], result & 0xFFFF);
        descriptor = sound_sample_at(engine_audio.sounds[1]);
        voice = engine_audio.voices[1];
        value = (((sint16)native_input->voice1_level));
        left = descriptor->volume[0];
        right = descriptor->volume[1];
        offset = value << 2;
        sound_set_voice_volume(voice, (uint32)(sint32)(sint16)(left - offset), (uint32)(sint32)(sint16)(right + offset));
    }
    result = engine_audio.voices[0];
    if (result == 255)
    {
        (void)(uint32)(result);
        return;
    }
    descriptor = sound_sample_at(engine_audio.sounds[0]);
    value = (((sint16)native_input->voice0_modulation));
    magnitude = (sint32)value < 0 ? 0 - value : value;
    result = sound_offset_note((sint16)descriptor->note, (uint32)((sint32)(magnitude << 18) >> 16));
    sound_set_voice_note(engine_audio.voices[0], result & 0xFFFF);
    value = (((sint16)native_input->voice0_modulation));
    next = ((native_input->voice0_level));
    magnitude = (sint32)value < 0 ? 0 - value : value;
    next -= 0x200000;
    if ((sint32)next < 0)
        next = 0;
    if ((sint32)next > 0x400000)
        next = 0x400000;
    offset = (magnitude << 3) + 4096 - (uint32)((sint32)next >> 10);
    descriptor = sound_sample_at(engine_audio.sounds[0]);
    voice = engine_audio.voices[0];
    left = descriptor->volume[0];
    right = descriptor->volume[1];
    {
        (void)(sound_set_voice_volume(voice, (uint32)(sint32)(sint16)(left + offset), (uint32)(sint32)(sint16)(right + offset)));
        return;
    }
}

void engine_audio_init(void)
{
    FUNCTION_MARKER(0x80030178, "ACE3.BIN");
    engine_audio.voices[2] = 255;
    engine_audio.voices[1] = 255;
    engine_audio.voices[0] = 255;
    engine_audio.sounds[1] = 84;
    engine_audio.active = 0;
    engine_audio.sounds[0] = 0;
    engine_audio.sounds[2] = 85;
    return;
}

void engine_audio_apply(void)
{
    FLIGHT_DYNAMICS *state;
    FLIGHT_STATUS *context;
    FLIGHT_COMMAND *angles;
    uint32 value;
    ENGINE_AUDIO_INPUT input;
    FUNCTION_MARKER(0x80072114, "ACE3.BIN");
    context = &flight_status;
    state = &flight_dynamics;
    angles = &flight_command;
    value = context->ground_clearance;
    input.voice0_level = value;
    value = state->speed;
    input.voice1_pitch = value;
    value = angles->thrust;
    input.voice2_modulation = value;
    value = flight_orientation.response[0];
    input.voice0_modulation = value;
    value = flight_orientation.response[2];
    input.voice1_level = value;
    {
        (void)(engine_audio_update(&input));
        return;
    }
}

uint32 sound_play_spatial(uint32 sound, uint32 mode, const uint32 *native_position)
{
    uint32 voice, random;
    /* Defines the previous distance and direction before their first read */
    AUDIO_SPATIAL movement = {0};
    FUNCTION_MARKER(0x8002F3C0, "ACE3.BIN");
    voice = sound_play(sound);
    if (voice == 255)
        return 255;
    audio_spatial_update(&movement, native_position, game_runtime.camera_view.pose.position, game_runtime.camera_view.pose.angles);
    audio_spatial_attenuate(voice, mode & 255, &movement);
    audio_spatial_pan(voice, &movement);
    sound_apply_voice_dir(voice, (uint32)(sint32)movement.dir);
    random = game_random();
    return sound_offset_voice_note(voice, (random & 1023) - 512);
}

void sound_play_spatial_reuse(uint32 sound, uint32 mode, const uint32 *native_position)
{
    uint32 voice, random;
    /* Defines the previous distance and direction before their first read */
    AUDIO_SPATIAL movement = {0};
    FUNCTION_MARKER(0x8002F460, "ACE3.BIN");
    voice = sound_play_reuse(sound);
    if (voice == 255)
        return;
    audio_spatial_update(&movement, native_position, game_runtime.camera_view.pose.position, game_runtime.camera_view.pose.angles);
    audio_spatial_attenuate(voice, mode & 255, &movement);
    audio_spatial_pan(voice, &movement);
    sound_apply_voice_dir(voice, (uint32)(sint32)movement.dir);
    random = game_random();
    {
        (void)(uint32)(sound_offset_voice_note(voice, (random & 1023) - 512));
        return;
    }
}

void audio_aux_init(AUDIO_AUX_CONTROL *control, uint32 a, uint32 b, uint32 c, uint32 d)
{
    FUNCTION_MARKER(0x8002ED08, "ACE3.BIN");
    control->phase = 0;
    control->sounds[0] = a;
    control->sounds[1] = b;
    control->distance_threshold = (sint16)c;
    control->voice = 255;
    control->speed_threshold = d;
    return;
}

void audio_aux_update(AUDIO_AUX_CONTROL *control, const AUDIO_SPATIAL *spatial)
{
    uint32 flag, threshold, distance, value, voice;
    FUNCTION_MARKER(0x8002ED2C, "ACE3.BIN");
    flag = ((control->phase));
    if (flag == 0)
    {
        threshold = ((control->distance_threshold));
        distance = ((spatial->distance));
        if (distance < threshold)
        {
            threshold = ((control->speed_threshold));
            value = ((spatial->speed));
            if ((sint32)threshold < (sint32)value)
            {
                control->phase = 1;
                if (((spatial->dir)) >= 0)
                    voice = sound_play_allocated(((control->sounds[0])));
                else
                    voice = sound_play_allocated(((control->sounds[1])));
                control->voice = voice;
            }
            else if ((sint32)(threshold >> 1) < (sint32)value)
            {
                control->phase = 1;
                value = game_random();
                voice = sound_play((uint32)((sint32)value % 3) + 10);
                control->voice = voice;
            }
            value = game_random();
            voice = ((control->voice));
            sound_offset_voice_note(voice, 0 - (value & 0x3FF));
        }
    }
    flag = ((control->phase));
    if (flag == 1)
    {
        voice = ((control->voice));
        if (voice == 255)
            return;
        if (sound_voice_at(voice)->active == 0)
        {
            control->voice = 255;
            control->phase = 0;
        }
    }
    {
        (void)(uint32)(((control->voice)));
        return;
    }
}

void audio_modulation_update(AUDIO_MODULATION *state)
{
    uint32 voice, flags, numerator, divisor, phase, product, delta, left, right, count, note;
    SOUND_VOICE *target;
    sint32 left_signed;
    FUNCTION_MARKER(0x8002EEC8, "ACE3.BIN");
    voice = ((state->voice));
    if (voice == 255)
        return;
    voice = ((state->voice));
    flags = ((state->flags));
    target = sound_voice_at(voice);
    if (flags & 1)
    {
        numerator = (uint32)(((sint16)state->volume_ticks)) << 14;
        divisor = ((state->volume_period));
        if (divisor == 0)
            xport_mips_break(7);
        phase = (uint32)((sint32)numerator / (sint32)divisor) & 0x3FFF;
        product = (uint32)game_wave_at(phase * 2) * (uint32)(((sint16)state->volume_depth));
        delta = ((sint32)product >> 14);
        left = target->volume[0];
        target->volume[0] = (uint16)(left + delta);
        right = target->volume[1];
        left_signed = (sint16)target->volume[0];
        target->volume[1] = (uint16)(right + delta);
        if (left_signed >= 0x4000)
            target->volume[0] = (uint16)(0x3FFF);
        if ((sint16)target->volume[1] >= 0x4000)
            target->volume[1] = (uint16)(0x3FFF);
        if ((sint16)target->volume[0] < 0)
            target->volume[0] = (uint16)(0);
        if ((sint16)target->volume[1] < 0)
            target->volume[1] = (uint16)(0);
        count = ((state->volume_ticks));
        divisor = ((state->volume_period));
        ++count;
        state->volume_ticks = count;
        if ((sint16)count >= (sint32)divisor)
        {
            state->volume_ticks = 0;
        }
    }
    flags = ((state->flags));
    if (flags & 2)
    {
        numerator = (uint32)(((sint16)state->note_ticks)) << 14;
        divisor = ((state->note_period));
        if (divisor == 0)
            xport_mips_break(7);
        phase = (uint32)((sint32)numerator / (sint32)divisor) & 0x3FFF;
        product = (uint32)game_wave_at(phase * 2) * (uint32)(((sint16)state->note_depth));
        note = (sint16)target->note;
        delta = ((sint32)(product << 2) >> 16);
        target->note = (uint16)(sound_offset_note(note, delta));
        count = ((state->note_ticks));
        divisor = ((state->note_period));
        ++count;
        state->note_ticks = count;
        if ((sint16)count >= (sint32)divisor)
        {
            state->note_ticks = 0;
        }
    }
    {
        (void)(uint32)(((state->voice)));
        return;
    }
}

void audio_modulation_set_note(uint32 a, uint32 b, AUDIO_MODULATION *state)
{
    uint32 value;
    FUNCTION_MARKER(0x8002EEA8, "ACE3.BIN");
    value = ((state->flags));
    state->note_period = a;
    state->note_depth = b & 0xFF;
    state->note_ticks = 0;
    value |= 2;
    state->flags = value;
    {
        (void)(uint32)(value);
        return;
    }
}

void audio_modulation_set_volume(uint32 a, uint32 b, AUDIO_MODULATION *state)
{
    uint32 value;
    FUNCTION_MARKER(0x8002EE88, "ACE3.BIN");
    value = ((state->flags));
    state->volume_period = a;
    state->volume_depth = b & 0xFF;
    state->volume_ticks = 0;
    value |= 1;
    state->flags = value;
    {
        (void)(uint32)(value);
        return;
    }
}

void audio_modulation_init(AUDIO_MODULATION *state, uint32 voice)
{
    FUNCTION_MARKER(0x8002EE7C, "ACE3.BIN");
    state->flags = 0;
    state->voice = voice;
}

void audio_spatial_reset(AUDIO_SPATIAL *state)
{
    FUNCTION_MARKER(0x8002E834, "ACE3.BIN");
    state->speed = 0;
    state->distance = 0x100000;
    state->pan = 0;
    state->dir = 0;
    return;
}

void audio_spatial_update(AUDIO_SPATIAL *state, const uint32 *native_source, const uint32 *native_listener, const uint16 *native_listener_angles)
{
    MATRIX matrix = {0};
    uint32 distance, previous, value, absolute, i, input[3], high[3], low[3], result[3], magnitude;
    sint32 angle;
    FUNCTION_MARKER(0x8002E6DC, "ACE3.BIN");
    distance = game_measure_distance_gte(native_listener, native_source);
    previous = ((state->distance));
    if (previous == 0 || previous == 0x100000)
    {
        state->speed = 0;
    }
    else
    {
        state->speed = previous - distance;
    }
    absolute = ((state->speed));
    if ((sint32)absolute < 0)
        absolute = 0 - absolute;
    if ((sint32)absolute >= 301)
    {
        state->speed = 0;
    }
    {
        state->distance = distance;
    }
    game_build_rotation_via_gte(&matrix, native_listener_angles);
    for (i = 0; i < 3; ++i)
    {
        value = native_source[i];
        previous = native_listener[i];
        input[i] = ((sint32)(value - previous) >> 12);
    }
    /* Preserve the SDK long-vector transform and its GTE register effects */
    i = 5;
    SetRotMatrix(&matrix);
    for (i = 0; i < 3; ++i)
    {
        value = input[i];
        if ((sint32)value < 0)
        {
            magnitude = 0 - value;
            high[i] = 0 - (uint32)((sint32)magnitude >> 15);
            low[i] = 0 - (magnitude & 0x7FFF);
        }
        else
        {
            high[i] = ((sint32)value >> 15);
            low[i] = value & 0x7FFF;
        }
    }
    for (i = 0; i < 3; ++i)
        xport_gte_write_data(9 + i, high[i]);
    xport_gte_execute(0x41E012);
    for (i = 0; i < 3; ++i)
        high[i] = xport_gte_read_data(25 + i);
    for (i = 0; i < 3; ++i)
        xport_gte_write_data(9 + i, low[i]);
    xport_gte_execute(0x49E012);
    for (i = 0; i < 3; ++i)
        result[i] = xport_gte_read_data(25 + i) + (high[i] << 3);
    value = game_vector_atan2(result[2], result[0]);
    {
        state->pan = value & 0x3FFF;
    }
    value = (((uint16)state->dir));
    if ((sint32)result[2] < 0)
        value -= 256;
    else
        value += 256;
    {
        state->dir = value;
    }
    angle = ((state->dir));
    if (angle > 1024)
    {
        {
            state->dir = 1024;
        }
        return;
    }
    if (angle < -1024)
    {
        {
            state->dir = 0xFC00;
        }
        return;
    }
    return;
}

void audio_spatial_attenuate(uint32 voice, uint32 mode, const AUDIO_SPATIAL *state)
{
    uint32 value, gain, product;
    SOUND_VOICE *target;
    FUNCTION_MARKER(0x8002E84C, "ACE3.BIN");
    if (voice == 255)
        return;
    target = sound_voice_at(voice);
    value = target->flags;
    target->flags = (uint8)(value | 0x10);
    gain = 0x3FFF - (((state->distance)) << (mode & 31));
    if ((sint32)gain < 0)
        gain = 0;
    if ((sint32)gain >= 0x4001)
        gain = 0x4000;
    product = gain * gain;
    gain = ((sint32)product >> 14);
    product = (uint32)(sint16)target->volume[0] * gain;
    value = ((sint32)product >> 14);
    target->volume[1] = (uint16)(value);
    target->volume[0] = (uint16)(value);
    {
        (void)(uint32)(voice);
        return;
    }
}

void audio_spatial_pan(uint32 voice, const AUDIO_SPATIAL *state)
{
    uint32 value, phase, amplitude, sine_product, cosine_product, gain, minimum, raw;
    SOUND_VOICE *target;
    sint32 signed_value;
    FUNCTION_MARKER(0x8002E8DC, "ACE3.BIN");
    if (voice == 255)
        return;
    target = sound_voice_at(voice);
    value = target->flags;
    target->flags = (uint8)(value | 0x10);
    phase = ((state->pan));
    amplitude = (sint16)target->volume[0];
    phase = (phase >> 1) & 0x3FFF;
    sine_product = amplitude * (uint32)game_wave_at(phase * 2);
    cosine_product = amplitude * (uint32)game_wave_at((phase + 0x1000) * 2);
    target->volume[0] = (uint16)((uint32)((sint32)sine_product >> 14));
    target->volume[1] = (uint16)((uint32)((sint32)cosine_product >> 14));
    signed_value = (sint16)target->volume[0];
    raw = target->volume[0];
    if (signed_value < 0)
        target->volume[0] = (uint16)(0 - raw);
    signed_value = (sint16)target->volume[1];
    raw = target->volume[1];
    if (signed_value < 0)
        target->volume[1] = (uint16)(0 - raw);
    gain = 0x4000 - (((state->distance)) << 6);
    if ((sint32)gain < 0)
        gain = 0;
    value = (sint16)target->volume[0];
    minimum = ((sint32)(amplitude * gain) >> 14);
    if ((sint32)value < (sint32)minimum)
        target->volume[0] = (uint16)(minimum);
    if ((sint16)target->volume[1] < (sint32)minimum)
        target->volume[1] = (uint16)(minimum);
    if ((sint16)target->volume[0] >= 0x4000)
        target->volume[0] = (uint16)(0x3FFF);
    if ((sint16)target->volume[1] >= 0x4000)
        target->volume[1] = (uint16)(0x3FFF);
    {
        (void)(uint32)(voice);
        return;
    }
}

static const sint16 sound_doppler_offsets[512] = {
    -1113, -1110, -1107, -1103, -1100, -1096, -1093, -1089, -1086, -1083, -1079, -1076, -1072, -1069, -1065, -1062, -1058, -1055, -1051, -1047, -1044, -1040, -1037, -1033, -1030, -1027, -1023, -1019, -1015, -1012, -1008, -1005, -1001, -998, -994, -990, -987, -983, -980, -976, -972, -969, -965, -962, -957, -954, -950, -947, -943, -939, -936, -932, -928, -924, -921, -917, -913, -910, -906, -902, -898, -895, -890, -887, -883, -879, -876, -872, -868, -864, -860, -856, -853, -849, -845, -841, -837, -833, -829, -826, -821, -818, -814, -810, -806, -802, -798, -794, -790, -787, -782, -779, -774, -770, -766, -762, -758, -754, -751, -746, -742, -738, -734, -730, -726, -722, -718, -714, -709, -706, -701, -697, -693, -689, -685, -681, -677, -672, -668, -664, -660, -656, -651, -647, -643, -639, -634, -630,
    -626,  -622,  -618,  -613,  -609,  -604,  -600,  -596,  -592,  -588,  -583,  -579,  -574,  -570,  -565,  -561,  -556,  -552,  -548,  -543,  -539,  -534,  -530,  -525,  -521,  -517,  -512,  -508,  -503,  -499,  -494,  -490,  -485,  -481, -477, -471, -467, -462, -458, -453, -449, -445, -439, -435, -430, -426, -420, -416, -411, -407, -402, -397, -393, -388, -383, -378, -374, -369, -364, -360, -354, -350, -344, -340, -335, -330, -326, -320, -316, -310, -306, -300, -296, -291, -286, -281, -276, -271, -266, -261, -255, -251, -246, -241, -236, -230, -226, -220, -215, -211, -205, -200, -194, -190, -184, -179, -173, -169, -164, -158, -153, -147, -143, -137, -132, -127, -121, -116, -110, -105, -99,  -94,  -88,  -83,  -78,  -72,  -67,  -61,  -56,  -50,  -45,  -40,  -34,  -29,  -22,  -17,  -11,  -6,
    0,     5,     10,    16,    21,    28,    33,    39,    45,    50,    56,    61,    68,    73,    80,    85,    92,    97,    102,   109,   115,   121,   127,   133,   139,   144,   151,   156,   163,   169,   176,   181,   188,   194,  199,  206,  212,  219,  225,  232,  237,  243,  250,  256,  263,  269,  276,  282,  289,  295,  301,  308,  314,  321,  327,  334,  340,  346,  354,  360,  367,  373,  381,  387,  394,  400,  407,  414,  420,  428,  434,  442,  448,  455,  462,  469,  477,  483,  491,  497,  505,  512,  518,  526,  533,  541,  547,  555,  562,  568,  577,  583,  592,  598,  607,  613,  622,  629,  635,  644,  651,  659,  666,  675,  682,  689,  697,  704,  713,  720,  729,  736,  745,  752,  759,  768,  775,  784,  792,  801,  808,  816,  825,  832,  841,  849,  858,  866,
    875,   883,   890,   900,   908,   917,   925,   935,   942,   950,   960,   968,   978,   986,   996,   1004,  1014,  1022,  1030,  1040,  1048,  1058,  1067,  1077,  1085,  1094,  1104,  1112,  1123,  1131,  1142,  1151,  1161,  1170, 1179, 1190, 1198, 1209, 1218, 1229, 1238, 1247, 1258, 1267, 1278, 1288, 1299, 1308, 1320, 1329, 1338, 1350, 1359, 1371, 1381, 1392, 1402, 1412, 1424, 1433, 1446, 1455, 1468, 1478, 1490, 1500, 1510, 1523, 1533, 1546, 1556, 1569, 1579, 1590, 1603, 1613, 1626, 1637, 1651, 1661, 1675, 1686, 1697, 1710, 1721, 1735, 1746, 1761, 1772, 1783, 1797, 1809, 1823, 1835, 1850, 1861, 1876, 1888, 1900, 1915, 1927, 1943, 1955, 1970, 1983, 1995, 2011, 2024, 2040, 2052, 2069, 2082, 2098, 2111, 2124, 2141, 2154, 2171, 2185, 2202, 2216, 2230, 2247, 2261, 2279, 2293, 2311, 2326,
};

void audio_spatial_pitch(uint32 voice, const AUDIO_SPATIAL *state)
{
    uint32 value, numerator, high, index, note, offset;
    SOUND_VOICE *target;
    FUNCTION_MARKER(0x8002EC48, "ACE3.BIN");
    if (voice == 255)
        return;
    target = sound_voice_at(voice);
    value = target->flags;
    target->flags = (uint8)(value | 0x10);
    numerator = (((state->speed)) + 0x470) << 9;
    high = (((sint64)(sint32)numerator * (sint32)0xE6C2B449) >> 32);
    index = (uint32)((sint32)(high + numerator) >> 11) - (uint32)((sint32)numerator >> 31);
    if ((sint32)index < 0)
        index = 0;
    if ((sint32)index >= 512)
        index = 511;
    note = (sint16)target->base_note;
    offset = sound_doppler_offsets[index];
    target->note = (uint16)(sound_offset_note(note, offset));
    {
        (void)(uint32)(voice);
        return;
    }
}

void audio_spatial_crossfade(uint32 voice, uint32 other_voice, const AUDIO_SPATIAL *state)
{
    uint32 value, sign, phase, cosine, sine, product, left, right, raw_left, raw_right;
    SOUND_VOICE *target;
    SOUND_VOICE *other;
    sint32 old_left, angle;
    FUNCTION_MARKER(0x8002EA74, "ACE3.BIN");
    if (voice == 255 || other_voice == 255)
        return;
    target = sound_voice_at(voice);
    value = target->flags;
    target->flags = (uint8)(value | 0x10);
    other = sound_voice_at(other_voice);
    value = other->flags;
    old_left = (sint16)other->volume[0];
    other->flags = (uint8)(value | 0x10);
    value = target->note;
    other->note = (uint16)(value);
    if (old_left < 0)
        sign = 1;
    else
        sign = (other->volume[1] >> 14) & 2;
    angle = ((state->dir));
    if (angle > 1024)
    {
        other->volume[0] = (uint16)(0);
        other->volume[1] = (uint16)(0);
    }
    else if (angle < -1024)
    {
        value = target->volume[0];
        other->volume[0] = (uint16)(value);
        value = target->volume[1];
        other->volume[1] = (uint16)(value);
        target->volume[0] = (uint16)(0);
        target->volume[1] = (uint16)(0);
    }
    else
    {
        phase = (((uint32)angle + 1024) << 1) & 0x3FFF;
        left = (sint16)target->volume[0];
        cosine = game_wave_at((phase + 0x1000) * 2);
        product = left * cosine;
        other->volume[0] = (uint16)((uint32)((sint32)product >> 14));
        product = (uint32)(sint16)target->volume[1] * cosine;
        sine = game_wave_at(phase * 2);
        other->volume[1] = (uint16)((uint32)((sint32)product >> 14));
        product = (uint32)(sint16)target->volume[0] * sine;
        value = (sint16)target->volume[1];
        right = value * sine;
        target->volume[0] = (uint16)((uint32)((sint32)product >> 14));
        target->volume[1] = (uint16)((uint32)((sint32)right >> 14));
    }
    if (sign == 1)
    {
        value = other->volume[0];
        other->volume[0] = (uint16)(0 - value);
    }
    else if (sign == 2)
    {
        value = other->volume[1];
        other->volume[1] = (uint16)(0 - value);
    }
    else
    {
        left = (sint16)other->volume[0];
        right = (sint16)other->volume[1];
        raw_left = other->volume[0];
        raw_right = other->volume[1];
        if ((sint32)right < (sint32)left)
            other->volume[1] = (uint16)(0 - raw_right);
        else
            other->volume[0] = (uint16)(0 - raw_left);
    }
    {
        (void)(uint32)(voice);
        return;
    }
}

void sound_reset_slot(uint32 slot)
{
    SOUND_DELAYED *ptr;

    FUNCTION_MARKER(0x8002F178, "ACE3.BIN");
    ptr = &sound_schedule.delayed[slot];
    ptr->sound = 255;
    ptr->ticks = 0xFFFF;
    return;
}

void sound_init_slots(void)
{
    uint32 i;

    FUNCTION_MARKER(0x8002F140, "ACE3.BIN");
    for (i = 0; i < 8; ++i)
        sound_reset_slot(i);
    return;
}

void sound_schedule_delayed(uint32 kind, uint32 value)
{
    SOUND_DELAYED *slot;
    uint32 index;
    FUNCTION_MARKER(0x8002F19C, "ACE3.BIN");
    slot = sound_schedule.delayed;
    for (index = 0; index < 8; ++index, ++slot)
    {
        if ((sint16)slot->ticks == -1)
        {
            slot->sound = kind;
            slot->ticks = value;
            {
                (void)(uint32)(index);
                return;
            }
        }
    }
    return;
}

void sound_update_slots(void)
{
    uint32 i;
    SOUND_DELAYED *slot;
    uint32 timer;
    uint32 next;
    sint32 signed_timer;

    FUNCTION_MARKER(0x8002F1E0, "ACE3.BIN");
    slot = sound_schedule.delayed;
    for (i = 0; i < 8; ++i)
    {
        signed_timer = (sint16)slot->ticks;
        timer = slot->ticks;
        if (signed_timer >= 0)
        {
            next = timer - 1;
            slot->ticks = next;
            if (!(next << 16))
            {
                sound_play(slot->sound);
                sound_reset_slot(i);
            }
        }
        ++slot;
    }
    return;
}

void sound_init_queue(void)
{
    uint32 i;
    SOUND_QUEUED *slot;

    FUNCTION_MARKER(0x8002F254, "ACE3.BIN");
    sound_schedule.head = 0;
    sound_schedule.tail = 0;
    slot = sound_schedule.queued;
    for (i = 0; i < 32; ++i)
    {
        slot->sound = 255;
        slot->voice = 255;
        slot->ticks = 0xFFFF;
        ++slot;
    }
    return;
}

void sound_queue_push(uint32 value, uint32 kind)
{
    uint32 index, result;
    SOUND_QUEUED *entry;
    FUNCTION_MARKER(0x8002F294, "ACE3.BIN");
    index = sound_schedule.tail;
    entry = &sound_schedule.queued[index];
    entry->ticks = value;
    entry->sound = kind;
    index = sound_schedule.tail;
    result = (index + 1) & 31;
    sound_schedule.tail = result;
    {
        (void)(uint32)(result);
        return;
    }
}

void sound_update_queue(void)
{
    uint32 head;
    SOUND_QUEUED *slot;
    uint32 voice;
    uint32 result;
    sint32 timer;

    FUNCTION_MARKER(0x8002F2CC, "ACE3.BIN");
    head = sound_schedule.head;
    slot = &sound_schedule.queued[head];
    voice = slot->voice;
    if (voice != 255)
    {
        result = sound_voice_at(voice)->active;
        if (!result)
        {
            slot->voice = 255;
            slot->sound = 255;
            slot->ticks = 0xFFFF;
            result = (sound_schedule.head + 1) & 31;
            sound_schedule.head = result;
        }
        {
            (void)(uint32)(result);
            return;
        }
    }
    timer = (sint16)slot->ticks;
    if (timer == -1)
    {
        sound_schedule.tail = head;
        return;
    }
    result = ~game_runtime.flags & 0x20000000;
    if (result)
    {
        if (!timer)
            slot->voice = sound_play(slot->sound);
        result = slot->ticks - 1;
        slot->ticks = result;
    }
    {
        (void)(uint32)(result);
        return;
    }
}

void sound_load_bank(SOUND_BANK_INPUT bank)
{
    uint32 src;
    uint32 dst;
    uint32 i;
    SOUND_SAMPLE *sample;
    uint32 address;
    uint32 bytes;
    sint32 offset;
    sint64 source_offset;

    FUNCTION_MARKER(0x8002D96C, "ACE3.BIN");
    i = 0;
    if (!bank.data || bank.offset > bank.bytes || 12 > bank.bytes - bank.offset)
        abort();
    src = bank.offset + 12;
    const SOUND_BANK_HEADER *prefix = (const SOUND_BANK_HEADER *)(bank.data + bank.offset);
    if (prefix->sample_count)
    {
        do
        {
            dst = prefix->first_sample + i;
            if (dst >= SOUND_ID_COUNT)
                abort();
            sample = &sound_samples[dst];
            if (src > bank.bytes || 20 > bank.bytes - src)
                abort();
            const SOUND_BANK_SAMPLE *record = (const SOUND_BANK_SAMPLE *)(bank.data + src);
            sample->voice = record->voice;
            if (sample->voice >= 24 && sample->voice != 255)
                abort();
            sample->volume[0] = record->volume[0];
            sample->volume[1] = record->volume[1];
            sample->note = record->note;
            sample->sample_note = record->sample_note;
            sample->adsr[0] = record->adsr[0];
            sample->adsr[1] = record->adsr[1];
            sample->address = record->address;
            ++i;
            src += 20;
        } while (i < prefix->sample_count);
    }
    offset = prefix->source_word_offset;
    address = prefix->spu_address;
    bytes = prefix->transfer_bytes;
    source_offset = (sint64)bank.offset + (sint64)offset * 4;
    if (source_offset < 0 || source_offset > bank.bytes)
        abort();
    {
        (void)(uint32)(sound_upload(address, bank.data + (size_t)source_offset, bytes, bank.bytes - (uint32)source_offset));
        return;
    }
}

sint32 sound_init_reverb(void)
{
    FUNCTION_MARKER(0x8002DA20, "ACE3.BIN");
    sound_reverb_attr.mask = 1;
    sound_reverb_attr.mode = 3;
    sound_reverb_attr.depth.left = 0x1999;
    sound_reverb_attr.depth.right = 0x1999;
    return SpuSetReverbModeParam(&sound_reverb_attr);
}

void sound_enable_reverb(void)
{
    FUNCTION_MARKER(0x8002DA94, "ACE3.BIN");
    SpuClearReverbWorkArea(sound_reverb_attr.mode);
    sound_reverb_attr.mask = 6;
    SpuSetReverbDepth(&sound_reverb_attr);
    SpuSetReverb(1);
    {
        (void)(uint32)(SpuSetReverbVoice(1, 0xFFFFFF));
        return;
    }
}

sint32 sound_disable_reverb(void)
{
    SpuReverbAttr attr = {0};

    FUNCTION_MARKER(0x8002DAF4, "ACE3.BIN");
    attr.mask = 6;
    attr.depth.left = 0;
    attr.depth.right = 0;
    SpuSetReverbDepth(&attr);
    SpuSetReverb(0);
    SpuSetReverbVoice(0, 0xFFFFFF);
    return SpuClearReverbWorkArea(sound_reverb_attr.mode);
}

uint32 sound_reset_reverb_mode(void)
{
    SpuReverbAttr reverb = {0};
    FUNCTION_MARKER(0x8002DA60, "ACE3.BIN");
    reverb.mask = 1;
    reverb.mode = 0;
    SpuSetReverbModeParam(&reverb);
    return SpuSetReverb(0);
}

uint32 sound_play(uint32 sound)
{
    uint32 index;

    FUNCTION_MARKER(0x8002DB4C, "ACE3.BIN");
    if (sound - 46 < 4)
        sound_play_reuse(sound);
    index = sound_sample_at(sound)->voice;
    if (index == 255)
    {
        index = sound_reserve_voice();
        if (index == 255)
            return 255;
    }
    sound_start_voice(sound, index);
    return index;
}

uint32 sound_play_reuse(uint32 sound)
{
    uint32 index;

    FUNCTION_MARKER(0x8002DBE0, "ACE3.BIN");
    index = sound_sample_at(sound)->voice;
    if (index == 255)
    {
        index = sound_find_voice(sound);
        if (index == 255)
        {
            index = sound_reserve_voice();
            if (index == 255)
                return 255;
        }
    }
    sound_start_voice(sound, index);
    return index;
}

void sound_play_on_voice(uint32 sound, uint32 voice)
{
    FUNCTION_MARKER(0x8002DC7C, "ACE3.BIN");
    if (voice == 255)
        return;
    sound_start_voice(sound, voice);
    {
        (void)(uint32)(voice);
        return;
    }
}

uint32 sound_play_allocated(uint32 sound)
{
    uint32 voice;
    FUNCTION_MARKER(0x8002DCB0, "ACE3.BIN");
    voice = sound_reserve_voice();
    if (voice == 255)
        return 255;
    sound_start_voice(sound, voice);
    return voice;
}

uint32 sound_find_voice(uint32 sound)
{
    uint32 i;
    SOUND_VOICE *voice = &sound_voices[12];

    FUNCTION_MARKER(0x8002DCF8, "ACE3.BIN");
    for (i = 12; i < 24; ++i, ++voice)
        if (voice->sound == sound)
            return i;
    return 255;
}

uint32 sound_reserve_voice(void)
{
    uint32 i;
    SOUND_VOICE *voice = &sound_voices[12];

    FUNCTION_MARKER(0x8002DD38, "ACE3.BIN");
    for (i = 12; i < 24; ++i, ++voice)
    {
        if (!voice->active)
        {
            voice->active = (uint8)(2);
            return i;
        }
    }
    return 255;
}

uint32 sound_voice_status(uint32 voice)
{
    FUNCTION_MARKER(0x8002DD7C, "ACE3.BIN");
    if (voice == 255)
        return 255;
    return sound_voice_at(voice)->active;
}

uint32 sound_stop_voice(uint32 voice)
{
    FUNCTION_MARKER(0x8002DDA8, "ACE3.BIN");
    if (voice >= 24)
        return 255;
    sound_voice_at(voice)->flags = 2;
    return voice;
}

void sound_stop_all(void)
{
    uint32 i;

    FUNCTION_MARKER(0x8002DDE0, "ACE3.BIN");
    for (i = 0; i < 24; ++i)
        sound_reset_voice(i);
    SpuSetKey(0, 0xFFFFFF);
}

uint32 sound_set_voice_note(uint32 index, uint32 note)
{
    SOUND_VOICE *voice;
    uint32 flags;

    FUNCTION_MARKER(0x8002DE24, "ACE3.BIN");
    if (index >= 24)
        return 255;
    voice = sound_voice_at(index);
    flags = voice->flags;
    voice->note = (uint16)(note);
    voice->flags = (uint8)(flags | 0x10);
    return index;
}

uint32 sound_get_note(uint32 sound)
{
    FUNCTION_MARKER(0x8002DE68, "ACE3.BIN");
    return (sint16)sound_sample_at(sound)->note;
}

void sound_set_voice_volume(uint32 voice, uint32 left, uint32 right)
{
    uint32 flags;
    SOUND_VOICE *target;
    FUNCTION_MARKER(0x8002DEC8, "ACE3.BIN");
    if (voice >= 24)
        return;
    if ((sint16)left >= 0x4000)
        left = 0x3FFF;
    if ((sint16)right >= 0x4000)
        right = 0x3FFF;
    target = sound_voice_at(voice);
    flags = target->flags;
    target->volume[0] = (uint16)(left);
    target->volume[1] = (uint16)(right);
    target->flags = (uint8)(flags | 0x10);
    {
        (void)(uint32)(voice);
        return;
    }
}

uint32 sound_set_voice_fade(uint32 index, uint32 percent, uint32 steps)
{
    SOUND_VOICE *voice;
    sint32 scaled;
    sint32 result;

    FUNCTION_MARKER(0x8002DF40, "ACE3.BIN");
    if (index >= 24)
        return 255;
    scaled = (percent << 7);
    voice = sound_voice_at(index);
    voice->fade_ticks = (uint16)(steps);
    result = scaled / 100;
    voice->fade_target = (uint8)(result);
    return result;
}

void sound_publish_voice(uint32 index)
{
    SpuVoiceAttr attr = {0};
    SOUND_VOICE *voice;
    sint32 volume;
    sint32 left;
    sint32 right;
    sint32 value;
    uint32 fade;

    FUNCTION_MARKER(0x8002DF98, "ACE3.BIN");
    volume = audio_settings.volume[0];
    attr.voice = (uint32)1 << (index & 31);
    attr.mask = 0x23;
    voice = sound_voice_at(index);
    if (audio_settings.flags & 1)
    {
        left = (sint16)voice->volume[0];
        right = (sint16)voice->volume[1];
        if (left < 0)
            left = -left;
        if (right < 0)
            right = -right;
        value = (volume * (left + right)) >> 9;
        fade = voice->fade_level;
        value = (value * (sint32)fade) >> 7;
        attr.volume.left = (sint16)value;
        attr.volume.right = (sint16)value;
    }
    else
    {
        value = (sint16)voice->volume[0] * volume;
        fade = voice->fade_level;
        value >>= 8;
        if (fade != 128)
            value = (value * voice->fade_level) >> 7;
        attr.volume.left = (sint16)value;
        value = (sint16)voice->volume[1] * volume;
        fade = voice->fade_level;
        value >>= 8;
        if (fade != 128)
            value = (value * voice->fade_level) >> 7;
        attr.volume.right = (sint16)value;
    }
    attr.note = voice->note;
    SpuSetVoiceAttr(&attr);
}

void sound_publish_key_on(uint32 index)
{
    SpuVoiceAttr attr = {0};
    SOUND_VOICE *voice;
    uint32 sound;
    const SOUND_SAMPLE *descriptor;
    sint32 volume;
    sint32 left;
    sint32 right;

    FUNCTION_MARKER(0x8002E0D0, "ACE3.BIN");
    voice = sound_voice_at(index);
    sound = voice->sound;
    volume = audio_settings.volume[0];
    attr.voice = (uint32)1 << (index & 31);
    attr.mask = 0x600E3;
    descriptor = sound_sample_at(sound);
    if (audio_settings.flags & 1)
    {
        left = (sint16)voice->volume[0];
        right = (sint16)voice->volume[1];
        if (left < 0)
            left = -left;
        if (right < 0)
            right = -right;
        attr.volume.left = (sint16)((volume * (left + right)) >> 9);
        attr.volume.right = attr.volume.left;
    }
    else
    {
        attr.volume.left = (sint16)(((sint16)voice->volume[0] * volume) >> 8);
        attr.volume.right = (sint16)(((sint16)voice->volume[1] * volume) >> 8);
    }
    attr.note = voice->note;
    attr.sample_note = descriptor->sample_note;
    attr.addr = descriptor->address;
    attr.adsr1 = descriptor->adsr[0];
    attr.adsr2 = descriptor->adsr[1];
    SpuSetVoiceAttr(&attr);
}

void sound_update(void)
{
    uint8 key_status[24];
    uint32 id;
    SOUND_VOICE *voice;
    uint32 flags;
    uint32 key_on = 0;
    uint32 key_off = 0;
    uint32 i;
    uint32 volume;
    uint32 cur;
    uint32 timer;
    sint32 steps;
    sint32 remaining;
    sint32 delta;

    FUNCTION_MARKER(0x8002E1F4, "ACE3.BIN");
    id = (uint32)sound_get_cur_resource() & 0x7FF;
    if (id == 0x1E6 || id - 0x23 < 0x24)
        volume = audio_settings.volume[1];
    else
        volume = audio_settings.volume[0];
    sound_set_cd_volume(volume);
    sound_set_master_volume((game_runtime.flags & 0x20000000) ? 0 : 0x3FFF);
    if (!(game_runtime.flags & 0x20000000))
        sound_update_slots();
    sound_update_queue();
    voice = sound_voices;
    for (i = 0; i < 24; ++i)
    {
        flags = voice->flags;
        if (flags & 1)
        {
            key_on |= 1 << i;
            voice->flags = (uint8)(flags & 0xFE);
            sound_publish_key_on(i);
            flags = voice->flags;
            if (flags & 0x10)
                voice->flags = (uint8)(flags & 0xEF);
        }
        steps = (sint16)voice->fade_ticks;
        if (steps > 0)
        {
            volume = voice->fade_target;
            cur = voice->fade_level;
            delta = ((sint32)volume - (sint32)cur) / steps;
            remaining = (sint16)voice->fade_ticks;
            voice->fade_level = (uint8)(cur + (uint32)delta);
            timer = voice->fade_ticks;
            if (remaining > 0)
                voice->fade_ticks = (uint16)(timer - 1);
        }
        else
            voice->fade_level = (uint8)(voice->fade_target);
        if (voice->fade_level != 128)
            voice->flags = (uint8)(voice->flags | 0x10);
        if (voice->flags & 0x10)
        {
            sound_publish_voice(i);
            voice->flags = (uint8)(voice->flags & 0xEF);
        }
        ++voice;
    }
    SpuSetKey(1, key_on);
    SpuGetAllKeysStatus(key_status);
    voice = sound_voices;
    for (i = 0; i < 24; ++i)
    {
        if (key_status[i] == 1)
            voice->key_delay = (uint8)(8);
        else if (key_status[i] != 2)
        {
            if (!key_status[i])
            {
                if (voice->active)
                    voice->flags = (uint8)(4);
            }
            else
            {
                timer = voice->key_delay;
                if (!timer)
                    sound_stop_voice(i);
                else
                {
                    --timer;
                    voice->key_delay = (uint8)(timer);
                    if (!(timer & 255))
                        sound_stop_voice(i);
                }
            }
        }
        ++voice;
    }
    voice = sound_voices;
    for (i = 0; i < 24; ++i)
    {
        if (voice->flags & 4)
            sound_reset_voice(i);
        ++voice;
    }
    voice = sound_voices;
    for (i = 0; i < 24; ++i)
    {
        if (voice->flags & 2)
            key_off |= 1 << i;
        ++voice;
    }
    SpuSetKey(0, key_off);
}

void sound_start_voice(uint32 sound, uint32 index)
{
    SOUND_VOICE *voice;
    const SOUND_SAMPLE *descriptor;
    uint32 value;

    FUNCTION_MARKER(0x8002E514, "ACE3.BIN");
    if (index == 255)
        return;
    voice = sound_voice_at(index);
    voice->flags = (uint8)(1);
    voice->active = (uint8)(1);
    voice->key_delay = (uint8)(16);
    descriptor = sound_sample_at(sound);
    voice->sound = (uint8)(sound);
    value = descriptor->volume[0];
    voice->volume[0] = (uint16)(value);
    voice->base_volume[0] = (uint16)(value);
    value = descriptor->volume[1];
    voice->volume[1] = (uint16)(value);
    voice->base_volume[1] = (uint16)(value);
    value = descriptor->note;
    voice->fade_target = (uint8)(128);
    voice->fade_level = (uint8)(128);
    voice->fade_ticks = (uint16)(0);
    voice->note = (uint16)(value);
    voice->base_note = (uint16)(value);
    {
        (void)(uint32)(index);
        return;
    }
}

void sound_reset_voice(uint32 index)
{
    SOUND_VOICE *voice;

    FUNCTION_MARKER(0x8002E5B4, "ACE3.BIN");
    if (index == 255)
        return;
    voice = sound_voice_at(index);
    voice->sound = (uint8)(255);
    voice->flags = (uint8)(0);
    voice->key_delay = (uint8)(0);
    voice->fade_target = (uint8)(128);
    voice->fade_level = (uint8)(128);
    voice->fade_ticks = (uint16)(0);
    voice->active = (uint8)(0);
    {
        (void)(uint32)(index);
        return;
    }
}

void sound_init_voices(void)
{
    uint32 i;
    SOUND_VOICE *voice;

    FUNCTION_MARKER(0x8002E604, "ACE3.BIN");
    voice = sound_voices;
    for (i = 0; i < 24; ++i)
    {
        voice->sound = (uint8)(255);
        voice->flags = (uint8)(0);
        voice->key_delay = (uint8)(0);
        voice->active = (uint8)(0);
        voice->fade_target = (uint8)(128);
        voice->fade_level = (uint8)(128);
        voice->fade_ticks = (uint16)(0);
        ++voice;
    }
    return;
}

uint32 sound_offset_note(uint32 note, uint32 offset)
{
    uint32 value;
    FUNCTION_MARKER(0x8002E64C, "ACE3.BIN");
    offset += ((note & 0xFF00) >> 1) | (note & 0x7F);
    value = ((offset & 0x7F80) << 1) | (offset & 0x7F);
    return (sint16)value;
}

void sound_restore_voice_defaults(uint32 voice)
{
    uint32 value;
    SOUND_VOICE *target;
    FUNCTION_MARKER(0x8002E67C, "ACE3.BIN");
    if (voice == 255)
        return;
    target = sound_voice_at(voice);
    value = target->flags;
    target->flags = (uint8)(value | 0x10);
    value = ((uint32)target->base_volume[0] | ((uint32)target->base_volume[1] << 16));
    target->volume[0] = (uint16)value;
    target->volume[1] = (uint16)(value >> 16);
    value = target->base_note;
    target->note = (uint16)(value);
    {
        (void)(uint32)(voice);
        return;
    }
}

void sound_apply_voice_dir(uint32 voice, uint32 direction)
{
    uint32 value;
    SOUND_VOICE *state;
    FUNCTION_MARKER(0x8002EA24, "ACE3.BIN");
    if (voice == 255)
        return;
    state = sound_voice_at(voice);
    value = state->flags;
    state->flags = (uint8)(value | 0x10);
    if ((sint32)direction < 0)
    {
        value = state->volume[0];
        state->volume[0] = (uint16)(0 - value);
    }
    {
        (void)(uint32)(voice);
        return;
    }
}

uint32 sound_offset_voice_note(uint32 voice, uint32 offset)
{
    uint32 flags, note;
    SOUND_VOICE *target;
    FUNCTION_MARKER(0x8002F0D4, "ACE3.BIN");
    if (voice == 255)
        return 255;
    target = sound_voice_at(voice);
    offset = (sint16)offset;
    flags = target->flags;
    note = (sint16)target->note;
    target->flags = (uint8)(flags | 0x10);
    target->note = (uint16)(sound_offset_note(note, offset));
    return voice;
}

static const uint8 prompt_kind_offsets[7] = {255, 2, 1, 0, 1, 3, 0};
static const uint8 prompt_sound_codes[4] = {109, 110, 115, 116};

static const uint8 prompt_mission_speakers[GAME_MISSION_COUNT][11] = {
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {3, 5, 4, 5, 3, 3, 4, 0, 5, 4, 4}, {3, 5, 3, 4, 4, 3, 0, 0, 5, 3, 3}, {0, 1, 0, 0, 2, 0, 0, 0, 0, 0, 0}, {2, 5, 3, 1, 0, 3, 2, 1, 5, 5, 5}, {3, 5, 3, 5, 5, 3, 0, 0, 5, 3, 3}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {4, 0, 4, 0, 4, 0, 4, 0, 0, 4, 4}, {5, 3, 5, 3, 5, 3, 0, 0, 5, 5, 5}, {0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {4, 4, 4, 0, 4, 0, 4, 0, 0, 4, 4}, {4, 3, 3, 4, 3, 3, 4, 0, 0, 4, 4}, {4, 3, 3, 3, 4, 3, 4, 0, 0, 3, 3}, {4, 4, 4, 3, 3, 3, 4, 0, 0, 4, 4}, {4, 4, 4, 4, 4, 0, 4, 0, 0, 4, 4}, {3, 4, 3, 4, 3, 3, 4, 0, 0, 3, 3}, {3, 3, 3, 3, 3, 3, 0, 0, 0, 3, 3}, {3, 3, 3, 3, 3, 3, 0, 0, 0, 3, 3}, {3, 3, 3, 3, 3, 3, 0, 0, 0, 3, 3}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {2, 1, 2, 1, 2, 0, 2, 1, 0, 1, 1}, {1, 2, 2, 1, 1, 0, 2, 1, 0, 2, 2}, {1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1}, {2, 2, 2, 2, 2, 0, 2, 0, 0, 2, 2}, {1, 2, 1, 0, 1, 0, 2, 1, 0, 1, 1}, {1, 0, 1, 0, 0, 0, 2, 1, 0, 1, 1}, {2, 2, 2, 2, 2, 0, 2, 0, 0, 2, 2}, {1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1}, {2, 2, 2, 2, 2, 0, 2, 0, 0, 2, 2}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {2, 2, 2, 2, 2, 0, 2, 0, 0, 2, 2}, {2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {2, 2, 2, 2, 2, 0, 2, 0, 0, 2, 2}, {2, 0, 2, 2, 2, 0, 2, 0, 0, 2, 2}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {6, 0, 0, 5, 0, 6, 0, 0, 5, 0, 0}, {5, 0, 6, 5, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {5, 6, 5, 6, 6, 6, 0, 0, 5, 5, 5}, {0, 0, 0, 5, 6, 6, 0, 0, 0, 0, 6}, {6, 0, 6, 0, 6, 0, 0, 0, 0, 6, 6}, {6, 0, 6, 0, 6, 6, 0, 0, 0, 0, 0}, {6, 6, 6, 6, 6, 6, 0, 0, 0, 6, 6}, {6, 6, 6, 6, 6, 6, 0, 0, 0, 6, 6}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {5, 5, 0, 5, 0, 0, 0, 0, 5, 5, 5}, {5, 5, 5, 5, 5, 0, 0, 0, 5, 5, 5}, {5, 5, 5, 5, 5, 0, 0, 0, 5, 5, 5}, {0, 5, 0, 5, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
};

static const uint8 prompt_speaker_labels[7] = {0, 4, 7, 8, 1, 2, 5};

static const uint32 prompt_permissions[9] = {
    0x11111100, 0x01010100, 0x11111110, 0x01010100, 0x10101010, 0x01010101, 0x01010101, 0x00010000, 0x10101010,
};

uint32 mission_prompt_permission(uint32 id, uint32 mission, uint32 kind)
{
    uint32 slot, value, mask;
    FUNCTION_MARKER(0x8004925C, "ACE3.BIN");
    if ((sint32)mission < 31)
        return 0;
    switch (mission)
    {
        case 31:
            slot = 0;
            break;
        case 32:
            slot = 1;
            break;
        case 33:
            slot = 2;
            break;
        case 38:
            slot = 3;
            break;
        case 39:
            slot = 4;
            break;
        case 47:
            slot = 5;
            break;
        case 48:
            slot = 6;
            break;
        case 53:
            slot = 7;
            break;
        case 58:
            slot = 8;
            break;
        default:
            return 0;
    }
    if (id == 1)
    {
        switch (kind)
        {
            case 6:
                mask = 0x10000000;
                break;
            case 7:
                mask = 0x100000;
                break;
            case 8:
                mask = 0x1000;
                break;
            case 9:
                mask = 0x10;
                break;
            default:
                return 0;
        }
    }
    else if (id == 4)
    {
        switch (kind)
        {
            case 6:
                mask = 0x1000000;
                break;
            case 7:
                mask = 0x10000;
                break;
            case 8:
                mask = 0x100;
                break;
            case 9:
                mask = 1;
                break;
            default:
                return 0;
        }
    }
    else
        return 0;
    value = prompt_permissions[slot];
    return value & mask;
}

void mission_prompt_request(uint32 kind, uint32 value, uint32 id, uint32 code)
{
    uint32 flags, mission, tick, offset, bit, entry, mask;
    FUNCTION_MARKER(0x80048C0C, "ACE3.BIN");
    flags = game_runtime.mission_control.mode;
    mission = (sint16)game_runtime.selection.mission;
    if ((flags & 4) == 0)
        return;
    if ((sint16)mission_prompt.phase != 1)
        return;
    if (kind < 10)
    {
        tick = game_runtime.mission_control.ticks;
        mask = kind == 8 ? 15 : kind == 1 || kind == 2 || kind == 3 || kind == 7 ? 3 : 7;
        if ((tick & mask) != 0)
            return;
        if (kind <= 2)
        {
            flags = mission_prompt.played_mask;
            if (kind == 0)
            {
                bit = 0x10;
                offset = 0;
                code = 0x68;
                if ((flags & bit) != 0)
                {
                    bit = 0x20;
                    offset = 1;
                    code = 0x69;
                }
            }
            else if (kind == 1)
            {
                bit = 0x40;
                offset = 2;
                code = 0x6A;
                if ((flags & bit) != 0)
                {
                    bit = 0x80;
                    offset = 3;
                    code = 0x6B;
                }
            }
            else
            {
                bit = 0x100;
                offset = 4;
                code = 0x6C;
            }
            if ((flags & bit) != 0)
                return;
            if (mission >= GAME_MISSION_COUNT)
                abort();
            id = prompt_mission_speakers[mission][offset];
            if (id == 0)
                return;
            mission_prompt.played_mask = flags | bit;
        }
        else if (kind == 3)
        {
            entry = id & 255;
            if (entry >= 7)
                return;
            offset = prompt_kind_offsets[entry];
            if (offset == 255)
                return;
            if (mission >= GAME_MISSION_COUNT)
                abort();
            mask = prompt_mission_speakers[mission][offset];
            if (entry != mask)
                return;
            flags = mission_prompt.played_mask;
            bit = 1u << (offset & 31);
            if ((flags & bit) != 0)
                return;
            code = prompt_sound_codes[offset];
            mission_prompt.played_mask = flags | bit;
        }
        else if (kind == 4 || kind == 5)
        {
            flags = mission_prompt.played_mask;
            bit = kind == 4 ? 0x200 : 0x400;
            if ((flags & bit) != 0)
                return;
            if (mission >= GAME_MISSION_COUNT)
                abort();
            entry = prompt_mission_speakers[mission][kind == 4 ? 9 : 10];
            if ((id & 255) != entry)
                return;
            mission_prompt.played_mask = flags | bit;
            code = kind == 4 ? 0x75 : 0x76;
        }
        else
        {
            entry = id & 255;
            if (mission_prompt_permission(entry, mission, kind) == 0)
                return;
            if (entry != 1 && entry != 4)
                return;
            flags = mission_prompt.played_mask;
            offset = (kind - 6) * 2 + (entry == 4);
            bit = 1u << (11 + offset);
            code = 0x78 + offset;
            if ((flags & bit) != 0)
                return;
            mission_prompt.played_mask = flags | bit;
        }
    }
    mission_prompt.sound = code;
    mission_prompt.ticks = value;
    mission_prompt.speaker = id;
    return;
}

void mission_prompt_init(void)
{
    FUNCTION_MARKER(0x80048BDC, "ACE3.BIN");
    mission_prompt.phase = 1;
    mission_prompt.played_mask = 0;
    mission_prompt.sound = 255;
    mission_prompt.ticks = 65535;
    mission_prompt.voice = 255;
    mission_prompt.speaker = 0;
    return;
}

void mission_prompt_update(void)
{
    uint32 result, mode, value;
    FUNCTION_MARKER(0x80049438, "ACE3.BIN");
    if ((sint16)mission_prompt.phase == 1 && (sint16)mission_prompt.ticks >= 0)
    {
        result = speech_queue_mode();
        if (result != 0 || (flight_dynamics_flags() & 0x7000) != 0)
            mission_prompt.ticks = 0xFFFF;
    }
    if ((sint16)mission_prompt.phase == 2)
    {
        result = speech_queue_mode();
        mode = 2;
        if (result != 0)
        {
            sound_stop_voice(mission_prompt.voice);
            mode = 0;
        }
        else
        {
            result = sound_voice_status(mission_prompt.voice);
            if (result == 0 || mission_prompt.voice == 255)
                mode = 1;
        }
        if (mode != 2)
        {
            hud_portrait_hide(mode);
            mission_prompt.phase = 1;
            mission_prompt.ticks = 0xFFFF;
            mission_prompt.voice = 255;
        }
    }
    if ((sint16)mission_prompt.ticks == 0 && (game_runtime.mission_control.mode & 4) != 0 && (sint16)mission_prompt.phase == 1)
    {
        result = sound_play(mission_prompt.sound);
        mission_prompt.voice = result;
        value = mission_prompt.speaker;
        if (value >= 7)
            abort();
        value = prompt_speaker_labels[value];
        hud_portrait_show(value, 268, 30);
        mission_prompt.phase = 2;
        mission_prompt.ticks = 0xFFFF;
    }
    if ((sint16)mission_prompt.ticks == 12 && (sint16)mission_prompt.phase == 1 && speech_queue_mode() == 0)
        sound_play(53);
    result = (sint16)mission_prompt.ticks;
    value = mission_prompt.ticks;
    if ((sint32)result >= 0)
    {
        result = value - 1;
        mission_prompt.ticks = result;
    }
    {
        (void)(uint32)(result);
        return;
    }
}

uint32 sound_script_init(void)
{
    FUNCTION_MARKER(0x800566F0, "ACE3.BIN");
    arena_alloc(16);
    return sound_script_start(0, 0);
}

static uint32 sound_script_start(const uint8 *data, uint32 bytes)
{
    FUNCTION_MARKER(0x8005671C, "ACE3.BIN");
    if (data)
    {
        if (!speech_queue.catalog.data || data < speech_queue.catalog.data || data > speech_queue.catalog.end || bytes > (uint32)(speech_queue.catalog.end - data) || bytes < 2 || ((intptr)data & 3))
            abort();
    }
    else if (bytes)
        abort();
    sound_script.voice = 255;
    sound_script.sound = 255;
    sound_script.cursor = data;
    sound_script.end = data ? data + bytes : 0;
    sound_script.ticks = 0;
    sound_script.control[1] = 0;
    sound_script.control[0] = 0;
    speech_queue.mode = 2;
    sound_script.speaker = 0;
    return subtitles_configure(0, 0, 0);
}

typedef struct
{
    sint16 tick;
    uint8 type;
    uint8 payload;
} SOUND_COMMAND;

typedef char SOUND_COMMAND_LAYOUT_CHECK[sizeof(SOUND_COMMAND) == 4 && offsetof(SOUND_COMMAND, type) == 2 ? 1 : -1];

uint32 sound_script_update(void)
{
    uint32 value, stamp, timer, type, payload, selector, voice;
    const uint8 *stream, *cursor;
    const SOUND_COMMAND *command;
    FUNCTION_MARKER(0x80056778, "ACE3.BIN");
    stream = sound_script.cursor;
    if (stream == 0)
    {
        voice = sound_script.voice;
        if (voice != 255 && sound_voice_status(voice) == 0)
        {
            subtitles_configure(255, 0, 0);
            sound_script.voice = 255;
            hud_portrait_hide(1);
        }
        return 0;
    }
    if (stream > sound_script.end || (uint32)(sound_script.end - stream) < 2)
        abort();
    stamp = *(const sint16 *)stream;
    timer = (sint16)sound_script.ticks;
    if (stamp == timer)
    {
        cursor = stream;
        for (;;)
        {
            if (cursor > sound_script.end || (uint32)(sound_script.end - cursor) < sizeof(*command))
                abort();
            command = (const SOUND_COMMAND *)cursor;
            type = command->type;
            if (type == 0)
            {
                payload = command->payload;
                if (payload >= SOUND_ID_COUNT)
                    abort();
                sound_script.sound = payload;
                payload = command->payload;
                cursor += 4;
                value = sound_play(payload);
                sound_script.voice = value;
            }
            else
            {
                if (((type - 1) & 255) < 2)
                {
                    value = command->type;
                    payload = command->payload;
                    sound_script.control[value - 1] = payload;
                }
                else if (type == 4)
                {
                    value = command->payload;
                    game_runtime.subtitles.speaker = (uint8)(value);
                    value = game_runtime.options.captions;
                    selector = value == 1 ? 82 : 268;
                    payload = game_runtime.subtitles.speaker;
                    hud_portrait_show(payload, selector, 30);
                    value = command->payload;
                    sound_script.speaker = value;
                }
                cursor += 4;
            }
            stream += 4;
            sound_script.cursor = stream;
            if (stream > sound_script.end || (uint32)(sound_script.end - stream) < 2)
                abort();
            stamp = *(const sint16 *)stream;
            if (stamp == 0xFFFFFFFF)
            {
                sound_script.cursor = 0;
                break;
            }
            timer = (sint16)sound_script.ticks;
            if (stamp != timer)
                break;
        }
    }
    value = sound_script.control[0];
    game_runtime.subtitles.requested[0] = (uint8)(value);
    value = sound_script.control[1];
    game_runtime.subtitles.requested[2] = (uint8)(0);
    game_runtime.subtitles.requested[1] = (uint8)(value);
    if (sound_script.cursor != 0)
    {
        value = sound_script.ticks + 1;
        sound_script.ticks = value;
        if (stream > sound_script.end || (uint32)(sound_script.end - stream) < 2)
            abort();
        stamp = *(const sint16 *)stream;
        if ((sint32)stamp < (sint16)value)
            sound_script.cursor = 0;
    }
    return 1;
}

void speech_queue_init(const uint8 *root_data, uint32 root_bytes, uint32 variant_offset)
{
    uint32 count, i, result, limit = root_bytes, size = root_bytes, first, last, found = 0;
    const uint32 *root_words, *words;
    FUNCTION_MARKER(0x80056198, "ACE3.BIN");
    root_words = (const uint32 *)root_data;
    if (root_words[0] == 3)
    {
        if (size < 16)
            abort();
        for (i = 0; i < 3; ++i)
        {
            first = root_words[i + 1];
            last = i == 2 ? size : root_words[i + 2];
            if (first < 16 || first >= last || last > size || ((first | last) & 3))
                abort();
            if (variant_offset == first)
            {
                limit = last;
                found = 1;
            }
        }
    }
    else
        found = variant_offset == 0;
    if (!found || (variant_offset & 3) || limit - variant_offset < 4)
        abort();
    size = limit - variant_offset;
    speech_queue.catalog.data = root_data + variant_offset;
    speech_queue.catalog.end = speech_queue.catalog.data + size;
    speech_queue.catalog.entries = 0;
    speech_queue.catalog.count = 0;
    speech_queue.catalog.commands = 0;
    words = (const uint32 *)speech_queue.catalog.data;
    if (words[0] != 4)
    {
        if (words[0] != 5 || size < 24)
            abort();
        first = words[5];
        if (first < 24 || (first & 3) || first > size - 4)
            abort();
        count = *(const uint32 *)(speech_queue.catalog.data + first);
        first += 4;
        if (count > (size - first) / sizeof(SPEECH_ENTRY))
            abort();
        speech_queue.catalog.entries = (const SPEECH_ENTRY *)(speech_queue.catalog.data + first);
        speech_queue.catalog.count = count;
        speech_queue.catalog.commands = speech_queue.catalog.data + first + count * sizeof(SPEECH_ENTRY);
    }
    arena_alloc(24);
    speech_queue.cur_item = 255;
    for (i = 0; i < 4; ++i)
    {
        speech_queue.pending[i] = 255;
        speech_queue.duration[i] = 0;
        speech_queue.elapsed[i] = 0;
    }
    result = sound_script_init();
    speech_queue.mode = 0;
    {
        (void)(uint32)(result);
        return;
    }
}

void speech_queue_push(uint32 id, sint32 value)
{
    uint32 index, result;
    FUNCTION_MARKER(0x80056234, "ACE3.BIN");
    for (index = 0; index < 4; ++index)
    {
        if (speech_queue.pending[index] == 255)
        {
            result = index << 1;
            speech_queue.pending[index] = id;
            speech_queue.duration[index] = value;
            speech_queue.elapsed[index] = 0;
            {
                (void)(uint32)(result);
                return;
            }
        }
    }
    return;
}

void speech_queue_update(void)
{
    uint32 flags, index, item, elapsed, duration, raw, value, first, second, mode;
    sint32 displacement;
    const SPEECH_ENTRY *entry;
    const uint8 *commands;
    FUNCTION_MARKER(0x8005628C, "ACE3.BIN");
    flags = game_runtime.flags;
    if ((flags & 0x20000000) == 0)
    {
        for (index = 0; index < 4; ++index)
        {
            item = speech_queue.pending[index];
            if (item == 255)
                continue;
            elapsed = (sint16)speech_queue.elapsed[index];
            duration = (sint16)speech_queue.duration[index];
            raw = speech_queue.elapsed[index];
            if (elapsed != duration)
            {
                if ((game_runtime.flags & 0x20000000) == 0)
                    speech_queue.elapsed[index] = raw + 1;
                continue;
            }
            if (speech_queue.cur_item != 255)
                continue;
            if (game_runtime.options.captions == 1)
            {
                hud_panel_open();
                if (hud_panel_get_mode() != 2)
                {
                    speech_queue.mode = 3;
                    continue;
                }
            }
            if (item >= speech_queue.catalog.count)
                abort();
            entry = &speech_queue.catalog.entries[item];
            value = entry->mode;
            if (value == 0)
            {
                value = entry->phrase;
                speech_queue_play_phrase(value);
            }
            else
            {
                displacement = (sint32)entry->command_offset * 4;
                commands = speech_queue.catalog.commands;
                if (displacement < 0)
                {
                    if ((uint32)-displacement > (uint32)(commands - speech_queue.catalog.data))
                        abort();
                    commands -= (uint32)-displacement;
                }
                else
                {
                    if ((uint32)displacement > (uint32)(speech_queue.catalog.end - commands))
                        abort();
                    commands += displacement;
                }
                sound_script_start(commands, (uint32)(speech_queue.catalog.end - commands));
            }
            flags = game_runtime.flags;
            game_runtime.flags = flags | 0x10000000;
            speech_queue.cur_item = item;
            speech_queue.pending[index] = 255;
        }
    }
    first = speech_queue_update_phrase();
    second = sound_script_update();
    if (first == 0 && second == 0 && subtitles_has_visible() == 0)
    {
        mode = speech_queue.mode;
        if (mode != 3)
        {
            if (mode != 0)
            {
                if (game_runtime.mission_control.mode & 4)
                {
                    flags = game_runtime.flags;
                    game_runtime.flags = flags & 0xEFFFFFFF;
                }
            }
            speech_queue.mode = 0;
            speech_queue.cur_item = 255;
        }
    }
    if (speech_queue.cur_item == 255 && game_runtime.subtitles.speaker != 0)
    {
        game_runtime.subtitles.speaker = (uint8)(0);
        hud_portrait_hide(1);
    }
    if (game_runtime.options.captions == 1)
    {
        if (speech_queue.cur_item == 255)
        {
            (void)(hud_panel_close());
            return;
        }
        {
            (void)(hud_panel_open());
            return;
        }
    }
    return;
}

void speech_queue_stop(void)
{
    uint32 mode, value, index;
    FUNCTION_MARKER(0x80056518, "ACE3.BIN");
    mode = speech_queue.mode;
    if (mode == 1)
        resource_cancel_streams();
    else if (mode == 2)
    {
        value = sound_script.voice;
        if (value != 255)
            sound_stop_voice(value);
        sound_script_start(0, 0);
    }
    else if (mode == 3)
    {
        speech_queue.mode = 0;
        hud_panel_close();
    }
    hud_portrait_hide(0);
    speech_queue.cur_item = 255;
    for (index = 0; index < 4; ++index)
    {
        speech_queue.pending[index] = 255;
        speech_queue.duration[index] = 0;
        speech_queue.elapsed[index] = 0;
    }
    return;
}

uint32 speech_queue_mode(void)
{
    FUNCTION_MARKER(0x8005650C, "ACE3.BIN");
    return speech_queue.mode;
}

void speech_queue_play_phrase(uint32 index)
{
    uint32 mission;
    FUNCTION_MARKER(0x800565E8, "ACE3.BIN");
    speech_queue.mode = 1;
    mission = (sint16)game_runtime.selection.mission;
    speech_queue.phrase_phase = 2;
    speech_queue_enqueue_phrase(mission, index);
    {
        (void)(uint32)(subtitles_configure(0, 0, 0));
        return;
    }
}

void speech_queue_phrase_complete(void)
{
    FUNCTION_MARKER(0x80056A44, "ACE3.BIN");
    speech_queue.phrase_phase = 1;
    return;
}

void speech_queue_enqueue_phrase(uint32 mission, uint32 index)
{
    const RESOURCE_AUDIO_TRACK *track;
    FUNCTION_MARKER(0x80056A54, "ACE3.BIN");
    resource_cancel_streams();
    if (index == 255)
        return;
    track = audio_mission_phrase(mission, index);
    resource_queue_audio(track, 1, speech_queue_phrase_complete);
    {
        (void)(uint32)(index);
        return;
    }
}

void audio_play_main_track(uint32 index)
{
    uint32 id, current;
    const RESOURCE_AUDIO_TRACK *record;
    FUNCTION_MARKER(0x8005697C, "ACE3.BIN");
    if (index == 255)
    {
        resource_cancel_streams();
        return;
    }
    record = audio_main_track(index);
    id = (uint16)record->id & 0x7FF;
    current = sound_get_cur_resource();
    if (id != (current & 0x7FF))
    {
        resource_cancel_streams();
        record = audio_main_track(index);
        resource_queue_audio(record, 0, 0);
    }
    {
        (void)(uint32)(index);
        return;
    }
}

uint32 speech_queue_update_phrase(void)
{
    uint32 mode, value, selector;
    FUNCTION_MARKER(0x80056634, "ACE3.BIN");
    mode = speech_queue.phrase_phase;
    if (mode == 2)
    {
        value = game_runtime.options.captions;
        selector = value == 1 ? 82 : 268;
        value = game_runtime.subtitles.speaker;
        hud_portrait_show(value, selector, 30);
        return 1;
    }
    if (mode == 1)
    {
        speech_queue.phrase_phase = 0;
        subtitles_configure(255, 0, 0);
        if (game_runtime.mission_control.mode == 4)
        {
            value = mission_flow.request.music_id;
            audio_play_main_track(value);
        }
        hud_portrait_hide(1);
    }
    return 0;
}

void sound_initialize_mix(void)
{
    SpuCommonAttr attr;
    CdlATV volume;

    FUNCTION_MARKER(0x80025E38, "ACE3.BIN");
    attr.mask = 0x3C3;
    attr.mvol.left = 0x3FFF;
    attr.mvol.right = 0x3FFF;
    attr.cd.volume.left = 0x7FFF;
    attr.cd.volume.right = 0x7FFF;
    attr.cd.reverb = 1;
    attr.cd.mix = 1;
    SpuSetCommonAttr(&attr);
    volume.val0 = 255;
    volume.val1 = 0;
    volume.val2 = 255;
    volume.val3 = 0;
    cd_apply_volume(&volume);
}

static sint32 sound_upload(uint32 address, const uint8 *source, uint32 bytes, uint32 source_bytes)
{
    uint32 length = bytes;

    FUNCTION_MARKER(0x8002D910, "ACE3.BIN");
    SpuSetTransferMode(1);
    SpuSetTransferStartAddr(address);
    if (length > XPORT_SPU_WRITE_MAX_BYTES)
        length = XPORT_SPU_WRITE_MAX_BYTES;
    if (!source || length > source_bytes)
        abort();
    SpuWrite(source, bytes);
    if (spu_transfer_failed())
    {
#ifndef NDEBUG
        fprintf(stderr, "SPU upload failed: source %p bytes %u\n", (const void *)source, length);
#endif
        abort();
    }
    return SpuIsTransferCompleted(1);
}

void sound_initialize(void)
{
    FUNCTION_MARKER(0x8002D75C, "ACE3.BIN");
    sound_init_voices();
    sound_init_cd(0);
    sound_init_slots();
    sound_init_reverb();
    {
        (void)(sound_init_queue());
        return;
    }
}

void mission_audio_initialize_groups(void)
{
    FUNCTION_MARKER(0x80030464, "ACE3.BIN");
    engine_audio_init();
    scene_audio_init_groups(2);
    return;
}

void mission_audio_stop_groups(void)
{
    FUNCTION_MARKER(0x8003048C, "ACE3.BIN");
    engine_audio_stop();
    scene_audio_stop_groups();
    sound_stop_all();
    sound_init_slots();
    sound_init_queue();
    return;
}

void mission_audio_stop_scene_groups(void)
{
    FUNCTION_MARKER(0x800304CC, "ACE3.BIN");
    scene_audio_stop_groups();
    return;
}

void mission_audio_reset_sequence(void)
{
    FUNCTION_MARKER(0x800F74C8, "ACE3.BIN");
    mission_audio_player.cur_idx = 0;
    mission_audio_player.requested_idx = 0;
    mission_audio_player.flags = 0;
}

void mission_audio_bind_tracks(const AUDIO_TRACK_TABLE *tracks)
{
    FUNCTION_MARKER(0x800F7514, "ACE3.BIN");
    mission_audio_player.tracks = tracks;
}

uint32 mission_audio_current_track_id(void)
{
    uint32 index;
    const RESOURCE_AUDIO_TRACK *track;

    FUNCTION_MARKER(0x800F787C, "ACE3.BIN");
    index = mission_audio_player.cur_idx;
    if (!mission_audio_player.tracks || index >= mission_audio_player.tracks->count)
        abort();
    track = mission_audio_player.tracks->records + index;
    return (uint32)track->id;
}

uint32 mission_audio_control_track(sint32 mode, uint32 resource)
{
    uint32 flags, current, desired, result;
    const RESOURCE_AUDIO_TRACK *descriptor;
    sint32 status;

    FUNCTION_MARKER(0x800F7524, "ACE3.BIN");
    status = resource_wait(1);
    mission_audio_player.requested_idx = resource;
    if (mode == 1 && status == 0)
    {
        if (!mission_audio_player.tracks || resource >= mission_audio_player.tracks->count)
            abort();
        descriptor = mission_audio_player.tracks->records;
        resource_queue_audio(descriptor + resource, 1, 0);
        flags = mission_audio_player.flags;
        desired = mission_audio_player.requested_idx;
        mission_audio_player.cur_idx = desired;
        mission_audio_player.flags = flags | 1;
    }
    else if (mode == 2)
    {
        flags = mission_audio_player.flags;
        resource_pause_stream((flags & 2) ? 0 : 1);
    }
    else if (mode == 3)
    {
        resource_cancel_streams();
        flags = mission_audio_player.flags;
        mission_audio_player.flags = flags & 0xFFFFFFFC;
    }
    current = mission_audio_player.cur_idx;
    desired = mission_audio_player.requested_idx;
    result = status < 2;
    if (current == desired)
    {
        if (status != 0)
            return result;
        flags = mission_audio_player.flags;
        if (!(flags & 1))
            return status < 2;
    }
    if (!result)
        return 0x800C0000;
    result = game_input.held & 12;
    if (result || mode != 0)
        return result;
    resource_cancel_streams();
    desired = mission_audio_player.requested_idx;
    if (!mission_audio_player.tracks || desired >= mission_audio_player.tracks->count)
        abort();
    descriptor = mission_audio_player.tracks->records;
    resource_queue_audio(descriptor + desired, 1, 0);
    flags = mission_audio_player.flags;
    desired = mission_audio_player.requested_idx;
    mission_audio_player.cur_idx = desired;
    result = (flags | 1) & 0xFFFFFFFD;
    mission_audio_player.flags = result;
    return result;
}

uint32 mission_audio_music_index(uint32 index)
{
    FUNCTION_MARKER(0x80056964, "ACE3.BIN");
    return audio_mission_music_index(index);
}
