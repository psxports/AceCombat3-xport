#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "game_functions.h"
#include "psx_gte.h"

static const PSX_RECT resource002_detail_clip = {243, 100, 64, 128};

static const uint8 resource002_map_icon_colors[11][3] = {
    {56, 96, 200}, {56, 96, 200}, {255, 255, 255}, {255, 255, 255}, {232, 208, 104}, {232, 208, 104}, {240, 80, 104}, {240, 80, 104}, {144, 208, 208}, {0, 0, 0}, /* icon 9 is skipped before packet allocation */
    {56, 96, 200},
};

static const PSX_RECT resource002_grid_clip = {84, 0, 150, 256};

static const uint32 resource002_terrain_colors[16] = {
    0x804040, 0x606040, 0x608040, 0x408040, 0x408060, 0x408080, 0x406080, 0x404080, 0x404060, 0x849494, 0x90A0A0, 0xACB4B4, 0xB4C4C4, 0xC8D2D2, 0xD2DCDC, 0xE6F0F0,
};

typedef union
{
    SVECTOR vector;
    uint32 words[2];

    struct
    {
        uint16 x, y, z, retained;
    };
} MISSION_MAP_GRID_SCRATCH_CORNER;

typedef struct
{
    uint16 column, row;
    uint16 column_fraction, row_fraction;
    uint16 height[4];
    uint32 color[4];
    MISSION_MAP_GRID_SCRATCH_CORNER corner[4];
} MISSION_MAP_GRID_SCRATCH;

typedef char briefing_grid_scratch_layout_check[sizeof(MISSION_MAP_GRID_SCRATCH) == 64 && offsetof(MISSION_MAP_GRID_SCRATCH, row) == 2 && offsetof(MISSION_MAP_GRID_SCRATCH, column_fraction) == 4 && offsetof(MISSION_MAP_GRID_SCRATCH, row_fraction) == 6 && offsetof(MISSION_MAP_GRID_SCRATCH, height) == 8 && offsetof(MISSION_MAP_GRID_SCRATCH, color) == 16 && offsetof(MISSION_MAP_GRID_SCRATCH, corner) == 32 && sizeof(MISSION_MAP_GRID_SCRATCH_CORNER) == 8 && offsetof(MISSION_MAP_GRID_SCRATCH_CORNER, x) == 0 && offsetof(MISSION_MAP_GRID_SCRATCH_CORNER, y) == 2 && offsetof(MISSION_MAP_GRID_SCRATCH_CORNER, z) == 4 && offsetof(MISSION_MAP_GRID_SCRATCH_CORNER, retained) == 6 && sizeof(SVECTOR) == 8 && offsetof(SVECTOR, vy) == 2 && offsetof(SVECTOR, vz) == 4 && offsetof(SVECTOR, pad) == 6 ? 1 : -1];

typedef char briefing_grid_packet_layout_check[sizeof(DR_AREA) == 12 && sizeof(POLY_G4) == 36 && offsetof(POLY_G4, p) == 4 && sizeof(POINT_C) == 8 && offsetof(POINT_C, xy) == 4 && sizeof(TILE) == 16 && offsetof(TILE, rgbc) == 4 && offsetof(TILE, p) == 8 && offsetof(TILE, w) == 12 && offsetof(TILE, h) == 14 && sizeof(POLY_FT4) == 40 && offsetof(POLY_FT4, rgbc) == 4 && offsetof(POLY_FT4, p) == 8 && sizeof(POINT_T) == 8 && offsetof(POINT_T, uv) == 4 && offsetof(POINT_T, attr) == 6 ? 1 : -1];

typedef struct
{
    SPRT left[2];
    DR_MODE commands[3];
    SPRT right[2];
} BRIEFING_GPU_BANK;

static BRIEFING_GPU_BANK briefing_gpu_banks[2];

MISSION_BRIEFING_STATE mission_briefing;
MISSION_MAP_CAMERA mission_briefing_camera;
MISSION_HEIGHT_MAP mission_briefing_height_map;

uint32 mission_briefing_restore_arena(uint32 owner)
{
    FUNCTION_MARKER(0x80105FE0, "ACE3.BIN");
    return arena_set_cursor(owner);
}

uint32 mission_briefing_leave(void)
{
    FUNCTION_MARKER(0x80105FD8, "ACE3.BIN");
    return mission_briefing_restore_arena(mission_briefing.arena_bookmark);
}

uint32 mission_briefing_draw_marker(uint32 packet)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint8 arguments[8];
    uint32 value, screen, quotient, remainder;
    FUNCTION_MARKER(0x80105C20, "ACE3.BIN");
    if (mission_briefing.marker_time >= 0)
    {
        SetRotMatrix(&mission_briefing_camera.view);
        SetTransMatrix(&mission_briefing_camera.view);
        gte_ldv0(&mission_briefing.marker_position);
        xport_gte_execute(0x180001);
        screen = xport_gte_read_data(14);
        (void)xport_gte_read_data(8);
        (void)xport_gte_read_control(31);
        (void)xport_gte_read_data(19);
        text_set_pos((uint32)(sint32)(sint16)screen - 10, (uint32)((sint32)screen >> 16));
        text_set_color(128, 128, 128);
        value = (uint32)mission_briefing.marker_time;
        quotient = ((sint32)value / 60);
        remainder = value - quotient * 60;
        (wire_word_0 = (quotient), (void)memcpy(arguments, &wire_word_0, sizeof(wire_word_0)));
        (wire_word_1 = (remainder), (void)memcpy(arguments + 4, &wire_word_1, sizeof(wire_word_1)));
        text_print((const uint8 *)"%2d:%2d", arguments, sizeof(arguments));
    }
    return packet;
}

void mission_briefing_allocate(void)
{
    uint32 bookmark, bank;
    FUNCTION_MARKER(0x801020C0, "ACE3.BIN");
    bookmark = arena_alloc(2684);
    mission_briefing.marker_time = -1;
    mission_briefing.arena_bookmark = bookmark;
    if (!gpu_register_packet_range(briefing_gpu_banks, sizeof(briefing_gpu_banks)))
        abort();
    for (bank = 0; bank < 2; ++bank)
    {
        mission_briefing.packet_banks[bank].left_address = gpu_packet_link((uintptr_t)briefing_gpu_banks[bank].left);
        mission_briefing.packet_banks[bank].left = briefing_gpu_banks[bank].left;
        mission_briefing.packet_banks[bank].commands_address = gpu_packet_link((uintptr_t)briefing_gpu_banks[bank].commands);
        mission_briefing.packet_banks[bank].commands = briefing_gpu_banks[bank].commands;
        mission_briefing.packet_banks[bank].right_address = gpu_packet_link((uintptr_t)briefing_gpu_banks[bank].right);
        mission_briefing.packet_banks[bank].right = briefing_gpu_banks[bank].right;
    }
    {
        (void)(uint32)(bookmark);
        return;
    }
}

void mission_briefing_reset_camera_motion(void)
{
    MISSION_MAP_CAMERA *camera = &mission_briefing_camera;
    FUNCTION_MARKER(0x801020F4, "ACE3.BIN");
    camera->speed = (uint32)(0);
    camera->move_ticks = (uint32)(0);
    camera->heading = (sint16)(0);
}

void mission_briefing_reset_objects(void)
{
    MISSION_BRIEFING_OBJECT *object;
    uint32 index;
    FUNCTION_MARKER(0x80105F04, "ACE3.BIN");
    for (index = 0; index < MISSION_BRIEFING_OBJECT_COUNT; ++index)
    {
        arena_alloc(32);
        object = &mission_briefing.objects[index];
        object->ticks = (uint16)(0);
        object->duration = (uint16)(0);
        object->motion = (uint16)(0xFFFF);
        object->screen_x = (uint16)(0xFFFF);
        object->screen_y = (uint16)(0xFFFF);
    }
    return;
}

void mission_briefing_reset_panels(void)
{
    uint32 index;
    FUNCTION_MARKER(0x80105F80, "ACE3.BIN");
    mission_briefing.panel_flags = (uint16)(0);
    for (index = 0; index < 8; ++index)
    {
        arena_alloc(4);
    }
    return;
}

void mission_briefing_reset_panel_frame(void)
{
    FUNCTION_MARKER(0x80105F78, "ACE3.BIN");
    {
        (void)(mission_briefing_reset_panels());
        return;
    }
}

void mission_briefing_initialize_noop(void)
{
    FUNCTION_MARKER(0x80102564, "ACE3.BIN");
}

uint32 mission_briefing_begin(void)
{
    const MISSION_BUNDLE_DIRECTORY *directory = &mission_flow.assets.mission_directory;
    const MISSION_BRIEFING_EVENT *event;
    uint32 mission, value, type, argument;
    FUNCTION_MARKER(0x801067B0, "ACE3.BIN");
    mission = (sint16)game_runtime.selection.mission;
    if (audio_mission_track_id(mission) == -1)
    {
        value = mission_flow.mode;
        mission_flow.mode = value + 1;
        return 0;
    }
    mission_briefing_allocate();
    mission_briefing_reset_objects();
    mission_briefing_reset_panel_frame();
    mission_briefing_reset_camera_motion();
    mission_audio_bind_tracks(&audio_tracks_mission);
    mission = (sint16)game_runtime.selection.mission;
    if (audio_mission_uses_speech(mission))
    {
        subtitles_configure(2, 0, 0);
        (void)directory->briefing_speech;
        mission_image_optional_data_hook();
    }
    mission = (sint16)game_runtime.selection.mission;
    if (!audio_mission_uses_speech(mission))
        subtitles_configure(2, 0, 0);
    scene_texture_load_mission_images(directory->briefing_textures, directory);
    event = mission_flow.assets.mission_events;
    if (event == mission_flow.assets.mission_events_end)
        abort();
    mission_briefing.event_cursor = event;
    value = (uint32)event->time;
    while (value == 0xFFFFFFFF)
    {
        event = mission_briefing.event_cursor;
        type = event->type;
        argument = (uint32)event->time;
        mission_dispatch_event(type, argument);
        event = mission_briefing.event_cursor;
        if (event + 1 == mission_flow.assets.mission_events_end)
            abort();
        mission_briefing.event_cursor = event + 1;
        value = (uint32)event[1].time;
    }
    mission_briefing.panel_count = (uint8)(0);
    mission_briefing_initialize_noop();
    value = mission_flow.mode_phase;
    mission_flow.mode_phase = value + 1;
    mission_briefing.group = (uint8)(0);
    mission_briefing.phase = (uint8)(0);
    mission_briefing.exit_mode = (uint8)(0);
    ui_animation_set(2, 0, 0, 0);
    ui_animation_set(3, 0, 0, 0);
    ui_animation_set(4, 0, 0, 0);
    text_select_font(7);
    text_set_blend(0xFFFFFFFF);
    text_set_reveal_range(1, 1, 1);
    text_set_reveal_step(1);
    return menu_voices_restart(256);
}

uint32 mission_briefing_schedule_camera_move(void)
{
    MISSION_MAP_CAMERA *camera = &mission_briefing_camera;
    const MISSION_BRIEFING_EVENT *first_event, *second_event;
    uint32 value, x, z, old_x, old_z, dx, dz, root;
    sint32 divisor, quotient;

    FUNCTION_MARKER(0x80102E78, "ACE3.BIN");
    first_event = mission_briefing.event_cursor;
    second_event = mission_briefing.event_cursor;
    value = (uint32)first_event->parameter;
    camera->move_ticks = (uint32)(value);
    x = (uint32)second_event->x;
    old_x = ((uint32)camera->position.vx);
    dx = ((x - 128) << 7) - old_x;
    z = (uint32)second_event->z;
    old_z = ((uint32)camera->position.vz);
    dz = ((z - 128) << 7) - old_z;
    root = SquareRoot0((sint32)(dx * dx + dz * dz));
    divisor = ((sint32)camera->move_ticks);
    if (!divisor)
    {
#ifndef NDEBUG
        fprintf(stderr, "Resource2 event 80102E78 BREAK7\n");
#endif
        abort();
    }
    if (divisor == -1 && root == 0x80000000)
    {
#ifndef NDEBUG
        fprintf(stderr, "Resource2 event 80102E78 BREAK6\n");
#endif
        abort();
    }
    quotient = (sint32)root / divisor;
    camera->speed = (uint32)((uint32)quotient);
    value = (uint32)ratan2((sint32)dx, (sint32)dz);
    camera->heading = (sint16)(value);
    return value;
}

uint32 mission_briefing_set_camera_position(void)
{
    MISSION_MAP_CAMERA *camera = &mission_briefing_camera;
    const MISSION_BRIEFING_EVENT *first_event, *second_event;
    uint32 value;

    FUNCTION_MARKER(0x80102F5C, "ACE3.BIN");
    first_event = mission_briefing.event_cursor;
    second_event = mission_briefing.event_cursor;
    value = (uint32)first_event->x;
    camera->position.vy = (sint32)(0);
    camera->position.vx = (sint32)((value - 128) << 7);
    value = (uint32)second_event->z;
    value = (value - 128) << 7;
    camera->position.vz = (sint32)(value);
    return value;
}

uint32 mission_briefing_set_marker(void)
{
    const MISSION_BRIEFING_EVENT *first_event, *second_event;
    uint32 value;

    FUNCTION_MARKER(0x80102F9C, "ACE3.BIN");
    first_event = mission_briefing.event_cursor;
    value = (uint32)first_event->x;
    second_event = mission_briefing.event_cursor;
    mission_briefing.marker_position.vx = (sint16)((value - 128) << 7);
    value = (uint16)second_event->y;
    mission_briefing.marker_position.vy = (sint16)(value);
    value = (uint32)second_event->z;
    mission_briefing.marker_position.vz = (sint16)((value - 128) << 7);
    value = (uint32)second_event->parameter;
    mission_briefing.marker_time = (sint32)(value);
    return value;
}

void mission_briefing_event_noop(void)
{
    FUNCTION_MARKER(0x80105E18, "ACE3.BIN");
}

uint32 mission_briefing_update_events(void)
{
    const MISSION_BRIEFING_EVENT *event;
    uint32 frame, type, result;
    sint32 event_time;

    FUNCTION_MARKER(0x80105E20, "ACE3.BIN");
    event = mission_briefing.event_cursor;
    event_time = event->time;
    frame = mission_flow.phase_timer;
    if ((uint32)event_time == frame)
    {
        do
        {
            event = mission_briefing.event_cursor;
            type = event->type;
            event_time = event->time;
            mission_dispatch_event(type, (uint32)event_time);
            event = mission_briefing.event_cursor;
            if (event + 1 == mission_flow.assets.mission_events_end)
                abort();
            mission_briefing.event_cursor = event + 1;
            event_time = event[1].time;
            frame = mission_flow.phase_timer;
        } while ((uint32)event_time == frame);
    }
    mission_briefing_move_camera();
    mission_briefing_move_objects_frame();
    result = mission_flow.phase_timer + 1;
    mission_flow.phase_timer = (uint32)(result);
    return result;
}

void mission_briefing_move_camera(void)
{
    MISSION_MAP_CAMERA *camera = &mission_briefing_camera;
    uint32 product, speed, first_delta, second_delta, x, z, remaining;
    sint32 angle, component;

    FUNCTION_MARKER(0x80105D54, "ACE3.BIN");
    if (((sint32)camera->move_ticks) <= 0)
        return;
    angle = ((sint16)camera->heading);
    component = rsin(angle);
    speed = ((uint32)camera->speed);
    product = speed * (uint32)component;
    first_delta = ((sint32)product / 4096);
    angle = ((sint16)camera->heading);
    component = rcos(angle);
    speed = ((uint32)camera->speed);
    product = speed * (uint32)component;
    second_delta = ((sint32)product / 4096);
    x = ((uint32)camera->position.vx);
    remaining = ((uint32)camera->move_ticks);
    camera->position.vx = (sint32)(x + first_delta);
    z = ((uint32)camera->position.vz);
    camera->move_ticks = (uint32)(remaining - 1);
    camera->position.vz = (sint32)(z + second_delta);
    return;
}

void mission_briefing_move_objects(void)
{
    MISSION_BRIEFING_OBJECT *object;
    uint32 index, product, first_delta, second_delta, x, z, remaining;
    sint32 angle, component, speed;

    FUNCTION_MARKER(0x80102FF0, "ACE3.BIN");
    for (index = 0; index < MISSION_BRIEFING_OBJECT_COUNT; ++index)
    {
        object = &mission_briefing.objects[index];
        if ((sint16)object->motion != 2 || (sint16)object->ticks <= 0)
            continue;
        angle = (sint16)object->angles.vy;
        component = rsin(angle);
        speed = (sint16)object->speed;
        product = (uint32)speed * (uint32)component;
        first_delta = ((sint32)product / 4096);
        angle = (sint16)object->angles.vy;
        component = rcos(angle);
        speed = (sint16)object->speed;
        product = (uint32)speed * (uint32)component;
        second_delta = ((sint32)product / 4096);
        x = object->x;
        remaining = object->ticks;
        object->x = (uint16)(x + first_delta);
        z = object->z;
        object->ticks = (uint16)(remaining - 1);
        object->z = (uint16)(z + second_delta);
    }
    return;
}

void mission_briefing_move_objects_frame(void)
{
    FUNCTION_MARKER(0x801030E0, "ACE3.BIN");
    {
        (void)(mission_briefing_move_objects());
        return;
    }
}

static void mission_briefing_build_projection(void)
{
    MISSION_MAP_CAMERA *camera = &mission_briefing_camera;
    MATRIX matrix = {0}, rotation = {0};
    VECTOR vector, transformed;
    SVECTOR orbit_angles = {0}, view_angles = {0};
    uint32 frame, delta, scaled, value, factor, x, y;
    uint32 distance, dx, dy, dz, squared, root, angle, a, b;
    uint32 eye_x, eye_y, eye_z;
    sint32 counter;

    FUNCTION_MARKER(0x80102184, "ACE3.BIN");
    x = (sint16)ui_item_at(0, 5)->x;
    y = (sint16)ui_item_at(0, 5)->y;
    SetGeomOffset((sint32)(x + 154), (sint32)(y + 154));
    if (!mission_briefing.group)
    {
        frame = mission_flow.phase_timer;
        counter = frame;
        if (counter >= 82 && counter < 114)
        {
            delta = frame - 82;
            orbit_angles.vx = (sint16)(1024 - (delta << 4));
            frame = mission_flow.phase_timer;
            orbit_angles.vz = (sint16)(0);
            orbit_angles.vy = (sint16)((frame << 3) + 1392);
            delta = mission_flow.phase_timer - 82;
            value = (delta << 3) + delta;
            scaled = value << 6;
            value += scaled;
            value <<= 2;
            value -= delta;
            scaled = ((value << 2) - value) << 2;
            x = ((uint32)camera->position.vx);
            distance = 0x8000 - (uint32)((sint32)scaled / 32);
            frame = mission_flow.phase_timer;
            factor = 114 - frame;
            scaled = (uint32)((sint32)x / 256) * factor;
            x = ((sint32)scaled / 32);
            value = ((uint32)camera->position.vz);
            a = (sint16)ui_item_at(0, 5)->x;
            b = (sint16)ui_item_at(0, 5)->y;
            x += a + 154;
            scaled = (uint32)((sint32)value / 256) * factor;
            y = b + 154 - ((uint32)((sint32)scaled / 32) << 1);
            goto geometry_ready;
        }
    }
    orbit_angles.vx = (sint16)(512);
    orbit_angles.vy = (sint16)(2304);
    orbit_angles.vz = (sint16)(0);
    distance = 4700;
    x = (uint32)(sint16)ui_item_at(0, 5)->x + 154;
    y = (uint32)(sint16)ui_item_at(0, 5)->y + 154;
geometry_ready:
    SetGeomOffset((sint32)x, (sint32)y);
    RotMatrixYXZ(&orbit_angles, &matrix);
    TransMatrix(&matrix, &camera->position);
    SetRotMatrix(&matrix);
    SetTransMatrix(&matrix);
    xport_gte_write_data(0, 0);
    xport_gte_write_data(1, (uint16)distance);
    xport_gte_execute(0x480012);
    eye_x = xport_gte_read_data(25);
    eye_y = xport_gte_read_data(26);
    eye_z = xport_gte_read_data(27);
    (void)xport_gte_read_control(31);
    a = ((uint32)camera->position.vx);
    b = eye_x;
    dx = a - b;
    squared = dx * dx;
    a = ((uint32)camera->position.vz);
    b = eye_z;
    dz = a - b;
    a = ((uint32)camera->position.vy);
    b = eye_y;
    dy = a - b;
    squared += dz * dz;
    root = SquareRoot0((sint32)squared);
    angle = (uint32)ratan2((sint32)dy, (sint32)root);
    view_angles.vx = (sint16)(0 - angle);
    angle = (uint32)ratan2((sint32)dx, (sint32)dz);
    view_angles.vy = (sint16)angle;
    view_angles.vz = 0;
    RotMatrixYXZ(&view_angles, &rotation);
    vector.vx = (sint32)(0 - eye_x);
    vector.vy = (sint32)(0 - eye_y);
    vector.vz = (sint32)(0 - eye_z);
    TransposeMatrix(&rotation, &camera->view);
    matrix_halve_xz(&camera->view);
    ApplyMatrixLV(&camera->view, &vector, &transformed);
    TransMatrix(&camera->view, &transformed);
}

uint32 mission_briefing_intro_begin(void)
{
    uint32 index;

    FUNCTION_MARKER(0x80106000, "ACE3.BIN");
    mission_briefing.phase = (uint8)(1);
    mission_flow.phase_timer = (uint32)(0);
    ui_animation_set(1, 1, 0, 120);
    for (index = 2; index < 5; ++index)
        ui_widget_set(index, 0);
    return 0;
}

uint32 mission_briefing_intro_tick(void)
{
    uint32 value, result;
    sint32 counter;

    FUNCTION_MARKER(0x8010606C, "ACE3.BIN");
    mission_briefing_build_projection();
    counter = (sint32)mission_flow.phase_timer;
    mission_briefing.intro_stage = (uint8)(counter < 40 ? 1 : (counter < 270 ? 2 : 0));
    counter = (sint32)mission_flow.phase_timer;
    if (counter < 30 || counter >= 55)
        value = 0;
    else if (counter < 35)
        value = 1;
    else if (counter < 45)
        value = 2;
    else if (counter < 50)
        value = 3;
    else
        value = 4;
    mission_briefing.intro_widget = (uint8)(value);
    ui_widget_set(1, mission_briefing.intro_widget);
    counter = (sint32)mission_flow.phase_timer;
    if (counter >= 121)
    {
        value = mission_briefing.group + 1;
        mission_briefing.group = (uint8)(value);
        mission_briefing.phase = (uint8)(0);
        counter = (sint32)mission_flow.phase_timer;
    }
    if (counter < 121)
    {
        if ((game_input.pressed & 0x10) && !(game_input.held & 0x2F))
        {
            value = mission_briefing.group + 1;
            mission_briefing.group = (uint8)(value);
            mission_briefing.phase = (uint8)(0);
            ui_item_reset_motion(0, 5);
            ui_item_reset_motion(0, 7);
            ui_animation_set(1, 2, 120, 120);
            ui_widget_set(1, 0);
            ui_widget_set(2, 0);
            mission_flow.ui_timer = (uint32)(400);
        }
    }
    result = mission_flow.phase_timer + 1;
    mission_flow.phase_timer = (uint32)(result);
    return result;
}

uint32 mission_briefing_prepare_playback(void)
{
    uint32 value, index, bank;
    POLY_FT4 *quad;
    SPRT *sprite;
    DR_MODE *modes;

    FUNCTION_MARKER(0x801062B8, "ACE3.BIN");
    mission_briefing.visible_objects = 0;
    mission_flow.phase_timer = (uint32)(0);
    value = mission_briefing.phase + 1;
    mission_briefing.phase = (uint8)(value);
    mission_briefing.intro_widget = (uint8)(0);
    mission_audio_control_track(1, (uint32)(sint16)game_runtime.selection.mission);
    for (bank = 0; bank < 2; ++bank)
    {
        for (index = 0; index < 32; ++index)
        {
            quad = &ui_packets.scene_bank[bank].prims->objects[index];
            quad->tag = (quad->tag & 0xFFFFFF) | ((uint32)(9) << 24);
            quad->rgbc.code = (uint8)(44);
            quad->p[1].attr = (uint16)(15);
            quad->rgbc.r = (uint8)(128);
            quad->rgbc.g = (uint8)(128);
            quad->rgbc.b = (uint8)(128);
        }
    }
    for (bank = 0; bank < 2; ++bank)
    {
        sprite = mission_briefing.packet_banks[bank].left;
        modes = mission_briefing.packet_banks[bank].commands;
        SetDrawMode(modes, 0, 0, 0, NULL);
        SetDrawMode(modes + 1, 0, 0, 76, NULL);
        SetDrawMode(modes + 2, 0, 0, 11, NULL);
        sprite->tag = (sprite->tag & 0xFFFFFF) | ((uint32)(4) << 24);
        sprite->rgbc.code = (uint8)(102);
        sprite->rgbc.r = (uint8)(128);
        sprite->rgbc.g = (uint8)(128);
        sprite->rgbc.b = (uint8)(128);
        sprite->w = (sint16)(uint16)(60);
        sprite->h = (sint16)(uint16)(69);
        sprite += 1;
        sprite->tag = (sprite->tag & 0xFFFFFF) | ((uint32)(4) << 24);
        sprite->rgbc.code = (uint8)(102);
        sprite->rgbc.r = (uint8)(128);
        sprite->rgbc.g = (uint8)(128);
        sprite->rgbc.b = (uint8)(128);
        sprite->w = (sint16)(uint16)(60);
        sprite->h = (sint16)(uint16)(69);
    }
    for (bank = 0; bank < 2; ++bank)
    {
        sprite = mission_briefing.packet_banks[bank].right;
        sprite->tag = (sprite->tag & 0xFFFFFF) | ((uint32)(4) << 24);
        sprite->rgbc.code = (uint8)(100);
        sprite->rgbc.r = (uint8)(128);
        sprite->rgbc.g = (uint8)(128);
        sprite->rgbc.b = (uint8)(128);
        sprite->w = (sint16)(uint16)(56);
        sprite->h = (sint16)(uint16)(12);
        sprite += 1;
        sprite->tag = (sprite->tag & 0xFFFFFF) | ((uint32)(4) << 24);
        sprite->rgbc.code = (uint8)(100);
        sprite->rgbc.r = (uint8)(128);
        sprite->rgbc.g = (uint8)(128);
        sprite->rgbc.b = (uint8)(128);
        sprite->w = (sint16)(uint16)(56);
        sprite->h = (sint16)(uint16)(12);
    }
    mission_briefing_build_projection();
    mission_briefing_draw_objects_frame();
    return mission_briefing_draw_panels();
}

uint32 mission_briefing_start_playback_frame(void)
{
    FUNCTION_MARKER(0x8010655C, "ACE3.BIN");
    return mission_briefing_start_playback();
}

uint32 mission_briefing_start_playback(void)
{
    uint32 result;

    FUNCTION_MARKER(0x80106564, "ACE3.BIN");
    mission_audio_control_track(1, (uint32)(sint16)game_runtime.selection.mission);
    mission_briefing_build_projection();
    mission_briefing_draw_objects_frame();
    mission_briefing_draw_panels();
    mission_flow.audio_wait_ticks = (uint32)(180);
    result = mission_briefing.phase + 1;
    mission_briefing.phase = (uint8)(result);
    return result;
}

uint32 mission_briefing_playback_tick(void)
{
    uint32 packet, ordering, value, result;
    sint32 timer;

    FUNCTION_MARKER(0x801065C8, "ACE3.BIN");
    mission_briefing_build_projection();
    mission_briefing_update_events();
    mission_briefing_draw_objects_frame();
    mission_briefing_draw_panels();
    if (!resource_get_stream_elapsed())
    {
        timer = (sint32)mission_flow.audio_wait_ticks;
        if (timer >= 0)
        {
            mission_flow.audio_wait_ticks = (uint32)((uint32)timer - 1);
            return 0;
        }
    }
    ordering = ui_packets.scene_ot;
    packet = ui_packets.scene_packet_cursor;
    value = subtitles_draw(ordering, packet);
    ui_packets.scene_packet_cursor = (uint32)(value);
    value = mission_audio_current_track_id();
    value = resource_get_stream_time(value);
    if (value != 0xFFFFFFFF)
    {
        result = game_input.pressed & 0x10;
        if (!result)
            return result;
        result = game_input.held & 0x2F;
        if (result)
            return result;
    }
    value = mission_briefing.group + 1;
    mission_briefing.group = (uint8)(value);
    mission_briefing.phase = (uint8)(0);
    mission_briefing.exit_mode = (uint8)(1);
    ui_item_reverse_motion(0, 5);
    ui_item_reverse_motion(0, 7);
    ui_animation_set(1, 3, -1, -1);
    sound_play(91);
    result = game_input.pressed & 0x10;
    if (result)
    {
        result = game_input.held & 0x2F;
        if (!result)
            return sound_play(60);
    }
    return result;
}

uint32 mission_briefing_finish_frame(void)
{
    FUNCTION_MARKER(0x8010673C, "ACE3.BIN");
    return mission_briefing_finish_begin();
}

uint32 mission_briefing_finish_begin(void)
{
    uint32 value;

    FUNCTION_MARKER(0x80106744, "ACE3.BIN");
    value = mission_briefing.phase + 1;
    mission_briefing.phase = (uint8)(value);
    mission_briefing_build_projection();
    mission_briefing_draw_objects_frame();
    return mission_briefing_draw_panels();
}

uint32 mission_briefing_finish_tick(void)
{
    FUNCTION_MARKER(0x80106780, "ACE3.BIN");
    mission_briefing_build_projection();
    mission_briefing_draw_objects_frame();
    return mission_briefing_draw_panels();
}

void mission_briefing_draw_objects_frame(void)
{
    FUNCTION_MARKER(0x80103834, "ACE3.BIN");
    {
        (void)(mission_briefing_draw_objects_step());
        return;
    }
}

uint32 mission_briefing_spawn_object(uint32 event_time)
{
    MISSION_BRIEFING_OBJECT *object;
    static const sint16 uv_bias[8] = {32, -64, 47, -64, 32, -49, 47, -49};
    const MISSION_BRIEFING_EVENT *event;
    uint32 slot, flags, type, value, bank, index;
    POLY_FT4 *quad;
    sint32 signed_value, coordinate;

    FUNCTION_MARKER(0x80102658, "ACE3.BIN");
    event = mission_briefing.event_cursor;
    slot = event->slot;
    if (slot >= MISSION_BRIEFING_OBJECT_COUNT)
        abort();
    object = &mission_briefing.objects[slot];
    flags = mission_briefing.visible_objects;
    event = mission_briefing.event_cursor;
    mission_briefing.visible_objects = flags | ((uint32)1 << (slot & 31));
    type = event->type;
    value = event_time + 15;
    object->ticks = (uint16)(value);
    object->duration = (uint16)(value);
    object->motion = (uint16)(type);
    event = mission_briefing.event_cursor;
    value = (uint16)event->parameter;
    object->icon = (uint16)(value);
    event = mission_briefing.event_cursor;
    value = (uint32)event->x;
    object->x = (uint16)((value - 128) << 7);
    event = mission_briefing.event_cursor;
    signed_value = -event->y;
    object->y = (uint16)((uint32)(signed_value / 10));
    event = mission_briefing.event_cursor;
    value = (uint32)event->z;
    object->angles.vx = (sint16)(0);
    object->z = (uint16)((value - 128) << 7);
    event = mission_briefing.event_cursor;
    value = (uint32)event->angle_or_rows << 12;
    object->angles.vz = (sint16)(0);
    object->angles.vy = (sint16)((uint32)((sint32)value / 360));
    for (bank = 0; bank < 2; ++bank)
    {
        event = mission_briefing.event_cursor;
        slot = event->slot;
        ui_packets.scene_bank[bank].prims->objects[slot].p[0].attr = 0x303F;
        for (index = 0; index < 8; ++index)
        {
            value = object->icon;
            signed_value = (sint16)value;
            event = mission_briefing.event_cursor;
            slot = event->slot;
            if (index & 1)
                coordinate = signed_value / 10;
            else
                coordinate = signed_value % 10;
            coordinate = coordinate * 16 + uv_bias[index];
            quad = &ui_packets.scene_bank[bank].prims->objects[slot];
            if (index & 1)
                quad->p[index >> 1].uv.v = (uint8)coordinate;
            else
                quad->p[index >> 1].uv.u = (uint8)coordinate;
        }
    }
    return 0;
}

void mission_briefing_object_event_noop(void)
{
    FUNCTION_MARKER(0x80102650, "ACE3.BIN");
}

uint32 mission_briefing_schedule_object_move(void)
{
    MISSION_BRIEFING_OBJECT *object;
    const MISSION_BRIEFING_EVENT *event;
    uint32 slot, value, old_x, old_z, x, z, dx, dz, angle, root;
    sint32 divisor, result, short_dx, short_dz;

    FUNCTION_MARKER(0x80102AA8, "ACE3.BIN");
    event = mission_briefing.event_cursor;
    slot = event->slot;
    if (slot >= MISSION_BRIEFING_OBJECT_COUNT)
        abort();
    object = &mission_briefing.objects[slot];
    value = event->type;
    object->motion = (uint16)(value);
    event = mission_briefing.event_cursor;
    value = (uint16)event->parameter;
    object->ticks = (uint16)(value);
    event = mission_briefing.event_cursor;
    old_x = object->x;
    value = (uint16)event->parameter;
    object->duration = (uint16)(value);
    event = mission_briefing.event_cursor;
    x = (uint32)event->x - 128;
    z = (uint32)event->z - 128;
    x <<= 7;
    z <<= 7;
    dx = x - old_x;
    old_z = object->z;
    dz = z - old_z;
    angle = (uint32)ratan2((sint32)(sint16)(uint16)x - (sint32)(sint16)old_x, (sint32)(sint16)(uint16)z - (sint32)(sint16)old_z);
    short_dx = (sint16)(uint16)dx;
    short_dz = (sint16)(uint16)dz;
    object->angles.vy = (sint16)(angle);
    root = SquareRoot0((sint32)((uint32)(short_dx * short_dx) + (uint32)(short_dz * short_dz)));
    event = mission_briefing.event_cursor;
    divisor = event->parameter;
    if (!divisor)
    {
#ifndef NDEBUG
        fprintf(stderr, "Resource2 event 80102AA8 BREAK7\n");
#endif
        abort();
    }
    if (divisor == -1 && root == 0x80000000)
    {
#ifndef NDEBUG
        fprintf(stderr, "Resource2 event 80102AA8 BREAK6\n");
#endif
        abort();
    }
    result = (sint32)root / divisor;
    object->speed = (uint16)((uint32)result);
    return (uint32)result;
}

uint32 mission_briefing_add_panel(void)
{
    const MISSION_BRIEFING_EVENT *event;
    uint32 slot, shifted, flags, value;

    FUNCTION_MARKER(0x80102BFC, "ACE3.BIN");
    shifted = mission_briefing.panel_count;
    slot = mission_briefing.panel_count;
    flags = mission_briefing.panel_flags;
    mission_briefing.panel_flags = (uint16)(flags | ((uint32)1 << (shifted & 31)));
    event = mission_briefing.event_cursor;
    if (slot >= MISSION_BRIEFING_PANEL_COUNT)
        abort();
    value = (uint8)event->parameter;
    mission_briefing.panels[slot].label = (uint8)value;
    event = mission_briefing.event_cursor;
    value = event->slot;
    mission_briefing.panels[slot].event_slot = (uint8)value;
    value = mission_briefing.panel_count + 1;
    mission_briefing.panel_count = (uint8)(value);
    ui_animation_set(2, 1, 0, 30);
    ui_widget_set(5, mission_briefing.panels[slot].label);
    if (mission_briefing.panel_count >= 2)
    {
        ui_animation_set(3, 1, 0, 30);
        ui_widget_set(6, mission_briefing.panels[slot - 1].label);
    }
    if (mission_briefing.panel_count >= 3)
    {
        ui_animation_set(4, 1, 0, 30);
        ui_widget_set(7, mission_briefing.panels[slot - 2].label);
    }
    return sound_play(100);
}

uint32 mission_briefing_set_panel_rows(void)
{
    uint32 bank;
    sint32 rows;

    FUNCTION_MARKER(0x80102D1C, "ACE3.BIN");
    rows = mission_briefing.event_cursor->angle_or_rows;
    for (bank = 0; bank < 2; ++bank)
    {
        POLY_FT4 *panel = &ui_packets.scene_bank[bank].prims->panel;
        panel->p[0].xy.x = (sint16)(uint16)(32);
        panel->p[0].xy.y = (sint16)(uint16)(300);
        panel->p[1].xy.x = (sint16)(uint16)(288);
        panel->p[1].xy.y = (sint16)(uint16)(300);
        panel->p[2].xy.x = (sint16)(uint16)(32);
        panel->p[3].xy.x = (sint16)(uint16)(288);
        panel->p[2].xy.y = (sint16)(uint16)((uint32)(rows * 24 + 300));
        panel->p[0].uv.u = (uint8)(0);
        panel->p[3].xy.y = (sint16)(uint16)((uint32)(rows * 24 + 300));
        panel->p[1].uv.u = (uint8)(255);
        panel->p[0].uv.v = (uint8)(0);
        panel->p[2].uv.u = (uint8)(0);
        panel->p[1].uv.v = (uint8)(0);
        panel->p[3].uv.u = (uint8)(255);
        panel->p[2].uv.v = (uint8)((uint32)(rows * 12 - 2));
        panel->p[3].uv.v = (uint8)((uint32)(rows * 12 - 2));
    }
    return 0;
}

void mission_briefing_draw_objects_step(void)
{
    FUNCTION_MARKER(0x80103100, "ACE3.BIN");
    {
        (void)(mission_briefing_draw_objects());
        return;
    }
}

void mission_briefing_draw_objects(void)
{
    uint32 *ordering_words;
    POLY_FT4 *quad;
    MISSION_BRIEFING_OBJECT *object;
    MISSION_MAP_CAMERA *camera = &mission_briefing_camera;
    static const sint8 signs[8] = {-1, 1, 1, 1, -1, -1, 1, -1};
    MATRIX rotation = {0}, composed = {0};
    VECTOR translation;
    SVECTOR vertices[4] = {0};
    uint32 base, index, distance, value, remaining, numerator, slot;
    uint32 column, row, screen[4], tag, link, offset;
    sint32 divisor, quotient, coordinate, first, second;

    FUNCTION_MARKER(0x80103108, "ACE3.BIN");
    base = ui_packets.scene_prims_address;
    offset = 0;
    for (index = 0; index < MISSION_BRIEFING_OBJECT_COUNT; ++index, offset += 40)
    {
        if (!((mission_briefing.visible_objects >> index) & 1))
            continue;
        object = &mission_briefing.objects[index];
        object->screen_x = (uint16)(0xFFFF);
        object->screen_y = (uint16)(0xFFFF);
        value = (uint32)(sint16)object->x;
        distance = value - ((uint32)camera->position.vx);
        if ((sint32)distance < 0)
            distance = 0 - distance;
        if ((sint32)distance >= 1025)
            continue;
        value = (uint32)(sint16)object->z;
        distance = value - ((uint32)camera->position.vz);
        if ((sint32)distance < 0)
            distance = 0 - distance;
        if ((sint32)distance >= 1025)
            continue;
        if ((sint16)object->motion == 1 && (sint16)object->ticks > 0)
        {
            for (slot = 0; slot < 8; ++slot)
            {
                divisor = (sint16)object->duration;
                remaining = (uint32)(sint16)object->ticks;
                numerator = ((uint32)divisor - remaining) << 7;
                if (signs[slot] < 0)
                    numerator = 0 - numerator;
                if (!divisor)
                {
#ifndef NDEBUG
                    fprintf(stderr, "Resource2 object 80103108 BREAK7\n");
#endif
                    abort();
                }
                if (divisor == -1 && numerator == 0x80000000)
                {
#ifndef NDEBUG
                    fprintf(stderr, "Resource2 object 80103108 BREAK6\n");
#endif
                    abort();
                }
                quotient = (sint32)numerator / divisor;
                if (!(slot & 1))
                {
                    vertices[slot >> 1].vy = 0;
                    vertices[slot >> 1].vx = (sint16)quotient;
                }
                else
                    vertices[slot >> 1].vz = (sint16)quotient;
            }
            value = object->ticks;
            object->ticks = (uint16)(value - 1);
        }
        else
        {
            vertices[0].vx = -128;
            vertices[0].vy = 0;
            vertices[0].vz = 128;
            vertices[1].vx = 128;
            vertices[1].vy = 0;
            vertices[1].vz = 128;
            vertices[2].vx = -128;
            vertices[2].vy = 0;
            vertices[2].vz = -128;
            vertices[3].vx = 128;
            vertices[3].vy = 0;
            vertices[3].vz = -128;
        }
        RotMatrix(&object->angles, &rotation);
        translation.vx = (sint16)object->x;
        translation.vy = (sint16)object->y;
        translation.vz = (sint16)object->z;
        TransMatrix(&rotation, &translation);
        SetRotMatrix(&camera->view);
        for (column = 0; column < 3; ++column)
        {
            for (row = 0; row < 3; ++row)
                xport_gte_write_data(9 + row, (uint16)rotation.m[row][column]);
            xport_gte_execute(0x49E012);
            for (row = 0; row < 3; ++row)
                composed.m[row][column] = (sint16)xport_gte_read_data(9 + row);
        }
        SetTransMatrix(&camera->view);
        value = (uint16)rotation.t[0] | ((uint32)(uint16)rotation.t[1] << 16);
        xport_gte_write_data(0, value);
        xport_gte_write_data(1, (uint32)rotation.t[2]);
        xport_gte_execute(0x480012);
        composed.t[0] = (sint32)xport_gte_read_data(25);
        composed.t[1] = (sint32)xport_gte_read_data(26);
        composed.t[2] = (sint32)xport_gte_read_data(27);
        SetRotMatrix(&composed);
        SetTransMatrix(&composed);
        for (slot = 0; slot < 3; ++slot)
        {
            value = (uint16)vertices[slot].vx | ((uint32)(uint16)vertices[slot].vy << 16);
            xport_gte_write_data(slot * 2, value);
            xport_gte_write_data(slot * 2 + 1, (uint16)vertices[slot].vz);
        }
        xport_gte_execute(0x280030);
        screen[0] = xport_gte_read_data(12);
        screen[1] = xport_gte_read_data(13);
        screen[2] = xport_gte_read_data(14);
        (void)xport_gte_read_data(8);
        (void)xport_gte_read_control(31);
        (void)xport_gte_read_data(19);
        value = (uint16)vertices[3].vx | ((uint32)(uint16)vertices[3].vy << 16);
        xport_gte_write_data(0, value);
        xport_gte_write_data(1, (uint16)vertices[3].vz);
        xport_gte_execute(0x180001);
        screen[3] = xport_gte_read_data(14);
        (void)xport_gte_read_data(8);
        (void)xport_gte_read_control(31);
        (void)xport_gte_read_data(19);
        first = (sint16)screen[0];
        second = (sint16)screen[3];
        coordinate = (first + second) / 2;
        object->screen_x = (uint16)((uint32)coordinate);
        first = (sint16)(screen[0] >> 16);
        second = (sint16)(screen[3] >> 16);
        coordinate = (first + second) / 2;
        object->screen_y = (uint16)((uint32)coordinate);
        quad = &ui_packets.scene_prims->objects[index];
        for (slot = 0; slot < 4; ++slot)
        {
            quad->p[slot].xy.x = (sint16)screen[slot];
            quad->p[slot].xy.y = (sint16)(screen[slot] >> 16);
        }
        ordering_words = ui_detail_ot_span(ui_packets.scene_detail_ot, 256 * sizeof(*ordering_words));
        tag = quad->tag;
        link = ordering_words[0];
        quad->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
        ordering_words = ui_detail_ot_span(ui_packets.scene_detail_ot, 256 * sizeof(*ordering_words));
        link = ordering_words[0];
        ordering_words[0] = (link & 0xFF000000) | ((base + offset + 2956) & 0xFFFFFF);
    }
    return;
}

uint32 mission_briefing_draw_panels(void)
{
    uint32 phase2, phase3, phase4, progress3, progress4, bank;
    uint32 left, right, command, ordering, head, tag, link, count, value, result;
    sint32 item, scroll;
    SPRT *left_sprite, *right_sprite;
    DR_MODE *command_packet;
    uint32 *head_entry;

    FUNCTION_MARKER(0x80103854, "ACE3.BIN");
    phase2 = ui_animation_phase(2);
    phase3 = ui_animation_phase(3);
    phase4 = ui_animation_phase(4);
    progress3 = ui_animation_ticks(3);
    progress4 = ui_animation_ticks(4);
    bank = game_runtime.display_buffer;
    left = mission_briefing.packet_banks[bank].left_address;
    left_sprite = mission_briefing.packet_banks[bank].left;
    command = mission_briefing.packet_banks[bank].commands_address;
    command_packet = mission_briefing.packet_banks[bank].commands;
    right = mission_briefing.packet_banks[bank].right_address;
    right_sprite = mission_briefing.packet_banks[bank].right;
    tag = command_packet->tag;
    ordering = ui_packets.scene_ot;
    head_entry = ui_scene_ot_span(ordering + 4, sizeof(*head_entry));
    link = *head_entry;
    head = ordering + 4;
    command_packet->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    link = *head_entry;
    *head_entry = (link & 0xFF000000) | (command & 0xFFFFFF);
    count = mission_briefing.panel_count;
    command += 12;
    command_packet += 1;
    if (count >= 2 || (count == 1 && phase2 != 1))
    {
        if (!phase3 || phase3 == 2)
        {
            item = (sint32)mission_briefing.panel_count - 1;
            left_sprite->p[0].uv.u = (uint8)((uint32)((item % 4) * 60));
            left_sprite->p[0].uv.v = (uint8)(187);
            item = (sint32)mission_briefing.panel_count - 1;
            left_sprite->p[0].attr = (uint16)(((uint32)(item / 4 + 16) << 6) | 27);
            left_sprite->p[0].xy.x = (sint16)(uint16)(17);
            left_sprite->p[0].xy.y = (sint16)(uint16)(73);
            item = (sint32)mission_briefing.panel_count - 1;
            right_sprite->p[0].uv.u = (uint8)((uint32)((item % 4) * 56 + 32));
            right_sprite->p[0].uv.v = (uint8)(180);
            item = (sint32)mission_briefing.panel_count - 1;
            right_sprite->p[0].attr = (uint16)(((uint32)(item / 4 + 51) << 6) | 25);
            right_sprite->p[0].xy.x = (sint16)(uint16)(17);
            right_sprite->p[0].xy.y = (sint16)(uint16)(147);
        }
        else
        {
            item = (sint32)mission_briefing.panel_count - 2;
            left_sprite->p[0].uv.u = (uint8)((uint32)((item % 4) * 60));
            left_sprite->p[0].uv.v = (uint8)(187);
            item = (sint32)mission_briefing.panel_count - 2;
            left_sprite->p[0].attr = (uint16)(((uint32)(item / 4 + 16) << 6) | 27);
            left_sprite->p[0].xy.x = (sint16)(uint16)(17);
            scroll = (sint32)(progress3 * 107) / 30;
            left_sprite->p[0].xy.y = (sint16)(uint16)((uint32)(scroll + 73));
            item = (sint32)mission_briefing.panel_count - 2;
            right_sprite->p[0].uv.u = (uint8)((uint32)((item % 4) * 56 + 32));
            right_sprite->p[0].uv.v = (uint8)(180);
            item = (sint32)mission_briefing.panel_count - 2;
            value = GetClut(400, item / 4 + 51);
            right_sprite->p[0].attr = (uint16)(value);
            right_sprite->p[0].xy.x = (sint16)(uint16)(17);
            right_sprite->p[0].xy.y = (sint16)(uint16)((uint32)(scroll + 147));
        }
    }
    left += 20;
    left_sprite += 1;
    count = mission_briefing.panel_count;
    right += 20;
    right_sprite += 1;
    if (count >= 3 || (count == 2 && phase3 != 1))
    {
        if (!phase4 || phase4 == 2)
        {
            item = (sint32)mission_briefing.panel_count - 2;
            left_sprite->p[0].uv.u = (uint8)((uint32)((item % 4) * 60));
            left_sprite->p[0].uv.v = (uint8)(187);
            item = (sint32)mission_briefing.panel_count - 2;
            left_sprite->p[0].attr = (uint16)(((uint32)(item / 4 + 16) << 6) | 27);
            left_sprite->p[0].xy.x = (sint16)(uint16)(17);
            left_sprite->p[0].xy.y = (sint16)(uint16)(180);
            item = (sint32)mission_briefing.panel_count - 2;
            right_sprite->p[0].uv.u = (uint8)((uint32)((item % 4) * 56 + 32));
            right_sprite->p[0].uv.v = (uint8)(180);
            item = (sint32)mission_briefing.panel_count - 2;
            right_sprite->p[0].attr = (uint16)(((uint32)(item / 4 + 51) << 6) | 25);
            right_sprite->p[0].xy.x = (sint16)(uint16)(17);
            right_sprite->p[0].xy.y = (sint16)(uint16)(254);
        }
        else
        {
            item = (sint32)mission_briefing.panel_count - 3;
            left_sprite->p[0].uv.u = (uint8)((uint32)((item % 4) * 60));
            left_sprite->p[0].uv.v = (uint8)(187);
            item = (sint32)mission_briefing.panel_count - 3;
            left_sprite->p[0].attr = (uint16)(((uint32)(item / 4 + 16) << 6) | 27);
            left_sprite->p[0].xy.x = (sint16)(uint16)(17);
            scroll = (sint32)(progress4 * 311) / 30;
            left_sprite->p[0].xy.y = (sint16)(uint16)((uint32)(scroll + 180));
            item = (sint32)mission_briefing.panel_count - 3;
            right_sprite->p[0].uv.u = (uint8)((uint32)((item % 4) * 56 + 32));
            right_sprite->p[0].uv.v = (uint8)(180);
            item = (sint32)mission_briefing.panel_count - 3;
            right_sprite->p[0].attr = (uint16)(((uint32)(item / 4 + 51) << 6) | 25);
            right_sprite->p[0].xy.x = (sint16)(uint16)(17);
            right_sprite->p[0].xy.y = (sint16)(uint16)((uint32)(scroll + 254));
        }
    }
    bank = game_runtime.display_buffer;
    count = mission_briefing.panel_count;
    left = mission_briefing.packet_banks[bank].left_address;
    left_sprite = mission_briefing.packet_banks[bank].left;
    if ((count >= 2 || (count == 1 && phase2 != 1)) && !mission_briefing.exit_mode)
    {
        tag = left_sprite->tag;
        link = *head_entry;
        value = left & 0xFFFFFF;
        left_sprite->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
        link = *head_entry;
        left += 20;
        left_sprite += 1;
        *head_entry = (link & 0xFF000000) | value;
    }
    count = mission_briefing.panel_count;
    if (count >= 3 || (count == 2 && phase3 != 1))
    {
        if (mission_briefing.exit_mode)
            goto right_links;
        tag = left_sprite->tag;
        link = *head_entry;
        left_sprite->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
        link = *head_entry;
        *head_entry = (link & 0xFF000000) | (left & 0xFFFFFF);
    }
    if (!mission_briefing.exit_mode)
    {
        tag = command_packet->tag;
        link = *head_entry;
        value = command & 0xFFFFFF;
        command_packet->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
        link = *head_entry;
        command += 12;
        command_packet += 1;
        *head_entry = (link & 0xFF000000) | value;
    }
right_links:
    bank = game_runtime.display_buffer;
    count = mission_briefing.panel_count;
    right = mission_briefing.packet_banks[bank].right_address;
    right_sprite = mission_briefing.packet_banks[bank].right;
    if ((count >= 2 || (count == 1 && phase2 != 1)) && !mission_briefing.exit_mode)
    {
        tag = right_sprite->tag;
        link = *head_entry;
        value = right & 0xFFFFFF;
        right_sprite->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
        link = *head_entry;
        right += 20;
        right_sprite += 1;
        *head_entry = (link & 0xFF000000) | value;
    }
    count = mission_briefing.panel_count;
    if ((count >= 3 || (count == 2 && phase3 != 1)) && !mission_briefing.exit_mode)
    {
        tag = right_sprite->tag;
        link = *head_entry;
        right_sprite->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
        link = *head_entry;
        *head_entry = (link & 0xFF000000) | (right & 0xFFFFFF);
    }
    tag = command_packet->tag;
    link = *head_entry;
    command_packet->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    link = *head_entry;
    result = (link & 0xFF000000) | (command & 0xFFFFFF);
    *head_entry = result;
    return result;
}

typedef char briefing_initializer_packet_layout_check[sizeof(POLY_FT4) == 40 && offsetof(POLY_FT4, p) == 8 && sizeof(POINT_T) == 8 && sizeof(SPRT) == 20 && offsetof(SPRT, w) == 16 && offsetof(SPRT, h) == 18 && sizeof(DR_MODE) == 12 ? 1 : -1];

typedef char briefing_panel_packet_layout_check[sizeof(SPRT) == 20 && offsetof(SPRT, p) == 8 && sizeof(POINT_T) == 8 && offsetof(POINT_T, uv) == 4 && offsetof(POINT_T, attr) == 6 && sizeof(DR_MODE) == 12 && offsetof(DR_MODE, code) == 4 ? 1 : -1];

static void mission_briefing_interpolate_colors(uint32 colors[4], uint32 first, uint32 next_first, uint32 second, uint32 next_second, sint16 weight, uint32 adjustment)
{
    uint32 red, green, blue, alpha, other, factor, product, color, second_red, second_product;
    red = (colors[first] & 255u);
    other = (colors[next_first] & 255u);
    factor = (uint32)weight + adjustment;
    product = (other - red) * factor;
    color = red + (uint32)((sint32)product / 128);
    green = ((colors[first] >> 8) & 255u);
    other = ((colors[next_first] >> 8) & 255u);
    product = (other - green) * factor;
    color |= (green + (uint32)((sint32)product / 128)) << 8;
    blue = ((colors[first] >> 16) & 255u);
    other = ((colors[next_first] >> 16) & 255u);
    product = (other - blue) * factor;
    second_red = (colors[second] & 255u);
    other = (colors[next_second] & 255u);
    factor = (uint32)weight + adjustment;
    second_product = (other - second_red) * factor;
    color |= (blue + (uint32)((sint32)product / 128)) << 16;
    alpha = ((colors[first] >> 24) & 255u);
    colors[first] = color | (alpha << 24);
    color = second_red + (uint32)((sint32)second_product / 128);
    green = ((colors[second] >> 8) & 255u);
    other = ((colors[next_second] >> 8) & 255u);
    product = (other - green) * factor;
    color |= (green + (uint32)((sint32)product / 128)) << 8;
    blue = ((colors[second] >> 16) & 255u);
    other = ((colors[next_second] >> 16) & 255u);
    product = (other - blue) * factor;
    color |= (blue + (uint32)((sint32)product / 128)) << 16;
    alpha = ((colors[second] >> 24) & 255u);
    colors[second] = color | (alpha << 24);
}

static uint8 mission_briefing_height_byte(const MISSION_HEIGHT_MAP *map, uint32 index)
{
    if (index >= map->bytes)
        abort();
    return map->data[index];
}

uint32 mission_briefing_draw_height_map(uint32 packet)
{
    MISSION_MAP_CAMERA *camera = &mission_briefing_camera;
    const MISSION_HEIGHT_MAP *map = &mission_briefing_height_map;
    uint16 coordinates[8];
    MISSION_MAP_GRID_SCRATCH *scratch = (MISSION_MAP_GRID_SCRATCH *)getScratchAddr(0);
    uint32 *ordering_words;
    uint32 tag, link, value, other, base, parity, sample, cursor;
    uint32 first, second, factor, first_product, second_product, first_height, second_height, depth;
    uint32 height, next_height, color, time;
    sint32 row, column;
    DR_AREA *clip = ui_packet_span(packet, sizeof(*clip));
    FUNCTION_MARKER(0x801048F0, "ACE3.BIN");
    SetDrawArea(clip, &resource002_grid_clip);
    ordering_words = ui_detail_ot_span(ui_packets.scene_detail_ot, 256 * sizeof(*ordering_words));
    tag = clip->tag;
    link = ordering_words[255];
    clip->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    ordering_words = ui_detail_ot_span(ui_packets.scene_detail_ot, 256 * sizeof(*ordering_words));
    link = ordering_words[255];
    cursor = packet + 12;
    ordering_words[255] = (link & 0xFF000000) | (packet & 0xFFFFFF);
    VSync(1);
    value = mission_briefing.group;
    if (!value)
        value = (sint32)mission_flow.phase_timer >= 83;
    if (value)
    {
        SetRotMatrix(&camera->view);
        SetTransMatrix(&camera->view);
        for (row = -8; row < 9; ++row)
        {
            value = ((uint32)camera->position.vz);
            scratch->row = (uint16)((uint32)row + (uint32)((sint32)value / 128));
            value = ((uint32)camera->position.vz);
            scratch->row_fraction = (uint16)(value - (uint32)((sint32)value / 128) * 128);
            value = ((uint32)camera->position.vz);
            if ((sint32)value < 0)
            {
                value = scratch->row;
                other = scratch->row_fraction;
                scratch->row = (uint16)(value - 1);
                scratch->row_fraction = (uint16)(other + 128);
            }
            for (column = -8; column < 9; ++column)
            {
                value = ((uint32)camera->position.vx);
                other = ((sint32)value / 128);
                scratch->column = (uint16)((uint32)column + other);
                scratch->column_fraction = (uint16)(value - other * 128);
                if ((sint32)value < 0)
                {
                    value = scratch->column;
                    other = scratch->column_fraction;
                    scratch->column = (uint16)(value - 1);
                    scratch->column_fraction = (uint16)(other + 128);
                }
                packet = cursor;
                POLY_G4 *quad = ui_packet_span(packet, sizeof(*quad));
                cursor += 36;
                if (column == -8)
                {
                    value = scratch->row;
                    base = ((uint32)(sint32)(sint16)value + 128) * 258 + 128;
                    base += (uint32)(sint16)scratch->column;
                    parity = scratch->column & 1;
                    value = (sint16)scratch->column;
                    sample = mission_briefing_height_byte(map, (uint32)((sint32)base / 2));
                    height = ((sample >> (parity * 4)) & 15) << 7;
                    scratch->corner[0].x = (uint16)(value << 7);
                    scratch->height[0] = (uint16)(height);
                    value = scratch->row;
                    scratch->corner[0].y = (uint16)(0 - height);
                    scratch->corner[0].z = (uint16)((uint32)(sint32)(sint16)value << 7);
                }
                else
                {
                    value = ((uint32)scratch->corner[1].x | ((uint32)scratch->corner[1].y << 16));
                    other = scratch->corner[1].words[1];
                    height = scratch->height[1];
                    scratch->corner[0] = scratch->corner[1];
                    scratch->height[0] = (uint16)(height);
                }
                value = scratch->row;
                first = (sint16)scratch->column;
                second = (sint16)scratch->column;
                base = ((uint32)(sint32)(sint16)value + 128) * 258 + 129 + first;
                value = scratch->row;
                sample = mission_briefing_height_byte(map, (uint32)((sint32)base / 2));
                height = ((sample >> (((first + 1) & 1) * 4)) & 15) << 7;
                scratch->height[1] = (uint16)(height);
                base = ((uint32)(sint32)(sint16)value + 129) * 258 + 128;
                first = (sint16)scratch->column;
                value = scratch->row;
                base += first;
                other = scratch->column;
                sample = mission_briefing_height_byte(map, (uint32)((sint32)base / 2));
                height = ((sample >> ((other & 1) * 4)) & 15) << 7;
                base = ((uint32)(sint32)(sint16)value + 129) * 258 + 129 + second;
                scratch->height[2] = (uint16)(height);
                sample = mission_briefing_height_byte(map, (uint32)((sint32)base / 2));
                scratch->corner[1].x = (uint16)((second + 1) << 7);
                height = scratch->height[1];
                value = scratch->row;
                scratch->corner[1].y = (uint16)(0 - height);
                scratch->corner[2].x = (uint16)(second << 7);
                scratch->corner[1].z = (uint16)((uint32)(sint32)(sint16)value << 7);
                value = scratch->row;
                scratch->corner[3].x = (uint16)((second + 1) << 7);
                height = ((sample >> (((second + 1) & 1) * 4)) & 15) << 7;
                scratch->height[3] = (uint16)(height);
                scratch->corner[2].z = (uint16)(((uint32)(sint32)(sint16)value << 7) + 128);
                value = scratch->row;
                scratch->corner[3].z = (uint16)(((uint32)(sint32)(sint16)value << 7) + 128);
                other = scratch->height[2];
                scratch->corner[3].y = (uint16)(0 - height);
                scratch->corner[2].y = (uint16)(0 - other);
                if (column == -8)
                {
                    first = scratch->height[1];
                    first_height = scratch->height[0];
                    factor = scratch->column_fraction;
                    first_product = ((uint32)(sint32)(sint16)first - (uint32)(sint32)(sint16)first_height) * (uint32)(sint32)(sint16)factor;
                    value = scratch->corner[0].x;
                    factor = scratch->column_fraction;
                    other = scratch->corner[2].x;
                    scratch->corner[0].x = (uint16)(value + factor);
                    scratch->corner[2].x = (uint16)(other + factor);
                    second = scratch->height[3];
                    second_height = scratch->height[2];
                    factor = scratch->column_fraction;
                    second_product = ((uint32)(sint32)(sint16)second - (uint32)(sint32)(sint16)second_height) * (uint32)(sint32)(sint16)factor;
                    scratch->height[0] = (uint16)(first_height + (uint32)((sint32)first_product / 128));
                    scratch->height[2] = (uint16)(second_height + (uint32)((sint32)second_product / 128));
                    value = scratch->height[0];
                    other = scratch->height[2];
                    scratch->corner[0].y = (uint16)(0 - value);
                    scratch->corner[2].y = (uint16)(0 - other);
                }
                if (column == 8)
                {
                    first_height = scratch->height[1];
                    first = scratch->height[0];
                    factor = scratch->column_fraction;
                    first_product = ((uint32)(sint32)(sint16)first_height - (uint32)(sint32)(sint16)first) * ((uint32)(sint32)(sint16)factor - 128);
                    value = scratch->corner[1].x;
                    factor = scratch->column_fraction;
                    other = scratch->corner[3].x;
                    scratch->corner[1].x = (uint16)(value - 128 + factor);
                    factor = scratch->column_fraction;
                    scratch->corner[3].x = (uint16)(other - 128 + factor);
                    second_height = scratch->height[3];
                    second = scratch->height[2];
                    factor = scratch->column_fraction;
                    second_product = ((uint32)(sint32)(sint16)second_height - (uint32)(sint32)(sint16)second) * ((uint32)(sint32)(sint16)factor - 128);
                    scratch->height[1] = (uint16)(first_height + (uint32)((sint32)first_product / 128));
                    scratch->height[3] = (uint16)(second_height + (uint32)((sint32)second_product / 128));
                    value = scratch->height[1];
                    other = scratch->height[3];
                    scratch->corner[1].y = (uint16)(0 - value);
                    scratch->corner[3].y = (uint16)(0 - other);
                }
                if (row == -8)
                {
                    first = scratch->height[2];
                    first_height = scratch->height[0];
                    factor = scratch->row_fraction;
                    first_product = ((uint32)(sint32)(sint16)first - (uint32)(sint32)(sint16)first_height) * (uint32)(sint32)(sint16)factor;
                    value = scratch->corner[0].z;
                    factor = scratch->row_fraction;
                    other = scratch->corner[1].z;
                    scratch->corner[0].z = (uint16)(value + factor);
                    scratch->corner[1].z = (uint16)(other + factor);
                    second = scratch->height[3];
                    second_height = scratch->height[1];
                    factor = scratch->row_fraction;
                    second_product = ((uint32)(sint32)(sint16)second - (uint32)(sint32)(sint16)second_height) * (uint32)(sint32)(sint16)factor;
                    scratch->height[0] = (uint16)(first_height + (uint32)((sint32)first_product / 128));
                    scratch->height[1] = (uint16)(second_height + (uint32)((sint32)second_product / 128));
                    value = scratch->height[0];
                    other = scratch->height[1];
                    scratch->corner[0].y = (uint16)(0 - value);
                    scratch->corner[1].y = (uint16)(0 - other);
                }
                if (row == 8)
                {
                    first_height = scratch->height[2];
                    first = scratch->height[0];
                    factor = scratch->row_fraction;
                    first_product = ((uint32)(sint32)(sint16)first_height - (uint32)(sint32)(sint16)first) * ((uint32)(sint32)(sint16)factor - 128);
                    value = scratch->corner[2].z;
                    factor = scratch->row_fraction;
                    other = scratch->corner[3].z;
                    scratch->corner[2].z = (uint16)(value - 128 + factor);
                    factor = scratch->row_fraction;
                    scratch->corner[3].z = (uint16)(other - 128 + factor);
                    second_height = scratch->height[3];
                    second = scratch->height[1];
                    factor = scratch->row_fraction;
                    second_product = ((uint32)(sint32)(sint16)second_height - (uint32)(sint32)(sint16)second) * ((uint32)(sint32)(sint16)factor - 128);
                    scratch->height[2] = (uint16)(first_height + (uint32)((sint32)first_product / 128));
                    scratch->height[3] = (uint16)(second_height + (uint32)((sint32)second_product / 128));
                    value = scratch->height[2];
                    other = scratch->height[3];
                    scratch->corner[2].y = (uint16)(0 - value);
                    scratch->corner[3].y = (uint16)(0 - other);
                }
                if (column == -8)
                {
                    xport_gte_write_data(0, ((uint32)scratch->corner[0].x | ((uint32)scratch->corner[0].y << 16)));
                    xport_gte_write_data(1, scratch->corner[0].z);
                    xport_gte_execute(0x180001);
                    quad->p[0].xy.xy = xport_gte_read_data(14);
                }
                else
                    quad->p[0].xy.xy = xport_gte_read_data(12);
                xport_gte_write_data(0, ((uint32)scratch->corner[1].x | ((uint32)scratch->corner[1].y << 16)));
                xport_gte_write_data(1, scratch->corner[1].z);
                xport_gte_write_data(2, ((uint32)scratch->corner[2].x | ((uint32)scratch->corner[2].y << 16)));
                xport_gte_write_data(3, scratch->corner[2].z);
                xport_gte_write_data(4, ((uint32)scratch->corner[3].x | ((uint32)scratch->corner[3].y << 16)));
                xport_gte_write_data(5, scratch->corner[3].z);
                xport_gte_execute(0x280030);
                height = scratch->height[0];
                next_height = scratch->height[1];
                color = resource002_terrain_colors[(sint16)height / 128];
                scratch->color[0] = color;
                height = scratch->height[2];
                color = resource002_terrain_colors[(sint16)next_height / 128];
                scratch->color[1] = color;
                next_height = scratch->height[3];
                color = resource002_terrain_colors[(sint16)height / 128];
                scratch->color[2] = color;
                color = resource002_terrain_colors[(sint16)next_height / 128];
                scratch->color[3] = color;
                quad->p[1].xy.xy = xport_gte_read_data(12);
                quad->p[2].xy.xy = xport_gte_read_data(13);
                quad->p[3].xy.xy = xport_gte_read_data(14);
                depth = xport_gte_read_data(19);
                depth = ((sint32)depth >> 2);
                if (column == -8)
                    mission_briefing_interpolate_colors(scratch->color, 0, 1, 2, 3, (sint16)scratch->column_fraction, 0);
                if (row == -8)
                    mission_briefing_interpolate_colors(scratch->color, 0, 2, 1, 3, (sint16)scratch->row_fraction, 0);
                if (column == 8)
                    mission_briefing_interpolate_colors(scratch->color, 1, 0, 3, 2, (sint16)scratch->column_fraction, 0 - 128);
                if (row == 8)
                    mission_briefing_interpolate_colors(scratch->color, 2, 0, 3, 1, (sint16)scratch->row_fraction, 0 - 128);
                value = scratch->color[0];
                quad->p[0].rgbc.rgbc = value | 0x38000000;
                value = scratch->color[1];
                quad->p[1].rgbc.rgbc = value;
                value = scratch->color[2];
                quad->p[2].rgbc.rgbc = value;
                value = scratch->color[3];
                quad->p[3].rgbc.rgbc = value;
                ordering_words = ui_detail_ot_span(ui_packets.scene_detail_ot, 256 * sizeof(*ordering_words));
                other = ((uint32)((sint32)depth >> 6)) << 2;
                value = ordering_words[other >> 2];
                quad->tag = value | 0x08000000;
                ordering_words = ui_detail_ot_span(ui_packets.scene_detail_ot, 256 * sizeof(*ordering_words));
                ordering_words[other >> 2] = packet & 0xFFFFFF;
            }
        }
    }
    if (mission_briefing.group)
        return cursor;
    time = mission_flow.phase_timer;
    if ((sint32)time < 60)
        return cursor;
    if ((sint32)time < 66)
    {
        coordinates[4] = coordinates[0] = (154 - (time - 60) * 8);
        value = mission_flow.phase_timer;
        coordinates[6] = coordinates[2] = (value * 8 - 326);
        value = ui_item_at(0, 5)->y;
        coordinates[3] = coordinates[1] = (value + 153);
        value = ui_item_at(0, 5)->y;
        value += 155;
    }
    else if ((sint32)time < 82)
    {
        coordinates[4] = coordinates[0] = 96;
        coordinates[6] = coordinates[2] = 224;
        other = ui_item_at(0, 5)->y;
        value = mission_flow.phase_timer;
        coordinates[3] = coordinates[1] = (other + 154 - (value - 66) * 8);
        value = mission_flow.phase_timer;
        other = ui_item_at(0, 5)->y;
        value = (value - 66) * 8 + other + 154;
    }
    else
    {
        coordinates[4] = coordinates[0] = 96;
        coordinates[6] = coordinates[2] = 224;
        value = ui_item_at(0, 5)->y;
        coordinates[3] = coordinates[1] = (value + 26);
        value = ui_item_at(0, 5)->y;
        value += 282;
    }
    coordinates[7] = coordinates[5] = value;
    if (mission_flow.phase_timer == 60)
        sound_play(100);
    if (mission_flow.phase_timer == 82)
        sound_play(102);
    if (mission_flow.phase_timer - 82 < 32)
    {
        packet = cursor;
        TILE *highlight = ui_packet_span(packet, sizeof(*highlight));
        cursor += 16;
        highlight->tag = (highlight->tag & 0xFFFFFF) | ((uint32)(uint8)(3) << 24);
        highlight->rgbc.code = (uint8)(98);
        value = ((uint32)camera->position.vx);
        highlight->p[0].x = (sint16)(uint16)((uint32)((sint32)value / 256) + 146);
        value = ((uint32)camera->position.vz);
        value = 154 - ((uint32)((sint32)value / 256) + 8) * 2;
        highlight->p[0].y = (sint16)(uint16)(value);
        highlight->w = (sint16)(uint16)(16);
        highlight->h = (sint16)(uint16)(32);
        value = mission_flow.phase_timer;
        highlight->rgbc.r = (uint8)(0 - 128 - (value - 82) * 2);
        value = mission_flow.phase_timer;
        highlight->rgbc.g = (uint8)(0 - 128 - (value - 82) * 2);
        value = mission_flow.phase_timer;
        highlight->rgbc.b = (uint8)(0 - 128 - (value - 82) * 2);
        ordering_words = ui_detail_ot_span(ui_packets.scene_detail_ot, 256 * sizeof(*ordering_words));
        tag = highlight->tag;
        link = ordering_words[255];
        highlight->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
        ordering_words = ui_detail_ot_span(ui_packets.scene_detail_ot, 256 * sizeof(*ordering_words));
        link = ordering_words[255];
        ordering_words[255] = (link & 0xFF000000) | (packet & 0xFFFFFF);
    }
    packet = cursor;
    POLY_FT4 *frame = ui_packet_span(packet, sizeof(*frame));
    frame->tag = (frame->tag & 0xFFFFFF) | ((uint32)(uint8)(9) << 24);
    frame->rgbc.code = (uint8)(44);
    frame->p[0].xy.x = (sint16)(uint16)(coordinates[0]);
    frame->p[0].xy.y = (sint16)(uint16)(coordinates[1]);
    frame->p[1].xy.x = (sint16)(uint16)(coordinates[2]);
    frame->p[1].xy.y = (sint16)(uint16)(coordinates[3]);
    frame->p[2].xy.x = (sint16)(uint16)(coordinates[4]);
    frame->p[2].xy.y = (sint16)(uint16)(coordinates[5]);
    frame->p[3].xy.x = (sint16)(uint16)(coordinates[6]);
    frame->p[3].xy.y = (sint16)(uint16)(coordinates[7]);
    time = mission_flow.phase_timer;
    cursor += 40;
    if ((sint32)time < 82)
    {
        frame->rgbc.r = (uint8)(128);
        frame->rgbc.g = (uint8)(128);
        frame->rgbc.b = (uint8)(128);
    }
    else if ((sint32)time < 114)
    {
        frame->rgbc.r = (uint8)(0 - 128 - (time - 82) * 4);
        value = mission_flow.phase_timer;
        frame->rgbc.g = (uint8)(0 - 128 - (value - 82) * 4);
        value = mission_flow.phase_timer;
        frame->rgbc.b = (uint8)(0 - 128 - (value - 82) * 4);
    }
    else
    {
        frame->rgbc.r = (uint8)(0);
        frame->rgbc.g = (uint8)(0);
        frame->rgbc.b = (uint8)(0);
    }
    frame->p[1].attr = (uint16)(37);
    frame->p[0].attr = (uint16)(0x5714);
    frame->p[1].uv.u = (uint8)(127);
    frame->p[2].uv.v = (uint8)(127);
    frame->p[3].uv.u = (uint8)(127);
    frame->p[3].uv.v = (uint8)(127);
    frame->p[0].uv.u = (uint8)(0);
    frame->p[0].uv.v = (uint8)(0);
    frame->p[1].uv.v = (uint8)(0);
    frame->p[2].uv.u = (uint8)(0);
    ordering_words = ui_detail_ot_span(ui_packets.scene_detail_ot, 256 * sizeof(*ordering_words));
    tag = frame->tag;
    link = ordering_words[255];
    frame->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    ordering_words = ui_detail_ot_span(ui_packets.scene_detail_ot, 256 * sizeof(*ordering_words));
    link = ordering_words[255];
    ordering_words[255] = (link & 0xFF000000) | (packet & 0xFFFFFF);
    return cursor;
}

uint32 mission_briefing_frame(void)
{
    MISSION_BRIEFING_OBJECT *object;
    MISSION_MAP_CAMERA *camera = &mission_briefing_camera;
    uint32 group, phase, packet, ordering, tag, link, value, index, type;
    uint32 scroll_x, scroll_y, center_x, x, y, left, right, cursor;
    DR_AREA *area;
    TILE *icon;
    POLY_FT4 *map_quad;
    uint32 *ordering_entry;
    FUNCTION_MARKER(0x80106A34, "ACE3.BIN");
    group = mission_briefing.group;
    phase = mission_briefing.phase;
    mission_dispatch_map(group, phase);
    if (mission_briefing.exit_mode == 0)
    {
        packet = ui_packets.scene_packet_cursor;
        (void)ui_packets.scene_detail_ot;
        packet = mission_briefing_draw_height_map(packet);
        ui_packets.scene_packet_cursor = (uint32)(packet);
    }
    packet = ui_packets.scene_packet_cursor;
    (void)ui_packets.scene_detail_ot;
    packet = mission_briefing_draw_marker(packet);
    ui_packets.scene_packet_cursor = (uint32)(packet);
    packet = ui_packets.scene_packet_cursor;
    ordering = ui_packets.scene_detail_ot;
    ordering_entry = ui_detail_ot_span(ordering, sizeof(*ordering_entry));
    ui_packets.scene_packet_cursor = (uint32)(packet + 12);
    area = ui_packet_span(packet, sizeof(*area));
    SetDrawArea(area, &ui_fullscreen_clip);
    tag = area->tag;
    link = *ordering_entry;
    area->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    link = *ordering_entry;
    *ordering_entry = (link & 0xFF000000) | (packet & 0xFFFFFF);
    if (mission_briefing.group && mission_briefing.phase)
    {
        value = ((uint32)camera->position.vx);
        cursor = ui_packets.scene_packet_cursor;
        ordering = ui_packets.scene_detail_ot;
        ordering_entry = ui_detail_ot_span(ordering, sizeof(*ordering_entry));
        scroll_x = (uint32)((sint32)value / 128) + 128;
        value = 0 - ((uint32)camera->position.vz);
        scroll_y = (uint32)((sint32)value / 128) + 128;
        cursor += 16;
        center_x = (uint32)(sint32)(sint16)scroll_x - 128;
        for (index = 0; index < MISSION_BRIEFING_OBJECT_COUNT; ++index)
        {
            value = mission_briefing.visible_objects;
            if (((value >> index) & 1) == 0)
                continue;
            object = &mission_briefing.objects[index];
            type = (sint16)object->icon;
            if (type == 9 || (sint32)type >= 11)
                continue;
            x = (sint16)object->x;
            x = (uint32)((sint32)x / 128) - center_x;
            y = 0 - (uint32)(sint16)object->z;
            x = ((sint32)x / 2);
            y = (uint32)((sint32)y / 128) - scroll_y;
            packet = cursor;
            cursor += 16;
            icon = ui_packet_span(packet, sizeof(*icon));
            icon->tag = (icon->tag & 0xFFFFFF) | ((uint32)(3) << 24);
            icon->rgbc.code = (uint8)(96);
            icon->p[0].x = (sint16)(uint16)(x + 275);
            icon->p[0].y = (sint16)(uint16)(y + 292);
            icon->w = (sint16)(uint16)(3);
            icon->h = (sint16)(uint16)(6);
            type = (sint16)object->icon;
            if (type < 11)
            {
                icon->rgbc.r = (uint8)(resource002_map_icon_colors[type][0]);
                icon->rgbc.g = (uint8)(resource002_map_icon_colors[type][1]);
                icon->rgbc.b = (uint8)(resource002_map_icon_colors[type][2]);
            }
            if (mission_briefing.exit_mode == 0)
            {
                tag = icon->tag;
                link = *ordering_entry;
                icon->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
                link = *ordering_entry;
                *ordering_entry = (link & 0xFF000000) | (packet & 0xFFFFFF);
            }
        }
        packet = cursor;
        ui_packets.scene_packet_cursor = (uint32)(cursor);
        map_quad = ui_packet_span(packet, sizeof(*map_quad));
        map_quad->tag = (map_quad->tag & 0xFFFFFF) | ((uint32)(9) << 24);
        map_quad->rgbc.code = (uint8)(44);
        value = ((sint32)(sint16)scroll_x / 2);
        left = 275 - value;
        right = 403 - value;
        map_quad->p[0].xy.y = (sint16)(uint16)(164 - scroll_y);
        map_quad->p[1].xy.y = (sint16)(uint16)(164 - scroll_y);
        map_quad->p[2].xy.y = (sint16)(uint16)(420 - scroll_y);
        map_quad->p[3].xy.y = (sint16)(uint16)(420 - scroll_y);
        map_quad->p[1].attr = (uint16)(5);
        map_quad->rgbc.r = (uint8)(128);
        map_quad->rgbc.g = (uint8)(128);
        map_quad->rgbc.b = (uint8)(128);
        map_quad->p[0].uv.u = (uint8)(0);
        map_quad->p[0].uv.v = (uint8)(0);
        map_quad->p[1].uv.u = (uint8)(128);
        map_quad->p[1].uv.v = (uint8)(0);
        map_quad->p[2].uv.u = (uint8)(0);
        map_quad->p[2].uv.v = (uint8)(128);
        map_quad->p[3].uv.u = (uint8)(128);
        map_quad->p[3].uv.v = (uint8)(128);
        map_quad->p[0].xy.x = (sint16)(uint16)(left);
        map_quad->p[1].xy.x = (sint16)(uint16)(right);
        map_quad->p[2].xy.x = (sint16)(uint16)(left);
        map_quad->p[3].xy.x = (sint16)(uint16)(right);
        map_quad->p[0].attr = (uint16)(0x5714);
        cursor += 40;
        if (mission_briefing.exit_mode == 0)
        {
            tag = map_quad->tag;
            link = *ordering_entry;
            map_quad->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
            link = *ordering_entry;
            *ordering_entry = (link & 0xFF000000) | (packet & 0xFFFFFF);
        }
        ui_packets.scene_packet_cursor = (uint32)(cursor);
    }
    packet = ui_packets.scene_packet_cursor;
    ordering = ui_packets.scene_detail_ot;
    ordering_entry = ui_detail_ot_span(ordering, sizeof(*ordering_entry));
    ui_packets.scene_packet_cursor = (uint32)(packet + 12);
    area = ui_packet_span(packet, sizeof(*area));
    SetDrawArea(area, &resource002_detail_clip);
    tag = area->tag;
    link = *ordering_entry;
    area->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    link = *ordering_entry;
    *ordering_entry = (link & 0xFF000000) | (packet & 0xFFFFFF);
    if ((game_input.pressed & 32) && (game_input.held & 0x5F) == 0)
    {
        subtitles_configure(255, 0, 0);
        if ((sint16)ui_item_at(0, 5)->phase == 2)
        {
            ui_item_at(0, 5)->phase = (uint16)(3);
            ui_animation_set(1, 0, 0, 0);
        }
        else
        {
            mission_effect_open_sprs();
            ui_animation_set(1, 3, -1, -1);
            if (mission_briefing.group)
            {
                mission_flow.ui_timer = (uint32)(29);
            }
            else
            {
                value = (sint16)ui_item_at(0, 5)->phase;
                if (value == 0)
                    mission_flow.ui_timer = (uint32)(0);
                else if (value == 1)
                {
                    value = (mission_flow.ui_timer - 258) * 29;
                    value = ((sint32)value / 64);
                    mission_flow.ui_timer = (uint32)(value);
                    if ((sint32)value < 0)
                        mission_flow.ui_timer = (uint32)(0);
                    if ((sint32)mission_flow.ui_timer >= 30)
                        mission_flow.ui_timer = (uint32)(29);
                }
                else
                    mission_flow.ui_timer = (uint32)(29);
            }
            ui_item_reverse_motion(0, 5);
            ui_item_reverse_motion(0, 7);
            sound_play(91);
            sound_play(59);
        }
        mission_briefing.exit_mode = (uint8)(2);
    }
    if ((sint16)ui_item_at(0, 5)->phase != 3)
        return 3;
    if (mission_briefing.exit_mode == 1)
    {
        mission_audio_control_track(3, (uint32)(sint16)game_runtime.selection.mission);
        mission_flow.mode = 2;
        mission_flow.mode_phase = 0;
    }
    if (mission_briefing.exit_mode == 2)
    {
        mission_flow.root_kind = 0;
        mission_flow.root_phase = 0;
        mission_flow.root_variant = 0;
        mission_menu_start_item_motion(0, 1);
        mission_audio_control_track(3, (uint32)(sint16)game_runtime.selection.mission);
    }
    return mission_briefing_leave();
}

typedef char briefing_map_packet_layout_check[sizeof(DR_AREA) == 12 && offsetof(DR_AREA, code) == 4 && sizeof(TILE) == 16 && offsetof(TILE, p) == 8 && offsetof(TILE, wh) == 12 && sizeof(POLY_FT4) == 40 && offsetof(POLY_FT4, p) == 8 && sizeof(POINT_T) == 8 ? 1 : -1];
