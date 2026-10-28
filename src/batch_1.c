#include "game_functions.h"
#include <stdlib.h>
#include <string.h>

static uint32 pad_input_field;

/* Draft translations, pending final integration and runtime verification */
void game_run(void)
{
    uint32 state;
    uint32 flags;
    uint32 args;
    uint32 result;
    uint32 frame_ready;
    uint32 previous_draw_complete = 0;
    PSX_RECT previous_draw = {0};

    FUNCTION_MARKER(0x800261A8, "ACE3.BIN");
    /* The reviewed __main replacement is supplied by the native CRT */
    game_init();
    VSync(0);
    SetDispMask(1);
    for (;;)
    {
        gpu_frame_begin();
        sub_80025bc8();
        sub_80026050();
        sub_80026e84();
        args = r_u32(0x800BCE60);
        pad_update_input(args + 0x50, args + 0x120, args + 0x15C);
        sound_update();
        sub_80026350();
        sub_8002c4dc();
        state = r_u32(0x800BCE60);
        args = r_u32(state + 0xC);
        result = r_u32(state + 0x14);
        result = sub_800278a8(args, result);
        state = r_u32(0x800BCE60);
        w_u32(state + 0x14, result);
        result = GetRCnt(0xF2000001);
        state = r_u32(0x800BCE60);
        w_u32(state + 0x48, result);
        DrawSync(0);
        result = GetRCnt(0xF2000001);
        state = r_u32(0x800BCE60);
        args = r_u32(0x800BCE48);
        w_u32(state + 0x4C, result);
        VSync(r_s32(args + 0x14));
        result = VSync(-1);
        state = r_u32(0x800BCE60);
        w_u32(state + 0x20, result);
        ResetRCnt(0xF2000001);
        sub_800260c0();
        state = r_u32(0x800BCE60);
        flags = r_u32(state);
        if (flags & 8)
        {
            w_u32(state, flags & 0xFFFFFFF7);
            DrawSync(0);
        }
        frame_ready = game_frame_display_ready(&previous_draw, &previous_draw_complete);
        gpu_frame_end();
        if (frame_ready)
            gpu_present();
        state = r_u32(0x800BCE60);
        flags = r_u32(state);
        if (flags & 0x10000000)
        {
            w_u32(state, flags & 0xD3FFFFFF);
            continue;
        }
        if (flags & 0x04000000)
            w_u32(state, (flags & 0xFBFFFFFF) | 0x20000000);
        state = r_u32(0x800BCE60);
        flags = r_u32(state);
        if (flags & 0x08000000)
            w_u32(state, flags & 0xD7FFFFFF);
    }
}

void game_init(void)
{
    uint32 state;
    uint32 flags;

    FUNCTION_MARKER(0x80025F68, "ACE3.BIN");
    game_init_env();
    pad_init_profiles(r_u32(0x800BCE60) + 0x15C);
    state = r_u32(0x800BCE60);
    flags = r_u32(state + 0x120);
    w_u32(state + 0x120, flags | 1);
    game_init_devices();
    sub_80025ddc();
    sub_80025e38();
    sub_8002bcd8();
    sub_80025ea0();
    sub_8002d75c();
    sub_8002cbcc();
    sub_8002d880(1);
}

void game_init_env(void)
{
    uint32 env;
    uint32 state;
    uint32 prior;
    sint32 y;

    FUNCTION_MARKER(0x800257E8, "ACE3.BIN");
    w_u32(0x800BCE48, 0x800BD220);
    w_u32(0x800BCE60, 0x800BF2D0);
    w_u32(0x800BCE50, 0x800C039C);
    SetDefDrawEnv((DRAWENV *)psx_addr(0x800BD0D0, 92), 0, 0, 320, 240);
    SetDefDispEnv((DISPENV *)psx_addr(0x800BD1E4, 20), 0, 256, 320, 240);
    SetDefDrawEnv((DRAWENV *)psx_addr(0x800BD12C, 92), 0, 256, 320, 240);
    SetDefDispEnv((DISPENV *)psx_addr(0x800BD1F8, 20), 0, 0, 320, 240);
    SetDefDrawEnv((DRAWENV *)psx_addr(0x800BD188, 92), 0, 0, 320, 480);
    SetDefDispEnv((DISPENV *)psx_addr(0x800BD20C, 20), 0, 0, 320, 480);
    w_u8(0x800BD21C, 1);
    w_u8(0x800BD0E9, 4);
    w_u8(0x800BD0EA, 56);
    w_u8(0x800BD0EB, 72);
    w_u8(0x800BD145, 4);
    w_u8(0x800BD146, 56);
    w_u8(0x800BD147, 72);
    w_u8(0x800BD1A1, 4);
    w_u8(0x800BD1A2, 56);
    w_u8(0x800BD1A3, 72);
    w_u8(0x800BD0E8, 0);
    w_u8(0x800BD144, 0);
    w_u8(0x800BD1A0, 0);
    env = r_u32(0x800BCE48);
    w_u16(0x800BD1EC, 0xFFFE);
    w_u16(0x800BD1EE, 0);
    w_u32(env, 0);
    w_u32(env + 4, 0);
    w_u32(env + 8, r_s16(0x800BD1EC));
    y = r_s16(0x800BD1EE);
    w_u32(0x800BCE58, 0x800BD2A8);
    w_u8(env + 0x10, 0);
    prior = r_u32(0x800BCE48);
    w_u32(env + 0x14, 0);
    w_u32(env + 0xC, y);
    w_u8(prior + 0x11, 0);
    w_u8(r_u32(0x800BCE48) + 0x12, 0);
    state = r_u32(0x800BCE60);
    w_u32(state, 0);
    w_u32(state + 0x44, 0);
    w_u32(state + 4, 0);
    w_u32(state + 0x20, 0);
    w_u32(state + 0x34, 0xFFFFFFFF);
    w_u32(state + 0x38, 0xFFFFFFFF);
    w_u32(state + 0x3C, 0x80010000);
    w_u32(state + 0x40, 0x800CBFB8);
    w_u8(state + 0x398, 0xD0);
    prior = r_u32(0x800BCE60);
    w_u32(state + 0x24, 0);
    w_u32(state + 0x28, 0);
    w_u32(state + 0x2C, 0);
    w_u32(state + 0x30, 0);
    w_u16(state + 0x38C, 1);
    w_u16(state + 0x38E, 1);
    w_u16(state + 0x390, 1);
    w_u16(state + 0x392, 1);
    w_u32(state + 0x394, 1);
    w_u8(prior + 0x399, 0xD0);
    w_u8(r_u32(0x800BCE60) + 0x39A, 0);
    w_u8(r_u32(0x800BCE60) + 0x39C, 1);
    w_u8(r_u32(0x800BCE60) + 0x39D, 0);
    w_u8(r_u32(0x800BCE60) + 0x39E, 0);
    w_u8(r_u32(0x800BCE60) + 0x39F, 0);
    state = r_u32(0x800BCE60);
    w_u8(state + 0x3A4, 0);
    prior = r_u32(0x800BCE60);
    w_u32(state + 0x3A0, 0);
    w_u8(prior + 0x3A5, 0);
    w_u8(r_u32(0x800BCE60) + 0x3A6, 0);
    state = r_u32(0x800BCE60);
    w_u16(state + 0x3AE, 0);
    w_u8(state + 0x3A7, 1);
    w_u8(r_u32(0x800BCE60) + 0x3A8, 0);
    w_u8(r_u32(0x800BCE60) + 0x3A9, 1);
    w_u8(r_u32(0x800BCE60) + 0x3AA, 1);
    w_u8(r_u32(0x800BCE60) + 0x3AB, 1);
    state = r_u32(0x800BCE60);
    w_u8(state + 0x95E, 0);
    prior = r_u32(0x800BCE60);
    w_u16(state + 0x3AC, 0);
    w_u32(state + 0x3B0, 0);
    w_u32(state + 0x3B4, 0);
    w_u16(state + 0x954, 0);
    w_u8(prior + 0x158, 0);
    state = r_u32(0x800BCE60);
    w_u32(prior + 0x96C, 0);
    w_u32(prior + 0x1038, 0);
    w_u32(prior + 0x120, 0);
    w_u8(state + 0x15B, 0);
    w_u8(state + 0x15A, 0);
    w_u8(state + 0x159, 0);
    state = r_u32(0x800BCE60);
    w_u32(state + 0x1048, 0);
    w_u32(state + 0x106C, 0);
    w_u32(state + 0x4C, 0);
    w_u32(state + 0x48, 0);
    w_u32(state + 0x1068, 0);
    w_u32(state + 0x1064, 0);
    w_u32(state + 0x1060, 0);
    w_u32(state + 0x105C, 0);
    w_u32(state + 0x1058, 0);
    env = r_u32(0x800BCE50);
    w_u32(state + 0x1054, 0);
    w_u32(state + 0x1050, 0);
    w_u32(state + 0x104C, 0);
    w_u32(state + 0x1074, 0);
    w_u32(state + 0x1070, 0);
    w_u32(env + 0x70, 1);
    w_u32(env + 0x74, 0);
    game_set_arena(r_u32(state + 0x40));
}

sint32 cd_get_debug(void)
{
    FUNCTION_MARKER(0x800A2D3C, "ACE3.BIN");
    return r_s32(0x800B61B4);
}

uint32 game_set_arena(uint32 arena)
{
    uint32 state;

    FUNCTION_MARKER(0x800273FC, "ACE3.BIN");
    state = r_u32(0x800BCE60);
    w_u32(state + 0x1C, arena);
    w_u32(state + 0x18, arena);
    return state;
}

sint32 pad_init_profiles(uint32 profiles)
{
    sint32 result;

    FUNCTION_MARKER(0x80028BC0, "ACE3.BIN");
    result = pad_reset_slots();
    if (profiles)
    {
        pad_init_profile(1, profiles);
        pad_init_profile(2, profiles + 92);
        pad_init_profile(4, profiles + 184);
        pad_init_profile(8, profiles + 276);
        result = pad_init_profile(16, profiles + 368);
    }
    return result;
}

sint32 pad_reset_slots(void)
{
    uint32 slot;
    sint32 i;

    FUNCTION_MARKER(0x8002937C, "ACE3.BIN");
    slot = r_u32(0x800BCBA4);
    for (i = 5; i >= 0; --i)
    {
        w_u8(slot, 0);
        slot += 4;
    }
    return i;
}

uint32 pad_init_profile(uint32 kind, uint32 profile)
{
    uint32 axes;
    uint32 result;

    FUNCTION_MARKER(0x80028848, "ACE3.BIN");
    w_u32(profile + 0x58, 0);
    switch (kind)
    {
        case 1:
            pad_copy_bindings(profile, 0x800B01E0);
            result = r_u32(profile + 0x58);
            w_u16(profile + 0x50, 0);
            result |= 0x18;
            w_u32(profile + 0x58, result);
            return result;
        case 2:
            pad_copy_bindings(profile, 0x800B0220);
            axes = 0x800B02E0;
            break;
        case 4:
            pad_copy_bindings(profile, 0x800B0220);
            axes = 0x800B02F8;
            break;
        case 8:
            pad_copy_bindings(profile, 0x800B0260);
            axes = 0x800B0310;
            break;
        case 16:
            pad_copy_bindings(profile, 0x800B02A0);
            axes = 0x800B0328;
            break;
        default:
            return pad_copy_bindings(profile, 0x800B01E0);
    }
    pad_copy_axes(profile + 0x40, axes);
    result = r_u32(profile + 0x58) | 0x18;
    w_u32(profile + 0x58, result);
    return result;
}

uint32 pad_copy_bindings(uint32 dst, uint32 src)
{
    uint32 i;
    uint16 binding;

    FUNCTION_MARKER(0x800287BC, "ACE3.BIN");
    for (i = 0; i < 32; ++i)
    {
        binding = r_u16(src);
        src += 2;
        w_u16(dst, binding);
        dst += 2;
    }
    return 0;
}

uint32 pad_copy_axes(uint32 dst, uint32 src)
{
    uint32 words[4];
    uint32 tail;
    uint32 i;
    uint32 addr;
    uint32 left_shift;
    uint32 right_shift;
    uint32 left;
    uint32 right;

    FUNCTION_MARKER(0x800287E4, "ACE3.BIN");
    /* Preserve the LWL/LWR and SWL/SWR lane order for aliased buffers */
    for (i = 0; i < 4; ++i)
    {
        addr = src + i * 4;
        left_shift = 24 - ((addr + 3) & 3) * 8;
        right_shift = (addr & 3) * 8;
        left = r_masked_u32((addr + 3) & 0xFFFFFFFC, 15 >> (left_shift >> 3)) << left_shift;
        right = r_masked_u32(addr & 0xFFFFFFFC, (15 << (right_shift >> 3)) & 15) >> right_shift;
        words[i] = (left & ~(0xFFFFFFFF >> right_shift)) | right;
    }
    for (i = 0; i < 4; ++i)
    {
        addr = dst + i * 4;
        left_shift = 24 - ((addr + 3) & 3) * 8;
        right_shift = (addr & 3) * 8;
        w_masked_u32((addr + 3) & 0xFFFFFFFC, words[i] >> left_shift, 15 >> (left_shift >> 3));
        w_masked_u32(addr & 0xFFFFFFFC, words[i] << right_shift, (15 << (right_shift >> 3)) & 15);
    }
    for (i = 0; i < 2; ++i)
    {
        addr = src + 16 + i * 4;
        left_shift = 24 - ((addr + 3) & 3) * 8;
        right_shift = (addr & 3) * 8;
        left = r_masked_u32((addr + 3) & 0xFFFFFFFC, 15 >> (left_shift >> 3)) << left_shift;
        right = r_masked_u32(addr & 0xFFFFFFFC, (15 << (right_shift >> 3)) & 15) >> right_shift;
        words[i] = (left & ~(0xFFFFFFFF >> right_shift)) | right;
    }
    tail = words[0];
    for (i = 0; i < 2; ++i)
    {
        addr = dst + 16 + i * 4;
        left_shift = 24 - ((addr + 3) & 3) * 8;
        right_shift = (addr & 3) * 8;
        w_masked_u32((addr + 3) & 0xFFFFFFFC, words[i] >> left_shift, 15 >> (left_shift >> 3));
        w_masked_u32(addr & 0xFFFFFFFC, words[i] << right_shift, (15 << (right_shift >> 3)) & 15);
    }
    return tail;
}

void game_init_devices(void)
{
    FUNCTION_MARKER(0x80025B00, "ACE3.BIN");
    /* The original stack copies at 0x80025B10 are unused local storage */
    ResetCallback();
    cd_set_debug(0);
    while (!CdInit())
    {
    }
    CdSetDebug(0);
    VSync(3);
    ResetGraph(0);
    SetDispMask(1);
    InitGeom();
    sub_8009fd48();
    InitCARD(0);
    StartCARD();
    _bu_init();
    sub_8002654c(r_u32(0x800BCE60) + 0x50, 0x800BCB74, 2, 3);
    bios_exit_critical();
}

void bios_exit_critical(void)
{
    FUNCTION_MARKER(0x800AF3C8, "ACE3.BIN");
    xport_bios_exit_critical();
}

sint32 cd_set_debug(sint32 level)
{
    sint32 prev;

    FUNCTION_MARKER(0x800A2D28, "ACE3.BIN");
    prev = r_s32(0x800B61B4);
    w_u32(0x800B61B4, level);
    return prev;
}

uint32 cd_poll_irq(void)
{
    uint8 response[8];
    uint32 irq_port;
    uint32 irq;
    uint32 error = 0;
    uint32 count;
    uint32 i;
    uint32 port;
    uint32 command;
    uint32 status;
    uint32 code;
    uint32 name;

    FUNCTION_MARKER(0x800A34B8, "ACE3.BIN");
    w_u8(r_u32(0x800B6520), 1);
    irq_port = r_u32(0x800B652C);
    irq = r_u8(irq_port) & 7;
    if (!irq)
        return 0;
    while (irq != (r_u8(irq_port) & 7))
        irq = r_u8(irq_port) & 7;
    count = 0;
    do
    {
        port = r_u32(0x800B6520);
        if (!(r_u8(port) & 0x20))
            break;
        response[count++] = r_u8(r_u32(0x800B6524));
    } while (count < 8);
    for (i = count; i < 8; ++i)
        response[i] = 0;
    w_u8(r_u32(0x800B6520), 1);
    w_u8(r_u32(0x800B652C), 7);
    w_u8(r_u32(0x800B6528), 7);
    if (irq != 3 || r_u32(0x800B6420 + r_u8(0x800B6279) * 4))
    {
        if (!(r_u32(0x800B6268) & 0x10) && (response[0] & 0x10))
            w_u32(0x800B6270, r_u32(0x800B6270) + 1);
        error = response[0] & 0x1D;
        w_u32(0x800B6268, response[0]);
        w_u32(0x800B626C, response[1]);
    }
    if (irq == 5 && r_s32(0x800B6264) >= 3)
    {
#ifndef NDEBUG
        printf("DiskError: ");
#endif
        if (r_s32(0x800B6264) >= 3)
        {
            command = r_u8(0x800B6279);
            status = r_u32(0x800B6268);
            code = r_u32(0x800B626C);
            name = r_u32(0x800B6280 + command * 4);
#ifndef NDEBUG
            printf("com=%s,code=(%02x:%02x)\n", game_native_string(name), (uint32)(status), (uint32)(code));
#endif
        }
    }
    switch (irq)
    {
        case 1:
            if (error && count == 1)
                error = 0;
            w_u8(0x800B6539, error ? 5 : 1);
            for (i = 0; i < 8; ++i)
                w_u8(0x800C7EC8 + i, response[i]);
            w_u8(r_u32(0x800B6520), 0);
            w_u8(r_u32(0x800B652C), 0);
            return 4;
        case 2:
            w_u8(0x800B6538, error ? 5 : 2);
            for (i = 0; i < 8; ++i)
                w_u8(0x800C7EC0 + i, response[i]);
            return 2;
        case 3:
            if (error)
            {
                w_u8(0x800B6538, 5);
                for (i = 0; i < 8; ++i)
                    w_u8(0x800C7EC0 + i, response[i]);
                return 2;
            }
            if (r_u32(0x800B6320 + r_u8(0x800B6279) * 4))
            {
                w_u8(0x800B6538, 3);
                for (i = 0; i < 8; ++i)
                    w_u8(0x800C7EC0 + i, response[i]);
                return 1;
            }
            w_u8(0x800B6538, 2);
            for (i = 0; i < 8; ++i)
                w_u8(0x800C7EC0 + i, response[i]);
            return 2;
        case 4:
            w_u8(0x800B653A, 4);
            w_u8(0x800B6539, r_u8(0x800B653A));
            for (i = 0; i < 8; ++i)
                w_u8(0x800C7ED0 + i, response[i]);
            for (i = 0; i < 8; ++i)
                w_u8(0x800C7EC8 + i, response[i]);
            return 4;
        case 5:
            w_u8(0x800B6539, 5);
            w_u8(0x800B6538, r_u8(0x800B6539));
            for (i = 0; i < 8; ++i)
                w_u8(0x800C7EC0 + i, response[i]);
            for (i = 0; i < 8; ++i)
                w_u8(0x800C7EC8 + i, response[i]);
            return 6;
        default:
#ifndef NDEBUG
            fputs("CDROM: unknown intr", stdout);
#endif
#ifndef NDEBUG
            printf("(%d)\n", (sint32)(irq));
#endif
            return 0;
    }
}

sint32 bios_enter_critical(void)
{
    FUNCTION_MARKER(0x800AF3B8, "ACE3.BIN");
    return xport_bios_enter_critical();
}

void sub_8009fd48(void)
{
    FUNCTION_MARKER(0x8009FD48, "ACE3.BIN");
    SpuInit();
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

void sub_8002654c(uint32 buffers, uint32 context, uint32 mode, uint32 flags)
{
    uint32 state;
    uint32 ptr;
    uint32 i;

    FUNCTION_MARKER(0x8002654C, "ACE3.BIN");
    state = r_u32(0x800BCB84);
    for (i = 0; i < 0xD0; ++i)
        w_u8(state + i, 0);
    state = r_u32(0x800BCB84);
    w_u32(state + 0xC8, context);
    w_u32(state + 0xC4, mode);
    w_u32(state + 0xCC, flags);
    for (i = 0; i < 8; ++i)
    {
        ptr = r_u32(0x800BCB84) + 0x44 + i * 0x10;
        w_u32(ptr + 4, buffers);
        buffers += 0x1A;
        w_u8(ptr, i + (i >= 4 ? 12 : 0));
    }
    state = r_u32(0x800BCB84);
    PadInitDirect(psx_addr(state, 0x22), psx_addr(state + 0x22, 0x22));
    PadStartCom();
    pad_input_field = VSync(-1);
}

sint32 sub_80025ddc(void)
{
    FUNCTION_MARKER(0x80025DDC, "ACE3.BIN");
    geom_set_offset(0, 0);
    geom_set_screen(0x115);
    w_u32(r_u32(0x800BCE48), 11);
    sub_80025bc8();
    SetRCnt(0xF2000001, 0x1E00, 0x2000);
    return StartRCnt(0xF2000001);
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

uint32 sub_80025bc8(void)
{
    uint32 env;
    uint32 flags;
    uint32 buffers;
    uint32 mode;
    uint32 color_r;
    uint32 color_g;
    uint32 color_b;
    uint32 x;

    FUNCTION_MARKER(0x80025BC8, "ACE3.BIN");
    env = r_u32(0x800BCE48);
    flags = r_u32(env);
    if (!flags)
        return 0x800C0000;
    buffers = r_u32(0x800BCE58);
    if (flags & 0x40)
    {
        mode = r_u32(env + 4);
        w_u32(env, flags & 0xFFFFFFBF);
        if (!mode)
        {
            w_u32(buffers + 4, 0x800BD1E4);
            w_u32(buffers + 0x1018, 0x800BD1F8);
        }
        else if (mode == 1)
        {
            w_u32(buffers + 4, 0x800BD20C);
            w_u32(buffers + 0x1018, 0x800BD20C);
        }
        flags = r_u32(env);
    }
    if (flags & 1)
    {
        mode = r_u32(env + 4);
        w_u32(env, (flags & 0xFFFFFFFE) | 0x40);
        if (!mode)
        {
            w_u32(buffers, 0x800BD0D0);
            w_u32(buffers + 0x1014, 0x800BD12C);
        }
        else if (mode == 1)
        {
            w_u32(buffers, 0x800BD188);
            w_u32(buffers + 0x1014, 0x800BD188);
        }
    }
    flags = r_u32(env);
    if (flags & 0x10)
    {
        w_u32(env, flags & 0xFFFFFFEF);
        w_u32(buffers + 4, env + 0x18);
        w_u32(buffers + 0x1018, env + 0x18);
        w_u32(env, r_u32(env) | 1);
        flags = r_u32(env);
    }
    if (flags & 0x20)
    {
        w_u32(env, flags & 0xFFFFFFDF);
        w_u32(buffers, env + 0x2C);
        w_u32(buffers + 0x1014, env + 0x2C);
        w_u32(env, r_u32(env) | 1);
    }
    flags = r_u32(env);
    if (flags & 2)
    {
        x = r_u16(env + 8);
        w_u32(env, flags & 0xFFFFFFFD);
        w_u16(0x800BD1EC, x);
        w_u16(0x800BD1EE, r_u16(env + 0xC));
        w_u16(0x800BD200, r_u16(env + 8));
        w_u16(0x800BD202, r_u16(env + 0xC));
        w_u16(0x800BD214, r_u16(env + 8));
        w_u16(0x800BD216, r_u16(env + 0xC));
    }
    flags = r_u32(env);
    if (flags & 4)
    {
        w_u32(env, flags & 0xFFFFFFFB);
        w_u8(0x800BD1A0, 0);
        w_u8(0x800BD144, 0);
        w_u8(0x800BD0E8, 0);
        flags = r_u32(env);
    }
    if (!(flags & 8))
        return 0xFFFFFFF7;
    color_r = r_u8(env + 0x10);
    color_g = r_u8(env + 0x11);
    w_u32(env, flags & 0xFFFFFFF7);
    color_b = r_u8(env + 0x12);
    w_u8(0x800BD1A0, 1);
    w_u8(0x800BD144, 1);
    w_u8(0x800BD0E8, 1);
    w_u8(0x800BD0E9, color_r);
    w_u8(0x800BD0EA, color_g);
    w_u8(0x800BD0EB, color_b);
    w_u8(0x800BD145, color_r);
    w_u8(0x800BD146, color_g);
    w_u8(0x800BD147, color_b);
    w_u8(0x800BD1A1, color_r);
    w_u8(0x800BD1A2, color_g);
    w_u8(0x800BD1A3, color_b);
    return 1;
}

uint32 sub_8002bcd8(void)
{
    uint32 state;

    FUNCTION_MARKER(0x8002BCD8, "ACE3.BIN");
    sub_8002b034(r_u32(0x800BCE60) + 0x20);
    state = r_u32(0x800BCBBC);
    w_u32(state + 0x164, 255);
    w_u32(state, 0);
    w_u32(state + 4, 0);
    w_u32(state + 0x148, 0);
    w_u32(state + 0x160, 0);
    w_u32(state + 0x168, 1);
    w_u32(state + 0x16C, 0);
    w_u32(state + 0x178, 0);
    return state;
}

uint32 sub_8002b034(uint32 counter)
{
    uint32 state;
    sint32 result;

    FUNCTION_MARKER(0x8002B034, "ACE3.BIN");
    state = r_u32(0x800BCBA8);
    w_u32(state + 0x20, counter);
    w_u32(state, 0);
    w_u32(state + 4, 0);
    w_u32(state + 0xC, 0);
    w_u32(state + 8, 0);
    w_u32(state + 0x24, 0);
    sub_800299d8(255, 0);
    for (;;)
    {
        result = sub_8002ae0c();
        if (result)
            return result;
        VSync(0);
    }
}

uint32 cd_pos_to_sector(uint32 pos)
{
    uint32 minute;
    uint32 second;
    uint32 frame;

    FUNCTION_MARKER(0x800A606C, "ACE3.BIN");
    minute = r_u8(pos);
    second = r_u8(pos + 1);
    minute = (minute >> 4) * 10 + (minute & 15);
    second = (second >> 4) * 10 + (second & 15);
    frame = r_u8(pos + 2);
    frame = (frame >> 4) * 10 + (frame & 15);
    return (minute * 60 + second) * 75 + frame - 150;
}

uint32 cd_get_sector(void *dst, uint32 words)
{
    FUNCTION_MARKER(0x800A3430, "ACE3.BIN");
    return CdGetSector(dst, (sint32)words);
}

void pad_start(void)
{
    FUNCTION_MARKER(0x8009BC60, "ACE3.BIN");
    PadStartCom();
}

void sub_80025e38(void)
{
    SpuCommonAttr attr;
    CdlATV volume;

    FUNCTION_MARKER(0x80025E38, "ACE3.BIN");
    attr.mask = 0x3C3;
    attr.mvol.left = 0x3FFF;
    attr.mvol.right = 0x3FFF;
    attr.cd.volume.left = 0x7FFF;
    attr.cd.volume.right = 0x7FFF;
    attr.cd.reverb = 1;
    attr.cd.mix = 1;
    SpuSetCommonAttr(&attr);
    volume.val0 = 255;
    volume.val1 = 0;
    volume.val2 = 255;
    volume.val3 = 0;
    cd_apply_volume(&volume);
}

uint32 cd_apply_volume(CdlATV *volume)
{
    FUNCTION_MARKER(0x800A3410, "ACE3.BIN");
    CD_vol(volume);
    return 1;
}

uint32 sub_800299d8(sint32 volume, uint32 mono)
{
    CdlATV attr;

    FUNCTION_MARKER(0x800299D8, "ACE3.BIN");
    if (volume >= 151)
        volume = 150;
    else if (volume < 0)
        volume = 0;
    if (mono)
    {
        attr.val3 = volume >> 1;
        attr.val2 = attr.val3;
        attr.val1 = attr.val3;
        attr.val0 = attr.val3;
    }
    else
    {
        attr.val2 = volume;
        attr.val0 = volume;
        attr.val3 = 0;
        attr.val1 = 0;
    }
    return cd_apply_volume(&attr);
}

sint32 cd_find_files(uint32 paths, sint32 count, uint32 positions, uint32 sizes)
{
    CdlFILE file;
    uint32 path;
    uint32 len;
    uint32 pos;
    uint32 shift;
    sint32 i;

    FUNCTION_MARKER(0x8002ACD4, "ACE3.BIN");
    for (i = 0; i < count; ++i)
    {
        path = r_u32(paths);
        len = 0;
        while (r_u8(path + len))
            ++len;
        if (!CdSearchFile(&file, (const char *)psx_addr(path, (size_t)len + 1)))
        {
#ifndef NDEBUG
            printf("ERR : File Not Found.[%s]\n", game_native_string(r_u32(paths)));
#endif
            return -1;
        }
        paths += 4;
        pos = file.pos.minute | ((uint32)file.pos.second << 8) | ((uint32)file.pos.sector << 16) | ((uint32)file.pos.track << 24);
        shift = 24 - ((positions + 3) & 3) * 8;
        w_masked_u32((positions + 3) & 0xFFFFFFFC, pos >> shift, 0xFFFFFFFF >> shift);
        shift = (positions & 3) * 8;
        w_masked_u32(positions & 0xFFFFFFFC, pos << shift, 0xFFFFFFFF << shift);
        positions += 4;
        w_u32(sizes, file.size);
        sizes += 4;
    }
    return 1;
}

uint32 cd_read_sectors(uint32 count, uint32 pos, uint32 dst)
{
    uint32 result;
    uint8 response[8];

    FUNCTION_MARKER(0x8002AD90, "ACE3.BIN");
    do
    {
        result = CdControlB(2, (uint8 *)psx_addr(pos, 4), (uint8 *)psx_addr(r_u32(0x800BCBA8) + 0x28, 8));
    } while (!result);
    while (!CdRead((sint32)count, (uint32 *)psx_addr(dst, (size_t)count * 2048), 128))
    {
    }
    CdReadSync(0, response);
    return 1;
}

sint32 sub_8002ae0c(void)
{
    uint32 storage;
    uint32 state;
    uint32 header;
    uint32 other;
    uint32 entries;
    uint32 base_sector;
    uint32 entry;
    uint32 word;
    uint32 sector;
    sint32 count;
    sint32 i;
    sint32 result;

    FUNCTION_MARKER(0x8002AE0C, "ACE3.BIN");
    storage = r_u32(0x800BCBB8);
    w_u32(0x800BCE78, storage);
    w_u32(0x800BCE7C, storage + 0x2000);
    game_cd_sync_callback(0);
    CdDataCallback(NULL);
    game_cd_ready_callback(0);
    result = cd_find_files(0x800B0380, 4, 0x800C1368, 0x800C1378);
    if (result <= 0)
    {
        w_u32(r_u32(0x800BCBA8) + 0xC, 5);
        return result;
    }
    if (game_use_extracted_assets())
        game_read_archive_headers(r_u32(0x800BCE78), r_u32(0x800BCE7C));
    else
    {
        cd_read_sectors(4, 0x800C1368, r_u32(0x800BCE78));
        cd_read_sectors(4, 0x800C136C, r_u32(0x800BCE7C));
    }
    header = r_u32(0x800BCE78);
    word = r_u32(header);
    if (word == 0x45334341)
    {
        header = r_u32(0x800BCE7C);
        other = r_u32(header);
    }
    else
        other = word;
    if (other != 0x45334341)
    {
        w_u32(r_u32(0x800BCBA8) + 0xC, 5);
#ifndef NDEBUG
        printf("KCD : Error ID %08x != %08x\n", (uint32)(r_u32(header)), (uint32)(0x45334341));
#endif
        return 1;
    }
    sector = cd_pos_to_sector(0x800C1370);
    state = r_u32(0x800BCBA8);
    w_u32(state + 0x10, sector);
    sector = cd_pos_to_sector(0x800C1374);
    state = r_u32(0x800BCBA8);
    header = r_u32(0x800BCE78);
    w_u32(state + 0x14, sector);
    other = r_u32(0x800BCE7C);
    entries = header + 0x14;
    w_u32(state + 0x18, entries);
    w_u32(state + 0x1C, other + 0x14);
    count = r_s32(header + 0x10);
    base_sector = r_u32(state + 0x10);
    entry = entries + 4;
    for (i = 0; i < count; ++i)
    {
        word = r_u32(entry);
        w_u32(entry, word + base_sector);
        entry += 8;
    }
    other = r_u32(0x800BCE7C);
    state = r_u32(0x800BCBA8);
    count = r_s32(other + 0x10);
    base_sector = r_u32(state + 0x14);
    entries = r_u32(state + 0x1C);
    entry = entries + 4;
    for (i = 0; i < count; ++i)
    {
        word = r_u32(entry);
        w_u32(entry, word + base_sector);
        entry += 8;
    }
    if (count > 0)
        state = r_u32(0x800BCBA8);
    other = r_u32(0x800BCE7C);
    w_u32(state + 0xC, 0);
    word = r_u32(other + 4);
    if (word)
        w_u32(state + 0xC, word);
    header = r_u32(0x800BCE78);
    if (r_u32(header + 8) != 0x427)
    {
        w_u32(r_u32(0x800BCBA8) + 0xC, 5);
#ifndef NDEBUG
        printf("KCD : Error SBCODE != %08x\n", (uint32)(0x427));
#endif
    }
    other = r_u32(0x800BCE7C);
    if (r_u32(other + 8) != 0x19990427)
    {
        w_u32(r_u32(0x800BCBA8) + 0xC, 5);
#ifndef NDEBUG
        printf("KCD : Error SPCODE != %08x\n", (uint32)(0x19990427));
#endif
    }
    return 1;
}

uint32 sub_80025ea0(void)
{
    uint32 resource;
    uint32 bundle;
    uint32 ptr;
    uint32 data;
    uint32 header;

    FUNCTION_MARKER(0x80025EA0, "ACE3.BIN");
    resource = arena_peek(resource_get_size(0));
    resource_queue_read(0, resource, 0);
    resource_wait(0);
    bundle = sub_80027338(resource, 0);
    sub_8002d96c(r_u32(bundle + 0x10));
    data = sub_80027338(r_u32(bundle), 0);
    ptr = r_u32(bundle);
    header = r_u32(ptr);
    sub_8002722c(data, header);
    sub_800271a0(r_u32(bundle + 4));
    sub_800271a0(r_u32(bundle + 8));
    data = sub_80027338(r_u32(bundle + 0xC), 0);
    ptr = r_u32(bundle + 0xC);
    header = r_u32(ptr);
    sub_8002722c(data, header);
    DrawSync(0);
    return sub_800275c8();
}

uint32 resource_get_size(uint32 id)
{
    uint32 entry;
    uint32 word;

    FUNCTION_MARKER(0x8002C5E0, "ACE3.BIN");
    entry = resource_get_entry(id);
    word = r_u32(entry);
    if (id & 0x800)
        return (word & 0x7FFFF) << ((word >> 24) & 7);
    return (word & 0x7FFFF) << 2;
}

uint32 resource_get_entry(uint32 id)
{
    uint32 state;
    uint32 entries;

    FUNCTION_MARKER(0x8002995C, "ACE3.BIN");
    state = r_u32(0x800BCBA8);
    if (id & 0x800)
        entries = r_u32(state + 0x1C);
    else
        entries = r_u32(state + 0x18);
    return entries + (id & 0x7FF) * 8;
}

uint32 arena_peek(uint32 bytes)
{
    uint32 aligned;
    uint32 state;
    uint32 end;

    FUNCTION_MARKER(0x80027478, "ACE3.BIN");
    aligned = (bytes + 3) & 0xFFFFFFFC;
    state = r_u32(0x800BCE60);
    end = r_u32(state + 0x1C) + aligned;
    if (end <= 0x801FB700)
        return r_u32(state + 0x1C);
#ifndef NDEBUG
    printf("Error: MemNoAlloc: Overflow %d bytes (REQ: %d bytes)\n", (sint32)(end + 0x7FE04900), (sint32)(aligned));
#endif
    return 0;
}

uint32 resource_queue_read(uint32 id, uint32 dst, uint32 flags)
{
    uint32 state;
    uint32 count;
    uint32 request;
    uint32 size;
    uint32 end;

    FUNCTION_MARKER(0x8002C7AC, "ACE3.BIN");
    state = r_u32(0x800BCBBC);
    count = r_u32(state + 4);
    w_u32(state + 0x17C, dst);
    w_u32(state + 4, count + 1);
    request = state + 8 + (count & 15) * 20;
    w_u8(request, 2);
    w_u16(request + 2, id);
    w_u32(request + 4, dst);
    w_u32(request + 0x10, flags);
    w_u8(request + 1, 0);
    size = resource_get_size(id);
    end = dst + (size & 0xFFFFFFFC);
    state = r_u32(0x800BCBBC);
    w_u32(state + 0x180, end);
    return end;
}

uint32 resource_wait(uint32 mode)
{
    uint32 state;
    uint32 written;
    uint32 read;

    FUNCTION_MARKER(0x8002C580, "ACE3.BIN");
    if (mode)
    {
        state = r_u32(0x800BCBBC);
        written = r_u32(state + 4);
        read = r_u32(state);
        return written - read;
    }
    for (;;)
    {
        state = r_u32(0x800BCBBC);
        written = r_u32(state + 4);
        read = r_u32(state);
        if (written == read)
            return 0;
        sub_8002c4dc();
        VSync(0);
    }
}

uint32 sub_8002c4dc(void)
{
    uint32 state;
    uint32 read;
    uint32 written;
    uint32 result;

    FUNCTION_MARKER(0x8002C4DC, "ACE3.BIN");
    resource_poll_cd();
    state = r_u32(0x800BCBBC);
    if (r_u32(state + 0x148))
    {
        result = resource_poll_request(state + 0x14C);
        if (result)
        {
            result = r_u32(0x800BCBBC);
            w_u32(result + 0x148, 0);
        }
        return result;
    }
    read = r_u32(state);
    written = r_u32(state + 4);
    result = read & 15;
    if ((sint32)read < (sint32)written)
    {
        result = resource_poll_request(state + 8 + result * 20);
        if (result)
        {
            state = r_u32(0x800BCBBC);
            result = r_u32(state) + 1;
            w_u32(state, result);
        }
    }
    return result;
}

uint32 resource_poll_request(uint32 request)
{
    uint32 kind;
    uint32 target;
    uint32 state;
    uint32 arg;
    uint32 result;

    FUNCTION_MARKER(0x8002C45C, "ACE3.BIN");
    kind = r_u8(request);
    target = r_u32(0x800B0390 + kind * 4);
    result = xport_guest_call3(target, request, 0, 0);
    if (result)
    {
        target = r_u32(request + 0x10);
        if (target)
        {
            state = r_u32(0x800BCBBC);
            arg = r_u32(state + 0x16C);
            if (target == 0x8009B6AC)
                sub_8009b6ac();
            else if (target == 0x8009B904)
                sub_8009b904();
            else
                (void)xport_guest_call3(target, arg, 0, 0);
        }
    }
    return result;
}

uint32 resource_read_request(uint32 request)
{
    uint32 phase;
    uint32 state;
    uint32 entry;
    sint32 id;
    sint32 result;

    FUNCTION_MARKER(0x8002BE14, "ACE3.BIN");
    phase = r_u8(request + 1);
    if (!phase)
    {
        state = r_u32(0x800BCBBC);
        id = r_s16(request + 2);
        w_u32(state + 0x174, id);
        w_u32(state + 0x168, 0);
        entry = resource_get_entry(r_s16(request + 2));
        result = resource_begin_read(entry, r_u32(request + 4), 0, 0x8002BDB0, 0, 0);
        if (result == 1)
            w_u8(request + 1, 1);
        return 0;
    }
    if (phase == 1)
    {
        state = r_u32(0x800BCBBC);
        if (r_u32(state + 0x168))
        {
            if (r_u32(state + 0x16C) != 0xFFFFFFFE)
            {
                w_u32(state + 0x174, 0xFFFFFFFF);
                return 1;
            }
            w_u8(request + 1, 0);
        }
    }
    return 0;
}

uint32 resource_poll_cd(void)
{
    uint32 state;
    uint32 stream;
    uint32 callback;
    uint32 flags;
    uint32 result;
    uint32 callback_result;
    uint32 timer;
    sint32 progress;

    FUNCTION_MARKER(0x8002A2C0, "ACE3.BIN");
    if (game_resource_music_active())
    {
        game_poll_resource_music();
        return 0;
    }
    state = r_u32(0x800BCBA8);
    switch (r_u32(state))
    {
        case 0:
            state = r_u32(0x800BCBA8);
            if (r_u8(state + 0x28) & 0x10)
            {
                w_u32(state + 0xC, 4);
                return sub_8002b428();
            }
            game_cd_poll_stream();
            result = CdSync(1, (uint8 *)psx_addr(state + 0x28, 8));
            stream = r_u32(0x800BCBAC);
            callback = r_u32(stream + 4);
            callback_result = 0;
            if (callback == 0x8002A498 && r_u32(stream + 0xC) == 0x80029FE4 && (r_u32(stream) & 0x80000080) == 0x80000000)
            {
                // Advance file-backed stream commands from their native completion
                resource_sync_callback(result, psx_addr(state + 0x28, 8));
                stream = r_u32(0x800BCBAC);
                callback = r_u32(stream + 4);
            }
            if (callback)
                callback_result = xport_guest_call3(callback, result, 0, 0);
            else if (r_u32(stream + 0xC) == 0x80029E74 && (r_u32(stream) & 0x80000080) == 0x80000000)
            {
                if (result == CdlComplete)
                    callback_result = resource_read_phase();
                else if (result == CdlDiskError)
                {
                    state = r_u32(0x800BCBA8);
                    callback_result = resource_sync_callback(result, psx_addr(state + 0x28, 8));
                }
            }
            stream = r_u32(0x800BCBAC);
            result = r_u32(stream) & 0x10;
            if (!result || callback_result)
                return result;
            timer = r_u32(stream + 0x30);
            result = timer - 1;
            w_u32(stream + 0x30, result);
            if ((sint32)timer > 0)
                return result;
            flags = r_u32(stream);
            w_u32(stream + 0x20, 255);
            w_u32(stream, (flags | 0x20) & 0xFFFFFFEF);
            return CdControlF(9, NULL);
        case 2:
            progress = sub_8002b094(r_u32(0x800BCBA8) + 0x28);
            result = 3;
            if (progress == 1)
            {
                state = r_u32(0x800BCBA8);
                w_u32(state, 3);
                w_u32(state + 4, 0);
            }
            return result;
        case 3:
            progress = sub_8002b194(r_u32(0x800BCBA8) + 0x28);
            if (progress < 0)
            {
                state = r_u32(0x800BCBA8);
                w_u32(state, 2);
                w_u32(state + 4, 0);
                return 2;
            }
            if (progress == 1)
            {
                state = r_u32(0x800BCBA8);
                w_u32(state, 4);
                w_u32(state + 4, 0);
            }
            return 4;
        case 4:
            state = r_u32(0x800BCBA8);
            if (r_u8(state + 0x28) & 0x10)
                return sub_8002b428();
            progress = sub_8002ae0c();
            if (progress == 1)
            {
                state = r_u32(0x800BCBA8);
                w_u32(state, 0);
                w_u32(state + 4, 0);
                return state;
            }
            if (progress < 0)
            {
                state = r_u32(0x800BCBA8);
                w_u32(state, 2);
                w_u32(state + 4, 0);
                return 2;
            }
            return 1;
        default:
            return 0x80010000;
    }
}

uint32 sub_80027338(uint32 resource, uint32 *native_count)
{
    uint32 ptr;
    uint32 count;
    uint32 i;
    uint32 offset;

    FUNCTION_MARKER(0x80027338, "ACE3.BIN");
    ptr = resource + 4;
    count = r_u16(resource);
    for (i = 0; i < count; ++i)
    {
        offset = r_u32(ptr);
        if (!(offset & 0x80000000))
            w_u32(ptr, offset + resource);
        ptr += 4;
    }
    if (native_count)
        *native_count = i;
    return resource + 4;
}

uint32 sub_800275c8(void)
{
    uint32 state;

    FUNCTION_MARKER(0x800275C8, "ACE3.BIN");
    state = r_u32(0x800BCB98);
    w_u32(0x800BCE68, 0x100010);
    w_u32(state + 0xE14, 0x56140000);
    w_u32(state + 0xE18, 0x56540000);
    w_u32(state, 0);
    w_u32(state + 0xE1C, 0x56940000);
    w_u32(state + 0xE20, 0);
    return 0x56940000;
}

sint32 resource_begin_read(uint32 entry, uint32 dst, uint32 aux, uint32 callback, uint32 flags, const uint32 *native_entry)
{
    uint32 state;
    uint32 stream;
    uint32 other;
    uint32 word;
    uint32 mode;
    uint32 counter;
    uint32 units;
    uint32 start;
    uint32 stream_flags;
    uint32 sync;
    sint32 sectors;

    FUNCTION_MARKER(0x8002A79C, "ACE3.BIN");
    state = r_u32(0x800BCBA8);
    if (r_u32(state))
        return -1;
    sync = game_use_extracted_assets() ? CdlComplete : CdSync(1, (uint8 *)psx_addr(state + 0x28, 8));
    if (sync != 2)
        return 0;
    if ((sint32)(native_entry ? native_entry[0] : r_u32(entry)) >= 0)
    {
        w_u32(r_u32(0x800BCBB0), 0);
        xport_guest_call3(callback, 0xFFFFFFFD, 0, 0);
        return 1;
    }
    game_stop_resource_music();
    state = r_u32(0x800BCBA8);
    stream = r_u32(0x800BCBAC);
    w_u32(state + 0x24, 0);
    w_u32(stream, 0);
    start = (native_entry ? native_entry[1] : r_u32(entry + 4));
    w_u32(stream + 0x40, 0);
    w_u32(stream + 0x34, 0);
    w_u32(stream + 8, callback);
    w_u32(stream + 0x10, start);
    word = (native_entry ? native_entry[0] : r_u32(entry));
    mode = (word >> 27) & 3;
    counter = r_u32(r_u32(state + 0x20));
    switch (mode)
    {
        case 0:
            w_u32(stream + 0xC, 0x80029E74);
            w_u32(stream + 4, 0);
            w_u32(stream + 0x18, dst);
            w_u32(stream + 0x14, (native_entry ? native_entry[0] : r_u32(entry)) & 0x7FFFF);
            units = (native_entry ? native_entry[0] : r_u32(entry)) & 0x7FFFF;
            sectors = (sint32)units - 1;
            if (sectors < 0)
                sectors = (sint32)units + 510;
            start = r_u32(stream + 0x10);
            w_u32(stream + 0x2C, start + (uint32)(sectors >> 9));
            if (game_use_extracted_assets())
            {
                sint32 loaded = !native_entry && game_read_resource_file(entry, dst, units * 4);
                w_u32(stream + 0x18, loaded ? dst + units * 4 : dst);
                w_u32(stream + 0x14, 0);
                w_u32(r_u32(0x800BCBB0), 0);
                if (!loaded)
                {
#ifndef NDEBUG
                    fprintf(stderr, "Extracted BP read failed: entry=%08X native=%p destination=%08X bytes=%u table=%08X resource=%08X\n", entry, (const void *)native_entry, dst, units * 4, r_u32(state + 0x18), r_u32(r_u32(0x800BCBBC) + 0x174));
#endif
                    xport_message_error("Ace Combat 3 - Resource error", "Cannot read extracted ACE_BPB resource");
                    abort();
                }
                if (callback)
                    xport_guest_call3(callback, 0, 0, 0);
                return 1;
            }
            break;
        case 1:
            w_u32(stream + 0xC, 0x80029FE4);
            stream_flags = r_u32(stream);
            w_u32(stream + 4, 0x8002A498);
            w_u32(stream + 0x18, dst);
            w_u8(stream + 0x1C, 1);
            w_u32(stream, stream_flags | 0x2000);
            word = (native_entry ? native_entry[0] : r_u32(entry));
            other = r_u32(0x800BCBAC);
            w_u8(other + 0x1D, (word >> 19) & 31);
            word = (native_entry ? native_entry[0] : r_u32(entry));
            stream = r_u32(0x800BCBAC);
            start = r_u32(stream + 0x10);
            units = (word & 0x7FFFF) << ((word >> 24) & 7);
            w_u32(stream + 0x14, units);
            w_u32(stream + 0x2C, start + units - 1);
            if (flags & 0x800)
                w_u32(r_u32(0x800BCBA8) + 0x24, 1);
            sub_8002b610(dst, aux, (flags & 0x2000) != 0);
            sub_8002b6d0();
            stream = r_u32(0x800BCBAC);
            units = r_u32(stream + 0x14);
            w_u32(stream + 0x3C, counter + 2);
            w_u32(stream + 0x3C, counter + 2 + (uint32)((sint32)(units * 6) / 15));
            break;
        case 2:
            if (game_use_extracted_assets() && game_start_resource_music(start, callback, flags, 0))
                return 1;
            sub_8002999c(1);
            if (flags & 0x1000)
            {
                stream = r_u32(0x800BCBAC);
                w_u32(stream, r_u32(stream) | 0x4000);
            }
            stream = r_u32(0x800BCBAC);
            w_u32(stream + 0xC, 0x80029FE4);
            stream_flags = r_u32(stream);
            w_u32(stream + 4, 0x8002A498);
            w_u8(stream + 0x1C, 1);
            w_u32(stream, stream_flags | 0x400);
            word = (native_entry ? native_entry[0] : r_u32(entry));
            other = r_u32(0x800BCBAC);
            w_u8(other + 0x1D, (word >> 19) & 31);
            word = (native_entry ? native_entry[0] : r_u32(entry));
            stream = r_u32(0x800BCBAC);
            units = (word & 0x7FFFF) << ((word >> 24) & 7);
            w_u32(stream + 0x14, units);
            start = r_u32(stream + 0x10);
            w_u32(stream + 0x3C, counter + 2);
            w_u32(stream + 0x2C, start + units - 1);
            w_u32(stream + 0x3C, counter + 2 + (uint32)((sint32)(units * 6) / 15));
            break;
        case 3:
            sub_8002999c(1);
            if (flags & 0x1000)
            {
                stream = r_u32(0x800BCBAC);
                w_u32(stream, r_u32(stream) | 0x4000);
            }
            else if (flags & 0x4000)
            {
                stream = r_u32(0x800BCBAC);
                w_u32(stream, r_u32(stream) | 0x8000);
            }
            if (flags & 0x800)
                w_u32(r_u32(0x800BCBA8) + 0x24, 1);
            stream = r_u32(0x800BCBAC);
            w_u32(stream + 0xC, 0x80029FE4);
            w_u32(stream + 4, 0x8002A498);
            stream_flags = r_u32(stream);
            w_u8(stream + 0x1C, 1);
            other = r_u32(0x800BCBAC);
            w_u32(stream + 0x18, dst);
            w_u32(stream, stream_flags | 0x2000);
            word = (native_entry ? native_entry[0] : r_u32(entry));
            w_u8(other + 0x1D, (word >> 19) & 31);
            word = (native_entry ? native_entry[0] : r_u32(entry));
            stream = r_u32(0x800BCBAC);
            start = r_u32(stream + 0x10);
            units = (word & 0x7FFFF) << ((word >> 24) & 7);
            w_u32(stream + 0x14, units);
            w_u32(stream + 0x2C, start + units - 1);
            sub_8002b610(dst, aux, (flags & 0x2000) != 0);
            sub_8002b6d0();
            stream = r_u32(0x800BCBAC);
            units = r_u32(stream + 0x14);
            w_u32(stream + 0x3C, counter + 2);
            w_u32(stream + 0x3C, counter + 2 + (uint32)((sint32)(units * 6) / 15));
            break;
        default:
            return -1;
    }
    if (mode == 3 && game_use_extracted_assets() && game_start_resource_music(start, callback, flags, 1))
        return 1;
    resource_issue_seek();
    return 1;
}

uint32 resource_issue_seek(void)
{
    CdlLOC position;
    uint32 stream;
    uint32 sector;
    uint32 flags;
    uint32 state;

    FUNCTION_MARKER(0x80029DFC, "ACE3.BIN");
    stream = r_u32(0x800BCBAC);
    sector = r_u32(stream + 0x10);
    flags = r_u32(stream);
    w_u32(0x800BCE80, 0);
    w_u32(stream + 0x20, 0);
    w_u32(stream, (flags | 0x80000000) & 0xFCFFEFEF);
    CdIntToPos((sint32)sector, &position);
    if (r_u32(stream + 0xC) == 0x80029E74 || r_u32(stream + 0xC) == 0x80029FE4)
        game_cd_sync_callback(0);
    else
        game_cd_sync_callback(0x8002A200);
    state = r_u32(0x800BCBA8);
    w_u32(state + 0x28, 0);
    w_u32(state + 0x2C, 0);
    w_u32(r_u32(0x800BCBB0), 0);
    if (r_u32(stream + 0xC) == 0x80029E74 || r_u32(stream + 0xC) == 0x80029FE4)
    {
        if (!CdControlB(CdlPause, NULL, NULL))
            return 0;
        return CdControlB(2, (uint8 *)&position, NULL);
    }
    return CdControlF(2, (uint8 *)((const uint8 *)&position));
}

void sub_8002999c(uint32 enabled)
{
    SpuCommonAttr attr;

    FUNCTION_MARKER(0x8002999C, "ACE3.BIN");
    attr.mask = 0x200;
    attr.cd.mix = enabled == 0;
    SpuSetCommonAttr(&attr);
}

uint32 resource_read_complete(uint32 result)
{
    uint32 state;

    FUNCTION_MARKER(0x8002BDB0, "ACE3.BIN");
    state = r_u32(0x800BCBBC);
    w_u32(state + 0x168, 1);
    w_u32(state + 0x16C, result);
    return 1;
}

uint32 sub_8002d96c(uint32 resource)
{
    uint32 src;
    uint32 dst;
    uint32 i;
    uint32 word0;
    uint32 word1;
    uint32 word2;
    uint32 word3;
    uint32 address;
    uint32 bytes;
    sint32 offset;

    FUNCTION_MARKER(0x8002D96C, "ACE3.BIN");
    i = 0;
    src = resource + 12;
    if (r_u8(resource + 1))
    {
        do
        {
            dst = 0x800C5A10 + (r_u8(resource) + i) * 20;
            word0 = r_u32(src);
            word1 = r_u32(src + 4);
            word2 = r_u32(src + 8);
            word3 = r_u32(src + 12);
            w_u32(dst, word0);
            w_u32(dst + 4, word1);
            w_u32(dst + 8, word2);
            w_u32(dst + 12, word3);
            w_u32(dst + 16, r_u32(src + 16));
            ++i;
            src += 20;
        } while (i < r_u8(resource + 1));
    }
    offset = r_s16(resource + 2);
    address = r_u32(resource + 4);
    bytes = r_u32(resource + 8);
    return sound_upload(address, resource + (uint32)offset * 4, bytes);
}

sint32 sound_upload(uint32 address, uint32 src, uint32 bytes)
{
    FUNCTION_MARKER(0x8002D910, "ACE3.BIN");
    SpuSetTransferMode(1);
    SpuSetTransferStartAddr(address);
    sound_write(src, bytes);
    return SpuIsTransferCompleted(1);
}

uint32 sound_write(uint32 src, uint32 bytes)
{
    uint32 length;

    FUNCTION_MARKER(0x800A10B8, "ACE3.BIN");
    length = bytes;
    if (length > 0x7EFF0)
        length = 0x7EFF0;
    _spu_Fw(src, length);
    if (!r_u32(0x800B47FC))
        w_u32(0x800B47F8, 0);
    return length;
}

uint32 sub_8002d75c(void)
{
    FUNCTION_MARKER(0x8002D75C, "ACE3.BIN");
    sound_init_voices();
    sound_init_cd(0);
    sound_init_slots();
    sound_init_reverb();
    return sound_init_queue();
}

uint32 sound_init_voices(void)
{
    uint32 i;
    uint32 voice;

    FUNCTION_MARKER(0x8002E604, "ACE3.BIN");
    voice = 0x800C5830;
    for (i = 0; i < 24; ++i)
    {
        w_u8(voice, 255);
        w_u8(voice + 1, 0);
        w_u8(voice + 2, 0);
        w_u8(voice + 3, 0);
        w_u8(voice + 16, 128);
        w_u8(voice + 17, 128);
        w_u16(voice + 18, 0);
        voice += 20;
    }
    return 0;
}

void sound_init_cd(uint32 enabled)
{
    uint32 state;
    uint32 volume;

    FUNCTION_MARKER(0x8002D79C, "ACE3.BIN");
    w_u32(0x800C5800, 0x3C3);
    w_u16(0x800C5804, 0x3FFF);
    w_u16(0x800C5806, 0x3FFF);
    w_u32(0x800C5818, enabled != 0);
    state = r_u32(0x800BCE60);
    w_u32(0x800C5814, 0);
    volume = r_u8(state + 0x399) >> 1;
    w_u16(0x800C5812, volume);
    w_u16(0x800C5810, volume);
    SpuSetCommonAttr((SpuCommonAttr *)psx_addr(0x800C5800, 40));
}

uint32 sound_init_slots(void)
{
    uint32 i;

    FUNCTION_MARKER(0x8002F140, "ACE3.BIN");
    for (i = 0; i < 8; ++i)
        sound_reset_slot(i);
    return 0;
}

uint32 sound_reset_slot(uint32 slot)
{
    uint32 ptr;

    FUNCTION_MARKER(0x8002F178, "ACE3.BIN");
    ptr = 0x800C6550 + slot * 4;
    w_u8(ptr, 255);
    w_u16(ptr + 2, 0xFFFF);
    return 0xFFFFFFFF;
}

sint32 sound_init_reverb(void)
{
    FUNCTION_MARKER(0x8002DA20, "ACE3.BIN");
    w_u32(0x800C65F0, 1);
    w_u32(0x800C65F4, 3);
    w_u16(0x800C65F8, 0x1999);
    w_u16(0x800C65FA, 0x1999);
    return SpuSetReverbModeParam((SpuReverbAttr *)psx_addr(0x800C65F0, 20));
}

uint32 sound_init_queue(void)
{
    uint32 i;
    uint32 slot;

    FUNCTION_MARKER(0x8002F254, "ACE3.BIN");
    w_u8(0x800BCE98, 0);
    w_u8(0x800BCE99, 0);
    slot = 0x800C6570;
    for (i = 0; i < 32; ++i)
    {
        w_u8(slot, 255);
        w_u8(slot + 1, 255);
        w_u16(slot + 2, 0xFFFF);
        slot += 4;
    }
    return 0;
}

uint32 sub_8002cbcc(void)
{
    uint32 i;
    uint32 state;

    FUNCTION_MARKER(0x8002CBCC, "ACE3.BIN");
    w_u8(0x800BCE96, 0);
    w_u8(0x800BCE95, 0);
    w_u8(0x800BCE94, 0);
    w_u8(0x800BCE8F, 0);
    w_u8(0x800BCE8E, 0);
    w_u32(0x800BCE88, 0x800C57E0);
    for (i = 0; i < 3; ++i)
    {
        w_u8(r_u32(0x800BCE88) + i + 4, 0);
        w_u8(r_u32(0x800BCE88) + i + 7, 0);
        state = r_u32(0x800BCE88);
        w_u16(state + i * 2 + 10, 0);
    }
    w_u16(state + 30, 0x4A16);
    w_u16(state + 28, 58);
    return 0;
}

void sub_8002d880(uint32 mode)
{
    FUNCTION_MARKER(0x8002D880, "ACE3.BIN");
    w_u32(0x800C5800, 0x200);
    w_u32(0x800C5818, mode != 0);
    SpuSetCommonAttr((SpuCommonAttr *)psx_addr(0x800C5800, 40));
}

uint32 resource_sync_callback(uint32 event, const uint8 *response)
{
    uint32 state;
    uint32 stream;
    uint32 flags;
    uint32 last;
    uint32 target;

    FUNCTION_MARKER(0x8002A200, "ACE3.BIN");
    state = r_u32(0x800BCBA8);
    w_u32(state + 0x28, xport_load_le32(response));
    w_u32(state + 0x2C, xport_load_le32(response + 4));
    event &= 255;
    if (event == 5)
    {
        stream = r_u32(0x800BCBAC);
        if (r_u32(stream) & 0x80)
            return 255;
        w_u32(stream + 0x20, 255);
        w_u32(stream + 0x34, 1);
        flags = r_u32(stream);
        last = r_u32(stream + 0x24);
        w_u32(stream, (flags | 0x20) & 0xFFFFFFEF);
        if (last)
        {
            w_u32(stream + 0x10, last + 1);
            w_u32(stream + 0x24, 0);
        }
    }
    else if (event != 2)
        return 2;
    stream = r_u32(0x800BCBAC);
    target = r_u32(stream + 0xC);
    if (target)
        return xport_guest_call3(target, state + 0x28, 0, 0);
    return target;
}

uint32 resource_read_phase(void)
{
    uint8 mode;
    uint32 stream;
    uint32 phase;
    uint32 flags;
    uint32 target;
    uint32 result;
    sint32 remaining;

    FUNCTION_MARKER(0x80029E74, "ACE3.BIN");
    stream = r_u32(0x800BCBAC);
    phase = r_u32(stream + 0x20);
    switch (phase)
    {
        case 0:
            mode = 0xA0;
            w_u32(stream + 0x20, 1);
            return CdControlB(CdlSetmode, &mode, NULL);
        case 1:
            w_u32(stream + 0x24, 0);
            game_cd_ready_callback(0x80029A44);
            stream = r_u32(0x800BCBAC);
            w_u32(stream + 0x20, 2);
            resource_read_phase();
            return game_cd_read_resource_sector_sync();
        case 2:
            w_u32(stream + 0x30, 300);
            flags = r_u32(stream);
            w_u32(stream + 0x20, 3);
            result = flags | 0x10;
            w_u32(stream, result);
            return result;
        case 3:
            if (r_s32(stream + 0x14) > 0)
            {
                do
                {
                    remaining = r_s32(stream + 0x14);
                    game_cd_read_resource_sector_sync();
                    stream = r_u32(0x800BCBAC);
                    if (remaining <= 512 || r_u32(stream + 0x20) != 3)
                        break;
                } while (r_s32(stream + 0x14) > 0);
                return 0x7EFF0000;
            }
            flags = r_u32(stream);
            w_u32(stream, flags & 0x7EFFFFFF);
            game_cd_sync_callback(0);
            stream = r_u32(0x800BCBAC);
            target = r_u32(stream + 8);
            if (target)
                return xport_guest_call3(target, 0, 0, 0);
            return target;
        default:
            stream = r_u32(0x800BCBAC);
            flags = r_u32(stream);
            if (flags & 0x20)
            {
                w_u32(stream, flags & 0xFFFFFFDF);
                return resource_issue_seek();
            }
            w_u32(stream, flags | 0x80);
            game_cd_sync_callback(0);
            stream = r_u32(0x800BCBAC);
            target = r_u32(stream + 8);
            if (target)
                return xport_guest_call3(target, 0xFFFFFFFE, 0, 0);
            return target;
    }
}

uint32 resource_sector_ready(uint32 event, const uint8 *response, const uint8 *native_sector)
{
    uint32 sector_header[4];
    uint32 state;
    uint32 stream;
    uint32 flags;
    uint32 last;
    uint32 sector;
    uint32 dst;
    uint32 result = 1;
    uint32 error = 0;
    uint32 left;
    sint32 words = 512;

    FUNCTION_MARKER(0x80029A44, "ACE3.BIN");
    state = r_u32(0x800BCBA8);
    w_u32(state + 0x28, xport_load_le32(response));
    w_u32(state + 0x2C, xport_load_le32(response + 4));
    if ((event & 255) == 1)
    {
        stream = r_u32(0x800BCBAC);
        flags = r_u32(stream) | 0x01000000;
        w_u32(stream, flags);
        if (flags & 0x10)
            w_u32(stream, flags & 0xFFFFFFEF);
        if (native_sector)
            memcpy(sector_header, native_sector, 12);
        else
            cd_get_sector(sector_header, 3);
        sector = CdPosToInt((CdlLOC *)sector_header);
        stream = r_u32(0x800BCBAC);
        last = r_u32(stream + 0x24);
        if (last)
        {
            if (sector != last + 1)
                error = 4;
        }
        else if (sector != r_u32(stream + 0x10))
            error = 3;
        stream = r_u32(0x800BCBAC);
        left = r_u32(stream + 0x14);
        w_u32(stream + 0x24, sector);
        if ((sint32)left < words)
            words = (sint32)left;
        dst = r_u32(stream + 0x18);
        if (native_sector)
        {
            void *destination = psx_addr(dst, (size_t)(uint32)words * 4);
            if (destination && (uint32)words <= 512)
                memcpy(destination, native_sector + 12, (uint32)words * 4);
        }
        else
            cd_get_sector(psx_addr(dst, (size_t)(uint32)words * 4), (uint32)words);
        stream = r_u32(0x800BCBAC);
        dst = r_u32(stream + 0x18);
        left = r_u32(stream + 0x14);
        result = dst + (uint32)words * 4;
        left -= (uint32)words;
        w_u32(stream + 0x18, result);
        w_u32(stream + 0x14, left);
        if ((sint32)left <= 0)
        {
            game_cd_ready_callback(0);
            result = CdControlF(9, NULL);
        }
    }
    else
        error = 2;
    if (error)
    {
        stream = r_u32(0x800BCBAC);
        flags = r_u32(stream);
        last = r_u32(stream + 0x24);
        w_u32(stream + 0x20, 255);
        w_u32(stream + 0x34, error);
        w_u32(stream, (flags | 0x20) & 0xFFFFFFEF);
        if (last)
        {
            w_u32(stream + 0x10, last + 1);
            w_u32(stream + 0x24, 0);
        }
        game_cd_ready_callback(0);
        return CdControlF(9, NULL);
    }
    return result;
}

uint32 sub_80026050(void)
{
    uint32 state;
    uint32 index;
    uint32 buffer;
    uint32 ot;

    FUNCTION_MARKER(0x80026050, "ACE3.BIN");
    state = r_u32(0x800BCE60);
    index = r_u32(state + 4) ^ 1;
    w_u32(state + 4, index);
    buffer = r_u32(0x800BCE58) + index * 4116;
    ot = buffer + 8;
    w_u32(state + 8, buffer + 12);
    w_u32(state + 12, ot);
    w_u32(state + 16, buffer + 16);
    w_u32(state + 20, buffer + 20);
    ClearOTagR(psx_addr(ot, 12), 3);
    return ot;
}

uint32 sub_800260c0(void)
{
    uint32 state;
    uint32 index;
    uint32 buffer;
    uint32 env;
    uint32 flags;
    uint32 frame;

    FUNCTION_MARKER(0x800260C0, "ACE3.BIN");
    state = r_u32(0x800BCE60);
    index = r_u32(state + 4);
    buffer = r_u32(0x800BCE58) + index * 4116;
    env = r_u32(buffer + 4);
    PutDispEnv((DISPENV *)psx_addr(env, 20));
    env = r_u32(buffer);
    PutDrawEnv((DRAWENV *)psx_addr(env, 92));
    state = r_u32(0x800BCE60);
    if (!(r_u32(state) & 4))
        DrawOTag(psx_addr(buffer + 16, 4));
    state = r_u32(0x800BCE60);
    flags = r_u32(state);
    if (!(flags & 0x20000000))
    {
        if (!(flags & 0x02000000))
            w_u32(state + 0x28, r_u32(state + 0x28) + 1);
        else
            w_u32(state, flags & 0xFDFFFFFF);
        state = r_u32(0x800BCE60);
    }
    frame = r_u32(state + 0x24) + 1;
    w_u32(state + 0x24, frame);
    return frame;
}

uint32 sub_80026e84(void)
{
    uint32 state;
    uint32 packet;
    uint32 i;
    uint32 result;

    FUNCTION_MARKER(0x80026E84, "ACE3.BIN");
    result = VSync(-1);
    if (result == pad_input_field)
        return 0;
    pad_input_field = result;
    state = r_u32(0x800BCB84);
#if !defined(LOCKSTEP_DEBUG)
    game_pad_refresh_connection(state);
#endif
    if (r_u16(state) == 0x8000)
    {
        packet = state + 2;
        for (i = 0; i < 4; ++i)
        {
            pad_update_port(packet, r_u32(0x800BCB84) + 0x44 + i * 16);
            packet += 8;
        }
    }
    else
    {
        pad_update_port(state, r_u32(0x800BCB84) + 0x44);
        packet = state + 8;
        for (i = 1; i < 4; ++i)
        {
            w_u16(packet, 0xFFFF);
            pad_update_port(packet, r_u32(0x800BCB84) + 0x44 + i * 16);
            packet += 8;
        }
    }
    state = r_u32(0x800BCB84);
    if (r_u16(state + 0x22) == 0x8000)
    {
        packet = state + 0x24;
        for (i = 4; i < 8; ++i)
        {
            pad_update_port(packet, r_u32(0x800BCB84) + 0x44 + i * 16);
            packet += 8;
        }
    }
    else
    {
        pad_update_port(state + 0x22, r_u32(0x800BCB84) + 0x84);
        packet = state + 0x2A;
        for (i = 5; i < 8; ++i)
        {
            w_u16(packet, 0xFFFF);
            pad_update_port(packet, r_u32(0x800BCB84) + 0x44 + i * 16);
            packet += 8;
        }
    }
    return 0;
}
