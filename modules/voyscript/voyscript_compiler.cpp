/**************************************************************************/
/*  voyscript_compiler.cpp                                                 */
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

#include "voyscript_compiler.h"

#include "voyscript.h"
#include "voyscript_byte_codegen.h"
#include "voyscript_cache.h"
#include "voyscript_utility_functions.h"

#include "core/config/engine.h"
#include "core/config/project_settings.h"

#include "scene/scene_string_names.h"

bool VoyScriptCompiler::_is_class_member_property(CodeGen &codegen, const StringName &p_name) {
	if (codegen.function_node && codegen.function_node->is_static) {
		return false;
	}

	if (_is_local_or_parameter(codegen, p_name)) {
		return false; //shadowed
	}

	return _is_class_member_property(codegen.script, p_name);
}

bool VoyScriptCompiler::_is_class_member_property(VoyScript *owner, const StringName &p_name) {
	VoyScript *scr = owner;
	VoyScriptNativeClass *nc = nullptr;
	while (scr) {
		if (scr->native.is_valid()) {
			nc = scr->native.ptr();
		}
		scr = scr->_base;
	}

	ERR_FAIL_NULL_V(nc, false);

	return ClassDB::has_property(nc->get_name(), p_name);
}

bool VoyScriptCompiler::_is_local_or_parameter(CodeGen &codegen, const StringName &p_name) {
	return codegen.parameters.has(p_name) || codegen.locals.has(p_name);
}

void VoyScriptCompiler::_set_error(const String &p_error, const VoyScriptParser::Node *p_node) {
	if (!error.is_empty()) {
		return;
	}

	error = p_error;
	if (p_node) {
		err_line = p_node->start_line;
		err_column = p_node->leftmost_column;
	} else {
		err_line = 0;
		err_column = 0;
	}
}

VoyScriptDataType VoyScriptCompiler::_gdtype_from_datatype(const VoyScriptParser::DataType &p_datatype, VoyScript *p_owner, bool p_handle_metatype) {
	if (!p_datatype.is_set() || !p_datatype.is_hard_type() || p_datatype.is_coroutine) {
		return VoyScriptDataType();
	}

	VoyScriptDataType result;
	result.has_type = true;

	switch (p_datatype.kind) {
		case VoyScriptParser::DataType::VARIANT: {
			result.has_type = false;
		} break;
		case VoyScriptParser::DataType::BUILTIN: {
			result.kind = VoyScriptDataType::BUILTIN;
			result.builtin_type = p_datatype.builtin_type;
		} break;
		case VoyScriptParser::DataType::NATIVE: {
			if (p_handle_metatype && p_datatype.is_meta_type) {
				result.kind = VoyScriptDataType::NATIVE;
				result.builtin_type = Variant::OBJECT;
				// Fixes GH-82255. `VoyScriptNativeClass` is obtainable in VoyScript,
				// but is not a registered and exposed class, so `VoyScriptNativeClass`
				// is missing from `VoyScriptLanguage::get_singleton()->get_global_map()`.
				//result.native_type = VoyScriptNativeClass::get_class_static();
				result.native_type = Object::get_class_static();
				break;
			}

			result.kind = VoyScriptDataType::NATIVE;
			result.builtin_type = p_datatype.builtin_type;
			result.native_type = p_datatype.native_type;

#ifdef DEBUG_ENABLED
			if (unlikely(!VoyScriptLanguage::get_singleton()->get_global_map().has(result.native_type))) {
				_set_error(vformat(R"(VoyScript bug (please report): Native class "%s" not found.)", result.native_type), nullptr);
				return VoyScriptDataType();
			}
#endif
		} break;
		case VoyScriptParser::DataType::SCRIPT: {
			if (p_handle_metatype && p_datatype.is_meta_type) {
				result.kind = VoyScriptDataType::NATIVE;
				result.builtin_type = Variant::OBJECT;
				result.native_type = p_datatype.script_type.is_valid() ? p_datatype.script_type->get_class() : Script::get_class_static();
				break;
			}

			result.kind = VoyScriptDataType::SCRIPT;
			result.builtin_type = p_datatype.builtin_type;
			result.script_type_ref = p_datatype.script_type;
			result.script_type = result.script_type_ref.ptr();
			result.native_type = p_datatype.native_type;
		} break;
		case VoyScriptParser::DataType::CLASS: {
			if (p_handle_metatype && p_datatype.is_meta_type) {
				result.kind = VoyScriptDataType::NATIVE;
				result.builtin_type = Variant::OBJECT;
				result.native_type = VoyScript::get_class_static();
				break;
			}

			result.kind = VoyScriptDataType::VOYSCRIPT;
			result.builtin_type = p_datatype.builtin_type;
			result.native_type = p_datatype.native_type;

			bool is_local_class = parser->has_class(p_datatype.class_type);

			Ref<VoyScript> script;
			if (is_local_class) {
				script = Ref<VoyScript>(main_script);
			} else {
				Error err = OK;
				script = VoyScriptCache::get_shallow_script(p_datatype.script_path, err, p_owner->path);
				if (err) {
					_set_error(vformat(R"(Could not find script "%s": %s)", p_datatype.script_path, error_names[err]), nullptr);
					return VoyScriptDataType();
				}
			}

			if (script.is_valid()) {
				script = Ref<VoyScript>(script->find_class(p_datatype.class_type->fqcn));
			}

			if (script.is_null()) {
				_set_error(vformat(R"(Could not find class "%s" in "%s".)", p_datatype.class_type->fqcn, p_datatype.script_path), nullptr);
				return VoyScriptDataType();
			} else {
				// Only hold a strong reference if the owner of the element qualified with this type is not local, to avoid cyclic references (leaks).
				// TODO: Might lead to use after free if script_type is a subclass and is used after its parent is freed.
				if (!is_local_class) {
					result.script_type_ref = script;
				}
				result.script_type = script.ptr();
				result.native_type = p_datatype.native_type;
			}
		} break;
		case VoyScriptParser::DataType::ENUM:
			if (p_handle_metatype && p_datatype.is_meta_type) {
				result.kind = VoyScriptDataType::BUILTIN;
				result.builtin_type = Variant::DICTIONARY;
				break;
			}

			result.kind = VoyScriptDataType::BUILTIN;
			result.builtin_type = p_datatype.builtin_type;
			break;
		case VoyScriptParser::DataType::RESOLVING:
		case VoyScriptParser::DataType::UNRESOLVED: {
			_set_error("Parser bug (please report): converting unresolved type.", nullptr);
			return VoyScriptDataType();
		}
	}

	for (int i = 0; i < p_datatype.container_element_types.size(); i++) {
		result.set_container_element_type(i, _gdtype_from_datatype(p_datatype.get_container_element_type_or_variant(i), p_owner, false));
	}

	return result;
}

static bool _is_exact_type(const PropertyInfo &p_par_type, const VoyScriptDataType &p_arg_type) {
	if (!p_arg_type.has_type) {
		return false;
	}
	if (p_par_type.type == Variant::NIL) {
		return false;
	}
	if (p_par_type.type == Variant::OBJECT) {
		if (p_arg_type.kind == VoyScriptDataType::BUILTIN) {
			return false;
		}
		StringName class_name;
		if (p_arg_type.kind == VoyScriptDataType::NATIVE) {
			class_name = p_arg_type.native_type;
		} else {
			class_name = p_arg_type.native_type == StringName() ? p_arg_type.script_type->get_instance_base_type() : p_arg_type.native_type;
		}
		return p_par_type.class_name == class_name || ClassDB::is_parent_class(class_name, p_par_type.class_name);
	} else {
		if (p_arg_type.kind != VoyScriptDataType::BUILTIN) {
			return false;
		}
		return p_par_type.type == p_arg_type.builtin_type;
	}
}

static bool _can_use_validate_call(const MethodBind *p_method, const Vector<VoyScriptCodeGenerator::Address> &p_arguments) {
	if (p_method->is_vararg()) {
		// Validated call won't work with vararg methods.
		return false;
	}
	if (p_method->get_argument_count() != p_arguments.size()) {
		// Validated call won't work with default arguments.
		return false;
	}
	MethodInfo info;
	ClassDB::get_method_info(p_method->get_instance_class(), p_method->get_name(), &info);
	int i = 0;
	for (List<PropertyInfo>::ConstIterator itr = info.arguments.begin(); itr != info.arguments.end(); ++itr, ++i) {
		if (!_is_exact_type(*itr, p_arguments[i].type)) {
			return false;
		}
	}
	return true;
}

VoyScriptCodeGenerator::Address VoyScriptCompiler::_parse_expression(CodeGen &codegen, Error &r_error, const VoyScriptParser::ExpressionNode *p_expression, bool p_root, bool p_initializer) {
	if (p_expression->is_constant && !(p_expression->get_datatype().is_meta_type && p_expression->get_datatype().kind == VoyScriptParser::DataType::CLASS)) {
		return codegen.add_constant(p_expression->reduced_value);
	}

	VoyScriptCodeGenerator *gen = codegen.generator;

	switch (p_expression->type) {
		case VoyScriptParser::Node::IDENTIFIER: {
			// Look for identifiers in current scope.
			const VoyScriptParser::IdentifierNode *in = static_cast<const VoyScriptParser::IdentifierNode *>(p_expression);

			StringName identifier = in->name;

			switch (in->source) {
				// LOCALS.
				case VoyScriptParser::IdentifierNode::FUNCTION_PARAMETER:
				case VoyScriptParser::IdentifierNode::LOCAL_VARIABLE:
				case VoyScriptParser::IdentifierNode::LOCAL_CONSTANT:
				case VoyScriptParser::IdentifierNode::LOCAL_ITERATOR:
				case VoyScriptParser::IdentifierNode::LOCAL_BIND: {
					// Try function parameters.
					if (codegen.parameters.has(identifier)) {
						return codegen.parameters[identifier];
					}

					// Try local variables and constants.
					if (!p_initializer && codegen.locals.has(identifier)) {
						return codegen.locals[identifier];
					}
				} break;

				// MEMBERS.
				case VoyScriptParser::IdentifierNode::MEMBER_VARIABLE:
				case VoyScriptParser::IdentifierNode::MEMBER_FUNCTION:
				case VoyScriptParser::IdentifierNode::MEMBER_SIGNAL:
				case VoyScriptParser::IdentifierNode::INHERITED_VARIABLE: {
					// Try class members.
					if (_is_class_member_property(codegen, identifier)) {
						// Get property.
						VoyScriptCodeGenerator::Address temp = codegen.add_temporary(_gdtype_from_datatype(p_expression->get_datatype(), codegen.script));
						gen->write_get_member(temp, identifier);
						return temp;
					}

					// Try members.
					if (!codegen.function_node || !codegen.function_node->is_static) {
						// Try member variables.
						if (codegen.script->member_indices.has(identifier)) {
							if (codegen.script->member_indices[identifier].getter != StringName() && codegen.script->member_indices[identifier].getter != codegen.function_name) {
								// Perform getter.
								VoyScriptCodeGenerator::Address temp = codegen.add_temporary(codegen.script->member_indices[identifier].data_type);
								Vector<VoyScriptCodeGenerator::Address> args; // No argument needed.
								gen->write_call_self(temp, codegen.script->member_indices[identifier].getter, args);
								return temp;
							} else {
								// No getter or inside getter: direct member access.
								int idx = codegen.script->member_indices[identifier].index;
								return VoyScriptCodeGenerator::Address(VoyScriptCodeGenerator::Address::MEMBER, idx, codegen.script->get_member_type(identifier));
							}
						}
					}

					// Try methods and signals (can be Callable and Signal).
					{
						// Search upwards through parent classes:
						const VoyScriptParser::ClassNode *base_class = codegen.class_node;
						while (base_class != nullptr) {
							if (base_class->has_member(identifier)) {
								const VoyScriptParser::ClassNode::Member &member = base_class->get_member(identifier);
								if (member.type == VoyScriptParser::ClassNode::Member::FUNCTION || member.type == VoyScriptParser::ClassNode::Member::SIGNAL) {
									// Get like it was a property.
									VoyScriptCodeGenerator::Address temp = codegen.add_temporary(); // TODO: Get type here.

									VoyScriptCodeGenerator::Address base(VoyScriptCodeGenerator::Address::SELF);
									if (member.type == VoyScriptParser::ClassNode::Member::FUNCTION && member.function->is_static) {
										base = VoyScriptCodeGenerator::Address(VoyScriptCodeGenerator::Address::CLASS);
									}

									gen->write_get_named(temp, identifier, base);
									return temp;
								}
							}
							base_class = base_class->base_type.class_type;
						}

						// Try in native base.
						VoyScript *scr = codegen.script;
						VoyScriptNativeClass *nc = nullptr;
						while (scr) {
							if (scr->native.is_valid()) {
								nc = scr->native.ptr();
							}
							scr = scr->_base;
						}

						if (nc && (identifier == CoreStringName(free_) || ClassDB::has_signal(nc->get_name(), identifier) || ClassDB::has_method(nc->get_name(), identifier))) {
							// Get like it was a property.
							VoyScriptCodeGenerator::Address temp = codegen.add_temporary(); // TODO: Get type here.
							VoyScriptCodeGenerator::Address self(VoyScriptCodeGenerator::Address::SELF);

							gen->write_get_named(temp, identifier, self);
							return temp;
						}
					}
				} break;
				case VoyScriptParser::IdentifierNode::MEMBER_CONSTANT:
				case VoyScriptParser::IdentifierNode::MEMBER_CLASS: {
					// Try class constants.
					VoyScript *owner = codegen.script;
					while (owner) {
						VoyScript *scr = owner;
						VoyScriptNativeClass *nc = nullptr;

						while (scr) {
							if (scr->constants.has(identifier)) {
								return codegen.add_constant(scr->constants[identifier]); // TODO: Get type here.
							}
							if (scr->native.is_valid()) {
								nc = scr->native.ptr();
							}
							scr = scr->_base;
						}

						// Class C++ integer constant.
						if (nc) {
							bool success = false;
							int64_t constant = ClassDB::get_integer_constant(nc->get_name(), identifier, &success);
							if (success) {
								return codegen.add_constant(constant);
							}
						}

						owner = owner->_owner;
					}
				} break;
				case VoyScriptParser::IdentifierNode::STATIC_VARIABLE: {
					// Try static variables.
					VoyScript *scr = codegen.script;
					while (scr) {
						if (scr->static_variables_indices.has(identifier)) {
							if (scr->static_variables_indices[identifier].getter != StringName() && scr->static_variables_indices[identifier].getter != codegen.function_name) {
								// Perform getter.
								VoyScriptCodeGenerator::Address temp = codegen.add_temporary(scr->static_variables_indices[identifier].data_type);
								VoyScriptCodeGenerator::Address class_addr(VoyScriptCodeGenerator::Address::CLASS);
								Vector<VoyScriptCodeGenerator::Address> args; // No argument needed.
								gen->write_call(temp, class_addr, scr->static_variables_indices[identifier].getter, args);
								return temp;
							} else {
								// No getter or inside getter: direct variable access.
								VoyScriptCodeGenerator::Address temp = codegen.add_temporary(scr->static_variables_indices[identifier].data_type);
								VoyScriptCodeGenerator::Address _class = codegen.add_constant(scr);
								int index = scr->static_variables_indices[identifier].index;
								gen->write_get_static_variable(temp, _class, index);
								return temp;
							}
						}
						scr = scr->_base;
					}
				} break;

				// GLOBALS.
				case VoyScriptParser::IdentifierNode::UNDEFINED_SOURCE: {
					// Try globals.
					if (VoyScriptLanguage::get_singleton()->get_global_map().has(identifier)) {
						// If it's an autoload singleton, we postpone to load it at runtime.
						// This is so one autoload doesn't try to load another before it's compiled.
						HashMap<StringName, ProjectSettings::AutoloadInfo> autoloads = ProjectSettings::get_singleton()->get_autoload_list();
						if (autoloads.has(identifier) && autoloads[identifier].is_singleton) {
							VoyScriptCodeGenerator::Address global = codegen.add_temporary(_gdtype_from_datatype(in->get_datatype(), codegen.script));
							int idx = VoyScriptLanguage::get_singleton()->get_global_map()[identifier];
							gen->write_store_global(global, idx);
							return global;
						} else {
							int idx = VoyScriptLanguage::get_singleton()->get_global_map()[identifier];
							Variant global = VoyScriptLanguage::get_singleton()->get_global_array()[idx];
							return codegen.add_constant(global);
						}
					}

					// Try global classes.
					if (ScriptServer::is_global_class(identifier)) {
						const VoyScriptParser::ClassNode *class_node = codegen.class_node;
						while (class_node->outer) {
							class_node = class_node->outer;
						}

						Ref<Resource> res;

						if (class_node->identifier && class_node->identifier->name == identifier) {
							res = Ref<VoyScript>(main_script);
						} else {
							String global_class_path = ScriptServer::get_global_class_path(identifier);
							if (ResourceLoader::get_resource_type(global_class_path) == "VoyScript") {
								Error err = OK;
								// Should not need to pass p_owner since analyzer will already have done it.
								res = VoyScriptCache::get_shallow_script(global_class_path, err);
								if (err != OK) {
									_set_error("Can't load global class " + String(identifier), p_expression);
									r_error = ERR_COMPILATION_FAILED;
									return VoyScriptCodeGenerator::Address();
								}
							} else {
								res = ResourceLoader::load(global_class_path);
								if (res.is_null()) {
									_set_error("Can't load global class " + String(identifier) + ", cyclic reference?", p_expression);
									r_error = ERR_COMPILATION_FAILED;
									return VoyScriptCodeGenerator::Address();
								}
							}
						}

						return codegen.add_constant(res);
					}

#ifdef TOOLS_ENABLED
					if (VoyScriptLanguage::get_singleton()->get_named_globals_map().has(identifier)) {
						VoyScriptCodeGenerator::Address global = codegen.add_temporary(); // TODO: Get type.
						gen->write_store_named_global(global, identifier);
						return global;
					}
#endif

				} break;
			}

			// Not found, error.
			_set_error("Identifier not found: " + String(identifier), p_expression);
			r_error = ERR_COMPILATION_FAILED;
			return VoyScriptCodeGenerator::Address();
		} break;
		case VoyScriptParser::Node::LITERAL: {
			// Return constant.
			const VoyScriptParser::LiteralNode *cn = static_cast<const VoyScriptParser::LiteralNode *>(p_expression);

			return codegen.add_constant(cn->value);
		} break;
		case VoyScriptParser::Node::SELF: {
			//return constant
			if (codegen.function_node && codegen.function_node->is_static) {
				_set_error("'self' not present in static function.", p_expression);
				r_error = ERR_COMPILATION_FAILED;
				return VoyScriptCodeGenerator::Address();
			}
			return VoyScriptCodeGenerator::Address(VoyScriptCodeGenerator::Address::SELF);
		} break;
		case VoyScriptParser::Node::ARRAY: {
			const VoyScriptParser::ArrayNode *an = static_cast<const VoyScriptParser::ArrayNode *>(p_expression);
			Vector<VoyScriptCodeGenerator::Address> values;

			// Create the result temporary first since it's the last to be killed.
			VoyScriptDataType array_type = _gdtype_from_datatype(an->get_datatype(), codegen.script);
			VoyScriptCodeGenerator::Address result = codegen.add_temporary(array_type);

			for (int i = 0; i < an->elements.size(); i++) {
				VoyScriptCodeGenerator::Address val = _parse_expression(codegen, r_error, an->elements[i]);
				if (r_error) {
					return VoyScriptCodeGenerator::Address();
				}
				values.push_back(val);
			}

			if (array_type.has_container_element_type(0)) {
				gen->write_construct_typed_array(result, array_type.get_container_element_type(0), values);
			} else {
				gen->write_construct_array(result, values);
			}

			for (int i = 0; i < values.size(); i++) {
				if (values[i].mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
					gen->pop_temporary();
				}
			}

			return result;
		} break;
		case VoyScriptParser::Node::DICTIONARY: {
			const VoyScriptParser::DictionaryNode *dn = static_cast<const VoyScriptParser::DictionaryNode *>(p_expression);
			Vector<VoyScriptCodeGenerator::Address> elements;

			// Create the result temporary first since it's the last to be killed.
			VoyScriptDataType dict_type = _gdtype_from_datatype(dn->get_datatype(), codegen.script);
			VoyScriptCodeGenerator::Address result = codegen.add_temporary(dict_type);

			for (int i = 0; i < dn->elements.size(); i++) {
				// Key.
				VoyScriptCodeGenerator::Address element;
				switch (dn->style) {
					case VoyScriptParser::DictionaryNode::PYTHON_DICT:
						// Python-style: key is any expression.
						element = _parse_expression(codegen, r_error, dn->elements[i].key);
						if (r_error) {
							return VoyScriptCodeGenerator::Address();
						}
						break;
					case VoyScriptParser::DictionaryNode::LUA_TABLE:
						// Lua-style: key is an identifier interpreted as StringName.
						StringName key = dn->elements[i].key->reduced_value.operator StringName();
						element = codegen.add_constant(key);
						break;
				}

				elements.push_back(element);

				element = _parse_expression(codegen, r_error, dn->elements[i].value);
				if (r_error) {
					return VoyScriptCodeGenerator::Address();
				}

				elements.push_back(element);
			}

			if (dict_type.has_container_element_types()) {
				gen->write_construct_typed_dictionary(result, dict_type.get_container_element_type_or_variant(0), dict_type.get_container_element_type_or_variant(1), elements);
			} else {
				gen->write_construct_dictionary(result, elements);
			}

			for (int i = 0; i < elements.size(); i++) {
				if (elements[i].mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
					gen->pop_temporary();
				}
			}

			return result;
		} break;
		case VoyScriptParser::Node::CAST: {
			const VoyScriptParser::CastNode *cn = static_cast<const VoyScriptParser::CastNode *>(p_expression);
			VoyScriptDataType cast_type = _gdtype_from_datatype(cn->get_datatype(), codegen.script, false);

			VoyScriptCodeGenerator::Address result;
			if (cast_type.has_type) {
				// Create temporary for result first since it will be deleted last.
				result = codegen.add_temporary(cast_type);

				VoyScriptCodeGenerator::Address src = _parse_expression(codegen, r_error, cn->operand);

				gen->write_cast(result, src, cast_type);

				if (src.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
					gen->pop_temporary();
				}
			} else {
				result = _parse_expression(codegen, r_error, cn->operand);
			}

			return result;
		} break;
		case VoyScriptParser::Node::CALL: {
			const VoyScriptParser::CallNode *call = static_cast<const VoyScriptParser::CallNode *>(p_expression);
			bool is_awaited = p_expression == awaited_node;
			VoyScriptDataType type = _gdtype_from_datatype(call->get_datatype(), codegen.script);
			VoyScriptCodeGenerator::Address result;
			if (p_root) {
				result = VoyScriptCodeGenerator::Address(VoyScriptCodeGenerator::Address::NIL);
			} else {
				result = codegen.add_temporary(type);
			}

			Vector<VoyScriptCodeGenerator::Address> arguments;
			for (int i = 0; i < call->arguments.size(); i++) {
				VoyScriptCodeGenerator::Address arg = _parse_expression(codegen, r_error, call->arguments[i]);
				if (r_error) {
					return VoyScriptCodeGenerator::Address();
				}
				arguments.push_back(arg);
			}

			if (!call->is_super && call->callee->type == VoyScriptParser::Node::IDENTIFIER && VoyScriptParser::get_builtin_type(call->function_name) < Variant::VARIANT_MAX) {
				gen->write_construct(result, VoyScriptParser::get_builtin_type(call->function_name), arguments);
			} else if (!call->is_super && call->callee->type == VoyScriptParser::Node::IDENTIFIER && Variant::has_utility_function(call->function_name)) {
				// Variant utility function.
				gen->write_call_utility(result, call->function_name, arguments);
			} else if (!call->is_super && call->callee->type == VoyScriptParser::Node::IDENTIFIER && VoyScriptUtilityFunctions::function_exists(call->function_name)) {
				// VoyScript utility function.
				gen->write_call_voyscript_utility(result, call->function_name, arguments);
			} else {
				// Regular function.
				const VoyScriptParser::ExpressionNode *callee = call->callee;

				if (call->is_super) {
					// Super call.
					gen->write_super_call(result, call->function_name, arguments);
				} else {
					if (callee->type == VoyScriptParser::Node::IDENTIFIER) {
						// Self function call.
						if (ClassDB::has_method(codegen.script->native->get_name(), call->function_name)) {
							// Native method, use faster path.
							VoyScriptCodeGenerator::Address self;
							self.mode = VoyScriptCodeGenerator::Address::SELF;
							MethodBind *method = ClassDB::get_method(codegen.script->native->get_name(), call->function_name);

							if (_can_use_validate_call(method, arguments)) {
								// Exact arguments, use validated call.
								gen->write_call_method_bind_validated(result, self, method, arguments);
							} else {
								// Not exact arguments, but still can use method bind call.
								gen->write_call_method_bind(result, self, method, arguments);
							}
						} else if (call->is_static || codegen.is_static || (codegen.function_node && codegen.function_node->is_static) || call->function_name == "new") {
							VoyScriptCodeGenerator::Address self;
							self.mode = VoyScriptCodeGenerator::Address::CLASS;
							if (is_awaited) {
								gen->write_call_async(result, self, call->function_name, arguments);
							} else {
								gen->write_call(result, self, call->function_name, arguments);
							}
						} else {
							if (is_awaited) {
								gen->write_call_self_async(result, call->function_name, arguments);
							} else {
								gen->write_call_self(result, call->function_name, arguments);
							}
						}
					} else if (callee->type == VoyScriptParser::Node::SUBSCRIPT) {
						const VoyScriptParser::SubscriptNode *subscript = static_cast<const VoyScriptParser::SubscriptNode *>(call->callee);

						if (subscript->is_attribute) {
							// May be static built-in method call.
							if (!call->is_super && subscript->base->type == VoyScriptParser::Node::IDENTIFIER && VoyScriptParser::get_builtin_type(static_cast<VoyScriptParser::IdentifierNode *>(subscript->base)->name) < Variant::VARIANT_MAX) {
								gen->write_call_builtin_type_static(result, VoyScriptParser::get_builtin_type(static_cast<VoyScriptParser::IdentifierNode *>(subscript->base)->name), subscript->attribute->name, arguments);
							} else if (!call->is_super && subscript->base->type == VoyScriptParser::Node::IDENTIFIER && call->function_name != SNAME("new") &&
									ClassDB::class_exists(static_cast<VoyScriptParser::IdentifierNode *>(subscript->base)->name) && !Engine::get_singleton()->has_singleton(static_cast<VoyScriptParser::IdentifierNode *>(subscript->base)->name)) {
								// It's a static native method call.
								StringName class_name = static_cast<VoyScriptParser::IdentifierNode *>(subscript->base)->name;
								MethodBind *method = ClassDB::get_method(class_name, subscript->attribute->name);
								if (_can_use_validate_call(method, arguments)) {
									// Exact arguments, use validated call.
									gen->write_call_native_static_validated(result, method, arguments);
								} else {
									// Not exact arguments, use regular static call
									gen->write_call_native_static(result, class_name, subscript->attribute->name, arguments);
								}
							} else {
								VoyScriptCodeGenerator::Address base = _parse_expression(codegen, r_error, subscript->base);
								if (r_error) {
									return VoyScriptCodeGenerator::Address();
								}
								if (is_awaited) {
									gen->write_call_async(result, base, call->function_name, arguments);
								} else if (base.type.has_type && base.type.kind != VoyScriptDataType::BUILTIN) {
									// Native method, use faster path.
									StringName class_name;
									if (base.type.kind == VoyScriptDataType::NATIVE) {
										class_name = base.type.native_type;
									} else {
										class_name = base.type.native_type == StringName() ? base.type.script_type->get_instance_base_type() : base.type.native_type;
									}
									if (ClassDB::class_exists(class_name) && ClassDB::has_method(class_name, call->function_name)) {
										MethodBind *method = ClassDB::get_method(class_name, call->function_name);
										if (_can_use_validate_call(method, arguments)) {
											// Exact arguments, use validated call.
											gen->write_call_method_bind_validated(result, base, method, arguments);
										} else {
											// Not exact arguments, but still can use method bind call.
											gen->write_call_method_bind(result, base, method, arguments);
										}
									} else {
										gen->write_call(result, base, call->function_name, arguments);
									}
								} else if (base.type.has_type && base.type.kind == VoyScriptDataType::BUILTIN) {
									gen->write_call_builtin_type(result, base, base.type.builtin_type, call->function_name, arguments);
								} else {
									gen->write_call(result, base, call->function_name, arguments);
								}
								if (base.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
									gen->pop_temporary();
								}
							}
						} else {
							_set_error("Cannot call something that isn't a function.", call->callee);
							r_error = ERR_COMPILATION_FAILED;
							return VoyScriptCodeGenerator::Address();
						}
					} else {
						_set_error("Compiler bug (please report): incorrect callee type in call node.", call->callee);
						r_error = ERR_COMPILATION_FAILED;
						return VoyScriptCodeGenerator::Address();
					}
				}
			}

			for (int i = 0; i < arguments.size(); i++) {
				if (arguments[i].mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
					gen->pop_temporary();
				}
			}
			return result;
		} break;
		case VoyScriptParser::Node::GET_NODE: {
			const VoyScriptParser::GetNodeNode *get_node = static_cast<const VoyScriptParser::GetNodeNode *>(p_expression);

			Vector<VoyScriptCodeGenerator::Address> args;
			args.push_back(codegen.add_constant(NodePath(get_node->full_path)));

			VoyScriptCodeGenerator::Address result = codegen.add_temporary(_gdtype_from_datatype(get_node->get_datatype(), codegen.script));

			MethodBind *get_node_method = ClassDB::get_method("Node", "get_node");
			gen->write_call_method_bind_validated(result, VoyScriptCodeGenerator::Address(VoyScriptCodeGenerator::Address::SELF), get_node_method, args);

			return result;
		} break;
		case VoyScriptParser::Node::PRELOAD: {
			const VoyScriptParser::PreloadNode *preload = static_cast<const VoyScriptParser::PreloadNode *>(p_expression);

			// Add resource as constant.
			return codegen.add_constant(preload->resource);
		} break;
		case VoyScriptParser::Node::AWAIT: {
			const VoyScriptParser::AwaitNode *await = static_cast<const VoyScriptParser::AwaitNode *>(p_expression);

			VoyScriptCodeGenerator::Address result = codegen.add_temporary(_gdtype_from_datatype(p_expression->get_datatype(), codegen.script));
			VoyScriptParser::ExpressionNode *previous_awaited_node = awaited_node;
			awaited_node = await->to_await;
			VoyScriptCodeGenerator::Address argument = _parse_expression(codegen, r_error, await->to_await);
			awaited_node = previous_awaited_node;
			if (r_error) {
				return VoyScriptCodeGenerator::Address();
			}

			gen->write_await(result, argument);

			if (argument.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
				gen->pop_temporary();
			}

			return result;
		} break;
		// Indexing operator.
		case VoyScriptParser::Node::SUBSCRIPT: {
			const VoyScriptParser::SubscriptNode *subscript = static_cast<const VoyScriptParser::SubscriptNode *>(p_expression);
			VoyScriptCodeGenerator::Address result = codegen.add_temporary(_gdtype_from_datatype(subscript->get_datatype(), codegen.script));

			VoyScriptCodeGenerator::Address base = _parse_expression(codegen, r_error, subscript->base);
			if (r_error) {
				return VoyScriptCodeGenerator::Address();
			}

			bool named = subscript->is_attribute;
			StringName name;
			VoyScriptCodeGenerator::Address index;
			if (subscript->is_attribute) {
				if (subscript->base->type == VoyScriptParser::Node::SELF && codegen.script) {
					VoyScriptParser::IdentifierNode *identifier = subscript->attribute;
					HashMap<StringName, VoyScript::MemberInfo>::Iterator MI = codegen.script->member_indices.find(identifier->name);

#ifdef DEBUG_ENABLED
					if (MI && MI->value.getter == codegen.function_name) {
						String n = identifier->name;
						_set_error("Must use '" + n + "' instead of 'self." + n + "' in getter.", identifier);
						r_error = ERR_COMPILATION_FAILED;
						return VoyScriptCodeGenerator::Address();
					}
#endif

					if (MI && MI->value.getter == "") {
						// Remove result temp as we don't need it.
						gen->pop_temporary();
						// Faster than indexing self (as if no self. had been used).
						return VoyScriptCodeGenerator::Address(VoyScriptCodeGenerator::Address::MEMBER, MI->value.index, _gdtype_from_datatype(subscript->get_datatype(), codegen.script));
					}
				}

				name = subscript->attribute->name;
				named = true;
			} else {
				if (subscript->index->is_constant && subscript->index->reduced_value.get_type() == Variant::STRING_NAME) {
					// Also, somehow, named (speed up anyway).
					name = subscript->index->reduced_value;
					named = true;
				} else {
					// Regular indexing.
					index = _parse_expression(codegen, r_error, subscript->index);
					if (r_error) {
						return VoyScriptCodeGenerator::Address();
					}
				}
			}

			if (named) {
				gen->write_get_named(result, name, base);
			} else {
				gen->write_get(result, index, base);
			}

			if (index.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
				gen->pop_temporary();
			}
			if (base.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
				gen->pop_temporary();
			}

			return result;
		} break;
		case VoyScriptParser::Node::UNARY_OPERATOR: {
			const VoyScriptParser::UnaryOpNode *unary = static_cast<const VoyScriptParser::UnaryOpNode *>(p_expression);

			VoyScriptCodeGenerator::Address result = codegen.add_temporary(_gdtype_from_datatype(unary->get_datatype(), codegen.script));

			VoyScriptCodeGenerator::Address operand = _parse_expression(codegen, r_error, unary->operand);
			if (r_error) {
				return VoyScriptCodeGenerator::Address();
			}

			gen->write_unary_operator(result, unary->variant_op, operand);

			if (operand.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
				gen->pop_temporary();
			}

			return result;
		}
		case VoyScriptParser::Node::BINARY_OPERATOR: {
			const VoyScriptParser::BinaryOpNode *binary = static_cast<const VoyScriptParser::BinaryOpNode *>(p_expression);

			VoyScriptCodeGenerator::Address result = codegen.add_temporary(_gdtype_from_datatype(binary->get_datatype(), codegen.script));

			switch (binary->operation) {
				case VoyScriptParser::BinaryOpNode::OP_LOGIC_AND: {
					// AND operator with early out on failure.
					VoyScriptCodeGenerator::Address left_operand = _parse_expression(codegen, r_error, binary->left_operand);
					gen->write_and_left_operand(left_operand);
					VoyScriptCodeGenerator::Address right_operand = _parse_expression(codegen, r_error, binary->right_operand);
					gen->write_and_right_operand(right_operand);

					gen->write_end_and(result);

					if (right_operand.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
						gen->pop_temporary();
					}
					if (left_operand.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
						gen->pop_temporary();
					}
				} break;
				case VoyScriptParser::BinaryOpNode::OP_LOGIC_OR: {
					// OR operator with early out on success.
					VoyScriptCodeGenerator::Address left_operand = _parse_expression(codegen, r_error, binary->left_operand);
					gen->write_or_left_operand(left_operand);
					VoyScriptCodeGenerator::Address right_operand = _parse_expression(codegen, r_error, binary->right_operand);
					gen->write_or_right_operand(right_operand);

					gen->write_end_or(result);

					if (right_operand.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
						gen->pop_temporary();
					}
					if (left_operand.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
						gen->pop_temporary();
					}
				} break;
				default: {
					VoyScriptCodeGenerator::Address left_operand = _parse_expression(codegen, r_error, binary->left_operand);
					VoyScriptCodeGenerator::Address right_operand = _parse_expression(codegen, r_error, binary->right_operand);

					gen->write_binary_operator(result, binary->variant_op, left_operand, right_operand);

					if (right_operand.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
						gen->pop_temporary();
					}
					if (left_operand.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
						gen->pop_temporary();
					}
				}
			}
			return result;
		} break;
		case VoyScriptParser::Node::TERNARY_OPERATOR: {
			// x IF a ELSE y operator with early out on failure.
			const VoyScriptParser::TernaryOpNode *ternary = static_cast<const VoyScriptParser::TernaryOpNode *>(p_expression);
			VoyScriptCodeGenerator::Address result = codegen.add_temporary(_gdtype_from_datatype(ternary->get_datatype(), codegen.script));

			gen->write_start_ternary(result);

			VoyScriptCodeGenerator::Address condition = _parse_expression(codegen, r_error, ternary->condition);
			if (r_error) {
				return VoyScriptCodeGenerator::Address();
			}
			gen->write_ternary_condition(condition);

			if (condition.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
				gen->pop_temporary();
			}

			VoyScriptCodeGenerator::Address true_expr = _parse_expression(codegen, r_error, ternary->true_expr);
			if (r_error) {
				return VoyScriptCodeGenerator::Address();
			}
			gen->write_ternary_true_expr(true_expr);
			if (true_expr.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
				gen->pop_temporary();
			}

			VoyScriptCodeGenerator::Address false_expr = _parse_expression(codegen, r_error, ternary->false_expr);
			if (r_error) {
				return VoyScriptCodeGenerator::Address();
			}
			gen->write_ternary_false_expr(false_expr);
			if (false_expr.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
				gen->pop_temporary();
			}

			gen->write_end_ternary();

			return result;
		} break;
		case VoyScriptParser::Node::TYPE_TEST: {
			const VoyScriptParser::TypeTestNode *type_test = static_cast<const VoyScriptParser::TypeTestNode *>(p_expression);
			VoyScriptCodeGenerator::Address result = codegen.add_temporary(_gdtype_from_datatype(type_test->get_datatype(), codegen.script));

			VoyScriptCodeGenerator::Address operand = _parse_expression(codegen, r_error, type_test->operand);
			VoyScriptDataType test_type = _gdtype_from_datatype(type_test->test_datatype, codegen.script, false);
			if (r_error) {
				return VoyScriptCodeGenerator::Address();
			}

			if (test_type.has_type) {
				gen->write_type_test(result, operand, test_type);
			} else {
				gen->write_assign_true(result);
			}

			if (operand.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
				gen->pop_temporary();
			}

			return result;
		} break;
		case VoyScriptParser::Node::ASSIGNMENT: {
			const VoyScriptParser::AssignmentNode *assignment = static_cast<const VoyScriptParser::AssignmentNode *>(p_expression);

			if (assignment->assignee->type == VoyScriptParser::Node::SUBSCRIPT) {
				// SET (chained) MODE!
				const VoyScriptParser::SubscriptNode *subscript = static_cast<VoyScriptParser::SubscriptNode *>(assignment->assignee);
#ifdef DEBUG_ENABLED
				if (subscript->is_attribute && subscript->base->type == VoyScriptParser::Node::SELF && codegen.script) {
					HashMap<StringName, VoyScript::MemberInfo>::Iterator MI = codegen.script->member_indices.find(subscript->attribute->name);
					if (MI && MI->value.setter == codegen.function_name) {
						String n = subscript->attribute->name;
						_set_error("Must use '" + n + "' instead of 'self." + n + "' in setter.", subscript);
						r_error = ERR_COMPILATION_FAILED;
						return VoyScriptCodeGenerator::Address();
					}
				}
#endif
				/* Find chain of sets */

				StringName assign_class_member_property;

				VoyScriptCodeGenerator::Address target_member_property;
				bool is_member_property = false;
				bool member_property_has_setter = false;
				bool member_property_is_in_setter = false;
				bool is_static = false;
				VoyScriptCodeGenerator::Address static_var_class;
				int static_var_index = 0;
				VoyScriptDataType static_var_data_type;
				StringName var_name;
				StringName member_property_setter_function;

				List<const VoyScriptParser::SubscriptNode *> chain;

				{
					// Create get/set chain.
					const VoyScriptParser::SubscriptNode *n = subscript;
					while (true) {
						chain.push_back(n);
						if (n->base->type != VoyScriptParser::Node::SUBSCRIPT) {
							// Check for a property.
							if (n->base->type == VoyScriptParser::Node::IDENTIFIER) {
								VoyScriptParser::IdentifierNode *identifier = static_cast<VoyScriptParser::IdentifierNode *>(n->base);
								var_name = identifier->name;
								if (_is_class_member_property(codegen, var_name)) {
									assign_class_member_property = var_name;
								} else if (!_is_local_or_parameter(codegen, var_name)) {
									if (codegen.script->member_indices.has(var_name)) {
										is_member_property = true;
										is_static = false;
										const VoyScript::MemberInfo &minfo = codegen.script->member_indices[var_name];
										member_property_setter_function = minfo.setter;
										member_property_has_setter = member_property_setter_function != StringName();
										member_property_is_in_setter = member_property_has_setter && member_property_setter_function == codegen.function_name;
										target_member_property.mode = VoyScriptCodeGenerator::Address::MEMBER;
										target_member_property.address = minfo.index;
										target_member_property.type = minfo.data_type;
									} else {
										// Try static variables.
										VoyScript *scr = codegen.script;
										while (scr) {
											if (scr->static_variables_indices.has(var_name)) {
												is_member_property = true;
												is_static = true;
												const VoyScript::MemberInfo &minfo = scr->static_variables_indices[var_name];
												member_property_setter_function = minfo.setter;
												member_property_has_setter = member_property_setter_function != StringName();
												member_property_is_in_setter = member_property_has_setter && member_property_setter_function == codegen.function_name;
												static_var_class = codegen.add_constant(scr);
												static_var_index = minfo.index;
												static_var_data_type = minfo.data_type;
												break;
											}
											scr = scr->_base;
										}
									}
								}
							}
							break;
						}
						n = static_cast<const VoyScriptParser::SubscriptNode *>(n->base);
					}
				}

				/* Chain of gets */

				// Get at (potential) root stack pos, so it can be returned.
				VoyScriptCodeGenerator::Address base = _parse_expression(codegen, r_error, chain.back()->get()->base);
				const bool base_known_type = base.type.has_type;
				const bool base_is_shared = Variant::is_type_shared(base.type.builtin_type);

				if (r_error) {
					return VoyScriptCodeGenerator::Address();
				}

				VoyScriptCodeGenerator::Address prev_base = base;

				// In case the base has a setter, don't use the address directly, as we want to call that setter.
				// So use a temp value instead and call the setter at the end.
				VoyScriptCodeGenerator::Address base_temp;
				if ((!base_known_type || !base_is_shared) && base.mode == VoyScriptCodeGenerator::Address::MEMBER && member_property_has_setter && !member_property_is_in_setter) {
					base_temp = codegen.add_temporary(base.type);
					gen->write_assign(base_temp, base);
					prev_base = base_temp;
				}

				struct ChainInfo {
					bool is_named = false;
					VoyScriptCodeGenerator::Address base;
					VoyScriptCodeGenerator::Address key;
					StringName name;
				};

				List<ChainInfo> set_chain;

				for (List<const VoyScriptParser::SubscriptNode *>::Element *E = chain.back(); E; E = E->prev()) {
					if (E == chain.front()) {
						// Skip the main subscript, since we'll assign to that.
						break;
					}
					const VoyScriptParser::SubscriptNode *subscript_elem = E->get();
					VoyScriptCodeGenerator::Address value = codegen.add_temporary(_gdtype_from_datatype(subscript_elem->get_datatype(), codegen.script));
					VoyScriptCodeGenerator::Address key;
					StringName name;

					if (subscript_elem->is_attribute) {
						name = subscript_elem->attribute->name;
						gen->write_get_named(value, name, prev_base);
					} else {
						key = _parse_expression(codegen, r_error, subscript_elem->index);
						if (r_error) {
							return VoyScriptCodeGenerator::Address();
						}
						gen->write_get(value, key, prev_base);
					}

					// Store base and key for setting it back later.
					set_chain.push_front({ subscript_elem->is_attribute, prev_base, key, name }); // Push to front to invert the list.
					prev_base = value;
				}

				// Get value to assign.
				VoyScriptCodeGenerator::Address assigned = _parse_expression(codegen, r_error, assignment->assigned_value);
				if (r_error) {
					return VoyScriptCodeGenerator::Address();
				}
				// Get the key if needed.
				VoyScriptCodeGenerator::Address key;
				StringName name;
				if (subscript->is_attribute) {
					name = subscript->attribute->name;
				} else {
					key = _parse_expression(codegen, r_error, subscript->index);
					if (r_error) {
						return VoyScriptCodeGenerator::Address();
					}
				}

				// Perform operator if any.
				if (assignment->operation != VoyScriptParser::AssignmentNode::OP_NONE) {
					VoyScriptCodeGenerator::Address op_result = codegen.add_temporary(_gdtype_from_datatype(assignment->get_datatype(), codegen.script));
					VoyScriptCodeGenerator::Address value = codegen.add_temporary(_gdtype_from_datatype(subscript->get_datatype(), codegen.script));
					if (subscript->is_attribute) {
						gen->write_get_named(value, name, prev_base);
					} else {
						gen->write_get(value, key, prev_base);
					}
					gen->write_binary_operator(op_result, assignment->variant_op, value, assigned);
					gen->pop_temporary();
					if (assigned.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
						gen->pop_temporary();
					}
					assigned = op_result;
				}

				// Perform assignment.
				if (subscript->is_attribute) {
					gen->write_set_named(prev_base, name, assigned);
				} else {
					gen->write_set(prev_base, key, assigned);
				}
				if (key.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
					gen->pop_temporary();
				}
				if (assigned.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
					gen->pop_temporary();
				}

				assigned = prev_base;

				// Set back the values into their bases.
				for (const ChainInfo &info : set_chain) {
					bool known_type = assigned.type.has_type;
					bool is_shared = Variant::is_type_shared(assigned.type.builtin_type);

					if (!known_type || !is_shared) {
						if (!known_type) {
							// Jump shared values since they are already updated in-place.
							gen->write_jump_if_shared(assigned);
						}
						if (!info.is_named) {
							gen->write_set(info.base, info.key, assigned);
						} else {
							gen->write_set_named(info.base, info.name, assigned);
						}
						if (!known_type) {
							gen->write_end_jump_if_shared();
						}
					}
					if (!info.is_named && info.key.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
						gen->pop_temporary();
					}
					if (assigned.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
						gen->pop_temporary();
					}
					assigned = info.base;
				}

				bool known_type = assigned.type.has_type;
				bool is_shared = Variant::is_type_shared(assigned.type.builtin_type);

				if (!known_type || !is_shared) {
					// If this is a class member property, also assign to it.
					// This allow things like: position.x += 2.0
					if (assign_class_member_property != StringName()) {
						if (!known_type) {
							gen->write_jump_if_shared(assigned);
						}
						gen->write_set_member(assigned, assign_class_member_property);
						if (!known_type) {
							gen->write_end_jump_if_shared();
						}
					} else if (is_member_property) {
						// Same as above but for script members.
						if (!known_type) {
							gen->write_jump_if_shared(assigned);
						}
						if (member_property_has_setter && !member_property_is_in_setter) {
							Vector<VoyScriptCodeGenerator::Address> args;
							args.push_back(assigned);
							VoyScriptCodeGenerator::Address call_base = is_static ? VoyScriptCodeGenerator::Address(VoyScriptCodeGenerator::Address::CLASS) : VoyScriptCodeGenerator::Address(VoyScriptCodeGenerator::Address::SELF);
							gen->write_call(VoyScriptCodeGenerator::Address(), call_base, member_property_setter_function, args);
						} else if (is_static) {
							VoyScriptCodeGenerator::Address temp = codegen.add_temporary(static_var_data_type);
							gen->write_assign(temp, assigned);
							gen->write_set_static_variable(temp, static_var_class, static_var_index);
							gen->pop_temporary();
						} else {
							gen->write_assign(target_member_property, assigned);
						}
						if (!known_type) {
							gen->write_end_jump_if_shared();
						}
					}
				} else if (base_temp.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
					if (!base_known_type) {
						gen->write_jump_if_shared(base);
					}
					// Save the temp value back to the base by calling its setter.
					gen->write_call(VoyScriptCodeGenerator::Address(), base, member_property_setter_function, { assigned });
					if (!base_known_type) {
						gen->write_end_jump_if_shared();
					}
				}

				if (assigned.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
					gen->pop_temporary();
				}
			} else if (assignment->assignee->type == VoyScriptParser::Node::IDENTIFIER && _is_class_member_property(codegen, static_cast<VoyScriptParser::IdentifierNode *>(assignment->assignee)->name)) {
				// Assignment to member property.
				VoyScriptCodeGenerator::Address assigned_value = _parse_expression(codegen, r_error, assignment->assigned_value);
				if (r_error) {
					return VoyScriptCodeGenerator::Address();
				}

				VoyScriptCodeGenerator::Address to_assign = assigned_value;
				bool has_operation = assignment->operation != VoyScriptParser::AssignmentNode::OP_NONE;

				StringName name = static_cast<VoyScriptParser::IdentifierNode *>(assignment->assignee)->name;

				if (has_operation) {
					VoyScriptCodeGenerator::Address op_result = codegen.add_temporary(_gdtype_from_datatype(assignment->get_datatype(), codegen.script));
					VoyScriptCodeGenerator::Address member = codegen.add_temporary(_gdtype_from_datatype(assignment->assignee->get_datatype(), codegen.script));
					gen->write_get_member(member, name);
					gen->write_binary_operator(op_result, assignment->variant_op, member, assigned_value);
					gen->pop_temporary(); // Pop member temp.
					to_assign = op_result;
				}

				gen->write_set_member(to_assign, name);

				if (to_assign.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
					gen->pop_temporary(); // Pop the assigned expression or the temp result if it has operation.
				}
				if (has_operation && assigned_value.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
					gen->pop_temporary(); // Pop the assigned expression if not done before.
				}
			} else {
				// Regular assignment.
				if (assignment->assignee->type != VoyScriptParser::Node::IDENTIFIER) {
					_set_error("Compiler bug (please report): Expected the assignee to be an identifier here.", assignment->assignee);
					r_error = ERR_COMPILATION_FAILED;
					return VoyScriptCodeGenerator::Address();
				}
				VoyScriptCodeGenerator::Address member;
				bool is_member = false;
				bool has_setter = false;
				bool is_in_setter = false;
				bool is_static = false;
				VoyScriptCodeGenerator::Address static_var_class;
				int static_var_index = 0;
				VoyScriptDataType static_var_data_type;
				StringName var_name;
				StringName setter_function;
				var_name = static_cast<const VoyScriptParser::IdentifierNode *>(assignment->assignee)->name;
				if (!_is_local_or_parameter(codegen, var_name)) {
					if (codegen.script->member_indices.has(var_name)) {
						is_member = true;
						is_static = false;
						VoyScript::MemberInfo &minfo = codegen.script->member_indices[var_name];
						setter_function = minfo.setter;
						has_setter = setter_function != StringName();
						is_in_setter = has_setter && setter_function == codegen.function_name;
						member.mode = VoyScriptCodeGenerator::Address::MEMBER;
						member.address = minfo.index;
						member.type = minfo.data_type;
					} else {
						// Try static variables.
						VoyScript *scr = codegen.script;
						while (scr) {
							if (scr->static_variables_indices.has(var_name)) {
								is_member = true;
								is_static = true;
								VoyScript::MemberInfo &minfo = scr->static_variables_indices[var_name];
								setter_function = minfo.setter;
								has_setter = setter_function != StringName();
								is_in_setter = has_setter && setter_function == codegen.function_name;
								static_var_class = codegen.add_constant(scr);
								static_var_index = minfo.index;
								static_var_data_type = minfo.data_type;
								break;
							}
							scr = scr->_base;
						}
					}
				}

				VoyScriptCodeGenerator::Address target;
				if (is_member) {
					target = member; // _parse_expression could call its getter, but we want to know the actual address
				} else {
					target = _parse_expression(codegen, r_error, assignment->assignee);
					if (r_error) {
						return VoyScriptCodeGenerator::Address();
					}
				}

				VoyScriptCodeGenerator::Address assigned_value = _parse_expression(codegen, r_error, assignment->assigned_value);
				if (r_error) {
					return VoyScriptCodeGenerator::Address();
				}

				VoyScriptCodeGenerator::Address to_assign;
				bool has_operation = assignment->operation != VoyScriptParser::AssignmentNode::OP_NONE;
				if (has_operation) {
					// Perform operation.
					VoyScriptCodeGenerator::Address op_result = codegen.add_temporary(_gdtype_from_datatype(assignment->get_datatype(), codegen.script));
					VoyScriptCodeGenerator::Address og_value = _parse_expression(codegen, r_error, assignment->assignee);
					gen->write_binary_operator(op_result, assignment->variant_op, og_value, assigned_value);
					to_assign = op_result;

					if (og_value.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
						gen->pop_temporary();
					}
				} else {
					to_assign = assigned_value;
				}

				if (has_setter && !is_in_setter) {
					// Call setter.
					Vector<VoyScriptCodeGenerator::Address> args;
					args.push_back(to_assign);
					VoyScriptCodeGenerator::Address call_base = is_static ? VoyScriptCodeGenerator::Address(VoyScriptCodeGenerator::Address::CLASS) : VoyScriptCodeGenerator::Address(VoyScriptCodeGenerator::Address::SELF);
					gen->write_call(VoyScriptCodeGenerator::Address(), call_base, setter_function, args);
				} else if (is_static) {
					VoyScriptCodeGenerator::Address temp = codegen.add_temporary(static_var_data_type);
					if (assignment->use_conversion_assign) {
						gen->write_assign_with_conversion(temp, to_assign);
					} else {
						gen->write_assign(temp, to_assign);
					}
					gen->write_set_static_variable(temp, static_var_class, static_var_index);
					gen->pop_temporary();
				} else {
					// Just assign.
					if (assignment->use_conversion_assign) {
						gen->write_assign_with_conversion(target, to_assign);
					} else {
						gen->write_assign(target, to_assign);
					}
				}

				if (to_assign.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
					gen->pop_temporary(); // Pop assigned value or temp operation result.
				}
				if (has_operation && assigned_value.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
					gen->pop_temporary(); // Pop assigned value if not done before.
				}
				if (target.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
					gen->pop_temporary(); // Pop the target to assignment.
				}
			}
			return VoyScriptCodeGenerator::Address(); // Assignment does not return a value.
		} break;
		case VoyScriptParser::Node::LAMBDA: {
			const VoyScriptParser::LambdaNode *lambda = static_cast<const VoyScriptParser::LambdaNode *>(p_expression);
			VoyScriptCodeGenerator::Address result = codegen.add_temporary(_gdtype_from_datatype(lambda->get_datatype(), codegen.script));

			Vector<VoyScriptCodeGenerator::Address> captures;
			captures.resize(lambda->captures.size());
			for (int i = 0; i < lambda->captures.size(); i++) {
				captures.write[i] = _parse_expression(codegen, r_error, lambda->captures[i]);
				if (r_error) {
					return VoyScriptCodeGenerator::Address();
				}
			}

			VoyScriptFunction *function = _parse_function(r_error, codegen.script, codegen.class_node, lambda->function, false, true);
			if (r_error) {
				return VoyScriptCodeGenerator::Address();
			}

			codegen.script->lambda_info.insert(function, { (int)lambda->captures.size(), lambda->use_self });
			gen->write_lambda(result, function, captures, lambda->use_self);

			for (int i = 0; i < captures.size(); i++) {
				if (captures[i].mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
					gen->pop_temporary();
				}
			}

			return result;
		} break;
		default: {
			_set_error("Compiler bug (please report): Unexpected node in parse tree while parsing expression.", p_expression); // Unreachable code.
			r_error = ERR_COMPILATION_FAILED;
			return VoyScriptCodeGenerator::Address();
		} break;
	}
}

VoyScriptCodeGenerator::Address VoyScriptCompiler::_parse_match_pattern(CodeGen &codegen, Error &r_error, const VoyScriptParser::PatternNode *p_pattern, const VoyScriptCodeGenerator::Address &p_value_addr, const VoyScriptCodeGenerator::Address &p_type_addr, const VoyScriptCodeGenerator::Address &p_previous_test, bool p_is_first, bool p_is_nested) {
	switch (p_pattern->pattern_type) {
		case VoyScriptParser::PatternNode::PT_LITERAL: {
			if (p_is_nested) {
				codegen.generator->write_and_left_operand(p_previous_test);
			} else if (!p_is_first) {
				codegen.generator->write_or_left_operand(p_previous_test);
			}

			// Get literal type into constant map.
			Variant::Type literal_type = p_pattern->literal->value.get_type();
			VoyScriptCodeGenerator::Address literal_type_addr = codegen.add_constant(literal_type);

			// Equality is always a boolean.
			VoyScriptDataType equality_type;
			equality_type.has_type = true;
			equality_type.kind = VoyScriptDataType::BUILTIN;
			equality_type.builtin_type = Variant::BOOL;

			// Check type equality.
			VoyScriptCodeGenerator::Address type_equality_addr = codegen.add_temporary(equality_type);
			codegen.generator->write_binary_operator(type_equality_addr, Variant::OP_EQUAL, p_type_addr, literal_type_addr);

			if (literal_type == Variant::STRING) {
				VoyScriptCodeGenerator::Address type_stringname_addr = codegen.add_constant(Variant::STRING_NAME);

				// Check StringName <-> String type equality.
				VoyScriptCodeGenerator::Address tmp_comp_addr = codegen.add_temporary(equality_type);

				codegen.generator->write_binary_operator(tmp_comp_addr, Variant::OP_EQUAL, p_type_addr, type_stringname_addr);
				codegen.generator->write_binary_operator(type_equality_addr, Variant::OP_OR, type_equality_addr, tmp_comp_addr);

				codegen.generator->pop_temporary(); // Remove tmp_comp_addr from stack.
			} else if (literal_type == Variant::STRING_NAME) {
				VoyScriptCodeGenerator::Address type_string_addr = codegen.add_constant(Variant::STRING);

				// Check String <-> StringName type equality.
				VoyScriptCodeGenerator::Address tmp_comp_addr = codegen.add_temporary(equality_type);

				codegen.generator->write_binary_operator(tmp_comp_addr, Variant::OP_EQUAL, p_type_addr, type_string_addr);
				codegen.generator->write_binary_operator(type_equality_addr, Variant::OP_OR, type_equality_addr, tmp_comp_addr);

				codegen.generator->pop_temporary(); // Remove tmp_comp_addr from stack.
			}

			codegen.generator->write_and_left_operand(type_equality_addr);

			// Get literal.
			VoyScriptCodeGenerator::Address literal_addr = _parse_expression(codegen, r_error, p_pattern->literal);
			if (r_error) {
				return VoyScriptCodeGenerator::Address();
			}

			// Check value equality.
			VoyScriptCodeGenerator::Address equality_addr = codegen.add_temporary(equality_type);
			codegen.generator->write_binary_operator(equality_addr, Variant::OP_EQUAL, p_value_addr, literal_addr);
			codegen.generator->write_and_right_operand(equality_addr);

			// AND both together (reuse temporary location).
			codegen.generator->write_end_and(type_equality_addr);

			codegen.generator->pop_temporary(); // Remove equality_addr from stack.

			if (literal_addr.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
				codegen.generator->pop_temporary();
			}

			// If this isn't the first, we need to OR with the previous pattern. If it's nested, we use AND instead.
			if (p_is_nested) {
				// Use the previous value as target, since we only need one temporary variable.
				codegen.generator->write_and_right_operand(type_equality_addr);
				codegen.generator->write_end_and(p_previous_test);
			} else if (!p_is_first) {
				// Use the previous value as target, since we only need one temporary variable.
				codegen.generator->write_or_right_operand(type_equality_addr);
				codegen.generator->write_end_or(p_previous_test);
			} else {
				// Just assign this value to the accumulator temporary.
				codegen.generator->write_assign(p_previous_test, type_equality_addr);
			}
			codegen.generator->pop_temporary(); // Remove type_equality_addr.

			return p_previous_test;
		} break;
		case VoyScriptParser::PatternNode::PT_EXPRESSION: {
			if (p_is_nested) {
				codegen.generator->write_and_left_operand(p_previous_test);
			} else if (!p_is_first) {
				codegen.generator->write_or_left_operand(p_previous_test);
			}

			VoyScriptCodeGenerator::Address type_string_addr = codegen.add_constant(Variant::STRING);
			VoyScriptCodeGenerator::Address type_stringname_addr = codegen.add_constant(Variant::STRING_NAME);

			// Equality is always a boolean.
			VoyScriptDataType equality_type;
			equality_type.has_type = true;
			equality_type.kind = VoyScriptDataType::BUILTIN;
			equality_type.builtin_type = Variant::BOOL;

			// Create the result temps first since it's the last to go away.
			VoyScriptCodeGenerator::Address result_addr = codegen.add_temporary(equality_type);
			VoyScriptCodeGenerator::Address equality_test_addr = codegen.add_temporary(equality_type);
			VoyScriptCodeGenerator::Address stringy_comp_addr = codegen.add_temporary(equality_type);
			VoyScriptCodeGenerator::Address stringy_comp_addr_2 = codegen.add_temporary(equality_type);
			VoyScriptCodeGenerator::Address expr_type_addr = codegen.add_temporary();

			// Evaluate expression.
			VoyScriptCodeGenerator::Address expr_addr;
			expr_addr = _parse_expression(codegen, r_error, p_pattern->expression);
			if (r_error) {
				return VoyScriptCodeGenerator::Address();
			}

			// Evaluate expression type.
			Vector<VoyScriptCodeGenerator::Address> typeof_args;
			typeof_args.push_back(expr_addr);
			codegen.generator->write_call_utility(expr_type_addr, "typeof", typeof_args);

			// Check type equality.
			codegen.generator->write_binary_operator(result_addr, Variant::OP_EQUAL, p_type_addr, expr_type_addr);

			// Check for String <-> StringName comparison.
			codegen.generator->write_binary_operator(stringy_comp_addr, Variant::OP_EQUAL, p_type_addr, type_string_addr);
			codegen.generator->write_binary_operator(stringy_comp_addr_2, Variant::OP_EQUAL, expr_type_addr, type_stringname_addr);
			codegen.generator->write_binary_operator(stringy_comp_addr, Variant::OP_AND, stringy_comp_addr, stringy_comp_addr_2);
			codegen.generator->write_binary_operator(result_addr, Variant::OP_OR, result_addr, stringy_comp_addr);

			// Check for StringName <-> String comparison.
			codegen.generator->write_binary_operator(stringy_comp_addr, Variant::OP_EQUAL, p_type_addr, type_stringname_addr);
			codegen.generator->write_binary_operator(stringy_comp_addr_2, Variant::OP_EQUAL, expr_type_addr, type_string_addr);
			codegen.generator->write_binary_operator(stringy_comp_addr, Variant::OP_AND, stringy_comp_addr, stringy_comp_addr_2);
			codegen.generator->write_binary_operator(result_addr, Variant::OP_OR, result_addr, stringy_comp_addr);

			codegen.generator->pop_temporary(); // Remove expr_type_addr from stack.
			codegen.generator->pop_temporary(); // Remove stringy_comp_addr_2 from stack.
			codegen.generator->pop_temporary(); // Remove stringy_comp_addr from stack.

			codegen.generator->write_and_left_operand(result_addr);

			// Check value equality.
			codegen.generator->write_binary_operator(equality_test_addr, Variant::OP_EQUAL, p_value_addr, expr_addr);
			codegen.generator->write_and_right_operand(equality_test_addr);

			// AND both type and value equality.
			codegen.generator->write_end_and(result_addr);

			// We don't need the expression temporary anymore.
			if (expr_addr.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
				codegen.generator->pop_temporary();
			}
			codegen.generator->pop_temporary(); // Remove equality_test_addr from stack.

			// If this isn't the first, we need to OR with the previous pattern. If it's nested, we use AND instead.
			if (p_is_nested) {
				// Use the previous value as target, since we only need one temporary variable.
				codegen.generator->write_and_right_operand(result_addr);
				codegen.generator->write_end_and(p_previous_test);
			} else if (!p_is_first) {
				// Use the previous value as target, since we only need one temporary variable.
				codegen.generator->write_or_right_operand(result_addr);
				codegen.generator->write_end_or(p_previous_test);
			} else {
				// Just assign this value to the accumulator temporary.
				codegen.generator->write_assign(p_previous_test, result_addr);
			}
			codegen.generator->pop_temporary(); // Remove temp result addr.

			return p_previous_test;
		} break;
		case VoyScriptParser::PatternNode::PT_ARRAY: {
			if (p_is_nested) {
				codegen.generator->write_and_left_operand(p_previous_test);
			} else if (!p_is_first) {
				codegen.generator->write_or_left_operand(p_previous_test);
			}
			// Get array type into constant map.
			VoyScriptCodeGenerator::Address array_type_addr = codegen.add_constant((int)Variant::ARRAY);

			// Equality is always a boolean.
			VoyScriptDataType temp_type;
			temp_type.has_type = true;
			temp_type.kind = VoyScriptDataType::BUILTIN;
			temp_type.builtin_type = Variant::BOOL;

			// Check type equality.
			VoyScriptCodeGenerator::Address result_addr = codegen.add_temporary(temp_type);
			codegen.generator->write_binary_operator(result_addr, Variant::OP_EQUAL, p_type_addr, array_type_addr);
			codegen.generator->write_and_left_operand(result_addr);

			// Store pattern length in constant map.
			VoyScriptCodeGenerator::Address array_length_addr = codegen.add_constant(p_pattern->rest_used ? p_pattern->array.size() - 1 : p_pattern->array.size());

			// Get value length.
			temp_type.builtin_type = Variant::INT;
			VoyScriptCodeGenerator::Address value_length_addr = codegen.add_temporary(temp_type);
			Vector<VoyScriptCodeGenerator::Address> len_args;
			len_args.push_back(p_value_addr);
			codegen.generator->write_call_voyscript_utility(value_length_addr, "len", len_args);

			// Test length compatibility.
			temp_type.builtin_type = Variant::BOOL;
			VoyScriptCodeGenerator::Address length_compat_addr = codegen.add_temporary(temp_type);
			codegen.generator->write_binary_operator(length_compat_addr, p_pattern->rest_used ? Variant::OP_GREATER_EQUAL : Variant::OP_EQUAL, value_length_addr, array_length_addr);
			codegen.generator->write_and_right_operand(length_compat_addr);

			// AND type and length check.
			codegen.generator->write_end_and(result_addr);

			// Remove length temporaries.
			codegen.generator->pop_temporary();
			codegen.generator->pop_temporary();

			// Create temporaries outside the loop so they can be reused.
			VoyScriptCodeGenerator::Address element_addr = codegen.add_temporary();
			VoyScriptCodeGenerator::Address element_type_addr = codegen.add_temporary();

			// Evaluate element by element.
			for (int i = 0; i < p_pattern->array.size(); i++) {
				if (p_pattern->array[i]->pattern_type == VoyScriptParser::PatternNode::PT_REST) {
					// Don't want to access an extra element of the user array.
					break;
				}

				// Use AND here too, as we don't want to be checking elements if previous test failed (which means this might be an invalid get).
				codegen.generator->write_and_left_operand(result_addr);

				// Add index to constant map.
				VoyScriptCodeGenerator::Address index_addr = codegen.add_constant(i);

				// Get the actual element from the user-sent array.
				codegen.generator->write_get(element_addr, index_addr, p_value_addr);

				// Also get type of element.
				Vector<VoyScriptCodeGenerator::Address> typeof_args;
				typeof_args.push_back(element_addr);
				codegen.generator->write_call_utility(element_type_addr, "typeof", typeof_args);

				// Try the pattern inside the element.
				result_addr = _parse_match_pattern(codegen, r_error, p_pattern->array[i], element_addr, element_type_addr, result_addr, false, true);
				if (r_error != OK) {
					return VoyScriptCodeGenerator::Address();
				}

				codegen.generator->write_and_right_operand(result_addr);
				codegen.generator->write_end_and(result_addr);
			}
			// Remove element temporaries.
			codegen.generator->pop_temporary();
			codegen.generator->pop_temporary();

			// If this isn't the first, we need to OR with the previous pattern. If it's nested, we use AND instead.
			if (p_is_nested) {
				// Use the previous value as target, since we only need one temporary variable.
				codegen.generator->write_and_right_operand(result_addr);
				codegen.generator->write_end_and(p_previous_test);
			} else if (!p_is_first) {
				// Use the previous value as target, since we only need one temporary variable.
				codegen.generator->write_or_right_operand(result_addr);
				codegen.generator->write_end_or(p_previous_test);
			} else {
				// Just assign this value to the accumulator temporary.
				codegen.generator->write_assign(p_previous_test, result_addr);
			}
			codegen.generator->pop_temporary(); // Remove temp result addr.

			return p_previous_test;
		} break;
		case VoyScriptParser::PatternNode::PT_DICTIONARY: {
			if (p_is_nested) {
				codegen.generator->write_and_left_operand(p_previous_test);
			} else if (!p_is_first) {
				codegen.generator->write_or_left_operand(p_previous_test);
			}
			// Get dictionary type into constant map.
			VoyScriptCodeGenerator::Address dict_type_addr = codegen.add_constant((int)Variant::DICTIONARY);

			// Equality is always a boolean.
			VoyScriptDataType temp_type;
			temp_type.has_type = true;
			temp_type.kind = VoyScriptDataType::BUILTIN;
			temp_type.builtin_type = Variant::BOOL;

			// Check type equality.
			VoyScriptCodeGenerator::Address result_addr = codegen.add_temporary(temp_type);
			codegen.generator->write_binary_operator(result_addr, Variant::OP_EQUAL, p_type_addr, dict_type_addr);
			codegen.generator->write_and_left_operand(result_addr);

			// Store pattern length in constant map.
			VoyScriptCodeGenerator::Address dict_length_addr = codegen.add_constant(p_pattern->rest_used ? p_pattern->dictionary.size() - 1 : p_pattern->dictionary.size());

			// Get user's dictionary length.
			temp_type.builtin_type = Variant::INT;
			VoyScriptCodeGenerator::Address value_length_addr = codegen.add_temporary(temp_type);
			Vector<VoyScriptCodeGenerator::Address> func_args;
			func_args.push_back(p_value_addr);
			codegen.generator->write_call_voyscript_utility(value_length_addr, "len", func_args);

			// Test length compatibility.
			temp_type.builtin_type = Variant::BOOL;
			VoyScriptCodeGenerator::Address length_compat_addr = codegen.add_temporary(temp_type);
			codegen.generator->write_binary_operator(length_compat_addr, p_pattern->rest_used ? Variant::OP_GREATER_EQUAL : Variant::OP_EQUAL, value_length_addr, dict_length_addr);
			codegen.generator->write_and_right_operand(length_compat_addr);

			// AND type and length check.
			codegen.generator->write_end_and(result_addr);

			// Remove length temporaries.
			codegen.generator->pop_temporary();
			codegen.generator->pop_temporary();

			// Create temporaries outside the loop so they can be reused.
			VoyScriptCodeGenerator::Address element_addr = codegen.add_temporary();
			VoyScriptCodeGenerator::Address element_type_addr = codegen.add_temporary();

			// Evaluate element by element.
			for (int i = 0; i < p_pattern->dictionary.size(); i++) {
				const VoyScriptParser::PatternNode::Pair &element = p_pattern->dictionary[i];
				if (element.value_pattern && element.value_pattern->pattern_type == VoyScriptParser::PatternNode::PT_REST) {
					// Ignore rest pattern.
					break;
				}

				// Use AND here too, as we don't want to be checking elements if previous test failed (which means this might be an invalid get).
				codegen.generator->write_and_left_operand(result_addr);

				// Get the pattern key.
				VoyScriptCodeGenerator::Address pattern_key_addr = _parse_expression(codegen, r_error, element.key);
				if (r_error) {
					return VoyScriptCodeGenerator::Address();
				}

				// Check if pattern key exists in user's dictionary. This will be AND-ed with next result.
				func_args.clear();
				func_args.push_back(pattern_key_addr);
				codegen.generator->write_call(result_addr, p_value_addr, "has", func_args);

				if (element.value_pattern != nullptr) {
					// Use AND here too, as we don't want to be checking elements if previous test failed (which means this might be an invalid get).
					codegen.generator->write_and_left_operand(result_addr);

					// Get actual value from user dictionary.
					codegen.generator->write_get(element_addr, pattern_key_addr, p_value_addr);

					// Also get type of value.
					func_args.clear();
					func_args.push_back(element_addr);
					codegen.generator->write_call_utility(element_type_addr, "typeof", func_args);

					// Try the pattern inside the value.
					result_addr = _parse_match_pattern(codegen, r_error, element.value_pattern, element_addr, element_type_addr, result_addr, false, true);
					if (r_error != OK) {
						return VoyScriptCodeGenerator::Address();
					}
					codegen.generator->write_and_right_operand(result_addr);
					codegen.generator->write_end_and(result_addr);
				}

				codegen.generator->write_and_right_operand(result_addr);
				codegen.generator->write_end_and(result_addr);

				// Remove pattern key temporary.
				if (pattern_key_addr.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
					codegen.generator->pop_temporary();
				}
			}

			// Remove element temporaries.
			codegen.generator->pop_temporary();
			codegen.generator->pop_temporary();

			// If this isn't the first, we need to OR with the previous pattern. If it's nested, we use AND instead.
			if (p_is_nested) {
				// Use the previous value as target, since we only need one temporary variable.
				codegen.generator->write_and_right_operand(result_addr);
				codegen.generator->write_end_and(p_previous_test);
			} else if (!p_is_first) {
				// Use the previous value as target, since we only need one temporary variable.
				codegen.generator->write_or_right_operand(result_addr);
				codegen.generator->write_end_or(p_previous_test);
			} else {
				// Just assign this value to the accumulator temporary.
				codegen.generator->write_assign(p_previous_test, result_addr);
			}
			codegen.generator->pop_temporary(); // Remove temp result addr.

			return p_previous_test;
		} break;
		case VoyScriptParser::PatternNode::PT_REST:
			// Do nothing.
			return p_previous_test;
			break;
		case VoyScriptParser::PatternNode::PT_BIND: {
			if (p_is_nested) {
				codegen.generator->write_and_left_operand(p_previous_test);
			} else if (!p_is_first) {
				codegen.generator->write_or_left_operand(p_previous_test);
			}
			// Get the bind address.
			VoyScriptCodeGenerator::Address bind = codegen.locals[p_pattern->bind->name];

			// Assign value to bound variable.
			codegen.generator->write_assign(bind, p_value_addr);
		}
			[[fallthrough]]; // Act like matching anything too.
		case VoyScriptParser::PatternNode::PT_WILDCARD:
			// If this is a fall through we don't want to do this again.
			if (p_pattern->pattern_type != VoyScriptParser::PatternNode::PT_BIND) {
				if (p_is_nested) {
					codegen.generator->write_and_left_operand(p_previous_test);
				} else if (!p_is_first) {
					codegen.generator->write_or_left_operand(p_previous_test);
				}
			}
			// This matches anything so just do the same as `if(true)`.
			// If this isn't the first, we need to OR with the previous pattern. If it's nested, we use AND instead.
			if (p_is_nested) {
				// Use the operator with the `true` constant so it works as always matching.
				VoyScriptCodeGenerator::Address constant = codegen.add_constant(true);
				codegen.generator->write_and_right_operand(constant);
				codegen.generator->write_end_and(p_previous_test);
			} else if (!p_is_first) {
				// Use the operator with the `true` constant so it works as always matching.
				VoyScriptCodeGenerator::Address constant = codegen.add_constant(true);
				codegen.generator->write_or_right_operand(constant);
				codegen.generator->write_end_or(p_previous_test);
			} else {
				// Just assign this value to the accumulator temporary.
				codegen.generator->write_assign_true(p_previous_test);
			}
			return p_previous_test;
	}

	_set_error("Compiler bug (please report): Reaching the end of pattern compilation without matching a pattern.", p_pattern);
	r_error = ERR_COMPILATION_FAILED;
	return p_previous_test;
}

List<VoyScriptCodeGenerator::Address> VoyScriptCompiler::_add_block_locals(CodeGen &codegen, const VoyScriptParser::SuiteNode *p_block) {
	List<VoyScriptCodeGenerator::Address> addresses;
	for (int i = 0; i < p_block->locals.size(); i++) {
		if (p_block->locals[i].type == VoyScriptParser::SuiteNode::Local::PARAMETER || p_block->locals[i].type == VoyScriptParser::SuiteNode::Local::FOR_VARIABLE) {
			// Parameters are added directly from function and loop variables are declared explicitly.
			continue;
		}
		addresses.push_back(codegen.add_local(p_block->locals[i].name, _gdtype_from_datatype(p_block->locals[i].get_datatype(), codegen.script)));
	}
	return addresses;
}

// Avoid keeping in the stack long-lived references to objects, which may prevent `RefCounted` objects from being freed.
void VoyScriptCompiler::_clear_block_locals(CodeGen &codegen, const List<VoyScriptCodeGenerator::Address> &p_locals) {
	for (const VoyScriptCodeGenerator::Address &local : p_locals) {
		if (local.type.can_contain_object()) {
			codegen.generator->clear_address(local);
		}
	}
}

Error VoyScriptCompiler::_parse_block(CodeGen &codegen, const VoyScriptParser::SuiteNode *p_block, bool p_add_locals, bool p_clear_locals) {
	Error err = OK;
	VoyScriptCodeGenerator *gen = codegen.generator;
	List<VoyScriptCodeGenerator::Address> block_locals;

	gen->clear_temporaries();
	codegen.start_block();

	if (p_add_locals) {
		block_locals = _add_block_locals(codegen, p_block);
	}

	for (int i = 0; i < p_block->statements.size(); i++) {
		const VoyScriptParser::Node *s = p_block->statements[i];

#ifdef DEBUG_ENABLED
		// Add a newline before each statement, since the debugger needs those.
		gen->write_newline(s->start_line);
#endif

		switch (s->type) {
			case VoyScriptParser::Node::MATCH: {
				const VoyScriptParser::MatchNode *match = static_cast<const VoyScriptParser::MatchNode *>(s);

				codegen.start_block(); // Add an extra block, since @special locals belong to the match scope.

				// Evaluate the match expression.
				VoyScriptCodeGenerator::Address value = codegen.add_local("@match_value", _gdtype_from_datatype(match->test->get_datatype(), codegen.script));
				VoyScriptCodeGenerator::Address value_expr = _parse_expression(codegen, err, match->test);
				if (err) {
					return err;
				}

				// Assign to local.
				// TODO: This can be improved by passing the target to parse_expression().
				gen->write_assign(value, value_expr);

				if (value_expr.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
					codegen.generator->pop_temporary();
				}

				// Then, let's save the type of the value in the stack too, so we can reuse for later comparisons.
				VoyScriptDataType typeof_type;
				typeof_type.has_type = true;
				typeof_type.kind = VoyScriptDataType::BUILTIN;
				typeof_type.builtin_type = Variant::INT;
				VoyScriptCodeGenerator::Address type = codegen.add_local("@match_type", typeof_type);

				Vector<VoyScriptCodeGenerator::Address> typeof_args;
				typeof_args.push_back(value);
				gen->write_call_utility(type, "typeof", typeof_args);

				// Now we can actually start testing.
				// For each branch.
				for (int j = 0; j < match->branches.size(); j++) {
					if (j > 0) {
						// Use `else` to not check the next branch after matching.
						gen->write_else();
					}

					const VoyScriptParser::MatchBranchNode *branch = match->branches[j];

					codegen.start_block(); // Add an extra block, since binds belong to the match branch scope.

					// Add locals in block before patterns, so temporaries don't use the stack address for binds.
					List<VoyScriptCodeGenerator::Address> branch_locals = _add_block_locals(codegen, branch->block);

#ifdef DEBUG_ENABLED
					// Add a newline before each branch, since the debugger needs those.
					gen->write_newline(branch->start_line);
#endif
					// For each pattern in branch.
					VoyScriptCodeGenerator::Address pattern_result = codegen.add_temporary();
					for (int k = 0; k < branch->patterns.size(); k++) {
						pattern_result = _parse_match_pattern(codegen, err, branch->patterns[k], value, type, pattern_result, k == 0, false);
						if (err != OK) {
							return err;
						}
					}

					// If there's a guard, check its condition too.
					if (branch->guard_body != nullptr) {
						// Do this first so the guard does not run unless the pattern matched.
						gen->write_and_left_operand(pattern_result);

						// Don't actually use the block for the guard.
						// The binds are already in the locals and we don't want to clear the result of the guard condition before we check the actual match.
						VoyScriptCodeGenerator::Address guard_result = _parse_expression(codegen, err, static_cast<VoyScriptParser::ExpressionNode *>(branch->guard_body->statements[0]));
						if (err) {
							return err;
						}

						gen->write_and_right_operand(guard_result);
						gen->write_end_and(pattern_result);

						if (guard_result.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
							codegen.generator->pop_temporary();
						}
					}

					// Check if pattern did match.
					gen->write_if(pattern_result);

					// Remove the result from stack.
					gen->pop_temporary();

					// Parse the branch block.
					err = _parse_block(codegen, branch->block, false); // Don't add locals again.
					if (err) {
						return err;
					}

					_clear_block_locals(codegen, branch_locals);

					codegen.end_block(); // Get out of extra block for binds.
				}

				// End all nested `if`s.
				for (int j = 0; j < match->branches.size(); j++) {
					gen->write_endif();
				}

				codegen.end_block(); // Get out of extra block for match's @special locals.
			} break;
			case VoyScriptParser::Node::IF: {
				const VoyScriptParser::IfNode *if_n = static_cast<const VoyScriptParser::IfNode *>(s);
				VoyScriptCodeGenerator::Address condition = _parse_expression(codegen, err, if_n->condition);
				if (err) {
					return err;
				}

				gen->write_if(condition);

				if (condition.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
					codegen.generator->pop_temporary();
				}

				err = _parse_block(codegen, if_n->true_block);
				if (err) {
					return err;
				}

				if (if_n->false_block) {
					gen->write_else();

					err = _parse_block(codegen, if_n->false_block);
					if (err) {
						return err;
					}
				}

				gen->write_endif();
			} break;
			case VoyScriptParser::Node::FOR: {
				const VoyScriptParser::ForNode *for_n = static_cast<const VoyScriptParser::ForNode *>(s);

				// Add an extra block, since the iterator and @special locals belong to the loop scope.
				// Also we use custom logic to clear block locals.
				codegen.start_block();

				VoyScriptCodeGenerator::Address iterator = codegen.add_local(for_n->variable->name, _gdtype_from_datatype(for_n->variable->get_datatype(), codegen.script));

				gen->start_for(iterator.type, _gdtype_from_datatype(for_n->list->get_datatype(), codegen.script));

				VoyScriptCodeGenerator::Address list = _parse_expression(codegen, err, for_n->list);
				if (err) {
					return err;
				}

				gen->write_for_assignment(list);

				if (list.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
					codegen.generator->pop_temporary();
				}

				gen->write_for(iterator, for_n->use_conversion_assign);

				// Loop variables must be cleared even when `break`/`continue` is used.
				List<VoyScriptCodeGenerator::Address> loop_locals = _add_block_locals(codegen, for_n->loop);

				//_clear_block_locals(codegen, loop_locals); // Inside loop, before block - for `continue`. // TODO

				err = _parse_block(codegen, for_n->loop, false); // Don't add locals again.
				if (err) {
					return err;
				}

				gen->write_endfor();

				_clear_block_locals(codegen, loop_locals); // Outside loop, after block - for `break` and normal exit.

				codegen.end_block(); // Get out of extra block for loop iterator, @special locals, and custom locals clearing.
			} break;
			case VoyScriptParser::Node::WHILE: {
				const VoyScriptParser::WhileNode *while_n = static_cast<const VoyScriptParser::WhileNode *>(s);

				codegen.start_block(); // Add an extra block, since we use custom logic to clear block locals.

				gen->start_while_condition();

				VoyScriptCodeGenerator::Address condition = _parse_expression(codegen, err, while_n->condition);
				if (err) {
					return err;
				}

				gen->write_while(condition);

				if (condition.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
					codegen.generator->pop_temporary();
				}

				// Loop variables must be cleared even when `break`/`continue` is used.
				List<VoyScriptCodeGenerator::Address> loop_locals = _add_block_locals(codegen, while_n->loop);

				//_clear_block_locals(codegen, loop_locals); // Inside loop, before block - for `continue`. // TODO

				err = _parse_block(codegen, while_n->loop, false); // Don't add locals again.
				if (err) {
					return err;
				}

				gen->write_endwhile();

				_clear_block_locals(codegen, loop_locals); // Outside loop, after block - for `break` and normal exit.

				codegen.end_block(); // Get out of extra block for custom locals clearing.
			} break;
			case VoyScriptParser::Node::BREAK: {
				gen->write_break();
			} break;
			case VoyScriptParser::Node::CONTINUE: {
				gen->write_continue();
			} break;
			case VoyScriptParser::Node::RETURN: {
				const VoyScriptParser::ReturnNode *return_n = static_cast<const VoyScriptParser::ReturnNode *>(s);

				VoyScriptCodeGenerator::Address return_value;

				if (return_n->return_value != nullptr) {
					return_value = _parse_expression(codegen, err, return_n->return_value);
					if (err) {
						return err;
					}
				}

				if (return_n->void_return) {
					// Always return "null", even if the expression is a call to a void function.
					gen->write_return(codegen.add_constant(Variant()));
				} else {
					gen->write_return(return_value);
				}
				if (return_value.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
					codegen.generator->pop_temporary();
				}
			} break;
			case VoyScriptParser::Node::ASSERT: {
#ifdef DEBUG_ENABLED
				const VoyScriptParser::AssertNode *as = static_cast<const VoyScriptParser::AssertNode *>(s);

				VoyScriptCodeGenerator::Address condition = _parse_expression(codegen, err, as->condition);
				if (err) {
					return err;
				}

				VoyScriptCodeGenerator::Address message;

				if (as->message) {
					message = _parse_expression(codegen, err, as->message);
					if (err) {
						return err;
					}
				}
				gen->write_assert(condition, message);

				if (condition.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
					codegen.generator->pop_temporary();
				}
				if (message.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
					codegen.generator->pop_temporary();
				}
#endif
			} break;
			case VoyScriptParser::Node::BREAKPOINT: {
#ifdef DEBUG_ENABLED
				gen->write_breakpoint();
#endif
			} break;
			case VoyScriptParser::Node::VARIABLE: {
				const VoyScriptParser::VariableNode *lv = static_cast<const VoyScriptParser::VariableNode *>(s);
				// Should be already in stack when the block began.
				VoyScriptCodeGenerator::Address local = codegen.locals[lv->identifier->name];
				VoyScriptDataType local_type = _gdtype_from_datatype(lv->get_datatype(), codegen.script);

				bool initialized = false;
				if (lv->initializer != nullptr) {
					VoyScriptCodeGenerator::Address src_address = _parse_expression(codegen, err, lv->initializer);
					if (err) {
						return err;
					}
					if (lv->use_conversion_assign) {
						gen->write_assign_with_conversion(local, src_address);
					} else {
						gen->write_assign(local, src_address);
					}
					if (src_address.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
						codegen.generator->pop_temporary();
					}
					initialized = true;
				} else if ((local_type.has_type && local_type.kind == VoyScriptDataType::BUILTIN) || codegen.generator->is_local_dirty(local)) {
					// Initialize with default for the type. Built-in types must always be cleared (they cannot be `null`).
					// Objects and untyped variables are assigned to `null` only if the stack address has been reused and not cleared.
					codegen.generator->clear_address(local);
					initialized = true;
				}

				// Don't check `is_local_dirty()` since the variable must be assigned to `null` **on each iteration**.
				if (!initialized && p_block->is_in_loop) {
					codegen.generator->clear_address(local);
				}
			} break;
			case VoyScriptParser::Node::CONSTANT: {
				// Local constants.
				const VoyScriptParser::ConstantNode *lc = static_cast<const VoyScriptParser::ConstantNode *>(s);
				if (!lc->initializer->is_constant) {
					_set_error("Local constant must have a constant value as initializer.", lc->initializer);
					return ERR_PARSE_ERROR;
				}

				codegen.add_local_constant(lc->identifier->name, lc->initializer->reduced_value);
			} break;
			case VoyScriptParser::Node::PASS:
				// Nothing to do.
				break;
			default: {
				// Expression.
				if (s->is_expression()) {
					VoyScriptCodeGenerator::Address expr = _parse_expression(codegen, err, static_cast<const VoyScriptParser::ExpressionNode *>(s), true);
					if (err) {
						return err;
					}
					if (expr.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
						codegen.generator->pop_temporary();
					}
				} else {
					_set_error("Compiler bug (please report): unexpected node in parse tree while parsing statement.", s); // Unreachable code.
					return ERR_INVALID_DATA;
				}
			} break;
		}

		gen->clear_temporaries();
	}

	if (p_add_locals && p_clear_locals) {
		_clear_block_locals(codegen, block_locals);
	}

	codegen.end_block();
	return OK;
}

VoyScriptFunction *VoyScriptCompiler::_parse_function(Error &r_error, VoyScript *p_script, const VoyScriptParser::ClassNode *p_class, const VoyScriptParser::FunctionNode *p_func, bool p_for_ready, bool p_for_lambda) {
	r_error = OK;
	CodeGen codegen;
	codegen.generator = memnew(VoyScriptByteCodeGenerator);

	codegen.class_node = p_class;
	codegen.script = p_script;
	codegen.function_node = p_func;

	StringName func_name;
	bool is_static = false;
	Variant rpc_config;
	VoyScriptDataType return_type;
	return_type.has_type = true;
	return_type.kind = VoyScriptDataType::BUILTIN;
	return_type.builtin_type = Variant::NIL;

	if (p_func) {
		if (p_func->identifier) {
			func_name = p_func->identifier->name;
		} else {
			func_name = "<anonymous lambda>";
		}
		is_static = p_func->is_static;
		rpc_config = p_func->rpc_config;
		return_type = _gdtype_from_datatype(p_func->get_datatype(), p_script);
	} else {
		if (p_for_ready) {
			func_name = "@implicit_ready";
		} else {
			func_name = "@implicit_new";
		}
	}

	MethodInfo method_info;

	codegen.function_name = func_name;
	method_info.name = func_name;
	codegen.is_static = is_static;
	if (is_static) {
		method_info.flags |= METHOD_FLAG_STATIC;
	}
	codegen.generator->write_start(p_script, func_name, is_static, rpc_config, return_type);

	int optional_parameters = 0;

	if (p_func) {
		for (int i = 0; i < p_func->parameters.size(); i++) {
			const VoyScriptParser::ParameterNode *parameter = p_func->parameters[i];
			VoyScriptDataType par_type = _gdtype_from_datatype(parameter->get_datatype(), p_script);
			uint32_t par_addr = codegen.generator->add_parameter(parameter->identifier->name, parameter->initializer != nullptr, par_type);
			codegen.parameters[parameter->identifier->name] = VoyScriptCodeGenerator::Address(VoyScriptCodeGenerator::Address::FUNCTION_PARAMETER, par_addr, par_type);

			method_info.arguments.push_back(parameter->get_datatype().to_property_info(parameter->identifier->name));

			if (parameter->initializer != nullptr) {
				optional_parameters++;
			}
		}

		method_info.default_arguments.append_array(p_func->default_arg_values);
	}

	// Parse initializer if applies.
	bool is_implicit_initializer = !p_for_ready && !p_func && !p_for_lambda;
	bool is_initializer = p_func && !p_for_lambda && p_func->identifier->name == VoyScriptLanguage::get_singleton()->strings._init;
	bool is_implicit_ready = !p_func && p_for_ready;

	if (!p_for_lambda && is_implicit_initializer) {
		// Initialize the default values for typed variables before anything.
		// This avoids crashes if they are accessed with validated calls before being properly initialized.
		// It may happen with out-of-order access or with `@onready` variables.
		for (const VoyScriptParser::ClassNode::Member &member : p_class->members) {
			if (member.type != VoyScriptParser::ClassNode::Member::VARIABLE) {
				continue;
			}

			const VoyScriptParser::VariableNode *field = member.variable;
			if (field->is_static) {
				continue;
			}

			VoyScriptDataType field_type = _gdtype_from_datatype(field->get_datatype(), codegen.script);
			if (field_type.has_type) {
				codegen.generator->write_newline(field->start_line);

				VoyScriptCodeGenerator::Address dst_address(VoyScriptCodeGenerator::Address::MEMBER, codegen.script->member_indices[field->identifier->name].index, field_type);

				if (field_type.builtin_type == Variant::ARRAY && field_type.has_container_element_type(0)) {
					codegen.generator->write_construct_typed_array(dst_address, field_type.get_container_element_type(0), Vector<VoyScriptCodeGenerator::Address>());
				} else if (field_type.builtin_type == Variant::DICTIONARY && field_type.has_container_element_types()) {
					codegen.generator->write_construct_typed_dictionary(dst_address, field_type.get_container_element_type_or_variant(0),
							field_type.get_container_element_type_or_variant(1), Vector<VoyScriptCodeGenerator::Address>());
				} else if (field_type.kind == VoyScriptDataType::BUILTIN) {
					codegen.generator->write_construct(dst_address, field_type.builtin_type, Vector<VoyScriptCodeGenerator::Address>());
				}
				// The `else` branch is for objects, in such case we leave it as `null`.
			}
		}
	}

	if (!p_for_lambda && (is_implicit_initializer || is_implicit_ready)) {
		// Initialize class fields.
		for (int i = 0; i < p_class->members.size(); i++) {
			if (p_class->members[i].type != VoyScriptParser::ClassNode::Member::VARIABLE) {
				continue;
			}
			const VoyScriptParser::VariableNode *field = p_class->members[i].variable;
			if (field->is_static) {
				continue;
			}

			if (field->onready != is_implicit_ready) {
				// Only initialize in `@implicit_ready()`.
				continue;
			}

			if (field->initializer) {
				// Emit proper line change.
				codegen.generator->write_newline(field->initializer->start_line);

				VoyScriptCodeGenerator::Address src_address = _parse_expression(codegen, r_error, field->initializer, false, true);
				if (r_error) {
					memdelete(codegen.generator);
					return nullptr;
				}

				VoyScriptDataType field_type = _gdtype_from_datatype(field->get_datatype(), codegen.script);
				VoyScriptCodeGenerator::Address dst_address(VoyScriptCodeGenerator::Address::MEMBER, codegen.script->member_indices[field->identifier->name].index, field_type);

				if (field->use_conversion_assign) {
					codegen.generator->write_assign_with_conversion(dst_address, src_address);
				} else {
					codegen.generator->write_assign(dst_address, src_address);
				}
				if (src_address.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
					codegen.generator->pop_temporary();
				}
			}
		}
	}

	// Parse default argument code if applies.
	if (p_func) {
		if (optional_parameters > 0) {
			codegen.generator->start_parameters();
			for (int i = p_func->parameters.size() - optional_parameters; i < p_func->parameters.size(); i++) {
				const VoyScriptParser::ParameterNode *parameter = p_func->parameters[i];
				VoyScriptCodeGenerator::Address src_addr = _parse_expression(codegen, r_error, parameter->initializer);
				if (r_error) {
					memdelete(codegen.generator);
					return nullptr;
				}
				VoyScriptCodeGenerator::Address dst_addr = codegen.parameters[parameter->identifier->name];
				codegen.generator->write_assign_default_parameter(dst_addr, src_addr, parameter->use_conversion_assign);
				if (src_addr.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
					codegen.generator->pop_temporary();
				}
			}
			codegen.generator->end_parameters();
		}

		// No need to reset locals at the end of the function, the stack will be cleared anyway.
		r_error = _parse_block(codegen, p_func->body, true, false);
		if (r_error) {
			memdelete(codegen.generator);
			return nullptr;
		}
	}

#ifdef DEBUG_ENABLED
	if (EngineDebugger::is_active()) {
		String signature;
		// Path.
		if (!p_script->get_script_path().is_empty()) {
			signature += p_script->get_script_path();
		}
		// Location.
		if (p_func) {
			signature += "::" + itos(p_func->body->start_line);
		} else {
			signature += "::0";
		}

		// Function and class.

		if (p_class->identifier) {
			signature += "::" + String(p_class->identifier->name) + "." + String(func_name);
		} else {
			signature += "::" + String(func_name);
		}

		if (p_for_lambda) {
			signature += "(lambda)";
		}

		codegen.generator->set_signature(signature);
	}
#endif

	if (p_func) {
		codegen.generator->set_initial_line(p_func->start_line);
	} else {
		codegen.generator->set_initial_line(0);
	}

	VoyScriptFunction *voy_function = codegen.generator->write_end();

	if (is_initializer) {
		p_script->initializer = voy_function;
	} else if (is_implicit_initializer) {
		p_script->implicit_initializer = voy_function;
	} else if (is_implicit_ready) {
		p_script->implicit_ready = voy_function;
	}

	if (p_func) {
		// If no `return` statement, then return type is `void`, not `Variant`.
		if (p_func->body->has_return) {
			voy_function->return_type = _gdtype_from_datatype(p_func->get_datatype(), p_script);
			method_info.return_val = p_func->get_datatype().to_property_info(String());
		} else {
			voy_function->return_type = VoyScriptDataType();
			voy_function->return_type.has_type = true;
			voy_function->return_type.kind = VoyScriptDataType::BUILTIN;
			voy_function->return_type.builtin_type = Variant::NIL;
		}
	}

	voy_function->method_info = method_info;

	if (!is_implicit_initializer && !is_implicit_ready && !p_for_lambda) {
		p_script->member_functions[func_name] = voy_function;
	}

	memdelete(codegen.generator);

	return voy_function;
}

VoyScriptFunction *VoyScriptCompiler::_make_static_initializer(Error &r_error, VoyScript *p_script, const VoyScriptParser::ClassNode *p_class) {
	r_error = OK;
	CodeGen codegen;
	codegen.generator = memnew(VoyScriptByteCodeGenerator);

	codegen.class_node = p_class;
	codegen.script = p_script;

	StringName func_name = SNAME("@static_initializer");
	bool is_static = true;
	Variant rpc_config;
	VoyScriptDataType return_type;
	return_type.has_type = true;
	return_type.kind = VoyScriptDataType::BUILTIN;
	return_type.builtin_type = Variant::NIL;

	codegen.function_name = func_name;
	codegen.is_static = is_static;
	codegen.generator->write_start(p_script, func_name, is_static, rpc_config, return_type);

	// The static initializer is always called on the same class where the static variables are defined,
	// so the CLASS address (current class) can be used instead of `codegen.add_constant(p_script)`.
	VoyScriptCodeGenerator::Address class_addr(VoyScriptCodeGenerator::Address::CLASS);

	// Initialize the default values for typed variables before anything.
	// This avoids crashes if they are accessed with validated calls before being properly initialized.
	// It may happen with out-of-order access or with `@onready` variables.
	for (const VoyScriptParser::ClassNode::Member &member : p_class->members) {
		if (member.type != VoyScriptParser::ClassNode::Member::VARIABLE) {
			continue;
		}

		const VoyScriptParser::VariableNode *field = member.variable;
		if (!field->is_static) {
			continue;
		}

		VoyScriptDataType field_type = _gdtype_from_datatype(field->get_datatype(), codegen.script);
		if (field_type.has_type) {
			codegen.generator->write_newline(field->start_line);

			if (field_type.builtin_type == Variant::ARRAY && field_type.has_container_element_type(0)) {
				VoyScriptCodeGenerator::Address temp = codegen.add_temporary(field_type);
				codegen.generator->write_construct_typed_array(temp, field_type.get_container_element_type(0), Vector<VoyScriptCodeGenerator::Address>());
				codegen.generator->write_set_static_variable(temp, class_addr, p_script->static_variables_indices[field->identifier->name].index);
				codegen.generator->pop_temporary();
			} else if (field_type.builtin_type == Variant::DICTIONARY && field_type.has_container_element_types()) {
				VoyScriptCodeGenerator::Address temp = codegen.add_temporary(field_type);
				codegen.generator->write_construct_typed_dictionary(temp, field_type.get_container_element_type_or_variant(0),
						field_type.get_container_element_type_or_variant(1), Vector<VoyScriptCodeGenerator::Address>());
				codegen.generator->write_set_static_variable(temp, class_addr, p_script->static_variables_indices[field->identifier->name].index);
				codegen.generator->pop_temporary();
			} else if (field_type.kind == VoyScriptDataType::BUILTIN) {
				VoyScriptCodeGenerator::Address temp = codegen.add_temporary(field_type);
				codegen.generator->write_construct(temp, field_type.builtin_type, Vector<VoyScriptCodeGenerator::Address>());
				codegen.generator->write_set_static_variable(temp, class_addr, p_script->static_variables_indices[field->identifier->name].index);
				codegen.generator->pop_temporary();
			}
			// The `else` branch is for objects, in such case we leave it as `null`.
		}
	}

	for (int i = 0; i < p_class->members.size(); i++) {
		// Initialize static fields.
		if (p_class->members[i].type != VoyScriptParser::ClassNode::Member::VARIABLE) {
			continue;
		}
		const VoyScriptParser::VariableNode *field = p_class->members[i].variable;
		if (!field->is_static) {
			continue;
		}

		if (field->initializer) {
			// Emit proper line change.
			codegen.generator->write_newline(field->initializer->start_line);

			VoyScriptCodeGenerator::Address src_address = _parse_expression(codegen, r_error, field->initializer, false, true);
			if (r_error) {
				memdelete(codegen.generator);
				return nullptr;
			}

			VoyScriptDataType field_type = _gdtype_from_datatype(field->get_datatype(), codegen.script);
			VoyScriptCodeGenerator::Address temp = codegen.add_temporary(field_type);

			if (field->use_conversion_assign) {
				codegen.generator->write_assign_with_conversion(temp, src_address);
			} else {
				codegen.generator->write_assign(temp, src_address);
			}
			if (src_address.mode == VoyScriptCodeGenerator::Address::TEMPORARY) {
				codegen.generator->pop_temporary();
			}

			codegen.generator->write_set_static_variable(temp, class_addr, p_script->static_variables_indices[field->identifier->name].index);
			codegen.generator->pop_temporary();
		}
	}

	if (p_script->has_method(VoyScriptLanguage::get_singleton()->strings._static_init)) {
		codegen.generator->write_newline(p_class->start_line);
		codegen.generator->write_call(VoyScriptCodeGenerator::Address(), class_addr, VoyScriptLanguage::get_singleton()->strings._static_init, Vector<VoyScriptCodeGenerator::Address>());
	}

#ifdef DEBUG_ENABLED
	if (EngineDebugger::is_active()) {
		String signature;
		// Path.
		if (!p_script->get_script_path().is_empty()) {
			signature += p_script->get_script_path();
		}
		// Location.
		signature += "::0";

		// Function and class.

		if (p_class->identifier) {
			signature += "::" + String(p_class->identifier->name) + "." + String(func_name);
		} else {
			signature += "::" + String(func_name);
		}

		codegen.generator->set_signature(signature);
	}
#endif

	codegen.generator->set_initial_line(p_class->start_line);

	VoyScriptFunction *voy_function = codegen.generator->write_end();

	memdelete(codegen.generator);

	return voy_function;
}

Error VoyScriptCompiler::_parse_setter_getter(VoyScript *p_script, const VoyScriptParser::ClassNode *p_class, const VoyScriptParser::VariableNode *p_variable, bool p_is_setter) {
	Error err = OK;

	VoyScriptParser::FunctionNode *function;

	if (p_is_setter) {
		function = p_variable->setter;
	} else {
		function = p_variable->getter;
	}

	_parse_function(err, p_script, p_class, function);

	return err;
}

// Prepares given script, and inner class scripts, for compilation. It populates class members and
// initializes method RPC info for its base classes first, then for itself, then for inner classes.
// WARNING: This function cannot initiate compilation of other classes, or it will result in
// cyclic dependency issues.
Error VoyScriptCompiler::_prepare_compilation(VoyScript *p_script, const VoyScriptParser::ClassNode *p_class, bool p_keep_state) {
	if (parsed_classes.has(p_script)) {
		return OK;
	}

	if (parsing_classes.has(p_script)) {
		String class_name = p_class->identifier ? String(p_class->identifier->name) : p_class->fqcn;
		_set_error(vformat(R"(Cyclic class reference for "%s".)", class_name), p_class);
		return ERR_PARSE_ERROR;
	}

	parsing_classes.insert(p_script);

	p_script->clearing = true;

	p_script->native = Ref<VoyScriptNativeClass>();
	p_script->base = Ref<VoyScript>();
	p_script->_base = nullptr;
	p_script->members.clear();

	// This makes possible to clear script constants and member_functions without heap-use-after-free errors.
	HashMap<StringName, Variant> constants;
	for (const KeyValue<StringName, Variant> &E : p_script->constants) {
		constants.insert(E.key, E.value);
	}
	p_script->constants.clear();
	constants.clear();
	HashMap<StringName, VoyScriptFunction *> member_functions;
	for (const KeyValue<StringName, VoyScriptFunction *> &E : p_script->member_functions) {
		member_functions.insert(E.key, E.value);
	}
	p_script->member_functions.clear();
	for (const KeyValue<StringName, VoyScriptFunction *> &E : member_functions) {
		memdelete(E.value);
	}
	member_functions.clear();

	p_script->static_variables.clear();

	if (p_script->implicit_initializer) {
		memdelete(p_script->implicit_initializer);
	}
	if (p_script->implicit_ready) {
		memdelete(p_script->implicit_ready);
	}
	if (p_script->static_initializer) {
		memdelete(p_script->static_initializer);
	}

	p_script->member_functions.clear();
	p_script->member_indices.clear();
	p_script->static_variables_indices.clear();
	p_script->static_variables.clear();
	p_script->_signals.clear();
	p_script->initializer = nullptr;
	p_script->implicit_initializer = nullptr;
	p_script->implicit_ready = nullptr;
	p_script->static_initializer = nullptr;
	p_script->rpc_config.clear();
	p_script->lambda_info.clear();

	p_script->clearing = false;

	p_script->tool = parser->is_tool();

	if (p_script->local_name != StringName()) {
		if (ClassDB::class_exists(p_script->local_name) && ClassDB::is_class_exposed(p_script->local_name)) {
			_set_error(vformat(R"(The class "%s" shadows a native class)", p_script->local_name), p_class);
			return ERR_ALREADY_EXISTS;
		}
	}

	VoyScriptDataType base_type = _gdtype_from_datatype(p_class->base_type, p_script, false);

	if (base_type.native_type == StringName()) {
		_set_error(vformat(R"(Parser bug (please report): Empty native type in base class "%s")", p_script->path), p_class);
		return ERR_BUG;
	}

	int native_idx = VoyScriptLanguage::get_singleton()->get_global_map()[base_type.native_type];

	p_script->native = VoyScriptLanguage::get_singleton()->get_global_array()[native_idx];
	if (p_script->native.is_null()) {
		_set_error("Compiler bug (please report): script native type is null.", nullptr);
		return ERR_BUG;
	}

	// Inheritance
	switch (base_type.kind) {
		case VoyScriptDataType::NATIVE:
			// Nothing more to do.
			break;
		case VoyScriptDataType::VOYSCRIPT: {
			Ref<VoyScript> base = Ref<VoyScript>(base_type.script_type);
			if (base.is_null()) {
				_set_error("Compiler bug (please report): base script type is null.", nullptr);
				return ERR_BUG;
			}

			if (main_script->has_class(base.ptr())) {
				Error err = _prepare_compilation(base.ptr(), p_class->base_type.class_type, p_keep_state);
				if (err) {
					return err;
				}
			} else if (!base->is_valid()) {
				Error err = OK;
				Ref<VoyScript> base_root = VoyScriptCache::get_shallow_script(base->path, err, p_script->path);
				if (err) {
					_set_error(vformat(R"(Could not parse base class "%s" from "%s": %s)", base->fully_qualified_name, base->path, error_names[err]), nullptr);
					return err;
				}
				if (base_root.is_valid()) {
					base = Ref<VoyScript>(base_root->find_class(base->fully_qualified_name));
				}
				if (base.is_null()) {
					_set_error(vformat(R"(Could not find class "%s" in "%s".)", base->fully_qualified_name, base->path), nullptr);
					return ERR_COMPILATION_FAILED;
				}

				err = _prepare_compilation(base.ptr(), p_class->base_type.class_type, p_keep_state);
				if (err) {
					_set_error(vformat(R"(Could not populate class members of base class "%s" in "%s".)", base->fully_qualified_name, base->path), nullptr);
					return err;
				}
			}

			p_script->base = base;
			p_script->_base = base.ptr();
			p_script->member_indices = base->member_indices;
		} break;
		default: {
			_set_error("Parser bug (please report): invalid inheritance.", nullptr);
			return ERR_BUG;
		} break;
	}

	// Duplicate RPC information from base VoyScript
	// Base script isn't valid because it should not have been compiled yet, but the reference contains relevant info.
	if (base_type.kind == VoyScriptDataType::VOYSCRIPT && p_script->base.is_valid()) {
		p_script->rpc_config = p_script->base->rpc_config.duplicate();
	}

	for (int i = 0; i < p_class->members.size(); i++) {
		const VoyScriptParser::ClassNode::Member &member = p_class->members[i];
		switch (member.type) {
			case VoyScriptParser::ClassNode::Member::VARIABLE: {
				const VoyScriptParser::VariableNode *variable = member.variable;
				StringName name = variable->identifier->name;

				VoyScript::MemberInfo minfo;
				switch (variable->property) {
					case VoyScriptParser::VariableNode::PROP_NONE:
						break; // Nothing to do.
					case VoyScriptParser::VariableNode::PROP_SETGET:
						if (variable->setter_pointer != nullptr) {
							minfo.setter = variable->setter_pointer->name;
						}
						if (variable->getter_pointer != nullptr) {
							minfo.getter = variable->getter_pointer->name;
						}
						break;
					case VoyScriptParser::VariableNode::PROP_INLINE:
						if (variable->setter != nullptr) {
							minfo.setter = "@" + variable->identifier->name + "_setter";
						}
						if (variable->getter != nullptr) {
							minfo.getter = "@" + variable->identifier->name + "_getter";
						}
						break;
				}
				minfo.data_type = _gdtype_from_datatype(variable->get_datatype(), p_script);

				PropertyInfo prop_info = variable->get_datatype().to_property_info(name);
				PropertyInfo export_info = variable->export_info;

				if (variable->exported) {
					if (!minfo.data_type.has_type) {
						prop_info.type = export_info.type;
						prop_info.class_name = export_info.class_name;
					}
					prop_info.hint = export_info.hint;
					prop_info.hint_string = export_info.hint_string;
					prop_info.usage = export_info.usage;
				}
				prop_info.usage |= PROPERTY_USAGE_SCRIPT_VARIABLE;
				minfo.property_info = prop_info;

				if (variable->is_static) {
					minfo.index = p_script->static_variables_indices.size();
					p_script->static_variables_indices[name] = minfo;
				} else {
					minfo.index = p_script->member_indices.size();
					p_script->member_indices[name] = minfo;
					p_script->members.insert(name);
				}

#ifdef TOOLS_ENABLED
				if (variable->initializer != nullptr && variable->initializer->is_constant) {
					p_script->member_default_values[name] = variable->initializer->reduced_value;
					VoyScriptCompiler::convert_to_initializer_type(p_script->member_default_values[name], variable);
				} else {
					p_script->member_default_values.erase(name);
				}
#endif
			} break;

			case VoyScriptParser::ClassNode::Member::CONSTANT: {
				const VoyScriptParser::ConstantNode *constant = member.constant;
				StringName name = constant->identifier->name;

				p_script->constants.insert(name, constant->initializer->reduced_value);
			} break;

			case VoyScriptParser::ClassNode::Member::ENUM_VALUE: {
				const VoyScriptParser::EnumNode::Value &enum_value = member.enum_value;
				StringName name = enum_value.identifier->name;

				p_script->constants.insert(name, enum_value.value);
			} break;

			case VoyScriptParser::ClassNode::Member::SIGNAL: {
				const VoyScriptParser::SignalNode *signal = member.signal;
				StringName name = signal->identifier->name;

				p_script->_signals[name] = signal->method_info;
			} break;

			case VoyScriptParser::ClassNode::Member::ENUM: {
				const VoyScriptParser::EnumNode *enum_n = member.m_enum;
				StringName name = enum_n->identifier->name;

				p_script->constants.insert(name, enum_n->dictionary);
			} break;

			case VoyScriptParser::ClassNode::Member::GROUP: {
				const VoyScriptParser::AnnotationNode *annotation = member.annotation;
				// Avoid name conflict. See GH-78252.
				StringName name = vformat("@group_%d_%s", p_script->members.size(), annotation->export_info.name);

				// This is not a normal member, but we need this to keep indices in order.
				VoyScript::MemberInfo minfo;
				minfo.index = p_script->member_indices.size();

				PropertyInfo prop_info;
				prop_info.name = annotation->export_info.name;
				prop_info.usage = annotation->export_info.usage;
				prop_info.hint_string = annotation->export_info.hint_string;
				minfo.property_info = prop_info;

				p_script->member_indices[name] = minfo;
				p_script->members.insert(name);
			} break;

			case VoyScriptParser::ClassNode::Member::FUNCTION: {
				const VoyScriptParser::FunctionNode *function_n = member.function;

				Variant config = function_n->rpc_config;
				if (config.get_type() != Variant::NIL) {
					p_script->rpc_config[function_n->identifier->name] = config;
				}
			} break;
			default:
				break; // Nothing to do here.
		}
	}

	p_script->static_variables.resize(p_script->static_variables_indices.size());

	parsed_classes.insert(p_script);
	parsing_classes.erase(p_script);

	// Populate inner classes.
	for (int i = 0; i < p_class->members.size(); i++) {
		const VoyScriptParser::ClassNode::Member &member = p_class->members[i];
		if (member.type != member.CLASS) {
			continue;
		}
		const VoyScriptParser::ClassNode *inner_class = member.m_class;
		StringName name = inner_class->identifier->name;
		Ref<VoyScript> &subclass = p_script->subclasses[name];
		VoyScript *subclass_ptr = subclass.ptr();

		// Subclass might still be parsing, just skip it
		if (!parsing_classes.has(subclass_ptr)) {
			Error err = _prepare_compilation(subclass_ptr, inner_class, p_keep_state);
			if (err) {
				return err;
			}
		}

		p_script->constants.insert(name, subclass); //once parsed, goes to the list of constants
	}

	return OK;
}

Error VoyScriptCompiler::_compile_class(VoyScript *p_script, const VoyScriptParser::ClassNode *p_class, bool p_keep_state) {
	// Compile member functions, getters, and setters.
	for (int i = 0; i < p_class->members.size(); i++) {
		const VoyScriptParser::ClassNode::Member &member = p_class->members[i];
		if (member.type == member.FUNCTION) {
			const VoyScriptParser::FunctionNode *function = member.function;
			Error err = OK;
			_parse_function(err, p_script, p_class, function);
			if (err) {
				return err;
			}
		} else if (member.type == member.VARIABLE) {
			const VoyScriptParser::VariableNode *variable = member.variable;
			if (variable->property == VoyScriptParser::VariableNode::PROP_INLINE) {
				if (variable->setter != nullptr) {
					Error err = _parse_setter_getter(p_script, p_class, variable, true);
					if (err) {
						return err;
					}
				}
				if (variable->getter != nullptr) {
					Error err = _parse_setter_getter(p_script, p_class, variable, false);
					if (err) {
						return err;
					}
				}
			}
		}
	}

	{
		// Create `@implicit_new()` special function in any case.
		Error err = OK;
		_parse_function(err, p_script, p_class, nullptr);
		if (err) {
			return err;
		}
	}

	if (p_class->onready_used) {
		// Create `@implicit_ready()` special function.
		Error err = OK;
		_parse_function(err, p_script, p_class, nullptr, true);
		if (err) {
			return err;
		}
	}

	if (p_class->has_static_data) {
		Error err = OK;
		VoyScriptFunction *func = _make_static_initializer(err, p_script, p_class);
		p_script->static_initializer = func;
		if (err) {
			return err;
		}
	}

#ifdef DEBUG_ENABLED

	//validate instances if keeping state

	if (p_keep_state) {
		for (RBSet<Object *>::Element *E = p_script->instances.front(); E;) {
			RBSet<Object *>::Element *N = E->next();

			ScriptInstance *si = E->get()->get_script_instance();
			if (si->is_placeholder()) {
#ifdef TOOLS_ENABLED
				PlaceHolderScriptInstance *psi = static_cast<PlaceHolderScriptInstance *>(si);

				if (p_script->is_tool()) {
					//re-create as an instance
					p_script->placeholders.erase(psi); //remove placeholder

					VoyScriptInstance *instance = memnew(VoyScriptInstance);
					instance->base_ref_counted = Object::cast_to<RefCounted>(E->get());
					instance->members.resize(p_script->member_indices.size());
					instance->script = Ref<VoyScript>(p_script);
					instance->owner = E->get();

					//needed for hot reloading
					for (const KeyValue<StringName, VoyScript::MemberInfo> &F : p_script->member_indices) {
						instance->member_indices_cache[F.key] = F.value.index;
					}
					instance->owner->set_script_instance(instance);

					/* STEP 2, INITIALIZE AND CONSTRUCT */

					Callable::CallError ce;
					p_script->initializer->call(instance, nullptr, 0, ce);

					if (ce.error != Callable::CallError::CALL_OK) {
						//well, tough luck, not gonna do anything here
					}
				}
#endif // TOOLS_ENABLED
			} else {
				VoyScriptInstance *gi = static_cast<VoyScriptInstance *>(si);
				gi->reload_members();
			}

			E = N;
		}
	}
#endif //DEBUG_ENABLED

	has_static_data = p_class->has_static_data;

	for (int i = 0; i < p_class->members.size(); i++) {
		if (p_class->members[i].type != VoyScriptParser::ClassNode::Member::CLASS) {
			continue;
		}
		const VoyScriptParser::ClassNode *inner_class = p_class->members[i].m_class;
		StringName name = inner_class->identifier->name;
		VoyScript *subclass = p_script->subclasses[name].ptr();

		Error err = _compile_class(subclass, inner_class, p_keep_state);
		if (err) {
			return err;
		}

		has_static_data = has_static_data || inner_class->has_static_data;
	}

	p_script->_static_default_init();

	p_script->valid = true;
	return OK;
}

void VoyScriptCompiler::convert_to_initializer_type(Variant &p_variant, const VoyScriptParser::VariableNode *p_node) {
	// Set p_variant to the value of p_node's initializer, with the type of p_node's variable.
	VoyScriptParser::DataType member_t = p_node->datatype;
	VoyScriptParser::DataType init_t = p_node->initializer->datatype;
	if (member_t.is_hard_type() && init_t.is_hard_type() &&
			member_t.kind == VoyScriptParser::DataType::BUILTIN && init_t.kind == VoyScriptParser::DataType::BUILTIN) {
		if (Variant::can_convert_strict(init_t.builtin_type, member_t.builtin_type)) {
			const Variant *v = &p_node->initializer->reduced_value;
			Callable::CallError ce;
			Variant::construct(member_t.builtin_type, p_variant, &v, 1, ce);
		}
	}
}

void VoyScriptCompiler::make_scripts(VoyScript *p_script, const VoyScriptParser::ClassNode *p_class, bool p_keep_state) {
	p_script->fully_qualified_name = p_class->fqcn;
	p_script->local_name = p_class->identifier ? p_class->identifier->name : StringName();
	p_script->global_name = p_class->get_global_name();
	p_script->simplified_icon_path = p_class->simplified_icon_path;

	HashMap<StringName, Ref<VoyScript>> old_subclasses;

	if (p_keep_state) {
		old_subclasses = p_script->subclasses;
	}

	p_script->subclasses.clear();

	for (int i = 0; i < p_class->members.size(); i++) {
		if (p_class->members[i].type != VoyScriptParser::ClassNode::Member::CLASS) {
			continue;
		}
		const VoyScriptParser::ClassNode *inner_class = p_class->members[i].m_class;
		StringName name = inner_class->identifier->name;

		Ref<VoyScript> subclass;

		if (old_subclasses.has(name)) {
			subclass = old_subclasses[name];
		} else {
			subclass = VoyScriptLanguage::get_singleton()->get_orphan_subclass(inner_class->fqcn);
		}

		if (subclass.is_null()) {
			subclass.instantiate();
		}

		subclass->_owner = p_script;
		subclass->path = p_script->path;
		p_script->subclasses.insert(name, subclass);

		make_scripts(subclass.ptr(), inner_class, p_keep_state);
	}
}

VoyScriptCompiler::FunctionLambdaInfo VoyScriptCompiler::_get_function_replacement_info(VoyScriptFunction *p_func, int p_index, int p_depth, VoyScriptFunction *p_parent_func) {
	FunctionLambdaInfo info;
	info.function = p_func;
	info.parent = p_parent_func;
	info.script = p_func->get_script();
	info.name = p_func->get_name();
	info.line = p_func->_initial_line;
	info.index = p_index;
	info.depth = p_depth;
	info.capture_count = 0;
	info.use_self = false;
	info.arg_count = p_func->_argument_count;
	info.default_arg_count = p_func->_default_arg_count;
	info.sublambdas = _get_function_lambda_replacement_info(p_func, p_depth, p_parent_func);

	ERR_FAIL_NULL_V(info.script, info);
	VoyScript::LambdaInfo *extra_info = info.script->lambda_info.getptr(p_func);
	if (extra_info != nullptr) {
		info.capture_count = extra_info->capture_count;
		info.use_self = extra_info->use_self;
	} else {
		info.capture_count = 0;
		info.use_self = false;
	}

	return info;
}

Vector<VoyScriptCompiler::FunctionLambdaInfo> VoyScriptCompiler::_get_function_lambda_replacement_info(VoyScriptFunction *p_func, int p_depth, VoyScriptFunction *p_parent_func) {
	Vector<FunctionLambdaInfo> result;
	// Only scrape the lambdas inside p_func.
	for (int i = 0; i < p_func->lambdas.size(); ++i) {
		result.push_back(_get_function_replacement_info(p_func->lambdas[i], i, p_depth + 1, p_func));
	}
	return result;
}

VoyScriptCompiler::ScriptLambdaInfo VoyScriptCompiler::_get_script_lambda_replacement_info(VoyScript *p_script) {
	ScriptLambdaInfo info;

	if (p_script->implicit_initializer) {
		info.implicit_initializer_info = _get_function_lambda_replacement_info(p_script->implicit_initializer);
	}
	if (p_script->implicit_ready) {
		info.implicit_ready_info = _get_function_lambda_replacement_info(p_script->implicit_ready);
	}
	if (p_script->static_initializer) {
		info.static_initializer_info = _get_function_lambda_replacement_info(p_script->static_initializer);
	}

	for (const KeyValue<StringName, VoyScriptFunction *> &E : p_script->member_functions) {
		info.member_function_infos.insert(E.key, _get_function_lambda_replacement_info(E.value));
	}

	for (const KeyValue<StringName, Ref<VoyScript>> &KV : p_script->get_subclasses()) {
		info.subclass_info.insert(KV.key, _get_script_lambda_replacement_info(KV.value.ptr()));
	}

	return info;
}

bool VoyScriptCompiler::_do_function_infos_match(const FunctionLambdaInfo &p_old_info, const FunctionLambdaInfo *p_new_info) {
	if (p_new_info == nullptr) {
		return false;
	}

	if (p_new_info->capture_count != p_old_info.capture_count || p_new_info->use_self != p_old_info.use_self) {
		return false;
	}

	int old_required_arg_count = p_old_info.arg_count - p_old_info.default_arg_count;
	int new_required_arg_count = p_new_info->arg_count - p_new_info->default_arg_count;
	if (new_required_arg_count > old_required_arg_count || p_new_info->arg_count < old_required_arg_count) {
		return false;
	}

	return true;
}

void VoyScriptCompiler::_get_function_ptr_replacements(HashMap<VoyScriptFunction *, VoyScriptFunction *> &r_replacements, const FunctionLambdaInfo &p_old_info, const FunctionLambdaInfo *p_new_info) {
	ERR_FAIL_COND(r_replacements.has(p_old_info.function));
	if (!_do_function_infos_match(p_old_info, p_new_info)) {
		p_new_info = nullptr;
	}

	r_replacements.insert(p_old_info.function, p_new_info != nullptr ? p_new_info->function : nullptr);
	_get_function_ptr_replacements(r_replacements, p_old_info.sublambdas, p_new_info != nullptr ? &p_new_info->sublambdas : nullptr);
}

void VoyScriptCompiler::_get_function_ptr_replacements(HashMap<VoyScriptFunction *, VoyScriptFunction *> &r_replacements, const Vector<FunctionLambdaInfo> &p_old_infos, const Vector<FunctionLambdaInfo> *p_new_infos) {
	for (int i = 0; i < p_old_infos.size(); ++i) {
		const FunctionLambdaInfo &old_info = p_old_infos[i];
		const FunctionLambdaInfo *new_info = nullptr;
		if (p_new_infos != nullptr && p_new_infos->size() == p_old_infos.size()) {
			// For now only attempt if the size is the same.
			new_info = &p_new_infos->get(i);
		}
		_get_function_ptr_replacements(r_replacements, old_info, new_info);
	}
}

void VoyScriptCompiler::_get_function_ptr_replacements(HashMap<VoyScriptFunction *, VoyScriptFunction *> &r_replacements, const ScriptLambdaInfo &p_old_info, const ScriptLambdaInfo *p_new_info) {
	_get_function_ptr_replacements(r_replacements, p_old_info.implicit_initializer_info, p_new_info != nullptr ? &p_new_info->implicit_initializer_info : nullptr);
	_get_function_ptr_replacements(r_replacements, p_old_info.implicit_ready_info, p_new_info != nullptr ? &p_new_info->implicit_ready_info : nullptr);
	_get_function_ptr_replacements(r_replacements, p_old_info.static_initializer_info, p_new_info != nullptr ? &p_new_info->static_initializer_info : nullptr);

	for (const KeyValue<StringName, Vector<FunctionLambdaInfo>> &old_kv : p_old_info.member_function_infos) {
		_get_function_ptr_replacements(r_replacements, old_kv.value, p_new_info != nullptr ? p_new_info->member_function_infos.getptr(old_kv.key) : nullptr);
	}
	for (int i = 0; i < p_old_info.other_function_infos.size(); ++i) {
		const FunctionLambdaInfo &old_other_info = p_old_info.other_function_infos[i];
		const FunctionLambdaInfo *new_other_info = nullptr;
		if (p_new_info != nullptr && p_new_info->other_function_infos.size() == p_old_info.other_function_infos.size()) {
			// For now only attempt if the size is the same.
			new_other_info = &p_new_info->other_function_infos[i];
		}
		// Needs to be called on all old lambdas, even if there's no replacement.
		_get_function_ptr_replacements(r_replacements, old_other_info, new_other_info);
	}
	for (const KeyValue<StringName, ScriptLambdaInfo> &old_kv : p_old_info.subclass_info) {
		const ScriptLambdaInfo &old_subinfo = old_kv.value;
		const ScriptLambdaInfo *new_subinfo = p_new_info != nullptr ? p_new_info->subclass_info.getptr(old_kv.key) : nullptr;
		_get_function_ptr_replacements(r_replacements, old_subinfo, new_subinfo);
	}
}

Error VoyScriptCompiler::compile(const VoyScriptParser *p_parser, VoyScript *p_script, bool p_keep_state) {
	err_line = -1;
	err_column = -1;
	error = "";
	parser = p_parser;
	main_script = p_script;
	const VoyScriptParser::ClassNode *root = parser->get_tree();

	source = p_script->get_path();

	ScriptLambdaInfo old_lambda_info = _get_script_lambda_replacement_info(p_script);

	// Create scripts for subclasses beforehand so they can be referenced
	make_scripts(p_script, root, p_keep_state);

	main_script->_owner = nullptr;
	Error err = _prepare_compilation(main_script, parser->get_tree(), p_keep_state);

	if (err) {
		return err;
	}

	err = _compile_class(main_script, root, p_keep_state);
	if (err) {
		return err;
	}

	ScriptLambdaInfo new_lambda_info = _get_script_lambda_replacement_info(p_script);

	HashMap<VoyScriptFunction *, VoyScriptFunction *> func_ptr_replacements;
	_get_function_ptr_replacements(func_ptr_replacements, old_lambda_info, &new_lambda_info);
	main_script->_recurse_replace_function_ptrs(func_ptr_replacements);

	if (has_static_data && !root->annotated_static_unload) {
		VoyScriptCache::add_static_script(p_script);
	}

	err = VoyScriptCache::finish_compiling(main_script->path);
	if (err) {
		_set_error(R"(Failed to compile depended scripts.)", nullptr);
	}
	return err;
}

String VoyScriptCompiler::get_error() const {
	return error;
}

int VoyScriptCompiler::get_error_line() const {
	return err_line;
}

int VoyScriptCompiler::get_error_column() const {
	return err_column;
}

VoyScriptCompiler::VoyScriptCompiler() {
}
