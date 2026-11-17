#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "game_functions.h"

typedef struct
{
    PSX_RECT rect[20];
} SCENE_TEXTURE_UPLOADS;

static SCENE_TEXTURE_UPLOADS uploads;

void scene_texture_reset(void)
{
    memset(&uploads, 0, sizeof(uploads));
}

uint32 scene_texture_decode_frame(void)
{
    uint32 value, other, format, phase, kind, variant, last, i, j, index, bank, base, row, column, bytes;
    uint8 *payload;
    uint16 *palette;
    uint32 *cursor, *destination;
    PSX_RECT *rectangle;
    RESOURCE_SECTOR_HEADER *header;
    RESOURCE_RING_FRAME frame;
    FUNCTION_MARKER(0x80058938, "ACE3.BIN");
    if (cd_stream_get_frame(&frame) != 0)
        return 0;
    header = frame.header;
    payload = frame.payload;
    value = header->info.frame;
    if (value == 0)
        scene_movie.stream_finished = (uint8)(1);
    value = header->info.codec.bytes[0];
    kind = header->info.codec.bytes[3];
    format = value & 15;
    phase = value >> 4;
    value = header->info.codec.bytes[7];
    variant = header->info.codec.bytes[1];
    last = value & 15;
    bytes = resource_frame_payload_bytes(header);
    if (bytes < (kind < 2 ? 532u : 4504u))
        abort();
    if (kind < 2)
    {
        palette = (uint16 *)(payload + 20);
        for (i = 0; i < 256; ++i)
        {
            value = (sint16)palette[i];
            other = palette[i];
            if (value != 0)
                palette[i] = other | 0x8000;
        }
    }
    else
    {
        cursor = (uint32 *)(payload + 2292);
        for (i = 573; i < 1125; ++i, ++cursor)
        {
            value = cursor[1];
            cursor[0] = value;
        }
    }
    if (kind == last + 15)
    {
        value = scene_movie.decode_bank;
        scene_movie.decode_bank = (uint8)(value == 0);
        if (variant == 1)
            scene_movie.decode_bank = (uint8)(0);
        for (i = 0; i < 5; ++i)
        {
            bank = scene_movie.decode_bank;
            scene_movie.records[bank][i].mode = (uint8)(0);
            bank = scene_movie.decode_bank;
            scene_movie.records[bank][i].start_tick = (uint16)(0xFFFF);
            bank = scene_movie.decode_bank;
            scene_movie.records[bank][i].end_tick = (uint16)(0xFFFF);
            for (j = 0; j < 4; ++j)
            {
                bank = scene_movie.decode_bank;
                scene_movie.records[bank][i].parameters[j] = (uint16)(0);
            }
        }
    }
    if (kind - 2 < 16)
    {
        bank = scene_movie.decode_bank;
        value = header->info.codec.bytes[7];
        scene_movie.textures[bank][17 - kind] = (uint8)(value >> 4);
        if (kind < 6)
        {
            bank = scene_movie.decode_bank;
            value = header->info.codec.bytes[7];
            scene_movie.textures[bank][21 - kind] = (uint8)(value >> 4);
        }
    }
    value = header->info.codec.bytes[4];
    if ((value & 15) == 1)
    {
        index = value >> 4;
        if (index >= 5)
            abort();
        bank = scene_movie.decode_bank;
        value = header->info.codec.bytes[5];
        scene_movie.records[bank][index].mode = (uint8)(value & 15);
        bank = scene_movie.decode_bank;
        value = header->info.codec.halfwords[4];
        scene_movie.records[bank][index].start_tick = (uint16)(value);
        bank = scene_movie.decode_bank;
        value = header->info.codec.halfwords[5];
        scene_movie.records[bank][index].end_tick = (uint16)(value);
        for (i = 0; i < 4; ++i)
        {
            value = header->info.codec.halfwords[6 + i];
            bank = scene_movie.decode_bank;
            scene_movie.records[bank][index].parameters[i] = (uint16)(value);
        }
    }
    if (format == 0)
    {
        if (kind >= 20)
            abort();
        rectangle = uploads.rect + kind;
        if (kind >= 2)
        {
            bank = scene_movie.decode_bank;
            base = bank != 0 ? 896 : 768;
            column = kind - 2;
            row = (sint32)column < 0 ? kind + 1 : column;
            row = (sint32)row >> 2;
            rectangle->x = (sint16)(base - ((column - row * 4 - 3) << 5));
            rectangle->y = (sint16)(0 - (row - 3) * 70);
            rectangle->w = (sint16)(32);
            rectangle->h = (sint16)(70);
        }
        else
        {
            rectangle->x = (sint16)(768);
            rectangle->w = (sint16)(256);
            rectangle->h = (sint16)(1);
            bank = scene_movie.decode_bank;
            if (kind == 0)
                uploads.rect[0].y = bank != 0 ? 283 : 281;
            else
                uploads.rect[1].y = bank != 0 ? 282 : 280;
        }
        destination = (uint32 *)(payload + 20);
        LoadImagePSX(rectangle, destination);
    }
    if (phase == 1)
        scene_movie.stream_playback.pending_frame = (uint8)(variant == 1 ? 2 : 1);
    cd_stream_free_frame(header);
    return 1;
}


static uint32 scene_texture_upload_block(PSX_RECT rectangle, const uint8 *pixels)
{
    return game_upload_image(rectangle, pixels, (size_t)rectangle.w * rectangle.h * 2);
}

void scene_texture_upload_tim(uint32 resource, const uint8 *native_data, size_t native_bytes)
{
    uint32 result;
    /* Defines the descriptor when the SDK rejects the input */
    GpuTimImage image = {0};
    FUNCTION_MARKER(0x800271A0, "ACE3.BIN");
    result = scene_texture_read_tim(native_data, native_bytes, &image);
    if (result == 0)
    {
#ifndef NDEBUG
        result = printf("Can't read TIM [%08x]\n", (uint32)(resource));
#else
        result = 0;
#endif
    }
    if (image.pixel_data)
        result = scene_texture_upload_block(image.pixels, image.pixel_data);
    if (image.clut_data)
        { (void)(uint32)(scene_texture_upload_block(image.clut, image.clut_data)); return; }
    { (void)(uint32)(result); return; }
}

uint32 scene_texture_read_tim(const uint8 *source, size_t bytes, GpuTimImage *image)
{
    uint32 wire_word_0;
    if (!source || bytes < 8 || (memcpy(&wire_word_0, source, sizeof(wire_word_0)), wire_word_0) != 16)
        return 0;
    return gpu_decode_tim_body(source + 4, bytes - 4, image);
}

void scene_texture_upload_tim_group(uint32 directory, uint32 count, const uint8 *native_data, size_t native_bytes)
{
    uint32 index, resource, offset;
    /* Retains the previous descriptor after a later rejected TIM */
    GpuTimImage image = {0};
    FUNCTION_MARKER(0x8002722C, "ACE3.BIN");
    index = 0;
    if ((sint32)count <= 0)
        return;
    if (!native_data || native_bytes < 4 || count > (native_bytes - 4) / sizeof(uint32))
        abort();
    do
    {
        memcpy(&offset, native_data + 4 + (size_t)index * sizeof(offset), sizeof(offset));
        if (index >= (count & 0xFFFF) || (offset & 0x80000000) ||
            offset > native_bytes || native_bytes - offset < 8)
            abort();
        resource = directory + offset;
        if (scene_texture_read_tim(native_data + offset, native_bytes - offset, &image) == 0)
        {
#ifndef NDEBUG
            printf("Can't read TIM [%08x]\n", (uint32)(resource));
#endif
        }
        if (image.pixel_data)
            scene_texture_upload_block(image.pixels, image.pixel_data);
        if (image.clut_data)
            scene_texture_upload_block(image.clut, image.clut_data);
        index += 1;
    } while ((sint32)index < (sint32)count);
}

void scene_texture_upload_tim_at(const GpuTimImage *image, uint32 pixel_x, uint32 pixel_y, uint32 clut_x, uint32 clut_y)
{
    uint32 result = 0;
    PSX_RECT rectangle;
    FUNCTION_MARKER(0x80027084, "ACE3.BIN");
    if (image->clut_data)
    {
        rectangle = image->clut;
        rectangle.x = (sint16)clut_x;
        rectangle.y = (sint16)clut_y;
        scene_texture_upload_block(rectangle, image->clut_data);
    }
    if (image->pixel_data)
    {
        rectangle = image->pixels;
        rectangle.x = (sint16)pixel_x;
        rectangle.y = (sint16)pixel_y;
        result = scene_texture_upload_block(rectangle, image->pixel_data);
    }
    { (void)(uint32)(result); return; }
}

void scene_texture_load_mission_images(uint32 resources, const MISSION_BUNDLE_DIRECTORY *bundle)
{
    uint32 wire_word_0;
    uint8 decoded[0x9000];
    uint32 temporary, i, data, input_offset, input_bytes, magic;
    const uint8 *input;
    FUNCTION_MARKER(0x800ECEC8, "ACE3.BIN");
    MISSION_IMAGE_DIRECTORY textures = mission_image_directory_bind(resources, arena_cursor(), bundle->encoded_base, bundle->data, bundle->bytes);
    temporary = arena_alloc(0x9000);
    for (i = 0; i < textures.count; ++i)
    {
        data = mission_image_directory_at(&textures, i);
        if (!bundle->data || data < bundle->encoded_base ||
            data - bundle->encoded_base > bundle->bytes ||
            bundle->bytes - (data - bundle->encoded_base) < sizeof(uint32))
            abort();
        input_offset = data - bundle->encoded_base;
        input_bytes = bundle->bytes - input_offset;
        input = bundle->data + input_offset;
        memcpy(&magic, input, sizeof(magic));
        if (magic == 0x1A7A6C55)
        {
            if (input_bytes < 16)
                abort();
            size_t bytes;
            GpuTimImage image = {0};
            bytes = game_decompress_buffer(input, input_bytes, decoded, sizeof(decoded));
            if (bytes < 8 || (memcpy(&wire_word_0, decoded, sizeof(wire_word_0)), wire_word_0) != 16 ||
                !gpu_decode_tim_body(decoded + 4, bytes - 4, &image))
                abort();
            if (image.pixel_data)
                game_upload_image(image.pixels, image.pixel_data, (size_t)image.pixels.w * image.pixels.h * 2);
            if (image.clut_data)
                game_upload_image(image.clut, image.clut_data, (size_t)image.clut.w * image.clut.h * 2);
        }
        else
            scene_texture_upload_tim(data, input, input_bytes);
        DrawSync(0);
    }
    arena_set_cursor(temporary);
    return;
}

void scene_texture_load_group(uint32 resource, RESOURCE_ASSET_SOURCE parent)
{
    RESOURCE_STREAM_BUFFER buffer_allocation;
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 temporary, count, magic, input_bytes;
    const uint8 *input;

    FUNCTION_MARKER(0x800ECDA8, "ACE3.BIN");
    buffer_allocation = arena_alloc_backed(0x30000);
    temporary = buffer_allocation.encoded_begin;
    if (!parent.data || resource < parent.encoded_base || resource - parent.encoded_base > parent.bytes ||
        parent.bytes - (resource - parent.encoded_base) < sizeof(uint32))
        abort();
    input = parent.data + (resource - parent.encoded_base);
    input_bytes = parent.bytes - (resource - parent.encoded_base);
    memcpy(&magic, input, sizeof(magic));
    if (magic == 0x1A7A6C55)
    {
        if (input_bytes < 16)
            abort();
        const uint8 *ulz_input = input;
        uint8 *ulz_output = buffer_allocation.buffer.data;
        size_t native_group_bytes = buffer_allocation.buffer.bytes;
        game_decompress_buffer(ulz_input, input_bytes, ulz_output, native_group_bytes);
        if (native_group_bytes < sizeof(uint32))
            abort();
        count = (memcpy(&wire_word_0, ulz_output, sizeof(wire_word_0)), wire_word_0);
        scene_texture_upload_tim_group(temporary, count, ulz_output, native_group_bytes);
    }
    else
    {
        size_t raw_group_bytes = input_bytes;
        count = (memcpy(&wire_word_1, input, sizeof(wire_word_1)), wire_word_1);
        scene_texture_upload_tim_group(resource, count, input, raw_group_bytes);
    }
    DrawSync(0);
    arena_set_cursor(temporary);
    return;
}

void scene_texture_load_image(uint32 resource, RESOURCE_ASSET_SOURCE parent, uint32 header_limited)
{
    RESOURCE_STREAM_BUFFER buffer_allocation;
    uint32 temporary, data, magic, input_bytes;
    const uint8 *input, *tim_data;
    size_t tim_bytes;

    FUNCTION_MARKER(0x800ECF88, "ACE3.BIN");
    buffer_allocation = arena_alloc_backed(0x9000);
    temporary = buffer_allocation.encoded_begin;
    if (!parent.data || resource < parent.encoded_base || resource - parent.encoded_base > parent.bytes ||
        parent.bytes - (resource - parent.encoded_base) < sizeof(uint32))
        abort();
    input = parent.data + (resource - parent.encoded_base);
    input_bytes = parent.bytes - (resource - parent.encoded_base);
    memcpy(&magic, input, sizeof(magic));
    if (magic == 0x1A7A6C55)
    {
        if (input_bytes < 16)
            abort();
        const uint8 *ulz_input = input;
        uint8 *ulz_output = buffer_allocation.buffer.data;
        tim_bytes = buffer_allocation.buffer.bytes;
        if (header_limited && ulz_input[7] == 0)
            game_decompress_sized_buffer(ulz_input, input_bytes, ulz_output, tim_bytes);
        else
            game_decompress_buffer(ulz_input, input_bytes, ulz_output, tim_bytes);
        data = temporary;
        tim_data = ulz_output;
    }
    else
    {
        data = resource;
        tim_data = input;
        tim_bytes = input_bytes;
    }
    scene_texture_upload_tim(data, tim_data, tim_bytes);
    DrawSync(0);
    { (void)(uint32)(arena_set_cursor(temporary)); return; }
}

void scene_texture_load_image_at(const MISSION_IMAGE_DIRECTORY *directory, uint32 index, uint32 x, uint32 y, uint32 clut_x, uint32 clut_y)
{
    uint32 wire_word_0;
    uint8 decoded[0x9000];
    const uint8 *input;
    size_t offset, bytes, input_bytes;
    uint32 resource;
    uint32 temporary;
    GpuTimImage image = {0};

    FUNCTION_MARKER(0x800ED000, "ACE3.BIN");
    resource = mission_image_directory_at(directory, index);
    temporary = arena_alloc(0x9000);
    if (!directory->data || resource < directory->encoded_base ||
        resource - directory->encoded_base > directory->data_bytes ||
        directory->data_bytes - (resource - directory->encoded_base) < 16)
        abort();
    offset = resource - directory->encoded_base;
    input = directory->data + offset;
    input_bytes = directory->data_bytes - offset;
    if ((memcpy(&wire_word_0, input, sizeof(wire_word_0)), wire_word_0) == 0x1A7A6C55)
    {
        bytes = game_decompress_buffer(input, input_bytes, decoded, sizeof(decoded));
        if (!scene_texture_read_tim(decoded, bytes, &image))
            abort();
    }
    else
    {
        scene_texture_read_tim(input, input_bytes, &image);
    }
    scene_texture_upload_tim_at(&image, (uint32)(sint32)(sint16)x, (uint32)(sint32)(sint16)y, (uint32)(sint32)(sint16)clut_x, (uint32)(sint32)(sint16)clut_y);
    DrawSync(0);
    { (void)(uint32)(arena_set_cursor(temporary)); return; }
}

void scene_texture_load_group_sized(uint32 resource, uint32 bytes, RESOURCE_ASSET_SOURCE parent, uint32 header_limited)
{
    RESOURCE_STREAM_BUFFER buffer_allocation;
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 temporary, count, magic, input_bytes;
    const uint8 *input;

    FUNCTION_MARKER(0x800ECE38, "ACE3.BIN");
    buffer_allocation = arena_alloc_backed(bytes);
    temporary = buffer_allocation.encoded_begin;
    if (!parent.data || resource < parent.encoded_base || resource - parent.encoded_base > parent.bytes ||
        parent.bytes - (resource - parent.encoded_base) < sizeof(uint32))
        abort();
    input = parent.data + (resource - parent.encoded_base);
    input_bytes = parent.bytes - (resource - parent.encoded_base);
    memcpy(&magic, input, sizeof(magic));
    if (magic == 0x1A7A6C55)
    {
        if (input_bytes < 16)
            abort();
        const uint8 *ulz_input = input;
        uint8 *ulz_output = buffer_allocation.buffer.data;
        size_t native_group_bytes = buffer_allocation.buffer.bytes;
        if (header_limited && ulz_input[7] == 0)
            game_decompress_sized_buffer(ulz_input, input_bytes, ulz_output, native_group_bytes);
        else
            game_decompress_buffer(ulz_input, input_bytes, ulz_output, native_group_bytes);
        if (native_group_bytes < sizeof(uint32))
            abort();
        count = (memcpy(&wire_word_0, ulz_output, sizeof(wire_word_0)), wire_word_0);
        scene_texture_upload_tim_group(temporary, count, ulz_output, native_group_bytes);
    }
    else
    {
        size_t raw_group_bytes = input_bytes;
        count = (memcpy(&wire_word_1, input, sizeof(wire_word_1)), wire_word_1);
        scene_texture_upload_tim_group(resource, count, input, raw_group_bytes);
    }
    DrawSync(0);
    arena_set_cursor(temporary);
    return;
}

uint32 scene_texture_unpack_tim_group(uint32 data, uint32 header, RESOURCE_ASSET_SOURCE parent, RESOURCE_STREAM_BUFFER backing)
{
    uint32 wire_word_0;
    uint32 result, descriptor;
    FUNCTION_MARKER(0x800272F0, "ACE3.BIN");
    if (!parent.data || data < parent.encoded_base || data - parent.encoded_base > parent.bytes || parent.bytes - (data - parent.encoded_base) < 16)
        abort();
    const uint8 *ulz_input = parent.data + (data - parent.encoded_base);
    uint32 output_offset = header - parent.encoded_base;
    if (backing.buffer.data != parent.data || backing.encoded_begin != parent.encoded_base ||
        header < parent.encoded_base || output_offset >= backing.buffer.bytes)
        abort();
    uint8 *ulz_output = backing.buffer.data + output_offset;
    size_t native_group_bytes = backing.buffer.bytes - output_offset;
    result = (uint32)game_decompress_buffer(ulz_input, parent.bytes - (data - parent.encoded_base), ulz_output, native_group_bytes);
    if (native_group_bytes < sizeof(uint32))
        abort();
    descriptor = (memcpy(&wire_word_0, ulz_output, sizeof(wire_word_0)), wire_word_0);
    scene_texture_upload_tim_group(header, descriptor, ulz_output, native_group_bytes);
    return result;
}


void title_texture_upload_image(uint32 resource, const uint8 *data, size_t bytes)
{
    uint32 scratch;

    FUNCTION_MARKER(0x800D8CEC, "ACE3.BIN");
    scratch = arena_alloc(0x10000);
    scene_texture_upload_tim(resource, data, bytes);
    DrawSync(0);
    { (void)(uint32)(arena_set_cursor(scratch)); return; }
}

void title_texture_upload_group(const uint8 *input, size_t resource_bytes)
{
    uint16 wire_halfword_0;
    uint32 wire_word_1;
    uint8 *decoded;
    size_t bytes;
    uint32 scratch, index, count, offset;
    GpuTimImage image = {0};
    FUNCTION_MARKER(0x800D8BA8, "ACE3.BIN");
    scratch = arena_alloc(0x30000);
    decoded = malloc(329365);
    if (!decoded)
        abort();
    bytes = game_decompress_buffer(input, resource_bytes, decoded, 329365);
    if (bytes < 4)
        abort();
    count = (memcpy(&wire_halfword_0, decoded, sizeof(wire_halfword_0)), wire_halfword_0);
    if (count > (bytes - 4) / 4)
        abort();
    for (index = 0; index < count; ++index)
    {
        offset = (memcpy(&wire_word_1, decoded + 4 + index * 4, sizeof(wire_word_1)), wire_word_1);
        if (offset > bytes || !scene_texture_read_tim(decoded + offset, bytes - offset, &image))
            abort();
        if (image.pixel_data)
            game_upload_image(image.pixels, image.pixel_data, (size_t)image.pixels.w * image.pixels.h * 2);
        if (image.clut_data)
            game_upload_image(image.clut, image.clut_data, (size_t)image.clut.w * image.clut.h * 2);
    }
    DrawSync(0);
    free(decoded);
    arena_set_cursor(scratch);
    return;
}

void scene_texture_load_archive_group(const uint8 *input, size_t input_bytes)
{
    uint32 temporary = arena_alloc(0x30000);
    uint32 count, index, offset;
    uint8 *output;
    size_t bytes;
    if (!temporary || !input || input_bytes < 16 || memcmp(input, "Ulz\x1a", 4))
        abort();
    output = malloc(0x30000);
    if (!output)
        abort();
    bytes = game_decompress_sized_buffer(input, input_bytes, output, 0x30000);
    if (bytes < sizeof(count))
        abort();
    memcpy(&count, output, sizeof(count));
    if (count > (bytes - 4) / sizeof(uint32))
        abort();
    for (index = 0; index < count; ++index)
    {
        memcpy(&offset, output + 4 + index * 4, sizeof(offset));
        if (offset > bytes || bytes - offset < 8)
            abort();
    }
    scene_texture_upload_tim_group(temporary, count, output, bytes);
    DrawSync(0);
    free(output);
    arena_set_cursor(temporary);
}
