#ifndef FLIGHT_PHYSICS_H
#define FLIGHT_PHYSICS_H

#include "flight_model.h"
#include "flight_dynamics.h"
#include "flight_control.h"
#include "game_math.h"

typedef struct
{
    uint16 response[3];
    GAME_ANGLES angle_delta;
    GAME_ANGLES velocity_angles;
} FLIGHT_ORIENTATION;

extern FLIGHT_ORIENTATION flight_orientation;

void flight_physics_integrate_primary(const FLIGHT_COMMAND *control, const FLIGHT_AIRCRAFT_PROFILE *settings, FLIGHT_DYNAMICS *state, MISSION_ACTOR_MOTION *actor_motion, MISSION_ACTOR *native_object);
void flight_physics_integrate_secondary(const FLIGHT_COMMAND *control, const FLIGHT_AIRCRAFT_PROFILE *settings, FLIGHT_DYNAMICS *state, MISSION_ACTOR_MOTION *actor_motion, MISSION_ACTOR *native_object);

void flight_orientation_update_delta(FLIGHT_ORIENTATION *orientation, const MISSION_ACTOR *actor);
void flight_orientation_apply_object(FLIGHT_ORIENTATION *orientation, MISSION_ACTOR_TRAITS *actor_traits, MISSION_ACTOR_MOTION *actor_motion, MISSION_ACTOR *native_object);

uint32 flight_model_sample_altitude(uint32 value, const FLIGHT_AIRCRAFT_PROFILE *settings, uint32 *out0, uint32 *out1, uint32 *out2);

void flight_sample_ai_altitude_speeds(uint32 altitude, uint32 *first, uint32 *second, uint32 *third);

uint32 flight_thrust_response(uint32 value, uint32 maximum, uint32 middle, uint32 offset, uint32 speed, uint32 positive, uint32 negative);

#endif
