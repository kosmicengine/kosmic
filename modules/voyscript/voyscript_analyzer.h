/**************************************************************************/
/*  voyscript_analyzer.h                                                   */
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

#ifndef VOYSCRIPT_ANALYZER_H
#define VOYSCRIPT_ANALYZER_H

#include "voyscript_cache.h"
#include "voyscript_parser.h"

#include "core/object/object.h"
#include "core/object/ref_counted.h"

class VoyScriptAnalyzer {
	VoyScriptParser *parser = nullptr;

	template <typename Fn>
	class Finally {
		Fn fn;

	public:
		Finally(Fn p_fn) :
				fn(p_fn) {}
		~Finally() {
			fn();
		}
	};

	const VoyScriptParser::EnumNode *current_enum = nullptr;
	VoyScriptParser::LambdaNode *current_lambda = nullptr;
	List<VoyScriptParser::LambdaNode *> pending_body_resolution_lambdas;
	HashMap<const VoyScriptParser::ClassNode *, Ref<VoyScriptParserRef>> external_class_parser_cache;
	bool static_context = false;

	// Tests for detecting invalid overloading of script members
	static _FORCE_INLINE_ bool has_member_name_conflict_in_script_class(const StringName &p_name, const VoyScriptParser::ClassNode *p_current_class_node, const VoyScriptParser::Node *p_member);
	static _FORCE_INLINE_ bool has_member_name_conflict_in_native_type(const StringName &p_name, const StringName &p_native_type_string);
	Error check_native_member_name_conflict(const StringName &p_member_name, const VoyScriptParser::Node *p_member_node, const StringName &p_native_type_string);
	Error check_class_member_name_conflict(const VoyScriptParser::ClassNode *p_class_node, const StringName &p_member_name, const VoyScriptParser::Node *p_member_node);

	void get_class_node_current_scope_classes(VoyScriptParser::ClassNode *p_node, List<VoyScriptParser::ClassNode *> *p_list, VoyScriptParser::Node *p_source);

	Error resolve_class_inheritance(VoyScriptParser::ClassNode *p_class, const VoyScriptParser::Node *p_source = nullptr);
	Error resolve_class_inheritance(VoyScriptParser::ClassNode *p_class, bool p_recursive);
	VoyScriptParser::DataType resolve_datatype(VoyScriptParser::TypeNode *p_type);

	void decide_suite_type(VoyScriptParser::Node *p_suite, VoyScriptParser::Node *p_statement);

	void resolve_annotation(VoyScriptParser::AnnotationNode *p_annotation);
	void resolve_class_member(VoyScriptParser::ClassNode *p_class, const StringName &p_name, const VoyScriptParser::Node *p_source = nullptr);
	void resolve_class_member(VoyScriptParser::ClassNode *p_class, int p_index, const VoyScriptParser::Node *p_source = nullptr);
	void resolve_class_interface(VoyScriptParser::ClassNode *p_class, const VoyScriptParser::Node *p_source = nullptr);
	void resolve_class_interface(VoyScriptParser::ClassNode *p_class, bool p_recursive);
	void resolve_class_body(VoyScriptParser::ClassNode *p_class, const VoyScriptParser::Node *p_source = nullptr);
	void resolve_class_body(VoyScriptParser::ClassNode *p_class, bool p_recursive);
	void resolve_function_signature(VoyScriptParser::FunctionNode *p_function, const VoyScriptParser::Node *p_source = nullptr, bool p_is_lambda = false);
	void resolve_function_body(VoyScriptParser::FunctionNode *p_function, bool p_is_lambda = false);
	void resolve_node(VoyScriptParser::Node *p_node, bool p_is_root = true);
	void resolve_suite(VoyScriptParser::SuiteNode *p_suite);
	void resolve_assignable(VoyScriptParser::AssignableNode *p_assignable, const char *p_kind);
	void resolve_variable(VoyScriptParser::VariableNode *p_variable, bool p_is_local);
	void resolve_constant(VoyScriptParser::ConstantNode *p_constant, bool p_is_local);
	void resolve_parameter(VoyScriptParser::ParameterNode *p_parameter);
	void resolve_if(VoyScriptParser::IfNode *p_if);
	void resolve_for(VoyScriptParser::ForNode *p_for);
	void resolve_while(VoyScriptParser::WhileNode *p_while);
	void resolve_assert(VoyScriptParser::AssertNode *p_assert);
	void resolve_match(VoyScriptParser::MatchNode *p_match);
	void resolve_match_branch(VoyScriptParser::MatchBranchNode *p_match_branch, VoyScriptParser::ExpressionNode *p_match_test);
	void resolve_match_pattern(VoyScriptParser::PatternNode *p_match_pattern, VoyScriptParser::ExpressionNode *p_match_test);
	void resolve_return(VoyScriptParser::ReturnNode *p_return);

	// Reduction functions.
	void reduce_expression(VoyScriptParser::ExpressionNode *p_expression, bool p_is_root = false);
	void reduce_array(VoyScriptParser::ArrayNode *p_array);
	void reduce_assignment(VoyScriptParser::AssignmentNode *p_assignment);
	void reduce_await(VoyScriptParser::AwaitNode *p_await);
	void reduce_binary_op(VoyScriptParser::BinaryOpNode *p_binary_op);
	void reduce_call(VoyScriptParser::CallNode *p_call, bool p_is_await = false, bool p_is_root = false);
	void reduce_cast(VoyScriptParser::CastNode *p_cast);
	void reduce_dictionary(VoyScriptParser::DictionaryNode *p_dictionary);
	void reduce_get_node(VoyScriptParser::GetNodeNode *p_get_node);
	void reduce_identifier(VoyScriptParser::IdentifierNode *p_identifier, bool can_be_builtin = false);
	void reduce_identifier_from_base(VoyScriptParser::IdentifierNode *p_identifier, VoyScriptParser::DataType *p_base = nullptr);
	void reduce_lambda(VoyScriptParser::LambdaNode *p_lambda);
	void reduce_literal(VoyScriptParser::LiteralNode *p_literal);
	void reduce_preload(VoyScriptParser::PreloadNode *p_preload);
	void reduce_self(VoyScriptParser::SelfNode *p_self);
	void reduce_subscript(VoyScriptParser::SubscriptNode *p_subscript, bool p_can_be_pseudo_type = false);
	void reduce_ternary_op(VoyScriptParser::TernaryOpNode *p_ternary_op, bool p_is_root = false);
	void reduce_type_test(VoyScriptParser::TypeTestNode *p_type_test);
	void reduce_unary_op(VoyScriptParser::UnaryOpNode *p_unary_op);

	Variant make_expression_reduced_value(VoyScriptParser::ExpressionNode *p_expression, bool &is_reduced);
	Variant make_array_reduced_value(VoyScriptParser::ArrayNode *p_array, bool &is_reduced);
	Variant make_dictionary_reduced_value(VoyScriptParser::DictionaryNode *p_dictionary, bool &is_reduced);
	Variant make_subscript_reduced_value(VoyScriptParser::SubscriptNode *p_subscript, bool &is_reduced);

	// Helpers.
	Array make_array_from_element_datatype(const VoyScriptParser::DataType &p_element_datatype, const VoyScriptParser::Node *p_source_node = nullptr);
	Dictionary make_dictionary_from_element_datatype(const VoyScriptParser::DataType &p_key_element_datatype, const VoyScriptParser::DataType &p_value_element_datatype, const VoyScriptParser::Node *p_source_node = nullptr);
	VoyScriptParser::DataType type_from_variant(const Variant &p_value, const VoyScriptParser::Node *p_source);
	VoyScriptParser::DataType type_from_property(const PropertyInfo &p_property, bool p_is_arg = false, bool p_is_readonly = false) const;
	VoyScriptParser::DataType make_global_class_meta_type(const StringName &p_class_name, const VoyScriptParser::Node *p_source);
	bool get_function_signature(VoyScriptParser::Node *p_source, bool p_is_constructor, VoyScriptParser::DataType base_type, const StringName &p_function, VoyScriptParser::DataType &r_return_type, List<VoyScriptParser::DataType> &r_par_types, int &r_default_arg_count, BitField<MethodFlags> &r_method_flags, StringName *r_native_class = nullptr);
	bool function_signature_from_info(const MethodInfo &p_info, VoyScriptParser::DataType &r_return_type, List<VoyScriptParser::DataType> &r_par_types, int &r_default_arg_count, BitField<MethodFlags> &r_method_flags);
	void validate_call_arg(const List<VoyScriptParser::DataType> &p_par_types, int p_default_args_count, bool p_is_vararg, const VoyScriptParser::CallNode *p_call);
	void validate_call_arg(const MethodInfo &p_method, const VoyScriptParser::CallNode *p_call);
	VoyScriptParser::DataType get_operation_type(Variant::Operator p_operation, const VoyScriptParser::DataType &p_a, const VoyScriptParser::DataType &p_b, bool &r_valid, const VoyScriptParser::Node *p_source);
	VoyScriptParser::DataType get_operation_type(Variant::Operator p_operation, const VoyScriptParser::DataType &p_a, bool &r_valid, const VoyScriptParser::Node *p_source);
	void update_const_expression_builtin_type(VoyScriptParser::ExpressionNode *p_expression, const VoyScriptParser::DataType &p_type, const char *p_usage, bool p_is_cast = false);
	void update_array_literal_element_type(VoyScriptParser::ArrayNode *p_array, const VoyScriptParser::DataType &p_element_type);
	void update_dictionary_literal_element_type(VoyScriptParser::DictionaryNode *p_dictionary, const VoyScriptParser::DataType &p_key_element_type, const VoyScriptParser::DataType &p_value_element_type);
	bool is_type_compatible(const VoyScriptParser::DataType &p_target, const VoyScriptParser::DataType &p_source, bool p_allow_implicit_conversion = false, const VoyScriptParser::Node *p_source_node = nullptr);
	void push_error(const String &p_message, const VoyScriptParser::Node *p_origin = nullptr);
	void mark_node_unsafe(const VoyScriptParser::Node *p_node);
	void downgrade_node_type_source(VoyScriptParser::Node *p_node);
	void mark_lambda_use_self();
	void resolve_pending_lambda_bodies();
	bool class_exists(const StringName &p_class) const;
	void reduce_identifier_from_base_set_class(VoyScriptParser::IdentifierNode *p_identifier, VoyScriptParser::DataType p_identifier_datatype);
	Ref<VoyScriptParserRef> ensure_cached_external_parser_for_class(const VoyScriptParser::ClassNode *p_class, const VoyScriptParser::ClassNode *p_from_class, const char *p_context, const VoyScriptParser::Node *p_source);
	Ref<VoyScriptParserRef> find_cached_external_parser_for_class(const VoyScriptParser::ClassNode *p_class, const Ref<VoyScriptParserRef> &p_dependant_parser);
	Ref<VoyScriptParserRef> find_cached_external_parser_for_class(const VoyScriptParser::ClassNode *p_class, VoyScriptParser *p_dependant_parser);
	Ref<VoyScript> get_depended_shallow_script(const String &p_path, Error &r_error);
#ifdef DEBUG_ENABLED
	void is_shadowing(VoyScriptParser::IdentifierNode *p_identifier, const String &p_context, const bool p_in_local_scope);
#endif

public:
	Error resolve_inheritance();
	Error resolve_interface();
	Error resolve_body();
	Error resolve_dependencies();
	Error analyze();

	Variant make_variable_default_value(VoyScriptParser::VariableNode *p_variable);

	static bool check_type_compatibility(const VoyScriptParser::DataType &p_target, const VoyScriptParser::DataType &p_source, bool p_allow_implicit_conversion = false, const VoyScriptParser::Node *p_source_node = nullptr);
	static VoyScriptParser::DataType type_from_metatype(const VoyScriptParser::DataType &p_meta_type);

	VoyScriptAnalyzer(VoyScriptParser *p_parser);
};

#endif // VOYSCRIPT_ANALYZER_H
