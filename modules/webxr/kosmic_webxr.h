/**************************************************************************/
/*  kosmic_webxr.h                                                         */
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

#ifndef KOSMIC_WEBXR_H
#define KOSMIC_WEBXR_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>

enum WebXRInputEvent {
	WEBXR_INPUT_EVENT_SELECTSTART,
	WEBXR_INPUT_EVENT_SELECTEND,
	WEBXR_INPUT_EVENT_SQUEEZESTART,
	WEBXR_INPUT_EVENT_SQUEEZEEND,
};

typedef void (*KosmicWebXRSupportedCallback)(char *p_session_mode, int p_supported);
typedef void (*KosmicWebXRStartedCallback)(char *p_reference_space_type, char *p_enabled_features, char *p_environment_blend_mode);
typedef void (*KosmicWebXREndedCallback)();
typedef void (*KosmicWebXRFailedCallback)(char *p_message);
typedef void (*KosmicWebXRInputEventCallback)(int p_event_type, int p_input_source_id);
typedef void (*KosmicWebXRSimpleEventCallback)(char *p_signal_name);

extern int kosmic_webxr_is_supported();
extern void kosmic_webxr_is_session_supported(const char *p_session_mode, KosmicWebXRSupportedCallback p_callback);

extern void kosmic_webxr_initialize(
		const char *p_session_mode,
		const char *p_required_features,
		const char *p_optional_features,
		const char *p_requested_reference_space_types,
		KosmicWebXRStartedCallback p_on_session_started,
		KosmicWebXREndedCallback p_on_session_ended,
		KosmicWebXRFailedCallback p_on_session_failed,
		KosmicWebXRInputEventCallback p_on_input_event,
		KosmicWebXRSimpleEventCallback p_on_simple_event);
extern void kosmic_webxr_uninitialize();

extern int kosmic_webxr_get_view_count();
extern bool kosmic_webxr_get_render_target_size(int *r_size);
extern bool kosmic_webxr_get_transform_for_view(int p_view, float *r_transform);
extern bool kosmic_webxr_get_projection_for_view(int p_view, float *r_transform);
extern unsigned int kosmic_webxr_get_color_texture();
extern unsigned int kosmic_webxr_get_depth_texture();
extern unsigned int kosmic_webxr_get_velocity_texture();

extern bool kosmic_webxr_update_input_source(
		int p_input_source_id,
		float *r_target_pose,
		int *r_target_ray_mode,
		int *r_touch_index,
		int *r_has_grip_pose,
		float *r_grip_pose,
		int *r_has_standard_mapping,
		int *r_button_count,
		float *r_buttons,
		int *r_axes_count,
		float *r_axes,
		int *r_has_hand_data,
		float *r_hand_joints,
		float *r_hand_radii);

extern char *kosmic_webxr_get_visibility_state();
extern int kosmic_webxr_get_bounds_geometry(float **r_points);

extern float kosmic_webxr_get_frame_rate();
extern void kosmic_webxr_update_target_frame_rate(float p_frame_rate);
extern int kosmic_webxr_get_supported_frame_rates(float **r_frame_rates);

#ifdef __cplusplus
}
#endif

#endif // KOSMIC_WEBXR_H
