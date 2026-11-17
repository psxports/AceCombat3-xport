#include <stdlib.h>
#include <string.h>

#include "game_functions.h"

sint32 game_spu_read_decoded_native(void *destination, uint32 capacity, uint32 flags)
{
    uint32 bytes = flags == 5 ? 2048 : 4096;
    sint32 bank;
    if (!destination || capacity < bytes)
    {
#ifndef NDEBUG
        fprintf(stderr, "Invalid decoded PCM buffer %p/%u\n", destination, capacity);
#endif
        abort();
    }
    bank = SpuReadDecodedData(destination, (sint32)flags);
    if (bank < 0)
    {
#ifndef NDEBUG
        fprintf(stderr, "Native decoded PCM read failed\n");
#endif
        abort();
    }
    return bank;
}













uint32 game_upload_image(PSX_RECT rectangle, const void *pixels, size_t capacity)
{
    if (rectangle.w < 0 || rectangle.h < 0 ||
        (size_t)rectangle.w * rectangle.h * 2 > capacity)
        return 0xFFFFFFFF;
    return LoadImagePSX(&rectangle, (uint32 *)pixels);
}

uint32 game_capture_image(PSX_RECT rectangle, void *pixels, size_t capacity)
{
    if (rectangle.w < 0 || rectangle.h < 0 ||
        (size_t)rectangle.w * rectangle.h * 2 > capacity)
        return 0xFFFFFFFF;
    return StoreImage(&rectangle, (uint32 *)pixels);
}

static const uint8 game_cd_empty_response[8];

void game_cd_resource_ready(uint8 event, uint8 *response)
{
    resource_sector_ready(event, response ? response : game_cd_empty_response, NULL);
}

void game_cd_audio_ready(uint8 event, uint8 *response)
{
    cd_audio_ready(event, response ? response : game_cd_empty_response);
}

void game_cd_resource_sync(uint8 event, uint8 *response)
{
    resource_sync_callback(event, response ? response : game_cd_empty_response);
}

CdlCB game_cd_ready_callback(CdlCB callback)
{
    uint32 flags = resource_stream_status().flags;
    cd_stream_consumer_native(callback == game_cd_audio_ready && (!(flags & 0x400) || (flags & 0x4000)));
    return CdReadyCallback(callback);
}

static uint32 game_cd_data_notification_pending;

void game_cd_cancel_data_notification(void)
{
    game_cd_data_notification_pending = 0;
}

static void game_cd_data_service(void)
{
    if (!game_cd_data_notification_pending)
        return;
    game_cd_data_notification_pending = 0;
}

void game_cd_poll_stream(void)
{
    uint8 sector[2352], position[8], response[8] = {0};
    uint32 cursor;
    sint32 available;
    for (;;)
    {
        if (!resource_stream_accepts_audio_sector() || game_cd_data_notification_pending)
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

void game_cd_copy_sector(void *destination, uint32 words)
{
    void *buffer;
    if (game_cd_data_notification_pending || words > 585)
        return;
    buffer = destination;
    if (!buffer || !CdGetSector(buffer, (sint32)words))
        return;
    game_cd_data_notification_pending = 1;
    psx_wait_signal_service(6);
    return;
}

uint32 game_cd_publish_metadata(const RESOURCE_METADATA_RECORD *payload)
{
    RESOURCE_METADATA_RECORD record;
    uint32 channel, word;
    if (game_cd_data_notification_pending)
        return 0;
    record = *payload;
    resource_metadata.channels = record.channels;
    for (channel = 0; channel < 32; ++channel)
    {
        RESOURCE_FRAME *frame = resource_metadata.frames + channel;
        const RESOURCE_FRAME_RECORD *source = record.frames + channel;
        word = source->kind_auxiliary;
        frame->kind = (uint16)word;
        frame->auxiliary = (sint16)(word >> 16);
        frame->frame = source->frame;
        frame->extra[0] = source->extra[0];
        frame->extra[1] = source->extra[1];
    }
    game_cd_data_notification_pending = 1;
    psx_wait_signal_service(6);
    return 1;
}

uint32 game_cd_read_resource_sector_sync(void)
{
    RESOURCE_CD_SECTOR sector;
    uint8 response[8] = {0};
    sint32 accepted, status;

    accepted = CdRead(1, sector.words, 0xA0);
    status = CdReadSync(1, response);
    if (!accepted || status < 0)
        return resource_sector_ready(CdlDiskError, response, NULL);
    return resource_sector_ready(CdlDataReady, response, &sector);
}

#if 0
/* original card patch bytes consumed by the shared BIOS binding */
static const uint8 card_patch_bytes[] =
{
    0x0a, 0x00, 0x6f, 0x94, 0x00, 0x00, 0x08, 0x3c,
    0x25, 0xc0, 0xe2, 0x01, 0x12, 0x00, 0x19, 0x37,
    0x0a, 0x00, 0x79, 0xa4, 0x28, 0x00, 0x08, 0x24,
    0xff, 0xff, 0x08, 0x25, 0xfe, 0xff, 0x00, 0x15,
    0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0xe0, 0x03,
    0x00, 0x00, 0x00, 0x00, 0x74, 0x10, 0x62, 0x8c,
    0x00, 0x00, 0x00, 0x00, 0x80, 0x00, 0x42, 0x30,
    0x0b, 0x00, 0x40, 0x10, 0x00, 0x00, 0x00, 0x00,
    0x44, 0x10, 0x62, 0x8c, 0x00, 0x00, 0x00, 0x00,
    0x80, 0x00, 0x42, 0x30, 0xfc, 0xff, 0x40, 0x14,
    0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0x02, 0x3c,
    0xfc, 0xdf, 0x42, 0x8c, 0x00, 0x00, 0x00, 0x00,
    0x08, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x08, 0x00, 0xe0, 0x03, 0x00, 0x00, 0x00, 0x00,
    0x01, 0xa0, 0x02, 0x3c, 0xac, 0xdf, 0x42, 0x24,
    0x08, 0x00, 0x40, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x01, 0xa0, 0x08, 0x3c,
    0x80, 0xdf, 0x08, 0x25, 0x09, 0xf8, 0x00, 0x01,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};
#endif

void game_bind_sdk(void)
{
#if 0
    static const PSX_BIOS_FILE_GUEST_BINDING files = {0x8648, 0x8640, 0x6DD4};
    static const PsxCardSdkBinding card = {0x800BCB34, 0x800CBC78, 0x800AD5AC, 0x800AD61C, 0x800AD630, 0x800AD490, 0x800AD498, 0x800AD4A0, 0x800AD58C};
    static const PsxCardFormatGuestBinding card_format = {0x800CBC98, 0x800CBE78, 0x800CBED8};
#endif
    psx_native_bus_bind();
    game_cd_cancel_data_notification();
    psx_wait_bind_service(6, game_cd_data_service);
#if 0
    if (!psx_bios_bind_file_guest(&files))
        abort();
    uint32 *init_pad_flag = psx_addr(card.init_pad_flag, sizeof(*init_pad_flag));
    *init_pad_flag = 0;
    memcpy(psx_addr(card.copy_source, sizeof(card_patch_bytes)), card_patch_bytes, sizeof(card_patch_bytes));
    psx_bios_bind_card_sdk(&card);
    psx_card_bind_format_guest(&card_format);
#endif
}





void game_bios_close_guest(uint32 descriptor)
{
    { (void)(uint32)((uint32)closePSX((sint32)descriptor)); return; }
}

void game_bios_seek_guest(uint32 descriptor, sint32 offset, uint32 origin)
{
    { (void)(uint32)((uint32)lseekPSX((sint32)descriptor, offset, origin)); return; }
}


void bios_exit_critical(void)
{
    FUNCTION_MARKER(0x800AF3C8, "ACE3.BIN");
    xport_bios_exit_critical();
}

sint32 bios_enter_critical(void)
{
    FUNCTION_MARKER(0x800AF3B8, "ACE3.BIN");
    return xport_bios_enter_critical();
}

uint32 bios_init_card(uint32 mode)
{
    FUNCTION_MARKER(0x800AD538, "ACE3.BIN");
    return xport_bios_init_card(mode);
}

uint32 bios_start_card(void)
{
    FUNCTION_MARKER(0x800AD548, "ACE3.BIN");
    return xport_bios_start_card();
}

void geom_set_offset(uint32 x, uint32 y)
{
    FUNCTION_MARKER(0x800A7118, "ACE3.BIN");
    xport_gte_write_control(24, x << 16);
    xport_gte_write_control(25, y << 16);
}

void geom_set_screen(uint32 distance)
{
    FUNCTION_MARKER(0x800A7138, "ACE3.BIN");
    xport_gte_write_control(26, distance);
}

void cd_get_sector(void *dst, uint32 words)
{
    FUNCTION_MARKER(0x800A3430, "ACE3.BIN");
    { (void)(uint32)(CdGetSector(dst, (sint32)words)); return; }
}

void pad_start(void)
{
    FUNCTION_MARKER(0x8009BC60, "ACE3.BIN");
    PadStartCom();
}

void cd_apply_volume(CdlATV *volume)
{
    FUNCTION_MARKER(0x800A3410, "ACE3.BIN");
    CD_vol(volume);
    return;
}


uint32 geom_get_screen(void)
{
    FUNCTION_MARKER(0x800A70E8, "ACE3.BIN");
    return xport_gte_read_control(26);
}
