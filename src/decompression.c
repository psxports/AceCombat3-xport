#include "game_functions.h"
#include <stdlib.h>
#include <stdint.h>

typedef struct
{
    const uint8 *input;
    size_t input_bytes;
    uint8 *output;
    size_t output_bytes;
    size_t stop_after;
} ULZ_BUFFER_CONTEXT;

static const uint8 *ulz_input_at(const ULZ_BUFFER_CONTEXT *memory, size_t offset, size_t bytes)
{
    if (offset > memory->input_bytes || bytes > memory->input_bytes - offset)
        abort();
    return memory->input + offset;
}

static uint8 *decompress_output_at(const ULZ_BUFFER_CONTEXT *memory, size_t offset, size_t bytes, size_t end)
{
    if (offset > end || bytes > end - offset)
        abort();
    return memory->output + offset;
}

static const uint8 *decompress_history_at(const ULZ_BUFFER_CONTEXT *memory, size_t offset, size_t bytes)
{
    if (offset > memory->output_bytes || bytes > memory->output_bytes - offset)
        abort();
    return memory->output + offset;
}

static size_t ulz_decode_bytes(const ULZ_BUFFER_CONTEXT *memory, size_t source, size_t destination, uint32 distance_bits, size_t output_end)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint16 wire_halfword_3;
    size_t flags_cursor, literals, tokens, reference;
    const size_t output_begin = destination;
    uint32 bits, mask, flags, positive, token, distance, count, value;
    FUNCTION_MARKER(0x800AFD40, "ACE3.BIN");
    flags_cursor = source + 16;
    literals = (memcpy(&wire_word_0, ulz_input_at(memory, source + 8, 4), sizeof(wire_word_0)), wire_word_0);
    tokens = (memcpy(&wire_word_1, ulz_input_at(memory, source + 12, 4), sizeof(wire_word_1)), wire_word_1);
    literals = source + (literals & 0xFFFFFF);
    tokens = source + (tokens & 0xFFFFFF);
    bits = distance_bits & 31;
    mask = ((uint32)1 << bits) - 1;
    for (;;)
    {
        if (memory->stop_after != SIZE_MAX && destination - output_begin >= memory->stop_after)
            return destination;
        flags = (memcpy(&wire_word_2, ulz_input_at(memory, flags_cursor, 4), sizeof(wire_word_2)), wire_word_2);
        flags_cursor += 4;
        if (flags == 0)
            return destination;
        for (;;)
        {
            if (memory->stop_after != SIZE_MAX && destination - output_begin >= memory->stop_after)
                return destination;
            positive = (sint32)flags > 0;
            flags <<= 1;
            if (flags == 0)
                break;
            if (positive == 0)
            {
                value = *ulz_input_at(memory, literals, 1);
                literals += 1;
                *decompress_output_at(memory, destination, 1, output_end) = (uint8)value;
                destination += 1;
            }
            else
            {
                token = (memcpy(&wire_halfword_3, ulz_input_at(memory, tokens, 2), sizeof(wire_halfword_3)), wire_halfword_3);
                tokens += 2;
                distance = (token & mask) + 1;
                count = (token >> bits) + 3;
                reference = destination - distance;
                do
                {
                    value = *decompress_history_at(memory, reference, 1);
                    count -= 1;
                    *decompress_output_at(memory, destination, 1, output_end) = (uint8)value;
                    reference += 1;
                    destination += 1;
                } while ((sint32)count > 0);
            }
        }
    }
}

static size_t ulz_decode_halfwords(const ULZ_BUFFER_CONTEXT *memory, size_t source, size_t destination, uint32 distance_bits, size_t output_end)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint16 wire_halfword_3;
    uint16 wire_halfword_4;
    uint16 wire_halfword_5;
    uint16 wire_halfword_6;
    uint16 wire_halfword_7;
    size_t flags_cursor, literals, tokens, reference;
    uint32 bits, mask, flags, positive, token, distance, count, value;
    FUNCTION_MARKER(0x800AFDE4, "ACE3.BIN");
    flags_cursor = source + 16;
    literals = (memcpy(&wire_word_0, ulz_input_at(memory, source + 8, 4), sizeof(wire_word_0)), wire_word_0);
    tokens = (memcpy(&wire_word_1, ulz_input_at(memory, source + 12, 4), sizeof(wire_word_1)), wire_word_1);
    literals = source + (literals & 0xFFFFFF);
    tokens = source + (tokens & 0xFFFFFF);
    bits = (distance_bits - 1) & 31;
    mask = ((uint32)1 << bits) - 1;
    for (;;)
    {
        flags = (memcpy(&wire_word_2, ulz_input_at(memory, flags_cursor, 4), sizeof(wire_word_2)), wire_word_2);
        flags_cursor += 4;
        if (flags == 0)
            return destination;
        for (;;)
        {
            positive = (sint32)flags > 0;
            flags <<= 1;
            if (flags == 0)
                break;
            if (positive == 0)
            {
                value = (memcpy(&wire_halfword_3, ulz_input_at(memory, literals, 2), sizeof(wire_halfword_3)), wire_halfword_3);
                literals += 2;
                (wire_halfword_4 = (value), (void)memcpy(decompress_output_at(memory, destination, 2, output_end), &wire_halfword_4, sizeof(wire_halfword_4)));
                destination += 2;
            }
            else
            {
                token = (memcpy(&wire_halfword_5, ulz_input_at(memory, tokens, 2), sizeof(wire_halfword_5)), wire_halfword_5);
                tokens += 2;
                distance = ((token & mask) << 1) + 2;
                count = (token >> bits) + 2;
                reference = destination - distance;
                do
                {
                    value = (memcpy(&wire_halfword_6, decompress_history_at(memory, reference, 2), sizeof(wire_halfword_6)), wire_halfword_6);
                    count -= 1;
                    (wire_halfword_7 = (value), (void)memcpy(decompress_output_at(memory, destination, 2, output_end), &wire_halfword_7, sizeof(wire_halfword_7)));
                    reference += 2;
                    destination += 2;
                } while ((sint32)count > 0);
            }
        }
    }
}

static size_t ulz_decode_packed_bytes(const ULZ_BUFFER_CONTEXT *memory, size_t source, size_t destination, uint32 distance_bits, size_t output_end)
{
    uint32 wire_word_0;
    uint32 wire_word_1;
    uint32 wire_word_2;
    uint32 wire_word_3;
    uint32 wire_word_4;
    uint32 wire_word_5;
    uint16 wire_halfword_6;
    size_t flags_cursor, literals, tokens, reference;
    uint32 remaining, bits, mask, flags, flags_left, cache_left, cache, literal, token, count, value;
    FUNCTION_MARKER(0x800AFE90, "ACE3.BIN");
    flags_cursor = source + 16;
    literals = (memcpy(&wire_word_0, ulz_input_at(memory, source + 8, 4), sizeof(wire_word_0)), wire_word_0);
    tokens = (memcpy(&wire_word_1, ulz_input_at(memory, source + 12, 4), sizeof(wire_word_1)), wire_word_1);
    literals = source + (literals & 0xFFFFFF);
    tokens = source + (tokens & 0xFFFFFF);
    remaining = (memcpy(&wire_word_2, ulz_input_at(memory, source + 4, 4), sizeof(wire_word_2)), wire_word_2) & 0xFFFFFF;
    cache_left = 0xFFFFFFFF;
    bits = distance_bits & 31;
    mask = ((uint32)1 << bits) - 1;
    flags_left = 32;
    flags = (memcpy(&wire_word_3, ulz_input_at(memory, flags_cursor, 4), sizeof(wire_word_3)), wire_word_3);
    flags_cursor += 4;
    flags_left -= 1;
    while ((sint32)remaining > 0)
    {
        if ((sint32)flags_left < 0)
        {
            flags_left = 31;
            flags = (memcpy(&wire_word_4, ulz_input_at(memory, flags_cursor, 4), sizeof(wire_word_4)), wire_word_4);
            flags_cursor += 4;
        }
        literal = (sint32)flags < 0;
        flags <<= 1;
        if (literal != 0)
        {
            remaining -= 1;
            if ((sint32)cache_left > 0)
                cache_left -= 1;
            else
            {
                cache = (memcpy(&wire_word_5, ulz_input_at(memory, literals, 4), sizeof(wire_word_5)), wire_word_5);
                literals += 4;
                cache_left = 3;
            }
            *decompress_output_at(memory, destination, 1, output_end) = (uint8)cache;
            cache >>= 8;
            destination += 1;
        }
        else
        {
            token = (memcpy(&wire_halfword_6, ulz_input_at(memory, tokens, 2), sizeof(wire_halfword_6)), wire_halfword_6);
            tokens += 2;
            reference = destination - (token & mask) - 1;
            count = (token >> bits) + 3;
            remaining -= count;
            do
            {
                value = *decompress_history_at(memory, reference, 1);
                count -= 1;
                *decompress_output_at(memory, destination, 1, output_end) = (uint8)value;
                reference += 1;
                destination += 1;
            } while ((sint32)count > 0);
        }
        flags_left -= 1;
    }
    return destination;
}

static size_t decompress_dispatch(const ULZ_BUFFER_CONTEXT *memory, size_t source_offset, size_t destination_offset, size_t output_end)
{
    const uint8 *input = ulz_input_at(memory, source_offset, 16);
    uint32 format = input[7];
    uint32 distance_bits = input[11];
    switch (format)
    {
        case 0:
            return ulz_decode_bytes(memory, source_offset, destination_offset, distance_bits, output_end);
        case 1:
            return ulz_decode_halfwords(memory, source_offset, destination_offset, distance_bits, output_end);
        case 2:
            return ulz_decode_packed_bytes(memory, source_offset, destination_offset, distance_bits, output_end);
        default:
#ifndef NDEBUG
            fprintf(stderr, "Unsupported compression format %u\n", format);
#endif
            abort();
    }
}

size_t game_decompress_buffer(const uint8 *input, size_t input_bytes, uint8 *output, size_t output_bytes)
{
    ULZ_BUFFER_CONTEXT memory = {input, input_bytes, output, output_bytes, SIZE_MAX};
    if (!input || !output)
        abort();
    return decompress_dispatch(&memory, 0, 0, output_bytes);
}

size_t game_decompress_sized_buffer(const uint8 *input, size_t input_bytes, uint8 *output, size_t output_capacity)
{
    uint32 expected;
    size_t end;
    if (!input || input_bytes < 16 || !output || input[7] != 0)
        abort();
    memcpy(&expected, input + 4, sizeof(expected));
    expected &= 0xFFFFFF;
    if (expected > output_capacity)
        abort();
    ULZ_BUFFER_CONTEXT memory = {input, input_bytes, output, output_capacity, expected};
    end = decompress_dispatch(&memory, 0, 0, expected);
    if (end != expected)
        abort();
    return end;
}
