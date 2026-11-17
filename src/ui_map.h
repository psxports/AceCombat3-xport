#ifndef UI_MAP_H
#define UI_MAP_H

#include "psx.h"

void ui_map_reset(void);
void ui_map_initialize_packets(void);
void ui_map_begin(uint32 enabled, uint32 mode);
void ui_map_tick(void);
void ui_map_spread(void);
void ui_map_render(void);

void ui_map_spread_frame(void);

void ui_map_tick_frame(void);
void ui_map_draw_grid(void);

#endif
