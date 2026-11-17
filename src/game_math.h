#ifndef GAME_MATH_H
#define GAME_MATH_H

#include "psx.h"

typedef union
{
    struct { sint32 x, y, z; };
    sint32 component[3];
    uint32 word[3];
} GAME_FIXED_VECTOR;

typedef union
{
    struct { sint16 pitch, yaw, roll; };
    uint16 value[3];
} GAME_ANGLES;

typedef struct
{
    uint32 position[3];
    uint16 angles[3];
} NATIVE_POSE;

uint32 game_square_root(uint32 input);
uint32 game_screen_clip(uint32 xy);
sint16 game_wave_at(uint32 byte_offset);
uint32 game_divide_signed(uint32 numerator, uint32 denominator);
void game_compose_rotation(const MATRIX *matrix);
void game_build_scaled_rotation(const GAME_ANGLES *angles, uint32 scale, MATRIX *matrix);
void matrix_halve_xz(MATRIX *matrix);
MATRIX *matrix_build_transpose(const uint16 *angles, MATRIX *matrix, MATRIX *other_matrix);

uint32 game_angles_apply_delta(const uint16 *native_delta, uint16 *native_angles);
void game_vector_angles(const uint32 *native_vector, uint16 *native_output);
void game_rotate_vector(const uint16 *native_angles, const uint32 *native_vector, uint32 *native_out);

uint32 game_normal_angles(const uint16 *native_vector);
uint32 game_pose_target_angles(uint16 *native_output, const uint32 *native_target, const uint32 *native_origin, const uint16 *native_angles);

void game_pose_transform_offset(const NATIVE_POSE *origin, uint32 output[3], const uint32 vector[3]);

uint32 game_vector_axis_angle(const uint32 vector[3]);

void game_pose_relative_direction(const uint32 vector[3], uint32 output[3], const uint32 origin[3], const uint16 angles[3]);

void game_angles_normalize(uint16 *native_angles);

void game_angles_compose_delta(uint16 *native_output, const uint16 *native_delta, const uint16 *native_angles);

uint32 game_vector_atan2(uint32 first, uint32 second);

uint32 game_angle_asin(uint32 value);

void game_build_rotation_via_gte(MATRIX *native_out, const uint16 angles[3]);
void game_build_alternate_rotation_via_gte(MATRIX *native_out, const uint16 *native_angles);
uint32 game_square_root_with_lzcs(uint32 value);
uint32 game_vector_length_gte(uint32 x, uint32 y, uint32 z);

uint32 game_triangle_signed_area_gte(uint32 first, uint32 second, uint32 third);

void game_rotate_vector_current_gte(uint32 vector[3]);

uint32 game_measure_direction_14(const uint32 from[3], const uint32 to[3], uint32 out[3]);

void rotation_build(MATRIX *native_out, const uint16 angles[3]);
void copy_words(uint32 *dst, const uint32 *src, sint32 count);

uint32 game_measure_distance_gte(const uint32 *first, const uint32 *second);

#endif
