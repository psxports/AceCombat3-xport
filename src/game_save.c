#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "game_functions.h"
#include "game_save_format.h"

GAME_CARD_STATE title_card;
GAME_CARD_STATE mission_card;

void game_card_reset_directory(uint8 *header)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    (wire_word_0 = (0x990426), (void)memcpy(header + 4, &wire_word_0, sizeof(wire_word_0)));
    header[8] = 0;
    header[9] = 0;
    memset(header + 16, 0, 112);
    (wire_word_1 = (0), (void)memcpy(header + 12, &wire_word_1, sizeof(wire_word_1)));
}

void game_card_header_initialize(uint8 *header)
{
    static const uint8 prefix[] = {
        0x53, 0x43, 0x13, 0x02, 0x83, 0x47, 0x81, 0x5B, 0x83, 0x58, 0x83, 0x52, 0x83, 0x93, 0x83, 0x6F, 0x83, 0x62, 0x83, 0x67, 0x82, 0x52, 0x81, 0x40, 0x83, 0x47, 0x83, 0x8C, 0x83, 0x4E, 0x83, 0x67, 0x83, 0x8D, 0x83, 0x58, 0x83, 0x74, 0x83, 0x42, 0x83, 0x41, 0x00,
    };
    static const uint8 suffix[] = {
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x43, 0x52, 0x44, 0x30, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    };

    memcpy(header, prefix, sizeof(prefix));
    memcpy(header + 68, suffix, sizeof(suffix));
}

void game_card_header_write_assets(uint8 *header, const uint8 *const *icons, uint32 count, const uint8 *palette)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint32 wire_word_3;
    uint32 wire_word_4;
    uint32 wire_word_5;
    uint32 image, index;
    const uint8 *source = icons[0] + 20;

    for (index = 0; index < 32; index += 4)
        (wire_word_1 = ((memcpy(&wire_word_0, source + index, sizeof(wire_word_0)), wire_word_0)), (void)memcpy(header + 96 + index, &wire_word_1, sizeof(wire_word_1)));
    for (image = 0; image < count; ++image)
    {
        source = icons[image] + 64;
        for (index = 0; index < 128; index += 4)
            (wire_word_3 = ((memcpy(&wire_word_2, source + index, sizeof(wire_word_2)), wire_word_2)), (void)memcpy(header + 128 + image * 128 + index, &wire_word_3, sizeof(wire_word_3)));
    }
    source = palette;
    for (index = 0; index < 128; index += 4)
        (wire_word_5 = ((memcpy(&wire_word_4, source + index, sizeof(wire_word_4)), wire_word_4)), (void)memcpy(header + 512 + index, &wire_word_5, sizeof(wire_word_5)));
}

static uint32 game_save_checksum(const uint8 *data, uint32 bytes)
{
    uint32 hash = 2166136261u, index;
    for (index = 0; index < bytes; ++index)
        hash = (hash ^ data[index]) * 16777619u;
    return hash;
}

sint32 game_account_save(void)
{
    GAME_SAVE_FILE_V1 data;
    uint32 index, field;
    for (index = 0; index < 6; ++index)
    {
        const PAD_PROFILE *profile = &pad_profiles[index];
        /* Native version 1 stores original offsets 22..89, unlike card records */
        for (field = 0; field < 21; ++field)
            data.payload.profile[index].binding[field] = profile->binding[field + 11];
        for (field = 0; field < 4; ++field)
        {
            data.payload.profile[index].cal[field].low = profile->cal[field].low;
            data.payload.profile[index].cal[field].center = profile->cal[field].center;
            data.payload.profile[index].cal[field].high = profile->cal[field].high;
            data.payload.profile[index].cal[field].dead_zone = profile->cal[field].dead_zone;
        }
        data.payload.profile[index].axis_flags = profile->axis_flags;
        for (field = 0; field < 6; ++field)
            data.payload.profile[index].axis_map[field] = profile->axis_map[field];
        data.payload.profile[index].flags = (uint16)profile->flags;
    }
    data.payload.mission = game_runtime.selection.mission;
    data.payload.aircraft = game_runtime.selection.aircraft;
    data.payload.saved_mission = game_runtime.selection.saved_mission;
    data.payload.saved_aircraft = game_runtime.selection.saved_aircraft;
    data.payload.difficulty = game_runtime.selection.difficulty;
    data.payload.volume[0] = audio_settings.volume[0];
    data.payload.volume[1] = audio_settings.volume[1];
    data.payload.audio_flags = audio_settings.flags;
    data.payload.options_reserved = game_runtime.options.save_reserved;
    data.payload.captions = game_runtime.options.captions;
    data.payload.kill_camera = game_runtime.options.kill_camera;
    data.payload.hud_hidden = game_runtime.options.hud_hidden;
    data.payload.camera_mode = game_runtime.options.camera_mode;
    data.payload.return_mode = game_runtime.playback.return_mode;
    data.payload.intro_count = game_runtime.playback.intro_count;
    data.payload.demo_mode = game_runtime.playback.demo_mode;
    data.payload.demo_idx = game_runtime.playback.demo_idx;
    data.payload.caption_gate = game_runtime.playback.caption_gate;
    data.payload.movie_idx = game_runtime.playback.movie_idx;
    for (field = 0; field < 3; ++field)
        data.payload.weapon_code[field] = game_runtime.selection.weapon_code[field];
    memcpy(data.payload.account.name, game_runtime.account.name, 12);
    data.payload.account.route_mode = game_runtime.account.route_mode;
    data.payload.account.result_kind = game_runtime.account.result_kind;
    data.payload.account.mission_failed = game_runtime.account.mission_failed;
    data.payload.loss_count = game_runtime.account.loss_count;
    data.payload.hold_action_gate = game_runtime.account.hold_action_gate;
    data.payload.account_reserved = game_runtime.account.save_reserved;
    data.payload.record_reserved = game_runtime.progress.record_reserved;
    data.payload.record_slot = game_runtime.progress.record_slot;
    data.payload.sequence_idx = game_runtime.progress.sequence_idx;
    data.payload.movie_branch = game_runtime.progress.movie_branch;
    memcpy(data.payload.progress_reserved, game_runtime.progress.save_reserved, 4);
    data.payload.account_flags = game_runtime.progress.account_flags;
    data.payload.account_id = game_runtime.progress.account_id;
    data.payload.flow_flags = game_runtime.progress.flow_flags;
    data.payload.save_reserved_4 = game_runtime.progress.save_reserved[4];
    data.payload.menu_reserved = game_runtime.progress.menu_reserved;
    data.payload.save_reserved_5 = game_runtime.progress.save_reserved[5];
    data.payload.save_reserved_6 = game_runtime.progress.save_reserved[6];
    memcpy(data.payload.aircraft_unlocked, game_runtime.progress.aircraft_unlocked, 64);
    memcpy(data.payload.weapon_unlocked, game_runtime.progress.weapon_unlocked, 16);
    for (index = 0; index < 5; ++index)
        for (field = 0; field < GAME_MISSION_COUNT; ++field)
        {
            data.payload.mission_time[index][field] = game_runtime.progress.mission_time[index][field];
        }
    memcpy(data.payload.mission_rating, game_runtime.progress.mission_rating, 5 * GAME_MISSION_COUNT);
    memcpy(data.payload.archive_status, game_runtime.progress.archive_status, 80);
    memcpy(data.payload.scene_status, game_runtime.progress.scene_status, 180);
    for (field = 0; field < GAME_MISSION_COUNT; ++field)
    {
        data.payload.mission_count[field] = game_runtime.progress.mission_count[field];
    }
    for (field = 0; field < GAME_MISSION_COUNT; ++field)
    {
        data.payload.mission_losses[field] = game_runtime.progress.mission_losses[field];
    }
    data.payload.total_time = game_runtime.progress.total_time;
    data.payload.total_count = game_runtime.progress.total_count;
    data.payload.total_losses = game_runtime.progress.total_losses;
    data.payload.screen_x = game_runtime.display.screen[0];
    data.payload.screen_y = game_runtime.display.screen[1];
    data.payload.input_flags = game_input.flags & 3;
    data.magic = 0x53334341;
    data.version = 1;
    data.payload_bytes = GAME_SAVE_PAYLOAD;
    data.checksum = game_save_checksum((const uint8 *)&data.payload, GAME_SAVE_PAYLOAD);
    if (xport_save(&data, sizeof(data)) == 1)
    {
#ifndef NDEBUG
        fprintf(stderr, "AC3 account saved: %u bytes\n", (uint32)sizeof(data));
#endif
        return 1;
    }
    xport_message_error("Ace Combat 3 - Save error", "The account could not be saved. Please try again.");
    return 0;
}

sint32 game_account_load(void)
{
    GAME_SAVE_FILE_V1 data;
    sint32 bytes = xport_load(&data, sizeof(data));
    uint32 index, field;
    if (!bytes)
    {
#ifndef NDEBUG
        fprintf(stderr, "AC3 account load: no save\n");
#endif
        return 0;
    }
    if (bytes != sizeof(data) || data.magic != 0x53334341 || data.version != 1 || data.payload_bytes != GAME_SAVE_PAYLOAD || data.checksum != game_save_checksum((const uint8 *)&data.payload, GAME_SAVE_PAYLOAD) || data.payload.mission >= GAME_MISSION_COUNT || data.payload.saved_mission >= GAME_MISSION_COUNT || data.payload.aircraft >= GAME_AIRCRAFT_COUNT || data.payload.saved_aircraft >= GAME_AIRCRAFT_COUNT || data.payload.demo_idx >= GAME_DEMO_COUNT || data.payload.difficulty > 4 || data.payload.hold_action_gate > 1 || data.payload.kill_camera > 1 || !data.payload.account.name[0] || !memchr(data.payload.account.name, 0, 9) || (sint32)data.payload.screen_x < -12 || (sint32)data.payload.screen_x > 12 || data.payload.screen_y > 8 || data.payload.input_flags > 3)
    {
        xport_message_error("Ace Combat 3 - Load error", "The account could not be loaded. Create a new account to continue.");
        return 0;
    }
    for (index = 0; index < 6; ++index)
    {
        PAD_PROFILE *profile = &pad_profiles[index];
        for (field = 0; field < 21; ++field)
            profile->binding[field + 11] = data.payload.profile[index].binding[field];
        for (field = 0; field < 4; ++field)
        {
            profile->cal[field].low = data.payload.profile[index].cal[field].low;
            profile->cal[field].center = data.payload.profile[index].cal[field].center;
            profile->cal[field].high = data.payload.profile[index].cal[field].high;
            profile->cal[field].dead_zone = data.payload.profile[index].cal[field].dead_zone;
        }
        profile->axis_flags = data.payload.profile[index].axis_flags;
        for (field = 0; field < 6; ++field)
            profile->axis_map[field] = data.payload.profile[index].axis_map[field];
        profile->flags = (profile->flags & 0xFFFF0000) | data.payload.profile[index].flags;
    }
    game_runtime.selection.mission = data.payload.mission;
    game_runtime.selection.aircraft = data.payload.aircraft;
    game_runtime.selection.saved_mission = data.payload.saved_mission;
    game_runtime.selection.saved_aircraft = data.payload.saved_aircraft;
    game_runtime.selection.difficulty = data.payload.difficulty;
    audio_settings.volume[0] = data.payload.volume[0];
    audio_settings.volume[1] = data.payload.volume[1];
    audio_settings.flags = data.payload.audio_flags;
    game_runtime.options.save_reserved = data.payload.options_reserved;
    game_runtime.options.captions = data.payload.captions;
    game_runtime.options.kill_camera = data.payload.kill_camera;
    game_runtime.options.hud_hidden = data.payload.hud_hidden;
    game_runtime.options.camera_mode = data.payload.camera_mode;
    game_runtime.playback.return_mode = data.payload.return_mode;
    game_runtime.playback.intro_count = data.payload.intro_count;
    game_runtime.playback.demo_mode = data.payload.demo_mode;
    game_runtime.playback.demo_idx = data.payload.demo_idx;
    game_runtime.playback.caption_gate = data.payload.caption_gate;
    game_runtime.playback.movie_idx = data.payload.movie_idx;
    for (field = 0; field < 3; ++field)
        game_runtime.selection.weapon_code[field] = data.payload.weapon_code[field];
    memcpy(game_runtime.account.name, data.payload.account.name, 12);
    game_runtime.account.route_mode = data.payload.account.route_mode;
    game_runtime.account.result_kind = data.payload.account.result_kind;
    game_runtime.account.mission_failed = data.payload.account.mission_failed;
    game_runtime.account.loss_count = data.payload.loss_count;
    game_runtime.account.hold_action_gate = data.payload.hold_action_gate;
    game_runtime.account.save_reserved = data.payload.account_reserved;
    game_runtime.progress.record_reserved = data.payload.record_reserved;
    game_runtime.progress.record_slot = data.payload.record_slot;
    game_runtime.progress.sequence_idx = data.payload.sequence_idx;
    game_runtime.progress.movie_branch = data.payload.movie_branch;
    memcpy(game_runtime.progress.save_reserved, data.payload.progress_reserved, 4);
    game_runtime.progress.account_flags = data.payload.account_flags;
    game_runtime.progress.account_id = data.payload.account_id;
    game_runtime.progress.flow_flags = data.payload.flow_flags;
    game_runtime.progress.save_reserved[4] = data.payload.save_reserved_4;
    game_runtime.progress.menu_reserved = data.payload.menu_reserved;
    game_runtime.progress.save_reserved[5] = data.payload.save_reserved_5;
    game_runtime.progress.save_reserved[6] = data.payload.save_reserved_6;
    memcpy(game_runtime.progress.aircraft_unlocked, data.payload.aircraft_unlocked, 64);
    memcpy(game_runtime.progress.weapon_unlocked, data.payload.weapon_unlocked, 16);
    for (index = 0; index < 5; ++index)
        for (field = 0; field < GAME_MISSION_COUNT; ++field)
        {
            game_runtime.progress.mission_time[index][field] = data.payload.mission_time[index][field];
        }
    memcpy(game_runtime.progress.mission_rating, data.payload.mission_rating, 5 * GAME_MISSION_COUNT);
    memcpy(game_runtime.progress.archive_status, data.payload.archive_status, 80);
    memcpy(game_runtime.progress.scene_status, data.payload.scene_status, 180);
    for (field = 0; field < GAME_MISSION_COUNT; ++field)
    {
        game_runtime.progress.mission_count[field] = data.payload.mission_count[field];
    }
    for (field = 0; field < GAME_MISSION_COUNT; ++field)
    {
        game_runtime.progress.mission_losses[field] = data.payload.mission_losses[field];
    }
    game_runtime.progress.total_time = data.payload.total_time;
    game_runtime.progress.total_count = data.payload.total_count;
    game_runtime.progress.total_losses = data.payload.total_losses;
    game_runtime.display.screen[0] = (uint32)(data.payload.screen_x);
    game_runtime.display.screen[1] = (uint32)(data.payload.screen_y);
    game_runtime.display.flags = (uint32)(game_runtime.display.flags | 2);
    game_input.flags = (game_input.flags & ~3u) | data.payload.input_flags;
    game_runtime.account.route_mode = (uint16)(2);
    game_runtime.progress.flow_flags = (uint32)(game_runtime.progress.flow_flags & ~0x10u);
    resource_set_mono((audio_settings.flags & 1) != 0);
    resource_apply_mix();
#ifndef NDEBUG
    fprintf(stderr, "AC3 account restored: %u bytes\n", (uint32)bytes);
#endif
    return 1;
}

uint32 game_account_save_requested(void)
{
    uint32 mode = mission_flow.account_mode;
    sint32 mission;
    game_account_save();
    ui_group_bind(ui_group_directory(0), 0);
    mission_flow.mode = 7;
    mission_flow.mode_phase = 0;
    if (mode == 0)
    {
        mission = (sint16)game_runtime.selection.mission;
        if (mission == 20 || mission == 35 || mission == 40 || mission == 45)
        {
            game_runtime.account.route_mode = (uint16)(2);
            game_runtime.progress.flow_flags = (uint32)(game_runtime.progress.flow_flags | 1);
            mission_flow.next_scene = 0x60000003;
        }
        else
            game_runtime.progress.flow_flags = (uint32)(game_runtime.progress.flow_flags & ~1u);
    }
    game_runtime.progress.flow_flags = (uint32)(game_runtime.progress.flow_flags & ~8u);
    return 1;
}

void menu_load_card_header_assets(const uint8 *data, uint32 data_bytes, uint32 icons, uint32 palette)
{
    uint32 pointers[4], image;
    const uint8 *icon_data[4];
    uint8 *header = mission_card.icon_block;
    sint32 count;

    FUNCTION_MARKER(0x800E38DC, "ACE3.BIN");
    arena_alloc(GAME_CARD_ICON_BLOCK_BYTES);
    game_card_header_initialize(header);
    if (!data || icons > data_bytes || 5 * sizeof(uint32) > data_bytes - icons)
        abort();
    const uint32 *image_directory = (const uint32 *)(data + icons);
    count = (sint32)mission_image_relocate_entries(image_directory, icons, pointers, 4);
    if (count <= 0)
    {
#ifndef NDEBUG
        fprintf(stderr, "Resource2 save header requires a defined first relocation\n");
#endif
        abort();
    }
    header[2] = (uint8)(count + 16);
    for (image = 0; image < (uint32)count; ++image)
    {
        if (pointers[image] > data_bytes || 192 > data_bytes - pointers[image])
            abort();
        icon_data[image] = data + pointers[image];
    }
    if (palette > data_bytes || 128 > data_bytes - palette)
        abort();
    game_card_header_write_assets(header, icon_data, (uint32)count, data + palette);
    return;
}

static uint32 title_card_image_offsets(const uint8 *image_directory, uint32 base, uint32 *native_output, uint32 native_capacity)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    const uint8 *cursor;
    uint32 index, value, adjusted;
    sint32 count;

    FUNCTION_MARKER(0x800CCE38, "ACE3.BIN");
    count = (sint32)(memcpy(&wire_word_0, image_directory, sizeof(wire_word_0)), wire_word_0);
    index = 0;
    cursor = image_directory;
    if (count > 0)
    {
        do
        {
            if (index >= native_capacity)
            {
#ifndef NDEBUG
                fprintf(stderr, "Resource001 icon pointer output exceeds native capacity\n");
#endif
                abort();
            }
            value = (memcpy(&wire_word_1, cursor + 4, sizeof(wire_word_1)), wire_word_1);
            adjusted = value;
            if ((sint32)value < 0)
                adjusted += 3;
            adjusted = (uint32)((sint32)adjusted >> 2) << 2;
            value = base + adjusted;
            native_output[index] = value;
            value = (memcpy(&wire_word_2, image_directory, sizeof(wire_word_2)), wire_word_2);
            ++index;
            cursor += 4;
        } while ((sint32)index < (sint32)value);
    }
    return (uint32)count;
}

void title_load_card_header_assets(const uint8 *data, uint32 data_bytes, uint32 icon_offset, uint32 palette)
{
    uint32 icons[4];
    uint32 count, directory, image;
    const uint8 *icon_data[4];
    uint8 *header = title_card.icon_block;

    FUNCTION_MARKER(0x800D4FF0, "ACE3.BIN");
    arena_alloc(GAME_CARD_ICON_BLOCK_BYTES);
    game_card_header_initialize(header);
    directory = icon_offset;
    if (!data || directory > data_bytes || 5 * sizeof(uint32) > data_bytes - directory)
        abort();
    count = title_card_image_offsets(data + directory, directory, icons, 4);
    header[2] = (uint8)(count + 16);
    if ((sint32)count <= 0 || count > 4)
    {
#ifndef NDEBUG
        fprintf(stderr, "Unsupported resource001 card icon count %u\n", count);
#endif
        abort();
    }
    for (image = 0; image < count; ++image)
    {
        if (icons[image] > data_bytes || 192 > data_bytes - icons[image])
            abort();
        icon_data[image] = data + icons[image];
    }
    if (palette > data_bytes || 128 > data_bytes - palette)
        abort();
    game_card_header_write_assets(header, icon_data, count, data + palette);
    return;
}

uint32 game_card_open_account(uint32 port, uint32 mode)
{
    if (port >= sizeof(ui_card_account_paths) / sizeof(ui_card_account_paths[0]))
        abort();
    return (uint32)openPSX(ui_card_account_paths[port], mode);
}

void title_card_initialize(void)
{
    GAME_CARD_POLL *poll = &title_card.poll;
    GAME_CARD_IO *io = &title_card.io;
    uint32 index;

    FUNCTION_MARKER(0x800D5550, "ACE3.BIN");
    io->retry_count = (uint16)(0);
    io->phase = (uint16)(0);
    title_card_reset_directory();
    title_card_initialize_accounts();
    title_card_initialize_records_hook();
    title_card_begin_polling();
    io->pending = (uint8)(0);
    io->record_index = (uint16)(0);
    for (index = 0; index < 2; ++index)
    {
        poll->ports[index].info_status = (uint16)(2);
        poll->ports[index].load_status = (uint16)(2);
    }
    return;
}

void title_card_close_events(void)
{
    GAME_CARD_POLL *poll = &title_card.poll;
    uint32 handle, index;

    FUNCTION_MARKER(0x800D58B0, "ACE3.BIN");
    bios_enter_critical();
    for (index = 0; index < 7; ++index)
    {
        handle = poll->events[index];
        CloseEventPSX(handle);
    }
    bios_exit_critical();
}

sint32 title_card_clear_info_events(void)
{
    GAME_CARD_POLL *poll = &title_card.poll;
    uint32 handle, index;
    sint32 result;

    FUNCTION_MARKER(0x800D3F88, "ACE3.BIN");
    for (index = 0; index < 4; ++index)
    {
        handle = poll->events[index];
        result = TestEvent((sint32)handle);
    }
    return result;
}

sint32 title_card_clear_write_events(void)
{
    GAME_CARD_POLL *poll = &title_card.poll;
    uint32 handle, index;
    sint32 result;

    FUNCTION_MARKER(0x800D3FF4, "ACE3.BIN");
    for (index = 0; index < 4; ++index)
    {
        handle = index == 3 ? 0 : poll->events[index + 4];
        result = TestEvent((sint32)handle);
    }
    return result;
}

sint32 title_card_poll_info_event(uint32 port, uint32 next_step)
{
    GAME_CARD_POLL *poll = &title_card.poll;
    uint32 handle, index;

    FUNCTION_MARKER(0x800D3EA8, "ACE3.BIN");
    if (port >= 2)
        abort();
    for (index = 0; index < 4; ++index)
    {
        handle = poll->events[index];
        if (TestEvent((sint32)handle) == 1)
        {
            poll->ports[port].step = (uint8)(next_step);
            return index;
        }
    }
    return -1;
}

sint32 title_card_poll_write_event(uint32 port, uint32 next_step)
{
    GAME_CARD_POLL *poll = &title_card.poll;
    uint32 handle, index;

    FUNCTION_MARKER(0x800D4060, "ACE3.BIN");
    if (port >= 2)
        abort();
    for (index = 0; index < 4; ++index)
    {
        handle = index == 3 ? 0 : poll->events[index + 4];
        if (TestEvent((sint32)handle) == 1)
        {
            poll->ports[port].step = (uint8)(next_step);
            return index;
        }
    }
    return -1;
}

void title_card_poll_tick(void)
{
    GAME_CARD_POLL *poll = &title_card.poll;
    GAME_CARD_IO *io = &title_card.io;
    uint32 port, step, value, result, flags;
    sint32 status;

    FUNCTION_MARKER(0x800D5F6C, "ACE3.BIN");
    port = poll->next_port;
    if (port >= 2)
        abort();
    step = poll->ports[port].step;
    switch (step)
    {
        case 0:
            goto toggle_port;
        case 1:
            title_card_clear_info_events();
            _card_info((sint32)(port << 4));
            poll->ports[port].step = (uint8)(2);
            poll->timeout = (uint32)(300);
            return;
        case 2:
            result = title_card_poll_info_event(port, 3);
            poll->ports[port].event_result = (uint8)(result);
            value = poll->timeout;
            if ((sint32)value < 0)
            {
                poll->ports[port].step = (uint8)(1);
                return;
            }
            result = value - 1;
            poll->timeout = (uint32)(result);
            return;
        case 3:
            value = poll->ports[port].event_result;
            poll->ports[port].info_status = (uint16)(value);
            if (value == 0)
            {
                if (poll->ports[port].present == 1)
                {
                    flags = poll->enabled;
                    poll->ports[port].step = (uint8)(flags != 0);
                    goto toggle_port;
                }
                title_card_clear_write_events();
                _new_card();
                psx_bios_card_write_guest(port << 4, 63, 0);
                poll->ports[port].step = (uint8)(4);
                return;
            }
            if (value == 1 || value == 2)
            {
                flags = poll->ports[port].probe_retries;
                if (flags >= 17)
                    poll->ports[port].present = (uint8)(1);
                else
                {
                    poll->ports[port].present = (uint8)(0);
                    flags = poll->ports[port].probe_retries;
                    poll->ports[port].probe_retries = (uint8)(flags + 1);
                }
                flags = poll->enabled;
                poll->ports[port].step = (uint8)(value == 1 ? flags == 1 : flags != 0);
                if (value == 1)
                    goto toggle_port;
                flags = poll->next_port;
                result = 1 - flags;
                poll->next_port = (uint32)(result);
                poll->ports[port].file_found = (uint8)(0);
                return;
            }
            if (value == 3)
            {
                poll->ports[port].info_status = (uint16)(0);
                title_card_clear_write_events();
                _new_card();
                psx_bios_card_write_guest(port << 4, 63, 0);
                poll->ports[port].step = (uint8)(4);
                poll->ports[port].present = (uint8)(0);
                poll->ports[port].probe_retries = (uint8)(0);
                return;
            }
            poll->ports[port].present = (uint8)(1);
            poll->ports[port].step = (uint8)(1);
            goto toggle_port;
        case 4:
            do
            {
                result = title_card_poll_write_event(port, 5);
                poll->ports[port].event_result = (uint8)(result);
                result = poll->ports[port].event_result;
            } while (result == 255);
            return;
        case 5:
            value = poll->ports[port].event_result;
            poll->ports[port].info_status = (uint16)(value);
            title_card_clear_info_events();
            psx_bios_card_load_guest(port << 4);
            poll->ports[port].step = (uint8)(6);
            return;
        case 6:
            result = title_card_poll_info_event(port, 7);
            poll->ports[port].event_result = (uint8)(result);
            return;
        case 7:
            value = poll->ports[port].event_result;
            poll->ports[port].load_status = (uint16)(value);
            poll->ports[port].present = (uint8)(1);
            status = (sint16)poll->ports[port].load_status;
            if (status < 0)
                return;
            if (status == 0)
            {
                poll->ports[port].step = (uint8)(8);
                return;
            }
            if (status <= 2)
            {
                value = poll->ports[port].probe_retries;
                if (value >= 17)
                    poll->ports[port].present = (uint8)(1);
                else
                {
                    poll->ports[port].present = (uint8)(0);
                    value = poll->ports[port].probe_retries;
                    poll->ports[port].probe_retries = (uint8)(value + 1);
                }
                poll->ports[port].step = (uint8)(1);
                return;
            }
            if (status != 3)
                return;
            poll->ports[port].present = (uint8)(1);
            poll->ports[port].step = (uint8)(1);
            goto toggle_port;
        case 8:
            result = game_card_open_account(port, 0x8001);
            io->descriptor = (uint32)(result);
            if (result != 0xFFFFFFFF)
                poll->ports[port].file_found = (uint8)(1);
            value = io->descriptor;
            game_bios_close_guest(value);
            poll->ports[port].step = (uint8)(1);
            poll->ports[port].present = (uint8)(1);
            goto toggle_port;
        default:
            return;
    }
toggle_port:
    flags = poll->next_port;
    result = 1 - flags;
    poll->next_port = (uint32)(result);
}

uint32 title_card_stop_polling(void)
{
    GAME_CARD_POLL *poll = &title_card.poll;
    uint32 step;

    FUNCTION_MARKER(0x800D3D60, "ACE3.BIN");
    step = poll->ports[0].step;
    poll->enabled = (uint32)(0);
    if (step != 0)
        return 0;
    return poll->ports[1].step == 0;
}

void title_card_open_events(void)
{
    GAME_CARD_POLL *poll = &title_card.poll;
    static const uint32 specifications[7] = {4, 0x8000, 0x100, 0x2000, 4, 0x8000, 0x100};
    uint32 index, handle, first_handle;

    FUNCTION_MARKER(0x800D56E0, "ACE3.BIN");
    bios_enter_critical();
    for (index = 0; index < 6; ++index)
    {
        handle = OpenEventPSX(index < 4 ? 0xF4000001 : 0xF0000011, specifications[index], 0x2000, 0);
        poll->events[index] = (uint32)(handle);
    }
    handle = OpenEventPSX(0xF0000011, specifications[6], 0x2000, 0);
    first_handle = poll->events[0];
    poll->events[6] = (uint32)(handle);
    EnableEventPSX(first_handle);
    for (index = 1; index < 7; ++index)
    {
        handle = poll->events[index];
        EnableEventPSX(handle);
    }
    bios_exit_critical();
}

const char *game_card_record_name(const uint8 *record)
{
    const uint8 *name = record + 8;

    if (!memchr(name, 0, GAME_CARD_ACCOUNT_RECORD_BYTES - 8))
        abort();
    return (const char *)name;
}

uint32 title_card_port_status(uint32 port)
{
    GAME_CARD_POLL *poll = &title_card.poll;
    uint32 value;
    sint32 status;

    FUNCTION_MARKER(0x800D3D90, "ACE3.BIN");
    if (port >= 2)
        abort();
    value = poll->ports[port].present;
    if (!value)
    {
        status = (sint16)poll->ports[port].info_status;
        return status == 2 ? 6 : 0;
    }
    status = (sint16)poll->ports[port].info_status;
    switch (status)
    {
        case 1:
            return 1;
        case 2:
            return 2;
        case 0:
        case 3:
            break;
        default:
            return 0xFFFFFFFF;
    }
    status = (sint16)poll->ports[port].load_status;
    switch (status)
    {
        case 1:
            return 1;
        case 2:
            return 2;
        case 3:
            return 3;
        case 0:
            break;
        default:
            return 0xFFFFFFFF;
    }
    value = poll->ports[port].file_found;
    if (value == 1)
        return 5;
    if (value == 0)
        return 4;
    return 1;
}

uint8 *title_account_record(uint32 index)
{
    if (index >= sizeof(title_card.records.accounts) / sizeof(*title_card.records.accounts))
        abort();
    return title_card.records.accounts[index];
}

uint8 *title_mission_record(uint32 index)
{
    if (index >= sizeof(title_card.records.missions) / sizeof(*title_card.records.missions))
        abort();
    return title_card.records.missions[index];
}

void title_card_reserve_records(void)
{
    uint32 index;

    FUNCTION_MARKER(0x800D4F0C, "ACE3.BIN");
    arena_alloc(0x360);
    arena_alloc(128);
    for (index = 0; index < 4; ++index)
        arena_alloc(0x480);
    for (index = 0; index < 7; ++index)
        arena_alloc(0x180);
    return;
}

void title_card_reset_directory(void)
{
    FUNCTION_MARKER(0x800D51FC, "ACE3.BIN");
    game_card_reset_directory(title_card.records.header);
}

void title_card_initialize_records_hook(void)
{
    FUNCTION_MARKER(0x800D5548, "ACE3.BIN");
}

uint32 title_card_begin_polling(void)
{
    GAME_CARD_POLL *poll = &title_card.poll;

    FUNCTION_MARKER(0x800D3CE8, "ACE3.BIN");
    poll->ports[0].step = (uint8)(1);
    poll->enabled = (uint32)(1);
    poll->ports[0].present = (uint8)(0);
    poll->ports[0].file_found = (uint8)(0);
    poll->ports[0].probe_retries = (uint8)(0);
    poll->ports[0].load_status = (uint16)(0);
    poll->ports[1].step = (uint8)(1);
    poll->ports[1].present = (uint8)(0);
    poll->ports[1].file_found = (uint8)(0);
    poll->ports[1].probe_retries = (uint8)(0);
    poll->ports[1].load_status = (uint16)(0);
    poll->next_port = (uint32)(0);
    return 1;
}

uint32 title_card_initialize_accounts(void)
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
    uint32 wire_word_11;
    uint32 wire_word_12;
    uint32 wire_word_13;
    uint32 wire_word_14;
    uint32 wire_word_15;
    uint32 wire_word_16;
    uint32 wire_word_17;
    uint32 wire_word_18;
    uint8 *block;
    uint32 slot, index, row, value;

    FUNCTION_MARKER(0x800D5290, "ACE3.BIN");
    for (slot = 0; slot < 4; ++slot)
    {
        block = title_card.records.accounts[slot];
        memcpy(block + 8, ui_default_account_name, sizeof(ui_default_account_name));
        (wire_halfword_0 = (0), (void)memcpy(block + 20, &wire_halfword_0, sizeof(wire_halfword_0)));
        (wire_halfword_1 = (0), (void)memcpy(block + 22, &wire_halfword_1, sizeof(wire_halfword_1)));
        for (index = 0; index < 60; ++index)
        {
            (wire_halfword_2 = (0), (void)memcpy(block + index * 2 + 394, &wire_halfword_2, sizeof(wire_halfword_2)));
            (wire_halfword_3 = (0), (void)memcpy(block + index * 2 + 274, &wire_halfword_3, sizeof(wire_halfword_3)));
            (wire_halfword_4 = (0), (void)memcpy(block + index * 2 + 154, &wire_halfword_4, sizeof(wire_halfword_4)));
            (wire_halfword_5 = (0), (void)memcpy(block + index * 2 + 34, &wire_halfword_5, sizeof(wire_halfword_5)));
            block[index + 514] = (0);
            block[index + 544] = (0);
            block[index + 574] = (0);
            block[index + 604] = (0);
        }
        (wire_halfword_6 = (0), (void)memcpy(block + 704, &wire_halfword_6, sizeof(wire_halfword_6)));
        (wire_halfword_7 = (0), (void)memcpy(block + 706, &wire_halfword_7, sizeof(wire_halfword_7)));
        for (row = 0; row < 6; ++row)
        {
            pad_profile_encode(block + 708 + row * 68, &pad_profiles[row]);
        }
        block[1116] = (0);
        block[1117] = (204);
        block[1118] = (204);
        (wire_halfword_8 = (0), (void)memcpy(block + 1122, &wire_halfword_8, sizeof(wire_halfword_8)));
        (wire_halfword_9 = (0), (void)memcpy(block + 1120, &wire_halfword_9, sizeof(wire_halfword_9)));
        (wire_halfword_10 = (0), (void)memcpy(block + 22, &wire_halfword_10, sizeof(wire_halfword_10)));
        value = game_input.flags;
        block[1124] = ((value & 4) == 0);
        (wire_word_11 = (0), (void)memcpy(block + 1128, &wire_word_11, sizeof(wire_word_11)));
        if (game_runtime.options.captions & 1)
            (wire_word_12 = (1), (void)memcpy(block + 1128, &wire_word_12, sizeof(wire_word_12)));
        if (game_input.flags & 1)
        {
            value = (memcpy(&wire_word_13, block + 1128, sizeof(wire_word_13)), wire_word_13);
            (wire_word_14 = (value | 2), (void)memcpy(block + 1128, &wire_word_14, sizeof(wire_word_14)));
        }
        if (game_input.flags & 2)
        {
            value = (memcpy(&wire_word_15, block + 1128, sizeof(wire_word_15)), wire_word_15);
            (wire_word_16 = (value | 4), (void)memcpy(block + 1128, &wire_word_16, sizeof(wire_word_16)));
        }
        if (game_runtime.account.hold_action_gate == 1)
        {
            value = (memcpy(&wire_word_17, block + 1128, sizeof(wire_word_17)), wire_word_17);
            (wire_word_18 = (value | 8), (void)memcpy(block + 1128, &wire_word_18, sizeof(wire_word_18)));
        }
        for (index = 0; index < 17; ++index)
            block[1148 - index] = (0);
    }
    return 0;
}

uint32 title_card_format_port(uint32 port)
{
    uint32 result;

    FUNCTION_MARKER(0x800D5E74, "ACE3.BIN");
    if (title_card_stop_polling() == 0)
        return 0;
    result = (uint32)_card_format((sint32)(port << 4));
    title_card_begin_polling();
    return result == 1 ? 1 : 0xFFFFFFFF;
}

uint32 title_card_free_blocks(uint32 port)
{
    uint32 wire_word_0;
    uint8 pattern[16], directory[40];
    uint16 slots = 15;
    uint32 size, adjusted;
    void *found;

    FUNCTION_MARKER(0x800D64C4, "ACE3.BIN");
    if (title_card_stop_polling() == 0)
        return 0xFFFFFFFF;
    if (port >= 2)
        abort();
    sint32 pattern_bytes = snprintf((char *)pattern, sizeof(pattern), "bu%1d0:*.*", (sint32)port);
    if (pattern_bytes < 0 || (uint32)pattern_bytes >= sizeof(pattern))
        abort();
    found = firstfile((const char *)pattern, directory);
    while (found)
    {
        if (!memchr(directory, 0, sizeof(directory)))
            abort();
        size = (memcpy(&wire_word_0, directory + 24, sizeof(wire_word_0)), wire_word_0);
        adjusted = size + 0x1FFF;
        if ((sint32)adjusted < 0)
            adjusted = size + 0x3FFE;
        slots = (slots - (uint32)((sint32)adjusted >> 13));
        found = nextfile(directory);
    }
    title_card_begin_polling();
    return (sint16)slots;
}

uint32 title_card_create_file(uint32 port)
{
    GAME_CARD_IO *io = &title_card.io;
    uint32 descriptor;

    FUNCTION_MARKER(0x800D5EC4, "ACE3.BIN");
    if (title_card_stop_polling() == 0)
        return 0;
    descriptor = game_card_open_account(port, 0x20200);
    io->descriptor = (uint32)(descriptor);
    game_bios_close_guest(descriptor);
    title_card_begin_polling();
    return descriptor == 0xFFFFFFFF ? 0xFFFFFFFF : 1;
}

uint32 title_card_write_icons(uint32 port, uint32 finalize)
{
    GAME_CARD_IO *io = &title_card.io;
    uint32 descriptor, outcome, retry;
    sint32 attempts;

    FUNCTION_MARKER(0x800D6D78, "ACE3.BIN");
    if (title_card_stop_polling() == 0)
        return 0;
    if ((sint16)io->phase == 0)
    {
        io->phase = (uint16)(1);
        io->retry_count = (uint16)(0);
    }
    if (io->pending == 0)
    {
        descriptor = game_card_open_account(port, 0x8002);
        io->descriptor = (uint32)(descriptor);
        if (descriptor == 0xFFFFFFFF)
        {
            attempts = (sint16)io->retry_count;
            retry = io->retry_count;
            if (attempts < 3)
            {
                io->retry_count = (uint16)(retry + 1);
                return 0;
            }
            title_card_begin_polling();
            io->phase = (uint16)(0);
            return 0xFFFFFFFF;
        }
        writePSX((sint32)descriptor, title_card.icon_block, GAME_CARD_ICON_BLOCK_BYTES);
        io->pending = (uint8)(1);
    }
    outcome = title_card_poll_info_event(port, 0);
    if (outcome == 0xFFFFFFFF)
        return 0;
    descriptor = io->descriptor;
    game_bios_close_guest(descriptor);
    io->pending = (uint8)(0);
    if (outcome != 0)
    {
        attempts = (sint16)io->retry_count;
        retry = io->retry_count;
        if (attempts < 3)
        {
            io->retry_count = (uint16)(retry + 1);
            return 0;
        }
        io->phase = (uint16)(0);
        title_card_begin_polling();
        return 0xFFFFFFFF;
    }
    if (finalize == 1)
        title_card_begin_polling();
    return 1;
}

uint32 title_card_write_directory(uint32 port, sint32 slot, uint32 finalize)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint32 wire_word_3;
    uint32 wire_word_4;
    uint8 *header = title_card.records.header;
    GAME_CARD_IO *io = &title_card.io;
    uint32 value, checksum, index, descriptor, outcome, retry;
    sint32 attempts;

    FUNCTION_MARKER(0x800D6F04, "ACE3.BIN");
    if (title_card_stop_polling() == 0)
        return 0;
    if ((sint16)io->phase == 0)
    {
        io->phase = (uint16)(1);
        io->retry_count = (uint16)(0);
    }
    if (io->pending == 0)
    {
        if (slot >= 0)
        {
            value = header[8];
            header[8] = (value | ((uint32)1 << ((uint32)slot & 31)));
            value = (memcpy(&wire_word_0, header + 12, sizeof(wire_word_0)), wire_word_0);
            (wire_word_1 = (value + 1), (void)memcpy(header + 12, &wire_word_1, sizeof(wire_word_1)));
        }
        else
        {
            header[8] = (0);
            (wire_word_2 = (0), (void)memcpy(header + 12, &wire_word_2, sizeof(wire_word_2)));
        }
        checksum = 0x41434533;
        for (index = 0; index < 31; ++index)
            checksum ^= (memcpy(&wire_word_3, header + 4 + index * 4, sizeof(wire_word_3)), wire_word_3);
        (wire_word_4 = (checksum), (void)memcpy(header, &wire_word_4, sizeof(wire_word_4)));
        descriptor = game_card_open_account(port, 0x8002);
        io->descriptor = (uint32)(descriptor);
        if (descriptor == 0xFFFFFFFF)
        {
            attempts = (sint16)io->retry_count;
            retry = io->retry_count;
            if (attempts < 3)
            {
                io->retry_count = (uint16)(retry + 1);
                return 0;
            }
            title_card_begin_polling();
            io->phase = (uint16)(0);
            return 0xFFFFFFFF;
        }
        game_bios_seek_guest(descriptor, 640, 0);
        descriptor = io->descriptor;
        writePSX((sint32)descriptor, header, sizeof(title_card.records.header));
        io->pending = (uint8)(1);
    }
    outcome = title_card_poll_info_event(port, 0);
    if (outcome == 0xFFFFFFFF)
        return 0;
    descriptor = io->descriptor;
    game_bios_close_guest(descriptor);
    io->pending = (uint8)(0);
    if (outcome != 0)
    {
        attempts = (sint16)io->retry_count;
        retry = io->retry_count;
        if (attempts < 3)
        {
            io->retry_count = (uint16)(retry + 1);
            return 0;
        }
        title_card_begin_polling();
        io->phase = (uint16)(0);
        return 0xFFFFFFFF;
    }
    if (finalize == 1)
        title_card_begin_polling();
    return 1;
}

uint32 title_card_read_directory(uint32 port, uint32 finalize)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint8 *header = title_card.records.header;
    GAME_CARD_IO *io = &title_card.io;
    uint32 descriptor, outcome, retry, checksum, index, value;
    sint32 attempts;

    FUNCTION_MARKER(0x800D6604, "ACE3.BIN");
    if (title_card_stop_polling() == 0)
        return 0;
    if ((sint16)io->phase == 0)
    {
        io->phase = (uint16)(1);
        io->retry_count = (uint16)(0);
    }
    if (io->pending == 0)
    {
        descriptor = game_card_open_account(port, 0x8001);
        io->descriptor = (uint32)(descriptor);
        if (descriptor == 0xFFFFFFFF)
        {
            attempts = (sint16)io->retry_count;
            retry = io->retry_count;
            if (attempts < 3)
            {
                io->retry_count = (uint16)(retry + 1);
                return 0;
            }
            title_card_begin_polling();
            io->phase = (uint16)(0);
            return 0xFFFFFFFF;
        }
        game_bios_seek_guest(descriptor, 640, 0);
        descriptor = io->descriptor;
        readPSX((sint32)descriptor, header, sizeof(title_card.records.header));
        io->pending = (uint8)(1);
    }
    outcome = title_card_poll_info_event(port, 0);
    if (outcome == 0xFFFFFFFF)
        return 0;
    descriptor = io->descriptor;
    game_bios_close_guest(descriptor);
    io->pending = (uint8)(0);
    checksum = 0x41434533;
    for (index = 0; index < 31; ++index)
        checksum ^= (memcpy(&wire_word_0, header + 4 + index * 4, sizeof(wire_word_0)), wire_word_0);
    value = (memcpy(&wire_word_1, header, sizeof(wire_word_1)), wire_word_1);
    if (value != checksum || (memcpy(&wire_word_2, header + 4, sizeof(wire_word_2)), wire_word_2) != 0x00990426)
    {
        title_card_begin_polling();
        io->phase = (uint16)(0);
        return 2;
    }
    if (outcome != 0)
    {
        attempts = (sint16)io->retry_count;
        retry = io->retry_count;
        if (attempts < 3)
        {
            io->retry_count = (uint16)(retry + 1);
            return 0;
        }
        io->phase = (uint16)(0);
        title_card_begin_polling();
        return 0xFFFFFFFF;
    }
    io->phase = (uint16)(0);
    if (finalize == 1)
        title_card_begin_polling();
    return 1;
}

uint32 title_card_read_accounts(uint32 port, uint32 finalize)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    const uint8 *header = title_card.records.header;
    GAME_CARD_IO *io = &title_card.io;
    uint8 *buffer;
    uint32 flags, descriptor, outcome, retry, checksum, index, value, offset;
    sint32 slot, attempts;

    FUNCTION_MARKER(0x800D6814, "ACE3.BIN");
    if (title_card_stop_polling() == 0)
        return 0;
    slot = (sint16)io->record_index;
    flags = header[8];
    if (((flags >> ((uint32)slot & 31)) & 1) == 0)
        goto advance_slot;
    if (title_card_stop_polling() == 0)
        return 0;
    if ((sint16)io->phase == 0)
    {
        io->phase = (uint16)(1);
        io->retry_count = (uint16)(0);
    }
    if (io->pending == 0)
    {
        descriptor = game_card_open_account(port, 0x8001);
        io->descriptor = (uint32)(descriptor);
        if (descriptor == 0xFFFFFFFF)
        {
            attempts = (sint16)io->retry_count;
            retry = io->retry_count;
            if (attempts < 3)
            {
                io->retry_count = (uint16)(retry + 1);
                return 0;
            }
            goto failed;
        }
        slot = (sint16)io->record_index;
        offset = (uint32)slot * 1152 + 768;
        game_bios_seek_guest(descriptor, (sint32)offset, 0);
        slot = (sint16)io->record_index;
        descriptor = io->descriptor;
        buffer = title_account_record((uint32)slot);
        readPSX((sint32)descriptor, buffer, GAME_CARD_ACCOUNT_RECORD_BYTES);
        io->pending = (uint8)(1);
    }
    outcome = title_card_poll_info_event(port, 0);
    if (outcome == 0xFFFFFFFF)
        return 0;
    descriptor = io->descriptor;
    game_bios_close_guest(descriptor);
    io->pending = (uint8)(0);
    if (outcome != 0)
    {
        attempts = (sint16)io->retry_count;
        retry = io->retry_count;
        if (attempts < 3)
        {
            io->retry_count = (uint16)(retry + 1);
            return 0;
        }
        goto failed;
    }
    checksum = 0x41434533;
    slot = (sint16)io->record_index;
    buffer = title_account_record((uint32)slot);
    for (index = 0; index < 287; ++index)
        checksum ^= (memcpy(&wire_word_0, buffer + 4 + index * 4, sizeof(wire_word_0)), wire_word_0);
    slot = (sint16)io->record_index;
    buffer = title_account_record((uint32)slot);
    value = (memcpy(&wire_word_1, buffer, sizeof(wire_word_1)), wire_word_1);
    if (value != checksum)
    {
        title_card_begin_polling();
        io->phase = (uint16)(0);
        io->record_index = (uint16)(0);
        return 2;
    }
advance_slot:
    slot = (sint16)io->record_index;
    retry = io->record_index;
    if (slot < 2)
    {
        io->record_index = (uint16)(retry + 1);
        return 0;
    }
    io->record_index = (uint16)(0);
    if (finalize == 1)
        title_card_begin_polling();
    return 1;
failed:
    title_card_begin_polling();
    io->phase = (uint16)(0);
    io->record_index = (uint16)(0);
    return 0xFFFFFFFF;
}

uint32 title_card_account_present(uint32 slot)
{
    const uint8 *header = title_card.records.header;
    uint32 flags;

    FUNCTION_MARKER(0x800D5A38, "ACE3.BIN");
    if (slot >= 3)
        return 0xFFFFFFFF;
    flags = header[8];
    return (flags >> slot) & 1;
}

uint32 title_card_write_account(uint32 port, uint32 slot, uint32 kind, uint32 finalize)
{
    uint16 wire_halfword_0;
    uint16 wire_halfword_1;
    uint16 wire_halfword_2;
    uint16 wire_halfword_3;
    uint16 wire_halfword_4;
    uint16 wire_halfword_5;
    uint16 wire_halfword_6;
    uint32 wire_word_7;
    uint32 wire_word_8;
    GAME_CARD_IO *io = &title_card.io;
    uint8 *buffer;
    uint8 *source;
    uint32 value, checksum, index, descriptor, outcome, retry, offset;
    sint32 attempts;

    FUNCTION_MARKER(0x800D713C, "ACE3.BIN");
    if (title_card_stop_polling() == 0)
        return 0;
    if ((sint16)io->phase == 0)
    {
        io->phase = (uint16)(1);
        io->retry_count = (uint16)(0);
    }
    if (kind < 6)
    {
        buffer = title_card.records.accounts[3];
        (wire_halfword_0 = (kind), (void)memcpy(buffer + 20, &wire_halfword_0, sizeof(wire_halfword_0)));
        source = title_account_record(slot);
        buffer = title_card.records.accounts[3];
        value = (memcpy(&wire_halfword_1, source + 22, sizeof(wire_halfword_1)), wire_halfword_1);
        (wire_halfword_2 = (value), (void)memcpy(buffer + 22, &wire_halfword_2, sizeof(wire_halfword_2)));
        buffer = title_card.records.accounts[3];
        value = (memcpy(&wire_halfword_3, buffer + 22, sizeof(wire_halfword_3)), wire_halfword_3);
        (wire_halfword_4 = (value | ((uint32)1 << kind)), (void)memcpy(buffer + 22, &wire_halfword_4, sizeof(wire_halfword_4)));
    }
    else if (kind == 6)
    {
        buffer = title_card.records.accounts[3];
        (wire_halfword_5 = (0), (void)memcpy(buffer + 20, &wire_halfword_5, sizeof(wire_halfword_5)));
        buffer = title_card.records.accounts[3];
        (wire_halfword_6 = (0), (void)memcpy(buffer + 22, &wire_halfword_6, sizeof(wire_halfword_6)));
    }
    if (io->pending == 0)
    {
        checksum = 0x41434533;
        buffer = title_card.records.accounts[3];
        for (index = 0; index < 287; ++index)
            checksum ^= (memcpy(&wire_word_7, buffer + 4 + index * 4, sizeof(wire_word_7)), wire_word_7);
        buffer = title_card.records.accounts[3];
        (wire_word_8 = (checksum), (void)memcpy(buffer, &wire_word_8, sizeof(wire_word_8)));
        descriptor = game_card_open_account(port, 0x8002);
        io->descriptor = (uint32)(descriptor);
        if (descriptor == 0xFFFFFFFF)
        {
            attempts = (sint16)io->retry_count;
            retry = io->retry_count;
            if (attempts < 3)
            {
                io->retry_count = (uint16)(retry + 1);
                return 0;
            }
            title_card_begin_polling();
            io->phase = (uint16)(0);
            return 0xFFFFFFFF;
        }
        offset = slot * 1152 + 768;
        game_bios_seek_guest(descriptor, (sint32)offset, 0);
        descriptor = io->descriptor;
        buffer = title_card.records.accounts[3];
        writePSX((sint32)descriptor, buffer, GAME_CARD_ACCOUNT_RECORD_BYTES);
        io->pending = (uint8)(1);
    }
    outcome = title_card_poll_info_event(port, 0);
    if (outcome == 0xFFFFFFFF)
        return 0;
    descriptor = io->descriptor;
    game_bios_close_guest(descriptor);
    io->pending = (uint8)(0);
    if (outcome != 0)
    {
        attempts = (sint16)io->retry_count;
        retry = io->retry_count;
        if (attempts < 3)
        {
            io->retry_count = (uint16)(retry + 1);
            return 0;
        }
        io->phase = (uint16)(0);
        title_card_begin_polling();
        return 0xFFFFFFFF;
    }
    if (finalize == 1)
        title_card_begin_polling();
    return 1;
}

void title_card_encode_current_account(void)
{
    uint32 wire_word_0;
    uint16 wire_halfword_1;
    uint16 wire_halfword_2;
    uint16 wire_halfword_3;
    uint16 wire_halfword_4;
    uint16 wire_halfword_5;
    uint32 wire_word_6;
    uint16 wire_halfword_7;
    uint32 wire_word_8;
    uint32 wire_word_9;
    uint32 wire_word_10;
    uint32 wire_word_11;
    uint32 wire_word_12;
    uint32 wire_word_13;
    uint32 wire_word_14;
    static const uint32 half_destination[4] = {0x22, 0x9A, 0x112, 0x18A};
    static const uint32 nibble_destination[4] = {0x202, 0x220, 0x23E, 0x25C};
    uint8 *output;
    uint32 value, previous, block, i, j, row, column;
    uint8 *destination;
    uint32 group, count, second, result;
    FUNCTION_MARKER(0x800D41C8, "ACE3.BIN");
    output = title_card.records.accounts[3];
    value = game_runtime.progress.account_id;
    (wire_word_0 = (value), (void)memcpy(output + 4, &wire_word_0, sizeof(wire_word_0)));
    for (block = 0; block < 6; ++block)
    {
        pad_profile_encode(output + 0x2C4 + block * 0x44, &pad_profiles[block]);
        value = game_input.flags;
        output[0x464] = ((value & 4) == 0);
    }
    for (group = 0; group < 2; ++group)
    {
        count = group == 0 ? 8 : 2;
        const uint8 *unlocks = group == 0 ? game_runtime.progress.aircraft_unlocked : game_runtime.progress.weapon_unlocked;
        destination = output + (group == 0 ? 26 : 24);
        for (row = 0; row < count; ++row)
        {
            destination[row] = (0);
            for (column = 0; column < 8; ++column)
            {
                value = unlocks[row * 8 + column];
                previous = destination[row];
                destination[row] = (previous | ((value & 1) << column));
            }
        }
    }
    for (i = 0; i < 60; ++i)
        for (j = 0; j < 4; ++j)
        {
            value = game_runtime.progress.mission_time[j][i];
            (wire_halfword_1 = (value), (void)memcpy(output + half_destination[j] + i * 2, &wire_halfword_1, sizeof(wire_halfword_1)));
        }
    for (i = 0; i < 30; ++i)
        for (j = 0; j < 4; ++j)
        {
            value = game_runtime.progress.mission_rating[j][i * 2];
            second = game_runtime.progress.mission_rating[j][i * 2 + 1];
            output[nibble_destination[j] + i] = ((value & 15) | ((second & 15) << 4));
        }
    for (group = 0; group < 2; ++group)
    {
        count = group == 0 ? 20 : 45;
        const uint8 *status = group == 0 ? game_runtime.progress.archive_status : game_runtime.progress.scene_status;
        destination = output + (group == 0 ? 0x27A : 0x28E);
        for (row = 0; row < count; ++row)
        {
            destination[row] = (0);
            for (column = 0; column < 4; ++column)
            {
                value = status[row * 4 + column];
                previous = destination[row];
                destination[row] = (previous | ((value & 3) << (column * 2)));
            }
        }
    }
    value = (uint16)game_runtime.display.screen[0];
    (wire_halfword_2 = (value), (void)memcpy(output + 0x460, &wire_halfword_2, sizeof(wire_halfword_2)));
    value = (uint16)game_runtime.display.screen[1];
    (wire_halfword_3 = (value), (void)memcpy(output + 0x462, &wire_halfword_3, sizeof(wire_halfword_3)));
    value = audio_settings.volume[0];
    output[0x45D] = (value);
    value = audio_settings.volume[1];
    output[0x45E] = (value);
    value = audio_settings.flags;
    output[0x45C] = (value);
    value = (uint16)game_runtime.progress.total_count;
    (wire_halfword_4 = (value), (void)memcpy(output + 0x2C0, &wire_halfword_4, sizeof(wire_halfword_4)));
    value = (uint16)game_runtime.progress.total_losses;
    (wire_halfword_5 = (value), (void)memcpy(output + 0x2C2, &wire_halfword_5, sizeof(wire_halfword_5)));
    value = game_runtime.progress.record_slot;
    (wire_word_6 = (0), (void)memcpy(output + 0x468, &wire_word_6, sizeof(wire_word_6)));
    (wire_halfword_7 = (value), (void)memcpy(output + 0x14, &wire_halfword_7, sizeof(wire_halfword_7)));
    value = game_runtime.options.captions;
    if (value & 1)
        (wire_word_8 = (1), (void)memcpy(output + 0x468, &wire_word_8, sizeof(wire_word_8)));
    value = game_input.flags;
    if (value & 1)
    {
        value = (memcpy(&wire_word_9, output + 0x468, sizeof(wire_word_9)), wire_word_9);
        (wire_word_10 = (value | 2), (void)memcpy(output + 0x468, &wire_word_10, sizeof(wire_word_10)));
    }
    value = game_input.flags;
    if (value & 2)
    {
        value = (memcpy(&wire_word_11, output + 0x468, sizeof(wire_word_11)), wire_word_11);
        (wire_word_12 = (value | 4), (void)memcpy(output + 0x468, &wire_word_12, sizeof(wire_word_12)));
    }
    value = game_runtime.account.hold_action_gate;
    result = 1;
    if (value == 1)
    {
        result = (memcpy(&wire_word_13, output + 0x468, sizeof(wire_word_13)), wire_word_13) | 8;
        (wire_word_14 = (result), (void)memcpy(output + 0x468, &wire_word_14, sizeof(wire_word_14)));
    }
    for (i = 17; i > 0; --i)
        output[0x46C + i - 1] = (0);
    {
        (void)(uint32)(result);
        return;
    }
}

void title_card_select_account_copy(uint32 slot)
{
    FUNCTION_MARKER(0x800D47B8, "ACE3.BIN");
    memmove(title_account_record(3), title_account_record(slot), GAME_CARD_ACCOUNT_RECORD_BYTES);
}

uint32 title_card_current_record_mask(void)
{
    uint16 wire_halfword_0;
    uint8 *buffer;
    FUNCTION_MARKER(0x800D5CA0, "ACE3.BIN");
    buffer = title_card.records.accounts[3];
    return (sint16)(memcpy(&wire_halfword_0, buffer + 22, sizeof(wire_halfword_0)), wire_halfword_0);
}

void title_card_decode_mission(uint32 selection)
{
    uint32 wire_word_0;
    uint16 wire_halfword_1;
    const uint8 *source;
    uint32 value, i;
    FUNCTION_MARKER(0x800D4DF4, "ACE3.BIN");
    source = title_mission_record(selection & 255);
    game_runtime.progress.record_slot = (uint8)(selection);
    value = (memcpy(&wire_word_0, source + 0x114, sizeof(wire_word_0)), wire_word_0);
    game_runtime.progress.total_time = (uint32)(value);
    value = source[0x113];
    game_runtime.selection.mission = (uint16)(value);
    value = source[0x112];
    game_runtime.selection.difficulty = (uint32)(value);
    for (i = 0; i < 60; ++i)
    {
        value = (memcpy(&wire_halfword_1, source + i * 2 + 4, sizeof(wire_halfword_1)), wire_halfword_1);
        game_runtime.progress.mission_time[4][i] = (uint16)(value);
        value = source[i + 0x7C];
        game_runtime.progress.mission_rating[4][i] = (uint8)(value);
        value = source[i + 0x9A];
        game_runtime.progress.mission_count[i] = (uint32)(value);
        value = source[i + 0xD6];
        game_runtime.progress.mission_losses[i] = (uint32)(value);
        value = source[0x118];
        game_runtime.selection.aircraft = (uint16)(value);
        value = source[0x119];
        game_runtime.selection.weapon_code[0] = (uint8)(value);
        value = source[0x11A];
        game_runtime.selection.weapon_code[1] = (uint8)(value);
        value = source[0x11B];
        game_runtime.selection.weapon_code[2] = (uint8)(value);
    }
    for (i = 0; i < 30; ++i)
    {
        value = source[i + 0x7C];
        game_runtime.progress.mission_rating[4][i * 2] = (uint8)(value & 15);
        value = source[i + 0x7C];
        game_runtime.progress.mission_rating[4][i * 2 + 1] = (uint8)(value >> 4);
    }
    return;
}

void title_card_decode_account(uint8 *record_selection)
{
    uint32 wire_word_0;
    uint16 wire_halfword_1;
    uint16 wire_halfword_2;
    uint16 wire_halfword_3;
    uint16 wire_halfword_4;
    uint16 wire_halfword_5;
    uint32 wire_word_6;
    uint32 wire_word_7;
    uint32 wire_word_8;
    uint32 wire_word_9;
    static const uint32 half_source[4] = {0x22, 0x9A, 0x112, 0x18A};
    static const uint32 nibble_source[4] = {0x202, 0x220, 0x23E, 0x25C};
    uint8 *buffer;
    uint8 *fresh;
    uint32 value, flags, i, j, row, column, group, count;
    uint8 *source;
    FUNCTION_MARKER(0x800D4808, "ACE3.BIN");
    buffer = title_card.records.accounts[3];
    value = buffer[20];
    *record_selection = value;
    fresh = title_card.records.accounts[3];
    game_account_set_name(game_card_record_name(fresh));
    fresh = title_card.records.accounts[3];
    value = (memcpy(&wire_word_0, fresh + 4, sizeof(wire_word_0)), wire_word_0);
    game_runtime.progress.account_id = (uint32)(value);
    for (i = 0; i < 60; ++i)
        for (j = 0; j < 4; ++j)
        {
            value = (memcpy(&wire_halfword_1, buffer + half_source[j] + i * 2, sizeof(wire_halfword_1)), wire_halfword_1);
            game_runtime.progress.mission_time[j][i] = (uint16)(value);
        }
    for (i = 0; i < 30; ++i)
        for (column = 0; column < 2; ++column)
            for (j = 0; j < 4; ++j)
            {
                value = buffer[nibble_source[j] + i];
                game_runtime.progress.mission_rating[j][i * 2 + column] = (uint8)((value >> (column * 4)) & 15);
            }
    value = (sint16)(memcpy(&wire_halfword_2, buffer + 0x2C0, sizeof(wire_halfword_2)), wire_halfword_2);
    game_runtime.progress.total_count = (uint32)(value);
    value = (sint16)(memcpy(&wire_halfword_3, buffer + 0x2C2, sizeof(wire_halfword_3)), wire_halfword_3);
    game_runtime.progress.total_losses = (uint32)(value);
    value = buffer[20];
    game_runtime.progress.record_slot = (uint8)(value);
    for (i = 0; i < 6; ++i)
    {
        pad_profile_decode(&pad_profiles[i], buffer + 0x2C4 + i * 0x44);
    }
    flags = buffer[0x464];
    value = game_input.flags;
    game_input.flags = (uint32)(flags == 0 ? value | 4 : value & ~4);
    fresh = title_card.records.accounts[3];
    value = (sint16)(memcpy(&wire_halfword_4, fresh + 0x460, sizeof(wire_halfword_4)), wire_halfword_4);
    game_runtime.display.screen[0] = (uint32)(value);
    fresh = title_card.records.accounts[3];
    value = (sint16)(memcpy(&wire_halfword_5, fresh + 0x462, sizeof(wire_halfword_5)), wire_halfword_5);
    game_runtime.display.screen[1] = (uint32)(value);
    value = buffer[0x45D];
    audio_settings.volume[0] = (uint8)(value);
    value = buffer[0x45E];
    audio_settings.volume[1] = (uint8)(value);
    value = buffer[0x45C];
    audio_settings.flags = (uint8)(value);
    value = audio_settings.flags;
    resource_set_mono((value & 1) != 0);
    resource_apply_mix();
    for (group = 0; group < 2; ++group)
    {
        count = group == 0 ? 8 : 2;
        source = buffer + (group == 0 ? 26 : 24);
        uint8 *unlocks = group == 0 ? game_runtime.progress.aircraft_unlocked : game_runtime.progress.weapon_unlocked;
        for (row = 0; row < count; ++row)
            for (column = 0; column < 8; ++column)
            {
                value = source[row];
                unlocks[row * 8 + column] = ((value >> column) & 1);
            }
    }
    for (group = 0; group < 2; ++group)
    {
        count = group == 0 ? 20 : 45;
        source = buffer + (group == 0 ? 0x27A : 0x28E);
        uint8 *status = group == 0 ? game_runtime.progress.archive_status : game_runtime.progress.scene_status;
        for (row = 0; row < count; ++row)
            for (column = 0; column < 4; ++column)
            {
                value = source[row];
                status[row * 4 + column] = ((value >> (column * 2)) & 3);
            }
    }
    flags = (memcpy(&wire_word_6, buffer + 0x468, sizeof(wire_word_6)), wire_word_6);
    value = game_runtime.options.captions;
    game_runtime.options.captions = (uint8)(flags & 1 ? value | 1 : value & 254);
    flags = (memcpy(&wire_word_7, buffer + 0x468, sizeof(wire_word_7)), wire_word_7);
    value = game_input.flags;
    game_input.flags = (uint32)(flags & 2 ? value | 1 : value & ~1);
    flags = (memcpy(&wire_word_8, buffer + 0x468, sizeof(wire_word_8)), wire_word_8);
    value = game_input.flags;
    game_input.flags = (uint32)(flags & 4 ? value | 2 : value & ~2);
    flags = (memcpy(&wire_word_9, buffer + 0x468, sizeof(wire_word_9)), wire_word_9);
    game_runtime.account.hold_action_gate = (uint8)((flags & 8) != 0);
    value = game_runtime.display.flags | 2;
    game_runtime.display.flags = (uint32)(value);
    {
        (void)(uint32)(value);
        return;
    }
}

uint32 title_card_read_missions(uint32 port, uint32 slot, uint32 finalize)
{
    uint16 wire_halfword_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    GAME_CARD_IO *io = &title_card.io;
    uint8 *record;
    uint8 *buffer;
    uint32 mask, descriptor, phase, active, retry, offset, outcome, checksum, value, i;
    sint32 index, attempts;
    FUNCTION_MARKER(0x800D6AB4, "ACE3.BIN");
    if (!title_card_stop_polling())
        return 0;
    buffer = title_account_record(slot);
    index = (sint16)io->record_index;
    mask = (sint16)(memcpy(&wire_halfword_0, buffer + 22, sizeof(wire_halfword_0)), wire_halfword_0);
    if (((mask >> ((uint32)index & 31)) & 1) == 0)
        goto advance;
    if (!title_card_stop_polling())
        return 0;
    phase = (sint16)io->phase;
    if (phase == 0)
    {
        io->phase = (uint16)(1);
        io->retry_count = (uint16)(0);
    }
    active = io->pending;
    if (active == 0)
    {
        descriptor = game_card_open_account(port, 0x8001);
        io->descriptor = (uint32)(descriptor);
        if (descriptor == 0xFFFFFFFF)
        {
            attempts = (sint16)io->retry_count;
            retry = io->retry_count;
            if (attempts < 3)
            {
                io->retry_count = (uint16)(retry + 1);
                return 0;
            }
            io->phase = (uint16)(0);
            title_card_begin_polling();
            io->record_index = (uint16)(0);
            return 0xFFFFFFFF;
        }
        index = (sint16)io->record_index;
        offset = slot * 2304 + 0x1080 + (uint32)index * 384;
        game_bios_seek_guest(descriptor, (sint32)offset, 0);
        index = (sint16)io->record_index;
        descriptor = io->descriptor;
        record = title_mission_record((uint32)index);
        readPSX((sint32)descriptor, record, GAME_CARD_MISSION_RECORD_BYTES);
        io->pending = (uint8)(1);
    }
    outcome = title_card_poll_info_event(port, 0);
    if (outcome == 0xFFFFFFFF)
        return 0;
    descriptor = io->descriptor;
    game_bios_close_guest(descriptor);
    io->pending = (uint8)(0);
    if (outcome != 0)
    {
        attempts = (sint16)io->retry_count;
        retry = io->retry_count;
        if (attempts < 3)
        {
            io->retry_count = (uint16)(retry + 1);
            return 0;
        }
        io->phase = (uint16)(0);
        title_card_begin_polling();
        io->record_index = (uint16)(0);
        return 0xFFFFFFFF;
    }
    index = (sint16)io->record_index;
    record = title_mission_record((uint32)index);
    checksum = 0x41434533;
    for (i = 0; i < 95; ++i)
    {
        value = (memcpy(&wire_word_1, record + 4 + i * 4, sizeof(wire_word_1)), wire_word_1);
        checksum ^= value;
    }
    index = (sint16)io->record_index;
    record = title_mission_record((uint32)index);
    value = (memcpy(&wire_word_2, record, sizeof(wire_word_2)), wire_word_2);
    if (value != checksum)
    {
        title_card_begin_polling();
        io->phase = (uint16)(0);
        io->record_index = (uint16)(0);
        return 2;
    }
advance:
    index = (sint16)io->record_index;
    value = io->record_index;
    if (index < 6)
    {
        io->record_index = (uint16)(value + 1);
        return 0;
    }
    if (finalize == 1)
        title_card_begin_polling();
    io->phase = (uint16)(0);
    io->record_index = (uint16)(0);
    return 1;
}

uint32 title_card_mission_present(uint32 slot, uint32 selection)
{
    uint16 wire_halfword_0;
    uint8 *buffer;
    uint32 mask;
    FUNCTION_MARKER(0x800D59C0, "ACE3.BIN");
    buffer = title_account_record(slot == 0xFFFFFFFF ? 3 : slot);
    mask = (sint16)(memcpy(&wire_halfword_0, buffer + 22, sizeof(wire_halfword_0)), wire_halfword_0);
    return (mask >> (selection & 31)) & 1;
}

uint32 title_card_account_mask(void)
{
    const uint8 *header = title_card.records.header;
    FUNCTION_MARKER(0x800D5C80, "ACE3.BIN");
    return header[8];
}

uint8 *mission_card_mission_record(uint32 index)
{
    if (index >= sizeof(mission_card.records.missions) / sizeof(*mission_card.records.missions))
        abort();
    return mission_card.records.missions[index];
}

uint8 *mission_card_account_record(uint32 index)
{
    if (index >= sizeof(mission_card.records.accounts) / sizeof(*mission_card.records.accounts))
        abort();
    return mission_card.records.accounts[index];
}

void mission_card_reserve_records(void)
{
    uint32 i;
    FUNCTION_MARKER(0x800E37F8, "ACE3.BIN");
    arena_alloc(864);
    arena_alloc(GAME_CARD_DIRECTORY_BYTES);
    for (i = 0; i < 4; ++i)
        arena_alloc(GAME_CARD_ACCOUNT_RECORD_BYTES);
    for (i = 0; i < 7; ++i)
        arena_alloc(GAME_CARD_MISSION_RECORD_BYTES);
}

uint32 mission_card_begin_polling(void)
{
    GAME_CARD_POLL *poll = &mission_card.poll;
    FUNCTION_MARKER(0x800E25D4, "ACE3.BIN");
    poll->ports[0].step = (uint8)(1);
    poll->enabled = (uint32)(1);
    poll->ports[0].present = (uint8)(0);
    poll->ports[0].file_found = (uint8)(0);
    poll->ports[0].probe_retries = (uint8)(0);
    poll->ports[0].load_status = (uint16)(0);
    poll->ports[1].step = (uint8)(1);
    poll->ports[1].present = (uint8)(0);
    poll->ports[1].file_found = (uint8)(0);
    poll->ports[1].probe_retries = (uint8)(0);
    poll->ports[1].load_status = (uint16)(0);
    poll->next_port = (uint32)(0);
    return 1;
}

void mission_card_reset_directory(void)
{
    FUNCTION_MARKER(0x800E3AE8, "ACE3.BIN");
    game_card_reset_directory(mission_card.records.header);
}

uint32 mission_card_initialize_accounts(void)
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
    uint32 wire_word_11;
    uint32 wire_word_12;
    uint32 wire_word_13;
    uint32 wire_word_14;
    uint32 wire_word_15;
    uint32 wire_word_16;
    uint32 wire_word_17;
    uint32 wire_word_18;
    uint8 *account;
    uint32 slot, index, group, value;
    FUNCTION_MARKER(0x800E3B7C, "ACE3.BIN");
    for (slot = 0; slot < 4; ++slot)
    {
        account = mission_card_account_record(slot);
        memcpy(account + 8, ui_default_account_name, sizeof(ui_default_account_name));
        (wire_halfword_0 = (0), (void)memcpy(account + 20, &wire_halfword_0, sizeof(wire_halfword_0)));
        (wire_halfword_1 = (0), (void)memcpy(account + 22, &wire_halfword_1, sizeof(wire_halfword_1)));
        for (index = 0; index < 60; ++index)
        {
            (wire_halfword_2 = (0), (void)memcpy(account + 394 + index * 2, &wire_halfword_2, sizeof(wire_halfword_2)));
            (wire_halfword_3 = (0), (void)memcpy(account + 274 + index * 2, &wire_halfword_3, sizeof(wire_halfword_3)));
            (wire_halfword_4 = (0), (void)memcpy(account + 154 + index * 2, &wire_halfword_4, sizeof(wire_halfword_4)));
            (wire_halfword_5 = (0), (void)memcpy(account + 34 + index * 2, &wire_halfword_5, sizeof(wire_halfword_5)));
            account[514 + index] = (0);
            account[544 + index] = (0);
            account[574 + index] = (0);
            account[604 + index] = (0);
        }
        (wire_halfword_6 = (0), (void)memcpy(account + 704, &wire_halfword_6, sizeof(wire_halfword_6)));
        (wire_halfword_7 = (0), (void)memcpy(account + 706, &wire_halfword_7, sizeof(wire_halfword_7)));
        for (group = 0; group < 6; ++group)
        {
            pad_profile_encode(account + 708 + group * 68, &pad_profiles[group]);
        }
        account[1116] = (0);
        account[1117] = (204);
        account[1118] = (204);
        (wire_halfword_8 = (0), (void)memcpy(account + 1122, &wire_halfword_8, sizeof(wire_halfword_8)));
        (wire_halfword_9 = (0), (void)memcpy(account + 1120, &wire_halfword_9, sizeof(wire_halfword_9)));
        (wire_halfword_10 = (0), (void)memcpy(account + 22, &wire_halfword_10, sizeof(wire_halfword_10)));
        value = game_input.flags;
        account[1124] = ((value & 4) == 0);
        (wire_word_11 = (0), (void)memcpy(account + 1128, &wire_word_11, sizeof(wire_word_11)));
        if (game_runtime.options.captions & 1)
            (wire_word_12 = (1), (void)memcpy(account + 1128, &wire_word_12, sizeof(wire_word_12)));
        if (game_input.flags & 1)
        {
            value = (memcpy(&wire_word_13, account + 1128, sizeof(wire_word_13)), wire_word_13);
            (wire_word_14 = (value | 2), (void)memcpy(account + 1128, &wire_word_14, sizeof(wire_word_14)));
        }
        if (game_input.flags & 2)
        {
            value = (memcpy(&wire_word_15, account + 1128, sizeof(wire_word_15)), wire_word_15);
            (wire_word_16 = (value | 4), (void)memcpy(account + 1128, &wire_word_16, sizeof(wire_word_16)));
        }
        if (game_runtime.account.hold_action_gate == 1)
        {
            value = (memcpy(&wire_word_17, account + 1128, sizeof(wire_word_17)), wire_word_17);
            (wire_word_18 = (value | 8), (void)memcpy(account + 1128, &wire_word_18, sizeof(wire_word_18)));
        }
        for (index = 17; index > 0; --index)
            account[1131 + index] = (0);
    }
    return 0;
}

void mission_card_initialize_records_hook(void)
{
    FUNCTION_MARKER(0x800E3E34, "ACE3.BIN");
}

void mission_card_initialize(void)
{
    GAME_CARD_POLL *poll = &mission_card.poll;
    GAME_CARD_IO *io = &mission_card.io;
    uint32 index;
    FUNCTION_MARKER(0x800E3E3C, "ACE3.BIN");
    io->retry_count = (uint16)(0);
    io->phase = (uint16)(0);
    mission_card_reset_directory();
    mission_card_initialize_accounts();
    mission_card_initialize_records_hook();
    mission_card_begin_polling();
    io->pending = (uint8)(0);
    io->record_index = (uint16)(0);
    for (index = 0; index < 2; ++index)
    {
        poll->ports[index].info_status = (uint16)(2);
        poll->ports[index].load_status = (uint16)(2);
    }
}

void mission_card_open_events(void)
{
    GAME_CARD_POLL *poll = &mission_card.poll;
    static const uint32 classes[] = {0xF4000001u, 0xF4000001u, 0xF4000001u, 0xF4000001u, 0xF0000011u, 0xF0000011u, 0xF0000011u};
    static const uint16 specifications[] = {4, 32768, 256, 8192, 4, 32768, 256};
    uint32 handle, index;
    FUNCTION_MARKER(0x800E3FCC, "ACE3.BIN");
    bios_enter_critical();
    for (index = 0; index < 7; ++index)
    {
        handle = (uint32)OpenEvent(classes[index], specifications[index], 8192, NULL);
        poll->events[index] = (uint32)(handle);
    }
    for (index = 0; index < 7; ++index)
        EnableEvent((sint32)poll->events[index]);
    bios_exit_critical();
}

void mission_card_close_events(void)
{
    GAME_CARD_POLL *poll = &mission_card.poll;
    uint32 index;
    FUNCTION_MARKER(0x800E419C, "ACE3.BIN");
    bios_enter_critical();
    for (index = 0; index < 7; ++index)
        CloseEvent((sint32)poll->events[index]);
    bios_exit_critical();
}

void mission_card_poll_tick(void)
{
    GAME_CARD_POLL *poll = &mission_card.poll;
    GAME_CARD_IO *io = &mission_card.io;
    uint32 port, step, value, flags, result;
    sint32 status;
    FUNCTION_MARKER(0x800E4860, "ACE3.BIN");
    port = poll->next_port;
    if (port >= 2)
        abort();
    step = poll->ports[port].step;
    switch (step)
    {
        case 0:
            goto toggle_port;
        case 1:
            mission_card_clear_info_events();
            _card_info((sint32)(port << 4));
            poll->ports[port].step = (uint8)(2);
            poll->timeout = (uint32)(300);
            return;
        case 2:
            result = mission_card_poll_info_event(port, 3);
            poll->ports[port].event_result = (uint8)(result);
            value = poll->timeout;
            if ((sint32)value < 0)
            {
                poll->ports[port].step = (uint8)(1);
                return;
            }
            result = value - 1;
            poll->timeout = (uint32)(result);
            return;
        case 3:
            value = poll->ports[port].event_result;
            poll->ports[port].info_status = (uint16)(value);
            if (value == 0)
            {
                if (poll->ports[port].present == 1)
                {
                    flags = poll->enabled;
                    poll->ports[port].step = (uint8)(flags != 0);
                    goto toggle_port;
                }
                mission_card_clear_write_events();
                _new_card();
                psx_bios_card_write_guest(port << 4, 63, 0);
                poll->ports[port].step = (uint8)(4);
                return;
            }
            if (value == 1 || value == 2)
            {
                flags = poll->ports[port].probe_retries;
                if (flags >= 17)
                    poll->ports[port].present = (uint8)(1);
                else
                {
                    poll->ports[port].present = (uint8)(0);
                    flags = poll->ports[port].probe_retries;
                    poll->ports[port].probe_retries = (uint8)(flags + 1);
                }
                flags = poll->enabled;
                poll->ports[port].step = (uint8)(value == 1 ? flags == 1 : flags != 0);
                if (value == 1)
                    goto toggle_port;
                flags = poll->next_port;
                result = 1 - flags;
                poll->next_port = (uint32)(result);
                poll->ports[port].file_found = (uint8)(0);
                return;
            }
            if (value == 3)
            {
                poll->ports[port].info_status = (uint16)(0);
                mission_card_clear_write_events();
                _new_card();
                psx_bios_card_write_guest(port << 4, 63, 0);
                poll->ports[port].step = (uint8)(4);
                poll->ports[port].present = (uint8)(0);
                poll->ports[port].probe_retries = (uint8)(0);
                return;
            }
            poll->ports[port].present = (uint8)(1);
            poll->ports[port].step = (uint8)(1);
            goto toggle_port;
        case 4:
            do
            {
                result = mission_card_poll_write_event(port, 5);
                poll->ports[port].event_result = (uint8)(result);
                result = poll->ports[port].event_result;
            } while (result == 255);
            return;
        case 5:
            value = poll->ports[port].event_result;
            poll->ports[port].info_status = (uint16)(value);
            mission_card_clear_info_events();
            psx_bios_card_load_guest(port << 4);
            poll->ports[port].step = (uint8)(6);
            return;
        case 6:
            result = mission_card_poll_info_event(port, 7);
            poll->ports[port].event_result = (uint8)(result);
            return;
        case 7:
            value = poll->ports[port].event_result;
            poll->ports[port].load_status = (uint16)(value);
            poll->ports[port].present = (uint8)(1);
            status = (sint16)poll->ports[port].load_status;
            if (status < 0)
                return;
            if (status == 0)
            {
                poll->ports[port].step = (uint8)(8);
                return;
            }
            if (status <= 2)
            {
                value = poll->ports[port].probe_retries;
                if (value >= 17)
                    poll->ports[port].present = (uint8)(1);
                else
                {
                    poll->ports[port].present = (uint8)(0);
                    value = poll->ports[port].probe_retries;
                    poll->ports[port].probe_retries = (uint8)(value + 1);
                }
                poll->ports[port].step = (uint8)(1);
                return;
            }
            if (status != 3)
                return;
            poll->ports[port].present = (uint8)(1);
            poll->ports[port].step = (uint8)(1);
            goto toggle_port;
        case 8:
            result = game_card_open_account(port, 0x8001);
            io->descriptor = (uint32)(result);
            if (result != 0xFFFFFFFFu)
                poll->ports[port].file_found = (uint8)(1);
            game_bios_close_guest(io->descriptor);
            poll->ports[port].step = (uint8)(1);
            poll->ports[port].present = (uint8)(1);
            goto toggle_port;
        default:
            return;
    }
toggle_port:
    value = poll->next_port;
    result = 1 - value;
    poll->next_port = (uint32)(result);
}

uint32 mission_card_poll_info_event(uint32 port, uint32 phase)
{
    GAME_CARD_POLL *poll = &mission_card.poll;
    uint32 index, handle;
    FUNCTION_MARKER(0x800E2794, "ACE3.BIN");
    if (port >= 2)
        abort();
    for (index = 0; index < 4; ++index)
    {
        handle = poll->events[index];
        if (TestEvent((sint32)handle) == 1)
        {
            poll->ports[port].step = (uint8)(phase);
            return index;
        }
    }
    return 0xFFFFFFFFu;
}

void mission_card_clear_info_events(void)
{
    GAME_CARD_POLL *poll = &mission_card.poll;
    uint32 index, result = 0;
    FUNCTION_MARKER(0x800E2874, "ACE3.BIN");
    for (index = 0; index < 4; ++index)
        result = (uint32)TestEvent((sint32)poll->events[index]);
    {
        (void)(uint32)(result);
        return;
    }
}

void mission_card_clear_write_events(void)
{
    GAME_CARD_POLL *poll = &mission_card.poll;
    uint32 index, result = 0;
    FUNCTION_MARKER(0x800E28E0, "ACE3.BIN");
    for (index = 0; index < 4; ++index)
        result = (uint32)TestEvent((sint32)(index == 3 ? 0 : poll->events[index + 4]));
    {
        (void)(uint32)(result);
        return;
    }
}

uint32 mission_card_poll_write_event(uint32 port, uint32 phase)
{
    GAME_CARD_POLL *poll = &mission_card.poll;
    uint32 index, handle;
    FUNCTION_MARKER(0x800E294C, "ACE3.BIN");
    if (port >= 2)
        abort();
    for (index = 0; index < 4; ++index)
    {
        handle = (index == 3 ? 0 : poll->events[index + 4]);
        if (TestEvent((sint32)handle) == 1)
        {
            poll->ports[port].step = (uint8)(phase);
            return index;
        }
    }
    return 0xFFFFFFFFu;
}

uint32 mission_card_port_status(uint32 port)
{
    GAME_CARD_POLL *poll = &mission_card.poll;
    uint32 value;
    sint32 status;
    FUNCTION_MARKER(0x800E267C, "ACE3.BIN");
    if (port >= 2)
        abort();
    value = poll->ports[port].present;
    status = (sint16)poll->ports[port].info_status;
    if (value == 0)
        return status == 2 ? 6 : 0;
    if (status == 1 || status == 2)
        return status;
    if (status != 0 && status != 3)
        return 0xFFFFFFFFu;
    status = (sint16)poll->ports[port].load_status;
    if (status >= 1 && status <= 3)
        return status;
    if (status != 0)
        return 0xFFFFFFFFu;
    value = poll->ports[port].file_found;
    return value == 1 ? 5 : (value == 0 ? 4 : 1);
}

uint32 mission_card_stop_polling(void)
{
    GAME_CARD_POLL *poll = &mission_card.poll;
    uint32 value;
    FUNCTION_MARKER(0x800E264C, "ACE3.BIN");
    value = poll->ports[0].step;
    poll->enabled = (uint32)(0);
    if (value)
        return 0;
    return poll->ports[1].step == 0;
}

uint32 mission_card_format_port(uint32 port)
{
    sint32 result;
    FUNCTION_MARKER(0x800E4760, "ACE3.BIN");
    if (!mission_card_stop_polling())
        return 0;
    result = _card_format((sint32)(port << 4));
    mission_card_begin_polling();
    return result == 1 ? 1 : 0xFFFFFFFFu;
}

uint32 mission_card_create_file(uint32 port)
{
    GAME_CARD_IO *io = &mission_card.io;
    uint32 descriptor;
    FUNCTION_MARKER(0x800E47B0, "ACE3.BIN");
    if (!mission_card_stop_polling())
        return 0;
    descriptor = game_card_open_account(port, 0x20200);
    io->descriptor = (uint32)(descriptor);
    game_bios_close_guest(descriptor);
    mission_card_begin_polling();
    return descriptor == 0xFFFFFFFFu ? 0xFFFFFFFFu : 1;
}

uint32 mission_card_write_icons(uint32 port, uint32 finalize)
{
    GAME_CARD_IO *io = &mission_card.io;
    uint32 descriptor, event, retry;
    sint32 attempts;
    FUNCTION_MARKER(0x800E5664, "ACE3.BIN");
    if (!mission_card_stop_polling())
        return 0;
    if ((sint16)io->phase == 0)
    {
        io->phase = (uint16)(1);
        io->retry_count = (uint16)(0);
    }
    if (io->pending == 0)
    {
        descriptor = game_card_open_account(port, 0x8002);
        io->descriptor = (uint32)(descriptor);
        if (descriptor == 0xFFFFFFFFu)
        {
            attempts = (sint16)io->retry_count;
            retry = io->retry_count;
            if (attempts < 3)
            {
                io->retry_count = (uint16)(retry + 1);
                return 0;
            }
            mission_card_begin_polling();
            io->phase = (uint16)(0);
            return 0xFFFFFFFFu;
        }
        writePSX((sint32)descriptor, mission_card.icon_block, GAME_CARD_ICON_BLOCK_BYTES);
        io->pending = (uint8)(1);
    }
    event = mission_card_poll_info_event(port, 0);
    if (event == 0xFFFFFFFFu)
        return 0;
    game_bios_close_guest(io->descriptor);
    io->pending = (uint8)(0);
    if (event)
    {
        attempts = (sint16)io->retry_count;
        retry = io->retry_count;
        if (attempts < 3)
        {
            io->retry_count = (uint16)(retry + 1);
            return 0;
        }
        io->phase = (uint16)(0);
        mission_card_begin_polling();
        return 0xFFFFFFFFu;
    }
    if (finalize == 1)
        mission_card_begin_polling();
    return 1;
}

uint32 mission_card_write_directory(uint32 port, uint32 slot, uint32 finalize)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint32 wire_word_3;
    uint32 wire_word_4;
    GAME_CARD_IO *io = &mission_card.io;
    uint8 *buffer = mission_card.records.header;
    uint32 value, checksum, index, descriptor, event, retry;
    sint32 attempts;
    FUNCTION_MARKER(0x800E57F0, "ACE3.BIN");
    if (!mission_card_stop_polling())
        return 0;
    if ((sint16)io->phase == 0)
    {
        io->phase = (uint16)(1);
        io->retry_count = (uint16)(0);
    }
    if (io->pending == 0)
    {
        if ((sint32)slot >= 0)
        {
            value = buffer[8];
            buffer[8] = (value | (1u << (slot & 31)));
            value = (memcpy(&wire_word_0, buffer + 12, sizeof(wire_word_0)), wire_word_0);
            (wire_word_1 = (value + 1), (void)memcpy(buffer + 12, &wire_word_1, sizeof(wire_word_1)));
        }
        else
        {
            buffer[8] = (0);
            (wire_word_2 = (0), (void)memcpy(buffer + 12, &wire_word_2, sizeof(wire_word_2)));
        }
        checksum = 0x41434533;
        for (index = 0; index < 31; ++index)
            checksum ^= (memcpy(&wire_word_3, buffer + 4 + index * 4, sizeof(wire_word_3)), wire_word_3);
        (wire_word_4 = (checksum), (void)memcpy(buffer, &wire_word_4, sizeof(wire_word_4)));
        descriptor = game_card_open_account(port, 0x8002);
        io->descriptor = (uint32)(descriptor);
        if (descriptor == 0xFFFFFFFFu)
        {
            attempts = (sint16)io->retry_count;
            retry = io->retry_count;
            if (attempts < 3)
            {
                io->retry_count = (uint16)(retry + 1);
                return 0;
            }
            goto failure;
        }
        game_bios_seek_guest(descriptor, 640, 0);
        descriptor = io->descriptor;
        writePSX((sint32)descriptor, buffer, GAME_CARD_DIRECTORY_BYTES);
        io->pending = (uint8)(1);
    }
    event = mission_card_poll_info_event(port, 0);
    if (event == 0xFFFFFFFFu)
        return 0;
    game_bios_close_guest(io->descriptor);
    io->pending = (uint8)(0);
    if (event)
    {
        attempts = (sint16)io->retry_count;
        retry = io->retry_count;
        if (attempts < 3)
        {
            io->retry_count = (uint16)(retry + 1);
            return 0;
        }
        goto failure;
    }
    if (finalize == 1)
        mission_card_begin_polling();
    return 1;
failure:
    mission_card_begin_polling();
    io->phase = (uint16)(0);
    return 0xFFFFFFFFu;
}

uint32 mission_card_find_current_account(void)
{
    uint32 wire_word_0;
    const uint8 *header = mission_card.records.header;
    uint8 *account;
    uint32 index, identity, expected;
    const char *name, *current_name;
    FUNCTION_MARKER(0x800E46A0, "ACE3.BIN");
    for (index = 0; index < 3; ++index)
    {
        if ((header[8] >> index) & 1)
        {
            account = mission_card_account_record(index);
            name = game_card_record_name(account);
            current_name = game_runtime.account.name;
            if (strcmp(name, current_name) == 0)
            {
                account = mission_card_account_record(index);
                identity = (memcpy(&wire_word_0, account + 4, sizeof(wire_word_0)), wire_word_0);
                expected = game_runtime.progress.account_id;
                if (identity == expected)
                    return index;
            }
        }
    }
    return 0xFFFFFFFFu;
}

uint32 mission_card_read_directory(uint32 port, uint32 finalize)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    GAME_CARD_IO *io = &mission_card.io;
    uint8 *buffer = mission_card.records.header;
    uint32 descriptor, event, retry, checksum, index, value;
    sint32 attempts;
    FUNCTION_MARKER(0x800E4EF0, "ACE3.BIN");
    if (!mission_card_stop_polling())
        return 0;
    if ((sint16)io->phase == 0)
    {
        io->phase = (uint16)(1);
        io->retry_count = (uint16)(0);
    }
    if (io->pending == 0)
    {
        descriptor = game_card_open_account(port, 0x8001);
        io->descriptor = (uint32)(descriptor);
        if (descriptor == 0xFFFFFFFFu)
        {
            attempts = (sint16)io->retry_count;
            retry = io->retry_count;
            if (attempts < 3)
            {
                io->retry_count = (uint16)(retry + 1);
                return 0;
            }
            mission_card_begin_polling();
            io->phase = (uint16)(0);
            return 0xFFFFFFFFu;
        }
        game_bios_seek_guest(descriptor, 640, 0);
        descriptor = io->descriptor;
        readPSX((sint32)descriptor, buffer, GAME_CARD_DIRECTORY_BYTES);
        io->pending = (uint8)(1);
    }
    event = mission_card_poll_info_event(port, 0);
    if (event == 0xFFFFFFFFu)
        return 0;
    game_bios_close_guest(io->descriptor);
    io->pending = (uint8)(0);
    checksum = 0x41434533;
    for (index = 0; index < 31; ++index)
        checksum ^= (memcpy(&wire_word_0, buffer + 4 + index * 4, sizeof(wire_word_0)), wire_word_0);
    value = (memcpy(&wire_word_1, buffer, sizeof(wire_word_1)), wire_word_1);
    if (value != checksum || (memcpy(&wire_word_2, buffer + 4, sizeof(wire_word_2)), wire_word_2) != 0x990426)
    {
        mission_card_begin_polling();
        io->phase = (uint16)(0);
        return 2;
    }
    if (event)
    {
        attempts = (sint16)io->retry_count;
        retry = io->retry_count;
        if (attempts < 3)
        {
            io->retry_count = (uint16)(retry + 1);
            return 0;
        }
        io->phase = (uint16)(0);
        mission_card_begin_polling();
        return 0xFFFFFFFFu;
    }
    io->phase = (uint16)(0);
    if (finalize == 1)
        mission_card_begin_polling();
    return 1;
}

uint32 mission_card_read_accounts(uint32 port, uint32 finalize)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    GAME_CARD_IO *io = &mission_card.io;
    const uint8 *header = mission_card.records.header;
    uint8 *buffer;
    uint32 descriptor, event, index, checksum, retry, value;
    sint32 slot, attempts;
    FUNCTION_MARKER(0x800E5100, "ACE3.BIN");
    if (!mission_card_stop_polling())
        return 0;
    slot = (sint16)io->record_index;
    value = header[8];
    if ((value >> ((uint32)slot & 31)) & 1)
    {
        if (!mission_card_stop_polling())
            return 0;
        if ((sint16)io->phase == 0)
        {
            io->phase = (uint16)(1);
            io->retry_count = (uint16)(0);
        }
        if (io->pending == 0)
        {
            descriptor = game_card_open_account(port, 0x8001);
            io->descriptor = (uint32)(descriptor);
            if (descriptor == 0xFFFFFFFFu)
            {
                attempts = (sint16)io->retry_count;
                retry = io->retry_count;
                if (attempts < 3)
                {
                    io->retry_count = (uint16)(retry + 1);
                    return 0;
                }
                goto failure;
            }
            slot = (sint16)io->record_index;
            game_bios_seek_guest(descriptor, (sint32)((uint32)slot * 1152 + 768), 0);
            slot = (sint16)io->record_index;
            descriptor = io->descriptor;
            buffer = mission_card_account_record((uint32)slot);
            readPSX((sint32)descriptor, buffer, GAME_CARD_ACCOUNT_RECORD_BYTES);
            io->pending = (uint8)(1);
        }
        event = mission_card_poll_info_event(port, 0);
        if (event == 0xFFFFFFFFu)
            return 0;
        game_bios_close_guest(io->descriptor);
        io->pending = (uint8)(0);
        if (event)
        {
            attempts = (sint16)io->retry_count;
            retry = io->retry_count;
            if (attempts < 3)
            {
                io->retry_count = (uint16)(retry + 1);
                return 0;
            }
            goto failure;
        }
        slot = (sint16)io->record_index;
        buffer = mission_card_account_record((uint32)slot);
        checksum = 0x41434533;
        for (index = 0; index < 287; ++index)
            checksum ^= (memcpy(&wire_word_0, buffer + 4 + index * 4, sizeof(wire_word_0)), wire_word_0);
        slot = (sint16)io->record_index;
        buffer = mission_card_account_record((uint32)slot);
        if ((memcpy(&wire_word_1, buffer, sizeof(wire_word_1)), wire_word_1) != checksum)
        {
            mission_card_begin_polling();
            io->phase = (uint16)(0);
            io->record_index = (uint16)(0);
            return 2;
        }
    }
    slot = (sint16)io->record_index;
    value = io->record_index;
    if (slot < 2)
    {
        io->record_index = (uint16)(value + 1);
        return 0;
    }
    io->record_index = (uint16)(0);
    if (finalize == 1)
        mission_card_begin_polling();
    return 1;
failure:
    mission_card_begin_polling();
    io->phase = (uint16)(0);
    io->record_index = (uint16)(0);
    return 0xFFFFFFFFu;
}

uint32 mission_card_read_missions(uint32 port, uint32 account, uint32 finalize)
{
    uint16 wire_halfword_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    GAME_CARD_IO *io = &mission_card.io;
    uint8 *record;
    uint8 *buffer;
    uint32 mask, descriptor, event, index, checksum, retry, value;
    sint32 slot, attempts;
    FUNCTION_MARKER(0x800E53A0, "ACE3.BIN");
    if (!mission_card_stop_polling())
        return 0;
    buffer = mission_card_account_record(account);
    slot = (sint16)io->record_index;
    mask = (sint16)(memcpy(&wire_halfword_0, buffer + 22, sizeof(wire_halfword_0)), wire_halfword_0);
    if ((mask >> ((uint32)slot & 31)) & 1)
    {
        if (!mission_card_stop_polling())
            return 0;
        if ((sint16)io->phase == 0)
        {
            io->phase = (uint16)(1);
            io->retry_count = (uint16)(0);
        }
        if (io->pending == 0)
        {
            descriptor = game_card_open_account(port, 0x8001);
            io->descriptor = (uint32)(descriptor);
            if (descriptor == 0xFFFFFFFFu)
            {
                attempts = (sint16)io->retry_count;
                retry = io->retry_count;
                if (attempts < 3)
                {
                    io->retry_count = (uint16)(retry + 1);
                    return 0;
                }
                goto failure;
            }
            slot = (sint16)io->record_index;
            game_bios_seek_guest(descriptor, (sint32)(account * 2304 + 4224 + (uint32)slot * 384), 0);
            slot = (sint16)io->record_index;
            descriptor = io->descriptor;
            record = mission_card_mission_record((uint32)slot);
            readPSX((sint32)descriptor, record, GAME_CARD_MISSION_RECORD_BYTES);
            io->pending = (uint8)(1);
        }
        event = mission_card_poll_info_event(port, 0);
        if (event == 0xFFFFFFFFu)
            return 0;
        game_bios_close_guest(io->descriptor);
        io->pending = (uint8)(0);
        if (event)
        {
            attempts = (sint16)io->retry_count;
            retry = io->retry_count;
            if (attempts < 3)
            {
                io->retry_count = (uint16)(retry + 1);
                return 0;
            }
            goto failure;
        }
        slot = (sint16)io->record_index;
        record = mission_card_mission_record((uint32)slot);
        checksum = 0x41434533;
        for (index = 0; index < 95; ++index)
            checksum ^= (memcpy(&wire_word_1, record + 4 + index * 4, sizeof(wire_word_1)), wire_word_1);
        slot = (sint16)io->record_index;
        record = mission_card_mission_record((uint32)slot);
        if ((memcpy(&wire_word_2, record, sizeof(wire_word_2)), wire_word_2) != checksum)
        {
            mission_card_begin_polling();
            io->phase = (uint16)(0);
            io->record_index = (uint16)(0);
            return 2;
        }
    }
    slot = (sint16)io->record_index;
    value = io->record_index;
    if (slot < 6)
    {
        io->record_index = (uint16)(value + 1);
        return 0;
    }
    if (finalize == 1)
        mission_card_begin_polling();
    io->phase = (uint16)(0);
    io->record_index = (uint16)(0);
    return 1;
failure:
    io->phase = (uint16)(0);
    mission_card_begin_polling();
    io->record_index = (uint16)(0);
    return 0xFFFFFFFFu;
}

uint32 mission_card_free_blocks(uint32 port)
{
    uint32 wire_word_0;
    uint8 pattern[16], record[40];
    uint16 remaining = 15;
    uint32 size, adjusted, blocks;
    void *found;

    FUNCTION_MARKER(0x800E4DB0, "ACE3.BIN");
    if (!mission_card_stop_polling())
        return 0xFFFFFFFFu;
    sint32 pattern_bytes = snprintf((char *)pattern, sizeof(pattern), "bu%1d0:*.*", (sint32)port);
    if (pattern_bytes < 0 || (uint32)pattern_bytes >= sizeof(pattern))
        abort();
    found = firstfile((const char *)pattern, record);
    while (found)
    {
        if (!memchr(record, 0, sizeof(record)))
            abort();
        size = (memcpy(&wire_word_0, record + 24, sizeof(wire_word_0)), wire_word_0);
        adjusted = size + 8191;
        if ((sint32)adjusted < 0)
            adjusted = size + 16382;
        blocks = ((sint32)adjusted >> 13);
        remaining = (remaining - blocks);
        found = nextfile(record);
    }
    mission_card_begin_polling();
    return (sint16)remaining;
}

uint32 mission_card_account_present(uint32 account)
{
    FUNCTION_MARKER(0x800E4324, "ACE3.BIN");
    if (account >= 3)
        return 0xFFFFFFFFu;
    return (mission_card.records.header[8] >> account) & 1;
}

uint32 mission_card_account_mask(void)
{
    FUNCTION_MARKER(0x800E456C, "ACE3.BIN");
    return mission_card.records.header[8];
}

uint32 mission_card_write_account(uint32 port, uint32 account, uint32 slot, uint32 finalize)
{
    uint16 wire_halfword_0;
    uint16 wire_halfword_1;
    uint16 wire_halfword_2;
    uint16 wire_halfword_3;
    uint16 wire_halfword_4;
    uint16 wire_halfword_5;
    uint16 wire_halfword_6;
    uint32 wire_word_7;
    uint32 wire_word_8;
    GAME_CARD_IO *io = &mission_card.io;
    uint8 *buffer;
    uint8 *source;
    uint32 value, checksum, index, descriptor, event, retry;
    sint32 attempts;
    FUNCTION_MARKER(0x800E5A28, "ACE3.BIN");
    if (!mission_card_stop_polling())
        return 0;
    if ((sint16)io->phase == 0)
    {
        io->phase = (uint16)(1);
        io->retry_count = (uint16)(0);
    }
    if (slot < 6)
    {
        (wire_halfword_0 = (slot), (void)memcpy(mission_card_account_record(3) + 20, &wire_halfword_0, sizeof(wire_halfword_0)));
        source = mission_card_account_record(account);
        buffer = mission_card_account_record(3);
        value = (memcpy(&wire_halfword_1, source + 22, sizeof(wire_halfword_1)), wire_halfword_1);
        (wire_halfword_2 = (value), (void)memcpy(buffer + 22, &wire_halfword_2, sizeof(wire_halfword_2)));
        buffer = mission_card_account_record(3);
        value = (memcpy(&wire_halfword_3, buffer + 22, sizeof(wire_halfword_3)), wire_halfword_3);
        (wire_halfword_4 = (value | (1u << slot)), (void)memcpy(buffer + 22, &wire_halfword_4, sizeof(wire_halfword_4)));
    }
    else if (slot == 6)
    {
        (wire_halfword_5 = (0), (void)memcpy(mission_card_account_record(3) + 20, &wire_halfword_5, sizeof(wire_halfword_5)));
        (wire_halfword_6 = (0), (void)memcpy(mission_card_account_record(3) + 22, &wire_halfword_6, sizeof(wire_halfword_6)));
    }
    if (!io->pending)
    {
        checksum = 0x41434533;
        buffer = mission_card_account_record(3);
        for (index = 0; index < 287; ++index)
            checksum ^= (memcpy(&wire_word_7, buffer + 4 + index * 4, sizeof(wire_word_7)), wire_word_7);
        buffer = mission_card_account_record(3);
        (wire_word_8 = (checksum), (void)memcpy(buffer, &wire_word_8, sizeof(wire_word_8)));
        descriptor = game_card_open_account(port, 0x8002);
        io->descriptor = (uint32)(descriptor);
        if (descriptor == 0xFFFFFFFFu)
        {
            attempts = (sint16)io->retry_count;
            retry = io->retry_count;
            if (attempts < 3)
            {
                io->retry_count = (uint16)(retry + 1);
                return 0;
            }
            mission_card_begin_polling();
            io->phase = (uint16)(0);
            return 0xFFFFFFFFu;
        }
        game_bios_seek_guest(descriptor, account * 1152 + 768, 0);
        descriptor = io->descriptor;
        buffer = mission_card_account_record(3);
        writePSX((sint32)descriptor, buffer, GAME_CARD_ACCOUNT_RECORD_BYTES);
        io->pending = (uint8)(1);
    }
    event = mission_card_poll_info_event(port, 0);
    if (event == 0xFFFFFFFFu)
        return 0;
    game_bios_close_guest(io->descriptor);
    io->pending = (uint8)(0);
    if (event)
    {
        attempts = (sint16)io->retry_count;
        retry = io->retry_count;
        if (attempts < 3)
        {
            io->retry_count = (uint16)(retry + 1);
            return 0;
        }
        io->phase = (uint16)(0);
        mission_card_begin_polling();
        return 0xFFFFFFFFu;
    }
    if (finalize == 1)
        mission_card_begin_polling();
    return 1;
}

uint32 mission_card_write_mission(uint32 port, uint32 account, uint32 slot, uint32 finalize)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    GAME_CARD_IO *io = &mission_card.io;
    uint8 *buffer;
    uint32 checksum, index, descriptor, event, retry;
    sint32 attempts;
    FUNCTION_MARKER(0x800E5CA4, "ACE3.BIN");
    if (!mission_card_stop_polling())
        return 0;
    if ((sint16)io->phase == 0)
    {
        io->phase = (uint16)(1);
        io->retry_count = (uint16)(0);
    }
    if (!io->pending)
    {
        checksum = 0x41434533;
        buffer = mission_card_mission_record(6);
        for (index = 0; index < 95; ++index)
            checksum ^= (memcpy(&wire_word_0, buffer + 4 + index * 4, sizeof(wire_word_0)), wire_word_0);
        buffer = mission_card_mission_record(6);
        (wire_word_1 = (checksum), (void)memcpy(buffer, &wire_word_1, sizeof(wire_word_1)));
        descriptor = game_card_open_account(port, 0x8002);
        io->descriptor = (uint32)(descriptor);
        if (descriptor == 0xFFFFFFFFu)
        {
            attempts = (sint16)io->retry_count;
            retry = io->retry_count;
            if (attempts < 3)
            {
                io->retry_count = (uint16)(retry + 1);
                return 0;
            }
            goto failure;
        }
        game_bios_seek_guest(descriptor, slot * 384 + account * 2304 + 4224, 0);
        descriptor = io->descriptor;
        buffer = mission_card_mission_record(6);
        writePSX((sint32)descriptor, buffer, GAME_CARD_MISSION_RECORD_BYTES);
        io->pending = (uint8)(1);
    }
    event = mission_card_poll_info_event(port, 0);
    if (event == 0xFFFFFFFFu)
        return 0;
    game_bios_close_guest(io->descriptor);
    io->pending = (uint8)(0);
    if (event)
    {
        attempts = (sint16)io->retry_count;
        retry = io->retry_count;
        if (attempts < 3)
        {
            io->retry_count = (uint16)(retry + 1);
            return 0;
        }
        goto failure;
    }
    if (finalize == 1)
        mission_card_begin_polling();
    return 1;
failure:
    mission_card_begin_polling();
    io->phase = (uint16)(0);
    return 0xFFFFFFFFu;
}

void mission_card_poll_frame(void)
{
    FUNCTION_MARKER(0x800E4858, "ACE3.BIN");
    mission_card_poll_tick();
}

void mission_card_reset_directory_alias(void)
{
    FUNCTION_MARKER(0x800E3B5C, "ACE3.BIN");
    mission_card_reset_directory();
}

uint32 mission_card_initialize_accounts_alias(void)
{
    FUNCTION_MARKER(0x800E3E14, "ACE3.BIN");
    return mission_card_initialize_accounts();
}

void mission_card_select_account_copy(uint32 account)
{
    FUNCTION_MARKER(0x800E30A4, "ACE3.BIN");
    memmove(mission_card_account_record(3), mission_card_account_record(account), GAME_CARD_ACCOUNT_RECORD_BYTES);
}

void mission_card_encode_current_account(void)
{
    uint32 wire_word_0;
    uint16 wire_halfword_1;
    uint16 wire_halfword_2;
    uint16 wire_halfword_3;
    uint16 wire_halfword_4;
    uint16 wire_halfword_5;
    uint16 wire_halfword_6;
    uint16 wire_halfword_7;
    uint16 wire_halfword_8;
    uint32 wire_word_9;
    uint16 wire_halfword_10;
    uint32 wire_word_11;
    uint32 wire_word_12;
    uint32 wire_word_13;
    uint32 wire_word_14;
    uint32 wire_word_15;
    uint32 wire_word_16;
    uint32 wire_word_17;
    uint8 *account;
    uint8 *target;
    uint32 identity, group, index, first, second, value;
    uint32 row, bit, count, target_offset, result;
    FUNCTION_MARKER(0x800E2AB4, "ACE3.BIN");
    account = mission_card_account_record(3);
    identity = game_runtime.progress.account_id;
    (wire_word_0 = (identity), (void)memcpy(account + 4, &wire_word_0, sizeof(wire_word_0)));
    for (group = 0; group < 6; ++group)
    {
        pad_profile_encode(account + 708 + group * 68, &pad_profiles[group]);
        value = game_input.flags;
        account[1124] = ((value & 4) == 0);
    }
    for (group = 0; group < 2; ++group)
    {
        count = group ? 2 : 8;
        const uint8 *unlocks = group ? game_runtime.progress.weapon_unlocked : game_runtime.progress.aircraft_unlocked;
        target_offset = group ? 24 : 26;
        for (row = 0; row < count; ++row)
        {
            target = account + target_offset + row;
            *target = (0);
            for (bit = 0; bit < 8; ++bit)
            {
                first = unlocks[row * 8 + bit];
                second = *target;
                *target = (second | ((first & 1) << bit));
            }
        }
    }
    for (index = 0; index < 60; ++index)
    {
        value = game_runtime.progress.mission_time[0][index];
        (wire_halfword_1 = (value), (void)memcpy(account + 34 + index * 2, &wire_halfword_1, sizeof(wire_halfword_1)));
        value = game_runtime.progress.mission_time[1][index];
        (wire_halfword_2 = (value), (void)memcpy(account + 154 + index * 2, &wire_halfword_2, sizeof(wire_halfword_2)));
        value = game_runtime.progress.mission_time[2][index];
        (wire_halfword_3 = (value), (void)memcpy(account + 274 + index * 2, &wire_halfword_3, sizeof(wire_halfword_3)));
        value = game_runtime.progress.mission_time[3][index];
        (wire_halfword_4 = (value), (void)memcpy(account + 394 + index * 2, &wire_halfword_4, sizeof(wire_halfword_4)));
    }
    for (index = 0; index < 30; ++index)
    {
        for (group = 0; group < 4; ++group)
        {
            first = game_runtime.progress.mission_rating[group][index * 2];
            second = game_runtime.progress.mission_rating[group][index * 2 + 1];
            account[514 + index + group * 30] = ((first & 15) | ((second & 15) << 4));
        }
    }
    for (group = 0; group < 2; ++group)
    {
        count = group ? 45 : 20;
        const uint8 *status = group ? game_runtime.progress.scene_status : game_runtime.progress.archive_status;
        target_offset = group ? 654 : 634;
        for (row = 0; row < count; ++row)
        {
            target = account + target_offset + row;
            *target = (0);
            for (bit = 0; bit < 4; ++bit)
            {
                first = status[row * 4 + bit];
                second = *target;
                *target = (second | ((first & 3) << (bit * 2)));
            }
        }
    }
    value = (uint16)game_runtime.display.screen[0];
    (wire_halfword_5 = (value), (void)memcpy(account + 1120, &wire_halfword_5, sizeof(wire_halfword_5)));
    value = (uint16)game_runtime.display.screen[1];
    (wire_halfword_6 = (value), (void)memcpy(account + 1122, &wire_halfword_6, sizeof(wire_halfword_6)));
    value = audio_settings.volume[0];
    account[1117] = (value);
    value = audio_settings.volume[1];
    account[1118] = (value);
    value = audio_settings.flags;
    account[1116] = (value);
    value = (uint16)game_runtime.progress.total_count;
    (wire_halfword_7 = (value), (void)memcpy(account + 704, &wire_halfword_7, sizeof(wire_halfword_7)));
    value = (uint16)game_runtime.progress.total_losses;
    (wire_halfword_8 = (value), (void)memcpy(account + 706, &wire_halfword_8, sizeof(wire_halfword_8)));
    value = game_runtime.progress.record_slot;
    (wire_word_9 = (0), (void)memcpy(account + 1128, &wire_word_9, sizeof(wire_word_9)));
    (wire_halfword_10 = (value), (void)memcpy(account + 20, &wire_halfword_10, sizeof(wire_halfword_10)));
    if (game_runtime.options.captions & 1)
        (wire_word_11 = (1), (void)memcpy(account + 1128, &wire_word_11, sizeof(wire_word_11)));
    if (game_input.flags & 1)
    {
        value = (memcpy(&wire_word_12, account + 1128, sizeof(wire_word_12)), wire_word_12);
        (wire_word_13 = (value | 2), (void)memcpy(account + 1128, &wire_word_13, sizeof(wire_word_13)));
    }
    if (game_input.flags & 2)
    {
        value = (memcpy(&wire_word_14, account + 1128, sizeof(wire_word_14)), wire_word_14);
        (wire_word_15 = (value | 4), (void)memcpy(account + 1128, &wire_word_15, sizeof(wire_word_15)));
    }
    value = game_runtime.account.hold_action_gate;
    result = 1;
    if (value == 1)
    {
        result = (memcpy(&wire_word_16, account + 1128, sizeof(wire_word_16)), wire_word_16) | 8;
        (wire_word_17 = (result), (void)memcpy(account + 1128, &wire_word_17, sizeof(wire_word_17)));
    }
    for (index = 17; index > 0; --index)
        account[1131 + index] = (0);
    {
        (void)(uint32)(result);
        return;
    }
}

static void mission_card_store_account_name(uint8 *record)
{
    const char *name = game_runtime.account.name;
    const char *end = memchr(name, 0, sizeof(game_runtime.account.name));

    if (!end)
        abort();
    memcpy(record + 8, name, (size_t)(end - name) + 1);
}

void mission_card_prepare_account(uint32 account)
{
    uint16 wire_halfword_0;
    uint8 *record;
    FUNCTION_MARKER(0x800E3658, "ACE3.BIN");
    if (account == 0xFFFFFFFFu)
        record = mission_card_account_record(3);
    else
    {
        if ((sint32)account >= 3)
            return;
        record = mission_card_account_record(account);
    }
    (wire_halfword_0 = (0), (void)memcpy(record + 22, &wire_halfword_0, sizeof(wire_halfword_0)));
    mission_card_store_account_name(record);
}

const char *mission_card_account_name(uint32 account)
{
    FUNCTION_MARKER(0x800E425C, "ACE3.BIN");
    if (!account)
        return game_card_record_name(mission_card_account_record(3));
    if ((sint32)account >= 4)
        return "";
    return game_card_record_name(mission_card_account_record(account - 1));
}

void mission_card_update_current_name(void)
{
    FUNCTION_MARKER(0x800E2A7C, "ACE3.BIN");
    mission_card_store_current_name();
}

void mission_card_store_current_name(void)
{
    FUNCTION_MARKER(0x800E2A8C, "ACE3.BIN");
    mission_card_store_account_name(mission_card_account_record(3));
}

uint32 mission_card_current_record_mask(void)
{
    uint16 wire_halfword_0;
    FUNCTION_MARKER(0x800E458C, "ACE3.BIN");
    return (sint16)(memcpy(&wire_halfword_0, mission_card_account_record(3) + 22, sizeof(wire_halfword_0)), wire_halfword_0);
}

const char *mission_card_mission_label(uint32 slot)
{
    const uint8 *mission;
    uint32 index;
    FUNCTION_MARKER(0x800E44E0, "ACE3.BIN");
    mission = mission_card_mission_record(slot & 255);
    index = mission[275];
    if (index < 61)
        return title_mission_labels[index];
    return "----";
}

const char *mission_card_difficulty_label(uint32 slot)
{
    const uint8 *mission;
    uint32 index;
    FUNCTION_MARKER(0x800E435C, "ACE3.BIN");
    mission = mission_card_mission_record(slot & 255);
    index = mission[274];
    if (index < 4)
        return title_difficulty_labels[index];
    /* preserve the original label for difficulty 4 accepted by native saves */
    if (index == 4)
        return title_difficulty_labels[0];
    return "----";
}

const char *mission_card_unit_label(uint32 slot)
{
    const uint8 *mission;
    uint32 index;
    FUNCTION_MARKER(0x800E4518, "ACE3.BIN");
    mission = mission_card_mission_record(slot & 255);
    index = mission[275];
    if (index >= sizeof(title_mission_units))
        return "----";
    index = title_mission_units[index];
    if (index < 6)
        return title_unit_labels[index];
    return "----";
}

const char *mission_card_control_label(uint32 account)
{
    uint8 *pointer;
    uint32 index;
    FUNCTION_MARKER(0x800E45AC, "ACE3.BIN");
    pointer = mission_card_account_record(account);
    index = pointer[1124];
    if (index < 2)
        return title_difficulty_labels[index];
    return "----";
}

uint32 mission_card_mission_present(uint32 account, uint32 slot)
{
    uint16 wire_halfword_0;
    uint8 *pointer;
    FUNCTION_MARKER(0x800E42AC, "ACE3.BIN");
    if (account == 0xFFFFFFFFu)
        pointer = mission_card_account_record(3);
    else
        pointer = mission_card_account_record(account);
    return (uint32)((sint16)(memcpy(&wire_halfword_0, pointer + 22, sizeof(wire_halfword_0)), wire_halfword_0) >> (slot & 31)) & 1;
}

void mission_card_encode_current_mission(void)
{
    uint32 wire_word_0;
    uint16 wire_halfword_1;
    uint8 *mission;
    uint32 value, index, low, high;
    FUNCTION_MARKER(0x800E2F44, "ACE3.BIN");
    mission = mission_card_mission_record(6);
    value = game_runtime.progress.total_time;
    (wire_word_0 = (value), (void)memcpy(mission + 276, &wire_word_0, sizeof(wire_word_0)));
    value = (uint8)game_runtime.selection.mission;
    mission[275] = (value);
    value = (uint8)game_runtime.selection.difficulty;
    mission[274] = (value);
    for (index = 0; index < 60; ++index)
    {
        value = game_runtime.progress.mission_time[4][index];
        (wire_halfword_1 = (value), (void)memcpy(mission + 4 + index * 2, &wire_halfword_1, sizeof(wire_halfword_1)));
        value = (uint8)game_runtime.progress.mission_count[index];
        mission[154 + index] = (value);
        value = (uint8)game_runtime.progress.mission_losses[index];
        mission[214 + index] = (value);
        value = (uint8)game_runtime.selection.aircraft;
        mission[280] = (value);
        value = game_runtime.selection.weapon_code[0];
        mission[281] = (value);
        value = game_runtime.selection.weapon_code[1];
        mission[282] = (value);
        value = game_runtime.selection.weapon_code[2];
        mission[283] = (value);
    }
    for (index = 0; index < 30; ++index)
    {
        low = game_runtime.progress.mission_rating[4][index * 2];
        high = game_runtime.progress.mission_rating[4][index * 2 + 1];
        mission[124 + index] = ((low & 15) | ((high & 15) << 4));
    }
    for (index = 100; index > 0; --index)
        mission[283 + index] = (0);
    return;
}
