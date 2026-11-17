#include <stdlib.h>
#include <string.h>

#include "game_functions.h"

static const char *const detail_mission_labels[] = {
    "test map",          "Awakening", "Bravado",         "Enter Dision", "Paper Tiger", "Broken Truce", "Ghosts of the Past", "No Clearance",   "Fragile Cargo",  "Scylla and Charybdis", "Fates Intertwined", "Reaching for Stars", "One-Way Ticket",     "Bug Hunt",          "Pawns in the Game", "Damage Control", "Broken Wings", "Sphyrna",    "A Canopy of Stars", "----",    "Soldier of Fortune", "Megafloat",        "Target Acquisition", "Partners",      "Tainted Peace",   "Stratosphere",          "Welcoming Committee", "Technology Transfer", "Claustrophobia", "Dilemma", "Betrayal", "Heart of the Serpent", "Geofront Attack",
    "Casualties of War", "Geopelia",  "The Orientation", "Liquidation",  "Archnemesis", "Memory Error", "Electrosphere",      "Power for Life", "Guardian Angel", "Zero Gravity",         "The Prize",         "Utopian Dreams",     "Reality Distortion", "Counterrevolution", "Pursuit",           "Self Awareness", "----",         "Resistance", "Radio Silence",     "Revenge", "Sole Survivor",      "geopelia#1/end-a", "Tunnel Vision",      "outer space#0", "last enemy/m3#0", "electrosphere#1/end-b", "Mission Simulater",
};

static const char *const detail_faction_labels[] = {
    "UPEO", "General Resource", "neucom", "ouroboros", "free",
};

static const uint8 title_mission_discs[] = {
    3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 1, 2, 2, 1, 1, 3,
};

typedef char preview_ui_packet_layout_check[sizeof(TILE) == 16 && offsetof(TILE, tag) == 0 && offsetof(TILE, rgbc) == 4 && offsetof(TILE, p) == 8 && offsetof(TILE, w) == 12 && offsetof(TILE, h) == 14 && sizeof(XY) == 4 && offsetof(XY, y) == 2 && sizeof(RGBC) == 4 && offsetof(RGBC, code) == 3 && sizeof(DR_AREA) == 12 && offsetof(DR_AREA, tag) == 0 && sizeof(DR_MODE) == 12 && offsetof(DR_MODE, tag) == 0 ? 1 : -1];

typedef struct
{
    uint32 (*update)(void);
    void (*enter)(void);
} RESOURCE001_PANEL_CALLBACK;

static const uint8 title_boot_commands[] = {
    0x01, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDE, 0x00, 0x90, 0x00, 0x06, 0x00, 0x0B, 0x00, 0x00, 0xEB, 0xE1, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x20, 0x00, 0x00, 0x00, 0x01, 0x00, 0xE0, 0x01, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46, 0x00, 0xA3, 0x00, 0x02, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xA3, 0x00, 0x46, 0x00, 0xB4, 0x00, 0x34, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xA3, 0x00, 0x47, 0x00, 0xB4, 0x00, 0x35, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB5, 0x00, 0x33, 0x00, 0xA3, 0x00, 0x02, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 0x00, 0xA3, 0x00, 0x02, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xA3, 0x00, 0x52, 0x00, 0xB4, 0x00, 0x40, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xA3, 0x00, 0x53, 0x00, 0xB4, 0x00, 0x41, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB5, 0x00, 0x40, 0x00, 0xA3, 0x00, 0x02, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xC6, 0x00, 0xA3, 0x00, 0x02, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xA3, 0x00, 0xC6, 0x00, 0xB4, 0x00, 0xB4, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00,
    0xA3, 0x00, 0xC7, 0x00, 0xB4, 0x00, 0xB5, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB5, 0x00, 0xB4, 0x00, 0xA3, 0x00, 0x02, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xD2, 0x00, 0xA3, 0x00, 0x02, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xA3, 0x00, 0xD2, 0x00, 0xB4, 0x00, 0xC0, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xA3, 0x00, 0xD3, 0x00, 0xB4, 0x00, 0xC1, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB5, 0x00, 0xC0, 0x00, 0xA3, 0x00, 0x02, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xDE, 0x00, 0xA3, 0x00,
    0x02, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xA3, 0x00, 0xDE, 0x00, 0xB4, 0x00, 0xCC, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xA3, 0x00, 0xDF, 0x00, 0xB4, 0x00, 0xCD, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB5, 0x00, 0xCC, 0x00, 0xA3, 0x00, 0x02, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xEA, 0x00, 0xA3, 0x00, 0x02, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xA3, 0x00, 0xEA, 0x00, 0xB4, 0x00, 0xD8, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xA3, 0x00, 0xEB, 0x00, 0xB4, 0x00, 0xD9, 0x00, 0x00, 0x86, 0x90, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB5, 0x00, 0xD8, 0x00, 0xA3, 0x00, 0x02, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF6, 0x00, 0xA3, 0x00, 0x02, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xA3, 0x00, 0xF6, 0x00, 0xB4, 0x00, 0xE4, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xA3, 0x00, 0xF7, 0x00, 0xB4, 0x00, 0xE5, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB5, 0x00, 0xE4, 0x00, 0xA3, 0x00, 0x02, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x01, 0xA3, 0x00, 0x02, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00,
    0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xA3, 0x00, 0x02, 0x01, 0xB4, 0x00, 0xF0, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xA3, 0x00, 0x03, 0x01, 0xB4, 0x00, 0xF1, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB5, 0x00, 0xF0, 0x00, 0xA3, 0x00, 0x02, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAC, 0x01, 0xA3, 0x00, 0x02, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xA3, 0x00, 0xAC, 0x01, 0xB4, 0x00, 0x9A, 0x01, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xA3, 0x00, 0xAD, 0x01, 0xB4, 0x00, 0x9B, 0x01, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xB5, 0x00, 0x9A, 0x01, 0xA3, 0x00, 0x02, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB8, 0x01, 0xA3, 0x00, 0x02, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xA3, 0x00, 0xB8, 0x01, 0xB4, 0x00, 0xA6, 0x01, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0xA3, 0x00, 0xB9, 0x01, 0xB4, 0x00, 0xA7, 0x01, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xB5, 0x00, 0xA6, 0x01, 0xA3, 0x00, 0x02, 0x00, 0x00, 0x86, 0x90, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x40, 0x01, 0xE0, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF,
};

typedef struct
{
    uint32 commands;
    uint32 icons;
    uint32 icon_palette;
    uint32 reserved0;
    uint32 textures;
    uint32 reserved1;
    uint32 image;
    uint32 reserved2;
    uint32 sound_bank;
} TITLE_ASSET_DIRECTORY;

typedef char title_asset_directory_layout_check[sizeof(TITLE_ASSET_DIRECTORY) == 36 && offsetof(TITLE_ASSET_DIRECTORY, textures) == 16 && offsetof(TITLE_ASSET_DIRECTORY, image) == 24 && offsetof(TITLE_ASSET_DIRECTORY, sound_bank) == 32 ? 1 : -1];

const char *const title_difficulty_labels[4] = {
    "easy",
    "normal",
    "hard",
    "very hard",
};

const char *const title_mission_labels[61] = {
    "test map",       "Awakening",          "Bravado",           "Enter Dision", "Paper Tiger",    "Broken Truce", "Ghosts of the Past", "No Clearance",  "Fragile Cargo", "Scylla and Charybdis", "Fates Intertwined", "Reaching for Stars", "One-Way Ticket", "Bug Hunt",        "Pawns in the Game",     "Damage Control",    "Broken Wings", "Sphyrna", "A Canopy of Stars", "----", "Soldier of Fortune", "Megafloat", "Target Acquisition", "Partners", "Tainted Peace", "Stratosphere", "Welcoming Committee", "Technology Transfer", "Claustrophobia", "Dilemma", "Betrayal", "Heart of the Serpent", "Geofront Attack", "Casualties of War", "Geopelia", "The Orientation", "Liquidation", "Archnemesis", "Memory Error", "Electrosphere", "Power for Life", "Guardian Angel", "Zero Gravity", "The Prize",
    "Utopian Dreams", "Reality Distortion", "Counterrevolution", "Pursuit",      "Self Awareness", "----",         "Resistance",         "Radio Silence", "Revenge",       "Sole Survivor",        "geopelia#1/end-a",  "Tunnel Vision",      "outer space#0",  "last enemy/m3#0", "electrosphere#1/end-b", "Mission Simulater", "----",
};

const char *const title_unit_labels[6] = {
    "UPEO", "General Resource", "neucom", "ouroboros", "free", "none",
};

const uint8 title_mission_units[61] = {
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 3, 3, 3, 3, 3, 2, 2, 2, 2, 2, 3, 3, 3, 3, 5, 2, 2, 2, 2, 1, 2, 2, 3, 3, 5, 5,
};

MENU_STATE menu_state;
uint32 mission_menu_exit_fade_ticks;
MISSION_DETAIL_LIST mission_detail_list;
TITLE_MENU_STATE title_menu;

void menu_state_reset(void)
{
    memset(&menu_state, 0, sizeof(menu_state));
}

uint32 menu_state_load_begin(void)
{
    RESOURCE_STREAM_BUFFER asset_allocation;
    uint32 buffer, size, result;

    FUNCTION_MARKER(0x800CEAFC, "ACE3.BIN");
    buffer = arena_alloc(324);
    menu_state.arena_begin = buffer;
    game_runtime.resource_slot.resource_id = (uint32)(115);
    size = resource_total_size(&game_runtime.resource_slot.resource_id, 1);
    asset_allocation = arena_alloc_backed(size);
    buffer = asset_allocation.encoded_begin;
    game_runtime.resource_pending = 1;
    resource_queue_asset(&game_runtime.resource_slot, asset_allocation, resource_asset_read_finished);
    result = mission_flow.transition_phase + 1;
    mission_flow.transition_phase = result;
    return result;
}

uint32 menu_state_return_load_begin(void)
{
    RESOURCE_STREAM_BUFFER asset_allocation;
    uint32 buffer, size, result;

    FUNCTION_MARKER(0x800CEC70, "ACE3.BIN");
    buffer = arena_alloc(324);
    menu_state.arena_begin = buffer;
    game_runtime.resource_slot.resource_id = (uint32)(125);
    size = resource_total_size(&game_runtime.resource_slot.resource_id, 1);
    asset_allocation = arena_alloc_backed(size);
    buffer = asset_allocation.encoded_begin;
    game_runtime.resource_pending = 1;
    resource_queue_asset(&game_runtime.resource_slot, asset_allocation, resource_asset_read_finished);
    result = mission_flow.transition_phase + 1;
    mission_flow.transition_phase = result;
    return result;
}

typedef struct
{
    uint32 reserved;
    uint32 commands;
    uint32 detail_script;
    uint32 pad_textures;
    uint32 audio_script;
    uint32 aircraft_script;
    uint32 sound_test;
} MENU_OPTIONS_DIRECTORY;

typedef char menu_options_directory_layout[sizeof(MENU_OPTIONS_DIRECTORY) == 28 ? 1 : -1];

uint32 menu_bind_options_assets(uint32 resource, const uint8 *data, uint32 data_bytes)
{
    uint32 sections[7];

    mission_image_sections(resource, data, data_bytes, sections, 7);
    const MENU_OPTIONS_DIRECTORY directory = {
        sections[0], sections[1], sections[2], sections[3], sections[4], sections[5], sections[6],
    };
    mission_flow.assets.options_source = (RESOURCE_ASSET_SOURCE){data, data_bytes, resource};
    ui_group_bind(ui_command_directory(data, data_bytes, directory.commands - resource, directory.detail_script - resource), 2);
    mission_flow.assets.detail_script = directory.detail_script;
    mission_flow.assets.pad_settings_textures = directory.pad_textures;
    mission_flow.assets.audio_script = directory.audio_script;
    mission_flow.assets.aircraft_preview_script = directory.aircraft_script;
    return directory.sound_test;
}

void menu_state_ready(void)
{
    uint8 *const fields[5] = {&menu_state.main_choice, &menu_state.detail_choice, &menu_state.options_choice, &menu_state.header.close_requested, &menu_state.binding_tab};
    uint32 counter, index, value;

    FUNCTION_MARKER(0x800DE188, "ACE3.BIN");
    counter = mission_flow.menu_init_delay + 1;
    mission_flow.menu_init_delay = (uint32)(counter);
    if ((sint32)counter < 2)
        return;
    mission_flow.menu_init_delay = (uint32)(0);
    menu_state.header.kind = (uint8)(0);
    menu_state.profile_kind = (uint16)(0);
    menu_state.header.phase = (uint8)(0);
    for (index = 0; index < 5; ++index)
    {
        *fields[index] = (0);
    }
    menu_options_start();
    menu_state.binding_phase = (uint32)(0);
    menu_state.restore_choice = (uint8)(0);
    for (index = 0; index < 6; ++index)
    {
        menu_state.binding.values[index] = (uint8)(0);
    }
    mission_effect_close_sprs();
    value = (sint16)menu_state.profile_kind;
    menu_state.pad_refresh = (uint32)(0);
    if (value < 4)
        scene_texture_load_image(mission_flow.assets.pad_profile_images[value], mission_flow.assets.frontend_source, SCENE_TEXTURE_STANDARD);
    menu_bindings_default_analog();
    menu_bindings_default_negcon();
    menu_bindings_default_digital();
    menu_bindings_default_joystick();
    mission_flow.root_phase = 1;
    mission_flow.phase_timer = (uint32)(0);
    mission_flow.transition_phase = 0;
    mission_flow.assets.sound_test_assets = menu_bind_options_assets(game_runtime.resource_slot.encoded_base, game_runtime.resource_slot.data, game_runtime.resource_slot.read_bytes);
    return;
}

uint32 menu_state_return_ready(void)
{
    uint8 *const fields[4] = {&menu_state.options_choice, &menu_state.header.close_requested, &menu_state.binding_tab, &menu_state.restore_choice};
    uint32 phase, index, value;

    FUNCTION_MARKER(0x800DE540, "ACE3.BIN");
    phase = (sint16)mission_flow.transition_phase;
    mission_dispatch_loading(phase, 0, (RESOURCE_STREAM_BUFFER){0});
    phase = (sint16)mission_flow.transition_phase;
    if (phase != 3)
        return 3;
    menu_state.header.kind = (uint8)(9);
    menu_state.header.phase = (uint8)(0);
    menu_state.main_choice = (uint8)(0);
    menu_state.profile_kind = (uint16)(0);
    menu_state.detail_choice = (uint8)(0);
    for (index = 0; index < 3; ++index)
    {
        *fields[index] = (0);
    }
    menu_state.binding_phase = (uint32)(0);
    menu_options_start();
    *fields[3] = (0);
    for (index = 0; index < 6; ++index)
    {
        menu_state.binding.values[index] = (uint8)(0);
    }
    mission_effect_close_sprs();
    value = (sint16)menu_state.profile_kind;
    menu_state.pad_refresh = (uint32)(0);
    if (value < 4)
        scene_texture_load_image(mission_flow.assets.pad_profile_images[value], mission_flow.assets.frontend_source, SCENE_TEXTURE_STANDARD);
    menu_bindings_default_analog();
    menu_bindings_default_negcon();
    menu_bindings_default_digital();
    menu_bindings_default_joystick();
    mission_flow.phase_timer = (uint32)(0);
    mission_flow.root_phase = 1;
    return 1;
}

uint32 menu_state_frame(void)
{
    uint32 packet, kind, phase, result;

    FUNCTION_MARKER(0x800DE3CC, "ACE3.BIN");
    packet = ui_packets.scene_packet_cursor;
    kind = menu_state.header.kind;
    menu_state.packet_cursor = (uint32)(packet);
    phase = menu_state.header.phase;
    mission_dispatch_detail(kind, phase, &menu_state.header);
    if (menu_state.header.close_requested == 1)
    {
        mission_flow.root_kind = 0;
        mission_flow.root_phase = 0;
        mission_flow.menu_phase = 0;
        mission_flow.transition_phase = 0;
        arena_set_cursor(menu_state.arena_begin);
    }
    phase = (sint16)mission_flow.transition_phase;
    mission_dispatch_transition(phase);
    result = menu_state.packet_cursor;
    ui_packets.scene_packet_cursor = (uint32)(result);
    return result;
}

static void menu_detail_initialize(uint32 resource)
{
    uint32 phase;

    FUNCTION_MARKER(0x800D2414, "ACE3.BIN");
    scene_texture_load_group(resource, mission_flow.assets.options_source);
    ui_group_set_mask(0, 3);
    if (!menu_state.detail_choice)
    {
        ui_group_set_mask(2, 1);
        phase = 1;
    }
    else
    {
        ui_group_set_mask(2, 2);
        phase = 4;
    }
    menu_state.header.phase = (uint8)(phase);
    ui_animation_set(1, 1, 0, 40);
    ui_animation_set(2, 1, 0, 40);
    ui_widget_set(1, 4);
    ui_widget_set(5, menu_state.main_choice);
    {
        ui_widget_set(62, 1);
        return;
    }
}

void menu_detail_begin(void)
{
    FUNCTION_MARKER(0x800D240C, "ACE3.BIN");
    {
        menu_detail_initialize(mission_flow.assets.detail_script);
        return;
    }
}

void title_menu_draw_account_actions(void)
{
    uint8 no_arguments[1] = {0};
    uint32 can_download, can_reopen, value;

    FUNCTION_MARKER(0x800DC434, "ACE3.BIN");
    value = title_card_port_status(0);
    can_download = 1;
    if (value != 5)
        can_download = title_card_port_status(1) == 5;
    can_reopen = game_runtime.progress.account_flags & 1;
    title_text_select_font(4);
    title_text_set_blend(2);
    title_text_set_reveal_range(1, 30, 40);
    value = title_renderer_animation_phase(3);
    title_text_set_reveal_tick(value);
    title_text_set_color(128, 128, 128);
    title_text_set_pos(106, 150);
    title_text_print((const uint8 *)"New Account", no_arguments, 0);
    title_text_set_pos(106, 204);
    if (can_download)
        title_text_print((const uint8 *)"Download", no_arguments, 0);
    else
    {
        title_text_set_blend(1);
        title_text_set_color(8, 48, 40);
        title_text_print((const uint8 *)"Download", no_arguments, 0);
        title_text_set_blend(2);
        title_text_set_color(128, 128, 128);
    }
    title_text_set_pos(106, 257);
    if (can_reopen)
        title_text_print((const uint8 *)"re-Open", no_arguments, 0);
    else
    {
        title_text_set_blend(1);
        title_text_set_color(8, 48, 40);
        title_text_print((const uint8 *)"re-Open", no_arguments, 0);
        title_text_set_blend(2);
        title_text_set_color(128, 128, 128);
    }
    title_text_set_reveal_range(1, 30, 40);
    value = title_renderer_animation_phase(4);
    title_text_set_reveal_tick(value);
    {
        (void)(title_text_set_color(128, 128, 128));
        return;
    }
}

void title_menu_draw_keyboard(void)
{
    uint8 no_arguments[1] = {0};
    uint32 page, index, value;
    const UI_ACCOUNT_KEY *row;

    FUNCTION_MARKER(0x800DC5EC, "ACE3.BIN");
    page = title_menu.keyboard_bank;
    if (page >= UI_ACCOUNT_KEY_BANKS)
        abort();
    row = ui_account_keys[page];
    title_text_select_font(3);
    title_text_set_blend(2);
    title_text_set_reveal_range(1, 30, 40);
    value = title_renderer_animation_phase(15);
    title_text_set_reveal_tick(value);
    title_text_set_color(128, 128, 128);
    for (index = 0; index < 50; ++index)
    {
        title_text_set_pos((uint32)row->x, (uint32)row->y);
        title_text_print(row->glyph, no_arguments, 0);
        ++row;
    }
    return;
}

uint32 title_disc_transfer_status(void)
{
    FUNCTION_MARKER(0x800DA4DC, "ACE3.BIN");
    return title_menu.transfer.status;
}

void title_disc_transfer_set_mission(uint32 resource)
{
    FUNCTION_MARKER(0x800DA4F4, "ACE3.BIN");
    title_menu.transfer.mission = resource;
}

uint32 title_disc_transfer_begin(void)
{
    FUNCTION_MARKER(0x800DA504, "ACE3.BIN");
    title_menu.transfer.recovered = 0;
    title_menu.transfer.status = 0;
    return 1;
}

uint8 title_mission_disc(uint32 mission)
{
    if (mission >= sizeof(title_mission_discs))
        abort();
    return title_mission_discs[mission];
}

uint32 title_disc_transfer_poll(void)
{
    uint32 current, index, type, previous;

    FUNCTION_MARKER(0x800DA520, "ACE3.BIN");
    current = cd_resource_state();
    if (current == 0)
    {
        title_menu.transfer.status = 1;
        return 0;
    }
    index = title_menu.transfer.mission;
    type = title_mission_disc(index);
    if (type == 3 || current == 3)
    {
        title_menu.transfer.status = 0;
        return 2;
    }
    if (current == 4 || current == 0xFFFFFFFF)
    {
        if (type == 1)
        {
            previous = title_menu.transfer.recovered;
            title_menu.transfer.status = previous ? 4 : 2;
        }
        index = title_menu.transfer.mission;
        type = title_mission_disc(index);
        if (type == 2)
        {
            previous = title_menu.transfer.recovered;
            title_menu.transfer.status = previous ? 5 : 3;
        }
    }
    else if (current == 1)
    {
        if (type == 1)
        {
            title_menu.transfer.status = 6;
            return 2;
        }
        previous = title_menu.transfer.recovered;
        title_menu.transfer.status = previous ? 5 : 3;
    }
    else if (current == 2)
    {
        if (type == 2)
        {
            title_menu.transfer.status = 6;
            return 2;
        }
        previous = title_menu.transfer.recovered;
        title_menu.transfer.status = previous ? 4 : 2;
    }
    else
        return 0;
    resource_begin_recovery();
    return 1;
}

uint32 title_disc_transfer_recovery_ready(void)
{
    FUNCTION_MARKER(0x800DA694, "ACE3.BIN");
    return resource_get_recovery_state() == 3;
}

uint32 title_disc_transfer_recovery_finish(void)
{
    uint32 value;

    FUNCTION_MARKER(0x800DA6B8, "ACE3.BIN");
    value = resource_get_recovery_state();
    if (value != 1)
        return 0;
    title_menu.transfer.status = value;
    title_menu.transfer.recovered = value;
    return 4;
}

uint32 title_menu_disc_transfer_begin(void)
{
    uint32 step, resource;

    FUNCTION_MARKER(0x800CE24C, "ACE3.BIN");
    step = title_menu.screen_step;
    title_menu.transfer_phase = 0;
    title_menu.screen_step = (uint16)(step + 1);
    resource = (sint16)game_runtime.selection.mission;
    title_disc_transfer_set_mission(resource);
    return 0;
}

uint32 title_menu_disc_transfer_tick(void)
{
    uint32 step, value, status;

    FUNCTION_MARKER(0x800CE290, "ACE3.BIN");
    switch (title_menu.transfer_phase)
    {
        case 0:
            value = title_disc_transfer_begin();
            break;
        case 1:
            value = title_disc_transfer_poll();
            break;
        case 2:
            value = title_disc_transfer_recovery_ready();
            break;
        case 3:
            value = title_disc_transfer_recovery_finish();
            break;
        default:
            abort();
    }
    if (value & 1)
    {
        step = title_menu.transfer_phase;
        title_menu.transfer_phase = step + 1;
    }
    if (value & 2)
    {
        title_menu.exit_requested = (uint8)(1);
        return 1;
    }
    if (value & 4)
    {
        title_menu.transfer_phase = 1;
    }
    status = title_disc_transfer_status();
    step = title_menu.transfer_phase;
    if ((sint32)step >= 2)
    {
        title_renderer_set_visible(0x400036);
        title_renderer_set_group_value(4, 4);
    }
    switch (status)
    {
        case 1:
            title_renderer_set_group_value(1, 73);
            break;
        case 2:
            title_renderer_set_group_value(1, 70);
            break;
        case 3:
            title_renderer_set_group_value(1, 71);
            break;
        case 4:
            title_renderer_set_group_value(1, 101);
            break;
        case 5:
            title_renderer_set_group_value(1, 102);
            break;
        case 6:
            title_renderer_set_group_value(1, 0);
            return 0;
    }
    return 6;
}

void title_account_reset_records(void)
{
    uint32 index, flags;
    sint32 remaining;

    FUNCTION_MARKER(0x800CE3F4, "ACE3.BIN");
    game_runtime.progress.record_reserved = (uint8)(0);
    game_runtime.progress.record_slot = (uint8)(6);
    game_runtime.progress.sequence_idx = (uint8)(0);
    for (index = 0; index < 60; ++index)
    {
        game_runtime.progress.mission_time[0][index] = (uint16)(0x8CA0);
        game_runtime.progress.mission_time[1][index] = (uint16)(0x8CA0);
        game_runtime.progress.mission_time[2][index] = (uint16)(0x8CA0);
        game_runtime.progress.mission_time[3][index] = (uint16)(0x8CA0);
        game_runtime.progress.mission_time[4][index] = (uint16)(0x8CA0);
    }
    for (index = 0; index < 60; ++index)
    {
        game_runtime.progress.mission_rating[0][index] = (uint8)(0);
        game_runtime.progress.mission_rating[1][index] = (uint8)(0);
        game_runtime.progress.mission_rating[2][index] = (uint8)(0);
        game_runtime.progress.mission_rating[3][index] = (uint8)(0);
        game_runtime.progress.mission_rating[4][index] = (uint8)(0);
    }
    for (remaining = 59; remaining >= 0; --remaining)
        game_runtime.progress.archive_status[remaining] = 0;
    for (remaining = 179; remaining >= 0; --remaining)
        game_runtime.progress.scene_status[remaining] = 0;
    for (index = 0; index < 60; ++index)
    {
        game_runtime.progress.mission_count[index] = (uint32)(0);
        game_runtime.progress.mission_losses[index] = (uint32)(0);
    }
    game_runtime.progress.total_time = (uint32)(0);
    game_runtime.progress.total_count = (uint32)(0);
    game_runtime.progress.total_losses = (uint32)(0);
    for (remaining = 63; remaining >= 0; --remaining)
        game_runtime.progress.aircraft_unlocked[remaining] = 0;
    for (remaining = 15; remaining >= 0; --remaining)
        game_runtime.progress.weapon_unlocked[remaining] = 0;
    flags = game_runtime.progress.flow_flags & 0xFFFFFFEF;
    game_runtime.progress.flow_flags = (uint32)(flags);
    {
        (void)(uint32)(flags);
        return;
    }
}

uint32 title_menu_keyboard_begin(void)
{
    uint32 step;

    FUNCTION_MARKER(0x800CE518, "ACE3.BIN");
    step = title_menu.panel_step;
    title_menu.phase_ticks = 0;
    title_menu.panel_step = (uint16)(step + 1);
    title_renderer_reset_view(11);
    title_renderer_reset_view(6);
    title_renderer_set_visible(0x85E);
    title_renderer_set_animation(15, 1, 0, 60);
    title_renderer_set_group_value(1, 15);
    title_renderer_set_group_value(4, 22);
    title_renderer_set_hint(111, 150, 1, 1);
    return title_menu_wait_step();
}

const char *title_menu_account_name(sint32 index)
{
    FUNCTION_MARKER(0x800D5970, "ACE3.BIN");
    if (index == 0)
        return game_card_record_name(title_account_record(3));
    if (index < 4)
        return game_card_record_name(title_account_record((uint32)index - 1));
    return "";
}

uint32 title_menu_keyboard_tick(void)
{
    uint32 key, value, directions, buttons, bank, character, sound, result;
    uint8 *buffer;
    sint32 length;

    FUNCTION_MARKER(0x800CE5AC, "ACE3.BIN");
    key = title_menu.keyboard_key;
    title_renderer_set_group_value(2, key);
    buffer = title_account_record(3) + 8;
    length = (sint32)strlen(title_menu_account_name(0));
    directions = game_input.repeat;
    if (directions & 8)
    {
        key = title_menu.keyboard_key;
        value = key + 1;
        if (key < 14)
        {
            if (key == 13)
                value = 0;
        }
        else if (key < 27)
        {
            if (key == 26)
                value = 14;
        }
        else if (key < 39)
        {
            if (key == 38)
                value = 26;
        }
        else if (key < 50)
        {
            if (key == 49)
                value = 39;
        }
        else if (key < 52)
        {
            if (key == 51)
                value = 50;
        }
        else
            goto moved;
        title_menu.keyboard_key = (uint8)(value);
        goto moved;
    }
    if (directions & 4)
    {
        key = title_menu.keyboard_key;
        value = key - 1;
        if (key < 14)
        {
            if (key == 0)
                value = 13;
        }
        else if (key < 27)
        {
            if (key == 14)
                value = 26;
        }
        else if (key < 39)
        {
            if (key == 27)
                value = 26;
        }
        else if (key < 50)
        {
            if (key == 39)
                value = 49;
        }
        else if (key < 52)
        {
            if (key == 50)
                value = 51;
        }
        else
            goto moved;
        title_menu.keyboard_key = (uint8)(value);
        goto moved;
    }
    if (directions & 2)
    {
        key = title_menu.keyboard_key;
        if (key < 14)
            value = key == 13 ? key + 13 : key + 14;
        else if (key < 27)
            value = key == 26 ? key + 23 : key + 13;
        else if (key < 39)
            value = key == 38 ? key + 11 : key + 12;
        else if (key < 48)
            value = 50;
        else if (key < 50)
            value = 51;
        else if (key == 50)
            value = 2;
        else if (key == 51)
            value = 9;
        else
            goto moved;
        title_menu.keyboard_key = (uint8)(value);
        goto moved;
    }
    if (directions & 1)
    {
        key = title_menu.keyboard_key;
        if (key < 9)
            value = 50;
        else if (key < 14)
            value = 51;
        else if (key < 27)
            value = key - 14;
        else if (key < 39)
            value = key - 13;
        else if (key < 50)
            value = key - 12;
        else if (key == 50)
            value = 41;
        else
        {
            key = title_menu.keyboard_key;
            if (key >= 52)
                goto moved;
            value = 48;
        }
        title_menu.keyboard_key = (uint8)(value);
        goto moved;
    }
    buttons = game_input.pressed;
    if (buttons & 0x40)
    {
        sound = 48;
        if (length > 0)
        {
            title_menu.panel = (uint16)(1);
            title_menu.panel_step = (uint16)(0);
            sound = 89;
        }
        goto play_sound;
    }
    if (buttons & 0x300)
        goto toggle_bank;
    if (buttons & 0x10)
    {
        key = title_menu.keyboard_key;
        if (key == 26)
        {
            sound = 48;
            if (length > 0)
            {
                title_menu.panel = (uint16)(1);
                title_menu.panel_step = (uint16)(0);
                sound = 89;
            }
            goto play_sound;
        }
        if (key < 27)
        {
            if (key == 13)
            {
                if (length <= 0)
                {
                    buffer[(uint32)length] = (uint8)(0);
                    goto finish;
                }
                --length;
                goto erase_character;
            }
        }
        else
        {
            if (key == 49)
                goto toggle_bank;
            if (key == 51)
            {
                title_menu.keyboard_key = (uint8)(0);
#if 0
                /* Original return to link-mode selection */
                title_menu.screen = (uint16)(2);
#else
                title_menu.screen = (uint16)(1);
#endif
                title_menu.screen_step = (uint16)(0);
                title_renderer_reset_view(7);
                title_renderer_reset_view(9);
                title_renderer_set_visible(190);
                sound_play(59);
                arena_set_cursor(title_menu.keyboard_arena_begin);
                goto finish;
            }
        }
        if (length >= 8)
            goto finish;
        bank = title_menu.keyboard_bank;
        key = title_menu.keyboard_key;
        if (bank >= UI_ACCOUNT_KEY_BANKS || key >= UI_ACCOUNT_KEY_COUNT)
            abort();
        character = ui_account_keys[bank][key].glyph[0];
        if (character == 32 && length == 0)
        {
            sound = 48;
            goto play_sound;
        }
        buffer[(uint32)length] = (uint8)(character);
        buffer[(uint32)length + 1] = (uint8)(0);
        sound = 47;
        goto play_sound;
    }
    if (buttons & 0x20)
    {
        if (length > 0)
        {
            --length;
            goto erase_character;
        }
        key = title_menu.keyboard_key;
        if (key == 51)
            goto finish;
        title_menu.keyboard_key = (uint8)(51);
        goto moved;
    }
    goto finish;
toggle_bank:
    value = title_menu.keyboard_bank;
    title_menu.keyboard_bank = (uint8)(value ^ 1);
    sound = 47;
    goto play_sound;
erase_character:
    sound_play(46);
    buffer[(uint32)length] = (uint8)(0);
    goto finish;
moved:
    sound = 61;
play_sound:
    sound_play(sound);
finish:
    result = title_menu.phase_ticks + 1;
    title_menu.phase_ticks = result;
    return result;
}

uint32 title_menu_confirm_name_begin(void)
{
    uint32 step;

    FUNCTION_MARKER(0x800CEB20, "ACE3.BIN");
    step = title_menu.panel_step;
    title_menu.panel_step = (uint16)(step + 1);
    title_renderer_reset_view(12);
    title_renderer_set_visible(0x105E);
    title_renderer_set_group_value(1, 16);
    title_renderer_set_group_value(4, 22);
    title_renderer_set_hint(111, 150, 1, 0);
    return title_menu_wait_step();
}

uint32 title_menu_confirm_name_tick(void)
{
    uint32 flags;
    const char *name;

    FUNCTION_MARKER(0x800CEB90, "ACE3.BIN");
    flags = game_input.held;
    if (flags & 0x10)
    {
        flags = game_input.pressed;
        if (flags & 0x10)
        {
            title_menu.panel = (uint16)(2);
            title_menu.panel_step = (uint16)(0);
            name = title_menu_account_name(0);
            game_account_set_name(name);
            title_account_reset_records();
            flags = game_runtime.progress.account_flags;
            game_runtime.progress.account_flags = (uint32)(flags & 0xFFFFFFFE);
            sound_play(89);
        }
    }
    flags = game_input.held;
    if ((flags & 0x20) == 0)
        return 0;
    flags = game_input.pressed;
    if ((flags & 0x20) == 0)
        return 0x800E0000;
    title_menu.panel = (uint16)(0);
    title_menu.panel_step = (uint16)(0);
    return sound_play(59);
}

uint32 title_menu_difficulty_begin(void)
{
    uint32 step;

    FUNCTION_MARKER(0x800CEC70, "ACE3.BIN");
    step = title_menu.panel_step;
    title_menu.phase_ticks = 0;
    title_menu.panel_step = (uint16)(step + 1);
    title_renderer_reset_view(10);
    title_renderer_set_visible(0x43E);
    title_renderer_set_animation(15, 1, 0, 60);
    title_renderer_set_group_value(1, 11);
    title_renderer_set_group_value(4, 6);
    title_renderer_set_hint(64, 50, 1, 0);
    return title_menu_wait_step();
}

uint32 title_menu_difficulty_tick(void)
{
    uint32 directions, buttons, index, result;
    sint32 selection;

    FUNCTION_MARKER(0x800CECF8, "ACE3.BIN");
    selection = (sint32)game_runtime.selection.difficulty;
    directions = game_input.repeat;
    buttons = game_input.pressed;
    if (selection < 0)
        selection = 0;
    else if (selection >= 3)
        selection = 2;
    title_text_select_font(4);
    title_text_set_color(128, 128, 128);
    title_text_set_blend(2);
    title_text_set_pos(106, 166);
    title_text_print((const uint8 *)"easy", 0, 0);
    title_text_set_pos(106, 218);
    title_text_print((const uint8 *)"normal", 0, 0);
    title_text_set_pos(106, 270);
    title_text_print((const uint8 *)"hard", 0, 0);
    if (buttons & 0x20)
    {
        title_menu.panel = (uint16)(0);
        title_menu.panel_step = (uint16)(0);
        sound_play(59);
    }
    else if (buttons & 0x10)
    {
#if 0
        /* Original configuration and memory-card setup sequence */
        title_menu.panel = (uint16)(3);
        title_menu.panel_step = (uint16)(0);
#else
        title_account_reset_campaign();
        game_runtime.selection.mission = (uint16)(1);
        game_runtime.account.route_mode = (uint16)(2);
        game_runtime.progress.account_id = (uint32)(title_account_id_seed());
        if (!game_account_save())
            return 0;
        title_menu.screen = (uint16)(6);
        title_menu.screen_step = (uint16)(0);
#endif
        sound_play(89);
    }
    else if (directions & 3)
    {
        if (directions & 2)
            selection = (selection + 1) % 3;
        if (directions & 1)
            selection = (selection + 2) % 3;
        game_runtime.selection.difficulty = (uint32)(selection);
        sound_play(61);
    }
    for (index = 0; index < 3; ++index)
        title_renderer_set_group_value(index + 6, index != (uint32)selection);
    result = title_menu.phase_ticks + 1;
    title_menu.phase_ticks = result;
    return result;
}

uint32 title_account_id_seed(void)
{
    FUNCTION_MARKER(0x800D4140, "ACE3.BIN");
    return game_runtime.field_counter;
}

void title_account_reset_campaign(void)
{
    uint32 index;

    FUNCTION_MARKER(0x800DA1B8, "ACE3.BIN");
    for (index = 0; index < 180; ++index)
    {
        game_runtime.progress.scene_status[179 - index] = 0;
    }
    for (index = 0; index < 80; ++index)
    {
        game_runtime.progress.archive_status[79 - index] = 0;
    }
    for (index = 0; index < 60; ++index)
    {
        game_runtime.progress.mission_rating[3][index] = (uint8)(0);
        game_runtime.progress.mission_rating[2][index] = (uint8)(0);
        game_runtime.progress.mission_rating[1][index] = (uint8)(0);
        game_runtime.progress.mission_rating[0][index] = (uint8)(0);
        game_runtime.progress.mission_rating[4][index] = (uint8)(0);
        game_runtime.progress.mission_time[3][index] = (uint16)(0xD2F0);
        game_runtime.progress.mission_time[2][index] = (uint16)(0xD2F0);
        game_runtime.progress.mission_time[1][index] = (uint16)(0xD2F0);
        game_runtime.progress.mission_time[0][index] = (uint16)(0xD2F0);
        game_runtime.progress.mission_time[4][index] = (uint16)(0xD2F0);
        game_runtime.progress.mission_count[index] = (uint32)(0);
        game_runtime.progress.mission_losses[index] = (uint32)(0);
    }
    game_runtime.progress.total_time = (uint32)(0);
    game_runtime.progress.total_count = (uint32)(0);
    game_runtime.progress.total_losses = (uint32)(0);
    return;
}

uint32 title_menu_initialize(void)
{
    uint32 flags, first, second, index;

    FUNCTION_MARKER(0x800CC5D8, "ACE3.BIN");
    game_runtime.progress.sequence_idx = (uint8)(0);
    game_runtime.progress.movie_branch = (uint8)(0);
    flags = game_runtime.progress.flow_flags;
    first = game_runtime.selection.saved_mission;
    second = game_runtime.selection.saved_aircraft;
    game_runtime.progress.flow_flags = (uint32)(flags & 0xFFFFFFF7);
    game_runtime.selection.mission = (uint16)(first);
    game_runtime.selection.aircraft = (uint16)(second);
    arena_alloc(228);
    if (!gpu_register_packet_range(title_menu.packet_storage, sizeof(title_menu.packet_storage)))
        abort();
    for (index = 0; index < 2; ++index)
    {
        (void)arena_alloc(0x2000);
        title_menu.packet_pages[index] = gpu_packet_link((uintptr_t)title_menu.packet_storage[index]);
    }
    title_menu_initialize_hook();
    title_menu.exit_requested = (uint8)(0);
    title_menu.screen = (uint16)(1);
    title_menu.screen_step = (uint16)(0);
    title_menu.frame_count = 0;
    title_menu.idle_ticks = 0;
    title_menu.link_selection = (uint8)(0);
    title_menu.card_port = (uint8)(0);
    title_menu.account_selection = (uint8)(0);
    title_menu.record_selection = (uint8)(0);
    title_menu.record_action = (uint8)(0);
    title_menu.link_preview_ticks = (uint8)(0);
    title_menu.keyboard_bank = (uint8)(0);
    title_menu.keyboard_key = (uint8)(0);
    title_menu.card_notice = (uint8)(0);
    title_menu.card_port_hidden = (uint8)(1);
    game_runtime.progress.menu_reserved = (uint8)(0);
    title_card_reserve_records();
    title_card_initialize();
    title_menu.card_events_open = (uint32)(0);
    title_text_initialize();
    title_menu_reserve_legacy_packets();
    return 0x04000000;
}

static void title_menu_collect_ranks(uint8 output[52])
{
    uint32 counts[6] = {0};
    uint32 index, best, value, rank, count, cursor = 0;

    FUNCTION_MARKER(0x800CC938, "ACE3.BIN");
    for (index = 1; index < 56; ++index)
    {
        if (index == 19 || index == 49 || index == 54)
            continue;
        value = game_runtime.progress.mission_rating[0][index];
        best = value ? value : 5;
        value = game_runtime.progress.mission_rating[1][index];
        if (value && value < best)
            best = value;
        value = game_runtime.progress.mission_rating[2][index];
        if (value && value < best)
            best = value;
        if (best >= 6)
        {
#ifndef NDEBUG
            fprintf(stderr, "Resource001 rank %u exceeds original local count array\n", best);
#endif
            abort();
        }
        ++counts[best];
    }
    for (rank = 1; rank <= 5; ++rank)
    {
        count = counts[rank];
        for (index = 0; index < count; ++index)
            output[cursor++] = (uint8)rank;
    }
    while (cursor < 52)
        output[cursor++] = 5;
}

uint32 title_menu_start_begin(void)
{
    uint8 ranks[52];
    uint32 flags, index;

    FUNCTION_MARKER(0x800CCC90, "ACE3.BIN");
    title_menu.phase_ticks = 0;
    title_menu.screen_step = (uint16)(1);
    title_renderer_set_visible(1);
    flags = game_runtime.display.flags;
    game_runtime.display.background[0] = (uint8)(0xE5);
    game_runtime.display.flags = (uint32)(flags | 8);
    game_runtime.display.background[1] = (uint8)(0xE4);
    game_runtime.display.background[2] = (uint8)(0xDE);
    title_menu_collect_ranks(ranks);
    for (index = 0; index < sizeof(ranks); ++index)
        title_renderer_set_group_value(index + 1, ranks[index]);
    return 0;
}

void title_menu_draw_start_pulse(uint32 ordering, uint32 packet)
{
    uint32 tick, amplitude, red, green, blue, tag, link;
    sint32 high, quotient, scaled;

    FUNCTION_MARKER(0x800CCB38, "ACE3.BIN");
    TILE *tile = ui_packet_span(packet, sizeof(*tile));
    ((uint8 *)&tile->tag)[3] = (uint8)(3);
    tile->rgbc.code = (uint8)(0x60);
    tile->p[0].x = (sint16)(uint16)(110);
    tile->p[0].y = (sint16)(uint16)(229);
    tile->w = (sint16)(uint16)(8);
    tile->h = (sint16)(uint16)(16);
    tick = game_runtime.field_counter;
    high = (((sint64)(sint32)tick * (sint32)0x88888889) >> 32);
    quotient = (sint32)((uint32)high + tick) >> 6;
    quotient = ((uint32)quotient - (uint32)((sint32)tick >> 31));
    amplitude = tick - (uint32)quotient * 120 - 60;
    if ((sint32)amplitude < 0)
        amplitude = 0 - amplitude;
    red = 0 - amplitude * 29;
    green = 0 - amplitude * 132;
    blue = 0 - amplitude * 198;
    high = (((sint64)(sint32)red * (sint32)0x88888889) >> 32);
    scaled = (sint32)((uint32)high + red) >> 5;
    red = (uint32)scaled - (uint32)((sint32)red >> 31) + 229;
    tile->rgbc.r = (uint8)(red);
    high = (((sint64)(sint32)green * (sint32)0x88888889) >> 32);
    scaled = (sint32)((uint32)high + green) >> 5;
    green = (uint32)scaled - (uint32)((sint32)green >> 31) + 228;
    tile->rgbc.g = (uint8)(green);
    tag = tile->tag;
    high = (((sint64)(sint32)blue * (sint32)0x88888889) >> 32);
    scaled = (sint32)((uint32)high + blue) >> 5;
    blue = (uint32)scaled - (uint32)((sint32)blue >> 31) + 222;
    tile->rgbc.b = (uint8)(blue);
    link = *(uint32 *)ui_scene_ot_span(ordering, 4);
    tile->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
    link = *(uint32 *)ui_scene_ot_span(ordering, 4);
    *(uint32 *)ui_scene_ot_span(ordering, 4) = (link & 0xFF000000) | (packet & 0xFFFFFF);
    {
        (void)(uint32)(packet + 16);
        return;
    }
}

uint32 title_menu_start_tick(void)
{
    uint32 packet, ordering, flags, index, result;

    FUNCTION_MARKER(0x800CCD48, "ACE3.BIN");
    ordering = gpu_packet_link((uintptr_t)game_runtime.frame_packets.middle_tag);
    packet = title_menu.packet_cursor;
    title_menu_draw_start_pulse(ordering, packet);
    flags = game_input.pressed;
    if (flags & 0x40)
    {
#if 0
        /* Original link-mode selection */
        title_menu.screen = (uint16)(2);
#else
        title_menu.screen = (uint16)(game_account_load() ? 6 : 3);
        /* Keep the menu XA descriptor previously initialized by link mode */
        menu_audio_track.id = 0x83D;
        menu_audio_track.fade = 0;
        menu_audio_track.duration = 0;
        menu_audio_track.sector_offset = 0;
        menu_audio_track.next = NULL;
#endif
        title_menu.screen_step = (uint16)(0);
        flags = game_runtime.display.flags;
        game_runtime.display.background[0] = (uint8)(0);
        game_runtime.display.flags = (uint32)(flags | 8);
        game_runtime.display.background[1] = (uint8)(112);
        game_runtime.display.background[2] = (uint8)(120);
        sound_play(88);
        for (index = 1; index < 56; ++index)
            title_renderer_set_group_value(index, 0);
        title_menu.idle_ticks = 0;
        return 0;
    }
    result = title_menu.idle_ticks + 1;
    title_menu.idle_ticks = result;
    return result;
}

uint32 title_menu_exit_begin(void)
{
    FUNCTION_MARKER(0x800D75E4, "ACE3.BIN");
    title_menu.phase_ticks = 0;
    title_menu.screen_step = (uint16)(1);
    return 0;
}

uint32 title_menu_exit_tick(void)
{
#if 0
    /* Original memory-card completion and event teardown */
    uint32 result;

    FUNCTION_MARKER(0x800D7600, "ACE3.BIN");
    result = title_card_stop_polling();
    if (result != 0)
    {
        title_card_close_events();
        title_menu.card_events_open = (uint32)(0);
        title_menu.exit_requested = (uint8)(1);
        result = 1;
    }
    return result;
#else
    title_menu.card_events_open = (uint32)(0);
    title_menu.exit_requested = (uint8)(1);
    return 1;
#endif
}

void title_menu_account_begin(void)
{
    FUNCTION_MARKER(0x800D1798, "ACE3.BIN");
    title_menu.phase_ticks = 0;
    title_menu.screen_step = (uint16)(1);
    title_menu.keyboard_arena_begin = arena_alloc(4);
    title_menu.record_banner = (uint8)(0);
    title_menu.panel = (uint16)(0);
    title_menu.panel_step = (uint16)(0);
    title_card_reset_directory();
    title_card_initialize_accounts();
    title_card_initialize_records_hook();
}

uint32 title_menu_records_begin(void)
{
    FUNCTION_MARKER(0x800D3AE8, "ACE3.BIN");
    title_menu.phase_ticks = 0;
    title_menu.screen_step = (uint16)(1);
    title_menu.panel = (uint16)(0);
    title_menu.panel_step = (uint16)(0);
    title_menu.record_mode = (uint8)(0);
    title_renderer_set_hint(111, 148, 0, 0);
    return title_menu_wait_step();
}

uint32 title_menu_link_begin(void)
{
    uint32 mode, ordering, result, flags, color;
    MENU_POLYGON_DESCRIPTOR descriptor;
    static const uint16 coordinates[8] = {266, 28, 311, 28, 266, 148, 311, 148};

    FUNCTION_MARKER(0x800CD9A8, "ACE3.BIN");
    title_menu.link_preview_ticks = (uint8)(0);
    title_menu.screen_step = (uint16)(1);
    title_menu.phase_ticks = 0;
    flags = title_menu.card_events_open;
    if (flags == 0)
    {
        title_card_open_events();
        title_card_begin_polling();
    }
    title_menu.card_events_open = (uint32)(1);
    title_renderer_set_animation(1, 1, 0, 60);
    title_renderer_set_animation(3, 1, 0, 60);
    title_renderer_set_animation(15, 1, 0, 60);
    mode = title_renderer_ordering();
    for (color = 0; color < 8; ++color)
    {
        if (color & 1)
            descriptor.points[color / 2].y = coordinates[color];
        else
            descriptor.points[color / 2].x = coordinates[color];
    }
    /* Only the first flag and RGB bytes are consumed by the packet builder */
    for (color = 0; color < 4; ++color)
    {
        descriptor.color[color][0] = 0;
        descriptor.color[color][1] = 112;
        descriptor.color[color][2] = 120;
    }
    descriptor.color[0][3] = 0;
    ordering = title_menu.packet_cursor;
    result = title_geometry_emit_gouraud_quad(&descriptor, ordering, ui_scene_ot_span(mode + 56, sizeof(uint32)));
    title_menu.packet_cursor = result;
    flags = game_input.kind;
    title_renderer_set_group_value(5, (flags & 0x10) != 0);
    title_renderer_set_visible(190);
    title_renderer_set_group_value(4, 1);
    menu_audio_track.id = 0x83D;
    menu_audio_track.fade = 0;
    menu_audio_track.duration = 0;
    menu_audio_track.sector_offset = 0;
    menu_audio_track.next = NULL;
    title_menu_wait_step();
    title_menu.card_port_hidden = (uint8)(1);
    title_renderer_set_group_value(16, 2);
    return 0;
}

uint32 title_menu_link_tick(void)
{
    uint32 first_card, second_card, mode, available, flags, packet, result;
    uint32 row, column, color, index, old_selection, selection, value, counter;
    sint32 delay, tick, phase, enabled;
    uint16 coordinates[6];
    MENU_POLYGON_DESCRIPTOR descriptor;

    FUNCTION_MARKER(0x800CDB64, "ACE3.BIN");
    first_card = title_card_port_status(0);
    second_card = title_card_port_status(1);
    mode = title_renderer_ordering();
    available = game_runtime.progress.account_flags;
    for (color = 0; color < 3; ++color)
    {
        descriptor.color[color][0] = 0;
        descriptor.color[color][1] = 112;
        descriptor.color[color][2] = 120;
    }
    descriptor.color[0][3] = 0;
    descriptor.color[1][3] = 0;
    value = title_menu.link_preview_ticks;
    available &= 1;
    if (value < 30)
    {
        for (row = 0; row < 5; ++row)
            for (column = 0; column < 5; ++column)
            {
                value = title_menu.link_preview_ticks;
                delay = (2 * (column + row) + 30 - value);
                if (delay >= 0)
                {
                    coordinates[0] = (column * 9 + 275 - (uint32)delay);
                    coordinates[1] = (100 - row * 18);
                    coordinates[2] = (275 + column * 9);
                    coordinates[3] = coordinates[1];
                    coordinates[4] = coordinates[2];
                    coordinates[5] = (2 * delay + 100 - row * 18);
                    for (index = 0; index < 6; ++index)
                    {
                        if (index & 1)
                            descriptor.points[index / 2].y = coordinates[index];
                        else
                            descriptor.points[index / 2].x = coordinates[index];
                    }
                    packet = title_menu.packet_cursor;
                    result = title_geometry_emit_gouraud_triangle(&descriptor, packet, ui_scene_ot_span(mode + 56, sizeof(uint32)));
                    title_menu.packet_cursor = result;
                }
            }
        value = title_menu.link_preview_ticks;
        title_menu.link_preview_ticks = (uint8)(value + 1);
    }
    tick = (sint32)game_runtime.field_counter;
    phase = tick / 64;
    packet = title_menu.packet_cursor;
    result = title_geometry_draw_rotating_marker(70, 410, ((uint32)tick - (uint32)phase * 64) << 6, packet);
    title_menu.packet_cursor = result;
    enabled = title_renderer_animation_mode(1);
    flags = game_input.pressed;
    if ((flags & 0x10) || enabled == 0)
    {
        if ((uint32)enabled - 1 < 2)
        {
            title_renderer_set_animation(1, 3, -1, -1);
            title_renderer_set_animation(3, 3, -1, -1);
            title_renderer_set_animation(8, 3, -1, -1);
            sound_play(89);
        }
        else
        {
            title_renderer_set_animation(1, 0, 0, 0);
            title_renderer_set_animation(3, 0, 0, 0);
            title_renderer_set_animation(8, 0, 0, 0);
            selection = title_menu.link_selection;
            if (selection == 2)
            {
                value = title_menu.screen_step;
                title_menu.screen_step = (uint16)(value + 1);
            }
            else
            {
                selection = title_menu.link_selection;
                title_menu.screen_step = (uint16)(0);
                title_menu.screen = (uint16)(selection + 3);
            }
        }
    }
    else if (flags & 0x20)
    {
        title_menu.screen = (uint16)(1);
        title_menu.screen_step = (uint16)(0);
        title_menu.idle_ticks = 0;
        sound_play(59);
        resource_init_queue(0, 0);
        title_menu.card_events_open = (uint32)(0);
        title_card_close_events();
        return title_card_begin_polling();
    }
    old_selection = title_menu.link_selection;
    enabled = title_renderer_animation_mode(1);
    flags = game_input.pressed;
    if ((flags & 2) && enabled == 2)
    {
        selection = title_menu.link_selection;
        title_menu.link_selection = (uint8)(selection < 2 ? selection + 1 : 0);
        selection = title_menu.link_selection;
        if (selection == 1 && first_card != 5 && second_card != 5)
            title_menu.link_selection = (uint8)(2);
        selection = title_menu.link_selection;
        if (selection == 2 && !available)
            title_menu.link_selection = (uint8)(0);
    }
    flags = game_input.pressed;
    if ((flags & 1) && enabled == 2)
    {
        selection = title_menu.link_selection;
        title_menu.link_selection = (uint8)(selection == 0 ? 2 : selection - 1);
        selection = title_menu.link_selection;
        if (selection == 2 && !available)
            title_menu.link_selection = (uint8)(selection - 1);
        selection = title_menu.link_selection;
        if (selection == 1 && first_card != 5 && second_card != 5)
            title_menu.link_selection = (uint8)(0);
    }
    selection = title_menu.link_selection;
    if (selection != old_selection)
        sound_play(61);
    selection = title_menu.link_selection;
    if (selection == 1 && first_card != 5 && second_card != 5 && title_renderer_animation_mode(1) == 2)
    {
        title_menu.link_selection = (uint8)(0);
    }
    selection = title_menu.link_selection;
    title_renderer_set_group_value(6, selection != 0);
    value = 2;
    if (first_card == 5 || second_card == 5)
    {
        selection = title_menu.link_selection;
        value = selection != 1;
    }
    title_renderer_set_group_value(7, value);
    value = 2;
    if (available)
    {
        selection = title_menu.link_selection;
        value = selection != 2;
    }
    title_renderer_set_group_value(8, value);
    title_renderer_set_group_value(1, 1);
    counter = title_menu.phase_ticks;
    title_menu.phase_ticks = counter + 1;
    flags = game_input.kind;
    title_renderer_set_group_value(5, (flags & 0x10) != 0);
    value = (sint32)menu_audio_track.id;
    result = resource_is_cd_audio(value);
    if (result == 1)
    {
        value = (sint16)title_menu.screen;
        result = 2;
        if (value == 2)
        {
            result = resource_wait(1);
            if (result == 0)
            {
                result = resource_queue_audio(&menu_audio_track, 1, 0);
            }
        }
    }
    return result;
}

void title_menu_draw_account_slots(uint32 layout, uint32 hide_selection)
{
    const char *name;
    static const uint32 first_y[3] = {150, 204, 257};
    static const uint32 second_y[3] = {189, 242, 293};
    uint32 available, selection, flags, i, x, present, result;
    FUNCTION_MARKER(0x800DC098, "ACE3.BIN");
    available = title_card_account_mask();
    if (hide_selection == 1)
    {
        selection = title_menu.account_selection;
        available &= ~((uint32)1 << (selection & 31));
    }
    flags = title_renderer_hint_flags() & 2;
    if (flags == 0)
        return;
    if (layout != 0 && layout != 1)
        return;
    title_text_select_font(4);
    if (layout == 0)
    {
        title_text_set_reveal_range(1, 1, 1);
        title_text_set_blend(2);
    }
    else
    {
        title_text_set_blend(2);
        title_text_set_reveal_range(1, 1, 1);
    }
    title_text_set_reveal_tick(1);
    result = 1;
    for (i = 0; i < 3; ++i)
    {
        x = 106;
        if (layout == 1)
        {
            flags = title_renderer_hint_flags();
            if (flags & 8)
                x = 81;
        }
        title_text_set_pos(x, layout == 0 ? first_y[i] : second_y[i]);
        present = (available >> i) & 1;
        title_text_set_color(present ? 128 : 64, present ? 128 : 64, present ? 128 : 64);
        if (layout == 1 && i != 1)
            title_renderer_set_group_value(i == 0 ? 2 : 4, present == 0);
        name = present ? title_menu_account_name(i + 1) : "empty";
        result = title_text_print((const uint8 *)name, 0, 0);
    }
    {
        (void)(uint32)(layout == 0 ? 1 : result);
        return;
    }
}

void title_menu_draw_name_hint(void)
{
    uint32 flags;
    sint32 x, y;
    FUNCTION_MARKER(0x800DBFE4, "ACE3.BIN");
    flags = title_renderer_hint_flags() & 1;
    if (flags == 0)
        return;
    x = (sint16)title_renderer_hint_x();
    y = (sint16)title_renderer_hint_y();
    {
        (void)(title_menu_draw_account_name((uint32)x, (uint32)y, 0));
        return;
    }
}

void title_menu_draw_account_name(uint32 x, uint32 y, uint32 label)
{
    const char *name;
    static const uint16 offsets[6][4] = {{0, 0, 8, 0}, {0, 9, 8, 9}, {0, 18, 8, 18}, {0, 0, 0, 18}, {4, 0, 4, 18}, {8, 0, 8, 18}};
    uint16 origin[2], coordinates[4];
    MENU_LINE_DESCRIPTOR descriptor;
    uint32 value, flags, packet, ordering, i, j;
    FUNCTION_MARKER(0x800DBD20, "ACE3.BIN");
    title_text_select_font(4);
    title_text_set_blend(2);
    title_text_set_reveal_range(1, 30, 40);
    value = title_renderer_animation_phase(15);
    title_text_set_reveal_tick(value);
    title_text_set_color(128, 128, 128);
    title_text_set_pos((uint32)(sint32)(sint16)x, (uint32)(sint32)(sint16)y);
    name = title_menu_account_name(label);
    title_text_print((const uint8 *)name, 0, 0);
    flags = title_renderer_hint_flags() & 4;
    if (flags == 0)
        return;
    descriptor.color[0] = 40;
    descriptor.color[1] = 40;
    descriptor.color[2] = 40;
    descriptor.semitransparent = 0;
    title_text_get_pos(&origin[0], &origin[1]);
    value = 0;
    for (i = 0; i < 6; ++i)
    {
        for (j = 0; j < 4; ++j)
        {
            coordinates[j] = (origin[j & 1] + offsets[i][j]);
            descriptor.point[j / 2][j % 2] = coordinates[j];
        }
        packet = title_menu.packet_cursor;
        ordering = title_renderer_ordering();
        value = title_geometry_emit_double_line(&descriptor, packet, ordering);
        title_menu.packet_cursor = value;
    }
    {
        (void)(uint32)(value);
        return;
    }
}

static void title_menu_call_panel(const RESOURCE001_PANEL_CALLBACK *callbacks, size_t callback_count, const uint8 *row_start, size_t row_count, sint32 panel, sint32 step)
{
    sint32 index;

    if ((uint32)panel >= row_count)
        abort();
    index = row_start[panel] + step;
    if ((uint32)index >= callback_count)
        abort();
    if (callbacks[index].update)
        (void)callbacks[index].update();
    else if (callbacks[index].enter)
        callbacks[index].enter();
    else
        abort();
}

static void title_menu_call_primary(sint32 panel, sint32 step)
{
#if 0
    static const RESOURCE001_PANEL_CALLBACK callbacks[] =
    {
        {title_menu_link_begin, NULL},
        {title_menu_link_tick, NULL},
        {title_menu_disc_transfer_begin, NULL},
        {title_menu_disc_transfer_tick, NULL},
        {NULL, NULL},
        {title_menu_keyboard_begin, NULL},
        {title_menu_keyboard_tick, NULL},
        {title_menu_confirm_name_begin, NULL},
        {title_menu_confirm_name_tick, NULL},
        {title_menu_difficulty_begin, NULL},
        {title_menu_difficulty_tick, NULL},
        {title_menu_configure_begin, NULL},
        {title_menu_configure_tick, NULL},
        {title_menu_audio_begin, NULL},
        {title_menu_audio_tick, NULL},
        {title_menu_screen_begin, NULL},
        {title_menu_screen_tick, NULL},
        {title_menu_brightness_begin, NULL},
        {title_menu_brightness_tick, NULL},
        {title_menu_save_offer_begin, NULL},
        {title_menu_save_offer_tick, NULL},
        {title_menu_without_save_begin, NULL},
        {title_menu_without_save_tick, NULL},
        {title_save_wait_cards_begin, NULL},
        {title_save_wait_cards_tick, NULL},
        {title_save_unavailable_begin, NULL},
        {title_save_unavailable_tick, NULL},
        {title_save_select_card_begin, NULL},
        {title_save_select_card_tick, NULL},
        {title_save_info_notice_begin, NULL},
        {title_save_info_notice_tick, NULL},
        {title_save_format_confirm_begin, NULL},
        {title_save_format_confirm_tick, NULL},
        {title_save_format_tick, NULL},
        {title_save_check_file_tick, NULL},
        {title_save_check_space_tick, NULL},
        {title_save_space_notice_tick, NULL},
        {title_save_create_file_tick, NULL},
        {title_save_write_icons_tick, NULL},
        {title_save_initialize_directory_tick, NULL},
        {title_save_file_notice_tick, NULL},
        {title_save_read_begin, NULL},
        {title_save_read_directory_tick, NULL},
        {title_save_read_accounts_tick, NULL},
        {title_save_read_notice_tick, NULL},
        {title_save_select_account_begin, NULL},
        {title_save_select_account_tick, NULL},
        {title_save_overwrite_begin, NULL},
        {title_save_overwrite_tick, NULL},
        {NULL, title_save_write_begin},
        {title_save_write_directory_tick, NULL},
        {title_save_write_account_tick, NULL},
        {NULL, title_save_write_notice_tick},
        {title_save_success_tick, NULL},
        {title_save_disc_recovery_begin, NULL},
        {title_save_check_disc_tick, NULL},
        {title_save_recovery_wait_tick, NULL},
        {title_save_recovery_finish_tick, NULL},
    };
    static const uint8 row_start[] = {5, 7, 9, 11, 13, 15, 17, 19, 21, 23, 25, 27, 29, 31, 34, 41, 45, 47, 49, 54};
#else
    static const RESOURCE001_PANEL_CALLBACK callbacks[] = {
        {title_menu_keyboard_begin, NULL}, {title_menu_keyboard_tick, NULL}, {title_menu_confirm_name_begin, NULL}, {title_menu_confirm_name_tick, NULL}, {title_menu_difficulty_begin, NULL}, {title_menu_difficulty_tick, NULL},
    };
    static const uint8 row_start[] = {0, 2, 4};
#endif
    title_menu_call_panel(callbacks, sizeof(callbacks) / sizeof(callbacks[0]), row_start, sizeof(row_start) / sizeof(row_start[0]), panel, step);
}

uint32 title_menu_primary_tick(void)
{
    static const uint16 cursor_position[][2] = {
        {249, 361}, {70, 410}, {249, 361}, {58, 370}, {0, 0}, {0, 0}, {249, 361}, {249, 361}, {249, 361}, {249, 361}, {70, 410},
    };
    uint32 phase, angle, packet, result, value;
    sint32 tick, category, step, adjusted;
    FUNCTION_MARKER(0x800D181C, "ACE3.BIN");
    if (cd_resource_state() == 0)
    {
        game_input.pressed = (uint32)(0);
        game_input.held = (uint32)(0);
    }
    phase = title_menu.panel;
    adjusted = (sint16)(phase - 1);
    if ((uint32)adjusted < sizeof(cursor_position) / sizeof(cursor_position[0]) && cursor_position[adjusted][0] != 0)
    {
        tick = (sint32)game_runtime.field_counter;
        angle = ((uint32)tick - (uint32)(tick / 64) * 64) << 6;
        packet = title_menu.packet_cursor;
        result = title_geometry_draw_rotating_marker(cursor_position[adjusted][0], cursor_position[adjusted][1], angle, packet);
        title_menu.packet_cursor = result;
    }
    category = (sint16)title_menu.panel;
    step = (sint16)title_menu.panel_step;
    title_menu_call_primary(category, step);
    title_menu_draw_name_hint();
    value = title_menu.record_banner;
    title_menu_draw_account_slots(1, value);
    value = title_menu.card_notice;
    title_renderer_set_group_value(17, value);
    value = title_menu.card_port_hidden;
    value = value == 1 ? 2 : title_menu.card_port;
    title_renderer_set_group_value(16, value);
    value = game_runtime.selection.difficulty;
    title_renderer_set_group_value(3, value);
    value = game_input.kind;
    title_renderer_set_group_value(5, (value & 0x10) != 0);
    result = resource_wait(1);
    if (result == 0)
    {
        return resource_queue_audio(&menu_audio_track, 1, 0);
    }
    return result;
}

static void title_menu_call_secondary(sint32 panel, sint32 step)
{
    static const RESOURCE001_PANEL_CALLBACK callbacks[] = {
        {NULL, title_menu_account_begin},
        {title_menu_primary_tick, NULL},
        {NULL, NULL},
        {title_records_wait_cards_begin, NULL},
        {title_records_wait_cards_tick, NULL},
        {title_records_unavailable_begin, NULL},
        {title_records_unavailable_tick, NULL},
        {title_records_select_card_begin, NULL},
        {title_records_select_card_tick, NULL},
        {title_records_info_notice_begin, NULL},
        {title_records_info_notice_tick, NULL},
        {title_records_load_notice_begin, NULL},
        {title_records_load_notice_tick, NULL},
        {title_records_no_account_begin, NULL},
        {title_records_no_account_tick, NULL},
        {title_records_load_begin, NULL},
        {title_records_read_directory_tick, NULL},
        {title_records_read_accounts_tick, NULL},
        {title_records_read_notice_tick, NULL},
        {title_records_select_account_begin, NULL},
        {title_records_select_account_tick, NULL},
        {title_records_apply_account_begin, NULL},
        {title_records_apply_account_tick, NULL},
        {title_records_read_missions_tick, NULL},
        {title_records_mission_notice_tick, NULL},
        {title_records_select_mission_begin, NULL},
        {title_records_select_mission_tick, NULL},
        {title_records_difficulty_begin, NULL},
        {title_records_difficulty_tick, NULL},
        {title_records_disc_recovery_begin, NULL},
        {title_records_check_disc_tick, NULL},
        {title_records_recovery_wait_tick, NULL},
        {title_records_recovery_finish_tick, NULL},
    };
    static const uint8 row_start[] = {3, 5, 7, 9, 11, 13, 15, 19, 21, 25, 27, 29};
    title_menu_call_panel(callbacks, sizeof(callbacks) / sizeof(callbacks[0]), row_start, sizeof(row_start) / sizeof(row_start[0]), panel, step);
}

uint32 title_menu_records_tick(void)
{
    uint32 mode, angle, packet, result, value;
    sint32 tick, category, step;
    FUNCTION_MARKER(0x800D3B3C, "ACE3.BIN");
    mode = title_menu.record_mode;
    if (mode == 0 || mode == 1)
    {
        tick = (sint32)game_runtime.field_counter;
        angle = ((uint32)tick - (uint32)(tick / 64) * 64) << 6;
        packet = title_menu.packet_cursor;
        result = title_geometry_draw_rotating_marker(mode == 1 ? 51 : 70, mode == 1 ? 397 : 410, angle, packet);
        title_menu.packet_cursor = result;
    }
    category = (sint16)title_menu.panel;
    step = (sint16)title_menu.panel_step;
    title_menu_call_secondary(category, step);
    title_menu_draw_name_hint();
    title_menu_draw_account_slots(0, 0);
    value = title_menu.card_notice;
    title_renderer_set_group_value(17, value);
    value = title_menu.card_port_hidden;
    value = value == 1 ? 2 : title_menu.card_port;
    title_renderer_set_group_value(16, value);
    value = game_runtime.selection.difficulty;
    title_renderer_set_group_value(3, value);
    value = game_input.kind;
    title_renderer_set_group_value(5, (value & 0x10) != 0);
    result = resource_wait(1);
    if (result == 0)
    {
        return resource_queue_audio(&menu_audio_track, 1, 0);
    }
    return result;
}

static uint32 title_menu_enter_primary(void)
{
    title_menu_account_begin();
    return 0;
}

static void title_menu_call_screen(uint16 screen, uint16 step)
{
    static uint32 (*const callbacks[][4])(void) = {
        {NULL},
        {title_menu_start_begin, title_menu_start_tick},
#if 0
        {title_menu_link_begin, title_menu_link_tick, title_menu_disc_transfer_begin, title_menu_disc_transfer_tick},
#else
        {NULL},
#endif
        {title_menu_enter_primary, title_menu_primary_tick},
#if 0
        {title_menu_records_begin, title_menu_records_tick},
#else
        {NULL},
#endif
        {NULL},
        {title_menu_exit_begin, title_menu_exit_tick},
        {NULL},
    };
    if (screen == 2 || screen == 4)
        abort();
    if (screen >= sizeof(callbacks) / sizeof(callbacks[0]))
        abort();
    if (!callbacks[screen][0])
        return;
    if (step >= sizeof(callbacks[0]) / sizeof(callbacks[0][0]) || !callbacks[screen][step])
        abort();
    (void)callbacks[screen][step]();
}

uint32 title_menu_frame(void)
{
    uint32 timer, frame_packet, index, packet, value;
    sint32 age;
    FUNCTION_MARKER(0x800CC764, "ACE3.BIN");
    timer = title_menu.frame_count;
    frame_packet = gpu_packet_link((uintptr_t)game_runtime.frame_packets.middle_tag);
    title_menu.frame_count = timer + 1;
    index = game_runtime.display_buffer;
    packet = title_menu.packet_pages[index];
    title_menu.packet_cursor = packet;
    title_renderer_begin_frame(frame_packet + 4);
    value = title_renderer_ordering();
    title_text_begin_frame(value + 4);
    title_text_set_pos(50, 300);
    title_text_set_color(64, 64, 64);
    value = title_menu.card_events_open;
    if (value != 0)
        title_card_poll_tick();
    title_renderer_draw_frame();
    title_menu_call_screen(title_menu.screen, title_menu.screen_step);
    age = (sint32)title_menu.idle_ticks;
    if (age >= 601)
    {
        value = title_menu.card_events_open;
        if (value != 0)
            title_card_close_events();
        game_runtime.playback.return_mode = (uint32)(4);
        game_runtime.playback.movie_idx = (uint8)(0);
        return 0x60000005;
    }
    value = title_menu.exit_requested;
    if (value == 0)
        return 0;
    resource_init_queue(0, 0);
    value = title_menu.card_events_open;
    if (value != 0)
        title_card_close_events();
    value = game_runtime.progress.flow_flags;
    game_runtime.account.route_mode = (uint16)(value & 0x10 ? 0 : 2);
    value = game_runtime.display.flags;
    game_runtime.display.flags = (uint32)(value | 4);
    return 0x60000003;
}

uint32 title_menu_configure_begin(void)
{
    uint32 step;

    FUNCTION_MARKER(0x800CEECC, "ACE3.BIN");
    step = title_menu.panel_step;
    title_menu.phase_ticks = 0;
    title_menu.panel_step = (uint16)(step + 1);
    title_renderer_reset_view(13);
    title_renderer_set_visible(0x2056);
    title_renderer_set_group_value(1, 17);
    title_renderer_set_group_value(4, 23);
    title_renderer_set_hint(64, 50, 1, 0);
    return title_menu_wait_step();
}

uint32 title_menu_configure_tick(void)
{
    uint32 flags, result;

    FUNCTION_MARKER(0x800CEF44, "ACE3.BIN");
    flags = game_input.held;
    if (flags & 0x10)
    {
        flags = game_input.pressed;
        if (flags & 0x10)
        {
            title_menu.panel = (uint16)(4);
            title_menu.panel_step = (uint16)(0);
            sound_play(89);
            goto finish;
        }
    }
    flags = game_input.held;
    if (flags & 0x20)
    {
        flags = game_input.pressed;
        if (flags & 0x20)
        {
            title_menu.panel = (uint16)(7);
            title_menu.panel_step = (uint16)(0);
            sound_play(59);
        }
    }
finish:
    result = title_menu.phase_ticks + 1;
    title_menu.phase_ticks = result;
    return result;
}

uint32 title_menu_audio_begin(void)
{
    uint32 step;

    FUNCTION_MARKER(0x800CF004, "ACE3.BIN");
    step = title_menu.panel_step;
    title_menu.phase_ticks = 0;
    title_menu.panel_step = (uint16)(step + 1);
    title_renderer_reset_view(14);
    title_renderer_set_visible(0x4016);
    title_renderer_set_group_value(1, 18);
    title_renderer_set_hint(64, 50, 1, 0);
    return title_menu_wait_step();
}

static uint32 title_menu_scale_fader_byte(uint32 value)
{
    uint32 product;
    sint32 high, quotient;

    product = (value << 3) - value;
    product = (product << 3) - value;
    product = (product << 2) + value;
    high = (((sint64)(sint32)product * (sint32)0x80808081) >> 32);
    quotient = (sint32)((uint32)high + product) >> 7;
    return (uint32)quotient - (uint32)((sint32)product >> 31);
}

uint32 title_menu_audio_tick(void)
{
    MENU_FILL_DESCRIPTOR meter, bar;

    uint32 ordering, meter_ordering, packet, result, value, flags, selection, channel, product;
    uint32 directions, buttons, offset;
    sint32 high, quotient, volume;

    FUNCTION_MARKER(0x800CF06C, "ACE3.BIN");
    ordering = title_renderer_ordering();
    value = title_audio_mean_magnitude();
    product = (value << 4) - value;
    high = (((sint64)(sint32)product * (sint32)0x80010003) >> 32);
    quotient = (sint32)((uint32)high + product) >> 14;
    value = (uint32)quotient - (uint32)((sint32)product >> 31);
    meter.x = 68;
    meter.y = 224;
    meter.width = (uint16)(value * 7);
    meter.height = 12;
    meter.red = 200;
    meter.green = 200;
    meter.blue = 200;
    /* User-authorized fix for the original uninitialized blend flags */
    meter.semi = 0;
    bar.semi = 0;
    meter_ordering = title_renderer_ordering();
    packet = title_menu.packet_cursor;
    result = title_geometry_emit_fill(&meter, packet, ui_scene_ot_span(meter_ordering, sizeof(uint32)));
    title_menu.packet_cursor = result;
    flags = audio_settings.flags;
    if (flags & 1)
        value = 2;
    else
        value = title_menu.audio_cursor != 0;
    title_renderer_set_group_value(6, value);
    flags = audio_settings.flags;
    value = (flags & 1) ? title_menu.audio_cursor != 0 : 2;
    title_renderer_set_group_value(7, value);
    flags = audio_settings.flags;
    resource_set_mono((flags & 1) != 0);
    resource_apply_mix();
    value = audio_settings.volume[0];
    for (channel = 0; channel < 2; ++channel)
    {
        bar.x = (uint16)(title_menu_scale_fader_byte(value) + 69);
        bar.y = (uint16)(229 + channel * 78);
        bar.width = 4;
        bar.height = 64;
        bar.red = 0;
        bar.green = 0;
        bar.blue = 0;
        packet = title_menu.packet_cursor;
        result = title_geometry_emit_fill(&bar, packet, ui_scene_ot_span(ordering + 4, sizeof(uint32)));
        title_menu.packet_cursor = result;
        bar.x = 70;
        selection = title_menu.audio_cursor;
        if (selection == channel + 1)
        {
            bar.red = 200;
            bar.green = 200;
            bar.blue = 200;
        }
        else
        {
            bar.red = 8;
            bar.green = 200;
            bar.blue = 192;
        }
        value = audio_settings.volume[channel];
        bar.width = (uint16)title_menu_scale_fader_byte(value);
        packet = title_menu.packet_cursor;
        result = title_geometry_emit_fill(&bar, packet, ui_scene_ot_span(ordering + 4, sizeof(uint32)));
        title_menu.packet_cursor = result;
        if (channel == 0)
            value = audio_settings.volume[1];
    }
    directions = game_input.repeat;
    if (directions & 2)
    {
        selection = title_menu.audio_cursor;
        title_menu.audio_cursor = (uint32)((sint32)selection < 2 ? selection + 1 : 0);
        sound_play(61);
    }
    directions = game_input.repeat;
    if (directions & 1)
    {
        selection = title_menu.audio_cursor;
        title_menu.audio_cursor = (uint32)((sint32)selection > 0 ? selection - 1 : 2);
        sound_play(61);
    }
    directions = game_input.repeat;
    if (directions & 12)
    {
        selection = title_menu.audio_cursor;
        if (selection == 0)
        {
            if (directions & 8)
            {
                flags = audio_settings.flags;
                audio_settings.flags = (uint8)((flags & 1) ? flags & 0xFE : flags | 1);
            }
            if (directions & 4)
            {
                flags = audio_settings.flags;
                audio_settings.flags = (uint8)((flags & 1) ? flags & 0xFE : flags | 1);
            }
            sound_play(61);
        }
        else if (selection == 1 || selection == 2)
        {
            offset = selection - 1;
            volume = audio_settings.volume[offset];
            if (directions & 8)
                volume += 16;
            if (directions & 4)
                volume -= 16;
            if (volume >= 256)
                volume = 255;
            else if (volume < 0)
                volume = 0;
            value = audio_settings.volume[offset];
            if (value != (uint32)volume)
            {
                sound_play(46);
                audio_settings.volume[offset] = (uint8)volume;
            }
        }
    }
    buttons = game_input.pressed;
    if (buttons & 0x10)
    {
        title_menu.panel = (uint16)(6);
        title_menu.panel_step = (uint16)(0);
        sound_play(89);
    }
    else if (buttons & 0x20)
    {
        title_menu.panel = (uint16)(3);
        title_menu.panel_step = (uint16)(0);
        sound_play(59);
    }
    title_renderer_set_group_value(8, 0);
    title_renderer_set_group_value(9, 0);
    title_renderer_set_group_value(10, 0);
    selection = title_menu.audio_cursor;
    title_renderer_set_group_value(selection + 8, 1);
    result = title_menu.phase_ticks + 1;
    title_menu.phase_ticks = result;
    return result;
}

uint32 title_menu_screen_begin(void)
{
    uint32 step;

    FUNCTION_MARKER(0x800CF750, "ACE3.BIN");
    step = title_menu.panel_step;
    title_menu.panel_step = (uint16)(step + 1);
    title_renderer_reset_view(15);
    title_renderer_set_visible(0x8010);
    title_renderer_set_group_value(1, 20);
    title_renderer_set_hint(64, 50, 0, 0);
    return title_menu_wait_step();
}

uint32 title_menu_screen_tick(void)
{
    uint32 x, y, directions, flags, value;
    sint32 dx = 0, dy = 0;

    FUNCTION_MARKER(0x800CF7B4, "ACE3.BIN");
    directions = game_input.repeat;
    x = game_runtime.display.screen[0];
    y = game_runtime.display.screen[1];
    if (directions & 15)
    {
        if (directions & 8)
            dx = (sint32)x < 12;
        if ((directions & 4) && (sint32)x >= -11)
            dx = -1;
        if ((directions & 2) && (sint32)y < 8)
            dy = 1;
        if ((directions & 1) && (sint32)y > 0)
            dy = -1;
        if (dx != 0 || dy != 0)
        {
            sound_play(46);
            game_runtime.display.screen[0] = (uint32)(x + (uint32)dx);
            game_runtime.display.screen[1] = (uint32)(y + (uint32)dy);
        }
    }
    title_renderer_set_group_value(6, dy == -1);
    title_renderer_set_group_value(7, dx == 1);
    title_renderer_set_group_value(8, dy == 1);
    title_renderer_set_group_value(9, dx == -1);
    flags = game_input.kind;
    if (flags & 0x10)
    {
        if ((sint16)pad_state[0].axis[3] >= 129)
        {
            game_runtime.display.screen[0] = (uint32)(0xFFFFFFFE);
            game_runtime.display.screen[1] = (uint32)(0);
        }
    }
    else if (pad_state[0].pressed & 0x1000)
    {
        game_runtime.display.screen[0] = (uint32)(0xFFFFFFFE);
        game_runtime.display.screen[1] = (uint32)(0);
        sound_play(48);
    }
    if (game_gpu_get_ode() == 1)
    {
        value = game_runtime.display.flags;
        game_runtime.display.flags = (uint32)(value | 2);
    }
    flags = game_input.pressed;
    if ((flags & 0x30) == 0)
        return 0x800C0000;
    value = game_runtime.display.flags;
    game_runtime.display.flags = (uint32)(value | 2);
    if (flags & 0x10)
    {
        title_menu.panel = (uint16)(7);
        title_menu.panel_step = (uint16)(0);
        return sound_play(89);
    }
    if ((flags & 0x20) == 0)
        return 0;
    title_menu.panel = (uint16)(6);
    title_menu.panel_step = (uint16)(0);
    return sound_play(59);
}

uint32 title_menu_brightness_begin(void)
{
    uint32 step;

    FUNCTION_MARKER(0x800CFA34, "ACE3.BIN");
    step = title_menu.panel_step;
    title_menu.panel_step = (uint16)(step + 1);
    title_renderer_set_visible(0x10016);
    title_renderer_set_group_value(1, 19);
    title_renderer_set_hint(64, 50, 1, 0);
    return title_menu_wait_step();
}

uint32 title_menu_brightness_tick(void)
{
    uint32 flags;

    FUNCTION_MARKER(0x800CFA94, "ACE3.BIN");
    flags = game_input.pressed;
    if (flags & 0x10)
    {
        title_menu.panel = (uint16)(5);
        title_menu.panel_step = (uint16)(0);
        sound_play(89);
    }
    else if (flags & 0x20)
    {
        title_menu.panel = (uint16)(4);
        title_menu.panel_step = (uint16)(0);
        sound_play(59);
    }
    return title_geometry_draw_brightness();
}

uint32 title_menu_save_offer_begin(void)
{
    uint32 step;

    FUNCTION_MARKER(0x800CFB2C, "ACE3.BIN");
    step = title_menu.panel_step;
    title_menu.panel_step = (uint16)(step + 1);
    title_renderer_reset_view(17);
    title_renderer_set_visible(0x20056);
    title_renderer_set_animation(2, 3, -1, -1);
    title_renderer_set_group_value(2, 1);
    title_renderer_set_group_value(1, 22);
    title_renderer_set_group_value(4, 22);
    title_renderer_set_hint(64, 50, 1, 0);
    title_menu_wait_step();
    title_menu.card_port_hidden = (uint8)(1);
    return 1;
}

uint32 title_menu_save_offer_tick(void)
{
    uint32 buttons;

    FUNCTION_MARKER(0x800CFBD0, "ACE3.BIN");
    buttons = game_input.pressed;
    if (buttons & 0x10)
    {
        title_menu.panel = (uint16)(9);
        title_menu.panel_step = (uint16)(0);
        return sound_play(89);
    }
    if (buttons & 0x20)
    {
        title_menu.panel = (uint16)(8);
        title_menu.panel_step = (uint16)(0);
        return sound_play(59);
    }
    return 0;
}

uint32 title_menu_without_save_begin(void)
{
    uint32 step;

    FUNCTION_MARKER(0x800CFC38, "ACE3.BIN");
    title_renderer_set_group_value(2, 2);
    title_renderer_set_group_value(1, 23);
    step = title_menu.panel_step;
    title_menu.panel_step = (uint16)(step + 1);
    title_menu_wait_step();
    title_renderer_set_group_value(4, 22);
    return 0;
}

uint32 title_menu_without_save_tick(void)
{
    const char *name;
    uint32 buttons, value;

    FUNCTION_MARKER(0x800CFC94, "ACE3.BIN");
    buttons = game_input.pressed;
    if (buttons & 0x10)
    {
        title_menu.panel = (uint16)(19);
        title_menu.panel_step = (uint16)(0);
        title_account_reset_campaign();
        game_runtime.account.route_mode = (uint16)(2);
        value = title_account_id_seed();
        game_runtime.progress.account_id = (uint32)(value);
        name = title_menu_account_name(0);
        game_account_set_name(name);
        return sound_play(89);
    }
    if ((buttons & 0x20) == 0)
        return 0x800E0000;
    title_menu.panel = (uint16)(0);
    title_menu.panel_step = (uint16)(0);
    return sound_play(59);
}

uint32 title_menu_wait_counter(void)
{
    sint32 remaining = 4;

    FUNCTION_MARKER(0x800DA484, "ACE3.BIN");
    while (remaining-- >= 0)
    {
    }
    return remaining;
}

void title_menu_reserve_wait(void)
{
    FUNCTION_MARKER(0x800DA418, "ACE3.BIN");
    arena_alloc(12);
    {
        (void)(uint32)(title_menu_wait_counter());
        return;
    }
}

uint32 title_menu_wait_step(void)
{
    FUNCTION_MARKER(0x800DA49C, "ACE3.BIN");
    return title_menu_wait_counter();
}

uint32 title_bootstrap_begin(void)
{
    RESOURCE_STREAM_BUFFER asset_allocation;
    uint32 flags, value;

    FUNCTION_MARKER(0x800CCE98, "ACE3.BIN");
    game_runtime.display.mode = (uint32)(1);
    flags = game_runtime.display.flags;
    game_runtime.display.background[0] = (uint8)(0);
    game_runtime.display.field_wait = (uint32)(0);
    game_runtime.display.flags = (uint32)(flags | 9);
    game_runtime.display.background[1] = (uint8)(0);
    game_runtime.display.background[2] = (uint8)(0);
    resource_init_queue(0, 0);
    title_renderer_initialize();
    title_renderer_bind_commands(title_boot_commands, sizeof(title_boot_commands));
    value = game_runtime.progress.flow_flags;
    title_renderer_set_visible((value & 2) != 0);
    title_text_initialize();
    game_runtime.resource_slot.resource_id = (uint32)(112);
    value = resource_total_size(&game_runtime.resource_slot.resource_id, 1);
    asset_allocation = arena_alloc_backed(value);
    value = asset_allocation.encoded_begin;
    game_runtime.resource_pending = 1;
    resource_queue_asset(&game_runtime.resource_slot, asset_allocation, resource_asset_read_finished);
    title_menu.phase_ticks = 120;
    return 0x04000000;
}

uint32 title_bootstrap_bind_assets(void)
{
    uint32 bundle_base, bundle_bytes, directory, next_section, flags;
#if 0
    uint32 value;
#endif

    uint8 *bundle_data;
    uint32 bundle_read_bytes;

    FUNCTION_MARKER(0x800CCFA4, "ACE3.BIN");
    bundle_base = game_runtime.resource_slot.encoded_base;
    bundle_data = game_runtime.resource_slot.data;
    bundle_read_bytes = game_runtime.resource_slot.read_bytes;
    (void)resource_relocate_directory(bundle_base, 0, bundle_data, bundle_read_bytes);
    const TITLE_ASSET_DIRECTORY *assets = (const TITLE_ASSET_DIRECTORY *)(bundle_data + 4);
    sound_load_bank((SOUND_BANK_INPUT){bundle_data, bundle_read_bytes, assets->sound_bank - bundle_base});
    sound_enable_reverb();
    bundle_bytes = resource_get_size(112);
    directory = assets->commands;
    next_section = assets->icons;
    if (directory < bundle_base || next_section <= directory || next_section - bundle_base > bundle_bytes)
        abort();
    title_renderer_bind_commands(bundle_data + (directory - bundle_base), next_section - directory);
#if 0
    value = assets->icon_palette;
    title_load_card_header_assets(bundle_data, bundle_read_bytes, assets->icons - bundle_base, value - bundle_base);
#else
    /* preserve subsequent arena offsets until their address dependencies are retired */
    (void)arena_alloc(GAME_CARD_ICON_BLOCK_BYTES);
#endif
    if (assets->textures < bundle_base || assets->textures - bundle_base > bundle_read_bytes || bundle_read_bytes - (assets->textures - bundle_base) < 16)
        abort();
    title_texture_upload_group(bundle_data + (assets->textures - bundle_base), bundle_read_bytes - (assets->textures - bundle_base));
    if (assets->image < bundle_base || assets->image - bundle_base > bundle_read_bytes)
        abort();
    title_texture_upload_image(assets->image, bundle_data + (assets->image - bundle_base), bundle_read_bytes - (assets->image - bundle_base));
    arena_set_cursor(assets->textures);
    title_menu_reserve_wait();
    flags = game_runtime.progress.flow_flags;
    game_runtime.progress.flow_flags = (uint32)(flags & 0xFFFFFFFD);
    return 0x04000000;
}

uint32 title_bootstrap_tick(void)
{
    uint8 no_arguments[1] = {0};
    uint32 timer;

    FUNCTION_MARKER(0x800CD07C, "ACE3.BIN");
    timer = title_menu.phase_ticks;
    --timer;
    title_menu.phase_ticks = timer;
    if (!game_runtime.resource_pending)
    {
        if (!(game_runtime.progress.flow_flags & 2) || (game_input.pressed & 0x10) || (sint32)timer < 0)
            return title_bootstrap_bind_assets();
    }
    title_text_begin_frame(gpu_packet_link((uintptr_t)game_runtime.frame_packets.middle_tag));
    title_text_select_font(7);
    title_text_set_blend(0xFFFFFFFF);
    title_text_set_reveal_range(1, 1, 1);
    title_text_set_reveal_tick(1);
    title_text_set_color(24, 184, 208);
    if (game_runtime.progress.flow_flags & 2)
    {
        title_text_set_pos(233, 145);
        title_text_print((const uint8 *)"LOG OUT", no_arguments, 0);
    }
    title_renderer_begin_frame(gpu_packet_link((uintptr_t)game_runtime.frame_packets.middle_tag));
    title_renderer_draw_frame();
    return 0;
}

void title_menu_initialize_hook(void)
{
    FUNCTION_MARKER(0x800DA25C, "ACE3.BIN");
}

void title_menu_reserve_legacy_packets(void)
{
    uint32 page;

    FUNCTION_MARKER(0x800CD1A0, "ACE3.BIN");
    arena_alloc(36);
    for (page = 0; page < 2; ++page)
    {
        arena_alloc(20000);
        arena_alloc(1200);
    }
    return;
}

uint32 title_records_wait_cards_begin(void)
{
    uint32 step;
    FUNCTION_MARKER(0x800D1A5C, "ACE3.BIN");
    title_renderer_set_animation(2, 1, 0, 60);
    title_renderer_set_animation(4, 1, 0, 60);
    title_renderer_set_animation(9, 1, 0, 60);
    step = title_menu.panel_step;
    title_menu.panel_step = (uint16)(step + 1);
    title_renderer_set_group_value(1, 7);
    title_renderer_set_group_value(4, 5);
    return 0;
}

uint32 title_records_wait_cards_tick(void)
{
    uint32 first, second;
    FUNCTION_MARKER(0x800D1ADC, "ACE3.BIN");
    first = title_card_port_status(0);
    second = title_card_port_status(1);
    if (first == 0)
        return second;
    if (second == 0)
        return 0x800E0000;
    title_menu.panel = (uint16)(1);
    title_menu.panel_step = (uint16)(0);
    return 1;
}

uint32 title_records_unavailable_begin(void)
{
    uint32 first, second, step;
    FUNCTION_MARKER(0x800D1B2C, "ACE3.BIN");
    first = title_card_port_status(0);
    second = title_card_port_status(1);
    if ((first == 2 || first == 6) && (second == 2 || second == 6))
    {
        title_renderer_reset_view(8);
        title_renderer_set_visible(318);
        title_renderer_set_animation(4, 1, 0, 60);
        step = title_menu.panel_step;
        title_menu.panel_step = (uint16)(step + 1);
        title_renderer_set_group_value(1, 2);
        title_renderer_set_group_value(6, 2);
        title_renderer_set_group_value(7, 2);
        title_menu_wait_step();
        title_menu.card_notice = (uint8)(1);
        title_renderer_set_group_value(4, 5);
        return 0;
    }
    title_menu.panel = (uint16)(2);
    title_menu.panel_step = (uint16)(0);
    return 2;
}

uint32 title_records_unavailable_tick(void)
{
    uint32 first, second, flags;
    FUNCTION_MARKER(0x800D1C18, "ACE3.BIN");
    first = title_card_port_status(0);
    second = title_card_port_status(1);
    if ((first == 2 || first == 6) && (second == 2 || second == 6))
    {
        flags = game_input.pressed & 0x20;
        if (flags == 0)
            return 0;
        title_menu.screen = (uint16)(2);
        title_menu.screen_step = (uint16)(0);
        title_renderer_set_visible(158);
        sound_play(89);
        title_menu.card_notice = (uint8)(0);
        return 0;
    }
    title_menu.panel = (uint16)(0);
    title_menu.panel_step = (uint16)(0);
    title_menu.card_notice = (uint8)(0);
    title_renderer_set_group_value(1, 7);
    title_renderer_set_group_value(4, 5);
    return 0;
}

uint32 title_records_select_card_begin(void)
{
    uint32 first, second, step;
    FUNCTION_MARKER(0x800D1CF0, "ACE3.BIN");
    first = title_card_port_status(0);
    second = title_card_port_status(1);
    step = title_menu.panel_step;
    title_menu.panel_step = (uint16)(step + 1);
    title_renderer_reset_view(8);
    title_renderer_set_visible(318);
    title_renderer_set_animation(4, 1, 0, 60);
    title_renderer_set_group_value(1, 3);
    title_renderer_set_group_value(4, 2);
    title_menu_wait_step();
    title_menu.card_port_hidden = (uint8)(0);
    if (first == 5)
    {
        if (second == 5)
            return 5;
        title_menu.card_port = (uint8)(0);
        return 0;
    }
    if (second == 5)
    {
        title_menu.card_port = (uint8)(1);
    }
    return 1;
}

uint32 title_records_select_card_tick(void)
{
    uint32 first, second, flags, old_selection, selection;
    FUNCTION_MARKER(0x800D1DCC, "ACE3.BIN");
    first = title_card_port_status(0);
    second = title_card_port_status(1);
    if ((first == 2 || first == 6) && (second == 2 || second == 6))
    {
        title_menu.panel = (uint16)(1);
        title_menu.panel_step = (uint16)(0);
        title_renderer_set_animation(2, 1, 0, 20);
        title_renderer_set_animation(1, 3, -1, -1);
        title_menu.card_port_hidden = (uint8)(1);
        return 0;
    }
    flags = game_input.repeat;
    old_selection = title_menu.card_port;
    if (flags & 3)
        title_menu.card_port = (uint8)(old_selection ^ 1);
    if (first == 0 || first == 2 || first == 6)
    {
        title_menu.card_port = (uint8)(1);
    }
    if (second == 0 || second == 2 || second == 6)
    {
        title_menu.card_port = (uint8)(0);
    }
    selection = title_menu.card_port;
    if (selection != old_selection)
        sound_play(61);
    selection = title_menu.card_port;
    if (selection == 0)
    {
        title_renderer_set_group_value(6, 0);
        title_renderer_set_group_value(7, 1);
    }
    selection = title_menu.card_port;
    if (selection == 1)
    {
        title_renderer_set_group_value(6, 1);
        title_renderer_set_group_value(7, 0);
    }
    if (first == 0 || first == 2 || first == 6)
        title_renderer_set_group_value(6, 2);
    if (second == 0 || second == 2 || second == 6)
        title_renderer_set_group_value(7, 2);
    flags = game_input.pressed;
    if (flags & 0x10)
    {
        title_menu.panel = (uint16)(3);
        title_menu.panel_step = (uint16)(0);
        sound_play(89);
        title_renderer_set_group_value(4, 5);
        return 0;
    }
    if (flags & 0x20)
    {
        title_menu.screen = (uint16)(2);
        title_menu.screen_step = (uint16)(0);
        title_renderer_set_visible(190);
        return sound_play(59);
    }
    return 0x800E0000;
}

uint32 title_records_info_notice_begin(void)
{
    uint32 port, status, step;
    FUNCTION_MARKER(0x800D202C, "ACE3.BIN");
    port = title_menu.card_port;
    status = title_card_port_status(port);
    title_renderer_set_group_value(1, 7);
    if (status == 1)
    {
        step = title_menu.panel_step;
        title_menu.panel_step = (uint16)(step + 1);
        title_renderer_set_group_value(1, 4);
        title_menu_wait_step();
        title_menu.card_notice = (uint8)(1);
        return 0;
    }
    title_menu.panel = (uint16)(4);
    return 4;
}

uint32 title_records_info_notice_tick(void)
{
    uint32 port, status, flags;
    FUNCTION_MARKER(0x800D20BC, "ACE3.BIN");
    port = title_menu.card_port;
    status = title_card_port_status(port);
    if (status == 2 || status == 6)
    {
        title_menu.panel = (uint16)(0);
        title_menu.panel_step = (uint16)(0);
    }
    else
    {
        flags = game_input.pressed & 0x20;
        if (flags == 0)
            return 0;
        title_menu.panel = (uint16)(0);
        title_menu.panel_step = (uint16)(0);
        sound_play(89);
    }
    title_menu.card_notice = (uint8)(0);
    return 0;
}

uint32 title_records_load_notice_tick(void)
{
    uint32 port, status, flags;
    FUNCTION_MARKER(0x800D21D0, "ACE3.BIN");
    port = title_menu.card_port;
    status = title_card_port_status(port);
    if (status == 2 || status == 6)
    {
        title_menu.panel = (uint16)(0);
        title_menu.panel_step = (uint16)(0);
    }
    else
    {
        flags = game_input.pressed & 0x20;
        if (flags == 0)
            return 0;
        title_menu.panel = (uint16)(0);
        title_menu.panel_step = (uint16)(0);
        sound_play(89);
    }
    title_menu.card_notice = (uint8)(0);
    return 0;
}

uint32 title_records_no_account_tick(void)
{
    uint32 port, status, flags;
    FUNCTION_MARKER(0x800D22E4, "ACE3.BIN");
    port = title_menu.card_port;
    status = title_card_port_status(port);
    if (status == 2 || status == 6)
    {
        title_menu.panel = (uint16)(0);
        title_menu.panel_step = (uint16)(0);
    }
    else
    {
        flags = game_input.pressed & 0x20;
        if (flags == 0)
            return 0;
        title_menu.panel = (uint16)(2);
        title_menu.panel_step = (uint16)(0);
        sound_play(89);
    }
    title_menu.card_notice = (uint8)(0);
    return 0;
}

uint32 title_records_load_notice_begin(void)
{
    uint32 port, status, step;
    FUNCTION_MARKER(0x800D2154, "ACE3.BIN");
    port = title_menu.card_port;
    status = title_card_port_status(port);
    if (status == 3)
    {
        step = title_menu.panel_step;
        title_menu.panel_step = (uint16)(step + 1);
        title_renderer_set_group_value(1, 5);
        title_menu_wait_step();
        title_menu.card_notice = (uint8)(1);
        return 1;
    }
    title_menu.panel = (uint16)(5);
    return 5;
}

uint32 title_records_no_account_begin(void)
{
    uint32 port, status, step;
    FUNCTION_MARKER(0x800D2268, "ACE3.BIN");
    port = title_menu.card_port;
    status = title_card_port_status(port);
    if (status == 4)
    {
        step = title_menu.panel_step;
        title_menu.panel_step = (uint16)(step + 1);
        title_renderer_set_group_value(1, 6);
        title_menu_wait_step();
        title_menu.card_notice = (uint8)(1);
        return 1;
    }
    title_menu.panel = (uint16)(6);
    return 6;
}

uint32 title_records_load_begin(void)
{
    uint32 step, result;
    FUNCTION_MARKER(0x800D237C, "ACE3.BIN");
    step = title_menu.panel_step;
    title_menu.panel_step = (uint16)(step + 1);
    title_renderer_set_group_value(1, 7);
    title_card_reset_directory();
    result = title_card_initialize_accounts();
    title_card_initialize_records_hook();
    return result;
}

uint32 title_records_read_directory_tick(void)
{
    uint32 port, status, step;
    FUNCTION_MARKER(0x800D23CC, "ACE3.BIN");
    port = title_menu.card_port;
    status = title_card_read_directory(port, 1);
    if (status == 0xFFFFFFFF)
    {
        step = title_menu.panel_step;
        title_menu.panel_step = (uint16)(step + 2);
        title_renderer_set_group_value(1, 14);
        title_menu_wait_step();
    }
    if (status == 2)
    {
        step = title_menu.panel_step;
        title_menu.panel_step = (uint16)(step + 2);
        title_renderer_set_group_value(1, 8);
        title_menu_wait_step();
    }
    if (status == 1)
    {
        step = title_menu.panel_step + 1;
        title_menu.panel_step = (uint16)(step);
        return step;
    }
    return 1;
}

uint32 title_records_read_accounts_tick(void)
{
    uint32 port, status, step;
    FUNCTION_MARKER(0x800D248C, "ACE3.BIN");
    port = title_menu.card_port;
    status = title_card_read_accounts(port, 1);
    if (status == 0xFFFFFFFF)
    {
        step = title_menu.panel_step;
        title_menu.panel_step = (uint16)(step + 1);
        title_renderer_set_group_value(1, 14);
        title_menu_wait_step();
        title_menu.card_notice = (uint8)(1);
    }
    if (status == 2)
    {
        step = title_menu.panel_step;
        title_menu.panel_step = (uint16)(step + 1);
        title_renderer_set_group_value(1, 8);
        title_menu_wait_step();
        title_menu.card_notice = (uint8)(1);
    }
    if (status == 1)
    {
        title_menu.panel = (uint16)(7);
        title_menu.panel_step = (uint16)(0);
    }
    return 7;
}

uint32 title_records_read_notice_tick(void)
{
    uint32 port, status, flags;
    FUNCTION_MARKER(0x800D255C, "ACE3.BIN");
    port = title_menu.card_port;
    status = title_card_port_status(port);
    title_menu.card_notice = (uint8)(1);
    if (status == 2 || status == 6)
    {
        title_menu.panel = (uint16)(0);
        title_menu.panel_step = (uint16)(0);
    }
    else
    {
        flags = game_input.pressed & 0x20;
        if (flags == 0)
            return 0;
        title_menu.panel = (uint16)(2);
        title_menu.panel_step = (uint16)(0);
        sound_play(89);
    }
    title_menu.card_notice = (uint8)(0);
    return 0;
}

uint32 title_records_select_account_begin(void)
{
    uint32 step;
    FUNCTION_MARKER(0x800D2600, "ACE3.BIN");
    step = title_menu.panel_step;
    title_menu.phase_ticks = 0;
    title_menu.record_mode = (uint8)(0);
    title_menu.panel_step = (uint16)(step + 1);
    title_renderer_set_visible(190);
    title_renderer_set_animation(1, 1, 0, 20);
    title_renderer_set_animation(2, 3, -1, -1);
    title_renderer_set_animation(4, 3, -1, -1);
    title_renderer_set_animation(9, 3, -1, -1);
    title_renderer_set_animation(1, 1, 0, 60);
    title_renderer_set_animation(5, 1, 0, 60);
    title_renderer_set_animation(10, 1, 0, 60);
    title_renderer_set_name_hint_mode(1);
    title_renderer_set_group_value(1, 9);
    title_renderer_set_group_value(4, 3);
    return title_menu_wait_step();
}

uint32 title_records_select_account_tick(void)
{
    uint32 port, status, occupied, flags, selection;
    FUNCTION_MARKER(0x800D26F0, "ACE3.BIN");
    port = title_menu.card_port;
    status = title_card_port_status(port);
    occupied = title_card_account_mask();
    if (status == 2 || status == 6)
    {
        title_menu.panel = (uint16)(0);
        title_menu.panel_step = (uint16)(0);
        return title_renderer_set_name_hint_mode(0);
    }
    flags = game_input.repeat;
    if (flags & 2)
    {
        selection = title_menu.account_selection;
        title_menu.account_selection = (uint8)(selection < 2 ? selection + 1 : 0);
        sound_play(61);
    }
    flags = game_input.repeat;
    if (flags & 1)
    {
        selection = title_menu.account_selection;
        title_menu.account_selection = (uint8)(selection == 0 ? 2 : selection - 1);
        sound_play(61);
    }
    selection = title_menu.account_selection;
    title_renderer_set_group_value(6, selection != 0);
    selection = title_menu.account_selection;
    title_renderer_set_group_value(7, selection != 1);
    selection = title_menu.account_selection;
    title_renderer_set_group_value(8, selection != 2);
    flags = game_input.pressed;
    if (flags & 0x10)
    {
        selection = title_menu.account_selection;
        if ((occupied >> (selection & 31)) & 1)
        {
            title_menu.panel = (uint16)(8);
            title_menu.panel_step = (uint16)(0);
            sound_play(89);
            title_renderer_set_group_value(4, 5);
            return 0;
        }
        return sound_play(48);
    }
    if ((flags & 0x20) == 0)
        return 0;
    title_menu.panel = (uint16)(2);
    title_menu.panel_step = (uint16)(0);
    title_renderer_set_animation(2, 1, 0, 20);
    title_renderer_set_animation(1, 3, -1, -1);
    title_renderer_set_name_hint_mode(0);
    return sound_play(59);
}

uint32 title_records_apply_account_begin(void)
{
    uint32 step;
    FUNCTION_MARKER(0x800D2954, "ACE3.BIN");
    step = title_menu.panel_step;
    title_menu.panel_step = (uint16)(step + 1);
    title_renderer_set_group_value(1, 12);
    return title_menu_wait_step();
}

uint32 title_records_apply_account_tick(void)
{
    uint32 selection, flags, step;
    FUNCTION_MARKER(0x800D2994, "ACE3.BIN");
    selection = title_menu.account_selection;
    title_card_select_account_copy(selection);
    title_card_decode_account(&title_menu.record_selection);
    flags = game_runtime.progress.account_flags;
    game_runtime.progress.account_flags = (uint32)(flags & ~1);
    step = title_menu.panel_step + 1;
    title_menu.panel_step = (uint16)(step);
    return step;
}

uint32 title_records_read_missions_tick(void)
{
    uint32 port, slot, status, step;
    FUNCTION_MARKER(0x800D29FC, "ACE3.BIN");
    port = title_menu.card_port;
    slot = title_menu.account_selection;
    status = title_card_read_missions(port, slot, 1);
    if (status == 0xFFFFFFFF)
    {
        step = title_menu.panel_step;
        title_menu.panel_step = (uint16)(step + 1);
        title_renderer_set_group_value(1, 14);
        title_menu_wait_step();
        title_menu.card_notice = (uint8)(1);
        title_renderer_set_name_hint_mode(0);
    }
    if (status == 2)
    {
        step = title_menu.panel_step;
        title_menu.panel_step = (uint16)(step + 1);
        title_renderer_set_group_value(1, 8);
        title_menu_wait_step();
        title_menu.card_notice = (uint8)(1);
    }
    if (status == 1)
    {
        title_menu.panel = (uint16)(9);
        title_menu.panel_step = (uint16)(0);
        title_menu_wait_step();
        return title_renderer_set_name_hint_mode(0);
    }
    return 9;
}

uint32 title_records_mission_notice_tick(void)
{
    uint32 port, status, flags;
    FUNCTION_MARKER(0x800D2AE8, "ACE3.BIN");
    port = title_menu.card_port;
    status = title_card_port_status(port);
    title_menu.card_notice = (uint8)(1);
    flags = game_input.pressed;
    if (flags & 0x20)
    {
        title_menu.panel = (uint16)(2);
        title_menu.panel_step = (uint16)(0);
        sound_play(89);
        title_menu.card_notice = (uint8)(0);
        title_renderer_set_name_hint_mode(0);
    }
    if (status == 2 || status == 6)
    {
        title_menu.screen = (uint16)(2);
        title_menu.screen_step = (uint16)(0);
        title_menu.card_notice = (uint8)(0);
        return title_renderer_set_name_hint_mode(0);
    }
    return 6;
}

uint32 title_records_select_mission_begin(void)
{
    uint32 step, i, selection, flags, available, index;
    FUNCTION_MARKER(0x800D2E74, "ACE3.BIN");
    title_renderer_set_visible(542);
    title_renderer_reset_view(9);
    title_renderer_set_animation(1, 1, 0, 20);
    title_renderer_set_animation(15, 1, 0, 20);
    title_renderer_set_group_value(1, 10);
    step = title_menu.panel_step;
    title_menu.record_mode = (uint8)(1);
    title_menu.panel_step = (uint16)(step + 1);
    for (i = 0; i < 8; ++i)
    {
        selection = title_menu.record_selection;
        title_renderer_set_group_value(i + 6, selection == i);
    }
    available = title_records_special_entry_available();
    if (available)
    {
        flags = game_runtime.progress.flow_flags;
        game_runtime.progress.flow_flags = (uint32)(flags | 0x10);
        selection = title_menu.record_selection;
        index = selection == 6 ? 13 : 12;
    }
    else
    {
        flags = game_runtime.progress.flow_flags;
        game_runtime.progress.flow_flags = (uint32)(flags & ~0x10);
        index = 13;
    }
    title_renderer_set_group_value(index, 2);
    return 0;
}

uint32 title_records_select_mission_tick(void)
{
    uint32 mode, occupied, flags, selection, port, status, i, index, valid;
    FUNCTION_MARKER(0x800D2FB8, "ACE3.BIN");
    mode = title_renderer_animation_mode(1);
    occupied = title_card_current_record_mask();
    port = title_menu.card_port;
    status = title_card_port_status(port);
    if (status == 2 || status == 6)
    {
        title_menu.panel = (uint16)(0);
        title_menu.panel_step = (uint16)(0);
        title_menu.record_mode = (uint8)(0);
        return title_renderer_set_visible(318);
    }
    flags = game_input.repeat;
    if ((flags & 8) && mode == 2)
    {
        flags = game_runtime.progress.flow_flags;
        selection = title_menu.record_selection;
        if (flags & 0x10)
        {
            if (selection >= 8 || selection == 6)
                title_menu.record_selection = (uint8)(0);
            else if (selection == 5)
                title_menu.record_selection = (uint8)(7);
            else if (selection == 7)
                title_menu.record_selection = (uint8)(6);
            else
            {
                title_menu.record_selection = (uint8)(selection + 1);
            }
        }
        else
            title_menu.record_selection = (uint8)(selection < 6 ? selection + 1 : 0);
        sound_play(61);
    }
    flags = game_input.repeat;
    if ((flags & 4) && mode == 2)
    {
        flags = game_runtime.progress.flow_flags;
        selection = title_menu.record_selection;
        if (flags & 0x10)
        {
            if (selection >= 8)
                title_menu.record_selection = (uint8)(0);
            else if (selection == 0 || selection == 6)
                title_menu.record_selection = (uint8)(7);
            else if (selection == 7)
                title_menu.record_selection = (uint8)(5);
            else
            {
                title_menu.record_selection = (uint8)(selection - 1);
            }
        }
        else
            title_menu.record_selection = (uint8)(selection == 0 ? 6 : selection - 1);
        sound_play(61);
    }
    for (i = 0; i < 8; ++i)
    {
        selection = title_menu.record_selection;
        title_renderer_set_group_value(i + 6, selection == i);
    }
    flags = game_runtime.progress.flow_flags;
    index = 13;
    if (flags & 0x10)
    {
        selection = title_menu.record_selection;
        if (selection != 6)
            index = 12;
    }
    title_renderer_set_group_value(index, 2);
    port = title_menu.account_selection;
    selection = title_menu.record_selection;
    title_records_draw_mission_preview(port, selection);
    flags = game_input.pressed;
    if ((flags & 0x30) == 0x10 || mode == 0)
    {
        selection = title_menu.record_selection;
        valid = selection - 6 < 2;
        if (!valid)
        {
            selection = title_menu.record_selection;
            valid = (occupied >> (selection & 31)) & 1;
        }
        if (valid)
        {
            if (mode != 0 && mode != 3)
            {
                title_renderer_set_animation(1, 3, -1, -1);
                title_menu.record_action = (uint8)(1);
                sound_play(89);
            }
            else
            {
                status = title_menu.record_action;
                if (status == 1)
                {
                    selection = title_menu.record_selection;
                    if (selection < 6)
                    {
                        title_renderer_set_animation(1, 0, 0, 0);
                        selection = title_menu.record_selection;
                        title_card_decode_mission(selection);
                        title_menu.panel = (uint16)(11);
                        title_menu.panel_step = (uint16)(0);
                        game_runtime.account.route_mode = (uint16)(2);
                        flags = game_runtime.progress.flow_flags;
                        game_runtime.progress.flow_flags = (uint32)(flags & ~0x10);
                    }
                    selection = title_menu.record_selection;
                    if (selection == 6)
                    {
                        title_menu.panel = (uint16)(10);
                        title_menu.panel_step = (uint16)(0);
                        flags = game_runtime.progress.flow_flags;
                        game_runtime.progress.flow_flags = (uint32)(flags & ~0x10);
                    }
                    selection = title_menu.record_selection;
                    if (selection == 7)
                    {
                        title_menu.screen = (uint16)(6);
                        title_menu.screen_step = (uint16)(0);
                    }
                    selection = title_menu.record_selection;
                    game_runtime.progress.record_slot = (uint8)(selection);
                }
            }
        }
        else if (flags & 0x10)
            sound_play(48);
    }
    flags = game_input.pressed & 0x20;
    if (flags || mode == 0)
    {
        if (mode != 0 && mode != 3)
        {
            title_renderer_set_animation(1, 3, -1, -1);
            title_menu.record_action = (uint8)(2);
            return sound_play(59);
        }
        status = title_menu.record_action;
        if (status != 2)
            return 2;
        title_renderer_set_animation(1, 0, 0, 0);
        title_menu.panel = (uint16)(7);
        title_menu.panel_step = (uint16)(0);
        return title_renderer_set_hint(64, 30, 0, 0);
    }
    return flags;
}

uint32 title_records_difficulty_begin(void)
{
    uint32 step;
    FUNCTION_MARKER(0x800D34FC, "ACE3.BIN");
    step = title_menu.panel_step;
    title_menu.panel_step = (uint16)(step + 1);
    title_renderer_set_visible(1086);
    title_renderer_set_animation(15, 1, 0, 60);
    title_renderer_set_group_value(1, 11);
    title_renderer_set_group_value(4, 6);
    title_renderer_set_hint(64, 50, 1, 0);
    title_menu_wait_step();
    title_renderer_set_group_value(4, 6);
    return 0;
}

uint32 title_records_disc_recovery_begin(void)
{
    uint32 step;
    FUNCTION_MARKER(0x800D37FC, "ACE3.BIN");
    step = title_menu.panel_step;
    title_menu.recovery_notice_offset = (uint32)(0);
    title_menu.panel_step = (uint16)(step + 1);
    return title_renderer_set_hint(111, 150, 0, 0);
}

uint32 title_records_recovery_wait_tick(void)
{
    uint32 result;
    FUNCTION_MARKER(0x800D3A24, "ACE3.BIN");
    title_renderer_set_visible(0x400036);
    title_renderer_set_group_value(4, 4);
    title_menu.record_mode = (uint8)(0);
    result = resource_get_recovery_state();
    if (result == 3)
    {
        result = title_menu.panel_step + 1;
        title_menu.panel_step = (uint16)(result);
    }
    return result;
}

uint32 title_records_recovery_finish_tick(void)
{
    uint32 result;
    FUNCTION_MARKER(0x800D3A8C, "ACE3.BIN");
    result = resource_get_recovery_state();
    if (result == 1)
    {
        title_renderer_set_group_value(1, 73);
        title_menu_wait_step();
        result = title_menu.panel_step;
        title_menu.recovery_notice_offset = (uint32)(31);
        result -= 2;
        title_menu.panel_step = (uint16)(result);
    }
    return result;
}

uint32 title_records_check_disc_tick(void)
{
    uint32 status, kind, value, step;
    sint32 index;
    FUNCTION_MARKER(0x800D3840, "ACE3.BIN");
    status = cd_resource_state();
    if (status == 0)
    {
        title_renderer_set_group_value(1, 73);
        return 0;
    }
    index = (sint16)game_runtime.selection.mission;
    kind = title_mission_disc((uint32)index);
    if (kind == 3 || status == 3 || (status == 1 && kind == 1))
    {
        title_menu.screen = (uint16)(6);
        title_menu.screen_step = (uint16)(0);
        return 6;
    }
    if (status == 4 || status == 0xFFFFFFFF)
    {
        if (kind == 1)
        {
            value = title_menu.recovery_notice_offset;
            title_renderer_set_group_value(1, value + 70);
        }
        index = (sint16)game_runtime.selection.mission;
        kind = title_mission_disc((uint32)index);
        if (kind == 2)
        {
            value = title_menu.recovery_notice_offset;
            title_renderer_set_group_value(1, value + 71);
        }
        resource_begin_recovery();
        title_menu_wait_step();
        step = title_menu.panel_step + 1;
        title_menu.panel_step = (uint16)(step);
        return step;
    }
    if (status == 1)
    {
        value = title_menu.recovery_notice_offset;
        title_renderer_set_group_value(1, value + 71);
        resource_begin_recovery();
        title_menu_wait_step();
        step = title_menu.panel_step + 1;
        title_menu.panel_step = (uint16)(step);
        return 0x800C0000;
    }
    if (status != 2)
        return 0x800C0000;
    index = (sint16)game_runtime.selection.mission;
    kind = title_mission_disc((uint32)index);
    if (kind == 2)
    {
        title_menu.screen = (uint16)(6);
        title_menu.screen_step = (uint16)(0);
        return 6;
    }
    value = title_menu.recovery_notice_offset;
    title_renderer_set_group_value(1, value + 70);
    resource_begin_recovery();
    title_menu_wait_step();
    step = title_menu.panel_step + 1;
    title_menu.panel_step = (uint16)(step);
    return step;
}

uint32 title_records_difficulty_tick(void)
{
    uint32 port, status, flags, value, i, result;
    sint32 selection;
    FUNCTION_MARKER(0x800D3584, "ACE3.BIN");
    port = title_menu.card_port;
    status = title_card_port_status(port);
    title_text_select_font(4);
    title_text_set_color(128, 128, 128);
    title_text_set_blend(2);
    title_text_set_pos(106, 166);
    title_text_print((const uint8 *)"easy", 0, 0);
    title_text_set_pos(106, 218);
    title_text_print((const uint8 *)"normal", 0, 0);
    title_text_set_pos(106, 270);
    title_text_print((const uint8 *)"hard", 0, 0);
    flags = game_input.repeat;
    if (flags & 2)
    {
        selection = (sint16)title_menu.difficulty_cursor;
        value = title_menu.difficulty_cursor;
        title_menu.difficulty_cursor = (uint16)(selection < 2 ? value + 1 : 0);
        sound_play(61);
    }
    flags = game_input.repeat;
    if (flags & 1)
    {
        selection = (sint16)title_menu.difficulty_cursor;
        value = title_menu.difficulty_cursor;
        title_menu.difficulty_cursor = (uint16)(selection > 0 ? value - 1 : 2);
        sound_play(61);
    }
    flags = game_input.pressed;
    if (flags & 0x10)
    {
        selection = (sint16)title_menu.difficulty_cursor;
        game_runtime.selection.difficulty = (uint32)((uint32)selection);
        for (i = 0; i < 60; ++i)
        {
            game_runtime.progress.mission_rating[4][i] = (uint8)(0);
            game_runtime.progress.mission_time[4][i] = (uint16)(0xD2F0);
            game_runtime.progress.mission_count[i] = (uint32)(0);
            game_runtime.progress.mission_losses[i] = (uint32)(0);
        }
        title_menu.panel = (uint16)(11);
        title_menu.panel_step = (uint16)(0);
        sound_play(89);
        game_runtime.account.route_mode = (uint16)(2);
        game_runtime.selection.mission = (uint16)(1);
    }
    else if (flags & 0x20)
    {
        title_menu.panel = (uint16)(9);
        title_menu.panel_step = (uint16)(0);
        sound_play(59);
    }
    for (i = 0; i < 3; ++i)
    {
        selection = (sint16)title_menu.difficulty_cursor;
        title_renderer_set_group_value(i + 6, i != (uint32)selection);
    }
    result = 6;
    if (status == 2 || status == 6)
    {
        title_menu.panel = (uint16)(2);
        title_menu.panel_step = (uint16)(0);
        result = title_renderer_set_hint(111, 148, 0, 0);
    }
    return result;
}

uint32 title_records_special_entry_available(void)
{
    static const uint8 missions[5] = {34, 39, 18, 53, 48};
    uint32 group, value, next;
    FUNCTION_MARKER(0x800D2BA0, "ACE3.BIN");
    for (group = 0; group < 5; ++group)
    {
        value = game_runtime.progress.mission_rating[0][missions[group]];
        next = game_runtime.progress.mission_rating[1][missions[group]];
        value |= next;
        next = game_runtime.progress.mission_rating[2][missions[group]];
        value |= next;
        next = game_runtime.progress.mission_rating[3][missions[group]];
        value |= next;
        if (value == 0)
            return 0;
    }
    return 1;
}

static void title_records_format_mission_time(uint32 selection, uint8 text[MISSION_TIME_TEXT_SIZE])
{
    uint32 wire_word_0;
    const uint8 *buffer;
    sint32 elapsed;
    FUNCTION_MARKER(0x800D5AC8, "ACE3.BIN");
    buffer = title_mission_record(selection & 255);
    elapsed = (memcpy(&wire_word_0, buffer + 0x114, sizeof(wire_word_0)), wire_word_0);
    format_mission_time(text, elapsed);
}

void title_records_draw_mission_preview(uint32 slot, uint32 selection)
{
    const char *name;
    const char *text;
    uint32 value;
    uint8 mission_time[MISSION_TIME_TEXT_SIZE];
    FUNCTION_MARKER(0x800D2C84, "ACE3.BIN");
    title_text_select_font(4);
    title_text_set_blend(2);
    title_text_set_reveal_range(1, 10, 20);
    value = title_renderer_animation_phase(15);
    title_text_set_reveal_tick(value);
    title_text_set_color(128, 128, 128);
    title_text_set_pos(64, 50);
    name = title_menu_account_name(0);
    title_text_print((const uint8 *)name, 0, 0);
    title_text_select_font(3);
    selection &= 255;
    if (selection == 6)
    {
        title_text_set_pos(140, 327);
        {
            (void)(uint32)(title_text_print((const uint8 *)"new file", 0, 0));
            return;
        }
    }
    slot &= 255;
    if (selection == 7)
    {
        title_text_set_pos(140, 327);
        {
            (void)(uint32)(title_text_print((const uint8 *)"simulation", 0, 0));
            return;
        }
    }
    if (!title_card_mission_present(slot, selection))
    {
        title_text_set_pos(140, 327);
        {
            (void)(uint32)(title_text_print((const uint8 *)"no data", 0, 0));
            return;
        }
    }
    title_text_set_pos(106, 280);
    text = title_records_mission_label(selection);
    title_text_print((const uint8 *)text, 0, 0);
    title_text_set_pos(106, 320);
    title_text_print((const uint8 *)"level", 0, 0);
    title_text_set_pos(150, 320);
    text = title_records_difficulty_label(selection);
    title_text_print((const uint8 *)text, 0, 0);
    title_text_set_pos(106, 340);
    title_text_print((const uint8 *)"time", 0, 0);
    title_text_set_pos(150, 340);
    title_records_format_mission_time(selection, mission_time);
    title_text_print(mission_time, 0, 0);
    title_text_set_pos(106, 360);
    title_text_print((const uint8 *)"unit", 0, 0);
    title_text_set_pos(150, 360);
    text = title_records_unit_label(selection);
    title_text_print((const uint8 *)text, 0, 0);
    title_text_set_pos(106, 380);
    title_text_print((const uint8 *)"control", 0, 0);
    title_text_set_pos(150, 380);
    text = title_records_control_label(slot);
    {
        (void)(uint32)(title_text_print((const uint8 *)text, 0, 0));
        return;
    }
}

uint32 title_save_wait_cards_begin(void)
{
    uint32 step;

    FUNCTION_MARKER(0x800CFD28, "ACE3.BIN");
    step = title_menu.panel_step;
    title_menu.panel_step = (uint16)(step + 1);
    title_renderer_set_group_value(1, 26);
    return 0;
}

uint32 title_save_wait_cards_tick(void)
{
    uint32 first, second;

    FUNCTION_MARKER(0x800CFD60, "ACE3.BIN");
    first = title_card_port_status(0);
    second = title_card_port_status(1);
    if (first == 0 || first == 6 || second == 0 || second == 6)
        return second;
    title_menu.panel = (uint16)(10);
    title_menu.panel_step = (uint16)(0);
    title_renderer_set_group_value(4, 21);
    return 0;
}

uint32 title_save_unavailable_begin(void)
{
    uint32 first, second, step;

    FUNCTION_MARKER(0x800CFDCC, "ACE3.BIN");
    first = title_card_port_status(0);
    second = title_card_port_status(1);
    if ((first == 2 || first == 6) && (second == 2 || second == 6))
    {
        title_renderer_set_animation(1, 3, -1, -1);
        title_renderer_set_animation(2, 1, 0, 20);
        step = title_menu.panel_step;
        title_menu.panel_step = (uint16)(step + 1);
        title_renderer_set_group_value(1, 24);
        title_menu_wait_step();
        title_menu.card_port_hidden = (uint8)(1);
        return title_renderer_set_visible(0x20056);
    }
    title_menu.panel = (uint16)(11);
    title_menu.panel_step = (uint16)(0);
    return 11;
}

uint32 title_save_unavailable_tick(void)
{
    const char *name;
    uint32 first, second, buttons, value;

    FUNCTION_MARKER(0x800CFEA4, "ACE3.BIN");
    first = title_card_port_status(0);
    second = title_card_port_status(1);
    if ((first != 2 && first != 6) || (second != 2 && second != 6))
    {
        title_menu.panel = (uint16)(9);
        title_menu.panel_step = (uint16)(0);
        return 9;
    }
    buttons = game_input.pressed;
    if (buttons & 0x10)
    {
        title_menu.panel = (uint16)(19);
        title_menu.panel_step = (uint16)(0);
        title_account_reset_campaign();
        game_runtime.account.route_mode = (uint16)(2);
        value = title_account_id_seed();
        game_runtime.progress.account_id = (uint32)(value);
        name = title_menu_account_name(0);
        game_account_set_name(name);
        return sound_play(89);
    }
    if ((buttons & 0x20) == 0)
        return 0;
    title_menu.panel = (uint16)(7);
    title_menu.panel_step = (uint16)(0);
    return sound_play(59);
}

uint32 title_save_select_card_begin(void)
{
    uint32 first, second, step;

    FUNCTION_MARKER(0x800CFF90, "ACE3.BIN");
    first = title_card_port_status(0);
    second = title_card_port_status(1);
    step = title_menu.panel_step;
    title_menu.panel_step = (uint16)(step + 1);
    title_renderer_reset_view(18);
    title_renderer_set_visible(0x40036);
    title_renderer_set_animation(4, 1, 0, 60);
    title_renderer_set_group_value(1, 25);
    title_renderer_set_group_value(4, 2);
    title_renderer_set_hint(64, 50, 1, 0);
    title_menu_wait_step();
    if (first != 5 && second == 5)
    {
        title_menu.card_port = (uint8)(1);
    }
    title_menu.card_port_hidden = (uint8)(0);
    return 0;
}

uint32 title_save_select_card_tick(void)
{
    uint32 directions, previous, first, second, value, buttons;

    FUNCTION_MARKER(0x800D0074, "ACE3.BIN");
    directions = game_input.repeat;
    previous = title_menu.card_port;
    if (directions & 3)
        title_menu.card_port = (uint8)(previous ^ 1);
    first = title_card_port_status(0);
    second = title_card_port_status(1);
    if ((first == 2 || first == 6) && (second == 2 || second == 6))
    {
        title_menu.card_port_hidden = (uint8)(1);
        title_menu.panel = (uint16)(10);
        title_menu.panel_step = (uint16)(0);
        title_renderer_reset_view(17);
        title_renderer_set_visible(0x20056);
        title_renderer_set_animation(2, 1, 0, 20);
        return title_renderer_set_animation(1, 3, -1, -1);
    }
    if (first == 0 || first == 2 || first == 6)
    {
        title_menu.card_port = (uint8)(1);
    }
    if (second == 0 || second == 2 || second == 6)
    {
        title_menu.card_port = (uint8)(0);
    }
    value = title_menu.card_port;
    if (value != previous)
        sound_play(61);
    value = title_menu.card_port;
    if (value == 0)
    {
        title_renderer_set_group_value(6, 0);
        title_renderer_set_group_value(7, 1);
    }
    value = title_menu.card_port;
    if (value == 1)
    {
        title_renderer_set_group_value(6, 1);
        title_renderer_set_group_value(7, 0);
    }
    if (first == 0 || first == 2)
        title_renderer_set_group_value(6, 2);
    if (second == 0 || second == 2)
        title_renderer_set_group_value(7, 2);
    buttons = game_input.pressed;
    if (buttons & 0x10)
    {
        title_menu.panel = (uint16)(12);
        title_menu.panel_step = (uint16)(0);
        sound_play(89);
        return title_menu_wait_step();
    }
    if ((buttons & 0x20) == 0)
        return 0;
    title_menu.panel = (uint16)(7);
    title_menu.panel_step = (uint16)(0);
    return sound_play(59);
}

uint32 title_save_info_notice_begin(void)
{
    uint32 port, value, step;

    FUNCTION_MARKER(0x800D02DC, "ACE3.BIN");
    title_renderer_set_group_value(1, 26);
    port = title_menu.card_port;
    value = title_card_port_status(port);
    if (value != 1)
    {
        title_menu.panel = (uint16)(13);
        return 13;
    }
    step = title_menu.panel_step;
    title_menu.panel_step = (uint16)(step + 1);
    title_renderer_reset_view(17);
    title_renderer_set_visible(0x20056);
    title_renderer_set_group_value(1, 28);
    title_renderer_set_group_value(4, 21);
    return title_menu_wait_step();
}

uint32 title_save_info_notice_tick(void)
{
    const char *name;
    uint32 port, value, buttons;

    FUNCTION_MARKER(0x800D0384, "ACE3.BIN");
    port = title_menu.card_port;
    value = title_card_port_status(port);
    if (value == 2 || value == 6)
    {
        title_menu.panel = (uint16)(9);
        title_menu.panel_step = (uint16)(0);
        return 9;
    }
    buttons = game_input.pressed;
    if (buttons & 0x10)
    {
        title_menu.panel = (uint16)(19);
        title_menu.panel_step = (uint16)(0);
        title_account_reset_campaign();
        game_runtime.account.route_mode = (uint16)(2);
        value = title_account_id_seed();
        game_runtime.progress.account_id = (uint32)(value);
        name = title_menu_account_name(0);
        game_account_set_name(name);
        return sound_play(89);
    }
    if ((buttons & 0x20) == 0)
        return 0;
    title_menu.panel = (uint16)(7);
    title_menu.panel_step = (uint16)(0);
    return sound_play(59);
}

uint32 title_save_format_confirm_begin(void)
{
    uint32 port, value, step;

    FUNCTION_MARKER(0x800D0460, "ACE3.BIN");
    port = title_menu.card_port;
    value = title_card_port_status(port);
    if (value != 3)
    {
        title_menu.panel = (uint16)(14);
        return 14;
    }
    step = title_menu.panel_step;
    title_menu.panel_step = (uint16)(step + 1);
    title_renderer_reset_view(19);
    title_renderer_set_visible(0x80056);
    title_renderer_set_animation(1, 3, -1, -1);
    title_renderer_set_animation(2, 3, -1, -1);
    title_renderer_set_animation(3, 3, -1, -1);
    title_renderer_set_animation(4, 1, 0, 20);
    title_renderer_set_group_value(1, 30);
    title_renderer_set_group_value(4, 21);
    return title_menu_wait_step();
}

uint32 title_save_format_confirm_tick(void)
{
    uint32 port, value, buttons, step;

    FUNCTION_MARKER(0x800D0548, "ACE3.BIN");
    port = title_menu.card_port;
    value = title_card_port_status(port);
    if (value == 2 || value == 6)
    {
        title_menu.panel = (uint16)(9);
        title_menu.panel_step = (uint16)(0);
        return 9;
    }
    if (value == 0)
        return 0x800C0000;
    buttons = game_input.pressed;
    if (buttons & 0x40)
    {
        title_renderer_set_group_value(1, 32);
        step = title_menu.panel_step;
        title_menu.panel_step = (uint16)(step + 1);
        sound_play(89);
        return title_menu_wait_step();
    }
    if ((buttons & 0x20) == 0)
        return 0;
    title_menu.panel = (uint16)(7);
    title_menu.panel_step = (uint16)(0);
    return sound_play(59);
}

uint32 title_save_format_tick(void)
{
    uint32 port, value;

    FUNCTION_MARKER(0x800D061C, "ACE3.BIN");
    port = title_menu.card_port;
    value = title_card_format_port(port);
    if (value == 0)
        return 0;
    title_menu.panel = (uint16)(14);
    title_menu.panel_step = (uint16)(3);
    return 0;
}

uint32 title_save_check_file_tick(void)
{
    uint32 port, value, step;

    FUNCTION_MARKER(0x800D0668, "ACE3.BIN");
    port = title_menu.card_port;
    value = title_card_port_status(port);
    if (value == 4)
    {
        step = title_menu.panel_step + 1;
        title_menu.panel_step = (uint16)(step);
        return step;
    }
    title_menu.panel = (uint16)(15);
    title_renderer_set_group_value(1, 26);
    return 0;
}

uint32 title_save_check_space_tick(void)
{
    uint32 port, value, step;

    FUNCTION_MARKER(0x800D06D4, "ACE3.BIN");
    port = title_menu.card_port;
    value = title_card_free_blocks(port);
    if (value == 0xFFFFFFFF)
        return 1;
    if ((sint32)value < 2)
    {
        step = title_menu.panel_step;
        title_menu.panel_step = (uint16)(step + 1);
        title_renderer_set_group_value(1, 34);
        title_menu_wait_step();
        return title_renderer_set_visible(0x20016);
    }
    step = title_menu.panel_step + 2;
    title_menu.panel_step = (uint16)(step);
    return step;
}

uint32 title_save_space_notice_tick(void)
{
    const char *name;
    uint32 port, value, buttons;

    FUNCTION_MARKER(0x800D076C, "ACE3.BIN");
    port = title_menu.card_port;
    value = title_card_port_status(port);
    if (value == 2 || value == 6)
    {
        title_menu.panel = (uint16)(9);
        title_menu.panel_step = (uint16)(0);
        return 9;
    }
    buttons = game_input.pressed;
    if (buttons & 0x10)
    {
        title_menu.panel = (uint16)(19);
        title_menu.panel_step = (uint16)(0);
        title_account_reset_campaign();
        game_runtime.account.route_mode = (uint16)(2);
        value = title_account_id_seed();
        game_runtime.progress.account_id = (uint32)(value);
        name = title_menu_account_name(0);
        game_account_set_name(name);
        return sound_play(89);
    }
    if ((buttons & 0x20) == 0)
        return 0;
    title_menu.panel = (uint16)(7);
    title_menu.panel_step = (uint16)(0);
    return sound_play(59);
}

uint32 title_save_create_file_tick(void)
{
    uint32 port, value, step;

    FUNCTION_MARKER(0x800D0848, "ACE3.BIN");
    port = title_menu.card_port;
    value = title_card_create_file(port);
    if (value == 1)
    {
        step = title_menu.panel_step + 1;
        title_menu.panel_step = (uint16)(step);
        return step;
    }
    if (value != 0xFFFFFFFF)
        return 0xFFFFFFFF;
    step = title_menu.panel_step;
    title_menu.panel_step = (uint16)(step + 3);
    title_renderer_set_group_value(1, 35);
    title_menu_wait_step();
    return title_renderer_set_visible(0x20016);
}

uint32 title_save_write_icons_tick(void)
{
    uint32 port, value, step;

    FUNCTION_MARKER(0x800D08DC, "ACE3.BIN");
    port = title_menu.card_port;
    value = title_card_write_icons(port, 1);
    if (value == 0xFFFFFFFF)
    {
        step = title_menu.panel_step;
        title_menu.panel_step = (uint16)(step + 2);
        title_renderer_set_group_value(1, 35);
        title_menu_wait_step();
        title_renderer_set_visible(0x20016);
    }
    if (value != 1)
        return 1;
    step = title_menu.panel_step + 1;
    title_menu.panel_step = (uint16)(step);
    return step;
}

uint32 title_save_initialize_directory_tick(void)
{
    uint32 port, value, step;

    FUNCTION_MARKER(0x800D0978, "ACE3.BIN");
    port = title_menu.card_port;
    value = title_card_write_directory(port, -1, 1);
    if (value == 0xFFFFFFFF)
    {
        step = title_menu.panel_step;
        title_menu.panel_step = (uint16)(step + 1);
        title_renderer_set_group_value(1, 35);
        title_menu_wait_step();
        title_renderer_set_visible(0x20016);
    }
    if (value == 1)
    {
        title_menu.panel = (uint16)(16);
        title_menu.panel_step = (uint16)(0);
    }
    return 16;
}

uint32 title_save_file_notice_tick(void)
{
    const char *name;
    uint32 port, value, buttons;

    FUNCTION_MARKER(0x800D0A10, "ACE3.BIN");
    port = title_menu.card_port;
    value = title_card_port_status(port);
    if (value == 2 || value == 6)
    {
        title_menu.panel = (uint16)(9);
        title_menu.panel_step = (uint16)(0);
        return 9;
    }
    buttons = game_input.pressed;
    if (buttons & 0x10)
    {
        title_menu.panel = (uint16)(19);
        title_menu.panel_step = (uint16)(0);
        title_account_reset_campaign();
        game_runtime.account.route_mode = (uint16)(2);
        value = title_account_id_seed();
        game_runtime.progress.account_id = (uint32)(value);
        name = title_menu_account_name(0);
        game_account_set_name(name);
        return sound_play(89);
    }
    if ((buttons & 0x20) == 0)
        return 0;
    title_menu.panel = (uint16)(7);
    title_menu.panel_step = (uint16)(0);
    return sound_play(59);
}

uint32 title_save_read_begin(void)
{
    uint32 step;

    FUNCTION_MARKER(0x800D0AEC, "ACE3.BIN");
    step = title_menu.panel_step + 1;
    title_menu.panel_step = (uint16)(step);
    return step;
}

uint32 title_save_read_directory_tick(void)
{
    uint32 port, value, step;

    FUNCTION_MARKER(0x800D0B0C, "ACE3.BIN");
    port = title_menu.card_port;
    value = title_card_read_directory(port, 1);
    if (value == 0xFFFFFFFF || value == 2)
    {
        step = title_menu.panel_step;
        title_menu.panel_step = (uint16)(step + 2);
        title_renderer_set_group_value(1, value == 2 ? 37 : 35);
        title_renderer_set_visible(0x20056);
        title_menu_wait_step();
        title_renderer_set_group_value(4, 21);
    }
    if (value != 1)
        return 1;
    step = title_menu.panel_step + 1;
    title_menu.panel_step = (uint16)(step);
    return step;
}

uint32 title_save_read_accounts_tick(void)
{
    uint32 port, value, step;

    FUNCTION_MARKER(0x800D0BFC, "ACE3.BIN");
    port = title_menu.card_port;
    value = title_card_read_accounts(port, 1);
    if (value == 0xFFFFFFFF || value == 2)
    {
        step = title_menu.panel_step;
        title_menu.panel_step = (uint16)(step + 1);
        title_renderer_set_group_value(1, value == 2 ? 37 : 35);
        if (value == 2)
            title_renderer_reset_view(6);
        title_renderer_set_visible(0x20056);
        title_menu_wait_step();
        title_renderer_set_group_value(4, 21);
    }
    if (value == 1)
    {
        title_menu.panel = (uint16)(16);
        title_menu.panel_step = (uint16)(0);
    }
    return 16;
}

uint32 title_save_read_notice_tick(void)
{
    const char *name;
    uint32 port, value, buttons;

    FUNCTION_MARKER(0x800D0CEC, "ACE3.BIN");
    port = title_menu.card_port;
    value = title_card_port_status(port);
    if (value == 2 || value == 6)
    {
        title_menu.panel = (uint16)(9);
        title_menu.panel_step = (uint16)(0);
        return 9;
    }
    buttons = game_input.pressed;
    if (buttons & 0x10)
    {
        title_menu.panel = (uint16)(19);
        title_menu.panel_step = (uint16)(0);
        title_account_reset_campaign();
        game_runtime.account.route_mode = (uint16)(2);
        value = title_account_id_seed();
        game_runtime.progress.account_id = (uint32)(value);
        name = title_menu_account_name(0);
        game_account_set_name(name);
        return sound_play(89);
    }
    if ((buttons & 0x20) == 0)
        return 0;
    title_menu.panel = (uint16)(7);
    title_menu.panel_step = (uint16)(0);
    return sound_play(59);
}

uint32 title_save_select_account_begin(void)
{
    uint32 step;

    FUNCTION_MARKER(0x800D0DC8, "ACE3.BIN");
    step = title_menu.panel_step;
    title_menu.phase_ticks = 0;
    title_menu.panel_step = (uint16)(step + 1);
    title_renderer_reset_view(20);
    title_renderer_set_visible(0x100056);
    title_renderer_set_group_value(1, 36);
    title_renderer_set_group_value(4, 22);
    title_renderer_set_name_hint_mode(1);
    title_menu.record_banner = (uint8)(0);
    title_renderer_set_hint(64, 50, 1, 0);
    return title_menu_wait_step();
}

uint32 title_save_select_account_tick(void)
{
    uint32 port, value, directions, selection, buttons;

    FUNCTION_MARKER(0x800D0E54, "ACE3.BIN");
    port = title_menu.card_port;
    value = title_card_port_status(port);
    if (value == 2 || value == 6)
    {
        title_menu.panel = (uint16)(9);
        title_menu.panel_step = (uint16)(0);
        return title_renderer_set_name_hint_mode(0);
    }
    directions = game_input.repeat;
    if (directions & 2)
    {
        selection = title_menu.account_selection;
        title_menu.account_selection = (uint8)(selection < 2 ? selection + 1 : 0);
        sound_play(61);
    }
    directions = game_input.repeat;
    if (directions & 1)
    {
        selection = title_menu.account_selection;
        title_menu.account_selection = (uint8)(selection != 0 ? selection - 1 : 2);
        sound_play(61);
    }
    selection = title_menu.account_selection;
    title_renderer_set_group_value(6, selection != 0);
    selection = title_menu.account_selection;
    title_renderer_set_group_value(7, selection != 1);
    selection = title_menu.account_selection;
    title_renderer_set_group_value(8, selection != 2);
    buttons = game_input.pressed;
    if (buttons & 0x40)
    {
        selection = title_menu.account_selection;
        value = title_card_account_present(selection);
        if (value != 0)
            title_menu.panel = (uint16)(17);
        else
        {
            title_menu.panel = (uint16)(18);
            title_menu.record_banner = (uint8)(1);
        }
        title_menu.panel_step = (uint16)(0);
        return sound_play(89);
    }
    if ((buttons & 0x20) == 0)
        return 0;
    title_menu.panel = (uint16)(11);
    title_menu.panel_step = (uint16)(0);
    title_renderer_set_name_hint_mode(0);
    return sound_play(59);
}

uint32 title_save_overwrite_begin(void)
{
    uint32 step, selection;

    FUNCTION_MARKER(0x800D1080, "ACE3.BIN");
    step = title_menu.panel_step;
    title_menu.panel_step = (uint16)(step + 1);
    title_renderer_reset_view(21);
    title_renderer_set_visible(0x200056);
    title_renderer_set_group_value(1, 38);
    title_renderer_set_group_value(4, 24);
    selection = title_menu.account_selection;
    title_renderer_set_animation(selection + 6, 1, 0, 10);
    title_renderer_set_name_hint_mode(2);
    return title_menu_wait_step();
}

uint32 title_save_overwrite_tick(void)
{
    uint32 port, value, selection, buttons;

    FUNCTION_MARKER(0x800D1108, "ACE3.BIN");
    port = title_menu.card_port;
    value = title_card_port_status(port);
    if (value == 2 || value == 6)
    {
        title_menu.panel = (uint16)(9);
        title_menu.panel_step = (uint16)(0);
        return title_renderer_set_name_hint_mode(0);
    }
    selection = title_menu.account_selection;
    title_renderer_set_group_value(6, selection != 0);
    selection = title_menu.account_selection;
    title_renderer_set_group_value(7, selection != 1);
    selection = title_menu.account_selection;
    title_renderer_set_group_value(8, selection != 2);
    buttons = game_input.pressed;
    if (buttons & 0x40)
    {
        selection = title_menu.account_selection;
        title_menu.panel = (uint16)(18);
        title_menu.panel_step = (uint16)(0);
        title_renderer_set_animation(selection + 6, 0, 0, 0);
        sound_play(89);
    }
    else if (buttons & 0x20)
    {
        selection = title_menu.account_selection;
        title_menu.panel = (uint16)(16);
        title_menu.panel_step = (uint16)(0);
        title_renderer_set_animation(selection + 6, 0, 0, 0);
        sound_play(59);
    }
    value = title_menu.phase_ticks + 1;
    title_menu.phase_ticks = value;
    return value;
}

void title_save_write_begin(void)
{
    const char *name;
    uint32 step, value;

    FUNCTION_MARKER(0x800D12A4, "ACE3.BIN");
    step = title_menu.panel_step;
    title_menu.panel_step = (uint16)(step + 1);
    title_renderer_set_group_value(1, 39);
    title_renderer_set_group_value(4, 24);
    value = title_account_id_seed();
    game_runtime.progress.account_id = (uint32)(value);
    name = title_menu_account_name(0);
    {
        game_account_set_name(name);
        return;
    }
}

uint32 title_save_write_directory_tick(void)
{
    const char *name;
    uint32 port, selection, value, step;

    FUNCTION_MARKER(0x800D1300, "ACE3.BIN");
    port = title_menu.card_port;
    selection = title_menu.account_selection;
    value = title_card_write_directory(port, (sint32)selection, 1);
    if (value == 0xFFFFFFFF)
    {
        step = title_menu.panel_step;
        title_menu.panel_step = (uint16)(step + 2);
        title_renderer_set_group_value(1, 40);
        title_renderer_set_visible(0x20016);
        title_menu_wait_step();
        title_renderer_set_name_hint_mode(0);
    }
    if (value != 1)
        return 1;
    name = title_menu_account_name(0);
    game_account_set_name(name);
    title_card_encode_current_account();
    step = title_menu.panel_step + 1;
    title_menu.panel_step = (uint16)(step);
    return step;
}

uint32 title_save_write_account_tick(void)
{
    uint32 port, selection, value, step;

    FUNCTION_MARKER(0x800D13B8, "ACE3.BIN");
    port = title_menu.card_port;
    selection = title_menu.account_selection;
    value = title_card_write_account(port, selection, 6, 1);
    if (value == 0xFFFFFFFF)
    {
        step = title_menu.panel_step;
        title_menu.panel_step = (uint16)(step + 1);
        title_renderer_set_group_value(1, 40);
        title_renderer_set_visible(0x20056);
        title_menu_wait_step();
        title_renderer_set_name_hint_mode(0);
    }
    if (value != 1)
        return 1;
    step = title_menu.panel_step;
    title_menu.card_result_ticks = (uint32)(120);
    title_menu.panel_step = (uint16)(step + 2);
    title_account_reset_campaign();
    game_runtime.account.route_mode = (uint16)(2);
    return title_menu_wait_step();
}

void title_save_write_notice_tick(void)
{
    uint32 port, value, buttons;

    FUNCTION_MARKER(0x800D1488, "ACE3.BIN");
    port = title_menu.card_port;
    value = title_card_port_status(port);
    if (value == 2 || value == 6)
    {
        title_menu.panel = (uint16)(9);
        title_menu.panel_step = (uint16)(0);
        return;
    }
    title_renderer_set_group_value(1, 40);
    buttons = game_input.pressed;
    if (buttons & 0x10)
    {
        title_menu.panel = (uint16)(19);
        title_menu.panel_step = (uint16)(0);
        title_account_reset_campaign();
        game_runtime.account.route_mode = (uint16)(2);
        return;
    }
    if (buttons & 0x20)
    {
        title_menu.panel = (uint16)(11);
        title_menu.panel_step = (uint16)(0);
    }
    return;
}

uint32 title_save_success_tick(void)
{
    uint32 timer, buttons;

    FUNCTION_MARKER(0x800D1554, "ACE3.BIN");
    title_renderer_set_group_value(1, 42);
    timer = title_menu.card_result_ticks;
    if ((sint32)timer < 0)
    {
        title_menu.panel = (uint16)(19);
        title_menu.panel_step = (uint16)(0);
        return 19;
    }
    buttons = game_input.pressed;
    if (buttons & 0x30)
    {
        title_menu.panel = (uint16)(19);
        title_menu.panel_step = (uint16)(0);
        return 19;
    }
    --timer;
    title_menu.card_result_ticks = (uint32)(timer);
    return timer;
}

uint32 title_save_disc_recovery_begin(void)
{
    uint32 step;

    FUNCTION_MARKER(0x800D15C4, "ACE3.BIN");
    step = title_menu.panel_step;
    title_menu.recovery_notice_offset = (uint32)(0);
    title_menu.panel_step = (uint16)(step + 1);
    game_runtime.selection.mission = (uint16)(1);
    return title_renderer_set_hint(111, 150, 0, 1);
}

uint32 title_save_check_disc_tick(void)
{
    uint32 status, value, step;

    FUNCTION_MARKER(0x800D1618, "ACE3.BIN");
    status = cd_resource_state();
    if (status == 0)
        title_renderer_set_group_value(1, 73);
    if (status == 3 || status == 1)
    {
        title_menu.screen = (uint16)(6);
        title_menu.screen_step = (uint16)(0);
        return 6;
    }
    if (status == 4 || status == 0xFFFFFFFF || status == 2)
    {
        value = title_menu.recovery_notice_offset;
        title_renderer_set_group_value(1, value + 70);
        resource_begin_recovery();
        step = title_menu.panel_step;
        title_menu.panel_step = (uint16)(step + 1);
        return title_menu_wait_step();
    }
    return 2;
}

uint32 title_save_recovery_wait_tick(void)
{
    uint32 status, step;

    FUNCTION_MARKER(0x800D16D4, "ACE3.BIN");
    title_renderer_set_visible(0x400036);
    title_renderer_set_group_value(4, 4);
    title_renderer_set_name_hint_mode(0);
    status = resource_get_recovery_state();
    if (status != 3)
        return 0x800E0000;
    step = title_menu.panel_step + 1;
    title_menu.panel_step = (uint16)(step);
    return step;
}

uint32 title_save_recovery_finish_tick(void)
{
    uint32 status, step;

    FUNCTION_MARKER(0x800D1738, "ACE3.BIN");
    status = resource_get_recovery_state();
    if (status != 1)
        return status;
    title_renderer_set_group_value(1, 73);
    step = title_menu.panel_step;
    title_menu.panel_step = (uint16)(step - 2);
    title_menu_wait_step();
    title_menu.recovery_notice_offset = (uint32)(31);
    return 31;
}

const char *title_records_difficulty_label(uint32 selection)
{
    const uint8 *buffer;
    uint32 index;
    FUNCTION_MARKER(0x800D5A70, "ACE3.BIN");
    buffer = title_mission_record(selection & 255);
    index = buffer[0x112];
    if (index >= sizeof(title_difficulty_labels) / sizeof(*title_difficulty_labels))
        abort();
    return title_difficulty_labels[index];
}

const char *title_records_mission_label(uint32 selection)
{
    const uint8 *buffer;
    uint32 index;
    FUNCTION_MARKER(0x800D5BF4, "ACE3.BIN");
    buffer = title_mission_record(selection & 255);
    index = buffer[0x113];
    if (index >= sizeof(title_mission_labels) / sizeof(*title_mission_labels))
        abort();
    return title_mission_labels[index];
}

const char *title_records_unit_label(uint32 selection)
{
    const uint8 *buffer;
    uint32 index;
    FUNCTION_MARKER(0x800D5C2C, "ACE3.BIN");
    buffer = title_mission_record(selection & 255);
    index = buffer[0x113];
    if (index >= sizeof(title_mission_units))
        abort();
    index = title_mission_units[index];
    if (index >= sizeof(title_unit_labels) / sizeof(*title_unit_labels))
        abort();
    return title_unit_labels[index];
}

const char *title_records_control_label(uint32 slot)
{
    uint8 *buffer;
    uint32 index;
    FUNCTION_MARKER(0x800D5CC0, "ACE3.BIN");
    buffer = title_account_record(slot);
    index = buffer[0x464];
    if (index >= 2)
        abort();
    return title_difficulty_labels[index];
}

uint32 mission_menu_brightness_frame(void)
{
    FUNCTION_MARKER(0x800DB3F8, "ACE3.BIN");
    return menu_brightness_frame();
}

void mission_menu_screen_frame(void)
{
    FUNCTION_MARKER(0x800DB714, "ACE3.BIN");
    {
        mission_menu_screen_tick();
        return;
    }
}

void mission_menu_screen_tick(void)
{
    static const uint8 bits[] = {8, 4, 2, 1};
    static const uint8 slots[] = {7, 9, 8, 6};
    uint32 value, index, moved = 0, changed, result;
    sint32 position;
    FUNCTION_MARKER(0x800DB71C, "ACE3.BIN");
    for (index = 0; index < 4; ++index)
    {
        if (index)
        {
        }
        changed = 0;
        if (game_input.repeat & bits[index])
        {
            position = (sint32)game_runtime.display.screen[index >> 1];
            if ((index == 0 && position < 12) || (index == 1 && position >= -11) || (index == 2 && position < 8) || (index == 3 && position > 0))
            {
                value = (uint32)position + (index & 1 ? 0xFFFFFFFFu : 1u);
                game_runtime.display.screen[index >> 1] = (uint32)(value);
                changed = 1;
            }
        }
        ui_widget_set(slots[index], changed);
        if (changed)
        {
            if (!moved)
                sound_play(46);
            moved = 1;
        }
    }
    if (game_gpu_get_ode())
    {
        value = game_runtime.display.flags;
        game_runtime.display.flags = (uint32)(value | 2);
    }
    if ((game_input.pressed & 16) && !(game_input.held & 0x2F))
    {
        ui_animation_set(1, 1, 0, 30);
        menu_state.header.phase = (uint8)(1);
        ui_group_set_mask(0, 3);
        ui_group_set_mask(2, 4096);
        value = game_runtime.display.flags;
        game_runtime.display.flags = (uint32)(value | 2);
        {
            sound_play(60);
            return;
        }
    }
    if ((game_input.pressed & 32) && !(game_input.held & 0x5F))
    {
        value = (sint16)menu_state.saved_screen_x;
        game_runtime.display.screen[0] = (uint32)(value);
        value = (sint16)menu_state.saved_screen_y;
        game_runtime.display.screen[1] = (uint32)(value);
        ui_animation_set(1, 1, 0, 30);
        menu_state.header.phase = (uint8)(1);
        ui_group_set_mask(0, 3);
        ui_group_set_mask(2, 4096);
        value = game_runtime.display.flags;
        game_runtime.display.flags = (uint32)(value | 2);
        sound_play(59);
        game_input.pressed = (uint32)(0);
        return;
    }
    if (game_input.kind & 16)
    {
        if ((sint16)pad_state[0].axis[3] >= 129)
        {
            game_runtime.display.screen[0] = (uint32)(0xFFFFFFFE);
            game_runtime.display.screen[1] = (uint32)(0);
        }
        result = game_input.kind & 16;
        if (result)
            return;
    }
    result = pad_state[0].pressed & 0x1000;
    if (result)
    {
        game_runtime.display.screen[0] = (uint32)(0xFFFFFFFE);
        game_runtime.display.screen[1] = (uint32)(0);
        {
            sound_play(48);
            return;
        }
    }
    return;
}

uint32 mission_menu_records_begin(void)
{
    const uint8 *asset_data;
    uint32 asset_bytes;
    MISSION_DETAIL_LIST *detail = &mission_detail_list;
    uint32 resource, count, value, index, selected;
    uint32 sections[2];

    FUNCTION_MARKER(0x800DC144, "ACE3.BIN");
    resource = game_runtime.resource_slot.encoded_base;
    asset_data = game_runtime.resource_slot.data;
    asset_bytes = game_runtime.resource_slot.read_bytes;
    mission_flow.assets.arena_begin = (uint32)(resource);
    mission_image_sections(resource, game_runtime.resource_slot.data, game_runtime.resource_slot.read_bytes, sections, 2);
    scene_texture_load_group(sections[1], (RESOURCE_ASSET_SOURCE){asset_data, asset_bytes, resource});
    scene_texture_load_group(mission_flow.assets.shared_menu_textures, mission_flow.assets.frontend_source);
    arena_set_cursor(sections[1]);
    mission_flow.assets.detail_image_table = mission_image_directory_bind(sections[0], sections[1], resource, asset_data, asset_bytes);
    arena_alloc(64);
    detail->mission_count = (uint8)(0);
    for (index = 0; index < 59; ++index)
    {
        if (game_runtime.progress.mission_rating[4][index])
        {
            count = detail->mission_count;
            detail->mission[count] = (uint8)(index);
            count = detail->mission_count;
            detail->mission_count = (uint8)(count + 1);
        }
    }
    count = detail->mission_count;
    value = (uint8)game_runtime.selection.mission;
    if (value >= GAME_MISSION_COUNT)
        abort();
    detail->mission[count] = (uint8)(value);
    count = detail->mission_count;
    detail->mission_count = (uint8)(count + 1);
    detail->aircraft_count = (uint8)(0);
    for (index = 0; index < 64; ++index)
    {
        if (game_runtime.progress.aircraft_unlocked[index] == 1)
        {
            count = detail->aircraft_count;
            detail->aircraft_count = (uint8)(count + 1);
        }
    }
    detail->weapon_count = (uint8)(0);
    for (index = 0; index < 16; ++index)
    {
        if (game_runtime.progress.weapon_unlocked[index] == 1)
        {
            count = detail->weapon_count;
            detail->weapon_count = (uint8)(count + 1);
        }
    }
    count = detail->mission_count;
    detail->selected = (uint8)(count - 1);
    selected = detail->selected;
    value = mission_flow_resolve_faction(detail->mission[selected]);
    scene_texture_load_image_at(&mission_flow.assets.detail_image_table, value, 960, 256, 768, 1);
    value = menu_state.header.phase;
    menu_state.header.phase = (uint8)(value + 1);
    ui_item_reset_motion(2, 17);
    ui_item_reset_motion(2, 18);
    ui_group_set_mask(2, 393216);
    selected = detail->selected;
    selected = detail->mission[selected];
    value = game_runtime.progress.mission_rating[4][selected];
    *ui_widget_value(50) = (uint8)(value);
    value = (uint8)game_runtime.selection.difficulty;
    *ui_widget_value(51) = (uint8)(value);
    value = game_runtime.progress.record_slot;
    *ui_widget_value(53) = (uint8)(value);
    return value;
}

uint32 mission_menu_records_tick(void)
{
    MISSION_DETAIL_LIST *detail = &mission_detail_list;
    uint32 selected, faction, mode, value, count, flags, changed = 0;
    uint32 arguments[3];
    sint32 frames = 0;
    FUNCTION_MARKER(0x800DC43C, "ACE3.BIN");
    selected = detail->selected;
    value = detail->mission[selected];
    faction = mission_flow_resolve_faction(value);
    text_select_font(4);
    text_set_blend(2);
    text_set_color(128, 128, 128);
    text_set_pos(29, 65);
    text_print((const uint8 *)game_runtime.account.name, NULL, 0);
    text_set_blend(0xFFFFFFFFu);
    text_select_font(8);
    selected = detail->selected;
    mode = game_runtime.selection.difficulty;
    selected = detail->mission[selected];
    if (mode == 0)
        frames = game_runtime.progress.mission_time[0][selected];
    else if (mode == 1)
        frames = game_runtime.progress.mission_time[1][selected];
    else if (mode == 2)
        frames = game_runtime.progress.mission_time[2][selected];
    // Initialize the original undefined value for an invalid mode
    if (frames < 0)
        frames = 0;
    if (frames > 107999999)
        frames = 107999999;
    text_set_pos(234, 152);
    arguments[0] = frames / 1800;
    arguments[1] = frames / 30 - 60 * (frames / 1800);
    arguments[2] = 100 * (frames % 30) / 30;
    text_print_digits((const uint8 *)"%2d:%2d;%2d", (const uint8 *)arguments, 12);
    text_select_font(3);
    text_set_blend(2);
    text_set_pos(180, 110);
    arguments[0] = game_runtime.progress.total_count;
    text_print((const uint8 *)"%d\n", (const uint8 *)arguments, 4);
    text_set_pos(180, 128);
    arguments[0] = game_runtime.progress.total_losses;
    text_print((const uint8 *)"%d\n", (const uint8 *)arguments, 4);
    text_set_pos(180, 147);
    arguments[0] = detail->aircraft_count;
    arguments[1] = 20;
    text_print((const uint8 *)"%d/%d\n", (const uint8 *)arguments, 8);
    text_set_pos(180, 167);
    arguments[0] = detail->weapon_count;
    arguments[1] = 13;
    text_print((const uint8 *)"%d/%d\n", (const uint8 *)arguments, 8);
    text_set_pos(145, 215);
    selected = detail->selected;
    selected = detail->mission[selected];
    text_print((const uint8 *)detail_mission_labels[selected], NULL, 0);
    text_set_pos(94, 253);
    text_print((const uint8 *)detail_faction_labels[faction], NULL, 0);
    text_set_blend(1);
    text_set_pos(253, 251);
    selected = detail->selected;
    selected = detail->mission[selected];
    frames = game_runtime.progress.mission_time[4][selected];
    arguments[0] = frames / 1800;
    arguments[1] = frames / 30 - 60 * (frames / 1800);
    arguments[2] = 100 * (frames % 30) / 30;
    text_print_digits((const uint8 *)"%2d:%2d.%2d\n", (const uint8 *)arguments, 12);
    text_set_pos(175, 282);
    selected = detail->selected;
    selected = detail->mission[selected];
    arguments[0] = game_runtime.progress.mission_count[selected];
    text_print((const uint8 *)"%d\n", (const uint8 *)arguments, 4);
    text_set_pos(175, 303);
    selected = detail->selected;
    selected = detail->mission[selected];
    arguments[0] = game_runtime.progress.mission_losses[selected];
    text_print((const uint8 *)"%d\n", (const uint8 *)arguments, 4);
    text_select_font(8);
    text_set_blend(2);
    text_set_pos(145, 359);
    frames = (sint32)game_runtime.progress.total_time;
    if (frames < 0)
        frames = 0;
    if (frames > 107999999)
        frames = 107999999;
    arguments[0] = frames / 108000;
    arguments[1] = frames / 1800 - 60 * (frames / 108000);
    arguments[2] = frames / 30 - 60 * (frames / 1800);
    text_print_digits((const uint8 *)"%3d:%2d:%2d", (const uint8 *)arguments, 12);
    selected = detail->selected;
    selected = detail->mission[selected];
    value = game_runtime.progress.mission_rating[4][selected];
    *ui_widget_value(50) = (uint8)(value);
    value = (uint8)game_runtime.selection.difficulty;
    *ui_widget_value(51) = (uint8)(value);
    value = game_runtime.progress.record_slot;
    *ui_widget_value(53) = (uint8)(value);
    count = detail->mission_count;
    selected = detail->selected;
    value = 2 * ((sint32)selected < (sint32)count - 1);
    if (selected)
        value |= 1;
    ui_widget_set(49, value);
    *ui_widget_value(52) = (uint8)(0);
    flags = game_input.repeat;
    if (flags & 8)
    {
        count = detail->mission_count;
        selected = detail->selected;
        if ((sint32)selected < (sint32)count - 1)
        {
            detail->selected = (uint8)(selected + 1);
            sound_play(61);
            changed = 1;
        }
    }
    else if (flags & 4)
    {
        selected = detail->selected;
        if (selected)
        {
            detail->selected = (uint8)(selected - 1);
            sound_play(61);
            changed = 2;
        }
    }
    else if ((game_input.pressed & 32) && !(game_input.held & 0x5F))
    {
        mission_menu_start_item_motion(0, 1);
        menu_state.header.kind = (uint8)(0);
        menu_state.header.phase = (uint8)(0);
        arena_set_cursor(mission_flow.assets.arena_begin);
        sound_play(59);
        game_input.pressed = (uint32)(0);
    }
    if (changed)
    {
        *ui_widget_value(52) = (uint8)(changed);
        selected = detail->selected;
        value = mission_flow_resolve_faction(detail->mission[selected]);
        scene_texture_load_image_at(&mission_flow.assets.detail_image_table, value, 960, 256, 768, 1);
    }
    value = game_runtime.progress.record_slot;
    *ui_widget_value(53) = (uint8)(value);
    return value;
}

void mission_menu_account_frame(void)
{
    FUNCTION_MARKER(0x800DCB5C, "ACE3.BIN");
    {
        mission_menu_account_begin();
        return;
    }
}

void mission_menu_account_begin(void)
{
    uint32 value;
    FUNCTION_MARKER(0x800DCB64, "ACE3.BIN");
    value = menu_state.header.phase;
    menu_state.header.phase = (uint8)(value + 1);
    ui_item_at(0, 15)->phase = (uint16)(1);
    mission_flow.mode = 6;
    mission_flow.mode_phase = 0;
    value = game_runtime.progress.flow_flags;
    mission_flow.account_mode = (uint32)(value & 16 ? 3 : 1);
    {
        ui_group_set_mask(0, 32768);
        return;
    }
}

uint32 mission_menu_account_tick(void)
{
    uint32 value;
    FUNCTION_MARKER(0x800DCBE0, "ACE3.BIN");
    mission_dispatch_mode(6, (uint32)(sint16)mission_flow.mode_phase);
    if ((sint16)mission_flow.mode == 7)
    {
        ui_group_set_mask(1, 0);
        mission_flow.mode = 0;
        value = menu_state.header.phase;
        menu_state.header.phase = (uint8)(value + 1);
        ui_animation_set(1, 1, 0, 30);
        return mission_effect_close_trails();
    }
    return 7;
}

void mission_menu_account_fade_tick(void)
{
    static const uint8 factors[] = {143, 135, 83};
    uint32 progress, result, product, index, flags;
    sint32 high, quotient;
    FUNCTION_MARKER(0x800DCC80, "ACE3.BIN");
    progress = ui_animation_ticks(1);
    result = ui_animation_phase(1);
    if (result == 2)
    {
        mission_menu_start_item_motion(0, 1);
        ui_item_reverse_motion(0, 15);
        mission_effect_open_ring();
        result = menu_state.header.phase;
        menu_state.header.phase = (uint8)(result + 1);
        {
            ui_animation_set(1, 1, 0, 25);
            return;
        }
    }
    for (index = 0; index < 3; ++index)
    {
        product = progress * factors[index];
        high = (((sint64)(sint32)product * (sint32)0x8D3DCB09u) >> 32);
        quotient = ((sint32)((uint32)high + product) >> 4) - ((sint32)product >> 31);
        game_runtime.display.background[index] = (uint8)((uint32)quotient);
    }
    flags = game_runtime.display.flags;
    result = flags | 8;
    game_runtime.display.flags = (uint32)(result);
    return;
}

void mission_menu_account_finish_tick(void)
{
    uint32 result;
    FUNCTION_MARKER(0x800DCDC4, "ACE3.BIN");
    result = ui_animation_phase(1);
    if (result == 2)
    {
        menu_state.header.kind = (uint8)(0);
        menu_state.header.phase = (uint8)(0);
    }
    return;
}

typedef char menu_fill_packet_layout_check[sizeof(TILE) == 16 && offsetof(TILE, rgbc) == 4 && offsetof(TILE, p) == 8 && offsetof(TILE, w) == 12 && offsetof(TILE, h) == 14 ? 1 : -1];

uint32 mission_menu_movie_begin(void)
{
    const uint8 *asset_data;
    uint32 asset_bytes;
    uint32 resource, value;
    uint32 sections[3];

    FUNCTION_MARKER(0x800DDFA4, "ACE3.BIN");
    ui_item_reset_motion(2, 22);
    ui_group_set_mask(2, 0x400000);
    if (game_runtime.resource_pending == 1)
        return 1;
    resource = game_runtime.resource_slot.encoded_base;
    asset_data = game_runtime.resource_slot.data;
    asset_bytes = game_runtime.resource_slot.read_bytes;
    mission_flow.assets.arena_begin = (uint32)(resource);
    mission_image_sections(resource, game_runtime.resource_slot.data, game_runtime.resource_slot.read_bytes, sections, 3);
    scene_texture_load_group(sections[2], (RESOURCE_ASSET_SOURCE){asset_data, asset_bytes, resource});
    arena_set_cursor(sections[2]);
    mission_flow.assets.row_image_table = mission_image_directory_bind(sections[0], sections[1], resource, asset_data, asset_bytes);
    mission_flow.assets.detail_image_table = mission_image_directory_bind(sections[1], sections[2], resource, asset_data, asset_bytes);
    value = menu_state.header.phase + 1;
    menu_state.header.phase = (uint8)(value);
    return value;
}

void mission_menu_movie_frame(void)
{
    FUNCTION_MARKER(0x800DE078, "ACE3.BIN");
    {
        mission_menu_movie_tick();
        return;
    }
}

void mission_menu_movie_tick(void)
{
    FUNCTION_MARKER(0x800DE080, "ACE3.BIN");
    if ((game_input.pressed & 16) && !(game_input.held & 0x2F))
    {
        mission_flow.playback_requested = (uint32)(1);
        game_runtime.progress.movie_branch = (uint8)(1);
        game_runtime.playback.movie_idx = 2;
    }
    if ((game_input.pressed & 32) && !(game_input.held & 0x5F))
    {
        mission_menu_start_item_motion(0, 1);
        menu_state.header.kind = (uint8)(0);
        menu_state.header.phase = (uint8)(0);
        mission_flow.mode = 0;
        mission_flow.mode_phase = 0;
        game_runtime.progress.movie_branch = (uint8)(0);
        arena_set_cursor(mission_flow.assets.arena_begin);
        game_input.pressed = (uint32)(0);
    }
    game_runtime.mission_control.mode = (uint32)(0);
    return;
}

uint32 mission_menu_exit_begin(void)
{
    uint32 value;
    FUNCTION_MARKER(0x801071B8, "ACE3.BIN");
    value = menu_state.header.phase;
    menu_state.header.phase = (uint8)(value + 1);
    ui_animation_set(15, 1, 0, 20);
    return mission_effect_set_ring(3);
}

uint32 mission_menu_exit_wait_ring(void)
{
    uint32 value, result;
    FUNCTION_MARKER(0x80107200, "ACE3.BIN");
    if (mission_effect.layers[MISSION_EFFECT_RING].mode)
        return 0x80110000;
    value = menu_state.header.phase;
    menu_state.header.phase = (uint8)(value + 1);
    value = game_runtime.display.background[0];
    result = value < 143 ? 1 : 32;
    mission_menu_exit_fade_ticks = result;
    return result;
}

uint32 mission_menu_exit_fade_tick(void)
{
    uint32 counter, value, product;
    FUNCTION_MARKER(0x80107270, "ACE3.BIN");
    counter = mission_menu_exit_fade_ticks - 1;
    mission_menu_exit_fade_ticks = counter;
    if (counter == 0)
    {
        value = menu_state.header.phase;
        menu_state.header.phase = (uint8)(value + 1);
    }
    counter = mission_menu_exit_fade_ticks;
    product = counter * 143;
    game_runtime.display.background[0] = (uint8)((sint32)product / 32);
    counter = mission_menu_exit_fade_ticks;
    product = counter * 135;
    game_runtime.display.background[1] = (uint8)((sint32)product / 32);
    counter = mission_menu_exit_fade_ticks;
    product = counter * 83;
    game_runtime.display.background[2] = (uint8)((sint32)product / 32);
    value = game_runtime.display.flags | 8;
    game_runtime.display.flags = (uint32)(value);
    return value;
}

uint32 mission_menu_exit_finish(void)
{
    uint32 value;
    FUNCTION_MARKER(0x80107358, "ACE3.BIN");
    mission_flow.exit_requested = 1;
    ui_animation_set(2, 0, 0, 0);
    value = game_runtime.progress.flow_flags | 2;
    game_runtime.progress.flow_flags = (uint32)(value);
    return value;
}

void menu_calibration_update(void)
{
    uint32 value, result;
    FUNCTION_MARKER(0x800DA444, "ACE3.BIN");
    if ((sint16)menu_state.profile_kind == 1)
        menu_calibration_negcon_frame();
    if ((sint16)menu_state.profile_kind == 3)
        menu_calibration_joystick_frame();
    if ((sint16)game_input.status != 2)
    {
        value = game_input.kind;
        result = menu_state.previous_pad_kind;
        if (value == result)
            return;
    }
    menu_state.header.kind = (uint8)(1);
    menu_state.header.phase = (uint8)(0);
    return;
}

void mission_menu_options_wait_frame(void)
{
    FUNCTION_MARKER(0x800DA5EC, "ACE3.BIN");
    mission_menu_options_wait_tick();
    return;
}

void mission_menu_options_wait_tick(void)
{
    uint32 phase, result = 2;
    FUNCTION_MARKER(0x800DA5F4, "ACE3.BIN");
    if ((game_input.pressed & 16) && !(game_input.held & 0x2F))
        ui_animation_set(1, 2, 30, 30);
    phase = ui_animation_phase(1);
    if (phase == 2)
    {
        menu_state.header.phase = (uint8)(phase);
    }
    return;
}

void mission_menu_options_select_frame(void)
{
    FUNCTION_MARKER(0x800DA66C, "ACE3.BIN");
    {
        mission_menu_options_select_tick();
        return;
    }
}

void mission_menu_options_select_tick(void)
{
    uint32 value, result;
    FUNCTION_MARKER(0x800DA674, "ACE3.BIN");
    if (game_input.repeat & 2)
    {
        value = menu_state.options_choice;
#if 0
        menu_state.options_choice = (uint8)(value < 3 ? value + 1 : 0);
#else
        menu_state.options_choice = (uint8)(value == 0 ? 3 : 0);
#endif
        sound_play(61);
    }
    if (game_input.repeat & 1)
    {
        value = menu_state.options_choice;
#if 0
        menu_state.options_choice = (uint8)(value ? value - 1 : 3);
#else
        menu_state.options_choice = (uint8)(value == 0 ? 3 : 0);
#endif
        sound_play(61);
    }
    if ((game_input.pressed & 16) && !(game_input.held & 0x2F))
    {
        ui_animation_set(1, 3, -1, -1);
        menu_state.header.phase = (uint8)(3);
        menu_state.header.action = (uint8)(1);
        sound_play(60);
        sound_play(91);
    }
    value = menu_state.options_choice;
    ui_widget_set(5, value);
    ui_widget_set(6, 0);
    if (game_input.held & 1)
        ui_widget_set(6, 1);
    if (game_input.held & 2)
        ui_widget_set(6, 2);
    result = game_input.pressed & 32;
    if (result)
    {
        result = game_input.held & 0x5F;
        if (!result)
        {
            ui_animation_set(1, 3, -1, -1);
            menu_state.header.phase = (uint8)(3);
            menu_state.header.action = (uint8)(2);
            sound_play(59);
            game_input.pressed = (uint32)(0);
        }
    }
    return;
}

void mission_menu_options_leave_frame(void)
{
    FUNCTION_MARKER(0x800DA870, "ACE3.BIN");
    mission_menu_options_leave_tick();
    return;
}

void mission_menu_options_leave_tick(void)
{
#if 0
    uint32 phase, context, action, choice, result, state, value, index;
#else
    uint32 phase, action, choice, result, index;
#endif
    FUNCTION_MARKER(0x800DA878, "ACE3.BIN");
    if ((game_input.pressed & 16) && !(game_input.held & 0x2F))
        ui_animation_set(1, 0, 0, 0);
    phase = ui_animation_phase(1);
    if (phase)
        return;
    action = menu_state.header.action;
    if (action == 1)
    {
        choice = menu_state.options_choice;
        switch (choice)
        {
            case 0:
                menu_state.header.phase = (uint8)(4);
                ui_item_reset_motion(2, 13);
                ui_group_set_mask(2, 8192);
                result = 0;
                for (index = 304; index <= 320; index += 4)
                    result = sound_stop_voice(*menu_voice_slot((index - 304) / 4));
                return;
#if 0
            case 1:
                menu_state.header.phase = (uint8)(5);
                ui_item_reset_motion(2, 14);
                ui_group_set_mask(2, 16384);
                return;
            case 2:
                menu_state.header.phase = (uint8)(6);
                ui_item_reset_motion(2, 15);
                ui_group_set_mask(0, 0);
                ui_group_set_mask(2, 32768);
                value = (uint16)game_runtime.display.screen[0];
                menu_state.saved_screen_x = (uint16)(value);
                result = (uint16)game_runtime.display.screen[1];
                menu_state.saved_screen_y = (uint16)(result);
                return;
#endif
            case 3:
                menu_state.header.phase = (uint8)(7);
                return;
            default:
                return;
        }
    }
    if (action == 2)
    {
        mission_menu_start_item_motion(0, 1);
        menu_state.header.kind = (uint8)(0);
        menu_state.header.phase = (uint8)(0);
        return;
    }
    return;
}

void mission_menu_main_open_tick(void)
{
    uint32 value;

    FUNCTION_MARKER(0x800D24E8, "ACE3.BIN");
    if (!ui_item_motion_phase(0, 1))
    {
        value = mission_flow.ui_timer + 1;
        mission_flow.ui_timer = (uint32)(value);
    }
    value = ui_animation_phase(1);
    if (value == 2)
        menu_state.header.phase = (uint8)(value);
    {
        ui_widget_set(5, menu_state.main_choice);
        return;
    }
}

static uint32 menu_confirmation_ready(void)
{
    return game_runtime.menu_confirmation_gate != 5;
}

void mission_menu_main_select_tick(void)
{
    uint32 restricted, changed, selection, result;

    FUNCTION_MARKER(0x800D2568, "ACE3.BIN");
    restricted = game_runtime.progress.flow_flags & 0x10;
    ui_widget_set(62, 1);
    selection = menu_state.main_choice;
    changed = 0;
    switch (selection)
    {
        case 0:
            if ((game_input.repeat & 3) && !restricted)
            {
                changed = 1;
                menu_state.main_choice = (uint8)(2);
            }
            if (game_input.repeat & 8)
            {
                changed = 1;
                menu_state.main_choice = (uint8)(1);
            }
            if (game_input.repeat & 4)
            {
                changed = 1;
                menu_state.main_choice = (uint8)(4);
            }
            break;
        case 1:
            if (game_input.repeat & 3)
            {
                changed = 1;
                menu_state.main_choice = (uint8)(3);
            }
            if (game_input.repeat & 8)
            {
                changed = 1;
                menu_state.main_choice = (uint8)(4);
            }
            if (game_input.repeat & 4)
            {
                changed = 1;
                menu_state.main_choice = (uint8)(0);
            }
            break;
        case 2:
            if (game_input.repeat & 3)
            {
                changed = 1;
                menu_state.main_choice = (uint8)(0);
            }
            if (game_input.repeat & 8)
            {
                changed = 1;
                menu_state.main_choice = (uint8)(3);
            }
            if (game_input.repeat & 4)
            {
                changed = 1;
                menu_state.main_choice = (uint8)(5);
            }
            break;
        case 3:
            if (game_input.repeat & 3)
            {
                changed = 1;
                menu_state.main_choice = (uint8)(1);
            }
            if (game_input.repeat & 8)
            {
                changed = 1;
                menu_state.main_choice = (uint8)(5);
            }
            if (game_input.repeat & 4)
            {
                changed = 1;
                menu_state.main_choice = (uint8)(restricted ? 5 : 2);
            }
            break;
        case 4:
            ui_widget_set(62, 0);
            if (game_input.repeat & 8)
            {
                changed = 1;
                menu_state.main_choice = (uint8)(0);
            }
            if (game_input.repeat & 4)
            {
                changed = 1;
                menu_state.main_choice = (uint8)(1);
            }
            break;
        case 5:
            ui_widget_set(62, 0);
            if (game_input.repeat & 8)
            {
                changed = 1;
                menu_state.main_choice = (uint8)(restricted ? 3 : 2);
            }
            if (game_input.repeat & 4)
            {
                changed = 1;
                menu_state.main_choice = (uint8)(3);
            }
            break;
    }
    if ((uint32)(menu_state.main_choice - 4) >= 2)
    {
        if ((game_input.repeat & 3) && changed)
            sound_play(61);
    }
    if ((game_input.repeat & 0xC) && changed)
        sound_play(61);
    ui_widget_set(5, menu_state.main_choice);
    if ((uint32)(menu_state.main_choice - 4) < 2)
    {
        if ((game_input.pressed & 0x40) && !(game_input.held & 0x2F) && menu_confirmation_ready())
        {
            menu_state.header.phase = (uint8)(3);
            menu_state.header.action = (uint8)(0);
            ui_animation_set(1, 3, -1, -1);
            ui_item_reverse_motion(0, 1);
            ui_animation_set(2, 3, -1, -1);
            ui_animation_set(15, 3, -1, -1);
            sound_play(60);
            sound_play(91);
        }
    }
    else
    {
        if ((game_input.pressed & 0x10) && !(game_input.held & 0x2F) && menu_confirmation_ready())
        {
            menu_state.header.phase = (uint8)(3);
            menu_state.header.action = (uint8)(0);
            ui_animation_set(1, 3, -1, -1);
            if (menu_state.main_choice == 2)
            {
            }
            if (menu_state.main_choice == 3)
                ui_animation_set(15, 3, -1, -1);
            ui_item_reverse_motion(0, 1);
            ui_animation_set(2, 3, -1, -1);
            sound_play(60);
            sound_play(91);
        }
    }
    result = game_input.pressed & 0x20;
    if (result)
    {
        result = game_input.held & 0x5F;
        if (!result)
        {
            menu_state.header.phase = (uint8)(3);
            menu_state.header.action = (uint8)(1);
            ui_animation_set(1, 3, -1, -1);
            ui_animation_set(2, 3, -1, -1);
            sound_play(59);
            sound_play(91);
            game_input.pressed = (uint32)(0);
        }
    }
    return;
}

uint32 mission_menu_main_close_tick(void)
{
    uint32 selection, result;

    FUNCTION_MARKER(0x800D2BCC, "ACE3.BIN");
    result = ui_animation_phase(1);
    if (result)
        return result;
    result = (sint16)mission_flow.transition_phase;
    if (result)
        return result;
    selection = menu_state.detail_choice;
    result = 4;
    if (selection == 1)
    {
        menu_state.header.phase = (uint8)(result);
        return result;
    }
    result = menu_state.header.action;
    if (result)
    {
        if (result == 1)
            menu_state.header.close_requested = (uint8)(result);
        return result;
    }
    if (!menu_state.main_choice)
        menu_state.header.kind = (uint8)(1);
    if (menu_state.main_choice == 1)
        menu_state.header.kind = (uint8)(3);
    if (menu_state.main_choice == 2)
        menu_state.header.kind = (uint8)(5);
    if (menu_state.main_choice == 3)
    {
        mission_flow.mode_phase = 0;
        menu_state.header.kind = (uint8)(6);
        ui_group_set_mask(2, 0);
        ui_group_set_mask(0, 0x8000);
        mission_menu_start_item_motion(0, 15);
    }
    if ((uint32)(menu_state.main_choice - 4) < 2)
    {
        mission_flow.mode_phase = 0;
        menu_state.header.kind = (uint8)(10);
        ui_group_set_mask(2, 0);
        ui_group_set_mask(0, 0);
    }
    ui_animation_set(1, 1, 0, 40);
    menu_state.header.phase = (uint8)(0);
    ui_animation_set(2, 1, 0, 40);
    return sound_play(90);
}

void mission_menu_detail_open_tick(void)
{
    uint32 value;

    FUNCTION_MARKER(0x800D2D74, "ACE3.BIN");
    if (!ui_item_motion_phase(0, 1))
    {
        value = mission_flow.ui_timer + 1;
        mission_flow.ui_timer = (uint32)(value);
    }
    if (ui_animation_phase(1) == 2)
        menu_state.header.phase = (uint8)(5);
    {
        ui_widget_set(5, menu_state.detail_choice);
        return;
    }
}

void mission_menu_detail_frame(void)
{
    FUNCTION_MARKER(0x800D2DF0, "ACE3.BIN");
    {
        mission_menu_detail_select_tick();
        return;
    }
}

void mission_menu_detail_select_tick(void)
{
    uint32 selection, result;

    FUNCTION_MARKER(0x800D2DF8, "ACE3.BIN");
    if (game_input.repeat & 8)
    {
        selection = menu_state.detail_choice;
        menu_state.detail_choice = (uint8)(selection < 3 ? selection + 1 : 1);
    }
    if (game_input.repeat & 4)
    {
        selection = menu_state.detail_choice;
        menu_state.detail_choice = (uint8)(selection < 2 ? 3 : selection - 1);
    }
    ui_widget_set(5, menu_state.detail_choice);
    if ((game_input.pressed & 0x10) && !(game_input.held & 0x2F))
    {
        selection = menu_state.detail_choice;
        if (selection == 3)
        {
            mission_flow.transition_phase = selection;
        }
        menu_state.header.phase = (uint8)(6);
        ui_animation_set(1, 3, -1, -1);
        ui_animation_set(2, 3, -1, -1);
        ui_item_reverse_motion(0, 1);
    }
    result = game_input.pressed & 0x20;
    if (result)
    {
        result = game_input.held & 0x5F;
        if (!result)
        {
            menu_state.detail_choice = (uint8)(0);
            menu_state.header.phase = (uint8)(6);
            ui_animation_set(1, 3, -1, -1);
            game_input.pressed = (uint32)(0);
        }
    }
    return;
}

void mission_menu_detail_close_tick(void)
{
    uint32 selection, result;

    FUNCTION_MARKER(0x800D2FA8, "ACE3.BIN");
    result = ui_animation_phase(1);
    if (result)
        return;
    result = (sint16)mission_flow.transition_phase;
    if (result)
        return;
    selection = menu_state.detail_choice;
    if (!selection)
    {
        menu_state.header.phase = (uint8)(1);
        ui_animation_set(1, 1, 0, 40);
        ui_group_set_mask(2, 1);
        return;
    }
    if (selection == 1)
        menu_state.header.kind = (uint8)(7);
    selection = menu_state.detail_choice;
    if (selection == 2)
    {
        menu_state.header.kind = (uint8)(8);
        selection = menu_state.detail_choice;
    }
    if (selection == 3)
        menu_state.header.kind = (uint8)(9);
    menu_state.header.phase = (uint8)(0);
    return;
}

void mission_menu_start_item_motion(uint32 group, uint32 item)
{
    uint32 first, second, third;

    FUNCTION_MARKER(0x800DEBB8, "ACE3.BIN");
    switch (item)
    {
        case 0:
            first = 0;
            second = 55;
            third = 10;
            break;
        case 1:
            first = -329;
            second = 0;
            third = 25;
            break;
        case 5:
            first = 0;
            second = -504;
            third = 31;
            break;
        case 8:
        case 13:
            first = 325;
            second = 0;
            third = -25;
            break;
        case 15:
            first = 0;
            second = 512;
            third = -31;
            break;
        default:
            first = 0;
            second = 0;
            third = 0;
            break;
    }
    ui_item_set_motion(group, item, 0, first, second, third);
    mission_flow.ui_timer = (uint32)(0);
    return;
}
