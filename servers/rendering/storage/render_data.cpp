/**************************************************************************/
/*  render_data.cpp                                                       */
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

#include "render_data.h"

void RenderData::_bind_methods() {
	ClassDB::bind_method(D_METHOD("get_render_scene_buffers"), &RenderData::get_render_scene_buffers);
	ClassDB::bind_method(D_METHOD("get_render_scene_data"), &RenderData::get_render_scene_data);
	ClassDB::bind_method(D_METHOD("get_environment"), &RenderData::get_environment);
	ClassDB::bind_method(D_METHOD("get_camera_attributes"), &RenderData::get_camera_attributes);
}

void RenderDataExtension::_bind_methods() {
	KSVIRTUAL_BIND(_get_render_scene_buffers);
	KSVIRTUAL_BIND(_get_render_scene_data)
	KSVIRTUAL_BIND(_get_environment)
	KSVIRTUAL_BIND(_get_camera_attributes)
}

Ref<RenderSceneBuffers> RenderDataExtension::get_render_scene_buffers() const {
	Ref<RenderSceneBuffers> ret;
	KSVIRTUAL_CALL(_get_render_scene_buffers, ret);
	return ret;
}

RenderSceneData *RenderDataExtension::get_render_scene_data() const {
	RenderSceneData *ret = nullptr;
	KSVIRTUAL_CALL(_get_render_scene_data, ret);
	return ret;
}

RID RenderDataExtension::get_environment() const {
	RID ret;
	KSVIRTUAL_CALL(_get_environment, ret);
	return ret;
}

RID RenderDataExtension::get_camera_attributes() const {
	RID ret;
	KSVIRTUAL_CALL(_get_camera_attributes, ret);
	return ret;
}
