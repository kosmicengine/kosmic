partial class ExportedFields
{
#pragma warning disable CS0109 // Disable warning about redundant 'new' keyword
#if TOOLS
    /// <summary>
    /// Get the default values for all properties declared in this class.
    /// This method is used by Godot to determine the value that will be
    /// used by the inspector when resetting properties.
    /// Do not call this method.
    /// </summary>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    internal new static global::System.Collections.Generic.Dictionary<global::Kosmic.StringName, global::Kosmic.Variant> GetKosmicPropertyDefaultValues()
    {
        var values = new global::System.Collections.Generic.Dictionary<global::Kosmic.StringName, global::Kosmic.Variant>(60);
        bool ___fieldBoolean_default_value = true;
        values.Add(PropertyName.@_fieldBoolean, global::Kosmic.Variant.From<bool>(___fieldBoolean_default_value));
        char ___fieldChar_default_value = 'f';
        values.Add(PropertyName.@_fieldChar, global::Kosmic.Variant.From<char>(___fieldChar_default_value));
        sbyte ___fieldSByte_default_value = 10;
        values.Add(PropertyName.@_fieldSByte, global::Kosmic.Variant.From<sbyte>(___fieldSByte_default_value));
        short ___fieldInt16_default_value = 10;
        values.Add(PropertyName.@_fieldInt16, global::Kosmic.Variant.From<short>(___fieldInt16_default_value));
        int ___fieldInt32_default_value = 10;
        values.Add(PropertyName.@_fieldInt32, global::Kosmic.Variant.From<int>(___fieldInt32_default_value));
        long ___fieldInt64_default_value = 10;
        values.Add(PropertyName.@_fieldInt64, global::Kosmic.Variant.From<long>(___fieldInt64_default_value));
        byte ___fieldByte_default_value = 10;
        values.Add(PropertyName.@_fieldByte, global::Kosmic.Variant.From<byte>(___fieldByte_default_value));
        ushort ___fieldUInt16_default_value = 10;
        values.Add(PropertyName.@_fieldUInt16, global::Kosmic.Variant.From<ushort>(___fieldUInt16_default_value));
        uint ___fieldUInt32_default_value = 10;
        values.Add(PropertyName.@_fieldUInt32, global::Kosmic.Variant.From<uint>(___fieldUInt32_default_value));
        ulong ___fieldUInt64_default_value = 10;
        values.Add(PropertyName.@_fieldUInt64, global::Kosmic.Variant.From<ulong>(___fieldUInt64_default_value));
        float ___fieldSingle_default_value = 10;
        values.Add(PropertyName.@_fieldSingle, global::Kosmic.Variant.From<float>(___fieldSingle_default_value));
        double ___fieldDouble_default_value = 10;
        values.Add(PropertyName.@_fieldDouble, global::Kosmic.Variant.From<double>(___fieldDouble_default_value));
        string ___fieldString_default_value = "foo";
        values.Add(PropertyName.@_fieldString, global::Kosmic.Variant.From<string>(___fieldString_default_value));
        global::Kosmic.Vector2 ___fieldVector2_default_value = new(10f, 10f);
        values.Add(PropertyName.@_fieldVector2, global::Kosmic.Variant.From<global::Kosmic.Vector2>(___fieldVector2_default_value));
        global::Kosmic.Vector2I ___fieldVector2I_default_value = global::Kosmic.Vector2I.Up;
        values.Add(PropertyName.@_fieldVector2I, global::Kosmic.Variant.From<global::Kosmic.Vector2I>(___fieldVector2I_default_value));
        global::Kosmic.Rect2 ___fieldRect2_default_value = new(new global::Kosmic.Vector2(10f, 10f), new global::Kosmic.Vector2(10f, 10f));
        values.Add(PropertyName.@_fieldRect2, global::Kosmic.Variant.From<global::Kosmic.Rect2>(___fieldRect2_default_value));
        global::Kosmic.Rect2I ___fieldRect2I_default_value = new(new global::Kosmic.Vector2I(10, 10), new global::Kosmic.Vector2I(10, 10));
        values.Add(PropertyName.@_fieldRect2I, global::Kosmic.Variant.From<global::Kosmic.Rect2I>(___fieldRect2I_default_value));
        global::Kosmic.Transform2D ___fieldTransform2D_default_value = global::Kosmic.Transform2D.Identity;
        values.Add(PropertyName.@_fieldTransform2D, global::Kosmic.Variant.From<global::Kosmic.Transform2D>(___fieldTransform2D_default_value));
        global::Kosmic.Vector3 ___fieldVector3_default_value = new(10f, 10f, 10f);
        values.Add(PropertyName.@_fieldVector3, global::Kosmic.Variant.From<global::Kosmic.Vector3>(___fieldVector3_default_value));
        global::Kosmic.Vector3I ___fieldVector3I_default_value = global::Kosmic.Vector3I.Back;
        values.Add(PropertyName.@_fieldVector3I, global::Kosmic.Variant.From<global::Kosmic.Vector3I>(___fieldVector3I_default_value));
        global::Kosmic.Basis ___fieldBasis_default_value = new global::Kosmic.Basis(global::Kosmic.Quaternion.Identity);
        values.Add(PropertyName.@_fieldBasis, global::Kosmic.Variant.From<global::Kosmic.Basis>(___fieldBasis_default_value));
        global::Kosmic.Quaternion ___fieldQuaternion_default_value = new global::Kosmic.Quaternion(global::Kosmic.Basis.Identity);
        values.Add(PropertyName.@_fieldQuaternion, global::Kosmic.Variant.From<global::Kosmic.Quaternion>(___fieldQuaternion_default_value));
        global::Kosmic.Transform3D ___fieldTransform3D_default_value = global::Kosmic.Transform3D.Identity;
        values.Add(PropertyName.@_fieldTransform3D, global::Kosmic.Variant.From<global::Kosmic.Transform3D>(___fieldTransform3D_default_value));
        global::Kosmic.Vector4 ___fieldVector4_default_value = new(10f, 10f, 10f, 10f);
        values.Add(PropertyName.@_fieldVector4, global::Kosmic.Variant.From<global::Kosmic.Vector4>(___fieldVector4_default_value));
        global::Kosmic.Vector4I ___fieldVector4I_default_value = global::Kosmic.Vector4I.One;
        values.Add(PropertyName.@_fieldVector4I, global::Kosmic.Variant.From<global::Kosmic.Vector4I>(___fieldVector4I_default_value));
        global::Kosmic.Projection ___fieldProjection_default_value = global::Kosmic.Projection.Identity;
        values.Add(PropertyName.@_fieldProjection, global::Kosmic.Variant.From<global::Kosmic.Projection>(___fieldProjection_default_value));
        global::Kosmic.Aabb ___fieldAabb_default_value = new global::Kosmic.Aabb(10f, 10f, 10f, new global::Kosmic.Vector3(1f, 1f, 1f));
        values.Add(PropertyName.@_fieldAabb, global::Kosmic.Variant.From<global::Kosmic.Aabb>(___fieldAabb_default_value));
        global::Kosmic.Color ___fieldColor_default_value = global::Kosmic.Colors.Aquamarine;
        values.Add(PropertyName.@_fieldColor, global::Kosmic.Variant.From<global::Kosmic.Color>(___fieldColor_default_value));
        global::Kosmic.Plane ___fieldPlane_default_value = global::Kosmic.Plane.PlaneXZ;
        values.Add(PropertyName.@_fieldPlane, global::Kosmic.Variant.From<global::Kosmic.Plane>(___fieldPlane_default_value));
        global::Kosmic.Callable ___fieldCallable_default_value = new global::Kosmic.Callable(global::Kosmic.Engine.GetMainLoop(), "_process");
        values.Add(PropertyName.@_fieldCallable, global::Kosmic.Variant.From<global::Kosmic.Callable>(___fieldCallable_default_value));
        global::Kosmic.Signal ___fieldSignal_default_value = new global::Kosmic.Signal(global::Kosmic.Engine.GetMainLoop(), "property_list_changed");
        values.Add(PropertyName.@_fieldSignal, global::Kosmic.Variant.From<global::Kosmic.Signal>(___fieldSignal_default_value));
        global::ExportedFields.MyEnum ___fieldEnum_default_value = global::ExportedFields.MyEnum.C;
        values.Add(PropertyName.@_fieldEnum, global::Kosmic.Variant.From<global::ExportedFields.MyEnum>(___fieldEnum_default_value));
        global::ExportedFields.MyFlagsEnum ___fieldFlagsEnum_default_value = global::ExportedFields.MyFlagsEnum.C;
        values.Add(PropertyName.@_fieldFlagsEnum, global::Kosmic.Variant.From<global::ExportedFields.MyFlagsEnum>(___fieldFlagsEnum_default_value));
        byte[] ___fieldByteArray_default_value = { 0, 1, 2, 3, 4, 5, 6  };
        values.Add(PropertyName.@_fieldByteArray, global::Kosmic.Variant.From<byte[]>(___fieldByteArray_default_value));
        int[] ___fieldInt32Array_default_value = { 0, 1, 2, 3, 4, 5, 6  };
        values.Add(PropertyName.@_fieldInt32Array, global::Kosmic.Variant.From<int[]>(___fieldInt32Array_default_value));
        long[] ___fieldInt64Array_default_value = { 0, 1, 2, 3, 4, 5, 6  };
        values.Add(PropertyName.@_fieldInt64Array, global::Kosmic.Variant.From<long[]>(___fieldInt64Array_default_value));
        float[] ___fieldSingleArray_default_value = { 0f, 1f, 2f, 3f, 4f, 5f, 6f  };
        values.Add(PropertyName.@_fieldSingleArray, global::Kosmic.Variant.From<float[]>(___fieldSingleArray_default_value));
        double[] ___fieldDoubleArray_default_value = { 0d, 1d, 2d, 3d, 4d, 5d, 6d  };
        values.Add(PropertyName.@_fieldDoubleArray, global::Kosmic.Variant.From<double[]>(___fieldDoubleArray_default_value));
        string[] ___fieldStringArray_default_value = { "foo", "bar"  };
        values.Add(PropertyName.@_fieldStringArray, global::Kosmic.Variant.From<string[]>(___fieldStringArray_default_value));
        string[] ___fieldStringArrayEnum_default_value = { "foo", "bar"  };
        values.Add(PropertyName.@_fieldStringArrayEnum, global::Kosmic.Variant.From<string[]>(___fieldStringArrayEnum_default_value));
        global::Kosmic.Vector2[] ___fieldVector2Array_default_value = { global::Kosmic.Vector2.Up, global::Kosmic.Vector2.Down, global::Kosmic.Vector2.Left, global::Kosmic.Vector2.Right   };
        values.Add(PropertyName.@_fieldVector2Array, global::Kosmic.Variant.From<global::Kosmic.Vector2[]>(___fieldVector2Array_default_value));
        global::Kosmic.Vector3[] ___fieldVector3Array_default_value = { global::Kosmic.Vector3.Up, global::Kosmic.Vector3.Down, global::Kosmic.Vector3.Left, global::Kosmic.Vector3.Right   };
        values.Add(PropertyName.@_fieldVector3Array, global::Kosmic.Variant.From<global::Kosmic.Vector3[]>(___fieldVector3Array_default_value));
        global::Kosmic.Color[] ___fieldColorArray_default_value = { global::Kosmic.Colors.Aqua, global::Kosmic.Colors.Aquamarine, global::Kosmic.Colors.Azure, global::Kosmic.Colors.Beige   };
        values.Add(PropertyName.@_fieldColorArray, global::Kosmic.Variant.From<global::Kosmic.Color[]>(___fieldColorArray_default_value));
        global::Kosmic.KosmicObject[] ___fieldKosmicObjectOrDerivedArray_default_value = { null  };
        values.Add(PropertyName.@_fieldKosmicObjectOrDerivedArray, global::Kosmic.Variant.CreateFrom(___fieldKosmicObjectOrDerivedArray_default_value));
        global::Kosmic.StringName[] ___fieldStringNameArray_default_value = { "foo", "bar"  };
        values.Add(PropertyName.@_fieldStringNameArray, global::Kosmic.Variant.From<global::Kosmic.StringName[]>(___fieldStringNameArray_default_value));
        global::Kosmic.NodePath[] ___fieldNodePathArray_default_value = { "foo", "bar"  };
        values.Add(PropertyName.@_fieldNodePathArray, global::Kosmic.Variant.From<global::Kosmic.NodePath[]>(___fieldNodePathArray_default_value));
        global::Kosmic.Rid[] ___fieldRidArray_default_value = { default, default, default  };
        values.Add(PropertyName.@_fieldRidArray, global::Kosmic.Variant.From<global::Kosmic.Rid[]>(___fieldRidArray_default_value));
        int[] ___fieldEmptyInt32Array_default_value = global::System.Array.Empty<int>();
        values.Add(PropertyName.@_fieldEmptyInt32Array, global::Kosmic.Variant.From<int[]>(___fieldEmptyInt32Array_default_value));
        int[] ___fieldArrayFromList_default_value = new global::System.Collections.Generic.List<int>(global::System.Array.Empty<int>()).ToArray();
        values.Add(PropertyName.@_fieldArrayFromList, global::Kosmic.Variant.From<int[]>(___fieldArrayFromList_default_value));
        global::Kosmic.Variant ___fieldVariant_default_value = "foo";
        values.Add(PropertyName.@_fieldVariant, global::Kosmic.Variant.From<global::Kosmic.Variant>(___fieldVariant_default_value));
        global::Kosmic.KosmicObject ___fieldKosmicObjectOrDerived_default_value = default;
        values.Add(PropertyName.@_fieldKosmicObjectOrDerived, global::Kosmic.Variant.From<global::Kosmic.KosmicObject>(___fieldKosmicObjectOrDerived_default_value));
        global::Kosmic.Texture ___fieldKosmicResourceTexture_default_value = default;
        values.Add(PropertyName.@_fieldKosmicResourceTexture, global::Kosmic.Variant.From<global::Kosmic.Texture>(___fieldKosmicResourceTexture_default_value));
        global::Kosmic.StringName ___fieldStringName_default_value = new global::Kosmic.StringName("foo");
        values.Add(PropertyName.@_fieldStringName, global::Kosmic.Variant.From<global::Kosmic.StringName>(___fieldStringName_default_value));
        global::Kosmic.NodePath ___fieldNodePath_default_value = new global::Kosmic.NodePath("foo");
        values.Add(PropertyName.@_fieldNodePath, global::Kosmic.Variant.From<global::Kosmic.NodePath>(___fieldNodePath_default_value));
        global::Kosmic.Rid ___fieldRid_default_value = default;
        values.Add(PropertyName.@_fieldRid, global::Kosmic.Variant.From<global::Kosmic.Rid>(___fieldRid_default_value));
        global::Kosmic.Collections.Dictionary ___fieldKosmicDictionary_default_value = new()  { { "foo", 10  }, { global::Kosmic.Vector2.Up, global::Kosmic.Colors.Chocolate   }  };
        values.Add(PropertyName.@_fieldKosmicDictionary, global::Kosmic.Variant.From<global::Kosmic.Collections.Dictionary>(___fieldKosmicDictionary_default_value));
        global::Kosmic.Collections.Array ___fieldGodotArray_default_value = new()  { "foo", 10, global::Kosmic.Vector2.Up, global::Kosmic.Colors.Chocolate   };
        values.Add(PropertyName.@_fieldGodotArray, global::Kosmic.Variant.From<global::Kosmic.Collections.Array>(___fieldGodotArray_default_value));
        global::Kosmic.Collections.Dictionary<string, bool> ___fieldGodotGenericDictionary_default_value = new()  { { "foo", true  }, { "bar", false  }  };
        values.Add(PropertyName.@_fieldGodotGenericDictionary, global::Kosmic.Variant.CreateFrom(___fieldGodotGenericDictionary_default_value));
        global::Kosmic.Collections.Array<int> ___fieldGodotGenericArray_default_value = new()  { 0, 1, 2, 3, 4, 5, 6  };
        values.Add(PropertyName.@_fieldGodotGenericArray, global::Kosmic.Variant.CreateFrom(___fieldGodotGenericArray_default_value));
        long[] ___fieldEmptyInt64Array_default_value = global::System.Array.Empty<long>();
        values.Add(PropertyName.@_fieldEmptyInt64Array, global::Kosmic.Variant.From<long[]>(___fieldEmptyInt64Array_default_value));
        return values;
    }
#endif // TOOLS
#pragma warning restore CS0109
}
