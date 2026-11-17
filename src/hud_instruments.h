#ifndef HUD_INSTRUMENTS_H
#define HUD_INSTRUMENTS_H

#include "psx.h"
#include "mission_actor.h"
#include "flight_target.h"

typedef struct HUD_INSTRUMENT_GPU_BANK HUD_INSTRUMENT_GPU_BANK;

enum { HUD_INSTRUMENT_GPU_BYTES = 1060 };

typedef struct
{
    uint32 events;
    uint32 speed;
    uint32 health;
    uint32 ammunition;
    uint32 target_angle_limit;
    uint32 weapon_limit;
    uint32 active_weapons;
    uint32 ground_height;
    uint32 ground_clearance;
    const uint32 *position;
    const uint16 *angles;
    const uint16 *camera_steer;
    const uint32 *camera_mode;
    const uint16 *delta_angles;
    const FLIGHT_TARGET_STATE *target;
} HUD_CONTEXT;

void *hud_native_packet_span(uint32 address, size_t bytes);

uint32 hud_draw(void);
uint32 hud_draw_context(const HUD_CONTEXT *context);
void hud_instruments_draw_context(const HUD_CONTEXT *context);

void hud_target_init(void);
void hud_target_begin(void);
uint32 hud_target_submit(void);
uint32 hud_target_draw(const HUD_CONTEXT *context);
void hud_target_draw_empty(void);
void hud_target_draw_attitude(void);
void hud_target_draw_arrow(uint32 kind, uint32 x, uint32 y);
void hud_target_emit_path3(uint32 first, uint32 second, uint32 third, uint32 phase);
void hud_target_emit_path4(uint32 first, uint32 second, uint32 third, uint32 fourth);

void hud_notice_init(void);
void hud_notice_begin(void);
void hud_notice_draw_frame(const HUD_CONTEXT *context);
void hud_notice_submit_panel(void);
void hud_panel_draw(uint32 size);
void hud_portrait_draw(void);
void hud_panel_reset(void);
void hud_portrait_reset(void);

void hud_panel_update(void);
void hud_panel_close(void);
void hud_panel_open(void);
uint32 hud_panel_get_mode(void);
void hud_portrait_update(void);
void hud_portrait_hide(uint32 mode);
void hud_portrait_show(uint32 speaker, uint32 x, uint32 y);

enum
{
    HUD_RADAR_CONTACT_COUNT = 64,
    HUD_RADAR_MARKER_COUNT = 32,
    HUD_AIM_HISTORY_COUNT = 32
};

typedef struct
{
    uint8 active;
    uint8 kind;
    uint16 ticks;
} HUD_NOTICE;

typedef struct
{
    uint8 flags;
    uint8 shape;
    uint8 color;
    uint8 intensity;
    uint8 heading;
    uint8 height_band;
    uint8 x;
    uint8 y;
    uint32 distance;
} HUD_RADAR_MARK;

typedef struct HUD_GPU_BANK HUD_GPU_BANK;

typedef struct
{
    uint32 arena_begin;
    uint32 bank[2];
    uint32 cur;
    HUD_GPU_BANK *bank_data[2];
    HUD_GPU_BANK *current_data;
    uint32 spr_count, tile_count, line_count, polyline_count;
    uint32 flags;
} HUD_PACKET_STATE;

typedef struct
{
    LINE_F3 paths[128];
    LINE_F2 lines[128];
    POLY_F3 polys[4];
} HUD_TARGET_PACKET_BANK;

typedef char hud_target_packet_bank_layout_check[
    sizeof(LINE_F3) == 24 && sizeof(LINE_F2) == 16 && sizeof(POLY_F3) == 20 &&
    sizeof(HUD_TARGET_PACKET_BANK) == 5200 &&
    offsetof(HUD_TARGET_PACKET_BANK, lines) == 3072 &&
    offsetof(HUD_TARGET_PACKET_BANK, polys) == 5120 ? 1 : -1];

typedef struct
{
    uint32 arena_begin;
    uint32 bank[2];
    uint32 cur;
    uint32 polyline_count, line_count, poly_count;
    HUD_TARGET_PACKET_BANK *bank_data[2];
    HUD_TARGET_PACKET_BANK *native;
} HUD_TARGET_PACKET_STATE;

typedef struct
{
    union
    {
        struct
        {
            SPRT sprites[2];
            TILE tiles[3];
            DR_TPAGE modes[2];
        } panel;
        /* the third sprite initializer overlaps the first panel tile */
        SPRT panel_init[3];
    } prefix;
    SPRT notice[16];
    DR_TPAGE notice_modes[8];
    POLY_FT4 portrait[2];
    TILE portrait_tiles[3];
    DR_TPAGE portrait_modes[2];
} HUD_NOTICE_PACKET_BANK;

typedef char hud_notice_packet_bank_layout_check[
    sizeof(SPRT) == 20 && sizeof(TILE) == 16 && sizeof(POLY_FT4) == 40 && sizeof(DR_TPAGE) == 8 &&
    sizeof(HUD_NOTICE_PACKET_BANK) == 632 &&
    offsetof(HUD_NOTICE_PACKET_BANK, prefix.panel.tiles) == 40 &&
    offsetof(HUD_NOTICE_PACKET_BANK, prefix.panel.modes) == 88 &&
    offsetof(HUD_NOTICE_PACKET_BANK, notice) == 104 &&
    offsetof(HUD_NOTICE_PACKET_BANK, notice_modes) == 424 &&
    offsetof(HUD_NOTICE_PACKET_BANK, portrait) == 488 &&
    offsetof(HUD_NOTICE_PACKET_BANK, portrait_tiles) == 568 &&
    offsetof(HUD_NOTICE_PACKET_BANK, portrait_modes) == 616 ? 1 : -1];

typedef struct
{
    uint32 arena_begin;
    uint32 bank[2];
    uint32 cur;
    uint32 spr_count, mode_count;
    HUD_NOTICE_PACKET_BANK *bank_data[2];
    HUD_NOTICE_PACKET_BANK *native;
} HUD_NOTICE_PACKET_STATE;

typedef struct
{
    uint32 reset_word;
    uint16 width;
    uint8 mode;
} HUD_PANEL_STATE;

typedef struct
{
    uint16 ticks, x, y;
    uint8 mode, image, reset_byte, w, h;
} HUD_PORTRAIT_STATE;

typedef struct
{
    uint16 radius, x, y;
    uint8 mode, target_idx;
} HUD_LOCK_STATE;

enum
{
    HUD_MARKER_LINES,
    HUD_MARKER_POLYLINES,
    HUD_MARKER_LABELS,
    HUD_MARKER_DIGITS,
    HUD_MARKER_POOL_COUNT
};

typedef struct
{
    DR_TPAGE mode;
    LINE_F2 lines[64];
    LINE_F3 polylines[128];
    SPRT labels[64];
    SPRT digits[4];
} HUD_MARKER_PACKET_BANK;

typedef char hud_marker_packet_bank_layout[
    sizeof(HUD_MARKER_PACKET_BANK) == 5464 && sizeof(DR_TPAGE) == 8 &&
    offsetof(HUD_MARKER_PACKET_BANK, lines) == 8 &&
    offsetof(HUD_MARKER_PACKET_BANK, polylines) == 1032 &&
    offsetof(HUD_MARKER_PACKET_BANK, labels) == 4104 &&
    offsetof(HUD_MARKER_PACKET_BANK, digits) == 5384 ? 1 : -1];

typedef struct
{
    uint32 arena_begin;
    uint32 bank[2];
    uint32 cur;
    HUD_MARKER_PACKET_BANK *bank_data[2];
    HUD_MARKER_PACKET_BANK *native;
    uint32 count[HUD_MARKER_POOL_COUNT];
} HUD_MARKER_PACKET_STATE;

enum
{
    HUD_FLIGHT_QUADS,
    HUD_FLIGHT_SHADED_TRIS,
    HUD_FLIGHT_TEXTURED_QUADS,
    HUD_FLIGHT_LINES,
    HUD_FLIGHT_SHADED_LINES,
    HUD_FLIGHT_WAVEFORM,
    HUD_FLIGHT_DIGITS,
    HUD_FLIGHT_FIXED_CHAIN,
    HUD_FLIGHT_SLOT_COUNT
};

typedef struct
{
    DR_TPAGE mode;
    POLY_F4 flat_quads[16];
    POLY_G3 triangles[4];
    POLY_FT4 textured_quads[12];
    LINE_F2 lines[64];
    LINE_G2 gradient_lines[8];
    LINE_G4 waveform;
    SPRT digits[8];
    SPRT indicator;
    POLY_FT3 markers[4];
} HUD_FLIGHT_PACKET_BANK;

typedef struct
{
    uint32 bank[2];
    uint32 cur;
    HUD_FLIGHT_PACKET_BANK *bank_data[2];
    HUD_FLIGHT_PACKET_BANK *native;
    uint32 used[HUD_FLIGHT_SLOT_COUNT];
} HUD_FLIGHT_PACKET_STATE;

typedef struct
{
    uint32 notice_priority;
    uint32 notice_ticks;
    HUD_NOTICE notice_pair[2];
    HUD_NOTICE notice_single;
    uint16 timer_mode;
    uint16 alert_ticks;
    uint8 alert_mode;
    uint8 weapon_lock_mode;
    uint8 number_color;
    uint8 locked_target_idx;
    uint8 held_target_idx;
    uint32 map_scale;
    uint32 display_ticks;
    uint8 dim_color[3];
    uint8 score_name[12];
    uint8 selected_target_idx;
    uint32 aim_distance_parameter;
    HUD_RADAR_MARK radar_contacts[HUD_RADAR_CONTACT_COUNT];
    HUD_RADAR_MARK radar_markers[HUD_RADAR_MARKER_COUNT];
    uint8 primary_color[3];
    uint16 projected_x;
    uint16 projected_y;
    uint32 projected_depth;
    uint8 aim_head;
    uint8 aim_slot;
    uint32 aim_history[HUD_AIM_HISTORY_COUNT][3];
    uint8 waveform_noise[4][38];
    uint8 waveform_curve[4];
    HUD_PACKET_STATE packets;
    HUD_TARGET_PACKET_STATE target_packets;
    HUD_NOTICE_PACKET_STATE notice_packets;
    HUD_PANEL_STATE panel;
    HUD_PORTRAIT_STATE portrait;
    HUD_LOCK_STATE lock;
    HUD_MARKER_PACKET_STATE marker_packets;
    HUD_FLIGHT_PACKET_STATE flight_packets;
    uint32 instrument_banks[2];
    HUD_INSTRUMENT_GPU_BANK *instrument_bank_data[2];
} HUD_CONTROL_STATE;

extern HUD_CONTROL_STATE hud_control;

void hud_speed_init(uint32 dst, SPRT *spr, POLY_F4 *poly);
void hud_altitude_init(uint32 dst, SPRT *spr);
void hud_timer_init(uint32 dst, SPRT *spr);
void hud_health_init(uint32 dst, SPRT *spr);
void hud_gauge_init(SPRT *spr);
void hud_color_marks_init(uint32 dst, SPRT *spr, LINE_F2 *line);
void hud_color_frames_init(uint32 dst, SPRT *spr, LINE_F2 *line);

void hud_instruments_init_bank(uint32 dst, HUD_INSTRUMENT_GPU_BANK *bank);

void hud_instruments_init_banks(void);

void hud_digit_draw(uint32 glyph, uint32 x, uint32 y, uint32 font, SPRT *spr, uint32 color);

HUD_RADAR_MARK *hud_radar_contact(uint32 index);
void hud_init_packet_banks(void);
void hud_radar_init_map(HUD_GPU_BANK *const banks[2]);
void hud_lock_update(void);
void hud_markers_classify(void);
uint32 hud_lock_begin(uint32 index);
uint32 hud_lock_cancel(void);
void hud_lock_animate(void);
void hud_marker_draw_heading(uint32 index);
void hud_marker_draw_corners(uint32 index, uint32 color);
void hud_marker_draw_outline(uint32 index, uint32 color);
void hud_marker_draw_line_frame(uint32 index, uint32 color);
void hud_marker_draw_affiliation(uint32 kind, uint32 x, uint32 y);
void hud_markers_init(void);
void hud_markers_draw(void);
void hud_markers_begin(void);
void hud_markers_submit(void);
void hud_markers_reset(void);
void hud_markers_update(void);
void hud_lock_init(void);
void hud_lock_draw(void);
void hud_map_draw(void);
void hud_map_submit(void);
void hud_radar_draw_frame(const HUD_CONTEXT *context);
void hud_radar_begin(void);
void hud_radar_submit(void);
void hud_radar_draw_scale(void);
void hud_radar_update_scale(void);
void hud_radar_draw_marker(const HUD_RADAR_MARK *marker, HUD_GPU_BANK *bank);
void hud_radar_bind_contact(uint32 index, const MISSION_ACTOR_DEFINITION *definition);

typedef struct
{
    uint32 flags;
    uint32 altitude;
    uint32 speed;
    uint32 health;
    uint32 gauge_value;
    uint32 gauge_count;
    uint32 gauge_dim_count;
    uint8 primary_color[3];
    uint8 secondary_color[3];
    uint16 number_color;
    uint16 timer_mode;
    uint32 timer_ticks;
} HUD_INSTRUMENTS;

uint32 hud_instruments_draw(uint32 object, uint32 dst, const HUD_INSTRUMENTS *instruments);
void hud_instruments_submit(uint32 object, uint32 dst, const HUD_INSTRUMENTS *instruments, HUD_INSTRUMENT_GPU_BANK *bank);
void hud_color_frames_draw(uint32 object, uint32 dst, const HUD_INSTRUMENTS *instruments, HUD_INSTRUMENT_GPU_BANK *bank);
void hud_speed_draw(uint32 object, uint32 dst, const HUD_INSTRUMENTS *instruments, HUD_INSTRUMENT_GPU_BANK *bank);
void hud_altitude_draw(uint32 object, uint32 dst, const HUD_INSTRUMENTS *instruments, HUD_INSTRUMENT_GPU_BANK *bank);
void hud_timer_draw(uint32 object, uint32 dst, const HUD_INSTRUMENTS *instruments, HUD_INSTRUMENT_GPU_BANK *bank);
void hud_color_marks_draw(uint32 object, uint32 dst, const HUD_INSTRUMENTS *instruments, HUD_INSTRUMENT_GPU_BANK *bank);
void hud_gauge_draw(uint32 object, uint32 dst, const HUD_INSTRUMENTS *instruments, HUD_INSTRUMENT_GPU_BANK *bank);
void hud_health_draw(uint32 object, uint32 dst, const HUD_INSTRUMENTS *instruments, HUD_INSTRUMENT_GPU_BANK *bank);

void hud_aim_update(void);
uint32 hud_waveform_draw(uint32 packet, uint32 ordering);

uint32 hud_notice_draw(HUD_NOTICE *notice);
void hud_notice_reset_pair(void);
void hud_notice_reset(void);
void hud_notice_show_pair(uint32 index);
void hud_notice_show(uint32 kind);

void hud_notice_update_warnings(const HUD_CONTEXT *context);

void hud_draw_orientation_tetrahedron(uint32 x, uint32 y);
void hud_draw_compass_segment(uint32 angle, const HUD_CONTEXT *context);

void hud_draw_attitude_indicator(void);

uint32 hud_draw_brand_footer(uint32 ordering, uint32 packet, uint32 strength);
void hud_draw_steering_reference(const HUD_CONTEXT *context);
void hud_draw_compass_ring(const HUD_CONTEXT *context);
void hud_draw_target_direction(void);
void hud_draw_target_angle_frame(const HUD_CONTEXT *context);

void hud_update_alert_colors(const HUD_CONTEXT *context);
void hud_update_panels_and_fade(void);
void hud_project_actor_markers(void);
uint32 hud_marker_radius_for_depth(uint32 distance);
void hud_begin_flight_packets(void);
void hud_submit_flight_packets(void);
void hud_draw_steering_attitude(const HUD_CONTEXT *context);
void hud_draw_center_reference(void);

void hud_draw_mission_scores(void);
uint32 hud_map_world_position(const uint32 position[3], const uint16 origin[2], uint16 output[2]);
void hud_draw_flight_instruments(const HUD_CONTEXT *context);

void hud_initialize_default_colors(void);
void hud_draw_single_notice(void);
void hud_draw_impact_reticle(const HUD_CONTEXT *context);
uint32 hud_estimate_impact_ticks(uint32 distance, uint32 speed);

void hud_draw_pitch_ladder(const HUD_CONTEXT *context);

void hud_initialize_mission(void);
void hud_initialize_flight_packet_banks(void);

#endif
