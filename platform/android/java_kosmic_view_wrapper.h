/**************************************************************************/
/*  java_kosmic_view_wrapper.h                                             */
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

#ifndef JAVA_KOSMIC_VIEW_WRAPPER_H
#define JAVA_KOSMIC_VIEW_WRAPPER_H

#include "jni_utils.h"

#include "core/math/vector2.h"

#include <android/log.h>
#include <jni.h>

// Class that makes functions in java/src/org/kosmicengine/kosmic/KosmicRenderView.java callable from C++
class KosmicJavaViewWrapper {
private:
	jclass _cls;

	jobject _kosmic_view;

	jmethodID _can_capture_pointer = 0;
	jmethodID _request_pointer_capture = 0;
	jmethodID _release_pointer_capture = 0;

	jmethodID _configure_pointer_icon = 0;
	jmethodID _set_pointer_icon = 0;

public:
	KosmicJavaViewWrapper(jobject kosmic_view);

	bool can_update_pointer_icon() const;
	bool can_capture_pointer() const;

	void request_pointer_capture();
	void release_pointer_capture();

	void configure_pointer_icon(int pointer_type, const String &image_path, const Vector2 &p_hotspot);
	void set_pointer_icon(int pointer_type);

	~KosmicJavaViewWrapper();
};

#endif // JAVA_KOSMIC_VIEW_WRAPPER_H
