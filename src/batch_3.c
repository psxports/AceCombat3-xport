#include "game_functions.h"
#include <stdlib.h>
#include <string.h>

/* Complete draft bodies; runtime equivalence remains unverified */
sint32 cd_resource_state(void)
{
    sint32 state;

    FUNCTION_MARKER(0x8002B504, "ACE3.BIN");
    state = r_s32(r_u32(0x800BCBA8) + 12);
    return state == 5 ? -1 : state;
}

uint32 resource_is_cd_audio(uint32 id)
{
    FUNCTION_MARKER(0x8002CB58, "ACE3.BIN");
    if ((uint32)cd_resource_state() - 1 < 3)
        return r_s32(resource_get_entry(id)) < 0;
    return 0;
}

uint32 resource_queue_audio(uint32 descriptor, uint32 enabled, uint32 callback)
{
    uint32 context;
    uint32 sequence;
    uint32 request;

    FUNCTION_MARKER(0x8002C8D8, "ACE3.BIN");
    context = r_u32(0x800BCBBC);
    sequence = r_u32(context + 4);
    w_u32(context + 4, sequence + 1);
    request = context + (sequence & 15) * 20 + 8;
    w_u8(request, 3);
    w_u16(request + 2, 0);
    w_u32(request + 4, descriptor);
    w_u32(request + 8, enabled);
    w_u32(request + 16, callback);
    w_u8(request + 1, 0);
    return 3;
}

uint32 sub_8002dda8(uint32 voice)
{
    FUNCTION_MARKER(0x8002DDA8, "ACE3.BIN");
    if (voice >= 24)
        return 255;
    w_u8(0x800C5830 + voice * 20 + 1, 2);
    return voice;
}

uint32 sub_8002e5b4(uint32 index)
{
    uint32 voice;

    FUNCTION_MARKER(0x8002E5B4, "ACE3.BIN");
    if (index == 255)
        return 255;
    voice = 0x800C5830 + index * 20;
    w_u8(voice, 255);
    w_u8(voice + 1, 0);
    w_u8(voice + 2, 0);
    w_u8(voice + 16, 128);
    w_u8(voice + 17, 128);
    w_u16(voice + 18, 0);
    w_u8(voice + 3, 0);
    return index;
}

uint32 cd_audio_step(void)
{
    uint32 context;
    uint32 phase;
    uint32 flags;
    uint32 other;
    uint32 callback;
    uint8 parameters[1];

    FUNCTION_MARKER(0x80029FE4, "ACE3.BIN");
    context = r_u32(0x800BCBAC);
    phase = r_u32(context + 32);
    switch (phase)
    {
        case 0:
            w_u32(context + 32, 16);
            return CdControlB(13, psx_addr(context + 28, 2), NULL);
        case 16:
            parameters[0] = 0xE8;
            flags = r_u32(context);
            w_u32(context + 32, 32);
            w_u32(context, flags | 0x1000);
            return CdControlB(14, parameters, NULL);
        case 32:
            w_u32(context + 36, 0);
            game_cd_ready_callback(0x80029D70);
            context = r_u32(0x800BCBAC);
            w_u32(context + 32, 48);
            return CdControlB(27, NULL, NULL);
        case 48:
            sub_8002999c(0);
            context = r_u32(0x800BCBAC);
            w_u32(context + 48, 300);
            flags = r_u32(context);
            w_u32(context + 32, 64);
            flags = (flags & 0xBFFFFFFF) | 0x10;
            w_u32(context, flags);
            return flags;
        case 64:
            if (r_s32(context + 20) <= 0)
            {
                w_u32(context, r_u32(context) & 0x7EFFFFFF);
                game_cd_sync_callback(0);
                context = r_u32(0x800BCBAC);
                other = r_u32(0x800BCBB0);
                w_u32(context + 4, 0);
                w_u32(other, 0);
                callback = r_u32(context + 8);
                return callback ? xport_guest_call3(callback, 0, 0, 0) : 0;
            }
            flags = r_u32(context);
            if (!(flags & 0x40000000))
                return 0;
            if (flags & 0x02000000)
            {
                w_u32(context, flags & 0xFCFFFFFF);
                w_u32(context + 32, 48);
                return CdControlB(27, NULL, NULL);
            }
            flags |= 0x02000000;
            w_u32(context, flags);
            return flags;
        default:
            context = r_u32(0x800BCBAC);
            flags = r_u32(context);
            if (flags & 0x20)
            {
                w_u32(context, flags & 0xFFFFFFDF);
                return resource_issue_seek();
            }
            w_u32(context, flags | 0x80);
            game_cd_sync_callback(0);
            context = r_u32(0x800BCBAC);
            other = r_u32(0x800BCBB0);
            w_u32(context + 4, 0);
            w_u32(other, 0);
            callback = r_u32(context + 8);
            return callback ? xport_guest_call3(callback, 0xFFFFFFFE, 0, 0) : 0;
    }
}

uint32 cd_audio_poll(uint32 event)
{
    uint32 context;
    uint32 flags;
    uint32 other;
    uint32 clock;
    uint32 delta;
    uint32 value;
    uint32 callback;
    uint32 stop = 0;

    FUNCTION_MARKER(0x8002A498, "ACE3.BIN");
    context = r_u32(0x800BCBAC);
    // Music progress follows sectors consumed by the shared XA mixer
    if ((r_u32(context) & 0xC0004400) == 0x80000400 && r_u32(context + 32) == 64)
        cd_audio_sector();
    context = r_u32(0x800BCBAC);
    flags = r_u32(context);
    if (flags & 0x01000000)
    {
        if (!(flags & 0x40000000))
        {
            clock = r_u32(r_u32(r_u32(0x800BCBA8) + 32));
            delta = clock - r_u32(context + 68);
            value = delta * 5;
            w_u32(context + 40, r_u32(context + 36) + (uint32)((sint32)value >> 1));
        }
        context = r_u32(0x800BCBAC);
        flags = r_u32(context);
        if (flags & 0x10)
        {
            w_u32(context, flags & 0xFFFFFFEF);
            context = r_u32(0x800BCBAC);
        }
        if (r_s32(context + 20) <= 0 || r_s32(r_u32(r_u32(0x800BCBA8) + 32)) >= r_s32(context + 60))
        {
            if (event == 2 && !(r_u32(context) & 0x40000000))
            {
                sub_8002999c(1);
                context = r_u32(0x800BCBAC);
                w_u32(context + 20, 0);
                game_cd_ready_callback(0);
                CdControlF(9, NULL);
                return 1;
            }
            context = r_u32(0x800BCBAC);
        }
        flags = r_u32(context);
        if ((flags & 0x40) && event == 2)
        {
            flags &= 0xFFFFFFBF;
            w_u32(context, flags);
            if (!(flags & 0x40000000))
                w_u32(context, flags | 0x40000000);
            CdControlF(9, NULL);
            return 1;
        }
        context = r_u32(0x800BCBAC);
        if (!(r_u32(context) & 0x40000000))
        {
            delta = r_u32(context + 36) - r_u32(context + 16);
            value = (uint32)((sint32)(delta * 6) / 15);
            clock = r_u32(r_u32(r_u32(0x800BCBA8) + 32));
            w_u32(context + 64, value + clock - r_u32(context + 68));
        }
        return 0;
    }
    if ((sint32)flags < 0)
    {
        if (!(flags & 0x40000000))
            stop = r_s32(r_u32(r_u32(0x800BCBA8) + 32)) >= r_s32(context + 60);
        if ((~flags & 0x40000010) == 0x40000010)
        {
            value = r_u32(context + 48) - 1;
            other = r_u32(0x800BCBA8);
            w_u32(context + 48, value);
            if (!(r_u8(other + 40) & 2) || (sint32)value <= 0)
                stop = 1;
        }
    }
    if (!stop)
        return 0;
    sub_8002999c(1);
    context = r_u32(0x800BCBAC);
    w_u32(context + 20, 0);
    game_cd_ready_callback(0);
    context = r_u32(0x800BCBAC);
    w_u32(context, r_u32(context) & 0x7FFFFFFF);
    game_cd_sync_callback(0);
    context = r_u32(0x800BCBAC);
    other = r_u32(0x800BCBB0);
    w_u32(context + 4, 0);
    w_u32(other, 0);
    callback = r_u32(context + 8);
    if (callback)
        xport_guest_call3(callback, 0xFFFFFFFF, 0, 0);
    other = r_u32(0x800BCBA8);
    if (!CdControl(9, NULL, (uint8 *)psx_addr(other + 40, 8)))
        CdReset(0);
    return 1;
}

uint32 cd_get_sector_async(uint32 dst, uint32 words)
{
    FUNCTION_MARKER(0x800A3450, "ACE3.BIN");
    return game_cd_copy_sector(dst, words);
}

uint32 cd_audio_sector(void)
{
    uint8 header[12];
    uint32 context;
    uint32 flags;
    uint32 channel = 256;
    uint32 position;
    uint32 remaining;
    uint32 clock;
    uint32 result;

    FUNCTION_MARKER(0x80029BD8, "ACE3.BIN");
    context = r_u32(0x800BCBAC);
    if ((r_u32(context) & 0x4400) == 0x400)
    {
        if (!CdControlBPSX(16, NULL, header))
            return 0;
    }
    else
        cd_get_sector(header, 3);
    flags = r_u32(context) | 0x01000000;
    w_u32(context, flags);
    if (flags & 0x10)
        w_u32(context, flags & 0xFFFFFFEF);
    result = ~header[6] & 8;
    if (result)
        return result;
    position = (uint32)CdPosToInt((CdlLOC *)header);
    context = r_u32(0x800BCBAC);
    if (r_u32(context) & 0x1000)
        channel = header[5];
    remaining = r_u32(context + 44) - position;
    w_u32(context + 20, remaining);
    w_u32(context + 36, position);
    clock = r_u32(r_u32(r_u32(0x800BCBA8) + 32));
    w_u32(context + 68, clock);
    w_u32(context + 60, clock + 2);
    w_u32(context + 60, clock + 2 + (uint32)((sint32)(remaining * 6) / 15));
    if (!channel && (r_u32(context) & 0x4000))
        cd_get_sector_async(r_u32(0x800BCBB0), 129);
    else
    {
        context = r_u32(0x800BCBAC);
        flags = ~r_u32(context);
        if ((flags & 0x400) && ((flags & 0x2000) || channel == r_u8(context + 29)))
            cd_stream_sector(NULL);
    }
    context = r_u32(0x800BCBAC);
    result = r_u32(context + 20);
    if ((sint32)result > 0)
        return result;
    sub_8002999c(1);
    game_cd_ready_callback(0);
    return CdControlF(9, NULL);
}

uint32 cd_audio_ready(uint32 event, const uint8 *response)
{
    uint32 context;
    uint32 value;

    FUNCTION_MARKER(0x80029D70, "ACE3.BIN");
    context = r_u32(0x800BCBA8);
    value = xport_load_le32(response);
    w_u32(context + 40, value);
    value = xport_load_le32(response + 4);
    event &= 255;
    w_u32(context + 44, value);
    if (event != 1)
    {
        w_u32(r_u32(0x800BCBAC) + 52, 5);
        return 5;
    }
    if (r_u32(context + 36))
    {
        value = (uint32)DecDCToutSync(1);
        if (value)
        {
            w_u32(0x800BCE80, 1);
            return value;
        }
    }
    return cd_audio_sector();
}

uint32 cd_stream_sector(const uint8 *native_sector)
{
    uint32 context;
    uint32 header;
    uint32 destination;
    uint32 payload;
    uint32 source;
    uint32 target;
    uint32 words;
    uint32 header_words;
    uint32 payload_words;
    uint32 result;
    uint32 sequence;
    uint32 count;
    uint32 total;
    uint32 capacity;
    uint32 value;
    uint32 i;

    FUNCTION_MARKER(0x8002B760, "ACE3.BIN");
    context = r_u32(0x800BCBB4);
    header = r_u32(context + 44);
    result = r_u8(header);
    if (result == 2 || result == 4)
        return result;
    words = r_u32(context);
    if (native_sector)
    {
        if (words > 8)
            abort();
        memcpy(psx_addr(header, words << 2), native_sector, words << 2);
    }
    else
        cd_get_sector(psx_addr(header, words << 2), words);
    if (r_u8(header) != 1)
    {
        w_u8(header, 0);
        return 1;
    }
    context = r_u32(0x800BCBB4);
    if (r_u32(context + 36) != r_u16(header + 6))
    {
        result = r_u8(header + 1);
        if (result)
            return result;
        destination = r_u32(context + 28);
        w_u32(context + 44, destination);
        payload = r_u32(context + 32);
        w_u32(context + 48, payload);
        count = r_u8(header + 2);
        total = r_u32(context + 24) + count;
        capacity = r_u32(context + 12);
        if ((sint32)capacity < (sint32)total)
        {
            w_u8(header, 4);
            context = r_u32(0x800BCBB4);
            value = r_u32(context + 16);
            w_u32(context + 28, value);
            payload = r_u32(context + 20);
            destination = r_u32(context + 28);
            w_u32(context + 24, 0);
            w_u32(context + 44, destination);
            w_u32(context + 32, payload);
            w_u32(context + 48, payload);
            result = r_u8(destination);
            if (result == 2)
                return result;
        }
        context = r_u32(0x800BCBB4);
        value = r_u16(header + 6);
        w_u32(context + 36, value);
        if (header != destination)
        {
            source = header;
            target = destination;
            words = r_u32(context);
            header = r_u32(context + 28);
            i = 0;
            if ((sint32)words > 0)
            {
                do
                {
                    value = r_u32(source);
                    source += 4;
                    w_u32(target, value);
                    words = r_u32(r_u32(0x800BCBB4));
                    ++i;
                    target += 4;
                } while ((sint32)i < (sint32)words);
            }
        }
    }
    else
    {
        result = r_u32(context + 40) + 1;
        sequence = r_u8(header + 1);
        if (result != sequence)
            return result;
    }
    context = r_u32(0x800BCBB4);
    sequence = r_u8(header + 1);
    payload = r_u32(context + 48);
    payload_words = r_u32(context + 4);
    header_words = r_u32(context);
    w_u32(context + 40, sequence);
    if (native_sector)
    {
        if (header_words + payload_words > 582)
            abort();
        memcpy(psx_addr(payload, payload_words << 2), native_sector + (header_words << 2), payload_words << 2);
    }
    else
        cd_get_sector(psx_addr(payload, payload_words << 2), payload_words);
    context = r_u32(0x800BCBB4);
    sequence = r_u32(context + 40) + 1;
    count = r_u8(header + 2);
    if (sequence == count)
    {
        destination = r_u32(context + 28);
        payload = r_u32(context + 32);
        if ((sint32)sequence > 0)
        {
            i = 0;
            do
            {
                w_u8(destination, 2);
                destination += header_words << 2;
                count = r_u8(header + 2);
                ++i;
                payload += payload_words << 2;
            } while (i < count);
            context = r_u32(0x800BCBB4);
        }
        w_u32(context + 52, r_u32(context + 52) + 1);
        count = r_u8(header + 2);
        total = r_u32(context + 24);
        capacity = r_u32(context + 12);
        total += count;
        w_u32(context + 24, total);
        if ((sint32)total >= (sint32)capacity)
        {
            destination = r_u32(context + 16);
            payload = r_u32(context + 20);
            w_u32(context + 24, 0);
            w_u32(context + 28, destination);
            w_u32(context + 32, payload);
        }
        else
        {
            w_u32(context + 28, destination);
            w_u32(context + 32, payload);
        }
        context = r_u32(0x800BCBB4);
        destination = r_u32(context + 28);
        payload = r_u32(context + 32);
        w_u32(context + 44, destination);
        w_u32(context + 48, payload);
        return context;
    }
    value = r_u32(context + 44) + (header_words << 2);
    w_u32(context + 44, value);
    result = r_u32(context + 48) + (payload_words << 2);
    w_u32(context + 48, result);
    return result;
}

uint32 sub_8002bdc4(void)
{
    FUNCTION_MARKER(0x8002BDC4, "ACE3.BIN");
    return 1;
}

uint32 guest_strlen(uint32 text)
{
    uint32 length = 0;

    FUNCTION_MARKER(0x800ADEF8, "ACE3.BIN");
    if (text)
        while (r_s8(text++))
            ++length;
    return length;
}

uint32 sound_set_voice_fade(uint32 index, uint32 percent, uint32 steps)
{
    uint32 voice;
    sint32 scaled;
    sint32 result;

    FUNCTION_MARKER(0x8002DF40, "ACE3.BIN");
    if (index >= 24)
        return 255;
    scaled = (sint32)(percent << 7);
    voice = 0x800C5830 + index * 20;
    w_u16(voice + 18, steps);
    result = scaled / 100;
    w_u8(voice + 16, result);
    return (uint32)result;
}

void sub_8002df98(uint32 index)
{
    SpuVoiceAttr attr = {0};
    uint32 game;
    uint32 voice;
    sint32 volume;
    sint32 left;
    sint32 right;
    sint32 value;
    uint32 fade;

    FUNCTION_MARKER(0x8002DF98, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    volume = r_u8(game + 920);
    attr.voice = (uint32)1 << (index & 31);
    attr.mask = 0x23;
    voice = 0x800C5830 + index * 20;
    if (r_u8(game + 922) & 1)
    {
        left = r_s16(voice + 4);
        right = r_s16(voice + 6);
        if (left < 0)
            left = -left;
        if (right < 0)
            right = -right;
        value = (volume * (left + right)) >> 9;
        fade = r_u8(voice + 17);
        value = (value * (sint32)fade) >> 7;
        attr.volume.left = (sint16)value;
        attr.volume.right = (sint16)value;
    }
    else
    {
        value = r_s16(voice + 4) * volume;
        fade = r_u8(voice + 17);
        value >>= 8;
        if (fade != 128)
            value = (value * r_u8(voice + 17)) >> 7;
        attr.volume.left = (sint16)value;
        value = r_s16(voice + 6) * volume;
        fade = r_u8(voice + 17);
        value >>= 8;
        if (fade != 128)
            value = (value * r_u8(voice + 17)) >> 7;
        attr.volume.right = (sint16)value;
    }
    attr.note = r_u16(voice + 12);
    SpuSetVoiceAttr(&attr);
}

sint32 sound_disable_reverb(void)
{
    SpuReverbAttr attr = {0};

    FUNCTION_MARKER(0x8002DAF4, "ACE3.BIN");
    attr.mask = 6;
    attr.depth.left = 0;
    attr.depth.right = 0;
    SpuSetReverbDepth(&attr);
    SpuSetReverb(0);
    SpuSetReverbVoice(0, 0xFFFFFF);
    return SpuClearReverbWorkArea(r_s32(0x800C65F4));
}

void sound_stop_all(void)
{
    uint32 i;

    FUNCTION_MARKER(0x8002DDE0, "ACE3.BIN");
    for (i = 0; i < 24; ++i)
        sub_8002e5b4(i);
    SpuSetKey(0, 0xFFFFFF);
}

uint32 sub_80057cc4(void)
{
    uint32 state;

    FUNCTION_MARKER(0x80057CC4, "ACE3.BIN");
    state = r_u32(0x800BCD8C);
    w_u32(state + 8, 0);
    return state;
}

uint32 sub_80057cd0(void)
{
    uint32 game;
    uint32 context;
    uint32 state;
    uint32 enabled;
    uint32 index;
    uint32 destination;
    sint32 id;

    FUNCTION_MARKER(0x80057CD0, "ACE3.BIN");
    w_u32(0x800BD040, 0);
    resource_init_queue(0, 0);
    w_u8(r_u32(0x800BCE48) + 16, 0);
    w_u8(r_u32(0x800BCE48) + 17, 0);
    w_u8(r_u32(0x800BCE48) + 18, 0);
    context = r_u32(0x800BCE48);
    state = r_u32(context);
    game = r_u32(0x800BCE60);
    w_u32(context, state | 8);
    if (!r_u8(game + 936))
    {
        w_u8(game + 932, r_u8(game + 932) + 1);
        game = r_u32(0x800BCE60);
        if ((r_u8(game + 932) & 3) != 1)
        {
            w_u8(game + 933, (r_u32(game + 296) & 0x300) == 0x300 ? 2 : 1);
            return 0x60000002;
        }
    }
    game = r_u32(0x800BCE60);
    enabled = r_u8(game + 924);
    state = r_u8(game + 935);
    context = r_u32(0x800BCD8C);
    enabled &= state;
    w_u32(context + 4, enabled);
    index = r_u8(game + 936);
    id = r_s16(r_u32(0x800BCD94) + index * 8 + 4);
    if (enabled && id)
    {
        destination = arena_alloc(resource_get_size((uint32)id));
        context = r_u32(0x800BCD8C);
        w_u32(context, destination);
        w_u32(context + 8, 1);
        resource_queue_read((uint32)id, destination, 0x80057CC4);
    }
    else
    {
        context = r_u32(0x800BCD8C);
        w_u32(context, 0);
        w_u32(context + 4, 0);
    }
    return 0x04000000;
}

uint32 movie_current_buffer(void)
{
    FUNCTION_MARKER(0x80056B1C, "ACE3.BIN");
    return r_u8(r_u32(0x800BD048) + 2) == 0;
}

uint32 movie_configure(const MOVIE_CONFIG *config)
{
    uint32 state;
    uint32 upper;

    FUNCTION_MARKER(0x80056B88, "ACE3.BIN");
    state = r_u32(0x800BD048);
    upper = state + 0x10000;
    w_u32(upper + 0x1038, config->resource);
    w_u32(upper + 0x103C, config->stream_bytes);
    w_u32(upper + 0x1040, config->stream_buffer);
    w_u32(state + 8, config->vlc_buffer0);
    w_u32(state + 12, config->vlc_buffer1);
    w_u32(state + 16, config->frame_buffer0);
    w_u32(state + 20, config->frame_buffer1);
    w_u8(state + 5, config->mode);
    state = r_u32(0x800BD048);
    w_u16(state + 24, config->x0);
    w_u16(state + 26, config->y0);
    w_u16(state + 32, config->x1);
    w_u16(state + 28, 0);
    w_u16(state + 30, 0);
    w_u16(state + 36, 0);
    w_u16(state + 38, 0);
    w_u16(state + 34, config->y1);
    w_u16(state + 40, config->x0);
    w_u16(state + 42, config->y0);
    w_u16(state + 44, config->mode ? 24 : 16);
    state = r_u32(0x800BD048);
    w_u8(state + 1, 0);
    w_u8(state, 0);
    upper = r_u32(0x800BD048);
    w_u16(state + 46, 0);
    w_u16(upper + 0x11032, 0);
    w_u16(upper + 0x11030, 0);
    w_u16(upper + 0x11036, 0);
    w_u8(upper + 2, 0);
    state = r_u32(0x800BD048);
    w_u8(state + 3, 0);
    return state;
}

uint32 movie_init_decoder(void)
{
    FUNCTION_MARKER(0x80056B30, "ACE3.BIN");
#if AP_WEB
    w_u8(r_u32(0x800BD048) + 3, 1);
    return 1;
#endif
    DecDCTReset(0);
    DecDCTvlcSize2(0);
    DecDCToutCallback(movie_output_callback);
    DecDCTvlcBuild(psx_addr(r_u32(0x800BD048) + 48, sizeof(DECDCTTAB)));
    w_u8(r_u32(0x800BD048) + 4, 0);
    w_u8(r_u32(0x800BD048) + 3, 1);
    return 1;
}

uint32 movie_begin(uint32 resource, uint32 mode, uint32 arena, uint32 stream_bytes, uint32 vlc_bytes, uint32 frame_bytes, sint32 x0, sint32 y0, sint32 x1, sint32 y1)
{
    MOVIE_CONFIG config;
    uint32 cursor;

    FUNCTION_MARKER(0x80056CAC, "ACE3.BIN");
    w_u32(0x800BD048, arena);
    cursor = arena + 0x11044;
    config.stream_buffer = cursor;
    stream_bytes = ((stream_bytes + 3) >> 2) << 2;
    cursor += stream_bytes;
    config.resource = resource;
    config.stream_bytes = stream_bytes;
    config.vlc_buffer0 = cursor;
    config.mode = mode;
    vlc_bytes = ((vlc_bytes + 3) >> 2) << 2;
    frame_bytes = ((frame_bytes + 3) >> 2) << 2;
    cursor += vlc_bytes;
    config.vlc_buffer1 = cursor;
    cursor += vlc_bytes;
    config.frame_buffer0 = cursor;
    cursor += frame_bytes;
    config.frame_buffer1 = cursor;
    cursor += frame_bytes;
    config.x0 = (sint16)x0;
    config.y0 = (sint16)y0;
    config.x1 = (sint16)x1;
    config.y1 = (sint16)y1;
    movie_configure(&config);
    movie_init_decoder();
    resource_init_queue(0, 0);
    return cursor;
}

uint32 movie_queue_stream(void)
{
    uint32 state;
    uint32 flags;
    uint32 enabled;

    FUNCTION_MARKER(0x80056D7C, "ACE3.BIN");
#if AP_WEB
    state = r_u32(0x800BD048);
    flags = r_u32(r_u32(0x800BCD8C) + 4) ? 0x4000 : 0;
    game_web_movie_prepare(r_u32(state + 0x11038), state, 0);
    return sub_8002c928(r_u32(state + 0x11038), r_u32(state + 0x11040), r_u32(state + 0x1103C), flags, 0x80056C9C);
#endif
    enabled = r_u32(r_u32(0x800BCD8C) + 4);
    state = r_u32(0x800BD048);
    flags = enabled ? 0x4000 : 0;
    if (r_u8(state + 5))
        flags |= 0x800;
    return sub_8002c928(r_u32(state + 0x11038), r_u32(state + 0x11040), r_u32(state + 0x1103C), flags, 0x80056C9C);
}

uint32 sub_80058134(void)
{
    PSX_RECT capture;
    uint32 first;
    uint32 second;
    uint32 pixels;
    uint32 game;

    FUNCTION_MARKER(0x80058134, "ACE3.BIN");
    resource_init_queue(0, 0);
    DecDCToutCallback(NULL);
    r_masked_u32(0x800BCD98, 15);
    first = r_masked_u32(0x800BCD98, 15);
    r_masked_u32(0x800BCD9C, 15);
    second = r_masked_u32(0x800BCD9C, 15);
    capture.x = (sint16)first;
    capture.y = (sint16)(first >> 16);
    capture.w = (sint16)second;
    capture.h = (sint16)(second >> 16);
    VSync(0);
    pixels = r_u32(r_u32(0x800BD044) + 60);
    LoadImagePSX(&capture, (uint32 *)psx_addr(pixels, (uint32)(uint16)capture.w * (uint16)capture.h * 2));
    DrawSync(0);
    game = r_u32(0x800BCE60);
    w_u32(game, r_u32(game) & 0xFFFFFFFB);
    return 1;
}

uint32 sub_800581c4(void)
{
    uint32 game;
    uint32 state;
    uint32 buffer;
    uint32 ot;
    uint32 packet;
    uint32 index;

    FUNCTION_MARKER(0x800581C4, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    buffer = r_u32(game + 4);
    state = r_u32(0x800BD044);
    ot = state + (buffer << 3) + 12;
    w_u32(state + 32, ot);
    packet = r_u32(state + (buffer << 2) + 4);
    w_u32(state + 28, packet);
    ClearOTagR(psx_addr(ot, 8), 2);
    if (sub_80057720())
    {
        sub_80058134();
        game = r_u32(0x800BCE60);
        if (!(r_u32(game + 0x96C) & 0x10))
        {
            index = r_u8(game + 936);
            if (index == 9 || index == 21 || index == 23 || index == 25 || index == 28)
                return 0x60000007;
        }
        return r_u32(r_u32(0x800BCE60) + 928) | 0x60000000;
    }
    if (r_u32(r_u32(0x800BCD8C) + 4))
    {
        state = r_u32(0x800BD044);
        packet = sub_8002d3f4(r_u32(state + 32), r_u32(state + 28));
        w_u32(r_u32(0x800BD044) + 28, packet);
    }
    state = r_u32(0x800BD044);
    DrawOTag(psx_addr(r_u32(state + 32) + 4, 4));
    return 0;
}

uint32 sub_8002c928(uint32 id, uint32 dst, uint32 bytes, uint32 flags, uint32 callback)
{
    uint32 context;
    uint32 sequence;
    uint32 request;

    FUNCTION_MARKER(0x8002C928, "ACE3.BIN");
    context = r_u32(0x800BCBBC);
    sequence = r_u32(context + 4);
    w_u32(context + 4, sequence + 1);
    request = context + (sequence & 15) * 20 + 8;
    w_u8(request, 4);
    w_u16(request + 2, id);
    w_u32(request + 4, dst);
    w_u32(request + 8, bytes);
    w_u32(request + 12, flags);
    w_u8(request + 1, 0);
    w_u32(request + 16, callback);
    return 4;
}

uint32 movie_stream_complete(void)
{
    FUNCTION_MARKER(0x80056C9C, "ACE3.BIN");
    w_u8(r_u32(0x800BD048) + 4, 1);
    return 1;
}

uint32 movie_wait_frame(uint32 nonblocking)
{
    sint32 remaining;

    FUNCTION_MARKER(0x80056DDC, "ACE3.BIN");
#if AP_WEB
    (void)nonblocking;
    return game_web_movie_poll(r_u32(0x800BD048));
#endif
    if (nonblocking)
        return sub_800572c8(r_u32(0x800BD048)) != 0;
    remaining = 120;
    for (;;)
    {
        if (sub_800572c8(r_u32(0x800BD048)))
            return 1;
        sub_8002c4dc();
        VSync(0);
        if (--remaining < 0)
        {
#ifndef NDEBUG
            printf("GMV : timeout VLC\n");
#endif
            w_u8(r_u32(0x800BD048) + 4, 1);
            return 1;
        }
    }
}

uint32 movie_end(void)
{
    FUNCTION_MARKER(0x80056E8C, "ACE3.BIN");
#if AP_WEB
    game_web_movie_close();
#endif
    resource_init_queue(0, 0);
    DecDCToutCallback(NULL);
    return 1;
}

uint32 movie_decode(uint32 mode)
{
    uint32 state;
    uint32 index;
    uint32 input;
    uint32 format;
    uint32 output;
    sint32 remaining;
    sint32 words;

    FUNCTION_MARKER(0x80056EB8, "ACE3.BIN");
#if AP_WEB
    (void)mode;
    return game_web_movie_decode(r_u32(0x800BD048));
#endif
    state = r_u32(0x800BD048);
    if (r_u8(state + 4))
        return 1;
    w_u8(state + 3, 0);
    state = r_u32(0x800BD048);
    index = r_u8(state);
    format = r_u8(state + 5);
    input = r_u32(state + (index << 2) + 8);
    DecDCTin(psx_addr(input, 4 + ((uint32)r_u16(input) << 2)), format | 2);
    state = r_u32(0x800BD048);
    index = r_u8(state + 1);
    words = r_s16(state + 0x11036);
    output = r_u32(state + (index << 2) + 16);
    DecDCTout(psx_addr(output, ((uint32)words & ~31u) * 4), words);
    remaining = 0x40000;
    while (!sub_800572c8(r_u32(0x800BD048)))
    {
        if (r_u8(r_u32(0x800BD048) + 4) == 1)
            break;
        if (--remaining <= 0)
            break;
    }
    if (remaining <= 0)
    {
#ifndef NDEBUG
        printf("GMV : timeout VLC\n");
#endif
        w_u8(r_u32(0x800BD048) + 4, 1);
    }
    sub_80057084(r_u32(0x800BD048), mode);
    return 0;
}

uint32 sub_80057720(void)
{
    uint32 state;
    uint32 phase;
    uint32 context;
    uint32 arena;
    uint32 stream_bytes;
    uint32 index;
    uint32 entry;
    uint32 flags;
    uint32 mode;
    uint32 value;
    sint32 x0;
    sint32 y0;
    sint32 x1;
    sint32 y1;
    sint32 width;
    sint32 height;

    FUNCTION_MARKER(0x80057720, "ACE3.BIN");
    state = r_u32(0x800BD044);
    phase = r_u32(state);
    switch (phase)
    {
        case 0:
            arena = arena_peek(0);
            state = r_u32(0x800BD044);
            stream_bytes = r_u32(state + 56) ? 0x2100 : 0x1600;
            state = r_u32(0x800BD044);
            x0 = r_s16(state + 36) + r_s16(state + 48);
            y0 = r_s16(state + 38) + r_s16(state + 50);
            x1 = r_s16(state + 40) + r_s16(state + 48);
            y1 = r_s16(state + 42) + r_s16(state + 50);
            movie_begin(r_u32(state + 52), r_u32(state + 56), arena, 0x12300, 0x1B804, stream_bytes, x0, y0, x1, y1);
            movie_queue_stream();
            w_u32(r_u32(0x800BD044), 1);
            return 0;
        case 1:
            if (r_u32(r_u32(0x800BCE60) + 4))
                return 0;
            if (movie_wait_frame(1))
            {
                w_u32(r_u32(0x800BD044), 2);
                goto submit_frame;
            }
            state = r_u32(0x800BD044);
#if !AP_WEB
            value = r_u32(state + 64) - 1;
            w_u32(state + 64, value);
            if ((sint32)value <= 0)
            {
#ifndef NDEBUG
                printf("GMV : FirstSync timeout\n");
#endif
                w_u8(r_u32(0x800BD048) + 4, 1);
            }
#endif
            if (!r_u8(r_u32(0x800BD048) + 4))
                return 0;
            if (r_u32(r_u32(0x800BCD8C) + 4))
                sub_8002cc44(255, 0, 0);
            movie_end();
            w_u32(r_u32(0x800BD044), 3);
            return 0;
        case 2:
            if (r_u32(r_u32(0x800BCE60) + 300) & 0x40)
                w_u8(r_u32(0x800BD048) + 4, 1);
            if (!movie_decode(0))
                goto submit_frame;
            movie_end();
            index = movie_current_buffer();
            state = r_u32(0x800BD044);
            entry = state + (index << 2);
            w_u16(0x800BD038, r_u16(entry + 36));
            w_u16(0x800BD03A, r_u16(entry + 38));
            w_u16(0x800BD03E, r_u16(state + 46));
            mode = r_u32(state + 56);
            w_u32(0x800BCD90, index);
            if (mode)
                width = (r_s16(state + 44) * 3) >> 1;
            else
                width = r_u16(state + 44);
            w_u16(0x800BD03C, width);
            ClearImage((PSX_RECT *)psx_addr(0x800BD038, 8), 1, 1, 1);
            if (r_u32(r_u32(0x800BCD8C) + 4))
                sub_8002cc44(255, 0, 0);
            w_u32(r_u32(0x800BD044), 3);
            return 0;
        case 3:
            w_u32(r_u32(0x800BD044), 4);
            return 0;
        case 4:
            return 1;
        default:
            return 0;
    }

submit_frame:
    index = movie_current_buffer();
    context = r_u32(0x800BCE48);
    flags = r_u32(context);
    w_u32(context, flags | 0x30);
    state = r_u32(0x800BD044);
    entry = state + (index << 2);
    x0 = r_s16(entry + 36);
    y0 = r_s16(entry + 38);
    width = r_s16(state + 44);
    height = r_s16(state + 46);
    w_u32(0x800BCD90, index);
    SetDefDispEnv((DISPENV *)psx_addr(context + 24, 20), x0, y0, width, height);
    w_u8(r_u32(0x800BCE48) + 40, 0);
    state = r_u32(0x800BD044);
    context = r_u32(0x800BCE48);
    value = r_u8(state + 56);
    w_u8(context + 41, value);
    context = r_u32(0x800BCE48);
    state = r_u32(0x800BD044);
    value = r_u16(context + 8);
    mode = r_u16(context + 12);
    w_u16(context + 32, value);
    w_u16(context + 34, mode);
    mode = r_u32(state + 56);
    index ^= 1;
    entry = state + (index << 2);
    x0 = r_s16(entry + 36);
    y0 = r_s16(entry + 38);
    width = r_s16(state + 44);
    if (mode)
        width = (width * 3) >> 1;
    height = r_s16(state + 46);
    SetDefDrawEnv((DRAWENV *)psx_addr(context + 44, 92), x0, y0, width, height);
    w_u8(r_u32(0x800BCE48) + 68, 1);
    w_u8(r_u32(0x800BCE48) + 69, 0);
    w_u8(r_u32(0x800BCE48) + 70, 0);
    w_u8(r_u32(0x800BCE48) + 71, 0);
    return 0;
}

uint32 resource_poll_frame(uint32 *frame, sint32 *auxiliary)
{
    uint32 context;
    uint32 entry;
    sint32 consumed;
    sint32 queued;

    FUNCTION_MARKER(0x8002C980, "ACE3.BIN");
    context = r_u32(0x800BCBBC);
    consumed = r_s32(context);
    queued = r_s32(context + 4);
    if (consumed >= queued || r_u32(context + 328))
        return 0;
    entry = sub_8002b5b0();
    if (!entry || !r_u16(entry))
        return 0;
    if (frame)
        *frame = r_u32(entry + 4);
    if (auxiliary)
        *auxiliary = r_s16(entry + 2);
    return 1;
}

uint32 sub_8002cd24(void)
{
    uint32 i;
    uint32 context;
    uint32 value;
    uint32 changes = 0;
    uint32 descriptor;
    uint32 counter;
    sint32 threshold;

    FUNCTION_MARKER(0x8002CD24, "ACE3.BIN");
    for (i = 0; i < 3; ++i)
    {
        context = r_u32(0x800BCE88);
        value = r_u8(0x800BCE94 + i);
        if (value == r_u8(context + i + 7))
        {
            descriptor = 0x800B03AC + r_u32(context) * 20;
            threshold = r_u8(descriptor + 14) * 30;
            counter = r_u16(context + i * 2 + 10);
            if (r_s16(context + i * 2 + 10) >= threshold)
                ++changes;
            else if (!(r_u32(r_u32(0x800BCE60)) & 0x20000000))
                w_u16(context + i * 2 + 10, counter + 1);
        }
        else
        {
            w_u8(context + i + 7, value);
            context = r_u32(0x800BCE88);
            value = r_u8(0x800BCE94 + i);
            w_u8(context + i + 4, value);
            context = r_u32(0x800BCE88);
            changes = i == 0 ? 128 : 0;
            w_u16(context + i * 2 + 10, 0);
        }
    }
    if ((changes & 255) == 3)
    {
        for (i = 0; i < 3; ++i)
            w_u8(r_u32(0x800BCE88) + i + 4, 0);
    }
    else if (changes & 128)
    {
        for (i = 1; i < 3; ++i)
            w_u8(r_u32(0x800BCE88) + i + 4, 0);
    }
    return 0;
}

uint32 sub_8002d3f4(uint32 ot, uint32 packet)
{
    uint32 frame;
    uint32 context;
    uint32 id;

    FUNCTION_MARKER(0x8002D3F4, "ACE3.BIN");
    if (resource_poll_frame(&frame, 0))
    {
        if (frame != 0xFFFFFFFF && r_u32(0x800BCE90) != frame)
        {
            w_u8(0x800BCE94, frame >> 25);
            w_u8(0x800BCE95, (frame >> 18) & 127);
            w_u8(0x800BCE96, (frame >> 11) & 127);
            w_u8(0x800BCE8E, (frame >> 5) & 63);
            w_u8(0x800BCE8F, (frame >> 2) & 7);
        }
        w_u32(0x800BCE90, frame);
    }
    sub_8002cd24();
    context = r_u32(0x800BCE88);
    id = r_u32(context);
    if ((id < 2 || id == 7 || id == 10 || id == 9) && r_u8(r_u32(0x800BCE60) + 924) != 1)
        return packet;
    if (r_u8(0x800BCE8C) != 1)
        return packet;
    id = r_u32(r_u32(0x800BCE88));
    if (id == 9)
        return sub_8002d330(ot, packet);
    if (id == 10)
        return sub_8002d10c(ot, packet);
    return sub_8002ce70(ot, packet);
}

uint32 sub_80057e34(void)
{
    uint32 state;
    uint32 resource;
    uint32 table;

    FUNCTION_MARKER(0x80057E34, "ACE3.BIN");
    state = r_u32(0x800BCD8C);
    if (!r_u32(state + 4))
        return 0x04000000;
    resource = r_u32(state);
    if (!resource)
        return 0x04000000;
    if (r_u32(state + 8))
        return 0;
    table = sub_80027338(resource, 0);
    resource = sub_80027338(r_u32(table), 0);
    table = r_u32(table);
    sub_8002722c(resource, r_u32(table));
    DrawSync(0);
    return 0x04000000;
}

uint32 sub_8002cc44(uint32 id, uint32 argument0, uint32 argument1)
{
    uint32 i;
    uint32 context = 0;
    uint32 descriptor;
    uint32 value;

    FUNCTION_MARKER(0x8002CC44, "ACE3.BIN");
    w_u8(0x800BCE96, 0);
    w_u8(0x800BCE95, 0);
    w_u8(0x800BCE94, 0);
    w_u8(0x800BCE8F, 0);
    w_u8(0x800BCE8E, 0);
    for (i = 0; i < 3; ++i)
    {
        w_u8(r_u32(0x800BCE88) + i + 4, 0);
        w_u8(r_u32(0x800BCE88) + i + 7, 0);
        context = r_u32(0x800BCE88);
        w_u16(context + i * 2 + 10, 0);
    }
    if (id == 255)
    {
        w_u8(0x800BCE8C, 0);
        return 0;
    }
    descriptor = 0x800B03AC + id * 20;
    w_u8(0x800BCE8C, 1);
    w_u32(context, id);
    w_u32(context + 16, argument0);
    w_u32(context + 20, argument1);
    value = r_u8(descriptor + 2);
    w_u16(context + 24, value * 4);
    value = r_u8(descriptor + 2);
    w_u16(context + 26, value * 6);
    value = r_u16(descriptor + 16);
    w_u32(0x800BCE90, 0xFFFFFFFF);
    w_u16(context + 28, ((value & 0x3FF) >> 6) | 0x30);
    return 0;
}

uint32 sub_80057ebc(void)
{
    PSX_RECT capture;
    uint32 game;
    uint32 context;
    uint32 state;
    uint32 index;
    uint32 frames;
    uint32 address;
    uint32 pixels;
    uint32 first;
    uint32 second;

    FUNCTION_MARKER(0x80057EBC, "ACE3.BIN");
    game = r_u32(0x800BCE60);
    index = r_u8(game + 936);
    if (index == 7 || index == 16 || index == 12)
        return 0x60000006;
    if (index == 29)
        return 0x60000008;
    if (index == 30)
        return 0x60000007;
    frames = r_u32(0x800BD040);
    if (!frames)
    {
        context = r_u32(0x800BCE48);
        w_u8(context + 16, 0);
        state = r_u32(0x800BCE48);
        w_u32(context + 4, 0);
        w_u32(context + 20, 0);
        w_u8(state + 17, 0);
        w_u8(r_u32(0x800BCE48) + 18, 0);
        context = r_u32(0x800BCE48);
        state = r_u32(context);
        game = r_u32(0x800BCE60);
        w_u32(context, state | 9);
        w_u32(game, r_u32(game) | 0x10000004);
        frames = r_u32(0x800BD040);
    }
    ++frames;
    w_u32(0x800BD040, frames);
    if ((sint32)frames < 3)
        return 0;
    game = r_u32(0x800BCE60);
    if (!r_u32(game + 4))
        return 0;
    w_u32(0x800BD044, arena_alloc(68));
    address = arena_alloc(1024);
    context = r_u32(0x800BD044);
    w_u32(context + 4, address);
    address = arena_alloc(1024);
    context = r_u32(0x800BD044);
    game = r_u32(0x800BCE60);
    w_u32(context + 8, address);
    w_u16(context + 44, 320);
    w_u32(context, 0);
    w_u16(context + 36, 0);
    w_u16(context + 38, 0);
    w_u16(context + 40, 0);
    w_u16(context + 42, 256);
    w_u16(context + 46, 240);
    w_u16(context + 48, 0);
    w_u16(context + 50, 32);
    index = r_u8(game + 936);
    state = r_u32(0x800BCD94);
    w_u32(0x800BCE80, 0);
    address = (uint32)r_s16(state + index * 8);
    w_u32(context + 56, 1);
    w_u32(context + 64, 120);
    w_u32(context + 52, address);
    r_masked_u32(0x800BCD98, 15);
    first = r_masked_u32(0x800BCD98, 15);
    r_masked_u32(0x800BCD9C, 15);
    second = r_masked_u32(0x800BCD9C, 15);
    capture.x = (sint16)first;
    capture.y = (sint16)(first >> 16);
    capture.w = (sint16)second;
    capture.h = (sint16)(second >> 16);
    pixels = arena_alloc(0x26C00);
    context = r_u32(0x800BD044);
    w_u32(context + 60, pixels);
    StoreImage(&capture, (uint32 *)psx_addr(pixels, (uint32)(uint16)capture.w * (uint16)capture.h * 2));
    DrawSync(0);
    w_u16(0x800BD038, 0);
    w_u16(0x800BD03A, 0);
    w_u16(0x800BD03C, 480);
    w_u16(0x800BD03E, 240);
    ClearImage((PSX_RECT *)psx_addr(0x800BD038, 8), 1, 1, 1);
    w_u16(0x800BD038, 0);
    w_u16(0x800BD03A, 256);
    ClearImage((PSX_RECT *)psx_addr(0x800BD038, 8), 1, 1, 1);
    DrawSync(0);
    if (r_u32(r_u32(0x800BCD8C) + 4))
        sub_8002cc44(9, 0, 0);
    return 0x04000000;
}

uint32 sub_8002b5b0(void)
{
    uint32 channel;
    uint32 context;

    FUNCTION_MARKER(0x8002B5B0, "ACE3.BIN");
    channel = r_u8(r_u32(0x800BCBAC) + 29);
    if (channel >= 32)
        return 0;
    context = r_u32(0x800BCBB0);
    if (!(r_u32(context) & ((uint32)1 << channel)))
        return 0;
    return context + (channel << 4) + 4;
}

uint32 sub_8002b610(uint32 buffer, uint32 bytes, uint32 mode)
{
    uint32 sector_bytes;
    uint32 context;
    uint32 payload;
    uint32 headers;
    sint32 count;

    FUNCTION_MARKER(0x8002B610, "ACE3.BIN");
    sector_bytes = mode ? 0x918 : 0x800;
    count = (sint32)bytes / (sint32)sector_bytes;
    context = r_u32(0x800BCBB4);
    w_u32(context, 8);
    w_u32(context + 16, buffer);
    w_u32(context + 8, mode);
    w_u32(context + 24, 0);
    w_u32(context + 40, 0);
    w_u32(context + 52, 0);
    w_u32(context + 56, 0);
    w_u32(context + 12, (uint32)count);
    w_u32(context + 20, buffer + (uint32)count * 32);
    w_u32(context + 28, buffer);
    payload = r_u32(context + 20);
    w_u32(context + 4, (sector_bytes - 32) >> 2);
    w_u32(context + 32, payload);
    headers = r_u32(context + 28);
    payload = r_u32(context + 32);
    w_u32(context + 36, 0xFFFFFFFF);
    w_u32(context + 44, headers);
    w_u32(context + 48, payload);
    w_u32(context + 60, headers);
    w_u32(context + 64, payload);
    return 0xFFFFFFFF;
}

uint32 sub_8002b6d0(void)
{
    uint32 context;
    uint32 cursor;
    uint32 words;
    uint32 stride;
    uint32 i = 0;
    sint32 count;

    FUNCTION_MARKER(0x8002B6D0, "ACE3.BIN");
    context = r_u32(0x800BCBB4);
    cursor = r_u32(context + 16);
    count = r_s32(context + 12);
    words = r_u32(context);
    if (count <= 0)
        return words;
    stride = words << 2;
    do
    {
        w_u8(cursor, 0);
        context = r_u32(0x800BCBB4);
        ++i;
        count = r_s32(context + 12);
        cursor += stride;
    } while ((sint32)i < count);
    return 0;
}

uint32 sub_8002b588(void)
{
    FUNCTION_MARKER(0x8002B588, "ACE3.BIN");
    return r_u32(r_u32(0x800BCBAC));
}

uint32 sub_8002b59c(void)
{
    FUNCTION_MARKER(0x8002B59C, "ACE3.BIN");
    return r_u32(r_u32(0x800BCBAC) + 64);
}

uint32 sub_8002cae8(uint32 id, uint32 context)
{
    uint32 consumed;
    uint32 queued;

    FUNCTION_MARKER(0x8002CAE8, "ACE3.BIN");
    if (r_u32(context + 372) != id)
        return 0xFFFFFFFF;
    consumed = r_u32(context);
    queued = r_u32(context + 4);
    if ((sint32)consumed >= (sint32)queued || r_u32(context + 328))
        return 0xFFFFFFFF;
    if ((sub_8002b588() & 0x81000000) != 0x81000000)
        return 0xFFFFFFFF;
    return sub_8002b59c();
}

uint32 cd_stream_get_frame(uint32 *payload, uint32 *header_out, uint32 guest_payload, uint32 guest_header)
{
    uint32 stream;
    uint32 ring;
    uint32 flags;
    uint32 channel;
    uint32 header;
    uint32 status;
    uint32 value;
    uint32 slot;
    uint32 i;
    uint32 count;
    uint32 total;
    uint32 capacity;
    uint32 header_words;
    uint32 payload_words;
    uint32 headers;
    uint32 data;

    FUNCTION_MARKER(0x8002B9EC, "ACE3.BIN");
    stream = r_u32(0x800BCBAC);
    if (r_u32(stream + 32) != 64 || r_s32(r_u32(0x800BCBB4) + 52) <= 0)
    {
        flags = r_u32(stream);
        channel = r_u8(stream + 29);
        if (flags & 0x8000)
        {
            slot = r_u32(0x800BCBB0);
            value = r_u32(slot) & ~((uint32)1 << (channel & 31));
            w_u32(slot, value);
        }
        return 1;
    }
    ring = r_u32(0x800BCBB4);
    header = r_u32(ring + 60);
    status = r_u8(header);
    if (status == 4)
    {
        w_u8(header, 0);
        ring = r_u32(0x800BCBB4);
        headers = r_u32(ring + 16);
        w_u32(ring + 60, headers);
        header = headers;
        data = r_u32(ring + 20);
        w_u32(ring + 56, 0);
        w_u32(ring + 64, data);
        status = r_u8(header);
    }
    if (status != 2)
        return 1;
    stream = r_u32(0x800BCBAC);
    if (r_u32(stream) & 0x8000)
    {
        channel = r_u8(stream + 29);
        slot = r_u32(0x800BCBB0);
        value = r_u32(slot);
        w_u32(slot, value | ((uint32)1 << (channel & 31)));
        slot += (channel << 4) + 4;
        for (i = 0; i < 4; ++i)
            w_u32(slot + i * 4, r_u32(header + 16 + i * 4));
    }
    ring = r_u32(0x800BCBB4);
    value = r_u32(ring + 60);
    if (header_out)
        *header_out = value;
    else
        w_u32(guest_header, value);
    ring = r_u32(0x800BCBB4);
    value = r_u32(ring + 64);
    if (payload)
        *payload = value;
    else
        w_u32(guest_payload, value);
    ring = r_u32(0x800BCBB4);
    count = r_u8(header + 2);
    total = r_u32(ring + 56);
    capacity = r_u32(ring + 12);
    header_words = r_u32(ring);
    total += count;
    w_u32(ring + 56, total);
    payload_words = r_u32(ring + 4);
    if ((sint32)total >= (sint32)capacity)
    {
        headers = r_u32(ring + 16);
        data = r_u32(ring + 20);
        w_u32(ring + 56, 0);
        w_u32(ring + 60, headers);
        w_u32(ring + 64, data);
    }
    else if (r_u8(header + 2))
    {
        i = 0;
        do
        {
            headers = r_u32(ring + 60);
            data = r_u32(ring + 64);
            headers += header_words << 2;
            data += payload_words << 2;
            w_u32(ring + 60, headers);
            w_u32(ring + 64, data);
            count = r_u8(header + 2);
            ++i;
        } while (i < count);
    }
    ring = r_u32(0x800BCBB4);
    w_u32(ring + 52, r_u32(ring + 52) - 1);
    return 0;
}

uint32 cd_stream_free_frame(uint32 header)
{
    uint32 cursor = header;
    uint32 count;
    uint32 index = 0;
    uint32 value;
    uint32 first;
    uint32 second;

    FUNCTION_MARKER(0x8002BBE0, "ACE3.BIN");
    count = r_u8(cursor + 2);
    while (index < count)
    {
        value = r_u8(cursor);
        if (value != 2)
        {
            first = r_u8(cursor + 1);
            second = r_u8(cursor + 2);
#ifndef NDEBUG
            printf("ERR : KCdStFreeRing %d:(%08x:%08x) %02x %02x %02x\n", (sint32)(index), (uint32)(header), (uint32)(cursor), (uint32)(value), (uint32)(first), (uint32)(second));
#endif
#ifndef NDEBUG
            printf("      %08x\n", (uint32)(r_u32(r_u32(0x800BCBB4) + 44)));
#endif
            CdControlB(9, NULL, (uint8 *)psx_addr(r_u32(0x800BCBA8) + 40, 8));
            StopCallback();
            return 1;
        }
        w_u8(cursor, 0);
        value = r_u32(r_u32(0x800BCBB4));
        ++index;
        cursor += value << 2;
    }
    return 0;
}

uint32 sub_800572c8(uint32 state)
{
    uint32 payload;
    uint32 header;
    uint32 upper;
    uint32 index;
    uint32 output;
    sint32 height;
    sint32 width;
    sint32 stride;

    FUNCTION_MARKER(0x800572C8, "ACE3.BIN");
    if (cd_stream_get_frame(&payload, &header, 0, 0))
        return 0;
    if (!r_u16(header + 6))
        w_u8(state + 4, 1);
    upper = state + 0x10000;
    height = r_s16(upper + 0x1032);
    w_u16(upper + 0x1034, r_u16(header + 6));
    stride = r_s16(header + 10);
    if (height != stride)
    {
        w_u16(upper + 0x1032, stride);
        height = r_s16(state + 44);
        w_u16(upper + 0x1036, (stride * height) >> 1);
        w_u16(state + 46, stride);
        w_u16(state + 38, stride);
        w_u16(state + 30, stride);
    }
    height = r_s16(upper + 0x1030);
    width = r_s16(header + 8);
    if (height != width)
    {
        w_u16(upper + 0x1030, width);
        if (r_u8(state + 5))
            width = (width * 3) >> 1;
        w_u16(state + 36, width);
        w_u16(state + 28, width);
    }
    index = r_u8(state) ^ 1;
    w_u8(state, index);
    output = r_u32(state + ((index & 255) << 2) + 8);
    DecDCTvlc2(psx_addr(payload, (uint32)r_u8(header + 2) * r_u32(r_u32(0x800BCBB4) + 4) * 4), psx_addr(output, ((uint32)r_u16(payload) + 1) * 4), psx_addr(state + 48, sizeof(DECDCTTAB)));
    cd_stream_free_frame(header);
    return 1;
}

uint32 sub_80057084(uint32 state, uint32 nonblocking)
{
    sint32 remaining = 0x40000;
    uint32 stopped;
    uint32 index;
    uint32 entry;
    uint32 x;
    uint32 y;

    FUNCTION_MARKER(0x80057084, "ACE3.BIN");
    if (nonblocking)
        return r_u8(state + 3);
    while (!r_u8(state + 3))
    {
        stopped = r_u8(r_u32(0x800BD048) + 4);
        --remaining;
        if (remaining <= 0 || stopped)
        {
            if (remaining <= 0)
            {
#ifndef NDEBUG
                printf("GMV : timeout DCT\n");
#endif
            }
            index = r_u8(state + 2) == 0;
            w_u8(state + 2, index);
            entry = state + (index << 3);
            w_u8(state + 3, 1);
            index = r_u8(state + 2);
            x = r_u16(entry + 24);
            entry = state + (index << 3);
            w_u16(state + 40, x);
            y = r_u16(entry + 26);
            w_u16(state + 42, y);
        }
    }
    return 1;
}

void movie_output_callback(void)
{
    uint32 state;
    uint32 index;
    uint32 buffer;
    uint32 entry;
    uint32 value;
    uint32 x;
    uint32 y;
    uint32 bytes;
    sint32 end;
    sint32 next_x;
    sint32 words;

    FUNCTION_MARKER(0x80057170, "ACE3.BIN");
    state = r_u32(0x800BD048);
    if (r_u8(state + 5) && r_s32(0x800BCE80) > 0)
    {
        cd_audio_sector();
        w_u32(0x800BCE80, 0);
    }
    state = r_u32(0x800BD048);
    index = r_u8(state + 1);
    buffer = r_u32(state + (index << 2) + 16);
    bytes = (uint32)r_u16(state + 44) * r_u16(state + 46) * 2;
    LoadImagePSX((PSX_RECT *)psx_addr(state + 40, 8), (uint32 *)psx_addr(buffer, bytes));
    state = r_u32(0x800BD048);
    w_u8(state + 1, r_u8(state + 1) ^ 1);
    state = r_u32(0x800BD048);
    x = r_u16(state + 40);
    value = r_u16(state + 44);
    x += value;
    index = r_u8(state + 2);
    w_u16(state + 40, x);
    entry = state + (index << 3);
    end = r_s16(entry + 24) + r_s16(entry + 28);
    next_x = (sint16)x;
    if (next_x < end)
    {
        index = r_u8(state + 1);
        words = r_s16(state + 0x11036);
        buffer = r_u32(state + (index << 2) + 16);
        DecDCTout(psx_addr(buffer, ((uint32)words & ~31u) * 4), words);
        return;
    }
    w_u8(state + 3, 1);
    state = r_u32(0x800BD048);
    w_u8(state + 2, r_u8(state + 2) ^ 1);
    state = r_u32(0x800BD048);
    index = r_u8(state + 2);
    x = r_u16(state + (index << 3) + 24);
    index = r_u8(state + 2);
    entry = state + (index << 3);
    w_u16(state + 40, x);
    y = r_u16(entry + 26);
    w_u16(state + 42, y);
}

uint32 sub_8002d330(uint32 ot, uint32 packet)
{
    uint32 context;
    uint32 y;
    uint32 glyph;
    uint32 tag;
    uint32 head;

    FUNCTION_MARKER(0x8002D330, "ACE3.BIN");
    context = r_u32(0x800BCE88);
    if (!r_u8(context + 4))
        return packet;
    y = r_u32(0x800BCD90) == 1 ? 470 : 214;
    w_u32(packet, 0x05000000);
    w_u32(packet + 4, 0x01000000);
    w_u32(packet + 8, 0x80000000);
    glyph = r_u8(context + 4);
    w_u32(packet + 20, 0x000A0180);
    w_u32(packet + 16, (y << 16) | 0x36);
    glyph = (glyph - 1) * 5;
    tag = r_u32(packet);
    w_u32(packet + 12, (glyph << 17) | 0x240);
    head = r_u32(ot);
    w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
    head = r_u32(ot);
    w_u32(ot, (head & 0xFF000000) | (packet & 0xFFFFFF));
    return packet + 24;
}

uint32 resource_stream_request(uint32 request)
{
    uint32 phase;
    uint32 context;
    uint32 flags;
    uint32 entry;
    uint32 result;
    uint32 id;

    FUNCTION_MARKER(0x8002C1D4, "ACE3.BIN");
#if AP_WEB
    if (game_web_movie_matches((uint32)r_s16(request + 2)))
        return game_web_movie_request(request);
#endif
    phase = r_u8(request + 1);
    if (!phase)
    {
        context = r_u32(0x800BCBBC);
        flags = r_u32(context + 352);
        w_u32(context + 356, 255);
        sub_800299d8(255, flags);
        id = (uint32)r_s16(request + 2);
        flags = r_u32(request + 12);
        if (~id & 0x1000)
            flags &= 0xFFFFEFFF;
        context = r_u32(0x800BCBBC);
        w_u32(context + 360, 0);
        id = (uint32)r_s16(request + 2);
        w_u32(context + 372, id);
        entry = resource_get_entry((uint32)r_s16(request + 2));
        result = (uint32)resource_begin_read(entry, r_u32(request + 4), r_u32(request + 8), 0x8002BDB0, flags, 0);
        if (result == 1)
            w_u8(request + 1, 1);
        return 0;
    }
    if (phase != 1)
        return 0;
    context = r_u32(0x800BCBBC);
    if (!r_u32(context + 360))
        return 0;
    if (r_u32(context + 364) == 0xFFFFFFFE)
    {
        w_u8(request + 1, 0);
        return 0;
    }
    w_u32(context + 372, 0xFFFFFFFF);
    return 1;
}

uint32 resource_audio_request(uint32 request)
{
    uint32 context;
    uint32 descriptor;
    uint32 entry;
    uint32 entry_words[2];
    uint32 phase;
    uint32 id;
    uint32 flags;
    uint32 value;
    uint32 elapsed;
    uint32 game;
    uint32 numerator;
    uint32 shift;
    sint32 offset;
    sint32 duration;
    sint32 volume;

    FUNCTION_MARKER(0x8002BEE4, "ACE3.BIN");
    context = r_u32(0x800BCBBC);
    if (r_u32(context + 376))
        return 0;
    phase = r_u8(request + 1);
    descriptor = r_u32(request + 4);
    switch (phase)
    {
        case 0:
            w_u32(context + 356, r_s16(descriptor + 2) ? 0 : 255);
            context = r_u32(0x800BCBBC);
            sub_800299d8(r_s32(context + 356), r_u32(context + 352));
            value = r_u32(request + 8);
            id = (uint32)r_s16(descriptor);
            if (!value)
                id &= 0xFFFFEFFF;
            context = r_u32(0x800BCBBC);
            w_u32(context + 372, id);
            entry = resource_get_entry(id);
            entry_words[0] = r_u32(entry);
            entry_words[1] = r_u32(entry + 4);
            offset = r_s16(descriptor + 6);
            entry_words[1] += (uint32)offset;
            shift = r_u8(entry + 3);
            offset = r_s16(descriptor + 6);
            value = (entry_words[0] & 0x7FFFF) - (uint32)(offset >> (shift & 7));
            entry_words[0] = (entry_words[0] & 0xFFF80000) | (value & 0x7FFFF);
            context = r_u32(0x800BCBBC);
            w_u32(context + 360, 0);
            if (resource_begin_read(0, 0, 0, 0x8002BDB0, id & 0x1000, entry_words) != 1)
                return 0;
            if (!r_s16(descriptor + 2))
            {
                w_u8(request + 1, 2);
                return 0;
            }
            w_u8(request + 1, 1);
            game = r_u32(0x800BCE60);
            context = r_u32(0x800BCBBC);
            w_u32(context + 368, r_u32(game + 32));
            return 0;
        case 1:
            if (sub_8002b588() & 0x01000000)
            {
                game = r_u32(0x800BCE60);
                context = r_u32(0x800BCBBC);
                elapsed = r_u32(game + 32) - r_u32(context + 368);
                duration = r_s16(descriptor + 4);
                numerator = elapsed * 255;
                if (duration)
                {
                    if (duration == -1 && numerator == 0x80000000)
                        trap(6);
                    volume = (sint32)numerator / duration;
                }
                else
                    volume = (sint32)numerator >> 7;
                if (volume < 0)
                    volume = 0;
                if (volume >= 256)
                {
                    volume = 255;
                    w_u8(request + 1, 2);
                }
                context = r_u32(0x800BCBBC);
                flags = r_u32(context + 352);
                w_u32(context + 356, (uint32)volume);
                sub_800299d8(volume, flags);
            }
            else
            {
                game = r_u32(0x800BCE60);
                context = r_u32(0x800BCBBC);
                w_u32(context + 368, r_u32(game + 32));
            }
            if (r_u32(r_u32(0x800BCBBC) + 360))
                w_u8(request + 1, 2);
            return 0;
        case 2:
            if (!r_u32(context + 360))
                return 0;
            if (r_u32(context + 364) == 0xFFFFFFFE)
            {
                w_u8(request + 1, 0);
                return 0;
            }
            if (!r_u32(descriptor + 8))
            {
                w_u32(context + 372, 0xFFFFFFFF);
                return 1;
            }
            w_u8(request + 1, 0);
            w_u32(request + 4, r_u32(descriptor + 8));
            return 0;
        default:
            return 0;
    }
}

uint32 sub_8002ce70(uint32 ot, uint32 packet)
{
    uint32 context;
    uint32 id;
    uint32 descriptor;
    uint32 i;
    uint32 count = 0;
    uint32 glyph;
    uint32 columns;
    uint32 row;
    uint32 column;
    uint32 v;
    uint32 clut;
    uint32 page;
    uint32 value;
    uint32 tag;
    uint32 head;

    FUNCTION_MARKER(0x8002CE70, "ACE3.BIN");
    context = r_u32(0x800BCE88);
    id = r_u32(context);
    descriptor = 0x800B03AC + id * 20;
    if (!id && r_u32(r_u32(0x800BCE60) + 44) == 2 && (r_u32(r_u32(0x800BCE54)) & 1))
        return packet;
    for (i = 0; i < 3; ++i)
    {
        context = r_u32(0x800BCE88);
        glyph = r_u8(context + i + 4);
        if (!glyph)
            continue;
        columns = r_u8(descriptor + 2);
        --glyph;
        if (!columns)
            trap(7);
        row = glyph / columns;
        column = glyph % columns;
        v = column * r_u8(descriptor + 1);
        ++count;
        w_u8(packet + 3, 4);
        w_u8(packet + 7, 0x64);
        clut = (row & 7) << 6;
        page = row >> 1;
        value = r_u8(descriptor + 6);
        w_u8(packet + 7, value ? 0x66 : 0x64);
        w_u8(packet + 4, r_u8(descriptor + 3));
        w_u8(packet + 5, r_u8(descriptor + 4));
        w_u8(packet + 6, r_u8(descriptor + 5));
        w_u16(packet + 8, r_u8(descriptor + 7));
        value = r_u16(descriptor + 8 + i * 2);
        w_u8(packet + 12, 0);
        context = r_u32(0x800BCE88);
        w_u16(packet + 10, value);
        w_u16(packet + 16, r_u8(descriptor));
        w_u16(packet + 18, r_u8(descriptor + 1));
        if (r_u32(context) == 6 && i == 1)
            w_u16(packet + 18, 138);
        w_u16(r_u32(0x800BCE88) + 30, 0x4A16);
        tag = r_u32(packet);
        w_u16(packet + 14, clut + 0x4A16);
        w_u8(packet + 13, v);
        head = r_u32(ot);
        w_u32(packet, (tag & 0xFF000000) | (head & 0xFFFFFF));
        head = r_u32(ot);
        w_u32(ot, (head & 0xFF000000) | (packet & 0xFFFFFF));
        w_u8(packet + 23, 1);
        value = r_u16(r_u32(0x800BCE88) + 28);
        w_u32(packet + 24, ((value + page) & 0x9FF) | 0xE1000200);
        tag = r_u32(packet + 20);
        head = r_u32(ot);
        w_u32(packet + 20, (tag & 0xFF000000) | (head & 0xFFFFFF));
        head = r_u32(ot);
        w_u32(ot, (head & 0xFF000000) | ((packet + 20) & 0xFFFFFF));
        packet += 28;
    }
    if ((r_u8(descriptor + 15) & 0x10) && count)
    {
        value = r_u8(0x800BCE8E);
        if (value)
            return sub_8002d560(ot, packet, value - 1);
    }
    return packet;
}

uint32 resource_fade_request(uint32 request)
{
    uint32 context;
    uint32 phase;
    uint32 elapsed;
    uint32 value;
    uint32 result;
    sint32 remaining;
    sint32 volume;

    FUNCTION_MARKER(0x8002C2E4, "ACE3.BIN");
    context = r_u32(0x800BCBBC);
    if (r_u32(context + 376))
        return 0;
    phase = r_u8(request + 1);
    if (phase == 2)
    {
        result = resource_stop_cd(0);
        if (result)
            w_u32(r_u32(0x800BCBBC) + 376, 0);
        return result;
    }
    if (phase > 1)
        return 0;
    if (!phase)
    {
        value = r_u32(r_u32(0x800BCE60) + 32);
        w_u32(context + 368, value);
        w_u8(request + 1, 1);
    }
    value = r_u32(r_u32(0x800BCE60) + 32);
    context = r_u32(0x800BCBBC);
    elapsed = value - r_u32(context + 368);
    remaining = 4096 - ((sint32)(elapsed << 12) >> 6);
    if (remaining > 4096)
        remaining = 4096;
    if (remaining < 0)
        remaining = 0;
    value = r_u32(context + 356);
    volume = (sint32)(value * (uint32)remaining) >> 12;
    if (volume < 5)
    {
        w_u8(request + 1, 2);
        volume = 0;
        w_u32(r_u32(0x800BCBBC) + 356, 0);
    }
    sub_800299d8(volume, r_u32(r_u32(0x800BCBBC) + 352));
    return 0;
}
