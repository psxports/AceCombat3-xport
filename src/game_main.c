#include "game_functions.h"
#include <string.h>

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
