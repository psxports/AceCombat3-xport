#include <stdlib.h>

#include "game_functions.h"

typedef struct
{
    DR_TPAGE page;
    RGBC fill_color;
    XY fill_position;
    uint32 fill_dimensions;
    DR_TPAGE restore;
} LOADING_STAR_COMMANDS;

typedef struct
{
    uint32 tag;
    POINT_C points[7];
} LOADING_STAR_GPU_BLOCK;

typedef struct
{
    LOADING_STAR_COMMANDS commands;
    LOADING_STAR_GPU_BLOCK blocks[74];
} LOADING_STAR_GPU_BANK;

typedef char loading_star_gpu_bank_layout_check[sizeof(LOADING_STAR_GPU_BLOCK) == 60 && offsetof(LOADING_STAR_GPU_BLOCK, points) == 4 && offsetof(LOADING_STAR_GPU_BANK, blocks) == 28 && sizeof(LOADING_STAR_GPU_BANK) == 4468 ? 1 : -1];

static LOADING_STAR_GPU_BANK loading_star_packets[2];

typedef struct
{
    union
    {
        SVECTOR vector;
        uint32 words[2];
    } point;

    uint32 rotation[5];
    uint32 base[3];
    uint32 saved_seed;
} LOADING_STAR_PUBLICATION;

typedef char loading_star_packet_layout[sizeof(LOADING_STAR_COMMANDS) == 28 && offsetof(LOADING_STAR_COMMANDS, fill_color) == 8 && offsetof(LOADING_STAR_COMMANDS, fill_position) == 12 && offsetof(LOADING_STAR_COMMANDS, fill_dimensions) == 16 && offsetof(LOADING_STAR_COMMANDS, restore) == 20 && sizeof(DR_TPAGE) == 8 && offsetof(DR_TPAGE, code) == 4 && sizeof(POINT_C) == 8 && offsetof(POINT_C, xy) == 4 && sizeof(LOADING_STAR_PUBLICATION) == 44 && offsetof(LOADING_STAR_PUBLICATION, rotation) == 8 && offsetof(LOADING_STAR_PUBLICATION, base) == 28 && offsetof(LOADING_STAR_PUBLICATION, saved_seed) == 40 && sizeof(SVECTOR) == 8 && offsetof(SVECTOR, vz) == 4 && offsetof(SVECTOR, pad) == 6 ? 1 : -1];

static const char *const mission_loading_titles[GAME_MISSION_COUNT] = {
    "MISSION CODE",         "AWAKENING",       "BRAVADO",           "ENTER DISION", "PAPER TIGER",     "BROKEN TRUCE", "GHOSTS OF THE PAST", "NO CLEARANCE", "FRAGILE CARGO", "SCYLLA AND CHARYBDIS", "FATES INTERTWINED", "REACHING FOR STARS", "ONE-WAY TICKET", "BUG HUNT",       "PAWNS IN THE GAME",  "DAMAGE CONTROL",    "BROKEN WINGS", "SPHYRNA",        "A CANOPY OF STARS", "GAME SHOW",  "SOLDIER OF FORTUNE", "MEGAFLOAT", "TARGET ACQUISITION", "PARTNERS", "TAINTED PEACE", "STRATOSPHERE", "WELCOMING COMMITTEE", "TECHNOLOGY TRANSFER", "CLAUSTROPHOBIA", "DILEMMA", "BETRAYAL",
    "HEART OF THE SERPENT", "GEOFRONT ATTACK", "CASUALTIES OF WAR", "GEOPELIA",     "THE ORIENTATION", "LIQUIDATION",  "ARCHNEMESIS",        "MEMORY ERROR", "ELECTROSPHERE", "POWER FOR LIFE",       "GUARDIAN ANGEL",    "ZERO GRAVITY",       "THE PRIZE",      "UTOPIAN DREAMS", "REALITY DISTORTION", "COUNTERREVOLUTION", "PURSUIT",      "SELF AWARENESS", "NO TITLE",          "RESISTANCE", "RADIO SILENCE",      "REVENGE",   "SOLE SURVIVOR",      "GEOPELIA", "TUNNEL VISION", "ZERO GRAVITY", "ARCHNEMESIS",         "ELECTROSPHERE",       "NO TITLE",
};

static const char *const mission_loading_difficulties[4] = {
    "LEVEL: EASY",
    "LEVEL: NORMAL",
    "LEVEL: HARD",
    "LEVEL: VERY HARD",
};

uint32 frontend_load_begin(void)
{
    RESOURCE_STREAM_BUFFER asset_allocation;
    uint32 bytes, buffer;

    FUNCTION_MARKER(0x800FB4F0, "ACE3.BIN");
    game_runtime.resource_slot.resource_id = (uint32)(113);
    bytes = resource_total_size(&game_runtime.resource_slot.resource_id, 1);
    asset_allocation = arena_alloc_backed(bytes);
    buffer = asset_allocation.encoded_begin;
    game_runtime.resource_pending = 1;
    resource_queue_asset(&game_runtime.resource_slot, asset_allocation, resource_asset_read_finished);
    mission_flow.frontend_load.total_bytes = (uint32)(1);
    mission_flow.frontend_load.completed_bytes = (uint32)(0);
    return 1;
}

uint32 frontend_load_mission(void)
{
    RESOURCE_STREAM_BUFFER asset_allocation;
    uint32 resource, bytes, total, buffer;
    sint32 mission;

    FUNCTION_MARKER(0x800FB6E8, "ACE3.BIN");
    mission = (sint16)game_runtime.selection.mission;
    if ((uint32)mission >= GAME_MISSION_COUNT)
        abort();
    resource = 126 + (uint32)mission;
    game_runtime.resource_slot.resource_id = (uint32)(resource);
    bytes = resource_total_size(&game_runtime.resource_slot.resource_id, 1);
    total = mission_flow.frontend_load.total_bytes;
    mission_flow.frontend_load.total_bytes = (uint32)(total + bytes);
    asset_allocation = arena_alloc_backed(bytes);
    buffer = asset_allocation.encoded_begin;
    mission_flow.frontend_load.read_cursor = (uint32)(buffer);
    mission_flow.frontend_load.completed_bytes = (uint32)(0);
    game_runtime.resource_pending = 1;
    resource_queue_asset(&game_runtime.resource_slot, asset_allocation, resource_asset_read_finished);
    return 1;
}

uint32 frontend_load_poll_mission(void)
{
    uint32 current, previous, total;

    FUNCTION_MARKER(0x800FB7A4, "ACE3.BIN");
    resource_read_progress(&current);
    previous = mission_flow.frontend_load.read_cursor;
    mission_flow.frontend_load.read_cursor = (uint32)(current);
    total = mission_flow.frontend_load.completed_bytes;
    mission_flow.frontend_load.completed_bytes = (uint32)(total + current - previous);
    return game_runtime.resource_pending == 0;
}

uint32 frontend_load_common(void)
{
    RESOURCE_STREAM_BUFFER asset_allocation;
    uint32 bytes, buffer;

    FUNCTION_MARKER(0x800FB948, "ACE3.BIN");
    game_runtime.resource_slot.resource_id = (uint32)(0x72);
    bytes = resource_total_size(&game_runtime.resource_slot.resource_id, 1);
    asset_allocation = arena_alloc_backed(bytes);
    buffer = asset_allocation.encoded_begin;
    mission_flow.frontend_load.read_cursor = (uint32)(buffer);
    game_runtime.resource_pending = 1;
    resource_queue_asset(&game_runtime.resource_slot, asset_allocation, resource_asset_read_finished);
    return 1;
}

uint32 frontend_load_poll_common(void)
{
    uint32 current, previous, total;

    FUNCTION_MARKER(0x800FB9C4, "ACE3.BIN");
    resource_read_progress(&current);
    previous = mission_flow.frontend_load.read_cursor;
    mission_flow.frontend_load.read_cursor = (uint32)(current);
    total = mission_flow.frontend_load.completed_bytes;
    mission_flow.frontend_load.completed_bytes = (uint32)(total + current - previous);
    if (game_runtime.resource_pending)
        return 0;
    sound_stop_voice(*menu_voice_slot(0));
    return 1;
}

uint32 frontend_draw_loading_transition(uint32 object, uint32 dst, uint32 mission)
{
    static const uint32 table[17] = {
        0x00282000, 0x00281DEB, 0x00511C00, 0x005119EB, 0x007A1800, 0x007A15EB, 0x007A1400, 0x00A311EB, 0x00A31000, 0x05470E66, 0x09EB0CCC, 0x0E8F0B33, 0x13330999, 0x12660B33, 0x11990CCC, 0x10CC0E66, 0x10001000,
    };
    PSX_RECT rectangle;
    uint32 state, progress, inverse, packet, selection, value, width, height, result;
    FUNCTION_MARKER(0x80036E84, "ACE3.BIN");
    state = mission_flow.loading.resource_kind;
    if (state == 1)
        progress = (sint16)mission_flow.loading.progress;
    else
        progress = state == 2 ? 4096 : 0;
    if ((sint32)progress >= 4097)
        progress = 4096;
    inverse = 4096 - progress;
    packet = frontend_draw_loading_labels(object, dst, mission);
    selection = mission_flow.loading.animation_ticks;
    if ((sint32)selection < 0)
        selection = 0;
    if ((sint32)selection >= 17)
        selection = 16;
    value = table[selection];
    width = ((sint32)(sint16)value * 320);
    width = (sint32)width / 4096;
    rectangle.w = (sint16)width;
    height = ((sint32)(sint16)(value >> 16) * 240);
    height = (sint32)height / 4096;
    rectangle.x = (sint16)((sint32)(320 - width) >> 1);
    rectangle.h = (sint16)height;
    rectangle.y = (sint16)((sint32)(240 - height) >> 1);
    result = ui_packets_draw_noise_rect(object, packet, inverse << 1, &rectangle);
    value = mission_flow.loading.animation_ticks;
    mission_flow.loading.animation_ticks = value + 1;
    return result;
}

uint32 frontend_draw_loading_stars(uint32 ordering, uint32 packet)
{
    uint32 *ordering_word;
    P_TAG *packet_link;
    LOADING_STAR_GPU_BANK *bank;
    uint32 bank_index = game_runtime.display_buffer, block_index = 0;
    uint32 value, random;
    LOADING_STAR_PUBLICATION *published = (LOADING_STAR_PUBLICATION *)getScratchAddr(0);
    uint32 *rotation = published->rotation;
    uint16 point_x, point_y, point_z;
    uint32 screen, depth, distance, current, count, index, next, end, tail, tag;
    FUNCTION_MARKER(0x80083FE0, "ACE3.BIN");
    if (bank_index >= 2 || !gpu_register_packet_range(loading_star_packets, sizeof(loading_star_packets)))
        abort();
    bank = &loading_star_packets[bank_index];
    uint32 a = xport_gte_read_control(0);
    uint32 b = xport_gte_read_control(1);
    uint32 c = xport_gte_read_control(2);
    uint32 d = xport_gte_read_control(3);
    rotation[4] = xport_gte_read_control(4);
    rotation[0] = (b & 0xFFFF0000) | (a & 0xFFFF);
    rotation[1] = (a & 0xFFFF0000) | (d & 0xFFFF);
    rotation[2] = (d & 0xFFFF0000) | (c & 0xFFFF);
    rotation[3] = (c & 0xFFFF0000) | (b & 0xFFFF);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, 0x1C00);
    xport_gte_execute(0x480012);
    LOADING_STAR_COMMANDS *commands = &bank->commands;
    commands->page.tag = gpu_packet_link((uintptr_t)&commands->restore) | 0x04000000;
    commands->page.code[0] = 0xE1000240;
    commands->fill_color.rgbc = 0x620F1F1F;
    commands->fill_position.xy = 0;
    commands->fill_dimensions = 0x00F00140;
    commands->restore.tag = gpu_packet_link((uintptr_t)&bank->blocks[0]) | 0x01000000;
    commands->restore.code[0] = 0xE1000220;
    published->saved_seed = game_random();
    random_seed = 765;
    published->base[0] = 0 - xport_gte_read_data(25);
    published->base[1] = 0 - xport_gte_read_data(26);
    published->base[2] = 0 - xport_gte_read_data(27);
    point_x = (uint16)((published->base[0] & 0x1FFF) - 4096);
    point_y = (uint16)((published->base[1] & 0x1FFF) - 4096);
    point_z = (uint16)((published->base[2] & 0x1FFF) - 4096);
    for (index = 0; index < 5; ++index)
        xport_gte_write_control(index, rotation[index]);
    xport_gte_write_control(5, 0);
    xport_gte_write_control(6, 0);
    xport_gte_write_control(7, 0x1C00);
    xport_gte_write_control(21, 0x360);
    xport_gte_write_control(22, 0x370);
    xport_gte_write_control(23, 0x380);
    xport_gte_write_data(6, 0x6AC0B0A0);
    count = 0;
    index = 0;
    current = packet + 28;
    do
    {
        xport_gte_write_data(0, point_x | ((uint32)point_y << 16));
        xport_gte_write_data(1, point_z);
        xport_gte_execute(0x180001);
        random = game_random();
        point_x = (uint16)(((published->base[0] + random) & 0x1FFF) - 4096);
        random = game_random();
        point_y = (uint16)(((published->base[1] + random) & 0x1FFF) - 4096);
        random = game_random();
        point_z = (uint16)(((published->base[2] + random) & 0x1FFF) - 4096);
        screen = xport_gte_read_data(14);
        depth = xport_gte_read_data(19);
        if ((uint32)((sint32)screen >> 16) < 241 && (uint32)((sint32)(screen << 16) >> 16) < 321)
        {
            distance = depth - 0x1C00;
            if ((sint32)distance < 0)
                distance = 0x1C00 - depth;
            if ((sint32)distance < 4096)
            {
                xport_gte_write_data(8, distance);
                xport_gte_execute(0x780010);
                POINT_C *point = &bank->blocks[block_index].points[count];
                count += 1;
                point->xy.xy = screen;
                if (count >= 7)
                {
                    next = current + 60;
                    uint32 *block_tag = &bank->blocks[block_index].tag;
                    *block_tag = gpu_packet_link((uintptr_t)&bank->blocks[block_index + 1]) | 0x0E000000;
                    current = next;
                    ++block_index;
                    count = 0;
                }
                value = xport_gte_read_data(22);
                point->rgbc.rgbc = value;
            }
        }
        index += 1;
    } while (index < 512);
    if (count != 0)
    {
        end = current + (count << 3) + 4;
        uint32 *block_tag = &bank->blocks[block_index].tag;
        *block_tag = count << 25;
        tail = current;
    }
    else
    {
        end = current;
        tail = packet + 20;
        if (tail + 8 != end)
            tail = end - 60;
    }
    ordering_word = (void *)ui_scene_ot_span(ordering, sizeof(*ordering_word));
    tag = *ordering_word;
    packet_link = tail == packet + 20 ? (P_TAG *)&commands->restore : (P_TAG *)&bank->blocks[(tail - packet - 28) / 60].tag;
    packet_link->addr = tag;
    ((P_TAG *)ordering_word)->addr = gpu_packet_link((uintptr_t)commands);
    published->point.words[0] = point_x | ((uint32)point_y << 16);
    published->point.vector.vz = (sint16)point_z;
    random_seed = published->saved_seed;
    return end;
}

uint32 frontend_draw_animated_loading_stars(uint32 ordering, uint32 packet)
{
    MATRIX matrix;
    uint16 angles[3];
    uint32 ticks, initial, first, second, phase, value;
    FUNCTION_MARKER(0x800364C4, "ACE3.BIN");
    ticks = game_runtime.field_counter;
    initial = (uint16)game_wave_at(((ticks << 2) & 0x7FFC));
    angles[0] = initial;
    angles[1] = initial;
    angles[2] = initial;
    rotation_build(&matrix, angles);
    SetRotMatrix(&matrix);
    first = (uint16)game_wave_at(((ticks << 4) & 0x7FF0));
    phase = ticks << 4;
    second = (uint16)game_wave_at(((phase & 0x3FFF) << 1));
    first = (sint32)(first << 16) >> 17;
    initial = (sint32)(initial << 16) >> 17;
    second = (sint32)(second << 16) >> 17;
    xport_gte_write_control(5, first);
    xport_gte_write_control(6, initial);
    xport_gte_write_control(7, second);
    value = (uint32)rcos((sint32)phase);
    value = (uint32)((sint32)value >> 6) + 300;
    xport_gte_write_control(26, value);
    SetGeomOffset(160, 120);
    return frontend_draw_loading_stars(ordering, packet);
}

static void frontend_initialize_loading_instruments(void)
{
    uint32 value;
    FUNCTION_MARKER(0x800369C8, "ACE3.BIN");
    value = game_runtime.display.flags;
    game_runtime.display.background[0] = (uint8)(0);
    game_runtime.display.flags = (uint32)(value | 8);
    game_runtime.display.background[1] = (uint8)(12);
    game_runtime.display.background[2] = (uint8)(0);
    mission_flow.loading.instruments.altitude = 0xF8000000;
    mission_flow.loading.instruments.speed = 0xA00000;
    mission_flow.loading.instruments.gauge_value = 199;
    mission_flow.loading.instruments.gauge_count = 4;
    mission_flow.loading.instruments.primary_color[0] = 0;
    mission_flow.loading.animation_ticks = 0;
    mission_flow.loading.instruments.flags = 0;
    mission_flow.loading.instruments.health = 0;
    mission_flow.loading.instruments.gauge_dim_count = 0;
    mission_flow.loading.instruments.timer_ticks = 0x2C3B2;
    mission_flow.loading.instruments.timer_mode = 0;
    mission_flow.loading.instruments.primary_color[1] = 255;
    mission_flow.loading.instruments.primary_color[2] = 0;
    mission_flow.loading.instruments.secondary_color[0] = 0;
    mission_flow.loading.instruments.secondary_color[1] = 0x83;
    mission_flow.loading.instruments.secondary_color[2] = 0;
    mission_flow.loading.instruments.number_color = 0;
}

void mission_loading_frame_begin(void)
{
    uint32 mode, value, arg0, arg1;
    FUNCTION_MARKER(0x80037070, "ACE3.BIN");
    mission_flow.loading.style_reserved = 0;
    mode = mission_flow.loading.scr_type;
    if (mode < 2)
    {
        frontend_initialize_loading_instruments();
        return;
    }
    if (mode != 2)
        return;
    arg0 = gpu_packet_link((uintptr_t)game_runtime.frame_packets.first_tag);
    arg1 = game_frame_packet_cursor();
    value = ui_copy_display_framebuffer(arg0, arg1);
    game_frame_set_packet_cursor(value);
}

void frontend_draw_loading_screen(uint32 mission)
{
    uint32 mode, value, arg0, arg1;
    FUNCTION_MARKER(0x80037108, "ACE3.BIN");
    mode = mission_flow.loading.scr_type;
    if (mode > 2)
        return;
    arg0 = gpu_packet_link((uintptr_t)game_runtime.frame_packets.middle_tag);
    arg1 = game_frame_packet_cursor();
    if (mode == 0)
        value = frontend_draw_loading_instruments(arg0, arg1, mission);
    else if (mode == 1)
        value = frontend_draw_loading_transition(arg0, arg1, mission);
    else
        value = frontend_draw_animated_loading_stars(arg0, arg1);
    game_frame_set_packet_cursor(value);
    {
        (void)(uint32)(value);
        return;
    }
}

uint32 frontend_draw_loading_instruments(uint32 object, uint32 dst, uint32 mission)
{
    uint32 mode, inverse, value, flags, other, scale, color, result;
    sint32 progress, angle, sine, phase;
    PSX_RECT rect;
    FUNCTION_MARKER(0x80036A84, "ACE3.BIN");
    mode = mission_flow.loading.resource_kind;
    progress = mode == 1 ? (sint16)mission_flow.loading.progress : (mode == 2) << 12;
    dst = frontend_draw_loading_labels(object, dst, mission);
    if (progress >= 0x1001)
        progress = 0x1000;
    inverse = 0x1000 - (uint32)progress;
    value = (uint32)progress * 255;
    mission_flow.loading.instruments.primary_color[1] = (sint32)value / 4096;
    value = (uint32)progress * 131;
    mission_flow.loading.instruments.secondary_color[1] = (sint32)value / 4096;
    if ((sint32)mission_flow.loading.animation_ticks < 0xA00)
        mission_flow.loading.instruments.flags = 0;
    if (progress >= 0x401)
    {
        flags = mission_flow.loading.instruments.flags;
        mission_flow.loading.instruments.flags = flags | 0x80;
    }
    if (progress >= 0x501)
    {
        flags = mission_flow.loading.instruments.flags;
        mission_flow.loading.instruments.speed = inverse * 2560;
        mission_flow.loading.instruments.flags = flags | 1;
    }
    if (progress >= 0x601)
    {
        flags = mission_flow.loading.instruments.flags;
        value = 0 - (inverse << 15);
        mission_flow.loading.instruments.altitude = value;
        mission_flow.loading.instruments.flags = flags | 2;
        if ((sint32)value > 0)
            mission_flow.loading.instruments.altitude = 0;
    }
    if (progress >= 0x701)
    {
        value = inverse * 181170;
        flags = mission_flow.loading.instruments.flags;
        mission_flow.loading.instruments.flags = flags | 4;
        mission_flow.loading.instruments.timer_ticks = (sint32)value / 4096;
    }
    if (progress >= 0x801)
    {
        flags = mission_flow.loading.instruments.flags;
        mission_flow.loading.instruments.flags = flags | 0x40;
    }
    if (progress >= 0x901)
    {
        flags = mission_flow.loading.instruments.flags;
        mission_flow.loading.instruments.health = progress;
        mission_flow.loading.instruments.flags = flags | 0x18;
        mission_flow.loading.instruments.gauge_value = (sint32)(inverse * 199) / 4096;
        mission_flow.loading.instruments.gauge_count = (sint32)inverse / 1024;
        other = mission_flow.loading.instruments.gauge_count;
        value = progress % 5;
        mission_flow.loading.instruments.gauge_dim_count = value;
        if ((sint32)other < (sint32)value)
            mission_flow.loading.instruments.gauge_dim_count = other;
    }
    if (game_runtime.options.hud_hidden == 0)
        dst = hud_instruments_draw(object, dst, &mission_flow.loading.instruments);
    angle = (sint32)((uint32)progress << 12) / 768;
    if (angle >= 0x1001)
        angle = 0x1000;
    sine = (sint16)(uint16)game_wave_at(((uint32)angle & 0x3FFF) * 2) >> 2;
    rect.w = (sint16)((sint32)((uint32)sine * 1280) / 4096);
    if (rect.w >= 321)
        rect.w = 320;
    rect.h = (sint16)((sint32)((uint32)sine * 240) / 4096);
    rect.y = (240 - rect.h) >> 1;
    rect.x = (320 - rect.w) >> 1;
    dst = ui_packets_draw_noise_rect(object, dst, inverse << 1, &rect);
    phase = ((uint32)(sint16)mission_flow.loading.ticks << 6);
    if (phase >= 0x1001)
        phase = 0x1000;
    scale = 0x1000 - (uint32)phase;
    color = ((scale * 131) >> 4) & 0xFF00;
    result = ui_draw_frame_border(object, dst, phase, color, (color >> 1) & 0xFF00);
    value = mission_flow.loading.animation_ticks;
    mission_flow.loading.animation_ticks = value + 1;
    return result;
}

uint32 frontend_draw_loading_labels(uint32 object, uint32 dst, uint32 mission)
{
    const char *title;
    uint32 mode, length, color;
    sint32 x, value;
    FUNCTION_MARKER(0x80036824, "ACE3.BIN");
    if ((sint16)mission_flow.loading.ticks < 40)
    {
        value = game_runtime.selection.difficulty & 3;
        dst = ui_draw_compact_label(object, dst, 190, 18, 0x4000, (const uint8 *)mission_loading_difficulties[value]);
    }
    mode = mission_flow.loading.resource_kind;
    if ((mode == 1 && (sint16)mission_flow.loading.progress > 0) || mode == 2)
    {
        title = mission_loading_titles[mission];
        length = (uint32)strlen(title);
        x = (sint32)(320 - (length << 3)) >> 1;
        mode = mission_flow.loading.resource_kind;
        value = (sint16)mission_flow.loading.progress;
        if (mode != 1)
            value = (0x1000 - (uint32)value);
        color = ((uint32)(value / 32) & 255) << 8;
        dst = ui_draw_compact_label(object, dst, x, 118, color, (const uint8 *)title);
        if (game_runtime.field_counter & 0x10)
            dst = ui_draw_compact_label(object, dst, (uint32)x - 12, 118, color, (const uint8 *)"_");
    }
    return dst;
}
