/**************************************************************************/
/*  ksextension_interface.h                                               */
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

#ifndef KSEXTENSION_INTERFACE_H
#define KSEXTENSION_INTERFACE_H

/* This is a C class header, you can copy it and use it directly in your own binders.
 * Together with the JSON file, you should be able to generate any binder.
 */

#include <stddef.h>
#include <stdint.h>

#ifndef __cplusplus
typedef uint32_t char32_t;
typedef uint16_t char16_t;
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* VARIANT TYPES */

typedef enum {
	KSEXTENSION_VARIANT_TYPE_NIL,

	/*  atomic types */
	KSEXTENSION_VARIANT_TYPE_BOOL,
	KSEXTENSION_VARIANT_TYPE_INT,
	KSEXTENSION_VARIANT_TYPE_FLOAT,
	KSEXTENSION_VARIANT_TYPE_STRING,

	/* math types */
	KSEXTENSION_VARIANT_TYPE_VECTOR2,
	KSEXTENSION_VARIANT_TYPE_VECTOR2I,
	KSEXTENSION_VARIANT_TYPE_RECT2,
	KSEXTENSION_VARIANT_TYPE_RECT2I,
	KSEXTENSION_VARIANT_TYPE_VECTOR3,
	KSEXTENSION_VARIANT_TYPE_VECTOR3I,
	KSEXTENSION_VARIANT_TYPE_TRANSFORM2D,
	KSEXTENSION_VARIANT_TYPE_VECTOR4,
	KSEXTENSION_VARIANT_TYPE_VECTOR4I,
	KSEXTENSION_VARIANT_TYPE_PLANE,
	KSEXTENSION_VARIANT_TYPE_QUATERNION,
	KSEXTENSION_VARIANT_TYPE_AABB,
	KSEXTENSION_VARIANT_TYPE_BASIS,
	KSEXTENSION_VARIANT_TYPE_TRANSFORM3D,
	KSEXTENSION_VARIANT_TYPE_PROJECTION,

	/* misc types */
	KSEXTENSION_VARIANT_TYPE_COLOR,
	KSEXTENSION_VARIANT_TYPE_STRING_NAME,
	KSEXTENSION_VARIANT_TYPE_NODE_PATH,
	KSEXTENSION_VARIANT_TYPE_RID,
	KSEXTENSION_VARIANT_TYPE_OBJECT,
	KSEXTENSION_VARIANT_TYPE_CALLABLE,
	KSEXTENSION_VARIANT_TYPE_SIGNAL,
	KSEXTENSION_VARIANT_TYPE_DICTIONARY,
	KSEXTENSION_VARIANT_TYPE_ARRAY,

	/* typed arrays */
	KSEXTENSION_VARIANT_TYPE_PACKED_BYTE_ARRAY,
	KSEXTENSION_VARIANT_TYPE_PACKED_INT32_ARRAY,
	KSEXTENSION_VARIANT_TYPE_PACKED_INT64_ARRAY,
	KSEXTENSION_VARIANT_TYPE_PACKED_FLOAT32_ARRAY,
	KSEXTENSION_VARIANT_TYPE_PACKED_FLOAT64_ARRAY,
	KSEXTENSION_VARIANT_TYPE_PACKED_STRING_ARRAY,
	KSEXTENSION_VARIANT_TYPE_PACKED_VECTOR2_ARRAY,
	KSEXTENSION_VARIANT_TYPE_PACKED_VECTOR3_ARRAY,
	KSEXTENSION_VARIANT_TYPE_PACKED_COLOR_ARRAY,
	KSEXTENSION_VARIANT_TYPE_PACKED_VECTOR4_ARRAY,

	KSEXTENSION_VARIANT_TYPE_VARIANT_MAX
} KSExtensionVariantType;

typedef enum {
	/* comparison */
	KSEXTENSION_VARIANT_OP_EQUAL,
	KSEXTENSION_VARIANT_OP_NOT_EQUAL,
	KSEXTENSION_VARIANT_OP_LESS,
	KSEXTENSION_VARIANT_OP_LESS_EQUAL,
	KSEXTENSION_VARIANT_OP_GREATER,
	KSEXTENSION_VARIANT_OP_GREATER_EQUAL,

	/* mathematic */
	KSEXTENSION_VARIANT_OP_ADD,
	KSEXTENSION_VARIANT_OP_SUBTRACT,
	KSEXTENSION_VARIANT_OP_MULTIPLY,
	KSEXTENSION_VARIANT_OP_DIVIDE,
	KSEXTENSION_VARIANT_OP_NEGATE,
	KSEXTENSION_VARIANT_OP_POSITIVE,
	KSEXTENSION_VARIANT_OP_MODULE,
	KSEXTENSION_VARIANT_OP_POWER,

	/* bitwise */
	KSEXTENSION_VARIANT_OP_SHIFT_LEFT,
	KSEXTENSION_VARIANT_OP_SHIFT_RIGHT,
	KSEXTENSION_VARIANT_OP_BIT_AND,
	KSEXTENSION_VARIANT_OP_BIT_OR,
	KSEXTENSION_VARIANT_OP_BIT_XOR,
	KSEXTENSION_VARIANT_OP_BIT_NEGATE,

	/* logic */
	KSEXTENSION_VARIANT_OP_AND,
	KSEXTENSION_VARIANT_OP_OR,
	KSEXTENSION_VARIANT_OP_XOR,
	KSEXTENSION_VARIANT_OP_NOT,

	/* containment */
	KSEXTENSION_VARIANT_OP_IN,
	KSEXTENSION_VARIANT_OP_MAX

} KSExtensionVariantOperator;

// In this API there are multiple functions which expect the caller to pass a pointer
// on return value as parameter.
// In order to make it clear if the caller should initialize the return value or not
// we have two flavor of types:
// - `KSExtensionXXXPtr` for pointer on an initialized value
// - `KSExtensionUninitializedXXXPtr` for pointer on uninitialized value
//
// Notes:
// - Not respecting those requirements can seems harmless, but will lead to unexpected
//   segfault or memory leak (for instance with a specific compiler/OS, or when two
//   native extensions start doing ptrcall on each other).
// - Initialization must be done with the function pointer returned by `variant_get_ptr_constructor`,
//   zero-initializing the variable should not be considered a valid initialization method here !
// - Some types have no destructor (see `extension_api.json`'s `has_destructor` field), for
//   them it is always safe to skip the constructor for the return value if you are in a hurry ;-)

typedef void *KSExtensionVariantPtr;
typedef const void *KSExtensionConstVariantPtr;
typedef void *KSExtensionUninitializedVariantPtr;
typedef void *KSExtensionStringNamePtr;
typedef const void *KSExtensionConstStringNamePtr;
typedef void *KSExtensionUninitializedStringNamePtr;
typedef void *KSExtensionStringPtr;
typedef const void *KSExtensionConstStringPtr;
typedef void *KSExtensionUninitializedStringPtr;
typedef void *KSExtensionObjectPtr;
typedef const void *KSExtensionConstObjectPtr;
typedef void *KSExtensionUninitializedObjectPtr;
typedef void *KSExtensionTypePtr;
typedef const void *KSExtensionConstTypePtr;
typedef void *KSExtensionUninitializedTypePtr;
typedef const void *KSExtensionMethodBindPtr;
typedef int64_t KSExtensionInt;
typedef uint8_t KSExtensionBool;
typedef uint64_t KSObjectInstanceID;
typedef void *KSExtensionRefPtr;
typedef const void *KSExtensionConstRefPtr;

/* VARIANT DATA I/O */

typedef enum {
	KSEXTENSION_CALL_OK,
	KSEXTENSION_CALL_ERROR_INVALID_METHOD,
	KSEXTENSION_CALL_ERROR_INVALID_ARGUMENT, // Expected a different variant type.
	KSEXTENSION_CALL_ERROR_TOO_MANY_ARGUMENTS, // Expected lower number of arguments.
	KSEXTENSION_CALL_ERROR_TOO_FEW_ARGUMENTS, // Expected higher number of arguments.
	KSEXTENSION_CALL_ERROR_INSTANCE_IS_NULL,
	KSEXTENSION_CALL_ERROR_METHOD_NOT_CONST, // Used for const call.
} KSExtensionCallErrorType;

typedef struct {
	KSExtensionCallErrorType error;
	int32_t argument;
	int32_t expected;
} KSExtensionCallError;

typedef void (*KSExtensionVariantFromTypeConstructorFunc)(KSExtensionUninitializedVariantPtr, KSExtensionTypePtr);
typedef void (*KSExtensionTypeFromVariantConstructorFunc)(KSExtensionUninitializedTypePtr, KSExtensionVariantPtr);
typedef void *(*KSExtensionVariantGetInternalPtrFunc)(KSExtensionVariantPtr);
typedef void (*KSExtensionPtrOperatorEvaluator)(KSExtensionConstTypePtr p_left, KSExtensionConstTypePtr p_right, KSExtensionTypePtr r_result);
typedef void (*KSExtensionPtrBuiltInMethod)(KSExtensionTypePtr p_base, const KSExtensionConstTypePtr *p_args, KSExtensionTypePtr r_return, int p_argument_count);
typedef void (*KSExtensionPtrConstructor)(KSExtensionUninitializedTypePtr p_base, const KSExtensionConstTypePtr *p_args);
typedef void (*KSExtensionPtrDestructor)(KSExtensionTypePtr p_base);
typedef void (*KSExtensionPtrSetter)(KSExtensionTypePtr p_base, KSExtensionConstTypePtr p_value);
typedef void (*KSExtensionPtrGetter)(KSExtensionConstTypePtr p_base, KSExtensionTypePtr r_value);
typedef void (*KSExtensionPtrIndexedSetter)(KSExtensionTypePtr p_base, KSExtensionInt p_index, KSExtensionConstTypePtr p_value);
typedef void (*KSExtensionPtrIndexedGetter)(KSExtensionConstTypePtr p_base, KSExtensionInt p_index, KSExtensionTypePtr r_value);
typedef void (*KSExtensionPtrKeyedSetter)(KSExtensionTypePtr p_base, KSExtensionConstTypePtr p_key, KSExtensionConstTypePtr p_value);
typedef void (*KSExtensionPtrKeyedGetter)(KSExtensionConstTypePtr p_base, KSExtensionConstTypePtr p_key, KSExtensionTypePtr r_value);
typedef uint32_t (*KSExtensionPtrKeyedChecker)(KSExtensionConstVariantPtr p_base, KSExtensionConstVariantPtr p_key);
typedef void (*KSExtensionPtrUtilityFunction)(KSExtensionTypePtr r_return, const KSExtensionConstTypePtr *p_args, int p_argument_count);

typedef KSExtensionObjectPtr (*KSExtensionClassConstructor)();

typedef void *(*KSExtensionInstanceBindingCreateCallback)(void *p_token, void *p_instance);
typedef void (*KSExtensionInstanceBindingFreeCallback)(void *p_token, void *p_instance, void *p_binding);
typedef KSExtensionBool (*KSExtensionInstanceBindingReferenceCallback)(void *p_token, void *p_binding, KSExtensionBool p_reference);

typedef struct {
	KSExtensionInstanceBindingCreateCallback create_callback;
	KSExtensionInstanceBindingFreeCallback free_callback;
	KSExtensionInstanceBindingReferenceCallback reference_callback;
} KSExtensionInstanceBindingCallbacks;

/* EXTENSION CLASSES */

typedef void *KSExtensionClassInstancePtr;

typedef KSExtensionBool (*KSExtensionClassSet)(KSExtensionClassInstancePtr p_instance, KSExtensionConstStringNamePtr p_name, KSExtensionConstVariantPtr p_value);
typedef KSExtensionBool (*KSExtensionClassGet)(KSExtensionClassInstancePtr p_instance, KSExtensionConstStringNamePtr p_name, KSExtensionVariantPtr r_ret);
typedef uint64_t (*KSExtensionClassGetRID)(KSExtensionClassInstancePtr p_instance);

typedef struct {
	KSExtensionVariantType type;
	KSExtensionStringNamePtr name;
	KSExtensionStringNamePtr class_name;
	uint32_t hint; // Bitfield of `PropertyHint` (defined in `extension_api.json`).
	KSExtensionStringPtr hint_string;
	uint32_t usage; // Bitfield of `PropertyUsageFlags` (defined in `extension_api.json`).
} KSExtensionPropertyInfo;

typedef struct {
	KSExtensionStringNamePtr name;
	KSExtensionPropertyInfo return_value;
	uint32_t flags; // Bitfield of `KSExtensionClassMethodFlags`.
	int32_t id;

	/* Arguments: `default_arguments` is an array of size `argument_count`. */
	uint32_t argument_count;
	KSExtensionPropertyInfo *arguments;

	/* Default arguments: `default_arguments` is an array of size `default_argument_count`. */
	uint32_t default_argument_count;
	KSExtensionVariantPtr *default_arguments;
} KSExtensionMethodInfo;

typedef const KSExtensionPropertyInfo *(*KSExtensionClassGetPropertyList)(KSExtensionClassInstancePtr p_instance, uint32_t *r_count);
typedef void (*KSExtensionClassFreePropertyList)(KSExtensionClassInstancePtr p_instance, const KSExtensionPropertyInfo *p_list);
typedef void (*KSExtensionClassFreePropertyList2)(KSExtensionClassInstancePtr p_instance, const KSExtensionPropertyInfo *p_list, uint32_t p_count);
typedef KSExtensionBool (*KSExtensionClassPropertyCanRevert)(KSExtensionClassInstancePtr p_instance, KSExtensionConstStringNamePtr p_name);
typedef KSExtensionBool (*KSExtensionClassPropertyGetRevert)(KSExtensionClassInstancePtr p_instance, KSExtensionConstStringNamePtr p_name, KSExtensionVariantPtr r_ret);
typedef KSExtensionBool (*KSExtensionClassValidateProperty)(KSExtensionClassInstancePtr p_instance, KSExtensionPropertyInfo *p_property);
typedef void (*KSExtensionClassNotification)(KSExtensionClassInstancePtr p_instance, int32_t p_what); // Deprecated. Use KSExtensionClassNotification2 instead.
typedef void (*KSExtensionClassNotification2)(KSExtensionClassInstancePtr p_instance, int32_t p_what, KSExtensionBool p_reversed);
typedef void (*KSExtensionClassToString)(KSExtensionClassInstancePtr p_instance, KSExtensionBool *r_is_valid, KSExtensionStringPtr p_out);
typedef void (*KSExtensionClassReference)(KSExtensionClassInstancePtr p_instance);
typedef void (*KSExtensionClassUnreference)(KSExtensionClassInstancePtr p_instance);
typedef void (*KSExtensionClassCallVirtual)(KSExtensionClassInstancePtr p_instance, const KSExtensionConstTypePtr *p_args, KSExtensionTypePtr r_ret);
typedef KSExtensionObjectPtr (*KSExtensionClassCreateInstance)(void *p_class_userdata);
typedef KSExtensionObjectPtr (*KSExtensionClassCreateInstance2)(void *p_class_userdata, KSExtensionBool p_notify_postinitialize);
typedef void (*KSExtensionClassFreeInstance)(void *p_class_userdata, KSExtensionClassInstancePtr p_instance);
typedef KSExtensionClassInstancePtr (*KSExtensionClassRecreateInstance)(void *p_class_userdata, KSExtensionObjectPtr p_object);
typedef KSExtensionClassCallVirtual (*KSExtensionClassGetVirtual)(void *p_class_userdata, KSExtensionConstStringNamePtr p_name);
typedef void *(*KSExtensionClassGetVirtualCallData)(void *p_class_userdata, KSExtensionConstStringNamePtr p_name);
typedef void (*KSExtensionClassCallVirtualWithData)(KSExtensionClassInstancePtr p_instance, KSExtensionConstStringNamePtr p_name, void *p_virtual_call_userdata, const KSExtensionConstTypePtr *p_args, KSExtensionTypePtr r_ret);

typedef struct {
	KSExtensionBool is_virtual;
	KSExtensionBool is_abstract;
	KSExtensionClassSet set_func;
	KSExtensionClassGet get_func;
	KSExtensionClassGetPropertyList get_property_list_func;
	KSExtensionClassFreePropertyList free_property_list_func;
	KSExtensionClassPropertyCanRevert property_can_revert_func;
	KSExtensionClassPropertyGetRevert property_get_revert_func;
	KSExtensionClassNotification notification_func;
	KSExtensionClassToString to_string_func;
	KSExtensionClassReference reference_func;
	KSExtensionClassUnreference unreference_func;
	KSExtensionClassCreateInstance create_instance_func; // (Default) constructor; mandatory. If the class is not instantiable, consider making it virtual or abstract.
	KSExtensionClassFreeInstance free_instance_func; // Destructor; mandatory.
	KSExtensionClassGetVirtual get_virtual_func; // Queries a virtual function by name and returns a callback to invoke the requested virtual function.
	KSExtensionClassGetRID get_rid_func;
	void *class_userdata; // Per-class user data, later accessible in instance bindings.
} KSExtensionClassCreationInfo; // Deprecated. Use KSExtensionClassCreationInfo4 instead.

typedef struct {
	KSExtensionBool is_virtual;
	KSExtensionBool is_abstract;
	KSExtensionBool is_exposed;
	KSExtensionClassSet set_func;
	KSExtensionClassGet get_func;
	KSExtensionClassGetPropertyList get_property_list_func;
	KSExtensionClassFreePropertyList free_property_list_func;
	KSExtensionClassPropertyCanRevert property_can_revert_func;
	KSExtensionClassPropertyGetRevert property_get_revert_func;
	KSExtensionClassValidateProperty validate_property_func;
	KSExtensionClassNotification2 notification_func;
	KSExtensionClassToString to_string_func;
	KSExtensionClassReference reference_func;
	KSExtensionClassUnreference unreference_func;
	KSExtensionClassCreateInstance create_instance_func; // (Default) constructor; mandatory. If the class is not instantiable, consider making it virtual or abstract.
	KSExtensionClassFreeInstance free_instance_func; // Destructor; mandatory.
	KSExtensionClassRecreateInstance recreate_instance_func;
	// Queries a virtual function by name and returns a callback to invoke the requested virtual function.
	KSExtensionClassGetVirtual get_virtual_func;
	// Paired with `call_virtual_with_data_func`, this is an alternative to `get_virtual_func` for extensions that
	// need or benefit from extra data when calling virtual functions.
	// Returns user data that will be passed to `call_virtual_with_data_func`.
	// Returning `NULL` from this function signals to Kosmic that the virtual function is not overridden.
	// Data returned from this function should be managed by the extension and must be valid until the extension is deinitialized.
	// You should supply either `get_virtual_func`, or `get_virtual_call_data_func` with `call_virtual_with_data_func`.
	KSExtensionClassGetVirtualCallData get_virtual_call_data_func;
	// Used to call virtual functions when `get_virtual_call_data_func` is not null.
	KSExtensionClassCallVirtualWithData call_virtual_with_data_func;
	KSExtensionClassGetRID get_rid_func;
	void *class_userdata; // Per-class user data, later accessible in instance bindings.
} KSExtensionClassCreationInfo2; // Deprecated. Use KSExtensionClassCreationInfo4 instead.

typedef struct {
	KSExtensionBool is_virtual;
	KSExtensionBool is_abstract;
	KSExtensionBool is_exposed;
	KSExtensionBool is_runtime;
	KSExtensionClassSet set_func;
	KSExtensionClassGet get_func;
	KSExtensionClassGetPropertyList get_property_list_func;
	KSExtensionClassFreePropertyList2 free_property_list_func;
	KSExtensionClassPropertyCanRevert property_can_revert_func;
	KSExtensionClassPropertyGetRevert property_get_revert_func;
	KSExtensionClassValidateProperty validate_property_func;
	KSExtensionClassNotification2 notification_func;
	KSExtensionClassToString to_string_func;
	KSExtensionClassReference reference_func;
	KSExtensionClassUnreference unreference_func;
	KSExtensionClassCreateInstance create_instance_func; // (Default) constructor; mandatory. If the class is not instantiable, consider making it virtual or abstract.
	KSExtensionClassFreeInstance free_instance_func; // Destructor; mandatory.
	KSExtensionClassRecreateInstance recreate_instance_func;
	// Queries a virtual function by name and returns a callback to invoke the requested virtual function.
	KSExtensionClassGetVirtual get_virtual_func;
	// Paired with `call_virtual_with_data_func`, this is an alternative to `get_virtual_func` for extensions that
	// need or benefit from extra data when calling virtual functions.
	// Returns user data that will be passed to `call_virtual_with_data_func`.
	// Returning `NULL` from this function signals to Kosmic that the virtual function is not overridden.
	// Data returned from this function should be managed by the extension and must be valid until the extension is deinitialized.
	// You should supply either `get_virtual_func`, or `get_virtual_call_data_func` with `call_virtual_with_data_func`.
	KSExtensionClassGetVirtualCallData get_virtual_call_data_func;
	// Used to call virtual functions when `get_virtual_call_data_func` is not null.
	KSExtensionClassCallVirtualWithData call_virtual_with_data_func;
	KSExtensionClassGetRID get_rid_func;
	void *class_userdata; // Per-class user data, later accessible in instance bindings.
} KSExtensionClassCreationInfo3; // Deprecated. Use KSExtensionClassCreationInfo4 instead.

typedef struct {
	KSExtensionBool is_virtual;
	KSExtensionBool is_abstract;
	KSExtensionBool is_exposed;
	KSExtensionBool is_runtime;
	KSExtensionConstStringPtr icon_path;
	KSExtensionClassSet set_func;
	KSExtensionClassGet get_func;
	KSExtensionClassGetPropertyList get_property_list_func;
	KSExtensionClassFreePropertyList2 free_property_list_func;
	KSExtensionClassPropertyCanRevert property_can_revert_func;
	KSExtensionClassPropertyGetRevert property_get_revert_func;
	KSExtensionClassValidateProperty validate_property_func;
	KSExtensionClassNotification2 notification_func;
	KSExtensionClassToString to_string_func;
	KSExtensionClassReference reference_func;
	KSExtensionClassUnreference unreference_func;
	KSExtensionClassCreateInstance2 create_instance_func; // (Default) constructor; mandatory. If the class is not instantiable, consider making it virtual or abstract.
	KSExtensionClassFreeInstance free_instance_func; // Destructor; mandatory.
	KSExtensionClassRecreateInstance recreate_instance_func;
	// Queries a virtual function by name and returns a callback to invoke the requested virtual function.
	KSExtensionClassGetVirtual get_virtual_func;
	// Paired with `call_virtual_with_data_func`, this is an alternative to `get_virtual_func` for extensions that
	// need or benefit from extra data when calling virtual functions.
	// Returns user data that will be passed to `call_virtual_with_data_func`.
	// Returning `NULL` from this function signals to Kosmic that the virtual function is not overridden.
	// Data returned from this function should be managed by the extension and must be valid until the extension is deinitialized.
	// You should supply either `get_virtual_func`, or `get_virtual_call_data_func` with `call_virtual_with_data_func`.
	KSExtensionClassGetVirtualCallData get_virtual_call_data_func;
	// Used to call virtual functions when `get_virtual_call_data_func` is not null.
	KSExtensionClassCallVirtualWithData call_virtual_with_data_func;
	void *class_userdata; // Per-class user data, later accessible in instance bindings.
} KSExtensionClassCreationInfo4;

typedef void *KSExtensionClassLibraryPtr;

/* Method */

typedef enum {
	KSEXTENSION_METHOD_FLAG_NORMAL = 1,
	KSEXTENSION_METHOD_FLAG_EDITOR = 2,
	KSEXTENSION_METHOD_FLAG_CONST = 4,
	KSEXTENSION_METHOD_FLAG_VIRTUAL = 8,
	KSEXTENSION_METHOD_FLAG_VARARG = 16,
	KSEXTENSION_METHOD_FLAG_STATIC = 32,
	KSEXTENSION_METHOD_FLAGS_DEFAULT = KSEXTENSION_METHOD_FLAG_NORMAL,
} KSExtensionClassMethodFlags;

typedef enum {
	KSEXTENSION_METHOD_ARGUMENT_METADATA_NONE,
	KSEXTENSION_METHOD_ARGUMENT_METADATA_INT_IS_INT8,
	KSEXTENSION_METHOD_ARGUMENT_METADATA_INT_IS_INT16,
	KSEXTENSION_METHOD_ARGUMENT_METADATA_INT_IS_INT32,
	KSEXTENSION_METHOD_ARGUMENT_METADATA_INT_IS_INT64,
	KSEXTENSION_METHOD_ARGUMENT_METADATA_INT_IS_UINT8,
	KSEXTENSION_METHOD_ARGUMENT_METADATA_INT_IS_UINT16,
	KSEXTENSION_METHOD_ARGUMENT_METADATA_INT_IS_UINT32,
	KSEXTENSION_METHOD_ARGUMENT_METADATA_INT_IS_UINT64,
	KSEXTENSION_METHOD_ARGUMENT_METADATA_REAL_IS_FLOAT,
	KSEXTENSION_METHOD_ARGUMENT_METADATA_REAL_IS_DOUBLE,
	KSEXTENSION_METHOD_ARGUMENT_METADATA_INT_IS_CHAR16,
	KSEXTENSION_METHOD_ARGUMENT_METADATA_INT_IS_CHAR32,
} KSExtensionClassMethodArgumentMetadata;

typedef void (*KSExtensionClassMethodCall)(void *method_userdata, KSExtensionClassInstancePtr p_instance, const KSExtensionConstVariantPtr *p_args, KSExtensionInt p_argument_count, KSExtensionVariantPtr r_return, KSExtensionCallError *r_error);
typedef void (*KSExtensionClassMethodValidatedCall)(void *method_userdata, KSExtensionClassInstancePtr p_instance, const KSExtensionConstVariantPtr *p_args, KSExtensionVariantPtr r_return);
typedef void (*KSExtensionClassMethodPtrCall)(void *method_userdata, KSExtensionClassInstancePtr p_instance, const KSExtensionConstTypePtr *p_args, KSExtensionTypePtr r_ret);

typedef struct {
	KSExtensionStringNamePtr name;
	void *method_userdata;
	KSExtensionClassMethodCall call_func;
	KSExtensionClassMethodPtrCall ptrcall_func;
	uint32_t method_flags; // Bitfield of `KSExtensionClassMethodFlags`.

	/* If `has_return_value` is false, `return_value_info` and `return_value_metadata` are ignored.
	 *
	 * @todo Consider dropping `has_return_value` and making the other two properties match `KSExtensionMethodInfo` and `KSExtensionClassVirtualMethod` for consistency in future version of this struct.
	 */
	KSExtensionBool has_return_value;
	KSExtensionPropertyInfo *return_value_info;
	KSExtensionClassMethodArgumentMetadata return_value_metadata;

	/* Arguments: `arguments_info` and `arguments_metadata` are array of size `argument_count`.
	 * Name and hint information for the argument can be omitted in release builds. Class name should always be present if it applies.
	 *
	 * @todo Consider renaming `arguments_info` to `arguments` for consistency in future version of this struct.
	 */
	uint32_t argument_count;
	KSExtensionPropertyInfo *arguments_info;
	KSExtensionClassMethodArgumentMetadata *arguments_metadata;

	/* Default arguments: `default_arguments` is an array of size `default_argument_count`. */
	uint32_t default_argument_count;
	KSExtensionVariantPtr *default_arguments;
} KSExtensionClassMethodInfo;

typedef struct {
	KSExtensionStringNamePtr name;
	uint32_t method_flags; // Bitfield of `KSExtensionClassMethodFlags`.

	KSExtensionPropertyInfo return_value;
	KSExtensionClassMethodArgumentMetadata return_value_metadata;

	uint32_t argument_count;
	KSExtensionPropertyInfo *arguments;
	KSExtensionClassMethodArgumentMetadata *arguments_metadata;
} KSExtensionClassVirtualMethodInfo;

typedef void (*KSExtensionCallableCustomCall)(void *callable_userdata, const KSExtensionConstVariantPtr *p_args, KSExtensionInt p_argument_count, KSExtensionVariantPtr r_return, KSExtensionCallError *r_error);
typedef KSExtensionBool (*KSExtensionCallableCustomIsValid)(void *callable_userdata);
typedef void (*KSExtensionCallableCustomFree)(void *callable_userdata);

typedef uint32_t (*KSExtensionCallableCustomHash)(void *callable_userdata);
typedef KSExtensionBool (*KSExtensionCallableCustomEqual)(void *callable_userdata_a, void *callable_userdata_b);
typedef KSExtensionBool (*KSExtensionCallableCustomLessThan)(void *callable_userdata_a, void *callable_userdata_b);

typedef void (*KSExtensionCallableCustomToString)(void *callable_userdata, KSExtensionBool *r_is_valid, KSExtensionStringPtr r_out);

typedef KSExtensionInt (*KSExtensionCallableCustomGetArgumentCount)(void *callable_userdata, KSExtensionBool *r_is_valid);

typedef struct {
	/* Only `call_func` and `token` are strictly required, however, `object_id` should be passed if its not a static method.
	 *
	 * `token` should point to an address that uniquely identifies the KSExtension (for example, the
	 * `KSExtensionClassLibraryPtr` passed to the entry symbol function.
	 *
	 * `hash_func`, `equal_func`, and `less_than_func` are optional. If not provided both `call_func` and
	 * `callable_userdata` together are used as the identity of the callable for hashing and comparison purposes.
	 *
	 * The hash returned by `hash_func` is cached, `hash_func` will not be called more than once per callable.
	 *
	 * `is_valid_func` is necessary if the validity of the callable can change before destruction.
	 *
	 * `free_func` is necessary if `callable_userdata` needs to be cleaned up when the callable is freed.
	 */
	void *callable_userdata;
	void *token;

	KSObjectInstanceID object_id;

	KSExtensionCallableCustomCall call_func;
	KSExtensionCallableCustomIsValid is_valid_func;
	KSExtensionCallableCustomFree free_func;

	KSExtensionCallableCustomHash hash_func;
	KSExtensionCallableCustomEqual equal_func;
	KSExtensionCallableCustomLessThan less_than_func;

	KSExtensionCallableCustomToString to_string_func;
} KSExtensionCallableCustomInfo; // Deprecated. Use KSExtensionCallableCustomInfo2 instead.

typedef struct {
	/* Only `call_func` and `token` are strictly required, however, `object_id` should be passed if its not a static method.
	 *
	 * `token` should point to an address that uniquely identifies the KSExtension (for example, the
	 * `KSExtensionClassLibraryPtr` passed to the entry symbol function.
	 *
	 * `hash_func`, `equal_func`, and `less_than_func` are optional. If not provided both `call_func` and
	 * `callable_userdata` together are used as the identity of the callable for hashing and comparison purposes.
	 *
	 * The hash returned by `hash_func` is cached, `hash_func` will not be called more than once per callable.
	 *
	 * `is_valid_func` is necessary if the validity of the callable can change before destruction.
	 *
	 * `free_func` is necessary if `callable_userdata` needs to be cleaned up when the callable is freed.
	 */
	void *callable_userdata;
	void *token;

	KSObjectInstanceID object_id;

	KSExtensionCallableCustomCall call_func;
	KSExtensionCallableCustomIsValid is_valid_func;
	KSExtensionCallableCustomFree free_func;

	KSExtensionCallableCustomHash hash_func;
	KSExtensionCallableCustomEqual equal_func;
	KSExtensionCallableCustomLessThan less_than_func;

	KSExtensionCallableCustomToString to_string_func;

	KSExtensionCallableCustomGetArgumentCount get_argument_count_func;
} KSExtensionCallableCustomInfo2;

/* SCRIPT INSTANCE EXTENSION */

typedef void *KSExtensionScriptInstanceDataPtr; // Pointer to custom ScriptInstance native implementation.

typedef KSExtensionBool (*KSExtensionScriptInstanceSet)(KSExtensionScriptInstanceDataPtr p_instance, KSExtensionConstStringNamePtr p_name, KSExtensionConstVariantPtr p_value);
typedef KSExtensionBool (*KSExtensionScriptInstanceGet)(KSExtensionScriptInstanceDataPtr p_instance, KSExtensionConstStringNamePtr p_name, KSExtensionVariantPtr r_ret);
typedef const KSExtensionPropertyInfo *(*KSExtensionScriptInstanceGetPropertyList)(KSExtensionScriptInstanceDataPtr p_instance, uint32_t *r_count);
typedef void (*KSExtensionScriptInstanceFreePropertyList)(KSExtensionScriptInstanceDataPtr p_instance, const KSExtensionPropertyInfo *p_list); // Deprecated. Use KSExtensionScriptInstanceFreePropertyList2 instead.
typedef void (*KSExtensionScriptInstanceFreePropertyList2)(KSExtensionScriptInstanceDataPtr p_instance, const KSExtensionPropertyInfo *p_list, uint32_t p_count);
typedef KSExtensionBool (*KSExtensionScriptInstanceGetClassCategory)(KSExtensionScriptInstanceDataPtr p_instance, KSExtensionPropertyInfo *p_class_category);

typedef KSExtensionVariantType (*KSExtensionScriptInstanceGetPropertyType)(KSExtensionScriptInstanceDataPtr p_instance, KSExtensionConstStringNamePtr p_name, KSExtensionBool *r_is_valid);
typedef KSExtensionBool (*KSExtensionScriptInstanceValidateProperty)(KSExtensionScriptInstanceDataPtr p_instance, KSExtensionPropertyInfo *p_property);

typedef KSExtensionBool (*KSExtensionScriptInstancePropertyCanRevert)(KSExtensionScriptInstanceDataPtr p_instance, KSExtensionConstStringNamePtr p_name);
typedef KSExtensionBool (*KSExtensionScriptInstancePropertyGetRevert)(KSExtensionScriptInstanceDataPtr p_instance, KSExtensionConstStringNamePtr p_name, KSExtensionVariantPtr r_ret);

typedef KSExtensionObjectPtr (*KSExtensionScriptInstanceGetOwner)(KSExtensionScriptInstanceDataPtr p_instance);
typedef void (*KSExtensionScriptInstancePropertyStateAdd)(KSExtensionConstStringNamePtr p_name, KSExtensionConstVariantPtr p_value, void *p_userdata);
typedef void (*KSExtensionScriptInstanceGetPropertyState)(KSExtensionScriptInstanceDataPtr p_instance, KSExtensionScriptInstancePropertyStateAdd p_add_func, void *p_userdata);

typedef const KSExtensionMethodInfo *(*KSExtensionScriptInstanceGetMethodList)(KSExtensionScriptInstanceDataPtr p_instance, uint32_t *r_count);
typedef void (*KSExtensionScriptInstanceFreeMethodList)(KSExtensionScriptInstanceDataPtr p_instance, const KSExtensionMethodInfo *p_list); // Deprecated. Use KSExtensionScriptInstanceFreeMethodList2 instead.
typedef void (*KSExtensionScriptInstanceFreeMethodList2)(KSExtensionScriptInstanceDataPtr p_instance, const KSExtensionMethodInfo *p_list, uint32_t p_count);

typedef KSExtensionBool (*KSExtensionScriptInstanceHasMethod)(KSExtensionScriptInstanceDataPtr p_instance, KSExtensionConstStringNamePtr p_name);

typedef KSExtensionInt (*KSExtensionScriptInstanceGetMethodArgumentCount)(KSExtensionScriptInstanceDataPtr p_instance, KSExtensionConstStringNamePtr p_name, KSExtensionBool *r_is_valid);

typedef void (*KSExtensionScriptInstanceCall)(KSExtensionScriptInstanceDataPtr p_self, KSExtensionConstStringNamePtr p_method, const KSExtensionConstVariantPtr *p_args, KSExtensionInt p_argument_count, KSExtensionVariantPtr r_return, KSExtensionCallError *r_error);
typedef void (*KSExtensionScriptInstanceNotification)(KSExtensionScriptInstanceDataPtr p_instance, int32_t p_what); // Deprecated. Use KSExtensionScriptInstanceNotification2 instead.
typedef void (*KSExtensionScriptInstanceNotification2)(KSExtensionScriptInstanceDataPtr p_instance, int32_t p_what, KSExtensionBool p_reversed);
typedef void (*KSExtensionScriptInstanceToString)(KSExtensionScriptInstanceDataPtr p_instance, KSExtensionBool *r_is_valid, KSExtensionStringPtr r_out);

typedef void (*KSExtensionScriptInstanceRefCountIncremented)(KSExtensionScriptInstanceDataPtr p_instance);
typedef KSExtensionBool (*KSExtensionScriptInstanceRefCountDecremented)(KSExtensionScriptInstanceDataPtr p_instance);

typedef KSExtensionObjectPtr (*KSExtensionScriptInstanceGetScript)(KSExtensionScriptInstanceDataPtr p_instance);
typedef KSExtensionBool (*KSExtensionScriptInstanceIsPlaceholder)(KSExtensionScriptInstanceDataPtr p_instance);

typedef void *KSExtensionScriptLanguagePtr;

typedef KSExtensionScriptLanguagePtr (*KSExtensionScriptInstanceGetLanguage)(KSExtensionScriptInstanceDataPtr p_instance);

typedef void (*KSExtensionScriptInstanceFree)(KSExtensionScriptInstanceDataPtr p_instance);

typedef void *KSExtensionScriptInstancePtr; // Pointer to ScriptInstance.

typedef struct {
	KSExtensionScriptInstanceSet set_func;
	KSExtensionScriptInstanceGet get_func;
	KSExtensionScriptInstanceGetPropertyList get_property_list_func;
	KSExtensionScriptInstanceFreePropertyList free_property_list_func;

	KSExtensionScriptInstancePropertyCanRevert property_can_revert_func;
	KSExtensionScriptInstancePropertyGetRevert property_get_revert_func;

	KSExtensionScriptInstanceGetOwner get_owner_func;
	KSExtensionScriptInstanceGetPropertyState get_property_state_func;

	KSExtensionScriptInstanceGetMethodList get_method_list_func;
	KSExtensionScriptInstanceFreeMethodList free_method_list_func;
	KSExtensionScriptInstanceGetPropertyType get_property_type_func;

	KSExtensionScriptInstanceHasMethod has_method_func;

	KSExtensionScriptInstanceCall call_func;
	KSExtensionScriptInstanceNotification notification_func;

	KSExtensionScriptInstanceToString to_string_func;

	KSExtensionScriptInstanceRefCountIncremented refcount_incremented_func;
	KSExtensionScriptInstanceRefCountDecremented refcount_decremented_func;

	KSExtensionScriptInstanceGetScript get_script_func;

	KSExtensionScriptInstanceIsPlaceholder is_placeholder_func;

	KSExtensionScriptInstanceSet set_fallback_func;
	KSExtensionScriptInstanceGet get_fallback_func;

	KSExtensionScriptInstanceGetLanguage get_language_func;

	KSExtensionScriptInstanceFree free_func;

} KSExtensionScriptInstanceInfo; // Deprecated. Use KSExtensionScriptInstanceInfo3 instead.

typedef struct {
	KSExtensionScriptInstanceSet set_func;
	KSExtensionScriptInstanceGet get_func;
	KSExtensionScriptInstanceGetPropertyList get_property_list_func;
	KSExtensionScriptInstanceFreePropertyList free_property_list_func;
	KSExtensionScriptInstanceGetClassCategory get_class_category_func; // Optional. Set to NULL for the default behavior.

	KSExtensionScriptInstancePropertyCanRevert property_can_revert_func;
	KSExtensionScriptInstancePropertyGetRevert property_get_revert_func;

	KSExtensionScriptInstanceGetOwner get_owner_func;
	KSExtensionScriptInstanceGetPropertyState get_property_state_func;

	KSExtensionScriptInstanceGetMethodList get_method_list_func;
	KSExtensionScriptInstanceFreeMethodList free_method_list_func;
	KSExtensionScriptInstanceGetPropertyType get_property_type_func;
	KSExtensionScriptInstanceValidateProperty validate_property_func;

	KSExtensionScriptInstanceHasMethod has_method_func;

	KSExtensionScriptInstanceCall call_func;
	KSExtensionScriptInstanceNotification2 notification_func;

	KSExtensionScriptInstanceToString to_string_func;

	KSExtensionScriptInstanceRefCountIncremented refcount_incremented_func;
	KSExtensionScriptInstanceRefCountDecremented refcount_decremented_func;

	KSExtensionScriptInstanceGetScript get_script_func;

	KSExtensionScriptInstanceIsPlaceholder is_placeholder_func;

	KSExtensionScriptInstanceSet set_fallback_func;
	KSExtensionScriptInstanceGet get_fallback_func;

	KSExtensionScriptInstanceGetLanguage get_language_func;

	KSExtensionScriptInstanceFree free_func;

} KSExtensionScriptInstanceInfo2; // Deprecated. Use KSExtensionScriptInstanceInfo3 instead.

typedef struct {
	KSExtensionScriptInstanceSet set_func;
	KSExtensionScriptInstanceGet get_func;
	KSExtensionScriptInstanceGetPropertyList get_property_list_func;
	KSExtensionScriptInstanceFreePropertyList2 free_property_list_func;
	KSExtensionScriptInstanceGetClassCategory get_class_category_func; // Optional. Set to NULL for the default behavior.

	KSExtensionScriptInstancePropertyCanRevert property_can_revert_func;
	KSExtensionScriptInstancePropertyGetRevert property_get_revert_func;

	KSExtensionScriptInstanceGetOwner get_owner_func;
	KSExtensionScriptInstanceGetPropertyState get_property_state_func;

	KSExtensionScriptInstanceGetMethodList get_method_list_func;
	KSExtensionScriptInstanceFreeMethodList2 free_method_list_func;
	KSExtensionScriptInstanceGetPropertyType get_property_type_func;
	KSExtensionScriptInstanceValidateProperty validate_property_func;

	KSExtensionScriptInstanceHasMethod has_method_func;

	KSExtensionScriptInstanceGetMethodArgumentCount get_method_argument_count_func;

	KSExtensionScriptInstanceCall call_func;
	KSExtensionScriptInstanceNotification2 notification_func;

	KSExtensionScriptInstanceToString to_string_func;

	KSExtensionScriptInstanceRefCountIncremented refcount_incremented_func;
	KSExtensionScriptInstanceRefCountDecremented refcount_decremented_func;

	KSExtensionScriptInstanceGetScript get_script_func;

	KSExtensionScriptInstanceIsPlaceholder is_placeholder_func;

	KSExtensionScriptInstanceSet set_fallback_func;
	KSExtensionScriptInstanceGet get_fallback_func;

	KSExtensionScriptInstanceGetLanguage get_language_func;

	KSExtensionScriptInstanceFree free_func;

} KSExtensionScriptInstanceInfo3;

/* INITIALIZATION */

typedef enum {
	KSEXTENSION_INITIALIZATION_CORE,
	KSEXTENSION_INITIALIZATION_SERVERS,
	KSEXTENSION_INITIALIZATION_SCENE,
	KSEXTENSION_INITIALIZATION_EDITOR,
	KSEXTENSION_MAX_INITIALIZATION_LEVEL,
} KSExtensionInitializationLevel;

typedef struct {
	/* Minimum initialization level required.
	 * If Core or Servers, the extension needs editor or game restart to take effect */
	KSExtensionInitializationLevel minimum_initialization_level;
	/* Up to the user to supply when initializing */
	void *userdata;
	/* This function will be called multiple times for each initialization level. */
	void (*initialize)(void *userdata, KSExtensionInitializationLevel p_level);
	void (*deinitialize)(void *userdata, KSExtensionInitializationLevel p_level);
} KSExtensionInitialization;

typedef void (*KSExtensionInterfaceFunctionPtr)();
typedef KSExtensionInterfaceFunctionPtr (*KSExtensionInterfaceGetProcAddress)(const char *p_function_name);

/*
 * Each KSExtension should define a C function that matches the signature of KSExtensionInitializationFunction,
 * and export it so that it can be loaded via dlopen() or equivalent for the given platform.
 *
 * For example:
 *
 *   KSExtensionBool my_extension_init(KSExtensionInterfaceGetProcAddress p_get_proc_address, KSExtensionClassLibraryPtr p_library, KSExtensionInitialization *r_initialization);
 *
 * This function's name must be specified as the 'entry_symbol' in the .ksextension file.
 *
 * This makes it the entry point of the KSExtension and will be called on initialization.
 *
 * The KSExtension can then modify the r_initialization structure, setting the minimum initialization level,
 * and providing pointers to functions that will be called at various stages of initialization/shutdown.
 *
 * The rest of the KSExtension's interface to Kosmic consists of function pointers that can be loaded
 * by calling p_get_proc_address("...") with the name of the function.
 *
 * For example:
 *
 *   KSExtensionInterfaceGetKosmicVersion get_kosmic_version = (KSExtensionInterfaceGetKosmicVersion)p_get_proc_address("get_kosmic_version");
 *
 * (Note that snippet may cause "cast between incompatible function types" on some compilers, you can
 * silence this by adding an intermediary `void*` cast.)
 *
 * You can then call it like a normal function:
 *
 *   KSExtensionKosmicVersion kosmic_version;
 *   get_kosmic_version(&kosmic_version);
 *   printf("Kosmic v%d.%d.%d\n", kosmic_version.major, kosmic_version.minor, kosmic_version.patch);
 *
 * All of these interface functions are described below, together with the name that's used to load it,
 * and the function pointer typedef that shows its signature.
 */
typedef KSExtensionBool (*KSExtensionInitializationFunction)(KSExtensionInterfaceGetProcAddress p_get_proc_address, KSExtensionClassLibraryPtr p_library, KSExtensionInitialization *r_initialization);

/* INTERFACE */

typedef struct {
	uint32_t major;
	uint32_t minor;
	uint32_t patch;
	const char *string;
} KSExtensionKosmicVersion;

/**
 * @name get_kosmic_version
 * @since 4.1
 *
 * Gets the Kosmic version that the KSExtension was loaded into.
 *
 * @param r_kosmic_version A pointer to the structure to write the version information into.
 */
typedef void (*KSExtensionInterfaceGetKosmicVersion)(KSExtensionKosmicVersion *r_kosmic_version);

/* INTERFACE: Memory */

/**
 * @name mem_alloc
 * @since 4.1
 *
 * Allocates memory.
 *
 * @param p_bytes The amount of memory to allocate in bytes.
 *
 * @return A pointer to the allocated memory, or NULL if unsuccessful.
 */
typedef void *(*KSExtensionInterfaceMemAlloc)(size_t p_bytes);

/**
 * @name mem_realloc
 * @since 4.1
 *
 * Reallocates memory.
 *
 * @param p_ptr A pointer to the previously allocated memory.
 * @param p_bytes The number of bytes to resize the memory block to.
 *
 * @return A pointer to the allocated memory, or NULL if unsuccessful.
 */
typedef void *(*KSExtensionInterfaceMemRealloc)(void *p_ptr, size_t p_bytes);

/**
 * @name mem_free
 * @since 4.1
 *
 * Frees memory.
 *
 * @param p_ptr A pointer to the previously allocated memory.
 */
typedef void (*KSExtensionInterfaceMemFree)(void *p_ptr);

/* INTERFACE: Kosmic Core */

/**
 * @name print_error
 * @since 4.1
 *
 * Logs an error to Kosmic's built-in debugger and to the OS terminal.
 *
 * @param p_description The code triggering the error.
 * @param p_function The function name where the error occurred.
 * @param p_file The file where the error occurred.
 * @param p_line The line where the error occurred.
 * @param p_editor_notify Whether or not to notify the editor.
 */
typedef void (*KSExtensionInterfacePrintError)(const char *p_description, const char *p_function, const char *p_file, int32_t p_line, KSExtensionBool p_editor_notify);

/**
 * @name print_error_with_message
 * @since 4.1
 *
 * Logs an error with a message to Kosmic's built-in debugger and to the OS terminal.
 *
 * @param p_description The code triggering the error.
 * @param p_message The message to show along with the error.
 * @param p_function The function name where the error occurred.
 * @param p_file The file where the error occurred.
 * @param p_line The line where the error occurred.
 * @param p_editor_notify Whether or not to notify the editor.
 */
typedef void (*KSExtensionInterfacePrintErrorWithMessage)(const char *p_description, const char *p_message, const char *p_function, const char *p_file, int32_t p_line, KSExtensionBool p_editor_notify);

/**
 * @name print_warning
 * @since 4.1
 *
 * Logs a warning to Kosmic's built-in debugger and to the OS terminal.
 *
 * @param p_description The code triggering the warning.
 * @param p_function The function name where the warning occurred.
 * @param p_file The file where the warning occurred.
 * @param p_line The line where the warning occurred.
 * @param p_editor_notify Whether or not to notify the editor.
 */
typedef void (*KSExtensionInterfacePrintWarning)(const char *p_description, const char *p_function, const char *p_file, int32_t p_line, KSExtensionBool p_editor_notify);

/**
 * @name print_warning_with_message
 * @since 4.1
 *
 * Logs a warning with a message to Kosmic's built-in debugger and to the OS terminal.
 *
 * @param p_description The code triggering the warning.
 * @param p_message The message to show along with the warning.
 * @param p_function The function name where the warning occurred.
 * @param p_file The file where the warning occurred.
 * @param p_line The line where the warning occurred.
 * @param p_editor_notify Whether or not to notify the editor.
 */
typedef void (*KSExtensionInterfacePrintWarningWithMessage)(const char *p_description, const char *p_message, const char *p_function, const char *p_file, int32_t p_line, KSExtensionBool p_editor_notify);

/**
 * @name print_script_error
 * @since 4.1
 *
 * Logs a script error to Kosmic's built-in debugger and to the OS terminal.
 *
 * @param p_description The code triggering the error.
 * @param p_function The function name where the error occurred.
 * @param p_file The file where the error occurred.
 * @param p_line The line where the error occurred.
 * @param p_editor_notify Whether or not to notify the editor.
 */
typedef void (*KSExtensionInterfacePrintScriptError)(const char *p_description, const char *p_function, const char *p_file, int32_t p_line, KSExtensionBool p_editor_notify);

/**
 * @name print_script_error_with_message
 * @since 4.1
 *
 * Logs a script error with a message to Kosmic's built-in debugger and to the OS terminal.
 *
 * @param p_description The code triggering the error.
 * @param p_message The message to show along with the error.
 * @param p_function The function name where the error occurred.
 * @param p_file The file where the error occurred.
 * @param p_line The line where the error occurred.
 * @param p_editor_notify Whether or not to notify the editor.
 */
typedef void (*KSExtensionInterfacePrintScriptErrorWithMessage)(const char *p_description, const char *p_message, const char *p_function, const char *p_file, int32_t p_line, KSExtensionBool p_editor_notify);

/**
 * @name get_native_struct_size
 * @since 4.1
 *
 * Gets the size of a native struct (ex. ObjectID) in bytes.
 *
 * @param p_name A pointer to a StringName identifying the struct name.
 *
 * @return The size in bytes.
 */
typedef uint64_t (*KSExtensionInterfaceGetNativeStructSize)(KSExtensionConstStringNamePtr p_name);

/* INTERFACE: Variant */

/**
 * @name variant_new_copy
 * @since 4.1
 *
 * Copies one Variant into a another.
 *
 * @param r_dest A pointer to the destination Variant.
 * @param p_src A pointer to the source Variant.
 */
typedef void (*KSExtensionInterfaceVariantNewCopy)(KSExtensionUninitializedVariantPtr r_dest, KSExtensionConstVariantPtr p_src);

/**
 * @name variant_new_nil
 * @since 4.1
 *
 * Creates a new Variant containing nil.
 *
 * @param r_dest A pointer to the destination Variant.
 */
typedef void (*KSExtensionInterfaceVariantNewNil)(KSExtensionUninitializedVariantPtr r_dest);

/**
 * @name variant_destroy
 * @since 4.1
 *
 * Destroys a Variant.
 *
 * @param p_self A pointer to the Variant to destroy.
 */
typedef void (*KSExtensionInterfaceVariantDestroy)(KSExtensionVariantPtr p_self);

/**
 * @name variant_call
 * @since 4.1
 *
 * Calls a method on a Variant.
 *
 * @param p_self A pointer to the Variant.
 * @param p_method A pointer to a StringName identifying the method.
 * @param p_args A pointer to a C array of Variant.
 * @param p_argument_count The number of arguments.
 * @param r_return A pointer a Variant which will be assigned the return value.
 * @param r_error A pointer the structure which will hold error information.
 *
 * @see Variant::callp()
 */
typedef void (*KSExtensionInterfaceVariantCall)(KSExtensionVariantPtr p_self, KSExtensionConstStringNamePtr p_method, const KSExtensionConstVariantPtr *p_args, KSExtensionInt p_argument_count, KSExtensionUninitializedVariantPtr r_return, KSExtensionCallError *r_error);

/**
 * @name variant_call_static
 * @since 4.1
 *
 * Calls a static method on a Variant.
 *
 * @param p_self A pointer to the Variant.
 * @param p_method A pointer to a StringName identifying the method.
 * @param p_args A pointer to a C array of Variant.
 * @param p_argument_count The number of arguments.
 * @param r_return A pointer a Variant which will be assigned the return value.
 * @param r_error A pointer the structure which will be updated with error information.
 *
 * @see Variant::call_static()
 */
typedef void (*KSExtensionInterfaceVariantCallStatic)(KSExtensionVariantType p_type, KSExtensionConstStringNamePtr p_method, const KSExtensionConstVariantPtr *p_args, KSExtensionInt p_argument_count, KSExtensionUninitializedVariantPtr r_return, KSExtensionCallError *r_error);

/**
 * @name variant_evaluate
 * @since 4.1
 *
 * Evaluate an operator on two Variants.
 *
 * @param p_op The operator to evaluate.
 * @param p_a The first Variant.
 * @param p_b The second Variant.
 * @param r_return A pointer a Variant which will be assigned the return value.
 * @param r_valid A pointer to a boolean which will be set to false if the operation is invalid.
 *
 * @see Variant::evaluate()
 */
typedef void (*KSExtensionInterfaceVariantEvaluate)(KSExtensionVariantOperator p_op, KSExtensionConstVariantPtr p_a, KSExtensionConstVariantPtr p_b, KSExtensionUninitializedVariantPtr r_return, KSExtensionBool *r_valid);

/**
 * @name variant_set
 * @since 4.1
 *
 * Sets a key on a Variant to a value.
 *
 * @param p_self A pointer to the Variant.
 * @param p_key A pointer to a Variant representing the key.
 * @param p_value A pointer to a Variant representing the value.
 * @param r_valid A pointer to a boolean which will be set to false if the operation is invalid.
 *
 * @see Variant::set()
 */
typedef void (*KSExtensionInterfaceVariantSet)(KSExtensionVariantPtr p_self, KSExtensionConstVariantPtr p_key, KSExtensionConstVariantPtr p_value, KSExtensionBool *r_valid);

/**
 * @name variant_set_named
 * @since 4.1
 *
 * Sets a named key on a Variant to a value.
 *
 * @param p_self A pointer to the Variant.
 * @param p_key A pointer to a StringName representing the key.
 * @param p_value A pointer to a Variant representing the value.
 * @param r_valid A pointer to a boolean which will be set to false if the operation is invalid.
 *
 * @see Variant::set_named()
 */
typedef void (*KSExtensionInterfaceVariantSetNamed)(KSExtensionVariantPtr p_self, KSExtensionConstStringNamePtr p_key, KSExtensionConstVariantPtr p_value, KSExtensionBool *r_valid);

/**
 * @name variant_set_keyed
 * @since 4.1
 *
 * Sets a keyed property on a Variant to a value.
 *
 * @param p_self A pointer to the Variant.
 * @param p_key A pointer to a Variant representing the key.
 * @param p_value A pointer to a Variant representing the value.
 * @param r_valid A pointer to a boolean which will be set to false if the operation is invalid.
 *
 * @see Variant::set_keyed()
 */
typedef void (*KSExtensionInterfaceVariantSetKeyed)(KSExtensionVariantPtr p_self, KSExtensionConstVariantPtr p_key, KSExtensionConstVariantPtr p_value, KSExtensionBool *r_valid);

/**
 * @name variant_set_indexed
 * @since 4.1
 *
 * Sets an index on a Variant to a value.
 *
 * @param p_self A pointer to the Variant.
 * @param p_index The index.
 * @param p_value A pointer to a Variant representing the value.
 * @param r_valid A pointer to a boolean which will be set to false if the operation is invalid.
 * @param r_oob A pointer to a boolean which will be set to true if the index is out of bounds.
 */
typedef void (*KSExtensionInterfaceVariantSetIndexed)(KSExtensionVariantPtr p_self, KSExtensionInt p_index, KSExtensionConstVariantPtr p_value, KSExtensionBool *r_valid, KSExtensionBool *r_oob);

/**
 * @name variant_get
 * @since 4.1
 *
 * Gets the value of a key from a Variant.
 *
 * @param p_self A pointer to the Variant.
 * @param p_key A pointer to a Variant representing the key.
 * @param r_ret A pointer to a Variant which will be assigned the value.
 * @param r_valid A pointer to a boolean which will be set to false if the operation is invalid.
 */
typedef void (*KSExtensionInterfaceVariantGet)(KSExtensionConstVariantPtr p_self, KSExtensionConstVariantPtr p_key, KSExtensionUninitializedVariantPtr r_ret, KSExtensionBool *r_valid);

/**
 * @name variant_get_named
 * @since 4.1
 *
 * Gets the value of a named key from a Variant.
 *
 * @param p_self A pointer to the Variant.
 * @param p_key A pointer to a StringName representing the key.
 * @param r_ret A pointer to a Variant which will be assigned the value.
 * @param r_valid A pointer to a boolean which will be set to false if the operation is invalid.
 */
typedef void (*KSExtensionInterfaceVariantGetNamed)(KSExtensionConstVariantPtr p_self, KSExtensionConstStringNamePtr p_key, KSExtensionUninitializedVariantPtr r_ret, KSExtensionBool *r_valid);

/**
 * @name variant_get_keyed
 * @since 4.1
 *
 * Gets the value of a keyed property from a Variant.
 *
 * @param p_self A pointer to the Variant.
 * @param p_key A pointer to a Variant representing the key.
 * @param r_ret A pointer to a Variant which will be assigned the value.
 * @param r_valid A pointer to a boolean which will be set to false if the operation is invalid.
 */
typedef void (*KSExtensionInterfaceVariantGetKeyed)(KSExtensionConstVariantPtr p_self, KSExtensionConstVariantPtr p_key, KSExtensionUninitializedVariantPtr r_ret, KSExtensionBool *r_valid);

/**
 * @name variant_get_indexed
 * @since 4.1
 *
 * Gets the value of an index from a Variant.
 *
 * @param p_self A pointer to the Variant.
 * @param p_index The index.
 * @param r_ret A pointer to a Variant which will be assigned the value.
 * @param r_valid A pointer to a boolean which will be set to false if the operation is invalid.
 * @param r_oob A pointer to a boolean which will be set to true if the index is out of bounds.
 */
typedef void (*KSExtensionInterfaceVariantGetIndexed)(KSExtensionConstVariantPtr p_self, KSExtensionInt p_index, KSExtensionUninitializedVariantPtr r_ret, KSExtensionBool *r_valid, KSExtensionBool *r_oob);

/**
 * @name variant_iter_init
 * @since 4.1
 *
 * Initializes an iterator over a Variant.
 *
 * @param p_self A pointer to the Variant.
 * @param r_iter A pointer to a Variant which will be assigned the iterator.
 * @param r_valid A pointer to a boolean which will be set to false if the operation is invalid.
 *
 * @return true if the operation is valid; otherwise false.
 *
 * @see Variant::iter_init()
 */
typedef KSExtensionBool (*KSExtensionInterfaceVariantIterInit)(KSExtensionConstVariantPtr p_self, KSExtensionUninitializedVariantPtr r_iter, KSExtensionBool *r_valid);

/**
 * @name variant_iter_next
 * @since 4.1
 *
 * Gets the next value for an iterator over a Variant.
 *
 * @param p_self A pointer to the Variant.
 * @param r_iter A pointer to a Variant which will be assigned the iterator.
 * @param r_valid A pointer to a boolean which will be set to false if the operation is invalid.
 *
 * @return true if the operation is valid; otherwise false.
 *
 * @see Variant::iter_next()
 */
typedef KSExtensionBool (*KSExtensionInterfaceVariantIterNext)(KSExtensionConstVariantPtr p_self, KSExtensionVariantPtr r_iter, KSExtensionBool *r_valid);

/**
 * @name variant_iter_get
 * @since 4.1
 *
 * Gets the next value for an iterator over a Variant.
 *
 * @param p_self A pointer to the Variant.
 * @param r_iter A pointer to a Variant which will be assigned the iterator.
 * @param r_ret A pointer to a Variant which will be assigned false if the operation is invalid.
 * @param r_valid A pointer to a boolean which will be set to false if the operation is invalid.
 *
 * @see Variant::iter_get()
 */
typedef void (*KSExtensionInterfaceVariantIterGet)(KSExtensionConstVariantPtr p_self, KSExtensionVariantPtr r_iter, KSExtensionUninitializedVariantPtr r_ret, KSExtensionBool *r_valid);

/**
 * @name variant_hash
 * @since 4.1
 *
 * Gets the hash of a Variant.
 *
 * @param p_self A pointer to the Variant.
 *
 * @return The hash value.
 *
 * @see Variant::hash()
 */
typedef KSExtensionInt (*KSExtensionInterfaceVariantHash)(KSExtensionConstVariantPtr p_self);

/**
 * @name variant_recursive_hash
 * @since 4.1
 *
 * Gets the recursive hash of a Variant.
 *
 * @param p_self A pointer to the Variant.
 * @param p_recursion_count The number of recursive loops so far.
 *
 * @return The hash value.
 *
 * @see Variant::recursive_hash()
 */
typedef KSExtensionInt (*KSExtensionInterfaceVariantRecursiveHash)(KSExtensionConstVariantPtr p_self, KSExtensionInt p_recursion_count);

/**
 * @name variant_hash_compare
 * @since 4.1
 *
 * Compares two Variants by their hash.
 *
 * @param p_self A pointer to the Variant.
 * @param p_other A pointer to the other Variant to compare it to.
 *
 * @return The hash value.
 *
 * @see Variant::hash_compare()
 */
typedef KSExtensionBool (*KSExtensionInterfaceVariantHashCompare)(KSExtensionConstVariantPtr p_self, KSExtensionConstVariantPtr p_other);

/**
 * @name variant_booleanize
 * @since 4.1
 *
 * Converts a Variant to a boolean.
 *
 * @param p_self A pointer to the Variant.
 *
 * @return The boolean value of the Variant.
 */
typedef KSExtensionBool (*KSExtensionInterfaceVariantBooleanize)(KSExtensionConstVariantPtr p_self);

/**
 * @name variant_duplicate
 * @since 4.1
 *
 * Duplicates a Variant.
 *
 * @param p_self A pointer to the Variant.
 * @param r_ret A pointer to a Variant to store the duplicated value.
 * @param p_deep Whether or not to duplicate deeply (when supported by the Variant type).
 */
typedef void (*KSExtensionInterfaceVariantDuplicate)(KSExtensionConstVariantPtr p_self, KSExtensionVariantPtr r_ret, KSExtensionBool p_deep);

/**
 * @name variant_stringify
 * @since 4.1
 *
 * Converts a Variant to a string.
 *
 * @param p_self A pointer to the Variant.
 * @param r_ret A pointer to a String to store the resulting value.
 */
typedef void (*KSExtensionInterfaceVariantStringify)(KSExtensionConstVariantPtr p_self, KSExtensionStringPtr r_ret);

/**
 * @name variant_get_type
 * @since 4.1
 *
 * Gets the type of a Variant.
 *
 * @param p_self A pointer to the Variant.
 *
 * @return The variant type.
 */
typedef KSExtensionVariantType (*KSExtensionInterfaceVariantGetType)(KSExtensionConstVariantPtr p_self);

/**
 * @name variant_has_method
 * @since 4.1
 *
 * Checks if a Variant has the given method.
 *
 * @param p_self A pointer to the Variant.
 * @param p_method A pointer to a StringName with the method name.
 *
 * @return
 */
typedef KSExtensionBool (*KSExtensionInterfaceVariantHasMethod)(KSExtensionConstVariantPtr p_self, KSExtensionConstStringNamePtr p_method);

/**
 * @name variant_has_member
 * @since 4.1
 *
 * Checks if a type of Variant has the given member.
 *
 * @param p_type The Variant type.
 * @param p_member A pointer to a StringName with the member name.
 *
 * @return
 */
typedef KSExtensionBool (*KSExtensionInterfaceVariantHasMember)(KSExtensionVariantType p_type, KSExtensionConstStringNamePtr p_member);

/**
 * @name variant_has_key
 * @since 4.1
 *
 * Checks if a Variant has a key.
 *
 * @param p_self A pointer to the Variant.
 * @param p_key A pointer to a Variant representing the key.
 * @param r_valid A pointer to a boolean which will be set to false if the key doesn't exist.
 *
 * @return true if the key exists; otherwise false.
 */
typedef KSExtensionBool (*KSExtensionInterfaceVariantHasKey)(KSExtensionConstVariantPtr p_self, KSExtensionConstVariantPtr p_key, KSExtensionBool *r_valid);

/**
 * @name variant_get_object_instance_id
 * @since 4.4
 *
 * Gets the object instance ID from a variant of type KSEXTENSION_VARIANT_TYPE_OBJECT.
 *
 * If the variant isn't of type KSEXTENSION_VARIANT_TYPE_OBJECT, then zero will be returned.
 * The instance ID will be returned even if the object is no longer valid - use `object_get_instance_by_id()` to check if the object is still valid.
 *
 * @param p_self A pointer to the Variant.
 *
 * @return The instance ID for the contained object.
 */
typedef KSObjectInstanceID (*KSExtensionInterfaceVariantGetObjectInstanceId)(KSExtensionConstVariantPtr p_self);

/**
 * @name variant_get_type_name
 * @since 4.1
 *
 * Gets the name of a Variant type.
 *
 * @param p_type The Variant type.
 * @param r_name A pointer to a String to store the Variant type name.
 */
typedef void (*KSExtensionInterfaceVariantGetTypeName)(KSExtensionVariantType p_type, KSExtensionUninitializedStringPtr r_name);

/**
 * @name variant_can_convert
 * @since 4.1
 *
 * Checks if Variants can be converted from one type to another.
 *
 * @param p_from The Variant type to convert from.
 * @param p_to The Variant type to convert to.
 *
 * @return true if the conversion is possible; otherwise false.
 */
typedef KSExtensionBool (*KSExtensionInterfaceVariantCanConvert)(KSExtensionVariantType p_from, KSExtensionVariantType p_to);

/**
 * @name variant_can_convert_strict
 * @since 4.1
 *
 * Checks if Variant can be converted from one type to another using stricter rules.
 *
 * @param p_from The Variant type to convert from.
 * @param p_to The Variant type to convert to.
 *
 * @return true if the conversion is possible; otherwise false.
 */
typedef KSExtensionBool (*KSExtensionInterfaceVariantCanConvertStrict)(KSExtensionVariantType p_from, KSExtensionVariantType p_to);

/**
 * @name get_variant_from_type_constructor
 * @since 4.1
 *
 * Gets a pointer to a function that can create a Variant of the given type from a raw value.
 *
 * @param p_type The Variant type.
 *
 * @return A pointer to a function that can create a Variant of the given type from a raw value.
 */
typedef KSExtensionVariantFromTypeConstructorFunc (*KSExtensionInterfaceGetVariantFromTypeConstructor)(KSExtensionVariantType p_type);

/**
 * @name get_variant_to_type_constructor
 * @since 4.1
 *
 * Gets a pointer to a function that can get the raw value from a Variant of the given type.
 *
 * @param p_type The Variant type.
 *
 * @return A pointer to a function that can get the raw value from a Variant of the given type.
 */
typedef KSExtensionTypeFromVariantConstructorFunc (*KSExtensionInterfaceGetVariantToTypeConstructor)(KSExtensionVariantType p_type);

/**
 * @name variant_get_ptr_internal_getter
 * @since 4.4
 *
 * Provides a function pointer for retrieving a pointer to a variant's internal value.
 * Access to a variant's internal value can be used to modify it in-place, or to retrieve its value without the overhead of variant conversion functions.
 * It is recommended to cache the getter for all variant types in a function table to avoid retrieval overhead upon use.
 *
 * @note Each function assumes the variant's type has already been determined and matches the function.
 * Invoking the function with a variant of a mismatched type has undefined behavior, and may lead to a segmentation fault.
 *
 * @param p_type The Variant type.
 *
 * @return A pointer to a type-specific function that returns a pointer to the internal value of a variant. Check the implementation of this function (ksextension_variant_get_ptr_internal_getter) for pointee type info of each variant type.
 */
typedef KSExtensionVariantGetInternalPtrFunc (*KSExtensionInterfaceGetVariantGetInternalPtrFunc)(KSExtensionVariantType p_type);

/**
 * @name variant_get_ptr_operator_evaluator
 * @since 4.1
 *
 * Gets a pointer to a function that can evaluate the given Variant operator on the given Variant types.
 *
 * @param p_operator The variant operator.
 * @param p_type_a The type of the first Variant.
 * @param p_type_b The type of the second Variant.
 *
 * @return A pointer to a function that can evaluate the given Variant operator on the given Variant types.
 */
typedef KSExtensionPtrOperatorEvaluator (*KSExtensionInterfaceVariantGetPtrOperatorEvaluator)(KSExtensionVariantOperator p_operator, KSExtensionVariantType p_type_a, KSExtensionVariantType p_type_b);

/**
 * @name variant_get_ptr_builtin_method
 * @since 4.1
 *
 * Gets a pointer to a function that can call a builtin method on a type of Variant.
 *
 * @param p_type The Variant type.
 * @param p_method A pointer to a StringName with the method name.
 * @param p_hash A hash representing the method signature.
 *
 * @return A pointer to a function that can call a builtin method on a type of Variant.
 */
typedef KSExtensionPtrBuiltInMethod (*KSExtensionInterfaceVariantGetPtrBuiltinMethod)(KSExtensionVariantType p_type, KSExtensionConstStringNamePtr p_method, KSExtensionInt p_hash);

/**
 * @name variant_get_ptr_constructor
 * @since 4.1
 *
 * Gets a pointer to a function that can call one of the constructors for a type of Variant.
 *
 * @param p_type The Variant type.
 * @param p_constructor The index of the constructor.
 *
 * @return A pointer to a function that can call one of the constructors for a type of Variant.
 */
typedef KSExtensionPtrConstructor (*KSExtensionInterfaceVariantGetPtrConstructor)(KSExtensionVariantType p_type, int32_t p_constructor);

/**
 * @name variant_get_ptr_destructor
 * @since 4.1
 *
 * Gets a pointer to a function than can call the destructor for a type of Variant.
 *
 * @param p_type The Variant type.
 *
 * @return A pointer to a function than can call the destructor for a type of Variant.
 */
typedef KSExtensionPtrDestructor (*KSExtensionInterfaceVariantGetPtrDestructor)(KSExtensionVariantType p_type);

/**
 * @name variant_construct
 * @since 4.1
 *
 * Constructs a Variant of the given type, using the first constructor that matches the given arguments.
 *
 * @param p_type The Variant type.
 * @param p_base A pointer to a Variant to store the constructed value.
 * @param p_args A pointer to a C array of Variant pointers representing the arguments for the constructor.
 * @param p_argument_count The number of arguments to pass to the constructor.
 * @param r_error A pointer the structure which will be updated with error information.
 */
typedef void (*KSExtensionInterfaceVariantConstruct)(KSExtensionVariantType p_type, KSExtensionUninitializedVariantPtr r_base, const KSExtensionConstVariantPtr *p_args, int32_t p_argument_count, KSExtensionCallError *r_error);

/**
 * @name variant_get_ptr_setter
 * @since 4.1
 *
 * Gets a pointer to a function that can call a member's setter on the given Variant type.
 *
 * @param p_type The Variant type.
 * @param p_member A pointer to a StringName with the member name.
 *
 * @return A pointer to a function that can call a member's setter on the given Variant type.
 */
typedef KSExtensionPtrSetter (*KSExtensionInterfaceVariantGetPtrSetter)(KSExtensionVariantType p_type, KSExtensionConstStringNamePtr p_member);

/**
 * @name variant_get_ptr_getter
 * @since 4.1
 *
 * Gets a pointer to a function that can call a member's getter on the given Variant type.
 *
 * @param p_type The Variant type.
 * @param p_member A pointer to a StringName with the member name.
 *
 * @return A pointer to a function that can call a member's getter on the given Variant type.
 */
typedef KSExtensionPtrGetter (*KSExtensionInterfaceVariantGetPtrGetter)(KSExtensionVariantType p_type, KSExtensionConstStringNamePtr p_member);

/**
 * @name variant_get_ptr_indexed_setter
 * @since 4.1
 *
 * Gets a pointer to a function that can set an index on the given Variant type.
 *
 * @param p_type The Variant type.
 *
 * @return A pointer to a function that can set an index on the given Variant type.
 */
typedef KSExtensionPtrIndexedSetter (*KSExtensionInterfaceVariantGetPtrIndexedSetter)(KSExtensionVariantType p_type);

/**
 * @name variant_get_ptr_indexed_getter
 * @since 4.1
 *
 * Gets a pointer to a function that can get an index on the given Variant type.
 *
 * @param p_type The Variant type.
 *
 * @return A pointer to a function that can get an index on the given Variant type.
 */
typedef KSExtensionPtrIndexedGetter (*KSExtensionInterfaceVariantGetPtrIndexedGetter)(KSExtensionVariantType p_type);

/**
 * @name variant_get_ptr_keyed_setter
 * @since 4.1
 *
 * Gets a pointer to a function that can set a key on the given Variant type.
 *
 * @param p_type The Variant type.
 *
 * @return A pointer to a function that can set a key on the given Variant type.
 */
typedef KSExtensionPtrKeyedSetter (*KSExtensionInterfaceVariantGetPtrKeyedSetter)(KSExtensionVariantType p_type);

/**
 * @name variant_get_ptr_keyed_getter
 * @since 4.1
 *
 * Gets a pointer to a function that can get a key on the given Variant type.
 *
 * @param p_type The Variant type.
 *
 * @return A pointer to a function that can get a key on the given Variant type.
 */
typedef KSExtensionPtrKeyedGetter (*KSExtensionInterfaceVariantGetPtrKeyedGetter)(KSExtensionVariantType p_type);

/**
 * @name variant_get_ptr_keyed_checker
 * @since 4.1
 *
 * Gets a pointer to a function that can check a key on the given Variant type.
 *
 * @param p_type The Variant type.
 *
 * @return A pointer to a function that can check a key on the given Variant type.
 */
typedef KSExtensionPtrKeyedChecker (*KSExtensionInterfaceVariantGetPtrKeyedChecker)(KSExtensionVariantType p_type);

/**
 * @name variant_get_constant_value
 * @since 4.1
 *
 * Gets the value of a constant from the given Variant type.
 *
 * @param p_type The Variant type.
 * @param p_constant A pointer to a StringName with the constant name.
 * @param r_ret A pointer to a Variant to store the value.
 */
typedef void (*KSExtensionInterfaceVariantGetConstantValue)(KSExtensionVariantType p_type, KSExtensionConstStringNamePtr p_constant, KSExtensionUninitializedVariantPtr r_ret);

/**
 * @name variant_get_ptr_utility_function
 * @since 4.1
 *
 * Gets a pointer to a function that can call a Variant utility function.
 *
 * @param p_function A pointer to a StringName with the function name.
 * @param p_hash A hash representing the function signature.
 *
 * @return A pointer to a function that can call a Variant utility function.
 */
typedef KSExtensionPtrUtilityFunction (*KSExtensionInterfaceVariantGetPtrUtilityFunction)(KSExtensionConstStringNamePtr p_function, KSExtensionInt p_hash);

/* INTERFACE: String Utilities */

/**
 * @name string_new_with_latin1_chars
 * @since 4.1
 *
 * Creates a String from a Latin-1 encoded C string.
 *
 * @param r_dest A pointer to a Variant to hold the newly created String.
 * @param p_contents A pointer to a Latin-1 encoded C string (null terminated).
 */
typedef void (*KSExtensionInterfaceStringNewWithLatin1Chars)(KSExtensionUninitializedStringPtr r_dest, const char *p_contents);

/**
 * @name string_new_with_utf8_chars
 * @since 4.1
 *
 * Creates a String from a UTF-8 encoded C string.
 *
 * @param r_dest A pointer to a Variant to hold the newly created String.
 * @param p_contents A pointer to a UTF-8 encoded C string (null terminated).
 */
typedef void (*KSExtensionInterfaceStringNewWithUtf8Chars)(KSExtensionUninitializedStringPtr r_dest, const char *p_contents);

/**
 * @name string_new_with_utf16_chars
 * @since 4.1
 *
 * Creates a String from a UTF-16 encoded C string.
 *
 * @param r_dest A pointer to a Variant to hold the newly created String.
 * @param p_contents A pointer to a UTF-16 encoded C string (null terminated).
 */
typedef void (*KSExtensionInterfaceStringNewWithUtf16Chars)(KSExtensionUninitializedStringPtr r_dest, const char16_t *p_contents);

/**
 * @name string_new_with_utf32_chars
 * @since 4.1
 *
 * Creates a String from a UTF-32 encoded C string.
 *
 * @param r_dest A pointer to a Variant to hold the newly created String.
 * @param p_contents A pointer to a UTF-32 encoded C string (null terminated).
 */
typedef void (*KSExtensionInterfaceStringNewWithUtf32Chars)(KSExtensionUninitializedStringPtr r_dest, const char32_t *p_contents);

/**
 * @name string_new_with_wide_chars
 * @since 4.1
 *
 * Creates a String from a wide C string.
 *
 * @param r_dest A pointer to a Variant to hold the newly created String.
 * @param p_contents A pointer to a wide C string (null terminated).
 */
typedef void (*KSExtensionInterfaceStringNewWithWideChars)(KSExtensionUninitializedStringPtr r_dest, const wchar_t *p_contents);

/**
 * @name string_new_with_latin1_chars_and_len
 * @since 4.1
 *
 * Creates a String from a Latin-1 encoded C string with the given length.
 *
 * @param r_dest A pointer to a Variant to hold the newly created String.
 * @param p_contents A pointer to a Latin-1 encoded C string.
 * @param p_size The number of characters (= number of bytes).
 */
typedef void (*KSExtensionInterfaceStringNewWithLatin1CharsAndLen)(KSExtensionUninitializedStringPtr r_dest, const char *p_contents, KSExtensionInt p_size);

/**
 * @name string_new_with_utf8_chars_and_len
 * @since 4.1
 * @deprecated in Kosmic 4.3. Use `string_new_with_utf8_chars_and_len2` instead.
 *
 * Creates a String from a UTF-8 encoded C string with the given length.
 *
 * @param r_dest A pointer to a Variant to hold the newly created String.
 * @param p_contents A pointer to a UTF-8 encoded C string.
 * @param p_size The number of bytes (not code units).
 */
typedef void (*KSExtensionInterfaceStringNewWithUtf8CharsAndLen)(KSExtensionUninitializedStringPtr r_dest, const char *p_contents, KSExtensionInt p_size);

/**
 * @name string_new_with_utf8_chars_and_len2
 * @since 4.3
 *
 * Creates a String from a UTF-8 encoded C string with the given length.
 *
 * @param r_dest A pointer to a Variant to hold the newly created String.
 * @param p_contents A pointer to a UTF-8 encoded C string.
 * @param p_size The number of bytes (not code units).
 *
 * @return Error code signifying if the operation successful.
 */
typedef KSExtensionInt (*KSExtensionInterfaceStringNewWithUtf8CharsAndLen2)(KSExtensionUninitializedStringPtr r_dest, const char *p_contents, KSExtensionInt p_size);

/**
 * @name string_new_with_utf16_chars_and_len
 * @since 4.1
 * @deprecated in Kosmic 4.3. Use `string_new_with_utf16_chars_and_len2` instead.
 *
 * Creates a String from a UTF-16 encoded C string with the given length.
 *
 * @param r_dest A pointer to a Variant to hold the newly created String.
 * @param p_contents A pointer to a UTF-16 encoded C string.
 * @param p_size The number of characters (not bytes).
 */
typedef void (*KSExtensionInterfaceStringNewWithUtf16CharsAndLen)(KSExtensionUninitializedStringPtr r_dest, const char16_t *p_contents, KSExtensionInt p_char_count);

/**
 * @name string_new_with_utf16_chars_and_len2
 * @since 4.3
 *
 * Creates a String from a UTF-16 encoded C string with the given length.
 *
 * @param r_dest A pointer to a Variant to hold the newly created String.
 * @param p_contents A pointer to a UTF-16 encoded C string.
 * @param p_size The number of characters (not bytes).
 * @param p_default_little_endian If true, UTF-16 use little endian.
 *
 * @return Error code signifying if the operation successful.
 */
typedef KSExtensionInt (*KSExtensionInterfaceStringNewWithUtf16CharsAndLen2)(KSExtensionUninitializedStringPtr r_dest, const char16_t *p_contents, KSExtensionInt p_char_count, KSExtensionBool p_default_little_endian);

/**
 * @name string_new_with_utf32_chars_and_len
 * @since 4.1
 *
 * Creates a String from a UTF-32 encoded C string with the given length.
 *
 * @param r_dest A pointer to a Variant to hold the newly created String.
 * @param p_contents A pointer to a UTF-32 encoded C string.
 * @param p_size The number of characters (not bytes).
 */
typedef void (*KSExtensionInterfaceStringNewWithUtf32CharsAndLen)(KSExtensionUninitializedStringPtr r_dest, const char32_t *p_contents, KSExtensionInt p_char_count);

/**
 * @name string_new_with_wide_chars_and_len
 * @since 4.1
 *
 * Creates a String from a wide C string with the given length.
 *
 * @param r_dest A pointer to a Variant to hold the newly created String.
 * @param p_contents A pointer to a wide C string.
 * @param p_size The number of characters (not bytes).
 */
typedef void (*KSExtensionInterfaceStringNewWithWideCharsAndLen)(KSExtensionUninitializedStringPtr r_dest, const wchar_t *p_contents, KSExtensionInt p_char_count);

/**
 * @name string_to_latin1_chars
 * @since 4.1
 *
 * Converts a String to a Latin-1 encoded C string.
 *
 * It doesn't write a null terminator.
 *
 * @param p_self A pointer to the String.
 * @param r_text A pointer to the buffer to hold the resulting data. If NULL is passed in, only the length will be computed.
 * @param p_max_write_length The maximum number of characters that can be written to r_text. It has no affect on the return value.
 *
 * @return The resulting encoded string length in characters (not bytes), not including a null terminator.
 */
typedef KSExtensionInt (*KSExtensionInterfaceStringToLatin1Chars)(KSExtensionConstStringPtr p_self, char *r_text, KSExtensionInt p_max_write_length);

/**
 * @name string_to_utf8_chars
 * @since 4.1
 *
 * Converts a String to a UTF-8 encoded C string.
 *
 * It doesn't write a null terminator.
 *
 * @param p_self A pointer to the String.
 * @param r_text A pointer to the buffer to hold the resulting data. If NULL is passed in, only the length will be computed.
 * @param p_max_write_length The maximum number of characters that can be written to r_text. It has no affect on the return value.
 *
 * @return The resulting encoded string length in characters (not bytes), not including a null terminator.
 */
typedef KSExtensionInt (*KSExtensionInterfaceStringToUtf8Chars)(KSExtensionConstStringPtr p_self, char *r_text, KSExtensionInt p_max_write_length);

/**
 * @name string_to_utf16_chars
 * @since 4.1
 *
 * Converts a String to a UTF-16 encoded C string.
 *
 * It doesn't write a null terminator.
 *
 * @param p_self A pointer to the String.
 * @param r_text A pointer to the buffer to hold the resulting data. If NULL is passed in, only the length will be computed.
 * @param p_max_write_length The maximum number of characters that can be written to r_text. It has no affect on the return value.
 *
 * @return The resulting encoded string length in characters (not bytes), not including a null terminator.
 */
typedef KSExtensionInt (*KSExtensionInterfaceStringToUtf16Chars)(KSExtensionConstStringPtr p_self, char16_t *r_text, KSExtensionInt p_max_write_length);

/**
 * @name string_to_utf32_chars
 * @since 4.1
 *
 * Converts a String to a UTF-32 encoded C string.
 *
 * It doesn't write a null terminator.
 *
 * @param p_self A pointer to the String.
 * @param r_text A pointer to the buffer to hold the resulting data. If NULL is passed in, only the length will be computed.
 * @param p_max_write_length The maximum number of characters that can be written to r_text. It has no affect on the return value.
 *
 * @return The resulting encoded string length in characters (not bytes), not including a null terminator.
 */
typedef KSExtensionInt (*KSExtensionInterfaceStringToUtf32Chars)(KSExtensionConstStringPtr p_self, char32_t *r_text, KSExtensionInt p_max_write_length);

/**
 * @name string_to_wide_chars
 * @since 4.1
 *
 * Converts a String to a wide C string.
 *
 * It doesn't write a null terminator.
 *
 * @param p_self A pointer to the String.
 * @param r_text A pointer to the buffer to hold the resulting data. If NULL is passed in, only the length will be computed.
 * @param p_max_write_length The maximum number of characters that can be written to r_text. It has no affect on the return value.
 *
 * @return The resulting encoded string length in characters (not bytes), not including a null terminator.
 */
typedef KSExtensionInt (*KSExtensionInterfaceStringToWideChars)(KSExtensionConstStringPtr p_self, wchar_t *r_text, KSExtensionInt p_max_write_length);

/**
 * @name string_operator_index
 * @since 4.1
 *
 * Gets a pointer to the character at the given index from a String.
 *
 * @param p_self A pointer to the String.
 * @param p_index The index.
 *
 * @return A pointer to the requested character.
 */
typedef char32_t *(*KSExtensionInterfaceStringOperatorIndex)(KSExtensionStringPtr p_self, KSExtensionInt p_index);

/**
 * @name string_operator_index_const
 * @since 4.1
 *
 * Gets a const pointer to the character at the given index from a String.
 *
 * @param p_self A pointer to the String.
 * @param p_index The index.
 *
 * @return A const pointer to the requested character.
 */
typedef const char32_t *(*KSExtensionInterfaceStringOperatorIndexConst)(KSExtensionConstStringPtr p_self, KSExtensionInt p_index);

/**
 * @name string_operator_plus_eq_string
 * @since 4.1
 *
 * Appends another String to a String.
 *
 * @param p_self A pointer to the String.
 * @param p_b A pointer to the other String to append.
 */
typedef void (*KSExtensionInterfaceStringOperatorPlusEqString)(KSExtensionStringPtr p_self, KSExtensionConstStringPtr p_b);

/**
 * @name string_operator_plus_eq_char
 * @since 4.1
 *
 * Appends a character to a String.
 *
 * @param p_self A pointer to the String.
 * @param p_b A pointer to the character to append.
 */
typedef void (*KSExtensionInterfaceStringOperatorPlusEqChar)(KSExtensionStringPtr p_self, char32_t p_b);

/**
 * @name string_operator_plus_eq_cstr
 * @since 4.1
 *
 * Appends a Latin-1 encoded C string to a String.
 *
 * @param p_self A pointer to the String.
 * @param p_b A pointer to a Latin-1 encoded C string (null terminated).
 */
typedef void (*KSExtensionInterfaceStringOperatorPlusEqCstr)(KSExtensionStringPtr p_self, const char *p_b);

/**
 * @name string_operator_plus_eq_wcstr
 * @since 4.1
 *
 * Appends a wide C string to a String.
 *
 * @param p_self A pointer to the String.
 * @param p_b A pointer to a wide C string (null terminated).
 */
typedef void (*KSExtensionInterfaceStringOperatorPlusEqWcstr)(KSExtensionStringPtr p_self, const wchar_t *p_b);

/**
 * @name string_operator_plus_eq_c32str
 * @since 4.1
 *
 * Appends a UTF-32 encoded C string to a String.
 *
 * @param p_self A pointer to the String.
 * @param p_b A pointer to a UTF-32 encoded C string (null terminated).
 */
typedef void (*KSExtensionInterfaceStringOperatorPlusEqC32str)(KSExtensionStringPtr p_self, const char32_t *p_b);

/**
 * @name string_resize
 * @since 4.2
 *
 * Resizes the underlying string data to the given number of characters.
 *
 * Space needs to be allocated for the null terminating character ('\0') which
 * also must be added manually, in order for all string functions to work correctly.
 *
 * Warning: This is an error-prone operation - only use it if there's no other
 * efficient way to accomplish your goal.
 *
 * @param p_self A pointer to the String.
 * @param p_resize The new length for the String.
 *
 * @return Error code signifying if the operation successful.
 */
typedef KSExtensionInt (*KSExtensionInterfaceStringResize)(KSExtensionStringPtr p_self, KSExtensionInt p_resize);

/* INTERFACE: StringName Utilities */

/**
 * @name string_name_new_with_latin1_chars
 * @since 4.2
 *
 * Creates a StringName from a Latin-1 encoded C string.
 *
 * If `p_is_static` is true, then:
 * - The StringName will reuse the `p_contents` buffer instead of copying it.
 *   You must guarantee that the buffer remains valid for the duration of the application (e.g. string literal).
 * - You must not call a destructor for this StringName. Incrementing the initial reference once should achieve this.
 *
 * `p_is_static` is purely an optimization and can easily introduce undefined behavior if used wrong. In case of doubt, set it to false.
 *
 * @param r_dest A pointer to uninitialized storage, into which the newly created StringName is constructed.
 * @param p_contents A pointer to a C string (null terminated and Latin-1 or ASCII encoded).
 * @param p_is_static Whether the StringName reuses the buffer directly (see above).
 */
typedef void (*KSExtensionInterfaceStringNameNewWithLatin1Chars)(KSExtensionUninitializedStringNamePtr r_dest, const char *p_contents, KSExtensionBool p_is_static);

/**
 * @name string_name_new_with_utf8_chars
 * @since 4.2
 *
 * Creates a StringName from a UTF-8 encoded C string.
 *
 * @param r_dest A pointer to uninitialized storage, into which the newly created StringName is constructed.
 * @param p_contents A pointer to a C string (null terminated and UTF-8 encoded).
 */
typedef void (*KSExtensionInterfaceStringNameNewWithUtf8Chars)(KSExtensionUninitializedStringNamePtr r_dest, const char *p_contents);

/**
 * @name string_name_new_with_utf8_chars_and_len
 * @since 4.2
 *
 * Creates a StringName from a UTF-8 encoded string with a given number of characters.
 *
 * @param r_dest A pointer to uninitialized storage, into which the newly created StringName is constructed.
 * @param p_contents A pointer to a C string (null terminated and UTF-8 encoded).
 * @param p_size The number of bytes (not UTF-8 code points).
 */
typedef void (*KSExtensionInterfaceStringNameNewWithUtf8CharsAndLen)(KSExtensionUninitializedStringNamePtr r_dest, const char *p_contents, KSExtensionInt p_size);

/* INTERFACE: XMLParser Utilities */

/**
 * @name xml_parser_open_buffer
 * @since 4.1
 *
 * Opens a raw XML buffer on an XMLParser instance.
 *
 * @param p_instance A pointer to an XMLParser object.
 * @param p_buffer A pointer to the buffer.
 * @param p_size The size of the buffer.
 *
 * @return A Kosmic error code (ex. OK, ERR_INVALID_DATA, etc).
 *
 * @see XMLParser::open_buffer()
 */
typedef KSExtensionInt (*KSExtensionInterfaceXmlParserOpenBuffer)(KSExtensionObjectPtr p_instance, const uint8_t *p_buffer, size_t p_size);

/* INTERFACE: FileAccess Utilities */

/**
 * @name file_access_store_buffer
 * @since 4.1
 *
 * Stores the given buffer using an instance of FileAccess.
 *
 * @param p_instance A pointer to a FileAccess object.
 * @param p_src A pointer to the buffer.
 * @param p_length The size of the buffer.
 *
 * @see FileAccess::store_buffer()
 */
typedef void (*KSExtensionInterfaceFileAccessStoreBuffer)(KSExtensionObjectPtr p_instance, const uint8_t *p_src, uint64_t p_length);

/**
 * @name file_access_get_buffer
 * @since 4.1
 *
 * Reads the next p_length bytes into the given buffer using an instance of FileAccess.
 *
 * @param p_instance A pointer to a FileAccess object.
 * @param p_dst A pointer to the buffer to store the data.
 * @param p_length The requested number of bytes to read.
 *
 * @return The actual number of bytes read (may be less than requested).
 */
typedef uint64_t (*KSExtensionInterfaceFileAccessGetBuffer)(KSExtensionConstObjectPtr p_instance, uint8_t *p_dst, uint64_t p_length);

/* INTERFACE: Image Utilities */

/**
 * @name image_ptrw
 * @since 4.3
 *
 * Returns writable pointer to internal Image buffer.
 *
 * @param p_instance A pointer to a Image object.
 *
 * @return Pointer to internal Image buffer.
 *
 * @see Image::ptrw()
 */
typedef uint8_t *(*KSExtensionInterfaceImagePtrw)(KSExtensionObjectPtr p_instance);

/**
 * @name image_ptr
 * @since 4.3
 *
 * Returns read only pointer to internal Image buffer.
 *
 * @param p_instance A pointer to a Image object.
 *
 * @return Pointer to internal Image buffer.
 *
 * @see Image::ptr()
 */
typedef const uint8_t *(*KSExtensionInterfaceImagePtr)(KSExtensionObjectPtr p_instance);

/* INTERFACE: WorkerThreadPool Utilities */

/**
 * @name worker_thread_pool_add_native_group_task
 * @since 4.1
 *
 * Adds a group task to an instance of WorkerThreadPool.
 *
 * @param p_instance A pointer to a WorkerThreadPool object.
 * @param p_func A pointer to a function to run in the thread pool.
 * @param p_userdata A pointer to arbitrary data which will be passed to p_func.
 * @param p_tasks The number of tasks needed in the group.
 * @param p_high_priority Whether or not this is a high priority task.
 * @param p_description A pointer to a String with the task description.
 *
 * @return The task group ID.
 *
 * @see WorkerThreadPool::add_group_task()
 */
typedef int64_t (*KSExtensionInterfaceWorkerThreadPoolAddNativeGroupTask)(KSExtensionObjectPtr p_instance, void (*p_func)(void *, uint32_t), void *p_userdata, int p_elements, int p_tasks, KSExtensionBool p_high_priority, KSExtensionConstStringPtr p_description);

/**
 * @name worker_thread_pool_add_native_task
 * @since 4.1
 *
 * Adds a task to an instance of WorkerThreadPool.
 *
 * @param p_instance A pointer to a WorkerThreadPool object.
 * @param p_func A pointer to a function to run in the thread pool.
 * @param p_userdata A pointer to arbitrary data which will be passed to p_func.
 * @param p_high_priority Whether or not this is a high priority task.
 * @param p_description A pointer to a String with the task description.
 *
 * @return The task ID.
 */
typedef int64_t (*KSExtensionInterfaceWorkerThreadPoolAddNativeTask)(KSExtensionObjectPtr p_instance, void (*p_func)(void *), void *p_userdata, KSExtensionBool p_high_priority, KSExtensionConstStringPtr p_description);

/* INTERFACE: Packed Array */

/**
 * @name packed_byte_array_operator_index
 * @since 4.1
 *
 * Gets a pointer to a byte in a PackedByteArray.
 *
 * @param p_self A pointer to a PackedByteArray object.
 * @param p_index The index of the byte to get.
 *
 * @return A pointer to the requested byte.
 */
typedef uint8_t *(*KSExtensionInterfacePackedByteArrayOperatorIndex)(KSExtensionTypePtr p_self, KSExtensionInt p_index);

/**
 * @name packed_byte_array_operator_index_const
 * @since 4.1
 *
 * Gets a const pointer to a byte in a PackedByteArray.
 *
 * @param p_self A const pointer to a PackedByteArray object.
 * @param p_index The index of the byte to get.
 *
 * @return A const pointer to the requested byte.
 */
typedef const uint8_t *(*KSExtensionInterfacePackedByteArrayOperatorIndexConst)(KSExtensionConstTypePtr p_self, KSExtensionInt p_index);

/**
 * @name packed_float32_array_operator_index
 * @since 4.1
 *
 * Gets a pointer to a 32-bit float in a PackedFloat32Array.
 *
 * @param p_self A pointer to a PackedFloat32Array object.
 * @param p_index The index of the float to get.
 *
 * @return A pointer to the requested 32-bit float.
 */
typedef float *(*KSExtensionInterfacePackedFloat32ArrayOperatorIndex)(KSExtensionTypePtr p_self, KSExtensionInt p_index);

/**
 * @name packed_float32_array_operator_index_const
 * @since 4.1
 *
 * Gets a const pointer to a 32-bit float in a PackedFloat32Array.
 *
 * @param p_self A const pointer to a PackedFloat32Array object.
 * @param p_index The index of the float to get.
 *
 * @return A const pointer to the requested 32-bit float.
 */
typedef const float *(*KSExtensionInterfacePackedFloat32ArrayOperatorIndexConst)(KSExtensionConstTypePtr p_self, KSExtensionInt p_index);

/**
 * @name packed_float64_array_operator_index
 * @since 4.1
 *
 * Gets a pointer to a 64-bit float in a PackedFloat64Array.
 *
 * @param p_self A pointer to a PackedFloat64Array object.
 * @param p_index The index of the float to get.
 *
 * @return A pointer to the requested 64-bit float.
 */
typedef double *(*KSExtensionInterfacePackedFloat64ArrayOperatorIndex)(KSExtensionTypePtr p_self, KSExtensionInt p_index);

/**
 * @name packed_float64_array_operator_index_const
 * @since 4.1
 *
 * Gets a const pointer to a 64-bit float in a PackedFloat64Array.
 *
 * @param p_self A const pointer to a PackedFloat64Array object.
 * @param p_index The index of the float to get.
 *
 * @return A const pointer to the requested 64-bit float.
 */
typedef const double *(*KSExtensionInterfacePackedFloat64ArrayOperatorIndexConst)(KSExtensionConstTypePtr p_self, KSExtensionInt p_index);

/**
 * @name packed_int32_array_operator_index
 * @since 4.1
 *
 * Gets a pointer to a 32-bit integer in a PackedInt32Array.
 *
 * @param p_self A pointer to a PackedInt32Array object.
 * @param p_index The index of the integer to get.
 *
 * @return A pointer to the requested 32-bit integer.
 */
typedef int32_t *(*KSExtensionInterfacePackedInt32ArrayOperatorIndex)(KSExtensionTypePtr p_self, KSExtensionInt p_index);

/**
 * @name packed_int32_array_operator_index_const
 * @since 4.1
 *
 * Gets a const pointer to a 32-bit integer in a PackedInt32Array.
 *
 * @param p_self A const pointer to a PackedInt32Array object.
 * @param p_index The index of the integer to get.
 *
 * @return A const pointer to the requested 32-bit integer.
 */
typedef const int32_t *(*KSExtensionInterfacePackedInt32ArrayOperatorIndexConst)(KSExtensionConstTypePtr p_self, KSExtensionInt p_index);

/**
 * @name packed_int64_array_operator_index
 * @since 4.1
 *
 * Gets a pointer to a 64-bit integer in a PackedInt64Array.
 *
 * @param p_self A pointer to a PackedInt64Array object.
 * @param p_index The index of the integer to get.
 *
 * @return A pointer to the requested 64-bit integer.
 */
typedef int64_t *(*KSExtensionInterfacePackedInt64ArrayOperatorIndex)(KSExtensionTypePtr p_self, KSExtensionInt p_index);

/**
 * @name packed_int64_array_operator_index_const
 * @since 4.1
 *
 * Gets a const pointer to a 64-bit integer in a PackedInt64Array.
 *
 * @param p_self A const pointer to a PackedInt64Array object.
 * @param p_index The index of the integer to get.
 *
 * @return A const pointer to the requested 64-bit integer.
 */
typedef const int64_t *(*KSExtensionInterfacePackedInt64ArrayOperatorIndexConst)(KSExtensionConstTypePtr p_self, KSExtensionInt p_index);

/**
 * @name packed_string_array_operator_index
 * @since 4.1
 *
 * Gets a pointer to a string in a PackedStringArray.
 *
 * @param p_self A pointer to a PackedStringArray object.
 * @param p_index The index of the String to get.
 *
 * @return A pointer to the requested String.
 */
typedef KSExtensionStringPtr (*KSExtensionInterfacePackedStringArrayOperatorIndex)(KSExtensionTypePtr p_self, KSExtensionInt p_index);

/**
 * @name packed_string_array_operator_index_const
 * @since 4.1
 *
 * Gets a const pointer to a string in a PackedStringArray.
 *
 * @param p_self A const pointer to a PackedStringArray object.
 * @param p_index The index of the String to get.
 *
 * @return A const pointer to the requested String.
 */
typedef KSExtensionStringPtr (*KSExtensionInterfacePackedStringArrayOperatorIndexConst)(KSExtensionConstTypePtr p_self, KSExtensionInt p_index);

/**
 * @name packed_vector2_array_operator_index
 * @since 4.1
 *
 * Gets a pointer to a Vector2 in a PackedVector2Array.
 *
 * @param p_self A pointer to a PackedVector2Array object.
 * @param p_index The index of the Vector2 to get.
 *
 * @return A pointer to the requested Vector2.
 */
typedef KSExtensionTypePtr (*KSExtensionInterfacePackedVector2ArrayOperatorIndex)(KSExtensionTypePtr p_self, KSExtensionInt p_index);

/**
 * @name packed_vector2_array_operator_index_const
 * @since 4.1
 *
 * Gets a const pointer to a Vector2 in a PackedVector2Array.
 *
 * @param p_self A const pointer to a PackedVector2Array object.
 * @param p_index The index of the Vector2 to get.
 *
 * @return A const pointer to the requested Vector2.
 */
typedef KSExtensionTypePtr (*KSExtensionInterfacePackedVector2ArrayOperatorIndexConst)(KSExtensionConstTypePtr p_self, KSExtensionInt p_index);

/**
 * @name packed_vector3_array_operator_index
 * @since 4.1
 *
 * Gets a pointer to a Vector3 in a PackedVector3Array.
 *
 * @param p_self A pointer to a PackedVector3Array object.
 * @param p_index The index of the Vector3 to get.
 *
 * @return A pointer to the requested Vector3.
 */
typedef KSExtensionTypePtr (*KSExtensionInterfacePackedVector3ArrayOperatorIndex)(KSExtensionTypePtr p_self, KSExtensionInt p_index);

/**
 * @name packed_vector3_array_operator_index_const
 * @since 4.1
 *
 * Gets a const pointer to a Vector3 in a PackedVector3Array.
 *
 * @param p_self A const pointer to a PackedVector3Array object.
 * @param p_index The index of the Vector3 to get.
 *
 * @return A const pointer to the requested Vector3.
 */
typedef KSExtensionTypePtr (*KSExtensionInterfacePackedVector3ArrayOperatorIndexConst)(KSExtensionConstTypePtr p_self, KSExtensionInt p_index);

/**
 * @name packed_vector4_array_operator_index
 * @since 4.3
 *
 * Gets a pointer to a Vector4 in a PackedVector4Array.
 *
 * @param p_self A pointer to a PackedVector4Array object.
 * @param p_index The index of the Vector4 to get.
 *
 * @return A pointer to the requested Vector4.
 */
typedef KSExtensionTypePtr (*KSExtensionInterfacePackedVector4ArrayOperatorIndex)(KSExtensionTypePtr p_self, KSExtensionInt p_index);

/**
 * @name packed_vector4_array_operator_index_const
 * @since 4.3
 *
 * Gets a const pointer to a Vector4 in a PackedVector4Array.
 *
 * @param p_self A const pointer to a PackedVector4Array object.
 * @param p_index The index of the Vector4 to get.
 *
 * @return A const pointer to the requested Vector4.
 */
typedef KSExtensionTypePtr (*KSExtensionInterfacePackedVector4ArrayOperatorIndexConst)(KSExtensionConstTypePtr p_self, KSExtensionInt p_index);

/**
 * @name packed_color_array_operator_index
 * @since 4.1
 *
 * Gets a pointer to a color in a PackedColorArray.
 *
 * @param p_self A pointer to a PackedColorArray object.
 * @param p_index The index of the Color to get.
 *
 * @return A pointer to the requested Color.
 */
typedef KSExtensionTypePtr (*KSExtensionInterfacePackedColorArrayOperatorIndex)(KSExtensionTypePtr p_self, KSExtensionInt p_index);

/**
 * @name packed_color_array_operator_index_const
 * @since 4.1
 *
 * Gets a const pointer to a color in a PackedColorArray.
 *
 * @param p_self A const pointer to a PackedColorArray object.
 * @param p_index The index of the Color to get.
 *
 * @return A const pointer to the requested Color.
 */
typedef KSExtensionTypePtr (*KSExtensionInterfacePackedColorArrayOperatorIndexConst)(KSExtensionConstTypePtr p_self, KSExtensionInt p_index);

/**
 * @name array_operator_index
 * @since 4.1
 *
 * Gets a pointer to a Variant in an Array.
 *
 * @param p_self A pointer to an Array object.
 * @param p_index The index of the Variant to get.
 *
 * @return A pointer to the requested Variant.
 */
typedef KSExtensionVariantPtr (*KSExtensionInterfaceArrayOperatorIndex)(KSExtensionTypePtr p_self, KSExtensionInt p_index);

/**
 * @name array_operator_index_const
 * @since 4.1
 *
 * Gets a const pointer to a Variant in an Array.
 *
 * @param p_self A const pointer to an Array object.
 * @param p_index The index of the Variant to get.
 *
 * @return A const pointer to the requested Variant.
 */
typedef KSExtensionVariantPtr (*KSExtensionInterfaceArrayOperatorIndexConst)(KSExtensionConstTypePtr p_self, KSExtensionInt p_index);

/**
 * @name array_ref
 * @since 4.1
 *
 * Sets an Array to be a reference to another Array object.
 *
 * @param p_self A pointer to the Array object to update.
 * @param p_from A pointer to the Array object to reference.
 */
typedef void (*KSExtensionInterfaceArrayRef)(KSExtensionTypePtr p_self, KSExtensionConstTypePtr p_from);

/**
 * @name array_set_typed
 * @since 4.1
 *
 * Makes an Array into a typed Array.
 *
 * @param p_self A pointer to the Array.
 * @param p_type The type of Variant the Array will store.
 * @param p_class_name A pointer to a StringName with the name of the object (if p_type is KSEXTENSION_VARIANT_TYPE_OBJECT).
 * @param p_script A pointer to a Script object (if p_type is KSEXTENSION_VARIANT_TYPE_OBJECT and the base class is extended by a script).
 */
typedef void (*KSExtensionInterfaceArraySetTyped)(KSExtensionTypePtr p_self, KSExtensionVariantType p_type, KSExtensionConstStringNamePtr p_class_name, KSExtensionConstVariantPtr p_script);

/* INTERFACE: Dictionary */

/**
 * @name dictionary_operator_index
 * @since 4.1
 *
 * Gets a pointer to a Variant in a Dictionary with the given key.
 *
 * @param p_self A pointer to a Dictionary object.
 * @param p_key A pointer to a Variant representing the key.
 *
 * @return A pointer to a Variant representing the value at the given key.
 */
typedef KSExtensionVariantPtr (*KSExtensionInterfaceDictionaryOperatorIndex)(KSExtensionTypePtr p_self, KSExtensionConstVariantPtr p_key);

/**
 * @name dictionary_operator_index_const
 * @since 4.1
 *
 * Gets a const pointer to a Variant in a Dictionary with the given key.
 *
 * @param p_self A const pointer to a Dictionary object.
 * @param p_key A pointer to a Variant representing the key.
 *
 * @return A const pointer to a Variant representing the value at the given key.
 */
typedef KSExtensionVariantPtr (*KSExtensionInterfaceDictionaryOperatorIndexConst)(KSExtensionConstTypePtr p_self, KSExtensionConstVariantPtr p_key);

/**
 * @name dictionary_set_typed
 * @since 4.4
 *
 * Makes a Dictionary into a typed Dictionary.
 *
 * @param p_self A pointer to the Dictionary.
 * @param p_key_type The type of Variant the Dictionary key will store.
 * @param p_key_class_name A pointer to a StringName with the name of the object (if p_key_type is KSEXTENSION_VARIANT_TYPE_OBJECT).
 * @param p_key_script A pointer to a Script object (if p_key_type is KSEXTENSION_VARIANT_TYPE_OBJECT and the base class is extended by a script).
 * @param p_value_type The type of Variant the Dictionary value will store.
 * @param p_value_class_name A pointer to a StringName with the name of the object (if p_value_type is KSEXTENSION_VARIANT_TYPE_OBJECT).
 * @param p_value_script A pointer to a Script object (if p_value_type is KSEXTENSION_VARIANT_TYPE_OBJECT and the base class is extended by a script).
 */
typedef void (*KSExtensionInterfaceDictionarySetTyped)(KSExtensionTypePtr p_self, KSExtensionVariantType p_key_type, KSExtensionConstStringNamePtr p_key_class_name, KSExtensionConstVariantPtr p_key_script, KSExtensionVariantType p_value_type, KSExtensionConstStringNamePtr p_value_class_name, KSExtensionConstVariantPtr p_value_script);

/* INTERFACE: Object */

/**
 * @name object_method_bind_call
 * @since 4.1
 *
 * Calls a method on an Object.
 *
 * @param p_method_bind A pointer to the MethodBind representing the method on the Object's class.
 * @param p_instance A pointer to the Object.
 * @param p_args A pointer to a C array of Variants representing the arguments.
 * @param p_arg_count The number of arguments.
 * @param r_ret A pointer to Variant which will receive the return value.
 * @param r_error A pointer to a KSExtensionCallError struct that will receive error information.
 */
typedef void (*KSExtensionInterfaceObjectMethodBindCall)(KSExtensionMethodBindPtr p_method_bind, KSExtensionObjectPtr p_instance, const KSExtensionConstVariantPtr *p_args, KSExtensionInt p_arg_count, KSExtensionUninitializedVariantPtr r_ret, KSExtensionCallError *r_error);

/**
 * @name object_method_bind_ptrcall
 * @since 4.1
 *
 * Calls a method on an Object (using a "ptrcall").
 *
 * @param p_method_bind A pointer to the MethodBind representing the method on the Object's class.
 * @param p_instance A pointer to the Object.
 * @param p_args A pointer to a C array representing the arguments.
 * @param r_ret A pointer to the Object that will receive the return value.
 */
typedef void (*KSExtensionInterfaceObjectMethodBindPtrcall)(KSExtensionMethodBindPtr p_method_bind, KSExtensionObjectPtr p_instance, const KSExtensionConstTypePtr *p_args, KSExtensionTypePtr r_ret);

/**
 * @name object_destroy
 * @since 4.1
 *
 * Destroys an Object.
 *
 * @param p_o A pointer to the Object.
 */
typedef void (*KSExtensionInterfaceObjectDestroy)(KSExtensionObjectPtr p_o);

/**
 * @name global_get_singleton
 * @since 4.1
 *
 * Gets a global singleton by name.
 *
 * @param p_name A pointer to a StringName with the singleton name.
 *
 * @return A pointer to the singleton Object.
 */
typedef KSExtensionObjectPtr (*KSExtensionInterfaceGlobalGetSingleton)(KSExtensionConstStringNamePtr p_name);

/**
 * @name object_get_instance_binding
 * @since 4.1
 *
 * Gets a pointer representing an Object's instance binding.
 *
 * @param p_o A pointer to the Object.
 * @param p_library A token the library received by the KSExtension's entry point function.
 * @param p_callbacks A pointer to a KSExtensionInstanceBindingCallbacks struct.
 *
 * @return
 */
typedef void *(*KSExtensionInterfaceObjectGetInstanceBinding)(KSExtensionObjectPtr p_o, void *p_token, const KSExtensionInstanceBindingCallbacks *p_callbacks);

/**
 * @name object_set_instance_binding
 * @since 4.1
 *
 * Sets an Object's instance binding.
 *
 * @param p_o A pointer to the Object.
 * @param p_library A token the library received by the KSExtension's entry point function.
 * @param p_binding A pointer to the instance binding.
 * @param p_callbacks A pointer to a KSExtensionInstanceBindingCallbacks struct.
 */
typedef void (*KSExtensionInterfaceObjectSetInstanceBinding)(KSExtensionObjectPtr p_o, void *p_token, void *p_binding, const KSExtensionInstanceBindingCallbacks *p_callbacks);

/**
 * @name object_free_instance_binding
 * @since 4.2
 *
 * Free an Object's instance binding.
 *
 * @param p_o A pointer to the Object.
 * @param p_library A token the library received by the KSExtension's entry point function.
 */
typedef void (*KSExtensionInterfaceObjectFreeInstanceBinding)(KSExtensionObjectPtr p_o, void *p_token);

/**
 * @name object_set_instance
 * @since 4.1
 *
 * Sets an extension class instance on a Object.
 *
 * @param p_o A pointer to the Object.
 * @param p_classname A pointer to a StringName with the registered extension class's name.
 * @param p_instance A pointer to the extension class instance.
 */
typedef void (*KSExtensionInterfaceObjectSetInstance)(KSExtensionObjectPtr p_o, KSExtensionConstStringNamePtr p_classname, KSExtensionClassInstancePtr p_instance); /* p_classname should be a registered extension class and should extend the p_o object's class. */

/**
 * @name object_get_class_name
 * @since 4.1
 *
 * Gets the class name of an Object.
 *
 * If the KSExtension wraps the Kosmic object in an abstraction specific to its class, this is the
 * function that should be used to determine which wrapper to use.
 *
 * @param p_object A pointer to the Object.
 * @param p_library A pointer the library received by the KSExtension's entry point function.
 * @param r_class_name A pointer to a String to receive the class name.
 *
 * @return true if successful in getting the class name; otherwise false.
 */
typedef KSExtensionBool (*KSExtensionInterfaceObjectGetClassName)(KSExtensionConstObjectPtr p_object, KSExtensionClassLibraryPtr p_library, KSExtensionUninitializedStringNamePtr r_class_name);

/**
 * @name object_cast_to
 * @since 4.1
 *
 * Casts an Object to a different type.
 *
 * @param p_object A pointer to the Object.
 * @param p_class_tag A pointer uniquely identifying a built-in class in the ClassDB.
 *
 * @return Returns a pointer to the Object, or NULL if it can't be cast to the requested type.
 */
typedef KSExtensionObjectPtr (*KSExtensionInterfaceObjectCastTo)(KSExtensionConstObjectPtr p_object, void *p_class_tag);

/**
 * @name object_get_instance_from_id
 * @since 4.1
 *
 * Gets an Object by its instance ID.
 *
 * @param p_instance_id The instance ID.
 *
 * @return A pointer to the Object.
 */
typedef KSExtensionObjectPtr (*KSExtensionInterfaceObjectGetInstanceFromId)(KSObjectInstanceID p_instance_id);

/**
 * @name object_get_instance_id
 * @since 4.1
 *
 * Gets the instance ID from an Object.
 *
 * @param p_object A pointer to the Object.
 *
 * @return The instance ID.
 */
typedef KSObjectInstanceID (*KSExtensionInterfaceObjectGetInstanceId)(KSExtensionConstObjectPtr p_object);

/**
 * @name object_has_script_method
 * @since 4.3
 *
 * Checks if this object has a script with the given method.
 *
 * @param p_object A pointer to the Object.
 * @param p_method A pointer to a StringName identifying the method.
 *
 * @returns true if the object has a script and that script has a method with the given name. Returns false if the object has no script.
 */
typedef KSExtensionBool (*KSExtensionInterfaceObjectHasScriptMethod)(KSExtensionConstObjectPtr p_object, KSExtensionConstStringNamePtr p_method);

/**
 * @name object_call_script_method
 * @since 4.3
 *
 * Call the given script method on this object.
 *
 * @param p_object A pointer to the Object.
 * @param p_method A pointer to a StringName identifying the method.
 * @param p_args A pointer to a C array of Variant.
 * @param p_argument_count The number of arguments.
 * @param r_return A pointer a Variant which will be assigned the return value.
 * @param r_error A pointer the structure which will hold error information.
 */
typedef void (*KSExtensionInterfaceObjectCallScriptMethod)(KSExtensionObjectPtr p_object, KSExtensionConstStringNamePtr p_method, const KSExtensionConstVariantPtr *p_args, KSExtensionInt p_argument_count, KSExtensionUninitializedVariantPtr r_return, KSExtensionCallError *r_error);

/* INTERFACE: Reference */

/**
 * @name ref_get_object
 * @since 4.1
 *
 * Gets the Object from a reference.
 *
 * @param p_ref A pointer to the reference.
 *
 * @return A pointer to the Object from the reference or NULL.
 */
typedef KSExtensionObjectPtr (*KSExtensionInterfaceRefGetObject)(KSExtensionConstRefPtr p_ref);

/**
 * @name ref_set_object
 * @since 4.1
 *
 * Sets the Object referred to by a reference.
 *
 * @param p_ref A pointer to the reference.
 * @param p_object A pointer to the Object to refer to.
 */
typedef void (*KSExtensionInterfaceRefSetObject)(KSExtensionRefPtr p_ref, KSExtensionObjectPtr p_object);

/* INTERFACE: Script Instance */

/**
 * @name script_instance_create
 * @since 4.1
 * @deprecated in Kosmic 4.2. Use `script_instance_create3` instead.
 *
 * Creates a script instance that contains the given info and instance data.
 *
 * @param p_info A pointer to a KSExtensionScriptInstanceInfo struct.
 * @param p_instance_data A pointer to a data representing the script instance in the KSExtension. This will be passed to all the function pointers on p_info.
 *
 * @return A pointer to a ScriptInstanceExtension object.
 */
typedef KSExtensionScriptInstancePtr (*KSExtensionInterfaceScriptInstanceCreate)(const KSExtensionScriptInstanceInfo *p_info, KSExtensionScriptInstanceDataPtr p_instance_data);

/**
 * @name script_instance_create2
 * @since 4.2
 * @deprecated in Kosmic 4.3. Use `script_instance_create3` instead.
 *
 * Creates a script instance that contains the given info and instance data.
 *
 * @param p_info A pointer to a KSExtensionScriptInstanceInfo2 struct.
 * @param p_instance_data A pointer to a data representing the script instance in the KSExtension. This will be passed to all the function pointers on p_info.
 *
 * @return A pointer to a ScriptInstanceExtension object.
 */
typedef KSExtensionScriptInstancePtr (*KSExtensionInterfaceScriptInstanceCreate2)(const KSExtensionScriptInstanceInfo2 *p_info, KSExtensionScriptInstanceDataPtr p_instance_data);

/**
 * @name script_instance_create3
 * @since 4.3
 *
 * Creates a script instance that contains the given info and instance data.
 *
 * @param p_info A pointer to a KSExtensionScriptInstanceInfo3 struct.
 * @param p_instance_data A pointer to a data representing the script instance in the KSExtension. This will be passed to all the function pointers on p_info.
 *
 * @return A pointer to a ScriptInstanceExtension object.
 */
typedef KSExtensionScriptInstancePtr (*KSExtensionInterfaceScriptInstanceCreate3)(const KSExtensionScriptInstanceInfo3 *p_info, KSExtensionScriptInstanceDataPtr p_instance_data);

/**
 * @name placeholder_script_instance_create
 * @since 4.2
 *
 * Creates a placeholder script instance for a given script and instance.
 *
 * This interface is optional as a custom placeholder could also be created with script_instance_create().
 *
 * @param p_language A pointer to a ScriptLanguage.
 * @param p_script A pointer to a Script.
 * @param p_owner A pointer to an Object.
 *
 * @return A pointer to a PlaceHolderScriptInstance object.
 */
typedef KSExtensionScriptInstancePtr (*KSExtensionInterfacePlaceHolderScriptInstanceCreate)(KSExtensionObjectPtr p_language, KSExtensionObjectPtr p_script, KSExtensionObjectPtr p_owner);

/**
 * @name placeholder_script_instance_update
 * @since 4.2
 *
 * Updates a placeholder script instance with the given properties and values.
 *
 * The passed in placeholder must be an instance of PlaceHolderScriptInstance
 * such as the one returned by placeholder_script_instance_create().
 *
 * @param p_placeholder A pointer to a PlaceHolderScriptInstance.
 * @param p_properties A pointer to an Array of Dictionary representing PropertyInfo.
 * @param p_values A pointer to a Dictionary mapping StringName to Variant values.
 */
typedef void (*KSExtensionInterfacePlaceHolderScriptInstanceUpdate)(KSExtensionScriptInstancePtr p_placeholder, KSExtensionConstTypePtr p_properties, KSExtensionConstTypePtr p_values);

/**
 * @name object_get_script_instance
 * @since 4.2
 *
 * Get the script instance data attached to this object.
 *
 * @param p_object A pointer to the Object.
 * @param p_language A pointer to the language expected for this script instance.
 *
 * @return A KSExtensionScriptInstanceDataPtr that was attached to this object as part of script_instance_create.
 */
typedef KSExtensionScriptInstanceDataPtr (*KSExtensionInterfaceObjectGetScriptInstance)(KSExtensionConstObjectPtr p_object, KSExtensionObjectPtr p_language);

/* INTERFACE: Callable */

/**
 * @name callable_custom_create
 * @since 4.2
 * @deprecated in Kosmic 4.3. Use `callable_custom_create2` instead.
 *
 * Creates a custom Callable object from a function pointer.
 *
 * Provided struct can be safely freed once the function returns.
 *
 * @param r_callable A pointer that will receive the new Callable.
 * @param p_callable_custom_info The info required to construct a Callable.
 */
typedef void (*KSExtensionInterfaceCallableCustomCreate)(KSExtensionUninitializedTypePtr r_callable, KSExtensionCallableCustomInfo *p_callable_custom_info);

/**
 * @name callable_custom_create2
 * @since 4.3
 *
 * Creates a custom Callable object from a function pointer.
 *
 * Provided struct can be safely freed once the function returns.
 *
 * @param r_callable A pointer that will receive the new Callable.
 * @param p_callable_custom_info The info required to construct a Callable.
 */
typedef void (*KSExtensionInterfaceCallableCustomCreate2)(KSExtensionUninitializedTypePtr r_callable, KSExtensionCallableCustomInfo2 *p_callable_custom_info);

/**
 * @name callable_custom_get_userdata
 * @since 4.2
 *
 * Retrieves the userdata pointer from a custom Callable.
 *
 * If the Callable is not a custom Callable or the token does not match the one provided to callable_custom_create() via KSExtensionCallableCustomInfo then NULL will be returned.
 *
 * @param p_callable A pointer to a Callable.
 * @param p_token A pointer to an address that uniquely identifies the KSExtension.
 */
typedef void *(*KSExtensionInterfaceCallableCustomGetUserData)(KSExtensionConstTypePtr p_callable, void *p_token);

/* INTERFACE: ClassDB */

/**
 * @name classdb_construct_object
 * @since 4.1
 * @deprecated in Kosmic 4.4. Use `classdb_construct_object2` instead.
 *
 * Constructs an Object of the requested class.
 *
 * The passed class must be a built-in Kosmic class, or an already-registered extension class. In both cases, object_set_instance() should be called to fully initialize the object.
 *
 * @param p_classname A pointer to a StringName with the class name.
 *
 * @return A pointer to the newly created Object.
 */
typedef KSExtensionObjectPtr (*KSExtensionInterfaceClassdbConstructObject)(KSExtensionConstStringNamePtr p_classname);

/**
 * @name classdb_construct_object2
 * @since 4.4
 *
 * Constructs an Object of the requested class.
 *
 * The passed class must be a built-in Kosmic class, or an already-registered extension class. In both cases, object_set_instance() should be called to fully initialize the object.
 *
 * "NOTIFICATION_POSTINITIALIZE" must be sent after construction.
 *
 * @param p_classname A pointer to a StringName with the class name.
 *
 * @return A pointer to the newly created Object.
 */
typedef KSExtensionObjectPtr (*KSExtensionInterfaceClassdbConstructObject2)(KSExtensionConstStringNamePtr p_classname);

/**
 * @name classdb_get_method_bind
 * @since 4.1
 *
 * Gets a pointer to the MethodBind in ClassDB for the given class, method and hash.
 *
 * @param p_classname A pointer to a StringName with the class name.
 * @param p_methodname A pointer to a StringName with the method name.
 * @param p_hash A hash representing the function signature.
 *
 * @return A pointer to the MethodBind from ClassDB.
 */
typedef KSExtensionMethodBindPtr (*KSExtensionInterfaceClassdbGetMethodBind)(KSExtensionConstStringNamePtr p_classname, KSExtensionConstStringNamePtr p_methodname, KSExtensionInt p_hash);

/**
 * @name classdb_get_class_tag
 * @since 4.1
 *
 * Gets a pointer uniquely identifying the given built-in class in the ClassDB.
 *
 * @param p_classname A pointer to a StringName with the class name.
 *
 * @return A pointer uniquely identifying the built-in class in the ClassDB.
 */
typedef void *(*KSExtensionInterfaceClassdbGetClassTag)(KSExtensionConstStringNamePtr p_classname);

/* INTERFACE: ClassDB Extension */

/**
 * @name classdb_register_extension_class
 * @since 4.1
 * @deprecated in Kosmic 4.2. Use `classdb_register_extension_class4` instead.
 *
 * Registers an extension class in the ClassDB.
 *
 * Provided struct can be safely freed once the function returns.
 *
 * @param p_library A pointer the library received by the KSExtension's entry point function.
 * @param p_class_name A pointer to a StringName with the class name.
 * @param p_parent_class_name A pointer to a StringName with the parent class name.
 * @param p_extension_funcs A pointer to a KSExtensionClassCreationInfo struct.
 */
typedef void (*KSExtensionInterfaceClassdbRegisterExtensionClass)(KSExtensionClassLibraryPtr p_library, KSExtensionConstStringNamePtr p_class_name, KSExtensionConstStringNamePtr p_parent_class_name, const KSExtensionClassCreationInfo *p_extension_funcs);

/**
 * @name classdb_register_extension_class2
 * @since 4.2
 * @deprecated in Kosmic 4.3. Use `classdb_register_extension_class4` instead.
 *
 * Registers an extension class in the ClassDB.
 *
 * Provided struct can be safely freed once the function returns.
 *
 * @param p_library A pointer the library received by the KSExtension's entry point function.
 * @param p_class_name A pointer to a StringName with the class name.
 * @param p_parent_class_name A pointer to a StringName with the parent class name.
 * @param p_extension_funcs A pointer to a KSExtensionClassCreationInfo2 struct.
 */
typedef void (*KSExtensionInterfaceClassdbRegisterExtensionClass2)(KSExtensionClassLibraryPtr p_library, KSExtensionConstStringNamePtr p_class_name, KSExtensionConstStringNamePtr p_parent_class_name, const KSExtensionClassCreationInfo2 *p_extension_funcs);

/**
 * @name classdb_register_extension_class3
 * @since 4.3
 * @deprecated in Kosmic 4.4. Use `classdb_register_extension_class4` instead.
 *
 * Registers an extension class in the ClassDB.
 *
 * Provided struct can be safely freed once the function returns.
 *
 * @param p_library A pointer the library received by the KSExtension's entry point function.
 * @param p_class_name A pointer to a StringName with the class name.
 * @param p_parent_class_name A pointer to a StringName with the parent class name.
 * @param p_extension_funcs A pointer to a KSExtensionClassCreationInfo2 struct.
 */
typedef void (*KSExtensionInterfaceClassdbRegisterExtensionClass3)(KSExtensionClassLibraryPtr p_library, KSExtensionConstStringNamePtr p_class_name, KSExtensionConstStringNamePtr p_parent_class_name, const KSExtensionClassCreationInfo3 *p_extension_funcs);

/**
 * @name classdb_register_extension_class4
 * @since 4.4
 *
 * Registers an extension class in the ClassDB.
 *
 * Provided struct can be safely freed once the function returns.
 *
 * @param p_library A pointer the library received by the KSExtension's entry point function.
 * @param p_class_name A pointer to a StringName with the class name.
 * @param p_parent_class_name A pointer to a StringName with the parent class name.
 * @param p_extension_funcs A pointer to a KSExtensionClassCreationInfo2 struct.
 */
typedef void (*KSExtensionInterfaceClassdbRegisterExtensionClass4)(KSExtensionClassLibraryPtr p_library, KSExtensionConstStringNamePtr p_class_name, KSExtensionConstStringNamePtr p_parent_class_name, const KSExtensionClassCreationInfo4 *p_extension_funcs);

/**
 * @name classdb_register_extension_class_method
 * @since 4.1
 *
 * Registers a method on an extension class in the ClassDB.
 *
 * Provided struct can be safely freed once the function returns.
 *
 * @param p_library A pointer the library received by the KSExtension's entry point function.
 * @param p_class_name A pointer to a StringName with the class name.
 * @param p_method_info A pointer to a KSExtensionClassMethodInfo struct.
 */
typedef void (*KSExtensionInterfaceClassdbRegisterExtensionClassMethod)(KSExtensionClassLibraryPtr p_library, KSExtensionConstStringNamePtr p_class_name, const KSExtensionClassMethodInfo *p_method_info);

/**
 * @name classdb_register_extension_class_virtual_method
 * @since 4.3
 *
 * Registers a virtual method on an extension class in ClassDB, that can be implemented by scripts or other extensions.
 *
 * Provided struct can be safely freed once the function returns.
 *
 * @param p_library A pointer the library received by the KSExtension's entry point function.
 * @param p_class_name A pointer to a StringName with the class name.
 * @param p_method_info A pointer to a KSExtensionClassMethodInfo struct.
 */
typedef void (*KSExtensionInterfaceClassdbRegisterExtensionClassVirtualMethod)(KSExtensionClassLibraryPtr p_library, KSExtensionConstStringNamePtr p_class_name, const KSExtensionClassVirtualMethodInfo *p_method_info);

/**
 * @name classdb_register_extension_class_integer_constant
 * @since 4.1
 *
 * Registers an integer constant on an extension class in the ClassDB.
 *
 * Note about registering bitfield values (if p_is_bitfield is true): even though p_constant_value is signed, language bindings are
 * advised to treat bitfields as uint64_t, since this is generally clearer and can prevent mistakes like using -1 for setting all bits.
 * Language APIs should thus provide an abstraction that registers bitfields (uint64_t) separately from regular constants (int64_t).
 *
 * @param p_library A pointer the library received by the KSExtension's entry point function.
 * @param p_class_name A pointer to a StringName with the class name.
 * @param p_enum_name A pointer to a StringName with the enum name.
 * @param p_constant_name A pointer to a StringName with the constant name.
 * @param p_constant_value The constant value.
 * @param p_is_bitfield Whether or not this constant is part of a bitfield.
 */
typedef void (*KSExtensionInterfaceClassdbRegisterExtensionClassIntegerConstant)(KSExtensionClassLibraryPtr p_library, KSExtensionConstStringNamePtr p_class_name, KSExtensionConstStringNamePtr p_enum_name, KSExtensionConstStringNamePtr p_constant_name, KSExtensionInt p_constant_value, KSExtensionBool p_is_bitfield);

/**
 * @name classdb_register_extension_class_property
 * @since 4.1
 *
 * Registers a property on an extension class in the ClassDB.
 *
 * Provided struct can be safely freed once the function returns.
 *
 * @param p_library A pointer the library received by the KSExtension's entry point function.
 * @param p_class_name A pointer to a StringName with the class name.
 * @param p_info A pointer to a KSExtensionPropertyInfo struct.
 * @param p_setter A pointer to a StringName with the name of the setter method.
 * @param p_getter A pointer to a StringName with the name of the getter method.
 */
typedef void (*KSExtensionInterfaceClassdbRegisterExtensionClassProperty)(KSExtensionClassLibraryPtr p_library, KSExtensionConstStringNamePtr p_class_name, const KSExtensionPropertyInfo *p_info, KSExtensionConstStringNamePtr p_setter, KSExtensionConstStringNamePtr p_getter);

/**
 * @name classdb_register_extension_class_property_indexed
 * @since 4.2
 *
 * Registers an indexed property on an extension class in the ClassDB.
 *
 * Provided struct can be safely freed once the function returns.
 *
 * @param p_library A pointer the library received by the KSExtension's entry point function.
 * @param p_class_name A pointer to a StringName with the class name.
 * @param p_info A pointer to a KSExtensionPropertyInfo struct.
 * @param p_setter A pointer to a StringName with the name of the setter method.
 * @param p_getter A pointer to a StringName with the name of the getter method.
 * @param p_index The index to pass as the first argument to the getter and setter methods.
 */
typedef void (*KSExtensionInterfaceClassdbRegisterExtensionClassPropertyIndexed)(KSExtensionClassLibraryPtr p_library, KSExtensionConstStringNamePtr p_class_name, const KSExtensionPropertyInfo *p_info, KSExtensionConstStringNamePtr p_setter, KSExtensionConstStringNamePtr p_getter, KSExtensionInt p_index);

/**
 * @name classdb_register_extension_class_property_group
 * @since 4.1
 *
 * Registers a property group on an extension class in the ClassDB.
 *
 * @param p_library A pointer the library received by the KSExtension's entry point function.
 * @param p_class_name A pointer to a StringName with the class name.
 * @param p_group_name A pointer to a String with the group name.
 * @param p_prefix A pointer to a String with the prefix used by properties in this group.
 */
typedef void (*KSExtensionInterfaceClassdbRegisterExtensionClassPropertyGroup)(KSExtensionClassLibraryPtr p_library, KSExtensionConstStringNamePtr p_class_name, KSExtensionConstStringPtr p_group_name, KSExtensionConstStringPtr p_prefix);

/**
 * @name classdb_register_extension_class_property_subgroup
 * @since 4.1
 *
 * Registers a property subgroup on an extension class in the ClassDB.
 *
 * @param p_library A pointer the library received by the KSExtension's entry point function.
 * @param p_class_name A pointer to a StringName with the class name.
 * @param p_subgroup_name A pointer to a String with the subgroup name.
 * @param p_prefix A pointer to a String with the prefix used by properties in this subgroup.
 */
typedef void (*KSExtensionInterfaceClassdbRegisterExtensionClassPropertySubgroup)(KSExtensionClassLibraryPtr p_library, KSExtensionConstStringNamePtr p_class_name, KSExtensionConstStringPtr p_subgroup_name, KSExtensionConstStringPtr p_prefix);

/**
 * @name classdb_register_extension_class_signal
 * @since 4.1
 *
 * Registers a signal on an extension class in the ClassDB.
 *
 * Provided structs can be safely freed once the function returns.
 *
 * @param p_library A pointer the library received by the KSExtension's entry point function.
 * @param p_class_name A pointer to a StringName with the class name.
 * @param p_signal_name A pointer to a StringName with the signal name.
 * @param p_argument_info A pointer to a KSExtensionPropertyInfo struct.
 * @param p_argument_count The number of arguments the signal receives.
 */
typedef void (*KSExtensionInterfaceClassdbRegisterExtensionClassSignal)(KSExtensionClassLibraryPtr p_library, KSExtensionConstStringNamePtr p_class_name, KSExtensionConstStringNamePtr p_signal_name, const KSExtensionPropertyInfo *p_argument_info, KSExtensionInt p_argument_count);

/**
 * @name classdb_unregister_extension_class
 * @since 4.1
 *
 * Unregisters an extension class in the ClassDB.
 *
 * @param p_library A pointer the library received by the KSExtension's entry point function.
 * @param p_class_name A pointer to a StringName with the class name.
 */
typedef void (*KSExtensionInterfaceClassdbUnregisterExtensionClass)(KSExtensionClassLibraryPtr p_library, KSExtensionConstStringNamePtr p_class_name); /* Unregistering a parent class before a class that inherits it will result in failure. Inheritors must be unregistered first. */

/**
 * @name get_library_path
 * @since 4.1
 *
 * Gets the path to the current KSExtension library.
 *
 * @param p_library A pointer the library received by the KSExtension's entry point function.
 * @param r_path A pointer to a String which will receive the path.
 */
typedef void (*KSExtensionInterfaceGetLibraryPath)(KSExtensionClassLibraryPtr p_library, KSExtensionUninitializedStringPtr r_path);

/**
 * @name editor_add_plugin
 * @since 4.1
 *
 * Adds an editor plugin.
 *
 * It's safe to call during initialization.
 *
 * @param p_class_name A pointer to a StringName with the name of a class (descending from EditorPlugin) which is already registered with ClassDB.
 */
typedef void (*KSExtensionInterfaceEditorAddPlugin)(KSExtensionConstStringNamePtr p_class_name);

/**
 * @name editor_remove_plugin
 * @since 4.1
 *
 * Removes an editor plugin.
 *
 * @param p_class_name A pointer to a StringName with the name of a class that was previously added as an editor plugin.
 */
typedef void (*KSExtensionInterfaceEditorRemovePlugin)(KSExtensionConstStringNamePtr p_class_name);

/**
 * @name editor_help_load_xml_from_utf8_chars
 * @since 4.3
 *
 * Loads new XML-formatted documentation data in the editor.
 *
 * The provided pointer can be immediately freed once the function returns.
 *
 * @param p_data A pointer to a UTF-8 encoded C string (null terminated).
 */
typedef void (*KSExtensionsInterfaceEditorHelpLoadXmlFromUtf8Chars)(const char *p_data);

/**
 * @name editor_help_load_xml_from_utf8_chars_and_len
 * @since 4.3
 *
 * Loads new XML-formatted documentation data in the editor.
 *
 * The provided pointer can be immediately freed once the function returns.
 *
 * @param p_data A pointer to a UTF-8 encoded C string.
 * @param p_size The number of bytes (not code units).
 */
typedef void (*KSExtensionsInterfaceEditorHelpLoadXmlFromUtf8CharsAndLen)(const char *p_data, KSExtensionInt p_size);

#ifdef __cplusplus
}
#endif

#endif // KSEXTENSION_INTERFACE_H
