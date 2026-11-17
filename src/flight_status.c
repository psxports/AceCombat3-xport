#include "game_functions.h"

static const FLIGHT_AIRCRAFT_PROFILE flight_aircraft_profiles[24] =
{
    {
        {0x00000030, 0x0000002C}, 0x00000010, 0x00000154, 0x000002AC,
        {0x0000000A, 0x0000000A, 0x0000000A}, {0x00000080, 0x00000080, 0x00000080},
        {0xFFFF1D70, 0x00045000, 0x00058000, 0x0000D000, 0x00066000, 0x00061000, 0x00021000},
        0x0000076C, 0x00000A14, 0x00000270, {0xFFFFF9C8, 0x00000638}, {0x00002000, 0x00000400},
        0x00000500, 0x00001400, 0x00000200, 0x000000B4, 0x00000300, 0x000000C7,
        {12, 4}, {8192, 8192},
        0x00000064, {2, 19},
    },
    {
        {0x0000002C, 0x00000024}, 0x0000000C, 0x0000012C, 0x0000020C,
        {0x0000000A, 0x0000000A, 0x0000000A}, {0x00000080, 0x00000080, 0x00000080},
        {0xFFFF1D70, 0x00045000, 0x00058000, 0x0000D000, 0x00066000, 0x00061000, 0x00021000},
        0x000004B0, 0x00000A14, 0x00000270, {0xFFFFF9C8, 0x00000638}, {0x00002000, 0x00000400},
        0x00000500, 0x00001400, 0x00000200, 0x0000008C, 0x00000300, 0x00000048,
        {2, 2}, {8192, 8192},
        0x00000064, {2, 19},
    },
    {
        {0x00000030, 0x0000002C}, 0x00000010, 0x00000154, 0x0000020C,
        {0x0000000A, 0x0000000A, 0x0000000A}, {0x00000080, 0x00000080, 0x00000080},
        {0xFFFF3CB0, 0x00045000, 0x00058000, 0x0000D000, 0x00066000, 0x00061000, 0x00021000},
        0x000004B0, 0x00000A64, 0x0000027F, {0xFFFFF9C8, 0x00000638}, {0x00002000, 0x00000400},
        0x00000500, 0x00001400, 0x00000200, 0x0000008C, 0x00000300, 0x00000050,
        {2, 2}, {8192, 9150},
        0x00000064, {2, 24},
    },
    {
        {0x00000030, 0x0000002C}, 0x00000010, 0x00000154, 0x000002AC,
        {0x0000000A, 0x0000000A, 0x0000000A}, {0x00000080, 0x00000080, 0x00000080},
        {0xFFFF3CB0, 0x0002E000, 0x0004F000, 0x0000D000, 0x00053000, 0x00050000, 0x00021000},
        0x000004B0, 0x00000898, 0x0000020D, {0xFFFFF9C8, 0x00000638}, {0x00002000, 0x00000400},
        0x00000500, 0x00001400, 0x00000200, 0x000000B4, 0x00000300, 0x00000060,
        {4, 4}, {8192, 8192},
        0x00000064, {2, 19},
    },
    {
        {0x00000030, 0x0000002C}, 0x00000010, 0x00000154, 0x000002AC,
        {0x0000000A, 0x0000000A, 0x0000000A}, {0x00000080, 0x00000080, 0x00000080},
        {0xFFFF3CB0, 0x00045000, 0x00058000, 0x0000D000, 0x00066000, 0x00061000, 0x00021000},
        0x000004B0, 0x00000A14, 0x00000270, {0xFFFFF9C8, 0x00000638}, {0x00002000, 0x00000400},
        0x00000500, 0x00001400, 0x00000200, 0x000000B4, 0x00000300, 0x00000050,
        {2, 2}, {9010, 6436},
        0x00000064, {2, 24},
    },
    {
        {0x00000030, 0x0000002C}, 0x00000010, 0x00000154, 0x000002AC,
        {0x0000000A, 0x0000000A, 0x0000000A}, {0x00000080, 0x00000080, 0x00000080},
        {0xFFFF1D70, 0x00045000, 0x00058000, 0x0000D000, 0x00066000, 0x00061000, 0x00021000},
        0x0000076C, 0x00000A14, 0x00000270, {0xFFFFF9C8, 0x00000638}, {0x00002000, 0x00000400},
        0x00000500, 0x00001400, 0x00000200, 0x000000B4, 0x00000314, 0x00000058,
        {2, 2}, {8192, 8192},
        0x00000064, {3, 29},
    },
    {
        {0x00000024, 0x0000001C}, 0x0000000C, 0x000000DC, 0x00000300,
        {0x0000000A, 0x0000000A, 0x0000000A}, {0x00000080, 0x00000080, 0x00000080},
        {0xFFFF5038, 0x00020000, 0x00034000, 0x0000C000, 0x00037000, 0x00035000, 0x00021000},
        0x00000384, 0x00000738, 0x000001C2, {0xFFFFF9C8, 0x00000638}, {0x00002000, 0x00000400},
        0x00000500, 0x00001400, 0x00000200, 0x000000F5, 0x00000300, 0x00000068,
        {6, 6}, {8192, 9150},
        0x00000064, {1, 17},
    },
    {
        {0x00000030, 0x0000002C}, 0x00000010, 0x00000154, 0x00000300,
        {0x0000000A, 0x0000000A, 0x0000000A}, {0x00000080, 0x00000080, 0x00000080},
        {0xFFFF1D70, 0x00052000, 0x0005E000, 0x0000D000, 0x00076000, 0x00064000, 0x00021000},
        0x000004B0, 0x00000A14, 0x00000270, {0xFFFFF9C8, 0x00000638}, {0x00002000, 0x00000400},
        0x00000500, 0x00001400, 0x00000200, 0x000000B4, 0x00000300, 0x00000060,
        {4, 4}, {8192, 8192},
        0x00000064, {1, 17},
    },
    {
        {0x00000030, 0x0000002C}, 0x00000010, 0x00000154, 0x000002AC,
        {0x0000000A, 0x0000000A, 0x0000000A}, {0x00000080, 0x00000080, 0x00000080},
        {0xFFFF1D70, 0x00052000, 0x0005E000, 0x0000D000, 0x00076000, 0x00064000, 0x00021000},
        0x0000076C, 0x00000A14, 0x00000270, {0xFFFFF9C8, 0x00000638}, {0x00002000, 0x00000400},
        0x00000500, 0x00001400, 0x00000200, 0x000000B4, 0x00000314, 0x00000070,
        {6, 6}, {8192, 8192},
        0x00000064, {2, 19},
    },
    {
        {0x0000002C, 0x00000024}, 0x0000000C, 0x0000012C, 0x00000300,
        {0x0000000A, 0x0000000A, 0x0000000A}, {0x00000080, 0x00000080, 0x00000080},
        {0xFFFF5038, 0x0002E000, 0x0004F000, 0x0000D000, 0x00053000, 0x00050000, 0x00021000},
        0x000004B0, 0x00000A14, 0x00000270, {0xFFFFF9C8, 0x00000638}, {0x00002000, 0x00000400},
        0x00000500, 0x00001400, 0x00000200, 0x0000008C, 0x00000300, 0x00000060,
        {2, 2}, {6436, 9150},
        0x00000064, {2, 19},
    },
    {
        {0x00000030, 0x0000002C}, 0x00000010, 0x00000154, 0x00000300,
        {0x0000000A, 0x0000000A, 0x0000000A}, {0x00000080, 0x00000080, 0x00000080},
        {0xFFFF1D70, 0x00045000, 0x00058000, 0x0000D000, 0x00066000, 0x00061000, 0x00021000},
        0x0000076C, 0x00000A64, 0x0000027F, {0xFFFFF9C8, 0x00000638}, {0x00002000, 0x00000400},
        0x00000500, 0x00001400, 0x00000200, 0x00000136, 0x00000314, 0x00000068,
        {4, 4}, {9010, 8192},
        0x00000064, {2, 19},
    },
    {
        {0x00000034, 0x00000030}, 0x00000010, 0x00000168, 0x00000300,
        {0x0000000A, 0x0000000A, 0x0000000A}, {0x00000080, 0x00000080, 0x00000080},
        {0xFFFF1D70, 0x0002E000, 0x0004F000, 0x0000D000, 0x00053000, 0x00050000, 0x00021000},
        0x0000076C, 0x00000A14, 0x00000270, {0xFFFFF9C8, 0x00000638}, {0x00002000, 0x00000400},
        0x00000500, 0x00001400, 0x00000200, 0x00000136, 0x00000300, 0x00000088,
        {6, 6}, {8192, 9150},
        0x00000064, {1, 17},
    },
    {
        {0x00000024, 0x0000001C}, 0x0000000C, 0x000000DC, 0x000002AC,
        {0x0000000A, 0x0000000A, 0x0000000A}, {0x00000080, 0x00000080, 0x00000080},
        {0xFFFF0150, 0x0005C000, 0x000A5000, 0x0000D000, 0x000B5000, 0x000AF000, 0x00021000},
        0x00000898, 0x00000ADC, 0x00000291, {0xFFFFF9C8, 0x00000638}, {0x00002000, 0x00000400},
        0x00000500, 0x00001400, 0x00000200, 0x000000B4, 0x00000300, 0x000000A0,
        {6, 6}, {9010, 4681},
        0x00000064, {1, 16},
    },
    {
        {0x00000034, 0x00000030}, 0x00000010, 0x00000168, 0x00000300,
        {0x0000000A, 0x0000000A, 0x0000000A}, {0x00000080, 0x00000080, 0x00000080},
        {0xFFFF1D70, 0x00052000, 0x0005E000, 0x0000D000, 0x00076000, 0x00064000, 0x00021000},
        0x00000898, 0x00000A14, 0x00000270, {0xFFFFF9C8, 0x00000638}, {0x00002000, 0x00000400},
        0x00000500, 0x00001400, 0x00000200, 0x000000F5, 0x00000328, 0x00000078,
        {4, 4}, {9010, 9150},
        0x00000064, {2, 19},
    },
    {
        {0x00000030, 0x0000002C}, 0x00000010, 0x00000154, 0x00000184,
        {0x0000000C, 0x00000008, 0x0000000C}, {0x00000100, 0x00000100, 0x00000100},
        {0xFFFF0150, 0x0005C000, 0x000A5000, 0x0000D000, 0x000B5000, 0x000AF000, 0x00021000},
        0x00000898, 0x00000ADC, 0x00000291, {0xFFFFF9C8, 0x00000638}, {0x00002000, 0x00000400},
        0x00000500, 0x00001400, 0x00000200, 0x00000078, 0x00000314, 0x00000048,
        {2, 2}, {8192, 9150},
        0x00000064, {3, 29},
    },
    {
        {0x00000034, 0x00000030}, 0x00000010, 0x00000168, 0x000002AC,
        {0x0000000A, 0x0000000A, 0x0000000A}, {0x00000080, 0x00000080, 0x00000080},
        {0xFFFF15A0, 0x00052000, 0x0005E000, 0x0000D000, 0x00076000, 0x00064000, 0x00021000},
        0x00000898, 0x00000A64, 0x0000027F, {0xFFFFF9C8, 0x00000638}, {0x00002000, 0x00000400},
        0x00000500, 0x00001400, 0x00000200, 0x000000B4, 0x0000033C, 0x00000080,
        {4, 4}, {9010, 9150},
        0x00000064, {2, 19},
    },
    {
        {0x0000002C, 0x00000024}, 0x0000000C, 0x0000012C, 0x0000036D,
        {0x0000000A, 0x0000000A, 0x0000000A}, {0x00000080, 0x00000080, 0x00000080},
        {0xFFFF1D70, 0x00052000, 0x0005E000, 0x0000D000, 0x00076000, 0x00064000, 0x00021000},
        0x0000076C, 0x00000898, 0x0000020D, {0xFFFFF9C8, 0x00000638}, {0x00002000, 0x00000400},
        0x00000500, 0x00001400, 0x00000200, 0x000000F5, 0x00000328, 0x00000090,
        {6, 6}, {8192, 10112},
        0x00000064, {1, 17},
    },
    {
        {0x00000024, 0x00000024}, 0x00000018, 0x00000050, 0x0000036D,
        {0x0000000A, 0x0000000A, 0x0000000A}, {0x00000080, 0x00000080, 0x00000080},
        {0xFFFF0BDC, 0x00045000, 0x00058000, 0x0000D000, 0x00066000, 0x00061000, 0x00021000},
        0x0000076C, 0x00000ADC, 0x00000291, {0xFFFFF9C8, 0x00000638}, {0x00002000, 0x00000400},
        0x00000500, 0x00001400, 0x00000200, 0x00000136, 0x00000300, 0x00000064,
        {6, 6}, {8192, 8192},
        0x00000064, {1, 16},
    },
    {
        {0x00000044, 0x00000034}, 0x00000014, 0x00000190, 0x000002AC,
        {0x0000000A, 0x0000000A, 0x0000000A}, {0x00000080, 0x00000080, 0x00000080},
        {0xFFFF15A0, 0x00052000, 0x0005E000, 0x0000D000, 0x00076000, 0x00064000, 0x00021000},
        0x000009C4, 0x00000A64, 0x0000027F, {0xFFFFF9C8, 0x00000638}, {0x00002000, 0x00000400},
        0x00000500, 0x00001400, 0x00000200, 0x000000B4, 0x00000350, 0x00000098,
        {4, 4}, {9010, 10112},
        0x00000064, {1, 17},
    },
    {
        {0x00000034, 0x00000030}, 0x00000010, 0x00000168, 0x00000300,
        {0x0000000A, 0x0000000A, 0x0000000A}, {0x00000080, 0x00000080, 0x00000080},
        {0xFFFF15A0, 0x00052000, 0x0005E000, 0x0000D000, 0x00076000, 0x00064000, 0x00021000},
        0x00000898, 0x00000ADC, 0x00000291, {0xFFFFF9C8, 0x00000638}, {0x00002000, 0x00000400},
        0x00000500, 0x00001400, 0x00000200, 0x000000F5, 0x00000350, 0x000000B8,
        {6, 6}, {9830, 10112},
        0x00000064, {1, 17},
    },
    {
        {0x00000034, 0x00000030}, 0x00000010, 0x00000168, 0x000002AC,
        {0x0000000A, 0x0000000A, 0x0000000A}, {0x00000080, 0x00000080, 0x00000080},
        {0xFFFF1D70, 0x00058000, 0x00078000, 0x0000D000, 0x0008C000, 0x00070000, 0x00021000},
        0x000009C4, 0x00000ADC, 0x00000291, {0xFFFFF9C8, 0x00000638}, {0x00002000, 0x00000400},
        0x00000500, 0x00001400, 0x00000200, 0x000000F5, 0x00000350, 0x000000A8,
        {4, 4}, {9830, 9150},
        0x00000064, {2, 19},
    },
    {
        {0x00000044, 0x00000034}, 0x00000014, 0x00000190, 0x00000300,
        {0x0000000A, 0x0000000A, 0x0000000A}, {0x00000080, 0x00000080, 0x00000080},
        {0xFFFF1D70, 0x0005C000, 0x000A5000, 0x0000D000, 0x000B5000, 0x000AF000, 0x00021000},
        0x000009C4, 0x00000ADC, 0x00000291, {0xFFFFF9C8, 0x00000638}, {0x00002000, 0x00000400},
        0x00000500, 0x00001400, 0x00000200, 0x00000136, 0x00000350, 0x000000B8,
        {6, 6}, {9830, 8192},
        0x00000064, {1, 16},
    },
    {
        {0x00000058, 0x00000040}, 0x00000018, 0x000001E0, 0x000002AC,
        {0x0000000A, 0x0000000A, 0x0000000A}, {0x00000080, 0x00000080, 0x00000080},
        {0xFFFF0BDC, 0x0005C000, 0x000A5000, 0x0000D000, 0x000B5000, 0x000AF000, 0x00021000},
        0x000009C4, 0x00000ADC, 0x00000291, {0xFFFFF9C8, 0x00000638}, {0x00002000, 0x00000400},
        0x00000500, 0x00001400, 0x00000200, 0x00000136, 0x00000350, 0x000000C0,
        {6, 6}, {9830, 8192},
        0x00000064, {1, 16},
    },
    {
        {0x00000044, 0x00000034}, 0x00000014, 0x00000190, 0x00000300,
        {0x0000000A, 0x0000000A, 0x0000000A}, {0x00000080, 0x00000080, 0x00000080},
        {0xFFFF0BDC, 0x00058000, 0x00078000, 0x0000D000, 0x0008C000, 0x00070000, 0x00021000},
        0x000009C4, 0x00000ADC, 0x00000291, {0xFFFFF9C8, 0x00000638}, {0x00002000, 0x00000400},
        0x00000500, 0x00001400, 0x00000200, 0x000000F5, 0x00000350, 0x000000B0,
        {6, 6}, {9830, 9150},
        0x00000064, {1, 16},
    },
};

FLIGHT_STATUS flight_status;

const sint16 flight_weapon_limits[FLIGHT_SECONDARY_WEAPON_KIND_COUNT] = {0, 4, 6, 2, 2, 2, 1, 1, 4, 4, 4, 1, 12};

static FLIGHT_PRIMARY_WEAPON_CALLBACK const primary_weapon_callbacks[FLIGHT_PRIMARY_WEAPON_KIND_COUNT] =
{
    0,
    flight_weapon_fire_primary_1,
    flight_weapon_fire_primary_2,
    flight_weapon_fire_primary_3,
    flight_weapon_fire_primary_4,
    flight_weapon_fire_primary_5,
    flight_weapon_fire_primary_6,
};

static FLIGHT_SECONDARY_WEAPON_CALLBACK const secondary_weapon_callbacks[FLIGHT_SECONDARY_WEAPON_KIND_COUNT] =
{
    0,
    flight_weapon_fire_variant_1,
    flight_weapon_fire_variant_3,
    flight_weapon_fire_variant_4,
    flight_weapon_fire_variant_5,
    flight_weapon_fire_variant_6,
    flight_weapon_fire_variant_7,
    flight_weapon_fire_variant_8,
    flight_weapon_fire_variant_9,
    flight_weapon_fire_variant_10,
    flight_weapon_fire_variant_2,
    flight_weapon_fire_variant_11,
    flight_weapon_fire_variant_1,
};



uint32 flight_status_sample_ground(const uint32 position[3], uint16 normal[3], uint32 *surface)
{
    const uint16 *map;
    uint32 first[3], second[3], hit[3], x, z, gx, gz, value, result;
    FUNCTION_MARKER(0x800711FC, "ACE3.BIN");
    x = (position[0]);
    z = (position[2]);
    gx = (uint32)((sint32)x >> 23) + 128;
    gz = (uint32)((sint32)z >> 23) + 128;
    map = game_runtime.scene_assets.tile_map;
    if ((sint32)gx < 0)
        gx &= 7;
    if ((sint32)gz < 0)
        gz &= 7;
    if ((sint32)gx >= 256)
        gx = (gx & 7) + 248;
    if ((sint32)gz >= 256)
        gz = (gz & 7) + 248;
    if ((map[gx + (gz << 8)] & 0x1FF) == 0)
    {
        if (normal != 0)
        {
            normal[0] = (0);
            normal[1] = (0xF000);
            normal[2] = (0);
        }
        if (surface != 0)
            *surface = (1);
        return 0;
    }
    value = (position[1]);
    if ((sint32)value < -12288000)
        value = 0xFF448000;
    first[0] = x;
    first[1] = value;
    first[2] = (position[2]);
    second[0] = (position[0]);
    second[1] = 0;
    second[2] = (position[2]);
    result = collision_query_tile(first, second, hit);
    if ((sint32)result > 0x7FFFFFFE)
    {
        if (normal != 0)
        {
            normal[0] = (0);
            normal[1] = (0xF000);
            normal[2] = (0);
        }
        if (surface != 0)
            *surface = (0);
        return 0;
    }
    if (normal != 0)
    {
        value = game_runtime.collision_result.normal[0];
        normal[0] = (value);
        value = game_runtime.collision_result.normal[1];
        normal[1] = (value);
        value = game_runtime.collision_result.normal[2];
        normal[2] = (value);
    }
    if (surface != 0)
        *surface = (uint32)(game_runtime.collision_result.flags);
    return hit[1];
}

void flight_weapon_build_pose(NATIVE_POSE *output, const MISSION_ACTOR *actor)
{
    FLIGHT_STATUS *root;
    MATRIX matrix;
    const uint32 *source;
    uint32 count, a, b, c, d, e, x, y, z, value, base, i;
    FUNCTION_MARKER(0x80071770, "ACE3.BIN");
    root = &flight_status;
    count = root->ammunition;
    source = game_runtime.aircraft_mission_metadata.weapon_vectors[count & 1];
    rotation_build(&matrix, actor->pose.angles);
    a = ((uint32)(uint16)matrix.m[0][0] | ((uint32)(uint16)matrix.m[0][1] << 16));
    b = ((uint32)(uint16)matrix.m[0][2] | ((uint32)(uint16)matrix.m[1][0] << 16));
    c = ((uint32)(uint16)matrix.m[1][1] | ((uint32)(uint16)matrix.m[1][2] << 16));
    d = ((uint32)(uint16)matrix.m[2][0] | ((uint32)(uint16)matrix.m[2][1] << 16));
    e = ((uint16)matrix.m[2][2]);
    SetRotMatrix(&matrix);
    value = source[0];
    xport_gte_write_data(0, value);
    value = source[1];
    xport_gte_write_data(1, value);
    xport_gte_execute(0x486012);
    for (i = 0; i < 3; ++i)
    {
        value = xport_gte_read_data(25 + i);
        output->position[i] = value;
    }
    x = output->position[0];
    z = output->position[2];
    output->position[0] = x << 4;
    y = output->position[1];
    output->position[2] = z << 4;
    output->position[1] = y << 4;
    for (i = 0; i < 3; ++i)
    {
        value = output->position[i];
        base = actor->pose.position[i];
        output->position[i] = value + base;
    }
    for (i = 0; i < 3; ++i)
    {
        value = actor->pose.angles[i];
        output->angles[i] = value;
    }
    { (void)(uint32)(value); return; }
}

intptr flight_weapon_fire_variant_1(uint8 *slots)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    FLIGHT_DYNAMICS *state;
    FLIGHT_STATUS *root;
    NATIVE_POSE pose;
    uint32 result, count, value, flags, owner;
    FUNCTION_MARKER(0x80071950, "ACE3.BIN");
    root = &flight_status;
    result = root->ammunition;
    if (result == 0)
        return result;
    count = root->weapon_limit;
    result = flight_weapon_update_slots(slots, count, 150);
    if (result == 0)
        return result;
    state = &flight_dynamics;
    value = state->events;
    root = &flight_status;
    state->events = (uint32)(value | 0x4000);
    value = root->ammunition;
    actor_index_table = 0;
    root->ammunition = (uint32)(value - 1);
    pose_owner_object = mission_actor(actor_index_table);
    actor_motion_object = mission_actor_motion(actor_index_table);
    actor_traits_object = mission_actor_traits(actor_index_table);
    sound_play(14);
    pad_rumble_add_pulse(1);
    flight_weapon_build_pose(&pose, pose_owner_object);
    root = &flight_status;
    flags = actor_traits_object->flags;
    owner = root->target_index;
    return (intptr)effect_object_create_variant_1(flags, owner, 0x10102, &pose, actor_motion_object->velocity);
}

intptr flight_weapon_fire_variant_2(uint8 *slots)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    FLIGHT_DYNAMICS *state;
    FLIGHT_STATUS *context;
    uint32 count, value, flags, owner;
    NATIVE_POSE pose;
    FUNCTION_MARKER(0x80071A00, "ACE3.BIN");
    context = &flight_status;
    count = context->ammunition;
    if (count == 0)
        return count;
    value = context->weapon_limit;
    value = flight_weapon_update_slots(slots, value, 150);
    if (value == 0)
        return value;
    state = &flight_dynamics;
    flags = state->events;
    context = &flight_status;
    state->events = (uint32)(flags | 0x4000);
    count = context->ammunition;
    actor_index_table = 0;
    context->ammunition = (uint32)(count - 1);
    pose_owner_object = mission_actor(actor_index_table);
    actor_motion_object = mission_actor_motion(actor_index_table);
    actor_traits_object = mission_actor_traits(actor_index_table);
    sound_play(26);
    pad_rumble_add_pulse(10);
    flight_weapon_build_pose(&pose, pose_owner_object);
    context = &flight_status;
    flags = actor_traits_object->flags;
    owner = context->target_index;
    return (intptr)effect_object_create_variant_1(flags, owner, 0x07010102, &pose, actor_motion_object->velocity);
}

intptr flight_weapon_fire_variant_3(uint8 *slots)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    FLIGHT_DYNAMICS *state;
    FLIGHT_STATUS *root;
    NATIVE_POSE pose;
    uint32 result, count, value, flags, owner;
    FUNCTION_MARKER(0x80071AB0, "ACE3.BIN");
    root = &flight_status;
    result = root->ammunition;
    if (result == 0)
        return result;
    count = root->weapon_limit;
    result = flight_weapon_update_slots(slots, count, 75);
    if (result == 0)
        return result;
    state = &flight_dynamics;
    value = state->events;
    root = &flight_status;
    state->events = (uint32)(value | 0x4000);
    value = root->ammunition;
    actor_index_table = 0;
    root->ammunition = (uint32)(value - 1);
    pose_owner_object = mission_actor(actor_index_table);
    actor_motion_object = mission_actor_motion(actor_index_table);
    actor_traits_object = mission_actor_traits(actor_index_table);
    sound_play(14);
    pad_rumble_add_pulse(2);
    flight_weapon_build_pose(&pose, pose_owner_object);
    root = &flight_status;
    flags = actor_traits_object->flags;
    owner = root->target_index;
    return (intptr)effect_object_create_variant_2(flags, owner, &pose, actor_motion_object->velocity);
}

intptr flight_weapon_fire_variant_4(uint8 *slots)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    FLIGHT_DYNAMICS *state;
    FLIGHT_STATUS *root;
    NATIVE_POSE pose;
    uint32 result, count, value, flags, owner;
    FUNCTION_MARKER(0x80071B54, "ACE3.BIN");
    root = &flight_status;
    result = root->ammunition;
    if (result == 0)
        return result;
    count = root->weapon_limit;
    result = flight_weapon_update_slots(slots, count, 150);
    if (result == 0)
        return result;
    state = &flight_dynamics;
    value = state->events;
    root = &flight_status;
    state->events = (uint32)(value | 0x4000);
    value = root->ammunition;
    actor_index_table = 0;
    root->ammunition = (uint32)(value - 1);
    pose_owner_object = mission_actor(actor_index_table);
    actor_motion_object = mission_actor_motion(actor_index_table);
    actor_traits_object = mission_actor_traits(actor_index_table);
    sound_play(16);
    pad_rumble_add_pulse(3);
    flight_weapon_build_pose(&pose, pose_owner_object);
    root = &flight_status;
    flags = actor_traits_object->flags;
    owner = root->target_index;
    return (intptr)effect_object_create_variant_3(flags, owner, &pose, actor_motion_object->velocity);
}

intptr flight_weapon_fire_variant_5(uint8 *slots)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    FLIGHT_DYNAMICS *state;
    FLIGHT_STATUS *root;
    NATIVE_POSE pose;
    uint32 result, count, value, flags, owner;
    FUNCTION_MARKER(0x80071BF8, "ACE3.BIN");
    root = &flight_status;
    result = root->ammunition;
    if (result == 0)
        return result;
    count = root->weapon_limit;
    result = flight_weapon_update_slots(slots, count, 250);
    if (result == 0)
        return result;
    state = &flight_dynamics;
    value = state->events;
    root = &flight_status;
    state->events = (uint32)(value | 0x4000);
    value = root->ammunition;
    actor_index_table = 0;
    root->ammunition = (uint32)(value - 1);
    pose_owner_object = mission_actor(actor_index_table);
    actor_motion_object = mission_actor_motion(actor_index_table);
    actor_traits_object = mission_actor_traits(actor_index_table);
    sound_play(14);
    pad_rumble_add_pulse(4);
    flight_weapon_build_pose(&pose, pose_owner_object);
    root = &flight_status;
    flags = actor_traits_object->flags;
    owner = root->target_index;
    return (intptr)effect_object_create_variant_4(flags, owner, &pose, actor_motion_object->velocity);
}

intptr flight_weapon_fire_variant_6(uint8 *slots)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    FLIGHT_DYNAMICS *state;
    FLIGHT_STATUS *root;
    NATIVE_POSE pose;
    uint32 result, count, value, flags, owner;
    FUNCTION_MARKER(0x80071C9C, "ACE3.BIN");
    root = &flight_status;
    result = root->ammunition;
    if (result == 0)
        return result;
    count = root->weapon_limit;
    result = flight_weapon_update_slots(slots, count, 180);
    if (result == 0)
        return result;
    state = &flight_dynamics;
    value = state->events;
    root = &flight_status;
    state->events = (uint32)(value | 0x4000);
    value = root->ammunition;
    actor_index_table = 0;
    root->ammunition = (uint32)(value - 1);
    pose_owner_object = mission_actor(actor_index_table);
    actor_motion_object = mission_actor_motion(actor_index_table);
    actor_traits_object = mission_actor_traits(actor_index_table);
    sound_play(16);
    pad_rumble_add_pulse(5);
    flight_weapon_build_pose(&pose, pose_owner_object);
    root = &flight_status;
    flags = actor_traits_object->flags;
    owner = root->target_index;
    return (intptr)effect_object_create_variant_5(flags, owner, &pose, actor_motion_object->velocity);
}

intptr flight_weapon_fire_variant_7(uint8 *slots)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    FLIGHT_DYNAMICS *state;
    FLIGHT_STATUS *root;
    NATIVE_POSE pose;
    uint32 result, count, value, flags, owner;
    FUNCTION_MARKER(0x80071D40, "ACE3.BIN");
    root = &flight_status;
    result = root->ammunition;
    if (result == 0)
        return result;
    count = root->weapon_limit;
    result = flight_weapon_update_slots(slots, count, 180);
    if (result == 0)
        return result;
    state = &flight_dynamics;
    value = state->events;
    root = &flight_status;
    state->events = (uint32)(value | 0x4000);
    value = root->ammunition;
    actor_index_table = 0;
    root->ammunition = (uint32)(value - 1);
    pose_owner_object = mission_actor(actor_index_table);
    actor_motion_object = mission_actor_motion(actor_index_table);
    actor_traits_object = mission_actor_traits(actor_index_table);
    sound_play(15);
    pad_rumble_add_pulse(6);
    flight_weapon_build_pose(&pose, pose_owner_object);
    root = &flight_status;
    flags = actor_traits_object->flags;
    owner = root->target_index;
    return (intptr)effect_object_create_variant_6(flags, owner, &pose, actor_motion_object->velocity);
}

intptr flight_weapon_fire_variant_8(uint8 *slots)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    FLIGHT_DYNAMICS *state;
    FLIGHT_STATUS *context;
    uint32 count, owner, value, flags;
    FUNCTION_MARKER(0x80071DE4, "ACE3.BIN");
    context = &flight_status;
    count = context->ammunition;
    if (count == 0)
        return 0xFFFFFFFF;
    owner = context->target_index;
    if (owner == 0xFFFFFFFF)
        return 0xFFFFFFFF;
    value = context->weapon_limit;
    value = flight_weapon_update_slots(slots, value, 180);
    if (value == 0)
        return value;
    state = &flight_dynamics;
    flags = state->events;
    context = &flight_status;
    state->events = (uint32)(flags | 0x4000);
    count = context->ammunition;
    actor_index_table = 0;
    context->ammunition = (uint32)(count - 1);
    pose_owner_object = mission_actor(actor_index_table);
    actor_motion_object = mission_actor_motion(actor_index_table);
    actor_traits_object = mission_actor_traits(actor_index_table);
    sound_play(25);
    pad_rumble_add_pulse(7);
    context = &flight_status;
    flags = actor_traits_object->flags;
    owner = context->target_index;
    effect_object_create_variant_10(flags, owner, &pose_owner_object->pose, actor_motion_object->velocity);
    context = &flight_status;
    owner = context->target_index;
    actor_index_table = 0;
    pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(((owner & 63) << 2)) >> 2))));
    actor_motion_object = mission_actor_motion((actor_index_table + ((uint32)((sint32)(((owner & 63) << 2)) >> 2))));
    actor_traits_object = mission_actor_traits((actor_index_table + ((uint32)((sint32)(((owner & 63) << 2)) >> 2))));
    return effect_burst_trigger(pose_owner_object->pose.position, actor_motion_object->velocity);
}

intptr flight_weapon_fire_variant_9(uint8 *slots)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    FLIGHT_DYNAMICS *state;
    FLIGHT_STATUS *context;
    uint32 count, value, flags, owner;
    NATIVE_POSE pose;
    FUNCTION_MARKER(0x80071EBC, "ACE3.BIN");
    context = &flight_status;
    count = context->ammunition;
    if (count == 0)
        return count;
    value = context->weapon_limit;
    value = flight_weapon_update_slots(slots, value, 90);
    if (value == 0)
        return value;
    state = &flight_dynamics;
    flags = state->events;
    context = &flight_status;
    state->events = (uint32)(flags | 0x4000);
    count = context->ammunition;
    actor_index_table = 0;
    context->ammunition = (uint32)(count - 1);
    pose_owner_object = mission_actor(actor_index_table);
    actor_motion_object = mission_actor_motion(actor_index_table);
    actor_traits_object = mission_actor_traits(actor_index_table);
    sound_play(14);
    pad_rumble_add_pulse(8);
    flight_weapon_build_pose(&pose, pose_owner_object);
    context = &flight_status;
    flags = actor_traits_object->flags;
    owner = context->target_index;
    return (intptr)effect_object_create_variant_7(flags, owner, &pose, actor_motion_object->velocity);
}

intptr flight_weapon_fire_variant_10(uint8 *slots)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    FLIGHT_DYNAMICS *state;
    FLIGHT_STATUS *context;
    uint32 count, value, flags, owner;
    NATIVE_POSE pose;
    FUNCTION_MARKER(0x80071F60, "ACE3.BIN");
    context = &flight_status;
    count = context->ammunition;
    if (count == 0)
        return count;
    value = context->weapon_limit;
    value = flight_weapon_update_slots(slots, value, 180);
    if (value == 0)
        return value;
    state = &flight_dynamics;
    flags = state->events;
    context = &flight_status;
    state->events = (uint32)(flags | 0x4000);
    count = context->ammunition;
    actor_index_table = 0;
    context->ammunition = (uint32)(count - 1);
    pose_owner_object = mission_actor(actor_index_table);
    actor_motion_object = mission_actor_motion(actor_index_table);
    actor_traits_object = mission_actor_traits(actor_index_table);
    sound_play(16);
    pad_rumble_add_pulse(9);
    flight_weapon_build_pose(&pose, pose_owner_object);
    context = &flight_status;
    flags = actor_traits_object->flags;
    owner = context->target_index;
    return (intptr)effect_object_create_variant_8(flags, owner, &pose, actor_motion_object->velocity);
}

intptr flight_weapon_fire_variant_11(uint8 *slots)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    FLIGHT_DYNAMICS *state;
    FLIGHT_STATUS *context;
    uint32 count, value, flags, owner;
    FUNCTION_MARKER(0x80072004, "ACE3.BIN");
    context = &flight_status;
    count = context->ammunition;
    if (count == 0)
        return count;
    value = context->weapon_limit;
    value = flight_weapon_update_slots(slots, value, 180);
    if (value == 0)
        return value;
    state = &flight_dynamics;
    flags = state->events;
    context = &flight_status;
    state->events = (uint32)(flags | 0x4000);
    count = context->ammunition;
    actor_index_table = 0;
    context->ammunition = (uint32)(count - 1);
    pose_owner_object = mission_actor(actor_index_table);
    actor_motion_object = mission_actor_motion(actor_index_table);
    actor_traits_object = mission_actor_traits(actor_index_table);
    sound_play(88);
    context = &flight_status;
    flags = actor_traits_object->flags;
    owner = context->target_index;
    return (intptr)effect_object_create_variant_11(flags, owner, &pose_owner_object->pose, actor_motion_object->velocity);
}


uint32 flight_weapon_fire_primary_1(void)
{
    FLIGHT_DYNAMICS *state;
    uint32 result, flags;
    FUNCTION_MARKER(0x800714E0, "ACE3.BIN");
    state = &flight_dynamics;
    result = (sint16)state->primary_cooldown;
    if (result != 0)
        return result;
    flags = state->events;
    state->events = (uint32)(flags | 0x2000);
    effect_spawn_player(0);
    sound_play(17);
    pad_rumble_add_preset(1);
    effect_aircraft_trigger(0x20000);
    state = &flight_dynamics;
    state->primary_cooldown = (uint16)(2);
    return 2;
}

uint32 flight_weapon_fire_primary_2(void)
{
    FLIGHT_DYNAMICS *state;
    uint32 result, value;
    FUNCTION_MARKER(0x80071544, "ACE3.BIN");
    state = &flight_dynamics;
    result = (sint16)state->primary_cooldown;
    if (result != 0)
        return result;
    value = state->events;
    state->events = (uint32)(value | 0x2000);
    effect_spawn_player(1);
    sound_play(18);
    pad_rumble_add_preset(2);
    effect_aircraft_trigger(0x30001);
    state = &flight_dynamics;
    state->primary_cooldown = (uint16)(3);
    return 3;
}

uint32 flight_weapon_fire_primary_3(void)
{
    FLIGHT_DYNAMICS *state;
    uint32 result, value;
    FUNCTION_MARKER(0x800715AC, "ACE3.BIN");
    state = &flight_dynamics;
    result = (sint16)state->primary_cooldown;
    if (result != 0)
        return result;
    value = state->events;
    state->events = (uint32)(value | 0x2000);
    effect_spawn_player(2);
    sound_play(19);
    pad_rumble_add_preset(3);
    effect_aircraft_trigger(0x30002);
    state = &flight_dynamics;
    state->primary_cooldown = (uint16)(6);
    return 6;
}

uint32 flight_weapon_fire_primary_4(void)
{
    FLIGHT_DYNAMICS *state;
    uint32 result, value;
    FUNCTION_MARKER(0x80071614, "ACE3.BIN");
    state = &flight_dynamics;
    result = (sint16)state->primary_cooldown;
    if (result != 0)
        return result;
    value = state->events;
    state->events = (uint32)(value | 0x2000);
    effect_spawn_player(3);
    sound_play(20);
    pad_rumble_add_preset(4);
    effect_aircraft_trigger(0x40003);
    state = &flight_dynamics;
    state->primary_cooldown = (uint16)(2);
    return 2;
}

uint32 flight_weapon_fire_primary_5(void)
{
    FLIGHT_DYNAMICS *state;
    uint32 count, result, value;
    FUNCTION_MARKER(0x8007167C, "ACE3.BIN");
    state = &flight_dynamics;
    count = (sint16)state->primary_cooldown;
    if (count != 0)
    {
        result = ~count;
        if ((sint32)count < 17)
            return result;
        result &= 1;
        if (result == 0)
            return 0;
        return effect_spawn_player(4);
    }
    value = state->events;
    state->events = (uint32)(value | 0x2000);
    effect_aircraft_trigger(0x100005);
    effect_spawn_player(4);
    state = &flight_dynamics;
    state->primary_cooldown = (uint16)(32);
    sound_play(21);
    return pad_rumble_add_preset(5);
}

uint32 flight_weapon_fire_primary_6(void)
{
    FLIGHT_DYNAMICS *state;
    uint32 result, value;
    FUNCTION_MARKER(0x80071708, "ACE3.BIN");
    state = &flight_dynamics;
    result = (sint16)state->primary_cooldown;
    if (result != 0)
        return result;
    value = state->events;
    state->events = (uint32)(value | 0x2000);
    effect_spawn_player(5);
    sound_play(21);
    pad_rumble_add_preset(6);
    effect_aircraft_trigger(0x40004);
    state = &flight_dynamics;
    state->primary_cooldown = (uint16)(3);
    return 3;
}

uint32 flight_weapon_update_slots(uint8 *slots, uint32 count, uint32 value)
{
    uint32 index, available = 0;
    FUNCTION_MARKER(0x800718A0, "ACE3.BIN");
    for (index = 0; (sint32)index < (sint32)count; ++index, ++slots)
    {
        if (*slots == 0)
        {
            *slots = (value);
            value = 0;
            ++available;
        }
    }
    return available;
}

void flight_weapon_tick_cooldowns(void)
{
    uint8 *target;
    uint32 index, value;
    FUNCTION_MARKER(0x800718E0, "ACE3.BIN");
    for (index = 0; index < 12; ++index)
    {
        target = flight_status.weapon_cooldown[0] + index;
        value = *target;
        if (value != 0)
            *target = (value - 1);
    }
    for (index = 0; index < 12; ++index)
    {
        target = flight_status.weapon_cooldown[1] + index;
        value = *target;
        if (value != 0)
            *target = (value - 1);
    }
    return;
}

void flight_weapon_fire_primary(uint32 kind)
{
    FLIGHT_PRIMARY_WEAPON_CALLBACK target;
    FUNCTION_MARKER(0x80072094, "ACE3.BIN");
    target = primary_weapon_callbacks[kind];
    if (target != 0)
        target();
}

intptr flight_weapon_fire_secondary(uint32 kind, uint8 *slots)
{
    FLIGHT_SECONDARY_WEAPON_CALLBACK target;
    FUNCTION_MARKER(0x800720D4, "ACE3.BIN");
    target = secondary_weapon_callbacks[kind];
    return target != 0 ? target(slots) : 0;
}


void flight_restore_player_health(void)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    const FLIGHT_AIRCRAFT_PROFILE *data;
    uint32 flags, kind, value;
    FUNCTION_MARKER(0x8006D078, "ACE3.BIN");
    actor_index_table = 0;
    actor_traits_object = mission_actor_traits(actor_index_table);
    flags = actor_traits_object->flags;
    kind = (sint16)actor_traits_object->kind;
    actor_traits_object->flags = (uint32)(flags | 0x1C000);
    data = flight_model_for_kind(kind);
    value = (uint16)data->max_health;
    actor_traits_object->health = (uint16)(value);
    value = game_runtime.selection.difficulty;
    if ((sint32)value >= 2)
        actor_traits_object->health = (uint16)(30);
    return;
}

const FLIGHT_AIRCRAFT_PROFILE *flight_model_for_kind(sint32 kind)
{
    uint32 index;
    FUNCTION_MARKER(0x8006CD6C, "ACE3.BIN");
    switch (kind)
    {
        case 1:
        case 12:
            index = 3;
            break;
        case 2:
        case 13:
            index = 4;
            break;
        case 3:
            index = 7;
            break;
        case 4:
            index = 8;
            break;
        case 5:
            index = 9;
            break;
        case 6:
            index = 10;
            break;
        case 7:
            index = 11;
            break;
        case 8:
        case 28:
            index = 19;
            break;
        case 9:
            index = 12;
            break;
        case 10:
            index = 1;
            break;
        case 11:
            index = 2;
            break;
        case 14:
        case 18:
            index = 5;
            break;
        case 15:
        case 19:
            index = 6;
            break;
        case 16:
            index = 13;
            break;
        case 17:
        case 29:
            index = 18;
            break;
        case 20:
            index = 15;
            break;
        case 21:
            index = 16;
            break;
        case 22:
        case 30:
            index = 20;
            break;
        case 23:
            index = 14;
            break;
        case 24:
            index = 17;
            break;
        case 25:
            index = 22;
            break;
        case 26:
            index = 21;
            break;
        case 27:
            index = 23;
            break;
        default:
            index = 0;
            break;
    }
    return &flight_aircraft_profiles[index];
}

void flight_initialize_actor(const MISSION_ACTOR_TRAITS *actor_traits_source)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;
    uint32 actor_index_table;

    FLIGHT_DYNAMICS *control;
    FLIGHT_STATUS *other;
    const FLIGHT_AIRCRAFT_PROFILE *model;
    uint32 value, index;
    sint32 kind, limit, amount;
    FUNCTION_MARKER(0x8006CE70, "ACE3.BIN");
    flight_initialize_control_storage();
    actor_index_table = 0;
    kind = (sint16)actor_traits_source->kind;
    pose_owner_object = mission_actor(actor_index_table);
    actor_motion_object = mission_actor_motion(actor_index_table);
    actor_traits_object = mission_actor_traits(actor_index_table);
    model = flight_model_for_kind(kind);
    actor_motion_object->local_velocity[0] = (uint32)(0);
    actor_motion_object->local_velocity[1] = (uint32)(0);
    actor_motion_object->local_velocity[2] = (uint32)(0x20000);
    actor_motion_object->angular_delta[0] = (uint16)(0);
    actor_motion_object->angular_delta[1] = (uint16)(0);
    actor_motion_object->angular_delta[2] = (uint16)(0);
    game_rotate_vector(pose_owner_object->pose.angles, actor_motion_object->local_velocity, actor_motion_object->velocity);
    control = &flight_dynamics;
    actor_traits_object->events = (uint32)(0x80000000);
    control->flags = (uint32)(0);
    control->mode = (uint32)(2);
    control->phase = (uint32)(0);
    control->ticks = (uint32)(0);
    control->events = (uint32)(0);
    control->contact_normal[0] = (uint16)(0);
    control->contact_normal[1] = (uint16)(0);
    control->contact_normal[2] = (uint16)(0);
    control->contact_point.word[0] = (uint32)(0);
    control->contact_point.word[1] = (uint32)(0);
    control->contact_point.word[2] = (uint32)(0);
    control->contact_material = (uint32)(0);
    flight_orientation.response[0] = (uint16)(0);
    flight_orientation.response[1] = (uint16)(0);
    flight_orientation.response[2] = (uint16)(0);
    flight_orientation.angle_delta.value[0] = (uint16)(0);
    flight_orientation.angle_delta.value[1] = (uint16)(0);
    flight_orientation.angle_delta.value[2] = (uint16)(0);
    flight_orientation.velocity_angles.value[0] = (uint16)(0);
    flight_orientation.velocity_angles.value[1] = (uint16)(0);
    flight_orientation.velocity_angles.value[2] = (uint16)(0);
    value = actor_motion_object->local_velocity[2];
    control->control_gain = (uint16)(4096);
    control->rumble_ticks = (uint16)(0);
    control->primary_cooldown = (uint16)(0);
    control->speed = (uint32)(value);
    for (index = 0; index < 12; ++index)
        flight_status.weapon_cooldown[0][index] = 0;
    for (index = 0; index < 12; ++index)
        flight_status.weapon_cooldown[1][index] = 0;
    other = &flight_status;
    other->model = model;
    value = model->max_health;
    other->max_health = (uint32)(value);
    actor_traits_object->health = (uint16)(value);
    if ((sint32)game_runtime.selection.difficulty >= 2)
    {
        other->max_health = (uint32)(30);
        actor_traits_object->health = (uint16)(30);
    }
    other = &flight_status;
    other->target_angle_limit = (uint32)(model->target_angle_limit);
    other->target_distance_limit[0] = (uint32)(model->target_distance_limit[0]);
    amount = model->target_distance_limit[1];
    other->target_index = (uint32)(0xFFFFFFFF);
    other->target_distance_limit[1] = (uint32)(amount);
    other->ammunition = (uint32)(model->ammunition);
    amount = model->weapon_limit[0];
    other->weapon_limit = (uint32)(amount);
    value = game_runtime.selection.weapon_code[1];
    limit = flight_weapon_limits[value];
    if (limit < amount)
        other->weapon_limit = (uint32)(limit);
    other = &flight_status;
    other->active_weapons = (uint32)(0);
    other->events = (uint32)(0);
    other->ground_height = (uint32)(0);
    other->ground_clearance = (uint32)(0x7FFFFFFF);
    other->ground_normal[0] = (uint16)(0);
    other->ground_normal[1] = (uint16)(0);
    other->ground_normal[2] = (uint16)(0);
    other->surface_flags = (uint32)(0);
    { (void)(engine_audio_init()); return; }
}

void flight_status_apply_actor_events(uint32 flags)
{
    MISSION_ACTOR *pose_owner_object;
    MISSION_ACTOR_MOTION *actor_motion_object;

    MISSION_ACTOR_TRAITS *actor_traits_object;

    FLIGHT_DYNAMICS *state;
    FLIGHT_STATUS *context;
    const FLIGHT_AIRCRAFT_PROFILE *model;
    uint32 value, other, events, damage;
    uint8 *slots;
    uint32 first_kind, second_kind, limit, result, x, z, speed;
    FUNCTION_MARKER(0x80072178, "ACE3.BIN");
    pose_owner_object = mission_actor(0);
    actor_motion_object = mission_actor_motion(0);
    actor_traits_object = mission_actor_traits(0);
    model = flight_status.model;
    if ((flags & 1) != 0)
    {
        value = actor_traits_object->events;
        events = 0;
        if ((value & 0x3F) != 0)
        {
            actor_traits_object->events = (uint32)(value & 0xFFFFFFC0);
            value = actor_traits_object->health;
            other = model->impact_damage[0];
            actor_traits_object->health = (uint16)(value - other);
            events = 1;
            value = actor_traits_object->events;
        }
        if ((value & 0x7FC0) != 0)
        {
            actor_traits_object->events = (uint32)(value & 0xFFFF803F);
            value = actor_traits_object->health;
            other = model->impact_damage[1];
            actor_traits_object->health = (uint16)(value - other);
            events |= 2;
        }
        value = (sint16)flight_dynamics.control_gain;
        if ((sint32)value < 8)
            events |= 16;
        else if ((sint32)value < 1536)
            events |= 8;
        flight_status.events = (uint32)(events);
    }
    if ((flags & 4) != 0)
        flight_orientation_apply_object(&flight_orientation, actor_traits_object, actor_motion_object, pose_owner_object);
    state = &flight_dynamics;
    value = state->events;
    if ((value & 0x1000) != 0)
    {
        state->events = (uint32)(value & 0xFFFFEFFF);
        value = actor_traits_object->render_flags | 8;
    }
    else
        value = actor_traits_object->render_flags & 0xF7;
    actor_traits_object->render_flags = (uint8)(value);
    state = &flight_dynamics;
    value = state->events;
    state->events = (uint32)(value & 0xFFFFFFDF);
    flight_dynamics_probe_collision(state, actor_motion_object, pose_owner_object);
    state = &flight_dynamics;
    value = state->events;
    state->events = (uint32)(value & 0xFFFFFFF0);
    if ((flags & 2) != 0)
        flight_dynamics_resolve_collision(state, actor_motion_object, pose_owner_object);
    if ((flags & 4) != 0)
        mission_actor_advance_position(actor_traits_object, actor_motion_object, pose_owner_object);
    if ((flags & 2) != 0)
    {
        context = &flight_status;
        result = flight_status_sample_ground(pose_owner_object->pose.position, context->ground_normal, &context->surface_flags);
        value = result - pose_owner_object->pose.position[1];
        if (((sint32)value >> 12) < 66)
        {
            state = &flight_dynamics;
            value = state->events;
            state->events = (uint32)(value | 8);
        }
        context = &flight_status;
        context->ground_height = (uint32)(result);
        value = result - pose_owner_object->pose.position[1];
        context->ground_clearance = (uint32)(value);
    }
    state = &flight_dynamics;
    if ((state->events & 0x20) != 0)
    {
        actor_traits_object->health = (uint16)(0xFFFF);
        actor_motion_object->velocity[0] = (uint32)(0);
        actor_motion_object->velocity[1] = (uint32)(0);
        actor_motion_object->velocity[2] = (uint32)(0);
        value = state->contact_point.word[0];
        pose_owner_object->pose.position[0] = (uint32)(value);
        value = state->contact_point.word[1];
        pose_owner_object->pose.position[1] = (uint32)(value);
        value = state->contact_point.word[2];
        pose_owner_object->pose.position[2] = (uint32)(value);
    }
    if ((flags & 2) != 0)
    {
        value = flight_dynamics.events;
        if ((value & 15) != 0)
        {
            if ((value & 1) != 0 && (flags & 0x40) != 0)
                actor_traits_object->health = (uint16)(0xFFFF);
            if ((flags & 0x10) != 0)
            {
                x = ((sint16)pose_owner_object->pose.angles[0] * 3840);
                if ((sint32)x < 0)
                    x += 4095;
                z = ((sint16)pose_owner_object->pose.angles[2] * 3840);
                pose_owner_object->pose.angles[0] = (uint16)((sint32)x >> 12);
                if ((sint32)z < 0)
                    z += 4095;
                speed = actor_motion_object->velocity[1];
                pose_owner_object->pose.angles[2] = (uint16)((sint32)z >> 12);
                if ((sint32)speed > 0)
                    actor_motion_object->velocity[1] = (uint32)(0);
                if ((flags & 0x20) != 0)
                {
                    value = game_runtime.selection.difficulty;
                    damage = (sint32)value > 0 ? 5 : 3;
                    if ((sint32)value >= 2)
                        damage = 100;
                    value = actor_traits_object->health;
                    context = &flight_status;
                    actor_traits_object->health = (uint16)(value - damage);
                    value = context->events;
                    context->events = (uint32)(value | 4);
                    if ((game_runtime.gameplay_counter & 7) == 0)
                        sound_play_reuse(39);
                    flight_dynamics_emit_contact(flight_dynamics.contact_point.word, 0, actor_motion_object->velocity, pose_owner_object->pose.position);
                }
            }
        }
    }
    if ((flight_dynamics.flags & 0x7000) == 0)
    {
        if ((flight_status.events & 1) != 0)
        {
            pad_rumble_add_decay_both();
            flight_dynamics.rumble_ticks = (uint16)(1);
        }
        if ((flight_status.events & 2) != 0)
        {
            pad_rumble_add_decay_strong();
            flight_dynamics.rumble_ticks = (uint16)(12);
        }
        if ((flight_status.events & 4) != 0)
        {
            pad_rumble_add_decay_slow();
            flight_dynamics.rumble_ticks = (uint16)(1);
        }
        if ((sint16)flight_dynamics.control_gain < 1024)
            camera_apply_shake(0);
        engine_audio_apply();
    }
    state = &flight_dynamics;
    value = state->rumble_ticks;
    if ((sint16)value != 0)
    {
        state->rumble_ticks = (uint16)(value - 1);
        camera_apply_shake(1);
    }
    if ((sint32)flight_dynamics.speed > 0x8C000)
    {
        camera_apply_shake(2);
        pad_rumble_add_scaled(2048);
    }
    result = mission_target_uses_alternate_weapon();
    first_kind = game_runtime.selection.weapon_code[0];
    if (result == 0)
    {
        second_kind = game_runtime.selection.weapon_code[1];
        state = &flight_dynamics;
        limit = model->weapon_limit[0];
        slots = flight_status.weapon_cooldown[0];
    }
    else
    {
        second_kind = game_runtime.selection.weapon_code[2];
        state = &flight_dynamics;
        limit = model->weapon_limit[1];
        slots = flight_status.weapon_cooldown[1];
    }
    result = flight_primary_weapon_override();
    if ((sint32)result >= 0)
        first_kind = result;
    result = flight_secondary_weapon_override();
    if ((sint32)result >= 0)
        second_kind = result;
    value = flight_weapon_limits[second_kind];
    if ((sint32)value < (sint32)limit)
        limit = value;
    flight_status.weapon_limit = (uint32)(limit);
    flight_weapon_tick_cooldowns();
    if ((flags & 8) != 0)
    {
        state = &flight_dynamics;
        value = state->primary_cooldown;
        if ((sint16)value != 0)
            state->primary_cooldown = (uint16)(value - 1);
        state = &flight_dynamics;
        value = state->events;
        state->events = (uint32)(value & 0xFFFFDFFF);
        if ((game_input.held & 0x800) != 0)
            flight_weapon_fire_primary(first_kind);
        state = &flight_dynamics;
        value = state->events;
        state->events = (uint32)(value & 0xFFFFBFFF);
        if ((game_input.pressed & 0x1000) != 0)
            flight_weapon_fire_secondary(second_kind, slots);
    }
    result = flight_weapon_update_slots(slots, flight_status.weapon_limit, 0);
    context = &flight_status;
    value = context->weapon_limit;
    context->active_weapons = (uint32)(value - result);
    { (void)(uint32)(result); return; }
}


uint32 flight_primary_weapon_override(void)
{
    FUNCTION_MARKER(0x8003A624, "ACE3.BIN");
    return 0xFFFFFFFF;
}

uint32 flight_secondary_weapon_override(void)
{
    uint32 mode, target, flags;
    FUNCTION_MARKER(0x8003A62C, "ACE3.BIN");
    mode = (sint16)game_runtime.selection.mission;
    target = hud_control.locked_target_idx;
    if (mode != 22 || target == 255)
        return 0xFFFFFFFF;
    flags = (*mission_actor_flags(target));
    if ((flags & 0x100) != 0 && (~flags & 0x400) != 0)
        return 11;
    return 0xFFFFFFFF;
}
