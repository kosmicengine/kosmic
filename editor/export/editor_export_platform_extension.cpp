/**************************************************************************/
/*  editor_export_platform_extension.cpp                                  */
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

#include "editor_export_platform_extension.h"

void EditorExportPlatformExtension::_bind_methods() {
	ClassDB::bind_method(D_METHOD("set_config_error", "error_text"), &EditorExportPlatformExtension::set_config_error);
	ClassDB::bind_method(D_METHOD("get_config_error"), &EditorExportPlatformExtension::get_config_error);

	ClassDB::bind_method(D_METHOD("set_config_missing_templates", "missing_templates"), &EditorExportPlatformExtension::set_config_missing_templates);
	ClassDB::bind_method(D_METHOD("get_config_missing_templates"), &EditorExportPlatformExtension::get_config_missing_templates);

	KSVIRTUAL_BIND(_get_preset_features, "preset");
	KSVIRTUAL_BIND(_is_executable, "path");
	KSVIRTUAL_BIND(_get_export_options);
	KSVIRTUAL_BIND(_should_update_export_options);
	KSVIRTUAL_BIND(_get_export_option_visibility, "preset", "option");
	KSVIRTUAL_BIND(_get_export_option_warning, "preset", "option");

	KSVIRTUAL_BIND(_get_os_name);
	KSVIRTUAL_BIND(_get_name);
	KSVIRTUAL_BIND(_get_logo);

	KSVIRTUAL_BIND(_poll_export);
	KSVIRTUAL_BIND(_get_options_count);
	KSVIRTUAL_BIND(_get_options_tooltip);

	KSVIRTUAL_BIND(_get_option_icon, "device");
	KSVIRTUAL_BIND(_get_option_label, "device");
	KSVIRTUAL_BIND(_get_option_tooltip, "device");
	KSVIRTUAL_BIND(_get_device_architecture, "device");

	KSVIRTUAL_BIND(_cleanup);

	KSVIRTUAL_BIND(_run, "preset", "device", "debug_flags");
	KSVIRTUAL_BIND(_get_run_icon);

	KSVIRTUAL_BIND(_can_export, "preset", "debug");
	KSVIRTUAL_BIND(_has_valid_export_configuration, "preset", "debug");
	KSVIRTUAL_BIND(_has_valid_project_configuration, "preset");

	KSVIRTUAL_BIND(_get_binary_extensions, "preset");

	KSVIRTUAL_BIND(_export_project, "preset", "debug", "path", "flags");
	KSVIRTUAL_BIND(_export_pack, "preset", "debug", "path", "flags");
	KSVIRTUAL_BIND(_export_zip, "preset", "debug", "path", "flags");
	KSVIRTUAL_BIND(_export_pack_patch, "preset", "debug", "path", "patches", "flags");
	KSVIRTUAL_BIND(_export_zip_patch, "preset", "debug", "path", "patches", "flags");

	KSVIRTUAL_BIND(_get_platform_features);

	KSVIRTUAL_BIND(_get_debug_protocol);
}

void EditorExportPlatformExtension::get_preset_features(const Ref<EditorExportPreset> &p_preset, List<String> *r_features) const {
	Vector<String> ret;
	if (KSVIRTUAL_CALL(_get_preset_features, p_preset, ret) && r_features) {
		for (const String &E : ret) {
			r_features->push_back(E);
		}
	}
}

bool EditorExportPlatformExtension::is_executable(const String &p_path) const {
	bool ret = false;
	KSVIRTUAL_CALL(_is_executable, p_path, ret);
	return ret;
}

void EditorExportPlatformExtension::get_export_options(List<ExportOption> *r_options) const {
	TypedArray<Dictionary> ret;
	if (KSVIRTUAL_CALL(_get_export_options, ret) && r_options) {
		for (const Variant &var : ret) {
			const Dictionary &d = var;
			ERR_CONTINUE(!d.has("name"));
			ERR_CONTINUE(!d.has("type"));

			PropertyInfo pinfo = PropertyInfo::from_dict(d);
			ERR_CONTINUE(pinfo.name.is_empty() && (pinfo.usage & PROPERTY_USAGE_STORAGE));
			ERR_CONTINUE(pinfo.type < 0 || pinfo.type >= Variant::VARIANT_MAX);

			Variant default_value;
			if (d.has("default_value")) {
				default_value = d["default_value"];
			}
			bool update_visibility = false;
			if (d.has("update_visibility")) {
				update_visibility = d["update_visibility"];
			}
			bool required = false;
			if (d.has("required")) {
				required = d["required"];
			}

			r_options->push_back(ExportOption(pinfo, default_value, update_visibility, required));
		}
	}
}

bool EditorExportPlatformExtension::should_update_export_options() {
	bool ret = false;
	KSVIRTUAL_CALL(_should_update_export_options, ret);
	return ret;
}

bool EditorExportPlatformExtension::get_export_option_visibility(const EditorExportPreset *p_preset, const String &p_option) const {
	bool ret = true;
	KSVIRTUAL_CALL(_get_export_option_visibility, Ref<EditorExportPreset>(p_preset), p_option, ret);
	return ret;
}

String EditorExportPlatformExtension::get_export_option_warning(const EditorExportPreset *p_preset, const StringName &p_name) const {
	String ret;
	KSVIRTUAL_CALL(_get_export_option_warning, Ref<EditorExportPreset>(p_preset), p_name, ret);
	return ret;
}

String EditorExportPlatformExtension::get_os_name() const {
	String ret;
	KSVIRTUAL_CALL(_get_os_name, ret);
	return ret;
}

String EditorExportPlatformExtension::get_name() const {
	String ret;
	KSVIRTUAL_CALL(_get_name, ret);
	return ret;
}

Ref<Texture2D> EditorExportPlatformExtension::get_logo() const {
	Ref<Texture2D> ret;
	KSVIRTUAL_CALL(_get_logo, ret);
	return ret;
}

bool EditorExportPlatformExtension::poll_export() {
	bool ret = false;
	KSVIRTUAL_CALL(_poll_export, ret);
	return ret;
}

int EditorExportPlatformExtension::get_options_count() const {
	int ret = 0;
	KSVIRTUAL_CALL(_get_options_count, ret);
	return ret;
}

String EditorExportPlatformExtension::get_options_tooltip() const {
	String ret;
	KSVIRTUAL_CALL(_get_options_tooltip, ret);
	return ret;
}

Ref<ImageTexture> EditorExportPlatformExtension::get_option_icon(int p_index) const {
	Ref<ImageTexture> ret;
	if (KSVIRTUAL_CALL(_get_option_icon, p_index, ret)) {
		return ret;
	}
	return EditorExportPlatform::get_option_icon(p_index);
}

String EditorExportPlatformExtension::get_option_label(int p_device) const {
	String ret;
	KSVIRTUAL_CALL(_get_option_label, p_device, ret);
	return ret;
}

String EditorExportPlatformExtension::get_option_tooltip(int p_device) const {
	String ret;
	KSVIRTUAL_CALL(_get_option_tooltip, p_device, ret);
	return ret;
}

String EditorExportPlatformExtension::get_device_architecture(int p_device) const {
	String ret;
	KSVIRTUAL_CALL(_get_device_architecture, p_device, ret);
	return ret;
}

void EditorExportPlatformExtension::cleanup() {
	KSVIRTUAL_CALL(_cleanup);
}

Error EditorExportPlatformExtension::run(const Ref<EditorExportPreset> &p_preset, int p_device, BitField<EditorExportPlatform::DebugFlags> p_debug_flags) {
	Error ret = OK;
	KSVIRTUAL_CALL(_run, p_preset, p_device, p_debug_flags, ret);
	return ret;
}

Ref<Texture2D> EditorExportPlatformExtension::get_run_icon() const {
	Ref<Texture2D> ret;
	if (KSVIRTUAL_CALL(_get_run_icon, ret)) {
		return ret;
	}
	return EditorExportPlatform::get_run_icon();
}

bool EditorExportPlatformExtension::can_export(const Ref<EditorExportPreset> &p_preset, String &r_error, bool &r_missing_templates, bool p_debug) const {
	bool ret = false;
	config_error = r_error;
	config_missing_templates = r_missing_templates;
	if (KSVIRTUAL_CALL(_can_export, p_preset, p_debug, ret)) {
		r_error = config_error;
		r_missing_templates = config_missing_templates;
		return ret;
	}
	return EditorExportPlatform::can_export(p_preset, r_error, r_missing_templates, p_debug);
}

bool EditorExportPlatformExtension::has_valid_export_configuration(const Ref<EditorExportPreset> &p_preset, String &r_error, bool &r_missing_templates, bool p_debug) const {
	bool ret = false;
	config_error = r_error;
	config_missing_templates = r_missing_templates;
	if (KSVIRTUAL_CALL(_has_valid_export_configuration, p_preset, p_debug, ret)) {
		r_error = config_error;
		r_missing_templates = config_missing_templates;
	}
	return ret;
}

bool EditorExportPlatformExtension::has_valid_project_configuration(const Ref<EditorExportPreset> &p_preset, String &r_error) const {
	bool ret = false;
	config_error = r_error;
	if (KSVIRTUAL_CALL(_has_valid_project_configuration, p_preset, ret)) {
		r_error = config_error;
	}
	return ret;
}

List<String> EditorExportPlatformExtension::get_binary_extensions(const Ref<EditorExportPreset> &p_preset) const {
	List<String> ret_list;
	Vector<String> ret;
	if (KSVIRTUAL_CALL(_get_binary_extensions, p_preset, ret)) {
		for (const String &E : ret) {
			ret_list.push_back(E);
		}
	}
	return ret_list;
}

Error EditorExportPlatformExtension::export_project(const Ref<EditorExportPreset> &p_preset, bool p_debug, const String &p_path, BitField<EditorExportPlatform::DebugFlags> p_flags) {
	ExportNotifier notifier(*this, p_preset, p_debug, p_path, p_flags);

	Error ret = FAILED;
	KSVIRTUAL_CALL(_export_project, p_preset, p_debug, p_path, p_flags, ret);
	return ret;
}

Error EditorExportPlatformExtension::export_pack(const Ref<EditorExportPreset> &p_preset, bool p_debug, const String &p_path, BitField<EditorExportPlatform::DebugFlags> p_flags) {
	ExportNotifier notifier(*this, p_preset, p_debug, p_path, p_flags);

	Error ret = FAILED;
	if (KSVIRTUAL_CALL(_export_pack, p_preset, p_debug, p_path, p_flags, ret)) {
		return ret;
	}
	return save_pack(p_preset, p_debug, p_path);
}

Error EditorExportPlatformExtension::export_zip(const Ref<EditorExportPreset> &p_preset, bool p_debug, const String &p_path, BitField<EditorExportPlatform::DebugFlags> p_flags) {
	ExportNotifier notifier(*this, p_preset, p_debug, p_path, p_flags);

	Error ret = FAILED;
	if (KSVIRTUAL_CALL(_export_zip, p_preset, p_debug, p_path, p_flags, ret)) {
		return ret;
	}
	return save_zip(p_preset, p_debug, p_path);
}

Error EditorExportPlatformExtension::export_pack_patch(const Ref<EditorExportPreset> &p_preset, bool p_debug, const String &p_path, const Vector<String> &p_patches, BitField<EditorExportPlatform::DebugFlags> p_flags) {
	ExportNotifier notifier(*this, p_preset, p_debug, p_path, p_flags);

	Error err = _load_patches(p_patches.is_empty() ? p_preset->get_patches() : p_patches);
	if (err != OK) {
		return err;
	}

	Error ret = FAILED;
	if (KSVIRTUAL_CALL(_export_pack_patch, p_preset, p_debug, p_path, p_patches, p_flags, ret)) {
		_unload_patches();
		return ret;
	}

	err = save_pack_patch(p_preset, p_debug, p_path);
	_unload_patches();
	return err;
}

Error EditorExportPlatformExtension::export_zip_patch(const Ref<EditorExportPreset> &p_preset, bool p_debug, const String &p_path, const Vector<String> &p_patches, BitField<EditorExportPlatform::DebugFlags> p_flags) {
	ExportNotifier notifier(*this, p_preset, p_debug, p_path, p_flags);

	Error err = _load_patches(p_patches.is_empty() ? p_preset->get_patches() : p_patches);
	if (err != OK) {
		return err;
	}

	Error ret = FAILED;
	if (KSVIRTUAL_CALL(_export_zip_patch, p_preset, p_debug, p_path, p_patches, p_flags, ret)) {
		_unload_patches();
		return ret;
	}

	err = save_zip_patch(p_preset, p_debug, p_path);
	_unload_patches();
	return err;
}

void EditorExportPlatformExtension::get_platform_features(List<String> *r_features) const {
	Vector<String> ret;
	if (KSVIRTUAL_CALL(_get_platform_features, ret) && r_features) {
		for (const String &E : ret) {
			r_features->push_back(E);
		}
	}
}

String EditorExportPlatformExtension::get_debug_protocol() const {
	String ret;
	if (KSVIRTUAL_CALL(_get_debug_protocol, ret)) {
		return ret;
	}
	return EditorExportPlatform::get_debug_protocol();
}

EditorExportPlatformExtension::EditorExportPlatformExtension() {
	//NOP
}

EditorExportPlatformExtension::~EditorExportPlatformExtension() {
	//NOP
}
