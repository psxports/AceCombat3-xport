#include <stdlib.h>

#include "game_functions.h"

SCENE_AUDIO_ROUTING scene_audio_routing;

const SCENE_AUDIO_PRESET scene_audio_presets[SCENE_AUDIO_PRESET_COUNT] =
{
    {{4, 6, 8}, {1, 2}, 16, 1024, {40, 100}, {60, 70}, {0, 0}, {0, 0}},
    {{91, 91, 8}, {88, 88}, 16, 1024, {40, 100}, {60, 70}, {0, 0}, {0, 0}},
    {{92, 93, 8}, {1, 2}, 16, 1024, {40, 100}, {60, 70}, {0, 0}, {0, 0}},
    {{94, 95, 8}, {102, 102}, 16, 1024, {40, 100}, {60, 70}, {0, 0}, {0, 0}},
    {{96, 97, 8}, {89, 89}, 16, 1024, {40, 100}, {60, 70}, {0, 0}, {0, 0}},
};

const uint8 scene_audio_preset_for_kind[SCENE_AUDIO_KIND_COUNT] =
{
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 4,
    0, 4, 4, 2, 2, 4, 4, 0, 4, 4, 4, 2, 4, 3, 0, 0
};

typedef struct
{
    sint16 sound;
    uint16 angles[3];
    uint32 position[3];
    uint32 parameter;
} SCENE_AUDIO_CHANNEL;

typedef struct
{
    uint32 reserved0;
    uint16 angles[3];
    uint16 reserved1;
    uint32 position[3];
    SCENE_AUDIO_CHANNEL channels[4];
} SCENE_AUDIO_FRAME;

typedef struct
{
    sint16 sound;
    uint16 reserved0;
    uint32 reserved1;
    uint32 position[3];
} SCENE_AUDIO_EVENT;

typedef char SCENE_AUDIO_LAYOUT_CHECK[
    sizeof(SCENE_AUDIO_CHANNEL) == 24 &&
    offsetof(SCENE_AUDIO_CHANNEL, angles) == 2 &&
    offsetof(SCENE_AUDIO_CHANNEL, position) == 8 &&
    offsetof(SCENE_AUDIO_CHANNEL, parameter) == 20 &&
    sizeof(SCENE_AUDIO_FRAME) == 120 &&
    offsetof(SCENE_AUDIO_FRAME, position) == 12 &&
    offsetof(SCENE_AUDIO_FRAME, channels) == 24 &&
    sizeof(SCENE_AUDIO_EVENT) == 20 &&
    offsetof(SCENE_AUDIO_EVENT, position) == 8 ? 1 : -1];

void scene_audio_apply_frame(const uint8 *data, uint32 bytes)
{
    uint32 pose[6] = {0}, selected[4], mapping[4], sound_position[3];
    uint16 listener_angles[3];
    const SCENE_AUDIO_FRAME *frame;
    const SCENE_AUDIO_CHANNEL *record;
    const SCENE_AUDIO_EVENT *event;
    uint32 index, count, slot, value;
    SCENE_AUDIO_GROUP *group;
    FUNCTION_MARKER(0x80030528, "ACE3.BIN");
    if (!data || ((intptr)data & 3) || bytes < sizeof(*frame) + sizeof(sint16))
        abort();
    frame = (const SCENE_AUDIO_FRAME *)data;
    pose[0] = frame->position[0] << 12;
    pose[1] = frame->position[1] << 12;
    pose[2] = frame->position[2] << 12;
    value = frame->angles[0];
    listener_angles[0] = value;
    pose[4] = value;
    value = frame->angles[1];
    listener_angles[1] = value;
    pose[4] |= value << 16;
    value = frame->angles[2];
    listener_angles[2] = value;
    pose[5] = value;
    game_measure_distance_gte(pose, scene_audio_routing.listener_pos);
    count = scene_audio_routing.count;
    scene_audio_routing.listener_pos[0] = pose[0];
    scene_audio_routing.listener_pos[1] = pose[1];
    scene_audio_routing.listener_pos[2] = pose[2];
    /* Fix016 defines the fourth position word copied by the original */
    scene_audio_routing.listener_pos[3] = pose[3];
    index = 0;
    record = frame->channels;
    if ((sint32)count > 0)
    {
        do
        {
            if (index >= 4)
                abort();
            selected[index] = record->sound;
            ++record;
            ++index;
        } while ((sint32)index < (sint32)scene_audio_routing.count);
    }
    scene_audio_map_groups(selected, mapping);
    count = scene_audio_routing.count;
    index = 0;
    record = frame->channels;
    if ((sint32)count > 0)
    {
        do
        {
            if (index >= 4)
                abort();
            slot = mapping[index];
            value = record->position[0];
            group = &scene_audio_routing.group[slot];
            group->position[0] = value << 12;
            value = record->position[1];
            group->position[1] = value << 12;
            value = record->position[2];
            group->position[2] = value << 12;
            value = record->parameter;
            ++index;
            scene_audio_set_distance(slot, value);
            count = scene_audio_routing.count;
            ++record;
        } while ((sint32)index < (sint32)count);
    }
    scene_audio_update_groups(pose, listener_angles);
    event = (const SCENE_AUDIO_EVENT *)(data + sizeof(*frame));
    if (event->sound != -1)
    {
        if (bytes < sizeof(*frame) + sizeof(*event))
            abort();
        /* Original MIPS rechecks the same event after playback */
        do
        {
            sound_position[0] = event->position[0] << 12;
            sound_position[1] = event->position[1] << 12;
            sound_position[2] = event->position[2] << 12;
            sound_play_spatial(event->sound, 4, sound_position);
        } while (event->sound != -1);
    }
    return;
}

void scene_audio_init_groups(sint32 count)
{
    uint32 index = 0;
    FUNCTION_MARKER(0x8002F500, "ACE3.BIN");
    if (count > SCENE_AUDIO_SLOT_COUNT)
        abort();
    scene_audio_routing.count = count;
    if (count > 0)
    {
        do
        {
            scene_audio_routing.voices[0][index] = 6 + index * 3;
            scene_audio_routing.voices[1][index] = 7 + index * 3;
            scene_audio_routing.voices[2][index] = 8 + index * 3;
            scene_audio_routing.group[index].flags = 0;
            scene_audio_routing.group[index].object_idx = 0xFFFFFFFF;
            audio_spatial_reset(&scene_audio_routing.spatial[index]);
            scene_audio_routing.aux[index].phase = 0;
            scene_audio_routing.previous_volume[index][2].right = 0;
            scene_audio_routing.previous_volume[index][2].left = 0;
            scene_audio_routing.previous_volume[index][1].right = 0;
            scene_audio_routing.previous_volume[index][1].left = 0;
            scene_audio_routing.previous_volume[index][0].right = 0;
            scene_audio_routing.previous_volume[index][0].left = 0;
            ++index;
        } while ((sint32)index < count);
    }
    while (index < 4)
    {
        scene_audio_routing.voices[0][index] = 255;
        scene_audio_routing.voices[1][index] = 255;
        scene_audio_routing.voices[2][index] = 255;
        ++index;
    }
    return;
}

void scene_audio_stop_groups(void)
{
    uint32 index;
    FUNCTION_MARKER(0x8002F620, "ACE3.BIN");
    scene_audio_routing.count = 0;
    for (index = 0; index < 4; ++index)
    {
        sound_stop_voice(scene_audio_routing.voices[0][index]);
        sound_stop_voice(scene_audio_routing.voices[1][index]);
        sound_stop_voice(scene_audio_routing.voices[2][index]);
        scene_audio_routing.voices[0][index] = 255;
        scene_audio_routing.voices[1][index] = 255;
        scene_audio_routing.voices[2][index] = 255;
        scene_audio_routing.group[index].flags = 0;
        scene_audio_routing.group[index].object_idx = 0xFFFFFFFF;
        audio_spatial_reset(&scene_audio_routing.spatial[index]);
        scene_audio_routing.aux[index].phase = 0;
    }
    return;
}

void scene_audio_init_listener(void)
{
    FUNCTION_MARKER(0x800304F4, "ACE3.BIN");
    scene_audio_init_groups(2);
    scene_audio_routing.listener_pos[2] = 0;
    scene_audio_routing.listener_pos[1] = 0;
    scene_audio_routing.listener_pos[0] = 0;
    return;
}

void scene_audio_slew_volume(uint32 voice, const uint16 *volume, const SpuVolume *previous, uint32 step)
{
    uint32 left, old_left, right, old_right, delta_left, delta_right, minimum;
    FUNCTION_MARKER(0x800300E0, "ACE3.BIN");
    left = (sint16)volume[0];
    old_left = previous->left;
    minimum = 0 - step;
    delta_left = left - old_left;
    if ((sint32)delta_left < (sint32)minimum)
        delta_left = minimum;
    if ((sint32)step < (sint32)delta_left)
        delta_left = step;
    right = (sint16)volume[1];
    old_right = previous->right;
    delta_right = right - old_right;
    left = delta_left + old_left;
    if ((sint32)delta_right < (sint32)minimum)
        delta_right = minimum;
    if ((sint32)step < (sint32)delta_right)
        delta_right = step;
    right = delta_right + old_right;
    { (void)(sound_set_voice_volume(voice & 0xFF, (uint32)(sint32)(sint16)left, (uint32)(sint32)(sint16)right)); return; }
}

void scene_audio_select_preset(uint32 slot, uint32 object_index)
{
    MISSION_ACTOR_TRAITS *actor_traits_object;

    uint32 kind, result;
    FUNCTION_MARKER(0x8002F70C, "ACE3.BIN");
    actor_traits_object = mission_actor_traits(((uint32)((sint32)(object_index * 4) >> 2)));
    kind = actor_traits_object->kind;
    if (kind >= 48)
        return;
    result = scene_audio_preset_for_kind[kind & 0x3F];
    scene_audio_routing.group[slot].preset_idx = result;
    { (void)(uint32)(result); return; }
}

void scene_audio_set_distance(uint32 index, uint32 value)
{
    FUNCTION_MARKER(0x8002F6E4, "ACE3.BIN");
    scene_audio_routing.spatial[index].distance = value;
}

void scene_audio_map_groups(const uint32 *native_selected, uint32 *native_mapping)
{
    uint32 assigned_slots = 0, assigned_inputs = 0, slot, input, value, other, result;
    SCENE_AUDIO_GROUP *group;
    FUNCTION_MARKER(0x8002F770, "ACE3.BIN");
    result = scene_audio_routing.count;
    slot = 0;
    if ((sint32)result > 0)
    {
        do
        {
            group = &scene_audio_routing.group[slot];
            if ((((assigned_slots & 0xFF) >> (slot & 31)) & 1) == 0 && (sint32)scene_audio_routing.count > 0)
            {
                input = 0;
                do
                {
                    if ((((assigned_inputs & 0xFF) >> (input & 31)) & 1) == 0)
                    {
                        value = group->object_idx;
                        other = native_selected[input];
                        if (value == other)
                        {
                            native_mapping[input] = slot;
                            assigned_slots |= 1u << (slot & 31);
                            assigned_inputs |= 1u << (input & 31);
                            break;
                        }
                    }
                    ++input;
                } while ((sint32)input < (sint32)scene_audio_routing.count);
            }
            ++slot;
        } while ((sint32)slot < (sint32)scene_audio_routing.count);
    }
    result = scene_audio_routing.count;
    slot = 0;
    if ((sint32)result <= 0)
        { (void)(uint32)(result); return; }
    do
    {
        group = &scene_audio_routing.group[slot];
        if ((((assigned_slots & 0xFF) >> (slot & 31)) & 1) == 0 && (sint32)scene_audio_routing.count > 0)
        {
            input = 0;
            do
            {
                if ((((assigned_inputs & 0xFF) >> (input & 31)) & 1) == 0)
                {
                    value = native_selected[input];
                    group->object_idx = value;
                    value = native_selected[input];
                    if (value == 0xFFFFFFFF)
                    {
                        value = group->flags;
                        group->flags = value | 2;
                        audio_spatial_reset(&scene_audio_routing.spatial[slot]);
                    }
                    else
                    {
                        value = group->flags;
                        group->flags = value | 1;
                        value = native_selected[input];
                        scene_audio_select_preset(slot, value);
                    }
                    native_mapping[input] = slot;
                    assigned_slots |= 1u << (slot & 31);
                    assigned_inputs |= 1u << (input & 31);
                    break;
                }
                ++input;
            } while ((sint32)input < (sint32)scene_audio_routing.count);
        }
        ++slot;
    } while ((sint32)slot < (sint32)scene_audio_routing.count);
    return;
}

void scene_audio_update_groups(const uint32 *native_position, const uint16 *native_listener_angles)
{
    uint32 voices[3], index = 0;
    SCENE_AUDIO_GROUP *group;
    const uint32 *source;
    const SCENE_AUDIO_PRESET *preset;
    uint32 result, value, flags, i, voice, a, b, c, d, left, right, step, bypass;
    SOUND_VOICE *volume;
    AUDIO_AUX_CONTROL *control;
    AUDIO_SPATIAL *movement;
    AUDIO_MODULATION *modulation0, *modulation1;
    FUNCTION_MARKER(0x8002F9AC, "ACE3.BIN");
    result = scene_audio_routing.count;
    if ((sint32)result <= 0)
        { (void)(uint32)(result); return; }
    do
    {
        group = &scene_audio_routing.group[index];
        preset = &scene_audio_presets[group->preset_idx];
        movement = &scene_audio_routing.spatial[index];
        control = &scene_audio_routing.aux[index];
        modulation0 = &scene_audio_routing.modulation[index][0];
        modulation1 = &scene_audio_routing.modulation[index][1];
        source = group->position;
        voices[0] = scene_audio_routing.voices[0][index];
        voices[1] = scene_audio_routing.voices[1][index];
        voices[2] = scene_audio_routing.voices[2][index];
        if (voices[0] == 255 || voices[1] == 255 || voices[2] == 255)
            goto next_slot;
        flags = group->flags;
        if ((flags & 1) != 0)
        {
            group->flags = flags & 0xFFFFFFFE;
            sound_play_on_voice(preset->sounds[0], voices[0]);
            sound_play_on_voice(preset->sounds[1], voices[1]);
            sound_play_on_voice(preset->sounds[2], voices[2]);
            for (i = 0; i < 3; ++i)
            {
                volume = sound_voice_at(voices[i]);
                a = (sint16)volume->base_note;
                value = sound_offset_note(a, voices[i] << 4);
                volume->base_note = (uint16)value;
            }
            a = preset->aux_sounds[0];
            b = preset->aux_sounds[1];
            c = preset->distance_threshold;
            d = preset->speed_threshold;
            audio_aux_init(control, a, b, c, d);
            audio_modulation_init(modulation0, voices[0]);
            a = preset->volume_period[0];
            b = preset->volume_depth[0];
            audio_modulation_set_volume(a, b, modulation0);
            a = preset->note_period[0];
            b = preset->note_depth[0];
            audio_modulation_set_note(a, b, modulation0);
            audio_modulation_init(modulation1, voices[2]);
            a = preset->volume_period[1];
            b = preset->volume_depth[1];
            audio_modulation_set_volume(a, b, modulation1);
            a = preset->note_period[1];
            b = preset->note_depth[1];
            audio_modulation_set_note(a, b, modulation1);
        }
        else if ((flags & 2) != 0)
        {
            group->flags = flags & 0xFFFFFFFD;
            sound_set_voice_volume(voices[0], 0, 0);
            sound_set_voice_volume(voices[1], 0, 0);
            sound_set_voice_volume(voices[2], 0, 0);
            goto next_slot;
        }
        if (group->object_idx == 0xFFFFFFFF)
        {
            sound_set_voice_volume(voices[0], 0, 0);
            sound_set_voice_volume(voices[1], 0, 0);
            sound_set_voice_volume(voices[2], 0, 0);
            goto next_slot;
        }
        audio_spatial_update(movement, source, native_position, native_listener_angles);
        sound_set_voice_volume(voices[2], 4096, 4096);
        audio_modulation_update(modulation1);
        audio_spatial_attenuate(voices[2], 4, movement);
        audio_spatial_pan(voices[2], movement);
        sound_restore_voice_defaults(voices[0]);
        audio_modulation_update(modulation0);
        audio_spatial_attenuate(voices[0], 2, movement);
        audio_spatial_pan(voices[0], movement);
        audio_spatial_pitch(voices[0], movement);
        audio_spatial_crossfade(voices[0], voices[1], movement);
        bypass = 0;
        step = 512;
        if ((sint32)source[1] < -12582912)
        {
            audio_aux_update(control, movement);
            if (control->phase == 1)
            {
                sound_restore_voice_defaults(control->voice);
                audio_spatial_pan(control->voice, movement);
                audio_spatial_pitch(control->voice, movement);
                if ((game_runtime.mission_control.mode & 0x20) == 0)
                    bypass = 1;
                else if (control->voice != 255)
                {
                    voice = control->voice;
                    volume = sound_voice_at(voice);
                    value = ((sint16)volume->volume[0] / 2);
                    volume->volume[0] = (uint16)(value);
                    voice = control->voice;
                    volume = sound_voice_at(voice);
                    value = ((sint16)volume->volume[1] / 2);
                    volume->volume[1] = (uint16)(value);
                }
            }
        }
        if (bypass == 0 && (game_runtime.mission_control.mode & 0x20) != 0)
        {
            for (i = 0; i < 3; ++i)
            {
                volume = sound_voice_at(voices[i]);
                left = (uint32)(sint16)volume->volume[0] * 30;
                right = (uint32)(sint16)volume->volume[1] * 30;
                volume->volume[0] = (uint16)((sint32)left / 100);
                volume->volume[1] = (uint16)((sint32)right / 100);
            }
            step = 2048;
        }
        for (i = 0; i < 3; ++i)
            scene_audio_slew_volume(voices[i], sound_voice_at(voices[i])->volume, &scene_audio_routing.previous_volume[index][i], step);
        value = ((uint32)sound_voice_at(voices[0])->volume[0] | ((uint32)sound_voice_at(voices[0])->volume[1] << 16));
        scene_audio_routing.previous_volume[index][0].left = (sint16)value;
        scene_audio_routing.previous_volume[index][0].right = (sint16)(value >> 16);
        value = ((uint32)sound_voice_at(voices[1])->volume[0] | ((uint32)sound_voice_at(voices[1])->volume[1] << 16));
        scene_audio_routing.previous_volume[index][1].left = (sint16)value;
        scene_audio_routing.previous_volume[index][1].right = (sint16)(value >> 16);
        value = ((uint32)sound_voice_at(voices[2])->volume[0] | ((uint32)sound_voice_at(voices[2])->volume[1] << 16));
        scene_audio_routing.previous_volume[index][2].left = (sint16)value;
        scene_audio_routing.previous_volume[index][2].right = (sint16)(value >> 16);
    next_slot:
        result = scene_audio_routing.count;
        ++index;
    } while ((sint32)index < (sint32)result);
    return;
}

void scene_audio_update_actors(void)
{
    MISSION_ACTOR *pose_owner_object;
    uint32 actor_index_table;

    uint32 nearest[2] = {0xFFFFFFFF, 0xFFFFFFFF}, mapping[2];
    uint32 distances[2] = {0x8000, 0x8000};
    uint32 index, flags, distance, a, b, c;
    FUNCTION_MARKER(0x8004833C, "ACE3.BIN");
    for (index = 2; index < 32; ++index)
    {
        flags = (*mission_actor_flags(index));
        if ((~flags & 1) != 0 || (flags & 0x400000) != 0)
            continue;
        if (((mission_actor_target(index)->projection_flags) & 0x10000) != 0)
        {
            actor_index_table = 0;
            pose_owner_object = mission_actor((actor_index_table + ((uint32)((sint32)(index * 4) >> 2))));
            distance = game_measure_distance_gte(game_runtime.camera_view.pose.position, pose_owner_object->pose.position);
        }
        else
            distance = 0x8000;
        if ((sint32)distance < (sint32)distances[0])
        {
            distances[1] = distances[0];
            distances[0] = distance;
            nearest[1] = nearest[0];
            nearest[0] = index;
        }
        else if ((sint32)distance < (sint32)distances[1])
        {
            distances[1] = distance;
            nearest[1] = index;
        }
        if (distances[0] == 0x8000)
            nearest[0] = 0xFFFFFFFF;
        if (distances[1] == 0x8000)
            nearest[1] = 0xFFFFFFFF;
    }
    scene_audio_map_groups(nearest, mapping);
    index = scene_audio_routing.group[0].object_idx;
    if (index != 0xFFFFFFFF)
    {
        pose_owner_object = mission_actor(((uint32)((sint32)(index * 4) >> 2)));
        a = pose_owner_object->pose.position[0];
        b = pose_owner_object->pose.position[1];
        c = pose_owner_object->pose.position[2];
        scene_audio_routing.group[0].position[0] = a;
        scene_audio_routing.group[0].position[1] = b;
        scene_audio_routing.group[0].position[2] = c;
    }
    index = scene_audio_routing.group[1].object_idx;
    if (index != 0xFFFFFFFF)
    {
        pose_owner_object = mission_actor(((uint32)((sint32)(index * 4) >> 2)));
        a = pose_owner_object->pose.position[0];
        b = pose_owner_object->pose.position[1];
        c = pose_owner_object->pose.position[2];
        scene_audio_routing.group[1].position[0] = a;
        scene_audio_routing.group[1].position[1] = b;
        scene_audio_routing.group[1].position[2] = c;
    }
    { (void)(scene_audio_update_groups(game_runtime.camera_view.pose.position, game_runtime.camera_view.pose.angles)); return; }
}


void scene_audio_quit_spu(void)
{
    FUNCTION_MARKER(0x8009FD68, "ACE3.BIN");
    SpuQuit();
}


void scene_audio_stop_all(void)
{
    FUNCTION_MARKER(0x80030750, "ACE3.BIN");
    sound_stop_all();
    scene_audio_stop_groups();
    return;
}


void scene_audio_start_transition(void)
{
    FUNCTION_MARKER(0x80056A10, "ACE3.BIN");
    resource_cancel_streams();
    { (void)(uint32)(resource_queue_audio(audio_tracks_transition.records, 0, 0)); return; }
}
