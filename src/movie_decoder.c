#include <stdlib.h>
#include <string.h>

#include "game_functions.h"
#include "psx_mdec.h"

static void movie_present_play(void);

static void overlay_movie_stream_complete(void);

static const PSX_RECT movie_fade_rect = {32, 100, 256, 280};

const PSX_RECT movie_frame_capture_rect = {320, 0, 160, 496};
const PSX_RECT movie_strip_capture_rect = {0, 240, 320, 16};
static const PSX_RECT movie_backdrop_capture_rect = {768, 0, 256, 284};

static const sint16 movie_catalog[][3] = {
    {2534, 0, 0},
    {2534, 0, 0},
    {2048, 2, 103},
    {2053, 0, 0},
    {2049, 0, 0},
    {2534, 0, 0},
    {2050, 1, 0},
    {6176, 1, 104},
    {2051, 2, 105},
    {2052, 0, 0},
    {2053, 0, 0},
    {2054, 0, 0},
    {6177, 1, 0},
    {2534, 0, 0},
    {2054, 0, 0},
    {2534, 0, 0},
    {6178, 1, 0},
    {2534, 0, 0},
    {2055, 2, 106},
    {2056, 2, 107},
    {2054, 0, 0},
    {2057, 2, 108},
    {2054, 0, 0},
    {2058, 2, 109},
    {2534, 0, 0},
    {2059, 2, 110},
    {2054, 0, 0},
    {2060, 1, 0},
    {2061, 2, 111},
    {2534, 0, 0},
    {2083, 0, 0},
    /* byte indices above the movie catalog read adjacent original data */
    {31952, -32763, 32308},
    {32444, -32763, -32316},
    {-19704, -32763, -19228},
    {4096, 0, 0},
    {4096, 0, 0},
    {4096, 0, 0},
    {0, 0, 0},
    {0, -20, 0},
    {0, 20, 180},
    {-90, 20, -30},
    {90, 20, -30},
    {-256, 256, -255},
    {-254, 770, -253},
    {0, 0, -11032},
    {-10632, -32760, -11968},
    {-11708, -32760, -11448},
    {0, 0, -16224},
    {-13868, -32760, 0},
    {0, 0, 0},
    {0, 0, 0},
    {-28612, -32762, -28328},
    {-25320, -32762, -24760},
    {-25236, -32762, -27444},
    {-27964, -32762, -26924},
    {-26368, -32762, -25956},
    {-25460, -32762, -25208},
    {-25072, -32762, -24896},
    {-24640, -32762, 0},
    {5, 6, 6},
    {0, 2, 5},
    {0, 2, 6},
    {3, 1, 0},
    {1, 0, 1},
    {2, 0, 0},
    {1, 2, 0},
    {1, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 0},
    {0, 1, 68},
    {52, 0, 12},
    {400, 0, 1024},
    {10, 0, 10},
    {10, 0, 128},
    {128, 0, 128},
    {15536, -1, -20480},
    {8192, 5, -12288},
    {-28672, 5, 28672},
    {4096, 2, 1800},
    {2500, 0, 450},
    {-1592, -1, 796},
    {8192, 0, 1024},
    {2048, 0, 5120},
    {512, 0, 180},
    {768, 0, 100},
    {4, 4, 8192},
    {100, 0, 4},
    {48, 0, 44},
    {16, 0, 340},
    {684, 0, 10},
    {10, 0, 10},
    {128, 0, 128},
    {128, 0, 7536},
    {20480, 4, -32768},
    {-12288, 0, 24576},
    {4096, 6, 4096},
    {1900, 0, 2580},
    {624, 0, -1592},
    {1592, 0, 8192},
    {1024, 0, 1280},
    {5120, 0, 512},
    {180, 0, 768},
    {199, 0, 12},
    {8192, 8192, 100},
    {2, 19, 44},
    {36, 0, 12},
    {300, 0, 524},
    {10, 0, 10},
    {10, 0, 128},
    {128, 0, 128},
    {7536, -1, 20480},
    {-32768, 5, -12288},
    {24576, 6, 4096},
    {4096, 2, 1200},
    {2580, 0, 624},
    {-1592, -1, 1592},
    {8192, 0, 1024},
    {1280, 0, 5120},
    {512, 0, 140},
    {768, 0, 72},
    {2, 2, 8192},
    {100, 0, 2},
    {48, 0, 44},
    {16, 0, 340},
    {524, 0, 10},
    {10, 0, 10},
    {128, 0, 128},
    {128, 0, 15536},
    {20480, 4, -32768},
    {-12288, 0, 24576},
    {4096, 6, 4096},
    {1200, 0, 2660},
    {639, 0, -1592},
    {1592, 0, 8192},
    {1024, 0, 1280},
    {5120, 0, 512},
    {140, 0, 768},
    {80, 0, 2},
    {8192, 9150, 100},
    {2, 24, 48},
    {44, 0, 16},
    {340, 0, 684},
    {10, 0, 10},
    {10, 0, 128},
    {128, 0, 128},
    {15536, -1, -8192},
    {-4096, 4, -12288},
    {12288, 5, 0},
    {4096, 2, 1200},
    {2200, 0, 525},
    {-1592, -1, 1592},
    {8192, 0, 1024},
    {1280, 0, 5120},
    {512, 0, 180},
    {768, 0, 96},
    {4, 4, 8192},
    {100, 0, 2},
    {48, 0, 44},
    {16, 0, 340},
    {684, 0, 10},
    {10, 0, 10},
    {128, 0, 128},
    {128, 0, 15536},
    {20480, 4, -32768},
    {-12288, 0, 24576},
    {4096, 6, 4096},
    {1200, 0, 2580},
    {624, 0, -1592},
    {1592, 0, 8192},
    {1024, 0, 1280},
    {5120, 0, 512},
    {180, 0, 768},
    {80, 0, 2},
    {9010, 6436, 100},
    {2, 24, 48},
    {44, 0, 16},
    {340, 0, 684},
    {10, 0, 10},
    {10, 0, 128},
    {128, 0, 128},
    {7536, -1, 20480},
    {-32768, 5, -12288},
    {24576, 6, 4096},
    {4096, 2, 1900},
    {2580, 0, 624},
    {-1592, -1, 1592},
    {8192, 0, 1024},
    {1280, 0, 5120},
    {512, 0, 180},
    {788, 0, 88},
    {2, 2, 8192},
    {100, 0, 3},
    {36, 0, 28},
    {12, 0, 220},
    {768, 0, 10},
    {10, 0, 10},
    {128, 0, 128},
    {128, 0, 20536},
    {0, 2, 16384},
    {-16384, 0, 28672},
    {20480, 3, 4096},
    {900, 0, 1848},
    {450, 0, -1592},
    {1592, 0, 8192},
    {1024, 0, 1280},
    {5120, 0, 512},
    {245, 0, 768},
    {104, 0, 6},
    {8192, 9150, 100},
    {1, 17, 48},
    {44, 0, 16},
    {340, 0, 768},
    {10, 0, 10},
    {10, 0, 128},
    {128, 0, 128},
    {7536, -1, 8192},
    {-8192, 5, -12288},
    {24576, 7, 16384},
    {4096, 2, 1200},
    {2580, 0, 624},
    {-1592, -1, 1592},
    {8192, 0, 1024},
    {1280, 0, 5120},
    {512, 0, 180},
    {768, 0, 96},
    {4, 4, 8192},
    {100, 0, 1},
    {48, 0, 44},
    {16, 0, 340},
    {684, 0, 10},
    {10, 0, 10},
    {128, 0, 128},
    {128, 0, 7536},
    {8192, 5, -8192},
    {-12288, 0, 24576},
    {16384, 6, 4096},
    {1900, 0, 2580},
    {624, 0, -1592},
    {1592, 0, 8192},
    {1024, 0, 1280},
    {5120, 0, 512},
    {180, 0, 788},
    {112, 0, 6},
    {8192, 8192, 100},
    {2, 19, 44},
    {36, 0, 12},
    {300, 0, 768},
    {10, 0, 10},
    {10, 0, 128},
    {128, 0, 128},
    {20536, -1, -8192},
    {-4096, 4, -12288},
    {12288, 5, 0},
    {4096, 2, 1200},
    {2580, 0, 624},
};

sint16 movie_catalog_value(uint8 index, MOVIE_CATALOG_FIELD field)
{
    return movie_catalog[index][field];
}

typedef struct
{
    uint16 x;
    uint16 y;
} MOVIE_PAGE;

typedef struct
{
    uint32 phase;
    MOVIE_PAGE page[2];
    uint16 width;
    uint16 height;
    uint16 offset_x;
    uint16 offset_y;
    uint32 resource;
    uint32 mode;
    uint32 capture_valid;
    PSX_RECT clear_rect;
    uint32 startup_frames;
} MOVIE_CONTROL;

static MOVIE_CONTROL movie_control;

static struct
{
    uint32 ordering[2];
    uint32 packets[32];
} movie_caption_packets;

static uint32 movie_capture_pixels[160 * 496 / 2];

#if AP_WEB && defined(_DEBUG)
uint32 movie_control_debug_phase(void)
{
    return movie_control.phase;
}
#endif
SCENE_MOVIE_CONTROL scene_movie;

typedef struct
{
    uint32 ordering[5];
    SCENE_MOVIE_GPU_PAGE packets;
} SCENE_MOVIE_GPU_STORAGE;

static SCENE_MOVIE_GPU_STORAGE scene_movie_gpu_storage[2];

uint32 *scene_movie_native_ot_span(uint32 address, size_t bytes)
{
    uint32 index;

    for (index = 0; index < 2; ++index)
    {
        uint32 base = scene_movie.ordering_bank[index];
        size_t capacity = sizeof(scene_movie_gpu_storage[index].ordering);

        if (scene_movie.ordering_data[index] && bytes && address >= base && !((address - base) & 3) && address - base <= capacity && bytes <= capacity - (address - base))
            return scene_movie.ordering_data[index] + (address - base) / sizeof(uint32);
    }
    return NULL;
}

void *scene_movie_native_packet_span(uint32 address, size_t bytes)
{
    uint32 index;

    for (index = 0; index < 2; ++index)
    {
        uint32 base = scene_movie.packet_bank[index];
        size_t capacity = sizeof(*scene_movie.packet_data[index]);

        if (scene_movie.packet_data[index] && bytes && address >= base && address - base <= capacity && bytes <= capacity - (address - base))
            return (uint8 *)scene_movie.packet_data[index] + (address - base);
    }
    return NULL;
}

static struct
{
    uint32 strip[320 * 16 / 2];
    uint32 backdrop[256 * 284 / 2];
    uint32 strip_valid;
    uint32 backdrop_valid;
} scene_movie_capture;

static void scene_movie_bind_stream(uint32 resource, uint32 buffer, uint32 bytes);
static void scene_movie_initialize_stream(void);
static void scene_movie_queue_stream(uint32 id, RESOURCE_STREAM_BUFFER destination);
static void scene_movie_complete_stream(void);
static void scene_movie_reset_queue(void);
static void scene_movie_stream_idle(void);
static uint32 scene_movie_wait_frame(uint32 nonblocking);

typedef struct
{
    MOVIE_SURFACE surface;
    MOVIE_PLAYBACK playback;
    uint32 draining;
    uint32 arena_bookmark;
} OVERLAY_MOVIE_PLAYER;

MOVIE_CAPTION_STATE movie_captions;
static uint8 *primary_caption_resource;
static OVERLAY_MOVIE_PLAYER overlay_movie_player;

uint32 overlay_movie_display_page(void)
{
    return overlay_movie_player.playback.display_index;
}

void overlay_movie_reset_bookmark(void)
{
    overlay_movie_player.arena_bookmark = 0;
}

void movie_decoder_reset(void)
{
    memset(&movie_control, 0, sizeof(movie_control));
    memset(&scene_movie, 0, sizeof(scene_movie));
    free(primary_caption_resource);
    primary_caption_resource = NULL;
    memset(&movie_captions, 0, sizeof(movie_captions));
    memset(&overlay_movie_player, 0, sizeof(overlay_movie_player));
    movie_control.capture_valid = 0;
    scene_movie_capture.strip_valid = 0;
    scene_movie_capture.backdrop_valid = 0;
}

static void overlay_movie_prepare(uint32 resource, uint32 mode, uint32 arena, sint16 x0, sint16 y0, sint16 x1, sint16 y1)
{
    uint32 value;

    if (arena & 3)
        abort();
    overlay_movie_player.arena_bookmark = arena;
    value = resource;
    overlay_movie_player.surface.resource = (uint32)(value);
    overlay_movie_player.surface.stream = (RESOURCE_STREAM_BUFFER){0};
    value = (uint8)mode;
    overlay_movie_player.playback.mode = (uint8)(value);
    value = (uint16)x0;
    overlay_movie_player.surface.page[0].x = (sint16)(value);
    value = (uint16)y0;
    overlay_movie_player.surface.page[0].y = (sint16)(value);
    value = (uint16)x1;
    overlay_movie_player.surface.page[1].x = (sint16)(value);
    value = (uint16)y1;
    overlay_movie_player.surface.page[0].w = (sint16)(0);
    overlay_movie_player.surface.page[0].h = (sint16)(0);
    overlay_movie_player.surface.page[1].w = (sint16)(0);
    overlay_movie_player.surface.page[1].h = (sint16)(0);
    overlay_movie_player.surface.page[1].y = (sint16)(value);
    value = (uint16)x0;
    overlay_movie_player.surface.strip.x = (sint16)(value);
    value = (uint16)y0;
    overlay_movie_player.surface.strip.y = (sint16)(value);
    value = mode;
    overlay_movie_player.surface.strip.w = (sint16)(value ? 24 : 16);
    overlay_movie_player.playback.frame_index = (uint8)(0);
    overlay_movie_player.playback.vlc_index = (uint8)(0);
    overlay_movie_player.surface.strip.h = (sint16)(0);
    overlay_movie_player.surface.height = (sint16)(0);
    overlay_movie_player.surface.width = (sint16)(0);
    overlay_movie_player.surface.words = (sint16)(0);
    overlay_movie_player.playback.display_index = (uint8)(0);
    overlay_movie_player.playback.ready = (uint8)(0);
    overlay_movie_player.playback.done = (uint8)(0);
    overlay_movie_player.draining = 0;
    overlay_movie_player.playback.ready = 1;
    resource_init_queue(0, 0);
}

static void overlay_movie_queue(void)
{
    game_movie_prepare(overlay_movie_player.surface.resource, &overlay_movie_player.playback, &overlay_movie_player.surface);
    resource_queue_stream(overlay_movie_player.surface.resource, overlay_movie_player.surface.stream, 0x1000, overlay_movie_stream_complete);
}

typedef struct
{
    const uint8 *data;
    uint32 bytes;
} MOVIE_CAPTION_VIEW;

static MOVIE_CAPTION_VIEW movie_caption_child(MOVIE_CAPTION_VIEW parent, uint32 index)
{
    uint16 wire_halfword_0;
    uint32 wire_word_1;
    MOVIE_CAPTION_VIEW child;
    uint16 count;
    uint32 offset, header_bytes;

    if (!parent.data || parent.bytes < 4)
        abort();
    count = (memcpy(&wire_halfword_0, parent.data, sizeof(wire_halfword_0)), wire_halfword_0);
    if (count > (parent.bytes - 4) / 4 || index >= count)
        abort();
    header_bytes = 4 + (uint32)count * 4;
    offset = (memcpy(&wire_word_1, parent.data + 4 + index * 4, sizeof(wire_word_1)), wire_word_1);
    if (offset < header_bytes || offset > parent.bytes || (offset & 3))
        abort();
    child.data = parent.data + offset;
    child.bytes = parent.bytes - offset;
    return child;
}

static void movie_caption_upload_images(MOVIE_CAPTION_VIEW resource)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    MOVIE_CAPTION_VIEW images, tim;
    GpuTimImage image = {0};
    uint32 count, index, tag, accepted;

    images = movie_caption_child(resource, 0);
    if (images.bytes < 4)
        abort();
    count = (memcpy(&wire_word_0, images.data, sizeof(wire_word_0)), wire_word_0);
    if ((sint32)count <= 0)
        return;
    if (count > 0xFFFF || count > (images.bytes - 4) / 4)
        abort();
    index = 0;
    do
    {
        tim = movie_caption_child(images, index);
        accepted = 0;
        if (tim.bytes >= 8)
        {
            tag = (memcpy(&wire_word_1, tim.data, sizeof(wire_word_1)), wire_word_1);
            if (tag == 0x10)
                accepted = gpu_decode_tim_body(tim.data + 4, tim.bytes - 4, &image);
        }
        if (!accepted)
        {
#ifndef NDEBUG
            printf("Can't read TIM [%p]\n", (void *)tim.data);
#endif
        }
        if (image.pixel_data)
            LoadImagePSX(&image.pixels, (uint32 *)image.pixel_data);
        if (image.clut_data)
            LoadImagePSX(&image.clut, (uint32 *)image.clut_data);
        ++index;
    } while ((sint32)index < (sint32)count);
}

uint32 movie_caption_load_poll(void)
{
    MOVIE_CAPTION_VIEW resource;

    FUNCTION_MARKER(0x80057E34, "ACE3.BIN");
    if (!movie_captions.primary.enabled)
        return 0x04000000;
    resource.data = movie_captions.primary.data;
    if (!resource.data)
        return 0x04000000;
    if (movie_captions.primary.pending)
        return 0;
    resource.bytes = movie_captions.primary.bytes;
    movie_caption_upload_images(resource);
    DrawSync(0);
    free(primary_caption_resource);
    primary_caption_resource = NULL;
    movie_captions.primary.data = NULL;
    movie_captions.primary.bytes = 0;
    return 0x04000000;
}

static uint32 scene_movie_render_frame(void)
{
    PSX_RECT rectangle;
    uint32 value, packet, ordering, time, phase, intensity, result;
    FUNCTION_MARKER(0x8005911C, "ACE3.BIN");
    value = scene_movie.stream_playback.pending_frame;
    if (value != 0)
    {
        if (value == 1)
        {
            value = scene_movie.render_bank;
            scene_movie.render_bank = (uint8)(value ^ 1);
            value = scene_movie.stream_playback.frame_count;
            scene_movie.stream_playback.frame_count = (uint16)(value + 1);
        }
        else
        {
            scene_movie.render_bank = (uint8)(0);
            scene_movie.stream_playback.frame_count = (uint16)(0);
            scene_movie.stream_playback.render_active = (uint8)(1);
        }
        scene_movie.stream_playback.pending_frame = (uint8)(0);
        scene_movie.stream_playback.record_index = (uint8)(0);
        scene_movie.stream_playback.frame_ticks = (uint32)(0);
        scene_movie.stream_playback.effect_mode = (uint8)(0);
    }
    value = scene_movie.stream_playback.render_active;
    if (value == 1)
    {
        packet = scene_movie.packet_cursor;
        ordering = scene_movie.ot;
        result = scene_movie_draw_stream_effects(packet, ordering);
    }
    else
    {
        rectangle = movie_fade_rect;
        time = scene_movie.stream_playback.frame_ticks;
        phase = time - 30;
        if (phase < 181)
        {
            if (phase < 64)
                intensity = 4096 - (phase << 6);
            else if (time - 94 < 52)
                intensity = 0;
            else if (time - 146 < 64)
                intensity = (time - 146) << 6;
            else
                intensity = 4096;
            if ((sint32)intensity < 0)
                intensity = 0;
            ordering = scene_movie.ot;
            packet = scene_movie.packet_cursor;
            result = ui_draw_noise_rectangle(ordering + 12, packet, rectangle, intensity);
            scene_movie.packet_cursor = (uint32)(result);
        }
        packet = scene_movie.packet_cursor;
        ordering = scene_movie.ot;
        result = hud_waveform_draw(packet, ordering);
    }
    scene_movie.packet_cursor = (uint32)(result);
    scene_texture_decode_frame();
    result = scene_movie.stream_finished;
    value = scene_movie.stream_playback.frame_ticks;
    scene_movie.stream_playback.frame_ticks = (uint32)(value + 1);
    return (result ^ 1) == 0;
}

static uint32 scene_movie_update_phase(void)
{
    PSX_RECT rectangle;
    uint32 mode, value, buffer, resource;
    FUNCTION_MARKER(0x800582F8, "ACE3.BIN");
    mode = scene_movie.phase;
    switch (mode)
    {
        case 0:
            buffer = arena_peek(0);
            resource = scene_movie.resource;
            scene_movie_bind_stream(resource, buffer, 18368);
            scene_movie.phase = (uint32)(1);
            break;
        case 1:
            value = game_runtime.display_buffer;
            if (value == 0)
            {
                game_runtime.display_buffer = 0;
                if (scene_movie_wait_frame(1) != 0)
                {
                    scene_movie.phase = (uint32)(2);
                }
                else
                {
                    value = scene_movie.timeout - 1;
                    scene_movie.timeout = (uint32)(value);
                    if ((sint32)value <= 0)
                    {
                        scene_movie.stream_finished = (uint8)(1);
                    }
                    value = scene_movie.stream_finished;
                    if (value != 0)
                    {
                        value = movie_captions.secondary_enabled;
                        if (value != 0)
                            subtitles_configure(255, 0, 0);
                        scene_movie.phase = (uint32)(3);
                    }
                }
            }
            value = scene_movie.stream_playback.frame_ticks;
            scene_movie.stream_playback.frame_ticks = (uint32)(value + 1);
            break;
        case 2:
            value = game_input.pressed;
            if ((value & 64) != 0)
            {
                scene_movie.stream_finished = (uint8)(1);
            }
            if (scene_movie_render_frame() != 0)
            {
                scene_movie.phase = (uint32)(3);
                scene_movie.stream_playback.frame_ticks = (uint32)(0);
            }
            break;
        case 3:
            scene_movie_reset_queue();
            scene_movie_stream_idle();
            rectangle.w = 320;
            rectangle.x = 0;
            rectangle.y = 0;
            rectangle.h = 480;
            ClearImage(&rectangle, 0, 0, 0);
            value = game_runtime.display_buffer;
            if (value == 0)
            {
                scene_movie.phase = (uint32)(4);
            }
            break;
        case 4:
            rectangle.w = 320;
            rectangle.x = 0;
            rectangle.y = 0;
            rectangle.h = 480;
            ClearImage(&rectangle, 0, 0, 0);
            scene_movie.phase = (uint32)(5);
            break;
        case 5:
            scene_movie.phase = (uint32)(6);
            break;
        case 6:
            value = game_runtime.flags;
            game_runtime.flags = value & 0xEFFFFFFF;
            value = game_runtime.display.flags;
            game_runtime.display.field_wait = (uint32)(2);
            game_runtime.display.mode = (uint32)(0);
            game_runtime.display.flags = (uint32)(value | 5);
            scene_movie.phase = (uint32)(7);
            break;
        case 7:
            scene_movie.phase = (uint32)(8);
            break;
        case 8:
            scene_movie.phase = (uint32)(9);
            break;
        case 9:
            return 1;
        default:
            break;
    }
    return 0;
}

static void scene_movie_begin_mission(void)
{
    uint32 flags, cursor, selection, value, enabled, mode, context;
    PSX_RECT capture, clear;
    FUNCTION_MARKER(0x8005AE8C, "ACE3.BIN");
    flags = game_runtime.flags;
    game_runtime.flags = flags | 0x10000000;
    game_runtime.display.field_wait = (uint32)(0);
    cursor = game_runtime.mission_control.render_arena;
    arena_set_cursor(cursor);
    game_runtime.display.background[0] = (uint8)(0);
    game_runtime.display.mode = (uint32)(1);
    game_runtime.display.background[1] = (uint8)(0);
    game_runtime.display.background[2] = (uint8)(0);
    flags = game_runtime.display.flags;
    game_runtime.display.flags = (uint32)(flags | 9);
    (void)arena_alloc(100);
    if (!gpu_register_packet_range(scene_movie_gpu_storage, sizeof(scene_movie_gpu_storage)))
        abort();
    scene_movie.ordering_bank[0] = gpu_packet_link((uintptr_t)scene_movie_gpu_storage[0].ordering);
    scene_movie.ordering_bank[1] = gpu_packet_link((uintptr_t)scene_movie_gpu_storage[1].ordering);
    scene_movie.ordering_data[0] = scene_movie_gpu_storage[0].ordering;
    scene_movie.ordering_data[1] = scene_movie_gpu_storage[1].ordering;
    (void)arena_alloc(0x2000);
    scene_movie.packet_bank[0] = gpu_packet_link((uintptr_t)&scene_movie_gpu_storage[0].packets);
    scene_movie.packet_data[0] = &scene_movie_gpu_storage[0].packets;
    (void)arena_alloc(0x2000);
    scene_movie.packet_bank[1] = gpu_packet_link((uintptr_t)&scene_movie_gpu_storage[1].packets);
    scene_movie.packet_data[1] = &scene_movie_gpu_storage[1].packets;
    scene_movie.height = (uint16)(480);
    scene_movie.phase = (uint32)(0);
    scene_movie.width = (uint16)(320);
    scene_movie.timeout = (uint32)(120);
    selection = game_runtime.playback.movie_idx;
    value = movie_catalog_value(selection, MOVIE_CATALOG_RESOURCE);
    scene_movie.resource = (uint32)(value);
    capture = movie_strip_capture_rect;
    scene_movie_capture.strip_valid = 1;
    StoreImage(&capture, scene_movie_capture.strip);
    capture.x = 768;
    capture.w = 256;
    capture.y = 0;
    capture.h = 284;
    scene_movie_capture.backdrop_valid = 1;
    StoreImage(&capture, scene_movie_capture.backdrop);
    clear.x = 0;
    clear.y = 0;
    clear.w = 320;
    clear.h = 496;
    ClearImage(&clear, 0, 0, 0);
    DrawSync(0);
    enabled = game_runtime.options.captions;
    flags = game_runtime.playback.caption_gate;
    if ((enabled & flags) != 0)
    {
        selection = game_runtime.playback.movie_idx;
        value = movie_catalog_value(selection, MOVIE_CATALOG_CAPTION_KIND);
        movie_captions.secondary_enabled = (uint32)(value == 1);
    }
    else
    {
        movie_captions.secondary_enabled = (uint32)(0);
    }
    value = movie_captions.secondary_enabled;
    if (value != 0)
        subtitles_configure(10, 0, 0);
    value = (sint16)game_runtime.playback.resume_count;
    if (value != 0)
    {
        selection = game_runtime.playback.movie_idx;
        if (selection == 12 || selection == 27)
        {
            game_runtime.display.field_wait = (uint32)(2);
            flags = game_runtime.display.flags;
            game_runtime.display.mode = (uint32)(0);
            game_runtime.display.flags = (uint32)(flags | 5);
            context = game_runtime.mission_control.argument;
            mode = 0x80000;
            goto switch_scene;
        }
    }
    context = game_runtime.mission_control.argument;
    mode = 0x40000;
switch_scene:
{
    (void)(uint32)(mission_request_mode(mode, context));
    return;
}
}

static void scene_movie_end_mission(void)
{
    uint32 cursor, enabled;
    PSX_RECT clear;
    FUNCTION_MARKER(0x8005B234, "ACE3.BIN");
    resource_init_queue(0, 0);
    cursor = game_runtime.mission_control.render_arena;
    arena_set_cursor(cursor);
    cursor = game_runtime.mission_control.render_arena;
    mission_rebuild_render_arena(cursor);
    clear.w = 320;
    clear.x = 0;
    clear.y = 0;
    clear.h = 480;
    ClearImage(&clear, 0, 0, 0);
    clear = movie_strip_capture_rect;
    LoadImagePSX(&clear, scene_movie_capture.strip_valid ? scene_movie_capture.strip : NULL);
    clear = movie_backdrop_capture_rect;
    LoadImagePSX(&clear, scene_movie_capture.backdrop_valid ? scene_movie_capture.backdrop : NULL);
    enabled = movie_captions.secondary_enabled;
    if (enabled != 0)
        subtitles_configure(255, 0, 0);
    {
        (void)(uint32)(mission_dispatch_scene_request());
        return;
    }
}

void scene_movie_update_mission(void)
{
    uint32 phase, buffer, ordering, packet, result, context;
    FUNCTION_MARKER(0x8005B124, "ACE3.BIN");
    phase = game_runtime.mission_control.mode;
    if (phase == 0x20000)
    {
        (void)(scene_movie_begin_mission());
        return;
    }
    if (phase == 0x80000)
    {
        (void)(scene_movie_end_mission());
        return;
    }
    buffer = game_runtime.display_buffer;
    ordering = scene_movie.ordering_bank[buffer];
    scene_movie.ot = (uint32)(ordering);
    scene_movie.ot_native = scene_movie.ordering_data[buffer];
    packet = scene_movie.packet_bank[buffer];
    scene_movie.packet_cursor = (uint32)(packet);
    scene_movie.packet_begin = packet;
    scene_movie.packet_native = scene_movie.packet_data[buffer];
    ClearOTagR(scene_movie.ot_native, 5);
    if (scene_movie_update_phase() != 0)
    {
        context = game_runtime.mission_control.argument;
        {
            (void)(uint32)(mission_request_mode(0x80000, context));
            return;
        }
    }
    if (movie_captions.secondary_enabled != 0)
    {
        ordering = scene_movie.ot;
        packet = scene_movie.packet_cursor;
        result = subtitles_draw(ordering, packet);
        scene_movie.packet_cursor = (uint32)(result);
    }
    ordering = scene_movie.ot;
    DrawOTag(scene_movie.ot_native + 4);
    return;
}

uint32 scene_movie_begin_menu(void)
{
    uint32 buffer, flags, selection, value, enabled;
    PSX_RECT capture, clear;
    FUNCTION_MARKER(0x8005B308, "ACE3.BIN");
    buffer = game_runtime.display_buffer;
    if (buffer == 0)
        return 0;
    flags = game_runtime.flags;
    game_runtime.flags = flags | 0x10000000;
    game_runtime.display.background[0] = (uint8)(0);
    game_runtime.display.field_wait = (uint32)(0);
    game_runtime.display.mode = (uint32)(1);
    game_runtime.display.background[1] = (uint8)(0);
    game_runtime.display.background[2] = (uint8)(0);
    flags = game_runtime.display.flags;
    game_runtime.display.flags = (uint32)(flags | 9);
    (void)arena_alloc(100);
    if (!gpu_register_packet_range(scene_movie_gpu_storage, sizeof(scene_movie_gpu_storage)))
        abort();
    scene_movie.ordering_bank[0] = gpu_packet_link((uintptr_t)scene_movie_gpu_storage[0].ordering);
    scene_movie.ordering_bank[1] = gpu_packet_link((uintptr_t)scene_movie_gpu_storage[1].ordering);
    scene_movie.ordering_data[0] = scene_movie_gpu_storage[0].ordering;
    scene_movie.ordering_data[1] = scene_movie_gpu_storage[1].ordering;
    (void)arena_alloc(0x2000);
    scene_movie.packet_bank[0] = gpu_packet_link((uintptr_t)&scene_movie_gpu_storage[0].packets);
    scene_movie.packet_data[0] = &scene_movie_gpu_storage[0].packets;
    (void)arena_alloc(0x2000);
    scene_movie.packet_bank[1] = gpu_packet_link((uintptr_t)&scene_movie_gpu_storage[1].packets);
    scene_movie.packet_data[1] = &scene_movie_gpu_storage[1].packets;
    scene_movie.phase = (uint32)(0);
    scene_movie.timeout = 120;
    scene_movie.width = (uint16)(320);
    scene_movie.height = (uint16)(480);
    selection = game_runtime.playback.movie_idx;
    value = movie_catalog_value(selection, MOVIE_CATALOG_RESOURCE);
    scene_movie.resource = (uint32)(value);
    capture = movie_strip_capture_rect;
    scene_movie_capture.strip_valid = 1;
    StoreImage(&capture, scene_movie_capture.strip);
    clear.x = 0;
    clear.y = 0;
    clear.w = 320;
    clear.h = 480;
    ClearImage(&clear, 0, 0, 0);
    DrawSync(0);
    enabled = game_runtime.options.captions;
    flags = game_runtime.playback.caption_gate;
    if ((enabled & flags) != 0)
    {
        selection = game_runtime.playback.movie_idx;
        value = movie_catalog_value(selection, MOVIE_CATALOG_CAPTION_KIND);
        movie_captions.secondary_enabled = (uint32)(value == 1);
    }
    else
    {
        movie_captions.secondary_enabled = (uint32)(0);
    }
    value = movie_captions.secondary_enabled;
    if (value != 0)
        subtitles_configure(10, 0, 0);
    return 0x04000000;
}

static void scene_movie_end_menu(void)
{
    uint32 enabled;
    PSX_RECT clear;
    FUNCTION_MARKER(0x8005B5BC, "ACE3.BIN");
    resource_init_queue(0, 0);
    clear.w = 320;
    clear.x = 0;
    clear.y = 0;
    clear.h = 480;
    ClearImage(&clear, 0, 0, 0);
    clear = movie_strip_capture_rect;
    LoadImagePSX(&clear, scene_movie_capture.strip_valid ? scene_movie_capture.strip : NULL);
    enabled = movie_captions.secondary_enabled;
    if (enabled != 0)
        subtitles_configure(255, 0, 0);
    return;
}

uint32 scene_movie_update_menu(void)
{
    uint32 buffer, ordering, packet, result;
    FUNCTION_MARKER(0x8005B4E4, "ACE3.BIN");
    buffer = game_runtime.display_buffer;
    ordering = scene_movie.ordering_bank[buffer];
    scene_movie.ot = (uint32)(ordering);
    scene_movie.ot_native = scene_movie.ordering_data[buffer];
    packet = scene_movie.packet_bank[buffer];
    scene_movie.packet_cursor = (uint32)(packet);
    scene_movie.packet_begin = packet;
    scene_movie.packet_native = scene_movie.packet_data[buffer];
    ClearOTagR(scene_movie.ot_native, 5);
    result = scene_movie_update_phase();
    if (result != 0)
    {
        scene_movie_end_menu();
        result = game_runtime.playback.return_mode;
        return result | 0x60000000;
    }
    result = movie_captions.secondary_enabled;
    if (result != 0)
    {
        ordering = scene_movie.ot;
        packet = scene_movie.packet_cursor;
        result = subtitles_draw(ordering, packet);
        scene_movie.packet_cursor = (uint32)(result);
    }
    ordering = scene_movie.ot;
    DrawOTag(scene_movie.ot_native + 4);
    return 0;
}

static void scene_movie_queue_stream(uint32 id, RESOURCE_STREAM_BUFFER destination)
{
    uint32 value, flags;
    FUNCTION_MARKER(0x80058800, "ACE3.BIN");
    value = movie_captions.secondary_enabled;
    flags = value != 0 ? 0x3000 : 0x2000;
    {
        (void)(resource_queue_stream(id, destination, flags, scene_movie_complete_stream));
        return;
    }
}

static void scene_movie_complete_stream(void)
{
    FUNCTION_MARKER(0x80058854, "ACE3.BIN");
    scene_movie.stream_finished = (uint8)(1);
    return;
}

static void scene_movie_reset_queue(void)
{
    FUNCTION_MARKER(0x80058864, "ACE3.BIN");
    resource_init_queue(0, 0);
    return;
}

static void scene_movie_stream_idle(void)
{
    uint32 value;
    FUNCTION_MARKER(0x80058888, "ACE3.BIN");
    value = scene_movie.decode_bank;
    {
        (void)(uint32)(value == 0);
        return;
    }
}

static uint32 scene_movie_wait_frame(uint32 nonblocking)
{
    uint32 value, remaining;
    FUNCTION_MARKER(0x8005889C, "ACE3.BIN");
    if (nonblocking != 0)
    {
        return scene_texture_decode_frame() != 0;
    }
    remaining = 120;
    for (;;)
    {
        value = scene_texture_decode_frame();
        if (value != 0)
            return 1;
        resource_poll();
        VSync(0);
        --remaining;
        if ((sint32)remaining < 0)
        {
            scene_movie.stream_finished = (uint8)(1);
            return 1;
        }
    }
}

static void scene_movie_initialize_stream(void)
{
    uint32 i, j, bank;
    FUNCTION_MARKER(0x800586AC, "ACE3.BIN");
    scene_movie.decode_bank = (uint8)(0);
    scene_movie.render_bank = (uint8)(0);
    scene_movie.stream_finished = (uint8)(0);
    scene_movie.stream_playback.effect_mode = (uint8)(0);
    scene_movie.stream_playback.render_active = (uint8)(0);
    scene_movie.stream_playback.effect_ticks = (uint32)(0);
    scene_movie.stream_playback.frame_ticks = (uint32)(0);
    scene_movie.stream_playback.pending_frame = (uint8)(0);
    scene_movie.stream_playback.record_index = (uint8)(0);
    scene_movie.stream_playback.frame_count = (uint16)(0);
    for (bank = 0; bank < 2; ++bank)
    {
        for (i = 0; i < 5; ++i)
        {
            scene_movie.records[bank][i].mode = (uint8)(0);
            scene_movie.records[bank][i].start_tick = 0;
            scene_movie.records[bank][i].end_tick = 0;
            for (j = 0; j < 4; ++j)
                scene_movie.records[bank][i].parameters[j] = 0xFFFF;
        }
    }
    for (bank = 0; bank < 2; ++bank)
        for (i = 0; i < 20; ++i)
        {
            scene_movie.textures[bank][i] = (uint8)(0);
        }
}

static union
{
    RESOURCE_SECTOR_HEADER alignment;
    uint8 data[18368];
} movie_stream_storage;

static void scene_movie_bind_stream(uint32 resource, uint32 buffer, uint32 bytes)
{
    uint32 rounded;
    RESOURCE_STREAM_BUFFER destination;
    FUNCTION_MARKER(0x8005860C, "ACE3.BIN");
    rounded = ((bytes + 3) >> 2) << 2;
    if (rounded > sizeof(movie_stream_storage.data))
        abort();
    scene_movie_initialize_stream();
    resource_init_queue(0, 0);
    destination.encoded_begin = buffer + 216;
    destination.buffer.bytes = rounded;
    destination.buffer.data = movie_stream_storage.data;
    scene_movie_queue_stream(resource, destination);
}

static void overlay_movie_restore_dialogue(void)
{
    PSX_RECT first_page = {832, 256, 192, 128};
    PSX_RECT second_page = {832, 384, 192, 128};
    uint32 value;

    FUNCTION_MARKER(0x800FCF80, "ACE3.BIN");
    value = game_runtime.flags;
    game_runtime.flags = value & 0xEFFFFFFF;
    ClearImage(&first_page, 0, 0, 0);
    ClearImage(&second_page, 0, 0, 0);
    dialogue_ui.entry.complete = (uint32)(1);
    {
        (void)(uint32)(arena_set_cursor(overlay_movie_player.arena_bookmark));
        return;
    }
}

void overlay_movie_control_begin(void)
{
    PSX_RECT first_page = {832, 256, 192, 128};
    PSX_RECT second_page = {832, 384, 192, 128};
    uint32 value;

    FUNCTION_MARKER(0x800FCE40, "ACE3.BIN");
    value = game_runtime.flags;
    game_runtime.flags = value | 0x10000000;
    game_runtime.display.field_wait = (uint32)(0);
    movie_control.phase = (uint32)(0);
    movie_control.page[0].x = (uint16)(832);
    movie_control.page[0].y = (uint16)(256);
    movie_control.page[1].x = (uint16)(832);
    movie_control.page[1].y = (uint16)(384);
    movie_control.width = (uint16)(192);
    movie_control.height = (uint16)(128);
    movie_control.offset_x = (uint16)(0);
    movie_control.offset_y = (uint16)(0);
    value = dialogue_ui.entry.audio_resource;
    if (value != 0)
        movie_control.resource = (uint32)(value);
    movie_control.mode = (uint32)(0);
    movie_control.capture_valid = 0;
    ClearImage(&first_page, 0, 0, 0);
    ClearImage(&second_page, 0, 0, 0);
    dialogue_ui.entry.image_mode = (uint32)(0x8000);
    return;
}

uint32 overlay_movie_control_frame(uint32 mask)
{
    FUNCTION_MARKER(0x800FD028, "ACE3.BIN");
    return overlay_movie_control_tick(mask);
}

void overlay_movie_update_image_mode(uint32 mask)
{
    uint32 mode;

    FUNCTION_MARKER(0x800FD408, "ACE3.BIN");
    mode = dialogue_ui.entry.image_mode;
    if (mode == 0x4000)
    {
        overlay_movie_control_begin();
        return;
    }
    if (mode == 0x10000)
    {
        overlay_movie_restore_dialogue();
        return;
    }
    if (overlay_movie_control_frame(mask) != 0)
    {
        dialogue_ui.entry.image_mode = (uint32)(0x10000);
    }
    return;
}

uint32 overlay_movie_control_tick(uint32 mask)
{
    PSX_RECT rectangle;
    uint32 phase, arena, value, index, resource, mode;
    sint32 x0, y0, x1, y1;

    FUNCTION_MARKER(0x800FD030, "ACE3.BIN");
    phase = movie_control.phase;
    switch (phase)
    {
        case 0:
            arena = arena_peek(0);
            x0 = (sint16)movie_control.page[0].x;
            x0 += (sint16)movie_control.offset_x;
            y0 = (sint16)movie_control.page[0].y;
            y0 += (sint16)movie_control.offset_y;
            x1 = (sint16)movie_control.page[1].x;
            x1 += (sint16)movie_control.offset_x;
            y1 = (sint16)movie_control.page[1].y;
            y1 += (sint16)movie_control.offset_y;
            resource = movie_control.resource;
            mode = movie_control.mode;
            overlay_movie_prepare(resource, mode, arena, (sint16)x0, (sint16)y0, (sint16)x1, (sint16)y1);
            overlay_movie_queue();
            movie_control.phase = (uint32)(1);
            return 0;
        case 1:
            if (!game_runtime.display_buffer && game_movie_poll(&overlay_movie_player.playback))
            {
                movie_control.phase = (uint32)(2);
                movie_current_buffer(&overlay_movie_player.playback);
                game_runtime.surfaces.movie_display.isinter = (uint8)(0);
                value = (uint8)movie_control.mode;
                game_runtime.surfaces.movie_display.isrgb24 = (uint8)(value);
                value = (uint16)game_runtime.display.screen[0];
                index = (uint16)game_runtime.display.screen[1];
                game_runtime.surfaces.movie_display.screen.x = (sint16)((uint16)(value));
                game_runtime.surfaces.movie_display.screen.y = (sint16)((uint16)(index));
                game_runtime.surfaces.movie_draw.isbg = (uint8)(1);
                game_runtime.surfaces.movie_draw.r0 = (uint8)(0);
                game_runtime.surfaces.movie_draw.g0 = (uint8)(0);
                game_runtime.surfaces.movie_draw.b0 = (uint8)(0);
            }
            if (overlay_movie_player.playback.done != 1)
                return 0;
            movie_end();
            movie_control.phase = (uint32)(3);
            return 0;
        case 2:
            if (game_input.pressed & mask)
                overlay_movie_player.playback.done = (uint8)(1);
            if (overlay_movie_player.playback.done == 1)
            {
                movie_end();
                movie_control.phase = (uint32)(3);
            }
            if (!overlay_movie_player.draining)
            {
                if (game_movie_decode(&overlay_movie_player.playback))
                    overlay_movie_player.draining = (uint32)(1);
            }
            else if (overlay_movie_player.playback.ready == 1)
            {
                movie_end();
                index = movie_current_buffer(&overlay_movie_player.playback);
                rectangle.x = (sint16)movie_control.page[index].x;
                rectangle.y = (sint16)movie_control.page[index].y;
                rectangle.h = (sint16)movie_control.height;
                if (movie_control.mode)
                {
                    value = (sint16)movie_control.width;
                    value = ((sint32)((value << 1) + value) >> 1);
                }
                else
                    value = movie_control.width;
                rectangle.w = (sint16)value;
                ClearImage(&rectangle, 0, 0, 0);
                movie_control.phase = (uint32)(3);
                return 0;
            }
            movie_current_buffer(&overlay_movie_player.playback);
            game_runtime.surfaces.movie_display.isinter = (uint8)(0);
            value = (uint8)movie_control.mode;
            game_runtime.surfaces.movie_display.isrgb24 = (uint8)(value);
            value = (uint16)game_runtime.display.screen[0];
            index = (uint16)game_runtime.display.screen[1];
            game_runtime.surfaces.movie_display.screen.x = (sint16)((uint16)(value));
            game_runtime.surfaces.movie_display.screen.y = (sint16)((uint16)(index));
            game_runtime.surfaces.movie_draw.isbg = (uint8)(1);
            game_runtime.surfaces.movie_draw.r0 = (uint8)(0);
            game_runtime.surfaces.movie_draw.g0 = (uint8)(0);
            game_runtime.surfaces.movie_draw.b0 = (uint8)(0);
            return 0;
        case 3:
            movie_control.phase = (uint32)(4);
            return 0;
        case 4:
            return 1;
        default:
            return 0;
    }
}

void mission_movie_overlay_frame(void)
{
    FUNCTION_MARKER(0x800332A4, "ACE3.BIN");
    {
        (void)(mission_movie_overlay_tick());
        return;
    }
}

void mission_movie_update_frame(void)
{
    FUNCTION_MARKER(0x800332C4, "ACE3.BIN");
    {
        (void)(scene_movie_update_mission());
        return;
    }
}

void mission_movie_overlay_tick(void)
{
    uint32 phase, argument;
    FUNCTION_MARKER(0x80057BBC, "ACE3.BIN");
    phase = game_runtime.mission_control.mode;
    if (phase == 0x4000)
    {
        mission_movie_overlay_begin();
        return;
    }
    if (phase == 0x10000)
    {
        mission_movie_overlay_end();
        return;
    }
    movie_present_play();
    argument = game_runtime.mission_control.argument;
    mission_request_mode(0x10000, argument);
}

void mission_movie_overlay_end(void)
{
    uint32 value;
    FUNCTION_MARKER(0x80057640, "ACE3.BIN");
    value = game_runtime.flags;
    game_runtime.flags = value & 0xEFFFFFFF;
    game_runtime.display.field_wait = (uint32)(2);
    value = movie_captions.primary.enabled;
    if (value != 0)
        subtitles_configure(255, 0, 0);
    value = game_runtime.mission_control.render_arena;
    arena_set_cursor(value);
    value = game_runtime.mission_control.render_arena;
    mission_rebuild_render_arena(value);
    movie_control.clear_rect.w = (sint16)(320);
    movie_control.clear_rect.x = (sint16)(0);
    movie_control.clear_rect.y = (sint16)(0);
    movie_control.clear_rect.h = (sint16)(240);
    ClearImage(&movie_control.clear_rect, 1, 1, 1);
    movie_control.clear_rect.x = (sint16)(0);
    movie_control.clear_rect.y = (sint16)(256);
    ClearImage(&movie_control.clear_rect, 1, 1, 1);
    {
        (void)(uint32)(mission_dispatch_scene_request());
        return;
    }
}

void mission_movie_overlay_begin(void)
{
    uint32 index, first, second, value, address, mode, argument;
    FUNCTION_MARKER(0x800573E8, "ACE3.BIN");
    DrawSync(0);
    index = game_runtime.playback.movie_idx;
    if (index == 7 || index == 16 || index == 12)
    {
        argument = game_runtime.mission_control.argument;
        {
            (void)(uint32)(mission_request_mode(0x20000, argument));
            return;
        }
    }
    first = game_runtime.options.captions;
    second = game_runtime.playback.caption_gate;
    if ((first & second) != 0)
    {
        index = game_runtime.playback.movie_idx;
        value = movie_catalog_value(index, MOVIE_CATALOG_CAPTION_KIND);
        movie_captions.primary.enabled = (uint32)(value == 1);
    }
    else
    {
        movie_captions.primary.enabled = (uint32)(0);
    }
    value = game_runtime.flags;
    game_runtime.flags = value | 0x10000000;
    game_runtime.display.field_wait = (uint32)(0);
    address = game_runtime.mission_control.render_arena;
    arena_set_cursor(address);
    movie_control.phase = (uint32)(0);
    movie_control.page[0].x = (uint16)(0);
    movie_control.page[0].y = (uint16)(0);
    movie_control.page[1].x = (uint16)(0);
    movie_control.page[1].y = (uint16)(256);
    movie_control.width = (uint16)(320);
    movie_control.height = (uint16)(240);
    movie_control.offset_x = (uint16)(0);
    movie_control.offset_y = (uint16)(32);
    index = game_runtime.playback.movie_idx;
    value = movie_catalog_value(index, MOVIE_CATALOG_RESOURCE);
    movie_control.mode = (uint32)(0);
    movie_control.capture_valid = 0;
    movie_control.resource = (uint32)(value);
    movie_control.clear_rect.x = (sint16)(0);
    movie_control.clear_rect.y = (sint16)(0);
    movie_control.clear_rect.w = (sint16)(320);
    movie_control.clear_rect.h = (sint16)(240);
    ClearImage(&movie_control.clear_rect, 1, 1, 1);
    movie_control.clear_rect.x = (sint16)(0);
    movie_control.clear_rect.y = (sint16)(256);
    ClearImage(&movie_control.clear_rect, 1, 1, 1);
    value = movie_captions.primary.enabled;
    if (value != 0)
        subtitles_configure(7, 0, 0);
    value = (sint16)game_runtime.playback.resume_count;
    mode = 0x8000;
    if (value != 0)
    {
        index = game_runtime.playback.movie_idx;
        if (index == 12 || index == 27)
            mode = 0x10000;
    }
    else
    {
        index = game_runtime.playback.movie_idx;
        if (index - 29 < 2)
            mode = 0x10000;
    }
    argument = game_runtime.mission_control.argument;
    {
        (void)(uint32)(mission_request_mode(mode, argument));
        return;
    }
}

uint32 scene_movie_draw_stream_effects(uint32 packet, uint32 ordering)
{
    const SCENE_MOVIE_RECORD *record;
    uint32 index, bank, first, second, time, mode, value;
    FUNCTION_MARKER(0x800592E0, "ACE3.BIN");
    index = scene_movie.stream_playback.record_index;
    bank = scene_movie.render_bank;
    if (index >= 5)
        abort();
    record = &scene_movie.records[bank][index];
    first = (sint16)record->end_tick;
    second = (sint16)record->parameters[3];
    time = scene_movie.stream_playback.frame_ticks;
    if (time == first + second)
    {
        index = scene_movie.stream_playback.record_index;
        scene_movie.stream_playback.effect_mode = (uint8)(0);
        scene_movie.stream_playback.effect_ticks = (uint32)(0);
        scene_movie.stream_playback.record_index = (uint8)(index + 1);
    }
    index = scene_movie.stream_playback.record_index;
    bank = scene_movie.render_bank;
    if (index >= 5)
        abort();
    record = &scene_movie.records[bank][index];
    value = (sint16)record->start_tick;
    time = scene_movie.stream_playback.frame_ticks;
    if (time == value)
    {
        value = record->mode;
        scene_movie.stream_playback.effect_ticks = (uint32)(0);
        scene_movie.stream_playback.effect_mode = (uint8)(value);
    }
    mode = scene_movie.stream_playback.effect_mode;
    switch (mode)
    {
        case 0:
            packet = scene_movie_draw_base_frame(packet, ordering);
            break;
        case 1:
            packet = scene_movie_draw_base_frame(packet, ordering);
            packet = scene_movie_draw_effect_mode_one(packet, ordering);
            break;
        case 2:
        case 3:
        case 4:
        case 5:
            packet = scene_movie_draw_base_frame(packet, ordering);
            packet = scene_movie_draw_effect_modes_two_to_five(packet, ordering);
            break;
        case 6:
            packet = scene_movie_draw_effect_mode_six(packet, ordering);
            break;
        case 7:
            packet = scene_movie_draw_effect_mode_seven(packet, ordering);
            break;
        case 8:
        case 9:
            packet = scene_movie_draw_base_frame(packet, ordering);
            break;
        default:
            break;
    }
    if (mode - 1 < 9)
    {
        value = scene_movie.stream_playback.effect_ticks;
        scene_movie.stream_playback.effect_ticks = (uint32)(value + 1);
    }
    return hud_waveform_draw(packet, ordering);
}

uint32 scene_movie_draw_base_frame(uint32 packet, uint32 ordering)
{
    SPRT *spr;
    DR_TPAGE *page;
    uint32 *extent, *ot_entry;
    const SCENE_MOVIE_RECORD *record;
    uint32 mode, index, bank, first, second, numerator, divisor, quotient, intensity, period, amplitude, time, sample, square, color, texture, clut, i, row, height, x, y, uv_y, value;
    FUNCTION_MARKER(0x80059488, "ACE3.BIN");
    texture = 0;
    clut = 0;
    intensity = 128;
    mode = scene_movie.stream_playback.effect_mode;
    if (mode == 1)
    {
        index = scene_movie.stream_playback.record_index;
        bank = scene_movie.render_bank;
        record = &scene_movie.records[bank][index];
        first = (sint16)record->end_tick;
        second = (sint16)record->start_tick;
        time = scene_movie.stream_playback.effect_ticks;
        divisor = first - second - 1;
        numerator = 0 - (time << 7);
        if (divisor == 0)
            xport_mips_break(7);
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        intensity = (uint32)((sint32)numerator / (sint32)divisor) + 128;
        if ((sint16)intensity >= 129)
            intensity = 128;
        if ((sint16)intensity < 0)
            intensity = 0;
        mode = scene_movie.stream_playback.effect_mode;
    }
    if (mode == 8)
    {
        index = scene_movie.stream_playback.record_index;
        bank = scene_movie.render_bank;
        record = &scene_movie.records[bank][index];
        amplitude = record->parameters[1];
        first = (sint16)record->parameters[0];
        period = record->parameters[0];
        if (first == 0)
        {
            period = 16;
            amplitude = 8;
        }
        period = (sint16)period;
        time = scene_movie.stream_playback.effect_ticks;
        sample = 0;
        if ((sint32)time < (sint32)(period << 1))
            sample = (uint16)scene_movie.stream_playback.effect_ticks;
        sample = (sint16)sample;
        amplitude = (sint16)amplitude;
        square = sample * sample;
        numerator = square * (0 - amplitude);
        divisor = period * period;
        if (divisor == 0)
            xport_mips_break(7);
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        quotient = (sint32)numerator / (sint32)divisor;
        numerator = sample * (amplitude << 1);
        divisor = period;
        if (divisor == 0)
            xport_mips_break(7);
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        intensity = quotient + (uint32)((sint32)numerator / (sint32)divisor) + 128;
        if ((sint16)intensity >= 256)
            intensity = 255;
        if ((sint16)intensity < 0)
            intensity = 0;
    }
    intensity = (sint16)intensity;
    color = (intensity << 16) | (intensity << 8) | intensity;
    for (i = 0; i < 20; ++i, packet += 24)
    {
        row = i >> 2;
        height = row == 3 ? 46 : row == 4 ? 24 : 70;
        bank = scene_movie.render_bank;
        if (bank == 0)
        {
            clut = i < 16 ? 140 : 156;
            value = scene_movie.textures[0][i];
            texture = value != 0 ? 0x4670 : 0x4630;
        }
        else if (bank == 1)
        {
            clut = i < 16 ? 142 : 158;
            value = scene_movie.textures[1][i];
            texture = value != 0 ? 0x46F0 : 0x46B0;
        }
        x = ((i - row * 4) << 6) | 32;
        if (row == 3)
        {
            y = 310;
            uv_y = 210;
        }
        else if (row == 4)
        {
            y = 356;
            uv_y = 0;
        }
        else
        {
            uv_y = height * row;
            y = uv_y + 100;
        }
        page = (DR_TPAGE *)((uint8 *)scene_movie.packet_native + (packet - scene_movie.packet_begin));
        spr = (SPRT *)((uint8 *)scene_movie.packet_native + (packet + 4 - scene_movie.packet_begin));
        extent = (uint32 *)((uint8 *)scene_movie.packet_native + (packet + 20 - scene_movie.packet_begin));
        spr->p[0].xy.xy = (y << 16) | (uint32)(sint32)(sint16)x;
        *extent = (height << 16) | 64;
        spr->rgbc.rgbc = color;
        spr->rgbc.code = 100;
        page->code[0] = clut;
        spr->p[0].uv_attr = (texture << 16) | ((uv_y & 255) << 8) | (((i - row * 4) << 6) & 255);
        ((uint8 *)page->code)[3] = 0xE1;
        ((uint8 *)page->code)[2] = 0;
        ot_entry = (uint32 *)(scene_movie.ot_native + 3);
        value = *ot_entry;
        page->tag = value | 0x05000000;
        *ot_entry = packet & 0xFFFFFF;
    }
    return packet;
}

uint32 scene_movie_draw_effect_mode_one(uint32 packet, uint32 ordering)
{
    SPRT *spr;
    DR_TPAGE *page;
    uint32 *extent, *ot_entry;
    const SCENE_MOVIE_RECORD *record;
    uint32 index, bank, first, second, time, divisor, numerator, intensity, color, texture, clut, i, row, height, x, y, uv_y, value, head, tag;
    FUNCTION_MARKER(0x800598BC, "ACE3.BIN");
    index = scene_movie.stream_playback.record_index;
    bank = scene_movie.render_bank;
    record = &scene_movie.records[bank][index];
    first = (sint16)record->end_tick;
    second = (sint16)record->start_tick;
    time = scene_movie.stream_playback.effect_ticks;
    divisor = first - second - 1;
    numerator = time << 7;
    if (divisor == 0)
        xport_mips_break(7);
    if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
        xport_mips_break(6);
    intensity = (sint32)numerator / (sint32)divisor;
    texture = 0;
    clut = 0;
    bank = scene_movie.render_bank ^ 1;
    if ((sint16)intensity >= 129)
        intensity = 128;
    if ((sint16)intensity < 0)
        intensity = 0;
    intensity = (sint16)intensity;
    color = (intensity << 16) | (intensity << 8) | intensity | 0x66000000;
    for (i = 0; i < 20; ++i, packet += 28)
    {
        row = i >> 2;
        height = row == 3 ? 46 : row == 4 ? 24 : 70;
        if (bank == 0)
        {
            clut = i < 16 ? 172 : 188;
            value = scene_movie.textures[0][i];
            texture = value != 0 ? 0x4670 : 0x4630;
        }
        else if (bank == 1)
        {
            clut = i < 16 ? 174 : 190;
            value = scene_movie.textures[1][i];
            texture = value != 0 ? 0x46F0 : 0x46B0;
        }
        x = ((i - row * 4) << 6) | 32;
        if (row == 3)
        {
            y = 310;
            uv_y = 210;
        }
        else if (row == 4)
        {
            y = 356;
            uv_y = 0;
        }
        else
        {
            uv_y = height * row;
            y = uv_y + 100;
        }
        spr = (SPRT *)((uint8 *)scene_movie.packet_native + (packet - scene_movie.packet_begin));
        page = (DR_TPAGE *)((uint8 *)scene_movie.packet_native + (packet + 20 - scene_movie.packet_begin));
        extent = (uint32 *)((uint8 *)scene_movie.packet_native + (packet + 16 - scene_movie.packet_begin));
        spr->p[0].xy.xy = (y << 16) | (uint32)(sint32)(sint16)x;
        spr->rgbc.rgbc = color;
        spr->p[0].uv_attr = (texture << 16) | ((uv_y & 255) << 8) | (((i - row * 4) << 6) & 255);
        value = spr->rgbc.code;
        *extent = (height << 16) | 64;
        spr->rgbc.code = (uint8)(value | 2);
        ot_entry = (uint32 *)(scene_movie.ot_native + 2);
        head = *ot_entry;
        spr->tag = head | 0x04000000;
        *ot_entry = packet & 0xFFFFFF;
        setlen(page, 1);
        page->code[0] = clut | 0xE1000200;
        tag = page->tag;
        head = *ot_entry;
        page->tag = (tag & 0xFF000000) | (head & 0xFFFFFF);
        head = *ot_entry;
        *ot_entry = (head & 0xFF000000) | ((packet + 20) & 0xFFFFFF);
    }
    return packet;
}

uint32 scene_movie_draw_effect_modes_two_to_five(uint32 packet, uint32 ordering)
{
    TILE *tile;
    DR_TPAGE *page;
    uint32 *ot_entry;
    const SCENE_MOVIE_RECORD *record;
    uint32 index, bank, start, offset, end, value, divisor, mode, rate, initial, time, numerator, intensity, head, tag, blend;
    FUNCTION_MARKER(0x80059BD8, "ACE3.BIN");
    index = scene_movie.stream_playback.record_index;
    bank = scene_movie.render_bank;
    record = &scene_movie.records[bank][index];
    start = record->start_tick;
    offset = record->parameters[0];
    end = record->parameters[1];
    start += offset;
    value = (sint16)record->parameters[1];
    if (value == 0)
        end = record->end_tick;
    divisor = (uint32)(sint32)(sint16)end - (uint32)(sint32)(sint16)start;
    mode = scene_movie.stream_playback.effect_mode;
    rate = 0;
    initial = 0;
    if (mode == 2 || mode == 4)
        rate = 255;
    else if (mode == 3 || mode == 5)
    {
        rate = 0xFFFFFF01;
        initial = 255;
    }
    time = scene_movie.stream_playback.frame_ticks;
    if ((sint32)time < (sint16)start)
        intensity = initial;
    else
    {
        time = scene_movie.stream_playback.effect_ticks;
        numerator = time * (uint32)(sint32)(sint16)rate;
        if (divisor == 0)
            xport_mips_break(7);
        if (divisor == 0xFFFFFFFF && numerator == 0x80000000)
            xport_mips_break(6);
        intensity = initial + (uint32)((sint32)numerator / (sint32)divisor);
    }
    if ((sint16)intensity >= 256)
        intensity = 255;
    else if ((sint16)intensity < 0)
        intensity = 0;
    intensity &= 255;
    tile = (TILE *)((uint8 *)scene_movie.packet_native + (packet - scene_movie.packet_begin));
    tile->rgbc.rgbc = (intensity << 16) | (intensity << 8) | intensity | 0x62000000;
    tile->p[0].xy = 0x00640020;
    tile->wh = 0x01180100;
    ot_entry = (uint32 *)(scene_movie.ot_native + 1);
    head = *ot_entry;
    tile->tag = head | 0x03000000;
    *ot_entry = packet & 0xFFFFFF;
    mode = scene_movie.stream_playback.effect_mode;
    packet += 16;
    blend = mode >= 2 && mode < 4 ? 32 : mode >= 4 && mode < 6 ? 64 : 0;
    page = (DR_TPAGE *)((uint8 *)scene_movie.packet_native + (packet - scene_movie.packet_begin));
    setlen(page, 1);
    page->code[0] = blend | 0xE1000200;
    tag = page->tag;
    head = *ot_entry;
    page->tag = (tag & 0xFF000000) | (head & 0xFFFFFF);
    head = *ot_entry;
    *ot_entry = (head & 0xFF000000) | (packet & 0xFFFFFF);
    return packet + 8;
}

uint32 scene_movie_draw_effect_mode_six(uint32 packet, uint32 ordering)
{
    POLY_FT4 *poly;
    uint32 *ot_entry;
    const SCENE_MOVIE_RECORD *record;
    uint32 index, bank, divisor, width, height, x, y, texture, clut, right, bottom, tag, head;
    FUNCTION_MARKER(0x80059E1C, "ACE3.BIN");
    index = scene_movie.stream_playback.record_index;
    bank = scene_movie.render_bank;
    record = &scene_movie.records[bank][index];
    divisor = (sint16)record->parameters[2];
    if (divisor == 0)
        xport_mips_break(7);
    width = 256 / (sint32)divisor;
    if (divisor == 0)
        xport_mips_break(7);
    height = 280 / (sint32)divisor;
    x = (uint8)record->parameters[0] - (uint32)((sint16)width / 2);
    y = (uint8)record->parameters[1] - (uint32)((sint16)height / 2);
    texture = 0;
    clut = 0;
    if (bank == 0)
    {
        clut = 140;
        texture = 0x4630;
    }
    else if (bank == 1)
    {
        clut = 142;
        texture = 0x46B0;
    }
    poly = (POLY_FT4 *)((uint8 *)scene_movie.packet_native + (packet - scene_movie.packet_begin));
    poly->rgbc.rgbc = 0x00808080;
    poly->p[0].xy.xy = 0x00640020;
    y &= 255;
    x &= 255;
    poly->p[0].uv_attr = (texture << 16) | (y << 8) | x;
    right = x + (uint32)(sint32)(sint16)width - 1;
    poly->p[1].xy.xy = 0x00640120;
    poly->p[1].uv_attr = (clut << 16) | (y << 8) | right;
    bottom = (y + (uint32)(sint32)(sint16)height - 1) << 8;
    setlen(poly, 9);
    poly->p[3].xy.xy = 0x017C0120;
    tag = poly->tag;
    poly->p[2].xy.xy = 0x017C0020;
    poly->p[2].uv_attr = bottom | x;
    poly->p[3].uv_attr = bottom | right;
    poly->rgbc.code = 44;
    ot_entry = (uint32 *)(scene_movie.ot_native + 3);
    head = *ot_entry;
    poly->tag = (tag & 0xFF000000) | (head & 0xFFFFFF);
    head = *ot_entry;
    *ot_entry = (head & 0xFF000000) | (packet & 0xFFFFFF);
    return packet + 40;
}

uint32 scene_movie_draw_effect_mode_seven(uint32 packet, uint32 ordering)
{
    POLY_FT4 *poly;
    uint32 *ot_entry;
    uint32 time, texture, clut, uv_y, screen_y, height, uv_height, count, left, right, i, bank, y, bottom, uv, tag, head;
    FUNCTION_MARKER(0x8005A034, "ACE3.BIN");
    time = scene_movie.stream_playback.effect_ticks;
    texture = 0;
    clut = 0;
    screen_y = 100;
    uv_y = time << 2;
    if ((sint32)(uv_y & 255) + (sint16)(280 - (time << 3)) < 256)
    {
        count = 1;
        height = 280;
        uv_height = 24 - (time << 3);
    }
    else
    {
        count = 2;
        height = uv_y + 256;
        uv_height = ~uv_y;
    }
    left = (time * 3) & 255;
    right = left + ((~(time * 6)) & 255);
    for (i = 0; i < count; ++i, packet += 40)
    {
        if (i == 1)
        {
            time = scene_movie.stream_playback.effect_ticks * 4;
            uv_y = 0;
            screen_y = time + 356;
            height = 24 - time;
            uv_height = height;
        }
        bank = scene_movie.render_bank;
        if (bank == 0)
        {
            clut = i == 0 ? 140 : 156;
            texture = 0x4630;
        }
        else if (bank == 1)
        {
            clut = i == 0 ? 142 : 158;
            texture = 0x46B0;
        }
        y = (sint16)screen_y;
        poly = (POLY_FT4 *)((uint8 *)scene_movie.packet_native + (packet - scene_movie.packet_begin));
        poly->p[0].xy.xy = (y << 16) | 32;
        uv = (uv_y & 255) << 8;
        poly->p[0].uv_attr = (texture << 16) | uv | left;
        poly->p[1].uv_attr = (clut << 16) | uv | right;
        bottom = (y + (uint32)(sint32)(sint16)height) << 16;
        poly->p[2].xy.xy = bottom | 32;
        uv = ((uv_y & 255) + (uv_height & 255)) << 8;
        poly->p[2].uv_attr = uv | left;
        setlen(poly, 9);
        poly->rgbc.rgbc = 0x00808080;
        poly->p[1].xy.xy = (y << 16) | 288;
        poly->p[3].xy.xy = bottom | 288;
        poly->p[3].uv_attr = uv | right;
        poly->rgbc.code = 44;
        tag = poly->tag;
        ot_entry = (uint32 *)(scene_movie.ot_native + 3);
        head = *ot_entry;
        poly->tag = (tag & 0xFF000000) | (head & 0xFFFFFF);
        head = *ot_entry;
        *ot_entry = (head & 0xFF000000) | (packet & 0xFFFFFF);
    }
    return packet;
}

void movie_caption_read_complete(void)
{
    FUNCTION_MARKER(0x80057CC4, "ACE3.BIN");
    movie_captions.primary.pending = (uint32)(0);
    return;
}

uint32 movie_frontend_prepare(void)
{
    uint32 state;
    uint32 enabled;
    uint32 index;
    uint32 destination;
    uint32 bytes;
    sint32 id;
    RESOURCE_READ_BUFFER buffer;

    FUNCTION_MARKER(0x80057CD0, "ACE3.BIN");
    movie_control.startup_frames = 0;
    resource_init_queue(0, 0);
    game_runtime.display.background[0] = (uint8)(0);
    game_runtime.display.background[1] = (uint8)(0);
    game_runtime.display.background[2] = (uint8)(0);
    state = game_runtime.display.flags;
    game_runtime.display.flags = (uint32)(state | 8);
    if (!game_runtime.playback.movie_idx)
    {
        game_runtime.playback.intro_count = (uint8)(game_runtime.playback.intro_count + 1);
        if ((game_runtime.playback.intro_count & 3) != 1)
        {
            game_runtime.playback.demo_mode = (uint8)((game_input.held & 0x300) == 0x300 ? 2 : 1);
            return 0x60000002;
        }
    }
    enabled = game_runtime.options.captions;
    state = game_runtime.playback.caption_gate;
    enabled &= state;
    movie_captions.primary.enabled = (uint32)(enabled);
    index = game_runtime.playback.movie_idx;
    id = movie_catalog_value(index, MOVIE_CATALOG_CAPTION_RESOURCE);
    if (enabled && id)
    {
        bytes = resource_get_size((uint32)id);
        destination = arena_alloc(bytes);
        primary_caption_resource = malloc(bytes);
        if (!destination || !primary_caption_resource)
            abort();
        buffer.data = primary_caption_resource;
        buffer.bytes = bytes;
        movie_captions.primary.data = buffer.data;
        movie_captions.primary.bytes = buffer.bytes;
        movie_captions.primary.pending = (uint32)(1);
        resource_queue_read((uint32)id, destination, movie_caption_read_complete, buffer);
    }
    else
    {
        movie_captions.primary.data = NULL;
        movie_captions.primary.bytes = 0;
        movie_captions.primary.enabled = (uint32)(0);
    }
    return 0x04000000;
}

uint32 movie_current_buffer(const MOVIE_PLAYBACK *player)
{
    FUNCTION_MARKER(0x80056B1C, "ACE3.BIN");
    /* Fix movie-overlay-buffer-owner selects the caller-owned player */
    return player->display_index == 0;
}

void movie_restore_capture(void)
{
    PSX_RECT capture;

    FUNCTION_MARKER(0x80058134, "ACE3.BIN");
    resource_init_queue(0, 0);
    movie_stop_output();
    capture = movie_frame_capture_rect;
    VSync(0);
    if (!movie_control.capture_valid)
        abort();
    LoadImagePSX(&capture, movie_capture_pixels);
    DrawSync(0);
    movie_control.capture_valid = 0;
    game_runtime.flags = game_runtime.flags & 0xFFFFFFFB;
    return;
}

typedef struct
{
    GAME_SUBTITLE_STATE before_tick;

    union
    {
        uint32 words[2560];
        uint16 pixels[5120];
    } caption;

    uint32 caption_ready;
} MOVIE_PRESENTATION;

static void movie_present_captions(MOVIE_PRESENTATION *context)
{
    PSX_RECT rectangle = {54, 214, 384, 10};
    DRAWENV previous, draw;
    context->caption_ready = 0;
    if (!movie_captions.primary.enabled)
        return;
    movie_captions.display_page = 0;
    if (!movie_control.mode)
    {
        GetDrawEnv(&previous);
        SetDefDrawEnv(&draw, 0, 0, 320, 240);
        PutDrawEnv(&draw);
        rectangle = (PSX_RECT){0, 208, 320, 16};
        ClearImage(&rectangle, 0, 0, 0);
    }
    ClearOTagR(movie_caption_packets.ordering, 2);
    subtitles_draw_movie(movie_caption_packets.ordering, movie_caption_packets.packets, sizeof(movie_caption_packets.packets));
    DrawOTag(movie_caption_packets.ordering + 1);
    DrawSync(0);
    if (game_runtime.options.captions == 1 && game_runtime.subtitles.enabled == 1 && game_runtime.subtitles.visible[0])
    {
        StoreImage(&rectangle, context->caption.words);
        DrawSync(0);
        context->caption_ready = 1;
    }
    if (!movie_control.mode)
        PutDrawEnv(&previous);
}

static sint32 movie_present_tick(void *opaque)
{
    MOVIE_PRESENTATION *context = opaque;
    pad_poll();
    pad_update_input(&pad_state[0], &game_input, pad_profiles);
    sound_update();
    game_runtime.field_counter = VSync(-1);
    context->before_tick = game_runtime.subtitles;
    movie_present_captions(context);
    return 1;
}

static sint32 movie_present_frame(void *opaque, uint32 number)
{
    MOVIE_PRESENTATION *context = opaque;
    game_movie_publish_frame(number);
    /* recompute this iteration after admitting its delivered frame metadata */
    game_runtime.subtitles = context->before_tick;
    movie_present_captions(context);
    return 1;
}

static void movie_present_rgb555(uint8 *rgb, uint16 color)
{
    uint32 channel;
    for (channel = 0; channel < 3; ++channel)
    {
        uint32 component = ((uint32)color >> (channel * 5)) & 31;
        rgb[channel] = ((component << 3) | (component >> 2));
    }
}

static sint32 movie_present_strip(void *opaque, uint32 word_x, uint8 *rgb, uint32 rows)
{
    MOVIE_PRESENTATION *context = opaque;
    uint32 x, y, left = word_x * 2 / 3;
    if (!movie_control.mode)
        for (y = 0; y < rows; ++y)
            for (x = 0; x < 16; ++x)
            {
                uint8 *pixel = rgb + (y * 16 + x) * 3;
                uint32 color = pixel[0] | ((uint32)pixel[1] << 8) | ((uint32)pixel[2] << 16);
                movie_present_rgb555(pixel, (uint16)mdec_rgb_fifteen(color, 1));
            }
    if (!context->caption_ready)
        return 1;
    if (!movie_control.mode)
    {
        for (y = 176; y < 192 && y < rows; ++y)
            for (x = 0; x < 16 && left + x < 320; ++x)
                movie_present_rgb555(rgb + (y * 16 + x) * 3, context->caption.pixels[(y - 176) * 320 + left + x]);
        return 1;
    }
    for (y = 182; y < 192 && y < rows; ++y)
        for (x = 0; x < 16; ++x)
            if (left + x >= 36 && left + x < 292)
                memcpy(rgb + (y * 16 + x) * 3, (uint8 *)context->caption.words + ((y - 182) * 256 + left + x - 36) * 3, 3);
    return 1;
}

static sint32 movie_present_finish(void *opaque, uint32 step)
{
    PSX_RECT rectangle = {0, 0, 480, 240};
    if (!step)
    {
        ClearImage(&rectangle, 1, 1, 1);
        rectangle.y = 256;
        ClearImage(&rectangle, 1, 1, 1);
        if (movie_captions.primary.enabled)
            subtitles_configure(255, 0, 0);
    }
    return step < 2;
}

static void movie_present_play(void)
{
    MOVIE_PRESENTATION context = {0};
    XportFmvPresentation presentation = {0};
    XportFmvPlayback playback = {&presentation, 256, NULL, 0, 1};
    sint32 result;

    if (!gpu_register_packet_range(&movie_caption_packets, sizeof(movie_caption_packets)))
        abort();
    presentation.width = 320;
    presentation.height = 240;
    presentation.video_y = 32;
    presentation.bottom_rows = 208;
    presentation.screen_x = (sint16)game_runtime.display.screen[0];
    presentation.screen_y = (sint16)game_runtime.display.screen[1];
    presentation.controllers = 1;
    presentation.skip_buttons = PADstart;
    presentation.context = &context;
    presentation.tick = movie_present_tick;
    presentation.frame = movie_present_frame;
    presentation.strip = movie_present_strip;
    presentation.finish = movie_present_finish;
    movie_control.phase = 2;
    /* let synchronous playback own each completed GPU frame */
    gpu_frame_end();
    result = game_movie_play(movie_control.resource, movie_captions.primary.enabled, &playback);
    /* AC3 keeps native display output enabled outside fullscreen playback */
    SetDispMask(1);
    gpu_frame_begin();
    if (result < 0)
    {
        xport_message_error("Ace Combat 3 - Movie error", "Fullscreen movie playback failed");
        abort();
    }
    if (movie_captions.primary.enabled)
        subtitles_configure(255, 0, 0);
    movie_control.phase = 4;
    game_runtime.field_counter = VSync(-1);
}

uint32 movie_frontend_frame(void)
{
    uint32 index;
    FUNCTION_MARKER(0x800581C4, "ACE3.BIN");
    movie_present_play();
    movie_restore_capture();
    if (!(game_runtime.progress.flow_flags & 0x10))
    {
        index = game_runtime.playback.movie_idx;
        if (index == 9 || index == 21 || index == 23 || index == 25 || index == 28)
            return 0x60000007;
    }
    return game_runtime.playback.return_mode | 0x60000000;
}

void movie_stop_output(void)
{
    DecDCToutCallback(NULL);
#if !AP_WEB
    DecDCToutSync(0);
#endif
}

void movie_end(void)
{
    FUNCTION_MARKER(0x80056E8C, "ACE3.BIN");
    game_movie_close();
    resource_init_queue(0, 0);
    movie_stop_output();
    return;
}

uint32 movie_frontend_initialize_display(void)
{
    PSX_RECT capture;
    uint32 state;
    uint32 index;
    uint32 frames;
    uint32 address;

    FUNCTION_MARKER(0x80057EBC, "ACE3.BIN");
    index = game_runtime.playback.movie_idx;
    if (index == 7 || index == 16 || index == 12)
        return 0x60000006;
    if (index == 29)
        return 0x60000008;
    if (index == 30)
        return 0x60000007;
    frames = movie_control.startup_frames;
    if (!frames)
    {
        game_runtime.display.background[0] = (uint8)(0);
        game_runtime.display.mode = (uint32)(0);
        game_runtime.display.field_wait = (uint32)(0);
        game_runtime.display.background[1] = (uint8)(0);
        game_runtime.display.background[2] = (uint8)(0);
        state = game_runtime.display.flags;
        game_runtime.display.flags = (uint32)(state | 9);
        game_runtime.flags = game_runtime.flags | 0x10000004;
        frames = movie_control.startup_frames;
    }
    ++frames;
    movie_control.startup_frames = frames;
    if ((sint32)frames < 3)
        return 0;
    if (!game_runtime.display_buffer)
        return 0;
    movie_control.width = (uint16)(320);
    movie_control.phase = (uint32)(0);
    movie_control.page[0].x = (uint16)(0);
    movie_control.page[0].y = (uint16)(0);
    movie_control.page[1].x = (uint16)(0);
    movie_control.page[1].y = (uint16)(256);
    movie_control.height = (uint16)(240);
    movie_control.offset_x = (uint16)(0);
    movie_control.offset_y = (uint16)(32);
    index = game_runtime.playback.movie_idx;
    resource_stream_mdec_complete();
    address = movie_catalog_value(index, MOVIE_CATALOG_RESOURCE);
    movie_control.mode = (uint32)(1);
    movie_control.resource = (uint32)(address);
    capture = movie_frame_capture_rect;
    StoreImage(&capture, movie_capture_pixels);
    DrawSync(0);
    movie_control.capture_valid = 1;
    movie_control.clear_rect.x = (sint16)(0);
    movie_control.clear_rect.y = (sint16)(0);
    movie_control.clear_rect.w = (sint16)(480);
    movie_control.clear_rect.h = (sint16)(240);
    ClearImage(&movie_control.clear_rect, 1, 1, 1);
    movie_control.clear_rect.x = (sint16)(0);
    movie_control.clear_rect.y = (sint16)(256);
    ClearImage(&movie_control.clear_rect, 1, 1, 1);
    DrawSync(0);
    if (movie_captions.primary.enabled)
        subtitles_configure(9, 0, 0);
    return 0x04000000;
}

static void overlay_movie_stream_complete(void)
{
    if (!game_overlay_is_active(2))
        abort();
    FUNCTION_MARKER(0x800FC750, "ACE3.BIN");
    overlay_movie_player.playback.done = (uint8)(1);
    return;
}
