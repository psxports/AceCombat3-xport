#ifndef MENU_STATE_H
#define MENU_STATE_H

#include "psx.h"
#include "menu_options.h"

typedef struct
{
    uint32 packet_storage[2][2048];
    uint32 packet_pages[2];
    uint32 packet_cursor;
    uint32 frame_count;
    uint32 phase_ticks;
    uint32 idle_ticks;
    uint32 card_result_ticks;
    uint32 recovery_notice_offset;
    uint16 screen;
    uint16 screen_step;
    uint32 card_events_open;
    uint32 transfer_phase;
    uint32 audio_cursor;
    uint16 difficulty_cursor;

    struct
    {
        uint32 status;
        uint32 recovered;
        uint32 mission;
    } transfer;

    uint16 panel;
    uint16 panel_step;
    uint8 exit_requested;
    uint8 link_selection;
    uint8 card_port;
    uint8 keyboard_key;
    uint8 keyboard_bank;
    uint32 keyboard_arena_begin;
    uint8 account_selection;
    uint8 link_preview_ticks;
    uint8 record_selection;
    uint8 record_action;
    uint8 card_notice;
    uint8 card_port_hidden;
    uint8 record_banner;
    uint8 record_mode;
} TITLE_MENU_STATE;

extern TITLE_MENU_STATE title_menu;

typedef union
{
    struct
    {
        uint8 kind;
        uint8 phase;
        uint8 close_requested;
        uint8 action;
    };

    MENU_OPTIONS options;
} MENU_HEADER;

typedef union
{
    struct
    {
        uint8 axis_cursor;
        uint8 button_cursor;
        uint8 axis_action;
        uint8 button_action;
        uint8 axis_phase;
        uint8 button_phase;
    };

    uint8 values[6];
} MENU_BINDING_CURSOR;

typedef struct
{
    MENU_HEADER header;
    uint8 main_choice;
    uint8 detail_choice;
    uint8 options_choice;
    uint32 packet_cursor;
    uint16 profile_kind;
    uint16 mapping_selection;
    uint32 binding_phase;
    uint8 binding_tab;
    uint8 restore_choice;
    MENU_BINDING_CURSOR binding;
    uint32 pad_pending;
    uint32 pad_refresh;
    uint32 previous_pad_kind;
    uint16 saved_screen_x;
    uint16 saved_screen_y;
    uint32 arena_begin;
} MENU_STATE;

typedef char MENU_HEADER_LANES_CHECK[sizeof(MENU_HEADER) == 4 ? 1 : -1];
typedef char MENU_BINDING_CURSOR_LANES_CHECK[sizeof(MENU_BINDING_CURSOR) == 6 ? 1 : -1];

extern MENU_STATE menu_state;
void menu_detail_begin(void);
void menu_state_reset(void);
uint32 menu_state_load_begin(void);
uint32 menu_state_return_load_begin(void);
uint32 menu_bind_options_assets(uint32 resource, const uint8 *data, uint32 data_bytes);
void menu_state_ready(void);
uint32 menu_state_return_ready(void);
uint32 menu_state_frame(void);

void title_menu_draw_account_actions(void);

void title_menu_draw_keyboard(void);

uint32 title_menu_disc_transfer_begin(void);
uint32 title_menu_disc_transfer_tick(void);
void title_account_reset_records(void);
uint32 title_menu_keyboard_begin(void);
uint32 title_menu_keyboard_tick(void);
uint32 title_menu_confirm_name_begin(void);
uint32 title_menu_confirm_name_tick(void);
uint32 title_menu_difficulty_begin(void);
uint32 title_menu_difficulty_tick(void);
uint32 title_account_id_seed(void);
const char *title_menu_account_name(sint32 index);
void title_account_reset_campaign(void);
uint32 title_disc_transfer_status(void);
void title_disc_transfer_set_mission(uint32 resource);
uint32 title_disc_transfer_begin(void);
uint32 title_disc_transfer_poll(void);
uint32 title_disc_transfer_recovery_ready(void);
uint32 title_disc_transfer_recovery_finish(void);
uint8 title_mission_disc(uint32 mission);

uint32 title_menu_initialize(void);
uint32 title_menu_frame(void);
void title_menu_draw_start_pulse(uint32 ordering, uint32 packet);
uint32 title_menu_start_begin(void);
uint32 title_menu_start_tick(void);
uint32 title_menu_link_begin(void);
uint32 title_menu_link_tick(void);
void title_menu_account_begin(void);
uint32 title_menu_primary_tick(void);
uint32 title_menu_records_begin(void);
uint32 title_menu_records_tick(void);
uint32 title_menu_exit_begin(void);
uint32 title_menu_exit_tick(void);
void title_menu_draw_account_name(uint32 x, uint32 y, uint32 label);
void title_menu_draw_name_hint(void);
void title_menu_draw_account_slots(uint32 layout, uint32 hide_selection);

uint32 title_menu_configure_begin(void);
uint32 title_menu_configure_tick(void);
uint32 title_menu_audio_begin(void);
uint32 title_menu_audio_tick(void);
uint32 title_menu_screen_begin(void);
uint32 title_menu_screen_tick(void);
uint32 title_menu_brightness_begin(void);
uint32 title_menu_brightness_tick(void);
uint32 title_menu_save_offer_begin(void);
uint32 title_menu_save_offer_tick(void);
uint32 title_menu_without_save_begin(void);
uint32 title_menu_without_save_tick(void);

void title_menu_reserve_wait(void);
uint32 title_menu_wait_counter(void);
uint32 title_menu_wait_step(void);

uint32 title_bootstrap_begin(void);
uint32 title_bootstrap_bind_assets(void);
uint32 title_bootstrap_tick(void);

void title_menu_reserve_legacy_packets(void);
void title_menu_initialize_hook(void);

uint32 title_records_wait_cards_begin(void);
uint32 title_records_wait_cards_tick(void);
uint32 title_records_unavailable_begin(void);
uint32 title_records_unavailable_tick(void);
uint32 title_records_select_card_begin(void);
uint32 title_records_select_card_tick(void);
uint32 title_records_info_notice_begin(void);
uint32 title_records_info_notice_tick(void);
uint32 title_records_load_notice_begin(void);
uint32 title_records_load_notice_tick(void);
uint32 title_records_no_account_begin(void);
uint32 title_records_no_account_tick(void);
uint32 title_records_load_begin(void);
uint32 title_records_read_directory_tick(void);
uint32 title_records_read_accounts_tick(void);
uint32 title_records_read_notice_tick(void);
uint32 title_records_select_account_begin(void);
uint32 title_records_select_account_tick(void);
uint32 title_records_apply_account_begin(void);
uint32 title_records_apply_account_tick(void);
uint32 title_records_read_missions_tick(void);
uint32 title_records_mission_notice_tick(void);
uint32 title_records_special_entry_available(void);
void title_records_draw_mission_preview(uint32 slot, uint32 selection);
uint32 title_records_select_mission_begin(void);
uint32 title_records_select_mission_tick(void);
uint32 title_records_difficulty_begin(void);
uint32 title_records_difficulty_tick(void);
uint32 title_records_disc_recovery_begin(void);
uint32 title_records_check_disc_tick(void);
uint32 title_records_recovery_wait_tick(void);
uint32 title_records_recovery_finish_tick(void);

uint32 title_save_wait_cards_begin(void);
uint32 title_save_wait_cards_tick(void);
uint32 title_save_unavailable_begin(void);
uint32 title_save_unavailable_tick(void);
uint32 title_save_select_card_begin(void);
uint32 title_save_select_card_tick(void);
uint32 title_save_info_notice_begin(void);
uint32 title_save_info_notice_tick(void);
uint32 title_save_format_confirm_begin(void);
uint32 title_save_format_confirm_tick(void);
uint32 title_save_format_tick(void);
uint32 title_save_check_file_tick(void);
uint32 title_save_check_space_tick(void);
uint32 title_save_space_notice_tick(void);
uint32 title_save_create_file_tick(void);
uint32 title_save_write_icons_tick(void);
uint32 title_save_initialize_directory_tick(void);
uint32 title_save_file_notice_tick(void);
uint32 title_save_read_begin(void);
uint32 title_save_read_directory_tick(void);
uint32 title_save_read_accounts_tick(void);
uint32 title_save_read_notice_tick(void);
uint32 title_save_select_account_begin(void);
uint32 title_save_select_account_tick(void);
uint32 title_save_overwrite_begin(void);
uint32 title_save_overwrite_tick(void);
void title_save_write_begin(void);
uint32 title_save_write_directory_tick(void);
uint32 title_save_write_account_tick(void);
void title_save_write_notice_tick(void);
uint32 title_save_success_tick(void);
uint32 title_save_disc_recovery_begin(void);
uint32 title_save_check_disc_tick(void);
uint32 title_save_recovery_wait_tick(void);
uint32 title_save_recovery_finish_tick(void);

const char *title_records_difficulty_label(uint32 selection);
const char *title_records_mission_label(uint32 selection);
const char *title_records_unit_label(uint32 selection);
const char *title_records_control_label(uint32 slot);

uint32 mission_menu_brightness_frame(void);
void mission_menu_screen_frame(void);
void mission_menu_screen_tick(void);
uint32 mission_menu_records_begin(void);
uint32 mission_menu_records_tick(void);
void mission_menu_account_frame(void);
void mission_menu_account_begin(void);
uint32 mission_menu_account_tick(void);
void mission_menu_account_fade_tick(void);
void mission_menu_account_finish_tick(void);

uint32 mission_menu_movie_begin(void);
void mission_menu_movie_frame(void);
void mission_menu_movie_tick(void);

uint32 mission_menu_exit_begin(void);
uint32 mission_menu_exit_wait_ring(void);
uint32 mission_menu_exit_fade_tick(void);
uint32 mission_menu_exit_finish(void);

extern uint32 mission_menu_exit_fade_ticks;

void menu_calibration_update(void);
void mission_menu_options_wait_frame(void);
void mission_menu_options_wait_tick(void);
void mission_menu_options_select_frame(void);
void mission_menu_options_select_tick(void);
void mission_menu_options_leave_frame(void);
void mission_menu_options_leave_tick(void);

void mission_menu_main_open_tick(void);
void mission_menu_main_select_tick(void);
uint32 mission_menu_main_close_tick(void);
void mission_menu_detail_open_tick(void);
void mission_menu_detail_frame(void);
void mission_menu_detail_select_tick(void);
void mission_menu_detail_close_tick(void);

void mission_menu_start_item_motion(uint32 group, uint32 item);

#endif
