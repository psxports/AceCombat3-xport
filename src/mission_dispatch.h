#ifndef MISSION_DISPATCH_H
#define MISSION_DISPATCH_H

#include <stddef.h>

#include "psx.h"
#include "resource_queue.h"
#include "dialogue_sequence.h"
#include "ui_items.h"
#include "menu_options.h"
#include "menu_state.h"
#include "hud_instruments.h"
#include "mission_spawn.h"
#include "flight_dynamics.h"

enum
{
    MISSION_SCENARIO_COUNT = 24,
    MISSION_AIRCRAFT_MODEL_BUFFER_BYTES = 0x4000
};

typedef struct
{
    uint8 kind;
    uint8 members[2];
    uint8 fixed_target_idx;
} MISSION_SCENARIO;

extern const MISSION_SCENARIO mission_scenarios[MISSION_SCENARIO_COUNT];

typedef struct
{
    uint8 mission_arg;
    uint8 script_arg;
    uint8 scene_arg;
    uint8 movie_arg;
    uint8 restore_arg;
    uint8 saved_scene_arg;
    uint8 mode;
    uint8 flags;
    uint8 event_id;
    uint8 music_id;
    uint8 current_mission_arg;
    uint8 effect_id;
    uint8 dispatch_count;
} MISSION_SCENE_REQUEST;

typedef struct
{
    uint16 ticks;
    uint16 progress;
    uint8 scr_type;
    uint8 style_reserved;
    uint8 resource_kind;
    uint32 pending;
    uint32 read_sequence;
    HUD_INSTRUMENTS instruments;
    uint32 animation_ticks;
} MISSION_LOADING_STATE;

typedef struct
{
    uint32 completed_bytes;
    uint32 read_cursor;
    uint32 total_bytes;
} MISSION_FRONTEND_LOAD;

typedef struct
{
    const uint32 *entries;
    uint32 count, base;
    const uint8 *data;
    uint32 data_bytes, encoded_base;
} MISSION_IMAGE_DIRECTORY;

uint32 mission_image_address(uint32 resource, uint32 offset);
void mission_image_sections(uint32 resource, const uint8 *data, uint32 data_bytes, uint32 *sections, uint32 count);
MISSION_IMAGE_DIRECTORY mission_image_directory_bind(uint32 resource, uint32 end, uint32 base, const uint8 *data, uint32 data_bytes);
uint32 mission_image_directory_at(const MISSION_IMAGE_DIRECTORY *directory, uint32 index);

typedef struct
{
    uint32 height_map;
    uint32 dialogue;
    uint32 briefing_events;
    uint32 briefing_textures;
    uint32 result_textures[2];
    uint32 briefing_speech;
    uint32 map_textures;
    uint32 interface_textures;
    const uint8 *data;
    uint32 bytes, encoded_base;
} MISSION_BUNDLE_DIRECTORY;


typedef struct
{
    DIALOGUE_SEQUENCE_SOURCE briefing;
    DIALOGUE_SEQUENCE_SOURCE result[4];
    const uint8 *legacy_result_tail;
    RESOURCE_ASSET_SOURCE parent;
} MISSION_DIALOGUE_DIRECTORY;

DIALOGUE_SEQUENCE_SOURCE mission_dialogue_result(uint8 kind);

typedef struct MISSION_BRIEFING_EVENT
{
    sint16 time;
    uint8 type, slot;
    sint16 parameter, x, y, z, angle_or_rows, reserved;
} MISSION_BRIEFING_EVENT;

typedef char mission_briefing_event_layout[sizeof(MISSION_BRIEFING_EVENT) == 16 ? 1 : -1];

typedef struct
{
    uint32 arena_begin;
    MISSION_IMAGE_DIRECTORY row_image_table;
    MISSION_IMAGE_DIRECTORY secondary_image_table;
    MISSION_IMAGE_DIRECTORY detail_image_table;
    RESOURCE_STREAM_BUFFER selection_buffer;
    uint32 shared_menu_textures;
    uint32 pad_settings_textures;
    MISSION_BUNDLE_DIRECTORY mission_directory;
    uint32 detail_script;
    uint32 audio_script;
    uint32 aircraft_preview_script;
    uint32 sound_test_assets;
    uint32 save_screen_assets;
    uint8 *aircraft_profiles;
    uint32 aircraft_profile_bytes;
    const uint32 *aircraft_model_table;
    uint32 aircraft_model_count;
    uint32 aircraft_bundle_end;
    MISSION_DIALOGUE_DIRECTORY dialogue_sequences;
    uint32 menu_work_buffer;
    UI_COMMAND_STREAM save_screen_ui;
    uint32 pad_profile_images[4];
    RESOURCE_ASSET_SOURCE frontend_source;
    RESOURCE_ASSET_SOURCE options_source;
    const MISSION_BRIEFING_EVENT *mission_events, *mission_events_end;
} MISSION_SCENE_ASSETS;

typedef struct
{
    uint32 arena_begin;
    uint32 count;
    uint8 page;
    uint8 row;
    uint8 ids[149];
} MISSION_SCENE_ARCHIVE;

typedef struct
{
    SPRT sprites[11];
    uint32 reserved[5];
    DR_MODE mode;
} ARCHIVE_BROWSER_PACKET_BANK;

typedef struct
{
    ARCHIVE_BROWSER_PACKET_BANK banks[2];
} ARCHIVE_BROWSER_PACKETS;

typedef char archive_browser_packets_layout_check[
    sizeof(SPRT) == 20 && sizeof(DR_MODE) == 12 &&
    offsetof(ARCHIVE_BROWSER_PACKET_BANK, mode) == 240 &&
    sizeof(ARCHIVE_BROWSER_PACKET_BANK) == 252 &&
    sizeof(ARCHIVE_BROWSER_PACKETS) == 504 &&
    8 + sizeof(ARCHIVE_BROWSER_PACKETS) <= 608 ? 1 : -1];

typedef struct
{
    uint32 packet_link;
    uint32 packet_cursor;
    ARCHIVE_BROWSER_PACKETS *packets;
    uint32 count;
    uint32 scroll_dir;
    uint32 detail_icons;
    uint8 phase;
    uint8 exit_mode;
    uint8 page;
    uint8 row;
    uint8 detail_page;
    uint8 ids[79];
} MISSION_ARCHIVE_BROWSER;

typedef struct
{
    uint32 finish_tick;
    uint32 fade_tick;
    uint32 notice_tick;
    uint8 fade_started;
    uint8 stream_phase;
} MISSION_TRANSITION_STATE;

typedef struct
{
    uint32 opening_progress;
    uint32 closing_progress;
    uint32 effect_ticks;
    uint32 phase;
    uint32 total_frames;
    uint32 active;
    uint32 skip_requested;
    uint16 remaining_frames;
    uint32 stream_ready;
    uint16 block_frames;
    uint16 frame_index;
    uint32 wait_ticks;
} MISSION_SCRIPT_PLAYBACK;

typedef struct
{
    uint32 mode;
    const FLIGHT_TARGET_PROFILE *target;
    const FLIGHT_RELATIVE_PROFILE *relative;
    const FLIGHT_MODEL_OFFSET *offsets;
} MISSION_FLIGHT_SELECTION;

typedef struct
{
    uint8 root_kind;
    uint8 root_phase;
    uint8 root_variant;
    uint8 exit_requested;
    uint16 mode;
    uint16 mode_phase;
    uint16 transition_phase;
    uint16 menu_phase;
    uint16 init_mode;
    uint16 init_phase;
    uint32 next_scene;
    uint32 phase_timer;
    uint32 ui_timer;
    uint32 account_mode;
    uint32 frontend_frames;
    uint32 menu_init_delay;
    uint32 playback_requested;
    uint32 debrief_movie_phase;
    uint32 menu_arrow_delay;
    uint32 faction;
    uint32 audio_wait_ticks;
    uint32 scenario_idx;
    uint32 scenario_mask;
    uint8 start_pose_count;
    ACTOR_POSE start_pose;
    MISSION_FLIGHT_SELECTION flight_selection;
    MISSION_SCRIPT_PLAYBACK script_playback;
    MISSION_TRANSITION_STATE transition;
    MISSION_SCENE_REQUEST request;
    MISSION_LOADING_STATE loading;
    MISSION_FRONTEND_LOAD frontend_load;
    MISSION_SCENE_ASSETS assets;
    MISSION_SCENE_ARCHIVE scene_archive;
    MISSION_ARCHIVE_BROWSER archive_browser;
} MISSION_FLOW;

typedef struct
{
    uint32 phase;
    uint32 status;
    uint32 recovered;
    uint32 mission;
} MISSION_RESOURCE_LOAD;

enum
{
    MISSION_SELECTOR_CAPACITY = 60
};

typedef struct
{
    uint8 phase;
    uint8 result;
    uint16 action;
    uint8 col;
    uint8 row;
    uint16 count;
    uint8 entries[MISSION_SELECTOR_CAPACITY];
} MISSION_SELECTOR;

enum
{
    MISSION_DEBRIEF_MARKER_SLOTS = 256,
    MISSION_DEBRIEF_KIND_SLOTS = 48,
    MISSION_DEBRIEF_SPECIAL_SLOTS = 8,
    MISSION_DEBRIEF_LABEL_COUNT = 53,
    MISSION_DEBRIEF_LABEL_BYTES = 11
};

typedef struct
{
    uint8 x;
    uint8 y;
    uint8 category;
    uint8 visible;
} MISSION_DEBRIEF_MARKER;

typedef struct
{
    char label[40];
    uint8 kind;
    uint8 count;
} MISSION_DEBRIEF_ROW;

typedef struct
{
    uint8 phase;
    uint8 event_idx;
    uint16 revealed_seconds;
    MISSION_DEBRIEF_MARKER markers[MISSION_DEBRIEF_MARKER_SLOTS];
    MISSION_DEBRIEF_ROW kind_rows[MISSION_DEBRIEF_KIND_SLOTS];
    MISSION_DEBRIEF_ROW special_rows[MISSION_DEBRIEF_SPECIAL_SLOTS];
    uint8 kind_count;
    uint8 special_count;
    uint8 phase_done;
    uint16 delay_ticks;
} MISSION_DEBRIEF;

extern const char mission_debrief_labels[MISSION_DEBRIEF_LABEL_COUNT][MISSION_DEBRIEF_LABEL_BYTES];

/* live serialized statistics, without inferred physics units */
#pragma pack(push, 1)
typedef struct
{
    uint8 retained_tag;
    uint8 chart_levels[5];
    sint16 hundredths[3];
    uint32 integer_value;
    sint16 signed_integer_value;
    uint8 widget5_base, widget6_value;
    uint8 retained[4];
} AIRCRAFT_PROFILE;
#pragma pack(pop)

typedef char AIRCRAFT_PROFILE_LAYOUT_CHECK[
    sizeof(AIRCRAFT_PROFILE) == 24 &&
    offsetof(AIRCRAFT_PROFILE, chart_levels) == 1 &&
    offsetof(AIRCRAFT_PROFILE, hundredths) == 6 &&
    offsetof(AIRCRAFT_PROFILE, integer_value) == 12 &&
    offsetof(AIRCRAFT_PROFILE, signed_integer_value) == 16 &&
    offsetof(AIRCRAFT_PROFILE, widget5_base) == 18 &&
    offsetof(AIRCRAFT_PROFILE, widget6_value) == 19 &&
    offsetof(AIRCRAFT_PROFILE, retained) == 20 ? 1 : -1];

enum
{
    AIRCRAFT_PREVIEW_CAPACITY = 64
};

typedef struct
{
    SPRT primary[2][2];
    SPRT secondary[2][2];
    DR_MODE modes[2];
} MISSION_WEAPON_PACKET_BANKS;

typedef char mission_weapon_packet_layout_check[
    sizeof(SPRT) == 20 && sizeof(DR_MODE) == 12 &&
    offsetof(MISSION_WEAPON_PACKET_BANKS, secondary) == 80 &&
    offsetof(MISSION_WEAPON_PACKET_BANKS, modes) == 160 &&
    sizeof(MISSION_WEAPON_PACKET_BANKS) == 184 ? 1 : -1];

typedef struct
{
    uint16 phase;
    uint16 action;
    uint32 flags;
    uint32 desc_saved;
    uint32 variant;
    uint32 group;
    uint32 variant_count;
    uint32 unlock_flags;
    uint16 preview[AIRCRAFT_PREVIEW_CAPACITY];
    uint16 preview_count;
    uint8 weapon_tab;
    uint8 weapon_idx[2];
    uint8 pending_weapon_idx[2];
    uint8 weapon_count[2];
    uint8 weapon_info;
    uint32 selection_bookmark;
    MISSION_WEAPON_PACKET_BANKS *weapon_packets;
    uint32 model_buffer;
    uint8 *model_data;
    const AIRCRAFT_PROFILE *profile;
    const AIRCRAFT_PROFILE *prev_profile;
    AIRCRAFT_PROFILE saved_profile;
} MISSION_AIRCRAFT;

extern MISSION_FLOW mission_flow;
extern MISSION_RESOURCE_LOAD mission_resource_load;
extern MISSION_SELECTOR mission_selector;
extern MISSION_DEBRIEF mission_debrief;
extern MISSION_AIRCRAFT mission_aircraft;
extern const AIRCRAFT_PROFILE aircraft_fallback_profile;

uint32 mission_aircraft_group(uint32 mission, sint32 aircraft);
uint32 mission_aircraft_count(uint32 mission, uint32 group);
uint32 mission_model_variant(uint32 mission, uint32 variant);
uint32 mission_aircraft_id(uint32 mission, uint32 variant);

const AIRCRAFT_PROFILE *aircraft_profile_at(uint32 index);
uint32 aircraft_model_index(uint32 mission, uint32 aircraft);
void mission_unlock_aircraft(uint32 mission);
void mission_unlock_weapons(uint32 mission);
uint32 aircraft_weapon_id(uint32 mission, uint32 aircraft, uint32 kind, uint32 record);
uint32 aircraft_weapon_count(uint32 mission, uint32 aircraft, uint32 kind);
uint32 aircraft_default_weapon_slot(uint32 mission, uint32 aircraft, uint32 kind);

uint32 aircraft_preview_draw_info(void);
uint32 aircraft_selection_input(void);
uint32 aircraft_selection_init(void);

uint32 mission_menu_init(void);

void scene_archive_load_page(uint32 page);
void scene_archive_load_detail(uint32 item);
uint32 scene_archive_init(void);
uint32 scene_archive_update(void);
uint32 scene_archive_resume(void);

void archive_browser_load_page(uint32 page);
void archive_browser_load_detail(uint32 item);
void archive_browser_init(void);
void archive_browser_load_row(uint32 page, uint32 item);
uint32 archive_browser_enter_list(void);
void archive_browser_transition(void);
uint32 archive_browser_wait_entry(void);
void archive_browser_reset_detail(void);
void archive_browser_draw_detail(void);
uint32 archive_browser_update_list(uint32 countdown);
uint32 archive_browser_input(void);
uint32 archive_browser_update_detail(void);
void archive_browser_leave_detail(void);
void archive_browser_dispatch(void);

uint32 mission_dispatch_scene_request(void);
void mission_flow_reset(void);
void mission_asset_load_complete(void);
uint32 mission_asset_load_poll(void);
uint32 mission_load_begin(void);
uint32 aircraft_load_begin(void);

void mission_dispatch_root(uint32 kind, uint32 phase);
void mission_dispatch_mode(uint32 kind, uint32 phase);
void mission_dispatch_detail(uint32 kind, uint32 phase, MENU_HEADER *header);
uint32 mission_dispatch_detail_aux(uint32 phase, MENU_OPTIONS *options);
void mission_dispatch_transition(uint32 phase);
uint32 mission_dispatch_loading(uint32 phase, uint32 mode, RESOURCE_STREAM_BUFFER buffer);

uint32 mission_dispatch_debrief(uint32 phase);

uint32 mission_dispatch_loading_screen(uint32 phase);

uint32 mission_dispatch_init(uint32 phase);

void mission_dispatch_select(uint32 phase);

uint32 mission_dispatch_load(uint32 phase);

void mission_dispatch_aircraft(uint32 phase);

void mission_dispatch_selection(uint32 kind, uint32 phase);

void mission_dispatch_event(uint32 type, uint32 event_time);
void mission_dispatch_map(uint32 kind, uint32 phase);

uint32 mission_debrief_load_assets(uint32 loaded, const uint8 *data, uint32 data_bytes);

uint32 mission_debrief_draw_trace(uint32 packet);
uint32 mission_debrief_draw_recent_trace(uint32 packet);
void mission_debrief_tick_phase(void);
void mission_debrief_reveal_trace(void);
void mission_debrief_clear_markers(void);
uint32 mission_debrief_special_missions_complete(uint32 mission);
void mission_debrief_commit_result(void);
void mission_debrief_clear_event_rows(void);
uint32 mission_frontend_initialize(void);
uint32 mission_flow_resolve_faction(sint32 mission);

uint32 mission_save_leave_mode(uint32 mode);
uint32 mission_save_offer_begin(void);
uint32 mission_save_offer_tick(void);
uint32 mission_save_wait_cards_begin(void);
void mission_save_wait_cards_tick(void);
uint32 mission_save_unavailable_begin(void);
uint32 mission_save_unavailable_tick(void);
uint32 mission_save_begin(void);
uint32 mission_save_wait_ring(void);
uint32 mission_save_fade_background(void);
uint32 mission_save_frame(void);
uint32 mission_save_update(void);

uint32 mission_save_draw_account_slots(uint32 hide_selected, uint32 layout);
void mission_save_select_card_begin(void);
uint32 mission_save_select_card_tick(void);
uint32 mission_save_selected_card_missing_begin(void);
uint32 mission_save_selected_card_missing_tick(void);
uint32 mission_save_poll_wait_begin(void);
uint32 mission_save_poll_wait_tick(void);
uint32 mission_save_info_notice_begin(void);
uint32 mission_save_info_notice_tick(void);
uint32 mission_save_format_begin(void);
void mission_save_format_transition_tick(void);
uint32 mission_save_format_confirm_tick(void);
uint32 mission_save_format_tick(void);
uint32 mission_save_format_notice_tick(void);
uint32 mission_save_load_begin(void);
uint32 mission_save_read_directory_tick(void);
uint32 mission_save_read_accounts_tick(void);
uint32 mission_save_find_account_tick(void);
uint32 mission_save_load_account_tick(void);
void mission_save_read_missions_tick(void);
uint32 mission_save_read_notice_tick(void);
uint32 mission_save_create_offer_begin(void);
uint32 mission_save_create_offer_tick(void);
uint32 mission_save_check_file_tick(void);
uint32 mission_save_check_space_tick(void);
uint32 mission_save_space_notice_tick(void);
uint32 mission_save_create_file_tick(void);
uint32 mission_save_write_icons_tick(void);
uint32 mission_save_initialize_directory_tick(void);
uint32 mission_save_file_notice_tick(void);
uint32 mission_save_select_account_begin(void);
uint32 mission_save_select_account_frame(void);
uint32 mission_save_select_account_tick(void);
uint32 mission_save_overwrite_account_begin(void);
uint32 mission_save_overwrite_account_tick(void);
uint32 mission_save_account_file_check_tick(void);
uint32 mission_save_create_account_file_tick(void);
uint32 mission_save_write_account_icons_tick(void);
uint32 mission_save_write_account_directory_tick(void);
uint32 mission_save_write_account_frame(void);
uint32 mission_save_write_account_tick(void);
uint32 mission_save_account_write_notice_tick(void);
uint32 mission_save_select_mission_begin(void);
uint32 mission_save_select_mission_tick(void);
uint32 mission_save_overwrite_mission_begin(void);
uint32 mission_save_overwrite_mission_tick(void);
uint32 mission_save_confirm_mission_tick(void);
uint32 mission_save_commit_mission_begin(void);
uint32 mission_save_write_mission_account_tick(void);
uint32 mission_save_write_mission_record_tick(void);
uint32 mission_save_mission_write_notice_tick(void);
uint32 mission_save_success_begin(void);
uint32 mission_save_success_tick(void);
uint32 mission_save_leave_offer_begin(void);
uint32 mission_save_leave_offer_tick(void);
uint32 mission_save_check_disc_tick(void);
uint32 mission_save_recovery_wait_tick(void);
uint32 mission_save_recovery_finish_tick(void);

void mission_save_draw_mission_preview(uint32 slot, uint32 account, uint32 hide_data);

void mission_detail_notify_noop(void);
void mission_select_grid_begin(void);
uint32 mission_select_grid_frame(void);
uint32 mission_select_grid_tick(uint32 cooldown);
void mission_select_grid_finish(void);
uint32 mission_select_difficulty_begin(void);
uint32 mission_select_difficulty_frame(void);
uint32 mission_select_difficulty_tick(void);
void mission_select_difficulty_finish(void);
uint32 mission_select_load_begin(void);
uint32 mission_select_begin(void);
uint32 mission_select_advance_phase(void);
uint32 mission_select_tick(void);

void aircraft_selection_commit_weapons_frame(void);
void aircraft_selection_commit_weapons(void);
uint32 aircraft_selection_begin_input(void);
uint32 aircraft_selection_input_frame(void);
uint32 aircraft_selection_finish(void);
uint32 aircraft_selection_wait_weapons(void);
uint32 aircraft_selection_weapons_input(void);
uint32 aircraft_selection_finish_weapons_frame(void);
uint32 aircraft_selection_finish_weapons(void);
void mission_select_load_preview(uint32 row, uint32 column);
void mission_select_load_row(uint32 row);
void mission_select_load_detail(uint32 column);

uint32 aircraft_selection_rating_unlocks(void);
void aircraft_selection_update_weapons(void);
uint32 aircraft_selection_frame(void);
uint32 aircraft_selection_tick(void);
uint32 mission_select_load_tick(void);
uint32 aircraft_selection_model_variant(uint32 mission, uint32 variant);
uint32 mission_load_status(void);
void mission_load_set_mission(uint32 mission);
uint32 mission_load_begin_media_check(void);
uint32 mission_load_check_media(void);
uint32 mission_load_wait_recovery(void);
uint32 mission_load_finish_recovery(void);

uint32 mission_frontend_frame(void);
void mission_frontend_idle_phase(void);
uint32 mission_frontend_load_resource124(void);
void mission_frontend_wait_resource124(void);
uint32 mission_frontend_load_resource125(void);
void mission_frontend_wait_resource125(void);
void mission_launch_begin(void);
uint32 mission_launch_wait_frame(void);
uint32 mission_launch_wait_ring(void);
uint32 mission_launch_fade_tick(void);
uint32 mission_resume_confirm_tick(void);
uint32 mission_replay_prompt_begin(void);
uint32 mission_replay_prompt_tick(void);
void mission_dispatch_current_mode(void);

void mission_image_optional_data_hook(void);

void mission_frontend_choose_root_tick(void);
uint32 mission_frontend_root_frame(void);
void mission_dialogue_dispatch_media_request(void);
void mission_dialogue_dispatch_media_cancel(void);
void mission_dialogue_media_request_begin(void);
uint32 mission_dialogue_media_request_check(void);
void mission_dialogue_media_request_wait(void);
void mission_dialogue_media_request_finish(void);
void mission_dialogue_media_request_retry(void);
void mission_dialogue_media_request_recheck(void);
uint32 mission_dialogue_media_cancel_begin(void);
uint32 mission_dialogue_media_cancel_wait(void);
uint32 mission_dialogue_media_cancel_finish(void);
void mission_dialogue_media_cancel_retry(void);

uint32 mission_frontend_load_aircraft_assets(uint32 mode, RESOURCE_STREAM_BUFFER incoming_buffer);
uint32 mission_frontend_queue_aircraft_assets(uint32 mode, RESOURCE_STREAM_BUFFER incoming_buffer);
uint32 mission_frontend_wait_aircraft_assets(void);
uint32 mission_frontend_bind_aircraft_assets(void);
uint32 mission_load_preserve_result(uint32 incoming_v0);
uint32 mission_frontend_load_resource121(void);
uint32 mission_frontend_wait_resource121(void);
uint32 mission_frontend_bind_resource121(void);
uint32 mission_frontend_load_resource122(void);
uint32 mission_frontend_wait_resource122(void);
uint32 mission_frontend_bind_resource122(void);
uint32 mission_frontend_load_resource123(void);
uint32 mission_frontend_wait_resource123(void);
uint32 mission_frontend_bind_resource123(void);
uint32 mission_frontend_wait_options_assets(void);
uint32 mission_frontend_bind_options_assets(void);
uint32 mission_frontend_finish_asset_load(void);
void mission_frontend_root_open_tick(void);
void mission_frontend_root_close_tick(void);
uint32 aircraft_bind_loaded_assets(uint32 loaded, uint8 *data, uint32 data_bytes);

uint32 mission_frontend_initialize_packet_cursor(void);
uint32 mission_frontend_initialize_state(void);
uint32 mission_frontend_fade_in_tick(void);
uint32 mission_frontend_open_ring_frame(void);
uint32 mission_frontend_open_ring(void);
uint32 mission_frontend_wait_open_ring(void);
uint32 mission_frontend_initialize_step(void);
uint32 mission_frontend_begin_archive_transition(void);
uint32 mission_frontend_reset_root_selection(void);
uint32 mission_frontend_cancel_root_selection(void);
void mission_frontend_select_mode(void);
uint32 mission_debrief_wait_assets(void);
uint32 mission_debrief_bind_loaded_assets(void);
uint32 mission_debrief_wait_animation(void);
uint32 mission_frontend_begin_status_screen(void);
uint32 mission_frontend_status_frame(void);
uint32 mission_frontend_status_tick(void);
uint32 mission_frontend_loading_frame(void);
uint32 mission_frontend_begin_debrief(void);
uint32 mission_frontend_debrief_frame(void);
uint32 mission_frontend_debrief_tick(void);
void mission_unlock_archive_for_mission(sint32 mission);

uint32 mission_image_relocate_entries(const uint32 *image_directory, uint32 base, uint32 *native_destination, uint32 native_capacity);
uint32 mission_debrief_draw_map(uint32 packet);
void mission_debrief_reveal_events_frame(void);
void mission_debrief_reveal_events(void);
uint32 mission_debrief_draw_markers(uint32 packet);
void mission_debrief_draw_counts(void);
void mission_debrief_draw_time_frame(void);
void mission_debrief_draw_time(void);
uint32 mission_debrief_frame(void);
uint32 mission_frontend_bind_mission_assets(void);
uint32 mission_frontend_bind_common_assets(void);
uint32 mission_frontend_begin_asset_fade(void);
uint32 mission_frontend_wait_asset_fade(void);
uint32 mission_bind_loaded_assets(uint32 loaded, const uint8 *data, uint32 data_bytes);
uint32 frontend_bind_loaded_assets(uint32 loaded, const uint8 *data, uint32 data_bytes);

uint32 mission_unpack_assets(void);
void aircraft_bind_thrust_metadata(const uint8 *data, uint32 bytes);
uint32 aircraft_unpack_assets(void);

#endif
