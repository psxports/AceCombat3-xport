#ifndef GAME_FUNCTIONS_H
    #define GAME_FUNCTIONS_H

    #include "psx.h"
    #include "byte_buffer.h"
    #include "game_math.h"
    #include "flight_dynamics.h"
    #include "effect_events.h"
#include "effect_trails.h"
    #include "effect_visuals.h"
    #include "effect_objects.h"
    #include "particles.h"
    #include "scene_geometry.h"
    #include "scene_sky.h"
    #include "scene_collision.h"
    #include "flight_control.h"
    #include "flight_physics.h"
    #include "flight_target.h"
    #include "flight_status.h"
    #include "random.h"
    #include "game_runtime.h"
    #include "camera.h"
    #include "mission_frame.h"
    #include "mission_menu.h"
    #include "subtitles.h"
    #include "mission_result.h"
    #include "mission_actor.h"
    #include "mission_status_text.h"
    #include "mission_links.h"
    #include "mission_groups.h"
    #include "mission_secondary.h"
    #include "mission_replay.h"
    #include "mission_target.h"
    #include "mission_spawn.h"
    #include "pad_input.h"
    #include "pad_settings.h"
    #include "menu_options.h"
    #include "menu_audio.h"
    #include "menu_model.h"
    #include "dialogue_sequence.h"
    #include "dialogue_ui.h"
    #include "dialogue_caption.h"
    #include "dialogue_video.h"
    #include "menu_geometry.h"
    #include "menu_binding_data.h"
    #include "menu_bindings.h"
    #include "menu_state.h"
    #include "ui_widgets.h"
    #include "ui_map.h"
    #include "ui_animation.h"
    #include "ui_control.h"
    #include "ui_packets.h"
    #include "ui_items.h"
    #include "menu_orbits.h"
    #include "scene_palette.h"
    #include "scene_dispatch.h"
    #include "mission_dispatch.h"
    #include "mission_briefing.h"
    #include "frontend_loading.h"
    #include "mission_effect.h"
    #include "text_renderer.h"
    #include "resource_directory.h"
    #include "resource_metadata.h"
    #include "resource_controller.h"
    #include "resource_queue.h"
    #include "resource_stream.h"
    #include "resource_ring.h"
    #include "scene_script.h"
    #include "mission_vm.h"
    #include "scene_audio.h"
    #include "audio_player.h"
    #include "scene_texture.h"
    #include "movie_decoder.h"
    #include "psx_gpu.h"
    #include "psx_spu.h"
    #include "psx_press.h"

void game_card_reset_directory(uint8 *header);
void game_card_header_initialize(uint8 *header);
void game_card_header_write_assets(uint8 *header, const uint8 *const *icons, uint32 count, const uint8 *palette);
sint32 game_account_save(void);
sint32 game_account_load(void);
uint32 game_account_save_requested(void);
void game_cd_copy_sector(void *destination, uint32 words);
uint32 game_cd_read_resource_sector_sync(void);
void game_cd_cancel_data_notification(void);
void game_cd_poll_stream(void);
void game_bind_sdk(void);
uint32 game_frame_display_ready(PSX_RECT *previous_draw, uint32 *previous_complete);
sint32 game_mount_data(uint32 disc);
uint32 game_current_data_disc(void);
uint8 game_data_mission_media_kind(uint32 mission);
sint32 game_use_extracted_assets(void);
sint32 game_read_resource_file(const RESOURCE_ENTRY *entry, void *destination, uint32 bytes);
sint32 game_start_resource_music(uint32 start, uint32 flags, uint32 data_stream);
uint32 game_cd_publish_metadata(const RESOURCE_METADATA_RECORD *payload);
sint32 game_resource_music_active(void);
void game_poll_resource_music(void);
void game_stop_resource_music(void);
void game_movie_open(uint32 resource, MOVIE_PLAYBACK *owner, MOVIE_SURFACE *surface, uint32 flags);
void game_movie_prepare(uint32 resource, MOVIE_PLAYBACK *owner, MOVIE_SURFACE *surface);
uint32 game_movie_poll(MOVIE_PLAYBACK *owner);
uint32 game_movie_decode(MOVIE_PLAYBACK *owner);
void game_movie_close(void);
uint32 game_movie_matches(uint32 id);
void game_movie_request_begin(uint32 flags);
uint32 game_movie_request_complete(void);
void game_movie_publish_frame(uint32 number);
sint32 game_movie_play(uint32 resource, uint32 captions, const XportFmvPlayback *playback);






sint32 pad_init_profiles(PAD_PROFILE *profiles);
sint32 pad_reset_slots(void);
void bios_exit_critical(void);
sint32 bios_enter_critical(void);

/* Pending game translations */
void pad_poll(void);
void pad_update_input(PAD_PORT_STATE *pad, GAME_INPUT *state, const PAD_PROFILE *profiles);
void pad_init_ports(void);
uint32 sub_800af1e8(uint32 text);

uint32 bios_init_card(uint32 mode);
uint32 bios_start_card(void);
void geom_set_offset(uint32 x, uint32 y);
void geom_set_screen(uint32 distance);
void cd_get_sector(void *dst, uint32 words);
void cd_apply_volume(CdlATV *volume);
void pad_start(void);
uint32 game_upload_image(PSX_RECT rectangle, const void *pixels, size_t capacity);
uint32 sub_800a2fec(uint32 mode, uint32 response);
sint32 pad_map_mode(uint32 mode);
uint32 pad_get_mode_param0(uint32 mode);
uint32 pad_get_mode_param1(uint32 mode);
void pad_normalize_axes(sint16 *axes, const PAD_AXIS_CALIBRATION *calibration);
sint32 pad_curve_expand(sint32 value);
sint32 pad_curve_contract(sint32 value);

uint32 sub_800a2ee4(uint32 mode);
sint32 DecDCToutSync(sint32 mode);











size_t game_decompress_buffer(const uint8 *input, size_t input_bytes, uint8 *output, size_t output_bytes);
size_t game_decompress_sized_buffer(const uint8 *input, size_t input_bytes, uint8 *output, size_t output_capacity);


void scene_geometry_prepare_capture_textures(uint32 colors, MISSION_TERRAIN_CAPTURE_RECORD *records, uint32 count);




uint32 mission_initialize_scene(void);
















void pad_rumble_add_scaled(sint32 value);








uint32 geom_get_screen(void);

uint32 scene_geometry_draw_models(const GAME_SCENE_ASSETS *assets, uint32 ordering, uint32 packet);












void pad_rumble_add_pulse(uint32 mode);
uint32 pad_rumble_add_preset(uint32 mode);



void resource_pause_stream(uint32 argument);





void hud_target_draw_path(const HUD_CONTEXT *context);
void hud_target_draw_relative(const HUD_CONTEXT *context);





void pad_rumble_add_decay_both(void);
void pad_rumble_add_decay_strong(void);
void pad_rumble_add_decay_slow(void);

















































































/* Reviewed SDK identities awaiting shared interface integration */
sint32 CdInit(void);
sint32 _bu_init(void);


void flight_weapon_build_pose(NATIVE_POSE *output, const MISSION_ACTOR *actor);





























uint32 scene_geometry_draw_terrain(const GAME_SCENE_ASSETS *assets, uint32 ordering, uint32 packet, const SCENE_TERRAIN_VIEW *view);




















































void scene_geometry_prepare_models(const GAME_SCENE_ASSETS *assets, const SCENE_GEOMETRY_DESCRIPTOR *descriptors, SCENE_GEOMETRY_MESH *meshes, const SCENE_GEOMETRY_COMPOSITE_BUFFER *animated, const SCENE_GEOMETRY_COMPOSITE_BUFFER *fixed, SCENE_GEOMETRY_SPRITE *sprites);











void pad_rumble_add_sustained(void);
void pad_rumble_add_random_large(void);
void pad_rumble_add_burst(void);
uint32 pad_rumble_add_random_both(void);















uint32 scene_geometry_draw_capture_records(uint32 ordering, uint32 packet, const MISSION_TERRAIN_CAPTURE_RECORD *records, uint32 count);















typedef struct
{
    uint32 xy;
    uint16 z, flags;
    uint32 screen, color;
    uint16 depth, uv;
} TERRAIN_VERTEX;

void scene_terrain_average_vertex(TERRAIN_VERTEX *out, const TERRAIN_VERTEX *first, const TERRAIN_VERTEX *second);


#endif























uint32 game_capture_image(PSX_RECT rectangle, void *pixels, size_t capacity);



void title_load_card_header_assets(const uint8 *data, uint32 data_bytes, uint32 icon_offset, uint32 palette);
void scene_overlay_reset(void);
sint32 game_overlay_is_active(uint32 resource);
void game_bios_close_guest(uint32 descriptor);
void game_bios_seek_guest(uint32 descriptor, sint32 offset, uint32 origin);


void menu_load_card_header_assets(const uint8 *data, uint32 data_bytes, uint32 icons, uint32 palette);


sint32 game_spu_read_decoded_native(void *destination, uint32 capacity, uint32 flags);
extern const char *const title_mission_labels[61];
extern const char *const title_difficulty_labels[4];
extern const char *const title_unit_labels[6];
extern const uint8 title_mission_units[61];





uint32 scene_geometry_draw_aircraft(const GAME_SCENE_ASSETS *assets, uint32 ordering, uint32 packet);






void scene_geometry_prepare_aircraft(const GAME_SCENE_ASSETS *assets, const SCENE_GEOMETRY_DESCRIPTOR *native_entries, SCENE_GEOMETRY_MESH *native_meshes, const SCENE_GEOMETRY_COMPOSITE_BUFFER *native_animated, const SCENE_GEOMETRY_COMPOSITE_BUFFER *native_fixed, SCENE_GEOMETRY_SPRITE *native_sprites);


















































void particle_initialize(uint32 index, uint32 mode, const NATIVE_POSE *native_position, const uint32 *native_vector);
