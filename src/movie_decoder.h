#ifndef MOVIE_DECODER_H
#define MOVIE_DECODER_H

#include "psx.h"
#include "psx_press.h"
#include "resource_ring.h"
#include "resource_queue.h"

typedef struct
{
    uint8 vlc_index;
    uint8 frame_index;
    uint8 display_index;
    uint8 ready;
    uint8 done;
    uint8 mode;
} MOVIE_PLAYBACK;

typedef struct
{
    PSX_RECT page[2];
    PSX_RECT strip;
    sint16 width;
    sint16 height;
    uint16 frame;
    sint16 words;
    uint32 resource;
    RESOURCE_STREAM_BUFFER stream;
} MOVIE_SURFACE;







typedef struct
{
    uint32 words[2048];
} SCENE_MOVIE_GPU_PAGE;


typedef struct
{
    uint16 start_tick;
    uint16 end_tick;
    uint16 parameters[4];
    uint8 mode;
} SCENE_MOVIE_RECORD;

typedef struct
{
    uint32 ordering_bank[2];
    uint32 phase;
    uint32 packet_bank[2];
    uint32 packet_cursor;
    uint32 ot;
    SCENE_MOVIE_GPU_PAGE *packet_data[2];
    SCENE_MOVIE_GPU_PAGE *packet_native;
    uint32 packet_begin;
    uint32 *ordering_data[2];
    uint32 *ot_native;
    uint16 width;
    uint16 height;
    uint32 resource;
    SCENE_MOVIE_RECORD records[2][5];
    uint8 textures[2][20];
    uint8 decode_bank;
    uint8 render_bank;
    uint8 stream_finished;
    struct
    {
        uint8 pending_frame;
        uint8 render_active;
        uint32 frame_ticks;
        uint16 frame_count;
        uint8 effect_mode;
        uint32 effect_ticks;
        uint8 record_index;
    } stream_playback;
    uint32 timeout;
} SCENE_MOVIE_CONTROL;

typedef struct
{
    const uint8 *data;
    uint32 bytes;
    uint32 enabled;
    uint32 pending;
} MOVIE_CAPTION_CONTROL;

typedef struct
{
    MOVIE_CAPTION_CONTROL primary;
    uint32 secondary_enabled;
    uint32 display_page;
} MOVIE_CAPTION_STATE;

extern MOVIE_CAPTION_STATE movie_captions;

extern const PSX_RECT movie_frame_capture_rect;
extern const PSX_RECT movie_strip_capture_rect;
extern SCENE_MOVIE_CONTROL scene_movie;

uint32 *scene_movie_native_ot_span(uint32 address, size_t bytes);
void *scene_movie_native_packet_span(uint32 address, size_t bytes);





typedef enum
{
    MOVIE_CATALOG_RESOURCE,
    MOVIE_CATALOG_CAPTION_KIND,
    MOVIE_CATALOG_CAPTION_RESOURCE,
} MOVIE_CATALOG_FIELD;

sint16 movie_catalog_value(uint8 index, MOVIE_CATALOG_FIELD field);
void movie_decoder_reset(void);
#if AP_WEB && defined(_DEBUG)
uint32 movie_control_debug_phase(void);
#endif
uint32 overlay_movie_display_page(void);
void overlay_movie_reset_bookmark(void);
void movie_stop_output(void);
uint32 movie_caption_load_poll(void);
uint32 scene_movie_begin_menu(void);
uint32 scene_movie_update_menu(void);
void scene_movie_update_mission(void);


void overlay_movie_control_begin(void);
uint32 overlay_movie_control_frame(uint32 mask);
uint32 overlay_movie_control_tick(uint32 mask);
void overlay_movie_update_image_mode(uint32 mask);

void mission_movie_overlay_frame(void);
void mission_movie_update_frame(void);
void mission_movie_overlay_tick(void);

void mission_movie_overlay_begin(void);
void mission_movie_overlay_end(void);
uint32 scene_movie_draw_stream_effects(uint32 packet, uint32 ordering);
uint32 scene_movie_draw_base_frame(uint32 packet, uint32 ordering);
uint32 scene_movie_draw_effect_mode_one(uint32 packet, uint32 ordering);
uint32 scene_movie_draw_effect_modes_two_to_five(uint32 packet, uint32 ordering);
uint32 scene_movie_draw_effect_mode_six(uint32 packet, uint32 ordering);
uint32 scene_movie_draw_effect_mode_seven(uint32 packet, uint32 ordering);

void movie_caption_read_complete(void);
uint32 movie_frontend_prepare(void);
uint32 movie_frontend_initialize_display(void);
void movie_restore_capture(void);
uint32 movie_frontend_frame(void);
uint32 movie_current_buffer(const MOVIE_PLAYBACK *player);
void movie_end(void);

#endif
