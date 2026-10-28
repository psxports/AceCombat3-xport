#include "game_functions.h"
#include <stdlib.h>
#include <string.h>

#ifndef XPORT_EXTRACTED_ASSETS
    #define XPORT_EXTRACTED_ASSETS 1
#endif

sint32 game_use_extracted_assets(void)
{
#if AP_WEB || (AP_WIN && XPORT_EXTRACTED_ASSETS)
    return 1;
#else
    return 0;
#endif
}

static uint32 game_data_disc;
static uint32 game_music_active, game_music_done, game_music_callback;
static FILE *game_music_metadata;
static uint8 game_music_record[520];
static uint32 game_music_record_ready, game_music_start, game_music_end;
static uint32 game_music_metadata_remaining;
static FILE *game_stream_data;
static uint8 game_stream_record[2352];
static uint32 game_stream_record_ready;
static uint32 game_stream_remaining;
static FILE *game_data_open(uint32 disc, const char *name);

static long game_file_length(FILE *file)
{
    long length;
    if (fseek(file, 0, SEEK_END) || (length = ftell(file)) < 0 || fseek(file, 0, SEEK_SET))
        return -1;
    return length;
}

static void game_select_resource_channel(uint32 entry)
{
    uint32 stream = r_u32(0x800BCBAC);
    w_u8(stream + 29, (r_u32(entry) >> 19) & 31);
    w_u32(r_u32(0x800BCBB0), 0);
}

#if AP_WEB
static uint8 *game_movie_rgb, *game_movie_pixels;
static XportFmvFrame game_movie_frame;
static uint32 game_movie_pending, game_movie_overlay, game_movie_active, game_movie_resource, game_movie_done;
static uint32 game_movie_owner;
static FILE *game_movie_metadata;
static uint32 game_movie_metadata_mode;
static uint8 game_movie_metadata_record[520];
static uint32 game_movie_metadata_remaining, game_movie_metadata_record_ready;

uint32 game_web_movie_matches(uint32 id)
{
    return game_movie_active && (id & 0xFFF) == (game_movie_resource & 0xFFF);
}

uint32 game_web_movie_request(uint32 request)
{
    uint32 context = r_u32(0x800BCBBC);
    if (!r_u8(request + 1))
    {
        uint32 flags = r_u32(request + 12);
        if (!(r_u16(request + 2) & 0x1000))
            flags &= ~0x1000u;
        game_web_movie_open(game_movie_resource, game_movie_owner, game_movie_overlay, flags);
        w_u32(context + 0x174, (uint32)r_s16(request + 2));
        w_u32(context + 0x168, 0);
        w_u8(request + 1, 1);
    }
    if (!game_movie_done)
        return 0;
    resource_read_complete(0);
    w_u32(context + 0x174, 0xFFFFFFFF);
    return 1;
}

void game_web_movie_close(void)
{
    xport_fmv_close();
    free(game_movie_rgb);
    free(game_movie_pixels);
    game_movie_rgb = game_movie_pixels = NULL;
    game_movie_pending = 0;
    game_movie_active = game_movie_done = 0;
    if (game_movie_metadata)
        fclose(game_movie_metadata);
    game_movie_metadata = NULL;
    game_movie_metadata_mode = game_movie_metadata_remaining = game_movie_metadata_record_ready = 0;
}

void game_web_movie_prepare(uint32 resource, uint32 owner, uint32 overlay)
{
    game_web_movie_close();
    game_movie_resource = resource;
    game_movie_owner = owner;
    game_movie_overlay = overlay;
    game_movie_active = 1;
}

uint32 game_web_movie_open(uint32 resource, uint32 owner, uint32 overlay, uint32 flags)
{
    char path[64];
    long length;
    game_web_movie_close();
    game_stop_resource_music();
    resource_stop_cd(0);
    game_select_resource_channel(resource_get_entry(resource));
    snprintf(path, sizeof(path), "DATA/%u/VIDEO/%u.STR", game_data_disc, resource & 0x7FF);
    game_movie_rgb = malloc(640 * 256 * 3);
    game_movie_pixels = malloc(640 * 256 * 3);
    if (!game_movie_rgb || !game_movie_pixels || xport_fmv_open(path) <= 0)
    {
        game_web_movie_close();
        xport_message_error("Ace Combat 3 - Movie error", path);
        abort();
    }
    game_movie_overlay = overlay;
    game_movie_active = 1;
    game_movie_resource = resource;
    game_movie_metadata_mode = flags & 0x1000 ? 2 : flags & 0x4000 ? 1 : 0;
    if (game_movie_metadata_mode)
    {
        snprintf(path, sizeof(path), "%s/%u", game_movie_metadata_mode == 2 ? "ACE_META_AUX" : "ACE_META", resource & 0x7FF);
        game_movie_metadata = game_data_open(game_data_disc, path);
        if (!game_movie_metadata)
            abort();
        length = game_file_length(game_movie_metadata);
        if (length < 0 || length % (game_movie_metadata_mode == 2 ? 520 : 20))
            abort();
        game_movie_metadata_remaining = (uint32)length;
    }
    sub_8002999c(0);
    w_u8(owner + 4, 0);
    w_u8(owner + 3, 1);
    return 1;
}

static uint32 game_web_movie_metadata_ready(void)
{
    if (game_movie_metadata_mode != 2)
        return 1;
    while (game_movie_metadata)
    {
        if (!game_movie_metadata_record_ready)
        {
            if (!game_movie_metadata_remaining)
            {
                fclose(game_movie_metadata);
                game_movie_metadata = NULL;
                break;
            }
            if (xport_fread(game_movie_metadata_record, 1, 520, game_movie_metadata) != 520)
                abort();
            game_movie_metadata_remaining -= 520;
            game_movie_metadata_record_ready = 1;
        }
        if (game_movie_pending != 2 && xport_load_le32(game_movie_metadata_record) > game_movie_frame.number)
            break;
        if (!game_cd_publish_metadata(game_movie_metadata_record + 4))
            return 0;
        game_movie_metadata_record_ready = 0;
    }
    return 1;
}

uint32 game_web_movie_poll(uint32 owner)
{
    sint32 result;
    if (!game_movie_rgb)
        return 0;
    if (game_movie_pending)
        goto metadata;
    result = xport_fmv_next(game_movie_rgb, 640 * 256 * 3, &game_movie_frame);
    if (result < 0)
    {
        game_web_movie_close();
        xport_message_error("Ace Combat 3 - Movie error", "WebCodecs movie decoding failed");
        abort();
    }
    if (!result)
        return 0;
    game_movie_pending = (uint32)result;
    if (result == 1)
    {
        uint32 coordinates = game_movie_overlay ? 56 : 24;
        uint32 dimensions = game_movie_overlay ? 76 : 44;
        uint32 width = r_u8(owner + 5) ? game_movie_frame.width * 3 / 2 : game_movie_frame.width;
        w_u16(owner + coordinates + 4, width);
        w_u16(owner + coordinates + 6, game_movie_frame.height);
        w_u16(owner + coordinates + 12, width);
        w_u16(owner + coordinates + 14, game_movie_frame.height);
        w_u16(owner + dimensions + 2, game_movie_frame.height);
        if (game_movie_metadata_mode == 1)
        {
            uint8 record[20];
            uint32 entry = resource_get_entry(game_movie_resource);
            uint32 channel = (r_u32(entry) >> 19) & 31;
            uint32 table = r_u32(0x800BCBB0);
            if (!game_movie_frame.number || fseek(game_movie_metadata, (long)((game_movie_frame.number - 1) * 20), SEEK_SET) ||
                xport_fread(record, 1, 20, game_movie_metadata) != 20 || xport_load_le32(record) != game_movie_frame.number)
                abort();
            w_u32(table, r_u32(table) | ((uint32)1 << channel));
            memcpy(psx_addr(table + 4 + channel * 16, 16), record + 4, 16);
        }
    }
metadata:
    if (!game_web_movie_metadata_ready())
        return 0;
    if (game_movie_pending == 2)
        game_movie_done = 1;
    return 1;
}

uint32 game_web_movie_decode(uint32 owner)
{
    PSX_RECT rectangle;
    uint32 page, coordinates, dimensions, index, count, bytes;
    if (r_u8(owner + 4))
        return 1;
    if (!game_web_movie_poll(owner))
        return 0;
    if (game_movie_pending == 2)
        return 0;
    page = r_u8(owner + 2);
    coordinates = game_movie_overlay ? 56 : 24;
    dimensions = game_movie_overlay ? 76 : 44;
    rectangle.x = r_s16(owner + coordinates + page * 8);
    rectangle.y = r_s16(owner + coordinates + page * 8 + 2);
    rectangle.h = (sint16)game_movie_frame.height;
    count = game_movie_frame.width * game_movie_frame.height;
    if (r_u8(owner + 5))
    {
        rectangle.w = (sint16)(game_movie_frame.width * 3 / 2);
        bytes = count * 3;
        memcpy(game_movie_pixels, game_movie_rgb, bytes);
    }
    else
    {
        rectangle.w = (sint16)game_movie_frame.width;
        for (index = 0; index < count; ++index)
            xport_store_le16(game_movie_pixels + index * 2, (game_movie_rgb[index * 3] >> 3) |
                ((game_movie_rgb[index * 3 + 1] >> 3) << 5) | ((game_movie_rgb[index * 3 + 2] >> 3) << 10));
    }
    if (LoadImagePSX(&rectangle, (uint32 *)game_movie_pixels) < 0)
        abort();
    w_u16(owner + coordinates + 4, rectangle.w);
    w_u16(owner + coordinates + 6, rectangle.h);
    w_u16(owner + coordinates + 12, rectangle.w);
    w_u16(owner + coordinates + 14, rectangle.h);
    w_u16(owner + dimensions, r_u8(owner + 5) ? 24 : 16);
    w_u16(owner + dimensions + 2, rectangle.h);
    w_u8(owner + 2, page ^ 1);
    w_u8(owner + 3, 1);
    game_movie_pending = 0;
    return 0;
}
#endif

static void game_music_ready(uint8 event, uint8 *response)
{
    (void)response;
    if (event == CdlDataEnd)
        game_music_done = 1;
}

sint32 game_resource_music_active(void)
{
    return game_music_active != 0;
}

sint32 game_start_resource_music(uint32 start, uint32 callback, uint32 flags, uint32 data_stream)
{
    uint32 context = r_u32(0x800BCBBC);
    uint32 id = r_u32(context + 0x174);
    uint32 entry = resource_get_entry(id);
    uint32 stream = r_u32(0x800BCBAC);
    uint32 offset;
    uint32 sectors;
    long length;
    char path[64];
    if (!(id & 0x800) || start < r_u32(entry + 4))
        return 0;
    offset = start - r_u32(entry + 4);
    sectors = (r_u32(entry) & 0x7FFFF) << ((r_u32(entry) >> 24) & 7);
    if (offset >= sectors)
        abort();
    game_select_resource_channel(entry);
    if ((id & 0x7FF) >= 36 && (id & 0x7FF) <= 70 && offset)
        snprintf(path, sizeof(path), "MUSIC/%u_%u_%u.XA", game_data_disc, id & 0x7FF, offset);
    else if ((id & 0x7FF) >= 36 && (id & 0x7FF) <= 70)
        snprintf(path, sizeof(path), "MUSIC/%u_%u.XA", game_data_disc, id & 0x7FF);
    else
        snprintf(path, sizeof(path), "DATA/%u/ACE_XA/%u", game_data_disc, id & 0x7FF);
    game_cd_ready_callback(0);
    game_cd_sync_callback(0);
    CdReadyCallback(game_music_ready);
    if (!xport_xa_play_file(path, 0))
    {
        CdReadyCallback(NULL);
        xport_message_error("Ace Combat 3 - Music error", path);
        abort();
    }
    game_music_active = 1;
    game_music_done = 0;
    game_music_callback = callback;
    game_music_start = start;
    game_music_end = r_u32(entry + 4) + sectors - 1;
    game_music_record_ready = 0;
    if (data_stream)
    {
        snprintf(path, sizeof(path), "ACE_STREAM/%u", id & 0x7FF);
        game_stream_data = game_data_open(game_data_disc, path);
        if (!game_stream_data)
            abort();
        length = game_file_length(game_stream_data);
        if (length < 0 || length % sizeof(game_stream_record))
            abort();
        game_stream_remaining = (uint32)length;
        game_stream_record_ready = 0;
    }
    else if (flags & 0x1000)
    {
        snprintf(path, sizeof(path), "ACE_META/%u", id & 0x7FF);
        game_music_metadata = game_data_open(game_data_disc, path);
        if (!game_music_metadata)
            abort();
        length = game_file_length(game_music_metadata);
        if (length < 0 || length % sizeof(game_music_record))
            abort();
        game_music_metadata_remaining = (uint32)length;
    }
    w_u32(stream, data_stream ? r_u32(stream) | 0x80000010 : 0x80000410 | ((flags & 0x1000) ? 0x4000 : 0));
    w_u32(stream + 4, 0);
    w_u32(stream + 32, 64);
    w_u32(stream + 20, sectors - offset);
    w_u32(stream + 44, game_music_end);
    w_u32(stream + 64, 0);
    sub_8002999c(0);
    return 1;
}

void game_stop_resource_music(void)
{
    if (game_music_active)
    {
        CdReadyCallback(NULL);
        game_music_active = game_music_done = 0;
        if (game_music_metadata)
            fclose(game_music_metadata);
        game_music_metadata = NULL;
        if (game_stream_data)
            fclose(game_stream_data);
        game_stream_data = NULL;
        CdControl(CdlStop, NULL, NULL);
    }
}

void game_poll_resource_music(void)
{
    uint32 stream = r_u32(0x800BCBAC);
    uint32 frames, sector;
    CdlLOC position;
    uint32 callback;
    if (xport_xa_file_position(&position, &frames))
    {
        sector = (uint32)CdPosToInt(&position);
        w_u32(stream, (r_u32(stream) | 0x01000000) & ~0x10);
        w_u32(stream + 36, sector);
        w_u32(stream + 40, sector);
        w_u32(stream + 20, sector < game_music_end ? game_music_end - sector : 0);
        w_u32(stream + 64, (uint32)((uint64)frames * 60 / 44100));
        if (game_music_done)
            sector = game_music_end;
        while (game_stream_data)
        {
            size_t count;
            uint32 key, result;
            if (!game_stream_record_ready)
            {
                if (!game_stream_remaining)
                {
                    fclose(game_stream_data);
                    game_stream_data = NULL;
                    break;
                }
                count = xport_fread(game_stream_record, 1, sizeof(game_stream_record), game_stream_data);
                if (count != sizeof(game_stream_record) || (game_stream_record[18] & 14) != 8)
                    abort();
                game_stream_remaining -= sizeof(game_stream_record);
                game_stream_record_ready = 1;
            }
            memcpy(&position, game_stream_record + 12, 3);
            key = (uint32)CdPosToInt(&position);
            if (key > sector)
                break;
            if (key >= game_music_start)
            {
                uint32 state = r_u32(0x800BCBA8);
                if (r_u32(state + 0x24) && DecDCToutSync(1))
                    return;
                if (game_stream_record[17] == 0 && (r_u32(stream) & 0x4000))
                {
                    if (!game_cd_publish_metadata(game_stream_record + 24))
                        return;
                }
                else
                {
                    result = cd_stream_sector(game_stream_record + 24);
                    if (result == 2 || result == 4)
                        return;
                }
            }
            game_stream_record_ready = 0;
        }
        while (game_music_metadata)
        {
            size_t count;
            uint32 key;
            if (!game_music_record_ready)
            {
                if (!game_music_metadata_remaining)
                {
                    fclose(game_music_metadata);
                    game_music_metadata = NULL;
                    break;
                }
                count = xport_fread(game_music_record, 1, sizeof(game_music_record), game_music_metadata);
                if (count != sizeof(game_music_record))
                    abort();
                game_music_metadata_remaining -= sizeof(game_music_record);
                game_music_record_ready = 1;
            }
            key = xport_load_le32(game_music_record);
            if (key > sector)
                break;
            if (key >= game_music_start && !game_cd_publish_metadata(game_music_record + 4))
                return;
            game_music_record_ready = 0;
        }
    }
    if (!game_music_done)
        return;
    callback = game_music_callback;
    game_stop_resource_music();
    w_u32(stream, 0);
    w_u32(stream + 20, 0);
    sub_8002999c(1);
    if (callback)
        xport_guest_call3(callback, 0, 0, 0);
}

typedef struct
{
    const char *name;
    uint32 lba, logical_bytes, physical_bytes, sector_bytes;
} GAME_DATA_FILE;

typedef struct
{
    const GAME_DATA_FILE *files;
    uint32 count;
    uint32 disc;
    FILE *streams[7];
} GAME_DATA_SOURCE;

static const GAME_DATA_FILE game_disc_1[] = {
    {"ACE.BPB", 611, 94062592, 94062592, 2048}, {"ACE.BPH", 606, 4612, 4612, 2048}, {"ACE.SPB", 46540, 527171584, 605423616, 2352}, {"ACE.SPH", 609, 3916, 3916, 2048}, {"ACE3.BIN", 259, 710656, 710656, 2048}, {"SLPS_020.20", 24, 481280, 481280, 2048}, {"SYSTEM.CNF", 23, 67, 67, 2048},
};

static const GAME_DATA_FILE game_disc_2[] = {
    {"ACE.BPB", 611, 94212096, 94212096, 2048}, {"ACE.BPH", 606, 4612, 4612, 2048}, {"ACE.SPB", 46613, 533725184, 612950016, 2352}, {"ACE.SPH", 609, 3916, 3916, 2048}, {"ACE3.BIN", 259, 710656, 710656, 2048}, {"SLPS_020.21", 24, 481280, 481280, 2048}, {"SYSTEM.CNF", 23, 67, 67, 2048},
};

static void game_data_close(void *context)
{
    GAME_DATA_SOURCE *source = context;
    uint32 index;
    for (index = 0; index < source->count; ++index)
        if (source->streams[index])
            fclose(source->streams[index]);
    free(source);
}

static FILE *game_data_open(uint32 disc, const char *name)
{
    char path[80];
    sint32 length = snprintf(path, sizeof(path), "DATA/%u/%s", disc, name);
    if (length < 0 || (uint32)length >= sizeof(path))
        return NULL;
    return xport_fopen(path, "rb");
}

void game_read_archive_headers(uint32 bp, uint32 sp)
{
    const char *names[2] = {"ACE.BPH", "ACE.SPH"};
    uint32 sizes[2] = {4612, 3916};
    uint32 addresses[2] = {bp, sp};
    uint32 index;
    for (index = 0; index < 2; ++index)
    {
        FILE *file = game_data_open(game_data_disc, names[index]);
        uint8 *buffer = psx_addr(addresses[index], 8192);
        if (!file || !buffer || game_file_length(file) != (long)sizes[index] || xport_fread(buffer, 1, sizes[index], file) != sizes[index])
        {
            if (file)
                fclose(file);
            xport_message_error("Ace Combat 3 - Resource error", names[index]);
            abort();
        }
        memset(buffer + sizes[index], 0, 8192 - sizes[index]);
        fclose(file);
    }
}

sint32 game_read_resource_file(uint32 entry, uint32 destination, uint32 bytes)
{
    uint32 state = r_u32(0x800BCBA8);
    uint32 table = r_u32(state + 0x18);
    uint32 index;
    char name[48];
    FILE *file;
    void *buffer;
    long length;
    sint32 result;
    if (entry < table || (entry - table) % 8 || (entry - table) / 8 >= 574)
        return 0;
    index = (entry - table) / 8;
    snprintf(name, sizeof(name), "ACE_BPB/%u", index);
    file = game_data_open(game_data_disc, name);
    if (!file)
        return 0;
    buffer = psx_addr(destination, bytes);
    result = buffer && !fseek(file, 0, SEEK_END) && (length = ftell(file)) >= 0 &&
             (uint32)length == bytes && !fseek(file, 0, SEEK_SET) && xport_fread(buffer, 1, bytes, file) == bytes;
    fclose(file);
    return result;
}

static sint32 game_data_read(void *context, uint32 sector, uint8 buffer[2352], uint32 *sector_bytes)
{
    GAME_DATA_SOURCE *source = context;
    uint32 index, count;
    uint64 offset;
    for (index = 0; index < source->count; ++index)
    {
        const GAME_DATA_FILE *file = source->files + index;
        if (sector < file->lba)
            continue;
        offset = (uint64)(sector - file->lba) * file->sector_bytes;
        if (offset >= file->physical_bytes)
            continue;
        if (game_use_extracted_assets() && (index == 0 || index == 2))
            return 0;
        if (!source->streams[index])
            source->streams[index] = game_data_open(source->disc, file->name);
        if (!source->streams[index])
            return 0;
        if (offset > 0x7FFFFFFF || fseek(source->streams[index], (long)offset, SEEK_SET))
            return 0;
        count = file->physical_bytes - (uint32)offset;
        if (count > file->sector_bytes)
            count = file->sector_bytes;
        *sector_bytes = file->sector_bytes;
        return xport_fread(buffer + (file->sector_bytes == 2048 ? 24 : 0), 1, count, source->streams[index]) == count;
    }
    return 0;
}

static CdlFILE *game_data_find(void *context, CdlFILE *result, const char *path)
{
    GAME_DATA_SOURCE *source = context;
    uint32 index;
    size_t length, cursor;
    const char *version;
    while (*path == '/' || *path == '\\')
        ++path;
    version = strchr(path, ';');
    length = version ? (size_t)(version - path) : strlen(path);
    for (index = 0; index < source->count; ++index)
    {
        const GAME_DATA_FILE *file = source->files + index;
        if (strlen(file->name) != length)
            continue;
        for (cursor = 0; cursor < length; ++cursor)
        {
            uint8 character = (uint8)path[cursor];
            if (character >= 'a' && character <= 'z')
                character -= 'a' - 'A';
            if ((uint8)file->name[cursor] != character)
                break;
        }
        if (cursor != length)
            continue;
        memset(result, 0, sizeof(*result));
        CdIntToPos((sint32)file->lba, &result->pos);
        result->size = file->logical_bytes;
        memcpy(result->name, file->name, length < sizeof(result->name) ? length : sizeof(result->name) - 1);
        return result;
    }
    return NULL;
}

sint32 game_mount_data(uint32 disc)
{
    GAME_DATA_SOURCE *source;
    XPORT_CD_SOURCE native = {0};
    uint32 index, end;
    if (disc != 1 && disc != 2)
        return 0;
    source = calloc(1, sizeof(*source));
    if (!source)
        return 0;
    source->files = disc == 1 ? game_disc_1 : game_disc_2;
    source->count = sizeof(game_disc_1) / sizeof(game_disc_1[0]);
    source->disc = disc;
    for (index = 0; index < source->count; ++index)
    {
        const GAME_DATA_FILE *file = source->files + index;
        end = file->lba + file->physical_bytes / file->sector_bytes + (file->physical_bytes % file->sector_bytes != 0);
        if (end > native.sectors)
            native.sectors = end;
    }
    native.context = source;
    native.read_sector = game_data_read;
    native.search_file = game_data_find;
    native.close = game_data_close;
    if (xport_cd_mount_source(&native))
    {
        game_cd_cancel_data_notification();
        game_data_disc = disc;
        return 1;
    }
    game_data_close(source);
    return 0;
}
