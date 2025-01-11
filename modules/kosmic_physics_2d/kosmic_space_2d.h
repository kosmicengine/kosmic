/**************************************************************************/
/*  kosmic_space_2d.h                                                      */
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

#ifndef KOSMIC_SPACE_2D_H
#define KOSMIC_SPACE_2D_H

#include "kosmic_area_2d.h"
#include "kosmic_body_2d.h"
#include "kosmic_broad_phase_2d.h"
#include "kosmic_collision_object_2d.h"

#include "core/typedefs.h"

class KosmicPhysicsDirectSpaceState2D : public PhysicsDirectSpaceState2D {
	KSCLASS(KosmicPhysicsDirectSpaceState2D, PhysicsDirectSpaceState2D);

public:
	KosmicSpace2D *space = nullptr;

	virtual int intersect_point(const PointParameters &p_parameters, ShapeResult *r_results, int p_result_max) override;
	virtual bool intersect_ray(const RayParameters &p_parameters, RayResult &r_result) override;
	virtual int intersect_shape(const ShapeParameters &p_parameters, ShapeResult *r_results, int p_result_max) override;
	virtual bool cast_motion(const ShapeParameters &p_parameters, real_t &p_closest_safe, real_t &p_closest_unsafe) override;
	virtual bool collide_shape(const ShapeParameters &p_parameters, Vector2 *r_results, int p_result_max, int &r_result_count) override;
	virtual bool rest_info(const ShapeParameters &p_parameters, ShapeRestInfo *r_info) override;

	KosmicPhysicsDirectSpaceState2D() {}
};

class KosmicSpace2D {
public:
	enum ElapsedTime {
		ELAPSED_TIME_INTEGRATE_FORCES,
		ELAPSED_TIME_GENERATE_ISLANDS,
		ELAPSED_TIME_SETUP_CONSTRAINTS,
		ELAPSED_TIME_SOLVE_CONSTRAINTS,
		ELAPSED_TIME_INTEGRATE_VELOCITIES,
		ELAPSED_TIME_MAX

	};

private:
	struct ExcludedShapeSW {
		KosmicShape2D *local_shape = nullptr;
		const KosmicCollisionObject2D *against_object = nullptr;
		int against_shape_index = 0;
	};

	uint64_t elapsed_time[ELAPSED_TIME_MAX] = {};

	KosmicPhysicsDirectSpaceState2D *direct_access = nullptr;
	RID self;

	KosmicBroadPhase2D *broadphase = nullptr;
	SelfList<KosmicBody2D>::List active_list;
	SelfList<KosmicBody2D>::List mass_properties_update_list;
	SelfList<KosmicBody2D>::List state_query_list;
	SelfList<KosmicArea2D>::List monitor_query_list;
	SelfList<KosmicArea2D>::List area_moved_list;

	static void *_broadphase_pair(KosmicCollisionObject2D *A, int p_subindex_A, KosmicCollisionObject2D *B, int p_subindex_B, void *p_self);
	static void _broadphase_unpair(KosmicCollisionObject2D *A, int p_subindex_A, KosmicCollisionObject2D *B, int p_subindex_B, void *p_data, void *p_self);

	HashSet<KosmicCollisionObject2D *> objects;

	KosmicArea2D *area = nullptr;

	int solver_iterations = 0;

	real_t contact_recycle_radius = 0.0;
	real_t contact_max_separation = 0.0;
	real_t contact_max_allowed_penetration = 0.0;
	real_t contact_bias = 0.0;
	real_t constraint_bias = 0.0;

	enum {
		INTERSECTION_QUERY_MAX = 2048
	};

	KosmicCollisionObject2D *intersection_query_results[INTERSECTION_QUERY_MAX];
	int intersection_query_subindex_results[INTERSECTION_QUERY_MAX];

	real_t body_linear_velocity_sleep_threshold = 0.0;
	real_t body_angular_velocity_sleep_threshold = 0.0;
	real_t body_time_to_sleep = 0.0;

	bool locked = false;

	real_t last_step = 0.001;

	int island_count = 0;
	int active_objects = 0;
	int collision_pairs = 0;

	int _cull_aabb_for_body(KosmicBody2D *p_body, const Rect2 &p_aabb);

	Vector<Vector2> contact_debug;
	int contact_debug_count = 0;

	friend class KosmicPhysicsDirectSpaceState2D;

public:
	_FORCE_INLINE_ void set_self(const RID &p_self) { self = p_self; }
	_FORCE_INLINE_ RID get_self() const { return self; }

	void set_default_area(KosmicArea2D *p_area) { area = p_area; }
	KosmicArea2D *get_default_area() const { return area; }

	const SelfList<KosmicBody2D>::List &get_active_body_list() const;
	void body_add_to_active_list(SelfList<KosmicBody2D> *p_body);
	void body_remove_from_active_list(SelfList<KosmicBody2D> *p_body);
	void body_add_to_mass_properties_update_list(SelfList<KosmicBody2D> *p_body);
	void body_remove_from_mass_properties_update_list(SelfList<KosmicBody2D> *p_body);
	void area_add_to_moved_list(SelfList<KosmicArea2D> *p_area);
	void area_remove_from_moved_list(SelfList<KosmicArea2D> *p_area);
	const SelfList<KosmicArea2D>::List &get_moved_area_list() const;

	void body_add_to_state_query_list(SelfList<KosmicBody2D> *p_body);
	void body_remove_from_state_query_list(SelfList<KosmicBody2D> *p_body);

	void area_add_to_monitor_query_list(SelfList<KosmicArea2D> *p_area);
	void area_remove_from_monitor_query_list(SelfList<KosmicArea2D> *p_area);

	KosmicBroadPhase2D *get_broadphase();

	void add_object(KosmicCollisionObject2D *p_object);
	void remove_object(KosmicCollisionObject2D *p_object);
	const HashSet<KosmicCollisionObject2D *> &get_objects() const;

	_FORCE_INLINE_ int get_solver_iterations() const { return solver_iterations; }
	_FORCE_INLINE_ real_t get_contact_recycle_radius() const { return contact_recycle_radius; }
	_FORCE_INLINE_ real_t get_contact_max_separation() const { return contact_max_separation; }
	_FORCE_INLINE_ real_t get_contact_max_allowed_penetration() const { return contact_max_allowed_penetration; }
	_FORCE_INLINE_ real_t get_contact_bias() const { return contact_bias; }
	_FORCE_INLINE_ real_t get_constraint_bias() const { return constraint_bias; }
	_FORCE_INLINE_ real_t get_body_linear_velocity_sleep_threshold() const { return body_linear_velocity_sleep_threshold; }
	_FORCE_INLINE_ real_t get_body_angular_velocity_sleep_threshold() const { return body_angular_velocity_sleep_threshold; }
	_FORCE_INLINE_ real_t get_body_time_to_sleep() const { return body_time_to_sleep; }

	void update();
	void setup();
	void call_queries();

	bool is_locked() const;
	void lock();
	void unlock();

	real_t get_last_step() const { return last_step; }
	void set_last_step(real_t p_step) { last_step = p_step; }

	void set_param(PhysicsServer2D::SpaceParameter p_param, real_t p_value);
	real_t get_param(PhysicsServer2D::SpaceParameter p_param) const;

	void set_island_count(int p_island_count) { island_count = p_island_count; }
	int get_island_count() const { return island_count; }

	void set_active_objects(int p_active_objects) { active_objects = p_active_objects; }
	int get_active_objects() const { return active_objects; }

	int get_collision_pairs() const { return collision_pairs; }

	bool test_body_motion(KosmicBody2D *p_body, const PhysicsServer2D::MotionParameters &p_parameters, PhysicsServer2D::MotionResult *r_result);

	void set_debug_contacts(int p_amount) { contact_debug.resize(p_amount); }
	_FORCE_INLINE_ bool is_debugging_contacts() const { return !contact_debug.is_empty(); }
	_FORCE_INLINE_ void add_debug_contact(const Vector2 &p_contact) {
		if (contact_debug_count < contact_debug.size()) {
			contact_debug.write[contact_debug_count++] = p_contact;
		}
	}
	_FORCE_INLINE_ Vector<Vector2> get_debug_contacts() { return contact_debug; }
	_FORCE_INLINE_ int get_debug_contact_count() { return contact_debug_count; }

	KosmicPhysicsDirectSpaceState2D *get_direct_state();

	void set_elapsed_time(ElapsedTime p_time, uint64_t p_msec) { elapsed_time[p_time] = p_msec; }
	uint64_t get_elapsed_time(ElapsedTime p_time) const { return elapsed_time[p_time]; }

	KosmicSpace2D();
	~KosmicSpace2D();
};

#endif // KOSMIC_SPACE_2D_H
