#include "game_functions.h"
#include <stdlib.h>
#include <string.h>
#include "game_overlay_resource001.inc"
#include "game_overlay_resource002.inc"

static uint32 overlay_pending, overlay_resource, overlay_destination, overlay_active;

sint32 game_spu_read_decoded_native(uint32 destination, uint32 flags)
{
    uint32 bytes = flags == 5 ? 2048 : 4096;
    void *buffer = psx_addr(destination, bytes);
    sint32 bank;
    if (!buffer)
    {
#ifndef NDEBUG
        fprintf(stderr, "Invalid decoded PCM buffer %08X/%u\n", destination, bytes);
#endif
        abort();
    }
    bank = SpuReadDecodedData(buffer, (sint32)flags);
    if (bank < 0)
    {
#ifndef NDEBUG
        fprintf(stderr, "Native decoded PCM read failed\n");
#endif
        abort();
    }
    return bank;
}

static sint32 game_overlay_resource001_matches(void)
{
    uint32 index;
    for (index = 0; index < sizeof(resource001_code_spans) / sizeof(resource001_code_spans[0]); ++index)
    {
        const void *code = psx_addr(resource001_code_spans[index].address, resource001_code_spans[index].bytes);
        if (!code || memcmp(code, resource001_code_bytes + resource001_code_spans[index].offset, resource001_code_spans[index].bytes))
            return 0;
    }
    return 1;
}

static sint32 game_overlay_resource002_matches(void)
{
    uint32 index;
    for (index = 0; index < sizeof(resource002_code_spans) / sizeof(resource002_code_spans[0]); ++index)
    {
        const void *code = psx_addr(resource002_code_spans[index].address, resource002_code_spans[index].bytes);
        if (!code || memcmp(code, resource002_code_bytes + resource002_code_spans[index].offset, resource002_code_spans[index].bytes))
            return 0;
    }
    return 1;
}

void game_overlay_load_begin(uint32 resource, uint32 destination)
{
    overlay_active = 0;
    overlay_pending = 1;
    overlay_resource = resource;
    overlay_destination = destination;
}

void game_overlay_load_complete(void)
{
    if (!overlay_pending)
        return;
    overlay_pending = 0;
    if (overlay_resource == 1 && overlay_destination == 0x800CBFE8)
    {
        if (!game_overlay_resource001_matches())
        {
#ifndef NDEBUG
            fprintf(stderr, "Resource001 executable code identity differs\n");
#endif
            abort();
        }
        overlay_active = 1;
    }
    else if (overlay_resource == 2 && overlay_destination == 0x800CBFE8)
    {
        if (!game_overlay_resource002_matches())
        {
#ifndef NDEBUG
            fprintf(stderr, "Resource002 executable code identity differs\n");
#endif
            abort();
        }
        overlay_active = 2;
    }
}

uint32 game_overlay_call(uint32 target, uint32 argument_count, uint32 argument0, uint32 argument1, uint32 argument2)
{
    if (overlay_active == 1 && game_overlay_resource001_matches())
    {
        if (target == 0x800CCE98)
            return resource001_bootstrap();
        if (target == 0x800CD07C)
            return resource001_sub_800cd07c();
        if (target == 0x800CC5D8)
            return resource001_sub_800cc5d8();
        if (target == 0x800CC764)
            return resource001_sub_800cc764();
    }
    else if (overlay_active == 2 && game_overlay_resource002_matches())
    {
        if (target == 0x800F60F0)
            return resource002_sub_800f60f0();
        if (target == 0x800F6848)
            return resource002_sub_800f6848();
        if (target == 0x800FBC48)
            return resource002_sub_800fbc48();
        if (target == 0x800FBD74)
            return resource002_sub_800fbd74();
        if (target == 0x800FBF70)
            return resource002_sub_800fbf70();
        if (target == 0x800FD634)
            return resource002_sub_800fd634();
        if (target == 0x800FD698)
            return resource002_sub_800fd698();
        if (target == 0x800CDCD4)
            return resource002_sub_800cdcd4();
        if (target == 0x800CDF40)
            return resource002_sub_800cdf40();
        if (target == 0x800FC750)
            return resource002_sub_800fc750();
    }
#ifndef NDEBUG
    fprintf(stderr, "Unbound guest callback %08X/%u resource=%u active=%u\n", target, argument_count, overlay_resource, overlay_active);
#endif
    abort();
}

static sint32 game_image_rectangle(uint32 address, const uint8 *native, PSX_RECT *rectangle)
{
    const void *source = native ? native : psx_addr(address, sizeof(*rectangle));
    if (!source)
        return 0;
    memcpy(rectangle, source, sizeof(*rectangle));
    return 1;
}

uint32 sub_800a8440(uint32 rectangle, uint32 pixels, uint8 *native_rectangle, const uint8 *native_pixels, uint32 native_capacity)
{
    PSX_RECT bounds;
    uint32 bytes;
    uint32 *source;
    if (!game_image_rectangle(rectangle, native_rectangle, &bounds) || bounds.w < 0 || bounds.h < 0)
        return 0xFFFFFFFF;
    bytes = (uint32)bounds.w * bounds.h * 2;
    if (native_pixels && bytes > native_capacity)
        return 0xFFFFFFFF;
    source = native_pixels ? (uint32 *)native_pixels : psx_addr(pixels, bytes);
    return LoadImagePSX(&bounds, source);
}

uint32 sub_800a84a0(uint32 rectangle, uint32 pixels, uint8 *native_rectangle, uint8 *native_pixels, uint32 native_capacity)
{
    PSX_RECT bounds;
    uint32 bytes;
    uint32 *destination;
    if (!game_image_rectangle(rectangle, native_rectangle, &bounds) || bounds.w < 0 || bounds.h < 0)
        return 0xFFFFFFFF;
    bytes = (uint32)bounds.w * bounds.h * 2;
    if (native_pixels && bytes > native_capacity)
        return 0xFFFFFFFF;
    destination = native_pixels ? (uint32 *)native_pixels : psx_addr(pixels, bytes);
    return StoreImage(&bounds, destination);
}

uint32 sub_800a8318(uint32 rectangle, uint32 red, uint32 green, uint32 blue, uint8 *native_rectangle)
{
    PSX_RECT bounds;
    if (!game_image_rectangle(rectangle, native_rectangle, &bounds))
        return 0xFFFFFFFF;
    return ClearImage(&bounds, red, green, blue);
}

uint32 sub_800a8500(uint32 rectangle, uint32 x, uint32 y, uint8 *native_rectangle)
{
    PSX_RECT bounds;
    if (!game_image_rectangle(rectangle, native_rectangle, &bounds))
        return 0xFFFFFFFF;
    return MoveImage(&bounds, x, y);
}

const char *game_native_string(uint32 address)
{
    const char *text, *end;
    uintptr_t offset;
    if (!address)
        return "(null)";
    text = psx_addr(address, 1);
    if (!text || (uintptr_t)text < (uintptr_t)DRAM || (uintptr_t)text - (uintptr_t)DRAM >= sizeof(DRAM))
        abort();
    offset = (uintptr_t)text - (uintptr_t)DRAM;
    end = memchr(text, 0, sizeof(DRAM) - offset);
    if (!end)
        abort();
    return psx_addr(address, end - text + 1);
}

static const uint8 game_cd_empty_response[8];

static void game_cd_resource_ready(uint8 event, uint8 *response)
{
    resource_sector_ready(event, response ? response : game_cd_empty_response, NULL);
}

static void game_cd_audio_ready(uint8 event, uint8 *response)
{
    cd_audio_ready(event, response ? response : game_cd_empty_response);
}

static void game_cd_resource_sync(uint8 event, uint8 *response)
{
    resource_sync_callback(event, response ? response : game_cd_empty_response);
}

typedef struct
{
    uint32 target;
    CdlCB callback;
} GAME_CD_CALLBACK;

static const GAME_CD_CALLBACK game_cd_callbacks[] = {{0, NULL}, {0x80029A44, game_cd_resource_ready}, {0x80029D70, game_cd_audio_ready}, {0x8002A200, game_cd_resource_sync}};

static CdlCB game_cd_callback_native(uint32 target)
{
    uint32 index;
    for (index = 0; index < sizeof(game_cd_callbacks) / sizeof(game_cd_callbacks[0]); ++index)
        if (game_cd_callbacks[index].target == target)
            return game_cd_callbacks[index].callback;
#ifndef NDEBUG
    fprintf(stderr, "Unbound native CD callback %08X\n", target);
#endif
    abort();
}

static uint32 game_cd_callback_address(CdlCB callback)
{
    uint32 index;
    for (index = 0; index < sizeof(game_cd_callbacks) / sizeof(game_cd_callbacks[0]); ++index)
        if (game_cd_callbacks[index].callback == callback)
            return game_cd_callbacks[index].target;
#ifndef NDEBUG
    fprintf(stderr, "Unbound previous native CD callback\n");
#endif
    abort();
}

uint32 game_cd_ready_callback(uint32 target)
{
    uint32 context = r_u32(0x800BCBAC);
    uint32 flags = r_u32(context);
    cd_stream_consumer_native(target == 0x80029D70 && (!(flags & 0x400) || (flags & 0x4000)));
    return game_cd_callback_address(CdReadyCallback(game_cd_callback_native(target)));
}

uint32 game_cd_sync_callback(uint32 target)
{
    return game_cd_callback_address(CdSyncCallback(game_cd_callback_native(target)));
}

static uint32 game_cd_data_notification_pending;

void game_cd_cancel_data_notification(void)
{
    game_cd_data_notification_pending = 0;
}

static void game_cd_data_service(void)
{
    PsxInterruptCallback callback;
    if (!game_cd_data_notification_pending)
        return;
    game_cd_data_notification_pending = 0;
    callback = psx_dma_callback_native(3);
    if (callback)
        callback();
}

void game_cd_poll_stream(void)
{
    uint8 sector[2352], position[8], response[8] = {0};
    uint32 stream, flags, cursor;
    sint32 available;
    for (;;)
    {
        stream = r_u32(0x800BCBAC);
        flags = r_u32(stream);
        if (r_u32(stream + 0xC) != 0x80029FE4 || r_u32(stream + 32) != 64 || (flags & 0xC0000080) != 0x80000000 || ((flags & 0x400) && !(flags & 0x4000)) || game_cd_data_notification_pending || r_u32(0x800BCE80))
            return;
        // Preserve the decoder read head while installing its queued payload
        xport_audio_lock();
        available = cd_stream_pop_sector_native(sector);
        if (available > 0)
        {
            if (!CdControlBPSX(17, NULL, position))
                abort();
            cursor = (uint32)CdPosToInt((CdlLOC *)(position + 5)) + 1;
            if (!cd_restore_fifo_native(sector + 12, 2340, 0xE8, cursor))
                abort();
        }
        response[0] = (uint8)CdStatus();
        xport_audio_unlock();
        if (!available)
            return;
        cd_audio_ready(available > 0 ? CdlDataReady : CdlDiskError, response);
        if (available < 0)
            return;
    }
}

uint32 game_cd_copy_sector(uint32 destination, uint32 words)
{
    void *buffer;
    if (game_cd_data_notification_pending || words > 585)
        return 0;
    buffer = psx_addr(destination, words * 4);
    if (!buffer || !CdGetSector(buffer, (sint32)words))
        return 0;
    game_cd_data_notification_pending = 1;
    psx_wait_signal_service(6);
    return 1;
}

uint32 game_cd_read_resource_sector_sync(void)
{
    uint32 sector[585];
    uint8 response[8] = {0};
    sint32 accepted, status;

    accepted = CdRead(1, sector, 0xA0);
    status = CdReadSync(1, response);
    if (!accepted || status < 0)
        return resource_sector_ready(CdlDiskError, response, NULL);
    return resource_sector_ready(CdlDataReady, response, (const uint8 *)sector);
}

void game_bind_sdk(void)
{
    static const PSX_BIOS_FILE_GUEST_BINDING files = {0x8648, 0x8640, 0x6DD4};
    static const PsxCardSdkBinding card = {0x800BCB34, 0x800CBC78, 0x800AD5AC, 0x800AD61C, 0x800AD630, 0x800AD490, 0x800AD498, 0x800AD4A0, 0x800AD58C};
    static const PsxCardFormatGuestBinding card_format = {0x800CBC98, 0x800CBE78, 0x800CBED8};
    psx_native_bus_bind();
    game_cd_cancel_data_notification();
    psx_wait_bind_service(6, game_cd_data_service);
    if (!psx_bios_bind_file_guest(&files))
        abort();
    psx_bios_bind_card_sdk(&card);
    psx_card_bind_format_guest(&card_format);
    psx_bios_bind_card_stop(sub_800ad788, 0x800AD528);
    psx_bind_guest_dispatch(game_dispatch);
}

uint32 game_random(void)
{
    uint32 seed = r_u32(0x800CBF58);
    uint32 result = (uint32)psyq_rand_next(&seed);
    w_u32(0x800CBF58, seed);
    return result;
}

uint32 xport_guest_call_position2(uint32 target, uint32 position, uint32 value, const uint32 *native_position)
{
    switch (target)
    {
        case 0x80083554:
            return sub_80083554(position, value, native_position);
        case 0x80083764:
            return sub_80083764(position, native_position);
        case 0x80083844:
            return sub_80083844(position, value, native_position);
        case 0x80083A40:
            return sub_80083a40(position, value, native_position);
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound position callback %08X\n", target);
#endif
            abort();
    }
}

uint32 xport_guest_call_vectors3(uint32 target, uint32 position, uint32 vector, uint32 kind, const uint32 *native_position, const uint32 *native_vector)
{
    switch (target)
    {
        case 0x8008136C:
            return sub_8008136c(position, vector, native_position, native_vector);
        case 0x8008158C:
            return sub_8008158c(position, vector, native_position, native_vector);
        case 0x80081760:
            return sub_80081760(position, vector, native_position, native_vector);
        case 0x80081994:
            return sub_80081994(position, vector, native_position, native_vector);
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound vector callback %08X\n", target);
#endif
            abort();
    }
}

uint32 xport_guest_call_camera3(uint32 target, const uint32 *native_context, uint32 control, uint32 duration)
{
    switch (target)
    {
        case 0x8007A0F0:
            return sub_8007a0f0(0, control, duration, native_context);
        case 0x8007A260:
            return sub_8007a260(0, control, duration, native_context);
        case 0x8007A3B4:
            return sub_8007a3b4(0, control, duration, native_context);
        case 0x8007A52C:
            return sub_8007a52c(0, control, duration, native_context);
        case 0x8007A6CC:
            return sub_8007a6cc(0, control, duration, native_context);
        case 0x8007A8C4:
            return sub_8007a8c4(0, control, duration, native_context);
        case 0x8007AAB4:
            return sub_8007aab4(0, control, duration, native_context);
        case 0x8007AC50:
            return sub_8007ac50(0, control, duration, native_context);
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound camera callback %08X\n", target);
#endif
            abort();
    }
}

uint32 xport_decompress_guest_call(uint32 target, uint32 source, uint32 destination, uint32 distance_bits)
{
    switch (target)
    {
        case 0x800AFD40:
            return sub_800afd40(source, destination, distance_bits);
        case 0x800AFDE4:
            return sub_800afde4(source, destination, distance_bits);
        case 0x800AFE90:
            return sub_800afe90(source, destination, distance_bits);
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound decompression target %08X\n", target);
#endif
            abort();
    }
}

uint32 game_bios_open_guest(uint32 filename, uint32 flags, uint32 capacity)
{
    char path[26];
    uint32 index, value;

    if (capacity == 0 || capacity > sizeof(path))
    {
#ifndef NDEBUG
        fprintf(stderr, "Unsupported BIOS filename capacity %u\n", capacity);
#endif
        abort();
    }
    for (index = 0; index < capacity; ++index)
    {
        value = r_u8(filename + index);
        path[index] = (char)value;
        if (value == 0)
            return (uint32)openPSX(path, flags);
    }
#ifndef NDEBUG
    fprintf(stderr, "Unterminated BIOS filename %08X/%u\n", filename, capacity);
#endif
    abort();
}

uint32 game_bios_close_guest(uint32 descriptor)
{
    return (uint32)closePSX((sint32)descriptor);
}

uint32 game_bios_write_guest(uint32 descriptor, uint32 source, uint32 bytes)
{
    const void *data = psx_addr(source, bytes);
    if (!data && bytes != 0)
    {
#ifndef NDEBUG
        fprintf(stderr, "Unmapped BIOS write buffer %08X/%u\n", source, bytes);
#endif
        abort();
    }
    return (uint32)writePSX((sint32)descriptor, data, bytes);
}

uint32 game_bios_read_guest(uint32 descriptor, uint32 destination, uint32 bytes)
{
    void *data = psx_addr(destination, bytes);
    if (!data && bytes != 0)
    {
#ifndef NDEBUG
        fprintf(stderr, "Unmapped BIOS read buffer %08X/%u\n", destination, bytes);
#endif
        abort();
    }
    return (uint32)readPSX((sint32)descriptor, data, bytes);
}

uint32 game_bios_seek_guest(uint32 descriptor, sint32 offset, uint32 origin)
{
    return (uint32)lseekPSX((sint32)descriptor, offset, origin);
}

void xport_guest_call1(uint32 target, uint32 argument0)
{
    switch (target)
    {
        case 0x8006903C:
            sub_8006903c(argument0);
            return;
        case 0x80069158:
            sub_80069158(argument0);
            return;
        case 0x80069D18:
            sub_80069d18(argument0);
            return;
        case 0x80069F48:
            sub_80069f48(argument0);
            return;
        case 0x80069D6C:
            sub_80069d6c(argument0);
            return;
        case 0x800694CC:
            sub_800694cc(argument0);
            return;
        case 0x800692C4:
            sub_800692c4(argument0);
            return;
        case 0x800696D4:
            sub_800696d4(argument0);
            return;
        case 0x80069900:
            sub_80069900(argument0);
            return;
        case 0x80069A9C:
            sub_80069a9c(argument0);
            return;
        case 0x80069C8C:
            sub_80069c8c(argument0);
            return;
        case 0x80069D88:
            sub_80069d88(argument0);
            return;
        case 0x80069E10:
            sub_80069e10(argument0);
            return;
        case 0x80069EC0:
            sub_80069ec0(argument0);
            return;
        case 0x80069FC0:
            sub_80069fc0();
            return;
        case 0x800A9E90:
            DrawOTag(psx_addr(argument0, 4));
            return;
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unbound one-argument callback %08X\n", target);
#endif
            abort();
    }
}
