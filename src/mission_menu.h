#ifndef MISSION_MENU_H
#define MISSION_MENU_H

#include "psx.h"

uint32 mission_menu_draw_mode(uint32 ordering, uint32 packet, uint32 mode);
uint32 mission_menu_draw_options(uint32 ordering, uint32 packet);
uint32 mission_menu_draw_confirm(uint32 ordering, uint32 packet);
uint32 mission_menu_draw_status_banner(uint32 ordering, uint32 packet);

void mission_menu_update_game_options(void);

#endif
