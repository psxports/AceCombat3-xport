#ifndef FLIGHT_CONTROL_H
#define FLIGHT_CONTROL_H

#include "flight_model.h"
#include "flight_dynamics.h"
#include "pad_input.h"

typedef struct
{
    uint16 axis[4];
} FLIGHT_INPUT;

typedef struct
{
    uint16 pitch;
    uint16 yaw;
    uint16 roll;
    uint16 thrust;
} FLIGHT_COMMAND;

extern FLIGHT_INPUT flight_input;
extern FLIGHT_COMMAND flight_command;


typedef struct
{
    uint16 axis[3];
    uint16 thrust;
    uint16 brake;
    uint32 emitter_bits;
    uint32 phase_origin;
} FLIGHT_CONTROL;

extern FLIGHT_CONTROL flight_control;

void flight_control_initialize(uint32 phase_origin);
void flight_control_scale_axes(sint32 distance, FLIGHT_CONTROL *vector);
void flight_control_update_standard(const GAME_INPUT *input, FLIGHT_INPUT *control, FLIGHT_COMMAND *output, FLIGHT_CONTROL *smooth);
void flight_control_update_object_input(FLIGHT_INPUT *control, FLIGHT_COMMAND *output, FLIGHT_CONTROL *smooth, uint32 enabled, const MISSION_ACTOR *native_input);
void flight_control_update_physics(const GAME_INPUT *input, FLIGHT_INPUT *control, FLIGHT_COMMAND *output, FLIGHT_CONTROL *smooth, const FLIGHT_AIRCRAFT_PROFILE *settings, const FLIGHT_DYNAMICS *physics, MISSION_ACTOR *native_object);
void flight_control_update_selection(FLIGHT_DYNAMICS *state, FLIGHT_CONTROL *smooth, const FLIGHT_RELATIVE_PROFILE *selection, MISSION_ACTOR_MOTION *actor_motion, MISSION_ACTOR *native_object);
void flight_control_update_target_option(FLIGHT_DYNAMICS *state, FLIGHT_COMMAND *control, FLIGHT_CONTROL *smooth, const FLIGHT_TARGET_PROFILE *target, uint32 option, MISSION_ACTOR_MOTION *actor_motion, MISSION_ACTOR *native_object);
uint32 flight_control_update_target(FLIGHT_DYNAMICS *state, FLIGHT_CONTROL *smooth, const FLIGHT_TARGET_PROFILE *target, MISSION_ACTOR_MOTION *actor_motion, MISSION_ACTOR *native_object);
uint32 flight_control_update_target_phase(FLIGHT_DYNAMICS *state, FLIGHT_COMMAND *control, FLIGHT_CONTROL *smooth, const FLIGHT_TARGET_PROFILE *target, uint32 option, MISSION_ACTOR_MOTION *actor_motion, MISSION_ACTOR *native_object);
void flight_control_update_object_targets(FLIGHT_INPUT *output, FLIGHT_COMMAND *control, FLIGHT_CONTROL *smooth, const MISSION_ACTOR *native_object);
void flight_control_update_input(const GAME_INPUT *input, FLIGHT_INPUT *output, FLIGHT_COMMAND *control, FLIGHT_CONTROL *smooth);
void flight_control_update_phase(FLIGHT_DYNAMICS *state, FLIGHT_INPUT *output, FLIGHT_COMMAND *control, FLIGHT_CONTROL *smooth, const MISSION_ACTOR *native_object);

void flight_control_scale_command(uint32 value, FLIGHT_COMMAND *control);

void flight_control_update_for_model(const FLIGHT_AIRCRAFT_PROFILE *settings, MISSION_ACTOR *native_object);

void flight_initialize_control_storage(void);

#endif
