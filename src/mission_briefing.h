#ifndef MISSION_BRIEFING_H
#define MISSION_BRIEFING_H

#include "psx.h"

void mission_briefing_allocate(void);
void mission_briefing_reset_camera_motion(void);
void mission_briefing_initialize_noop(void);
uint32 mission_briefing_draw_marker(uint32 packet);
void mission_briefing_reset_objects(void);
void mission_briefing_reset_panel_frame(void);
void mission_briefing_reset_panels(void);
uint32 mission_briefing_leave(void);
uint32 mission_briefing_restore_arena(uint32 owner);
uint32 mission_briefing_begin(void);

uint32 mission_briefing_schedule_camera_move(void);
uint32 mission_briefing_set_camera_position(void);
uint32 mission_briefing_set_marker(void);
void mission_briefing_move_objects(void);
void mission_briefing_move_objects_frame(void);
void mission_briefing_move_camera(void);
void mission_briefing_event_noop(void);
uint32 mission_briefing_update_events(void);

void mission_briefing_object_event_noop(void);
uint32 mission_briefing_spawn_object(uint32 event_time);
uint32 mission_briefing_schedule_object_move(void);
uint32 mission_briefing_add_panel(void);
uint32 mission_briefing_set_panel_rows(void);
void mission_briefing_draw_objects_step(void);
void mission_briefing_draw_objects(void);
void mission_briefing_draw_objects_frame(void);
uint32 mission_briefing_draw_panels(void);
uint32 mission_briefing_intro_begin(void);
uint32 mission_briefing_intro_tick(void);
uint32 mission_briefing_prepare_playback(void);
uint32 mission_briefing_start_playback_frame(void);
uint32 mission_briefing_start_playback(void);
uint32 mission_briefing_playback_tick(void);
uint32 mission_briefing_finish_frame(void);
uint32 mission_briefing_finish_begin(void);
uint32 mission_briefing_finish_tick(void);

uint32 mission_briefing_draw_height_map(uint32 packet);

uint32 mission_briefing_frame(void);

#endif
