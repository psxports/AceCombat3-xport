#ifndef MISSION_GROUPS_H
#define MISSION_GROUPS_H

#include "psx.h"

void mission_groups_reset(void);
void mission_groups_bind(const uint8 *source, uint32 count);
void mission_groups_update(void);
void mission_group_update(uint32 index);

#endif
