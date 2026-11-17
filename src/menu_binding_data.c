#include <stdlib.h>

#include "menu_binding_data.h"

static const MENU_BINDING_LABEL labels_0[15] = {
    {0, {0x009D0059u, 0x009D0069u, 0x00A6006Eu}},
    {0, {0x00BE0059u, 0x00BE0068u, 0x00BB006Cu}},
    {0, {0x00DF0059u, 0x00DF0063u, 0x00FD006Au}},
    {0, {0x01000059u, 0x01000071u, 0x01130075u}},
    {0, {0x01210059u, 0x0121005Bu, 0x0113005Fu}},
    {0, {0x01420059u, 0x01420062u, 0x0128006Au}},
    {0, {0x01630059u, 0x0163007Bu, 0x014D0086u}},
    {0, {0x009200E7u, 0x009200D7u, 0x00A600CDu}},
    {0, {0x00B100E7u, 0x00B100D4u, 0x00BB00CFu}},
    {0, {0x00D000E7u, 0x00D000E1u, 0x00F500D0u}},
    {0, {0x00EF00E7u, 0x00EF00DDu, 0x011300C1u}},
    {0, {0x010E00E7u, 0x010E00E3u, 0x011300DFu}},
    {0, {0x012D00E7u, 0x012D00D5u, 0x013200D0u}},
    {0, {0x014C00E7u, 0x014C00CBu, 0x014D00B5u}},
    {0, {0x016B00E7u, 0x016B00A4u, 0x0111008Eu}},
};

static const MENU_BINDING_LABEL labels_1[10] = {
    {0, {0x00BE0059u, 0x00BE0071u, 0x00C50075u}},
    {0, {0x00DF0059u, 0x00DF0061u, 0x00F9006Cu}},
    {0, {0x01000059u, 0x0100006Fu, 0x010D0076u}},
    {0, {0x01210059u, 0x0121005Eu, 0x010D0062u}},
    {0, {0x01420059u, 0x01420064u, 0x0120006Cu}},
    {0, {0x00BE00E7u, 0x00BE00CDu, 0x00C500C9u}},
    {0, {0x00DF00E7u, 0x00DF00D8u, 0x00EF00D0u}},
    {0, {0x010000E7u, 0x010000C8u, 0x010900C3u}},
    {0, {0x012100E7u, 0x012100E3u, 0x010900DDu}},
    {0, {0x014200E7u, 0x014200DEu, 0x012400D0u}},
};

static const MENU_BINDING_LABEL labels_2[13] = {
    {0, {0x00A70059u, 0x00A7006Bu, 0x00A4006Eu}},
    {0, {0x00C80059u, 0x00C80064u, 0x00B9006Cu}},
    {0, {0x00E90059u, 0x00E90061u, 0x00FD006Bu}},
    {0, {0x010A0059u, 0x010A0070u, 0x01130076u}},
    {0, {0x012B0059u, 0x012B005Cu, 0x01130060u}},
    {0, {0x014C0059u, 0x014C0060u, 0x0128006Bu}},
    {0, {0x009C00E7u, 0x009C00D1u, 0x00A400CDu}},
    {0, {0x00BB00E7u, 0x00BB00D3u, 0x00B900CFu}},
    {0, {0x00DA00E7u, 0x00DA00DDu, 0x00F500D0u}},
    {0, {0x00F900E7u, 0x00F900D8u, 0x011300C1u}},
    {0, {0x011800E7u, 0x011800E3u, 0x011300DFu}},
    {0, {0x013700E7u, 0x013700D5u, 0x013200D0u}},
    {0, {0x015600E7u, 0x015600ACu, 0x01270094u}},
};

static const MENU_BINDING_LABEL labels_3[13] = {
    {4, {0x009F0059u, 0x009F005Fu, 0x00BE0066u}},
    {6, {0x00F60059u, 0x00F6005Du, 0x00D1006Au}},
    {6, {0x01140059u, 0x0114005Du, 0x00FC0062u}},
    {0, {0x015C0059u, 0x015C0063u, 0x0114007Eu}},
    {0, {0x00B900C2u, 0x00B900C6u, 0x00A700C6u}},
    {0, {0x00D800C2u, 0x00D800C6u, 0x00B900C6u}},
    {0, {0x00F900C2u, 0x00F900C6u, 0x00D800C6u}},
    {5, {0x009E00E7u, 0x009E00E3u, 0x00BE00D9u}},
    {7, {0x00F600E7u, 0x00F600E1u, 0x00D100D5u}},
    {7, {0x011400E7u, 0x011400E3u, 0x00FC00DCu}},
    {0, {0x00000000u, 0x00000000u, 0x00000000u}},
    {0, {0x00000000u, 0x00000000u, 0x00000000u}},
    {0, {0x00000000u, 0x00000000u, 0x00000000u}},
};

static const MENU_BINDING_QUAD_LABEL extra_labels[3] = {
    {4, {0x00BD0059u, 0x00CC005Fu, 0x00CC006Fu, 0x00C10074u}},
    {0, {0x009900C2u, 0x009900C6u, 0x00A700C6u, 0x00BA00D0u}},
    {5, {0x00BD00E7u, 0x00CC00E3u, 0x00CC00D2u, 0x00C100CAu}},
};

static const uint8 action_keys[21] = {23, 24, 25, 26, 29, 30, 27, 28, 16, 17, 18, 19, 12, 11, 21, 22, 15, 13, 20, 14, 31};

typedef struct
{
    uint8 count;
    uint8 keys_with_prefix[16];
    const MENU_BINDING_LABEL *labels;
} MENU_BINDING_PROFILE;

static const MENU_BINDING_PROFILE profiles[4] = {
    {15, {0, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14}, labels_0},
    {10, {0, 1, 2, 3, 4, 5, 8, 9, 10, 11, 12, 0, 0, 0, 1, 2}, labels_1},
    {13, {0, 0, 1, 2, 3, 4, 5, 7, 8, 9, 10, 11, 12, 14, 0, 0}, labels_2},
    {13, {0, 1, 8, 0, 7, 14, 2, 3, 4, 5, 11, 12, 10, 9, 0, 0}, labels_3},
};

static const MENU_BINDING_PROFILE *menu_binding_profile(uint32 profile)
{
    if (profile >= 4)
        abort();
    return profiles + profile;
}

uint32 menu_binding_count(uint32 profile)
{
    return menu_binding_profile(profile)->count;
}

uint32 menu_binding_key(uint32 profile, sint32 index)
{
    const MENU_BINDING_PROFILE *data = menu_binding_profile(profile);

    if (index < -1 || index >= 15)
        abort();
    return data->keys_with_prefix[index + 1];
}

const MENU_BINDING_LABEL *menu_binding_label(uint32 profile, uint32 index)
{
    const MENU_BINDING_PROFILE *data = menu_binding_profile(profile);

    if (index >= data->count)
        abort();
    return data->labels + index;
}

const MENU_BINDING_QUAD_LABEL *menu_binding_extra_label(uint32 index)
{
    if (index >= 3)
        abort();
    return extra_labels + index;
}

uint32 menu_binding_action_key(uint32 action)
{
    if (action >= 21)
        abort();
    return action_keys[action];
}

static const MENU_BINDING_IMAGE button_images[24] = {
    {896, 307, 40, 17},
    {896, 324, 44, 17},
    {937, 290, 44, 17},
    {918, 324, 44, 17},
    {953, 341, 28, 17},
    {896, 341, 28, 17},
    {896, 394, 36, 18},
    {919, 375, 48, 18},
    {951, 324, 36, 17},
    {941, 341, 48, 17},
    {943, 358, 32, 17},
    {951, 358, 32, 17},
    {911, 341, 32, 17},
    {903, 341, 20, 17},
    {931, 375, 44, 18},
    {908, 375, 44, 19},
    {918, 358, 44, 17},
    {907, 358, 44, 17},
    {896, 358, 44, 17},
    {929, 341, 48, 17},
    {919, 341, 40, 17},
    {939, 324, 48, 17},
    {942, 375, 44, 18},
    {896, 375, 48, 19},
};

const MENU_BINDING_IMAGE *menu_binding_button_image(uint32 index)
{
    if (index >= 24)
        abort();
    return button_images + index;
}

static const MENU_BINDING_IMAGE axis_images[14] = {
    {896, 307, 40, 17},
    {896, 324, 44, 17},
    {937, 290, 44, 17},
    {918, 324, 44, 17},
    {953, 341, 28, 17},
    {896, 341, 28, 17},
    {896, 394, 36, 18},
    {919, 375, 48, 18},
    {938, 307, 44, 17},
    {949, 307, 44, 17},
    {918, 307, 40, 17},
    {907, 324, 44, 17},
    {919, 341, 40, 17},
    {919, 341, 40, 17},
};

const MENU_BINDING_IMAGE *menu_binding_axis_image(uint32 index)
{
    if (index >= 14)
        abort();
    return axis_images + index;
}
