#include "game_functions.h"
#include <string.h>
#include <stdio.h>

enum { GAME_SAVE_HEADER = 16, GAME_SAVE_PAYLOAD = 2228, GAME_SAVE_BYTES = GAME_SAVE_HEADER + GAME_SAVE_PAYLOAD };

/* Original account, progress and controller fields contain no pointers */
static const struct { uint32 offset, bytes; } game_save_regions[] = {
    {370, 68}, {462, 68}, {554, 68}, {646, 68}, {738, 68}, {830, 68},
    {908, 32}, {2376, 20}, {2396, 1756}
};

static uint32 game_save_checksum(const uint8 *data, uint32 bytes)
{
    uint32 hash = 2166136261u, index;
    for (index = 0; index < bytes; ++index)
        hash = (hash ^ data[index]) * 16777619u;
    return hash;
}

sint32 game_account_save(void)
{
    uint8 data[GAME_SAVE_BYTES];
    uint32 game = r_u32(0x800BCE60), display = r_u32(0x800BCE48);
    uint32 index, offset = GAME_SAVE_HEADER;
    for (index = 0; index < sizeof(game_save_regions) / sizeof(game_save_regions[0]); ++index)
    {
        uint32 bytes = game_save_regions[index].bytes;
        memcpy(data + offset, psx_addr(game + game_save_regions[index].offset, bytes), bytes);
        offset += bytes;
    }
    xport_store_le32(data + offset, r_u32(display + 8));
    xport_store_le32(data + offset + 4, r_u32(display + 12));
    xport_store_le32(data + offset + 8, r_u32(game + 288) & 3);
    xport_store_le32(data, 0x53334341);
    xport_store_le32(data + 4, 1);
    xport_store_le32(data + 8, GAME_SAVE_PAYLOAD);
    xport_store_le32(data + 12, game_save_checksum(data + GAME_SAVE_HEADER, GAME_SAVE_PAYLOAD));
    if (xport_save(data, sizeof(data)) == 1)
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
    uint8 data[GAME_SAVE_BYTES];
    sint32 bytes = xport_load(data, sizeof(data));
    uint32 index, offset = GAME_SAVE_HEADER;
    uint32 game = r_u32(0x800BCE60), display = r_u32(0x800BCE48);
    if (!bytes)
    {
#ifndef NDEBUG
        fprintf(stderr, "AC3 account load: no save\n");
#endif
        return 0;
    }
    if (bytes != sizeof(data) || xport_load_le32(data) != 0x53334341 ||
        xport_load_le32(data + 4) != 1 || xport_load_le32(data + 8) != GAME_SAVE_PAYLOAD ||
        xport_load_le32(data + 12) != game_save_checksum(data + GAME_SAVE_HEADER, GAME_SAVE_PAYLOAD) ||
        xport_load_le32(data + GAME_SAVE_HEADER + 408 + 8) > 4 ||
        !data[GAME_SAVE_HEADER + 440] || !memchr(data + GAME_SAVE_HEADER + 440, 0, 9) ||
        (sint32)xport_load_le32(data + sizeof(data) - 12) < -12 ||
        (sint32)xport_load_le32(data + sizeof(data) - 12) > 12 ||
        xport_load_le32(data + sizeof(data) - 8) > 8 || xport_load_le32(data + sizeof(data) - 4) > 3)
    {
        xport_message_error("Ace Combat 3 - Load error", "The account could not be loaded. Create a new account to continue.");
        return 0;
    }
    for (index = 0; index < sizeof(game_save_regions) / sizeof(game_save_regions[0]); ++index)
    {
        uint32 count = game_save_regions[index].bytes;
        memcpy(psx_addr(game + game_save_regions[index].offset, count), data + offset, count);
        offset += count;
    }
    w_u32(display + 8, xport_load_le32(data + offset));
    w_u32(display + 12, xport_load_le32(data + offset + 4));
    w_u32(display, r_u32(display) | 2);
    w_u32(game + 288, (r_u32(game + 288) & ~3u) | xport_load_le32(data + offset + 8));
    w_u16(game + 0x954, 2);
    w_u32(game + 0x96C, r_u32(game + 0x96C) & ~0x10u);
    sub_8002bd28((r_u8(game + 922) & 1) != 0);
    sub_8002bd5c();
#ifndef NDEBUG
    fprintf(stderr, "AC3 account restored: %u bytes\n", (uint32)bytes);
#endif
    return 1;
}

uint32 game_account_save_requested(uint32 state)
{
    uint32 game = r_u32(0x800BCE60), mode = r_u32(state + 224);
    sint32 mission;
    game_account_save();
    resource002_sub_800de908(r_u32(state + 348), 27, 0);
    w_u16(state + 132, 7);
    w_u16(state + 134, 0);
    if (mode == 0)
    {
        mission = r_s16(game + 908);
        if (mission == 20 || mission == 35 || mission == 40 || mission == 45)
        {
            w_u16(game + 2388, 2);
            w_u32(game + 2412, r_u32(game + 2412) | 1);
            w_u32(state + 148, 0x60000003);
        }
        else
            w_u32(game + 2412, r_u32(game + 2412) & ~1u);
    }
    w_u32(game + 2412, r_u32(game + 2412) & ~8u);
    return 1;
}
