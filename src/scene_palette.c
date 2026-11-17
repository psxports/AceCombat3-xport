#include <string.h>

#include "game_functions.h"

typedef char scene_palette_upload_layout_check[sizeof(DR_LOAD) == 68 && offsetof(DR_LOAD, code) == 4 && offsetof(DR_LOAD, p) == 16 && sizeof(((DR_LOAD *)0)->p[0]) == 4 ? 1 : -1];

SCENE_PALETTE scene_palette;

void scene_palette_reset(void)
{
    memset(&scene_palette, 0, sizeof(scene_palette));
}

void scene_palette_begin(void)
{
    scene_palette_reset();
    scene_palette.rectangle.x = 1008;
    scene_palette.rectangle.y = 248;
    scene_palette.rectangle.w = 16;
    scene_palette.rectangle.h = 8;
}

void scene_palette_capture(uint32 resource, RESOURCE_ASSET_SOURCE parent)
{
    uint32 index;
    FUNCTION_MARKER(0x800ED0D4, "ACE3.BIN");
    scene_texture_load_group_sized(resource, 0x10000, parent, SCENE_TEXTURE_STANDARD);
    game_capture_image(scene_palette.rectangle, scene_palette.saved, sizeof(scene_palette.saved));
    DrawSync(0);
    for (index = 0; index < 64; ++index)
        scene_palette.neutral[index] = 0xCE73CE73;
    {
        (void)(uint32)(game_upload_image(scene_palette.rectangle, scene_palette.neutral, sizeof(scene_palette.neutral)));
        return;
    }
}

uint32 scene_palette_restore(uint32 mode)
{
    const uint32 *pixels;
    FUNCTION_MARKER(0x80108478, "ACE3.BIN");
    if (mode != 0 && mode != 1)
        return 1;
    pixels = mode ? scene_palette.neutral : scene_palette.saved;
    return game_upload_image(scene_palette.rectangle, pixels, sizeof(scene_palette.saved));
}

void scene_palette_blend(uint32 amount)
{
    PSX_RECT rectangle;
    DR_LOAD *upload;
    uint32 *entry;
    uint16 colors[SCENE_PALETTE_COLORS] = {0};
    const uint32 *source, *destination;
    uint32 first, second, red, green, blue, product, index, row, word;
    uint32 packet, ordering, tag, link, value, other;
    sint32 rows;
    FUNCTION_MARKER(0x800F7C24, "ACE3.BIN");
    rows = scene_palette.rectangle.h;
    index = 0;
    if (rows > 0)
    {
        do
        {
            destination = scene_palette.neutral;
            source = scene_palette.saved;
            first = (destination[index >> 1] >> ((index & 1) * 16)) & 0xFFFF;
            second = (source[index >> 1] >> ((index & 1) * 16)) & 0xFFFF;
            product = ((second & 31) - (first & 31)) * amount;
            red = (first & 31) + (uint32)((sint32)product / 25);
            product = (((second >> 5) & 31) - ((first >> 5) & 31)) * amount;
            green = ((first >> 5) & 31) + (uint32)((sint32)product / 25);
            product = (((second >> 10) & 31) - ((first >> 10) & 31)) * amount;
            blue = ((first >> 10) & 31) + (uint32)((sint32)product / 25);
            colors[index] = (red | (green << 5) | (blue << 10));
            rows = scene_palette.rectangle.h;
            ++index;
        } while ((sint32)index < rows * 16);
    }
    rectangle = scene_palette.rectangle;
    rectangle.h = 1;
    rows = scene_palette.rectangle.h;
    row = 0;
    value = rows;
    if (rows > 0)
    {
        do
        {
            packet = game_runtime.mission_control.packet_cur;
            game_runtime.mission_control.packet_cur = (uint32)(packet + 68);
            rectangle.y = (sint16)((uint16)scene_palette.rectangle.y + row);
            upload = mission_packet_span(packet, sizeof(*upload));
            SetDrawLoad(upload, &rectangle);
            for (word = 0; word < 8; ++word)
            {
                other = colors[row * 16 + word * 2] | ((uint32)colors[row * 16 + word * 2 + 1] << 16);
                upload->p[word] = other;
            }
            ordering = game_runtime.mission_control.ot;
            entry = mission_ot_span(ordering, sizeof(*entry));
            tag = upload->tag;
            link = *entry;
            upload->tag = (tag & 0xFF000000) | (link & 0xFFFFFF);
            ordering = game_runtime.mission_control.ot;
            entry = mission_ot_span(ordering, sizeof(*entry));
            link = *entry;
            *entry = (link & 0xFF000000) | (packet & 0xFFFFFF);
            rows = scene_palette.rectangle.h;
            ++row;
            value = (sint32)row < rows;
        } while (value);
    }
    {
        (void)(uint32)(value);
        return;
    }
}

void scene_palette_blend_pixels(uint8 *dst, const uint8 *src, sint32 count, uint32 colors, uint32 factor)
{
    uint16 wire_halfword_0;
    uint16 wire_halfword_1;
    uint32 red, green, blue, pixel, value, result;
    sint32 i;
    FUNCTION_MARKER(0x8005EB20, "ACE3.BIN");
    red = colors & 255;
    green = (colors >> 8) & 255;
    blue = (colors >> 16) & 255;
    xport_gte_write_control(21, red << 4);
    xport_gte_write_control(22, green << 4);
    xport_gte_write_control(23, blue << 4);
    xport_gte_write_data(8, factor);
    result = green << 4;
    for (i = 0; i < count; ++i)
    {
        pixel = (memcpy(&wire_halfword_0, src, sizeof(wire_halfword_0)), wire_halfword_0);
        src += 2;
        if (pixel != 0)
        {
            xport_gte_write_data(28, pixel);
            xport_gte_execute(0x980011);
            value = xport_gte_read_data(29);
            if (pixel & 0x8000)
                value |= 0x8000;
        }
        else
        {
            value = 0;
        }
        (wire_halfword_1 = (value), (void)memcpy(dst, &wire_halfword_1, sizeof(wire_halfword_1)));
        result = i + 1 < count;
        dst += 2;
    }
    {
        (void)(uint32)(result);
        return;
    }
}
