#include "game_functions.h"
#include <stdlib.h>

/* Generated scalar bindings; typed native pointer ABIs remain separate */
uint32 game_dispatch(uint32 target, uint32 argument_count, uint32 argument0, uint32 argument1, uint32 argument2)
{
    switch (target)
    {
        case 0x80025BC8:
            return (uint32)sub_80025bc8();
        case 0x80025DDC:
            return (uint32)sub_80025ddc();
        case 0x80025EA0:
            return (uint32)sub_80025ea0();
        case 0x80025FE8:
            return (uint32)sub_80025fe8();
        case 0x80026044:
            return (uint32)sub_80026044();
        case 0x80026050:
            return (uint32)sub_80026050();
        case 0x800260C0:
            return (uint32)sub_800260c0();
        case 0x80026350:
            return (uint32)sub_80026350();
        case 0x80026694:
            if (argument_count < 1)
                break;
            return (uint32)pad_map_mode(argument0);
        case 0x80026700:
            if (argument_count < 1)
                break;
            return (uint32)pad_get_mode_param0(argument0);
        case 0x80026754:
            if (argument_count < 1)
                break;
            return (uint32)pad_get_mode_param1(argument0);
        case 0x8002685C:
            if (argument_count < 1)
                break;
            return (uint32)pad_negotiate(argument0);
        case 0x80026BA4:
            if (argument_count < 2)
                break;
            return (uint32)pad_update_port(argument0, argument1);
        case 0x80026E84:
            return (uint32)sub_80026e84();
        case 0x800271A0:
            if (argument_count < 1)
                break;
            return (uint32)sub_800271a0(argument0);
        case 0x800272F0:
            if (argument_count < 2)
                break;
            return (uint32)sub_800272f0(argument0, argument1);
        case 0x80027338:
            if (argument_count < 1)
                break;
            return (uint32)sub_80027338(argument0, 0);
        case 0x80027380:
            if (argument_count < 3)
                break;
            return (uint32)relocate_pointer_array(argument0, argument1, argument2);
        case 0x800273FC:
            if (argument_count < 1)
                break;
            return (uint32)game_set_arena(argument0);
        case 0x80027410:
            if (argument_count < 1)
                break;
            return (uint32)arena_alloc(argument0);
        case 0x80027478:
            if (argument_count < 1)
                break;
            return (uint32)arena_peek(argument0);
        case 0x800274E0:
            if (argument_count < 1)
                break;
            return (uint32)sub_800274e0(argument0);
        case 0x80027538:
            if (argument_count < 1)
                break;
            return (uint32)arena_set_cursor(argument0);
        case 0x80027584:
            return (uint32)sub_80027584();
        case 0x8002759C:
            if (argument_count < 3)
                break;
            return (uint32)copy_words(argument0, argument1, argument2);
        case 0x800275C8:
            return (uint32)sub_800275c8();
        case 0x800278A8:
            if (argument_count < 2)
                break;
            return (uint32)sub_800278a8(argument0, argument1);
        case 0x80027AF0:
            if (argument_count < 2)
                break;
            return (uint32)sub_80027af0(argument0, argument1);
        case 0x8002877C:
            if (argument_count < 3)
                break;
            return (uint32)format_to_buffer(argument0, argument1, argument2);
        case 0x800287BC:
            if (argument_count < 2)
                break;
            return (uint32)pad_copy_bindings(argument0, argument1);
        case 0x800287E4:
            if (argument_count < 2)
                break;
            return (uint32)pad_copy_axes(argument0, argument1);
        case 0x80028848:
            if (argument_count < 2)
                break;
            return (uint32)pad_init_profile(argument0, argument1);
        case 0x80028A7C:
            if (argument_count < 1)
                break;
            return (uint32)pad_curve_expand(argument0);
        case 0x80028B1C:
            if (argument_count < 1)
                break;
            return (uint32)pad_curve_contract(argument0);
        case 0x80028BC0:
            if (argument_count < 1)
                break;
            return (uint32)pad_init_profiles(argument0);
        case 0x80028C24:
            if (argument_count < 3)
                break;
            return (uint32)pad_update_input(argument0, argument1, argument2);
        case 0x8002937C:
            return (uint32)pad_reset_slots();
        case 0x8002939C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8002939c(argument0);
        case 0x800293F0:
            if (argument_count < 1)
                break;
            return (uint32)sub_800293f0(argument0);
        case 0x80029448:
            if (argument_count < 1)
                break;
            return (uint32)sub_80029448(argument0);
        case 0x800294C4:
            if (argument_count < 1)
                break;
            return (uint32)sub_800294c4(argument0);
        case 0x80029588:
            return (uint32)sub_80029588();
        case 0x800295CC:
            return (uint32)sub_800295cc();
        case 0x80029604:
            return (uint32)sub_80029604();
        case 0x8002963C:
            return (uint32)sub_8002963c();
        case 0x80029674:
            return (uint32)sub_80029674();
        case 0x800296B8:
            return (uint32)sub_800296b8();
        case 0x800296F0:
            return (uint32)sub_800296f0();
        case 0x80029734:
            if (argument_count < 1)
                break;
            return (uint32)pad_rumble_mix(argument0);
        case 0x8002995C:
            if (argument_count < 1)
                break;
            return (uint32)resource_get_entry(argument0);
        case 0x800299D8:
            if (argument_count < 2)
                break;
            return (uint32)sub_800299d8(argument0, argument1);
        case 0x80029A44:
            if (argument_count < 2)
                break;
            return (uint32)resource_sector_ready(argument0, psx_addr(argument1, 8), NULL);
        case 0x80029BD8:
            return (uint32)cd_audio_sector();
        case 0x80029D70:
            if (argument_count < 2)
                break;
            return (uint32)cd_audio_ready(argument0, psx_addr(argument1, 8));
        case 0x80029DFC:
            return (uint32)resource_issue_seek();
        case 0x80029E74:
            return (uint32)resource_read_phase();
        case 0x80029FE4:
            return (uint32)cd_audio_step();
        case 0x8002A200:
            if (argument_count < 2)
                break;
            return (uint32)resource_sync_callback(argument0, psx_addr(argument1, 8));
        case 0x8002A2C0:
            return (uint32)resource_poll_cd();
        case 0x8002A498:
            if (argument_count < 1)
                break;
            return (uint32)cd_audio_poll(argument0);
        case 0x8002ABE0:
            if (argument_count < 1)
                break;
            return (uint32)sub_8002abe0(argument0);
        case 0x8002AC3C:
            if (argument_count < 1)
                break;
            return (uint32)resource_stop_cd(argument0);
        case 0x8002AD90:
            if (argument_count < 3)
                break;
            return (uint32)cd_read_sectors(argument0, argument1, argument2);
        case 0x8002AE0C:
            return (uint32)sub_8002ae0c();
        case 0x8002B034:
            if (argument_count < 1)
                break;
            return (uint32)sub_8002b034(argument0);
        case 0x8002B094:
            if (argument_count < 1)
                break;
            return (uint32)sub_8002b094(argument0);
        case 0x8002B194:
            if (argument_count < 1)
                break;
            return (uint32)sub_8002b194(argument0);
        case 0x8002B428:
            return (uint32)sub_8002b428();
        case 0x8002B470:
            if (argument_count < 3)
                break;
            return (uint32)sub_8002b470(argument0, argument1, argument2);
        case 0x8002B504:
            return (uint32)cd_resource_state();
        case 0x8002B528:
            return (uint32)sub_8002b528();
        case 0x8002B588:
            return (uint32)sub_8002b588();
        case 0x8002B59C:
            return (uint32)sub_8002b59c();
        case 0x8002B5B0:
            return (uint32)sub_8002b5b0();
        case 0x8002B5FC:
            return (uint32)sub_8002b5fc();
        case 0x8002B610:
            if (argument_count < 3)
                break;
            return (uint32)sub_8002b610(argument0, argument1, argument2);
        case 0x8002B6D0:
            return (uint32)sub_8002b6d0();
        case 0x8002B760:
            return (uint32)cd_stream_sector(NULL);
        case 0x8002BBE0:
            if (argument_count < 1)
                break;
            return (uint32)cd_stream_free_frame(argument0);
        case 0x8002BCA4:
            return (uint32)sub_8002bca4();
        case 0x8002BCD8:
            return (uint32)sub_8002bcd8();
        case 0x8002BD28:
            if (argument_count < 1)
                break;
            return (uint32)sub_8002bd28(argument0);
        case 0x8002BD5C:
            return (uint32)sub_8002bd5c();
        case 0x8002BD60:
            if (argument_count < 1)
                break;
            return (uint32)sub_8002bd60(argument0);
        case 0x8002BDB0:
            if (argument_count < 1)
                break;
            return (uint32)resource_read_complete(argument0);
        case 0x8002BDC4:
            return (uint32)sub_8002bdc4();
        case 0x8002BDCC:
            if (argument_count < 1)
                break;
            return (uint32)resource_stop_request(argument0);
        case 0x8002BE14:
            if (argument_count < 1)
                break;
            return (uint32)resource_read_request(argument0);
        case 0x8002BEE4:
            if (argument_count < 1)
                break;
            return (uint32)resource_audio_request(argument0);
        case 0x8002C1D4:
            if (argument_count < 1)
                break;
            return (uint32)resource_stream_request(argument0);
        case 0x8002C2E4:
            if (argument_count < 1)
                break;
            return (uint32)resource_fade_request(argument0);
        case 0x8002C41C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8002c41c(argument0);
        case 0x8002C45C:
            if (argument_count < 1)
                break;
            return (uint32)resource_poll_request(argument0);
        case 0x8002C4DC:
            return (uint32)sub_8002c4dc();
        case 0x8002C580:
            if (argument_count < 1)
                break;
            return (uint32)resource_wait(argument0);
        case 0x8002C5E0:
            if (argument_count < 1)
                break;
            return (uint32)resource_get_size(argument0);
        case 0x8002C63C:
            if (argument_count < 2)
                break;
            return (uint32)resource_total_size(argument0, argument1);
        case 0x8002C6A8:
            if (argument_count < 2)
                break;
            return (uint32)resource_init_queue(argument0, argument1);
        case 0x8002C6F4:
            if (argument_count < 1)
                break;
            return (uint32)sub_8002c6f4(argument0);
        case 0x8002C71C:
            return (uint32)resource_cancel_streams();
        case 0x8002C7AC:
            if (argument_count < 3)
                break;
            return (uint32)resource_queue_read(argument0, argument1, argument2);
        case 0x8002C8B8:
            if (argument_count < 3)
                break;
            return (uint32)sub_8002c8b8(argument0, argument1, argument2);
        case 0x8002C8D8:
            if (argument_count < 3)
                break;
            return (uint32)resource_queue_audio(argument0, argument1, argument2);
        case 0x8002CA2C:
            if (argument_count < 1)
                break;
            return (uint32)resource_read_progress(argument0, 0);
        case 0x8002CAD0:
            return (uint32)sound_get_cur_resource();
        case 0x8002CAE8:
            if (argument_count < 2)
                break;
            return (uint32)sub_8002cae8(argument0, argument1);
        case 0x8002CB58:
            if (argument_count < 1)
                break;
            return (uint32)resource_is_cd_audio(argument0);
        case 0x8002CBA4:
            if (argument_count < 1)
                break;
            return (uint32)sub_8002cba4(argument0);
        case 0x8002CBCC:
            return (uint32)sub_8002cbcc();
        case 0x8002CC44:
            if (argument_count < 3)
                break;
            return (uint32)sub_8002cc44(argument0, argument1, argument2);
        case 0x8002CD24:
            return (uint32)sub_8002cd24();
        case 0x8002CE70:
            if (argument_count < 2)
                break;
            return (uint32)sub_8002ce70(argument0, argument1);
        case 0x8002D10C:
            if (argument_count < 2)
                break;
            return (uint32)sub_8002d10c(argument0, argument1);
        case 0x8002D330:
            if (argument_count < 2)
                break;
            return (uint32)sub_8002d330(argument0, argument1);
        case 0x8002D3F4:
            if (argument_count < 2)
                break;
            return (uint32)sub_8002d3f4(argument0, argument1);
        case 0x8002D544:
            return (uint32)sub_8002d544();
        case 0x8002D560:
            if (argument_count < 3)
                break;
            return (uint32)sub_8002d560(argument0, argument1, argument2);
        case 0x8002D6E8:
            return (uint32)sub_8002d6e8();
        case 0x8002D71C:
            return (uint32)sub_8002d71c();
        case 0x8002D73C:
            sub_8002d73c();
            return 0;
        case 0x8002D75C:
            return (uint32)sub_8002d75c();
        case 0x8002D8C8:
            if (argument_count < 1)
                break;
            sub_8002d8c8(argument0);
            return 0;
        case 0x8002D910:
            if (argument_count < 3)
                break;
            return (uint32)sound_upload(argument0, argument1, argument2);
        case 0x8002D96C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8002d96c(argument0);
        case 0x8002DA20:
            return (uint32)sound_init_reverb();
        case 0x8002DA60:
            return (uint32)sub_8002da60();
        case 0x8002DA94:
            return (uint32)sound_enable_reverb();
        case 0x8002DAF4:
            return (uint32)sound_disable_reverb();
        case 0x8002DB4C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8002db4c(argument0);
        case 0x8002DBE0:
            if (argument_count < 1)
                break;
            return (uint32)sub_8002dbe0(argument0);
        case 0x8002DC7C:
            if (argument_count < 2)
                break;
            return (uint32)sub_8002dc7c(argument0, argument1);
        case 0x8002DCB0:
            if (argument_count < 1)
                break;
            return (uint32)sub_8002dcb0(argument0);
        case 0x8002DCF8:
            if (argument_count < 1)
                break;
            return (uint32)sound_find_voice(argument0);
        case 0x8002DD38:
            return (uint32)sound_reserve_voice();
        case 0x8002DD7C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8002dd7c(argument0);
        case 0x8002DDA8:
            if (argument_count < 1)
                break;
            return (uint32)sub_8002dda8(argument0);
        case 0x8002DE24:
            if (argument_count < 2)
                break;
            return (uint32)sound_set_voice_note(argument0, argument1);
        case 0x8002DEC8:
            if (argument_count < 3)
                break;
            return (uint32)sub_8002dec8(argument0, argument1, argument2);
        case 0x8002DF40:
            if (argument_count < 3)
                break;
            return (uint32)sound_set_voice_fade(argument0, argument1, argument2);
        case 0x8002E514:
            if (argument_count < 2)
                break;
            return (uint32)sound_start_voice(argument0, argument1);
        case 0x8002E5B4:
            if (argument_count < 1)
                break;
            return (uint32)sub_8002e5b4(argument0);
        case 0x8002E604:
            return (uint32)sound_init_voices();
        case 0x8002E64C:
            if (argument_count < 2)
                break;
            return (uint32)sub_8002e64c(argument0, argument1);
        case 0x8002E67C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8002e67c(argument0);
        case 0x8002E834:
            if (argument_count < 1)
                break;
            return (uint32)sub_8002e834(argument0);
        case 0x8002EA24:
            if (argument_count < 2)
                break;
            return (uint32)sub_8002ea24(argument0, argument1);
        case 0x8002EA74:
            if (argument_count < 3)
                break;
            return (uint32)sub_8002ea74(argument0, argument1, argument2);
        case 0x8002EC48:
            if (argument_count < 2)
                break;
            return (uint32)sub_8002ec48(argument0, argument1);
        case 0x8002ED2C:
            if (argument_count < 2)
                break;
            return (uint32)sub_8002ed2c(argument0, argument1);
        case 0x8002EE88:
            if (argument_count < 3)
                break;
            return (uint32)sub_8002ee88(argument0, argument1, argument2);
        case 0x8002EEA8:
            if (argument_count < 3)
                break;
            return (uint32)sub_8002eea8(argument0, argument1, argument2);
        case 0x8002EEC8:
            if (argument_count < 1)
                break;
            return (uint32)sub_8002eec8(argument0);
        case 0x8002F0D4:
            if (argument_count < 2)
                break;
            return (uint32)sub_8002f0d4(argument0, argument1);
        case 0x8002F140:
            return (uint32)sound_init_slots();
        case 0x8002F178:
            if (argument_count < 1)
                break;
            return (uint32)sound_reset_slot(argument0);
        case 0x8002F19C:
            if (argument_count < 2)
                break;
            return (uint32)sub_8002f19c(argument0, argument1);
        case 0x8002F1E0:
            return (uint32)sound_update_slots();
        case 0x8002F254:
            return (uint32)sound_init_queue();
        case 0x8002F294:
            if (argument_count < 2)
                break;
            return (uint32)sub_8002f294(argument0, argument1);
        case 0x8002F2CC:
            return (uint32)sound_update_queue();
        case 0x8002F500:
            if (argument_count < 1)
                break;
            return (uint32)sub_8002f500(argument0);
        case 0x8002F620:
            return (uint32)sub_8002f620();
        case 0x8002F6E4:
            if (argument_count < 2)
                break;
            return (uint32)sub_8002f6e4(argument0, argument1);
        case 0x8002F70C:
            if (argument_count < 2)
                break;
            return (uint32)sub_8002f70c(argument0, argument1);
        case 0x80030178:
            return (uint32)sub_80030178();
        case 0x800301A8:
            return (uint32)sub_800301a8();
        case 0x800301F0:
            if (argument_count < 1)
                break;
            return (uint32)sub_800301f0(argument0);
        case 0x80030464:
            return (uint32)sub_80030464();
        case 0x8003048C:
            return (uint32)sub_8003048c();
        case 0x800304CC:
            return (uint32)sub_800304cc();
        case 0x800304EC:
            return (uint32)sub_800304ec();
        case 0x800304F4:
            return (uint32)sub_800304f4();
        case 0x80030528:
            if (argument_count < 1)
                break;
            return (uint32)sub_80030528(argument0);
        case 0x80030750:
            return (uint32)sub_80030750();
        case 0x80030D54:
            if (argument_count < 2)
                break;
            return (uint32)sub_80030d54(argument0, argument1);
        case 0x80030EB8:
            if (argument_count < 1)
                break;
            return (uint32)sub_80030eb8(argument0);
        case 0x80030EF8:
            if (argument_count < 3)
                break;
            return (uint32)sub_80030ef8(argument0, argument1, argument2);
        case 0x80030F64:
            if (argument_count < 2)
                break;
            return (uint32)sub_80030f64(argument0, argument1, NULL);
        case 0x80031E64:
            if (argument_count < 1)
                break;
            return (uint32)sub_80031e64(argument0);
        case 0x80031EA4:
            if (argument_count < 1)
                break;
            return (uint32)sub_80031ea4(argument0);
        case 0x80031EDC:
            return (uint32)sub_80031edc();
        case 0x80031EFC:
            return (uint32)sub_80031efc();
        case 0x80031F24:
            if (argument_count < 1)
                break;
            return (uint32)sub_80031f24(argument0);
        case 0x80031F64:
            if (argument_count < 1)
                break;
            return (uint32)sub_80031f64(argument0);
        case 0x80031FA8:
            if (argument_count < 1)
                break;
            return (uint32)sub_80031fa8(argument0);
        case 0x80031FD8:
            if (argument_count < 1)
                break;
            return (uint32)sub_80031fd8(argument0);
        case 0x80032010:
            if (argument_count < 1)
                break;
            return (uint32)sub_80032010(argument0);
        case 0x80032040:
            if (argument_count < 1)
                break;
            return (uint32)sub_80032040(argument0);
        case 0x80032080:
            if (argument_count < 2)
                break;
            return (uint32)sub_80032080(argument0, argument1);
        case 0x800323F8:
            if (argument_count < 1)
                break;
            return (uint32)sub_800323f8(argument0);
        case 0x800324AC:
            if (argument_count < 1)
                break;
            return (uint32)sub_800324ac(argument0);
        case 0x80032518:
            if (argument_count < 1)
                break;
            return (uint32)sub_80032518(argument0);
        case 0x80032754:
            return (uint32)sub_80032754();
        case 0x80032F18:
            if (argument_count < 2)
                break;
            return (uint32)sub_80032f18(argument0, argument1);
        case 0x80032F3C:
            return (uint32)sub_80032f3c();
        case 0x8003307C:
            return (uint32)sub_8003307c();
        case 0x800330CC:
            return (uint32)sub_800330cc();
        case 0x80033104:
            return (uint32)sub_80033104();
        case 0x800331E0:
            return (uint32)sub_800331e0();
        case 0x800332A4:
            return (uint32)sub_800332a4();
        case 0x800332C4:
            return (uint32)sub_800332c4();
        case 0x800332E4:
            if (argument_count < 3)
                break;
            return (uint32)sub_800332e4(argument0, argument1, argument2);
        case 0x80033324:
            return (uint32)sub_80033324();
        case 0x800333D8:
            return (uint32)sub_800333d8();
        case 0x8003342C:
            return (uint32)sub_8003342c();
        case 0x8003346C:
            return (uint32)sub_8003346c();
        case 0x800337BC:
            return (uint32)sub_800337bc();
        case 0x800337DC:
            return (uint32)sub_800337dc();
        case 0x80033814:
            return (uint32)sub_80033814();
        case 0x80033DC0:
            return (uint32)sub_80033dc0();
        case 0x80033EF0:
            return (uint32)sub_80033ef0();
        case 0x80034000:
            return (uint32)sub_80034000();
        case 0x80034040:
            return (uint32)sub_80034040();
        case 0x80034160:
            return (uint32)sub_80034160();
        case 0x8003423C:
            return (uint32)sub_8003423c();
        case 0x80034274:
            return (uint32)sub_80034274();
        case 0x800342AC:
            return (uint32)sub_800342ac();
        case 0x800342EC:
            return (uint32)sub_800342ec();
        case 0x800346C0:
            if (argument_count < 3)
                break;
            return (uint32)sub_800346c0(argument0, argument1, argument2);
        case 0x80034718:
            return (uint32)sub_80034718();
        case 0x80034930:
            if (argument_count < 2)
                break;
            return (uint32)sub_80034930(argument0, argument1);
        case 0x80035044:
            return (uint32)sub_80035044();
        case 0x8003514C:
            return (uint32)sub_8003514c();
        case 0x80035184:
            if (argument_count < 2)
                break;
            return (uint32)sub_80035184(argument0, argument1);
        case 0x800353E8:
            if (argument_count < 2)
                break;
            return (uint32)sub_800353e8(argument0, argument1);
        case 0x8003545C:
            if (argument_count < 2)
                break;
            return (uint32)sub_8003545c(argument0, argument1);
        case 0x80035578:
            if (argument_count < 3)
                break;
            return (uint32)sub_80035578(argument0, argument1, argument2);
        case 0x80035740:
            if (argument_count < 1)
                break;
            return (uint32)sub_80035740(argument0);
        case 0x800357B4:
            return (uint32)sub_800357b4();
        case 0x800357C0:
            return (uint32)sub_800357c0();
        case 0x800358F8:
            return (uint32)sub_800358f8();
        case 0x80035A38:
            return (uint32)sub_80035a38();
        case 0x80035F18:
            return (uint32)sub_80035f18();
        case 0x80035F98:
            return (uint32)sub_80035f98();
        case 0x80036130:
            return (uint32)sub_80036130();
        case 0x800361BC:
            return (uint32)sub_800361bc();
        case 0x8003623C:
            return (uint32)sub_8003623c();
        case 0x800362A0:
            if (argument_count < 3)
                break;
            return (uint32)sub_800362a0(argument0, argument1, argument2);
        case 0x80036440:
            if (argument_count < 2)
                break;
            return (uint32)sub_80036440(argument0, argument1);
        case 0x800364C4:
            if (argument_count < 2)
                break;
            return (uint32)sub_800364c4(argument0, argument1);
        case 0x80036824:
            if (argument_count < 2)
                break;
            return (uint32)sub_80036824(argument0, argument1);
        case 0x800369C8:
            return (uint32)sub_800369c8();
        case 0x80036A84:
            if (argument_count < 2)
                break;
            return (uint32)sub_80036a84(argument0, argument1);
        case 0x80036E84:
            if (argument_count < 2)
                break;
            return (uint32)sub_80036e84(argument0, argument1);
        case 0x80037070:
            return (uint32)sub_80037070();
        case 0x80037108:
            return (uint32)sub_80037108();
        case 0x800371C4:
            return (uint32)sub_800371c4();
        case 0x80037E50:
            return (uint32)sub_80037e50();
        case 0x80038054:
            if (argument_count < 2)
                break;
            return (uint32)sub_80038054(argument0, argument1);
        case 0x80038078:
            if (argument_count < 2)
                break;
            return (uint32)sub_80038078(argument0, argument1);
        case 0x8003809C:
            return (uint32)sub_8003809c();
        case 0x80038100:
            return (uint32)sub_80038100();
        case 0x800382D4:
            if (argument_count < 1)
                break;
            return (uint32)sub_800382d4(argument0);
        case 0x80038320:
            if (argument_count < 1)
                break;
            return (uint32)sub_80038320(argument0);
        case 0x800383AC:
            return (uint32)sub_800383ac();
        case 0x80038A3C:
            return (uint32)sub_80038a3c();
        case 0x80038A6C:
            return (uint32)sub_80038a6c();
        case 0x80038AD4:
            return (uint32)sub_80038ad4();
        case 0x80038AF4:
            if (argument_count < 1)
                break;
            return (uint32)sub_80038af4(argument0);
        case 0x80038B2C:
            return (uint32)sub_80038b2c();
        case 0x80038C28:
            return (uint32)sub_80038c28();
        case 0x80038C78:
            return (uint32)sub_80038c78();
        case 0x80038D1C:
            if (argument_count < 1)
                break;
            return (uint32)sub_80038d1c(argument0);
        case 0x80038E48:
            return (uint32)sub_80038e48();
        case 0x80038E58:
            return (uint32)sub_80038e58();
        case 0x80038E98:
            if (argument_count < 1)
                break;
            return (uint32)sub_80038e98(argument0);
        case 0x80038F00:
            if (argument_count < 1)
                break;
            return (uint32)sub_80038f00(argument0);
        case 0x80039028:
            if (argument_count < 1)
                break;
            return (uint32)sub_80039028(argument0);
        case 0x800390E4:
            if (argument_count < 1)
                break;
            return (uint32)sub_800390e4(argument0);
        case 0x8003916C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8003916c(argument0);
        case 0x80039230:
            if (argument_count < 1)
                break;
            return (uint32)sub_80039230(argument0);
        case 0x80039380:
            if (argument_count < 1)
                break;
            return (uint32)sub_80039380(argument0);
        case 0x80039388:
            if (argument_count < 1)
                break;
            return (uint32)sub_80039388(argument0);
        case 0x800393B0:
            if (argument_count < 1)
                break;
            return (uint32)sub_800393b0(argument0);
        case 0x800393B8:
            if (argument_count < 1)
                break;
            return (uint32)sub_800393b8(argument0);
        case 0x80039420:
            return (uint32)sub_80039420();
        case 0x80039430:
            return (uint32)sub_80039430();
        case 0x800395D4:
            return (uint32)sub_800395d4();
        case 0x800396E8:
            return (uint32)sub_800396e8();
        case 0x800399D4:
            return (uint32)sub_800399d4();
        case 0x8003A008:
            return (uint32)sub_8003a008();
        case 0x8003A07C:
            return (uint32)sub_8003a07c();
        case 0x8003A2B8:
            return (uint32)sub_8003a2b8();
        case 0x8003A444:
            return (uint32)sub_8003a444();
        case 0x8003A4E8:
            return (uint32)sub_8003a4e8();
        case 0x8003A598:
            return (uint32)sub_8003a598();
        case 0x8003A5C0:
            return (uint32)sub_8003a5c0();
        case 0x8003A624:
            return (uint32)sub_8003a624();
        case 0x8003A62C:
            return (uint32)sub_8003a62c();
        case 0x8003A68C:
            return (uint32)sub_8003a68c();
        case 0x8003AA60:
            return (uint32)sub_8003aa60();
        case 0x8003ABEC:
            return (uint32)sub_8003abec();
        case 0x8003AC8C:
            return (uint32)sub_8003ac8c();
        case 0x8003ADB4:
            return (uint32)sub_8003adb4();
        case 0x8003AFC0:
            if (argument_count < 3)
                break;
            return (uint32)sub_8003afc0(argument0, argument1, argument2);
        case 0x8003B2E4:
            if (argument_count < 1)
                break;
            return (uint32)sub_8003b2e4(argument0);
        case 0x8003B34C:
            return (uint32)sub_8003b34c();
        case 0x8003B758:
            if (argument_count < 1)
                break;
            return (uint32)sub_8003b758(argument0);
        case 0x8003B924:
            return (uint32)sub_8003b924();
        case 0x8003C250:
            return (uint32)sub_8003c250();
        case 0x8003C650:
            return (uint32)sub_8003c650();
        case 0x8003C690:
            return (uint32)sub_8003c690();
        case 0x8003CB90:
            return (uint32)sub_8003cb90();
        case 0x8003D138:
            return (uint32)sub_8003d138();
        case 0x8003D280:
            return (uint32)sub_8003d280();
        case 0x8003D458:
            return (uint32)sub_8003d458();
        case 0x8003D5C0:
            return (uint32)sub_8003d5c0();
        case 0x8003D9C8:
            return (uint32)sub_8003d9c8();
        case 0x8003DA08:
            return (uint32)sub_8003da08();
        case 0x8003DCF0:
            return (uint32)sub_8003dcf0();
        case 0x8003DE5C:
            return (uint32)sub_8003de5c();
        case 0x8003DFF8:
            if (argument_count < 1)
                break;
            return (uint32)sub_8003dff8(argument0);
        case 0x8003E69C:
            return (uint32)sub_8003e69c();
        case 0x8003ECBC:
            return (uint32)sub_8003ecbc();
        case 0x8003EF40:
            return (uint32)sub_8003ef40();
        case 0x8003EFE0:
            return (uint32)sub_8003efe0();
        case 0x8003F018:
            return (uint32)sub_8003f018();
        case 0x8003F054:
            return (uint32)sub_8003f054();
        case 0x8003F284:
            return (uint32)sub_8003f284();
        case 0x8003F2CC:
            return (uint32)sub_8003f2cc();
        case 0x8003F520:
            return (uint32)sub_8003f520();
        case 0x8003FA80:
            if (argument_count < 2)
                break;
            return (uint32)sub_8003fa80(argument0, argument1);
        case 0x8003FD48:
            if (argument_count < 2)
                break;
            return (uint32)sub_8003fd48(argument0, argument1);
        case 0x800401AC:
            if (argument_count < 2)
                break;
            return (uint32)sub_800401ac(argument0, argument1);
        case 0x80040530:
            if (argument_count < 1)
                break;
            return (uint32)sub_80040530(argument0);
        case 0x8004084C:
            if (argument_count < 3)
                break;
            return (uint32)sub_8004084c(argument0, argument1, argument2);
        case 0x80040900:
            if (argument_count < 3)
                break;
            return (uint32)sub_80040900(argument0, argument1, argument2);
        case 0x800409AC:
            return (uint32)sub_800409ac();
        case 0x800409D4:
            if (argument_count < 1)
                break;
            return (uint32)sub_800409d4(argument0);
        case 0x80040A18:
            return (uint32)sub_80040a18();
        case 0x80040A24:
            return (uint32)sub_80040a24();
        case 0x80040E70:
            return (uint32)sub_80040e70();
        case 0x80041120:
            return (uint32)sub_80041120();
        case 0x800414A4:
            return (uint32)sub_800414a4();
        case 0x800414D8:
            return (uint32)sub_800414d8();
        case 0x80041654:
            return (uint32)sub_80041654();
        case 0x80041670:
            return (uint32)sub_80041670();
        case 0x8004167C:
            return (uint32)sub_8004167c();
        case 0x80041698:
            return (uint32)sub_80041698();
        case 0x80041794:
            if (argument_count < 1)
                break;
            return (uint32)sub_80041794(argument0);
        case 0x80041900:
            return (uint32)sub_80041900();
        case 0x80041920:
            if (argument_count < 3)
                break;
            return (uint32)sub_80041920(argument0, argument1, argument2);
        case 0x8004197C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004197c(argument0);
        case 0x800419C0:
            return (uint32)sub_800419c0();
        case 0x80041ADC:
            return (uint32)sub_80041adc();
        case 0x80041F88:
            if (argument_count < 1)
                break;
            return (uint32)sub_80041f88(argument0);
        case 0x80042524:
            return (uint32)sub_80042524();
        case 0x8004287C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004287c(argument0);
        case 0x800428F8:
            return (uint32)sub_800428f8();
        case 0x8004291C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004291c(argument0);
        case 0x80042938:
            return (uint32)sub_80042938();
        case 0x80042BF4:
            return (uint32)sub_80042bf4();
        case 0x80042C2C:
            return (uint32)sub_80042c2c();
        case 0x80043BF8:
            return (uint32)sub_80043bf8();
        case 0x80043EFC:
            if (argument_count < 3)
                break;
            return (uint32)sub_80043efc(argument0, argument1, argument2);
        case 0x80044034:
            if (argument_count < 1)
                break;
            return (uint32)sub_80044034(argument0);
        case 0x8004409C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004409c(argument0);
        case 0x80044474:
            if (argument_count < 2)
                break;
            return (uint32)sub_80044474(argument0, argument1);
        case 0x8004463C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004463c(argument0);
        case 0x8004470C:
            if (argument_count < 2)
                break;
            return (uint32)sub_8004470c(argument0, argument1);
        case 0x80044884:
            if (argument_count < 2)
                break;
            return (uint32)sub_80044884(argument0, argument1);
        case 0x800449DC:
            if (argument_count < 2)
                break;
            return (uint32)sub_800449dc(argument0, argument1);
        case 0x80044A6C:
            if (argument_count < 1)
                break;
            return (uint32)sub_80044a6c(argument0);
        case 0x80044C28:
            if (argument_count < 1)
                break;
            return (uint32)sub_80044c28(argument0);
        case 0x80045148:
            if (argument_count < 1)
                break;
            return (uint32)sub_80045148(argument0);
        case 0x800451AC:
            if (argument_count < 3)
                break;
            return (uint32)sub_800451ac(argument0, argument1, argument2);
        case 0x80045240:
            return (uint32)sub_80045240();
        case 0x80045278:
            return (uint32)sub_80045278();
        case 0x800455B8:
            return (uint32)sub_800455b8();
        case 0x80045768:
            return (uint32)sub_80045768();
        case 0x80045898:
            return (uint32)sub_80045898();
        case 0x800458D0:
            return (uint32)sub_800458d0();
        case 0x8004596C:
            return (uint32)sub_8004596c();
        case 0x80045A40:
            return (uint32)sub_80045a40();
        case 0x80045FF0:
            return (uint32)sub_80045ff0();
        case 0x80046030:
            return (uint32)sub_80046030();
        case 0x80046124:
            if (argument_count < 1)
                break;
            return (uint32)sub_80046124(argument0);
        case 0x800463B8:
            if (argument_count < 2)
                break;
            return (uint32)sub_800463b8(argument0, argument1);
        case 0x800464B0:
            if (argument_count < 1)
                break;
            return (uint32)sub_800464b0(argument0);
        case 0x8004651C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004651c(argument0);
        case 0x8004658C:
            return (uint32)sub_8004658c();
        case 0x80046600:
            return (uint32)sub_80046600();
        case 0x80046928:
            if (argument_count < 1)
                break;
            return (uint32)sub_80046928(argument0);
        case 0x800469EC:
            return (uint32)sub_800469ec();
        case 0x80046DFC:
            return (uint32)sub_80046dfc();
        case 0x8004701C:
            return (uint32)sub_8004701c();
        case 0x80047470:
            if (argument_count < 1)
                break;
            return (uint32)sub_80047470(argument0);
        case 0x800475B0:
            if (argument_count < 1)
                break;
            return (uint32)sub_800475b0(argument0);
        case 0x800476F0:
            if (argument_count < 1)
                break;
            return (uint32)sub_800476f0(argument0);
        case 0x8004781C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004781c(argument0);
        case 0x80047A54:
            return (uint32)sub_80047a54();
        case 0x80047B00:
            if (argument_count < 1)
                break;
            return (uint32)sub_80047b00(argument0);
        case 0x80047D68:
            if (argument_count < 1)
                break;
            return (uint32)sub_80047d68(argument0);
        case 0x8004833C:
            return (uint32)sub_8004833c();
        case 0x8004856C:
            return (uint32)sub_8004856c();
        case 0x80048658:
            return (uint32)sub_80048658();
        case 0x80048758:
            if (argument_count < 1)
                break;
            return (uint32)sub_80048758(argument0);
        case 0x80048A40:
            return (uint32)sub_80048a40();
        case 0x80048A80:
            return (uint32)sub_80048a80();
        case 0x80048AD4:
            if (argument_count < 1)
                break;
            return (uint32)sub_80048ad4(argument0);
        case 0x80048B48:
            return (uint32)sub_80048b48();
        case 0x80048BDC:
            return (uint32)sub_80048bdc();
        case 0x8004925C:
            if (argument_count < 3)
                break;
            return (uint32)sub_8004925c(argument0, argument1, argument2);
        case 0x80049438:
            return (uint32)sub_80049438();
        case 0x8004960C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004960c(argument0);
        case 0x8004966C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004966c(argument0);
        case 0x8004986C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004986c(argument0);
        case 0x80049A1C:
            if (argument_count < 1)
                break;
            return (uint32)sub_80049a1c(argument0);
        case 0x80049AF8:
            if (argument_count < 1)
                break;
            return (uint32)sub_80049af8(argument0);
        case 0x80049C4C:
            if (argument_count < 1)
                break;
            return (uint32)sub_80049c4c(argument0);
        case 0x80049E28:
            if (argument_count < 1)
                break;
            return (uint32)sub_80049e28(argument0);
        case 0x80049EC4:
            if (argument_count < 1)
                break;
            return (uint32)sub_80049ec4(argument0);
        case 0x80049F38:
            if (argument_count < 1)
                break;
            return (uint32)sub_80049f38(argument0);
        case 0x8004A018:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004a018(argument0);
        case 0x8004A0C0:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004a0c0(argument0);
        case 0x8004A2D8:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004a2d8(argument0);
        case 0x8004A394:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004a394(argument0);
        case 0x8004A474:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004a474(argument0);
        case 0x8004A56C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004a56c(argument0);
        case 0x8004A678:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004a678(argument0);
        case 0x8004A7E8:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004a7e8(argument0);
        case 0x8004AA3C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004aa3c(argument0);
        case 0x8004AC90:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004ac90(argument0);
        case 0x8004AD98:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004ad98(argument0);
        case 0x8004AFAC:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004afac(argument0);
        case 0x8004B108:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004b108(argument0);
        case 0x8004B308:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004b308(argument0);
        case 0x8004B3C8:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004b3c8(argument0);
        case 0x8004B4D8:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004b4d8(argument0);
        case 0x8004B6E4:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004b6e4(argument0);
        case 0x8004B88C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004b88c(argument0);
        case 0x8004BA0C:
            return (uint32)sub_8004ba0c();
        case 0x8004BA7C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004ba7c(argument0);
        case 0x8004BDA8:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004bda8(argument0);
        case 0x8004BED4:
            return (uint32)sub_8004bed4();
        case 0x8004C2C8:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004c2c8(argument0);
        case 0x8004C338:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004c338(argument0);
        case 0x8004C3C4:
            return (uint32)sub_8004c3c4();
        case 0x8004C5B8:
            return (uint32)sub_8004c5b8();
        case 0x8004C6EC:
            return (uint32)sub_8004c6ec();
        case 0x8004C824:
            return (uint32)sub_8004c824();
        case 0x8004C858:
            return (uint32)sub_8004c858();
        case 0x8004C86C:
            return (uint32)sub_8004c86c();
        case 0x8004C998:
            return (uint32)sub_8004c998();
        case 0x8004CA28:
            return (uint32)sub_8004ca28();
        case 0x8004CE9C:
            return (uint32)sub_8004ce9c();
        case 0x8004CEB0:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004ceb0(argument0);
        case 0x8004D038:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004d038(argument0);
        case 0x8004D124:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004d124(argument0);
        case 0x8004D1AC:
            return (uint32)sub_8004d1ac();
        case 0x8004D4D0:
            return (uint32)sub_8004d4d0();
        case 0x8004D680:
            return (uint32)sub_8004d680();
        case 0x8004D794:
            return (uint32)sub_8004d794();
        case 0x8004D800:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004d800(argument0);
        case 0x8004D84C:
            return (uint32)sub_8004d84c();
        case 0x8004DB40:
            return (uint32)sub_8004db40();
        case 0x8004DC38:
            return (uint32)sub_8004dc38();
        case 0x8004E060:
            return (uint32)sub_8004e060();
        case 0x8004E0FC:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004e0fc(argument0);
        case 0x8004E124:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004e124(argument0);
        case 0x8004E3A8:
            return (uint32)sub_8004e3a8();
        case 0x8004E620:
            return (uint32)sub_8004e620();
        case 0x8004E80C:
            return (uint32)sub_8004e80c();
        case 0x8004E900:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004e900(argument0);
        case 0x8004E94C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004e94c(argument0);
        case 0x8004EBA4:
            if (argument_count < 2)
                break;
            return (uint32)sub_8004eba4(argument0, argument1);
        case 0x8004ECC4:
            if (argument_count < 2)
                break;
            return (uint32)sub_8004ecc4(argument0, argument1);
        case 0x8004ED9C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004ed9c(argument0);
        case 0x8004EE68:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004ee68(argument0);
        case 0x8004EF80:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004ef80(argument0);
        case 0x8004F034:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004f034(argument0);
        case 0x8004F0C8:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004f0c8(argument0);
        case 0x8004F4D8:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004f4d8(argument0);
        case 0x8004F570:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004f570(argument0);
        case 0x8004FB28:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004fb28(argument0);
        case 0x8004FBA4:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004fba4(argument0);
        case 0x8004FC04:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004fc04(argument0);
        case 0x8004FCF0:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004fcf0(argument0);
        case 0x8004FD38:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004fd38(argument0);
        case 0x8004FD9C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004fd9c(argument0);
        case 0x8004FF88:
            if (argument_count < 1)
                break;
            return (uint32)sub_8004ff88(argument0);
        case 0x800500EC:
            if (argument_count < 1)
                break;
            return (uint32)sub_800500ec(argument0);
        case 0x80050134:
            if (argument_count < 1)
                break;
            return (uint32)sub_80050134(argument0);
        case 0x80050184:
            if (argument_count < 1)
                break;
            return (uint32)sub_80050184(argument0);
        case 0x800502D4:
            if (argument_count < 1)
                break;
            return (uint32)sub_800502d4(argument0);
        case 0x80050534:
            if (argument_count < 1)
                break;
            return (uint32)sub_80050534(argument0);
        case 0x800505DC:
            if (argument_count < 2)
                break;
            return (uint32)sub_800505dc(argument0, argument1);
        case 0x80050690:
            if (argument_count < 2)
                break;
            return (uint32)sub_80050690(argument0, argument1);
        case 0x80050720:
            if (argument_count < 1)
                break;
            return (uint32)sub_80050720(argument0);
        case 0x800508D0:
            if (argument_count < 1)
                break;
            return (uint32)sub_800508d0(argument0);
        case 0x80050AC0:
            if (argument_count < 1)
                break;
            return (uint32)sub_80050ac0(argument0);
        case 0x80050BB4:
            if (argument_count < 1)
                break;
            return (uint32)sub_80050bb4(argument0);
        case 0x80050DE8:
            if (argument_count < 1)
                break;
            return (uint32)sub_80050de8(argument0);
        case 0x80050FA8:
            if (argument_count < 2)
                break;
            return (uint32)sub_80050fa8(argument0, argument1);
        case 0x80051010:
            if (argument_count < 1)
                break;
            return (uint32)sub_80051010(argument0);
        case 0x80051304:
            if (argument_count < 1)
                break;
            return (uint32)sub_80051304(argument0);
        case 0x800513D8:
            if (argument_count < 1)
                break;
            return (uint32)sub_800513d8(argument0);
        case 0x80051414:
            return (uint32)sub_80051414();
        case 0x80051498:
            return (uint32)sub_80051498();
        case 0x8005157C:
            return (uint32)sub_8005157c();
        case 0x800517F4:
            return (uint32)sub_800517f4();
        case 0x80051824:
            return (uint32)sub_80051824();
        case 0x80051840:
            return (uint32)sub_80051840();
        case 0x80051A54:
            if (argument_count < 1)
                break;
            return (uint32)sub_80051a54(argument0);
        case 0x80051BA0:
            if (argument_count < 1)
                break;
            return (uint32)sub_80051ba0(argument0);
        case 0x80051E34:
            if (argument_count < 1)
                break;
            return (uint32)sub_80051e34(argument0);
        case 0x80051F88:
            if (argument_count < 1)
                break;
            return (uint32)sub_80051f88(argument0);
        case 0x80052000:
            if (argument_count < 1)
                break;
            return (uint32)sub_80052000(argument0);
        case 0x80052060:
            if (argument_count < 1)
                break;
            return (uint32)sub_80052060(argument0);
        case 0x800520C8:
            if (argument_count < 1)
                break;
            return (uint32)sub_800520c8(argument0);
        case 0x800522B0:
            if (argument_count < 1)
                break;
            return (uint32)sub_800522b0(argument0);
        case 0x80052FA8:
            if (argument_count < 1)
                break;
            return (uint32)sub_80052fa8(argument0);
        case 0x80053214:
            if (argument_count < 2)
                break;
            return (uint32)sub_80053214(argument0, argument1);
        case 0x800535D4:
            if (argument_count < 1)
                break;
            return (uint32)sub_800535d4(argument0);
        case 0x800539D0:
            return (uint32)sub_800539d0();
        case 0x80053B24:
            if (argument_count < 2)
                break;
            return (uint32)sub_80053b24(argument0, argument1);
        case 0x80053B5C:
            return (uint32)sub_80053b5c();
        case 0x80053B78:
            return (uint32)sub_80053b78();
        case 0x80053BAC:
            return (uint32)sub_80053bac();
        case 0x80053DA0:
            return (uint32)sub_80053da0();
        case 0x80053E08:
            return (uint32)sub_80053e08();
        case 0x800540E0:
            return (uint32)sub_800540e0();
        case 0x80054DBC:
            return (uint32)sub_80054dbc();
        case 0x80054EBC:
            if (argument_count < 1)
                break;
            return (uint32)sub_80054ebc(argument0);
        case 0x80055C4C:
            if (argument_count < 2)
                break;
            return (uint32)sub_80055c4c(argument0, argument1);
        case 0x80055CBC:
            if (argument_count < 1)
                break;
            return (uint32)sub_80055cbc(argument0);
        case 0x80055D2C:
            if (argument_count < 1)
                break;
            return (uint32)sub_80055d2c(argument0);
        case 0x80055D94:
            if (argument_count < 1)
                break;
            return (uint32)sub_80055d94(argument0);
        case 0x80055DFC:
            if (argument_count < 1)
                break;
            return (uint32)sub_80055dfc(argument0);
        case 0x80055E64:
            if (argument_count < 1)
                break;
            return (uint32)sub_80055e64(argument0);
        case 0x80055ED8:
            if (argument_count < 1)
                break;
            return (uint32)sub_80055ed8(argument0);
        case 0x80055F3C:
            if (argument_count < 1)
                break;
            return (uint32)sub_80055f3c(argument0);
        case 0x80055FA4:
            if (argument_count < 1)
                break;
            return (uint32)sub_80055fa4(argument0);
        case 0x80056004:
            if (argument_count < 1)
                break;
            return (uint32)sub_80056004(argument0);
        case 0x80056080:
            if (argument_count < 1)
                break;
            return (uint32)sub_80056080(argument0);
        case 0x800560E4:
            if (argument_count < 1)
                break;
            return (uint32)sub_800560e4(argument0);
        case 0x80056148:
            if (argument_count < 1)
                break;
            return (uint32)sub_80056148(argument0);
        case 0x80056164:
            if (argument_count < 1)
                break;
            return (uint32)sub_80056164(argument0);
        case 0x80056198:
            return (uint32)sub_80056198();
        case 0x80056234:
            if (argument_count < 2)
                break;
            return (uint32)sub_80056234(argument0, argument1);
        case 0x8005628C:
            return (uint32)sub_8005628c();
        case 0x8005650C:
            return (uint32)sub_8005650c();
        case 0x80056518:
            return (uint32)sub_80056518();
        case 0x800565E8:
            if (argument_count < 1)
                break;
            return (uint32)sub_800565e8(argument0);
        case 0x80056634:
            return (uint32)sub_80056634();
        case 0x800566F0:
            return (uint32)sub_800566f0();
        case 0x8005671C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8005671c(argument0);
        case 0x80056778:
            return (uint32)sub_80056778();
        case 0x80056964:
            if (argument_count < 1)
                break;
            return (uint32)sub_80056964(argument0);
        case 0x8005697C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8005697c(argument0);
        case 0x80056A10:
            return (uint32)sub_80056a10();
        case 0x80056A44:
            return (uint32)sub_80056a44();
        case 0x80056A54:
            if (argument_count < 2)
                break;
            return (uint32)sub_80056a54(argument0, argument1);
        case 0x80056B1C:
            return (uint32)movie_current_buffer();
        case 0x80056B30:
            return (uint32)movie_init_decoder();
        case 0x80056C9C:
            return (uint32)movie_stream_complete();
        case 0x80056D7C:
            return (uint32)movie_queue_stream();
        case 0x80056DDC:
            if (argument_count < 1)
                break;
            return (uint32)movie_wait_frame(argument0);
        case 0x80056E8C:
            return (uint32)movie_end();
        case 0x80056EB8:
            if (argument_count < 1)
                break;
            return (uint32)movie_decode(argument0);
        case 0x80056FD8:
            if (argument_count < 1)
                break;
            return (uint32)sub_80056fd8(argument0);
        case 0x80057084:
            if (argument_count < 2)
                break;
            return (uint32)sub_80057084(argument0, argument1);
        case 0x80057170:
            movie_output_callback();
            return 0;
        case 0x800572C8:
            if (argument_count < 1)
                break;
            return (uint32)sub_800572c8(argument0);
        case 0x800573E8:
            return (uint32)sub_800573e8();
        case 0x80057640:
            return (uint32)sub_80057640();
        case 0x80057720:
            return (uint32)sub_80057720();
        case 0x80057BBC:
            return (uint32)sub_80057bbc();
        case 0x80057CC4:
            return (uint32)sub_80057cc4();
        case 0x80057CD0:
            return (uint32)sub_80057cd0();
        case 0x80057E34:
            return (uint32)sub_80057e34();
        case 0x80057EBC:
            return (uint32)sub_80057ebc();
        case 0x80058134:
            return (uint32)sub_80058134();
        case 0x800581C4:
            return (uint32)sub_800581c4();
        case 0x800582F8:
            return (uint32)sub_800582f8();
        case 0x80058800:
            return (uint32)sub_80058800();
        case 0x80058854:
            return (uint32)sub_80058854();
        case 0x80058864:
            return (uint32)sub_80058864();
        case 0x80058888:
            return (uint32)sub_80058888();
        case 0x8005889C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8005889c(argument0);
        case 0x80058938:
            if (argument_count < 1)
                break;
            return (uint32)sub_80058938(argument0);
        case 0x8005911C:
            return (uint32)sub_8005911c();
        case 0x800592E0:
            if (argument_count < 3)
                break;
            return (uint32)sub_800592e0(argument0, argument1, argument2);
        case 0x80059488:
            if (argument_count < 3)
                break;
            return (uint32)sub_80059488(argument0, argument1, argument2);
        case 0x800598BC:
            if (argument_count < 3)
                break;
            return (uint32)sub_800598bc(argument0, argument1, argument2);
        case 0x80059BD8:
            if (argument_count < 3)
                break;
            return (uint32)sub_80059bd8(argument0, argument1, argument2);
        case 0x80059E1C:
            if (argument_count < 3)
                break;
            return (uint32)sub_80059e1c(argument0, argument1, argument2);
        case 0x8005A034:
            if (argument_count < 3)
                break;
            return (uint32)sub_8005a034(argument0, argument1, argument2);
        case 0x8005A2C4:
            if (argument_count < 2)
                break;
            return (uint32)sub_8005a2c4(argument0, argument1);
        case 0x8005AE8C:
            return (uint32)sub_8005ae8c();
        case 0x8005B124:
            return (uint32)sub_8005b124();
        case 0x8005B234:
            return (uint32)sub_8005b234();
        case 0x8005B308:
            return (uint32)sub_8005b308();
        case 0x8005B4E4:
            return (uint32)sub_8005b4e4();
        case 0x8005B5BC:
            return (uint32)sub_8005b5bc();
        case 0x8005B654:
            return (uint32)sub_8005b654();
        case 0x8005BC64:
            return (uint32)sub_8005bc64();
        case 0x8005BE0C:
            return (uint32)sub_8005be0c();
        case 0x8005C270:
            return (uint32)sub_8005c270();
        case 0x8005C5B0:
            if (argument_count < 1)
                break;
            return (uint32)sub_8005c5b0(argument0);
        case 0x8005C990:
            if (argument_count < 1)
                break;
            return (uint32)sub_8005c990(argument0);
        case 0x8005CEDC:
            if (argument_count < 1)
                break;
            return (uint32)sub_8005cedc(argument0);
        case 0x8005D344:
            if (argument_count < 1)
                break;
            return (uint32)sub_8005d344(argument0);
        case 0x8005D844:
            if (argument_count < 1)
                break;
            return (uint32)sub_8005d844(argument0);
        case 0x8005DCA8:
            if (argument_count < 1)
                break;
            return (uint32)sub_8005dca8(argument0);
        case 0x8005DEFC:
            if (argument_count < 1)
                break;
            return (uint32)sub_8005defc(argument0);
        case 0x8005E10C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8005e10c(argument0);
        case 0x8005E2C0:
            return (uint32)sub_8005e2c0();
        case 0x8005E40C:
            if (argument_count < 3)
                break;
            return (uint32)sub_8005e40c(argument0, argument1, argument2);
        case 0x8005E460:
            if (argument_count < 3)
                break;
            return (uint32)sub_8005e460(argument0, argument1, argument2);
        case 0x8005E7EC:
            if (argument_count < 3)
                break;
            return (uint32)sub_8005e7ec(argument0, argument1, argument2);
        case 0x8005EAB8:
            if (argument_count < 3)
                break;
            return (uint32)sub_8005eab8(argument0, argument1, argument2);
        case 0x8005EBB8:
            if (argument_count < 3)
                break;
            return (uint32)sub_8005ebb8(argument0, argument1, argument2);
        case 0x8005EFF0:
            if (argument_count < 2)
                break;
            return (uint32)sub_8005eff0(argument0, argument1);
        case 0x8005F0E4:
            if (argument_count < 1)
                break;
            return (uint32)sub_8005f0e4(argument0);
        case 0x800619E0:
            if (argument_count < 1)
                break;
            return (uint32)sub_800619e0(argument0);
        case 0x80061C3C:
            if (argument_count < 1)
                break;
            return (uint32)sub_80061c3c(argument0);
        case 0x80061E50:
            if (argument_count < 3)
                break;
            return (uint32)sub_80061e50(argument0, argument1, argument2);
        case 0x8006258C:
            if (argument_count < 2)
                break;
            return (uint32)sub_8006258c(argument0, argument1);
        case 0x80062D68:
            if (argument_count < 1)
                break;
            return (uint32)sub_80062d68(argument0);
        case 0x80062EC8:
            if (argument_count < 1)
                break;
            return (uint32)sub_80062ec8(argument0);
        case 0x8006305C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8006305c(argument0);
        case 0x800631F8:
            if (argument_count < 1)
                break;
            return (uint32)sub_800631f8(argument0);
        case 0x800633D8:
            if (argument_count < 1)
                break;
            return (uint32)sub_800633d8(argument0);
        case 0x8006358C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8006358c(argument0);
        case 0x80063AF0:
            return (uint32)sub_80063af0();
        case 0x80063F80:
            return (uint32)sub_80063f80();
        case 0x80063FCC:
            return (uint32)sub_80063fcc();
        case 0x8006466C:
            return (uint32)sub_8006466c();
        case 0x8006477C:
            if (argument_count < 2)
                break;
            return (uint32)sub_8006477c(argument0, argument1);
        case 0x80064CB4:
            return (uint32)sub_80064cb4();
        case 0x80065D8C:
            return (uint32)sub_80065d8c();
        case 0x800662B4:
            return (uint32)sub_800662b4();
        case 0x80066A74:
            if (argument_count < 2)
                break;
            return (uint32)sub_80066a74(argument0, argument1);
        case 0x800684D4:
            return (uint32)sub_800684d4();
        case 0x8006903C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8006903c(argument0);
        case 0x80069158:
            if (argument_count < 1)
                break;
            return (uint32)sub_80069158(argument0);
        case 0x800692C4:
            if (argument_count < 1)
                break;
            return (uint32)sub_800692c4(argument0);
        case 0x800694CC:
            if (argument_count < 1)
                break;
            return (uint32)sub_800694cc(argument0);
        case 0x800696D4:
            if (argument_count < 1)
                break;
            return (uint32)sub_800696d4(argument0);
        case 0x80069900:
            if (argument_count < 1)
                break;
            return (uint32)sub_80069900(argument0);
        case 0x80069A9C:
            if (argument_count < 1)
                break;
            return (uint32)sub_80069a9c(argument0);
        case 0x80069C8C:
            if (argument_count < 1)
                break;
            return (uint32)sub_80069c8c(argument0);
        case 0x80069D18:
            if (argument_count < 1)
                break;
            return (uint32)sub_80069d18(argument0);
        case 0x80069D6C:
            if (argument_count < 1)
                break;
            return (uint32)sub_80069d6c(argument0);
        case 0x80069D88:
            if (argument_count < 1)
                break;
            return (uint32)sub_80069d88(argument0);
        case 0x80069E10:
            if (argument_count < 1)
                break;
            return (uint32)sub_80069e10(argument0);
        case 0x80069EC0:
            if (argument_count < 1)
                break;
            return (uint32)sub_80069ec0(argument0);
        case 0x80069F48:
            if (argument_count < 1)
                break;
            return (uint32)sub_80069f48(argument0);
        case 0x80069FC8:
            return (uint32)sub_80069fc8();
        case 0x8006A11C:
            return (uint32)sub_8006a11c();
        case 0x8006A978:
            if (argument_count < 2)
                break;
            return (uint32)sub_8006a978(argument0, argument1);
        case 0x8006AAFC:
            if (argument_count < 1)
                break;
            return (uint32)sub_8006aafc(argument0);
        case 0x8006ABB8:
            return (uint32)sub_8006abb8();
        case 0x8006AC34:
            return (uint32)sub_8006ac34();
        case 0x8006AF20:
            return (uint32)sub_8006af20();
        case 0x8006B110:
            return (uint32)sub_8006b110();
        case 0x8006C530:
            return (uint32)sub_8006c530();
        case 0x8006C588:
            return (uint32)sub_8006c588();
        case 0x8006C5B4:
            return (uint32)sub_8006c5b4();
        case 0x8006C5EC:
            if (argument_count < 2)
                break;
            return (uint32)sub_8006c5ec(argument0, argument1);
        case 0x8006C740:
            if (argument_count < 1)
                break;
            return (uint32)sub_8006c740(argument0);
        case 0x8006C99C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8006c99c(argument0);
        case 0x8006C9B8:
            return (uint32)sub_8006c9b8();
        case 0x8006CB10:
            return (uint32)sub_8006cb10();
        case 0x8006CB30:
            return (uint32)sub_8006cb30();
        case 0x8006CB44:
            if (argument_count < 1)
                break;
            return (uint32)sub_8006cb44(argument0);
        case 0x8006CCE0:
            return (uint32)sub_8006cce0();
        case 0x8006CD6C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8006cd6c(argument0);
        case 0x8006CE70:
            if (argument_count < 1)
                break;
            return (uint32)sub_8006ce70(argument0);
        case 0x8006D078:
            return (uint32)sub_8006d078();
        case 0x8006E7E0:
            if (argument_count < 2)
                break;
            return (uint32)sub_8006e7e0(argument0, argument1);
        case 0x8006E874:
            if (argument_count < 2)
                break;
            return (uint32)sub_8006e874(argument0, argument1);
        case 0x8006EE38:
            if (argument_count < 2)
                break;
            return (uint32)sub_8006ee38(argument0, argument1);
        case 0x8007084C:
            if (argument_count < 2)
                break;
            return (uint32)sub_8007084c(argument0, argument1);
        case 0x80070A34:
            if (argument_count < 1)
                break;
            return (uint32)sub_80070a34(argument0);
        case 0x80070A80:
            if (argument_count < 2)
                break;
            return (uint32)sub_80070a80(argument0, argument1);
        case 0x80070CF4:
            if (argument_count < 2)
                break;
            return (uint32)sub_80070cf4(argument0, argument1);
        case 0x800711FC:
            if (argument_count < 3)
                break;
            return (uint32)sub_800711fc(argument0, argument1, argument2);
        case 0x800713B0:
            if (argument_count < 1)
                break;
            return (uint32)sub_800713b0(argument0);
        case 0x800714E0:
            return (uint32)sub_800714e0();
        case 0x80071544:
            return (uint32)sub_80071544();
        case 0x800715AC:
            return (uint32)sub_800715ac();
        case 0x80071614:
            return (uint32)sub_80071614();
        case 0x8007167C:
            return (uint32)sub_8007167c();
        case 0x80071708:
            return (uint32)sub_80071708();
        case 0x800718A0:
            if (argument_count < 3)
                break;
            return (uint32)sub_800718a0(argument0, argument1, argument2);
        case 0x800718E0:
            return (uint32)sub_800718e0();
        case 0x80071950:
            if (argument_count < 1)
                break;
            return (uint32)sub_80071950(argument0);
        case 0x80071A00:
            if (argument_count < 1)
                break;
            return (uint32)sub_80071a00(argument0);
        case 0x80071AB0:
            if (argument_count < 1)
                break;
            return (uint32)sub_80071ab0(argument0);
        case 0x80071B54:
            if (argument_count < 1)
                break;
            return (uint32)sub_80071b54(argument0);
        case 0x80071BF8:
            if (argument_count < 1)
                break;
            return (uint32)sub_80071bf8(argument0);
        case 0x80071C9C:
            if (argument_count < 1)
                break;
            return (uint32)sub_80071c9c(argument0);
        case 0x80071D40:
            if (argument_count < 1)
                break;
            return (uint32)sub_80071d40(argument0);
        case 0x80071DE4:
            if (argument_count < 1)
                break;
            return (uint32)sub_80071de4(argument0);
        case 0x80071EBC:
            if (argument_count < 1)
                break;
            return (uint32)sub_80071ebc(argument0);
        case 0x80071F60:
            if (argument_count < 1)
                break;
            return (uint32)sub_80071f60(argument0);
        case 0x80072004:
            if (argument_count < 1)
                break;
            return (uint32)sub_80072004(argument0);
        case 0x80072094:
            if (argument_count < 1)
                break;
            return (uint32)sub_80072094(argument0);
        case 0x800720D4:
            if (argument_count < 2)
                break;
            return (uint32)sub_800720d4(argument0, argument1);
        case 0x80072114:
            return (uint32)sub_80072114();
        case 0x80072178:
            if (argument_count < 1)
                break;
            return (uint32)sub_80072178(argument0);
        case 0x80072938:
            if (argument_count < 2)
                break;
            return (uint32)sub_80072938(argument0, argument1);
        case 0x800729DC:
            return (uint32)sub_800729dc();
        case 0x80073928:
            if (argument_count < 1)
                break;
            return (uint32)sub_80073928(argument0);
        case 0x80073A30:
            return (uint32)sub_80073a30();
        case 0x80074560:
            if (argument_count < 2)
                break;
            return (uint32)sub_80074560(argument0, argument1);
        case 0x80074878:
            if (argument_count < 3)
                break;
            return (uint32)sub_80074878(argument0, argument1, argument2);
        case 0x8007599C:
            if (argument_count < 3)
                break;
            return (uint32)sub_8007599c(argument0, argument1, argument2);
        case 0x8007601C:
            if (argument_count < 2)
                break;
            return (uint32)sub_8007601c(argument0, argument1);
        case 0x80076274:
            return (uint32)sub_80076274();
        case 0x80076290:
            return (uint32)sub_80076290();
        case 0x800762B0:
            return (uint32)sub_800762b0();
        case 0x800762D0:
            return (uint32)sub_800762d0();
        case 0x800762F0:
            if (argument_count < 1)
                break;
            return (uint32)sub_800762f0(argument0);
        case 0x80076360:
            if (argument_count < 1)
                break;
            return (uint32)sub_80076360(argument0);
        case 0x800763CC:
            return (uint32)sub_800763cc();
        case 0x80076428:
            if (argument_count < 2)
                break;
            return (uint32)sub_80076428(argument0, argument1);
        case 0x80076A38:
            if (argument_count < 2)
                break;
            return (uint32)sub_80076a38(argument0, argument1);
        case 0x80076AC0:
            if (argument_count < 1)
                break;
            return (uint32)sub_80076ac0(argument0);
        case 0x80076B18:
            if (argument_count < 2)
                break;
            return (uint32)sub_80076b18(argument0, argument1);
        case 0x80076C54:
            if (argument_count < 2)
                break;
            return (uint32)sub_80076c54(argument0, argument1);
        case 0x80076DBC:
            return (uint32)sub_80076dbc();
        case 0x80076DEC:
            if (argument_count < 2)
                break;
            return (uint32)sub_80076dec(argument0, argument1);
        case 0x80076F04:
            return (uint32)sub_80076f04();
        case 0x80076FA4:
            return (uint32)sub_80076fa4();
        case 0x80077460:
            if (argument_count < 1)
                break;
            return (uint32)sub_80077460(argument0);
        case 0x8007754C:
            if (argument_count < 2)
                break;
            return (uint32)sub_8007754c(argument0, argument1);
        case 0x80077768:
            if (argument_count < 2)
                break;
            return (uint32)sub_80077768(argument0, argument1);
        case 0x8007786C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8007786c(argument0);
        case 0x80077A30:
            if (argument_count < 1)
                break;
            return (uint32)sub_80077a30(argument0);
        case 0x80077DD0:
            if (argument_count < 1)
                break;
            return (uint32)sub_80077dd0(argument0);
        case 0x80078234:
            if (argument_count < 2)
                break;
            return (uint32)sub_80078234(argument0, argument1);
        case 0x8007867C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8007867c(argument0);
        case 0x8007895C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8007895c(argument0);
        case 0x80078B28:
            if (argument_count < 1)
                break;
            return (uint32)sub_80078b28(argument0);
        case 0x80078D70:
            if (argument_count < 1)
                break;
            return (uint32)sub_80078d70(argument0);
        case 0x80078FDC:
            if (argument_count < 1)
                break;
            return (uint32)sub_80078fdc(argument0);
        case 0x800791E8:
            if (argument_count < 1)
                break;
            return (uint32)sub_800791e8(argument0);
        case 0x800793C4:
            if (argument_count < 3)
                break;
            return (uint32)sub_800793c4(argument0, argument1, argument2);
        case 0x800794D8:
            if (argument_count < 1)
                break;
            return (uint32)sub_800794d8(argument0);
        case 0x8007953C:
            return (uint32)sub_8007953c();
        case 0x80079564:
            if (argument_count < 1)
                break;
            return (uint32)sub_80079564(argument0);
        case 0x800795C8:
            if (argument_count < 1)
                break;
            return (uint32)sub_800795c8(argument0);
        case 0x8007961C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8007961c(argument0);
        case 0x80079680:
            return (uint32)sub_80079680();
        case 0x800796B8:
            if (argument_count < 2)
                break;
            return (uint32)sub_800796b8(argument0, argument1);
        case 0x80079730:
            if (argument_count < 3)
                break;
            return (uint32)sub_80079730(argument0, argument1, argument2);
        case 0x800797FC:
            if (argument_count < 1)
                break;
            return (uint32)sub_800797fc(argument0);
        case 0x800798A8:
            return (uint32)sub_800798a8();
        case 0x800798EC:
            return (uint32)sub_800798ec();
        case 0x80079924:
            return (uint32)sub_80079924();
        case 0x80079CB0:
            if (argument_count < 1)
                break;
            return (uint32)sub_80079cb0(argument0);
        case 0x80079CFC:
            if (argument_count < 3)
                break;
            return (uint32)sub_80079cfc(argument0, argument1, argument2);
        case 0x8007AE40:
            if (argument_count < 2)
                break;
            return (uint32)sub_8007ae40(argument0, argument1);
        case 0x8007AFFC:
            if (argument_count < 1)
                break;
            return (uint32)sub_8007affc(argument0);
        case 0x8007B438:
            if (argument_count < 1)
                break;
            return (uint32)sub_8007b438(argument0);
        case 0x8007BC88:
            if (argument_count < 2)
                break;
            return (uint32)sub_8007bc88(argument0, argument1);
        case 0x8007BE3C:
            if (argument_count < 2)
                break;
            return (uint32)sub_8007be3c(argument0, argument1);
        case 0x8007BF30:
            return (uint32)sub_8007bf30();
        case 0x8007C00C:
            if (argument_count < 2)
                break;
            return (uint32)sub_8007c00c(argument0, argument1);
        case 0x8007C9E8:
            return (uint32)sub_8007c9e8();
        case 0x8007CC6C:
            if (argument_count < 2)
                break;
            return (uint32)sub_8007cc6c(argument0, argument1);
        case 0x8007CDF8:
            if (argument_count < 2)
                break;
            return (uint32)sub_8007cdf8(argument0, argument1);
        case 0x8007CEB0:
            return (uint32)sub_8007ceb0();
        case 0x8007CFA4:
            if (argument_count < 2)
                break;
            return (uint32)sub_8007cfa4(argument0, argument1);
        case 0x8007D7E0:
            return (uint32)sub_8007d7e0();
        case 0x8007DA1C:
            return (uint32)sub_8007da1c();
        case 0x8007DA98:
            return (uint32)sub_8007da98();
        case 0x8007DAC8:
            return (uint32)sub_8007dac8();
        case 0x8007DB00:
            if (argument_count < 2)
                break;
            return (uint32)sub_8007db00(argument0, argument1);
        case 0x8007DB30:
            return (uint32)sub_8007db30();
        case 0x8007DBC4:
            if (argument_count < 1)
                break;
            return (uint32)sub_8007dbc4(argument0);
        case 0x8007DC3C:
            if (argument_count < 2)
                break;
            return (uint32)sub_8007dc3c(argument0, argument1);
        case 0x8007DD58:
            if (argument_count < 2)
                break;
            return (uint32)sub_8007dd58(argument0, argument1);
        case 0x8007DDF4:
            if (argument_count < 1)
                break;
            return (uint32)sub_8007ddf4(argument0);
        case 0x8007DE64:
            if (argument_count < 1)
                break;
            return (uint32)sub_8007de64(argument0);
        case 0x8007DF28:
            if (argument_count < 1)
                break;
            return (uint32)sub_8007df28(argument0);
        case 0x8007E1CC:
            if (argument_count < 3)
                break;
            return (uint32)sub_8007e1cc(argument0, argument1, argument2);
        case 0x8007E5BC:
            if (argument_count < 2)
                break;
            return (uint32)sub_8007e5bc(argument0, argument1);
        case 0x8007E6D8:
            if (argument_count < 2)
                break;
            return (uint32)sub_8007e6d8(argument0, argument1);
        case 0x8007E720:
            return (uint32)sub_8007e720();
        case 0x8007E890:
            if (argument_count < 1)
                break;
            return (uint32)sub_8007e890(argument0);
        case 0x8007E900:
            if (argument_count < 1)
                break;
            return (uint32)sub_8007e900(argument0);
        case 0x8007E930:
            if (argument_count < 3)
                break;
            return (uint32)sub_8007e930(argument0, argument1, argument2);
        case 0x8007E968:
            if (argument_count < 1)
                break;
            return (uint32)sub_8007e968(argument0);
        case 0x8007E998:
            if (argument_count < 1)
                break;
            return (uint32)sub_8007e998(argument0);
        case 0x8007EB50:
            if (argument_count < 1)
                break;
            return (uint32)sub_8007eb50(argument0);
        case 0x8007EB80:
            if (argument_count < 1)
                break;
            return (uint32)sub_8007eb80(argument0);
        case 0x8007EC54:
            if (argument_count < 1)
                break;
            return (uint32)sub_8007ec54(argument0);
        case 0x8007EFBC:
            if (argument_count < 2)
                break;
            return (uint32)sub_8007efbc(argument0, argument1);
        case 0x8007F048:
            if (argument_count < 1)
                break;
            return (uint32)sub_8007f048(argument0);
        case 0x8007F0F8:
            return (uint32)sub_8007f0f8();
        case 0x8007F1D4:
            if (argument_count < 1)
                break;
            return (uint32)sub_8007f1d4(argument0);
        case 0x8007F298:
            return (uint32)sub_8007f298();
        case 0x8007F448:
            return (uint32)sub_8007f448();
        case 0x8007F5B4:
            return (uint32)sub_8007f5b4();
        case 0x8007F808:
            return (uint32)sub_8007f808();
        case 0x8007F8E4:
            return (uint32)sub_8007f8e4();
        case 0x8007F9CC:
            if (argument_count < 2)
                break;
            return (uint32)sub_8007f9cc(argument0, argument1);
        case 0x8007F9E8:
            if (argument_count < 2)
                break;
            return (uint32)sub_8007f9e8(argument0, argument1);
        case 0x8007FB24:
            if (argument_count < 2)
                break;
            return (uint32)sub_8007fb24(argument0, argument1);
        case 0x8007FCA0:
            if (argument_count < 2)
                break;
            return (uint32)sub_8007fca0(argument0, argument1);
        case 0x8007FDB4:
            if (argument_count < 3)
                break;
            return (uint32)sub_8007fdb4(argument0, argument1, argument2);
        case 0x800800E0:
            if (argument_count < 2)
                break;
            return (uint32)sub_800800e0(argument0, argument1);
        case 0x8008074C:
            if (argument_count < 3)
                break;
            return (uint32)sub_8008074c(argument0, argument1, argument2);
        case 0x80080A94:
            if (argument_count < 3)
                break;
            return (uint32)sub_80080a94(argument0, argument1, argument2);
        case 0x80080C08:
            if (argument_count < 3)
                break;
            return (uint32)sub_80080c08(argument0, argument1, argument2);
        case 0x80080F10:
            if (argument_count < 2)
                break;
            return (uint32)sub_80080f10(argument0, argument1);
        case 0x80081298:
            if (argument_count < 3)
                break;
            return (uint32)sub_80081298(argument0, argument1, argument2);
        case 0x8008130C:
            if (argument_count < 2)
                break;
            return (uint32)sub_8008130c(argument0, argument1);
        case 0x80081F94:
            if (argument_count < 2)
                break;
            return (uint32)sub_80081f94(argument0, argument1);
        case 0x800821EC:
            if (argument_count < 3)
                break;
            return (uint32)sub_800821ec(argument0, argument1, argument2);
        case 0x8008276C:
            if (argument_count < 2)
                break;
            return (uint32)sub_8008276c(argument0, argument1);
        case 0x800829BC:
            if (argument_count < 3)
                break;
            return (uint32)sub_800829bc(argument0, argument1, argument2);
        case 0x80082D38:
            if (argument_count < 2)
                break;
            return (uint32)sub_80082d38(argument0, argument1);
        case 0x80082E80:
            if (argument_count < 3)
                break;
            return (uint32)sub_80082e80(argument0, argument1, argument2);
        case 0x80082FF0:
            return (uint32)sub_80082ff0();
        case 0x8008350C:
            if (argument_count < 2)
                break;
            return (uint32)sub_8008350c(argument0, argument1);
        case 0x80083CA4:
            if (argument_count < 3)
                break;
            return (uint32)sub_80083ca4(argument0, argument1, argument2);
        case 0x80083D88:
            if (argument_count < 1)
                break;
            return (uint32)sub_80083d88(argument0);
        case 0x80083F40:
            if (argument_count < 3)
                break;
            return (uint32)sub_80083f40(argument0, argument1, argument2);
        case 0x80083FE0:
            if (argument_count < 2)
                break;
            return (uint32)sub_80083fe0(argument0, argument1);
        case 0x80084474:
            if (argument_count < 3)
                break;
            return (uint32)sub_80084474(argument0, argument1, argument2);
        case 0x80084560:
            if (argument_count < 3)
                break;
            return (uint32)sub_80084560(argument0, argument1, argument2);
        case 0x8008488C:
            if (argument_count < 3)
                break;
            return (uint32)sub_8008488c(argument0, argument1, argument2);
        case 0x80084978:
            if (argument_count < 2)
                break;
            return (uint32)sub_80084978(argument0, argument1);
        case 0x800849EC:
            return (uint32)sub_800849ec();
        case 0x80084A6C:
            return (uint32)sub_80084a6c();
        case 0x80084B84:
            if (argument_count < 1)
                break;
            return (uint32)sub_80084b84(argument0);
        case 0x80084E6C:
            if (argument_count < 2)
                break;
            return (uint32)sub_80084e6c(argument0, argument1);
        case 0x80084F6C:
            if (argument_count < 2)
                break;
            return (uint32)sub_80084f6c(argument0, argument1);
        case 0x800850D8:
            return (uint32)sub_800850d8();
        case 0x80085204:
            if (argument_count < 2)
                break;
            return (uint32)sub_80085204(argument0, argument1);
        case 0x800854B8:
            if (argument_count < 1)
                break;
            return (uint32)sub_800854b8(argument0);
        case 0x8008591C:
            if (argument_count < 2)
                break;
            return (uint32)sub_8008591c(argument0, argument1);
        case 0x80085A94:
            if (argument_count < 3)
                break;
            return (uint32)sub_80085a94(argument0, argument1, argument2);
        case 0x80085C8C:
            if (argument_count < 3)
                break;
            return (uint32)sub_80085c8c(argument0, argument1, argument2);
        case 0x80085EAC:
            if (argument_count < 3)
                break;
            return (uint32)sub_80085eac(argument0, argument1, argument2);
        case 0x800860A4:
            if (argument_count < 3)
                break;
            return (uint32)sub_800860a4(argument0, argument1, argument2);
        case 0x80086320:
            if (argument_count < 3)
                break;
            return (uint32)sub_80086320(argument0, argument1, argument2);
        case 0x80086948:
            if (argument_count < 1)
                break;
            return (uint32)sub_80086948(argument0);
        case 0x80086A0C:
            if (argument_count < 3)
                break;
            return (uint32)sub_80086a0c(argument0, argument1, argument2);
        case 0x8008721C:
            if (argument_count < 3)
                break;
            return (uint32)sub_8008721c(argument0, argument1, argument2);
        case 0x800873E0:
            if (argument_count < 3)
                break;
            return (uint32)sub_800873e0(argument0, argument1, argument2);
        case 0x80087520:
            if (argument_count < 3)
                break;
            return (uint32)sub_80087520(argument0, argument1, argument2);
        case 0x80087584:
            if (argument_count < 3)
                break;
            return (uint32)sub_80087584(argument0, argument1, argument2);
        case 0x8008762C:
            if (argument_count < 3)
                break;
            return (uint32)sub_8008762c(argument0, argument1, argument2);
        case 0x8008775C:
            if (argument_count < 3)
                break;
            return (uint32)sub_8008775c(argument0, argument1, argument2);
        case 0x800877FC:
            if (argument_count < 3)
                break;
            return (uint32)sub_800877fc(argument0, argument1, argument2);
        case 0x800878B4:
            if (argument_count < 3)
                break;
            return (uint32)sub_800878b4(argument0, argument1, argument2);
        case 0x80087AD0:
            if (argument_count < 3)
                break;
            return (uint32)sub_80087ad0(argument0, argument1, argument2);
        case 0x80087C18:
            if (argument_count < 3)
                break;
            return (uint32)sub_80087c18(argument0, argument1, argument2);
        case 0x80087C7C:
            if (argument_count < 3)
                break;
            return (uint32)sub_80087c7c(argument0, argument1, argument2);
        case 0x80087EE0:
            if (argument_count < 3)
                break;
            return (uint32)sub_80087ee0(argument0, argument1, argument2);
        case 0x8008812C:
            if (argument_count < 2)
                break;
            return (uint32)sub_8008812c(argument0, argument1);
        case 0x800881BC:
            if (argument_count < 3)
                break;
            return (uint32)sub_800881bc(argument0, argument1, argument2);
        case 0x80088254:
            if (argument_count < 3)
                break;
            return (uint32)sub_80088254(argument0, argument1, argument2);
        case 0x80088454:
            if (argument_count < 2)
                break;
            return (uint32)sub_80088454(argument0, argument1);
        case 0x800885DC:
            if (argument_count < 3)
                break;
            return (uint32)sub_800885dc(argument0, argument1, argument2);
        case 0x80088748:
            if (argument_count < 2)
                break;
            return (uint32)sub_80088748(argument0, argument1);
        case 0x80088968:
            if (argument_count < 3)
                break;
            return (uint32)sub_80088968(argument0, argument1, argument2);
        case 0x80088A84:
            if (argument_count < 2)
                break;
            return (uint32)sub_80088a84(argument0, argument1);
        case 0x80088B48:
            if (argument_count < 3)
                break;
            return (uint32)sub_80088b48(argument0, argument1, argument2);
        case 0x80088D60:
            if (argument_count < 3)
                break;
            return (uint32)sub_80088d60(argument0, argument1, argument2);
        case 0x80088EA8:
            if (argument_count < 3)
                break;
            return (uint32)sub_80088ea8(argument0, argument1, argument2);
        case 0x80088FBC:
            if (argument_count < 3)
                break;
            return (uint32)sub_80088fbc(argument0, argument1, argument2);
        case 0x800890D8:
            if (argument_count < 3)
                break;
            return (uint32)sub_800890d8(argument0, argument1, argument2);
        case 0x800891EC:
            if (argument_count < 2)
                break;
            return (uint32)sub_800891ec(argument0, argument1);
        case 0x800892FC:
            if (argument_count < 3)
                break;
            return (uint32)sub_800892fc(argument0, argument1, argument2);
        case 0x80089378:
            if (argument_count < 2)
                break;
            return (uint32)sub_80089378(argument0, argument1);
        case 0x80089488:
            if (argument_count < 3)
                break;
            return (uint32)sub_80089488(argument0, argument1, argument2);
        case 0x80089504:
            if (argument_count < 3)
                break;
            return (uint32)sub_80089504(argument0, argument1, argument2);
        case 0x8008978C:
            if (argument_count < 3)
                break;
            return (uint32)sub_8008978c(argument0, argument1, argument2);
        case 0x80089880:
            if (argument_count < 3)
                break;
            return (uint32)sub_80089880(argument0, argument1, argument2);
        case 0x80089974:
            if (argument_count < 3)
                break;
            return (uint32)sub_80089974(argument0, argument1, argument2);
        case 0x80089A04:
            if (argument_count < 3)
                break;
            return (uint32)sub_80089a04(argument0, argument1, argument2);
        case 0x80089A94:
            if (argument_count < 3)
                break;
            return (uint32)sub_80089a94(argument0, argument1, argument2);
        case 0x80089B24:
            if (argument_count < 3)
                break;
            return (uint32)sub_80089b24(argument0, argument1, argument2);
        case 0x80089C40:
            if (argument_count < 2)
                break;
            return (uint32)sub_80089c40(argument0, argument1);
        case 0x80089CD8:
            if (argument_count < 2)
                break;
            return (uint32)sub_80089cd8(argument0, argument1);
        case 0x80089E48:
            if (argument_count < 1)
                break;
            return (uint32)sub_80089e48(argument0);
        case 0x80089F44:
            if (argument_count < 2)
                break;
            return (uint32)sub_80089f44(argument0, argument1);
        case 0x8008A61C:
            if (argument_count < 2)
                break;
            return (uint32)sub_8008a61c(argument0, argument1);
        case 0x8008ABF0:
            if (argument_count < 2)
                break;
            return (uint32)sub_8008abf0(argument0, argument1);
        case 0x8008B178:
            if (argument_count < 1)
                break;
            return (uint32)sub_8008b178(argument0);
        case 0x8008B310:
            if (argument_count < 2)
                break;
            return (uint32)sub_8008b310(argument0, argument1);
        case 0x8008B7AC:
            if (argument_count < 2)
                break;
            return (uint32)sub_8008b7ac(argument0, argument1);
        case 0x8008BA8C:
            if (argument_count < 3)
                break;
            return (uint32)sub_8008ba8c(argument0, argument1, argument2);
        case 0x8008C0A0:
            if (argument_count < 1)
                break;
            return (uint32)sub_8008c0a0(argument0);
        case 0x8008C644:
            if (argument_count < 2)
                break;
            return (uint32)sub_8008c644(argument0, argument1);
        case 0x8008C9D4:
            if (argument_count < 1)
                break;
            return (uint32)sub_8008c9d4(argument0);
        case 0x8008CD80:
            if (argument_count < 3)
                break;
            return (uint32)sub_8008cd80(argument0, argument1, argument2);
        case 0x8008D140:
            if (argument_count < 1)
                break;
            return (uint32)sub_8008d140(argument0);
        case 0x8008D244:
            if (argument_count < 1)
                break;
            return (uint32)sub_8008d244(argument0);
        case 0x8008D348:
            if (argument_count < 1)
                break;
            return (uint32)sub_8008d348(argument0);
        case 0x8008D4E8:
            if (argument_count < 1)
                break;
            return (uint32)sub_8008d4e8(argument0);
        case 0x8008D678:
            if (argument_count < 1)
                break;
            return (uint32)sub_8008d678(argument0);
        case 0x8008D9F4:
            return (uint32)sub_8008d9f4();
        case 0x8008DE18:
            if (argument_count < 2)
                break;
            return (uint32)sub_8008de18(argument0, argument1);
        case 0x8008E038:
            return (uint32)sub_8008e038();
        case 0x8008E4F8:
            return (uint32)sub_8008e4f8();
        case 0x8008EB58:
            return (uint32)sub_8008eb58();
        case 0x8008F1B8:
            return (uint32)sub_8008f1b8();
        case 0x8008F580:
            return (uint32)sub_8008f580();
        case 0x8008F950:
            if (argument_count < 3)
                break;
            return (uint32)sub_8008f950(argument0, argument1, argument2);
        case 0x8008FC9C:
            if (argument_count < 3)
                break;
            return (uint32)sub_8008fc9c(argument0, argument1, argument2);
        case 0x8008FFAC:
            if (argument_count < 1)
                break;
            return (uint32)sub_8008ffac(argument0);
        case 0x800900A4:
            if (argument_count < 1)
                break;
            return (uint32)sub_800900a4(argument0);
        case 0x80090110:
            if (argument_count < 1)
                break;
            return (uint32)sub_80090110(argument0);
        case 0x8009044C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8009044c(argument0);
        case 0x800916A8:
            if (argument_count < 3)
                break;
            return (uint32)sub_800916a8(argument0, argument1, argument2);
        case 0x80091A70:
            if (argument_count < 3)
                break;
            return (uint32)sub_80091a70(argument0, argument1, argument2);
        case 0x80092014:
            if (argument_count < 2)
                break;
            return (uint32)sub_80092014(argument0, argument1);
        case 0x8009236C:
            if (argument_count < 3)
                break;
            return (uint32)sub_8009236c(argument0, argument1, argument2);
        case 0x800928F4:
            if (argument_count < 1)
                break;
            return (uint32)sub_800928f4(argument0);
        case 0x80092AB8:
            return (uint32)sub_80092ab8();
        case 0x80092B04:
            return (uint32)sub_80092b04();
        case 0x80092B80:
            if (argument_count < 1)
                break;
            return (uint32)sub_80092b80(argument0);
        case 0x80092D90:
            if (argument_count < 1)
                break;
            return (uint32)sub_80092d90(argument0);
        case 0x80093040:
            if (argument_count < 1)
                break;
            return (uint32)sub_80093040(argument0);
        case 0x8009319C:
            if (argument_count < 1)
                break;
            return (uint32)sub_8009319c(argument0);
        case 0x80093324:
            if (argument_count < 2)
                break;
            return (uint32)sub_80093324(argument0, argument1);
        case 0x80093484:
            if (argument_count < 2)
                break;
            return (uint32)sub_80093484(argument0, argument1);
        case 0x8009351C:
            if (argument_count < 2)
                break;
            return (uint32)sub_8009351c(argument0, argument1);
        case 0x80093944:
            if (argument_count < 1)
                break;
            return (uint32)sub_80093944(argument0);
        case 0x800939D4:
            if (argument_count < 3)
                break;
            return (uint32)sub_800939d4(argument0, argument1, argument2);
        case 0x80093C90:
            if (argument_count < 3)
                break;
            return (uint32)sub_80093c90(argument0, argument1, argument2);
        case 0x80093FC0:
            if (argument_count < 1)
                break;
            return (uint32)sub_80093fc0(argument0);
        case 0x80094718:
            if (argument_count < 2)
                break;
            return (uint32)sub_80094718(argument0, argument1);
        case 0x800949AC:
            if (argument_count < 1)
                break;
            return (uint32)sub_800949ac(argument0);
        case 0x80094AC8:
            if (argument_count < 2)
                break;
            return (uint32)sub_80094ac8(argument0, argument1);
        case 0x80094D10:
            if (argument_count < 2)
                break;
            return (uint32)sub_80094d10(argument0, argument1);
        case 0x80094F50:
            if (argument_count < 2)
                break;
            return (uint32)sub_80094f50(argument0, argument1);
        case 0x80095298:
            if (argument_count < 3)
                break;
            return (uint32)sub_80095298(argument0, argument1, argument2);
        case 0x8009546C:
            if (argument_count < 3)
                break;
            return (uint32)sub_8009546c(argument0, argument1, argument2);
        case 0x800957A4:
            if (argument_count < 2)
                break;
            return (uint32)sub_800957a4(argument0, argument1);
        case 0x80095930:
            if (argument_count < 2)
                break;
            return (uint32)sub_80095930(argument0, argument1);
        case 0x80095FA0:
            if (argument_count < 2)
                break;
            return (uint32)sub_80095fa0(argument0, argument1);
        case 0x80096184:
            if (argument_count < 2)
                break;
            return (uint32)sub_80096184(argument0, argument1);
        case 0x800962B8:
            if (argument_count < 2)
                break;
            return (uint32)sub_800962b8(argument0, argument1);
        case 0x80096518:
            if (argument_count < 3)
                break;
            return (uint32)sub_80096518(argument0, argument1, argument2);
        case 0x800965D4:
            if (argument_count < 1)
                break;
            return (uint32)sub_800965d4(argument0);
        case 0x8009757C:
            if (argument_count < 3)
                break;
            return (uint32)sub_8009757c(argument0, argument1, argument2);
        case 0x800976F8:
            return (uint32)sub_800976f8();
        case 0x80097EB4:
            if (argument_count < 2)
                break;
            return (uint32)sub_80097eb4(argument0, argument1);
        case 0x80098044:
            if (argument_count < 2)
                break;
            return (uint32)sub_80098044(argument0, argument1);
        case 0x8009830C:
            if (argument_count < 3)
                break;
            return (uint32)sub_8009830c(argument0, argument1, argument2);
        case 0x80098414:
            if (argument_count < 2)
                break;
            return (uint32)sub_80098414(argument0, argument1);
        case 0x80098714:
            if (argument_count < 3)
                break;
            return (uint32)sub_80098714(argument0, argument1, argument2);
        case 0x80098850:
            if (argument_count < 1)
                break;
            return (uint32)sub_80098850(argument0);
        case 0x8009B6B8:
            if (argument_count < 2)
                break;
            return (uint32)sub_8009b6b8(argument0, argument1);
        case 0x8009B7CC:
            return (uint32)sub_8009b7cc();
        case 0x8009BC60:
            pad_start();
            return 0;
        case 0x8009BD2C:
            sub_8009bd2c();
            return 0;
        case 0x8009D054:
            if (argument_count < 3)
                break;
            return (uint32)sub_8009d054(argument0, argument1, argument2);
        case 0x8009FD68:
            sub_8009fd68();
            return 0;
        case 0x800A10B8:
            if (argument_count < 2)
                break;
            return (uint32)sound_write(argument0, argument1);
        case 0x800A2D28:
            if (argument_count < 1)
                break;
            return (uint32)cd_set_debug(argument0);
        case 0x800A2D3C:
            return (uint32)cd_get_debug();
        case 0x800A3450:
            if (argument_count < 2)
                break;
            return (uint32)cd_get_sector_async(argument0, argument1);
        case 0x800A34B8:
            return (uint32)cd_poll_irq();
        case 0x800A606C:
            if (argument_count < 1)
                break;
            return (uint32)cd_pos_to_sector(argument0);
        case 0x800A70E8:
            return (uint32)sub_800a70e8();
        case 0x800A7418:
            if (argument_count < 3)
                break;
            return (uint32)sub_800a7418(argument0, argument1, argument2);
        case 0x800A85B8:
            if (argument_count < 2)
                break;
            return (uint32)sub_800a85b8(argument0, argument1);
        case 0x800A85C0:
            if (argument_count < 3)
                break;
            return (uint32)sub_800a85c0(argument0, argument1, argument2);
        case 0x800A8EF0:
            return (uint32)sub_800a8ef0();
        case 0x800A8EF8:
            if (argument_count < 1)
                break;
            return (uint32)sub_800a8ef8(argument0);
        case 0x800AABFC:
            if (argument_count < 1)
                break;
            return (uint32)sub_800aabfc(argument0);
        case 0x800AAC04:
            if (argument_count < 2)
                break;
            return (uint32)sub_800aac04(argument0, argument1);
        case 0x800AD538:
            if (argument_count < 1)
                break;
            return (uint32)bios_init_card(argument0);
        case 0x800AD548:
            return (uint32)bios_start_card();
        case 0x800ADEA8:
            if (argument_count < 2)
                break;
            return (uint32)sub_800adea8(argument0, argument1);
        case 0x800ADEF8:
            if (argument_count < 1)
                break;
            return (uint32)guest_strlen(argument0);
        case 0x800AE904:
            if (argument_count < 3)
                break;
            return (uint32)sub_800ae904(argument0, argument1, argument2);
        case 0x800AF3B8:
            return (uint32)bios_enter_critical();
        case 0x800AF898:
            if (argument_count < 2)
                break;
            return (uint32)sub_800af898(argument0, argument1);
        case 0x800AFBB8:
            return (uint32)sub_800afbb8();
        case 0x800AFBD8:
            if (argument_count < 2)
                break;
            return (uint32)sub_800afbd8(argument0, argument1);
        case 0x800AFD08:
            if (argument_count < 2)
                break;
            return (uint32)sub_800afd08(argument0, argument1);
        case 0x800AFD40:
            if (argument_count < 3)
                break;
            return (uint32)sub_800afd40(argument0, argument1, argument2);
        case 0x800AFDE4:
            if (argument_count < 3)
                break;
            return (uint32)sub_800afde4(argument0, argument1, argument2);
        case 0x800AFE90:
            if (argument_count < 3)
                break;
            return (uint32)sub_800afe90(argument0, argument1, argument2);
        case 0x800AFF74:
            if (argument_count < 3)
                break;
            return (uint32)sub_800aff74(argument0, argument1, argument2);
        case 0x800AFFF0:
            return (uint32)sub_800afff0();
        default:
            break;
    }
    return game_overlay_call(target, argument_count, argument0, argument1, argument2);
}
