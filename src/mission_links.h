#ifndef MISSION_LINKS_H
#define MISSION_LINKS_H

#include "psx.h"

void mission_links_reset(void);
void mission_links_bind(const uint8 *source, uint32 count);
void mission_links_mark_actors(void);
void mission_links_update_poses(void);
void mission_link_activate(uint32 index);
void mission_link_deactivate(uint32 index);
void mission_links_update_health(void);
void mission_links_update_targets(void);

#endif
