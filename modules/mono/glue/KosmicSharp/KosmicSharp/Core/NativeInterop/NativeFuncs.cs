#pragma warning disable CA1707 // Identifiers should not contain underscores
#pragma warning disable IDE1006 // Naming rule violation
// ReSharper disable InconsistentNaming

using System;
using System.Runtime.CompilerServices;
using Kosmic.SourceGenerators.Internal;


namespace Kosmic.NativeInterop
{
    /*
     * IMPORTANT:
     * The order of the methods defined in NativeFuncs must match the order
     * in the array defined at the bottom of 'glue/runtime_interop.cpp'.
     */

    [GenerateUnmanagedCallbacks(typeof(UnmanagedCallbacks))]
    public static unsafe partial class NativeFuncs
    {
        private static bool initialized;

        // ReSharper disable once ParameterOnlyUsedForPreconditionCheck.Global
        public static void Initialize(IntPtr unmanagedCallbacks, int unmanagedCallbacksSize)
        {
            if (initialized)
                throw new InvalidOperationException("Already initialized.");
            initialized = true;

            if (unmanagedCallbacksSize != sizeof(UnmanagedCallbacks))
                throw new ArgumentException("Unmanaged callbacks size mismatch.", nameof(unmanagedCallbacksSize));

            _unmanagedCallbacks = Unsafe.AsRef<UnmanagedCallbacks>((void*)unmanagedCallbacks);
        }

        private partial struct UnmanagedCallbacks
        {
        }

        // Custom functions

        internal static partial kosmic_bool kosmicsharp_dotnet_module_is_initialized();

        public static partial IntPtr kosmicsharp_method_bind_get_method(in kosmic_string_name p_classname,
            in kosmic_string_name p_methodname);

        public static partial IntPtr kosmicsharp_method_bind_get_method_with_compatibility(
            in kosmic_string_name p_classname, in kosmic_string_name p_methodname, ulong p_hash);

        public static partial delegate* unmanaged<kosmic_bool, IntPtr> kosmicsharp_get_class_constructor(
            in kosmic_string_name p_classname);

        public static partial IntPtr kosmicsharp_engine_get_singleton(in kosmic_string p_name);


        internal static partial Error kosmicsharp_stack_info_vector_resize(
            ref DebuggingUtils.kosmic_stack_info_vector p_stack_info_vector, int p_size);

        internal static partial void kosmicsharp_stack_info_vector_destroy(
            ref DebuggingUtils.kosmic_stack_info_vector p_stack_info_vector);

        internal static partial void kosmicsharp_internal_editor_file_system_update_files(in kosmic_packed_string_array p_script_paths);

        internal static partial void kosmicsharp_internal_script_debugger_send_error(in kosmic_string p_func,
            in kosmic_string p_file, int p_line, in kosmic_string p_err, in kosmic_string p_descr,
            kosmic_error_handler_type p_type, in DebuggingUtils.kosmic_stack_info_vector p_stack_info_vector);

        internal static partial kosmic_bool kosmicsharp_internal_script_debugger_is_active();

        internal static partial IntPtr kosmicsharp_internal_object_get_associated_gchandle(IntPtr ptr);

        internal static partial void kosmicsharp_internal_object_disposed(IntPtr ptr, IntPtr gcHandleToFree);

        internal static partial void kosmicsharp_internal_refcounted_disposed(IntPtr ptr, IntPtr gcHandleToFree,
            kosmic_bool isFinalizer);

        internal static partial Error kosmicsharp_internal_signal_awaiter_connect(IntPtr source,
            in kosmic_string_name signal,
            IntPtr target, IntPtr awaiterHandlePtr);

        internal static partial void kosmicsharp_internal_tie_native_managed_to_unmanaged(IntPtr gcHandleIntPtr,
            IntPtr unmanaged, in kosmic_string_name nativeName, kosmic_bool refCounted);

        internal static partial void kosmicsharp_internal_tie_user_managed_to_unmanaged(IntPtr gcHandleIntPtr,
            IntPtr unmanaged, kosmic_ref* scriptPtr, kosmic_bool refCounted);

        internal static partial void kosmicsharp_internal_tie_managed_to_unmanaged_with_pre_setup(
            IntPtr gcHandleIntPtr, IntPtr unmanaged);

        internal static partial IntPtr kosmicsharp_internal_unmanaged_get_script_instance_managed(IntPtr p_unmanaged,
            out kosmic_bool r_has_cs_script_instance);

        internal static partial IntPtr kosmicsharp_internal_unmanaged_get_instance_binding_managed(IntPtr p_unmanaged);

        internal static partial IntPtr kosmicsharp_internal_unmanaged_instance_binding_create_managed(IntPtr p_unmanaged,
            IntPtr oldGCHandlePtr);

        internal static partial void kosmicsharp_internal_new_csharp_script(kosmic_ref* r_dest);

        internal static partial kosmic_bool kosmicsharp_internal_script_load(in kosmic_string p_path, kosmic_ref* r_dest);

        internal static partial void kosmicsharp_internal_reload_registered_script(IntPtr scriptPtr);

        internal static partial void kosmicsharp_array_filter_kosmic_objects_by_native(scoped in kosmic_string_name p_native_name,
            scoped in kosmic_array p_input, out kosmic_array r_output);

        internal static partial void kosmicsharp_array_filter_kosmic_objects_by_non_native(scoped in kosmic_array p_input,
            out kosmic_array r_output);

        public static partial void kosmicsharp_ref_new_from_ref_counted_ptr(out kosmic_ref r_dest,
            IntPtr p_ref_counted_ptr);

        public static partial void kosmicsharp_ref_destroy(ref kosmic_ref p_instance);

        public static partial void kosmicsharp_string_name_new_from_string(out kosmic_string_name r_dest,
            scoped in kosmic_string p_name);

        public static partial void kosmicsharp_node_path_new_from_string(out kosmic_node_path r_dest,
            scoped in kosmic_string p_name);

        public static partial void
            kosmicsharp_string_name_as_string(out kosmic_string r_dest, scoped in kosmic_string_name p_name);

        public static partial void kosmicsharp_node_path_as_string(out kosmic_string r_dest, scoped in kosmic_node_path p_np);

        public static partial kosmic_packed_byte_array kosmicsharp_packed_byte_array_new_mem_copy(byte* p_src,
            int p_length);

        public static partial kosmic_packed_int32_array kosmicsharp_packed_int32_array_new_mem_copy(int* p_src,
            int p_length);

        public static partial kosmic_packed_int64_array kosmicsharp_packed_int64_array_new_mem_copy(long* p_src,
            int p_length);

        public static partial kosmic_packed_float32_array kosmicsharp_packed_float32_array_new_mem_copy(float* p_src,
            int p_length);

        public static partial kosmic_packed_float64_array kosmicsharp_packed_float64_array_new_mem_copy(double* p_src,
            int p_length);

        public static partial kosmic_packed_vector2_array kosmicsharp_packed_vector2_array_new_mem_copy(Vector2* p_src,
            int p_length);

        public static partial kosmic_packed_vector3_array kosmicsharp_packed_vector3_array_new_mem_copy(Vector3* p_src,
            int p_length);

        public static partial kosmic_packed_vector4_array kosmicsharp_packed_vector4_array_new_mem_copy(Vector4* p_src,
            int p_length);

        public static partial kosmic_packed_color_array kosmicsharp_packed_color_array_new_mem_copy(Color* p_src,
            int p_length);

        public static partial void kosmicsharp_packed_string_array_add(ref kosmic_packed_string_array r_dest,
            in kosmic_string p_element);

        public static partial void kosmicsharp_callable_new_with_delegate(IntPtr p_delegate_handle, IntPtr p_trampoline,
            IntPtr p_object, out kosmic_callable r_callable);

        internal static partial kosmic_bool kosmicsharp_callable_get_data_for_marshalling(scoped in kosmic_callable p_callable,
            out IntPtr r_delegate_handle, out IntPtr r_trampoline, out IntPtr r_object, out kosmic_string_name r_name);

        internal static partial kosmic_variant kosmicsharp_callable_call(scoped in kosmic_callable p_callable,
            kosmic_variant** p_args, int p_arg_count, out kosmic_variant_call_error p_call_error);

        internal static partial void kosmicsharp_callable_call_deferred(in kosmic_callable p_callable,
            kosmic_variant** p_args, int p_arg_count);

        internal static partial Color kosmicsharp_color_from_ok_hsl(float p_h, float p_s, float p_l, float p_alpha);

        // GDNative functions

        // gdnative.h

        public static partial void kosmicsharp_method_bind_ptrcall(IntPtr p_method_bind, IntPtr p_instance, void** p_args,
            void* p_ret);

        public static partial kosmic_variant kosmicsharp_method_bind_call(IntPtr p_method_bind, IntPtr p_instance,
            kosmic_variant** p_args, int p_arg_count, out kosmic_variant_call_error p_call_error);

        // variant.h

        public static partial void
            kosmicsharp_variant_new_string_name(out kosmic_variant r_dest, scoped in kosmic_string_name p_s);

        public static partial void kosmicsharp_variant_new_copy(out kosmic_variant r_dest, scoped in kosmic_variant p_src);

        public static partial void kosmicsharp_variant_new_node_path(out kosmic_variant r_dest, scoped in kosmic_node_path p_np);

        public static partial void kosmicsharp_variant_new_object(out kosmic_variant r_dest, IntPtr p_obj);

        public static partial void kosmicsharp_variant_new_transform2d(out kosmic_variant r_dest, scoped in Transform2D p_t2d);

        public static partial void kosmicsharp_variant_new_basis(out kosmic_variant r_dest, scoped in Basis p_basis);

        public static partial void kosmicsharp_variant_new_transform3d(out kosmic_variant r_dest, scoped in Transform3D p_trans);

        public static partial void kosmicsharp_variant_new_projection(out kosmic_variant r_dest, scoped in Projection p_proj);

        public static partial void kosmicsharp_variant_new_aabb(out kosmic_variant r_dest, scoped in Aabb p_aabb);

        public static partial void kosmicsharp_variant_new_dictionary(out kosmic_variant r_dest,
            scoped in kosmic_dictionary p_dict);

        public static partial void kosmicsharp_variant_new_array(out kosmic_variant r_dest, scoped in kosmic_array p_arr);

        public static partial void kosmicsharp_variant_new_packed_byte_array(out kosmic_variant r_dest,
            scoped in kosmic_packed_byte_array p_pba);

        public static partial void kosmicsharp_variant_new_packed_int32_array(out kosmic_variant r_dest,
            scoped in kosmic_packed_int32_array p_pia);

        public static partial void kosmicsharp_variant_new_packed_int64_array(out kosmic_variant r_dest,
            scoped in kosmic_packed_int64_array p_pia);

        public static partial void kosmicsharp_variant_new_packed_float32_array(out kosmic_variant r_dest,
            scoped in kosmic_packed_float32_array p_pra);

        public static partial void kosmicsharp_variant_new_packed_float64_array(out kosmic_variant r_dest,
            scoped in kosmic_packed_float64_array p_pra);

        public static partial void kosmicsharp_variant_new_packed_string_array(out kosmic_variant r_dest,
            scoped in kosmic_packed_string_array p_psa);

        public static partial void kosmicsharp_variant_new_packed_vector2_array(out kosmic_variant r_dest,
            scoped in kosmic_packed_vector2_array p_pv2a);

        public static partial void kosmicsharp_variant_new_packed_vector3_array(out kosmic_variant r_dest,
            scoped in kosmic_packed_vector3_array p_pv3a);

        public static partial void kosmicsharp_variant_new_packed_vector4_array(out kosmic_variant r_dest,
            scoped in kosmic_packed_vector4_array p_pv4a);

        public static partial void kosmicsharp_variant_new_packed_color_array(out kosmic_variant r_dest,
            scoped in kosmic_packed_color_array p_pca);

        public static partial kosmic_bool kosmicsharp_variant_as_bool(scoped in kosmic_variant p_self);

        public static partial Int64 kosmicsharp_variant_as_int(scoped in kosmic_variant p_self);

        public static partial double kosmicsharp_variant_as_float(scoped in kosmic_variant p_self);

        public static partial kosmic_string kosmicsharp_variant_as_string(scoped in kosmic_variant p_self);

        public static partial Vector2 kosmicsharp_variant_as_vector2(scoped in kosmic_variant p_self);

        public static partial Vector2I kosmicsharp_variant_as_vector2i(scoped in kosmic_variant p_self);

        public static partial Rect2 kosmicsharp_variant_as_rect2(scoped in kosmic_variant p_self);

        public static partial Rect2I kosmicsharp_variant_as_rect2i(scoped in kosmic_variant p_self);

        public static partial Vector3 kosmicsharp_variant_as_vector3(scoped in kosmic_variant p_self);

        public static partial Vector3I kosmicsharp_variant_as_vector3i(scoped in kosmic_variant p_self);

        public static partial Transform2D kosmicsharp_variant_as_transform2d(scoped in kosmic_variant p_self);

        public static partial Vector4 kosmicsharp_variant_as_vector4(scoped in kosmic_variant p_self);

        public static partial Vector4I kosmicsharp_variant_as_vector4i(scoped in kosmic_variant p_self);

        public static partial Plane kosmicsharp_variant_as_plane(scoped in kosmic_variant p_self);

        public static partial Quaternion kosmicsharp_variant_as_quaternion(scoped in kosmic_variant p_self);

        public static partial Aabb kosmicsharp_variant_as_aabb(scoped in kosmic_variant p_self);

        public static partial Basis kosmicsharp_variant_as_basis(scoped in kosmic_variant p_self);

        public static partial Transform3D kosmicsharp_variant_as_transform3d(scoped in kosmic_variant p_self);

        public static partial Projection kosmicsharp_variant_as_projection(scoped in kosmic_variant p_self);

        public static partial Color kosmicsharp_variant_as_color(scoped in kosmic_variant p_self);

        public static partial kosmic_string_name kosmicsharp_variant_as_string_name(scoped in kosmic_variant p_self);

        public static partial kosmic_node_path kosmicsharp_variant_as_node_path(scoped in kosmic_variant p_self);

        public static partial Rid kosmicsharp_variant_as_rid(scoped in kosmic_variant p_self);

        public static partial kosmic_callable kosmicsharp_variant_as_callable(scoped in kosmic_variant p_self);

        public static partial kosmic_signal kosmicsharp_variant_as_signal(scoped in kosmic_variant p_self);

        public static partial kosmic_dictionary kosmicsharp_variant_as_dictionary(scoped in kosmic_variant p_self);

        public static partial kosmic_array kosmicsharp_variant_as_array(scoped in kosmic_variant p_self);

        public static partial kosmic_packed_byte_array kosmicsharp_variant_as_packed_byte_array(scoped in kosmic_variant p_self);

        public static partial kosmic_packed_int32_array kosmicsharp_variant_as_packed_int32_array(scoped in kosmic_variant p_self);

        public static partial kosmic_packed_int64_array kosmicsharp_variant_as_packed_int64_array(scoped in kosmic_variant p_self);

        public static partial kosmic_packed_float32_array kosmicsharp_variant_as_packed_float32_array(scoped in kosmic_variant p_self);

        public static partial kosmic_packed_float64_array kosmicsharp_variant_as_packed_float64_array(scoped in kosmic_variant p_self);

        public static partial kosmic_packed_string_array kosmicsharp_variant_as_packed_string_array(scoped in kosmic_variant p_self);

        public static partial kosmic_packed_vector2_array kosmicsharp_variant_as_packed_vector2_array(scoped in kosmic_variant p_self);

        public static partial kosmic_packed_vector3_array kosmicsharp_variant_as_packed_vector3_array(scoped in kosmic_variant p_self);

        public static partial kosmic_packed_vector4_array kosmicsharp_variant_as_packed_vector4_array(
            in kosmic_variant p_self);

        public static partial kosmic_packed_color_array kosmicsharp_variant_as_packed_color_array(scoped in kosmic_variant p_self);

        public static partial kosmic_bool kosmicsharp_variant_equals(scoped in kosmic_variant p_a, scoped in kosmic_variant p_b);

        // string.h

        public static partial void kosmicsharp_string_new_with_utf16_chars(out kosmic_string r_dest, char* p_contents);

        // string_name.h

        public static partial void kosmicsharp_string_name_new_copy(out kosmic_string_name r_dest,
            scoped in kosmic_string_name p_src);

        // node_path.h

        public static partial void kosmicsharp_node_path_new_copy(out kosmic_node_path r_dest, scoped in kosmic_node_path p_src);

        // array.h

        public static partial void kosmicsharp_array_new(out kosmic_array r_dest);

        public static partial void kosmicsharp_array_new_copy(out kosmic_array r_dest, scoped in kosmic_array p_src);

        public static partial kosmic_variant* kosmicsharp_array_ptrw(ref kosmic_array p_self);

        // dictionary.h

        public static partial void kosmicsharp_dictionary_new(out kosmic_dictionary r_dest);

        public static partial void kosmicsharp_dictionary_new_copy(out kosmic_dictionary r_dest,
            scoped in kosmic_dictionary p_src);

        // destroy functions

        public static partial void kosmicsharp_packed_byte_array_destroy(ref kosmic_packed_byte_array p_self);

        public static partial void kosmicsharp_packed_int32_array_destroy(ref kosmic_packed_int32_array p_self);

        public static partial void kosmicsharp_packed_int64_array_destroy(ref kosmic_packed_int64_array p_self);

        public static partial void kosmicsharp_packed_float32_array_destroy(ref kosmic_packed_float32_array p_self);

        public static partial void kosmicsharp_packed_float64_array_destroy(ref kosmic_packed_float64_array p_self);

        public static partial void kosmicsharp_packed_string_array_destroy(ref kosmic_packed_string_array p_self);

        public static partial void kosmicsharp_packed_vector2_array_destroy(ref kosmic_packed_vector2_array p_self);

        public static partial void kosmicsharp_packed_vector3_array_destroy(ref kosmic_packed_vector3_array p_self);

        public static partial void kosmicsharp_packed_vector4_array_destroy(ref kosmic_packed_vector4_array p_self);

        public static partial void kosmicsharp_packed_color_array_destroy(ref kosmic_packed_color_array p_self);

        public static partial void kosmicsharp_variant_destroy(ref kosmic_variant p_self);

        public static partial void kosmicsharp_string_destroy(ref kosmic_string p_self);

        public static partial void kosmicsharp_string_name_destroy(ref kosmic_string_name p_self);

        public static partial void kosmicsharp_node_path_destroy(ref kosmic_node_path p_self);

        public static partial void kosmicsharp_signal_destroy(ref kosmic_signal p_self);

        public static partial void kosmicsharp_callable_destroy(ref kosmic_callable p_self);

        public static partial void kosmicsharp_array_destroy(ref kosmic_array p_self);

        public static partial void kosmicsharp_dictionary_destroy(ref kosmic_dictionary p_self);

        // Array

        public static partial int kosmicsharp_array_add(ref kosmic_array p_self, in kosmic_variant p_item);

        public static partial int kosmicsharp_array_add_range(ref kosmic_array p_self, in kosmic_array p_collection);

        public static partial int kosmicsharp_array_binary_search(ref kosmic_array p_self, int p_index, int p_count, in kosmic_variant p_value);

        public static partial void kosmicsharp_array_duplicate(scoped ref kosmic_array p_self, kosmic_bool p_deep, out kosmic_array r_dest);

        public static partial void kosmicsharp_array_fill(ref kosmic_array p_self, in kosmic_variant p_value);

        public static partial int kosmicsharp_array_index_of(ref kosmic_array p_self, in kosmic_variant p_item, int p_index = 0);

        public static partial void kosmicsharp_array_insert(ref kosmic_array p_self, int p_index, in kosmic_variant p_item);

        public static partial int kosmicsharp_array_last_index_of(ref kosmic_array p_self, in kosmic_variant p_item, int p_index);

        public static partial void kosmicsharp_array_make_read_only(ref kosmic_array p_self);

        public static partial void kosmicsharp_array_set_typed(
            ref kosmic_array p_self,
            uint p_elem_type,
            in kosmic_string_name p_elem_class_name,
            in kosmic_ref p_elem_script);

        public static partial kosmic_bool kosmicsharp_array_is_typed(ref kosmic_array p_self);

        public static partial void kosmicsharp_array_max(scoped ref kosmic_array p_self, out kosmic_variant r_value);

        public static partial void kosmicsharp_array_min(scoped ref kosmic_array p_self, out kosmic_variant r_value);

        public static partial void kosmicsharp_array_pick_random(scoped ref kosmic_array p_self, out kosmic_variant r_value);

        public static partial kosmic_bool kosmicsharp_array_recursive_equal(ref kosmic_array p_self, in kosmic_array p_other);

        public static partial void kosmicsharp_array_remove_at(ref kosmic_array p_self, int p_index);

        public static partial Error kosmicsharp_array_resize(ref kosmic_array p_self, int p_new_size);

        public static partial void kosmicsharp_array_reverse(ref kosmic_array p_self);

        public static partial void kosmicsharp_array_shuffle(ref kosmic_array p_self);

        public static partial void kosmicsharp_array_slice(scoped ref kosmic_array p_self, int p_start, int p_end,
            int p_step, kosmic_bool p_deep, out kosmic_array r_dest);

        public static partial void kosmicsharp_array_sort(ref kosmic_array p_self);

        public static partial void kosmicsharp_array_to_string(ref kosmic_array p_self, out kosmic_string r_str);

        // Dictionary

        public static partial kosmic_bool kosmicsharp_dictionary_try_get_value(scoped ref kosmic_dictionary p_self,
            scoped in kosmic_variant p_key,
            out kosmic_variant r_value);

        public static partial void kosmicsharp_dictionary_set_value(ref kosmic_dictionary p_self, in kosmic_variant p_key,
            in kosmic_variant p_value);

        public static partial void kosmicsharp_dictionary_keys(scoped ref kosmic_dictionary p_self, out kosmic_array r_dest);

        public static partial void kosmicsharp_dictionary_values(scoped ref kosmic_dictionary p_self, out kosmic_array r_dest);

        public static partial int kosmicsharp_dictionary_count(ref kosmic_dictionary p_self);

        public static partial void kosmicsharp_dictionary_key_value_pair_at(scoped ref kosmic_dictionary p_self, int p_index,
            out kosmic_variant r_key, out kosmic_variant r_value);

        public static partial void kosmicsharp_dictionary_add(ref kosmic_dictionary p_self, in kosmic_variant p_key,
            in kosmic_variant p_value);

        public static partial void kosmicsharp_dictionary_clear(ref kosmic_dictionary p_self);

        public static partial kosmic_bool kosmicsharp_dictionary_contains_key(ref kosmic_dictionary p_self,
            in kosmic_variant p_key);

        public static partial void kosmicsharp_dictionary_duplicate(scoped ref kosmic_dictionary p_self, kosmic_bool p_deep,
            out kosmic_dictionary r_dest);

        public static partial void kosmicsharp_dictionary_merge(ref kosmic_dictionary p_self, in kosmic_dictionary p_dictionary, kosmic_bool p_overwrite);

        public static partial kosmic_bool kosmicsharp_dictionary_recursive_equal(ref kosmic_dictionary p_self, in kosmic_dictionary p_other);

        public static partial kosmic_bool kosmicsharp_dictionary_remove_key(ref kosmic_dictionary p_self,
            in kosmic_variant p_key);

        public static partial void kosmicsharp_dictionary_make_read_only(ref kosmic_dictionary p_self);

        public static partial void kosmicsharp_dictionary_set_typed(
            ref kosmic_dictionary p_self,
            uint p_key_type,
            in kosmic_string_name p_key_class_name,
            in kosmic_ref p_key_script,
            uint p_value_type,
            in kosmic_string_name p_value_class_name,
            in kosmic_ref p_value_script);

        public static partial kosmic_bool kosmicsharp_dictionary_is_typed_key(ref kosmic_dictionary p_self);

        public static partial kosmic_bool kosmicsharp_dictionary_is_typed_value(ref kosmic_dictionary p_self);

        public static partial uint kosmicsharp_dictionary_get_typed_key_builtin(ref kosmic_dictionary p_self);

        public static partial uint kosmicsharp_dictionary_get_typed_value_builtin(ref kosmic_dictionary p_self);

        public static partial void kosmicsharp_dictionary_get_typed_key_class_name(ref kosmic_dictionary p_self, out kosmic_string_name r_dest);

        public static partial void kosmicsharp_dictionary_get_typed_value_class_name(ref kosmic_dictionary p_self, out kosmic_string_name r_dest);

        public static partial void kosmicsharp_dictionary_get_typed_key_script(ref kosmic_dictionary p_self, out kosmic_variant r_dest);

        public static partial void kosmicsharp_dictionary_get_typed_value_script(ref kosmic_dictionary p_self, out kosmic_variant r_dest);

        public static partial void kosmicsharp_dictionary_to_string(scoped ref kosmic_dictionary p_self, out kosmic_string r_str);

        // StringExtensions

        public static partial void kosmicsharp_string_simplify_path(scoped in kosmic_string p_self,
            out kosmic_string r_simplified_path);

        public static partial void kosmicsharp_string_to_camel_case(scoped in kosmic_string p_self,
            out kosmic_string r_camel_case);

        public static partial void kosmicsharp_string_to_pascal_case(scoped in kosmic_string p_self,
            out kosmic_string r_pascal_case);

        public static partial void kosmicsharp_string_to_snake_case(scoped in kosmic_string p_self,
            out kosmic_string r_snake_case);

        // NodePath

        public static partial void kosmicsharp_node_path_get_as_property_path(in kosmic_node_path p_self,
            ref kosmic_node_path r_dest);

        public static partial void kosmicsharp_node_path_get_concatenated_names(scoped in kosmic_node_path p_self,
            out kosmic_string r_names);

        public static partial void kosmicsharp_node_path_get_concatenated_subnames(scoped in kosmic_node_path p_self,
            out kosmic_string r_subnames);

        public static partial void kosmicsharp_node_path_get_name(scoped in kosmic_node_path p_self, int p_idx,
            out kosmic_string r_name);

        public static partial int kosmicsharp_node_path_get_name_count(in kosmic_node_path p_self);

        public static partial void kosmicsharp_node_path_get_subname(scoped in kosmic_node_path p_self, int p_idx,
            out kosmic_string r_subname);

        public static partial int kosmicsharp_node_path_get_subname_count(in kosmic_node_path p_self);

        public static partial kosmic_bool kosmicsharp_node_path_is_absolute(in kosmic_node_path p_self);

        public static partial kosmic_bool kosmicsharp_node_path_equals(in kosmic_node_path p_self, in kosmic_node_path p_other);

        public static partial int kosmicsharp_node_path_hash(in kosmic_node_path p_self);

        // GD, etc

        internal static partial void kosmicsharp_bytes_to_var(scoped in kosmic_packed_byte_array p_bytes,
            kosmic_bool p_allow_objects,
            out kosmic_variant r_ret);

        internal static partial void kosmicsharp_convert(scoped in kosmic_variant p_what, int p_type,
            out kosmic_variant r_ret);

        internal static partial int kosmicsharp_hash(in kosmic_variant p_var);

        internal static partial IntPtr kosmicsharp_instance_from_id(ulong p_instance_id);

        internal static partial void kosmicsharp_print(in kosmic_string p_what);

        public static partial void kosmicsharp_print_rich(in kosmic_string p_what);

        internal static partial void kosmicsharp_printerr(in kosmic_string p_what);

        internal static partial void kosmicsharp_printraw(in kosmic_string p_what);

        internal static partial void kosmicsharp_prints(in kosmic_string p_what);

        internal static partial void kosmicsharp_printt(in kosmic_string p_what);

        internal static partial float kosmicsharp_randf();

        internal static partial uint kosmicsharp_randi();

        internal static partial void kosmicsharp_randomize();

        internal static partial double kosmicsharp_randf_range(double from, double to);

        internal static partial double kosmicsharp_randfn(double mean, double deviation);

        internal static partial int kosmicsharp_randi_range(int from, int to);

        internal static partial uint kosmicsharp_rand_from_seed(ulong seed, out ulong newSeed);

        internal static partial void kosmicsharp_seed(ulong seed);

        internal static partial void kosmicsharp_weakref(IntPtr p_obj, out kosmic_ref r_weak_ref);

        internal static partial void kosmicsharp_str_to_var(scoped in kosmic_string p_str, out kosmic_variant r_ret);

        internal static partial void kosmicsharp_var_to_bytes(scoped in kosmic_variant p_what, kosmic_bool p_full_objects,
            out kosmic_packed_byte_array r_bytes);

        internal static partial void kosmicsharp_var_to_str(scoped in kosmic_variant p_var, out kosmic_string r_ret);

        internal static partial void kosmicsharp_err_print_error(in kosmic_string p_function, in kosmic_string p_file, int p_line, in kosmic_string p_error, in kosmic_string p_message = default, kosmic_bool p_editor_notify = kosmic_bool.False, kosmic_error_handler_type p_type = kosmic_error_handler_type.ERR_HANDLER_ERROR);

        // Object

        public static partial void kosmicsharp_object_to_string(IntPtr ptr, out kosmic_string r_str);
    }
}
