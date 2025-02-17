/**************************************************************************/
/*  voyscript_lambda_callable.h                                            */
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

#ifndef VOYSCRIPT_LAMBDA_CALLABLE_H
#define VOYSCRIPT_LAMBDA_CALLABLE_H

#include "voyscript.h"

#include "core/object/ref_counted.h"
#include "core/templates/vector.h"
#include "core/variant/callable.h"
#include "core/variant/variant.h"

class VoyScriptFunction;
class VoyScriptInstance;

class VoyScriptLambdaCallable : public CallableCustom {
	VoyScript::UpdatableFuncPtr function;
	Ref<VoyScript> script;
	uint32_t h;

	Vector<Variant> captures;

	static bool compare_equal(const CallableCustom *p_a, const CallableCustom *p_b);
	static bool compare_less(const CallableCustom *p_a, const CallableCustom *p_b);

public:
	bool is_valid() const override;
	uint32_t hash() const override;
	String get_as_text() const override;
	CompareEqualFunc get_compare_equal_func() const override;
	CompareLessFunc get_compare_less_func() const override;
	ObjectID get_object() const override;
	StringName get_method() const override;
	int get_argument_count(bool &r_is_valid) const override;
	void call(const Variant **p_arguments, int p_argcount, Variant &r_return_value, Callable::CallError &r_call_error) const override;

	VoyScriptLambdaCallable(VoyScriptLambdaCallable &) = delete;
	VoyScriptLambdaCallable(const VoyScriptLambdaCallable &) = delete;
	VoyScriptLambdaCallable(Ref<VoyScript> p_script, VoyScriptFunction *p_function, const Vector<Variant> &p_captures);
	virtual ~VoyScriptLambdaCallable() = default;
};

// Lambda callable that references a particular object, so it can use `self` in the body.
class VoyScriptLambdaSelfCallable : public CallableCustom {
	VoyScript::UpdatableFuncPtr function;
	Ref<RefCounted> reference; // For objects that are RefCounted, keep a reference.
	Object *object = nullptr; // For non RefCounted objects, use a direct pointer.
	uint32_t h;

	Vector<Variant> captures;

	static bool compare_equal(const CallableCustom *p_a, const CallableCustom *p_b);
	static bool compare_less(const CallableCustom *p_a, const CallableCustom *p_b);

public:
	bool is_valid() const override;
	uint32_t hash() const override;
	String get_as_text() const override;
	CompareEqualFunc get_compare_equal_func() const override;
	CompareLessFunc get_compare_less_func() const override;
	ObjectID get_object() const override;
	StringName get_method() const override;
	int get_argument_count(bool &r_is_valid) const override;
	void call(const Variant **p_arguments, int p_argcount, Variant &r_return_value, Callable::CallError &r_call_error) const override;

	VoyScriptLambdaSelfCallable(VoyScriptLambdaSelfCallable &) = delete;
	VoyScriptLambdaSelfCallable(const VoyScriptLambdaSelfCallable &) = delete;
	VoyScriptLambdaSelfCallable(Ref<RefCounted> p_self, VoyScriptFunction *p_function, const Vector<Variant> &p_captures);
	VoyScriptLambdaSelfCallable(Object *p_self, VoyScriptFunction *p_function, const Vector<Variant> &p_captures);
	virtual ~VoyScriptLambdaSelfCallable() = default;
};

#endif // VOYSCRIPT_LAMBDA_CALLABLE_H
