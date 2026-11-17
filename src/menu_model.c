#include <stdlib.h>
#include <string.h>

#include "game_functions.h"

typedef struct
{
    uint32 pal;
    uint32 model;
} AIRCRAFT_MODEL_RESOURCES;

typedef char aircraft_model_resources_layout_check[sizeof(AIRCRAFT_MODEL_RESOURCES) == 8 && offsetof(AIRCRAFT_MODEL_RESOURCES, model) == 4 ? 1 : -1];

typedef struct
{
    sint32 depth;
    GAME_ANGLES angles;
} MENU_MODEL_VIEW;

static MENU_MODEL_VIEW view;
static const SCENE_MESH_MODEL_HEADER *detail_model_headers[AIRCRAFT_COMPOSITE_MODEL_COUNT];
static const SCENE_MESH_MODEL_HEADER *aircraft_preview_model_headers[64];
static RESOURCE_ASSET_SOURCE aircraft_preview_model_sources[64];
AIRCRAFT_PREVIEW_STATE menu_model_preview;

static const MENU_MODEL_PREVIEW_ENTRY preview_catalog[MENU_MODEL_PREVIEW_COUNT] = {
    {416, 0}, {417, 0}, {418, 69}, {419, 70}, {420, 97}, {421, 71}, {422, 72}, {423, 98}, {424, 73}, {425, 74}, {426, 75}, {427, 0}, {428, 0}, {429, 0}, {430, 0}, {431, 0}, {432, 0}, {433, 76}, {434, 77}, {435, 78}, {436, 79}, {437, 80}, {438, 81}, {439, 82}, {440, 99}, {441, 83}, {442, 0}, {443, 0}, {444, 84}, {445, 85}, {446, 86}, {447, 100}, {448, 87}, {449, 88}, {450, 101}, {451, 89}, {452, 102}, {453, 90}, {458, 91}, {454, 0}, {455, 0}, {456, 0}, {457, 0}, {459, 92}, {460, 93}, {461, 94}, {462, 95}, {463, 0},
};

const MENU_MODEL_PREVIEW_ENTRY *menu_model_preview_entry(uint32 index)
{
    if (index >= MENU_MODEL_PREVIEW_COUNT)
        abort();
    return &preview_catalog[index];
}

void menu_model_reset(void)
{
    memset(&view, 0, sizeof(view));
}

static void menu_model_adjust(void)
{
    uint16 delta[4] = {0, 0, 0, 0};
    uint32 value;
    FUNCTION_MARKER(0x801084D0, "ACE3.BIN");
    if (game_input.held & 0x200)
    {
        value = (uint32)view.depth;
        if ((sint32)value > -2097152)
            view.depth = (sint32)(uint32)(value - 4096);
    }
    if (game_input.held & 0x100)
    {
        value = (uint32)view.depth;
        if ((sint32)value < -524288)
            view.depth = (sint32)(uint32)(value + 4096);
    }
    if (game_input.held & 8)
        delta[1] = 128;
    if (game_input.held & 4)
        delta[1] = -128;
    if (game_input.held & 1)
        delta[0] = -128;
    if (game_input.held & 2)
        delta[0] = 128;
    {
        game_angles_apply_delta(delta, view.angles.value);
        return;
    }
}

void menu_model_apply_view(uint32 mode)
{
    AIRCRAFT_PREVIEW_STATE *preview = &menu_model_preview;
    MISSION_ACTOR *pose_owner_object;
    uint32 actor_index_slot;
    uint32 actor_index_table;

    uint32 fade, index, selected, delta, product;
    FUNCTION_MARKER(0x801085F8, "ACE3.BIN");
    fade = ui_animation_ticks(2);
    index = preview->index;
    selected = menu_model_preview_entry(index)->detail_resource == 0 ? 2 : 0;
    if (mode == 0)
    {
        delta = 25 - fade;
        product = delta * (0u - delta * 325u);
        SetGeomOffset((uint32)((sint32)product / 625) + 80, 132);
        index = 2;
        actor_index_table = 0;
        pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        pose_owner_object->pose.angles[0] = (uint16)(0);
        index = 2;
        pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        pose_owner_object->pose.angles[1] = (uint16)(4096);
        index = 2;
        pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
        pose_owner_object->pose.angles[2] = (uint16)(0);
    }
    else if (mode == 1)
    {
        menu_model_adjust();
        SetGeomOffset(160, 240);
        actor_index_table = 0;
        actor_index_slot = (actor_index_table + ((uint32)((sint32)(selected * 4) >> 2)));
        pose_owner_object = mission_actor(actor_index_slot);
        pose_owner_object->pose.angles[0] = (uint16)(view.angles.value[0]);
        pose_owner_object = mission_actor(actor_index_slot);
        pose_owner_object->pose.angles[1] = (uint16)(view.angles.value[1]);
        pose_owner_object = mission_actor(actor_index_slot);
        product = view.angles.value[2];
        pose_owner_object->pose.angles[2] = (uint16)(product);
        game_runtime.camera_view.pose.position[0] = (uint32)(0);
        game_runtime.camera_view.pose.position[1] = (uint32)((uint32)view.depth);
        game_runtime.camera_view.pose.position[2] = (uint32)(0);
        return;
    }
    else if (mode == 2)
    {
        delta = 25 - fade;
        product = delta * (0u - delta * 325u);
        SetGeomOffset((uint32)((sint32)product / 625) + 80, 132);
        actor_index_table = 0;
        pose_owner_object = mission_actor((actor_index_table + 2));
        pose_owner_object->pose.angles[0] = (uint16)(0);
        pose_owner_object = mission_actor((actor_index_table + 2));
        pose_owner_object->pose.angles[1] = (uint16)(4096);
        pose_owner_object = mission_actor((actor_index_table + 2));
        pose_owner_object->pose.angles[2] = (uint16)(0);
    }
    else
        return;
    game_runtime.camera_view.pose.position[0] = (uint32)(0);
    game_runtime.camera_view.pose.position[1] = (uint32)(0xFFF00000);
    game_runtime.camera_view.pose.position[2] = (uint32)(0);
    return;
}

uint32 menu_model_load(void)
{
    AIRCRAFT_PREVIEW_STATE *preview = &menu_model_preview;
    uint32 owner, resource, container, sections[2], source;
    FUNCTION_MARKER(0x80108334, "ACE3.BIN");
    if (preview->load_requested == 0)
        return 0xFFFFFFFF;
    if (game_runtime.resource_pending == 1)
        return 0;
    if (ui_animation_phase(2))
        return 0xFFFFFFFF;
    resource = game_runtime.resource_slot.encoded_base;
    const uint8 *asset_data = game_runtime.resource_slot.data;
    uint32 asset_bytes = game_runtime.resource_slot.read_bytes;
    mission_image_sections(resource, asset_data, asset_bytes, &container, 1);
    if (container < resource || container - resource > asset_bytes || asset_bytes - (container - resource) < 3 * sizeof(uint32))
        abort();
    mission_image_sections(container, asset_data + (container - resource), asset_bytes - (container - resource), sections, 2);
    scene_palette_capture(sections[0], (RESOURCE_ASSET_SOURCE){asset_data, asset_bytes, resource});
    source = sections[1];
    owner = preview->model_buffer;
    uint32 ulz_offset = source - resource;
    if (!asset_data || source < resource || ulz_offset > asset_bytes || asset_bytes - ulz_offset < 16)
        abort();
    const uint8 *ulz_input = asset_data + ulz_offset;
    uint8 *ulz_output = preview->model_data;
    if (!ulz_output)
        abort();
    uint32 decoded_bytes = (uint32)game_decompress_buffer(ulz_input, asset_bytes - ulz_offset, ulz_output, MENU_MODEL_BUFFER_BYTES);
    owner = preview->model_buffer;
    if (game_runtime.scene_assets.model_headers != aircraft_preview_model_headers || game_runtime.scene_assets.model_header_count != 64)
        abort();
    aircraft_preview_model_headers[8] = (const SCENE_MESH_MODEL_HEADER *)ulz_output;
    aircraft_preview_model_sources[8] = (RESOURCE_ASSET_SOURCE){ulz_output, decoded_bytes, owner};
    source = owner;
    relocate_pointer_array(source, ulz_output + 4, 6);
    view.depth = (sint32)(uint32)(0xFFF00000);
    view.angles.value[0] = (uint16)(3072);
    view.angles.value[1] = (uint16)(10240);
    view.angles.value[2] = (uint16)(0);
    preview->load_requested = (uint8)(0);
    ui_animation_set(2, 1, 0, 25);
    ui_animation_set(8, 1, 0, 25);
    arena_set_cursor(resource);
    return 1;
}

uint32 menu_model_restore(void)
{
    AIRCRAFT_PREVIEW_STATE *preview = &menu_model_preview;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 index, value, phase;
    FUNCTION_MARKER(0x80109660, "ACE3.BIN");
    ui_group_set_mask(2, 0x100000);
    index = preview->index;
    if (menu_model_preview_entry(index)->detail_resource)
    {
        actor_index_table = 0;
        actor_traits_object = mission_actor_traits(actor_index_table);
        actor_traits_object->flags = (uint32)(0);
        actor_traits_object = mission_actor_traits((actor_index_table + 2));
        value = actor_traits_object->flags;
        actor_traits_object->flags = (uint32)(value | 0x1C000);
        arena_set_cursor(preview->detail_bookmark);
        view.depth = (sint32)(uint32)(0xFFF00000);
        view.angles.value[0] = (uint16)(3072);
        view.angles.value[1] = (uint16)(10240);
        view.angles.value[2] = (uint16)(0);
        menu_model_request();
        phase = 1;
    }
    else
    {
        static const uint8 slots[] = {2, 1, 3, 11, 5, 8};
        for (index = 0; index < 6; ++index)
            ui_animation_set(slots[index], 1, 0, 25);
        phase = 2;
    }
    menu_state.header.phase = (uint8)(phase);
    return phase;
}

uint32 menu_model_draw_aircraft(uint32 mode)
{
    PSX_RECT rectangle;
    uint32 packet, ordering, link, tag, result, delta, product;
    FUNCTION_MARKER(0x80108874, "ACE3.BIN");
    SetRotMatrix(&game_runtime.mission_matrices.view);
    SetTransMatrix(&game_runtime.mission_matrices.view);
    SetBackColor((sint32)game_runtime.mission_lighting.back_color[0], (sint32)game_runtime.mission_lighting.back_color[1], (sint32)game_runtime.mission_lighting.back_color[2]);
    SetColorMatrix(&game_runtime.mission_lighting.colors);
    SetRotMatrix(&game_runtime.mission_matrices.view);
    SetTransMatrix(&game_runtime.mission_matrices.view);
    game_runtime.camera_proj.size[0] = (uint16)(320);
    game_runtime.camera_proj.size[1] = (uint16)(480);
    game_runtime.camera_proj.distance = (uint16)(250);
    if (mode == 1)
    {
        delta = 25 - ui_animation_ticks(1);
        product = delta * (0u - delta * 325u);
        packet = game_runtime.mission_control.packet_cur;
        game_runtime.mission_control.packet_cur = (uint32)(packet + 12);
        rectangle.x = (uint16)((uint32)((sint32)product / 625) + 23);
        rectangle.y = 58;
        rectangle.w = 155;
        rectangle.h = 147;
        DR_AREA *clip = ui_packet_span(packet, 12);
        SetDrawArea(clip, &rectangle);
        ordering = game_runtime.mission_control.ot;
        tag = clip->tag;
        link = *(uint32 *)mission_ot_span(ordering + 252, 4);
        clip->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
        ordering = game_runtime.mission_control.ot;
        link = *(uint32 *)mission_ot_span(ordering + 252, 4);
        *(uint32 *)mission_ot_span(ordering + 252, 4) = (link & 0xFF000000) | (packet & 0xFFFFFF);
    }
    packet = game_runtime.mission_control.packet_cur;
    game_runtime.mission_control.packet_cur = (uint32)(packet + 12);
    DR_MODE *draw_mode = ui_packet_span(packet, 12);
    SetDrawMode(draw_mode, 0, 0, 0, NULL);
    ordering = game_runtime.mission_control.ot;
    tag = draw_mode->tag;
    link = *(uint32 *)mission_ot_span(ordering + 4, 4);
    draw_mode->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    ordering = game_runtime.mission_control.ot;
    link = *(uint32 *)mission_ot_span(ordering + 4, 4);
    *(uint32 *)mission_ot_span(ordering + 4, 4) = (link & 0xFF000000) | (packet & 0xFFFFFF);
    ordering = game_runtime.mission_control.ot;
    packet = game_runtime.mission_control.packet_cur;
    if (game_runtime.scene_assets.model_headers && (game_runtime.scene_assets.model_headers != aircraft_preview_model_headers || aircraft_preview_model_headers[8]))
        packet = scene_geometry_draw_aircraft(&game_runtime.scene_assets, ordering + 4, packet);
    game_runtime.mission_control.packet_cur = (uint32)(packet + 12);
    draw_mode = ui_packet_span(packet, 12);
    SetDrawMode(draw_mode, 0, 1, 0, NULL);
    ordering = game_runtime.mission_control.ot;
    tag = draw_mode->tag;
    link = *(uint32 *)mission_ot_span(ordering, 4);
    draw_mode->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    ordering = game_runtime.mission_control.ot;
    link = *(uint32 *)mission_ot_span(ordering, 4);
    result = (link & 0xFF000000) | (packet & 0xFFFFFF);
    *(uint32 *)mission_ot_span(ordering, 4) = result;
    if (mode == 1)
    {
        packet = game_runtime.mission_control.packet_cur;
        game_runtime.mission_control.packet_cur = (uint32)(packet + 12);
        rectangle.x = 0;
        rectangle.y = 0;
        rectangle.w = 320;
        rectangle.h = 480;
        DR_AREA *clip = ui_packet_span(packet, 12);
        SetDrawArea(clip, &rectangle);
        ordering = game_runtime.mission_control.ot;
        tag = clip->tag;
        link = *(uint32 *)mission_ot_span(ordering, 4);
        clip->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
        ordering = game_runtime.mission_control.ot;
        link = *(uint32 *)mission_ot_span(ordering, 4);
        result = (link & 0xFF000000) | (packet & 0xFFFFFF);
        *(uint32 *)mission_ot_span(ordering, 4) = result;
    }
    return result;
}

void menu_model_request(void)
{
    RESOURCE_STREAM_BUFFER asset_allocation;
    AIRCRAFT_PREVIEW_STATE *preview = &menu_model_preview;
    uint32 index, resource, bytes, buffer;
    FUNCTION_MARKER(0x80108298, "ACE3.BIN");
    index = preview->index;
    resource = menu_model_preview_entry(index)->model_resource;
    game_runtime.resource_slot.resource_id = (uint32)(resource);
    bytes = resource_total_size(&game_runtime.resource_slot.resource_id, 1);
    asset_allocation = arena_alloc_backed(bytes);
    buffer = asset_allocation.encoded_begin;
    game_runtime.resource_pending = 1;
    resource_queue_asset(&game_runtime.resource_slot, asset_allocation, resource_asset_read_finished);
    preview->load_requested = (uint8)(1);
}

uint32 menu_model_begin(void)
{
    RESOURCE_STREAM_BUFFER buffer_allocation;
    AIRCRAFT_PREVIEW_STATE *preview = &menu_model_preview;
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 value;
    FUNCTION_MARKER(0x80108B7C, "ACE3.BIN");
    buffer_allocation = arena_alloc_backed(MENU_MODEL_BUFFER_BYTES);
    preview->model_buffer = buffer_allocation.encoded_begin;
    preview->model_data = buffer_allocation.buffer.data;
    preview->index = (uint8)(0);
    preview->load_requested = (uint8)(1);
    preview->exit_action = (uint8)(0);
    aircraft_preview_initialize();
    menu_model_initialize_camera();
    menu_model_request();
    scene_texture_load_group_sized(mission_flow.assets.aircraft_preview_script, 0x10000, mission_flow.assets.options_source, SCENE_TEXTURE_STANDARD);
    ui_item_reset_motion(2, 20);
    ui_group_set_mask(2, 0x100000);
    ui_item_reset_motion(0, 9);
    ui_item_reset_motion(0, 12);
    ui_item_reset_motion(0, 10);
    ui_item_reset_motion(0, 14);
    ui_item_reset_motion(0, 8);
    ui_group_set_mask(0, 0x5701);
    menu_state.header.phase = (uint8)(1);
    ui_animation_set(2, 0, 0, 0);
    ui_widget_set(1, 0);
    actor_index_table = 0;
    actor_traits_object = mission_actor_traits(actor_index_table);
    actor_traits_object->flags = (uint32)(0);
    actor_traits_object = mission_actor_traits((actor_index_table + 2));
    actor_traits_object->flags = (uint32)(0);
    actor_traits_object = mission_actor_traits(actor_index_table);
    actor_traits_object->model = (uint8)(2);
    actor_index_table = 0;
    actor_traits_object = mission_actor_traits((actor_index_table + 2));
    actor_traits_object->model = (uint8)(2);
    actor_index_table = 0;
    actor_traits_object = mission_actor_traits(actor_index_table);
    value = actor_traits_object->render_flags;
    actor_traits_object->render_flags = (uint8)(value | 1);
    actor_index_table = 0;
    pose_owner_object = mission_actor(actor_index_table);
    actor_traits_object = mission_actor_traits(actor_index_table);
    pose_owner_object->pose.position[0] = (uint32)(0);
    pose_owner_object = mission_actor(actor_index_table);
    actor_traits_object = mission_actor_traits(actor_index_table);
    pose_owner_object->pose.position[1] = (uint32)(0);
    pose_owner_object = mission_actor(actor_index_table);
    actor_traits_object = mission_actor_traits(actor_index_table);
    pose_owner_object->pose.position[2] = (uint32)(0);
    pose_owner_object = mission_actor(actor_index_table);
    actor_traits_object = mission_actor_traits(actor_index_table);
    pose_owner_object->pose.angles[0] = (uint16)(0);
    pose_owner_object = mission_actor(actor_index_table);
    actor_traits_object = mission_actor_traits(actor_index_table);
    pose_owner_object->pose.angles[1] = (uint16)(4096);
    pose_owner_object = mission_actor(actor_index_table);
    actor_traits_object = mission_actor_traits(actor_index_table);
    pose_owner_object->pose.angles[2] = (uint16)(4096);
    pose_owner_object = mission_actor((actor_index_table + 2));
    actor_traits_object = mission_actor_traits((actor_index_table + 2));
    pose_owner_object->pose.position[0] = (uint32)(0);
    pose_owner_object = mission_actor((actor_index_table + 2));
    actor_traits_object = mission_actor_traits((actor_index_table + 2));
    pose_owner_object->pose.position[1] = (uint32)(0);
    pose_owner_object = mission_actor((actor_index_table + 2));
    actor_traits_object = mission_actor_traits((actor_index_table + 2));
    pose_owner_object->pose.position[2] = (uint32)(0);
    pose_owner_object = mission_actor((actor_index_table + 2));
    actor_traits_object = mission_actor_traits((actor_index_table + 2));
    pose_owner_object->pose.angles[0] = (uint16)(0);
    pose_owner_object = mission_actor((actor_index_table + 2));
    actor_traits_object = mission_actor_traits((actor_index_table + 2));
    pose_owner_object->pose.angles[1] = (uint16)(4096);
    pose_owner_object = mission_actor((actor_index_table + 2));
    actor_traits_object = mission_actor_traits((actor_index_table + 2));
    pose_owner_object->pose.angles[2] = (uint16)(4096);
    game_runtime.camera_control.excluded_actor = (uint32)(1);
    return 1;
}

void menu_model_wait_load(void)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 value;
    FUNCTION_MARKER(0x80108D90, "ACE3.BIN");
    if (menu_model_load() == 1)
    {
        menu_state.header.phase = (uint8)(2);
        ui_animation_set(1, 1, 0, 25);
        ui_animation_set(3, 1, 0, 25);
        ui_animation_set(11, 1, 0, 25);
        ui_animation_set(5, 1, 0, 25);
        ui_animation_set(8, 1, 0, 25);
    }
    actor_index_table = 0;
    actor_traits_object = mission_actor_traits((actor_index_table + 2));
    value = actor_traits_object->flags;
    actor_traits_object->flags = (uint32)(value | 0x1C000);
    actor_traits_object = mission_actor_traits(actor_index_table);
    actor_traits_object->flags = (uint32)(0);
    ui_widget_set(1, 0);
    ui_widget_set(3, 0);
    ui_widget_set(2, 0);
    {
        ui_widget_set(4, 0);
        return;
    }
}

uint32 menu_model_select_tick(void)
{
    RESOURCE_STREAM_BUFFER asset_allocation;
    AIRCRAFT_PREVIEW_STATE *preview = &menu_model_preview;
    uint32 value, bytes, buffer, packet, ordering, tag, link, result, index;
    uint32 *model_head, *scene_entry;
    FUNCTION_MARKER(0x80108E88, "ACE3.BIN");
    ui_widget_set(1, 0);
    ui_widget_set(2, (game_input.held & 4) != 0);
    ui_widget_set(3, (game_input.held & 8) != 0);
    ui_widget_set(4, (sint16)ui_item_at(0, 8)->phase == 2);
    if (game_input.pressed & 8)
    {
        value = preview->index;
        if (value + 1 < MENU_MODEL_PREVIEW_COUNT)
            preview->index = (uint8)(value + 1);
        menu_model_request();
        ui_animation_set(2, 3, -1, -1);
        ui_animation_set(8, 3, -1, -1);
    }
    if (game_input.pressed & 4)
    {
        value = preview->index;
        if (value)
            preview->index = (uint8)(value - 1);
        menu_model_request();
        ui_animation_set(2, 3, -1, -1);
        ui_animation_set(8, 3, -1, -1);
    }
    if ((game_input.pressed & 16) && (game_input.held & 0x2F) == 0)
    {
        value = preview->index;
        value = (uint32)menu_model_preview_entry(value)->detail_resource;
        if (value)
        {
            game_runtime.resource_slot.resource_id = (uint32)(value);
            bytes = resource_total_size(&game_runtime.resource_slot.resource_id, 1);
            asset_allocation = arena_alloc_backed(bytes);
            buffer = asset_allocation.encoded_begin;
            game_runtime.resource_pending = 1;
            resource_queue_asset(&game_runtime.resource_slot, asset_allocation, resource_asset_read_finished);
        }
        menu_state.header.phase = (uint8)(3);
        {
            static const uint8 slots[] = {2, 8, 1, 3, 11, 5};
            for (index = 0; index < 6; ++index)
                ui_animation_set(slots[index], 3, -1, -1);
        }
        preview->exit_action = (uint8)(1);
    }
    menu_model_load();
    packet = game_runtime.mission_control.ot;
    ClearOTagR(mission_ot_span(packet, 256), 64);
    packet = game_runtime.mission_control.ot;
    ordering = ui_packets.scene_ot;
    model_head = mission_ot_span(packet, sizeof(*model_head));
    tag = *model_head;
    scene_entry = ui_scene_ot_span(ordering + 12, sizeof(*scene_entry));
    link = *scene_entry;
    *model_head = (tag & 0xFF000000) | (link & 0xFFFFFF);
    packet = game_runtime.mission_control.ot;
    ordering = ui_packets.scene_ot;
    scene_entry = ui_scene_ot_span(ordering + 12, sizeof(*scene_entry));
    link = *scene_entry;
    *scene_entry = (link & 0xFF000000) | ((packet + 252) & 0xFFFFFF);
    menu_model_apply_view(0);
    menu_model_draw_aircraft(1);
    result = game_input.pressed & 32;
    if (result)
    {
        result = game_input.held & 0x5F;
        if (!result)
        {
            static const uint8 slots[] = {8, 2, 1, 3, 11, 5};
            menu_state.header.phase = (uint8)(3);
            for (index = 0; index < 6; ++index)
                ui_animation_set(slots[index], 3, -1, -1);
            preview->exit_action = (uint8)(2);
            return 2;
        }
    }
    return result;
}

void menu_model_select_finish(void)
{
    AIRCRAFT_PREVIEW_STATE *preview = &menu_model_preview;
    uint32 value, packet, ordering, tag, link;
    uint32 *model_head, *scene_entry;
    FUNCTION_MARKER(0x801092BC, "ACE3.BIN");
    if (!ui_animation_phase(2))
    {
        value = preview->exit_action;
        if (value == 1)
        {
            menu_state.header.phase = (uint8)(4);
            {
                ui_group_set_mask(2, 0x200000);
                return;
            }
        }
        if (value == 2)
        {
            mission_menu_start_item_motion(0, 1);
            menu_state.header.kind = (uint8)(0);
            menu_state.header.phase = (uint8)(0);
            {
                arena_set_cursor(preview->model_buffer);
                return;
            }
        }
        return;
    }
    packet = game_runtime.mission_control.ot;
    ClearOTagR(mission_ot_span(packet, 256), 64);
    packet = game_runtime.mission_control.ot;
    ordering = ui_packets.scene_ot;
    model_head = mission_ot_span(packet, sizeof(*model_head));
    tag = *model_head;
    scene_entry = ui_scene_ot_span(ordering + 12, sizeof(*scene_entry));
    link = *scene_entry;
    *model_head = (tag & 0xFF000000) | (link & 0xFFFFFF);
    packet = game_runtime.mission_control.ot;
    ordering = ui_packets.scene_ot;
    scene_entry = ui_scene_ot_span(ordering + 12, sizeof(*scene_entry));
    link = *scene_entry;
    *scene_entry = (link & 0xFF000000) | ((packet + 252) & 0xFFFFFF);
    menu_model_apply_view(0);
    {
        menu_model_draw_aircraft(1);
        return;
    }
}

uint32 menu_model_detail_begin(void)
{
    RESOURCE_STREAM_BUFFER buffer_allocation;

    enum
    {
        DETAIL_MODEL_BUFFER_BYTES = 0x10000
    };

    AIRCRAFT_PREVIEW_STATE *preview = &menu_model_preview;
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    uint32 index, resource, sections[4], temporary, table, object, value;
    const uint32 *models;
    const uint8 *resource_data;
    uint32 resource_bytes;
    FUNCTION_MARKER(0x801093E8, "ACE3.BIN");
    index = preview->index;
    if (menu_model_preview_entry(index)->detail_resource == 0)
    {
        menu_state.header.phase = (uint8)(5);
        return scene_palette_restore(0);
    }
    value = game_runtime.resource_pending;
    if (value)
        return value;
    resource = game_runtime.resource_slot.encoded_base;
    resource_data = game_runtime.resource_slot.data;
    resource_bytes = game_runtime.resource_slot.read_bytes;
    preview->detail_bookmark = (uint32)(resource);
    mission_image_sections(resource, game_runtime.resource_slot.data, game_runtime.resource_slot.read_bytes, sections, 4);
    arena_set_cursor(sections[3]);
    scene_texture_load_group_sized(sections[2], 0x10000, (RESOURCE_ASSET_SOURCE){game_runtime.resource_slot.data, game_runtime.resource_slot.read_bytes, resource}, SCENE_TEXTURE_STANDARD);
    arena_set_cursor(sections[2]);
    buffer_allocation = arena_alloc_backed(DETAIL_MODEL_BUFFER_BYTES);
    temporary = buffer_allocation.encoded_begin;
    uint32 ulz_offset = sections[0] - resource;
    if (!resource_data || sections[0] < resource || ulz_offset > resource_bytes || resource_bytes - ulz_offset < 16)
        abort();
    const uint8 *ulz_input = resource_data + ulz_offset;
    uint8 *ulz_output = buffer_allocation.buffer.data;
    game_decompress_buffer(ulz_input, resource_bytes - ulz_offset, ulz_output, DETAIL_MODEL_BUFFER_BYTES);
    table = resource_relocate_directory(temporary, NULL, ulz_output, DETAIL_MODEL_BUFFER_BYTES);
    models = (const uint32 *)(ulz_output + 4);
    for (index = 0; index < AIRCRAFT_COMPOSITE_MODEL_COUNT; ++index)
    {
        object = models[index];
        if (object < temporary || object - temporary > DETAIL_MODEL_BUFFER_BYTES - 28)
            abort();
        relocate_pointer_array(object, ulz_output + (object - temporary) + 4, 6);
        detail_model_headers[index] = (const SCENE_MESH_MODEL_HEADER *)(ulz_output + (object - temporary));
    }
    if (sections[2] - sections[1] < 124)
        abort();
    if (!resource_data || sections[1] < resource || resource_bytes < 124 || sections[1] - resource > resource_bytes - 124)
        abort();
    const uint8 *metadata_data = resource_data + (sections[1] - resource);
    aircraft_bind_thrust_metadata(metadata_data, sections[2] - sections[1]);
    actor_index_table = 0;
    game_runtime.scene_assets.aircraft_headers = detail_model_headers;
    game_runtime.scene_assets.aircraft_model_source = (RESOURCE_ASSET_SOURCE){ulz_output, DETAIL_MODEL_BUFFER_BYTES, temporary};
    actor_traits_object = mission_actor_traits(actor_index_table);
    value = actor_traits_object->flags;
    actor_traits_object->flags = (uint32)(value | 0x1C000);
    actor_traits_object = mission_actor_traits((actor_index_table + 2));
    actor_traits_object->flags = (uint32)(0);
    menu_state.header.phase = (uint8)(5);
    return 5;
}

uint32 menu_model_detail_tick(void)
{
    uint32 packet, ordering, tag, link;
    uint32 *model_head, *scene_entry;
    FUNCTION_MARKER(0x80109588, "ACE3.BIN");
    packet = game_runtime.mission_control.ot;
    ClearOTagR(mission_ot_span(packet, 256), 64);
    packet = game_runtime.mission_control.ot;
    ordering = ui_packets.scene_ot;
    model_head = mission_ot_span(packet, sizeof(*model_head));
    tag = *model_head;
    scene_entry = ui_scene_ot_span(ordering + 12, sizeof(*scene_entry));
    link = *scene_entry;
    *model_head = (tag & 0xFF000000) | (link & 0xFFFFFF);
    ordering = ui_packets.scene_ot;
    packet = game_runtime.mission_control.ot;
    scene_entry = ui_scene_ot_span(ordering + 12, sizeof(*scene_entry));
    link = *scene_entry;
    *scene_entry = (link & 0xFF000000) | ((packet + 252) & 0xFFFFFF);
    if ((game_input.pressed & 32) && (game_input.held & 0x5F) == 0)
    {
        menu_state.header.phase = (uint8)(6);
        return 6;
    }
    menu_model_apply_view(1);
    return menu_model_draw_aircraft(0);
}

void menu_model_update_selection(uint32 mode)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;
    const AIRCRAFT_MODEL_RESOURCES *entries;
    uint8 *native_model_data;
    uint32 native_model_bytes, bundle_end;

    uint32 source, a, b, c, phase, index, resource, value;
    FUNCTION_MARKER(0x800F7994, "ACE3.BIN");
    if (ui_animation_phase(2) && mode == 1)
        return;
    phase = (sint16)mission_flow.transition_phase;
    if (phase != 3)
    {
        if (phase == 0)
        {
            if (!mission_aircraft.profile)
                abort();
            memmove(&mission_aircraft.saved_profile, mission_aircraft.profile, sizeof(AIRCRAFT_PROFILE));
            mission_aircraft.desc_saved = 1;
        }
        mission_aircraft.profile = &mission_aircraft.saved_profile;
        phase = (sint16)mission_flow.transition_phase;
        mission_dispatch_loading(phase, 1, (RESOURCE_STREAM_BUFFER){0});
        return;
    }
    a = mission_aircraft.variant;
    b = mission_flow.faction;
    c = (sint16)game_runtime.selection.mission;
    index = aircraft_selection_model_variant(c, a);
    if (index >= mission_flow.assets.aircraft_model_count)
        abort();
    resource = mission_flow.assets.aircraft_model_table[index];
    bundle_end = mission_flow.assets.aircraft_bundle_end;
    if (!mission_flow.assets.aircraft_profiles || resource > bundle_end || bundle_end - resource < 12 || bundle_end - resource > mission_flow.assets.aircraft_profile_bytes)
        abort();
    native_model_bytes = bundle_end - resource;
    native_model_data = mission_flow.assets.aircraft_profiles + (mission_flow.assets.aircraft_profile_bytes - native_model_bytes);
    (void)resource_relocate_directory(resource, NULL, native_model_data, native_model_bytes);
    entries = (const AIRCRAFT_MODEL_RESOURCES *)(native_model_data + 4);
    resource = entries->pal;
    scene_palette_capture(resource, (RESOURCE_ASSET_SOURCE){native_model_data, native_model_bytes, bundle_end - native_model_bytes});
    resource = entries->model;
    source = mission_aircraft.model_buffer;
    uint32 ulz_offset = resource - (bundle_end - native_model_bytes);
    if (!native_model_data || resource < (bundle_end - native_model_bytes) || ulz_offset > native_model_bytes || native_model_bytes - ulz_offset < 16)
        abort();
    const uint8 *ulz_input = native_model_data + ulz_offset;
    uint8 *ulz_output = mission_aircraft.model_data;
    if (!ulz_output)
        abort();
    game_decompress_buffer(ulz_input, native_model_bytes - ulz_offset, ulz_output, MISSION_AIRCRAFT_MODEL_BUFFER_BYTES);
    source = mission_aircraft.model_buffer;
    if (game_runtime.scene_assets.model_headers != aircraft_preview_model_headers || game_runtime.scene_assets.model_header_count != 64)
        abort();
    aircraft_preview_model_headers[8] = (const SCENE_MESH_MODEL_HEADER *)ulz_output;
    aircraft_preview_model_sources[8] = (RESOURCE_ASSET_SOURCE){ulz_output, MISSION_AIRCRAFT_MODEL_BUFFER_BYTES, source};
    source = mission_aircraft.model_buffer;
    relocate_pointer_array(source, ulz_output + 4, 6);
    actor_index_table = 0;
    actor_traits_object = mission_actor_traits(actor_index_table);
    actor_traits_object->flags = (uint32)(0);
    actor_traits_object = mission_actor_traits((actor_index_table + 2));
    value = actor_traits_object->flags;
    actor_traits_object->flags = (uint32)(value | 0x1C000);
    ui_animation_set(2, 1, 0, 25);
    ui_animation_set(8, 1, 0, 25);
    value = mission_aircraft.desc_saved;
    if (value)
    {
        mission_aircraft.prev_profile = &mission_aircraft.saved_profile;
        mission_aircraft.profile = &mission_aircraft.saved_profile;
        mission_aircraft.desc_saved = 0;
    }
    else
    {
        mission_aircraft.prev_profile = mission_aircraft.profile;
    }
    mission_aircraft.profile = aircraft_profile_at(index);
    return;
}

void menu_model_initialize_lighting(void)
{
    SVECTOR angles = {0, 0, 0, 0};
    SVECTOR vector = {0, 0, 4096, 0};
    MATRIX matrix = {0};
    FUNCTION_MARKER(0x800FC37C, "ACE3.BIN");
    game_runtime.mission_lighting.colors.m[0][0] = (sint16)(uint16)(2048);
    game_runtime.mission_lighting.colors.m[1][0] = (sint16)(uint16)(2048);
    game_runtime.mission_lighting.colors.m[2][0] = (sint16)(uint16)(2048);
    game_runtime.mission_lighting.colors.m[0][1] = (sint16)(uint16)(0);
    game_runtime.mission_lighting.colors.m[1][1] = (sint16)(uint16)(0);
    game_runtime.mission_lighting.colors.m[2][1] = (sint16)(uint16)(0);
    game_runtime.mission_lighting.colors.m[0][2] = (sint16)(uint16)(0);
    game_runtime.mission_lighting.colors.m[1][2] = (sint16)(uint16)(0);
    game_runtime.mission_lighting.colors.m[2][2] = (sint16)(uint16)(0);
    game_runtime.mission_lighting.directions.m[0][0] = (sint16)(uint16)(0);
    game_runtime.mission_lighting.directions.m[0][1] = (sint16)(uint16)(0);
    game_runtime.mission_lighting.directions.m[0][2] = (sint16)(uint16)(0);
    game_runtime.mission_lighting.directions.m[1][0] = (sint16)(uint16)(0);
    game_runtime.mission_lighting.directions.m[1][1] = (sint16)(uint16)(0);
    game_runtime.mission_lighting.directions.m[1][2] = (sint16)(uint16)(0);
    game_runtime.mission_lighting.directions.m[2][0] = (sint16)(uint16)(0);
    game_runtime.mission_lighting.directions.m[2][1] = (sint16)(uint16)(0);
    game_runtime.mission_lighting.directions.m[2][2] = (sint16)(uint16)(0);
    game_runtime.mission_lighting.back_color[0] = (uint32)(84);
    game_runtime.mission_lighting.back_color[1] = (uint32)(84);
    game_runtime.mission_lighting.back_color[2] = (uint32)(84);
    RotMatrixYXZ(&angles, &matrix);
    ApplyMatrixSV(&matrix, &vector, (SVECTOR *)game_runtime.mission_lighting.directions.m[0]);
    angles.vx = 0;
    angles.vy = 0;
    angles.vz = 0;
    RotMatrixYXZ(&angles, &matrix);
    ApplyMatrixSV(&matrix, &vector, (SVECTOR *)game_runtime.mission_lighting.directions.m[1]);
    return;
}

void menu_model_initialize_camera(void)
{
    const uint16 angles[3] = {0};
    FUNCTION_MARKER(0x800FC488, "ACE3.BIN");
    camera_initialize(200, 8000, 28000, angles);
    menu_model_initialize_lighting();
    game_runtime.camera_view.pose.position[0] = (uint32)(0);
    game_runtime.camera_view.pose.position[1] = (uint32)(0xFFE00000);
    game_runtime.camera_view.pose.position[2] = (uint32)(0);
    game_runtime.camera_view.pose.angles[0] = (uint16)(0xF000);
    game_runtime.camera_view.pose.angles[1] = (uint16)(0);
    game_runtime.camera_view.pose.angles[2] = (uint16)(0);
    game_runtime.mission_matrices.camera.t[2] = (sint32)(uint32)(0);
    game_runtime.mission_matrices.camera.t[1] = (sint32)(uint32)(0);
    game_runtime.mission_matrices.camera.t[0] = (sint32)(uint32)(0);
    game_runtime.mission_matrices.view.t[2] = (sint32)(uint32)(0);
    game_runtime.mission_matrices.view.t[1] = (sint32)(uint32)(0);
    game_runtime.mission_matrices.view.t[0] = (sint32)(uint32)(0);
    rotation_build(&game_runtime.mission_matrices.camera, game_runtime.camera_view.pose.angles);
    TransposeMatrix(&game_runtime.mission_matrices.camera, &game_runtime.mission_matrices.view);
    matrix_halve_xz(&game_runtime.mission_matrices.view);
    SetGeomOffset(0, 0);
    SetGeomScreen(((sint16)game_runtime.camera_proj.distance));
    return;
}

void aircraft_preview_initialize(void)
{
    uint32 value, index, frame, target;
    FUNCTION_MARKER(0x800FC148, "ACE3.BIN");
    scene_palette_begin();
    value = arena_alloc(256);
    target = (uint32)scene_palette.rectangle.h << 5;
    value = arena_alloc(target);
    for (index = 0; index < 2; ++index)
    {
        (void)arena_alloc(256);
        (void)arena_alloc(18432);
        mission_bind_packet_bank(index, 64, 18432);
    }
    frame = game_runtime.display_buffer;
    value = game_runtime.mission_control.banks[frame].ot;
    game_runtime.mission_control.ot = (uint32)(value);
    frame = game_runtime.display_buffer;
    value = game_runtime.mission_control.banks[frame].packet_begin;
    game_runtime.mission_control.packet_cur = (uint32)(value);
    value = arena_alloc(256);
    for (index = 0; index < 2; ++index)
    {
        value = arena_alloc(88);
        mission_actor_traits(((uint32)((sint32)(index * 4) >> 2)))->flags = 0;
    }
    for (index = 2; index < 32; ++index)
    {
        value = arena_alloc(92);
        mission_actor_traits(((uint32)((sint32)(index * 4) >> 2)))->flags = 0;
    }
    for (index = 0; index < 32; ++index)
    {
        mission_actor_traits(((uint32)((sint32)(index * 4) >> 2)))->render_flags = (uint8)(0);
        mission_actor_traits(((uint32)((sint32)(index * 4) >> 2)))->parameter = (uint16)(0);
    }
    for (index = 0; index < 32; ++index)
    {
        value = arena_alloc(148);
        mission_actor_traits((32 + ((uint32)((sint32)(index * 4) >> 2))))->flags = 0;
    }
    value = arena_alloc(4);
    game_runtime.scene_assets.aircraft_headers = NULL;
    game_runtime.scene_assets.aircraft_model_source = (RESOURCE_ASSET_SOURCE){0};
    game_runtime.scene_assets.model_headers = aircraft_preview_model_headers;
    game_runtime.scene_assets.model_header_sources = aircraft_preview_model_sources;
    game_runtime.scene_assets.model_header_count = 64;
    for (index = 0; index < 64; ++index)
    {
        value = arena_alloc(28);
        aircraft_preview_model_headers[index] = NULL;
        aircraft_preview_model_sources[index] = (RESOURCE_ASSET_SOURCE){0};
    }
    return;
}

void menu_model_draw_selection_preview(void)
{
    FUNCTION_MARKER(0x800F7BE8, "ACE3.BIN");
    if (!ui_animation_phase(2))
        return;
    menu_model_apply_view(2);
    menu_model_draw_aircraft(1);
    return;
}
