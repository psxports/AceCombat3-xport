#ifndef DIALOGUE_UI_H
#define DIALOGUE_UI_H

#include "psx.h"
#include "dialogue_video.h"

typedef struct
{
    SPRT lines[2][11];
    DR_MODE modes[2][2];
} DIALOGUE_CAPTION_PACKETS;

typedef struct
{
    uint8 script_idx;
    uint8 speaker;
    uint16 pos;
    DIALOGUE_CAPTION_PACKETS *packets;
} DIALOGUE_CAPTION;

typedef struct
{
    uint32 image_mode;
    uint32 complete;
    uint32 audio_resource;
    uint32 audio_argument[2];
} DIALOGUE_ENTRY;

typedef struct
{
    uint16 completed_entries;
    uint16 wait_for_completion;
    uint32 frame_ticks;
    uint32 active_item;
    uint32 prev_kind;
    uint32 panel_style;
    uint32 item_mask;
    uint32 render_state;
    uint32 wait_ticks;
    uint32 anim_slot;
    uint32 arena_begin;
    DIALOGUE_CAPTION caption;
    DIALOGUE_ENTRY entry;
} DIALOGUE_UI;

extern DIALOGUE_UI dialogue_ui;

void dialogue_ui_reset(void);
uint32 dialogue_ui_open_step(void);

uint32 dialogue_ui_begin_result(void);
uint32 dialogue_ui_briefing_frame(void);
uint32 dialogue_ui_begin_briefing(void);
uint32 dialogue_ui_result_fade_tick(void);
uint32 dialogue_ui_briefing_fade_tick(void);
uint32 dialogue_ui_wait_header_tick(void);
void dialogue_ui_return_account(void);
uint32 dialogue_ui_return_root_tick(void);

void dialogue_ui_draw_movie_progress(void);
uint32 dialogue_ui_draw_effects(uint32 fallback_result);

void dialogue_ui_dispatch_phase(void);
uint32 dialogue_ui_reset_entry_animations(uint32 kind);
void dialogue_ui_configure_entry_layout(uint32 kind);
void dialogue_ui_stop_audio_frame(void);
void dialogue_ui_stop_audio(void);
uint32 dialogue_ui_wait_selection_tick(void);
uint32 dialogue_ui_bind_entry_asset(void);
void dialogue_ui_wait_intro_audio(void);
uint32 dialogue_ui_open_entry_fade_tick(void);
uint32 dialogue_ui_begin_entry(void);
uint32 dialogue_ui_wait_entry_audio(void);
uint32 dialogue_ui_entry_tick(void);
uint32 dialogue_ui_begin_outro_audio(void);
void dialogue_ui_wait_outro_audio(void);
void dialogue_ui_advance_panel_tick(void);
uint32 dialogue_ui_finish_sequence(void);

uint32 dialogue_ui_draw_panel_frame(void);
uint32 dialogue_ui_draw_panel(void);
uint32 dialogue_ui_layout_item(uint32 kind);
uint32 dialogue_ui_is_detail_layout(uint8 kind);
uint32 dialogue_ui_queue_audio_frame(void);
uint32 dialogue_ui_queue_audio(void);
void dialogue_ui_queue_entry_frame(void);
void dialogue_ui_queue_entry_asset(void);
uint32 dialogue_ui_begin_sequence(void);
uint32 dialogue_ui_begin_intro_audio(void);

#endif
