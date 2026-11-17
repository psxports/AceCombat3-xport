#ifndef DIALOGUE_VIDEO_H
#define DIALOGUE_VIDEO_H

#include "psx.h"

typedef struct
{
    uint16 x, y, w, h;
} DIALOGUE_VIDEO_RECT;

typedef struct
{
    const uint8 *data;
    uint32 bytes;
} DIALOGUE_RESOURCE_VIEW;

typedef struct
{
    const uint8 *data;
    uint32 bytes;
} DIALOGUE_TABLE_VIEW;

typedef struct
{
    uint32 prefix[3];
    POLY_FT4 quads[2][4];
    uint8 reserved[116];
    uint32 uploads[2][6528];
    uint32 tail[3];
} DIALOGUE_VIDEO_PACKETS;

typedef char dialogue_video_packet_layout_check[
    sizeof(POLY_FT4) == 40 && sizeof(DIALOGUE_VIDEO_PACKETS) == 0xCDCC &&
    offsetof(DIALOGUE_VIDEO_PACKETS, quads) == 12 &&
    offsetof(DIALOGUE_VIDEO_PACKETS, uploads) == 448 &&
    sizeof(((DIALOGUE_VIDEO_PACKETS *)0)->uploads[0]) == 26112 ? 1 : -1];

typedef struct
{
    uint32 packet_link;
    DIALOGUE_VIDEO_PACKETS *packets;
    DIALOGUE_RESOURCE_VIEW resource;
    uint32 bank;
    uint16 frame_period;
    uint16 frame_idx;
    uint32 rect_count;
    uint32 elapsed_ticks;
    uint32 quad_cursor;
    DIALOGUE_TABLE_VIEW frame_table;
    const uint8 *frame_cursor;
    DIALOGUE_TABLE_VIEW glyph_table;
    uint32 records_offset, pixels_offset;
    uint32 tile_cursor;
    uint32 draw_calls;
    DIALOGUE_VIDEO_RECT palette_rect;
    DIALOGUE_VIDEO_RECT image_rect;
    DIALOGUE_VIDEO_RECT source_rect[4];
    DIALOGUE_VIDEO_RECT screen_rect[4];
    uint16 tile_columns, tile_rows;
    uint16 tile_word_width, tile_half_rows;
} DIALOGUE_TILE_CONTROL;

typedef struct
{
    uint32 frame;
    uint32 frames;
    uint32 rect_count;
    DIALOGUE_RESOURCE_VIEW resource;
    uint32 load_bytes;
    DIALOGUE_TILE_CONTROL decoder;
    DIALOGUE_VIDEO_RECT scr[3];
    DIALOGUE_VIDEO_RECT tex[3];
} DIALOGUE_VIDEO;

extern DIALOGUE_VIDEO dialogue_video;

void dialogue_video_reset(void);
void dialogue_video_init(uint32 kind);
uint32 dialogue_video_draw(uint32 ordering, uint32 *frame_ret);
uint32 dialogue_video_draw_rect(uint32 packet, uint32 ordering, const DIALOGUE_VIDEO_RECT *screen, const DIALOGUE_VIDEO_RECT *texture);

void dialogue_video_move_capture(void);
void dialogue_video_clear_capture(void);
void dialogue_video_store_capture(uint32 available);
void dialogue_video_restore_capture(void);

#endif
