#ifndef FLIGHT_MODEL_H
#define FLIGHT_MODEL_H

#include "psx.h"

typedef struct
{
    uint32 height_bound;
    uint32 reference_high;
    uint32 maximum_high;
    uint32 reference_low;
    uint32 maximum_low;
    uint32 cap_low;
    uint32 cruise_low;
} FLIGHT_ALTITUDE_SPEED_PROFILE;

typedef struct
{
    uint32 pitch_rate[2];
    uint32 yaw_rate;
    uint32 roll_rate;
    uint32 angular_response;
    uint32 rotation_damping_limit[3];
    uint32 rotation_damping[3];
    FLIGHT_ALTITUDE_SPEED_PROFILE altitude;
    uint32 thrust_gain;
    uint32 thrust_limit;
    uint32 thrust_pitch_limit;
    uint32 lift_pitch_limit[2];
    uint32 lift_pitch_factor[2];
    uint32 lift_min;
    uint32 lift_gain;
    uint32 lift_gain_high;
    uint32 linear_drag;
    uint32 target_angle_limit;
    uint32 ammunition;
    sint16 weapon_limit[2];
    sint16 target_distance_limit[2];
    uint32 max_health;
    uint16 impact_damage[2];
} FLIGHT_AIRCRAFT_PROFILE;

const FLIGHT_AIRCRAFT_PROFILE *flight_model_for_kind(sint32 kind);

#endif
