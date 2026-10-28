#include "game_functions.h"
#include <string.h>

void game_pad_refresh_connection(uint32 input_state)
{
    static uint8 available[2] = {255, 255};
    uint8 axes[4];
    uint32 controller, analog, port;
    for (controller = 0; controller < 2; ++controller)
    {
        xport_input_read((sint32)controller);
        analog = xport_input_analog((sint32)controller, axes) != 0;
        if (available[controller] != analog)
        {
            port = input_state + 0x44 + controller * 0x40;
            if (available[controller] != 255 && r_u8(r_u32(port + 4) + 12))
            {
                /* Reuse original negotiation when the host input device changes */
                w_u8(port + 1, 0);
                w_u8(port + 14, 1);
            }
            available[controller] = (uint8)analog;
        }
    }
}

static uint32 game_frame_contains(const PSX_RECT *draw, const PSX_RECT *display)
{
    return draw->w > 0 && draw->h > 0 && display->w > 0 && display->h > 0 &&
           draw->x <= display->x && draw->y <= display->y &&
           (sint32)draw->x + draw->w >= (sint32)display->x + display->w &&
           (sint32)draw->y + draw->h >= (sint32)display->y + display->h;
}

uint32 game_frame_display_ready(PSX_RECT *previous_draw, uint32 *previous_complete)
{
    uint32 game = r_u32(0x800BCE60);
    uint32 buffer = r_u32(0x800BCE58) + r_u32(game + 4) * 4116;
    uint32 draw_address = r_u32(buffer);
    uint32 display_address = r_u32(buffer + 4);
    uint32 movie = r_u32(game) & 4;
    uint32 complete = !movie;
    uint32 ready;
    PSX_RECT draw, display;
    draw.x = r_s16(draw_address);
    draw.y = r_s16(draw_address + 2);
    draw.w = r_s16(draw_address + 4);
    draw.h = r_s16(draw_address + 6);
    display.x = r_s16(display_address);
    display.y = r_s16(display_address + 2);
    display.w = r_s16(display_address + 4);
    display.h = r_s16(display_address + 6);
    /* Present only a completed draw page or movie frame */
    ready = movie || (complete && game_frame_contains(&draw, &display)) ||
            (*previous_complete && game_frame_contains(previous_draw, &display));
    if (movie)
        *previous_complete = 0;
    else if (complete)
    {
        *previous_draw = draw;
        *previous_complete = 1;
    }
    return ready;
}

#if AP_WEB && defined(_DEBUG)
    #include <emscripten.h>
EMSCRIPTEN_KEEPALIVE const uint32 *game_web_debug_snapshot(void)
{
    static uint32 snapshot[34];
    DISPENV display = {0};
    uint32 index, nonzero = 0;
    uint32 game = r_u32(0x800BCE60);
    uint32 scene = r_u32(0x800BCE54);
    uint32 queue = r_u32(0x800BCBBC);
    snapshot[0] = game;
    snapshot[1] = game ? r_u32(game) : 0;
    snapshot[2] = game ? r_u32(game + 0x24) : 0;
    snapshot[3] = game ? r_u32(game + 0x20) : 0;
    snapshot[4] = scene;
    snapshot[5] = scene ? r_u32(scene) : 0;
    snapshot[6] = scene ? r_u32(scene + 4) : 0;
    snapshot[7] = queue ? r_u32(queue) : 0;
    snapshot[8] = queue ? r_u32(queue + 4) : 0;
    snapshot[9] = queue ? r_u32(queue + 0x148) : 0;
    snapshot[10] = queue ? r_u32(queue + 0x174) : 0;
    snapshot[11] = game_resource_music_active();
    snapshot[12] = game ? r_u32(game + 0x120) : 0;
    snapshot[13] = game ? r_u32(game + 0x124) : 0;
    snapshot[14] = game ? r_u32(game + 0x128) : 0;
    snapshot[15] = game ? r_u32(game + 0x12C) : 0;
    snapshot[16] = r_u32(0x800BD044);
    snapshot[17] = snapshot[16] ? r_u32(snapshot[16]) : 0;
    snapshot[18] = r_u32(0x800BD048);
    snapshot[19] = snapshot[18] ? r_u32(snapshot[18] + 4) : 0;
    snapshot[20] = snapshot[18] ? r_u32(snapshot[18] + 44) : 0;
    snapshot[21] = snapshot[18] ? r_u32(snapshot[18] + 24) : 0;
    snapshot[22] = snapshot[18] ? r_u32(snapshot[18] + 32) : 0;
    snapshot[23] = g_xport_audio_submitted_buffers;
    snapshot[24] = g_xport_audio_nonzero_buffers;
    snapshot[25] = g_xport_audio_peak;
    snapshot[26] = snapshot[18] ? r_u32(snapshot[18]) : 0;
    gpu_get_display_env(&display);
    for (index = 0; index < GPU_VRAM_WIDTH * GPU_VRAM_HEIGHT; ++index)
        nonzero += VRAM[index] != 0;
    snapshot[27] = nonzero;
    snapshot[28] = (uint16)display.disp.x | ((uint32)(uint16)display.disp.y << 16);
    snapshot[29] = (uint16)display.disp.w | ((uint32)(uint16)display.disp.h << 16);
    snapshot[30] = display.isrgb24;
    snapshot[31] = 0;
    snapshot[32] = 0;
    snapshot[33] = gpu_get_raster();
    return snapshot;
}
#endif

/* Original ResetGraph environment owns the detected GPU type */
const uint32 xport_gpu_graph_type_address = 0x800BB964;

static sint32 game_load_main_image(void)
{
    uint8 header[2048];
    uint32 entry, address, bytes, cursor, stack, reserve, heap_bytes;
    void *payload;
    FILE *file = xport_fopen("DATA/1/ACE3.BIN", "rb");
    if (!file)
        return 0;
    if (xport_fread(header, 1, sizeof(header), file) != sizeof(header) || memcmp(header, "PS-X EXE", 8))
    {
        fclose(file);
        return 0;
    }
    entry = xport_load_le32(header + 0x10);
    address = xport_load_le32(header + 0x18);
    bytes = xport_load_le32(header + 0x1C);
    if (entry != 0x8002572C || address != 0x80010000 || bytes > 0x1F0000 || bytes < 0xACE40)
    {
        fclose(file);
        return 0;
    }
    payload = psx_addr(address, bytes);
    if (!payload || xport_fread(payload, 1, bytes, file) != bytes)
    {
        fclose(file);
        return 0;
    }
    fclose(file);
    for (cursor = 0x800BCE40; cursor < 0x800CBFE8; cursor += 4)
        w_u32(cursor, 0);
    stack = r_u32(0x800257DC);
    reserve = r_u32(0x800B473C);
    heap_bytes = stack - reserve - 0xCBFE8;
    /* The reviewed BIOS Exec JALR supplies this incoming CRT return address */
    w_u32(0x800BCE40, 0xBFC03D60);
    InitHeap(0x800CBFEC, heap_bytes);
    (void)r_u32(0x800BCE40);
    return 1;
}

void xport_main(void)
{
    /* SYSTEM.CNF numbers use the reviewed BIOS hexadecimal parser */
    if (!SetConf(0x16, 4, 0x801FFF00))
    {
#ifndef NDEBUG
        fprintf(stderr, "Unable to initialize reviewed BIOS kernel configuration\n");
#endif
        return;
    }
    if (!game_load_main_image())
    {
#ifndef NDEBUG
        fprintf(stderr, "Unable to load reviewed DATA/1/ACE3.BIN\n");
#endif
        return;
    }
    game_bind_sdk();
    if (!game_mount_data(1))
    {
#ifndef NDEBUG
        fprintf(stderr, "Unable to open extracted DATA/1 files\n");
#endif
        return;
    }
    game_run();
}
