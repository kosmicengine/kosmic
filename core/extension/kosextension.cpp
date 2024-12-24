/**************************************************************************/
/*  ksextension.cpp                                                       */
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

#include "ksextension.h"
#include "ksextension.compat.inc"

#include "core/config/project_settings.h"
#include "core/object/class_db.h"
#include "core/object/method_bind.h"
#include "ksextension_library_loader.h"
#include "ksextension_manager.h"

extern void ksextension_setup_interface();
extern KSExtensionInterfaceFunctionPtr ksextension_get_proc_address(const char *p_name);

typedef KSExtensionBool (*KSExtensionLegacyInitializationFunction)(void *p_interface, KSExtensionClassLibraryPtr p_library, KSExtensionInitialization *r_initialization);

String KSExtension::get_extension_list_config_file() {
	return ProjectSettings::get_singleton()->get_project_data_path().path_join("extension_list.cfg");
}

class KSExtensionMethodBind : public MethodBind {
	KSExtensionClassMethodCall call_func;
	KSExtensionClassMethodValidatedCall validated_call_func;
	KSExtensionClassMethodPtrCall ptrcall_func;
	void *method_userdata;
	bool vararg;
	uint32_t argument_count;
	PropertyInfo return_value_info;
	GodotTypeInfo::Metadata return_value_metadata;
	List<PropertyInfo> arguments_info;
	List<GodotTypeInfo::Metadata> arguments_metadata;

#ifdef TOOLS_ENABLED
	friend class KSExtension;

	StringName name;
	bool is_reloading = false;
	bool valid = true;
#endif

protected:
	virtual Variant::Type _gen_argument_type(int p_arg) const override {
		if (p_arg < 0) {
			return return_value_info.type;
		} else {
			return arguments_info.get(p_arg).type;
		}
	}
	virtual PropertyInfo _gen_argument_type_info(int p_arg) const override {
		if (p_arg < 0) {
			return return_value_info;
		} else {
			return arguments_info.get(p_arg);
		}
	}

public:
#ifdef TOOLS_ENABLED
	virtual bool is_valid() const override { return valid; }
#endif

#ifdef DEBUG_METHODS_ENABLED
	virtual GodotTypeInfo::Metadata get_argument_meta(int p_arg) const override {
		if (p_arg < 0) {
			return return_value_metadata;
		} else {
			return arguments_metadata.get(p_arg);
		}
	}
#endif

	virtual Variant call(Object *p_object, const Variant **p_args, int p_arg_count, Callable::CallError &r_error) const override {
#ifdef TOOLS_ENABLED
		ERR_FAIL_COND_V_MSG(!valid, Variant(), vformat("Cannot call invalid KSExtension method bind '%s'. It's probably cached - you may need to restart Kosmic.", name));
		ERR_FAIL_COND_V_MSG(p_object && p_object->is_extension_placeholder(), Variant(), vformat("Cannot call KSExtension method bind '%s' on placeholder instance.", name));
#endif
		Variant ret;
		KSExtensionClassInstancePtr extension_instance = is_static() ? nullptr : p_object->_get_extension_instance();
		KSExtensionCallError ce{ KSEXTENSION_CALL_OK, 0, 0 };
		call_func(method_userdata, extension_instance, reinterpret_cast<KSExtensionConstVariantPtr *>(p_args), p_arg_count, (KSExtensionVariantPtr)&ret, &ce);
		r_error.error = Callable::CallError::Error(ce.error);
		r_error.argument = ce.argument;
		r_error.expected = ce.expected;
		return ret;
	}
	virtual void validated_call(Object *p_object, const Variant **p_args, Variant *r_ret) const override {
#ifdef TOOLS_ENABLED
		ERR_FAIL_COND_MSG(!valid, vformat("Cannot call invalid KSExtension method bind '%s'. It's probably cached - you may need to restart Kosmic.", name));
		ERR_FAIL_COND_MSG(p_object && p_object->is_extension_placeholder(), vformat("Cannot call KSExtension method bind '%s' on placeholder instance.", name));
#endif
		ERR_FAIL_COND_MSG(vararg, "Vararg methods don't have validated call support. This is most likely an engine bug.");
		KSExtensionClassInstancePtr extension_instance = is_static() ? nullptr : p_object->_get_extension_instance();

		if (validated_call_func) {
			// This is added here, but it's unlikely to be provided by most extensions.
			validated_call_func(method_userdata, extension_instance, reinterpret_cast<KSExtensionConstVariantPtr *>(p_args), (KSExtensionVariantPtr)r_ret);
		} else {
			// If not provided, go via ptrcall, which is faster than resorting to regular call.
			const void **argptrs = (const void **)alloca(argument_count * sizeof(void *));
			for (uint32_t i = 0; i < argument_count; i++) {
				argptrs[i] = VariantInternal::get_opaque_pointer(p_args[i]);
			}

			void *ret_opaque = nullptr;
			if (r_ret) {
				VariantInternal::initialize(r_ret, return_value_info.type);
				ret_opaque = r_ret->get_type() == Variant::NIL ? r_ret : VariantInternal::get_opaque_pointer(r_ret);
			}

			ptrcall_func(method_userdata, extension_instance, reinterpret_cast<KSExtensionConstTypePtr *>(argptrs), (KSExtensionTypePtr)ret_opaque);

			if (r_ret && r_ret->get_type() == Variant::OBJECT) {
				VariantInternal::update_object_id(r_ret);
			}
		}
	}

	virtual void ptrcall(Object *p_object, const void **p_args, void *r_ret) const override {
#ifdef TOOLS_ENABLED
		ERR_FAIL_COND_MSG(!valid, vformat("Cannot call invalid KSExtension method bind '%s'. It's probably cached - you may need to restart Kosmic.", name));
		ERR_FAIL_COND_MSG(p_object && p_object->is_extension_placeholder(), vformat("Cannot call KSExtension method bind '%s' on placeholder instance.", name));
#endif
		ERR_FAIL_COND_MSG(vararg, "Vararg methods don't have ptrcall support. This is most likely an engine bug.");
		KSExtensionClassInstancePtr extension_instance = is_static() ? nullptr : p_object->_get_extension_instance();
		ptrcall_func(method_userdata, extension_instance, reinterpret_cast<KSExtensionConstTypePtr *>(p_args), (KSExtensionTypePtr)r_ret);
	}

	virtual bool is_vararg() const override {
		return vararg;
	}

#ifdef TOOLS_ENABLED
	bool try_update(const KSExtensionClassMethodInfo *p_method_info) {
		if (is_static() != (bool)(p_method_info->method_flags & KSEXTENSION_METHOD_FLAG_STATIC)) {
			return false;
		}

		if (vararg != (bool)(p_method_info->method_flags & KSEXTENSION_METHOD_FLAG_VARARG)) {
			return false;
		}

		if (has_return() != (bool)p_method_info->has_return_value) {
			return false;
		}

		if (has_return() && return_value_info.type != (Variant::Type)p_method_info->return_value_info->type) {
			return false;
		}

		if (argument_count != p_method_info->argument_count) {
			return false;
		}

		List<PropertyInfo>::ConstIterator itr = arguments_info.begin();
		for (uint32_t i = 0; i < p_method_info->argument_count; ++itr, ++i) {
			if (itr->type != (Variant::Type)p_method_info->arguments_info[i].type) {
				return false;
			}
		}

		update(p_method_info);
		return true;
	}
#endif

	void update(const KSExtensionClassMethodInfo *p_method_info) {
#ifdef TOOLS_ENABLED
		name = *reinterpret_cast<StringName *>(p_method_info->name);
#endif
		method_userdata = p_method_info->method_userdata;
		call_func = p_method_info->call_func;
		validated_call_func = nullptr;
		ptrcall_func = p_method_info->ptrcall_func;
		set_name(*reinterpret_cast<StringName *>(p_method_info->name));

		if (p_method_info->has_return_value) {
			return_value_info = PropertyInfo(*p_method_info->return_value_info);
			return_value_metadata = GodotTypeInfo::Metadata(p_method_info->return_value_metadata);
		}

		arguments_info.clear();
		arguments_metadata.clear();
		for (uint32_t i = 0; i < p_method_info->argument_count; i++) {
			arguments_info.push_back(PropertyInfo(p_method_info->arguments_info[i]));
			arguments_metadata.push_back(GodotTypeInfo::Metadata(p_method_info->arguments_metadata[i]));
		}

		set_hint_flags(p_method_info->method_flags);
		argument_count = p_method_info->argument_count;
		vararg = p_method_info->method_flags & KSEXTENSION_METHOD_FLAG_VARARG;
		_set_returns(p_method_info->has_return_value);
		_set_const(p_method_info->method_flags & KSEXTENSION_METHOD_FLAG_CONST);
		_set_static(p_method_info->method_flags & KSEXTENSION_METHOD_FLAG_STATIC);
#ifdef DEBUG_METHODS_ENABLED
		_generate_argument_types(p_method_info->argument_count);
#endif
		set_argument_count(p_method_info->argument_count);

		Vector<Variant> defargs;
		defargs.resize(p_method_info->default_argument_count);
		for (uint32_t i = 0; i < p_method_info->default_argument_count; i++) {
			defargs.write[i] = *static_cast<Variant *>(p_method_info->default_arguments[i]);
		}

		set_default_arguments(defargs);
	}

	explicit KSExtensionMethodBind(const KSExtensionClassMethodInfo *p_method_info) {
		update(p_method_info);
	}
};

#ifndef DISABLE_DEPRECATED
void KSExtension::_register_extension_class(KSExtensionClassLibraryPtr p_library, KSExtensionConstStringNamePtr p_class_name, KSExtensionConstStringNamePtr p_parent_class_name, const KSExtensionClassCreationInfo *p_extension_funcs) {
	const KSExtensionClassCreationInfo4 class_info4 = {
		p_extension_funcs->is_virtual, // KSExtensionBool is_virtual;
		p_extension_funcs->is_abstract, // KSExtensionBool is_abstract;
		true, // KSExtensionBool is_exposed;
		false, // KSExtensionBool is_runtime;
		nullptr, // KSExtensionConstStringPtr icon_path;
		p_extension_funcs->set_func, // KSExtensionClassSet set_func;
		p_extension_funcs->get_func, // KSExtensionClassGet get_func;
		p_extension_funcs->get_property_list_func, // KSExtensionClassGetPropertyList get_property_list_func;
		nullptr, // KSExtensionClassFreePropertyList2 free_property_list_func;
		p_extension_funcs->property_can_revert_func, // KSExtensionClassPropertyCanRevert property_can_revert_func;
		p_extension_funcs->property_get_revert_func, // KSExtensionClassPropertyGetRevert property_get_revert_func;
		nullptr, // KSExtensionClassValidateProperty validate_property_func;
		nullptr, // KSExtensionClassNotification2 notification_func;
		p_extension_funcs->to_string_func, // KSExtensionClassToString to_string_func;
		p_extension_funcs->reference_func, // KSExtensionClassReference reference_func;
		p_extension_funcs->unreference_func, // KSExtensionClassUnreference unreference_func;
		nullptr, // KSExtensionClassCreateInstance2 create_instance_func; /* this one is mandatory */
		p_extension_funcs->free_instance_func, // KSExtensionClassFreeInstance free_instance_func; /* this one is mandatory */
		nullptr, // KSExtensionClassRecreateInstance recreate_instance_func;
		p_extension_funcs->get_virtual_func, // KSExtensionClassGetVirtual get_virtual_func;
		nullptr, // KSExtensionClassGetVirtualCallData get_virtual_call_data_func;
		nullptr, // KSExtensionClassCallVirtualWithData call_virtual_func;
		p_extension_funcs->class_userdata, // void *class_userdata;
	};

	const ClassCreationDeprecatedInfo legacy = {
		p_extension_funcs->notification_func, // KSExtensionClassNotification notification_func;
		p_extension_funcs->free_property_list_func, // KSExtensionClassFreePropertyList free_property_list_func;
		p_extension_funcs->create_instance_func, // KSExtensionClassCreateInstance create_instance_func;
		p_extension_funcs->get_rid_func, // KSExtensionClassGetRID get_rid;
	};
	_register_extension_class_internal(p_library, p_class_name, p_parent_class_name, &class_info4, &legacy);
}

void KSExtension::_register_extension_class2(KSExtensionClassLibraryPtr p_library, KSExtensionConstStringNamePtr p_class_name, KSExtensionConstStringNamePtr p_parent_class_name, const KSExtensionClassCreationInfo2 *p_extension_funcs) {
	const KSExtensionClassCreationInfo4 class_info4 = {
		p_extension_funcs->is_virtual, // KSExtensionBool is_virtual;
		p_extension_funcs->is_abstract, // KSExtensionBool is_abstract;
		p_extension_funcs->is_exposed, // KSExtensionBool is_exposed;
		false, // KSExtensionBool is_runtime;
		nullptr, // KSExtensionConstStringPtr icon_path;
		p_extension_funcs->set_func, // KSExtensionClassSet set_func;
		p_extension_funcs->get_func, // KSExtensionClassGet get_func;
		p_extension_funcs->get_property_list_func, // KSExtensionClassGetPropertyList get_property_list_func;
		nullptr, // KSExtensionClassFreePropertyList2 free_property_list_func;
		p_extension_funcs->property_can_revert_func, // KSExtensionClassPropertyCanRevert property_can_revert_func;
		p_extension_funcs->property_get_revert_func, // KSExtensionClassPropertyGetRevert property_get_revert_func;
		p_extension_funcs->validate_property_func, // KSExtensionClassValidateProperty validate_property_func;
		p_extension_funcs->notification_func, // KSExtensionClassNotification2 notification_func;
		p_extension_funcs->to_string_func, // KSExtensionClassToString to_string_func;
		p_extension_funcs->reference_func, // KSExtensionClassReference reference_func;
		p_extension_funcs->unreference_func, // KSExtensionClassUnreference unreference_func;
		nullptr, // KSExtensionClassCreateInstance2 create_instance_func; /* this one is mandatory */
		p_extension_funcs->free_instance_func, // KSExtensionClassFreeInstance free_instance_func; /* this one is mandatory */
		p_extension_funcs->recreate_instance_func, // KSExtensionClassRecreateInstance recreate_instance_func;
		p_extension_funcs->get_virtual_func, // KSExtensionClassGetVirtual get_virtual_func;
		p_extension_funcs->get_virtual_call_data_func, // KSExtensionClassGetVirtualCallData get_virtual_call_data_func;
		p_extension_funcs->call_virtual_with_data_func, // KSExtensionClassCallVirtualWithData call_virtual_func;
		p_extension_funcs->class_userdata, // void *class_userdata;
	};

	const ClassCreationDeprecatedInfo legacy = {
		nullptr, // KSExtensionClassNotification notification_func;
		p_extension_funcs->free_property_list_func, // KSExtensionClassFreePropertyList free_property_list_func;
		p_extension_funcs->create_instance_func, // KSExtensionClassCreateInstance create_instance_func;
		p_extension_funcs->get_rid_func, // KSExtensionClassGetRID get_rid;
	};
	_register_extension_class_internal(p_library, p_class_name, p_parent_class_name, &class_info4, &legacy);
}

void KSExtension::_register_extension_class3(KSExtensionClassLibraryPtr p_library, KSExtensionConstStringNamePtr p_class_name, KSExtensionConstStringNamePtr p_parent_class_name, const KSExtensionClassCreationInfo3 *p_extension_funcs) {
	const KSExtensionClassCreationInfo4 class_info4 = {
		p_extension_funcs->is_virtual, // KSExtensionBool is_virtual;
		p_extension_funcs->is_abstract, // KSExtensionBool is_abstract;
		p_extension_funcs->is_exposed, // KSExtensionBool is_exposed;
		p_extension_funcs->is_runtime, // KSExtensionBool is_runtime;
		nullptr, // KSExtensionConstStringPtr icon_path;
		p_extension_funcs->set_func, // KSExtensionClassSet set_func;
		p_extension_funcs->get_func, // KSExtensionClassGet get_func;
		p_extension_funcs->get_property_list_func, // KSExtensionClassGetPropertyList get_property_list_func;
		p_extension_funcs->free_property_list_func, // KSExtensionClassFreePropertyList free_property_list_func;
		p_extension_funcs->property_can_revert_func, // KSExtensionClassPropertyCanRevert property_can_revert_func;
		p_extension_funcs->property_get_revert_func, // KSExtensionClassPropertyGetRevert property_get_revert_func;
		p_extension_funcs->validate_property_func, // KSExtensionClassValidateProperty validate_property_func;
		p_extension_funcs->notification_func, // KSExtensionClassNotification2 notification_func;
		p_extension_funcs->to_string_func, // KSExtensionClassToString to_string_func;
		p_extension_funcs->reference_func, // KSExtensionClassReference reference_func;
		p_extension_funcs->unreference_func, // KSExtensionClassUnreference unreference_func;
		nullptr, // KSExtensionClassCreateInstance2 create_instance_func; /* this one is mandatory */
		p_extension_funcs->free_instance_func, // KSExtensionClassFreeInstance free_instance_func; /* this one is mandatory */
		p_extension_funcs->recreate_instance_func, // KSExtensionClassRecreateInstance recreate_instance_func;
		p_extension_funcs->get_virtual_func, // KSExtensionClassGetVirtual get_virtual_func;
		p_extension_funcs->get_virtual_call_data_func, // KSExtensionClassGetVirtualCallData get_virtual_call_data_func;
		p_extension_funcs->call_virtual_with_data_func, // KSExtensionClassCallVirtualWithData call_virtual_func;
		p_extension_funcs->class_userdata, // void *class_userdata;
	};

	const ClassCreationDeprecatedInfo legacy = {
		nullptr, // KSExtensionClassNotification notification_func;
		nullptr, // KSExtensionClassFreePropertyList free_property_list_func;
		p_extension_funcs->create_instance_func, // KSExtensionClassCreateInstance2 create_instance_func;
		p_extension_funcs->get_rid_func, // KSExtensionClassGetRID get_rid;
	};
	_register_extension_class_internal(p_library, p_class_name, p_parent_class_name, &class_info4, &legacy);
}

#endif // DISABLE_DEPRECATED

void KSExtension::_register_extension_class4(KSExtensionClassLibraryPtr p_library, KSExtensionConstStringNamePtr p_class_name, KSExtensionConstStringNamePtr p_parent_class_name, const KSExtensionClassCreationInfo4 *p_extension_funcs) {
	_register_extension_class_internal(p_library, p_class_name, p_parent_class_name, p_extension_funcs);
}

void KSExtension::_register_extension_class_internal(KSExtensionClassLibraryPtr p_library, KSExtensionConstStringNamePtr p_class_name, KSExtensionConstStringNamePtr p_parent_class_name, const KSExtensionClassCreationInfo4 *p_extension_funcs, const ClassCreationDeprecatedInfo *p_deprecated_funcs) {
	KSExtension *self = reinterpret_cast<KSExtension *>(p_library);

	StringName class_name = *reinterpret_cast<const StringName *>(p_class_name);
	StringName parent_class_name = *reinterpret_cast<const StringName *>(p_parent_class_name);
	ERR_FAIL_COND_MSG(!String(class_name).is_valid_unicode_identifier(), vformat("Attempt to register extension class '%s', which is not a valid class identifier.", class_name));
	ERR_FAIL_COND_MSG(ClassDB::class_exists(class_name), vformat("Attempt to register extension class '%s', which appears to be already registered.", class_name));

	Extension *parent_extension = nullptr;

	if (self->extension_classes.has(parent_class_name)) {
		parent_extension = &self->extension_classes[parent_class_name];
	} else if (ClassDB::class_exists(parent_class_name)) {
		if (ClassDB::get_api_type(parent_class_name) == ClassDB::API_EXTENSION || ClassDB::get_api_type(parent_class_name) == ClassDB::API_EDITOR_EXTENSION) {
			ERR_PRINT("Unimplemented yet");
			//inheriting from another extension
		} else {
			//inheriting from engine class
		}
	} else {
		ERR_FAIL_MSG(vformat("Attempt to register an extension class '%s' using non-existing parent class '%s'.", String(class_name), String(parent_class_name)));
	}

#ifdef TOOLS_ENABLED
	Extension *extension = nullptr;
	bool is_runtime = (bool)p_extension_funcs->is_runtime;
	if (self->is_reloading && self->extension_classes.has(class_name)) {
		extension = &self->extension_classes[class_name];
		if (!parent_extension && parent_class_name != extension->ksextension.parent_class_name) {
			ERR_FAIL_MSG(vformat("KSExtension class '%s' cannot change parent type from '%s' to '%s' on hot reload. Restart Godot for this change to take effect.", class_name, extension->ksextension.parent_class_name, parent_class_name));
		}
		if (extension->ksextension.is_runtime != is_runtime) {
			ERR_PRINT(vformat("KSExtension class '%s' cannot change to/from runtime class on hot reload. Restart Godot for this change to take effect.", class_name));
			is_runtime = extension->ksextension.is_runtime;
		}
		extension->is_reloading = false;
	} else {
		self->extension_classes[class_name] = Extension();
		extension = &self->extension_classes[class_name];
	}
#else
	self->extension_classes[class_name] = Extension();
	Extension *extension = &self->extension_classes[class_name];
#endif

	if (parent_extension) {
		extension->ksextension.parent = &parent_extension->ksextension;
		parent_extension->ksextension.children.push_back(&extension->ksextension);
	}

	if (self->reloadable && p_extension_funcs->recreate_instance_func == nullptr) {
		ERR_PRINT(vformat("Extension marked as reloadable, but attempted to register class '%s' which doesn't support reloading. Perhaps your language binding don't support it? Reloading disabled for this extension.", class_name));
		self->reloadable = false;
	}

	extension->ksextension.library = self;
	extension->ksextension.parent_class_name = parent_class_name;
	extension->ksextension.class_name = class_name;
	extension->ksextension.editor_class = self->level_initialized == INITIALIZATION_LEVEL_EDITOR;
	extension->ksextension.is_virtual = p_extension_funcs->is_virtual;
	extension->ksextension.is_abstract = p_extension_funcs->is_abstract;
	extension->ksextension.is_exposed = p_extension_funcs->is_exposed;
#ifdef TOOLS_ENABLED
	extension->ksextension.is_runtime = is_runtime;
#endif
	extension->ksextension.set = p_extension_funcs->set_func;
	extension->ksextension.get = p_extension_funcs->get_func;
	extension->ksextension.get_property_list = p_extension_funcs->get_property_list_func;
	extension->ksextension.free_property_list2 = p_extension_funcs->free_property_list_func;
	extension->ksextension.property_can_revert = p_extension_funcs->property_can_revert_func;
	extension->ksextension.property_get_revert = p_extension_funcs->property_get_revert_func;
	extension->ksextension.validate_property = p_extension_funcs->validate_property_func;
#ifndef DISABLE_DEPRECATED
	if (p_deprecated_funcs) {
		extension->ksextension.notification = p_deprecated_funcs->notification_func;
		extension->ksextension.free_property_list = p_deprecated_funcs->free_property_list_func;
		extension->ksextension.create_instance = p_deprecated_funcs->create_instance_func;
		extension->ksextension.get_rid = p_deprecated_funcs->get_rid_func;
	}
#endif // DISABLE_DEPRECATED
	extension->ksextension.notification2 = p_extension_funcs->notification_func;
	extension->ksextension.to_string = p_extension_funcs->to_string_func;
	extension->ksextension.reference = p_extension_funcs->reference_func;
	extension->ksextension.unreference = p_extension_funcs->unreference_func;
	extension->ksextension.class_userdata = p_extension_funcs->class_userdata;
	extension->ksextension.create_instance2 = p_extension_funcs->create_instance_func;
	extension->ksextension.free_instance = p_extension_funcs->free_instance_func;
	extension->ksextension.recreate_instance = p_extension_funcs->recreate_instance_func;
	extension->ksextension.get_virtual = p_extension_funcs->get_virtual_func;
	extension->ksextension.get_virtual_call_data = p_extension_funcs->get_virtual_call_data_func;
	extension->ksextension.call_virtual_with_data = p_extension_funcs->call_virtual_with_data_func;

	extension->ksextension.reloadable = self->reloadable;
#ifdef TOOLS_ENABLED
	if (extension->ksextension.reloadable) {
		extension->ksextension.tracking_userdata = extension;
		extension->ksextension.track_instance = &KSExtension::_track_instance;
		extension->ksextension.untrack_instance = &KSExtension::_untrack_instance;
	} else {
		extension->ksextension.tracking_userdata = nullptr;
		extension->ksextension.track_instance = nullptr;
		extension->ksextension.untrack_instance = nullptr;
	}
#endif

	ClassDB::register_extension_class(&extension->ksextension);

	if (p_extension_funcs->icon_path != nullptr) {
		const String icon_path = *reinterpret_cast<const String *>(p_extension_funcs->icon_path);
		if (!icon_path.is_empty()) {
			self->class_icon_paths[class_name] = icon_path;
		}
	}
}

void KSExtension::_register_extension_class_method(KSExtensionClassLibraryPtr p_library, KSExtensionConstStringNamePtr p_class_name, const KSExtensionClassMethodInfo *p_method_info) {
	KSExtension *self = reinterpret_cast<KSExtension *>(p_library);

	StringName class_name = *reinterpret_cast<const StringName *>(p_class_name);
	StringName method_name = *reinterpret_cast<const StringName *>(p_method_info->name);
	ERR_FAIL_COND_MSG(!self->extension_classes.has(class_name), vformat("Attempt to register extension method '%s' for unexisting class '%s'.", String(method_name), class_name));

#ifdef TOOLS_ENABLED
	Extension *extension = &self->extension_classes[class_name];
	KSExtensionMethodBind *method = nullptr;

	// If the extension is still marked as reloading, that means it failed to register again.
	if (extension->is_reloading) {
		return;
	}

	if (self->is_reloading && extension->methods.has(method_name)) {
		method = extension->methods[method_name];

		// Try to update the method bind. If it doesn't work (because it's incompatible) then
		// mark as invalid and create a new one.
		if (!method->is_reloading || !method->try_update(p_method_info)) {
			method->valid = false;
			self->invalid_methods.push_back(method);

			method = nullptr;
		}
	}

	if (method == nullptr) {
		method = memnew(KSExtensionMethodBind(p_method_info));
		method->set_instance_class(class_name);
		extension->methods[method_name] = method;
	} else {
		method->is_reloading = false;
	}
#else
	KSExtensionMethodBind *method = memnew(KSExtensionMethodBind(p_method_info));
	method->set_instance_class(class_name);
#endif

	ClassDB::bind_method_custom(class_name, method);
}

void KSExtension::_register_extension_class_virtual_method(KSExtensionClassLibraryPtr p_library, KSExtensionConstStringNamePtr p_class_name, const KSExtensionClassVirtualMethodInfo *p_method_info) {
	StringName class_name = *reinterpret_cast<const StringName *>(p_class_name);
	ClassDB::add_extension_class_virtual_method(class_name, p_method_info);
}

void KSExtension::_register_extension_class_integer_constant(KSExtensionClassLibraryPtr p_library, KSExtensionConstStringNamePtr p_class_name, KSExtensionConstStringNamePtr p_enum_name, KSExtensionConstStringNamePtr p_constant_name, KSExtensionInt p_constant_value, KSExtensionBool p_is_bitfield) {
	KSExtension *self = reinterpret_cast<KSExtension *>(p_library);

	StringName class_name = *reinterpret_cast<const StringName *>(p_class_name);
	StringName enum_name = *reinterpret_cast<const StringName *>(p_enum_name);
	StringName constant_name = *reinterpret_cast<const StringName *>(p_constant_name);
	ERR_FAIL_COND_MSG(!self->extension_classes.has(class_name), vformat("Attempt to register extension constant '%s' for unexisting class '%s'.", constant_name, class_name));

#ifdef TOOLS_ENABLED
	// If the extension is still marked as reloading, that means it failed to register again.
	Extension *extension = &self->extension_classes[class_name];
	if (extension->is_reloading) {
		return;
	}
#endif

	ClassDB::bind_integer_constant(class_name, enum_name, constant_name, p_constant_value, p_is_bitfield);
}

void KSExtension::_register_extension_class_property(KSExtensionClassLibraryPtr p_library, KSExtensionConstStringNamePtr p_class_name, const KSExtensionPropertyInfo *p_info, KSExtensionConstStringNamePtr p_setter, KSExtensionConstStringNamePtr p_getter) {
	_register_extension_class_property_indexed(p_library, p_class_name, p_info, p_setter, p_getter, -1);
}

void KSExtension::_register_extension_class_property_indexed(KSExtensionClassLibraryPtr p_library, KSExtensionConstStringNamePtr p_class_name, const KSExtensionPropertyInfo *p_info, KSExtensionConstStringNamePtr p_setter, KSExtensionConstStringNamePtr p_getter, KSExtensionInt p_index) {
	KSExtension *self = reinterpret_cast<KSExtension *>(p_library);

	StringName class_name = *reinterpret_cast<const StringName *>(p_class_name);
	StringName setter = *reinterpret_cast<const StringName *>(p_setter);
	StringName getter = *reinterpret_cast<const StringName *>(p_getter);
	String property_name = *reinterpret_cast<const StringName *>(p_info->name);
	ERR_FAIL_COND_MSG(!self->extension_classes.has(class_name), vformat("Attempt to register extension class property '%s' for unexisting class '%s'.", property_name, class_name));

#ifdef TOOLS_ENABLED
	// If the extension is still marked as reloading, that means it failed to register again.
	Extension *extension = &self->extension_classes[class_name];
	if (extension->is_reloading) {
		return;
	}
#endif

	PropertyInfo pinfo(*p_info);

	ClassDB::add_property(class_name, pinfo, setter, getter, p_index);
}

void KSExtension::_register_extension_class_property_group(KSExtensionClassLibraryPtr p_library, KSExtensionConstStringNamePtr p_class_name, KSExtensionConstStringPtr p_group_name, KSExtensionConstStringPtr p_prefix) {
	KSExtension *self = reinterpret_cast<KSExtension *>(p_library);

	StringName class_name = *reinterpret_cast<const StringName *>(p_class_name);
	String group_name = *reinterpret_cast<const String *>(p_group_name);
	String prefix = *reinterpret_cast<const String *>(p_prefix);
	ERR_FAIL_COND_MSG(!self->extension_classes.has(class_name), vformat("Attempt to register extension class property group '%s' for unexisting class '%s'.", group_name, class_name));

#ifdef TOOLS_ENABLED
	// If the extension is still marked as reloading, that means it failed to register again.
	Extension *extension = &self->extension_classes[class_name];
	if (extension->is_reloading) {
		return;
	}
#endif

	ClassDB::add_property_group(class_name, group_name, prefix);
}

void KSExtension::_register_extension_class_property_subgroup(KSExtensionClassLibraryPtr p_library, KSExtensionConstStringNamePtr p_class_name, KSExtensionConstStringPtr p_subgroup_name, KSExtensionConstStringPtr p_prefix) {
	KSExtension *self = reinterpret_cast<KSExtension *>(p_library);

	StringName class_name = *reinterpret_cast<const StringName *>(p_class_name);
	String subgroup_name = *reinterpret_cast<const String *>(p_subgroup_name);
	String prefix = *reinterpret_cast<const String *>(p_prefix);
	ERR_FAIL_COND_MSG(!self->extension_classes.has(class_name), vformat("Attempt to register extension class property subgroup '%s' for unexisting class '%s'.", subgroup_name, class_name));

#ifdef TOOLS_ENABLED
	// If the extension is still marked as reloading, that means it failed to register again.
	Extension *extension = &self->extension_classes[class_name];
	if (extension->is_reloading) {
		return;
	}
#endif

	ClassDB::add_property_subgroup(class_name, subgroup_name, prefix);
}

void KSExtension::_register_extension_class_signal(KSExtensionClassLibraryPtr p_library, KSExtensionConstStringNamePtr p_class_name, KSExtensionConstStringNamePtr p_signal_name, const KSExtensionPropertyInfo *p_argument_info, KSExtensionInt p_argument_count) {
	KSExtension *self = reinterpret_cast<KSExtension *>(p_library);

	StringName class_name = *reinterpret_cast<const StringName *>(p_class_name);
	StringName signal_name = *reinterpret_cast<const StringName *>(p_signal_name);
	ERR_FAIL_COND_MSG(!self->extension_classes.has(class_name), vformat("Attempt to register extension class signal '%s' for unexisting class '%s'.", signal_name, class_name));

#ifdef TOOLS_ENABLED
	// If the extension is still marked as reloading, that means it failed to register again.
	Extension *extension = &self->extension_classes[class_name];
	if (extension->is_reloading) {
		return;
	}
#endif

	MethodInfo s;
	s.name = signal_name;
	for (int i = 0; i < p_argument_count; i++) {
		PropertyInfo arg(p_argument_info[i]);
		s.arguments.push_back(arg);
	}
	ClassDB::add_signal(class_name, s);
}

void KSExtension::_unregister_extension_class(KSExtensionClassLibraryPtr p_library, KSExtensionConstStringNamePtr p_class_name) {
	KSExtension *self = reinterpret_cast<KSExtension *>(p_library);

	StringName class_name = *reinterpret_cast<const StringName *>(p_class_name);
	ERR_FAIL_COND_MSG(!self->extension_classes.has(class_name), vformat("Attempt to unregister unexisting extension class '%s'.", class_name));

	Extension *ext = &self->extension_classes[class_name];
#ifdef TOOLS_ENABLED
	if (ext->is_reloading) {
		self->_clear_extension(ext);
	}
#endif
	ERR_FAIL_COND_MSG(ext->ksextension.children.size(), vformat("Attempt to unregister class '%s' while other extension classes inherit from it.", class_name));

#ifdef TOOLS_ENABLED
	ClassDB::unregister_extension_class(class_name, !ext->is_reloading);
#else
	ClassDB::unregister_extension_class(class_name);
#endif

	if (ext->ksextension.parent != nullptr) {
		ext->ksextension.parent->children.erase(&ext->ksextension);
	}

#ifdef TOOLS_ENABLED
	if (!ext->is_reloading) {
		self->extension_classes.erase(class_name);
	}

	KSExtensionEditorHelp::remove_class(class_name);
#else
	self->extension_classes.erase(class_name);
#endif
}

void KSExtension::_get_library_path(KSExtensionClassLibraryPtr p_library, KSExtensionUninitializedStringPtr r_path) {
	KSExtension *self = reinterpret_cast<KSExtension *>(p_library);

	Ref<KSExtensionLibraryLoader> library_loader = self->loader;
	String library_path;
	if (library_loader.is_valid()) {
		library_path = library_loader->library_path;
	}

	memnew_placement(r_path, String(library_path));
}

HashMap<StringName, KSExtensionInterfaceFunctionPtr> KSExtension::ksextension_interface_functions;

void KSExtension::register_interface_function(const StringName &p_function_name, KSExtensionInterfaceFunctionPtr p_function_pointer) {
	ERR_FAIL_COND_MSG(ksextension_interface_functions.has(p_function_name), vformat("Attempt to register interface function '%s', which appears to be already registered.", p_function_name));
	ksextension_interface_functions.insert(p_function_name, p_function_pointer);
}

KSExtensionInterfaceFunctionPtr KSExtension::get_interface_function(const StringName &p_function_name) {
	KSExtensionInterfaceFunctionPtr *function = ksextension_interface_functions.getptr(p_function_name);
	ERR_FAIL_NULL_V_MSG(function, nullptr, vformat("Attempt to get non-existent interface function: '%s'.", String(p_function_name)));
	return *function;
}

Error KSExtension::open_library(const String &p_path, const Ref<KSExtensionLoader> &p_loader) {
	ERR_FAIL_COND_V_MSG(p_loader.is_null(), FAILED, "Can't open KSExtension without a loader.");
	loader = p_loader;

	Error err = loader->open_library(p_path);

	ERR_FAIL_COND_V_MSG(err == ERR_FILE_NOT_FOUND, err, vformat("KSExtension dynamic library not found: '%s'.", p_path));
	ERR_FAIL_COND_V_MSG(err != OK, err, vformat("Can't open KSExtension dynamic library: '%s'.", p_path));

	err = loader->initialize(&ksextension_get_proc_address, this, &initialization);

	if (err != OK) {
		// Errors already logged in initialize().
		loader->close_library();
		return err;
	}

	level_initialized = -1;

	return OK;
}

void KSExtension::close_library() {
	ERR_FAIL_COND(!is_library_open());
	loader->close_library();

	class_icon_paths.clear();

#ifdef TOOLS_ENABLED
	instance_bindings.clear();
#endif
}

bool KSExtension::is_library_open() const {
	return loader.is_valid() && loader->is_library_open();
}

KSExtension::InitializationLevel KSExtension::get_minimum_library_initialization_level() const {
	ERR_FAIL_COND_V(!is_library_open(), INITIALIZATION_LEVEL_CORE);
	return InitializationLevel(initialization.minimum_initialization_level);
}

void KSExtension::initialize_library(InitializationLevel p_level) {
	ERR_FAIL_COND(!is_library_open());
	ERR_FAIL_COND_MSG(p_level <= int32_t(level_initialized), vformat("Level '%d' must be higher than the current level '%d'", p_level, level_initialized));

	level_initialized = int32_t(p_level);

	ERR_FAIL_NULL(initialization.initialize);

	initialization.initialize(initialization.userdata, KSExtensionInitializationLevel(p_level));
}
void KSExtension::deinitialize_library(InitializationLevel p_level) {
	ERR_FAIL_COND(!is_library_open());
	ERR_FAIL_COND(p_level > int32_t(level_initialized));

	level_initialized = int32_t(p_level) - 1;

	ERR_FAIL_NULL(initialization.deinitialize);

	initialization.deinitialize(initialization.userdata, KSExtensionInitializationLevel(p_level));
}

void KSExtension::_bind_methods() {
	ClassDB::bind_method(D_METHOD("is_library_open"), &KSExtension::is_library_open);
	ClassDB::bind_method(D_METHOD("get_minimum_library_initialization_level"), &KSExtension::get_minimum_library_initialization_level);

	BIND_ENUM_CONSTANT(INITIALIZATION_LEVEL_CORE);
	BIND_ENUM_CONSTANT(INITIALIZATION_LEVEL_SERVERS);
	BIND_ENUM_CONSTANT(INITIALIZATION_LEVEL_SCENE);
	BIND_ENUM_CONSTANT(INITIALIZATION_LEVEL_EDITOR);
}

KSExtension::KSExtension() {
}

KSExtension::~KSExtension() {
	if (is_library_open()) {
		close_library();
	}
#ifdef TOOLS_ENABLED
	// If we have any invalid method binds still laying around, we can finally free them!
	for (KSExtensionMethodBind *E : invalid_methods) {
		memdelete(E);
	}
#endif
}

void KSExtension::initialize_ksextensions() {
	ksextension_setup_interface();

#ifndef DISABLE_DEPRECATED
	register_interface_function("classdb_register_extension_class", (KSExtensionInterfaceFunctionPtr)&KSExtension::_register_extension_class);
	register_interface_function("classdb_register_extension_class2", (KSExtensionInterfaceFunctionPtr)&KSExtension::_register_extension_class2);
	register_interface_function("classdb_register_extension_class3", (KSExtensionInterfaceFunctionPtr)&KSExtension::_register_extension_class3);
#endif // DISABLE_DEPRECATED
	register_interface_function("classdb_register_extension_class4", (KSExtensionInterfaceFunctionPtr)&KSExtension::_register_extension_class4);
	register_interface_function("classdb_register_extension_class_method", (KSExtensionInterfaceFunctionPtr)&KSExtension::_register_extension_class_method);
	register_interface_function("classdb_register_extension_class_virtual_method", (KSExtensionInterfaceFunctionPtr)&KSExtension::_register_extension_class_virtual_method);
	register_interface_function("classdb_register_extension_class_integer_constant", (KSExtensionInterfaceFunctionPtr)&KSExtension::_register_extension_class_integer_constant);
	register_interface_function("classdb_register_extension_class_property", (KSExtensionInterfaceFunctionPtr)&KSExtension::_register_extension_class_property);
	register_interface_function("classdb_register_extension_class_property_indexed", (KSExtensionInterfaceFunctionPtr)&KSExtension::_register_extension_class_property_indexed);
	register_interface_function("classdb_register_extension_class_property_group", (KSExtensionInterfaceFunctionPtr)&KSExtension::_register_extension_class_property_group);
	register_interface_function("classdb_register_extension_class_property_subgroup", (KSExtensionInterfaceFunctionPtr)&KSExtension::_register_extension_class_property_subgroup);
	register_interface_function("classdb_register_extension_class_signal", (KSExtensionInterfaceFunctionPtr)&KSExtension::_register_extension_class_signal);
	register_interface_function("classdb_unregister_extension_class", (KSExtensionInterfaceFunctionPtr)&KSExtension::_unregister_extension_class);
	register_interface_function("get_library_path", (KSExtensionInterfaceFunctionPtr)&KSExtension::_get_library_path);
}

void KSExtension::finalize_ksextensions() {
	ksextension_interface_functions.clear();
}

Error KSExtensionResourceLoader::load_ksextension_resource(const String &p_path, Ref<KSExtension> &p_extension) {
	ERR_FAIL_COND_V_MSG(p_extension.is_valid() && p_extension->is_library_open(), ERR_ALREADY_IN_USE, "Cannot load KSExtension resource into already opened library.");

	KSExtensionManager *extension_manager = KSExtensionManager::get_singleton();

	KSExtensionManager::LoadStatus status = extension_manager->load_extension(p_path);
	if (status != KSExtensionManager::LOAD_STATUS_OK && status != KSExtensionManager::LOAD_STATUS_ALREADY_LOADED) {
		// Errors already logged in load_extension().
		return FAILED;
	}

	p_extension = extension_manager->get_extension(p_path);
	return OK;
}

Ref<Resource> KSExtensionResourceLoader::load(const String &p_path, const String &p_original_path, Error *r_error, bool p_use_sub_threads, float *r_progress, CacheMode p_cache_mode) {
	// We can't have two KSExtension resource object representing the same library, because
	// loading (or unloading) a KSExtension affects global data. So, we need reuse the same
	// object if one has already been loaded (even if caching is disabled at the resource
	// loader level).
	KSExtensionManager *manager = KSExtensionManager::get_singleton();
	if (manager->is_extension_loaded(p_path)) {
		return manager->get_extension(p_path);
	}

	Ref<KSExtension> lib;
	Error err = load_ksextension_resource(p_path, lib);
	if (err != OK && r_error) {
		// Errors already logged in load_ksextension_resource().
		*r_error = err;
	}
	return lib;
}

void KSExtensionResourceLoader::get_recognized_extensions(List<String> *p_extensions) const {
	p_extensions->push_back("ksextension");
}

bool KSExtensionResourceLoader::handles_type(const String &p_type) const {
	return p_type == "KSExtension";
}

String KSExtensionResourceLoader::get_resource_type(const String &p_path) const {
	String el = p_path.get_extension().to_lower();
	if (el == "ksextension") {
		return "KSExtension";
	}
	return "";
}

#ifdef TOOLS_ENABLED
bool KSExtension::has_library_changed() const {
	return loader->has_library_changed();
}

void KSExtension::prepare_reload() {
	is_reloading = true;

	for (KeyValue<StringName, Extension> &E : extension_classes) {
		E.value.is_reloading = true;

		for (KeyValue<StringName, KSExtensionMethodBind *> &M : E.value.methods) {
			M.value->is_reloading = true;
		}

		for (const ObjectID &obj_id : E.value.instances) {
			Object *obj = ObjectDB::get_instance(obj_id);
			if (!obj) {
				continue;
			}

			// Store instance state so it can be restored after reload.
			List<Pair<String, Variant>> state;
			List<PropertyInfo> prop_list;
			obj->get_property_list(&prop_list);
			for (const PropertyInfo &P : prop_list) {
				if (!(P.usage & PROPERTY_USAGE_STORAGE)) {
					continue;
				}

				Variant value = obj->get(P.name);
				Variant default_value = ClassDB::class_get_default_property_value(obj->get_class_name(), P.name);

				if (default_value.get_type() != Variant::NIL && bool(Variant::evaluate(Variant::OP_EQUAL, value, default_value))) {
					continue;
				}

				if (P.type == Variant::OBJECT && value.is_zero() && !(P.usage & PROPERTY_USAGE_STORE_IF_NULL)) {
					continue;
				}

				state.push_back(Pair<String, Variant>(P.name, value));
			}
			E.value.instance_state[obj_id] = {
				state, // List<Pair<String, Variant>> properties;
				obj->is_extension_placeholder(), // bool is_placeholder;
			};
		}
	}
}

void KSExtension::_clear_extension(Extension *p_extension) {
	// Clear out hierarchy information because it may change.
	p_extension->ksextension.parent = nullptr;
	p_extension->ksextension.children.clear();

	// Clear all objects of any KSExtension data. It will become its native parent class
	// until the reload can reset the object with the new KSExtension data.
	for (const ObjectID &obj_id : p_extension->instances) {
		Object *obj = ObjectDB::get_instance(obj_id);
		if (!obj) {
			continue;
		}

		obj->clear_internal_extension();
	}
}

void KSExtension::track_instance_binding(Object *p_object) {
	instance_bindings.push_back(p_object->get_instance_id());
}

void KSExtension::untrack_instance_binding(Object *p_object) {
	instance_bindings.erase(p_object->get_instance_id());
}

void KSExtension::clear_instance_bindings() {
	for (ObjectID obj_id : instance_bindings) {
		Object *obj = ObjectDB::get_instance(obj_id);
		if (!obj) {
			continue;
		}

		obj->free_instance_binding(this);
	}
	instance_bindings.clear();
}

void KSExtension::finish_reload() {
	is_reloading = false;

	// Clean up any classes or methods that didn't get re-added.
	Vector<StringName> classes_to_remove;
	for (KeyValue<StringName, Extension> &E : extension_classes) {
		if (E.value.is_reloading) {
			E.value.is_reloading = false;
			classes_to_remove.push_back(E.key);
		}

		Vector<StringName> methods_to_remove;
		for (KeyValue<StringName, KSExtensionMethodBind *> &M : E.value.methods) {
			if (M.value->is_reloading) {
				M.value->valid = false;
				invalid_methods.push_back(M.value);

				M.value->is_reloading = false;
				methods_to_remove.push_back(M.key);
			}
		}
		for (const StringName &method_name : methods_to_remove) {
			E.value.methods.erase(method_name);
		}
	}
	for (const StringName &class_name : classes_to_remove) {
		extension_classes.erase(class_name);
	}

	// Reset any the extension on instances made from the classes that remain.
	for (KeyValue<StringName, Extension> &E : extension_classes) {
		// Loop over 'instance_state' rather than 'instance' because new instances
		// may have been created when re-initializing the extension.
		for (const KeyValue<ObjectID, Extension::InstanceState> &S : E.value.instance_state) {
			Object *obj = ObjectDB::get_instance(S.key);
			if (!obj) {
				continue;
			}

			if (S.value.is_placeholder) {
				obj->reset_internal_extension(ClassDB::get_placeholder_extension(E.value.voyextension.class_name));
			} else {
				obj->reset_internal_extension(&E.value.voyextension);
			}
		}
	}

	// Now that all the classes are back, restore the state.
	for (KeyValue<StringName, Extension> &E : extension_classes) {
		for (const KeyValue<ObjectID, Extension::InstanceState> &S : E.value.instance_state) {
			Object *obj = ObjectDB::get_instance(S.key);
			if (!obj) {
				continue;
			}

			for (const Pair<String, Variant> &state : S.value.properties) {
				obj->set(state.first, state.second);
			}
		}
	}

	// Finally, let the objects know that we are done reloading them.
	for (KeyValue<StringName, Extension> &E : extension_classes) {
		for (const KeyValue<ObjectID, Extension::InstanceState> &S : E.value.instance_state) {
			Object *obj = ObjectDB::get_instance(S.key);
			if (!obj) {
				continue;
			}

			obj->notification(NOTIFICATION_EXTENSION_RELOADED);
		}

		// Clear the instance state, we're done looping.
		E.value.instance_state.clear();
	}
}

void KSExtension::_track_instance(void *p_user_data, void *p_instance) {
	Extension *extension = reinterpret_cast<Extension *>(p_user_data);
	Object *obj = reinterpret_cast<Object *>(p_instance);

	extension->instances.insert(obj->get_instance_id());
}

void KSExtension::_untrack_instance(void *p_user_data, void *p_instance) {
	Extension *extension = reinterpret_cast<Extension *>(p_user_data);
	Object *obj = reinterpret_cast<Object *>(p_instance);

	extension->instances.erase(obj->get_instance_id());
}

Vector<StringName> KSExtensionEditorPlugins::extension_classes;
KSExtensionEditorPlugins::EditorPluginRegisterFunc KSExtensionEditorPlugins::editor_node_add_plugin = nullptr;
KSExtensionEditorPlugins::EditorPluginRegisterFunc KSExtensionEditorPlugins::editor_node_remove_plugin = nullptr;

void KSExtensionEditorPlugins::add_extension_class(const StringName &p_class_name) {
	if (editor_node_add_plugin) {
		editor_node_add_plugin(p_class_name);
	} else {
		extension_classes.push_back(p_class_name);
	}
}

void KSExtensionEditorPlugins::remove_extension_class(const StringName &p_class_name) {
	if (editor_node_remove_plugin) {
		editor_node_remove_plugin(p_class_name);
	} else {
		extension_classes.erase(p_class_name);
	}
}

KSExtensionEditorHelp::EditorHelpLoadXmlBufferFunc KSExtensionEditorHelp::editor_help_load_xml_buffer = nullptr;
KSExtensionEditorHelp::EditorHelpRemoveClassFunc KSExtensionEditorHelp::editor_help_remove_class = nullptr;

void KSExtensionEditorHelp::load_xml_buffer(const uint8_t *p_buffer, int p_size) {
	ERR_FAIL_NULL(editor_help_load_xml_buffer);
	editor_help_load_xml_buffer(p_buffer, p_size);
}

void KSExtensionEditorHelp::remove_class(const String &p_class) {
	ERR_FAIL_NULL(editor_help_remove_class);
	editor_help_remove_class(p_class);
}
#endif // TOOLS_ENABLED
