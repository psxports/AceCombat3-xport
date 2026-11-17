#include <stdlib.h>
#include <string.h>

#include "game_functions.h"

typedef struct
{
    RESOURCE_RING_FRAME frame;
    const uint8 *commands;
} SCENE_SCRIPT_FRAMES;

static SCENE_SCRIPT_FRAMES script_frames;

void scene_script_reset_frames(void)
{
    memset(&script_frames, 0, sizeof(script_frames));
}

enum
{
    MISSION_MOVIE_CHOICE_OFFSET = 244
};

/* the original selectors can index across adjacent table rows */
static const uint8 mission_playback_choices[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x06, 0x5E, 0x00, 0x00, 0x07, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x0A, 0x0B, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0F, 0x10, 0x00, 0x00, 0x11, 0x12, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x00, 0x00, 0x00, 0x14, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x15, 0x16, 0x17, 0x18, 0x00, 0x00, 0x00, 0x00, 0x1A, 0x1B, 0x1C, 0x00, 0x1D, 0x00, 0x00, 0x00, 0x1E, 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x22, 0x23, 0x24, 0x00, 0x25, 0x26, 0x00, 0x00, 0x27, 0x28, 0x29, 0x00, 0x2A, 0x00, 0x00, 0x00, 0x2C, 0x2D, 0x00, 0x00, 0x2E, 0x2F, 0x30, 0x00, 0x33, 0x34, 0x35, 0x00, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x00, 0x00, 0x00,
    0x3C, 0x3D, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x40, 0x41, 0x00, 0x00, 0x42, 0x43, 0x00, 0x00, 0x44, 0x45, 0x00, 0x00, 0x47, 0x00, 0x00, 0x00, 0x48, 0x49, 0x4A, 0x5F, 0x4B, 0x00, 0x00, 0x00, 0x4C, 0x4D, 0x4E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x4F, 0x50, 0x00, 0x00, 0x51, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x54, 0x55, 0x56, 0x00, 0x57, 0x00, 0x00, 0x00, 0x5C, 0x5D, 0x00, 0x00, 0x58, 0x00, 0x00, 0x00, 0x59, 0x5A, 0x00, 0x00, 0x5B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0A, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x13, 0x00, 0x14, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x17, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1B, 0x00, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB0, 0x93, 0x03, 0x80, 0x98, 0x8E, 0x03, 0x80, 0x28, 0x90, 0x03, 0x80, 0xE4, 0x90, 0x03, 0x80, 0x6C, 0x91, 0x03, 0x80, 0x30, 0x92, 0x03, 0x80, 0x80, 0x93, 0x03, 0x80, 0x00, 0x8F, 0x03, 0x80, 0x88, 0x93, 0x03, 0x80, 0x47, 0x18, 0x48, 0x18, 0x49, 0x18, 0x49, 0x18, 0x4A, 0x18, 0x4B, 0x18, 0x4C, 0x18, 0x4E, 0x18, 0x4F, 0x18, 0x50, 0x18, 0x51, 0x18, 0x52, 0x18, 0x53, 0x18, 0x53, 0x18, 0x54, 0x18, 0x55, 0x18, 0x56, 0x18, 0x57, 0x18, 0x58, 0x18, 0x59, 0x18, 0x5A, 0x18, 0x5B, 0x18, 0x5C, 0x18, 0x5D, 0x18, 0x5E, 0x18, 0x5E, 0x18, 0x5F, 0x18, 0x60, 0x18, 0x61, 0x18, 0x62, 0x18, 0x63, 0x18, 0x64, 0x18, 0x64, 0x18, 0x64, 0x18, 0x65, 0x18, 0x66, 0x18, 0x67, 0x18, 0x68, 0x18,
    0x69, 0x18, 0x6A, 0x18, 0x6B, 0x18, 0x6C, 0x18, 0x6D, 0x18, 0x6D, 0x18, 0x6E, 0x18, 0x6F, 0x18, 0x70, 0x18, 0x71, 0x18, 0x72, 0x18, 0x72, 0x18, 0x72, 0x18, 0x73, 0x18, 0x74, 0x18, 0x75, 0x18, 0x76, 0x18, 0x77, 0x18, 0x78, 0x18, 0x79, 0x18, 0x7A, 0x18, 0x7A, 0x18, 0x7B, 0x18, 0x7C, 0x18, 0x7D, 0x18, 0x7D, 0x18, 0x7E, 0x18, 0x7F, 0x18, 0x80, 0x18, 0x81, 0x18, 0x82, 0x18, 0x83, 0x18, 0x83, 0x18, 0x84, 0x18, 0x85, 0x18, 0x86, 0x18, 0x87, 0x18, 0x89, 0x18, 0x8A, 0x18, 0x8B, 0x18, 0x8C, 0x18, 0x8D, 0x18, 0x8E, 0x18, 0x8F, 0x18, 0x8F, 0x18, 0x8F, 0x18, 0x90, 0x18, 0x91, 0x18, 0x92, 0x18, 0x93, 0x18, 0x96, 0x18, 0x97, 0x18, 0x98, 0x18, 0x99, 0x18, 0x94, 0x18, 0x95, 0x18, 0x4D, 0x18, 0x88, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00, 0x01,
    0x00, 0x01, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x01, 0x02, 0x03, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x01, 0x00, 0x01, 0x02, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0x01, 0x02, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x01, 0x01, 0x03, 0xFF, 0xFF, 0x04, 0xFF, 0x02, 0xFF, 0x02, 0x03, 0xFF, 0x02, 0xFF, 0xFF, 0x02, 0x03, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x02, 0xFF, 0x03, 0xFF, 0x00, 0x09, 0x05, 0x06, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0E, 0x0A, 0x0E, 0x00, 0x00, 0x00, 0x04, 0x00, 0x0A, 0x06, 0x0A, 0x00, 0x00, 0x00, 0x90, 0x35, 0x05, 0x06, 0x04, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x2C, 0x00, 0x00, 0x00, 0x0E, 0x00, 0x00, 0x00, 0x0C, 0x00, 0x00, 0x00, 0x2E, 0x00, 0x00, 0x00, 0x2C, 0x00, 0x00, 0x00, 0x2E, 0x00, 0x00, 0x00, 0x50, 0x70, 0x50, 0x70, 0x6F, 0x4F, 0x6F, 0x4F, 0x50, 0x70, 0x50, 0x70, 0x6F, 0x4F, 0x6F, 0x4F, 0x37, 0x37, 0x57, 0x57,
    0x37, 0x37, 0x57, 0x57, 0x56, 0x56, 0x36, 0x36, 0x56, 0x56, 0x36, 0x36, 0x00, 0x00, 0x00, 0x00, 0xC2, 0x36, 0x0E, 0x06, 0xEC, 0x36, 0x14, 0x06, 0x88, 0x72, 0x12, 0x06, 0x00, 0x00, 0x00, 0x00, 0xBA, 0x54, 0x1A, 0x06, 0x88, 0x54, 0x18, 0x06, 0xE8, 0x66, 0x18, 0x06, 0x88, 0x60, 0x22, 0x06, 0xAA, 0x66, 0x22, 0x06, 0xDE, 0x4E, 0x12, 0x06, 0xA4, 0x5A, 0x1A, 0x06, 0xAA, 0x60, 0x1E, 0x06, 0xCC, 0x66, 0x1C, 0x06, 0x88, 0x66, 0x22, 0x06, 0x88, 0x3C, 0x16, 0x06, 0xBE, 0x5A, 0x1A, 0x06, 0x88, 0x6C, 0x1C, 0x06, 0xA0, 0x48, 0x18, 0x06, 0x88, 0x4E, 0x18, 0x06, 0xCC, 0x48, 0x12, 0x06, 0xCC, 0x4E, 0x12, 0x06, 0xB8, 0x48, 0x14, 0x06, 0x9C, 0x42, 0x14, 0x06, 0xB8, 0x4E, 0x14, 0x06, 0xB0, 0x42, 0x14, 0x06, 0x88, 0x42, 0x14, 0x06, 0xC4, 0x42, 0x14, 0x06, 0xD8, 0x42, 0x14, 0x06, 0xA0, 0x54, 0x1A, 0x06, 0xF0, 0x4E, 0x10, 0x06, 0x88, 0x5A, 0x1C, 0x06, 0xAA, 0x60, 0x1E, 0x06, 0xCC, 0x4E, 0x12, 0x06, 0x88, 0x42, 0x14, 0x06, 0xE8, 0x3C, 0x18, 0x06, 0xCC, 0x66, 0x1C, 0x06,
    0xDE, 0x48, 0x12, 0x06, 0xB2, 0x3C, 0x0E, 0x06, 0x88, 0x48, 0x18, 0x06, 0xD8, 0x5A, 0x14, 0x06, 0xA0, 0x4E, 0x18, 0x06, 0xD4, 0x54, 0x18, 0x06, 0xD8, 0x42, 0x14, 0x06, 0xEC, 0x42, 0x14, 0x06, 0x9E, 0x3C, 0x14, 0x06, 0xEC, 0x5A, 0x14, 0x06, 0xEC, 0x54, 0x14, 0x06, 0xC8, 0x60, 0x1C, 0x06, 0xE4, 0x60, 0x1C, 0x06, 0xD4, 0x3C, 0x14, 0x06, 0x9A, 0x72, 0x10, 0x06, 0xF2, 0x30, 0x0E, 0x06, 0xD8, 0x2A, 0x14, 0x06, 0xD2, 0x0C, 0x18, 0x06, 0x88, 0x12, 0x2A, 0x06, 0xD6, 0x18, 0x2A, 0x06, 0x88, 0x24, 0x20, 0x06, 0xA8, 0x24, 0x1E, 0x06, 0xC4, 0x2A, 0x14, 0x06, 0xB2, 0x0C, 0x20, 0x06, 0xE2, 0x24, 0x1E, 0x06, 0xD6, 0x1E, 0x2A, 0x06, 0x88, 0x18, 0x2C, 0x06, 0x88, 0x1E, 0x2C, 0x06, 0xB6, 0x12, 0x26, 0x06, 0xB4, 0x1E, 0x22, 0x06, 0xEC, 0x2A, 0x14, 0x06, 0xD0, 0x36, 0x0E, 0x06, 0xD0, 0x30, 0x22, 0x06, 0xA8, 0x2A, 0x1C, 0x06, 0x88, 0x2A, 0x20, 0x06, 0xAC, 0x30, 0x24, 0x06, 0xC6, 0x24, 0x18, 0x06, 0xD2, 0x06, 0x1E, 0x06, 0x88, 0x06, 0x2A, 0x06, 0x88, 0x30, 0x24, 0x06,
    0x88, 0x0C, 0x2A, 0x06, 0x9A, 0x36, 0x14, 0x06, 0x88, 0x36, 0x12, 0x06, 0xDC, 0x12, 0x24, 0x06, 0xB2, 0x06, 0x20, 0x06, 0xF0, 0x48, 0x10, 0x06, 0xB4, 0x18, 0x22, 0x06, 0xAE, 0x36, 0x14, 0x06, 0xA4, 0x6C, 0x26, 0x06, 0xEA, 0x0C, 0x16, 0x06, 0xCA, 0x6C, 0x28, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA2, 0x00, 0x14, 0x06, 0xDA, 0x00, 0x12, 0x06, 0xB6, 0x00, 0x12, 0x06, 0xF0, 0x06, 0x10, 0x06, 0xC8, 0x00, 0x12, 0x06, 0x88, 0x00, 0x1A, 0x06, 0xEC, 0x00, 0x14, 0x06, 0x12, 0x00, 0x5A, 0x00, 0x5E, 0x00, 0x50, 0x00, 0x02, 0x10, 0x03, 0x40, 0x42, 0x00, 0x00, 0x00, 0x10, 0x00, 0x2D, 0x00, 0x5E, 0x00, 0x50, 0x00, 0x00, 0x1C, 0xFF, 0x00, 0x40, 0x00, 0x00, 0x00, 0x11, 0x00, 0x2D, 0x00, 0x5E, 0x00, 0x50, 0x00, 0x01, 0x1C, 0xFF, 0x00, 0x41, 0x00, 0x00, 0x00, 0x20, 0x00, 0x5A, 0x00, 0x5E, 0x00, 0xC0, 0x00, 0x08, 0x0C, 0x09, 0x3A, 0x45, 0x00, 0x1E, 0x00, 0x21, 0x01, 0x2D, 0x00, 0x5E, 0x00, 0xC0, 0x00,
};

typedef enum
{
    PLAYBACK_EFFECT_NONE = 0,
    PLAYBACK_EFFECT_NOISE_RAMP_151,
    PLAYBACK_EFFECT_NOISE_RAMP_571,
    PLAYBACK_EFFECT_NOISE_FADE_90,
    PLAYBACK_EFFECT_FEEDBACK_RAMP_511,
    PLAYBACK_EFFECT_FEEDBACK_RAMP_331,
    PLAYBACK_EFFECT_FEEDBACK_PULSE,
} SCENE_PLAYBACK_EFFECT;

static const uint8 playback_resume_skip[89] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0, 1, 1,
};

static const uint8 playback_reverse_close[83] = {
    1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
};

static const SCENE_PLAYBACK_EFFECT playback_effects[53] = {
    PLAYBACK_EFFECT_NOISE_RAMP_151, PLAYBACK_EFFECT_NOISE_FADE_90, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_FEEDBACK_PULSE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NOISE_RAMP_151, PLAYBACK_EFFECT_NOISE_FADE_90, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE,           PLAYBACK_EFFECT_NONE,          PLAYBACK_EFFECT_FEEDBACK_RAMP_511, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE,
    PLAYBACK_EFFECT_NONE,           PLAYBACK_EFFECT_NONE,          PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE,           PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE,           PLAYBACK_EFFECT_NONE,          PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NONE, PLAYBACK_EFFECT_NOISE_RAMP_571, PLAYBACK_EFFECT_NOISE_FADE_90, PLAYBACK_EFFECT_FEEDBACK_RAMP_331,
};

typedef struct
{
    uint32 flags;
    uint16 angles[3];
    uint16 reserved;
    uint32 position[3];
} SCENE_POSE_COMMAND;

typedef struct
{
    union
    {
        uint32 flags;

        struct
        {
            uint16 code;
            sint16 height;
        } projection;
    } command;

    sint16 near_clip;
    sint16 far_clip;
} SCENE_PROJECTION_COMMAND;

typedef struct
{
    union
    {
        uint32 flags;

        struct
        {
            uint16 code;
            sint16 parameter;
        } info;
    } control;

    union
    {
        uint32 words[2];
        sint16 signed16[4];
        uint16 unsigned16[4];
    } arguments;

    uint32 position[3];
} SCENE_EFFECT_COMMAND;

typedef struct
{
    uint16 code;
    sint16 parameter;
    uint8 tag;
    uint8 reserved;
    sint16 control;
    uint32 values[6];
} SCENE_RECORD_COMMAND;

typedef char SCENE_EFFECT_COMMAND_LAYOUT_CHECK[sizeof(SCENE_EFFECT_COMMAND) == 24 && offsetof(SCENE_EFFECT_COMMAND, control.info.parameter) == 2 && offsetof(SCENE_EFFECT_COMMAND, arguments) == 4 && offsetof(SCENE_EFFECT_COMMAND, position) == 12 && sizeof(SCENE_RECORD_COMMAND) == 32 && offsetof(SCENE_RECORD_COMMAND, tag) == 4 && offsetof(SCENE_RECORD_COMMAND, control) == 6 && offsetof(SCENE_RECORD_COMMAND, values) == 8 ? 1 : -1];

typedef char SCENE_COMMAND_LAYOUT_CHECK[sizeof(SCENE_POSE_COMMAND) == 24 && offsetof(SCENE_POSE_COMMAND, angles) == 4 && offsetof(SCENE_POSE_COMMAND, position) == 12 && sizeof(SCENE_PROJECTION_COMMAND) == 8 && offsetof(SCENE_PROJECTION_COMMAND, command.projection.height) == 2 && offsetof(SCENE_PROJECTION_COMMAND, near_clip) == 4 && offsetof(SCENE_PROJECTION_COMMAND, far_clip) == 6 ? 1 : -1];

static void create_record(const SCENE_RECORD_COMMAND *command)
{
    EFFECT_SEGMENT *record;
    uint32 value, first, second, third, byte, half, result;
    FUNCTION_MARKER(0x80093944, "ACE3.BIN");
    record = effect_segment_allocate();
    result = 3;
    if (record != 0)
    {
        value = command->parameter;
        record->kind = (uint16)(3);
        record->phase = (uint16)(value);
        first = command->values[0];
        second = command->values[1];
        third = command->values[2];
        byte = command->tag;
        half = command->control;
        record->origin[0] = (uint32)((first & 0xFFFFFF00) | byte);
        record->origin[2] = (uint32)(third);
        result = (second & 0xFFFFFF00) | (half & 255);
        record->origin[1] = (uint32)(result);
        first = command->values[3];
        second = command->values[4];
        third = command->values[5];
        record->finish[0] = (uint32)(first);
        record->finish[1] = (uint32)(second);
        record->finish[2] = (uint32)(third);
    }
    {
        (void)(uint32)(result);
        return;
    }
}

static void create_pooled_effect(const SCENE_EFFECT_COMMAND *command, uint32 kind)
{
    EFFECT_DRIFT *record;
    uint32 first, second, value, result;
    FUNCTION_MARKER(0x8009351C, "ACE3.BIN");
    record = effect_drift_allocate_pending();
    result = 0;
    if (record != 0)
    {
        first = command->position[0];
        second = command->position[1];
        record->position[0] = (uint32)(first);
        record->position[1] = (uint32)(second);
        first = command->position[2];
        second = command->arguments.words[0];
        record->position[2] = (uint32)(first);
        {
            uint32 packed_record_word = second;
            record->ticks = (uint16)packed_record_word;
            record->duration = (uint16)(packed_record_word >> 16);
        }
        value = command->arguments.signed16[3];
        record->flags = (uint16)(0);
        record->radius = (uint16)(value);
        result = (sint32)kind < 4 ? (kind & 3) << 8 : (((kind & 1) + 1) << 8) | 16;
        record->style = (uint16)(result);
    }
    {
        (void)(uint32)(result);
        return;
    }
}

static void create_render_effect(const SCENE_EFFECT_COMMAND *command, uint32 kind)
{
    EFFECT_RENDER *render_effect;
    uint32 first, second, third, value, half, result;
    FUNCTION_MARKER(0x80093484, "ACE3.BIN");
    render_effect = effect_render_allocate();
    result = 0;
    if (render_effect != 0)
    {
        first = command->position[0];
        second = command->position[1];
        third = command->position[2];
        render_effect->position[0] = (uint32)(first);
        render_effect->position[1] = (uint32)(second);
        render_effect->position[2] = (uint32)(third);
        if (kind != 0)
        {
            value = command->arguments.words[0];
            render_effect->kind = (uint16)(1);
            render_effect->angles[0] = (uint16)(value);
            render_effect->angles[1] = (uint16)(value >> 16);
        }
        else
            render_effect->kind = (uint16)(0);
        value = command->control.info.parameter;
        half = command->arguments.signed16[3];
        result = value | 0x10000000;
        render_effect->ticks = (uint16)(result);
        render_effect->duration = (uint16)(result >> 16);
        render_effect->radius = (uint16)(half);
    }
    {
        (void)(uint32)(result);
        return;
    }
}

static void create_drifting_effect(const SCENE_EFFECT_COMMAND *command, uint32 mode)
{
    EFFECT_DRIFT *record;
    uint32 x, y, z, value, random, scale, result;
    FUNCTION_MARKER(0x80093324, "ACE3.BIN");
    record = effect_drift_allocate(&effect_drift_secondary_pool);
    result = 0;
    if (record != 0)
    {
        record->ticks = (uint16)(0);
        x = command->arguments.signed16[0];
        y = command->arguments.signed16[1];
        z = command->arguments.signed16[2];
        record->vector[0] = (uint16)(x);
        record->vector[1] = (uint16)(y);
        record->vector[2] = (uint16)(z);
        value = command->position[0];
        record->position[0] = (uint32)(value - (x << 8) - (x << 6));
        value = command->position[1];
        record->position[1] = (uint32)(value - (y << 8) - (y << 6));
        value = command->position[2];
        record->position[2] = (uint32)(value - (z << 8) - (z << 6));
        random = game_random() & 127;
        scale = command->arguments.signed16[3];
        value = (random + 160) * scale;
        value = (sint32)value / 4096;
        record->radius = (uint16)(value);
        value = command->control.info.parameter;
        if (mode == 2)
        {
            value = (sint32)value / 256;
            record->duration = (uint16)(value);
            record->flags = (uint16)(140);
            result = 0xA04;
        }
        else
        {
            value *= 80;
            value = (sint32)value / 4096;
            record->duration = (uint16)(value);
            record->flags = (uint16)(397);
            result = mode == 0 ? 0x1600 : 0x1603;
        }
        record->style = (uint16)(result);
    }
    {
        (void)(uint32)(result);
        return;
    }
}

static void create_effect_burst(const SCENE_EFFECT_COMMAND *command, uint32 count, uint32 mode)
{
    EFFECT_DRIFT *record;
    uint32 saved_random, seed, x, y, z, start, spread, scale, index, random, position, lifetime, first, second, third, value, format;
    FUNCTION_MARKER(0x800935AC, "ACE3.BIN");
    if ((sint32)count <= 0)
        return;
    saved_random = game_random();
    seed = command->control.info.parameter;
    random_seed = seed;
    x = command->position[0];
    y = command->position[1];
    z = command->position[2];
    start = command->arguments.signed16[0];
    spread = command->arguments.signed16[1];
    scale = command->arguments.signed16[3];
    index = 0;
    while ((sint32)index < (sint32)count)
    {
        random = game_random();
        if (mode != 0)
        {
            position = start - (index + (random & 3));
            lifetime = (((uint32)((sint32)random >> 2)) & 7) + 20;
        }
        else
        {
            position = start - (index + (random & 1));
            lifetime = (random & 7) + 8;
        }
        if ((sint32)position < 0 || (sint32)position >= (sint32)lifetime)
        {
            ++index;
            game_random();
            game_random();
            continue;
        }
        record = effect_drift_allocate_pending();
        if (record != 0)
        {
            {
                uint32 packed_record_word = position | (lifetime << 16);
                record->ticks = (uint16)packed_record_word;
                record->duration = (uint16)(packed_record_word >> 16);
            }
            random = game_random();
            first = ((random & 0x1FF) - 256) * spread;
            second = ((((uint32)((sint32)random >> 2)) & 0x1FF) - 256) * spread;
            third = ((((uint32)((sint32)random >> 4)) & 0x1FF) - 256) * spread;
            record->flags = (uint16)(0);
            record->position[0] = (uint32)(x + (first << 4));
            record->position[1] = (uint32)(y + (second << 4));
            record->position[2] = (uint32)(z + (third << 4));
            random = game_random();
            value = scale * ((((uint32)((sint32)random >> 3)) & 0x7FF) + 3072);
            record->radius = (uint16)((sint32)value >> 12);
            if (mode != 0)
                format = mode == 1 ? 0x110 : 0x210;
            else
                format = (random & 3) != 0 ? 0x100 : 0;
            record->style = (uint16)(format);
        }
        ++index;
    }
    random_seed = saved_random;
}

static void create_effect_group(const SCENE_EFFECT_COMMAND *command, uint32 count)
{
    EFFECT_DRIFT *record;
    uint32 x, y, z, kind, spread, scale, index, random, first, second, third, x_offset, y_offset, z_offset, value;
    FUNCTION_MARKER(0x800937D8, "ACE3.BIN");
    if ((sint32)count <= 0)
        return;
    x = command->position[0];
    y = command->position[1];
    z = command->position[2];
    kind = command->arguments.signed16[0];
    spread = command->arguments.signed16[1];
    scale = command->arguments.signed16[3];
    for (index = 0; (sint32)index < (sint32)count; ++index, kind += 2)
    {
        record = effect_drift_allocate(&effect_drift_secondary_pool);
        if (record == 0)
            return;
        {
            uint32 packed_record_word = kind << 16;
            record->ticks = (uint16)packed_record_word;
            record->duration = (uint16)(packed_record_word >> 16);
        }
        random = game_random();
        first = ((((uint32)((sint32)random >> 2)) & 255) - 128) * spread;
        second = ((((uint32)((sint32)random >> 4)) & 255) - 128) * spread;
        third = ((random & 255) - 128) * spread;
        x_offset = (sint32)first >> 8;
        y_offset = (sint32)second >> 8;
        record->flags = (uint16)(0x188);
        record->style = (uint16)(0);
        record->position[0] = (uint32)(x + (x_offset << 9));
        record->position[1] = (uint32)(y + (y_offset << 9));
        record->vector[1] = (uint16)(x_offset - (uint32)((sint32)first >> 10));
        record->vector[2] = (uint16)(y_offset);
        z_offset = (sint32)third >> 8;
        record->position[2] = (uint32)(z + (z_offset << 9));
        record->vector[0] = (uint16)(z_offset);
        random = game_random();
        value = scale * ((random & 0xFFF) + 2048);
        value = (sint32)value / 4096;
        record->radius = (uint16)(value);
    }
}

static void create_periodic_effect(const SCENE_EFFECT_COMMAND *command)
{
    EFFECT_DRIFT *record;
    uint32 phase, ticks, result, scale, random, value, coordinate;
    FUNCTION_MARKER(0x80093040, "ACE3.BIN");
    phase = command->control.info.parameter;
    ticks = game_runtime.mission_control.ticks;
    result = (ticks + phase) & 3;
    if (result != 0)
    {
        (void)(uint32)(result);
        return;
    }
    scale = command->arguments.signed16[3];
    record = effect_drift_allocate(&effect_drift_primary_pool);
    if (record == 0)
        return;
    {
        uint32 packed_record_word = 0x500000;
        record->ticks = (uint16)packed_record_word;
        record->duration = (uint16)(packed_record_word >> 16);
    }
    random = game_random();
    value = scale * ((random & 63) - 32);
    coordinate = command->position[0];
    record->position[0] = (uint32)(coordinate + (uint32)((sint32)value >> 1));
    random = game_random();
    value = scale * ((random & 63) - 32);
    coordinate = command->position[1];
    record->position[1] = (uint32)(coordinate + (uint32)((sint32)value >> 1));
    random = game_random();
    value = scale * ((random & 63) - 32);
    coordinate = command->position[2];
    record->position[2] = (uint32)(coordinate + (uint32)((sint32)value >> 1));
    random = game_random();
    value = scale * ((random & 63) - 32);
    result = 0 - scale * 144;
    record->vector[1] = (uint16)((sint32)result >> 12);
    record->vector[0] = (uint16)((sint32)value >> 12);
    random = game_random();
    value = scale * ((random & 63) - 32);
    record->flags = (uint16)(128);
    record->style = (uint16)(0x210);
    record->vector[2] = (uint16)((sint32)value >> 12);
    random = game_random();
    value = scale * ((random & 63) + 88);
    result = (sint32)value >> 12;
    record->radius = (uint16)(result);
    {
        (void)(uint32)(result);
        return;
    }
}

static void create_even_tick_effect(const SCENE_EFFECT_COMMAND *command)
{
    EFFECT_DRIFT *record;
    uint32 result, phase, scale, random, value, x, y, z, coordinate;
    FUNCTION_MARKER(0x8009319C, "ACE3.BIN");
    result = game_runtime.mission_control.ticks & 1;
    if (result != 0)
    {
        (void)(uint32)(result);
        return;
    }
    record = effect_drift_allocate(&effect_drift_primary_pool);
    if (record == 0)
        return;
    phase = command->control.info.parameter;
    scale = command->arguments.signed16[3];
    record->ticks = (uint16)((sint32)phase / 1024);
    random = game_random();
    value = ((random & 15) + 64) * phase;
    value = (sint32)value / 4096;
    record->duration = (uint16)(value);
    x = command->arguments.signed16[0];
    y = command->arguments.signed16[1];
    z = command->arguments.signed16[2];
    record->flags = (uint16)(6);
    record->vector[0] = (uint16)(x);
    record->vector[1] = (uint16)(y);
    record->vector[2] = (uint16)(z);
    coordinate = command->position[0];
    x = coordinate + (x << 9);
    coordinate = command->position[1];
    y = coordinate + (y << 9);
    coordinate = command->position[2];
    z = coordinate + (z << 9);
    random = game_random();
    value = ((random & 255) - 128) * scale;
    record->position[0] = (uint32)(x + (uint32)((sint32)value >> 3));
    random = game_random();
    value = ((random & 255) - 128) * scale;
    record->position[1] = (uint32)(y + (uint32)((sint32)value >> 3));
    random = game_random();
    value = ((random & 255) - 128) * scale;
    record->style = (uint16)(0x111);
    record->position[2] = (uint32)(z + (uint32)((sint32)value >> 3));
    random = game_random();
    value = ((random & 15) + 240) * scale;
    result = (sint32)value >> 12;
    record->radius = (uint16)(result);
    {
        (void)(uint32)(result);
        return;
    }
}

static void create_random_effect(const SCENE_EFFECT_COMMAND *command)
{
    EFFECT_DRIFT *record;
    uint32 scale, random, value, coordinate, x, y, z, result, ax, ay, az;
    FUNCTION_MARKER(0x80092B80, "ACE3.BIN");
    scale = command->arguments.signed16[3];
    random = game_random();
    value = scale * ((random & 255) - 128);
    coordinate = command->position[0];
    x = coordinate + (uint32)((sint32)value >> 3);
    random = game_random();
    value = scale * ((random & 255) - 128);
    coordinate = command->position[1];
    y = coordinate + (uint32)((sint32)value >> 3);
    random = game_random();
    value = scale * ((random & 255) - 128);
    coordinate = command->position[2];
    z = coordinate + (uint32)((sint32)value >> 3);
    record = effect_drift_allocate(&effect_drift_primary_pool);
    if (record != 0)
    {
        record->duration = (uint16)(10);
        record->ticks = (uint16)(0);
        record->position[0] = (uint32)(x);
        record->position[1] = (uint32)(y);
        record->position[2] = (uint32)(z);
        record->flags = (uint16)(0);
        record->style = (uint16)(0x100);
        random = game_random();
        value = scale * ((random & 63) + 78);
        record->radius = (uint16)((sint32)value >> 12);
    }
    result = (command->control.flags >> 13) & 1;
    if (result == 0)
    {
        (void)(uint32)(result);
        return;
    }
    result = game_random() & 1;
    if (result == 0)
    {
        (void)(uint32)(result);
        return;
    }
    record = effect_drift_allocate(&effect_drift_secondary_pool);
    if (record == 0)
        return;
    ax = command->arguments.unsigned16[0];
    ay = command->arguments.unsigned16[1];
    az = command->arguments.unsigned16[2];
    record->ticks = (uint16)(0);
    ax <<= 16;
    ay <<= 16;
    az <<= 16;
    random = game_random();
    record->duration = (uint16)((random & 7) + 20);
    record->style = (uint16)(0);
    random = game_random();
    record->radius = (uint16)((random & 7) + 12);
    record->position[0] = (uint32)(x + (uint32)((sint32)ax >> 16));
    record->position[1] = (uint32)(y + (uint32)((sint32)ay >> 16));
    record->position[2] = (uint32)(z + (uint32)((sint32)az >> 16));
    random = game_random();
    value = (uint32)((sint32)ax >> 17) + (random & 255) - 128;
    record->vector[0] = (uint16)(value);
    random = game_random();
    value = (uint32)((sint32)ay >> 17) + (random & 255) - 128;
    record->vector[1] = (uint16)(value);
    random = game_random();
    value = (uint32)((sint32)az >> 17) + (random & 255) - 128;
    record->vector[2] = (uint16)(value);
    record->flags = (uint16)(0x188);
    return;
}

static void create_phased_effect(const SCENE_EFFECT_COMMAND *command)
{
    EFFECT_DRIFT *record;
    uint32 phase, ticks, result, scale, random, value, coordinate, x, y, z, flags, ax, ay, az;
    FUNCTION_MARKER(0x80092D90, "ACE3.BIN");
    phase = command->control.info.parameter;
    ticks = game_runtime.mission_control.ticks;
    result = (ticks + phase) & 1;
    if (result != 0)
    {
        (void)(uint32)(result);
        return;
    }
    scale = command->arguments.signed16[3];
    random = game_random();
    value = scale * ((random & 255) - 128);
    coordinate = command->position[0];
    x = coordinate + (uint32)((sint32)value >> 2);
    random = game_random();
    value = scale * ((random & 255) - 128);
    coordinate = command->position[1];
    y = coordinate + (uint32)((sint32)value >> 2);
    random = game_random();
    value = scale * ((random & 255) - 128);
    flags = command->control.flags;
    coordinate = command->position[2];
    z = coordinate + (uint32)((sint32)value >> 2);
    if ((flags & 0x4000) != 0)
    {
        random = game_random();
        if ((random & 3) != 0)
        {
            record = effect_drift_allocate(&effect_drift_primary_pool);
            if (record != 0)
            {
                record->duration = (uint16)(10);
                record->ticks = (uint16)(0);
                record->position[0] = (uint32)(x);
                record->position[1] = (uint32)(y);
                record->position[2] = (uint32)(z);
                record->flags = (uint16)(0);
                record->style = (uint16)(0x100);
                random = game_random();
                value = scale * ((random & 63) + 58);
                record->radius = (uint16)((sint32)value >> 12);
            }
        }
    }
    record = effect_drift_allocate(&effect_drift_primary_pool);
    if (record != 0)
    {
        record->ticks = (uint16)(-3);
        record->duration = (uint16)(40);
        record->position[0] = (uint32)(x);
        record->position[1] = (uint32)(y);
        record->position[2] = (uint32)(z);
        record->flags = (uint16)(0);
        record->style = (uint16)(0x210);
        random = game_random();
        value = scale * ((random & 127) + 76);
        record->radius = (uint16)((sint32)value >> 12);
    }
    result = (command->control.flags >> 13) & 1;
    if (result == 0)
    {
        (void)(uint32)(result);
        return;
    }
    result = game_random() & 1;
    if (result == 0)
    {
        (void)(uint32)(result);
        return;
    }
    record = effect_drift_allocate(&effect_drift_secondary_pool);
    if (record == 0)
        return;
    ax = command->arguments.unsigned16[0];
    ay = command->arguments.unsigned16[1];
    az = command->arguments.unsigned16[2];
    record->ticks = (uint16)(0);
    ax <<= 16;
    ay <<= 16;
    az <<= 16;
    random = game_random();
    record->duration = (uint16)((random & 7) + 20);
    record->style = (uint16)(0);
    random = game_random();
    record->radius = (uint16)((random & 7) + 12);
    record->position[0] = (uint32)(x + (uint32)((sint32)ax >> 16));
    record->position[1] = (uint32)(y + (uint32)((sint32)ay >> 16));
    record->position[2] = (uint32)(z + (uint32)((sint32)az >> 16));
    random = game_random();
    value = (uint32)((sint32)ax >> 17) + (random & 255) - 128;
    record->vector[0] = (uint16)(value);
    random = game_random();
    value = (uint32)((sint32)ay >> 17) + (random & 255) - 128;
    record->vector[1] = (uint16)(value);
    random = game_random();
    value = (uint32)((sint32)az >> 17) + (random & 255) - 128;
    record->vector[2] = (uint16)(value);
    record->flags = (uint16)(0x188);
    return;
}

static const uint8 *set_camera_pose(const uint8 *script)
{
    const SCENE_POSE_COMMAND *command;
    uint32 value;
    FUNCTION_MARKER(0x80038E98, "ACE3.BIN");
    command = (const SCENE_POSE_COMMAND *)script;
    value = command->angles[0];
    game_runtime.camera_view.pose.angles[0] = (uint16)(value);
    value = command->angles[1];
    game_runtime.camera_view.pose.angles[1] = (uint16)(value);
    value = command->angles[2];
    game_runtime.camera_view.pose.angles[2] = (uint16)(value);
    value = command->position[0];
    game_runtime.camera_view.pose.position[0] = (uint32)(value << 12);
    value = command->position[1];
    game_runtime.camera_view.pose.position[1] = (uint32)(value << 12);
    value = command->position[2];
    game_runtime.camera_view.pose.position[2] = (uint32)(value << 12);
    return script + 24;
}

static const uint8 *set_camera_projection(const uint8 *script)
{
    uint32 camera_actor_index_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    const SCENE_PROJECTION_COMMAND *command;
    uint32 value, height, near_value, far_value, changed, flags, kind, mode;
    FUNCTION_MARKER(0x80038F00, "ACE3.BIN");
    command = (const SCENE_PROJECTION_COMMAND *)script;
    value = command->command.projection.height;
    height = ((sint16)game_runtime.camera_proj.distance);
    near_value = ((sint16)game_runtime.camera_proj.near_plane);
    far_value = ((sint16)game_runtime.camera_proj.far_plane);
    changed = 0;
    if (value != 0 && height != value)
    {
        height = value;
        changed = 1;
    }
    value = command->near_clip;
    if (value != 0 && near_value != value)
    {
        near_value = value;
        changed = 2;
    }
    value = command->far_clip;
    if (value != 0 && far_value != value)
    {
        far_value = value;
        changed = 2;
    }
    if (changed == 1)
        camera_set_proj(&game_runtime.camera_proj, height);
    else if (changed == 2)
    {
        camera_set_clip(&game_runtime.camera_proj, near_value, far_value, height, game_runtime.mission_depth_buckets);
    }
    mode = 0x2000;
    flags = command->command.flags;
    actor_index_table = 0;
    kind = (flags >> 8) & 3;
    (void)mission_actor(actor_index_table);
    camera_actor_index_object = actor_index_table;
    actor_traits_object = mission_actor_traits(actor_index_table);
    if (kind != 0)
    {
        (void)mission_actor((actor_index_table + ((uint32)((sint32)(((flags >> 8) & 0xFC)) >> 2))));
        camera_actor_index_object = (actor_index_table + ((uint32)((sint32)(((flags >> 8) & 0xFC)) >> 2)));
        actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(((flags >> 8) & 0xFC)) >> 2))));
        if (kind == 1 || kind == 2)
            mode = kind;
    }
    camera_set_mode(mode, actor_traits_object, camera_actor_index_object);
    return script + 8;
}

static const uint8 *set_object_pose(const uint8 *script)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    const SCENE_POSE_COMMAND *command;
    uint32 flags, value;
    FUNCTION_MARKER(0x80039028, "ACE3.BIN");
    command = (const SCENE_POSE_COMMAND *)script;
    flags = command->flags;
    actor_index_table = 0;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(((flags >> 6) & 0xFC)) >> 2))));
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(((flags >> 6) & 0xFC)) >> 2))));
    value = actor_traits_object->flags;
    actor_traits_object->flags = (uint32)(value | 0x14000);
    value = command->angles[0];
    pose_owner_object->pose.angles[0] = (uint16)(value);
    value = command->angles[1];
    pose_owner_object->pose.angles[1] = (uint16)(value);
    value = command->angles[2];
    pose_owner_object->pose.angles[2] = (uint16)(value);
    value = command->position[0];
    pose_owner_object->pose.position[0] = (uint32)(value << 12);
    value = command->position[1];
    pose_owner_object->pose.position[1] = (uint32)(value << 12);
    value = command->position[2];
    pose_owner_object->pose.position[2] = (uint32)(value << 12);
    flags = command->flags;
    value = actor_traits_object->render_flags;
    actor_traits_object->render_flags = (uint8)((flags & 0x4000) != 0 ? value | 8 : value & 0xF7);
    return script + 24;
}

static const uint8 *set_object_transform(const uint8 *script)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    const SCENE_POSE_COMMAND *command;
    uint32 flags, value;
    FUNCTION_MARKER(0x800390E4, "ACE3.BIN");
    command = (const SCENE_POSE_COMMAND *)script;
    flags = command->flags;
    actor_index_table = 0;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(((flags >> 6) & 0xFC)) >> 2))));
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(((flags >> 6) & 0xFC)) >> 2))));
    value = actor_traits_object->flags;
    actor_traits_object->flags = (uint32)(value | 0x14000);
    value = command->angles[0];
    pose_owner_object->pose.angles[0] = (uint16)(value);
    value = command->angles[1];
    pose_owner_object->pose.angles[1] = (uint16)(value);
    value = command->angles[2];
    pose_owner_object->pose.angles[2] = (uint16)(value);
    value = command->position[0];
    pose_owner_object->pose.position[0] = (uint32)(value << 12);
    value = command->position[1];
    pose_owner_object->pose.position[1] = (uint32)(value << 12);
    value = command->position[2];
    pose_owner_object->pose.position[2] = (uint32)(value << 12);
    return script + 24;
}

static const uint8 *set_actor_pose(const uint8 *script)
{
    const SCENE_POSE_COMMAND *command;
    NATIVE_POSE pose;
    uint32 flags, index;
    FUNCTION_MARKER(0x8003916C, "ACE3.BIN");
    command = (const SCENE_POSE_COMMAND *)script;
    pose.position[0] = command->position[0] << 12;
    pose.position[1] = command->position[1] << 12;
    pose.position[2] = command->position[2] << 12;
    pose.angles[0] = command->angles[0];
    pose.angles[1] = command->angles[1];
    pose.angles[2] = command->angles[2];
    flags = command->flags;
    if (((flags >> 8) & 7) == 0)
    {
        index = (flags >> 11) & 31;
        if ((flags & 0x200000) != 0)
            effect_object_publish_pose(index, 0x1801, &pose);
        else
            effect_object_hide(index);
    }
    return script + 24;
}

static const uint8 *spawn_effect(const uint8 *script)
{
    const SCENE_EFFECT_COMMAND *command = (const SCENE_EFFECT_COMMAND *)script;
    uint32 flags, kind, count;
    FUNCTION_MARKER(0x80039230, "ACE3.BIN");
    flags = command->control.flags;
    kind = (flags >> 8) & 31;
    switch (kind)
    {
        case 0:
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
            create_pooled_effect(command, kind);
            break;
        case 8:
        case 9:
            create_render_effect(command, kind - 8);
            break;
        case 10:
            count = command->arguments.signed16[2];
            create_effect_burst(command, count, 0);
            break;
        case 11:
            count = command->arguments.signed16[2];
            create_effect_burst(command, count, 1);
            break;
        case 12:
            count = command->arguments.signed16[2];
            create_effect_burst(command, count, 2);
            break;
        case 13:
        case 17:
            create_random_effect(command);
            break;
        case 14:
        case 18:
        case 19:
            create_phased_effect(command);
            break;
        case 15:
            create_periodic_effect(command);
            break;
        case 20:
            create_even_tick_effect(command);
            break;
        case 21:
            create_drifting_effect(command, 0);
            break;
        case 22:
            create_drifting_effect(command, 1);
            break;
        case 23:
            create_drifting_effect(command, 2);
            break;
        case 31:
            create_effect_group(command, 4);
            break;
    }
    return script + 24;
}

static const uint8 *skip_command(const uint8 *script)
{
    FUNCTION_MARKER(0x80039380, "ACE3.BIN");
    return script + 4;
}

static const uint8 *emit_record(const uint8 *script)
{
    FUNCTION_MARKER(0x80039388, "ACE3.BIN");
    create_record((const SCENE_RECORD_COMMAND *)script);
    return script + 32;
}

static const uint8 *end_command(const uint8 *script)
{
    FUNCTION_MARKER(0x800393B0, "ACE3.BIN");
    return script + 4;
}

const uint8 *scene_script_execute(const uint8 *script, uint32 bytes)
{
    static const uint8 *(*const handlers[9])(const uint8 *) = {end_command, set_camera_pose, set_object_pose, set_object_transform, set_actor_pose, spawn_effect, skip_command, set_camera_projection, emit_record};
    static const uint8 command_bytes[9] = {4, 24, 24, 24, 24, 24, 4, 8, 32};
    uint32 flags, kind;
    FUNCTION_MARKER(0x800393B8, "ACE3.BIN");
    if (!script || ((intptr)script & 3) || bytes < 4)
        abort();
    flags = *(const uint32 *)script;
    kind = flags & 127;
    while (kind != 0)
    {
        if (kind >= 9 || bytes < command_bytes[kind])
            abort();
        script = handlers[kind](script);
        bytes -= command_bytes[kind];
        if (bytes < 4)
            abort();
        flags = *(const uint32 *)script;
        kind = flags & 127;
    }
    return script;
}

static SCENE_RESTORE_RECORD *restore_records;
static uint32 restore_count;

void scene_script_reset_restore(void)
{
    free(restore_records);
    restore_records = NULL;
    restore_count = 0;
}

void scene_script_bind_restore(const void *buffer, uint32 bytes)
{
    const uint8 *data = buffer;
    const uint32 *offsets;
    SCENE_RESTORE_RECORD *records;
    uint32 count, i, j, offset, end;

    if (!data || ((intptr)data & 3) || bytes < 4)
        abort();
    count = *(const uint16 *)data;
    if (count * 4 > bytes - 4)
        abort();
    offsets = (const uint32 *)(data + 4);
    records = count ? calloc(count, sizeof(*records)) : NULL;
    if (count && !records)
        abort();
    for (i = 0; i < count; ++i)
    {
        offset = offsets[i];
        if ((offset & 3) || offset < 4 + count * 4 || offset > bytes || bytes - offset < 8)
            abort();
        end = bytes;
        for (j = 0; j < count; ++j)
        {
            if (offsets[j] > offset && offsets[j] < end)
                end = offsets[j];
        }
        if (end - offset < 8)
            abort();
        records[i].frames = *(const uint32 *)(data + offset);
        records[i].commands = data + offset + 4;
        records[i].bytes = end - offset - 4;
    }
    free(restore_records);
    restore_records = records;
    restore_count = count;
}

const SCENE_RESTORE_RECORD *scene_script_get_restore(uint32 index)
{
    if (index >= restore_count)
        abort();
    return restore_records + index;
}

void scene_script_playback_ready(void)
{
    FUNCTION_MARKER(0x80039420, "ACE3.BIN");
    mission_flow.script_playback.stream_ready = (uint32)(1);
    return;
}

static const struct
{
    sint16 resource;
    uint8 restore;
} scene_playback_catalog[] = {
    {6215, 0},
    {6216, 0},
    {6217, 0},
    {6217, 0},
    {6218, 0},
    {6219, 0},
    {6220, 0},
    {6222, 0},
    {6223, 0},
    {6224, 0},
    {6225, 0},
    {6226, 1},
    {6227, 2},
    {6227, 0},
    {6228, 0},
    {6229, 0},
    {6230, 1},
    {6231, 0},
    {6232, 1},
    {6233, 0},
    {6234, 0},
    {6235, 0},
    {6236, 1},
    {6237, 2},
    {6238, 3},
    {6238, 0},
    {6239, 0},
    {6240, 1},
    {6241, 2},
    {6242, 0},
    {6243, 0},
    {6244, 1},
    {6244, 0},
    {6244, 1},
    {6245, 0},
    {6246, 1},
    {6247, 2},
    {6248, 0},
    {6249, 1},
    {6250, 0},
    {6251, 1},
    {6252, 2},
    {6253, 0},
    {6253, 1},
    {6254, 0},
    {6255, 1},
    {6256, 0},
    {6257, 1},
    {6258, 2},
    {6258, 0},
    {6258, 0},
    {6259, 0},
    {6260, 1},
    {6261, 2},
    {6262, 0},
    {6263, 1},
    {6264, 2},
    {6265, 3},
    {6266, 0},
    {6266, 1},
    {6267, 0},
    {6268, 1},
    {6269, 0},
    {6269, 0},
    {6270, 0},
    {6271, 1},
    {6272, 0},
    {6273, 1},
    {6274, 0},
    {6275, 1},
    {6275, 2},
    {6276, 0},
    {6277, 0},
    {6278, 1},
    {6279, 2},
    {6281, 0},
    {6282, 0},
    {6283, 1},
    {6284, 2},
    {6285, 0},
    {6286, 1},
    {6287, 0},
    {6287, 0},
    {6287, 0},
    {6288, 0},
    {6289, 1},
    {6290, 2},
    {6291, 0},
    {6294, 0},
    {6295, 0},
    {6296, 1},
    {6297, 0},
    {6292, 0},
    {6293, 1},
    {6221, 1},
    {6280, 3},
    /* preserve byte ids that reach adjacent original tables */
    {0, 255},
    {0, 255},
    {0, 4},
    {0, 255},
    {0, 2},
    {256, 255},
    {2, 2},
    {0, 3},
    {1, 255},
    {1, 2},
    {0, 255},
    {513, 255},
    {3, 2},
    {256, 3},
    {2, 255},
    {256, 255},
    {256, 255},
    {256, 255},
    {2, 255},
    {1, 255},
    {513, 2},
    {256, 255},
    {256, 3},
    {256, 255},
    {2, 0},
    {0, 9},
    {513, 5},
    {256, 6},
    {770, 6},
    {256, 0},
    {256, 0},
    {0, 0},
    {256, 0},
    {256, 0},
    {256, 14},
    {2, 10},
    {256, 14},
    {2, 0},
    {256, 0},
    {2, 0},
    {1, 4},
    {0, 0},
    {256, 10},
    {2, 6},
    {0, 10},
    {1, 0},
    {256, 0},
    {769, 0},
    {-1, 144},
    {-252, 53},
    {-254, 5},
    {770, 6},
    {767, 4},
    {-1, 0},
    {770, 0},
    {-1, 0},
    {-1, 12},
    {-1, 0},
    {-254, 0},
    {-253, 0},
    {2304, 14},
    {1541, 0},
    {6, 0},
    {0, 0},
    {0, 44},
    {2574, 0},
    {14, 0},
    {0, 0},
    {4, 14},
    {1546, 0},
    {10, 0},
    {0, 0},
    {13712, 12},
    {1541, 0},
    {4, 0},
    {0, 0},
    {12, 46},
    {0, 0},
    {14, 0},
    {0, 0},
    {44, 44},
    {0, 0},
    {14, 0},
    {0, 0},
    {12, 46},
    {0, 0},
    {46, 0},
    {0, 0},
    {44, 80},
    {0, 112},
    {46, 80},
    {0, 112},
    {28752, 111},
    {28752, 79},
    {20335, 111},
    {20335, 79},
    {28752, 80},
    {28752, 112},
    {20335, 80},
    {20335, 112},
    {14135, 111},
    {22359, 79},
    {14135, 111},
    {22359, 79},
    {22102, 55},
    {13878, 55},
    {22102, 87},
    {13878, 87},
    {0, 55},
    {0, 55},
    {14018, 87},
    {1550, 87},
    {14060, 86},
    {1556, 86},
    {29320, 54},
    {1554, 54},
    {0, 86},
    {0, 86},
    {21690, 54},
    {1562, 54},
    {21640, 0},
    {1560, 0},
    {26344, 0},
    {1560, 0},
    {24712, 194},
    {1570, 54},
    {26282, 14},
    {1570, 6},
    {20190, 236},
    {1554, 54},
    {23204, 20},
    {1562, 6},
    {24746, 136},
    {1566, 114},
    {26316, 18},
    {1564, 6},
    {26248, 0},
    {1570, 0},
    {15496, 0},
    {1558, 0},
    {23230, 186},
    {1562, 84},
    {27784, 26},
    {1564, 6},
    {18592, 136},
    {1560, 84},
    {20104, 24},
    {1560, 6},
    {18636, 232},
    {1554, 102},
    {20172, 24},
    {1554, 6},
    {18616, 136},
    {1556, 96},
    {17052, 34},
    {1556, 6},
    {20152, 170},
    {1556, 102},
    {17072, 34},
    {1556, 6},
};

static union
{
    RESOURCE_SECTOR_HEADER alignment;
    uint8 data[0x48C0];
} script_stream_storage;

void scene_script_playback_begin(void)
{
    uint32 mission, index, value, flags, buffer, resource, argument, result;
    const SCENE_RESTORE_RECORD *record;
    RESOURCE_STREAM_BUFFER destination;
    FUNCTION_MARKER(0x80039430, "ACE3.BIN");
    script_frames.commands = NULL;
    mission = game_runtime.playback.script_id;
    index = scene_playback_catalog[mission].restore;
    record = scene_script_get_restore(index);
    value = record->frames - 1;
    mission_flow.script_playback.total_frames = (uint32)(value);
    value = (uint16)mission_flow.script_playback.total_frames;
    mission_flow.script_playback.skip_requested = (uint32)(0);
    mission_flow.script_playback.remaining_frames = (uint16)(value);
    camera_set_movie_mode();
    flags = game_runtime.mission_control.flags;
    game_runtime.mission_control.flags = (uint32)(flags | 1);
    flags = game_runtime.flags;
    game_runtime.flags = flags | 0x10000000;
    resource_init_queue(0, 0);
    buffer = arena_alloc(0x48C0);
    mission = game_runtime.playback.script_id;
    mission_flow.script_playback.stream_ready = (uint32)(0);
    resource = (sint32)scene_playback_catalog[mission].resource;
    destination.encoded_begin = buffer;
    destination.buffer.bytes = 0x48C0;
    destination.buffer.data = script_stream_storage.data;
    resource_queue_stream(resource, destination, 0x3000, scene_script_playback_ready);
    scene_audio_init_listener();
    effect_pools_reset();
    effect_trails_reset();
    effect_objects_reset();
    subtitles_configure(1, 0, 0);
    value = (sint16)game_runtime.playback.resume_count;
    mission_flow.script_playback.active = (uint32)(0);
    if (value != 0)
    {
        mission = game_runtime.playback.script_id;
        index = mission - 5;
        if (index < 89)
        {
            if (playback_resume_skip[index])
            {
                argument = game_runtime.mission_control.argument;
                mission_request_mode(64, argument);
                mission_flow.script_playback.phase = (uint32)(5);
                return;
            }
        }
    }
    argument = game_runtime.mission_control.argument;
    result = mission_request_mode(32, argument);
    mission_flow.script_playback.phase = (uint32)(0);
    {
        (void)(uint32)(result);
        return;
    }
}

uint32 scene_script_playback_end(void)
{
    uint32 mission, index, flags, restore;
    const SCENE_RESTORE_RECORD *record;
    FUNCTION_MARKER(0x800395D4, "ACE3.BIN");
    resource_init_queue(0, 0);
    mission = game_runtime.playback.script_id;
    index = scene_playback_catalog[mission].restore;
    record = scene_script_get_restore(index);
    script_frames.commands = record->commands;
    effect_objects_clear();
    mission_actor_clear_script_flags();
    scene_script_execute(script_frames.commands, record->bytes);
    subtitles_configure(255, 0, 0);
    scene_audio_stop_all();
    flags = game_runtime.flags;
    game_runtime.flags = flags & 0xEFFFFFFF;
    mission = game_runtime.playback.script_id;
    restore = mission == 2 || mission == 4 || mission == 20 || mission == 30 || mission == 68 || mission == 85;
    return mission_actor_pause_restore(restore);
}

void scene_script_playback_update(void)
{
    uint32 phase, result, active, value, index, audio_offset, mission, remaining, flags, limit;
    RESOURCE_SECTOR_HEADER *header;
    const uint8 *frame;
    uint8 *payload;
    FUNCTION_MARKER(0x800396E8, "ACE3.BIN");
    phase = game_runtime.mission_control.mode;
    if (phase == 16)
    {
        scene_script_playback_begin();
        result = scene_script_playback_draw();
        mission_flow.script_playback.frame_index = (uint16)(0);
        mission_flow.script_playback.wait_ticks = (uint32)(0);
        {
            (void)(uint32)(result);
            return;
        }
    }
    if (phase == 64)
    {
        scene_script_playback_draw();
        result = scene_script_playback_end();
        mission_flow.script_playback.phase = (uint32)(0);
        {
            (void)(uint32)(result);
            return;
        }
    }
    value = mission_flow.script_playback.remaining_frames;
    if (value < 23)
        subtitles_configure(255, 0, 0);
    scene_script_playback_draw();
    result = resource_get_ready_frame_count();
    active = mission_flow.script_playback.active;
    if (active != 0)
    {
        if ((sint32)result < 2)
        {
            value = mission_flow.script_playback.stream_ready;
            if (value != 1)
            {
                mission_flow.script_playback.active = (uint32)(0);
                return;
            }
        }
        value = (sint16)mission_flow.script_playback.frame_index;
        if (value == 0)
        {
            result = cd_stream_get_frame(&script_frames.frame);
            if (result != 0)
            {
                result = mission_flow.script_playback.stream_ready;
                mission_flow.script_playback.active = (uint32)(0);
                if (result == 0)
                {
                    (void)(uint32)(result);
                    return;
                }
                goto stop_stream;
            }
            header = script_frames.frame.header;
            value = header->info.geometry.scripted[0];
            mission_flow.script_playback.block_frames = (uint16)(value);
        }
        index = (sint16)mission_flow.script_playback.frame_index;
        payload = script_frames.frame.payload;
        if (index >= (resource_frame_payload_bytes(script_frames.frame.header)) / 572)
            abort();
        frame = payload + index * 572;
        header = script_frames.frame.header;
        script_frames.commands = frame;
        index = mission_flow.script_playback.frame_index;
        value = header->info.frame;
        remaining = value - index;
        mission = game_runtime.playback.script_id;
        mission_flow.script_playback.remaining_frames = (uint16)(remaining);
        if (mission == 38)
        {
            remaining = mission_flow.script_playback.remaining_frames;
            value = mission_flow.script_playback.total_frames;
            effect_tick_scripted_flash(value - remaining + 2);
        }
        else if (mission == 72)
        {
            remaining = mission_flow.script_playback.remaining_frames;
            value = mission_flow.script_playback.total_frames;
            scene_script_tick_mission72(value - remaining + 2);
        }
        effect_objects_clear();
        mission_actor_clear_script_flags();
        frame = script_frames.commands;
        flags = *(const uint32 *)frame;
        effect_transients_clear();
        frame = script_frames.commands;
        scene_script_execute(frame + 4, 568);
        effect_tick_frame();
        mission = game_runtime.playback.script_id;
        if (mission != 75 && mission != 37)
        {
            frame = script_frames.commands;
            audio_offset = flags & 0xFFFFFFFC;
            if (audio_offset > 572)
                abort();
            scene_audio_apply_frame(frame + audio_offset, 572 - audio_offset);
        }
        index = mission_flow.script_playback.frame_index;
        limit = (sint16)mission_flow.script_playback.block_frames;
        ++index;
        mission_flow.script_playback.frame_index = (uint16)(index);
        result = (sint16)index < (sint32)limit;
        if (result == 0)
        {
            header = script_frames.frame.header;
            result = cd_stream_free_frame(header);
            mission_flow.script_playback.frame_index = (uint16)(0);
        }
        {
            (void)(uint32)(result);
            return;
        }
    }
    if ((sint32)result < 4)
    {
        value = mission_flow.script_playback.stream_ready;
        if (value != 1)
        {
            value = mission_flow.script_playback.wait_ticks + 1;
            mission_flow.script_playback.wait_ticks = (uint32)(value);
            if ((sint32)value < 150)
                return;
            goto stop_stream;
        }
    }
    result = mission_flow.script_playback.phase;
    mission_flow.script_playback.active = (uint32)(1);
    mission_flow.script_playback.frame_index = (uint16)(0);
    mission_flow.script_playback.wait_ticks = (uint32)(0);
    if (result == 0)
        mission_flow.script_playback.phase = (uint32)(1);
    {
        (void)(uint32)(result);
        return;
    }
stop_stream:
{
}
    value = game_runtime.mission_control.argument;
    mission_flow.script_playback.phase = (uint32)(5);
    {
        (void)(uint32)(mission_request_mode(64, value));
        return;
    }
}

uint32 scene_script_playback_draw(void)
{
    uint32 remaining, total, elapsed, buttons, phase, value, argument, direction, mission, index;
    SCENE_PLAYBACK_EFFECT effect;
    uint32 ordering, packet, result, draw_flags, strength, counter, numerator, high, quotient, threshold, shift;
    sint64 product;
    FUNCTION_MARKER(0x800399D4, "ACE3.BIN");
    remaining = mission_flow.script_playback.remaining_frames;
    total = mission_flow.script_playback.total_frames;
    buttons = game_input.pressed;
    elapsed = total - remaining + 2;
    if ((buttons & 64) != 0)
    {
        phase = mission_flow.script_playback.phase;
        mission_flow.script_playback.skip_requested = (uint32)(1);
        if (phase == 1)
            mission_flow.script_playback.phase = (uint32)(3);
        else if (phase == 2)
        {
            mission_flow.script_playback.phase = (uint32)(4);
            mission_flow.script_playback.opening_progress = (uint32)(0x1680);
            mission_flow.script_playback.closing_progress = (uint32)(0x1800);
        }
    }
    phase = mission_flow.script_playback.phase;
    if (phase - 1 < 3)
    {
        total = mission_flow.script_playback.total_frames;
        value = elapsed - 1;
        if ((sint32)total <= 0)
        {
            mission_flow.script_playback.opening_progress = (uint32)(0);
            mission_flow.script_playback.closing_progress = (uint32)(0);
        }
        else
        {
            mission_flow.script_playback.opening_progress = (uint32)(value << 7);
            mission_flow.script_playback.closing_progress = (uint32)(value << 8);
        }
        value = mission_flow.script_playback.opening_progress;
        if ((sint32)value < 0)
            mission_flow.script_playback.opening_progress = (uint32)(0);
        value = mission_flow.script_playback.closing_progress;
        if ((sint32)value < 0)
            mission_flow.script_playback.closing_progress = (uint32)(0);
        value = mission_flow.script_playback.opening_progress;
        if ((sint32)value >= 0x1800)
        {
            phase = mission_flow.script_playback.phase;
            if (phase == 1)
                mission_flow.script_playback.phase = (uint32)(2);
            else if (phase == 3)
            {
                mission_flow.script_playback.phase = (uint32)(4);
                mission_flow.script_playback.opening_progress = (uint32)(0x1680);
                mission_flow.script_playback.closing_progress = (uint32)(0x1800);
            }
        }
        remaining = mission_flow.script_playback.remaining_frames;
        if (remaining < 49)
        {
            mission_flow.script_playback.phase = (uint32)(4);
            mission_flow.script_playback.opening_progress = (uint32)(0x1680);
            mission_flow.script_playback.closing_progress = (uint32)(0x1800);
        }
    }
    else if (phase == 4)
    {
        value = mission_flow.script_playback.opening_progress - 128;
        mission_flow.script_playback.opening_progress = (uint32)(value);
        if ((sint32)value <= 0)
        {
            mission_flow.script_playback.opening_progress = (uint32)(0);
            argument = game_runtime.mission_control.argument;
            mission_flow.script_playback.phase = (uint32)(5);
            mission_request_mode(64, argument);
        }
        value = mission_flow.script_playback.skip_requested;
        if (value == 0)
        {
            remaining = mission_flow.script_playback.remaining_frames;
            if (remaining < 25)
            {
                value = mission_flow.script_playback.closing_progress;
                mission_flow.script_playback.closing_progress = (uint32)(value - 256);
            }
        }
        value = mission_flow.script_playback.closing_progress;
        if ((sint32)value <= 0)
            mission_flow.script_playback.closing_progress = (uint32)(0);
    }
    else if (phase == 0 || phase == 5)
    {
        mission_flow.script_playback.opening_progress = (uint32)(0);
        mission_flow.script_playback.closing_progress = (uint32)(0);
        mission_flow.script_playback.effect_ticks = (uint32)(0);
    }
    phase = mission_flow.script_playback.phase;
    direction = 0;
    if (phase - 4 < 2)
    {
        mission = game_runtime.playback.script_id;
        index = mission - 9;
        if (index < 83)
        {
            direction = playback_reverse_close[index];
        }
    }
    else if (phase == 1 || phase == 3 || phase == 0)
    {
        mission = game_runtime.playback.script_id;
        direction = mission == 42;
    }
    value = mission_flow.script_playback.opening_progress;
    ordering = gpu_packet_link((uintptr_t)game_runtime.frame_packets.first_tag);
    packet = game_runtime.mission_control.packet_cur;
    result = ui_draw_screen_wipe(ordering, packet, direction, value);
    value = mission_flow.script_playback.closing_progress;
    ordering = game_runtime.mission_control.ot;
    game_runtime.mission_control.packet_cur = (uint32)(result);
    result = ui_draw_letterbox(ordering, result, value - 0x800);
    game_runtime.mission_control.packet_cur = (uint32)(result);
    mission = game_runtime.playback.script_id;
    draw_flags = 0;
    strength = 0;
    index = mission - 39;
    if (index < 53)
    {
        effect = playback_effects[index];
        switch (effect)
        {
            case PLAYBACK_EFFECT_NONE:
                break;
            case PLAYBACK_EFFECT_NOISE_RAMP_151:
            case PLAYBACK_EFFECT_NOISE_RAMP_571:
            case PLAYBACK_EFFECT_FEEDBACK_RAMP_511:
            case PLAYBACK_EFFECT_FEEDBACK_RAMP_331:
                threshold = effect == PLAYBACK_EFFECT_NOISE_RAMP_571 ? 571 : effect == PLAYBACK_EFFECT_FEEDBACK_RAMP_511 ? 511 : effect == PLAYBACK_EFFECT_FEEDBACK_RAMP_331 ? 331 : 151;
                if ((sint32)elapsed >= (sint32)threshold)
                {
                    total = mission_flow.script_playback.total_frames;
                    if ((sint32)total >= (sint32)elapsed)
                    {
                        counter = mission_flow.script_playback.effect_ticks;
                        value = total - (threshold - 1);
                        shift = effect == PLAYBACK_EFFECT_FEEDBACK_RAMP_331 ? 11 : 12;
                        if (effect != PLAYBACK_EFFECT_FEEDBACK_RAMP_331)
                            draw_flags = effect == PLAYBACK_EFFECT_FEEDBACK_RAMP_511 ? 2 : 1;
                        numerator = counter << shift;
                        if (value == 0)
                            xport_mips_break(7);
                        if (value == 0xFFFFFFFF && numerator == 0x80000000)
                            xport_mips_break(6);
                        strength = (sint32)numerator / (sint32)value;
                        if (effect == PLAYBACK_EFFECT_FEEDBACK_RAMP_331)
                            draw_flags = 2;
                    }
                }
                break;
            case PLAYBACK_EFFECT_NOISE_FADE_90:
                if (elapsed - 1 < 90)
                {
                    counter = mission_flow.script_playback.effect_ticks;
                    numerator = counter << 12;
                    product = (sint64)(sint32)numerator * (sint32)0xB60B60B7;
                    high = (product >> 32);
                    quotient = (uint32)((sint32)(high + numerator) >> 6) - (uint32)((sint32)numerator >> 31);
                    draw_flags = 1;
                    strength = 0x1000 - quotient;
                }
                break;
            case PLAYBACK_EFFECT_FEEDBACK_PULSE:
                if (elapsed - 1 < 112)
                {
                    counter = mission_flow.script_playback.effect_ticks;
                    numerator = (counter * 5) << 9;
                    product = (sint64)(sint32)numerator * (sint32)0x8FB823EF;
                    value = effect_light.flags;
                    draw_flags = 2;
                    effect_light.flags = (uint32)(value | 1);
                    high = (product >> 32);
                    quotient = (uint32)((sint32)(high + numerator) >> 6) - (uint32)((sint32)numerator >> 31);
                    strength = 2560 - quotient;
                }
                if (elapsed - 113 < 16)
                {
                    draw_flags |= 1;
                    value = effect_light.flags;
                    strength = 1024;
                    effect_light.flags = (uint32)(value | 1);
                }
                break;
        }
    }
    if ((draw_flags & 1) != 0)
    {
        ordering = game_runtime.mission_control.ot;
        packet = game_runtime.mission_control.packet_cur;
        result = ui_draw_noise_overlay(ordering + 8, packet, strength);
        game_runtime.mission_control.packet_cur = (uint32)(result);
        counter = mission_flow.script_playback.effect_ticks;
        value = effect_light.flags;
        mission_flow.script_playback.effect_ticks = (uint32)(counter + 1);
        effect_light.flags = (uint32)(value | 1);
    }
    result = draw_flags & 2;
    if (result != 0)
    {
        ordering = game_runtime.mission_control.ot;
        packet = game_runtime.mission_control.packet_cur;
        result = effect_draw_screen_transition(ordering, packet, strength);
        counter = mission_flow.script_playback.effect_ticks;
        game_runtime.mission_control.packet_cur = (uint32)(result);
        mission_flow.script_playback.effect_ticks = (uint32)(counter + 1);
    }
    return result;
}

void scene_script_tick_mission72(uint32 event)
{
    FUNCTION_MARKER(0x80094380, "ACE3.BIN");
    if (event == 500)
        effect_schedule_timed_positions(0);
}

void scene_script_select_playback(uint8 index, uint8 offset)
{
    uint32 value, slot;
    FUNCTION_MARKER(0x80038054, "ACE3.BIN");
    slot = offset + (index << 2);
    value = mission_playback_choices[slot];
    game_runtime.playback.script_id = (uint8)value;
    {
        (void)(uint32)(value);
        return;
    }
}

void scene_script_select_movie(uint8 index, uint8 offset)
{
    uint32 value, slot;
    FUNCTION_MARKER(0x80038078, "ACE3.BIN");
    slot = offset + (index << 1);
    value = mission_playback_choices[MISSION_MOVIE_CHOICE_OFFSET + slot];
    game_runtime.playback.movie_idx = (uint8)(value);
    {
        (void)(uint32)(value);
        return;
    }
}
