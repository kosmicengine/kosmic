/**************************************************************************/
/*  gjk_epa.h                                                             */
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

#ifndef GJK_EPA_H
#define GJK_EPA_H

#include "kosmic_collision_solver_3d.h"
#include "kosmic_shape_3d.h"

bool gjk_epa_calculate_penetration(const KosmicShape3D *p_shape_A, const Transform3D &p_transform_A, const KosmicShape3D *p_shape_B, const Transform3D &p_transform_B, KosmicCollisionSolver3D::CallbackResult p_result_callback, void *p_userdata, bool p_swap = false, real_t p_margin_A = 0.0, real_t p_margin_B = 0.0);
bool gjk_epa_calculate_distance(const KosmicShape3D *p_shape_A, const Transform3D &p_transform_A, const KosmicShape3D *p_shape_B, const Transform3D &p_transform_B, Vector3 &r_result_A, Vector3 &r_result_B);

#endif // GJK_EPA_H
