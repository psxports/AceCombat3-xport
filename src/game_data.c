#include "game_functions.h"
#include "psx_mdec.h"
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

uint32 game_current_data_disc(void)
{
    return game_data_disc;
}

enum
{
    RESOURCE_DIRECTORY_ENTRIES = 574
};

typedef struct
{
    uint32 magic;
    uint32 status;
    uint32 code;
    uint32 count;
    RESOURCE_ENTRY entries[RESOURCE_DIRECTORY_ENTRIES];
} RESOURCE_DIRECTORY;

typedef struct
{
    uint32 flags;
    uint32 sector;
} RESOURCE_ENTRY_RECORD;

typedef struct
{
    uint32 magic;
    uint32 status;
    uint32 code;
    uint32 reserved;
    uint32 count;
    RESOURCE_ENTRY_RECORD entries[RESOURCE_DIRECTORY_ENTRIES];
} RESOURCE_DIRECTORY_RECORD;

typedef char RESOURCE_DIRECTORY_LAYOUT_CHECK[sizeof(RESOURCE_ENTRY_RECORD) == 8 && offsetof(RESOURCE_ENTRY_RECORD, sector) == 4 && offsetof(RESOURCE_DIRECTORY_RECORD, count) == 16 && offsetof(RESOURCE_DIRECTORY_RECORD, entries) == 20 && sizeof(RESOURCE_DIRECTORY_RECORD) == 4612 ? 1 : -1];

static RESOURCE_DIRECTORY resource_directories[2];
RESOURCE_METADATA resource_metadata;

static uint32 game_music_active, game_music_done;
static uint32 game_music_completion_enabled;
static FILE *game_music_metadata;
static RESOURCE_TIMED_METADATA_RECORD game_music_record;
static uint32 game_music_record_ready, game_music_start, game_music_end;
static uint32 game_music_metadata_remaining;
static FILE *game_stream_data;
static RESOURCE_SCRIPT_SECTOR game_stream_record;
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

static void game_select_resource_channel(const RESOURCE_ENTRY *entry)
{
    resource_stream_select_channel((entry->flags >> 19) & 31);
}

typedef enum
{
    GAME_MOVIE_IDLE,
    GAME_MOVIE_PREPARED,
    GAME_MOVIE_OPEN
} GAME_MOVIE_STATE;

static GAME_MOVIE_STATE game_movie_state;

static uint8 *game_movie_rgb, *game_movie_pixels;
static XportFmvFrame game_movie_frame;
static uint32 game_movie_pending, game_movie_resource, game_movie_done;
static MOVIE_PLAYBACK *game_movie_owner;
static MOVIE_SURFACE *game_movie_surface;
static FILE *game_movie_metadata;
static uint32 game_movie_metadata_mode;
static RESOURCE_TIMED_METADATA_RECORD game_movie_metadata_record;
static uint32 game_movie_metadata_remaining, game_movie_metadata_record_ready;

uint32 game_movie_matches(uint32 id)
{
    return game_movie_state != GAME_MOVIE_IDLE && (id & 0xFFF) == (game_movie_resource & 0xFFF);
}

void game_movie_request_begin(uint32 flags)
{
    game_movie_open(game_movie_resource, game_movie_owner, game_movie_surface, flags);
}

uint32 game_movie_request_complete(void)
{
    return game_movie_done;
}

void game_movie_close(void)
{
    uint32 opened = game_movie_state == GAME_MOVIE_OPEN;
#if !AP_WEB
    uint32 disc = game_current_data_disc();
#endif
    if (opened)
        xport_fmv_close();
    free(game_movie_rgb);
    free(game_movie_pixels);
    game_movie_rgb = game_movie_pixels = NULL;
    game_movie_pending = 0;
    game_movie_done = 0;
    game_movie_state = GAME_MOVIE_IDLE;
    if (game_movie_metadata)
        fclose(game_movie_metadata);
    game_movie_metadata = NULL;
    game_movie_metadata_mode = game_movie_metadata_remaining = game_movie_metadata_record_ready = 0;
#if !AP_WEB
    if (opened && !game_mount_data(disc))
    {
        xport_message_error("Ace Combat 3 - Movie error", "Cannot restore DATA source after movie");
        abort();
    }
#endif
}

void game_movie_prepare(uint32 resource, MOVIE_PLAYBACK *owner, MOVIE_SURFACE *surface)
{
    game_movie_close();
    game_movie_resource = resource;
    game_movie_owner = owner;
    game_movie_surface = surface;
    game_movie_state = GAME_MOVIE_PREPARED;
}

void game_movie_open(uint32 resource, MOVIE_PLAYBACK *owner, MOVIE_SURFACE *surface, uint32 flags)
{
    char path[64];
    long length;
    game_movie_close();
    game_stop_resource_music();
    resource_stop_cd(0);
    game_select_resource_channel(resource_get_entry(resource));
    snprintf(path, sizeof(path), "DATA/%u/VIDEO/%u.STR", game_data_disc, resource & 0x7FF);
    game_movie_rgb = malloc(640 * 256 * 3);
    game_movie_pixels = malloc(640 * 256 * 3);
    if (!game_movie_rgb || !game_movie_pixels || xport_fmv_open(path) <= 0)
    {
        game_movie_close();
        xport_message_error("Ace Combat 3 - Movie error", path);
        abort();
    }
    game_movie_surface = surface;
    game_movie_state = GAME_MOVIE_OPEN;
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
    resource_set_cd_muted(0);
    owner->done = (uint8)(0);
    owner->ready = (uint8)(1);
    return;
}

void game_movie_publish_frame(uint32 number)
{
    if (game_movie_metadata_mode == 1)
    {
        RESOURCE_MOVIE_FRAME_RECORD record;
        const RESOURCE_ENTRY *entry = resource_get_entry(game_movie_resource);
        uint32 channel = (entry->flags >> 19) & 31;
        RESOURCE_FRAME *metadata = resource_metadata.frames + channel;
        uint32 word;
        if (!number || fseek(game_movie_metadata, (long)((number - 1) * 20), SEEK_SET) || xport_fread(&record, 1, sizeof(record), game_movie_metadata) != sizeof(record) || record.number != number)
            abort();
        resource_metadata.channels |= (uint32)1 << channel;
        word = record.payload.kind_auxiliary;
        metadata->kind = (uint16)word;
        metadata->auxiliary = (sint16)(word >> 16);
        metadata->frame = record.payload.frame;
        metadata->extra[0] = record.payload.extra[0];
        metadata->extra[1] = record.payload.extra[1];
    }
}

sint32 game_movie_play(uint32 resource, uint32 captions, const XportFmvPlayback *playback)
{
    char path[64];
    long length;
    sint32 result;
    uint32 disc = game_current_data_disc();
    game_movie_close();
    game_stop_resource_music();
    resource_stop_cd(0);
    game_select_resource_channel(resource_get_entry(resource));
    game_movie_resource = resource;
    if (captions)
    {
        snprintf(path, sizeof(path), "ACE_META/%u", resource & 0x7FF);
        game_movie_metadata = game_data_open(disc, path);
        if (!game_movie_metadata || (length = game_file_length(game_movie_metadata)) < 0 || length % 20)
            abort();
        game_movie_metadata_mode = 1;
        game_movie_metadata_remaining = (uint32)length;
    }
    resource_begin_movie(resource);
    resource_set_cd_muted(0);
    snprintf(path, sizeof(path), "DATA/%u/VIDEO/%u.STR", disc, resource & 0x7FF);
    result = xport_fmv_play_configured(path, 0, 0, playback);
    game_movie_close();
    resource_end_movie();
#if !AP_WEB
    if (!game_mount_data(disc))
    {
        xport_message_error("Ace Combat 3 - Movie error", "Cannot restore DATA source after movie");
        abort();
    }
#endif
    return result;
}

static uint32 game_movie_metadata_ready(void)
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
            if (xport_fread(&game_movie_metadata_record, 1, sizeof(game_movie_metadata_record), game_movie_metadata) != sizeof(game_movie_metadata_record))
                abort();
            game_movie_metadata_remaining -= sizeof(game_movie_metadata_record);
            game_movie_metadata_record_ready = 1;
        }
        if (game_movie_pending != 2 && game_movie_metadata_record.key > game_movie_frame.number)
            break;
        if (!game_cd_publish_metadata(&game_movie_metadata_record.payload))
            return 0;
        game_movie_metadata_record_ready = 0;
    }
    return 1;
}

uint32 game_movie_poll(MOVIE_PLAYBACK *owner)
{
    sint32 result;
    if (!game_movie_rgb)
        return 0;
    if (game_movie_pending)
        goto metadata;
    result = xport_fmv_next(game_movie_rgb, 640 * 256 * 3, &game_movie_frame);
    if (result < 0)
    {
        game_movie_close();
        xport_message_error("Ace Combat 3 - Movie error", "Movie decoding failed");
        abort();
    }
    if (!result)
        return 0;
    game_movie_pending = (uint32)result;
    if (result == 1)
    {
        uint32 width = owner->mode ? game_movie_frame.width * 3 / 2 : game_movie_frame.width;
        game_movie_surface->page[0].w = (sint16)(width);
        game_movie_surface->page[0].h = (sint16)(game_movie_frame.height);
        game_movie_surface->page[1].w = (sint16)(width);
        game_movie_surface->page[1].h = (sint16)(game_movie_frame.height);
        game_movie_surface->strip.h = (sint16)game_movie_frame.height;
        game_movie_publish_frame(game_movie_frame.number);
    }
metadata:
    if (!game_movie_metadata_ready())
        return 0;
    if (game_movie_pending == 2)
        game_movie_done = 1;
    return 1;
}

uint32 game_movie_decode(MOVIE_PLAYBACK *owner)
{
    PSX_RECT rectangle;
    uint32 page, index, count, bytes;
    if (owner->done)
        return 1;
    if (!game_movie_poll(owner))
        return 0;
    if (game_movie_pending == 2)
        return 0;
    page = owner->display_index;
    rectangle.x = game_movie_surface->page[page].x;
    rectangle.y = game_movie_surface->page[page].y;
    rectangle.h = (sint16)game_movie_frame.height;
    count = game_movie_frame.width * game_movie_frame.height;
    if (owner->mode)
    {
        rectangle.w = (sint16)(game_movie_frame.width * 3 / 2);
        bytes = count * 3;
        memcpy(game_movie_pixels, game_movie_rgb, bytes);
    }
    else
    {
        rectangle.w = (sint16)game_movie_frame.width;
        for (index = 0; index < count; ++index)
        {
            uint32 rgb = game_movie_rgb[index * 3] | ((uint32)game_movie_rgb[index * 3 + 1] << 8) | ((uint32)game_movie_rgb[index * 3 + 2] << 16);
            uint16 pixel = (uint16)mdec_rgb_fifteen(rgb, 1);
            memcpy(game_movie_pixels + index * 2, &pixel, sizeof(pixel));
        }
    }
    if (LoadImagePSX(&rectangle, (uint32 *)game_movie_pixels) < 0)
        abort();
    game_movie_surface->page[0].w = (sint16)(rectangle.w);
    game_movie_surface->page[0].h = (sint16)(rectangle.h);
    game_movie_surface->page[1].w = (sint16)(rectangle.w);
    game_movie_surface->page[1].h = (sint16)(rectangle.h);
    game_movie_surface->strip.w = owner->mode ? 24 : 16;
    game_movie_surface->strip.h = rectangle.h;
    owner->display_index = (uint8)(page ^ 1);
    owner->ready = (uint8)(1);
    game_movie_pending = 0;
    return 0;
}

static void game_music_ready(uint8 event, uint8 *response)
{
    if (event == CdlDataEnd)
        game_music_done = 1;
}

sint32 game_resource_music_active(void)
{
    return game_music_active != 0;
}

sint32 game_start_resource_music(uint32 start, uint32 flags, uint32 data_stream)
{
    uint32 id = sound_get_cur_resource();
    const RESOURCE_ENTRY *entry = resource_get_entry(id);
    uint32 offset;
    uint32 sectors;
    long length;
    char path[64];
    if (!(id & 0x800) || start < entry->sector)
        return 0;
    offset = start - entry->sector;
    sectors = (entry->flags & 0x7FFFF) << ((entry->flags >> 24) & 7);
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
    CdSyncCallback(0);
    CdReadyCallback(game_music_ready);
    if (!xport_xa_play_file(path, 0))
    {
        CdReadyCallback(NULL);
        xport_message_error("Ace Combat 3 - Music error", path);
        abort();
    }
    game_music_active = 1;
    game_music_done = 0;
    game_music_completion_enabled = 1;
    game_music_start = start;
    game_music_end = entry->sector + sectors - 1;
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
    resource_stream_begin_file(flags, data_stream, sectors - offset, game_music_end);
    resource_set_cd_muted(0);
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
    uint32 frames, sector;
    CdlLOC position;
    uint32 completion_enabled;
    if (xport_xa_file_position(&position, &frames))
    {
        sector = (uint32)CdPosToInt(&position);
        resource_stream_file_position(sector, game_music_end, frames);
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
                count = xport_fread(&game_stream_record, 1, sizeof(game_stream_record), game_stream_data);
                if (count != sizeof(game_stream_record) || (game_stream_record.header[18] & 14) != 8)
                    abort();
                game_stream_remaining -= sizeof(game_stream_record);
                game_stream_record_ready = 1;
            }
            memcpy(&position, game_stream_record.header + 12, 3);
            key = (uint32)CdPosToInt(&position);
            if (key > sector)
                break;
            if (key >= game_music_start)
            {
                RESOURCE_CONTROLLER *state = &resource_controller;
                if (state->wait_mdec && DecDCToutSync(1))
                    return;
                if (game_stream_record.header[17] == 0 && (resource_stream_status().flags & 0x4000))
                {
                    if (!game_cd_publish_metadata(&game_stream_record.payload.metadata))
                        return;
                }
                else
                {
                    result = cd_stream_sector(&game_stream_record);
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
                count = xport_fread(&game_music_record, 1, sizeof(game_music_record), game_music_metadata);
                if (count != sizeof(game_music_record))
                    abort();
                game_music_metadata_remaining -= sizeof(game_music_record);
                game_music_record_ready = 1;
            }
            key = game_music_record.key;
            if (key > sector)
                break;
            if (key >= game_music_start && !game_cd_publish_metadata(&game_music_record.payload))
                return;
            game_music_record_ready = 0;
        }
    }
    if (!game_music_done)
        return;
    completion_enabled = game_music_completion_enabled;
    game_stop_resource_music();
    resource_stream_end_file();
    resource_set_cd_muted(1);
    if (completion_enabled)
        resource_read_complete(0);
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
    FILE *streams[4];
} GAME_DATA_SOURCE;

static const GAME_DATA_FILE game_disc_1[] = {
    {"ACE.BPB", 611, 94062592, 94062592, 2048},
    {"ACE.BPH", 606, 4612, 4612, 2048},
    {"ACE.SPB", 46540, 527171584, 605423616, 2352},
    {"ACE.SPH", 609, 3916, 3916, 2048},
};

static const GAME_DATA_FILE game_disc_2[] = {
    {"ACE.BPB", 611, 94212096, 94212096, 2048},
    {"ACE.BPH", 606, 4612, 4612, 2048},
    {"ACE.SPB", 46613, 533725184, 612950016, 2352},
    {"ACE.SPH", 609, 3916, 3916, 2048},
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

static void game_read_archive_headers(void)
{
    const char *names[2] = {"ACE.BPH", "ACE.SPH"};
    uint32 sizes[2] = {4612, 3916};
    RESOURCE_DIRECTORY_RECORD record;
    uint32 archive, index;
    for (archive = 0; archive < 2; ++archive)
    {
        RESOURCE_DIRECTORY *directory = resource_directories + archive;
        FILE *file = game_data_open(game_data_disc, names[archive]);
        if (!file || game_file_length(file) != (long)sizes[archive] || xport_fread(&record, 1, sizes[archive], file) != sizes[archive])
        {
            if (file)
                fclose(file);
            xport_message_error("Ace Combat 3 - Resource error", names[archive]);
            abort();
        }
        fclose(file);
        directory->magic = record.magic;
        directory->status = record.status;
        directory->code = record.code;
        directory->count = record.count;
        if (directory->count != (sizes[archive] - 20) / 8)
        {
            xport_message_error("Ace Combat 3 - Resource error", "Invalid archive directory count");
            abort();
        }
        for (index = 0; index < directory->count; ++index)
        {
            RESOURCE_ENTRY *entry = directory->entries + index;
            entry->flags = record.entries[index].flags;
            entry->sector = record.entries[index].sector;
            entry->id = index | (archive ? 0x800 : 0);
        }
    }
}

static sint32 cd_find_files(const char *const *paths, sint32 count, CdlLOC *positions, uint32 *sizes)
{
    CdlFILE file;
    sint32 i;

    FUNCTION_MARKER(0x8002ACD4, "ACE3.BIN");
    for (i = 0; i < count; ++i)
    {
        if (!CdSearchFile(&file, paths[i]))
        {
#ifndef NDEBUG
            printf("ERR : File Not Found.[%s]\n", paths[i]);
#endif
            return -1;
        }
        positions[i] = file.pos;
        sizes[i] = file.size;
    }
    return 1;
}

sint32 resource_init_archives(void)
{
    static const char *const paths[4] = {"\\ACE.BPH;1", "\\ACE.SPH;1", "\\ACE.BPB;1", "\\ACE.SPB;1"};
    CdlLOC positions[4];
    uint32 sizes[4];
    RESOURCE_DIRECTORY *bp = resource_directories;
    RESOURCE_DIRECTORY *sp = resource_directories + 1;
    uint32 bp_sector, sp_sector, index;
    RESOURCE_CONTROLLER *state;
    sint32 result;

    FUNCTION_MARKER(0x8002AE0C, "ACE3.BIN");
    CdSyncCallback(0);
    CdDataCallback(NULL);
    game_cd_ready_callback(0);
    result = cd_find_files(paths, 4, positions, sizes);
    if (result <= 0)
    {
        resource_controller.status = 5;
        return result;
    }
    game_read_archive_headers();
    if (bp->magic != 0x45334341 || sp->magic != 0x45334341)
    {
        resource_controller.status = 5;
#ifndef NDEBUG
        printf("KCD : Error ID %08x != %08x\n", bp->magic != 0x45334341 ? bp->magic : sp->magic, (uint32)0x45334341);
#endif
        return 1;
    }
    bp_sector = (uint32)CdPosToInt(positions + 2);
    state = &resource_controller;
    state->bp_sector = bp_sector;
    sp_sector = (uint32)CdPosToInt(positions + 3);
    state = &resource_controller;
    state->sp_sector = sp_sector;
    for (index = 0; index < bp->count; ++index)
        bp->entries[index].sector += bp_sector;
    for (index = 0; index < sp->count; ++index)
        sp->entries[index].sector += sp_sector;
    state = &resource_controller;
    state->status = sp->status;
    if (bp->code != 0x427)
    {
        resource_controller.status = 5;
#ifndef NDEBUG
        printf("KCD : Error SBCODE != %08x\n", (uint32)0x427);
#endif
    }
    if (sp->code != 0x19990427)
    {
        resource_controller.status = 5;
#ifndef NDEBUG
        printf("KCD : Error SPCODE != %08x\n", (uint32)0x19990427);
#endif
    }
    return 1;
}

const RESOURCE_ENTRY *resource_get_entry(uint32 id)
{
    RESOURCE_DIRECTORY *directory = resource_directories + ((id & 0x800) != 0);
    uint32 index = id & 0x7FF;
    FUNCTION_MARKER(0x8002995C, "ACE3.BIN");
    if (index >= directory->count)
    {
        xport_message_error("Ace Combat 3 - Resource error", "Archive resource index is out of bounds");
        abort();
    }
    return directory->entries + index;
}

uint32 resource_get_size(uint32 id)
{
    const RESOURCE_ENTRY *entry;
    uint32 word;
    FUNCTION_MARKER(0x8002C5E0, "ACE3.BIN");
    entry = resource_get_entry(id);
    word = entry->flags;
    if (id & 0x800)
        return (word & 0x7FFFF) << ((word >> 24) & 7);
    return (word & 0x7FFFF) << 2;
}

sint32 game_read_resource_file(const RESOURCE_ENTRY *entry, void *destination, uint32 bytes)
{
    uint32 index;
    char name[48];
    FILE *file;
    long length;
    sint32 result;
    if ((entry->id & 0x800) || (entry->id & 0x7FF) >= 574)
        return 0;
    index = entry->id & 0x7FF;
    if (index >= 1 && index <= 4)
        return 0;
    snprintf(name, sizeof(name), "ACE_BPB/%u", index);
    file = game_data_open(game_data_disc, name);
    if (!file)
        return 0;
    result = destination && !fseek(file, 0, SEEK_END) && (length = ftell(file)) >= 0 && (uint32)length == bytes && !fseek(file, 0, SEEK_SET) && xport_fread(destination, 1, bytes, file) == bytes;
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
            uint8 character = path[cursor];
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

static const uint8 mission_media_kinds[GAME_MISSION_COUNT] = {
    3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 2, 2, 2, 1, 2, 2, 1, 1, 3,
};

uint8 game_data_mission_media_kind(uint32 mission)
{
    if (mission >= GAME_MISSION_COUNT)
        abort();
    return mission_media_kinds[mission];
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

uint32 resource_relocate_directory(uint32 resource, uint32 *native_count, uint8 *native_data, uint32 native_bytes)
{
    uint16 wire_halfword_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint8 *entries;
    uint32 count;
    uint32 i;
    uint32 offset;

    FUNCTION_MARKER(0x80027338, "ACE3.BIN");
    if (!native_data || native_bytes < 4)
        abort();
    count = (memcpy(&wire_halfword_0, native_data, sizeof(wire_halfword_0)), wire_halfword_0);
    if (count > (native_bytes - 4) / 4)
        abort();
    entries = count ? native_data + 4 : NULL;
    for (i = 0; i < count; ++i)
    {
        offset = (memcpy(&wire_word_1, entries, sizeof(wire_word_1)), wire_word_1);
        if (!(offset & 0x80000000))
            (wire_word_2 = (offset + resource), (void)memcpy(entries, &wire_word_2, sizeof(wire_word_2)));
        entries += 4;
    }
    if (native_count)
        *native_count = i;
    return resource + 4;
}

uint32 resource_total_size(const uint32 *ids, sint32 count)
{
    uint32 total = 0;
    sint32 i;
    uint32 id;

    FUNCTION_MARKER(0x8002C63C, "ACE3.BIN");
    for (i = 0; i < count; ++i)
    {
        id = *ids;
        ++ids;
        total += resource_get_size(id);
    }
    return total;
}

void relocate_pointer_array(uint32 base, uint8 *pointers, sint32 count)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    sint32 i;
    uint32 value;

    FUNCTION_MARKER(0x80027380, "ACE3.BIN");
    for (i = 0; i < count; ++i, pointers += 4)
    {
        value = (memcpy(&wire_word_0, pointers, sizeof(wire_word_0)), wire_word_0);
        if (!(value & 0x80000000))
            (wire_word_1 = (value + base), (void)memcpy(pointers, &wire_word_1, sizeof(wire_word_1)));
    }
    {
        (void)(uint32)(base);
        return;
    }
}

uint32 resource_media_kind(uint32 id)
{
    const RESOURCE_ENTRY *entry;
    FUNCTION_MARKER(0x8002CBA4, "ACE3.BIN");
    entry = resource_get_entry(id);
    return (entry->flags >> 29) & 3;
}

uint32 resource_is_cd_audio(uint32 id)
{
    FUNCTION_MARKER(0x8002CB58, "ACE3.BIN");
    if ((uint32)cd_resource_state() - 1 < 3)
        return (sint32)resource_get_entry(id)->flags < 0;
    return 0;
}
