#include <stddef.h>
#include <stdlib.h>

#include "game_functions.h"

typedef struct
{
    uint16 x, y;
    const char *text;
} MISSION_STATUS_LINE;

typedef struct
{
    uint8 flags, duration;
    sint16 palette;
    uint8 line_count;
    uint32 track;
    MISSION_STATUS_LINE lines[2];
} MISSION_STATUS_MESSAGE;

enum { GLYPH_U, GLYPH_V, GLYPH_WIDTH, GLYPH_HEIGHT };

static const uint8 mission_status_atlas[][4] =
{
    {0, 160, 20, 22},
    {20, 160, 20, 22},
    {40, 160, 20, 22},
    {60, 160, 20, 22},
    {80, 160, 20, 22},
    {100, 160, 20, 22},
    {120, 160, 20, 22},
    {140, 160, 20, 22},
    {160, 160, 15, 22},
    {176, 160, 20, 22},
    {0, 182, 20, 22},
    {20, 182, 20, 22},
    {40, 182, 22, 22},
    {62, 182, 20, 22},
    {82, 182, 20, 22},
    {102, 182, 20, 22},
    {122, 182, 20, 22},
    {142, 182, 20, 22},
    {162, 182, 20, 22},
    {0, 204, 21, 22},
    {22, 204, 20, 22},
    {42, 204, 24, 22},
    {66, 204, 22, 22},
    {88, 204, 20, 22},
    {108, 204, 20, 22},
    {128, 204, 21, 22},
    {150, 204, 9, 22},
    {160, 204, 10, 22},
    {170, 204, 19, 22},
    {0, 226, 20, 22},
    {20, 226, 15, 22},
    {36, 226, 20, 22},
    {56, 226, 20, 22},
    {76, 226, 20, 22},
    {96, 226, 20, 22},
    {116, 226, 20, 22},
    {136, 226, 20, 22},
    {156, 226, 20, 22},
    {176, 226, 20, 22},
    /* byte selectors above the glyph atlas can read neighboring data */
    {9, 9, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {10, 9, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {11, 9, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {12, 9, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {13, 9, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {14, 9, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {15, 9, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {16, 9, 0, 0},
    {0, 0, 0, 0},
    {0, 0, 0, 0},
    {1, 150, 0, 0},
    {2, 0, 0, 0},
    {98, 0, 100, 0},
    {132, 205, 11, 128},
    {48, 0, 120, 0},
    {172, 65, 2, 128},
    {77, 0, 0, 0},
    {1, 150, 1, 0},
    {2, 0, 0, 0},
    {98, 0, 100, 0},
    {132, 205, 11, 128},
    {120, 0, 120, 0},
    {124, 205, 11, 128},
    {78, 0, 0, 0},
    {1, 150, 2, 0},
    {2, 0, 0, 0},
    {98, 0, 100, 0},
    {132, 205, 11, 128},
    {106, 0, 120, 0},
    {116, 205, 11, 128},
    {79, 0, 0, 0},
    {1, 60, 0, 0},
    {2, 0, 0, 0},
    {68, 0, 100, 0},
    {160, 65, 2, 128},
    {122, 0, 120, 0},
    {108, 205, 11, 128},
    {55, 0, 0, 0},
    {1, 60, 1, 0},
    {1, 0, 0, 0},
    {104, 0, 100, 0},
    {100, 205, 11, 128},
    {0, 0, 0, 0},
    {96, 205, 11, 128},
    {82, 0, 0, 0},
    {1, 60, 0, 0},
    {2, 0, 0, 0},
    {102, 0, 100, 0},
    {88, 205, 11, 128},
    {102, 0, 120, 0},
    {80, 205, 11, 128},
    {83, 0, 0, 0},
    {1, 30, 1, 0},
    {1, 0, 0, 0},
    {114, 0, 100, 0},
    {72, 205, 11, 128},
    {0, 0, 0, 0},
    {96, 205, 11, 128},
    {75, 0, 0, 0},
    {1, 30, 1, 0},
    {1, 0, 0, 0},
    {80, 0, 100, 0},
    {148, 65, 2, 128},
    {0, 0, 0, 0},
    {96, 205, 11, 128},
    {76, 0, 0, 0},
    {1, 60, 0, 0},
    {1, 0, 0, 0},
    {102, 0, 100, 0},
    {64, 205, 11, 128},
    {0, 0, 0, 0},
    {96, 205, 11, 128},
    {111, 0, 0, 0},
    {1, 60, 1, 0},
    {1, 0, 0, 0},
    {76, 0, 100, 0},
    {136, 65, 2, 128},
    {0, 0, 0, 0},
    {96, 205, 11, 128},
    {80, 0, 0, 0},
    {1, 60, 0, 0},
    {1, 0, 0, 0},
    {88, 0, 100, 0},
    {124, 65, 2, 128},
    {0, 0, 0, 0},
    {96, 205, 11, 128},
    {81, 0, 0, 0},
    {1, 60, 1, 0},
    {1, 0, 0, 0},
    {96, 0, 100, 0},
    {56, 205, 11, 128},
    {0, 0, 0, 0},
    {96, 205, 11, 128},
    {99, 0, 0, 0},
    {2, 60, 1, 0},
    {2, 0, 0, 0},
    {124, 0, 100, 0},
    {48, 205, 11, 128},
    {96, 0, 120, 0},
    {56, 205, 11, 128},
    {0, 0, 0, 0},
    {2, 60, 1, 0},
    {2, 0, 0, 0},
    {102, 0, 100, 0},
    {40, 205, 11, 128},
    {96, 0, 120, 0},
    {56, 205, 11, 128},
    {0, 0, 0, 0},
    {1, 60, 0, 0},
    {2, 0, 0, 0},
    {122, 0, 100, 0},
    {32, 205, 11, 128},
    {82, 0, 120, 0},
    {112, 65, 2, 128},
    {100, 0, 0, 0},
    {1, 60, 1, 0},
    {2, 0, 0, 0},
    {60, 0, 100, 0},
    {100, 65, 2, 128},
    {88, 0, 120, 0},
    {88, 65, 2, 128},
    {100, 0, 0, 0},
    {1, 60, 1, 0},
    {2, 0, 0, 0},
    {76, 0, 100, 0},
    {76, 65, 2, 128},
    {114, 0, 120, 0},
    {24, 205, 11, 128},
    {99, 0, 0, 0},
    {1, 60, 0, 0},
    {1, 0, 0, 0},
    {94, 0, 100, 0},
    {16, 205, 11, 128},
    {0, 0, 0, 0},
    {96, 205, 11, 128},
    {102, 0, 0, 0},
    {1, 60, 0, 0},
    {1, 0, 0, 0},
    {116, 0, 100, 0},
    {8, 205, 11, 128},
    {0, 0, 0, 0},
    {96, 205, 11, 128},
    {100, 0, 0, 0},
    {1, 60, 1, 0},
    {1, 0, 0, 0},
    {104, 0, 100, 0},
    {0, 205, 11, 128},
    {0, 0, 0, 0},
    {96, 205, 11, 128},
    {101, 0, 0, 0},
    {2, 60, 1, 0},
    {2, 0, 0, 0},
    {78, 0, 100, 0},
    {64, 65, 2, 128},
    {82, 0, 120, 0},
    {52, 65, 2, 128},
    {1, 0, 0, 0},
    {1, 60, 0, 0},
    {2, 0, 0, 0},
    {70, 0, 100, 0},
    {40, 65, 2, 128},
    {104, 0, 120, 0},
    {100, 205, 11, 128},
    {111, 0, 0, 0},
    {1, 60, 1, 0},
    {2, 0, 0, 0},
    {112, 0, 100, 0},
    {248, 204, 11, 128},
    {76, 0, 120, 0},
    {28, 65, 2, 128},
    {99, 0, 0, 0},
    {1, 60, 1, 0},
    {2, 0, 0, 0},
    {86, 0, 100, 0},
    {16, 65, 2, 128},
    {46, 0, 120, 0},
    {0, 65, 2, 128},
    {101, 0, 0, 0},
    {1, 60, 1, 0},
    {2, 0, 0, 0},
    {124, 0, 100, 0},
    {240, 204, 11, 128},
    {86, 0, 120, 0},
    {244, 64, 2, 128},
    {102, 0, 0, 0},
    {0, 60, 1, 0},
    {1, 0, 0, 0},
    {90, 0, 100, 0},
    {232, 64, 2, 128},
    {0, 0, 0, 0},
    {96, 205, 11, 128},
    {0, 0, 0, 0},
    {0, 60, 1, 0},
    {1, 0, 0, 0},
    {102, 0, 100, 0},
    {232, 204, 11, 128},
    {0, 0, 0, 0},
    {96, 205, 11, 128},
    {0, 0, 0, 0},
    {1, 60, 1, 0},
    {1, 0, 0, 0},
    {84, 0, 100, 0},
    {220, 64, 2, 128},
};

static const MISSION_STATUS_MESSAGE mission_status_messages[] =
{
    {1, 150, 0, 2, 77, {{98, 100, "MISSION"}, {48, 120, "ACCOMPLISHED"}}},
    {1, 150, 1, 2, 78, {{98, 100, "MISSION"}, {120, 120, "OVER"}}},
    {1, 150, 2, 2, 79, {{98, 100, "MISSION"}, {106, 120, "FAILED"}}},
    {1, 60, 0, 2, 55, {{68, 100, "CONNECTION"}, {122, 120, "LOST"}}},
    {1, 60, 1, 1, 82, {{104, 100, "ENGAGE"}}},
    {1, 60, 0, 2, 83, {{102, 100, "UPDATE"}, {102, 120, "TARGET"}}},
    {1, 30, 1, 1, 75, {{114, 100, "BINGO"}}},
    {1, 30, 1, 1, 76, {{80, 100, "BULLS EYE"}}},
    {1, 60, 0, 1, 111, {{102, 100, "MAYDAY"}}},
    {1, 60, 1, 1, 80, {{76, 100, "INTERCEPT"}}},
    {1, 60, 0, 1, 81, {{88, 100, "HURRY UP"}}},
    {1, 60, 1, 1, 99, {{96, 100, "LANDING"}}},
    {2, 60, 1, 2, 0, {{124, 100, "NICE"}, {96, 120, "LANDING"}}},
    {2, 60, 1, 2, 0, {{102, 100, "SMOOTH"}, {96, 120, "LANDING"}}},
    {1, 60, 0, 2, 100, {{122, 100, "HOLD"}, {82, 120, "YOUR FIRE"}}},
    {1, 60, 1, 2, 100, {{60, 100, "CLEARED FOR"}, {88, 120, "TAKE OFF"}}},
    {1, 60, 1, 2, 99, {{76, 100, "ERADICATE"}, {114, 120, "VIRUS"}}},
    {1, 60, 0, 1, 102, {{94, 100, "JAMMING"}}},
    {1, 60, 0, 1, 100, {{116, 100, "SPIKE"}}},
    {1, 60, 1, 1, 101, {{104, 100, "REFUEL"}}},
    {2, 60, 1, 2, 1, {{78, 100, "REFUELING"}, {82, 120, "COMPLETE"}}},
    {1, 60, 0, 2, 111, {{70, 100, "CLEARED TO"}, {104, 120, "ENGAGE"}}},
    {1, 60, 1, 2, 99, {{112, 100, "LASER"}, {76, 120, "TARGETING"}}},
    {1, 60, 1, 2, 101, {{86, 100, "ENTERING"}, {46, 120, "STRATOSPHERE"}}},
    {1, 60, 1, 2, 102, {{124, 100, "HIGH"}, {86, 120, "ALTITUDE"}}},
    {0, 60, 1, 1, 0, {{90, 100, "LIFT OFF"}}},
    {0, 60, 1, 1, 0, {{102, 100, "DETACH"}}},
    {1, 60, 1, 1, 99, {{84, 100, "SCRAMBLE"}}},
    {1, 60, 0, 2, 99, {{98, 100, "MISSILE"}, {84, 120, "LAUNCHED"}}},
    {1, 60, 1, 1, 103, {{96, 100, "DOCKING"}}},
    {1, 60, 1, 2, 99, {{114, 100, "TIMER"}, {74, 120, "ACTIVATED"}}},
    {1, 60, 0, 2, 99, {{56, 100, "AEONOSPHERE"}, {74, 120, "DESTROYED"}}},
    {1, 60, 0, 1, 99, {{96, 100, "HACKING"}}},
    {0, 60, 1, 1, 0, {{100, 100, "JACK IN"}}},
    {1, 60, 1, 2, 100, {{86, 100, "ENTERING"}, {36, 120, "ELECTROSPHERE"}}},
    {1, 60, 1, 1, 99, {{94, 100, "RELEASE"}}},
    {1, 60, 0, 1, 101, {{122, 100, "HELP"}}},
    {1, 60, 0, 2, 102, {{68, 100, "ASSISTANCE"}, {86, 120, "REQUIRED"}}},
    {1, 60, 1, 2, 101, {{94, 100, "LEAVING"}, {64, 120, "ATMOSPHERE"}}},
    {1, 60, 1, 2, 102, {{56, 100, "ATMOSPHERIC"}, {112, 120, "ENTRY"}}},
    {1, 60, 1, 1, 99, {{68, 100, "COUNT DOWN"}}},
    {2, 60, 2, 2, 2, {{96, 100, "LANDING"}, {106, 120, "FAILED"}}},
    {2, 60, 2, 2, 3, {{102, 100, "REFUEL"}, {106, 120, "FAILED"}}},
    {2, 60, 1, 2, 4, {{96, 100, "DOCKING"}, {82, 120, "COMPLETE"}}},
    {2, 60, 2, 2, 5, {{96, 100, "DOCKING"}, {106, 120, "FAILED"}}},
    {1, 60, 0, 1, 98, {{106, 100, "DECIDE"}}},
    {1, 60, 1, 1, 98, {{82, 100, "TARGETED"}}},
    {2, 60, 1, 1, 6, {{72, 100, "NICE ENTRY"}}},
    {2, 60, 2, 2, 7, {{112, 100, "ENTRY"}, {106, 120, "FAILED"}}},
    {1, 60, 1, 1, 112, {{88, 100, "TAKE OFF"}}},
    {0, 60, 1, 2, 0, {{56, 100, "TO BE"}, {106, 120, "CONTINUED"}}},
    {1, 60, 1, 2, 98, {{96, 100, "HACKING"}, {82, 120, "COMPLETE"}}},
};

static uint32 decode_glyph(uint32 character)
{
    uint32 value;
    FUNCTION_MARKER(0x8004D124, "ACE3.BIN");
    if (((character - 65) & 255) < 26)
        return (character & 255) - 65;
    if (((character - 48) & 255) < 10)
        return (character & 255) - 19;
    value = character & 255;
    if (value == 46)
        return 26;
    if (value == 33)
        return 27;
    if (value == 63)
        return 28;
    if (value == 32)
        return 80;
    return 0xFFFFFFFF;
}

static void append_line(const MISSION_STATUS_LINE *line)
{
    MISSION_STATUS_TEXT_STATE *status = &game_runtime.mission_status_text;
    MISSION_STATUS_GLYPH *record;
    uint32 index, x, y, character, glyph, value;
    FUNCTION_MARKER(0x8004D038, "ACE3.BIN");
    index = 0;
    x = line->x;
    y = line->y;
    for (;;)
    {
        character = (uint8)line->text[index];
        if (character == 0)
            return;
        ++index;
        glyph = decode_glyph(character);
        if (glyph == 80)
        {
            x += 10;
            continue;
        }
        if (glyph == 0xFFFFFFFF)
            return;
        record = &status->glyphs[status->glyph_count++];
        record->phase = 0;
        record->atlas_index = (uint8)glyph;
        record->ticks = 0;
        record->x = (uint16)x;
        record->y = (uint16)y;
        value = mission_status_atlas[glyph][GLYPH_WIDTH];
        x += value - 1;
    }
}

enum { MISSION_STATUS_PACKET_COUNT = 32 };

struct MISSION_STATUS_PACKET_PAGE
{
    SPRT sprites[MISSION_STATUS_PACKET_COUNT];
    POLY_FT4 quads[MISSION_STATUS_PACKET_COUNT];
    uint32 mode_tag;
    uint32 mode_code;
};

enum
{
    MISSION_STATUS_QUAD_OFFSET = offsetof(MISSION_STATUS_PACKET_PAGE, quads),
    MISSION_STATUS_MODE_OFFSET = offsetof(MISSION_STATUS_PACKET_PAGE, mode_tag),
    MISSION_STATUS_SPRITE_BYTES = sizeof(SPRT),
    MISSION_STATUS_QUAD_BYTES = sizeof(POLY_FT4)
};

typedef char mission_status_packet_page_layout[
    sizeof(MISSION_STATUS_PACKET_PAGE) == 1928 &&
    MISSION_STATUS_QUAD_OFFSET == 640 && MISSION_STATUS_MODE_OFFSET == 1920 &&
    offsetof(MISSION_STATUS_PACKET_PAGE, mode_code) == 1924 &&
    MISSION_STATUS_SPRITE_BYTES == 20 && MISSION_STATUS_QUAD_BYTES == 40 &&
    offsetof(SPRT, rgbc) == 4 && offsetof(POLY_FT4, rgbc) == 4 ? 1 : -1];

static MISSION_STATUS_PACKET_PAGE status_packet_pages[2];

void mission_status_text_init(void)
{
    MISSION_STATUS_TEXT_STATE *status = &game_runtime.mission_status_text;
    uint32 block, i, j;
    MISSION_STATUS_PACKET_PAGE *page;
    FUNCTION_MARKER(0x8004C6EC, "ACE3.BIN");
    /* retain the reservation while subsequent arena inputs depend on its address */
    if (!arena_alloc(288))
        abort();
    if (!gpu_register_packet_range(status_packet_pages, sizeof(status_packet_pages)))
        abort();
    for (i = 0; i < 2; ++i)
    {
        block = arena_alloc(sizeof(*page));
        if (!block)
            abort();
        page = &status_packet_pages[i];
        block = gpu_packet_link((uintptr_t)page);
        status->packet_address = block;
        status->packet_pages[i] = block;
        status->page_data[i] = page;
        status->packet_page = page;
        for (j = 0; j < MISSION_STATUS_PACKET_COUNT; ++j)
        {
            ((uint8 *)&page->quads[j].tag)[3] = 9;
            page->quads[j].rgbc.code = 44;
        }
        for (j = 0; j < MISSION_STATUS_PACKET_COUNT; ++j)
        {
            ((uint8 *)&page->sprites[j].tag)[3] = 4;
            page->sprites[j].rgbc.code = 100;
        }
        ((uint8 *)&page->mode_tag)[3] = 1;
        page->mode_code = 0xE1000656;
    }
    for (i = 0; i < MISSION_STATUS_GLYPH_CAPACITY; ++i)
        status->glyphs[i].phase = 0;
    status->sprite_count = 0;
    status->quad_count = 0;
    status->glyph_count = 0;
    status->duration = 0;
    status->palette = 0;
    status->current = 255;
}

void mission_status_text_begin(void)
{
    MISSION_STATUS_TEXT_STATE *status = &game_runtime.mission_status_text;
    uint32 value;
    FUNCTION_MARKER(0x8004C824, "ACE3.BIN");
    status->sprite_count = 0;
    status->quad_count = 0;
    value = game_runtime.display_buffer;
    status->packet_page = status->page_data[value];
    value = status->packet_pages[value];
    status->packet_address = value;
    { (void)(uint32)(value); return; }
}

uint32 mission_status_text_current(void)
{
    MISSION_STATUS_TEXT_STATE *status = &game_runtime.mission_status_text;
    FUNCTION_MARKER(0x8004C858, "ACE3.BIN");
    return status->current;
}

void mission_status_text_submit(void)
{
    MISSION_STATUS_TEXT_STATE *status = &game_runtime.mission_status_text;
    uint32 count, ot, index, packet, tag, head, value;
    uint32 *ot_word, *tag_word;
    FUNCTION_MARKER(0x8004C86C, "ACE3.BIN");
    count = status->quad_count;
    ot = game_runtime.mission_control.ot;
    ot_word = (uint32 *)mission_ot_span(ot, sizeof(*ot_word));
    index = 0;
    if ((sint32)count > 0)
    {
        packet = status->packet_address + MISSION_STATUS_QUAD_OFFSET;
        do
        {
            ++index;
            tag_word = &status->packet_page->quads[index - 1].tag;
            tag = (*tag_word);
            head = (*ot_word);
            *tag_word = ((tag & 0xFF000000) | (head & 0xFFFFFF));
            head = (*ot_word);
            *ot_word = ((head & 0xFF000000) | (packet & 0xFFFFFF));
            count = status->quad_count;
            value = (sint32)index < (sint32)count;
            packet += MISSION_STATUS_QUAD_BYTES;
        } while (value);
    }
    count = status->sprite_count;
    index = 0;
    if ((sint32)count > 0)
    {
        packet = status->packet_address;
        do
        {
            ++index;
            tag_word = &status->packet_page->sprites[index - 1].tag;
            tag = (*tag_word);
            head = (*ot_word);
            *tag_word = ((tag & 0xFF000000) | (head & 0xFFFFFF));
            head = (*ot_word);
            *ot_word = ((head & 0xFF000000) | (packet & 0xFFFFFF));
            count = status->sprite_count;
            value = (sint32)index < (sint32)count;
            packet += MISSION_STATUS_SPRITE_BYTES;
        } while (value);
    }
    packet = status->packet_address;
    head = (*ot_word);
    tag_word = &status->packet_page->mode_tag;
    tag = (*tag_word);
    *tag_word = ((tag & 0xFF000000) | (head & 0xFFFFFF));
    head = (*ot_word);
    value = (head & 0xFF000000) | ((packet + MISSION_STATUS_MODE_OFFSET) & 0xFFFFFF);
    *ot_word = (value);
    { (void)(uint32)(value); return; }
}

void mission_status_text_update(void)
{
    MISSION_STATUS_TEXT_STATE *status = &game_runtime.mission_status_text;
    uint32 flags;
    FUNCTION_MARKER(0x8004C998, "ACE3.BIN");
    flags = game_runtime.mission_control.mode;
    if ((flags & 0x2C0E) == 0)
    {
        if (status->current == 255)
            return;
        { (void)(mission_status_text_reset()); return; }
    }
    mission_status_text_begin();
    mission_status_text_draw();
    flags = (~game_runtime.flags) & 0x20000000;
    if (flags == 0)
        return;
    { (void)(mission_status_text_submit()); return; }
}

void mission_status_text_draw(void)
{
    MISSION_STATUS_TEXT_STATE *status = &game_runtime.mission_status_text;
    MISSION_STATUS_GLYPH *record;
    uint32 clut, count, index, active, selector, mode;
    uint32 value, other, opcode, x, y, width, height, right, bottom, shift, small_width, small_height;
    SPRT *spr;
    POLY_FT4 *poly;
    FUNCTION_MARKER(0x8004CA28, "ACE3.BIN");
    index = 0;
    active = 0;
    value = (uint16)status->palette;
    count = status->glyph_count;
    clut = value + 0x7F58;
    if ((sint32)count > 0)
        do
        {
            record = &status->glyphs[index];
            selector = record->atlas_index;
            mode = record->phase;
            if (mode == 1)
            {
                count = status->sprite_count;
                spr = &status->packet_page->sprites[count];
                opcode = spr->rgbc.code;
                spr->rgbc.r = (uint8)(128);
                spr->rgbc.g = (uint8)(128);
                spr->rgbc.b = (uint8)(128);
                spr->rgbc.code = (uint8)(opcode | 2);
                value = record->x;
                spr->p[0].xy.x = (sint16)(uint16)(value);
                value = record->y;
                spr->p[0].xy.y = (sint16)(uint16)(value);
                value = mission_status_atlas[selector][GLYPH_U];
                spr->p[0].uv.u = (uint8)(value);
                value = mission_status_atlas[selector][GLYPH_V];
                spr->p[0].attr = (uint16)(clut);
                spr->p[0].uv.v = (uint8)(value);
                value = mission_status_atlas[selector][GLYPH_WIDTH];
                spr->w = (sint16)(uint16)(value);
                value = mission_status_atlas[selector][GLYPH_HEIGHT];
                spr->h = (sint16)(uint16)(value);
                value = status->sprite_count;
                status->sprite_count = value + 1;
                if ((game_runtime.flags & 0x20000000) == 0)
                {
                    value = record->ticks;
                    record->ticks = (uint16)(value + 1);
                }
                value = (sint16)record->ticks;
                other = status->duration;
                if ((sint32)value >= (sint32)other)
                {
                    record->phase = 3;
                    record->ticks = 0;
                }
            }
            else if (mode == 2 || mode == 3)
            {
                count = status->quad_count;
                if (mode == 2)
                {
                    x = (sint16)record->x;
                    y = (sint16)record->y;
                    width = mission_status_atlas[selector][GLYPH_WIDTH];
                    height = mission_status_atlas[selector][GLYPH_HEIGHT];
                    poly = &status->packet_page->quads[count];
                    opcode = poly->rgbc.code;
                    right = x + width;
                    poly->p[0].xy.y = (sint16)(uint16)(y);
                    poly->p[1].xy.y = (sint16)(uint16)(y);
                    bottom = y + height;
                }
                else
                {
                    width = mission_status_atlas[selector][GLYPH_WIDTH];
                    shift = (sint16)record->ticks;
                    height = mission_status_atlas[selector][GLYPH_HEIGHT];
                    x = (sint16)record->x;
                    y = (sint16)record->y;
                    small_width = (sint32)width >> (shift & 31);
                    x += (sint32)(width - small_width) >> 1;
                    small_height = (sint32)height >> (shift & 31);
                    y += (sint32)(height - small_height) >> 1;
                    poly = &status->packet_page->quads[count];
                    opcode = poly->rgbc.code;
                    right = x + small_width;
                    poly->p[0].xy.y = (sint16)(uint16)(y);
                    poly->p[1].xy.y = (sint16)(uint16)(y);
                    bottom = y + small_height;
                    poly->rgbc.r = (uint8)(128);
                    poly->rgbc.g = (uint8)(128);
                    poly->rgbc.b = (uint8)(128);
                }
                poly->p[0].xy.x = (sint16)(uint16)(x);
                poly->p[1].xy.x = (sint16)(uint16)(right);
                poly->p[2].xy.x = (sint16)(uint16)(x);
                poly->p[2].xy.y = (sint16)(uint16)(bottom);
                poly->p[3].xy.x = (sint16)(uint16)(right);
                poly->p[3].xy.y = (sint16)(uint16)(bottom);
                poly->rgbc.code = (uint8)(opcode | 2);
                value = mission_status_atlas[selector][GLYPH_U];
                poly->p[0].uv.u = (uint8)(value);
                value = mission_status_atlas[selector][GLYPH_V];
                poly->p[0].uv.v = (uint8)(value);
                value = mission_status_atlas[selector][GLYPH_U];
                other = mission_status_atlas[selector][GLYPH_WIDTH];
                poly->p[1].uv.u = (uint8)(value + other);
                value = mission_status_atlas[selector][GLYPH_V];
                poly->p[1].uv.v = (uint8)(value);
                value = mission_status_atlas[selector][GLYPH_U];
                poly->p[2].uv.u = (uint8)(value);
                value = mission_status_atlas[selector][GLYPH_V];
                other = mission_status_atlas[selector][GLYPH_HEIGHT];
                poly->p[2].uv.v = (uint8)(value + other);
                value = mission_status_atlas[selector][GLYPH_U];
                other = mission_status_atlas[selector][GLYPH_WIDTH];
                poly->p[3].uv.u = (uint8)(value + other);
                value = mission_status_atlas[selector][GLYPH_V];
                other = mission_status_atlas[selector][GLYPH_HEIGHT];
                if (mode == 3)
                    poly->p[0].attr = (uint16)(clut);
                poly->p[3].uv.v = (uint8)(value + other);
                if (mode == 2)
                {
                    poly->p[0].attr = (uint16)(clut);
                    poly->p[1].attr = (uint16)(86);
                    value = (uint32)(sint16)record->ticks << 5;
                    poly->rgbc.r = (uint8)(value);
                    poly->rgbc.g = (uint8)(value);
                    poly->rgbc.b = (uint8)(value);
                }
                else
                {
                    poly->p[1].attr = (uint16)(86);
                }
                value = status->quad_count;
                status->quad_count = value + 1;
                if ((game_runtime.flags & 0x20000000) == 0)
                {
                    value = record->ticks;
                    record->ticks = (uint16)(value + 1);
                }
                if (mode == 2)
                {
                    if ((sint16)record->ticks >= 8)
                    {
                        record->phase = 1;
                        record->ticks = 0;
                    }
                }
                else if (small_width == 0 && small_height == 0)
                {
                    record->phase = 0;
                    record->ticks = 0;
                }
            }
            if (mode != 0 && record->phase != 0)
                ++active;
            count = status->glyph_count;
            ++index;
        } while ((sint32)index < (sint32)count);
    if (active == 0)
    {
        status->glyph_count = 0;
        status->current = 255;
    }
    return;
}

void mission_status_text_reset(void)
{
    MISSION_STATUS_TEXT_STATE *status = &game_runtime.mission_status_text;
    FUNCTION_MARKER(0x8004CE9C, "ACE3.BIN");
    status->glyph_count = 0;
    status->current = 255;
    return;
}

void mission_status_text_show(uint32 kind)
{
    MISSION_STATUS_TEXT_STATE *status = &game_runtime.mission_status_text;
    const MISSION_STATUS_MESSAGE *message;
    uint32 value, flags, i;
    FUNCTION_MARKER(0x8004CEB0, "ACE3.BIN");
    if (kind >= sizeof(mission_status_messages) / sizeof(*mission_status_messages))
        return;
    if (status->current == kind)
        return;
    message = &mission_status_messages[kind];
    status->current = kind;
    status->duration = message->duration;
    status->glyph_count = 0;
    status->palette = (uint32)(sint32)message->palette;
    flags = message->flags;
    if ((flags & 1) != 0)
    {
        value = (uint8)message->track;
        sound_schedule_delayed(value, 30);
    }
    else if ((flags & 2) != 0)
    {
        resource_cancel_streams();
        value = message->track;
        if (value >= audio_tracks_events.count)
            abort();
        resource_queue_audio(audio_tracks_events.records + value, 0, 0);
    }
    for (i = 0; i < message->line_count; ++i)
        append_line(&message->lines[i]);
    for (i = status->glyph_count; i < MISSION_STATUS_GLYPH_CAPACITY; ++i)
        status->glyphs[i].phase = 0;
    for (i = 0; i < status->glyph_count; ++i)
        status->glyphs[i].phase = 2;
}

void mission_status_text_frame(void)
{
    FUNCTION_MARKER(0x800337BC, "ACE3.BIN");
    { (void)(mission_status_text_update()); return; }
}
