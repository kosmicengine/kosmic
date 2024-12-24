/**************************************************************************/
/*  interop_types.h                                                       */
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

#ifndef INTEROP_TYPES_H
#define INTEROP_TYPES_H

#include "core/math/math_defs.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <stdbool.h>
#include <stdint.h>

// This is taken from the old GDNative, which was removed.

#define KOSMIC_VARIANT_SIZE (sizeof(real_t) * 4 + sizeof(int64_t))

typedef struct {
	uint8_t _dont_touch_that[KOSMIC_VARIANT_SIZE];
} kosmic_variant;

#define KOSMIC_ARRAY_SIZE sizeof(void *)

typedef struct {
	uint8_t _dont_touch_that[KOSMIC_ARRAY_SIZE];
} kosmic_array;

#define KOSMIC_DICTIONARY_SIZE sizeof(void *)

typedef struct {
	uint8_t _dont_touch_that[KOSMIC_DICTIONARY_SIZE];
} kosmic_dictionary;

#define KOSMIC_STRING_SIZE sizeof(void *)

typedef struct {
	uint8_t _dont_touch_that[KOSMIC_STRING_SIZE];
} kosmic_string;

#define KOSMIC_STRING_NAME_SIZE sizeof(void *)

typedef struct {
	uint8_t _dont_touch_that[KOSMIC_STRING_NAME_SIZE];
} kosmic_string_name;

#define KOSMIC_PACKED_ARRAY_SIZE (2 * sizeof(void *))

typedef struct {
	uint8_t _dont_touch_that[KOSMIC_PACKED_ARRAY_SIZE];
} kosmic_packed_array;

#define KOSMIC_VECTOR2_SIZE (sizeof(real_t) * 2)

typedef struct {
	uint8_t _dont_touch_that[KOSMIC_VECTOR2_SIZE];
} kosmic_vector2;

#define KOSMIC_VECTOR2I_SIZE (sizeof(int32_t) * 2)

typedef struct {
	uint8_t _dont_touch_that[KOSMIC_VECTOR2I_SIZE];
} kosmic_vector2i;

#define KOSMIC_RECT2_SIZE (sizeof(real_t) * 4)

typedef struct kosmic_rect2 {
	uint8_t _dont_touch_that[KOSMIC_RECT2_SIZE];
} kosmic_rect2;

#define KOSMIC_RECT2I_SIZE (sizeof(int32_t) * 4)

typedef struct kosmic_rect2i {
	uint8_t _dont_touch_that[KOSMIC_RECT2I_SIZE];
} kosmic_rect2i;

#define KOSMIC_VECTOR3_SIZE (sizeof(real_t) * 3)

typedef struct {
	uint8_t _dont_touch_that[KOSMIC_VECTOR3_SIZE];
} kosmic_vector3;

#define KOSMIC_VECTOR3I_SIZE (sizeof(int32_t) * 3)

typedef struct {
	uint8_t _dont_touch_that[KOSMIC_VECTOR3I_SIZE];
} kosmic_vector3i;

#define KOSMIC_TRANSFORM2D_SIZE (sizeof(real_t) * 6)

typedef struct {
	uint8_t _dont_touch_that[KOSMIC_TRANSFORM2D_SIZE];
} kosmic_transform2d;

#define KOSMIC_VECTOR4_SIZE (sizeof(real_t) * 4)

typedef struct {
	uint8_t _dont_touch_that[KOSMIC_VECTOR4_SIZE];
} kosmic_vector4;

#define KOSMIC_VECTOR4I_SIZE (sizeof(int32_t) * 4)

typedef struct {
	uint8_t _dont_touch_that[KOSMIC_VECTOR4I_SIZE];
} kosmic_vector4i;

#define KOSMIC_PLANE_SIZE (sizeof(real_t) * 4)

typedef struct {
	uint8_t _dont_touch_that[KOSMIC_PLANE_SIZE];
} kosmic_plane;

#define KOSMIC_QUATERNION_SIZE (sizeof(real_t) * 4)

typedef struct {
	uint8_t _dont_touch_that[KOSMIC_QUATERNION_SIZE];
} kosmic_quaternion;

#define KOSMIC_AABB_SIZE (sizeof(real_t) * 6)

typedef struct {
	uint8_t _dont_touch_that[KOSMIC_AABB_SIZE];
} kosmic_aabb;

#define KOSMIC_BASIS_SIZE (sizeof(real_t) * 9)

typedef struct {
	uint8_t _dont_touch_that[KOSMIC_BASIS_SIZE];
} kosmic_basis;

#define KOSMIC_TRANSFORM3D_SIZE (sizeof(real_t) * 12)

typedef struct {
	uint8_t _dont_touch_that[KOSMIC_TRANSFORM3D_SIZE];
} kosmic_transform3d;

#define KOSMIC_PROJECTION_SIZE (sizeof(real_t) * 4 * 4)

typedef struct {
	uint8_t _dont_touch_that[KOSMIC_PROJECTION_SIZE];
} kosmic_projection;

// Colors should always use 32-bit floats, so don't use real_t here.
#define KOSMIC_COLOR_SIZE (sizeof(float) * 4)

typedef struct {
	uint8_t _dont_touch_that[KOSMIC_COLOR_SIZE];
} kosmic_color;

#define KOSMIC_NODE_PATH_SIZE sizeof(void *)

typedef struct {
	uint8_t _dont_touch_that[KOSMIC_NODE_PATH_SIZE];
} kosmic_node_path;

#define KOSMIC_RID_SIZE sizeof(uint64_t)

typedef struct {
	uint8_t _dont_touch_that[KOSMIC_RID_SIZE];
} kosmic_rid;

// Alignment hardcoded in `core/variant/callable.h`.
#define KOSMIC_CALLABLE_SIZE (16)

typedef struct {
	uint8_t _dont_touch_that[KOSMIC_CALLABLE_SIZE];
} kosmic_callable;

// Alignment hardcoded in `core/variant/callable.h`.
#define KOSMIC_SIGNAL_SIZE (16)

typedef struct {
	uint8_t _dont_touch_that[KOSMIC_SIGNAL_SIZE];
} kosmic_signal;

#ifdef __cplusplus
}
#endif

#endif // INTEROP_TYPES_H
