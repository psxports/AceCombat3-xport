#ifndef EFFECT_VISUALS_H
#define EFFECT_VISUALS_H

#include "psx.h"
#include "game_math.h"

void effect_emit_debris(uint32 kind, const uint32 *native_position, const uint32 *native_vector);
void effect_emit_surface_hit(uint32 kind, const uint32 *native_position, const uint32 *native_vector, const uint16 *native_normal);
void effect_emit_burst(uint32 kind, const uint32 *native_position);
void effect_emit_trail(uint32 kind, const uint32 *native_position, const uint32 *native_vector);

void effect_emit_vector_style(uint32 kind, const uint32 *native_position, const uint32 *native_vector);

void effect_emit_position_style(uint32 value, uint32 mode, const uint32 *native_position);

void effect_emit_vector_periodic(uint32 tick, uint32 kind, const uint32 *native_vector, const uint32 *native_position);

void effect_emit_trail_periodic(uint32 tick, const uint32 *native_vector, const uint32 *native_position);

void effect_emit_position_periodic(uint32 value, uint32 mode, const uint32 *native_position);


void effect_emit_position_type0(uint32 kind, const uint32 *native_position);

void effect_emit_material_hit(uint32 material, uint32 mode, const uint32 *native_position);

void effect_emit_position_type2(uint32 tick, const uint32 *native_position);

void effect_packet_emit_segment(uint32 parameter, uint32 kind, const uint32 *native_start, const uint32 *native_end);

void effect_emit_position_type4(uint32 kind, const uint32 *native_position);


void effect_emit_height_scaled(uint32 height, const uint32 *native_vector, const uint32 *native_position);

void effect_spawn_debris(const uint32 *native_position, const uint32 *native_vector);

void effect_emit_drift_group(uint32 mode, const uint32 *native_vector, const uint32 *native_position);

void effect_spawn_surface_hit(uint32 direction, const uint32 *native_position, const uint32 *native_vector);

void effect_spawn_drift_group(uint32 mode, const uint32 *native_position, const uint32 *native_vector);

void effect_spawn_position_type0(uint32 kind, const uint32 *native_position);

void effect_spawn_vector_type7(const uint32 *native_position, const uint32 *native_vector);

void effect_emit_contact(uint32 kind, const uint32 *native_position, const uint32 *native_vector);

void effect_spawn_callback_kind(uint32 kind, const uint32 *native_position);

void effect_spawn_burst(uint32 kind, const uint32 *native_position);

void effect_spawn_position_type2(uint32 mode, const uint32 *native_position);

void effect_spawn_material_hit(uint32 kind, uint32 mode, const uint32 *native_position);

void effect_spawn_vector_type8(const uint32 *native_position, const uint32 *native_vector);

void effect_spawn_vector_type3(const uint32 *native_position, const uint32 *native_vector);

void effect_spawn_vector_type4(uint32 mode, const uint32 *native_position, const uint32 *native_vector);

void effect_emit_vector_type3(const uint32 *native_vector, const uint32 *native_position);

void effect_emit_vector_type8(const uint32 *native_vector, const uint32 *native_position);

void effect_emit_secondary_drift(uint32 kind, const uint32 *native_position, const uint32 *native_vector);




void effect_drift_emit_velocity_burst(uint32 mode, const uint32 *native_position, const uint32 *native_vector);

uint32 effect_draw_noise_overlay(uint32 ordering, uint32 packet, uint32 strength);
uint32 effect_draw_screen_transition(uint32 ordering, uint32 packet, uint32 strength);

uint32 effect_draw_signed_noise_overlay(uint32 ordering, uint32 packet, uint32 factor);
uint32 effect_draw_framebuffer_copy(uint32 ordering, uint32 packet, const XY *offset);
void effect_update_screen_overlays(EFFECT_FRAME_CONTEXT *frame);

void effect_emit_player_exhaust(uint32 mission, uint32 variant);

void effect_draw_mission_trails(void);
void effect_drift_emit_scaled(const uint32 position[3], uint32 scale);

void effect_spawn_secondary_drift_burst(uint32 x, uint32 y, uint32 z);
void effect_spawn_mixed_drift_burst(uint32 x, uint32 y, uint32 z);
void effect_spawn_scripted_drift(uint32 x, uint32 y, uint32 z, uint32 mode);
void effect_tick_scripted_flash(uint32 ticks);

void effect_emit_ground_trail(const uint32 *native_position, const uint32 *native_vector);
void effect_segment_trace_emit(uint32 scale, uint32 mode, const NATIVE_POSE *pose);

void effect_spawn_render_flash(uint32 x, uint32 y, uint32 z, uint32 parameters);
void effect_spawn_primary_group_0(uint32 x, uint32 y, uint32 z);
void effect_spawn_secondary_group_0(uint32 x, uint32 y, uint32 z);
void effect_spawn_kind_0(uint32 x, uint32 y, uint32 z);
void effect_spawn_kind_20(uint32 x, uint32 y, uint32 z);
void effect_spawn_secondary_group_1(uint32 x, uint32 y, uint32 z);
void effect_spawn_primary_group_1(uint32 x, uint32 y, uint32 z);
void effect_spawn_kind_1(uint32 x, uint32 y, uint32 z);
void effect_spawn_primary_group_2(uint32 x, uint32 y, uint32 z);
void effect_spawn_secondary_group_2(uint32 x, uint32 y, uint32 z);
void effect_spawn_kind_2(uint32 x, uint32 y, uint32 z);
void effect_spawn_primary_group_3(uint32 x, uint32 y, uint32 z);
void effect_spawn_secondary_group_3(uint32 x, uint32 y, uint32 z);
void effect_spawn_kind_3(uint32 x, uint32 y, uint32 z);
void effect_spawn_mixed_drift_group(uint32 x, uint32 y, uint32 z);
void effect_spawn_kind_8(uint32 x, uint32 y, uint32 z);
void effect_spawn_kind_13(uint32 x, uint32 y, uint32 z);
void effect_spawn_primary_group_12(uint32 x, uint32 y, uint32 z);
void effect_spawn_secondary_group_12(uint32 x, uint32 y, uint32 z);
void effect_spawn_kind_12(uint32 x, uint32 y, uint32 z);
void effect_spawn_kind_14(uint32 x, uint32 y, uint32 z);
void effect_spawn_kind_15(uint32 x, uint32 y, uint32 z);
void effect_spawn_kind_16(uint32 x, uint32 y, uint32 z);
void effect_spawn_kind_17(uint32 x, uint32 y, uint32 z);
void effect_spawn_kind_18(uint32 x, uint32 y, uint32 z);
void effect_spawn_kind_9(uint32 x, uint32 y, uint32 z);
void effect_spawn_kind_11(uint32 x, uint32 y, uint32 z);
void effect_spawn_kind_4(uint32 x, uint32 y, uint32 z);
void effect_spawn_kind_5(uint32 x, uint32 y, uint32 z);
void effect_spawn_kind_6(uint32 x, uint32 y, uint32 z);
void effect_spawn_kind_7(uint32 x, uint32 y, uint32 z);

uint32 effect_draw_scene_frame(uint32 ot, uint32 packet);

#endif
