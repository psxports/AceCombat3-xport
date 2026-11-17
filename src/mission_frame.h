#ifndef MISSION_FRAME_H
#define MISSION_FRAME_H

#include "psx.h"

void mission_rebuild_render_arena(uint32 cursor);
void mission_allocate_packet_banks(uint32 cursor);
uint32 mission_frame_flight(void);
uint32 mission_frame_phase_8(void);
uint32 mission_frame_replay(void);
uint32 mission_frame_phase_10(void);
uint32 mission_frame_phase_13(void);
uint32 mission_frame_confirm(void);

void mission_transition_update(void);

uint32 mission_request_mode(uint32 mode, uint32 argument);
uint32 mission_finish_frame(void);
uint32 mission_apply_mode_transition(uint32 old_mode, uint32 old_argument);

void mission_initialize_variant(void);

void mission_apply_hud_visibility(uint32 mode);

uint32 mission_frame_overlay_movie(void);
uint32 mission_frame_movie(void);
void mission_retry_wait_start(void);
uint32 mission_retry_draw_prompt(uint32 object, uint32 packet);
void mission_confirm_update(void);

#endif
