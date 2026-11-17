#ifndef CAMERA_H
#define CAMERA_H

#include "game_runtime.h"
#include "mission_actor.h"

void camera_clamp_position(const uint32 *first, const uint32 *second, uint32 *output);
void camera_debug_move(NATIVE_POSE *pose, uint32 input);
uint32 camera_ground_offset(const uint32 *position);
void camera_mark_dirty(void);
void camera_set_clip(GAME_CAMERA_PROJ *proj, uint32 near_value, uint32 far_value, uint32 height, uint16 *buckets);
void camera_set_mode(uint32 mode, const MISSION_ACTOR_TRAITS *actor_traits, uint32 actor_index);
void camera_set_proj(GAME_CAMERA_PROJ *proj, uint32 height);
uint32 camera_shot_dolly(const GAME_CAMERA_SHOT_CONTEXT *context, GAME_CAMERA_SHOT_CONTROL *control, uint32 duration);
void camera_shot_face_origin(const GAME_CAMERA_SHOT_CONTEXT *context, const uint32 *position, NATIVE_POSE *output);
uint32 camera_shot_hold(const GAME_CAMERA_SHOT_CONTEXT *context, GAME_CAMERA_SHOT_CONTROL *control, uint32 duration);
void camera_shot_offset_forward(const GAME_CAMERA_SHOT_CONTEXT *context, uint32 distance, uint32 scale, uint32 *native_output);
void camera_shot_offset_orbit(const GAME_CAMERA_SHOT_CONTEXT *context, uint32 scale, const uint16 *native_angles, NATIVE_POSE *output);
void camera_shot_offset_target(const GAME_CAMERA_SHOT_CONTEXT *context, const uint32 *native_offset, NATIVE_POSE *output);
uint32 camera_shot_orbit(const GAME_CAMERA_SHOT_CONTEXT *context, GAME_CAMERA_SHOT_CONTROL *control, uint32 duration);
uint32 camera_shot_overhead(const GAME_CAMERA_SHOT_CONTEXT *context, GAME_CAMERA_SHOT_CONTROL *control, uint32 duration);
void camera_shot_predict(const GAME_CAMERA_SHOT_CONTEXT *context, uint32 scale, uint32 *native_output);
void camera_shot_smooth(const uint32 *source_position, const uint16 *source_angles, uint32 *destination_position, uint16 *destination_angles);
uint32 camera_shot_speed(const GAME_CAMERA_SHOT_CONTEXT *context);
uint32 camera_shot_target(const GAME_CAMERA_SHOT_CONTEXT *context, GAME_CAMERA_SHOT_CONTROL *control, uint32 duration);
uint32 camera_shot_target_pan(const GAME_CAMERA_SHOT_CONTEXT *context, GAME_CAMERA_SHOT_CONTROL *control, uint32 duration);
uint32 camera_shot_track(const GAME_CAMERA_SHOT_CONTEXT *context, GAME_CAMERA_SHOT_CONTROL *control, uint32 duration);
uint32 camera_shot_trail(const GAME_CAMERA_SHOT_CONTEXT *context, GAME_CAMERA_SHOT_CONTROL *control, uint32 duration);
void camera_shots_set_player(const NATIVE_POSE *source, const uint32 velocity[3]);
void camera_shots_set_target(uint32 active, const NATIVE_POSE *source, const uint32 velocity[3], uint32 mode);
void camera_shots_update(void);
void camera_update(void);
void camera_vector_angles(const uint32 *vector, uint16 *angles);
void camera_bind_player(void);
void camera_draw_debug(void);
void camera_update_orbit_input(uint32 input);
void camera_initialize(uint32 projection, uint32 x, uint32 y, const uint16 angles[3]);
void camera_update_mode_1(void);
void camera_update_mode_2(void);
void camera_update_mode_2048(uint32 selected);
void camera_update_shot_input(void);
void camera_update_mode_4(void);
void camera_update_mode_16(void);
void camera_update_mode_32(void);
void camera_update_mode_64(void);
void camera_update_mode_256(void);

void camera_begin_player_loss_view(uint32 mode);

void camera_apply_shake(uint32 mode);
void camera_try_kill_view(uint32 flags);

void camera_apply_player_option(uint32 option);
void camera_apply_option(void);
void camera_enter_scripted_view(void);

void camera_set_cinematic_mode(uint32 enabled);
void camera_restore_player_mode(uint32 flags);
void camera_set_scripted_mode(void);

void camera_initialize_mission(void);
void camera_set_movie_mode(void);

#endif
