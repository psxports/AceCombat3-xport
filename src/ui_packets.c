#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_functions.h"

typedef struct
{
    uint32 tag;

    struct
    {
        uint32 mode;
        RGBC color;
        XY position;
        uint32 wh;
    } flat;

    struct
    {
        uint32 mode;
        RGBC color;
        POINT_T point;
        uint32 wh;
    } textured;
} FRAMEBUFFER_FEEDBACK_PACKET;

typedef char framebuffer_feedback_packet_layout_check[sizeof(FRAMEBUFFER_FEEDBACK_PACKET) == 40 && offsetof(FRAMEBUFFER_FEEDBACK_PACKET, flat) == 4 && offsetof(FRAMEBUFFER_FEEDBACK_PACKET, flat.color) == 8 && offsetof(FRAMEBUFFER_FEEDBACK_PACKET, flat.position) == 12 && offsetof(FRAMEBUFFER_FEEDBACK_PACKET, flat.wh) == 16 && offsetof(FRAMEBUFFER_FEEDBACK_PACKET, textured) == 20 && offsetof(FRAMEBUFFER_FEEDBACK_PACKET, textured.color) == 24 && offsetof(FRAMEBUFFER_FEEDBACK_PACKET, textured.point) == 28 && offsetof(FRAMEBUFFER_FEEDBACK_PACKET, textured.wh) == 36 && sizeof(POINT_T) == 8 && offsetof(POINT_T, uv) == 4 && offsetof(POINT_T, attr) == 6 && sizeof(DR_OFFSET) == 12 && offsetof(DR_OFFSET, code) == 4 && offsetof(DR_STP, code) == 4 ? 1 : -1];

typedef union
{
    PSX_RECT rectangle;
    uint32 words[2];
} SCREEN_RECT_PUBLICATION;

typedef char screen_rect_publication_layout[sizeof(SCREEN_RECT_PUBLICATION) == 8 && sizeof(DR_TPAGE) == 8 && offsetof(DR_TPAGE, code) == 4 ? 1 : -1];

typedef struct
{
    DR_AREA clip;
    DR_AREA restore;
    uint32 fill_tag;
    SCREEN_FILL_COMMAND fills[2];
} LETTERBOX_PACKET;

typedef char letterbox_packet_layout_check[sizeof(LETTERBOX_PACKET) == 52 && offsetof(LETTERBOX_PACKET, restore) == 12 && offsetof(LETTERBOX_PACKET, fill_tag) == 24 && offsetof(LETTERBOX_PACKET, fills) == 28 && sizeof(DR_AREA) == 12 && offsetof(DR_AREA, code) == 4 && sizeof(SCREEN_FILL_COMMAND) == 12 && offsetof(SCREEN_FILL_COMMAND, position) == 4 && offsetof(SCREEN_FILL_COMMAND, dimensions) == 8 ? 1 : -1];

typedef struct
{
    DR_STP mask;
    DR_TPAGE page;
    TILE fill;
    DR_STP restore;
} LOADING_FADE_PACKET;

typedef char loading_fade_packet_layout_check[sizeof(LOADING_FADE_PACKET) == 48 && offsetof(LOADING_FADE_PACKET, page) == 12 && offsetof(LOADING_FADE_PACKET, fill) == 20 && offsetof(LOADING_FADE_PACKET, restore) == 36 && sizeof(DR_STP) == 12 && offsetof(DR_STP, code) == 4 && sizeof(DR_TPAGE) == 8 && sizeof(TILE) == 16 && offsetof(TILE, rgbc) == 4 && offsetof(TILE, p) == 8 && offsetof(TILE, w) == 12 && offsetof(TILE, h) == 14 ? 1 : -1];

typedef char loading_label_packet_layout[sizeof(SPRT_8) == 16 && offsetof(SPRT_8, tag) == 0 && offsetof(SPRT_8, rgbc) == 4 && offsetof(SPRT_8, p) == 8 && sizeof(POINT_T) == 8 && offsetof(POINT_T, uv_attr) == 4 && sizeof(TILE) == 16 && offsetof(TILE, tag) == 0 && offsetof(TILE, rgbc) == 4 && offsetof(TILE, p) == 8 && offsetof(TILE, w) == 12 && offsetof(TILE, h) == 14 && sizeof(POLY_FT3) == 32 && offsetof(POLY_FT3, tag) == 0 && sizeof(XY) == 4 && offsetof(XY, y) == 2 ? 1 : -1];

typedef char frame_border_packet_layout_check[sizeof(POLY_G4) == 36 && offsetof(POLY_G4, p) == 4 && sizeof(POINT_C) == 8 && offsetof(POINT_C, xy) == 4 ? 1 : -1];

typedef char frame_copy_packet_layout[sizeof(DR_MOVE) == 24 && offsetof(DR_MOVE, tag) == 0 && offsetof(DR_MOVE, code) == 4 ? 1 : -1];

typedef char title_renderer_packet_layout_check[sizeof(TILE) == 16 && offsetof(TILE, rgbc) == 4 && offsetof(TILE, p) == 8 && offsetof(TILE, wh) == 12 && sizeof(LINE_F2) == 16 && offsetof(LINE_F2, rgbc) == 4 && offsetof(LINE_F2, p) == 8 && sizeof(SPRT) == 20 && offsetof(SPRT, rgbc) == 4 && offsetof(SPRT, p) == 8 && offsetof(SPRT, w) == 16 && offsetof(SPRT, h) == 18 && sizeof(POLY_FT4) == 40 && offsetof(POLY_FT4, rgbc) == 4 && offsetof(POLY_FT4, p) == 8 && sizeof(POINT_T) == 8 && offsetof(POINT_T, uv) == 4 && offsetof(POINT_T, attr) == 6 ? 1 : -1];

const PSX_RECT ui_fullscreen_clip = {0, 0, 320, 480};

UI_PACKET_STATE ui_packets;

typedef struct
{
    uint32 packet_pages[2];
    uint32 ordering_pages[2];
    UI_SCENE_ORDERING_TABLE *ordering_data[2];
    uint32 packet_cursor;
    uint32 packet_begin;
    UI_PACKET_PAGE *packet_data[2];
    UI_PACKET_PAGE *active_packet_page;
    uint32 ordering;
    uint32 visible_groups;
    TITLE_RENDER_VIEW views[32];
    uint16 animation_phase[16];
    uint16 animation_mode[16];
    uint16 animation_limit[16];
    uint8 group_values[64];

    struct
    {
        uint16 origin_x;
        uint16 origin_y;
        uint16 start;
        uint16 middle;
        uint16 end;
        uint16 transform;
        uint16 animation;
        uint16 ordering_slot;
    } config;

    UI_COMMAND_STREAM command_streams[32];
    uint16 hint_x;
    uint16 hint_y;
    uint32 hint_flags;
} TITLE_RENDER_STATE;

static TITLE_RENDER_STATE title_renderer;

typedef struct
{
    UI_PACKET_PAGE packets;
    UI_SCENE_ORDERING_TABLE ordering;
} TITLE_GPU_STORAGE;

typedef struct
{
    UI_PACKET_PAGE packets;
    UI_SCENE_ORDERING_TABLE ordering;
    UI_DETAIL_ORDERING_TABLE detail;
    UI_SCENE_PRIMITIVES primitives;
} UI_SCENE_GPU_STORAGE;

static TITLE_GPU_STORAGE title_gpu_storage[2];
static UI_SCENE_GPU_STORAGE scene_gpu_storage[2];

void ui_bind_scene_packet_page(uint32 index)
{
    UI_SCENE_BUFFER *bank;

    if (index >= 2 || !gpu_register_packet_range(scene_gpu_storage, sizeof(scene_gpu_storage)))
        abort();
    bank = &ui_packets.scene_bank[index];
    bank->packets = gpu_packet_link((uintptr_t)&scene_gpu_storage[index].packets);
    bank->packet_data = &scene_gpu_storage[index].packets;
}

void ui_bind_scene_ordering(uint32 index)
{
    UI_SCENE_BUFFER *bank;

    if (index >= 2 || !gpu_register_packet_range(scene_gpu_storage, sizeof(scene_gpu_storage)))
        abort();
    bank = &ui_packets.scene_bank[index];
    bank->ot = gpu_packet_link((uintptr_t)&scene_gpu_storage[index].ordering);
    bank->ot_data = &scene_gpu_storage[index].ordering;
    bank->detail_ot = gpu_packet_link((uintptr_t)&scene_gpu_storage[index].detail);
    bank->detail_ot_data = &scene_gpu_storage[index].detail;
}

void ui_bind_scene_primitives(uint32 index)
{
    UI_SCENE_BUFFER *bank;

    if (index >= 2 || !gpu_register_packet_range(scene_gpu_storage, sizeof(scene_gpu_storage)))
        abort();
    bank = &ui_packets.scene_bank[index];
    bank->prims_address = gpu_packet_link((uintptr_t)&scene_gpu_storage[index].primitives);
    bank->prims = &scene_gpu_storage[index].primitives;
}

uint32 *ui_native_ot_span(uint32 address, size_t bytes)
{
    uint32 index, kind;

    for (index = 0; index < 2; ++index)
    {
        const UI_SCENE_BUFFER *bank = &ui_packets.scene_bank[index];
        uint32 bases[3] = {bank->ot, bank->detail_ot, title_renderer.ordering_pages[index]};
        uint32 *data[3] = {bank->ot_data ? bank->ot_data->entries : NULL, bank->detail_ot_data ? bank->detail_ot_data->entries : NULL, title_renderer.ordering_data[index] ? title_renderer.ordering_data[index]->entries : NULL};
        const size_t capacities[3] = {sizeof(UI_SCENE_ORDERING_TABLE), sizeof(UI_DETAIL_ORDERING_TABLE), sizeof(UI_SCENE_ORDERING_TABLE)};

        for (kind = 0; kind < 3; ++kind)
        {
            uint32 offset = address - bases[kind];

            if (data[kind] && bytes && address >= bases[kind] && !(offset & 3) && offset <= capacities[kind] && bytes <= capacities[kind] - offset)
                return data[kind] + offset / sizeof(uint32);
        }
    }
    return NULL;
}

void *ui_native_packet_span(uint32 address, size_t bytes)
{
    uint32 index, kind;

    for (index = 0; index < 2; ++index)
    {
        const UI_SCENE_BUFFER *bank = &ui_packets.scene_bank[index];
        uint32 title_base = title_menu.packet_pages[index];

        if (title_base && bytes && address >= title_base && address - title_base <= sizeof(title_menu.packet_storage[index]) && bytes <= sizeof(title_menu.packet_storage[index]) - (address - title_base))
            return (uint8 *)title_menu.packet_storage[index] + (address - title_base);
        uint32 bases[3] = {bank->packets, bank->prims_address, title_renderer.packet_pages[index]};
        uint8 *data[3] = {(uint8 *)bank->packet_data, (uint8 *)bank->prims, (uint8 *)title_renderer.packet_data[index]};
        const size_t capacities[3] = {sizeof(UI_PACKET_PAGE), sizeof(UI_SCENE_PRIMITIVES), sizeof(UI_PACKET_PAGE)};

        for (kind = 0; kind < 3; ++kind)
        {
            uint32 offset = address - bases[kind];

            if (data[kind] && bytes && address >= bases[kind] && offset <= capacities[kind] && bytes <= capacities[kind] - offset)
                return data[kind] + offset;
        }
    }
    return NULL;
}

uint32 *ui_scene_ot_span(uint32 address, size_t bytes)
{
    uint32 *data = ui_native_ot_span(address, bytes);

    if (data)
        return data;
    return mission_ot_span(address, bytes);
}

uint32 *ui_detail_ot_span(uint32 address, size_t bytes)
{
    uint32 *data = ui_native_ot_span(address, bytes);

    if (data)
        return data;
    return mission_ot_span(address, bytes);
}

void *ui_packet_span(uint32 address, size_t bytes)
{
    void *data = ui_native_packet_span(address, bytes);

    if (data)
        return data;
    return mission_packet_span(address, bytes);
}

void ui_packets_reset(void)
{
    memset(&ui_packets, 0, sizeof(ui_packets));
}

typedef struct
{
    uint16 x, y;
    uint32 untouched[2];
} UI_MOTION_SEED;

typedef char ui_motion_seed_layout_check[sizeof(UI_MOTION_SEED) == 12 && offsetof(UI_MOTION_SEED, y) == 2 ? 1 : -1];

void ui_packets_initialize(void)
{
    RESOURCE_STREAM_BUFFER buffer_allocation;
    uint32 value, records, i, j;
    FUNCTION_MARKER(0x800DE924, "ACE3.BIN");
    value = arena_alloc(22296);
    for (i = 0; i < 3; ++i)
    {
        buffer_allocation = arena_alloc_backed(216);
        value = buffer_allocation.encoded_begin;
        records = value;
        value = arena_alloc(72);
        UI_MOTION_SEED *seeds = (UI_MOTION_SEED *)buffer_allocation.buffer.data;
        for (j = 0; j < 18; ++j)
        {
            seeds[j].x = 0;
            seeds[j].y = 0;
        }
    }
    for (i = 0; i < 2; ++i)
    {
        value = arena_alloc(64);
    }
    menu_orbits_reset();
    ui_control_reset();
    value = arena_alloc(32);
    value = arena_alloc(32);
    value = arena_alloc(32);
    ui_animation_reset();
    value = arena_alloc(64);
    ui_widgets_reset();
    ui_map_initialize_packets();
    return;
}

uint32 ui_packets_draw_noise_rect(uint32 object, uint32 dst, uint32 factor, const PSX_RECT *native_rect)
{
    uint32 *setup, *restore;
    uint32 result, xy, wh, carrier, value;
    sint32 width, height;
    FUNCTION_MARKER(0x800847A8, "ACE3.BIN");
    width = native_rect->w;
    if (width <= 0)
        return dst;
    height = native_rect->h;
    if (height <= 0)
        return dst;
    setup = (uint32 *)ui_packet_span(dst, 4 * sizeof(*setup));
    setup[0] = ((dst + 16) & 0xFFFFFF) | 0x3000000;
    setup[1] = 0xE1000218;
    setup[2] = 0xE20C4318;
    setup[3] = 0;
    xport_gte_write_data(6, 0x64838383);
    xport_gte_write_data(8, 0x1000 - factor);
    xy = (uint16)native_rect->x | ((uint32)(uint16)native_rect->y << 16);
    wh = (uint16)native_rect->w | ((uint32)(uint16)native_rect->h << 16);
    result = ui_packets_emit_noise_tiles(dst + 16, 0x46D6, xy, wh, &carrier);
    restore = (uint32 *)ui_packet_span(result, 4 * sizeof(*restore));
    restore[1] = 0xE1000220;
    restore[0] = 0x3000000;
    restore[2] = 0xE2000000;
    restore[3] = 0;
    value = effect_light_prepend_chain((void *)ui_scene_ot_span((object + 2) & 0xFFFFFFFC, sizeof(uint32)), (object + 2) & 3, dst, result, carrier);
    return result + 16;
}

uint32 ui_packets_emit_noise_tiles(uint32 dst, uint32 texture, uint32 xy, uint32 wh, uint32 *last_a0)
{
    uint32 color, field, link, random, seed, carrier = dst;
    sint32 x, y, width, height, remaining;
    SPRT *spr;
    FUNCTION_MARKER(0x80084318, "ACE3.BIN");
    xport_gte_write_control(21, 48);
    xport_gte_write_control(22, 48);
    xport_gte_write_control(23, 48);
    xport_gte_execute(0x780010);
    color = xport_gte_read_data(22);
    y = (sint32)xy >> 16;
    width = (sint16)wh;
    height = (sint32)wh >> 16;
    while (height > 0)
    {
        remaining = width;
        x = (sint16)xy;
        if (remaining > 0)
        {
            field = dst + 18;
            link = (dst << 8) + 0x1400;
            do
            {
                spr = (SPRT *)ui_packet_span(dst, sizeof(*spr));
                spr->tag = (link >> 8) | 0x4000000;
                spr->rgbc.rgbc = color;
                spr->p[0].xy.x = (sint16)(uint16)x;
                spr->p[0].xy.y = (sint16)(uint16)y;
                seed = random_seed;
                carrier = seed * 0x41C64E6D;
                random = psyq_rand_next(&seed);
                random_seed = seed;
                random &= texture == 0x4696 ? 0x7C7E : 0x7E7F;
                spr->p[0].uv.uv = (uint16)random;
                spr->p[0].attr = (uint16)texture;
                spr->w = (sint16)(uint16)(remaining < 32 ? remaining : 32);
                spr->h = (sint16)(uint16)(height < 48 ? height : 48);
                field += 20;
                link += 0x1400;
                dst += 20;
                remaining -= 32;
                x += 32;
            } while (remaining > 0);
        }
        height -= 48;
        y += 48;
    }
    if (last_a0)
        *last_a0 = carrier;
    return dst;
}

void title_renderer_bind_commands(const uint8 *data, uint32 bytes)
{
    uint16 wire_halfword_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    UI_COMMAND_STREAM *streams = title_renderer.command_streams;
    uint32 count, index, offset, end;

    if (!data || bytes < 4)
        abort();
    count = (memcpy(&wire_halfword_0, data, sizeof(wire_halfword_0)), wire_halfword_0);
    if (count > sizeof(title_renderer.command_streams) / sizeof(*streams) || bytes < 4 + count * 4)
        abort();
    memset(streams, 0, sizeof(title_renderer.command_streams));
    for (index = 0; index < count; ++index)
    {
        offset = (memcpy(&wire_word_1, data + 4 + index * 4, sizeof(wire_word_1)), wire_word_1);
        end = index + 1 < count ? (memcpy(&wire_word_2, data + 8 + index * 4, sizeof(wire_word_2)), wire_word_2) : bytes;
        if (offset < 4 + count * 4 || end > bytes || offset >= end || end - offset < 4 || ((offset | end) & 3))
            abort();
        streams[index].data = data + offset;
        streams[index].bytes = end - offset;
    }
}

uint32 title_renderer_set_visible(uint32 enabled)
{
    FUNCTION_MARKER(0x800DA828, "ACE3.BIN");
    title_renderer.visible_groups = (uint32)(enabled);
    return 0;
}

void title_renderer_set_view_origin(uint32 index, uint32 first, uint32 second)
{
    FUNCTION_MARKER(0x800DA714, "ACE3.BIN");
    if (index >= 32)
        return;
    title_renderer.views[index].x = (uint16)first;
    title_renderer.views[index].y = (uint16)second;
    return;
}

void title_renderer_reset_view(uint32 index)
{
    FUNCTION_MARKER(0x800DA77C, "ACE3.BIN");
    {
        (void)(title_renderer_set_view_origin(index, 0, 0));
        return;
    }
}

void title_renderer_initialize(void)
{
    uint32 index;

    FUNCTION_MARKER(0x800DC810, "ACE3.BIN");
    arena_alloc(88);
    arena_alloc(384);
    arena_alloc(128);
    memset(title_renderer.command_streams, 0, sizeof(title_renderer.command_streams));
    if (!gpu_register_packet_range(title_gpu_storage, sizeof(title_gpu_storage)))
        abort();
    for (index = 0; index < 2; ++index)
    {
        (void)arena_alloc(64);
        title_renderer.ordering_pages[index] = gpu_packet_link((uintptr_t)&title_gpu_storage[index].ordering);
        title_renderer.ordering_data[index] = &title_gpu_storage[index].ordering;
        (void)arena_alloc(0x8000);
        title_renderer.packet_pages[index] = gpu_packet_link((uintptr_t)&title_gpu_storage[index].packets);
        title_renderer.packet_data[index] = &title_gpu_storage[index].packets;
    }
    memset(&title_renderer.config, 0, sizeof(title_renderer.config));
    title_renderer.config.ordering_slot = 1;
    arena_alloc(32);
    arena_alloc(32);
    arena_alloc(32);
    for (index = 0; index < 16; ++index)
    {
        title_renderer.animation_phase[index] = 0;
        title_renderer.animation_mode[index] = 0;
        title_renderer.animation_limit[index] = 0;
    }
    arena_alloc(64);
    memset(title_renderer.group_values, 0, sizeof(title_renderer.group_values));
    title_renderer.visible_groups = (uint32)(0);
    title_renderer.hint_flags = (uint32)(0);
    for (index = 0; index < 32; ++index)
        title_renderer_reset_view(index);
    return;
}

void title_renderer_begin_frame(uint32 packet)
{
    uint32 *gpu_word;
    uint32 index, ordering, buffer, link, tag, result;

    FUNCTION_MARKER(0x800DC9EC, "ACE3.BIN");
    index = game_runtime.display_buffer;
    ordering = title_renderer.ordering_pages[index];
    title_renderer.ordering = (uint32)(ordering);
    index = game_runtime.display_buffer;
    ordering = title_renderer.ordering;
    buffer = title_renderer.packet_pages[index];
    title_renderer.packet_cursor = (uint32)(buffer);
    title_renderer.packet_begin = buffer;
    title_renderer.active_packet_page = title_renderer.packet_data[index];
    ClearOTagR((void *)ui_scene_ot_span(ordering, 64), 16);
    ordering = title_renderer.ordering;
    gpu_word = ui_packet_span(packet, sizeof(*gpu_word));
    link = *gpu_word;
    gpu_word = (void *)ui_scene_ot_span(ordering, sizeof(*gpu_word));
    tag = *gpu_word;
    gpu_word = (void *)ui_scene_ot_span(ordering, sizeof(*gpu_word));
    *gpu_word = (tag & 0xFF000000) | (link & 0xFFFFFF);
    gpu_word = ui_packet_span(packet, sizeof(*gpu_word));
    link = *gpu_word;
    ordering = title_renderer.ordering;
    result = (ordering + 60) & 0xFFFFFF;
    gpu_word = ui_packet_span(packet, sizeof(*gpu_word));
    *gpu_word = (link & 0xFF000000) | result;
    {
        (void)(uint32)(result);
        return;
    }
}

void title_renderer_tick_animations(void)
{
    uint32 index;

    FUNCTION_MARKER(0x800DA8E0, "ACE3.BIN");
    for (index = 0; index < 16; ++index)
    {
        uint16 *phase = &title_renderer.animation_phase[index];
        uint16 *mode = &title_renderer.animation_mode[index];
        if (*mode == 1)
        {
            if ((sint16)*phase < (sint16)title_renderer.animation_limit[index])
                ++*phase;
            else
                *mode = 2;
        }
        if (*mode == 3)
        {
            if ((sint16)*phase > 0)
                --*phase;
            else
                *mode = 0;
        }
    }
    return;
}

sint32 title_renderer_animation_phase(uint32 index)
{
    FUNCTION_MARKER(0x800DA8BC, "ACE3.BIN");
    if (index >= 16)
        abort();
    return (sint16)title_renderer.animation_phase[index];
}

static void title_renderer_emit_draw_mode(UI_COMMAND_STREAM input, uint32 ordering)
{
    uint16 wire_halfword_0;
    uint16 wire_halfword_1;
    uint32 *gpu_word;
    DR_MODE *draw_mode;
    uint32 packet, blend, x, y, mode, slot, tag, link;

    FUNCTION_MARKER(0x800DB254, "ACE3.BIN");
    const uint8 *command = input.data;
    if (!command || input.bytes < 12)
        abort();
    packet = title_renderer.packet_cursor;
    title_renderer.packet_cursor = (uint32)(packet + 12);
    blend = *(command + 7);
    y = (memcpy(&wire_halfword_0, command + 2, sizeof(wire_halfword_0)), wire_halfword_0);
    x = (memcpy(&wire_halfword_1, command, sizeof(wire_halfword_1)), wire_halfword_1);
    mode = ((blend & 3) << 5) | ((y & 0x100) >> 4) | ((x & 0x3FF) >> 6) | ((y & 0x200) << 2);
    if (!title_renderer.active_packet_page || packet < title_renderer.packet_begin || packet - title_renderer.packet_begin > sizeof(*title_renderer.active_packet_page) - 12)
        abort();
    draw_mode = (DR_MODE *)(void *)((uint8 *)title_renderer.active_packet_page + (packet - title_renderer.packet_begin));
    SetDrawMode(draw_mode, 0, 0, mode, NULL);
    slot = (sint16)title_renderer.config.ordering_slot;
    gpu_word = (void *)((uint8 *)title_renderer.active_packet_page + (packet - title_renderer.packet_begin));
    tag = *gpu_word;
    gpu_word = (void *)ui_scene_ot_span(ordering + slot * 4, sizeof(*gpu_word));
    link = *gpu_word;
    gpu_word = (void *)((uint8 *)title_renderer.active_packet_page + (packet - title_renderer.packet_begin));
    *gpu_word = (tag & 0xFF000000) | (link & 0xFFFFFF);
    slot = (sint16)title_renderer.config.ordering_slot;
    gpu_word = (void *)ui_scene_ot_span(ordering + slot * 4, sizeof(*gpu_word));
    link = *gpu_word;
    gpu_word = (void *)ui_scene_ot_span(ordering + slot * 4, sizeof(*gpu_word));
    *gpu_word = (link & 0xFF000000) | (packet & 0xFFFFFF);
}

static void title_renderer_emit_draw_area(UI_COMMAND_STREAM input, uint32 ordering)
{
    uint16 wire_halfword_0;
    uint16 wire_halfword_1;
    uint16 wire_halfword_2;
    uint16 wire_halfword_3;
    uint32 wire_word_4;
    uint32 wire_word_5;
    uint32 *gpu_word;
    PSX_RECT rectangle;
    uint32 packet, slot, tag, link;

    FUNCTION_MARKER(0x800DB834, "ACE3.BIN");
    const uint8 *command = input.data;
    if (!command || input.bytes < 20)
        abort();
    packet = title_renderer.packet_cursor;
    title_renderer.packet_cursor = (uint32)(packet + 12);
    rectangle.x = (sint16)(memcpy(&wire_halfword_0, command + 4, sizeof(wire_halfword_0)), wire_halfword_0);
    rectangle.y = (sint16)(memcpy(&wire_halfword_1, command + 6, sizeof(wire_halfword_1)), wire_halfword_1);
    rectangle.w = (sint16)(memcpy(&wire_halfword_2, command + 8, sizeof(wire_halfword_2)), wire_halfword_2);
    rectangle.h = (sint16)(memcpy(&wire_halfword_3, command + 10, sizeof(wire_halfword_3)), wire_halfword_3);
    if (!title_renderer.active_packet_page || packet < title_renderer.packet_begin || packet - title_renderer.packet_begin > sizeof(*title_renderer.active_packet_page) - 12)
        abort();
    SetDrawArea((DR_AREA *)(void *)((uint8 *)title_renderer.active_packet_page + (packet - title_renderer.packet_begin)), &rectangle);
    slot = (memcpy(&wire_word_4, command + 16, sizeof(wire_word_4)), wire_word_4);
    gpu_word = (void *)((uint8 *)title_renderer.active_packet_page + (packet - title_renderer.packet_begin));
    tag = *gpu_word;
    gpu_word = (void *)ui_scene_ot_span(ordering + slot * 4, sizeof(*gpu_word));
    link = *gpu_word;
    gpu_word = (void *)((uint8 *)title_renderer.active_packet_page + (packet - title_renderer.packet_begin));
    *gpu_word = (tag & 0xFF000000) | (link & 0xFFFFFF);
    slot = (memcpy(&wire_word_5, command + 16, sizeof(wire_word_5)), wire_word_5);
    gpu_word = (void *)ui_scene_ot_span(ordering + slot * 4, sizeof(*gpu_word));
    link = *gpu_word;
    gpu_word = (void *)ui_scene_ot_span(ordering + slot * 4, sizeof(*gpu_word));
    *gpu_word = (link & 0xFF000000) | (packet & 0xFFFFFF);
}

static void title_renderer_set_draw_config(UI_COMMAND_STREAM input)
{
    uint16 wire_halfword_0;
    uint16 wire_halfword_1;
    uint16 wire_halfword_2;
    uint16 wire_halfword_3;
    uint16 wire_halfword_4;
    uint16 wire_halfword_5;
    uint16 wire_halfword_6;
    uint16 wire_halfword_7;
    FUNCTION_MARKER(0x800DB910, "ACE3.BIN");
    const uint8 *command = input.data;
    if (!command || input.bytes < 16)
        abort();
    title_renderer.config.origin_x = (memcpy(&wire_halfword_0, command + 0, sizeof(wire_halfword_0)), wire_halfword_0);
    title_renderer.config.origin_y = (memcpy(&wire_halfword_1, command + 2, sizeof(wire_halfword_1)), wire_halfword_1);
    title_renderer.config.start = (memcpy(&wire_halfword_2, command + 4, sizeof(wire_halfword_2)), wire_halfword_2);
    title_renderer.config.middle = (memcpy(&wire_halfword_3, command + 6, sizeof(wire_halfword_3)), wire_halfword_3);
    title_renderer.config.end = (memcpy(&wire_halfword_4, command + 8, sizeof(wire_halfword_4)), wire_halfword_4);
    title_renderer.config.transform = (memcpy(&wire_halfword_5, command + 10, sizeof(wire_halfword_5)), wire_halfword_5);
    title_renderer.config.animation = (memcpy(&wire_halfword_6, command + 12, sizeof(wire_halfword_6)), wire_halfword_6);
    title_renderer.config.ordering_slot = (memcpy(&wire_halfword_7, command + 14, sizeof(wire_halfword_7)), wire_halfword_7);
}

uint32 title_renderer_transform_identity(void)
{
    FUNCTION_MARKER(0x800DA9D4, "ACE3.BIN");
    return 1;
}

static uint32 title_renderer_animation_index(void)
{
    sint32 index = (sint16)title_renderer.config.animation;

    if ((uint32)index >= 16)
        abort();
    return index;
}

uint32 title_renderer_transform_completed(void)
{
    sint32 index;

    FUNCTION_MARKER(0x800DAC98, "ACE3.BIN");
    index = title_renderer_animation_index();
    return (sint16)title_renderer.animation_mode[index] == 2;
}

uint32 title_renderer_transform_reveal(TITLE_RENDER_GEOMETRY *geometry)
{
    uint32 low, origin_word, numerator, quotient;
    sint32 index, start, middle, end, phase, mode, position, origin, delta, divisor;

    FUNCTION_MARKER(0x800DA9DC, "ACE3.BIN");
    index = title_renderer_animation_index();
    middle = (sint16)title_renderer.config.middle;
    low = title_renderer.config.start;
    phase = (sint16)title_renderer.animation_phase[index];
    start = (sint16)low;
    end = (sint16)title_renderer.config.end;
    index = title_renderer_animation_index();
    mode = (sint16)title_renderer.animation_mode[index];
    if (!mode)
        return 0;
    if (mode == 2)
        return 1;
    if (phase < start)
        return 0;
    if (phase < middle)
    {
        position = (sint16)geometry->x;
        origin = (sint16)title_renderer.config.origin_x;
        numerator = (uint32)(position - origin) * (uint32)(phase - start);
        divisor = middle - start;
        if (!divisor || (divisor == -1 && numerator == 0x80000000))
        {
#ifndef NDEBUG
            fprintf(stderr, "Resource001 reveal coordinate division trap\n");
#endif
            abort();
        }
        quotient = (uint32)((sint32)numerator / divisor);
        origin_word = title_renderer.config.origin_x;
        geometry->x = (uint16)(origin_word + quotient);
        position = (sint16)geometry->y;
        origin = (sint16)title_renderer.config.origin_y;
        delta = position - origin;
        divisor = delta < 0 ? -delta : delta;
        if (!divisor)
        {
#ifndef NDEBUG
            fprintf(stderr, "Resource001 reveal direction division trap7\n");
#endif
            abort();
        }
        quotient = (uint32)(delta / divisor);
        origin_word = title_renderer.config.origin_y;
        geometry->y = (uint16)(origin_word + quotient);
        position = (sint16)geometry->width;
        numerator = (uint32)position * (uint32)(phase - start);
        divisor = middle - start;
        if (!divisor || (divisor == -1 && numerator == 0x80000000))
        {
#ifndef NDEBUG
            fprintf(stderr, "Resource001 reveal width division trap\n");
#endif
            abort();
        }
        quotient = (uint32)((sint32)numerator / divisor);
        geometry->height = (uint16)(2);
        geometry->width = (uint16)(quotient);
    }
    else if (phase < end)
    {
        position = (sint16)geometry->y;
        origin = (sint16)title_renderer.config.origin_y;
        numerator = (uint32)(position - origin) * (uint32)(phase - middle);
        divisor = end - middle;
        if (!divisor || (divisor == -1 && numerator == 0x80000000))
        {
#ifndef NDEBUG
            fprintf(stderr, "Resource001 reveal vertical division trap\n");
#endif
            abort();
        }
        quotient = (uint32)((sint32)numerator / divisor);
        origin_word = title_renderer.config.origin_y;
        geometry->y = (uint16)(origin_word + quotient);
        position = (sint16)geometry->height;
        numerator = (uint32)position * (uint32)(phase - middle);
        divisor = end - middle;
        if (!divisor || (divisor == -1 && numerator == 0x80000000))
        {
#ifndef NDEBUG
            fprintf(stderr, "Resource001 reveal height division trap\n");
#endif
            abort();
        }
        quotient = (uint32)((sint32)numerator / divisor);
        geometry->height = (uint16)(quotient);
    }
    return 1;
}

uint32 title_renderer_transform_horizontal_reveal(TITLE_RENDER_GEOMETRY *geometry)
{
    uint32 low, numerator, quotient, origin_word;
    sint32 index, start, middle, phase, mode, position, origin, divisor;

    FUNCTION_MARKER(0x800DACD0, "ACE3.BIN");
    index = title_renderer_animation_index();
    middle = (sint16)title_renderer.config.middle;
    low = title_renderer.config.start;
    phase = (sint16)title_renderer.animation_phase[index];
    start = (sint16)low;
    index = title_renderer_animation_index();
    mode = (sint16)title_renderer.animation_mode[index];
    if (!mode)
        return 0;
    if (mode == 2)
        return 1;
    if (phase < start)
        return 0;
    if (phase < middle)
    {
        position = (sint16)geometry->x;
        origin = (sint16)title_renderer.config.origin_x;
        numerator = (uint32)(position - origin) * (uint32)(phase - start);
        divisor = middle - start;
        if (!divisor || (divisor == -1 && numerator == 0x80000000))
        {
#ifndef NDEBUG
            fprintf(stderr, "Resource001 horizontal reveal coordinate division trap\n");
#endif
            abort();
        }
        quotient = (uint32)((sint32)numerator / divisor);
        origin_word = title_renderer.config.origin_x;
        geometry->x = (uint16)(origin_word + quotient);
        position = (sint16)geometry->width;
        numerator = (uint32)position * (uint32)(phase - start);
        divisor = middle - start;
        if (!divisor || (divisor == -1 && numerator == 0x80000000))
        {
#ifndef NDEBUG
            fprintf(stderr, "Resource001 horizontal reveal width division trap\n");
#endif
            abort();
        }
        quotient = (uint32)((sint32)numerator / divisor);
        geometry->width = (uint16)(quotient);
    }
    return 1;
}

static uint32 title_renderer_apply_transform(uint32 index, TITLE_RENDER_GEOMETRY *geometry)
{
    if (!geometry && (index == 1 || index == 3))
        abort();
    switch (index)
    {
        case 0:
            return title_renderer_transform_identity();
        case 1:
            return title_renderer_transform_reveal(geometry);
        case 2:
            return title_renderer_transform_completed();
        case 3:
            return title_renderer_transform_horizontal_reveal(geometry);
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource001 coordinate callback %08X\n", index);
#endif
            abort();
    }
}

static uint8 title_renderer_group_value(sint32 index)
{
    if ((uint32)index >= sizeof(title_renderer.group_values))
        abort();
    return title_renderer.group_values[index];
}

static void title_renderer_emit_tile(UI_COMMAND_STREAM input, uint32 ordering, const TITLE_RENDER_VIEW *view)
{
    uint16 wire_halfword_0;
    uint16 wire_halfword_1;
    uint16 wire_halfword_2;
    uint16 wire_halfword_3;
    uint16 wire_halfword_4;
    uint16 wire_halfword_5;
    uint32 *gpu_word;
    TITLE_RENDER_GEOMETRY geometry;
    uint32 packet, value, slot, link;
    sint32 selector, expected, index, x, y, origin_x, origin_y;

    FUNCTION_MARKER(0x800DAE44, "ACE3.BIN");
    const uint8 *command = input.data;
    if (!command || input.bytes < 20)
        abort();
    selector = (sint16)(memcpy(&wire_halfword_0, command + 12, sizeof(wire_halfword_0)), wire_halfword_0);
    if (selector)
    {
        expected = (sint16)(memcpy(&wire_halfword_1, command + 14, sizeof(wire_halfword_1)), wire_halfword_1);
        value = title_renderer_group_value(selector);
        if (expected != (sint32)value)
            return;
    }
    packet = title_renderer.packet_cursor;
    title_renderer.packet_cursor = (uint32)(packet + 16);
    geometry.y = (memcpy(&wire_halfword_2, command + 2, sizeof(wire_halfword_2)), wire_halfword_2);
    geometry.x = (memcpy(&wire_halfword_3, command, sizeof(wire_halfword_3)), wire_halfword_3);
    geometry.height = (memcpy(&wire_halfword_4, command + 6, sizeof(wire_halfword_4)), wire_halfword_4);
    geometry.width = (memcpy(&wire_halfword_5, command + 4, sizeof(wire_halfword_5)), wire_halfword_5);
    index = (sint16)title_renderer.config.transform;
    if (!title_renderer_apply_transform((uint32)index, &geometry))
        return;
    if (!title_renderer.active_packet_page || packet < title_renderer.packet_begin || packet - title_renderer.packet_begin > sizeof(*title_renderer.active_packet_page) - 16)
        abort();
    TILE *output = (void *)((uint8 *)title_renderer.active_packet_page + (packet - title_renderer.packet_begin));
    origin_x = (sint16)view->x;
    x = (sint16)geometry.x;
    origin_y = (sint16)view->y;
    y = (sint16)geometry.y;
    output->p[0].xy = (uint32)(((uint32)(x + origin_x) & 0xFFFF) | ((uint32)(y + origin_y) << 16));
    output->wh = (uint32)((uint32)geometry.width | ((uint32)geometry.height << 16));
    value = *(command + 8);
    output->rgbc.r = (uint8)(value);
    value = *(command + 9);
    output->rgbc.g = (uint8)(value);
    value = *(command + 10);
    output->tag = (output->tag & 0xFFFFFF) | ((uint32)(uint8)(3) << 24);
    output->rgbc.code = (uint8)(0x60);
    output->rgbc.b = (uint8)(value);
    value = *(command + 11);
    output->rgbc.code = (uint8)(value ? 0x62 : 0x60);
    slot = (sint16)title_renderer.config.ordering_slot;
    gpu_word = (void *)ui_scene_ot_span(ordering + slot * 4, sizeof(*gpu_word));
    link = *gpu_word;
    output->tag = (uint32)(link | 0x03000000);
    slot = (sint16)title_renderer.config.ordering_slot;
    *gpu_word = packet & 0xFFFFFF;
}

static void title_renderer_emit_line(UI_COMMAND_STREAM input, uint32 ordering, const TITLE_RENDER_VIEW *view)
{
    uint16 wire_halfword_0;
    uint16 wire_halfword_1;
    uint16 wire_halfword_2;
    uint16 wire_halfword_3;
    uint16 wire_halfword_4;
    uint16 wire_halfword_5;
    uint32 *gpu_word;
    TITLE_RENDER_GEOMETRY geometry;
    uint32 packet, value, slot, tag, link, coordinate, origin;
    sint32 selector, expected, index;

    FUNCTION_MARKER(0x800DB928, "ACE3.BIN");
    const uint8 *command = input.data;
    if (!command || input.bytes < 20)
        abort();
    selector = (sint16)(memcpy(&wire_halfword_0, command + 12, sizeof(wire_halfword_0)), wire_halfword_0);
    if (selector)
    {
        expected = (sint16)(memcpy(&wire_halfword_1, command + 14, sizeof(wire_halfword_1)), wire_halfword_1);
        value = title_renderer_group_value(selector);
        if (expected != (sint32)value)
            return;
    }
    packet = title_renderer.packet_cursor;
    title_renderer.packet_cursor = (uint32)(packet + 16);
    geometry.y = (memcpy(&wire_halfword_2, command + 2, sizeof(wire_halfword_2)), wire_halfword_2);
    geometry.x = (memcpy(&wire_halfword_3, command, sizeof(wire_halfword_3)), wire_halfword_3);
    geometry.height = (memcpy(&wire_halfword_4, command + 6, sizeof(wire_halfword_4)), wire_halfword_4);
    geometry.width = (memcpy(&wire_halfword_5, command + 4, sizeof(wire_halfword_5)), wire_halfword_5);
    index = (sint16)title_renderer.config.transform;
    if (!title_renderer_apply_transform((uint32)index, &geometry))
        return;
    if (!title_renderer.active_packet_page || packet < title_renderer.packet_begin || packet - title_renderer.packet_begin > sizeof(*title_renderer.active_packet_page) - 16)
        abort();
    LINE_F2 *output = (void *)((uint8 *)title_renderer.active_packet_page + (packet - title_renderer.packet_begin));
    origin = view->x;
    coordinate = geometry.x;
    output->p[0].x = (sint16)(uint16)(coordinate + origin);
    coordinate = geometry.y;
    origin = view->y;
    output->p[0].y = (sint16)(uint16)(coordinate + origin);
    coordinate = geometry.width;
    origin = view->x;
    output->p[1].x = (sint16)(uint16)(coordinate + origin);
    coordinate = geometry.height;
    origin = view->y;
    output->p[1].y = (sint16)(uint16)(coordinate + origin);
    value = *(command + 8);
    output->rgbc.r = (uint8)(value);
    value = *(command + 9);
    output->rgbc.g = (uint8)(value);
    value = *(command + 10);
    output->tag = (output->tag & 0xFFFFFF) | ((uint32)(uint8)(3) << 24);
    output->rgbc.code = (uint8)(0x40);
    output->rgbc.b = (uint8)(value);
    value = *(command + 11);
    output->rgbc.code = (uint8)(value ? 0x42 : 0x40);
    slot = (sint16)title_renderer.config.ordering_slot;
    tag = output->tag;
    gpu_word = (void *)ui_scene_ot_span(ordering + slot * 4, sizeof(*gpu_word));
    link = *gpu_word;
    output->tag = (uint32)((tag & 0xFF000000) | (link & 0xFFFFFF));
    slot = (sint16)title_renderer.config.ordering_slot;
    link = *gpu_word;
    *gpu_word = (link & 0xFF000000) | (packet & 0xFFFFFF);
}

static void title_renderer_emit_sprite(UI_COMMAND_STREAM input, uint32 ordering, const TITLE_RENDER_VIEW *view)
{
    uint16 wire_halfword_0;
    uint16 wire_halfword_1;
    uint16 wire_halfword_2;
    uint16 wire_halfword_3;
    uint16 wire_halfword_4;
    uint16 wire_halfword_5;
    uint16 wire_halfword_6;
    uint16 wire_halfword_7;
    uint16 wire_halfword_8;
    uint16 wire_halfword_9;
    uint32 *gpu_word;
    uint32 packet, value, slot, link, u, v, clut;
    sint32 selector, expected, index, x, y, origin_x, origin_y, width, height, texture_x, texture_y;

    FUNCTION_MARKER(0x800DB024, "ACE3.BIN");
    const uint8 *command = input.data;
    if (!command || input.bytes < 28)
        abort();
    selector = (sint16)(memcpy(&wire_halfword_0, command + 20, sizeof(wire_halfword_0)), wire_halfword_0);
    if (selector)
    {
        expected = (sint16)(memcpy(&wire_halfword_1, command + 22, sizeof(wire_halfword_1)), wire_halfword_1);
        value = title_renderer_group_value(selector);
        if (expected != (sint32)value)
            return;
    }
    packet = title_renderer.packet_cursor;
    title_renderer.packet_cursor = (uint32)(packet + 20);
    index = (sint16)title_renderer.config.transform;
    if (!title_renderer_apply_transform((uint32)index, NULL))
        return;
    if (!title_renderer.active_packet_page || packet < title_renderer.packet_begin || packet - title_renderer.packet_begin > sizeof(*title_renderer.active_packet_page) - 20)
        abort();
    SPRT *output = (void *)((uint8 *)title_renderer.active_packet_page + (packet - title_renderer.packet_begin));
    x = (sint16)(memcpy(&wire_halfword_2, command, sizeof(wire_halfword_2)), wire_halfword_2);
    origin_x = (sint16)view->x;
    origin_y = (sint16)view->y;
    y = (sint16)(memcpy(&wire_halfword_3, command + 2, sizeof(wire_halfword_3)), wire_halfword_3);
    output->p[0].xy.xy = (uint32)(((uint32)(x + origin_x) & 0xFFFF) | ((uint32)(y + origin_y) << 16));
    width = (sint16)(memcpy(&wire_halfword_4, command + 16, sizeof(wire_halfword_4)), wire_halfword_4);
    height = (sint16)(memcpy(&wire_halfword_5, command + 18, sizeof(wire_halfword_5)), wire_halfword_5);
    {
        uint32 dimensions = ((((uint32)width << 2) & 0xFFFF) | ((uint32)height << 16));
        memcpy((uint8 *)output + offsetof(SPRT, w), &dimensions, sizeof(dimensions));
    }
    texture_x = (sint16)(memcpy(&wire_halfword_6, command + 12, sizeof(wire_halfword_6)), wire_halfword_6);
    texture_y = (sint16)(memcpy(&wire_halfword_7, command + 14, sizeof(wire_halfword_7)), wire_halfword_7);
    u = (uint32)(texture_x % 64) * 4;
    v = (uint32)(texture_y % 256) << 8;
    y = (sint16)(memcpy(&wire_halfword_8, command + 10, sizeof(wire_halfword_8)), wire_halfword_8);
    value = (memcpy(&wire_halfword_9, command + 8, sizeof(wire_halfword_9)), wire_halfword_9);
    clut = ((uint32)y << 6) | ((value >> 4) & 63);
    output->p[0].uv_attr = (uint32)(u | v | (clut << 16));
    value = *(command + 4);
    output->rgbc.r = (uint8)(value);
    value = *(command + 5);
    output->rgbc.g = (uint8)(value);
    value = *(command + 6);
    output->tag = (output->tag & 0xFFFFFF) | ((uint32)(uint8)(4) << 24);
    output->rgbc.code = (uint8)(0x64);
    output->rgbc.b = (uint8)(value);
    value = *(command + 7);
    output->rgbc.code = (uint8)(value ? 0x66 : 0x64);
    slot = (sint16)title_renderer.config.ordering_slot;
    gpu_word = (void *)ui_scene_ot_span(ordering + slot * 4, sizeof(*gpu_word));
    link = *gpu_word;
    output->tag = (uint32)(link | 0x04000000);
    slot = (sint16)title_renderer.config.ordering_slot;
    *gpu_word = packet & 0xFFFFFF;
}

static void title_renderer_emit_textured_quad(UI_COMMAND_STREAM input, uint32 ordering, const TITLE_RENDER_VIEW *view)
{
    uint16 wire_halfword_0;
    uint16 wire_halfword_1;
    uint16 wire_halfword_2;
    uint16 wire_halfword_3;
    uint16 wire_halfword_4;
    uint16 wire_halfword_5;
    uint16 wire_halfword_6;
    uint16 wire_halfword_7;
    uint16 wire_halfword_8;
    uint16 wire_halfword_9;
    uint16 wire_halfword_10;
    uint16 wire_halfword_11;
    uint16 wire_halfword_12;
    uint16 wire_halfword_13;
    uint16 wire_halfword_14;
    uint16 wire_halfword_15;
    uint16 wire_halfword_16;
    uint16 wire_halfword_17;
    uint16 wire_halfword_18;
    uint32 *gpu_word;
    TITLE_RENDER_GEOMETRY geometry;
    uint32 packet, value, blend = 0;
    uint32 width_span, height_span, top, bottom, left, right, clut, depth, page_y, page_x, page, slot, link;
    sint32 selector, expected, index, scale, texture_x, texture_y, flip, u, v, width, actual, height;
    sint32 x, y, origin_x, origin_y;
    int semitransparent;

    FUNCTION_MARKER(0x800DB374, "ACE3.BIN");
    const uint8 *command = input.data;
    if (!command || input.bytes < 32)
        abort();
    selector = (sint16)(memcpy(&wire_halfword_0, command + 24, sizeof(wire_halfword_0)), wire_halfword_0);
    if (selector)
    {
        expected = (sint16)(memcpy(&wire_halfword_1, command + 26, sizeof(wire_halfword_1)), wire_halfword_1);
        value = title_renderer_group_value(selector);
        if (expected != (sint32)value)
            return;
    }
    packet = title_renderer.packet_cursor;
    title_renderer.packet_cursor = (uint32)(packet + 40);
    value = *(command + 7);
    semitransparent = value != 255;
    if (semitransparent)
        blend = *(command + 7);
    geometry.y = (memcpy(&wire_halfword_2, command + 2, sizeof(wire_halfword_2)), wire_halfword_2);
    geometry.x = (memcpy(&wire_halfword_3, command, sizeof(wire_halfword_3)), wire_halfword_3);
    geometry.height = (memcpy(&wire_halfword_4, command + 22, sizeof(wire_halfword_4)), wire_halfword_4);
    geometry.width = (memcpy(&wire_halfword_5, command + 20, sizeof(wire_halfword_5)), wire_halfword_5);
    index = (sint16)title_renderer.config.transform;
    if (!title_renderer_apply_transform((uint32)index, &geometry))
        return;
    if (!title_renderer.active_packet_page || packet < title_renderer.packet_begin || packet - title_renderer.packet_begin > sizeof(*title_renderer.active_packet_page) - 40)
        abort();
    POLY_FT4 *output = (void *)((uint8 *)title_renderer.active_packet_page + (packet - title_renderer.packet_begin));
    scale = *(command + 30) ? 2 : 4;
    flip = (sint16)(memcpy(&wire_halfword_6, command + 20, sizeof(wire_halfword_6)), wire_halfword_6);
    texture_x = (sint16)(memcpy(&wire_halfword_7, command + 12, sizeof(wire_halfword_7)), wire_halfword_7);
    u = (texture_x % 64) * scale;
    if (flip < 0)
        --u;
    flip = (sint16)(memcpy(&wire_halfword_8, command + 22, sizeof(wire_halfword_8)), wire_halfword_8);
    texture_y = (sint16)(memcpy(&wire_halfword_9, command + 14, sizeof(wire_halfword_9)), wire_halfword_9);
    v = texture_y % 256;
    if (flip < 0)
        --v;
    width = (sint16)(memcpy(&wire_halfword_10, command + 16, sizeof(wire_halfword_10)), wire_halfword_10);
    actual = (sint16)(memcpy(&wire_halfword_11, command + 20, sizeof(wire_halfword_11)), wire_halfword_11);
    width *= scale;
    width_span = actual == width ? (uint32)actual : (uint32)width - 1;
    actual = (sint16)(memcpy(&wire_halfword_12, command + 22, sizeof(wire_halfword_12)), wire_halfword_12);
    height = (sint16)(memcpy(&wire_halfword_13, command + 18, sizeof(wire_halfword_13)), wire_halfword_13);
    value = (memcpy(&wire_halfword_14, command + 18, sizeof(wire_halfword_14)), wire_halfword_14);
    height_span = actual == height ? value : value - 1;
    if ((sint16)u + (sint16)width_span == 256)
        --width_span;
    if ((sint16)v + (sint16)height_span == 256)
        --height_span;
    value = *(command + 4);
    output->rgbc.r = (uint8)(value);
    value = *(command + 5);
    output->rgbc.g = (uint8)(value);
    value = *(command + 6);
    output->tag = (output->tag & 0xFFFFFF) | ((uint32)(uint8)(9) << 24);
    output->rgbc.code = (uint8)(0x2C);
    output->rgbc.b = (uint8)(value);
    output->rgbc.code = (uint8)(semitransparent ? 0x2E : 0x2C);
    left = (sint16)u;
    top = (uint32)(sint32)(sint16)v << 8;
    y = (sint16)(memcpy(&wire_halfword_15, command + 10, sizeof(wire_halfword_15)), wire_halfword_15);
    value = (memcpy(&wire_halfword_16, command + 8, sizeof(wire_halfword_16)), wire_halfword_16);
    clut = ((uint32)y << 6) | ((value >> 4) & 63);
    bottom = (uint32)((sint16)v + (sint16)height_span) << 8;
    output->p[0].uv_attr = (uint32)(left | top | (clut << 16));
    depth = *(command + 30);
    page_y = (memcpy(&wire_halfword_17, command + 14, sizeof(wire_halfword_17)), wire_halfword_17);
    page_x = (memcpy(&wire_halfword_18, command + 12, sizeof(wire_halfword_18)), wire_halfword_18);
    output->p[2].uv_attr = (uint32)(left | bottom);
    right = left + (uint32)(sint32)(sint16)width_span;
    output->p[3].uv_attr = (uint32)(right | bottom);
    page = ((depth & 3) << 7) | ((blend & 3) << 5) | ((page_y & 0x100) >> 4) | ((page_x & 0x3FF) >> 6) | ((page_y & 0x200) << 2);
    output->p[1].uv_attr = (uint32)(right | top | (page << 16));
    origin_y = (sint16)view->y;
    y = (sint16)geometry.y;
    origin_x = (sint16)view->x;
    x = (sint16)geometry.x;
    output->p[0].xy.xy = (uint32)(((uint32)(y + origin_y) << 16) | ((uint32)(x + origin_x) & 0xFFFF));
    y = (sint16)geometry.y;
    origin_y = (sint16)view->y;
    origin_x = (sint16)view->x;
    width = (sint16)geometry.width;
    x = (sint16)geometry.x;
    output->p[1].xy.xy = (uint32)(((uint32)(y + origin_y) << 16) | ((uint32)(x + origin_x + width) & 0xFFFF));
    y = (sint16)geometry.y;
    origin_y = (sint16)view->y;
    height = (sint16)geometry.height;
    x = (sint16)geometry.x;
    origin_x = (sint16)view->x;
    output->p[2].xy.xy = (uint32)(((uint32)(y + origin_y + height) << 16) | ((uint32)(x + origin_x) & 0xFFFF));
    y = (sint16)geometry.y;
    origin_y = (sint16)view->y;
    height = (sint16)geometry.height;
    width = (sint16)geometry.width;
    x = (sint16)geometry.x;
    origin_x = (sint16)view->x;
    output->p[3].xy.xy = (uint32)(((uint32)(y + origin_y + height) << 16) | ((uint32)(x + origin_x + width) & 0xFFFF));
    slot = (sint16)title_renderer.config.ordering_slot;
    gpu_word = (void *)ui_scene_ot_span(ordering + slot * 4, sizeof(*gpu_word));
    link = *gpu_word;
    output->tag = (uint32)(link | 0x09000000);
    slot = (sint16)title_renderer.config.ordering_slot;
    *gpu_word = packet & 0xFFFFFF;
}

static void title_renderer_emit_command(uint32 opcode, UI_COMMAND_STREAM command, uint32 ordering, const TITLE_RENDER_VIEW *view)
{
    switch (opcode)
    {
        case 0:
            title_renderer_emit_tile(command, ordering, view);
            return;
        case 1:
            title_renderer_emit_sprite(command, ordering, view);
            return;
        case 2:
            title_renderer_emit_draw_mode(command, ordering);
            return;
        case 3:
            title_renderer_emit_textured_quad(command, ordering, view);
            return;
        case 4:
            title_renderer_emit_draw_area(command, ordering);
            return;
        case 5:
            title_renderer_set_draw_config(command);
            return;
        case 6:
            title_renderer_emit_line(command, ordering, view);
            return;
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound resource001 packet producer %08X\n", opcode);
#endif
            abort();
    }
}

static void title_renderer_finish_group(uint32 index)
{
    switch (index)
    {
        case 7:
            title_menu_draw_account_actions();
            return;
        case 10:
            title_renderer_group10_hook();
            return;
        case 11:
            title_menu_draw_keyboard();
            return;
        case 12:
            title_renderer_group12_hook();
            return;
        case 13:
            title_renderer_group13_hook();
            return;
        case 14:
            title_renderer_group14_hook();
            return;
        case 17:
            title_renderer_group17_hook();
            return;
        case 18:
            title_renderer_group18_hook();
            return;
        case 19:
            title_renderer_group19_hook();
            return;
        default:
            title_renderer_default_group_hook();
            return;
    }
}

void title_renderer_draw_group(uint32 index)
{
    uint32 wire_word_0;
    const UI_COMMAND_STREAM *stream;
    const uint8 *cursor;
    uint32 remaining, ordering, opcode, bytes;
    static const uint8 payload_bytes[] = {20, 28, 12, 32, 20, 16, 20};

    FUNCTION_MARKER(0x800DC6C4, "ACE3.BIN");
    if (index >= sizeof(title_renderer.command_streams) / sizeof(*title_renderer.command_streams))
        abort();
    stream = &title_renderer.command_streams[index];
    cursor = stream->data;
    remaining = stream->bytes;
    ordering = title_renderer.ordering;
    while (remaining >= 4)
    {
        opcode = (memcpy(&wire_word_0, cursor, sizeof(wire_word_0)), wire_word_0);
        if (opcode == 0xFFFFFFFF)
        {
            title_renderer_finish_group(index);
            return;
        }
        if (opcode >= sizeof(payload_bytes))
            abort();
        bytes = 4 + payload_bytes[opcode];
        if (remaining < bytes)
            abort();
        title_renderer_emit_command(opcode, ui_command_directory(cursor, bytes, 4, bytes), ordering, &title_renderer.views[index]);
        cursor += bytes;
        remaining -= bytes;
    }
    abort();
}

void title_renderer_draw_visible(void)
{
    uint32 bits, index = 0;

    FUNCTION_MARKER(0x800DC7AC, "ACE3.BIN");
    bits = title_renderer.visible_groups;
    while (bits && index < 32)
    {
        if (bits & 1)
            title_renderer_draw_group(index);
        ++index;
        bits >>= 1;
    }
}

void title_renderer_draw_frame(void)
{
    uint32 index, buffer;

    FUNCTION_MARKER(0x800DCAA4, "ACE3.BIN");
    VSync(1);
    index = game_runtime.display_buffer;
    buffer = title_renderer.packet_pages[index];
    title_renderer.packet_cursor = (uint32)(buffer);
    title_renderer.packet_begin = buffer;
    title_renderer.active_packet_page = title_renderer.packet_data[index];
    title_renderer_draw_visible();
    {
        (void)(title_renderer_tick_animations());
        return;
    }
}

uint32 title_renderer_ordering(void)
{
    FUNCTION_MARKER(0x800DA6FC, "ACE3.BIN");
    return title_renderer.ordering;
}

void title_renderer_set_group_value(uint32 index, uint32 value)
{
    FUNCTION_MARKER(0x800DA9A8, "ACE3.BIN");
    if (index >= 64)
        return;
    title_renderer.group_values[index] = (uint8)value;
}

uint32 title_renderer_set_animation(uint32 index, uint32 enabled, sint32 value, sint32 duration)
{
    FUNCTION_MARKER(0x800DA838, "ACE3.BIN");
    if (index >= 16)
        return 0;
    title_renderer.animation_mode[index] = (uint16)enabled;
    if (duration >= 0)
        title_renderer.animation_limit[index] = (uint16)duration;
    if (value >= 0)
        title_renderer.animation_phase[index] = (uint16)value;
    return 0;
}

uint32 title_renderer_set_hint(uint32 x, uint32 y, uint32 first_flag, uint32 second_flag)
{
    uint32 flags;

    FUNCTION_MARKER(0x800DBF6C, "ACE3.BIN");
    title_renderer.hint_x = (uint16)(x);
    title_renderer.hint_y = (uint16)(y);
    flags = title_renderer.hint_flags;
    flags = first_flag == 1 ? flags | 1 : flags & 0xFFFFFFFE;
    title_renderer.hint_flags = (uint32)(flags);
    flags = title_renderer.hint_flags;
    flags = second_flag == 1 ? flags | 4 : flags & 0xFFFFFFFB;
    title_renderer.hint_flags = (uint32)(flags);
    return flags;
}

sint32 title_renderer_animation_mode(uint32 index)
{
    FUNCTION_MARKER(0x800DA898, "ACE3.BIN");
    if (index >= 16)
        abort();
    return (sint16)title_renderer.animation_mode[index];
}

void title_renderer_default_group_hook(void)
{
    FUNCTION_MARKER(0x800DC42C, "ACE3.BIN");
}

void title_renderer_group10_hook(void)
{
    FUNCTION_MARKER(0x800DC5E4, "ACE3.BIN");
}

void title_renderer_group12_hook(void)
{
    FUNCTION_MARKER(0x800DC694, "ACE3.BIN");
}

void title_renderer_group13_hook(void)
{
    FUNCTION_MARKER(0x800DC69C, "ACE3.BIN");
}

void title_renderer_group14_hook(void)
{
    FUNCTION_MARKER(0x800DC6A4, "ACE3.BIN");
}

void title_renderer_group17_hook(void)
{
    FUNCTION_MARKER(0x800DC6AC, "ACE3.BIN");
}

void title_renderer_group18_hook(void)
{
    FUNCTION_MARKER(0x800DC6B4, "ACE3.BIN");
}

void title_renderer_group19_hook(void)
{
    FUNCTION_MARKER(0x800DC6BC, "ACE3.BIN");
}

uint32 title_renderer_set_name_hint_mode(uint32 mode)
{
    uint32 flags;

    FUNCTION_MARKER(0x800DC028, "ACE3.BIN");
    flags = title_renderer.hint_flags;
    if (mode == 2)
        flags |= 10;
    else if (mode == 1)
        flags = (flags | 2) & 0xFFFFFFF7;
    else
        flags &= 0xFFFFFFF5;
    title_renderer.hint_flags = (uint32)(flags);
    return flags;
}

uint32 ui_emit_menu_fill(const MENU_FILL_DESCRIPTOR *descriptor, uint32 packet, uint32 *ot_entry)
{
    uint32 value, index, tag, link;
    TILE *fill = ui_packet_span(packet, sizeof(*fill));
    FUNCTION_MARKER(0x800F7180, "ACE3.BIN");
    fill->tag = (fill->tag & 0xFFFFFF) | ((uint32)(uint8)(3) << 24);
    fill->rgbc.code = (uint8)(0x60);
    value = descriptor->semi;
    fill->rgbc.code = (uint8)(value ? 0x62 : 0x60);
    value = descriptor->red;
    fill->rgbc.r = (uint8)value;
    value = descriptor->green;
    fill->rgbc.g = (uint8)value;
    value = descriptor->blue;
    fill->rgbc.b = (uint8)value;
    for (index = 0; index < 6; index += 2)
    {
        value = (index == 0) ? descriptor->x : (index == 2) ? descriptor->y : descriptor->width;
        if (index == 0)
            fill->p[0].x = (sint16)(uint16)value;
        else if (index == 2)
            fill->p[0].y = (sint16)(uint16)value;
        else
            fill->w = (sint16)(uint16)value;
    }
    value = descriptor->height;
    tag = fill->tag;
    fill->h = (sint16)(uint16)value;
    link = *ot_entry;
    fill->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    link = *ot_entry;
    *ot_entry = (link & 0xFF000000) | (packet & 0xFFFFFF);
    return packet + 16;
}

uint32 ui_emit_controller_label(uint32 x, uint32 y, const MENU_BINDING_IMAGE *descriptor, uint32 blend, uint32 packet)
{
    uint32 *ordering_words;
    uint32 value, coordinate, component, page, tag, link;
    sint32 remainder;

    POLY_FT4 *quad = ui_packet_span(packet, sizeof(*quad));
    FUNCTION_MARKER(0x800D1588, "ACE3.BIN");
    quad->tag = (quad->tag & 0xFFFFFF) | ((uint32)(9) << 24);
    quad->rgbc.code = (uint8)(46);
    quad->p[0].xy.x = (sint16)(uint16)(x);
    quad->p[0].xy.y = (sint16)(uint16)(y);
    value = descriptor->width;
    quad->p[2].xy.x = (sint16)(uint16)(x);
    x -= 1;
    quad->p[1].xy.y = (sint16)(uint16)(y);
    quad->p[1].xy.x = (sint16)(uint16)(value + x);
    value = descriptor->height;
    y -= 1;
    quad->p[2].xy.y = (sint16)(uint16)(value + y);
    value = descriptor->width;
    quad->p[3].xy.x = (sint16)(uint16)(value + x);
    value = descriptor->height;
    quad->p[3].xy.y = (sint16)(uint16)(value + y);
    remainder = (sint16)descriptor->vram_x % 64;
    quad->p[0].uv.u = (uint8)((uint32)(remainder * 4));
    remainder = (sint16)descriptor->vram_y % 256;
    quad->p[0].uv.v = (uint8)((uint32)remainder);
    remainder = (sint16)descriptor->vram_x % 64;
    value = (uint8)descriptor->width;
    quad->p[1].uv.u = (uint8)((uint32)(remainder * 4) + value - 1);
    remainder = (sint16)descriptor->vram_y % 256;
    quad->p[1].uv.v = (uint8)((uint32)remainder);
    remainder = (sint16)descriptor->vram_x % 64;
    quad->p[2].uv.u = (uint8)((uint32)(remainder * 4));
    remainder = (sint16)descriptor->vram_y % 256;
    value = (uint8)descriptor->height;
    quad->p[2].uv.v = (uint8)((uint32)remainder + value - 1);
    remainder = (sint16)descriptor->vram_x % 64;
    value = (uint8)descriptor->width;
    quad->p[3].uv.u = (uint8)((uint32)(remainder * 4) + value - 1);
    remainder = (sint16)descriptor->vram_y % 256;
    value = (uint8)descriptor->height;
    quad->rgbc.r = (uint8)(128);
    quad->rgbc.g = (uint8)(128);
    quad->rgbc.b = (uint8)(128);
    quad->p[3].uv.v = (uint8)((uint32)remainder + value - 1);
    component = (blend & 3) << 5;
    coordinate = descriptor->vram_y;
    value = descriptor->vram_x;
    quad->p[0].attr = (uint16)(0x55A);
    page = component | ((coordinate & 0x100) >> 4) | ((value & 0x3FF) >> 6) | ((coordinate & 0x200) << 2);
    quad->p[1].attr = (uint16)(page);
    ordering_words = ui_scene_ot_span(ui_packets.scene_ot, 16 * sizeof(*ordering_words));
    tag = quad->tag;
    link = ordering_words[1];
    quad->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    ordering_words = ui_scene_ot_span(ui_packets.scene_ot, 16 * sizeof(*ordering_words));
    link = ordering_words[1];
    ordering_words[1] = (link & 0xFF000000) | (packet & 0xFFFFFF);
    return packet + 40;
}

typedef char controller_label_packet_layout_check[sizeof(POLY_FT4) == 40 && offsetof(POLY_FT4, rgbc) == 4 && offsetof(POLY_FT4, p) == 8 && sizeof(POINT_T) == 8 && offsetof(POINT_T, uv) == 4 && offsetof(POINT_T, attr) == 6 ? 1 : -1];

uint32 ui_emit_draw_mode(uint32 mode, uint32 ordering, uint32 packet)
{
    uint32 *ordering_word;
    DR_MODE *draw_mode;
    uint32 link;

    FUNCTION_MARKER(0x80101514, "ACE3.BIN");
    draw_mode = (DR_MODE *)ui_packet_span(packet, sizeof(*draw_mode));
    SetDrawMode(draw_mode, 0, 0, (mode & 3) << 5, NULL);
    /* The original GPU packet ABI requires word-aligned packet and OT pointers */
    ordering_word = (void *)ui_scene_ot_span(ordering, sizeof(*ordering_word));
    link = *ordering_word;
    ((P_TAG *)draw_mode)->addr = link;
    ((P_TAG *)ordering_word)->addr = packet;
    return packet + 12;
}

void ui_draw_panel_line(uint32 x0, uint32 y0, uint32 x1, uint32 y1)
{
    UI_CONTROL clip_control;
    uint32 packet, ordering, depth, tag, link, result;

    FUNCTION_MARKER(0x800E0F88, "ACE3.BIN");
    packet = ui_packets.scene_packet_cursor;
    ordering = ui_packets.scene_ot;
    ui_packets.scene_packet_cursor = (uint32)(packet + 16);
    LINE_F2 *line = ui_packet_span(packet, sizeof(*line));
    ((uint8 *)&line->tag)[3] = (uint8)(3);
    line->rgbc.code = (uint8)(0x40);
    line->rgbc.r = (uint8)(96);
    line->rgbc.g = (uint8)(96);
    line->rgbc.b = (uint8)(96);
    ordering += 32;
    line->p[0].x = (sint16)(uint16)(x0);
    line->p[0].y = (sint16)(uint16)(y0);
    line->p[1].x = (sint16)(uint16)(x1);
    line->p[1].y = (sint16)(uint16)(y1);
    ui_control_read(&clip_control);
    depth = clip_control.layer;
    tag = line->tag;
    link = *(uint32 *)ui_scene_ot_span(ordering + depth * 4 + 4, 4);
    line->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    ui_control_read(&clip_control);
    depth = clip_control.layer;
    link = *(uint32 *)ui_scene_ot_span(ordering + depth * 4 + 4, 4);
    result = (link & 0xFF000000) | (packet & 0xFFFFFF);
    *(uint32 *)ui_scene_ot_span(ordering + depth * 4 + 4, 4) = result;
    {
        (void)(uint32)(result);
        return;
    }
}

void ui_draw_panel_tile(uint32 x, uint32 y, uint32 width, uint32 height, uint32 transparent)
{
    UI_CONTROL clip_control;
    uint32 packet, ordering, depth, tag, link, result;

    FUNCTION_MARKER(0x800E1040, "ACE3.BIN");
    packet = ui_packets.scene_packet_cursor;
    ui_packets.scene_packet_cursor = (uint32)(packet + 16);
    ordering = ui_packets.scene_ot;
    TILE *tile = ui_packet_span(packet, sizeof(*tile));
    ((uint8 *)&tile->tag)[3] = (uint8)(3);
    tile->rgbc.code = (uint8)(0x60);
    ordering += 32;
    tile->rgbc.code = (uint8)((transparent & 255) == 0 ? 0x60 : 0x62);
    tile->rgbc.r = (uint8)(96);
    tile->rgbc.g = (uint8)(96);
    tile->rgbc.b = (uint8)(96);
    tile->p[0].x = (sint16)(uint16)(x);
    tile->p[0].y = (sint16)(uint16)(y);
    tile->w = (sint16)(uint16)(width);
    tile->h = (sint16)(uint16)(height);
    ui_control_read(&clip_control);
    depth = clip_control.layer;
    tag = tile->tag;
    link = *(uint32 *)ui_scene_ot_span(ordering + depth * 4 + 4, 4);
    tile->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    ui_control_read(&clip_control);
    depth = clip_control.layer;
    link = *(uint32 *)ui_scene_ot_span(ordering + depth * 4 + 4, 4);
    result = (link & 0xFF000000) | (packet & 0xFFFFFF);
    *(uint32 *)ui_scene_ot_span(ordering + depth * 4 + 4, 4) = result;
    {
        (void)(uint32)(result);
        return;
    }
}

typedef char ui_line_packet_layout_check[sizeof(LINE_F2) == 16 && offsetof(LINE_F2, rgbc) == 4 && offsetof(LINE_F2, p) == 8 && sizeof(TILE) == 16 && offsetof(TILE, rgbc) == 4 && offsetof(TILE, p) == 8 && offsetof(TILE, w) == 12 && offsetof(TILE, h) == 14 && sizeof(LINE_G2) == 20 && offsetof(LINE_G2, p) == 4 && sizeof(POINT_C) == 8 && offsetof(POINT_C, xy) == 4 && sizeof(XY) == 4 && offsetof(XY, y) == 2 && sizeof(RGBC) == 4 && offsetof(RGBC, code) == 3 && sizeof(DR_MODE) == 12 ? 1 : -1];

void ui_command_idle_hook(void)
{
    FUNCTION_MARKER(0x800E03CC, "ACE3.BIN");
}

static sint32 ui_clip_divide(uint32 numerator, sint32 denominator)
{
    if (denominator == 0 || (numerator == 0x80000000 && denominator == -1))
    {
#ifndef NDEBUG
        fprintf(stderr, "Resource2 clipping division BREAK %u\n", denominator == 0 ? 7 : 6);
#endif
        abort();
    }
    return (sint32)numerator / denominator;
}

static uint32 ui_clip_allow(PSX_RECT *rect)
{
    FUNCTION_MARKER(0x800DEFEC, "ACE3.BIN");
    return 1;
}

static uint32 ui_clip_grow_axes(PSX_RECT *rect)
{
    UI_CONTROL clip_control;
    uint32 index, first, middle, last, product, value;
    sint32 current, phase, begin, split, finish, a, b, quotient, denominator;

    FUNCTION_MARKER(0x800DEFFC, "ACE3.BIN");
    ui_control_read(&clip_control);
    index = (uint32)clip_control.channel;
    middle = (uint32)clip_control.split_tick;
    first = (uint16)(clip_control.start_tick);
    current = (sint16)(ui_animation_channel(index)->ticks);
    ui_control_read(&clip_control);
    last = (uint16)(clip_control.end_tick);
    ui_control_read(&clip_control);
    index = (uint32)clip_control.channel;
    phase = (sint16)(ui_animation_channel(index)->phase);
    if (phase == 0)
        return 0;
    if (phase == 2)
        return 1;
    begin = (sint16)first;
    split = (sint16)middle;
    finish = (sint16)last;
    if (current < begin)
        return 0;
    if (current < split)
    {
        a = ((sint16)rect->x);
        b = clip_control.x;
        product = (uint32)(a - b) * (uint32)(current - begin);
        quotient = ui_clip_divide(product, split - begin);
        value = (uint16)(clip_control.x);
        rect->x = (sint16)(value + (uint32)quotient);
        ui_control_read(&clip_control);
        a = ((sint16)rect->y);
        b = clip_control.y;
        denominator = a - b;
        if (denominator < 0)
            denominator = -denominator;
        quotient = ui_clip_divide((uint32)(a - b), denominator);
        value = (uint16)(clip_control.y);
        rect->y = (sint16)(value + (uint32)quotient);
        a = ((sint16)rect->w);
        product = (uint32)a * (uint32)(current - begin);
        quotient = ui_clip_divide(product, split - begin);
        rect->h = (sint16)(2);
        rect->w = (sint16)((uint32)quotient);
    }
    else if (current < finish)
    {
        a = ((sint16)rect->y);
        b = clip_control.y;
        product = (uint32)(a - b) * (uint32)(current - split);
        quotient = ui_clip_divide(product, finish - split);
        value = (uint16)(clip_control.y);
        rect->y = (sint16)(value + (uint32)quotient);
        a = ((sint16)rect->h);
        product = (uint32)a * (uint32)(current - split);
        quotient = ui_clip_divide(product, finish - split);
        rect->h = (sint16)((uint32)quotient);
    }
    return 1;
}

static uint32 ui_clip_grow_axes_frame(PSX_RECT *rect)
{
    FUNCTION_MARKER(0x800DEFF4, "ACE3.BIN");
    return ui_clip_grow_axes(rect);
}

static uint32 ui_clip_when_open(PSX_RECT *rect)
{
    UI_CONTROL clip_control;
    uint32 index;

    FUNCTION_MARKER(0x800DF2B0, "ACE3.BIN");
    ui_control_read(&clip_control);
    index = (uint32)clip_control.channel;
    return (sint16)(ui_animation_channel(index)->phase) == 2;
}

static uint32 ui_clip_grow_horizontal(PSX_RECT *rect)
{
    UI_CONTROL clip_control;
    uint32 index, first, middle, product, value;
    sint32 current, phase, begin, split, a, b, quotient;

    FUNCTION_MARKER(0x800DF2F0, "ACE3.BIN");
    ui_control_read(&clip_control);
    index = (uint32)clip_control.channel;
    middle = (uint32)clip_control.split_tick;
    first = (uint16)(clip_control.start_tick);
    current = (sint16)(ui_animation_channel(index)->ticks);
    ui_control_read(&clip_control);
    ui_control_read(&clip_control);
    index = (uint32)clip_control.channel;
    phase = (sint16)(ui_animation_channel(index)->phase);
    if (phase == 0)
        return 0;
    if (phase == 2)
        return 1;
    begin = (sint16)first;
    split = (sint16)middle;
    if (current < begin)
        return 0;
    if (current < split)
    {
        a = ((sint16)rect->x);
        b = clip_control.x;
        product = (uint32)(a - b) * (uint32)(current - begin);
        quotient = ui_clip_divide(product, split - begin);
        value = (uint16)(clip_control.x);
        rect->x = (sint16)(value + (uint32)quotient);
        a = ((sint16)rect->w);
        product = (uint32)a * (uint32)(current - begin);
        quotient = ui_clip_divide(product, split - begin);
        rect->w = (sint16)((uint32)quotient);
    }
    return 1;
}

static uint32 ui_clip_grow_horizontal_frame(PSX_RECT *rect)
{
    FUNCTION_MARKER(0x800DF2E8, "ACE3.BIN");
    return ui_clip_grow_horizontal(rect);
}

static uint32 ui_clip_slide_linear(PSX_RECT *rect)
{
    UI_CONTROL clip_control;
    uint32 index, first, middle, product, value;
    sint32 current, phase, begin, split, a, quotient;

    FUNCTION_MARKER(0x800DF464, "ACE3.BIN");
    ui_control_read(&clip_control);
    index = (uint32)clip_control.channel;
    middle = (uint32)clip_control.split_tick;
    first = (uint16)(clip_control.start_tick);
    current = (sint16)(ui_animation_channel(index)->ticks);
    ui_control_read(&clip_control);
    ui_control_read(&clip_control);
    index = (uint32)clip_control.channel;
    phase = (sint16)(ui_animation_channel(index)->phase);
    if (phase == 0)
        return 0;
    if (phase == 2)
        return 1;
    begin = (sint16)first;
    split = (sint16)middle;
    if (current < begin)
        return 0;
    if (current < split)
    {
        a = clip_control.x;
        product = (uint32)a * (uint32)(split - current);
        quotient = ui_clip_divide(product, split - begin);
        value = ((uint16)rect->x);
        rect->x = (sint16)(value + (uint32)quotient);
        ui_control_read(&clip_control);
        a = clip_control.y;
        product = (uint32)a * (uint32)(split - current);
        quotient = ui_clip_divide(product, split - begin);
        value = ((uint16)rect->y);
        rect->y = (sint16)(value + (uint32)quotient);
    }
    return 1;
}

static uint32 ui_clip_slide_linear_frame(PSX_RECT *rect)
{
    FUNCTION_MARKER(0x800DF45C, "ACE3.BIN");
    return ui_clip_slide_linear(rect);
}

static uint32 ui_clip_slide_quadratic(PSX_RECT *rect)
{
    UI_CONTROL clip_control;
    uint32 index, first, middle, product, square, denominator, value;
    sint32 current, phase, begin, split, a, quotient;

    FUNCTION_MARKER(0x800DF5E0, "ACE3.BIN");
    ui_control_read(&clip_control);
    index = (uint32)clip_control.channel;
    middle = (uint32)clip_control.split_tick;
    first = (uint16)(clip_control.start_tick);
    current = (sint16)(ui_animation_channel(index)->ticks);
    ui_control_read(&clip_control);
    ui_control_read(&clip_control);
    index = (uint32)clip_control.channel;
    phase = (sint16)(ui_animation_channel(index)->phase);
    if (phase == 0)
        return 0;
    if (phase == 2)
        return 1;
    begin = (sint16)first;
    split = (sint16)middle;
    if (current < begin)
        return 0;
    if (current < split)
    {
        square = (uint32)(split - current) * (uint32)(split - current);
        denominator = (uint32)(split - begin) * (uint32)(split - begin);
        a = clip_control.x;
        product = (uint32)a * square;
        quotient = ui_clip_divide(product, (sint32)denominator);
        value = ((uint16)rect->x);
        rect->x = (sint16)(value + (uint32)quotient);
        ui_control_read(&clip_control);
        a = clip_control.y;
        product = (uint32)a * square;
        quotient = ui_clip_divide(product, (sint32)denominator);
        value = ((uint16)rect->y);
        rect->y = (sint16)(value + (uint32)quotient);
    }
    return 1;
}

static uint32 ui_clip_slide_quadratic_frame(PSX_RECT *rect)
{
    FUNCTION_MARKER(0x800DF5D8, "ACE3.BIN");
    return ui_clip_slide_quadratic(rect);
}

static uint32 ui_clip_apply_mode(sint32 mode, PSX_RECT *rect)
{
    static uint32 (*const modes[])(PSX_RECT *) = {
        ui_clip_allow, ui_clip_grow_axes_frame, ui_clip_when_open, ui_clip_grow_horizontal_frame, ui_clip_slide_quadratic_frame, ui_clip_slide_linear_frame,
    };

    if ((uint32)mode >= sizeof(modes) / sizeof(*modes))
    {
#ifndef NDEBUG
        fprintf(stderr, "Unbound resource2 clipping mode %d\n", mode);
#endif
        abort();
    }
    return modes[mode](rect);
}

const uint8 *ui_command_draw_tile(UI_COMMAND_STREAM input, uint32 ordering, const UI_ITEM *context)
{
    uint16 wire_halfword_0;
    uint16 wire_halfword_1;
    uint16 wire_halfword_2;
    uint16 wire_halfword_3;
    uint16 wire_halfword_4;
    uint16 wire_halfword_5;
    UI_CONTROL clip_control;
    PSX_RECT rectangle;
    PSX_RECT *rect = &rectangle;
    uint32 packet, value, x, y, width, height;
    uint32 red, green, blue, layer;
    sint32 condition, expected, mode, offset_x, offset_y;

    uint32 *ordering_entry;
    FUNCTION_MARKER(0x800DF760, "ACE3.BIN");
    const uint8 *command = input.data;
    if (!command || input.bytes < 20)
        abort();
    condition = (sint16)(memcpy(&wire_halfword_0, command + 12, sizeof(wire_halfword_0)), wire_halfword_0);
    if (condition)
    {
        expected = (sint16)(memcpy(&wire_halfword_1, command + 14, sizeof(wire_halfword_1)), wire_halfword_1);
        value = *ui_widget_value((uint32)condition);
        if ((uint32)expected != value)
            return command + 20;
    }
    packet = ui_packets.cursor;
    ui_packets.cursor = (uint32)(packet + 16);
    y = (sint16)(memcpy(&wire_halfword_2, command + 2, sizeof(wire_halfword_2)), wire_halfword_2);
    x = (memcpy(&wire_halfword_3, command, sizeof(wire_halfword_3)), wire_halfword_3);
    rect->x = (sint16)x;
    rect->y = (sint16)y;
    height = (sint16)(memcpy(&wire_halfword_4, command + 6, sizeof(wire_halfword_4)), wire_halfword_4);
    width = (memcpy(&wire_halfword_5, command + 4, sizeof(wire_halfword_5)), wire_halfword_5);
    rect->w = (sint16)width;
    rect->h = (sint16)height;
    ui_control_read(&clip_control);
    mode = clip_control.mode;
    if (!ui_clip_apply_mode(mode, rect))
        return command + 20;
    offset_x = (sint16)context->x;
    x = ((sint16)rect->x);
    offset_y = (sint16)context->y;
    y = ((sint16)rect->y);
    if (!ui_packets.scene_packet_data || packet < ui_packets.scene_packet_begin || packet - ui_packets.scene_packet_begin > sizeof(*ui_packets.scene_packet_data) - 16)
        abort();
    TILE *primitive = (void *)((uint8 *)ui_packets.scene_packet_data + (packet - ui_packets.scene_packet_begin));
    primitive->p[0].xy = ((x + (uint32)offset_x) & 0xFFFF) | ((y + (uint32)offset_y) << 16);
    height = ((sint16)rect->h);
    width = ((uint16)rect->w);
    primitive->wh = width | (height << 16);
    red = command[8];
    green = command[9];
    blue = command[10];
    ((uint8 *)&primitive->tag)[3] = (uint8)(3);
    primitive->rgbc.rgbc = red | (green << 8) | (blue << 16);
    primitive->rgbc.code = (uint8)(96);
    value = command[11];
    primitive->rgbc.code = (uint8)(value ? 98 : 96);
    ui_control_read(&clip_control);
    layer = (uint32)clip_control.layer;
    ordering_entry = (void *)ui_scene_ot_span(ordering + layer * 4, sizeof(*ordering_entry));
    value = *ordering_entry;
    primitive->tag = value | 0x03000000;
    ui_control_read(&clip_control);
    layer = (uint32)clip_control.layer;
    ordering_entry = (void *)ui_scene_ot_span(ordering + layer * 4, sizeof(*ordering_entry));
    *ordering_entry = packet & 0xFFFFFF;
    return command + 20;
}

const uint8 *ui_command_draw_sprite(UI_COMMAND_STREAM input, uint32 ordering, const UI_ITEM *context)
{
    uint16 wire_halfword_0;
    uint16 wire_halfword_1;
    uint16 wire_halfword_2;
    uint16 wire_halfword_3;
    uint16 wire_halfword_4;
    uint16 wire_halfword_5;
    uint16 wire_halfword_6;
    uint16 wire_halfword_7;
    uint16 wire_halfword_8;
    uint16 wire_halfword_9;
    uint16 wire_halfword_10;
    uint16 wire_halfword_11;
    UI_CONTROL clip_control;
    PSX_RECT rectangle;
    PSX_RECT *rect = &rectangle;
    uint32 packet, value, x, y, width, height;
    uint32 red, green, blue, layer, u, v, page;
    sint32 condition, expected, mode, offset_x, offset_y;

    uint32 *ordering_entry;
    FUNCTION_MARKER(0x800DF940, "ACE3.BIN");
    const uint8 *command = input.data;
    if (!command || input.bytes < 28)
        abort();
    condition = (sint16)(memcpy(&wire_halfword_0, command + 20, sizeof(wire_halfword_0)), wire_halfword_0);
    if (condition)
    {
        expected = (sint16)(memcpy(&wire_halfword_1, command + 22, sizeof(wire_halfword_1)), wire_halfword_1);
        value = *ui_widget_value((uint32)condition);
        if ((uint32)expected != value)
            return command + 28;
    }
    packet = ui_packets.cursor;
    ui_packets.cursor = (uint32)(packet + 20);
    y = (sint16)(memcpy(&wire_halfword_2, command + 2, sizeof(wire_halfword_2)), wire_halfword_2);
    x = (memcpy(&wire_halfword_3, command, sizeof(wire_halfword_3)), wire_halfword_3);
    rect->x = (sint16)x;
    rect->y = (sint16)y;
    height = (sint16)(memcpy(&wire_halfword_4, command + 18, sizeof(wire_halfword_4)), wire_halfword_4);
    width = (memcpy(&wire_halfword_5, command + 16, sizeof(wire_halfword_5)), wire_halfword_5);
    rect->w = (sint16)width;
    rect->h = (sint16)height;
    ui_control_read(&clip_control);
    mode = clip_control.mode;
    if (!ui_clip_apply_mode(mode, rect))
        return command + 28;
    offset_x = (sint16)context->x;
    x = ((sint16)rect->x);
    y = ((sint16)rect->y);
    offset_y = (sint16)context->y;
    if (!ui_packets.scene_packet_data || packet < ui_packets.scene_packet_begin || packet - ui_packets.scene_packet_begin > sizeof(*ui_packets.scene_packet_data) - 20)
        abort();
    SPRT *primitive = (void *)((uint8 *)ui_packets.scene_packet_data + (packet - ui_packets.scene_packet_begin));
    primitive->p[0].xy.xy = ((x + (uint32)offset_x) & 0xFFFF) | ((y + (uint32)offset_y) << 16);
    width = (sint16)(memcpy(&wire_halfword_6, command + 16, sizeof(wire_halfword_6)), wire_halfword_6);
    height = (sint16)(memcpy(&wire_halfword_7, command + 18, sizeof(wire_halfword_7)), wire_halfword_7);
    uint32 dimensions = ((width << 2) & 0xFFFF) | (height << 16);
    memcpy((uint8 *)primitive + offsetof(SPRT, w), &dimensions, sizeof(dimensions));
    u = (uint32)((sint16)(memcpy(&wire_halfword_8, command + 12, sizeof(wire_halfword_8)), wire_halfword_8) % 64) << 2;
    v = (uint32)((sint16)(memcpy(&wire_halfword_9, command + 14, sizeof(wire_halfword_9)), wire_halfword_9) % 256) << 8;
    page = (uint32)(sint16)(memcpy(&wire_halfword_10, command + 10, sizeof(wire_halfword_10)), wire_halfword_10) << 6;
    value = (memcpy(&wire_halfword_11, command + 8, sizeof(wire_halfword_11)), wire_halfword_11);
    page |= (value >> 4) & 63;
    primitive->p[0].uv_attr = u | v | (page << 16);
    red = command[4];
    green = command[5];
    blue = command[6];
    ((uint8 *)&primitive->tag)[3] = (uint8)(4);
    primitive->rgbc.rgbc = red | (green << 8) | (blue << 16);
    primitive->rgbc.code = (uint8)(100);
    value = command[7];
    primitive->rgbc.code = (uint8)(value ? 102 : 100);
    ui_control_read(&clip_control);
    layer = (uint32)clip_control.layer;
    ordering_entry = (void *)ui_scene_ot_span(ordering + layer * 4, sizeof(*ordering_entry));
    value = *ordering_entry;
    primitive->tag = value | 0x04000000;
    ui_control_read(&clip_control);
    layer = (uint32)clip_control.layer;
    ordering_entry = (void *)ui_scene_ot_span(ordering + layer * 4, sizeof(*ordering_entry));
    *ordering_entry = packet & 0xFFFFFF;
    return command + 28;
}

const uint8 *ui_command_draw_line(UI_COMMAND_STREAM input, uint32 ordering, const UI_ITEM *context)
{
    uint16 wire_halfword_0;
    uint16 wire_halfword_1;
    uint16 wire_halfword_2;
    uint16 wire_halfword_3;
    uint16 wire_halfword_4;
    uint16 wire_halfword_5;
    UI_CONTROL clip_control;
    PSX_RECT rectangle;
    PSX_RECT *rect = &rectangle;
    uint32 packet, value, x, y, width, height;
    uint32 red, green, blue, layer, tag, link;
    sint32 condition, expected, mode;

    uint32 *ordering_entry;
    FUNCTION_MARKER(0x800E01C4, "ACE3.BIN");
    const uint8 *command = input.data;
    if (!command || input.bytes < 20)
        abort();
    condition = (sint16)(memcpy(&wire_halfword_0, command + 12, sizeof(wire_halfword_0)), wire_halfword_0);
    if (condition)
    {
        expected = (sint16)(memcpy(&wire_halfword_1, command + 14, sizeof(wire_halfword_1)), wire_halfword_1);
        value = *ui_widget_value((uint32)condition);
        if ((uint32)expected != value)
            return command + 20;
    }
    packet = ui_packets.cursor;
    ui_packets.cursor = (uint32)(packet + 16);
    y = (sint16)(memcpy(&wire_halfword_2, command + 2, sizeof(wire_halfword_2)), wire_halfword_2);
    x = (memcpy(&wire_halfword_3, command, sizeof(wire_halfword_3)), wire_halfword_3);
    rect->x = (sint16)x;
    rect->y = (sint16)y;
    height = (sint16)(memcpy(&wire_halfword_4, command + 6, sizeof(wire_halfword_4)), wire_halfword_4);
    width = (memcpy(&wire_halfword_5, command + 4, sizeof(wire_halfword_5)), wire_halfword_5);
    rect->w = (sint16)width;
    rect->h = (sint16)height;
    ui_control_read(&clip_control);
    mode = clip_control.mode;
    if (!ui_clip_apply_mode(mode, rect))
        return command + 20;
    value = context->x;
    x = ((uint16)rect->x);
    if (!ui_packets.scene_packet_data || packet < ui_packets.scene_packet_begin || packet - ui_packets.scene_packet_begin > sizeof(*ui_packets.scene_packet_data) - 16)
        abort();
    LINE_F2 *primitive = (void *)((uint8 *)ui_packets.scene_packet_data + (packet - ui_packets.scene_packet_begin));
    primitive->p[0].x = (sint16)(uint16)(x + value);
    y = ((uint16)rect->y);
    value = context->y;
    primitive->p[0].y = (sint16)(uint16)(y + value);
    width = ((uint16)rect->w);
    value = context->x;
    primitive->p[1].x = (sint16)(uint16)(width + value);
    height = ((uint16)rect->h);
    value = context->y;
    primitive->p[1].y = (sint16)(uint16)(height + value);
    red = command[8];
    green = command[9];
    blue = command[10];
    ((uint8 *)&primitive->tag)[3] = (uint8)(3);
    primitive->rgbc.rgbc = red | (green << 8) | (blue << 16);
    primitive->rgbc.code = (uint8)(64);
    value = command[11];
    primitive->rgbc.code = (uint8)(value ? 66 : 64);
    ui_control_read(&clip_control);
    layer = (uint32)clip_control.layer;
    tag = primitive->tag;
    ordering_entry = (void *)ui_scene_ot_span(ordering + layer * 4, sizeof(*ordering_entry));
    link = *ordering_entry;
    primitive->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    ui_control_read(&clip_control);
    layer = (uint32)clip_control.layer;
    ordering_entry = (void *)ui_scene_ot_span(ordering + layer * 4, sizeof(*ordering_entry));
    tag = *ordering_entry;
    *ordering_entry = (tag & 0xFF000000) | (packet & 0xFFFFFF);
    return command + 20;
}

const uint8 *ui_command_draw_textured_quad(UI_COMMAND_STREAM input, uint32 ordering, const UI_ITEM *context)
{
    uint16 wire_halfword_0;
    uint16 wire_halfword_1;
    uint16 wire_halfword_2;
    uint16 wire_halfword_3;
    uint16 wire_halfword_4;
    uint16 wire_halfword_5;
    uint16 wire_halfword_6;
    uint16 wire_halfword_7;
    uint16 wire_halfword_8;
    uint16 wire_halfword_9;
    uint16 wire_halfword_10;
    uint16 wire_halfword_11;
    uint16 wire_halfword_12;
    uint16 wire_halfword_13;
    uint16 wire_halfword_14;
    uint16 wire_halfword_15;
    uint16 wire_halfword_16;
    uint16 wire_halfword_17;
    uint16 wire_halfword_18;
    UI_CONTROL clip_control;
    PSX_RECT rectangle;
    PSX_RECT *rect = &rectangle;
    uint32 packet, value, x, y, width, height;
    uint32 blend, semitransparent, scale, format, red, green, blue, layer, u, v, du, dv;
    uint32 product, clut, bottom, right, page, a, b, c;
    sint32 condition, expected, mode;
    int negative;

    FUNCTION_MARKER(0x800DFCC8, "ACE3.BIN");
    const uint8 *command = input.data;
    if (!command || input.bytes < 32)
        abort();
    condition = (sint16)(memcpy(&wire_halfword_0, command + 24, sizeof(wire_halfword_0)), wire_halfword_0);
    if (condition)
    {
        expected = (sint16)(memcpy(&wire_halfword_1, command + 26, sizeof(wire_halfword_1)), wire_halfword_1);
        value = *ui_widget_value((uint32)condition);
        if ((uint32)expected != value)
            return command + 32;
    }
    packet = ui_packets.cursor;
    blend = 0;
    ui_packets.cursor = (uint32)(packet + 40);
    value = command[7];
    semitransparent = value != 255;
    if (value != 255)
        blend = command[7];
    y = (uint32)(sint16)(memcpy(&wire_halfword_2, command + 2, sizeof(wire_halfword_2)), wire_halfword_2);
    x = (memcpy(&wire_halfword_3, command, sizeof(wire_halfword_3)), wire_halfword_3);
    rect->x = (sint16)x;
    rect->y = (sint16)y;
    height = (uint32)(sint16)(memcpy(&wire_halfword_4, command + 22, sizeof(wire_halfword_4)), wire_halfword_4);
    width = (memcpy(&wire_halfword_5, command + 20, sizeof(wire_halfword_5)), wire_halfword_5);
    rect->w = (sint16)width;
    rect->h = (sint16)height;
    ui_control_read(&clip_control);
    mode = clip_control.mode;
    if (!ui_clip_apply_mode(mode, rect))
        return command + 32;
    format = command[30];
    if (format == 0)
        scale = 4;
    else if (format == 1)
        scale = 2;
    else
    {
#ifndef NDEBUG
        fprintf(stderr, "Resource2 textured command has undefined scale for format %u\n", format);
#endif
        abort();
    }
    negative = (sint16)(memcpy(&wire_halfword_6, command + 20, sizeof(wire_halfword_6)), wire_halfword_6) < 0;
    value = (uint32)(sint16)(memcpy(&wire_halfword_7, command + 12, sizeof(wire_halfword_7)), wire_halfword_7);
    u = (uint32)((sint32)value % 64) * scale;
    if (negative)
        --u;
    negative = (sint16)(memcpy(&wire_halfword_8, command + 22, sizeof(wire_halfword_8)), wire_halfword_8) < 0;
    value = (uint32)(sint16)(memcpy(&wire_halfword_9, command + 14, sizeof(wire_halfword_9)), wire_halfword_9);
    v = (uint32)((sint32)value % 256);
    if (negative)
        --v;
    value = (uint32)(sint16)(memcpy(&wire_halfword_10, command + 16, sizeof(wire_halfword_10)), wire_halfword_10);
    product = value * scale;
    width = (uint32)(sint16)(memcpy(&wire_halfword_11, command + 20, sizeof(wire_halfword_11)), wire_halfword_11);
    du = width == product ? width : product - 1;
    height = (uint32)(sint16)(memcpy(&wire_halfword_12, command + 22, sizeof(wire_halfword_12)), wire_halfword_12);
    value = (uint32)(sint16)(memcpy(&wire_halfword_13, command + 18, sizeof(wire_halfword_13)), wire_halfword_13);
    a = (memcpy(&wire_halfword_14, command + 18, sizeof(wire_halfword_14)), wire_halfword_14);
    dv = height == value ? a : a - 1;
    if ((sint16)u + (sint16)du == 256)
        --du;
    if ((sint16)v + (sint16)dv == 256)
        --dv;
    red = command[4];
    green = command[5];
    blue = command[6];
    if (!ui_packets.scene_packet_data || packet < ui_packets.scene_packet_begin || packet - ui_packets.scene_packet_begin > sizeof(*ui_packets.scene_packet_data) - 40)
        abort();
    POLY_FT4 *quad = (void *)((uint8 *)ui_packets.scene_packet_data + (packet - ui_packets.scene_packet_begin));
    ((uint8 *)&quad->tag)[3] = (uint8)(9);
    quad->rgbc.rgbc = red | (green << 8) | (blue << 16);
    quad->rgbc.code = (uint8)(44);
    quad->rgbc.code = (uint8)(semitransparent ? 46 : 44);
    u = (uint32)(sint32)(sint16)u;
    v = (uint32)(sint32)(sint16)v;
    clut = (uint32)(sint16)(memcpy(&wire_halfword_15, command + 10, sizeof(wire_halfword_15)), wire_halfword_15) << 6;
    value = (memcpy(&wire_halfword_16, command + 8, sizeof(wire_halfword_16)), wire_halfword_16);
    clut |= (value >> 4) & 63;
    bottom = (v + (uint32)(sint32)(sint16)dv) << 8;
    quad->p[0].uv_attr = u | (v << 8) | (clut << 16);
    format = command[30];
    a = (memcpy(&wire_halfword_17, command + 14, sizeof(wire_halfword_17)), wire_halfword_17);
    b = (memcpy(&wire_halfword_18, command + 12, sizeof(wire_halfword_18)), wire_halfword_18);
    quad->p[2].uv_attr = u | bottom;
    right = u + (uint32)(sint32)(sint16)du;
    quad->p[3].uv_attr = right | bottom;
    page = ((format & 3) << 7) | ((blend & 3) << 5) | ((a & 0x100) >> 4) | ((b & 0x3FF) >> 6) | ((a & 0x200) << 2);
    quad->p[1].uv_attr = right | (v << 8) | (page << 16);
    a = (uint32)(sint16)context->y;
    y = (uint32)((sint16)rect->y);
    b = (uint32)(sint16)context->x;
    x = (uint32)((sint16)rect->x);
    quad->p[0].xy.xy = ((y + a) << 16) | ((x + b) & 0xFFFF);
    y = (uint32)((sint16)rect->y);
    a = (uint32)(sint16)context->y;
    b = (uint32)(sint16)context->x;
    width = (uint32)((sint16)rect->w);
    x = (uint32)((sint16)rect->x);
    quad->p[1].xy.xy = ((y + a) << 16) | ((x + b + width) & 0xFFFF);
    y = (uint32)((sint16)rect->y);
    a = (uint32)(sint16)context->y;
    height = (uint32)((sint16)rect->h);
    x = (uint32)((sint16)rect->x);
    b = (uint32)(sint16)context->x;
    quad->p[2].xy.xy = ((y + a + height) << 16) | ((x + b) & 0xFFFF);
    y = (uint32)((sint16)rect->y);
    a = (uint32)(sint16)context->y;
    height = (uint32)((sint16)rect->h);
    width = (uint32)((sint16)rect->w);
    x = (uint32)((sint16)rect->x);
    b = (uint32)(sint16)context->x;
    c = ((y + a + height) << 16) | ((x + b + width) & 0xFFFF);
    quad->p[3].xy.xy = c;
    ui_control_read(&clip_control);
    layer = (uint32)clip_control.layer;
    value = *(uint32 *)(void *)ui_scene_ot_span(ordering + layer * 4, 4);
    quad->tag = value | 0x09000000;
    ui_control_read(&clip_control);
    layer = (uint32)clip_control.layer;
    *(uint32 *)(void *)ui_scene_ot_span(ordering + layer * 4, 4) = packet & 0xFFFFFF;
    return command + 32;
}

uint32 ui_draw_frontend_load_progress(uint32 packet)
{
    MENU_POLYGON_DESCRIPTOR descriptor;
    uint32 completed, total, product, ordering, result, i;
    sint32 amount, enabled;

    FUNCTION_MARKER(0x800FB330, "ACE3.BIN");
    for (i = 0; i < 4; ++i)
    {
        descriptor.color[i][0] = 229;
        descriptor.color[i][1] = 229;
        descriptor.color[i][2] = 213;
    }
    descriptor.color[0][3] = 0;
    completed = mission_flow.frontend_load.completed_bytes;
    total = mission_flow.frontend_load.total_bytes;
    product = completed * 115;
    amount = ui_clip_divide(product, (sint32)total);
    if (amount >= 115)
        amount = 114;
    else if (amount < 0)
        amount = 0;
    descriptor.points[0].x = (uint16)(192);
    descriptor.points[2].x = (uint16)(192);
    descriptor.points[0].y = (uint16)(420);
    descriptor.points[1].y = (uint16)(420);
    descriptor.points[1].x = (uint16)((uint32)amount + 192);
    descriptor.points[2].y = (uint16)(426);
    descriptor.points[3].x = (uint16)((uint32)amount + 192);
    descriptor.points[3].y = (uint16)(426);
    ordering = ui_packets.scene_ot;
    result = ui_emit_colored_quad(&descriptor, packet, ui_scene_ot_span(ordering, sizeof(uint32)));
    for (i = 0; i < 6; ++i)
    {
        completed = mission_flow.frontend_load.completed_bytes;
        total = mission_flow.frontend_load.total_bytes;
        product = completed * 7;
        amount = ui_clip_divide(product, (sint32)total);
        enabled = (sint32)i < amount;
        ui_widget_set(i + 1, (uint32)enabled);
    }
    return result;
}

typedef char ui_basic_primitive_packet_layout_check[sizeof(TILE) == 16 && offsetof(TILE, tag) == 0 && offsetof(TILE, rgbc) == 4 && offsetof(TILE, p) == 8 && offsetof(TILE, wh) == 12 && sizeof(SPRT) == 20 && offsetof(SPRT, rgbc) == 4 && offsetof(SPRT, p) == 8 && offsetof(SPRT, w) == 16 && offsetof(SPRT, h) == 18 && sizeof(LINE_F2) == 16 && offsetof(LINE_F2, tag) == 0 && offsetof(LINE_F2, rgbc) == 4 && offsetof(LINE_F2, p) == 8 && sizeof(XY) == 4 && sizeof(POINT_T) == 8 && offsetof(POINT_T, uv_attr) == 4 && sizeof(RGBC) == 4 && offsetof(RGBC, code) == 3 ? 1 : -1];

uint32 ui_draw_debrief_area(uint32 packet, uint32 mode)
{
    uint32 ordering, tag, link;
    PSX_RECT rectangle;
    uint32 *ordering_entry;
    DR_AREA *draw_area;
    FUNCTION_MARKER(0x800F6774, "ACE3.BIN");
    if (mode == 0)
    {
        rectangle.x = 0;
        rectangle.y = 0;
        rectangle.w = 320;
        rectangle.h = 480;
    }
    else if (mode == 1)
    {
        rectangle.x = 20;
        rectangle.y = 70;
        rectangle.w = 128;
        rectangle.h = 240;
    }
    else
    {
#ifndef NDEBUG
        fprintf(stderr, "Resource2 draw-area mode has no initialized original rectangle: %u\n", mode);
#endif
        abort();
    }
    draw_area = ui_packet_span(packet, sizeof(*draw_area));
    SetDrawArea(draw_area, &rectangle);
    ordering = ui_packets.scene_ot;
    tag = draw_area->tag;
    ordering_entry = ui_scene_ot_span(ordering, sizeof(*ordering_entry));
    link = *ordering_entry;
    draw_area->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    ordering = ui_packets.scene_ot;
    ordering_entry = ui_scene_ot_span(ordering, sizeof(*ordering_entry));
    tag = *ordering_entry;
    *ordering_entry = (tag & 0xFF000000) | (packet & 0xFFFFFF);
    return packet + 12;
}

uint32 ui_render_frame(uint32 packet)
{
    DR_MODE *draw_mode;
    uint32 ordering, tag, link;

    uint32 *ordering_entry;
    FUNCTION_MARKER(0x800E2494, "ACE3.BIN");
    VSync(1);
    ui_packets.cursor = (uint32)(packet + 12);
    if (!ui_packets.scene_packet_data || packet < ui_packets.scene_packet_begin || packet - ui_packets.scene_packet_begin > sizeof(*ui_packets.scene_packet_data) - 12)
        abort();
    draw_mode = (DR_MODE *)(void *)((uint8 *)ui_packets.scene_packet_data + (packet - ui_packets.scene_packet_begin));
    SetDrawMode(draw_mode, 0, 0, 0, NULL);
    ordering = ui_packets.scene_ot;
    tag = draw_mode->tag;
    ordering_entry = ui_scene_ot_span(ordering, sizeof(*ordering_entry));
    link = *ordering_entry;
    draw_mode->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    ordering = ui_packets.scene_ot;
    ordering_entry = ui_scene_ot_span(ordering, sizeof(*ordering_entry));
    tag = *ordering_entry;
    *ordering_entry = (tag & 0xFF000000) | (packet & 0xFFFFFF);
    ui_groups_render();
    packet = ui_packets.cursor;
    ui_packets.cursor = (uint32)(packet + 12);
    if (!ui_packets.scene_packet_data || packet < ui_packets.scene_packet_begin || packet - ui_packets.scene_packet_begin > sizeof(*ui_packets.scene_packet_data) - 12)
        abort();
    draw_mode = (DR_MODE *)(void *)((uint8 *)ui_packets.scene_packet_data + (packet - ui_packets.scene_packet_begin));
    SetDrawMode(draw_mode, 0, 0, 0, NULL);
    ordering = ui_packets.scene_ot;
    tag = draw_mode->tag;
    ordering_entry = ui_scene_ot_span(ordering, sizeof(*ordering_entry));
    link = *ordering_entry;
    draw_mode->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    ordering = ui_packets.scene_ot;
    ordering_entry = ui_scene_ot_span(ordering, sizeof(*ordering_entry));
    tag = *ordering_entry;
    *ordering_entry = (tag & 0xFF000000) | (packet & 0xFFFFFF);
    ui_animation_tick();
    return ui_packets.cursor;
}

const uint8 *ui_command_skip(UI_COMMAND_STREAM input)
{
    FUNCTION_MARKER(0x800E019C, "ACE3.BIN");
    const uint8 *command = input.data;
    if (!command || input.bytes < 16)
        abort();
    return command + 16;
}

const uint8 *ui_command_set_draw_mode(UI_COMMAND_STREAM input, uint32 ordering)
{
    uint16 wire_halfword_0;
    uint16 wire_halfword_1;
    DR_MODE *draw_mode;
    UI_CONTROL clip_control;
    uint32 packet, blend, y, x, page, layer, tag, link;

    uint32 *ordering_entry;
    FUNCTION_MARKER(0x800DFBA8, "ACE3.BIN");
    const uint8 *command = input.data;
    if (!command || input.bytes < 12)
        abort();
    packet = ui_packets.cursor;
    ui_packets.cursor = (uint32)(packet + 12);
    blend = command[7];
    y = (memcpy(&wire_halfword_0, command + 2, sizeof(wire_halfword_0)), wire_halfword_0);
    x = (memcpy(&wire_halfword_1, command, sizeof(wire_halfword_1)), wire_halfword_1);
    page = ((blend & 3) << 5) | ((y & 0x100) >> 4) | ((x & 0x3FF) >> 6) | ((y & 0x200) << 2);
    if (!ui_packets.scene_packet_data || packet < ui_packets.scene_packet_begin || packet - ui_packets.scene_packet_begin > sizeof(*ui_packets.scene_packet_data) - 12)
        abort();
    draw_mode = (DR_MODE *)(void *)((uint8 *)ui_packets.scene_packet_data + (packet - ui_packets.scene_packet_begin));
    SetDrawMode(draw_mode, 0, 0, page, NULL);
    ui_control_read(&clip_control);
    layer = (uint32)clip_control.layer;
    tag = draw_mode->tag;
    ordering_entry = (void *)ui_scene_ot_span(ordering + layer * 4, sizeof(*ordering_entry));
    link = *ordering_entry;
    draw_mode->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    ui_control_read(&clip_control);
    layer = (uint32)clip_control.layer;
    ordering_entry = (void *)ui_scene_ot_span(ordering + layer * 4, sizeof(*ordering_entry));
    tag = *ordering_entry;
    *ordering_entry = (tag & 0xFF000000) | (packet & 0xFFFFFF);
    return command + 12;
}

void ui_compact_ordering_links(uint32 ordering, uint32 count, uint32 *entries)
{
    uint32 expected, index, run, cursor, value, result;
    FUNCTION_MARKER(0x80084978, "ACE3.BIN");
    expected = ordering & 0xFFFFFF;
    index = 1;
    run = 0;
    cursor = ordering + 4;
    while ((sint32)index < (sint32)count)
    {
        value = entries[index];
        if (value == expected)
            run += 4;
        else
        {
            if ((sint32)run >= 5)
                entries[index - 1] = expected - run;
            run = 0;
        }
        cursor += 4;
        index += 1;
        expected += 4;
    }
    result = index << 2;
    if (run != 0)
    {
        result += ordering;
        entries[index - 1] = expected - run;
    }
    {
        (void)(result);
        return;
    }
}

uint32 ui_draw_noise_overlay(uint32 ot, uint32 packet, uint32 intensity)
{
    uint32 *setup, *restore, *ot_tag;
    uint8 *tag_bytes;
    uint32 mode, end, value, link;
    FUNCTION_MARKER(0x80084474, "ACE3.BIN");
    if (intensity == 0)
        return packet;
    mode = 1;
    if ((sint32)intensity < 0)
    {
        mode = 2;
        intensity = 0 - intensity;
    }
    setup = (uint32 *)ui_packet_span(packet, 4 * sizeof(*setup));
    setup[0] = ((packet + 16) & 0xFFFFFF) | 0x03000000;
    setup[1] = (mode << 5) | 0xE1000218;
    setup[2] = 0xE20C4318;
    setup[3] = 0;
    xport_gte_write_data(6, 0x66838383);
    xport_gte_write_data(8, 4096 - intensity);
    end = ui_packets_emit_noise_tiles(packet + 16, 0x46D6, 0xFF88FF60, 0x00F00140, 0);
    restore = (uint32 *)ui_packet_span(end, 4 * sizeof(*restore));
    restore[1] = 0xE1000220;
    restore[0] = 0x03000000;
    restore[2] = 0xE2000000;
    restore[3] = 0;
    ot_tag = (uint32 *)(void *)ui_scene_ot_span(ot, sizeof(*ot_tag));
    link = *ot_tag & 0xFFFFFF;
    value = packet & 0xFFFFFF;
    tag_bytes = (uint8 *)restore;
    tag_bytes[0] = link;
    tag_bytes[1] = (link >> 8);
    tag_bytes[2] = (link >> 16);
    tag_bytes = (uint8 *)ot_tag;
    tag_bytes[0] = value;
    tag_bytes[1] = (value >> 8);
    tag_bytes[2] = (value >> 16);
    return end + 16;
}

uint32 ui_draw_scripted_flash(uint32 ordering, uint32 packet, uint32 intensity)
{
    TILE *fill;
    uint32 *ordering_word;
    uint32 tag, color;
    FUNCTION_MARKER(0x80093C90, "ACE3.BIN");
    if (intensity == 0)
        return packet;
    xport_gte_write_control(21, 48);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    xport_gte_write_data(6, 0x6203135F);
    xport_gte_write_data(8, 4096 - intensity);
    xport_gte_execute(0x780010);
    ordering_word = (void *)ui_scene_ot_span(ordering, sizeof(*ordering_word));
    tag = *ordering_word;
    fill = ui_packet_span(packet, sizeof(*fill));
    fill->tag = tag | 0x03000000;
    *ordering_word = packet & 0xFFFFFF;
    fill->p[0].xy = 0xFF88FF60;
    fill->wh = 0x00F00140;
    color = xport_gte_read_data(22);
    fill->rgbc.rgbc = color;
    return packet + 16;
}

uint32 ui_draw_framebuffer_feedback(uint32 ordering, uint32 packet, const XY *offset, uint32 intensity)
{
    uint32 *ordering_word;
    P_TAG *packet_link;
    uint32 fade, mode, value, cursor, color, page_x, page_y, flat, textured, x, link, page, tail, tag;
    FUNCTION_MARKER(0x8008FAA8, "ACE3.BIN");
    fade = 4096 - intensity;
    if ((sint32)fade < 0)
        fade = 0;
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    xport_gte_write_data(6, 0x007F7F7F);
    xport_gte_write_data(8, fade);
    value = game_runtime.display_buffer;
    mode = value == 0 ? 256 : 0;
    xport_gte_execute(0x780010);
    DR_STP *mask = ui_packet_span(packet, 8);
    cursor = packet + 8;
    mask->tag = (cursor & 0xFFFFFF) | 0x01000000;
    mask->code[0] = 0xE6000001;
    if (offset != 0)
    {
        DR_OFFSET *shift = ui_packet_span(packet + 8, sizeof(*shift));
        cursor = packet + 20;
        shift->tag = (cursor & 0xFFFFFF) | 0x02000000;
        shift->code[0] = (((mode + 256) & 256) << 11) | 0xE5000000;
        shift->code[1] = 0;
    }
    color = xport_gte_read_data(22);
    page_x = (mode & 256) >> 4;
    page_y = (mode & 512) << 2;
    flat = color | 0x62000000;
    textured = color | 0x66000000;
    x = 0;
    link = (cursor << 8) + 0x2800;
    do
    {
        FRAMEBUFFER_FEEDBACK_PACKET *feedback = ui_packet_span(cursor, sizeof(*feedback));
        page = (x & 0x3FF) >> 6;
        feedback->tag = (link >> 8) | 0x09000000;
        feedback->flat.mode = page_x | (page | 0x140) | page_y | 0xE1000200;
        feedback->textured.mode = page_x | (page | 0x120) | page_y | 0xE1000200;
        feedback->flat.color.rgbc = flat;
        feedback->textured.color.rgbc = textured;
        feedback->textured.point.xy.xy = x;
        feedback->flat.position.xy = x;
        feedback->textured.point.uv.uv = (uint16)(0);
        if (x != 0)
        {
            feedback->textured.wh = 0x00F00040;
            feedback->flat.wh = 0x00F00040;
        }
        else
        {
            feedback->textured.wh = 0x00F00100;
            feedback->flat.wh = 0x00F00100;
        }
        link += 0x2800;
        x += 256;
        cursor += 40;
    } while ((sint32)x < 320);
    tail = cursor;
    if (offset != 0)
    {
        DR_OFFSET *restore = ui_packet_span(cursor, sizeof(*restore));
        value = offset->xy;
        restore->tag = 0x02000000;
        restore->code[1] = 0;
        restore->code[0] = (value & 0x7FF) | ((((value << 5) >> 21) << 11)) | 0xE5000000;
        cursor += 12;
    }
    else
        tail = cursor - 40;
    ordering_word = (void *)ui_scene_ot_span(ordering, sizeof(*ordering_word));
    tag = *ordering_word;
    packet_link = ui_packet_span(tail, sizeof(*packet_link));
    packet_link->addr = tag;
    ((P_TAG *)ordering_word)->addr = packet & 0xFFFFFF;
    return cursor;
}

uint32 ui_draw_screen_wipe(uint32 ordering, uint32 packet, uint32 direction, uint32 progress)
{
    uint32 strength, current, value;
    FUNCTION_MARKER(0x80093BC4, "ACE3.BIN");
    if ((sint32)progress >= 4096)
        return packet;
    strength = 4096 - progress;
    SCREEN_RECT_PUBLICATION *published = (SCREEN_RECT_PUBLICATION *)getScratchAddr(0);
    published->words[0] = 0;
    published->words[1] = 0x00F00140;
    if (direction == 0)
        strength = 0 - strength;
    effect_light.flags = (uint32)(1);
    current = ui_draw_blend_rectangle(ordering, packet, published->rectangle, strength);
    DR_TPAGE *mask_packet = ui_packet_span(current, sizeof(*mask_packet));
    mask_packet->tag = 0x01000000;
    mask_packet->code[0] = 0xE6000001;
    uint32 *ordering_entry = (void *)ui_scene_ot_span(ordering, sizeof(*ordering_entry));
    value = *ordering_entry;
    mask_packet->tag = (value & 0xFFFFFF) | 0x01000000;
    value = *ordering_entry;
    *ordering_entry = (value & 0xFF000000) | (current & 0xFFFFFF);
    return current + 8;
}

uint32 ui_draw_letterbox(uint32 ordering, uint32 packet, uint32 progress)
{
    uint32 *ordering_word;
    P_TAG *ordering_link;
    uint32 value, edge, height, bottom, buffer, first, second, link;
    FUNCTION_MARKER(0x800939D4, "ACE3.BIN");
    if ((sint32)progress <= 0)
        return packet;
    if ((sint32)progress >= 4096)
        value = 4096;
    else
        value = rsin((((progress >> 2) << 2)) >> 2);
    edge = (sint32)(value << 5) >> 12;
    height = (sint32)((value * 3) << 4) >> 12;
    bottom = 240 - height;
    buffer = game_runtime.display_buffer;
    LETTERBOX_PACKET *bars = ui_packet_span(packet, sizeof(*bars));
    bars->clip.tag = (uint32)(0x02000000);
    buffer = (buffer != 0) << 8;
    first = buffer + edge;
    second = ((height << 16) | 320);
    bars->clip.code[0] = (uint32)(((first & 0x3FF) << 10) | 0xE3000000);
    bars->clip.code[1] = (uint32)((((first + bottom - edge - 1) & 0x3FF) << 10) | 0xE400013F);
    ordering_word = (void *)ui_scene_ot_span(ordering + 0x3FFC, sizeof(*ordering_word));
    value = *ordering_word;
    bars->clip.tag = (uint32)(value | 0x02000000);
    ordering_word = (void *)ui_scene_ot_span(ordering + 0x3FFC, sizeof(*ordering_word));
    *ordering_word = packet & 0xFFFFFF;
    bars->restore.tag = (uint32)(((packet + 24) & 0xFFFFFF) | 0x02000000);
    bars->restore.code[0] = (uint32)((buffer << 10) | 0xE3000000);
    bars->restore.code[1] = (uint32)(((buffer + 239) << 10) | 0xE400013F);
    bars->fill_tag = (uint32)(0x06000000);
    bars->fills[0].color.rgbc = (uint32)(0x60000000);
    bars->fills[1].color.rgbc = (uint32)(0x60000000);
    bars->fills[1].position.x = (sint16)(uint16)(0);
    bars->fills[0].position.x = (sint16)(uint16)(0);
    bars->fills[0].position.y = (sint16)(uint16)(0);
    bars->fills[1].position.y = (sint16)(uint16)(bottom);
    bars->fills[0].dimensions = (uint32)((edge << 16) | 320);
    bars->fills[1].dimensions = (uint32)(second);
    ordering_link = (void *)ui_scene_ot_span(ordering + 4, sizeof(uint32));
    link = ordering_link->addr;
    ordering_link = ui_packet_span(packet + 24, sizeof(uint32));
    ordering_link->addr = link;
    ordering_link = (void *)ui_scene_ot_span(ordering + 4, sizeof(uint32));
    ordering_link->addr = (packet + 12) & 0xFFFFFF;
    return packet + 52;
}

uint32 ui_copy_display_framebuffer(uint32 ordering, uint32 packet)
{
    uint32 flags, buffer;
    FUNCTION_MARKER(0x80036440, "ACE3.BIN");
    flags = game_runtime.display.flags;
    game_runtime.display.flags = (uint32)(flags | 4);
    buffer = game_runtime.display_buffer;
    if (buffer != 0)
        return gpu_copy_rect(ordering, packet, 0, 256, 0, 0, 320, 240);
    return gpu_copy_rect(ordering, packet, 0, 0, 0, 256, 320, 240);
}

uint32 ui_draw_noise_rectangle(uint32 ordering, uint32 packet, PSX_RECT rectangle, uint32 intensity)
{
    uint32 *setup, *restore;
    uint32 width, height, xy, wh, tail, carrier, value;
    FUNCTION_MARKER(0x800846CC, "ACE3.BIN");
    width = (sint32)rectangle.w;
    if ((sint32)width <= 0)
        return packet;
    height = (sint32)rectangle.h;
    if ((sint32)height <= 0)
        return packet;
    setup = (uint32 *)ui_packet_span(packet, 4 * sizeof(*setup));
    setup[0] = ((packet + 16) & 0xFFFFFF) | 0x03000000;
    setup[1] = 0xE1000218;
    setup[2] = 0xE20C6318;
    setup[3] = 0;
    xport_gte_write_data(6, 0x64838383);
    xport_gte_write_data(8, intensity);
    xy = (uint16)rectangle.x | ((uint32)(uint16)rectangle.y << 16);
    wh = (uint16)rectangle.w | ((uint32)(uint16)rectangle.h << 16);
    tail = ui_packets_emit_noise_tiles(packet + 16, 0x4696, xy, wh, &carrier);
    restore = (uint32 *)ui_packet_span(tail, 4 * sizeof(*restore));
    restore[1] = 0xE1000220;
    restore[0] = 0x03000000;
    restore[2] = 0xE2000000;
    restore[3] = 0;
    value = effect_light_prepend_chain((void *)ui_scene_ot_span((ordering + 2) & 0xFFFFFFFC, sizeof(uint32)), (ordering + 2) & 3, packet, tail, carrier);
    return tail + 16;
}

uint32 ui_draw_loading_fade(uint32 object, uint32 dst, uint32 mode)
{
    uint32 a = dst + 12, b = dst + 20, color = mode;
    uint32 command, last_tag, old, value;
    LOADING_FADE_PACKET *fade = ui_packet_span(dst, sizeof(*fade));
    uint8 *head_bytes;
    FUNCTION_MARKER(0x800362A0, "ACE3.BIN");
    fade->mask.tag = (uint32)((a & 0xFFFFFF) | 0x2000000);
    fade->mask.code[0] = (uint32)(0xE6000001);
    fade->mask.code[1] = (uint32)(0);
    if ((sint32)mode < 0)
    {
        color = 0 - mode;
        command = 0xE1000240;
    }
    else
    {
        command = 0xE1000220;
    }
    fade->page.tag = (uint32)((b & 0xFFFFFF) | 0x1000000);
    fade->page.code[0] = (uint32)(command);
    fade->fill.tag = (uint32)(0x3000000);
    fade->fill.rgbc.rgbc = (uint32)(color | (color << 8) | (color << 16) | 0x62000000);
    fade->fill.w = (sint16)(uint16)(320);
    fade->fill.h = (sint16)(uint16)(240);
    fade->restore.tag = (uint32)(0x2000000);
    fade->fill.p[0].x = (sint16)(uint16)(0);
    fade->fill.p[0].y = (sint16)(uint16)(0);
    fade->restore.code[0] = (uint32)(0xE6000000);
    fade->restore.code[1] = (uint32)(0);
    last_tag = fade->restore.tag;
    head_bytes = (void *)ui_scene_ot_span(object, sizeof(old));
    memcpy(&old, head_bytes, sizeof(old));
    last_tag = (last_tag & 0xFF000000) | (old & 0xFFFFFF);
    fade->restore.tag = (uint32)(last_tag);
    memcpy(&old, head_bytes, sizeof(old));
    old = (old & 0xFF000000) | ((b + 16) & 0xFFFFFF);
    memcpy(head_bytes, &old, sizeof(old));
    /* Preserve the LWL and SWL lane masks */
    value = effect_light_prepend_chain((void *)ui_scene_ot_span((object + 2) & 0xFFFFFFFC, sizeof(uint32)), (object + 2) & 3, dst, b, last_tag);
    return b + 28;
}

uint32 ui_draw_compact_label(uint32 object, uint32 dst, uint32 x, uint32 y, uint32 color, const uint8 *native_text)
{
    uint32 packet = dst, field, ch, code, uv, tag;
    FUNCTION_MARKER(0x8003563C, "ACE3.BIN");
    ch = *native_text++;
    if (ch != 0)
    {
        color |= 0x76000000;
        field = dst + 10;
        do
        {
            code = ch - 32;
            if (code != 0)
            {
                uv = ((code & 15) * 8 + 128) | (((uint32)((sint32)code >> 4) << 11) + 0x4700) | 0x7F980000;
                SPRT_8 *glyph = ui_packet_span(packet, sizeof(*glyph));
                glyph->rgbc.rgbc = color;
                glyph->p[0].uv_attr = uv;
                glyph->p[0].xy.x = (sint16)(uint16)x;
                glyph->p[0].xy.y = (sint16)(uint16)y;
                uint32 *ordering_entry = (void *)ui_scene_ot_span(object, sizeof(*ordering_entry));
                tag = *ordering_entry;
                field += 16;
                glyph->tag = tag | 0x3000000;
                tag = packet & 0xFFFFFF;
                packet += 16;
                *ordering_entry = tag;
                if (code == 45 || code == 55)
                    ++x;
                if (code == 59 || code == 61)
                    x -= 4;
                if (code - 26 < 2)
                    x -= 5;
            }
            x += 8;
            ch = *native_text++;
        } while (ch != 0);
    }
    return mission_menu_draw_mode(object, packet, 54);
}

uint32 ui_draw_frame_border(uint32 object, uint32 dst, uint32 amount, uint32 outer_color, uint32 inner_color)
{
    uint32 x, y, rx, ry, packet, tag, head, i, command;
    POLY_G4 *quads = ui_packet_span(dst, 4 * sizeof(*quads));
    uint32 *ordering_entry = (void *)ui_scene_ot_span(object, sizeof(*ordering_entry));
    FUNCTION_MARKER(0x800365FC, "ACE3.BIN");
    x = (sint32)(amount * 160) / 4096;
    rx = 320 - x;
    y = (sint32)(amount * 120) / 4096;
    ry = 240 - y;
    command = outer_color | 0x38000000;
    for (i = 0; i < 4; ++i)
    {
        POLY_G4 *quad = quads + i;
        packet = dst + i * 36;
        quad->tag = (quad->tag & 0xFFFFFF) | 0x08000000;
        if (i == 0)
            tag = quad->tag;
        quad->p[0].rgbc.code = (uint8)(56);
        quad->p[0].rgbc.rgbc = command;
        quad->p[1].rgbc.rgbc = outer_color;
        quad->p[2].rgbc.rgbc = inner_color;
        quad->p[3].rgbc.rgbc = inner_color;
        if (i == 0)
        {
            quad->p[0].xy.x = (sint16)(uint16)(0);
            quad->p[0].xy.y = (sint16)(uint16)(0);
            quad->p[1].xy.x = (sint16)(uint16)(320);
            quad->p[1].xy.y = (sint16)(uint16)(0);
            quad->p[2].xy.x = (sint16)(uint16)(x);
            quad->p[2].xy.y = (sint16)(uint16)(y);
            quad->p[3].xy.x = (sint16)(uint16)(rx);
            quad->p[3].xy.y = (sint16)(uint16)(y);
        }
        else if (i == 1)
        {
            quad->p[0].xy.x = (sint16)(uint16)(320);
            quad->p[0].xy.y = (sint16)(uint16)(240);
            quad->p[1].xy.x = (sint16)(uint16)(0);
            quad->p[1].xy.y = (sint16)(uint16)(240);
            quad->p[2].xy.x = (sint16)(uint16)(rx);
            quad->p[2].xy.y = (sint16)(uint16)(ry);
            quad->p[3].xy.x = (sint16)(uint16)(x);
            quad->p[3].xy.y = (sint16)(uint16)(ry);
        }
        else if (i == 2)
        {
            quad->p[0].xy.x = (sint16)(uint16)(0);
            quad->p[0].xy.y = (sint16)(uint16)(240);
            quad->p[1].xy.x = (sint16)(uint16)(0);
            quad->p[1].xy.y = (sint16)(uint16)(0);
            quad->p[2].xy.x = (sint16)(uint16)(x);
            quad->p[2].xy.y = (sint16)(uint16)(ry);
            quad->p[3].xy.x = (sint16)(uint16)(x);
            quad->p[3].xy.y = (sint16)(uint16)(y);
        }
        else
        {
            quad->p[0].xy.x = (sint16)(uint16)(320);
            quad->p[0].xy.y = (sint16)(uint16)(0);
            quad->p[1].xy.x = (sint16)(uint16)(320);
            quad->p[1].xy.y = (sint16)(uint16)(240);
            quad->p[2].xy.x = (sint16)(uint16)(rx);
            quad->p[2].xy.y = (sint16)(uint16)(y);
            quad->p[3].xy.x = (sint16)(uint16)(rx);
            quad->p[3].xy.y = (sint16)(uint16)(ry);
        }
        if (i != 0)
            tag = quad->tag;
        head = *ordering_entry;
        quad->tag = (tag & 0xFF000000) | (head & 0xFFFFFF);
        head = *ordering_entry;
        *ordering_entry = (head & 0xFF000000) | (packet & 0xFFFFFF);
    }
    return dst + 144;
}

uint32 ui_draw_blend_rectangle(uint32 ot, uint32 packet, PSX_RECT rect, uint32 depth)
{
    SCREEN_MODE_FILL_PACKET *fill;
    uint32 *ordering_word;
    uint32 mode = 1;
    uint32 head;
    uint32 xy;
    uint32 wh;

    FUNCTION_MARKER(0x80093B2C, "ACE3.BIN");
    if (!depth)
        return packet;
    if ((sint32)depth < 0)
    {
        depth = 0 - depth;
        mode = 2;
    }
    xport_gte_write_control(21, 0);
    xport_gte_write_control(22, 0);
    xport_gte_write_control(23, 0);
    xport_gte_write_data(6, 0x62FFFFFF);
    xport_gte_write_data(8, 4096 - depth);
    ordering_word = (void *)ui_scene_ot_span(ot, sizeof(*ordering_word));
    head = *ordering_word;
    fill = ui_packet_span(packet, sizeof(*fill));
    fill->tag = head | 0x04000000;
    *ordering_word = packet & 0xFFFFFF;
    xport_gte_execute(0x780010);
    fill->mode = 0xE1000200 | (mode << 5);
    xy = (uint16)rect.x | ((uint32)(uint16)rect.y << 16);
    wh = (uint16)rect.w | ((uint32)(uint16)rect.h << 16);
    fill->fill.position.xy = xy;
    fill->fill.dimensions = wh;
    fill->fill.color.rgbc = xport_gte_read_data(22);
    return packet + 20;
}

uint32 gpu_copy_rect(uint32 ot, uint32 packet, uint32 src_x, uint32 src_y, uint32 dst_x, uint32 dst_y, uint32 width, uint32 height)
{
    uint32 tag;
    uint32 head;

    FUNCTION_MARKER(0x800363B0, "ACE3.BIN");
    DR_MOVE *move = ui_packet_span(packet, sizeof(*move));
    move->tag = 0x05000000;
    move->code[0] = 0x01000000;
    move->code[2] = (src_x & 0xFFFF) | (src_y << 16);
    move->code[1] = 0x80000000;
    move->code[3] = (dst_x & 0xFFFF) | (dst_y << 16);
    move->code[4] = (width & 0xFFFF) | (height << 16);
    tag = move->tag;
    uint32 *ordering_entry = (void *)ui_scene_ot_span(ot, sizeof(*ordering_entry));
    head = *ordering_entry;
    move->tag = (tag & 0xFF000000) | (head & 0xFFFFFF);
    head = *ordering_entry;
    *ordering_entry = (head & 0xFF000000) | (packet & 0xFFFFFF);
    return packet + 24;
}

void title_renderer_reset_storage(void)
{
    memset(&title_renderer, 0, sizeof(title_renderer));
}

uint32 title_renderer_hint_flags(void)
{
    return title_renderer.hint_flags;
}

uint16 title_renderer_hint_x(void)
{
    return title_renderer.hint_x;
}

uint16 title_renderer_hint_y(void)
{
    return title_renderer.hint_y;
}
