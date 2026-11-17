#ifndef PAD_INPUT_H
#define PAD_INPUT_H

#include "psx.h"

typedef struct
{
    uint32 flags;
    uint32 kind;
    uint32 held;
    uint32 pressed;
    uint32 repeat;
    uint32 released;
    uint32 axis_mask;
    sint32 axis[6];
    uint8 motor[2];
    uint16 status;
    uint8 repeat_tick;
    uint8 delay_tick[2];
} GAME_INPUT;

typedef struct
{
    uint8 mode_id;
    uint8 motor_mask;
    uint8 motor[6];
    uint16 version;
    uint8 type;
    uint8 status;
    uint16 buttons;
    uint16 pressed;
    uint16 axis[4];
} PAD_PORT_STATE;

extern PAD_PORT_STATE pad_state[8];

typedef struct
{
    uint8 low;
    uint8 center;
    uint8 high;
    uint8 dead_zone;
} PAD_AXIS_CALIBRATION;

// fixed calibration encoding for card and native save records
typedef struct
{
    uint8 low;
    uint8 center;
    uint8 high;
    uint8 dead_zone;
} PAD_CALIBRATION_RECORD;

typedef struct
{
    uint16 binding[32];
    PAD_AXIS_CALIBRATION cal[4];
    uint16 axis_flags;
    uint8 axis_map[6];
    uint32 flags;
} PAD_PROFILE;

extern PAD_PROFILE pad_profiles[6];
void pad_profile_encode(uint8 *dst, const PAD_PROFILE *profile);
void pad_profile_decode(PAD_PROFILE *profile, const uint8 *src);

extern GAME_INPUT game_input;

void game_input_reset(void);
uint32 pad_profile_index(uint32 kind);

#endif
