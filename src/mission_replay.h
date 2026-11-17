#ifndef MISSION_REPLAY_H
#define MISSION_REPLAY_H

#include "psx.h"

void mission_replay_reset_state(void);
uint32 mission_replay_begin(void);
void mission_replay_end(void);
void mission_replay_reset(void);
uint32 mission_replay_update(void);
void mission_replay_initialize(void);
uint32 mission_replay_record_frame(void);
void mission_replay_record_actors(void);

void mission_replay_step(void);

void mission_demo_select_next(void);
uint32 mission_demo_load_start(void);
uint32 mission_demo_load_finish(void);

void mission_replay_record_target(uint32 index);

void mission_replay_record_particle_spawn(uint32 index, uint32 extra, const NATIVE_POSE *pose, const uint32 *velocity);
void mission_replay_record_particle_expiration(uint32 index);
void mission_replay_record_effect_pose(uint32 index, uint32 value, const NATIVE_POSE *pose);
void mission_replay_record_effect_hide(uint32 index);
void mission_replay_record_tile_change(uint32 kind, uint32 index, uint32 value, uint32 previous);
void mission_replay_record_aircraft_effect(uint32 kind, uint32 value);




void mission_replay_record_position_effect(uint32 kind, const uint32 *native_position, uint32 variant, uint32 parameter);

void mission_replay_record_vector_effect(uint32 kind, const uint32 *native_position, const uint32 *native_vector, uint32 variant);

void mission_replay_record_surface_hit(uint32 variant, uint32 direction, const uint32 *native_position, const uint32 *native_vector);

void mission_replay_record_segment(uint32 scale, uint32 variant, const uint32 *native_start, const uint32 *direction, uint32 shift);

#endif
