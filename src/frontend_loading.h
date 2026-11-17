#ifndef FRONTEND_LOADING_H
#define FRONTEND_LOADING_H

#include "psx.h"

uint32 frontend_load_begin(void);
uint32 frontend_load_mission(void);
uint32 frontend_load_poll_mission(void);
uint32 frontend_load_common(void);
uint32 frontend_load_poll_common(void);

uint32 frontend_draw_loading_transition(uint32 object, uint32 dst, uint32 mission);

uint32 frontend_draw_loading_stars(uint32 ordering, uint32 packet);

uint32 frontend_draw_animated_loading_stars(uint32 ordering, uint32 packet);

uint32 frontend_draw_loading_labels(uint32 object, uint32 dst, uint32 mission);
uint32 frontend_draw_loading_instruments(uint32 object, uint32 dst, uint32 mission);
void frontend_draw_loading_screen(uint32 mission);
void mission_loading_frame_begin(void);

#endif
