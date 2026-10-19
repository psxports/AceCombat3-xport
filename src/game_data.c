#include "game_functions.h"
#include <stdlib.h>
#include <string.h>

typedef struct
{
    const char *name;
    uint32 lba, logical_bytes, physical_bytes, sector_bytes;
} GAME_DATA_FILE;

typedef struct
{
    const GAME_DATA_FILE *files;
    uint32 count;
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
    char path[64];
    long bytes;
    if (disc != 1 && disc != 2)
        return 0;
    source = calloc(1, sizeof(*source));
    if (!source)
        return 0;
    source->files = disc == 1 ? game_disc_1 : game_disc_2;
    source->count = sizeof(game_disc_1) / sizeof(game_disc_1[0]);
    for (index = 0; index < source->count; ++index)
    {
        const GAME_DATA_FILE *file = source->files + index;
        sint32 length = snprintf(path, sizeof(path), "DATA/%u/%s", disc, file->name);
        if (length < 0 || (uint32)length >= sizeof(path))
            goto fail;
        source->streams[index] = xport_fopen(path, "rb");
        if (!source->streams[index] || fseek(source->streams[index], 0, SEEK_END))
            goto fail;
        bytes = ftell(source->streams[index]);
        if (bytes < 0 || (uint32)bytes != file->physical_bytes)
            goto fail;
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
        return 1;
    }
fail:
    game_data_close(source);
    return 0;
}
