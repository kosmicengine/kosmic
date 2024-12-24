/**************************************************************************/
/*  gltf_document_extension.cpp                                           */
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

#include "gltf_document_extension.h"

void GLTFDocumentExtension::_bind_methods() {
	// Import process.
	KSVIRTUAL_BIND(_import_preflight, "state", "extensions");
	KSVIRTUAL_BIND(_get_supported_extensions);
	KSVIRTUAL_BIND(_parse_node_extensions, "state", "gltf_node", "extensions");
	KSVIRTUAL_BIND(_parse_image_data, "state", "image_data", "mime_type", "ret_image");
	KSVIRTUAL_BIND(_get_image_file_extension);
	KSVIRTUAL_BIND(_parse_texture_json, "state", "texture_json", "ret_gltf_texture");
	KSVIRTUAL_BIND(_import_object_model_property, "state", "split_json_pointer", "partial_paths");
	KSVIRTUAL_BIND(_import_post_parse, "state");
	KSVIRTUAL_BIND(_import_pre_generate, "state");
	KSVIRTUAL_BIND(_generate_scene_node, "state", "gltf_node", "scene_parent");
	KSVIRTUAL_BIND(_import_node, "state", "gltf_node", "json", "node");
	KSVIRTUAL_BIND(_import_post, "state", "root");
	// Export process.
	KSVIRTUAL_BIND(_export_preflight, "state", "root");
	KSVIRTUAL_BIND(_convert_scene_node, "state", "gltf_node", "scene_node");
	KSVIRTUAL_BIND(_export_post_convert, "state", "root");
	KSVIRTUAL_BIND(_export_preserialize, "state");
	KSVIRTUAL_BIND(_export_object_model_property, "state", "node_path", "kosmic_node", "gltf_node_index", "target_object", "target_depth");
	KSVIRTUAL_BIND(_get_saveable_image_formats);
	KSVIRTUAL_BIND(_serialize_image_to_bytes, "state", "image", "image_dict", "image_format", "lossy_quality");
	KSVIRTUAL_BIND(_save_image_at_path, "state", "image", "file_path", "image_format", "lossy_quality");
	KSVIRTUAL_BIND(_serialize_texture_json, "state", "texture_json", "gltf_texture", "image_format");
	KSVIRTUAL_BIND(_export_node, "state", "gltf_node", "json", "node");
	KSVIRTUAL_BIND(_export_post, "state");
}

// Import process.
Error GLTFDocumentExtension::import_preflight(Ref<GLTFState> p_state, Vector<String> p_extensions) {
	ERR_FAIL_COND_V(p_state.is_null(), ERR_INVALID_PARAMETER);
	Error err = OK;
	KSVIRTUAL_CALL(_import_preflight, p_state, p_extensions, err);
	return err;
}

Vector<String> GLTFDocumentExtension::get_supported_extensions() {
	Vector<String> ret;
	KSVIRTUAL_CALL(_get_supported_extensions, ret);
	return ret;
}

Error GLTFDocumentExtension::parse_node_extensions(Ref<GLTFState> p_state, Ref<GLTFNode> p_gltf_node, Dictionary &p_extensions) {
	ERR_FAIL_COND_V(p_state.is_null(), ERR_INVALID_PARAMETER);
	ERR_FAIL_COND_V(p_gltf_node.is_null(), ERR_INVALID_PARAMETER);
	Error err = OK;
	KSVIRTUAL_CALL(_parse_node_extensions, p_state, p_gltf_node, p_extensions, err);
	return err;
}

Error GLTFDocumentExtension::parse_image_data(Ref<GLTFState> p_state, const PackedByteArray &p_image_data, const String &p_mime_type, Ref<Image> r_image) {
	ERR_FAIL_COND_V(p_state.is_null(), ERR_INVALID_PARAMETER);
	ERR_FAIL_COND_V(r_image.is_null(), ERR_INVALID_PARAMETER);
	Error err = OK;
	KSVIRTUAL_CALL(_parse_image_data, p_state, p_image_data, p_mime_type, r_image, err);
	return err;
}

String GLTFDocumentExtension::get_image_file_extension() {
	String ret;
	KSVIRTUAL_CALL(_get_image_file_extension, ret);
	return ret;
}

Error GLTFDocumentExtension::parse_texture_json(Ref<GLTFState> p_state, const Dictionary &p_texture_json, Ref<GLTFTexture> r_gltf_texture) {
	ERR_FAIL_COND_V(p_state.is_null(), ERR_INVALID_PARAMETER);
	ERR_FAIL_COND_V(r_gltf_texture.is_null(), ERR_INVALID_PARAMETER);
	Error err = OK;
	KSVIRTUAL_CALL(_parse_texture_json, p_state, p_texture_json, r_gltf_texture, err);
	return err;
}

Ref<GLTFObjectModelProperty> GLTFDocumentExtension::import_object_model_property(Ref<GLTFState> p_state, const PackedStringArray &p_split_json_pointer, const TypedArray<NodePath> &p_partial_paths) {
	Ref<GLTFObjectModelProperty> ret;
	ERR_FAIL_COND_V(p_state.is_null(), ret);
	KSVIRTUAL_CALL(_import_object_model_property, p_state, p_split_json_pointer, p_partial_paths, ret);
	return ret;
}

Error GLTFDocumentExtension::import_post_parse(Ref<GLTFState> p_state) {
	ERR_FAIL_COND_V(p_state.is_null(), ERR_INVALID_PARAMETER);
	Error err = OK;
	KSVIRTUAL_CALL(_import_post_parse, p_state, err);
	return err;
}

Error GLTFDocumentExtension::import_pre_generate(Ref<GLTFState> p_state) {
	ERR_FAIL_COND_V(p_state.is_null(), ERR_INVALID_PARAMETER);
	Error err = OK;
	KSVIRTUAL_CALL(_import_pre_generate, p_state, err);
	return err;
}

Node3D *GLTFDocumentExtension::generate_scene_node(Ref<GLTFState> p_state, Ref<GLTFNode> p_gltf_node, Node *p_scene_parent) {
	ERR_FAIL_COND_V(p_state.is_null(), nullptr);
	ERR_FAIL_COND_V(p_gltf_node.is_null(), nullptr);
	Node3D *ret_node = nullptr;
	KSVIRTUAL_CALL(_generate_scene_node, p_state, p_gltf_node, p_scene_parent, ret_node);
	return ret_node;
}

Error GLTFDocumentExtension::import_node(Ref<GLTFState> p_state, Ref<GLTFNode> p_gltf_node, Dictionary &r_dict, Node *p_node) {
	ERR_FAIL_COND_V(p_state.is_null(), ERR_INVALID_PARAMETER);
	ERR_FAIL_COND_V(p_gltf_node.is_null(), ERR_INVALID_PARAMETER);
	ERR_FAIL_NULL_V(p_node, ERR_INVALID_PARAMETER);
	Error err = OK;
	KSVIRTUAL_CALL(_import_node, p_state, p_gltf_node, r_dict, p_node, err);
	return err;
}

Error GLTFDocumentExtension::import_post(Ref<GLTFState> p_state, Node *p_root) {
	ERR_FAIL_NULL_V(p_root, ERR_INVALID_PARAMETER);
	ERR_FAIL_COND_V(p_state.is_null(), ERR_INVALID_PARAMETER);
	Error err = OK;
	KSVIRTUAL_CALL(_import_post, p_state, p_root, err);
	return err;
}

// Export process.
Error GLTFDocumentExtension::export_preflight(Ref<GLTFState> p_state, Node *p_root) {
	ERR_FAIL_NULL_V(p_root, ERR_INVALID_PARAMETER);
	Error err = OK;
	KSVIRTUAL_CALL(_export_preflight, p_state, p_root, err);
	return err;
}

void GLTFDocumentExtension::convert_scene_node(Ref<GLTFState> p_state, Ref<GLTFNode> p_gltf_node, Node *p_scene_node) {
	ERR_FAIL_COND(p_state.is_null());
	ERR_FAIL_COND(p_gltf_node.is_null());
	ERR_FAIL_NULL(p_scene_node);
	KSVIRTUAL_CALL(_convert_scene_node, p_state, p_gltf_node, p_scene_node);
}

Error GLTFDocumentExtension::export_post_convert(Ref<GLTFState> p_state, Node *p_root) {
	ERR_FAIL_COND_V(p_state.is_null(), ERR_INVALID_PARAMETER);
	ERR_FAIL_NULL_V(p_root, ERR_INVALID_PARAMETER);
	Error err = OK;
	KSVIRTUAL_CALL(_export_post_convert, p_state, p_root, err);
	return err;
}

Error GLTFDocumentExtension::export_preserialize(Ref<GLTFState> p_state) {
	ERR_FAIL_COND_V(p_state.is_null(), ERR_INVALID_PARAMETER);
	Error err = OK;
	KSVIRTUAL_CALL(_export_preserialize, p_state, err);
	return err;
}

Ref<GLTFObjectModelProperty> GLTFDocumentExtension::export_object_model_property(Ref<GLTFState> p_state, const NodePath &p_node_path, const Node *p_kosmic_node, GLTFNodeIndex p_gltf_node_index, const Object *p_target_object, int p_target_depth) {
	Ref<GLTFObjectModelProperty> ret;
	ERR_FAIL_COND_V(p_state.is_null(), ret);
	ERR_FAIL_NULL_V(p_kosmic_node, ret);
	ERR_FAIL_NULL_V(p_target_object, ret);
	KSVIRTUAL_CALL(_export_object_model_property, p_state, p_node_path, p_kosmic_node, p_gltf_node_index, p_target_object, p_target_depth, ret);
	return ret;
}

Vector<String> GLTFDocumentExtension::get_saveable_image_formats() {
	Vector<String> ret;
	KSVIRTUAL_CALL(_get_saveable_image_formats, ret);
	return ret;
}

PackedByteArray GLTFDocumentExtension::serialize_image_to_bytes(Ref<GLTFState> p_state, Ref<Image> p_image, Dictionary p_image_dict, const String &p_image_format, float p_lossy_quality) {
	PackedByteArray ret;
	ERR_FAIL_COND_V(p_state.is_null(), ret);
	ERR_FAIL_COND_V(p_image.is_null(), ret);
	KSVIRTUAL_CALL(_serialize_image_to_bytes, p_state, p_image, p_image_dict, p_image_format, p_lossy_quality, ret);
	return ret;
}

Error GLTFDocumentExtension::save_image_at_path(Ref<GLTFState> p_state, Ref<Image> p_image, const String &p_file_path, const String &p_image_format, float p_lossy_quality) {
	ERR_FAIL_COND_V(p_state.is_null(), ERR_INVALID_PARAMETER);
	ERR_FAIL_COND_V(p_image.is_null(), ERR_INVALID_PARAMETER);
	Error ret = OK;
	KSVIRTUAL_CALL(_save_image_at_path, p_state, p_image, p_file_path, p_image_format, p_lossy_quality, ret);
	return ret;
}

Error GLTFDocumentExtension::serialize_texture_json(Ref<GLTFState> p_state, Dictionary p_texture_json, Ref<GLTFTexture> p_gltf_texture, const String &p_image_format) {
	ERR_FAIL_COND_V(p_state.is_null(), ERR_INVALID_PARAMETER);
	ERR_FAIL_COND_V(p_gltf_texture.is_null(), ERR_INVALID_PARAMETER);
	Error err = OK;
	KSVIRTUAL_CALL(_serialize_texture_json, p_state, p_texture_json, p_gltf_texture, p_image_format, err);
	return err;
}

Error GLTFDocumentExtension::export_node(Ref<GLTFState> p_state, Ref<GLTFNode> p_gltf_node, Dictionary &r_dict, Node *p_node) {
	ERR_FAIL_COND_V(p_state.is_null(), ERR_INVALID_PARAMETER);
	ERR_FAIL_COND_V(p_gltf_node.is_null(), ERR_INVALID_PARAMETER);
	Error err = OK;
	KSVIRTUAL_CALL(_export_node, p_state, p_gltf_node, r_dict, p_node, err);
	return err;
}

Error GLTFDocumentExtension::export_post(Ref<GLTFState> p_state) {
	ERR_FAIL_COND_V(p_state.is_null(), ERR_INVALID_PARAMETER);
	Error err = OK;
	KSVIRTUAL_CALL(_export_post, p_state, err);
	return err;
}
