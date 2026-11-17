#ifndef MISSION_STATUS_TEXT_H
#define MISSION_STATUS_TEXT_H

#include "psx.h"

void mission_status_text_init(void);
void mission_status_text_begin(void);
uint32 mission_status_text_current(void);
void mission_status_text_submit(void);
void mission_status_text_update(void);
void mission_status_text_draw(void);
void mission_status_text_reset(void);
void mission_status_text_show(uint32 kind);
void mission_status_text_frame(void);

#endif
