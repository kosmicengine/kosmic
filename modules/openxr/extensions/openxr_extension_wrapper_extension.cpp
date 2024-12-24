/**************************************************************************/
/*  openxr_extension_wrapper_extension.cpp                                */
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

#include "openxr_extension_wrapper_extension.h"

#include "../openxr_api.h"

void OpenXRExtensionWrapperExtension::_bind_methods() {
	KSVIRTUAL_BIND(_get_requested_extensions);
	KSVIRTUAL_BIND(_set_system_properties_and_get_next_pointer, "next_pointer");
	KSVIRTUAL_BIND(_set_instance_create_info_and_get_next_pointer, "next_pointer");
	KSVIRTUAL_BIND(_set_session_create_and_get_next_pointer, "next_pointer");
	KSVIRTUAL_BIND(_set_swapchain_create_info_and_get_next_pointer, "next_pointer");
	KSVIRTUAL_BIND(_set_hand_joint_locations_and_get_next_pointer, "hand_index", "next_pointer");
	KSVIRTUAL_BIND(_set_projection_views_and_get_next_pointer, "view_index", "next_pointer");
	KSVIRTUAL_BIND(_get_composition_layer_count);
	KSVIRTUAL_BIND(_get_composition_layer, "index");
	KSVIRTUAL_BIND(_get_composition_layer_order, "index");
	KSVIRTUAL_BIND(_get_suggested_tracker_names);
	KSVIRTUAL_BIND(_on_register_metadata);
	KSVIRTUAL_BIND(_on_before_instance_created);
	KSVIRTUAL_BIND(_on_instance_created, "instance");
	KSVIRTUAL_BIND(_on_instance_destroyed);
	KSVIRTUAL_BIND(_on_session_created, "session");
	KSVIRTUAL_BIND(_on_process);
	KSVIRTUAL_BIND(_on_pre_render);
	KSVIRTUAL_BIND(_on_main_swapchains_created);
	KSVIRTUAL_BIND(_on_pre_draw_viewport, "viewport");
	KSVIRTUAL_BIND(_on_post_draw_viewport, "viewport");
	KSVIRTUAL_BIND(_on_session_destroyed);
	KSVIRTUAL_BIND(_on_state_idle);
	KSVIRTUAL_BIND(_on_state_ready);
	KSVIRTUAL_BIND(_on_state_synchronized);
	KSVIRTUAL_BIND(_on_state_visible);
	KSVIRTUAL_BIND(_on_state_focused);
	KSVIRTUAL_BIND(_on_state_stopping);
	KSVIRTUAL_BIND(_on_state_loss_pending);
	KSVIRTUAL_BIND(_on_state_exiting);
	KSVIRTUAL_BIND(_on_event_polled, "event");
	KSVIRTUAL_BIND(_set_viewport_composition_layer_and_get_next_pointer, "layer", "property_values", "next_pointer");
	KSVIRTUAL_BIND(_get_viewport_composition_layer_extension_properties);
	KSVIRTUAL_BIND(_get_viewport_composition_layer_extension_property_defaults);
	KSVIRTUAL_BIND(_on_viewport_composition_layer_destroyed, "layer");
	KSVIRTUAL_BIND(_set_android_surface_swapchain_create_info_and_get_next_pointer, "property_values", "next_pointer");

	ClassDB::bind_method(D_METHOD("get_openxr_api"), &OpenXRExtensionWrapperExtension::get_openxr_api);
	ClassDB::bind_method(D_METHOD("register_extension_wrapper"), &OpenXRExtensionWrapperExtension::register_extension_wrapper);
}

HashMap<String, bool *> OpenXRExtensionWrapperExtension::get_requested_extensions() {
	Dictionary request_extension;

	if (KSVIRTUAL_CALL(_get_requested_extensions, request_extension)) {
		HashMap<String, bool *> result;
		Array keys = request_extension.keys();
		for (int i = 0; i < keys.size(); i++) {
			String key = keys.get(i);
			KSExtensionPtr<bool> value = VariantCaster<KSExtensionPtr<bool>>::cast(request_extension.get(key, KSExtensionPtr<bool>(nullptr)));
			result.insert(key, value);
		}
		return result;
	}

	return HashMap<String, bool *>();
}

void *OpenXRExtensionWrapperExtension::set_system_properties_and_get_next_pointer(void *p_next_pointer) {
	uint64_t pointer;

	if (KSVIRTUAL_CALL(_set_system_properties_and_get_next_pointer, KSExtensionPtr<void>(p_next_pointer), pointer)) {
		return reinterpret_cast<void *>(pointer);
	}

	return nullptr;
}

void *OpenXRExtensionWrapperExtension::set_instance_create_info_and_get_next_pointer(void *p_next_pointer) {
	uint64_t pointer;

	if (KSVIRTUAL_CALL(_set_instance_create_info_and_get_next_pointer, KSExtensionPtr<void>(p_next_pointer), pointer)) {
		return reinterpret_cast<void *>(pointer);
	}

	return nullptr;
}

void *OpenXRExtensionWrapperExtension::set_session_create_and_get_next_pointer(void *p_next_pointer) {
	uint64_t pointer;

	if (KSVIRTUAL_CALL(_set_session_create_and_get_next_pointer, KSExtensionPtr<void>(p_next_pointer), pointer)) {
		return reinterpret_cast<void *>(pointer);
	}

	return nullptr;
}

void *OpenXRExtensionWrapperExtension::set_swapchain_create_info_and_get_next_pointer(void *p_next_pointer) {
	uint64_t pointer;

	if (KSVIRTUAL_CALL(_set_swapchain_create_info_and_get_next_pointer, KSExtensionPtr<void>(p_next_pointer), pointer)) {
		return reinterpret_cast<void *>(pointer);
	}

	return nullptr;
}

void *OpenXRExtensionWrapperExtension::set_hand_joint_locations_and_get_next_pointer(int p_hand_index, void *p_next_pointer) {
	uint64_t pointer;

	if (KSVIRTUAL_CALL(_set_hand_joint_locations_and_get_next_pointer, p_hand_index, KSExtensionPtr<void>(p_next_pointer), pointer)) {
		return reinterpret_cast<void *>(pointer);
	}

	return nullptr;
}

void *OpenXRExtensionWrapperExtension::set_projection_views_and_get_next_pointer(int p_view_index, void *p_next_pointer) {
	uint64_t pointer = 0;

	if (KSVIRTUAL_CALL(_set_projection_views_and_get_next_pointer, p_view_index, KSExtensionPtr<void>(p_next_pointer), pointer)) {
		return reinterpret_cast<void *>(pointer);
	}

	return nullptr;
}

PackedStringArray OpenXRExtensionWrapperExtension::get_suggested_tracker_names() {
	PackedStringArray ret;

	if (KSVIRTUAL_CALL(_get_suggested_tracker_names, ret)) {
		return ret;
	}

	return PackedStringArray();
}

int OpenXRExtensionWrapperExtension::get_composition_layer_count() {
	int count = 0;
	KSVIRTUAL_CALL(_get_composition_layer_count, count);
	return count;
}

XrCompositionLayerBaseHeader *OpenXRExtensionWrapperExtension::get_composition_layer(int p_index) {
	uint64_t pointer;

	if (KSVIRTUAL_CALL(_get_composition_layer, p_index, pointer)) {
		return reinterpret_cast<XrCompositionLayerBaseHeader *>(pointer);
	}

	return nullptr;
}

int OpenXRExtensionWrapperExtension::get_composition_layer_order(int p_index) {
	int order = 0;
	KSVIRTUAL_CALL(_get_composition_layer_order, p_index, order);
	return order;
}

void OpenXRExtensionWrapperExtension::on_register_metadata() {
	KSVIRTUAL_CALL(_on_register_metadata);
}

void OpenXRExtensionWrapperExtension::on_before_instance_created() {
	KSVIRTUAL_CALL(_on_before_instance_created);
}

void OpenXRExtensionWrapperExtension::on_instance_created(const XrInstance p_instance) {
	uint64_t instance = (uint64_t)p_instance;
	KSVIRTUAL_CALL(_on_instance_created, instance);
}

void OpenXRExtensionWrapperExtension::on_instance_destroyed() {
	KSVIRTUAL_CALL(_on_instance_destroyed);
}

void OpenXRExtensionWrapperExtension::on_session_created(const XrSession p_session) {
	uint64_t session = (uint64_t)p_session;
	KSVIRTUAL_CALL(_on_session_created, session);
}

void OpenXRExtensionWrapperExtension::on_process() {
	KSVIRTUAL_CALL(_on_process);
}

void OpenXRExtensionWrapperExtension::on_pre_render() {
	KSVIRTUAL_CALL(_on_pre_render);
}

void OpenXRExtensionWrapperExtension::on_main_swapchains_created() {
	KSVIRTUAL_CALL(_on_main_swapchains_created);
}

void OpenXRExtensionWrapperExtension::on_session_destroyed() {
	KSVIRTUAL_CALL(_on_session_destroyed);
}

void OpenXRExtensionWrapperExtension::on_pre_draw_viewport(RID p_render_target) {
	KSVIRTUAL_CALL(_on_pre_draw_viewport, p_render_target);
}

void OpenXRExtensionWrapperExtension::on_post_draw_viewport(RID p_render_target) {
	KSVIRTUAL_CALL(_on_post_draw_viewport, p_render_target);
}

void OpenXRExtensionWrapperExtension::on_state_idle() {
	KSVIRTUAL_CALL(_on_state_idle);
}

void OpenXRExtensionWrapperExtension::on_state_ready() {
	KSVIRTUAL_CALL(_on_state_ready);
}

void OpenXRExtensionWrapperExtension::on_state_synchronized() {
	KSVIRTUAL_CALL(_on_state_synchronized);
}

void OpenXRExtensionWrapperExtension::on_state_visible() {
	KSVIRTUAL_CALL(_on_state_visible);
}

void OpenXRExtensionWrapperExtension::on_state_focused() {
	KSVIRTUAL_CALL(_on_state_focused);
}

void OpenXRExtensionWrapperExtension::on_state_stopping() {
	KSVIRTUAL_CALL(_on_state_stopping);
}

void OpenXRExtensionWrapperExtension::on_state_loss_pending() {
	KSVIRTUAL_CALL(_on_state_loss_pending);
}

void OpenXRExtensionWrapperExtension::on_state_exiting() {
	KSVIRTUAL_CALL(_on_state_exiting);
}

bool OpenXRExtensionWrapperExtension::on_event_polled(const XrEventDataBuffer &p_event) {
	bool event_polled;

	if (KSVIRTUAL_CALL(_on_event_polled, KSExtensionConstPtr<void>(&p_event), event_polled)) {
		return event_polled;
	}

	return false;
}

void *OpenXRExtensionWrapperExtension::set_viewport_composition_layer_and_get_next_pointer(const XrCompositionLayerBaseHeader *p_layer, const Dictionary &p_property_values, void *p_next_pointer) {
	uint64_t pointer = 0;

	if (KSVIRTUAL_CALL(_set_viewport_composition_layer_and_get_next_pointer, KSExtensionConstPtr<void>(p_layer), p_property_values, KSExtensionPtr<void>(p_next_pointer), pointer)) {
		return reinterpret_cast<void *>(pointer);
	}

	return p_next_pointer;
}

void OpenXRExtensionWrapperExtension::on_viewport_composition_layer_destroyed(const XrCompositionLayerBaseHeader *p_layer) {
	KSVIRTUAL_CALL(_on_viewport_composition_layer_destroyed, KSExtensionConstPtr<void>(p_layer));
}

void OpenXRExtensionWrapperExtension::get_viewport_composition_layer_extension_properties(List<PropertyInfo> *p_property_list) {
	TypedArray<Dictionary> properties;

	if (KSVIRTUAL_CALL(_get_viewport_composition_layer_extension_properties, properties)) {
		for (int i = 0; i < properties.size(); i++) {
			p_property_list->push_back(PropertyInfo::from_dict(properties[i]));
		}
	}
}

Dictionary OpenXRExtensionWrapperExtension::get_viewport_composition_layer_extension_property_defaults() {
	Dictionary property_defaults;
	KSVIRTUAL_CALL(_get_viewport_composition_layer_extension_property_defaults, property_defaults);
	return property_defaults;
}

void *OpenXRExtensionWrapperExtension::set_android_surface_swapchain_create_info_and_get_next_pointer(const Dictionary &p_property_values, void *p_next_pointer) {
	uint64_t pointer = 0;

	if (KSVIRTUAL_CALL(_set_android_surface_swapchain_create_info_and_get_next_pointer, p_property_values, KSExtensionPtr<void>(p_next_pointer), pointer)) {
		return reinterpret_cast<void *>(pointer);
	}

	return p_next_pointer;
}

Ref<OpenXRAPIExtension> OpenXRExtensionWrapperExtension::get_openxr_api() {
	return openxr_api;
}

void OpenXRExtensionWrapperExtension::register_extension_wrapper() {
	OpenXRAPI::register_extension_wrapper(this);
}

OpenXRExtensionWrapperExtension::OpenXRExtensionWrapperExtension() {
	openxr_api.instantiate();
}

OpenXRExtensionWrapperExtension::~OpenXRExtensionWrapperExtension() {
}
