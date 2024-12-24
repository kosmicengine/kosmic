#pragma warning disable CA1707 // Identifiers should not contain underscores
#pragma warning disable IDE1006 // Naming rule violation
// ReSharper disable InconsistentNaming

namespace Kosmic.NativeInterop
{
    public static partial class NativeFuncs
    {
        public static kosmic_variant kosmicsharp_variant_new_copy(scoped in kosmic_variant src)
        {
            switch (src.Type)
            {
                case Variant.Type.Nil:
                    return default;
                case Variant.Type.Bool:
                    return new kosmic_variant() { Bool = src.Bool, Type = Variant.Type.Bool };
                case Variant.Type.Int:
                    return new kosmic_variant() { Int = src.Int, Type = Variant.Type.Int };
                case Variant.Type.Float:
                    return new kosmic_variant() { Float = src.Float, Type = Variant.Type.Float };
                case Variant.Type.Vector2:
                    return new kosmic_variant() { Vector2 = src.Vector2, Type = Variant.Type.Vector2 };
                case Variant.Type.Vector2I:
                    return new kosmic_variant() { Vector2I = src.Vector2I, Type = Variant.Type.Vector2I };
                case Variant.Type.Rect2:
                    return new kosmic_variant() { Rect2 = src.Rect2, Type = Variant.Type.Rect2 };
                case Variant.Type.Rect2I:
                    return new kosmic_variant() { Rect2I = src.Rect2I, Type = Variant.Type.Rect2I };
                case Variant.Type.Vector3:
                    return new kosmic_variant() { Vector3 = src.Vector3, Type = Variant.Type.Vector3 };
                case Variant.Type.Vector3I:
                    return new kosmic_variant() { Vector3I = src.Vector3I, Type = Variant.Type.Vector3I };
                case Variant.Type.Vector4:
                    return new kosmic_variant() { Vector4 = src.Vector4, Type = Variant.Type.Vector4 };
                case Variant.Type.Vector4I:
                    return new kosmic_variant() { Vector4I = src.Vector4I, Type = Variant.Type.Vector4I };
                case Variant.Type.Plane:
                    return new kosmic_variant() { Plane = src.Plane, Type = Variant.Type.Plane };
                case Variant.Type.Quaternion:
                    return new kosmic_variant() { Quaternion = src.Quaternion, Type = Variant.Type.Quaternion };
                case Variant.Type.Color:
                    return new kosmic_variant() { Color = src.Color, Type = Variant.Type.Color };
                case Variant.Type.Rid:
                    return new kosmic_variant() { Rid = src.Rid, Type = Variant.Type.Rid };
            }

            kosmicsharp_variant_new_copy(out kosmic_variant ret, src);
            return ret;
        }

        public static kosmic_string_name kosmicsharp_string_name_new_copy(scoped in kosmic_string_name src)
        {
            if (src.IsEmpty)
                return default;
            kosmicsharp_string_name_new_copy(out kosmic_string_name ret, src);
            return ret;
        }

        public static kosmic_node_path kosmicsharp_node_path_new_copy(scoped in kosmic_node_path src)
        {
            if (src.IsEmpty)
                return default;
            kosmicsharp_node_path_new_copy(out kosmic_node_path ret, src);
            return ret;
        }

        public static kosmic_array kosmicsharp_array_new()
        {
            kosmicsharp_array_new(out kosmic_array ret);
            return ret;
        }

        public static kosmic_array kosmicsharp_array_new_copy(scoped in kosmic_array src)
        {
            kosmicsharp_array_new_copy(out kosmic_array ret, src);
            return ret;
        }

        public static kosmic_dictionary kosmicsharp_dictionary_new()
        {
            kosmicsharp_dictionary_new(out kosmic_dictionary ret);
            return ret;
        }

        public static kosmic_dictionary kosmicsharp_dictionary_new_copy(scoped in kosmic_dictionary src)
        {
            kosmicsharp_dictionary_new_copy(out kosmic_dictionary ret, src);
            return ret;
        }

        public static kosmic_string_name kosmicsharp_string_name_new_from_string(string name)
        {
            using kosmic_string src = Marshaling.ConvertStringToNative(name);
            kosmicsharp_string_name_new_from_string(out kosmic_string_name ret, src);
            return ret;
        }

        public static kosmic_node_path kosmicsharp_node_path_new_from_string(string name)
        {
            using kosmic_string src = Marshaling.ConvertStringToNative(name);
            kosmicsharp_node_path_new_from_string(out kosmic_node_path ret, src);
            return ret;
        }
    }
}
