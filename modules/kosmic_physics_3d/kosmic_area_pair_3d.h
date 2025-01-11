/**************************************************************************/
/*  kosmic_area_pair_3d.h                                                  */
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

#ifndef KOSMIC_AREA_PAIR_3D_H
#define KOSMIC_AREA_PAIR_3D_H

#include "kosmic_area_3d.h"
#include "kosmic_body_3d.h"
#include "kosmic_constraint_3d.h"
#include "kosmic_soft_body_3d.h"

class KosmicAreaPair3D : public KosmicConstraint3D {
	KosmicBody3D *body = nullptr;
	KosmicArea3D *area = nullptr;
	int body_shape;
	int area_shape;
	bool colliding = false;
	bool process_collision = false;
	bool has_space_override = false;
	bool body_has_attached_area = false;

public:
	virtual bool setup(real_t p_step) override;
	virtual bool pre_solve(real_t p_step) override;
	virtual void solve(real_t p_step) override;

	KosmicAreaPair3D(KosmicBody3D *p_body, int p_body_shape, KosmicArea3D *p_area, int p_area_shape);
	~KosmicAreaPair3D();
};

class KosmicArea2Pair3D : public KosmicConstraint3D {
	KosmicArea3D *area_a = nullptr;
	KosmicArea3D *area_b = nullptr;
	int shape_a;
	int shape_b;
	bool colliding_a = false;
	bool colliding_b = false;
	bool process_collision_a = false;
	bool process_collision_b = false;
	bool area_a_monitorable;
	bool area_b_monitorable;

public:
	virtual bool setup(real_t p_step) override;
	virtual bool pre_solve(real_t p_step) override;
	virtual void solve(real_t p_step) override;

	KosmicArea2Pair3D(KosmicArea3D *p_area_a, int p_shape_a, KosmicArea3D *p_area_b, int p_shape_b);
	~KosmicArea2Pair3D();
};

class KosmicAreaSoftBodyPair3D : public KosmicConstraint3D {
	KosmicSoftBody3D *soft_body = nullptr;
	KosmicArea3D *area = nullptr;
	int soft_body_shape;
	int area_shape;
	bool colliding = false;
	bool process_collision = false;
	bool has_space_override = false;
	bool body_has_attached_area = false;

public:
	virtual bool setup(real_t p_step) override;
	virtual bool pre_solve(real_t p_step) override;
	virtual void solve(real_t p_step) override;

	KosmicAreaSoftBodyPair3D(KosmicSoftBody3D *p_sof_body, int p_soft_body_shape, KosmicArea3D *p_area, int p_area_shape);
	~KosmicAreaSoftBodyPair3D();
};

#endif // KOSMIC_AREA_PAIR_3D_H
