/**************************************************************************/
/*  kosmic_audio.h                                                         */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             KOSMIC ENGINE                              */
/*                        https://kosmicengine.org                        */
/**************************************************************************/
/* Copyright (c) 2024-present Kosmic Engine contributors (see AUTHORS.md).*/
/* Copyright (c) 2014-2024 Godot Engine contributors (see AUTHORS.md).    */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef KOSMIC_AUDIO_H
#define KOSMIC_AUDIO_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>

extern int kosmic_audio_is_available();
extern int kosmic_audio_has_worklet();
extern int kosmic_audio_has_script_processor();
extern int kosmic_audio_init(int *p_mix_rate, int p_latency, void (*_state_cb)(int), void (*_latency_cb)(float));
extern void kosmic_audio_resume();

extern int kosmic_audio_input_start();
extern void kosmic_audio_input_stop();

// Samples
extern int kosmic_audio_sample_stream_is_registered(const char *p_stream_object_id);
extern void kosmic_audio_sample_register_stream(const char *p_stream_object_id, float *p_frames_buf, int p_frames_total, const char *p_loop_mode, int p_loop_begin, int p_loop_end);
extern void kosmic_audio_sample_unregister_stream(const char *p_stream_object_id);
extern void kosmic_audio_sample_start(const char *p_playback_object_id, const char *p_stream_object_id, int p_bus_index, float p_offset, float p_pitch_scale, float *p_volume_ptr);
extern void kosmic_audio_sample_stop(const char *p_playback_object_id);
extern void kosmic_audio_sample_set_pause(const char *p_playback_object_id, bool p_pause);
extern int kosmic_audio_sample_is_active(const char *p_playback_object_id);
extern double kosmic_audio_get_sample_playback_position(const char *p_playback_object_id);
extern void kosmic_audio_sample_update_pitch_scale(const char *p_playback_object_id, float p_pitch_scale);
extern void kosmic_audio_sample_set_volumes_linear(const char *p_playback_object_id, int *p_buses_buf, int p_buses_size, float *p_volumes_buf, int p_volumes_size);
extern void kosmic_audio_sample_set_finished_callback(void (*p_callback)(const char *));

extern void kosmic_audio_sample_bus_set_count(int p_count);
extern void kosmic_audio_sample_bus_remove(int p_index);
extern void kosmic_audio_sample_bus_add(int p_at_pos = -1);
extern void kosmic_audio_sample_bus_move(int p_bus, int p_to_pos);
extern void kosmic_audio_sample_bus_set_send(int p_bus, int p_send_index);
extern void kosmic_audio_sample_bus_set_volume_db(int p_bus, float p_volume_db);
extern void kosmic_audio_sample_bus_set_solo(int p_bus, bool p_enable);
extern void kosmic_audio_sample_bus_set_mute(int p_bus, bool p_enable);

// Worklet
typedef int32_t KosmicAudioState[4];
extern int kosmic_audio_worklet_create(int p_channels);
extern void kosmic_audio_worklet_start(float *p_in_buf, int p_in_size, float *p_out_buf, int p_out_size, KosmicAudioState p_state);
extern void kosmic_audio_worklet_start_no_threads(float *p_out_buf, int p_out_size, void (*p_out_cb)(int p_pos, int p_frames), float *p_in_buf, int p_in_size, void (*p_in_cb)(int p_pos, int p_frames));
extern int kosmic_audio_worklet_state_add(KosmicAudioState p_state, int p_idx, int p_value);
extern int kosmic_audio_worklet_state_get(KosmicAudioState p_state, int p_idx);
extern int kosmic_audio_worklet_state_wait(int32_t *p_state, int p_idx, int32_t p_expected, int p_timeout);

// Script
extern int kosmic_audio_script_create(int *p_buffer_size, int p_channels);
extern void kosmic_audio_script_start(float *p_in_buf, int p_in_size, float *p_out_buf, int p_out_size, void (*p_cb)());

#ifdef __cplusplus
}
#endif

#endif // KOSMIC_AUDIO_H
