/**************************************************************************/
/*  render_scene_data.cpp                                                 */
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

#include "render_scene_data.h"

void RenderSceneData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_cam_transform"), &RenderSceneData::get_cam_transform);
	ClassDB::bind_method(D_METHOD("get_cam_projection"), &RenderSceneData::get_cam_projection);

	ClassDB::bind_method(D_METHOD("get_view_count"), &RenderSceneData::get_view_count);
	ClassDB::bind_method(D_METHOD("get_view_eye_offset", "view"), &RenderSceneData::get_view_eye_offset);
	ClassDB::bind_method(D_METHOD("get_view_projection", "view"), &RenderSceneData::get_view_projection);

	ClassDB::bind_method(D_METHOD("get_uniform_buffer"), &RenderSceneData::get_uniform_buffer);
}

void RenderSceneDataExtension::_bind_methods() {
	KSVIRTUAL_BIND(_get_cam_transform);
	KSVIRTUAL_BIND(_get_cam_projection);
	KSVIRTUAL_BIND(_get_view_count);
	KSVIRTUAL_BIND(_get_view_eye_offset, "view");
	KSVIRTUAL_BIND(_get_view_projection, "view");

	KSVIRTUAL_BIND(_get_uniform_buffer);
}

Transform3D RenderSceneDataExtension::get_cam_transform() const {
	Transform3D ret;
	KSVIRTUAL_CALL(_get_cam_transform, ret);
	return ret;
}

Projection RenderSceneDataExtension::get_cam_projection() const {
	Projection ret;
	KSVIRTUAL_CALL(_get_cam_projection, ret);
	return ret;
}

uint32_t RenderSceneDataExtension::get_view_count() const {
	uint32_t ret = 0;
	KSVIRTUAL_CALL(_get_view_count, ret);
	return ret;
}

Vector3 RenderSceneDataExtension::get_view_eye_offset(uint32_t p_view) const {
	Vector3 ret;
	KSVIRTUAL_CALL(_get_view_eye_offset, p_view, ret);
	return ret;
}

Projection RenderSceneDataExtension::get_view_projection(uint32_t p_view) const {
	Projection ret;
	KSVIRTUAL_CALL(_get_view_projection, p_view, ret);
	return ret;
}

RID RenderSceneDataExtension::get_uniform_buffer() const {
	RID ret;
	KSVIRTUAL_CALL(_get_uniform_buffer, ret);
	return ret;
}
