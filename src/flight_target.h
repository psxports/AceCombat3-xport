#ifndef FLIGHT_TARGET_H
#define FLIGHT_TARGET_H

#include "game_math.h"
#include "flight_dynamics.h"

typedef struct
{
    GAME_FIXED_VECTOR reference_position;
    GAME_ANGLES reference_angles;
    uint32 distance;
    GAME_FIXED_VECTOR displacement;
    GAME_ANGLES angle_delta;
    uint32 result;
} FLIGHT_TARGET_STATE;

extern FLIGHT_TARGET_STATE flight_target;

uint32 flight_target_set_primary_mode(uint32 mode, uint32 kind);
uint32 flight_target_set_secondary_mode(uint32 mode, uint32 kind);

uint32 flight_target_set_tertiary_mode(uint32 mode, uint32 kind);

uint32 flight_target_classify_landing(const FLIGHT_TARGET_PROFILE *target, const NATIVE_POSE *native_position);
uint32 flight_target_outside_bounds(const FLIGHT_TARGET_PROFILE *target, const uint32 *native_position);

#endif
