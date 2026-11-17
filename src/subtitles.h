#ifndef SUBTITLES_H
#define SUBTITLES_H

#include "psx.h"

void subtitles_initialize(void);
uint32 subtitles_configure(uint32 id, uint32 argument0, uint32 argument1);
uint32 subtitles_draw(uint32 ot, uint32 packet);
uint32 subtitles_draw_movie(uint32 *ordering, void *packets, uint32 capacity);
uint32 subtitles_has_visible(void);

void subtitles_submit_frame(void);

#endif
