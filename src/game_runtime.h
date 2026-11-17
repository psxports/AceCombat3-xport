#ifndef GAME_RUNTIME_H
#define GAME_RUNTIME_H

#include "psx.h"
#include "resource_queue.h"
#include "game_math.h"
#include "mission_result.h"
#include "scene_collision.h"

enum { GAME_MISSION_COUNT = 60, GAME_AIRCRAFT_COUNT = 37, GAME_DEMO_COUNT = 37 };


typedef struct
{
    uint32 descriptor;
    uint16 record_index;
    uint8 pending;
    uint16 retry_count;
    uint16 phase;
} GAME_CARD_IO;

typedef struct
{
    uint16 info_status;
    uint16 load_status;
    uint8 present;
    uint8 file_found;
    uint8 step;
    uint8 event_result;
    uint8 probe_retries;
} GAME_CARD_PORT;

typedef struct
{
    GAME_CARD_PORT ports[2];
    uint32 events[7];
    uint32 enabled;
    uint32 timeout;
    uint32 next_port;
} GAME_CARD_POLL;

typedef struct
{
    uint8 screen;
    uint8 phase;
    uint8 exit_requested;
    uint8 confirmation;
    uint8 card_port;
    uint8 account_slot;
    uint8 mission_slot;
    uint8 ticks;
    uint8 show_both_ports;
    uint8 hide_prompt_sprite;
    uint8 hide_data;
    uint32 packet_cursor;
    uint16 recovery_indicator;
    uint16 account_detail_mode;
    uint32 arena_bookmark;
} MISSION_SAVE_MENU;

extern MISSION_SAVE_MENU mission_save_menu;

typedef union
{
    uint32 words[2287];
    struct
    {
        SPRT levels[432];
        DR_MODE level_modes[4];
        SPRT orbit[10];
        uint8 preserved[200];
        DR_MODE orbit_modes[2];
        uint8 reserved[36];
    } gpu;
} SOUND_TEST_PACKET_STORAGE;

typedef char sound_test_packet_storage_layout[
    sizeof(SPRT) == 20 && sizeof(DR_MODE) == 12 &&
    sizeof(SOUND_TEST_PACKET_STORAGE) == 9148 &&
    offsetof(SOUND_TEST_PACKET_STORAGE, gpu.level_modes) == 8640 &&
    offsetof(SOUND_TEST_PACKET_STORAGE, gpu.orbit) == 8688 &&
    offsetof(SOUND_TEST_PACKET_STORAGE, gpu.orbit_modes) == 9088 ? 1 : -1];

typedef struct
{
    uint32 packet_link;
    SOUND_TEST_PACKET_STORAGE *packet_data;
    uint16 orbit_tick;
    uint8 option;
    uint8 random_track;
} MISSION_SOUND_TEST;

extern MISSION_SOUND_TEST mission_sound_test;

typedef struct
{
    VECTOR position;
    MATRIX view;
    uint32 speed;
    uint32 move_ticks;
    sint16 heading;
} MISSION_MAP_CAMERA;

extern MISSION_MAP_CAMERA mission_briefing_camera;

enum { MISSION_BRIEFING_OBJECT_COUNT = 32, MISSION_BRIEFING_PANEL_COUNT = 8 };

typedef struct
{
    uint16 icon;
    uint16 motion;
    uint16 ticks;
    uint16 duration;
    uint16 speed;
    uint16 x, y, z;
    SVECTOR angles;
    uint16 screen_x, screen_y;
} MISSION_BRIEFING_OBJECT;

typedef struct
{
    uint8 label;
    uint8 event_slot;
} MISSION_BRIEFING_PANEL;

typedef struct
{
    uint32 left_address, commands_address, right_address;
    SPRT *left;
    DR_MODE *commands;
    SPRT *right;
} MISSION_BRIEFING_PACKET_BANK;

typedef struct
{
    uint32 arena_bookmark;
    MISSION_BRIEFING_PACKET_BANK packet_banks[2];
    SVECTOR marker_position;
    sint32 marker_time;
    const struct MISSION_BRIEFING_EVENT *event_cursor;
    uint16 panel_flags;
    uint8 panel_count;
    uint32 visible_objects;
    uint8 group;
    uint8 phase;
    uint8 exit_mode;
    uint8 intro_widget;
    uint8 intro_stage;
    MISSION_BRIEFING_OBJECT objects[MISSION_BRIEFING_OBJECT_COUNT];
    MISSION_BRIEFING_PANEL panels[MISSION_BRIEFING_PANEL_COUNT];
} MISSION_BRIEFING_STATE;

extern MISSION_BRIEFING_STATE mission_briefing;

typedef struct
{
    const uint8 *data;
    uint32 bytes;
} MISSION_HEIGHT_MAP;

extern MISSION_HEIGHT_MAP mission_briefing_height_map;

typedef struct
{
    uint32 model_buffer;
    uint8 *model_data;
    uint32 detail_bookmark;
    uint8 index;
    uint8 load_requested;
    uint8 exit_action;
} AIRCRAFT_PREVIEW_STATE;

extern AIRCRAFT_PREVIEW_STATE menu_model_preview;

typedef struct
{
    uint8 mission[GAME_MISSION_COUNT];
    uint8 mission_count;
    uint8 aircraft_count;
    uint8 weapon_count;
    uint8 selected;
} MISSION_DETAIL_LIST;

extern MISSION_DETAIL_LIST mission_detail_list;

enum { GAME_CARD_ICON_BLOCK_BYTES = 640, GAME_CARD_DIRECTORY_BYTES = 128 };

enum { GAME_CARD_MISSION_RECORD_BYTES = 384, GAME_CARD_ACCOUNT_RECORD_BYTES = 1152 };

typedef struct
{
    uint8 header[GAME_CARD_DIRECTORY_BYTES];
    uint8 accounts[4][GAME_CARD_ACCOUNT_RECORD_BYTES];
    uint8 missions[7][GAME_CARD_MISSION_RECORD_BYTES];
} GAME_CARD_RECORDS;

typedef struct
{
    GAME_CARD_IO io;
    GAME_CARD_POLL poll;
    GAME_CARD_RECORDS records;
    uint8 icon_block[GAME_CARD_ICON_BLOCK_BYTES];
} GAME_CARD_STATE;

extern GAME_CARD_STATE title_card;
extern GAME_CARD_STATE mission_card;

typedef struct
{
    /* native storage supplied by xport */
    uint8 *begin;
    uint8 *cursor;
} GAME_ARENA;

struct SCENE_TERRAIN_MODEL_HEADER;

typedef struct TERRAIN_MODEL_TABLE
{
    const struct SCENE_TERRAIN_MODEL_HEADER *const *headers;
    uint32 entry_count;
    uint32 guest_base;
    const uint8 *data;
    uint32 bytes;
} TERRAIN_MODEL_TABLE;

const struct SCENE_TERRAIN_MODEL_HEADER *terrain_model_header(const TERRAIN_MODEL_TABLE *models, uint32 id);
RESOURCE_ASSET_SOURCE terrain_model_vertices(const TERRAIN_MODEL_TABLE *models, const struct SCENE_TERRAIN_MODEL_HEADER *header);

typedef struct
{
    const uint8 *records;
    uint32 count, stride;
} SCENE_FACE_GROUP;

SCENE_FACE_GROUP terrain_model_faces(const TERRAIN_MODEL_TABLE *models, const struct SCENE_TERRAIN_MODEL_HEADER *header, uint32 flat);
SCENE_FACE_GROUP scene_model_face_group(const RESOURCE_ASSET_SOURCE *source, uint32 address, uint32 stride);

typedef struct
{
    sint16 near_plane, far_plane;
    sint16 light_angles[2][2];
    sint16 flare_flags;
    uint8 layer_enabled, detail_kind;
    uint32 halo_color;
    uint16 terrain_flags;
    sint16 horizon_height, layer_height[2];
    uint32 palette_primary, palette_secondary, horizon_color;
    uint32 blend_start[2], blend_end[2];
    sint16 light_colors[2][3], ambient_color[3];
} MISSION_RENDER_SETTINGS;

typedef struct
{
    uint16 camera_offsets[3], effect_lines[4];
    uint32 effect_trails[2], effect_center;
    uint32 spawn_vector[2], weapon_vectors[2][2];
} AIRCRAFT_MISSION_METADATA;

typedef struct
{
    uint32 direction_xy, color_xy, edge_xy[2];
    uint32 tip_xy[2], tip_z_word[2];
    uint16 direction_z, color_z, edge_z[2];
    sint16 axis_low[2], axis_high[2];
} AIRCRAFT_THRUST_EMITTER;

typedef struct
{
    uint32 base_rgbc, far_color, extent_xy;
    AIRCRAFT_THRUST_EMITTER emitters[2];
    uint8 count, uses_axes;
} AIRCRAFT_THRUST_METADATA;

enum { MISSION_TERRAIN_CAPTURE_CAPACITY = 16 };

typedef struct
{
    uint16 texture, sample;
    sint16 shift, phase;
    uint8 red, green, blue;
} MISSION_TERRAIN_CAPTURE_RECORD;

typedef struct
{
    MISSION_TERRAIN_CAPTURE_RECORD records[MISSION_TERRAIN_CAPTURE_CAPACITY];
    uint32 count;
} MISSION_TERRAIN_CAPTURE;

typedef struct
{
    struct
    {
        sint16 height;
        uint8 count, slope;
    } layer[2];
    uint32 drift_color, layer_color, horizon_color;
    sint16 horizon_height, detail_density;
    uint8 wide_horizon, noise_attenuation, detail_repeat, detail_count;
    uint8 trail_back_color[3];
} GAME_SCENE_ENVIRONMENT;

typedef struct
{
    const uint8 *data;
    uint32 bytes;
    uint32 legacy_base;
} GAME_SCENE_SCRIPT_DATA;

typedef struct
{
    const uint8 *data;
    uint32 bytes;
    uint32 legacy_base;
} GAME_SCENE_ENTITY_DATA;

typedef struct
{
    const uint8 *data;
    uint32 bytes;
    uint32 legacy_base;
} GAME_SCENE_NORMAL_DATA;

typedef struct
{
    GAME_SCENE_NORMAL_DATA view;
    uint32 offset;
} SCENE_NORMAL_REFERENCE;

const uint8 *scene_normal_data(SCENE_NORMAL_REFERENCE source, uint32 bytes);

struct SCENE_MESH_MODEL_HEADER;
struct SCENE_SPRITE_MODEL_HEADER;

enum { GAME_SCENE_MODEL_HEADER_CAPACITY = 128, GAME_SPRITE_MODEL_HEADER_CAPACITY = 256 + 4, GAME_TERRAIN_MODEL_HEADER_CAPACITY = 512 };

typedef struct
{
    uint16 *tile_map;
    TERRAIN_MODEL_TABLE terrain_models;
    const struct SCENE_MESH_MODEL_HEADER *const *model_headers;
    const RESOURCE_ASSET_SOURCE *model_header_sources;
    uint32 model_header_count;
    const struct SCENE_MESH_MODEL_HEADER *const *aircraft_headers;
    RESOURCE_ASSET_SOURCE aircraft_model_source;
    const struct SCENE_SPRITE_MODEL_HEADER *const *animated_headers;
    uint32 animated_header_count;
    RESOURCE_ASSET_SOURCE animated_model_source;
    GAME_SCENE_NORMAL_DATA normals;
    GAME_SCENE_SCRIPT_DATA scripts;
    GAME_SCENE_ENTITY_DATA entities;
    GAME_SCENE_ENVIRONMENT environment;
} GAME_SCENE_ASSETS;

enum { AIRCRAFT_COMPOSITE_MODEL_COUNT = 12 };
const struct SCENE_MESH_MODEL_HEADER *const *scene_aircraft_composite_headers(const GAME_SCENE_ASSETS *assets);

typedef struct
{
    uint8 primary_count, secondary_count;
    uint16 script_saved_health;
} GAME_ACTOR_RESOURCE_SOURCES;

typedef struct
{
    GAME_SCENE_ASSETS mission_assets;
    uint32 restore_data;
    RESOURCE_ASSET_SOURCE aircraft_model_source;
    RESOURCE_ASSET_SOURCE aircraft_backing;
    const struct SCENE_MESH_MODEL_HEADER *model_header_storage[GAME_SCENE_MODEL_HEADER_CAPACITY];
    RESOURCE_ASSET_SOURCE model_header_source_storage[GAME_SCENE_MODEL_HEADER_CAPACITY];
    const struct SCENE_SPRITE_MODEL_HEADER *animated_header_storage[GAME_SPRITE_MODEL_HEADER_CAPACITY];
    const struct SCENE_TERRAIN_MODEL_HEADER *terrain_header_storage[GAME_TERRAIN_MODEL_HEADER_CAPACITY];
    const struct SCENE_MESH_MODEL_HEADER *aircraft_headers[AIRCRAFT_COMPOSITE_MODEL_COUNT];
    uint32 arena_mark;
    uint32 pending_mode;
    uint8 pending_argument;
} GAME_RESOURCE_CONTEXT;

typedef struct
{
    sint32 x;
    sint32 z;
} GAME_TERRAIN_ORIGIN;

typedef struct
{
    uint32 *entries;
    uint32 word_count;
} GAME_MISSION_ORDERING_VIEW;

typedef struct
{
    uint32 ot;
    uint32 packet_begin;
    uint8 *packet_data;
    uint32 packet_capacity;
    GAME_MISSION_ORDERING_VIEW ot_view;
} GAME_MISSION_PACKET_BANK;

typedef struct
{
    uint32 flags;
    uint32 mode;
    uint8 argument;
    uint32 ticks;
    uint32 ot;
    uint32 packet_cur;
    GAME_MISSION_PACKET_BANK banks[2];
    uint32 render_arena;
} GAME_MISSION_CONTROL;

typedef struct
{
    uint32 min[3];
    uint32 max[3];
} GAME_MISSION_BOUNDS;

typedef struct
{
    uint16 options_sel;
    uint16 confirm_sel;
} GAME_MISSION_MENU;

typedef struct
{
    uint16 offset[2];
    uint16 shift[2];
    XY origin;
    XY center;
} GAME_MISSION_VIEW;

typedef struct
{
    MATRIX camera;
    MATRIX view;
} GAME_MISSION_MATRICES;

typedef struct
{
    MATRIX directions;
    MATRIX colors;
    uint32 back_color[3];
} GAME_MISSION_LIGHTING;

typedef struct
{
    uint16 distance;
    uint16 near_plane;
    uint16 far_plane;
    uint16 origin[2];
    uint16 size[2];
} GAME_CAMERA_PROJ;

typedef struct
{
    uint32 flags;
    uint32 mode;
    uint32 look_target;
    uint32 view_actor;
    uint32 excluded_actor;
    uint32 follow_actor;
    uint32 shot_phase;
    uint32 mode_ticks;
    uint32 shot_index;
    uint32 shot_counter;
} GAME_CAMERA_CONTROL;

typedef struct
{
    uint32 offset[3];
    uint16 angles[3];
    uint16 steer[3];
    uint16 orbit_angles[3];
    uint32 orbit_offset[3];
} GAME_CAMERA_FOLLOW;

typedef struct
{
    NATIVE_POSE pose;
    uint32 min[3];
    uint32 max[3];
} GAME_CAMERA_VIEW;

typedef struct
{
    NATIVE_POSE pose;
} GAME_CAMERA_SHOT_POSE;

typedef struct
{
    NATIVE_POSE pose;
    uint32 velocity[3];
} GAME_CAMERA_SHOT_SUBJECT;

typedef struct
{
    const NATIVE_POSE *first;
    const NATIVE_POSE *second;
    const uint32 *velocity;
    const uint16 *offset;
} GAME_CAMERA_SHOT_CONTEXT;

typedef struct
{
    uint32 phase;
    GAME_CAMERA_SHOT_POSE output;
    uint32 ticks;
    uint32 limit;
    uint32 flags;
    uint32 distance;
} GAME_CAMERA_SHOT_CONTROL;

typedef uint32 (*GAME_CAMERA_SHOT_CALLBACK)(const GAME_CAMERA_SHOT_CONTEXT *context, GAME_CAMERA_SHOT_CONTROL *control, uint32 duration);

typedef struct
{
    GAME_CAMERA_SHOT_CALLBACK callback;
    uint16 duration;
    uint16 smooth;
} GAME_CAMERA_SHOT_DESC;

typedef struct
{
    uint16 offset[3];
    GAME_CAMERA_SHOT_POSE previous;
    GAME_CAMERA_SHOT_SUBJECT subjects[2];
    uint16 target_mode;
    uint16 target_changed;
    GAME_CAMERA_SHOT_CONTROL control;
} GAME_CAMERA_SHOTS;

typedef struct
{
    uint32 position[3];
    uint16 angles[3];
} GAME_CAMERA_DELTA;

typedef struct
{
    uint32 mode;
    uint32 phase;
    uint32 loaded_overlay;
} GAME_SCENE_STATE;

typedef struct
{
    uint16 mission;
    uint16 aircraft;
    uint16 saved_mission;
    uint16 saved_aircraft;
    uint32 difficulty;
    uint8 weapon_code[3];
} GAME_SELECTION;

typedef struct
{
    uint32 return_mode;
    uint32 resume_mode;
    uint8 script_id;
    uint16 resume_count;
    uint8 intro_count;
    uint8 demo_mode;
    uint8 demo_idx;
    uint8 caption_gate;
    uint8 movie_idx;
} GAME_PLAYBACK_FLOW;

typedef struct
{
    /* Preserve the serialized byte with no active game observer */
    uint8 save_reserved;
    uint8 captions;
    uint8 kill_camera;
    uint8 hud_hidden;
    uint8 camera_mode;
    uint16 overlay_toggles;
} GAME_OPTIONS;

typedef struct
{
    char name[12];
    uint16 route_mode;
    uint8 result_kind;
    uint8 mission_failed;
    uint16 loss_count;
    uint8 hold_action_gate;
    /* Preserve the serialized byte with no active game observer */
    uint8 save_reserved;
} GAME_ACCOUNT;

typedef struct
{
    uint32 account_flags;
    uint32 account_id;
    uint32 flow_flags;
    uint8 record_reserved;
    uint8 record_slot;
    uint8 sequence_idx;
    uint8 movie_branch;
    uint8 menu_reserved;
    /* Original serialized offsets 4..7, 20, 22..23 have no active observer */
    uint8 save_reserved[7];
    uint8 aircraft_unlocked[64];
    uint8 weapon_unlocked[16];
    /* Rows 0..3 are difficulty records and row 4 is the latest mission record */
    uint16 mission_time[5][GAME_MISSION_COUNT];
    uint8 mission_rating[5][GAME_MISSION_COUNT];
    uint8 archive_status[80];
    uint8 scene_status[180];
    uint32 mission_count[GAME_MISSION_COUNT];
    uint32 mission_losses[GAME_MISSION_COUNT];
    uint32 total_time;
    uint32 total_count;
    uint32 total_losses;
} GAME_PROGRESS;



typedef struct
{
    uint32 flags;
    uint32 mode;
    uint32 screen[2];
    uint8 background[3];
    uint32 field_wait;
} GAME_DISPLAY_CONTROL;

typedef struct
{
    DRAWENV *draw;
    DISPENV *display;
} GAME_DISPLAY_BANK;

typedef struct
{
    DRAWENV draw[3];
    DISPENV display[3];
    DRAWENV movie_draw;
    DISPENV movie_display;
    GAME_DISPLAY_BANK bank[2];
} GAME_DISPLAY_SURFACES;

typedef struct
{
    uint32 reserved[2];
    uint32 ordering[3];
    uint32 commands[1024];
} GAME_FRAME_PACKET_PAGE;

typedef char GAME_FRAME_PACKET_PAGE_LAYOUT_CHECK[
    sizeof(GAME_FRAME_PACKET_PAGE) == 4116 &&
    offsetof(GAME_FRAME_PACKET_PAGE, ordering) == 8 &&
    offsetof(GAME_FRAME_PACKET_PAGE, commands) == 20 ? 1 : -1];

typedef struct
{
    uint32 *first_tag;
    uint32 *middle_tag;
    uint32 *last_tag;
    uint32 *commands;
    uint32 command_bytes;
} GAME_FRAME_PACKETS;

typedef struct
{
    uint32 submit_counter;
    uint32 complete_counter;
} GAME_FRAME_TIMING;

typedef struct
{
    uint32 model_entries;
    uint32 aircraft_model_entries;
    uint32 animated_model_entries;
    uint32 unobserved[12];
} GAME_ASSET_COUNTS;

typedef struct
{
    uint32 descriptor_id;
    uint8 visible[3];
    uint8 latched[3];
    uint16 elapsed[3];
    uint32 arguments[2];
    uint16 extents[2];
    uint16 tpage;
    uint16 clut;
    uint8 enabled;
    uint8 speaker;
    uint8 metadata_flags;
    uint32 last_frame;
    uint8 requested[3];
} GAME_SUBTITLE_STATE;

typedef struct MISSION_STATUS_PACKET_PAGE MISSION_STATUS_PACKET_PAGE;

enum { MISSION_STATUS_GLYPH_CAPACITY = 32 };

typedef struct
{
    uint8 phase, atlas_index;
    uint16 ticks, x, y;
} MISSION_STATUS_GLYPH;

typedef struct
{
    MISSION_STATUS_GLYPH glyphs[MISSION_STATUS_GLYPH_CAPACITY];
    uint32 quad_count, sprite_count, glyph_count;
    uint32 duration, palette, current;
    uint32 packet_address;
    uint32 packet_pages[2];
    MISSION_STATUS_PACKET_PAGE *page_data[2];
    MISSION_STATUS_PACKET_PAGE *packet_page;
} MISSION_STATUS_TEXT_STATE;

typedef struct
{
    uint8 initialized;
    uint8 scene_initialized;
    uint32 flags;
    uint32 display_buffer;
    GAME_DISPLAY_CONTROL display;
    GAME_DISPLAY_SURFACES surfaces;
    GAME_FRAME_PACKETS frame_packets;
    GAME_FRAME_TIMING frame_timing;
    GAME_ASSET_COUNTS asset_counts;
    uint32 frame_counter;
    uint32 gameplay_counter;
    uint32 resource_pending;
    GAME_SCENE_STATE scene;
    uint32 field_counter;
    GAME_ARENA arena;
    GAME_SELECTION selection;
    GAME_PLAYBACK_FLOW playback;
    GAME_OPTIONS options;
    GAME_SUBTITLE_STATE subtitles;
    GAME_ACCOUNT account;
    GAME_PROGRESS progress;
    GAME_MISSION_RESULT mission_result;
    MISSION_METRICS metrics;
    GAME_MISSION_CONTROL mission_control;
    MISSION_STATUS_TEXT_STATE mission_status_text;
    GAME_SCENE_ASSETS scene_assets;
    RESOURCE_ASSET_SLOT resource_slot;
    GAME_RESOURCE_CONTEXT resource_context;
    uint32 menu_confirmation_gate;
    struct
    {
        uint32 ground_offset;
        uint32 minimum_y;
        uint32 altitude_limit;
        uint32 minimum_x, maximum_x;
        uint32 minimum_z, maximum_z;
        uint16 warning_limit;
        uint16 alternate_warning_limit;
        uint32 phase_timeout[4];
        uint32 phase_initial_ticks[4];
        uint8 phase_timer_mode[4];
        uint8 phase_message[4];
        uint8 has_fourth_initial_timer;
        uint8 has_phase_messages;
    } world_settings;
    GAME_ACTOR_RESOURCE_SOURCES actor_resources;
    GAME_TERRAIN_ORIGIN terrain_origin;
    uint16 mission_depth_buckets[17];
    GAME_MISSION_BOUNDS mission_bounds;
    GAME_MISSION_MENU mission_menu;
    GAME_MISSION_VIEW mission_view;
    GAME_MISSION_MATRICES mission_matrices;
    MISSION_RENDER_SETTINGS render_settings;
    AIRCRAFT_MISSION_METADATA aircraft_mission_metadata;
    AIRCRAFT_THRUST_METADATA aircraft_thrust_metadata;
    MISSION_TERRAIN_CAPTURE terrain_capture;
    GAME_MISSION_LIGHTING mission_lighting;
    GAME_CAMERA_PROJ camera_proj;
    GAME_CAMERA_CONTROL camera_control;
    GAME_CAMERA_FOLLOW camera_follow;
    GAME_CAMERA_VIEW camera_view;
    GAME_CAMERA_SHOTS camera_shots;
    const GAME_CAMERA_SHOT_DESC *camera_shot_desc;
    GAME_CAMERA_DELTA camera_delta;
    SCENE_COLLISION_RESULT collision_result;
} GAME_RUNTIME;

extern GAME_RUNTIME game_runtime;

void mission_bind_packet_bank(uint32 index, uint32 ordering_words, uint32 packet_bytes);
uint32 *mission_native_ot_span(uint32 address, size_t bytes);
void *mission_native_packet_span(uint32 address, size_t bytes);
uint32 *mission_ot_span(uint32 address, size_t bytes);
void *mission_packet_span(uint32 address, size_t bytes);

void game_runtime_reset(void);
char *game_account_set_name(const char *name);
void game_set_arena(uint8 *arena);
uint32 arena_cursor(void);
uint32 arena_alloc(uint32 bytes);
RESOURCE_STREAM_BUFFER arena_alloc_backed(uint32 bytes);
uint32 arena_peek(uint32 bytes);
uint32 arena_check_release(uint32 bytes);
uint32 arena_set_cursor(uint32 cursor);
uint32 arena_available(void);

void mission_bounds_initialize(uint32 index);

void game_init(void);
void game_init_devices(void);
void game_init_env(void);
void game_run(void);
void game_apply_display_changes(void);
sint32 game_initialize_frame_timing(void);
void game_load_boot_assets(void);
void game_frame_packets_reset(void);
void *game_frame_packet_span(uint32 address, size_t bytes);
uint32 game_frame_packet_cursor(void);
void game_frame_set_packet_cursor(uint32 encoded);
void game_begin_frame_packets(void);
void game_submit_frame(void);

uint32 title_card_stop_polling(void);
sint32 title_card_poll_info_event(uint32 port, uint32 next_step);
sint32 title_card_clear_info_events(void);
sint32 title_card_clear_write_events(void);
sint32 title_card_poll_write_event(uint32 port, uint32 next_step);
void title_card_initialize(void);
void title_card_open_events(void);
void title_card_close_events(void);
void title_card_poll_tick(void);
uint32 game_card_open_account(uint32 port, uint32 mode);
const char *game_card_record_name(const uint8 *record);

uint32 title_card_begin_polling(void);
uint32 title_card_port_status(uint32 port);
void title_card_reserve_records(void);
void title_card_reset_directory(void);
uint32 title_card_initialize_accounts(void);
void title_card_initialize_records_hook(void);
uint8 *title_account_record(uint32 index);
uint8 *title_mission_record(uint32 index);

void title_card_encode_current_account(void);
uint32 title_card_account_present(uint32 slot);
uint32 title_card_format_port(uint32 port);
uint32 title_card_create_file(uint32 port);
uint32 title_card_free_blocks(uint32 port);
uint32 title_card_read_directory(uint32 port, uint32 finalize);
uint32 title_card_read_accounts(uint32 port, uint32 finalize);
uint32 title_card_write_icons(uint32 port, uint32 finalize);
uint32 title_card_write_directory(uint32 port, sint32 slot, uint32 finalize);
uint32 title_card_write_account(uint32 port, uint32 slot, uint32 kind, uint32 finalize);

void title_card_select_account_copy(uint32 slot);
void title_card_decode_account(uint8 *record_selection);
void title_card_decode_mission(uint32 selection);
uint32 title_card_mission_present(uint32 slot, uint32 selection);
uint32 title_card_account_mask(void);
uint32 title_card_current_record_mask(void);
uint32 title_card_read_missions(uint32 port, uint32 slot, uint32 finalize);

uint8 *mission_card_account_record(uint32 index);
uint8 *mission_card_mission_record(uint32 index);
void mission_card_reserve_records(void);

uint32 mission_card_begin_polling(void);
uint32 mission_card_stop_polling(void);
uint32 mission_card_port_status(uint32 port);
uint32 mission_card_poll_info_event(uint32 port, uint32 phase);
void mission_card_clear_info_events(void);
void mission_card_clear_write_events(void);
uint32 mission_card_poll_write_event(uint32 port, uint32 phase);
void mission_card_reset_directory(void);
uint32 mission_card_initialize_accounts(void);
void mission_card_initialize_records_hook(void);
void mission_card_initialize(void);
void mission_card_open_events(void);
void mission_card_close_events(void);
uint32 mission_card_account_present(uint32 account);
uint32 mission_card_account_mask(void);
uint32 mission_card_find_current_account(void);
uint32 mission_card_format_port(uint32 port);
uint32 mission_card_create_file(uint32 port);
void mission_card_poll_tick(void);
uint32 mission_card_free_blocks(uint32 port);
uint32 mission_card_read_directory(uint32 port, uint32 finalize);
uint32 mission_card_read_accounts(uint32 port, uint32 finalize);
uint32 mission_card_read_missions(uint32 port, uint32 account, uint32 finalize);
uint32 mission_card_write_icons(uint32 port, uint32 finalize);
uint32 mission_card_write_directory(uint32 port, uint32 slot, uint32 finalize);
uint32 mission_card_write_account(uint32 port, uint32 account, uint32 slot, uint32 finalize);
uint32 mission_card_write_mission(uint32 port, uint32 account, uint32 slot, uint32 finalize);

void mission_card_poll_frame(void);

void mission_card_update_current_name(void);
void mission_card_store_current_name(void);
void mission_card_encode_current_account(void);
void mission_card_encode_current_mission(void);
void mission_card_select_account_copy(uint32 account);
void mission_card_prepare_account(uint32 account);
void mission_card_reset_directory_alias(void);
uint32 mission_card_initialize_accounts_alias(void);
const char *mission_card_account_name(uint32 account);
uint32 mission_card_mission_present(uint32 account, uint32 slot);
const char *mission_card_difficulty_label(uint32 slot);
const char *mission_card_mission_label(uint32 slot);
const char *mission_card_unit_label(uint32 slot);
uint32 mission_card_current_record_mask(void);
const char *mission_card_control_label(uint32 account);

void resource_rewind_current_asset(void);
void resource_rewind_loaded(uint32 loaded);

void game_stop_pad_communication(void);
uint32 game_gpu_get_ode(void);

#endif
