#ifndef GAME_SAVE_FORMAT_H
#define GAME_SAVE_FORMAT_H

#include <stddef.h>

#include "game_runtime.h"
#include "pad_input.h"

enum { GAME_SAVE_HEADER = 16, GAME_SAVE_PAYLOAD = 2228, GAME_SAVE_BYTES = GAME_SAVE_HEADER + GAME_SAVE_PAYLOAD };

typedef struct
{
    uint8 name[12];
    uint16 route_mode;
    uint8 result_kind;
    uint8 mission_failed;
} ACCOUNT_PREFIX;

typedef struct
{
    uint16 binding[21];
    PAD_CALIBRATION_RECORD cal[4];
    uint16 axis_flags;
    uint8 axis_map[6];
    uint16 flags;
} GAME_SAVE_PROFILE_V1;

typedef struct
{
    GAME_SAVE_PROFILE_V1 profile[6];
    uint16 mission;
    uint16 aircraft;
    uint16 saved_mission;
    uint16 saved_aircraft;
    uint32 difficulty;
    uint8 volume[2];
    uint8 audio_flags;
    uint8 options_reserved;
    uint8 captions;
    uint8 kill_camera;
    uint8 hud_hidden;
    uint8 camera_mode;
    uint32 return_mode;
    uint8 intro_count;
    uint8 demo_mode;
    uint8 demo_idx;
    uint8 caption_gate;
    uint8 movie_idx;
    uint8 weapon_code[3];
    ACCOUNT_PREFIX account;
    uint16 loss_count;
    uint8 hold_action_gate;
    uint8 account_reserved;
    uint8 record_reserved;
    uint8 record_slot;
    uint8 sequence_idx;
    uint8 movie_branch;
    uint8 progress_reserved[4];
    uint32 account_flags;
    uint32 account_id;
    uint32 flow_flags;
    uint8 save_reserved_4;
    uint8 menu_reserved;
    uint8 save_reserved_5;
    uint8 save_reserved_6;
    uint8 aircraft_unlocked[64];
    uint8 weapon_unlocked[16];
    uint16 mission_time[5][GAME_MISSION_COUNT];
    uint8 mission_rating[5][GAME_MISSION_COUNT];
    uint8 archive_status[80];
    uint8 scene_status[180];
    uint32 mission_count[GAME_MISSION_COUNT];
    uint32 mission_losses[GAME_MISSION_COUNT];
    uint32 total_time;
    uint32 total_count;
    uint32 total_losses;
    sint32 screen_x;
    uint32 screen_y;
    uint32 input_flags;
} GAME_SAVE_PAYLOAD_V1;

typedef struct
{
    uint32 magic;
    uint32 version;
    uint32 payload_bytes;
    uint32 checksum;
    GAME_SAVE_PAYLOAD_V1 payload;
} GAME_SAVE_FILE_V1;

typedef char GAME_SAVE_V1_LAYOUT_CHECK[
    GAME_MISSION_COUNT == 60 && sizeof(ACCOUNT_PREFIX) == 16 &&
    sizeof(PAD_CALIBRATION_RECORD) == 4 && sizeof(GAME_SAVE_PROFILE_V1) == 68 &&
    offsetof(GAME_SAVE_PROFILE_V1, cal) == 42 && offsetof(GAME_SAVE_PROFILE_V1, axis_flags) == 58 &&
    offsetof(GAME_SAVE_PROFILE_V1, axis_map) == 60 && offsetof(GAME_SAVE_PROFILE_V1, flags) == 66 &&
    offsetof(GAME_SAVE_PAYLOAD_V1, mission) == 408 && offsetof(GAME_SAVE_PAYLOAD_V1, volume) == 420 &&
    offsetof(GAME_SAVE_PAYLOAD_V1, return_mode) == 428 && offsetof(GAME_SAVE_PAYLOAD_V1, account) == 440 &&
    offsetof(GAME_SAVE_PAYLOAD_V1, record_reserved) == 460 && offsetof(GAME_SAVE_PAYLOAD_V1, aircraft_unlocked) == 484 &&
    offsetof(GAME_SAVE_PAYLOAD_V1, weapon_unlocked) == 548 && offsetof(GAME_SAVE_PAYLOAD_V1, mission_time) == 564 &&
    offsetof(GAME_SAVE_PAYLOAD_V1, mission_rating) == 1164 && offsetof(GAME_SAVE_PAYLOAD_V1, archive_status) == 1464 &&
    offsetof(GAME_SAVE_PAYLOAD_V1, scene_status) == 1544 && offsetof(GAME_SAVE_PAYLOAD_V1, mission_count) == 1724 &&
    offsetof(GAME_SAVE_PAYLOAD_V1, mission_losses) == 1964 && offsetof(GAME_SAVE_PAYLOAD_V1, total_time) == 2204 &&
    offsetof(GAME_SAVE_PAYLOAD_V1, screen_x) == 2216 && sizeof(GAME_SAVE_PAYLOAD_V1) == GAME_SAVE_PAYLOAD &&
    offsetof(GAME_SAVE_FILE_V1, payload) == GAME_SAVE_HEADER && sizeof(GAME_SAVE_FILE_V1) == GAME_SAVE_BYTES ? 1 : -1];

#endif
