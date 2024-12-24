partial class ExportedProperties
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
        var values = new global::System.Collections.Generic.Dictionary<global::Kosmic.StringName, global::Kosmic.Variant>(64);
        string __NotGenerateComplexLamdaProperty_default_value = default;
        values.Add(PropertyName.@NotGenerateComplexLamdaProperty, global::Kosmic.Variant.From<string>(__NotGenerateComplexLamdaProperty_default_value));
        string __NotGenerateLamdaNoFieldProperty_default_value = default;
        values.Add(PropertyName.@NotGenerateLamdaNoFieldProperty, global::Kosmic.Variant.From<string>(__NotGenerateLamdaNoFieldProperty_default_value));
        string __NotGenerateComplexReturnProperty_default_value = default;
        values.Add(PropertyName.@NotGenerateComplexReturnProperty, global::Kosmic.Variant.From<string>(__NotGenerateComplexReturnProperty_default_value));
        string __NotGenerateReturnsProperty_default_value = default;
        values.Add(PropertyName.@NotGenerateReturnsProperty, global::Kosmic.Variant.From<string>(__NotGenerateReturnsProperty_default_value));
        string __FullPropertyString_default_value = "FullPropertyString";
        values.Add(PropertyName.@FullPropertyString, global::Kosmic.Variant.From<string>(__FullPropertyString_default_value));
        string __FullPropertyString_Complex_default_value = new string("FullPropertyString_Complex")   + global::System.Convert.ToInt32("1");
        values.Add(PropertyName.@FullPropertyString_Complex, global::Kosmic.Variant.From<string>(__FullPropertyString_Complex_default_value));
        string __LamdaPropertyString_default_value = "LamdaPropertyString";
        values.Add(PropertyName.@LamdaPropertyString, global::Kosmic.Variant.From<string>(__LamdaPropertyString_default_value));
        bool __PropertyBoolean_default_value = true;
        values.Add(PropertyName.@PropertyBoolean, global::Kosmic.Variant.From<bool>(__PropertyBoolean_default_value));
        char __PropertyChar_default_value = 'f';
        values.Add(PropertyName.@PropertyChar, global::Kosmic.Variant.From<char>(__PropertyChar_default_value));
        sbyte __PropertySByte_default_value = 10;
        values.Add(PropertyName.@PropertySByte, global::Kosmic.Variant.From<sbyte>(__PropertySByte_default_value));
        short __PropertyInt16_default_value = 10;
        values.Add(PropertyName.@PropertyInt16, global::Kosmic.Variant.From<short>(__PropertyInt16_default_value));
        int __PropertyInt32_default_value = 10;
        values.Add(PropertyName.@PropertyInt32, global::Kosmic.Variant.From<int>(__PropertyInt32_default_value));
        long __PropertyInt64_default_value = 10;
        values.Add(PropertyName.@PropertyInt64, global::Kosmic.Variant.From<long>(__PropertyInt64_default_value));
        byte __PropertyByte_default_value = 10;
        values.Add(PropertyName.@PropertyByte, global::Kosmic.Variant.From<byte>(__PropertyByte_default_value));
        ushort __PropertyUInt16_default_value = 10;
        values.Add(PropertyName.@PropertyUInt16, global::Kosmic.Variant.From<ushort>(__PropertyUInt16_default_value));
        uint __PropertyUInt32_default_value = 10;
        values.Add(PropertyName.@PropertyUInt32, global::Kosmic.Variant.From<uint>(__PropertyUInt32_default_value));
        ulong __PropertyUInt64_default_value = 10;
        values.Add(PropertyName.@PropertyUInt64, global::Kosmic.Variant.From<ulong>(__PropertyUInt64_default_value));
        float __PropertySingle_default_value = 10;
        values.Add(PropertyName.@PropertySingle, global::Kosmic.Variant.From<float>(__PropertySingle_default_value));
        double __PropertyDouble_default_value = 10;
        values.Add(PropertyName.@PropertyDouble, global::Kosmic.Variant.From<double>(__PropertyDouble_default_value));
        string __PropertyString_default_value = "foo";
        values.Add(PropertyName.@PropertyString, global::Kosmic.Variant.From<string>(__PropertyString_default_value));
        global::Kosmic.Vector2 __PropertyVector2_default_value = new(10f, 10f);
        values.Add(PropertyName.@PropertyVector2, global::Kosmic.Variant.From<global::Kosmic.Vector2>(__PropertyVector2_default_value));
        global::Kosmic.Vector2I __PropertyVector2I_default_value = global::Kosmic.Vector2I.Up;
        values.Add(PropertyName.@PropertyVector2I, global::Kosmic.Variant.From<global::Kosmic.Vector2I>(__PropertyVector2I_default_value));
        global::Kosmic.Rect2 __PropertyRect2_default_value = new(new global::Kosmic.Vector2(10f, 10f), new global::Kosmic.Vector2(10f, 10f));
        values.Add(PropertyName.@PropertyRect2, global::Kosmic.Variant.From<global::Kosmic.Rect2>(__PropertyRect2_default_value));
        global::Kosmic.Rect2I __PropertyRect2I_default_value = new(new global::Kosmic.Vector2I(10, 10), new global::Kosmic.Vector2I(10, 10));
        values.Add(PropertyName.@PropertyRect2I, global::Kosmic.Variant.From<global::Kosmic.Rect2I>(__PropertyRect2I_default_value));
        global::Kosmic.Transform2D __PropertyTransform2D_default_value = global::Kosmic.Transform2D.Identity;
        values.Add(PropertyName.@PropertyTransform2D, global::Kosmic.Variant.From<global::Kosmic.Transform2D>(__PropertyTransform2D_default_value));
        global::Kosmic.Vector3 __PropertyVector3_default_value = new(10f, 10f, 10f);
        values.Add(PropertyName.@PropertyVector3, global::Kosmic.Variant.From<global::Kosmic.Vector3>(__PropertyVector3_default_value));
        global::Kosmic.Vector3I __PropertyVector3I_default_value = global::Kosmic.Vector3I.Back;
        values.Add(PropertyName.@PropertyVector3I, global::Kosmic.Variant.From<global::Kosmic.Vector3I>(__PropertyVector3I_default_value));
        global::Kosmic.Basis __PropertyBasis_default_value = new global::Kosmic.Basis(global::Kosmic.Quaternion.Identity);
        values.Add(PropertyName.@PropertyBasis, global::Kosmic.Variant.From<global::Kosmic.Basis>(__PropertyBasis_default_value));
        global::Kosmic.Quaternion __PropertyQuaternion_default_value = new global::Kosmic.Quaternion(global::Kosmic.Basis.Identity);
        values.Add(PropertyName.@PropertyQuaternion, global::Kosmic.Variant.From<global::Kosmic.Quaternion>(__PropertyQuaternion_default_value));
        global::Kosmic.Transform3D __PropertyTransform3D_default_value = global::Kosmic.Transform3D.Identity;
        values.Add(PropertyName.@PropertyTransform3D, global::Kosmic.Variant.From<global::Kosmic.Transform3D>(__PropertyTransform3D_default_value));
        global::Kosmic.Vector4 __PropertyVector4_default_value = new(10f, 10f, 10f, 10f);
        values.Add(PropertyName.@PropertyVector4, global::Kosmic.Variant.From<global::Kosmic.Vector4>(__PropertyVector4_default_value));
        global::Kosmic.Vector4I __PropertyVector4I_default_value = global::Kosmic.Vector4I.One;
        values.Add(PropertyName.@PropertyVector4I, global::Kosmic.Variant.From<global::Kosmic.Vector4I>(__PropertyVector4I_default_value));
        global::Kosmic.Projection __PropertyProjection_default_value = global::Kosmic.Projection.Identity;
        values.Add(PropertyName.@PropertyProjection, global::Kosmic.Variant.From<global::Kosmic.Projection>(__PropertyProjection_default_value));
        global::Kosmic.Aabb __PropertyAabb_default_value = new global::Kosmic.Aabb(10f, 10f, 10f, new global::Kosmic.Vector3(1f, 1f, 1f));
        values.Add(PropertyName.@PropertyAabb, global::Kosmic.Variant.From<global::Kosmic.Aabb>(__PropertyAabb_default_value));
        global::Kosmic.Color __PropertyColor_default_value = global::Kosmic.Colors.Aquamarine;
        values.Add(PropertyName.@PropertyColor, global::Kosmic.Variant.From<global::Kosmic.Color>(__PropertyColor_default_value));
        global::Kosmic.Plane __PropertyPlane_default_value = global::Kosmic.Plane.PlaneXZ;
        values.Add(PropertyName.@PropertyPlane, global::Kosmic.Variant.From<global::Kosmic.Plane>(__PropertyPlane_default_value));
        global::Kosmic.Callable __PropertyCallable_default_value = new global::Kosmic.Callable(global::Kosmic.Engine.GetMainLoop(), "_process");
        values.Add(PropertyName.@PropertyCallable, global::Kosmic.Variant.From<global::Kosmic.Callable>(__PropertyCallable_default_value));
        global::Kosmic.Signal __PropertySignal_default_value = new global::Kosmic.Signal(global::Kosmic.Engine.GetMainLoop(), "Propertylist_changed");
        values.Add(PropertyName.@PropertySignal, global::Kosmic.Variant.From<global::Kosmic.Signal>(__PropertySignal_default_value));
        global::ExportedProperties.MyEnum __PropertyEnum_default_value = global::ExportedProperties.MyEnum.C;
        values.Add(PropertyName.@PropertyEnum, global::Kosmic.Variant.From<global::ExportedProperties.MyEnum>(__PropertyEnum_default_value));
        global::ExportedProperties.MyFlagsEnum __PropertyFlagsEnum_default_value = global::ExportedProperties.MyFlagsEnum.C;
        values.Add(PropertyName.@PropertyFlagsEnum, global::Kosmic.Variant.From<global::ExportedProperties.MyFlagsEnum>(__PropertyFlagsEnum_default_value));
        byte[] __PropertyByteArray_default_value = { 0, 1, 2, 3, 4, 5, 6  };
        values.Add(PropertyName.@PropertyByteArray, global::Kosmic.Variant.From<byte[]>(__PropertyByteArray_default_value));
        int[] __PropertyInt32Array_default_value = { 0, 1, 2, 3, 4, 5, 6  };
        values.Add(PropertyName.@PropertyInt32Array, global::Kosmic.Variant.From<int[]>(__PropertyInt32Array_default_value));
        long[] __PropertyInt64Array_default_value = { 0, 1, 2, 3, 4, 5, 6  };
        values.Add(PropertyName.@PropertyInt64Array, global::Kosmic.Variant.From<long[]>(__PropertyInt64Array_default_value));
        float[] __PropertySingleArray_default_value = { 0f, 1f, 2f, 3f, 4f, 5f, 6f  };
        values.Add(PropertyName.@PropertySingleArray, global::Kosmic.Variant.From<float[]>(__PropertySingleArray_default_value));
        double[] __PropertyDoubleArray_default_value = { 0d, 1d, 2d, 3d, 4d, 5d, 6d  };
        values.Add(PropertyName.@PropertyDoubleArray, global::Kosmic.Variant.From<double[]>(__PropertyDoubleArray_default_value));
        string[] __PropertyStringArray_default_value = { "foo", "bar"  };
        values.Add(PropertyName.@PropertyStringArray, global::Kosmic.Variant.From<string[]>(__PropertyStringArray_default_value));
        string[] __PropertyStringArrayEnum_default_value = { "foo", "bar"  };
        values.Add(PropertyName.@PropertyStringArrayEnum, global::Kosmic.Variant.From<string[]>(__PropertyStringArrayEnum_default_value));
        global::Kosmic.Vector2[] __PropertyVector2Array_default_value = { global::Kosmic.Vector2.Up, global::Kosmic.Vector2.Down, global::Kosmic.Vector2.Left, global::Kosmic.Vector2.Right   };
        values.Add(PropertyName.@PropertyVector2Array, global::Kosmic.Variant.From<global::Kosmic.Vector2[]>(__PropertyVector2Array_default_value));
        global::Kosmic.Vector3[] __PropertyVector3Array_default_value = { global::Kosmic.Vector3.Up, global::Kosmic.Vector3.Down, global::Kosmic.Vector3.Left, global::Kosmic.Vector3.Right   };
        values.Add(PropertyName.@PropertyVector3Array, global::Kosmic.Variant.From<global::Kosmic.Vector3[]>(__PropertyVector3Array_default_value));
        global::Kosmic.Color[] __PropertyColorArray_default_value = { global::Kosmic.Colors.Aqua, global::Kosmic.Colors.Aquamarine, global::Kosmic.Colors.Azure, global::Kosmic.Colors.Beige   };
        values.Add(PropertyName.@PropertyColorArray, global::Kosmic.Variant.From<global::Kosmic.Color[]>(__PropertyColorArray_default_value));
        global::Kosmic.KosmicObject[] __PropertyKosmicObjectOrDerivedArray_default_value = { null  };
        values.Add(PropertyName.@PropertyKosmicObjectOrDerivedArray, global::Kosmic.Variant.CreateFrom(__PropertyKosmicObjectOrDerivedArray_default_value));
        global::Kosmic.StringName[] __field_StringNameArray_default_value = { "foo", "bar"  };
        values.Add(PropertyName.@field_StringNameArray, global::Kosmic.Variant.From<global::Kosmic.StringName[]>(__field_StringNameArray_default_value));
        global::Kosmic.NodePath[] __field_NodePathArray_default_value = { "foo", "bar"  };
        values.Add(PropertyName.@field_NodePathArray, global::Kosmic.Variant.From<global::Kosmic.NodePath[]>(__field_NodePathArray_default_value));
        global::Kosmic.Rid[] __field_RidArray_default_value = { default, default, default  };
        values.Add(PropertyName.@field_RidArray, global::Kosmic.Variant.From<global::Kosmic.Rid[]>(__field_RidArray_default_value));
        global::Kosmic.Variant __PropertyVariant_default_value = "foo";
        values.Add(PropertyName.@PropertyVariant, global::Kosmic.Variant.From<global::Kosmic.Variant>(__PropertyVariant_default_value));
        global::Kosmic.KosmicObject __PropertyKosmicObjectOrDerived_default_value = default;
        values.Add(PropertyName.@PropertyKosmicObjectOrDerived, global::Kosmic.Variant.From<global::Kosmic.KosmicObject>(__PropertyKosmicObjectOrDerived_default_value));
        global::Kosmic.Texture __PropertyKosmicResourceTexture_default_value = default;
        values.Add(PropertyName.@PropertyKosmicResourceTexture, global::Kosmic.Variant.From<global::Kosmic.Texture>(__PropertyKosmicResourceTexture_default_value));
        global::Kosmic.StringName __PropertyStringName_default_value = new global::Kosmic.StringName("foo");
        values.Add(PropertyName.@PropertyStringName, global::Kosmic.Variant.From<global::Kosmic.StringName>(__PropertyStringName_default_value));
        global::Kosmic.NodePath __PropertyNodePath_default_value = new global::Kosmic.NodePath("foo");
        values.Add(PropertyName.@PropertyNodePath, global::Kosmic.Variant.From<global::Kosmic.NodePath>(__PropertyNodePath_default_value));
        global::Kosmic.Rid __PropertyRid_default_value = default;
        values.Add(PropertyName.@PropertyRid, global::Kosmic.Variant.From<global::Kosmic.Rid>(__PropertyRid_default_value));
        global::Kosmic.Collections.Dictionary __PropertyKosmicDictionary_default_value = new()  { { "foo", 10  }, { global::Kosmic.Vector2.Up, global::Kosmic.Colors.Chocolate   }  };
        values.Add(PropertyName.@PropertyKosmicDictionary, global::Kosmic.Variant.From<global::Kosmic.Collections.Dictionary>(__PropertyKosmicDictionary_default_value));
        global::Kosmic.Collections.Array __PropertyGodotArray_default_value = new()  { "foo", 10, global::Kosmic.Vector2.Up, global::Kosmic.Colors.Chocolate   };
        values.Add(PropertyName.@PropertyGodotArray, global::Kosmic.Variant.From<global::Kosmic.Collections.Array>(__PropertyGodotArray_default_value));
        global::Kosmic.Collections.Dictionary<string, bool> __PropertyGodotGenericDictionary_default_value = new()  { { "foo", true  }, { "bar", false  }  };
        values.Add(PropertyName.@PropertyGodotGenericDictionary, global::Kosmic.Variant.CreateFrom(__PropertyGodotGenericDictionary_default_value));
        global::Kosmic.Collections.Array<int> __PropertyGodotGenericArray_default_value = new()  { 0, 1, 2, 3, 4, 5, 6  };
        values.Add(PropertyName.@PropertyGodotGenericArray, global::Kosmic.Variant.CreateFrom(__PropertyGodotGenericArray_default_value));
        return values;
    }
#endif // TOOLS
#pragma warning restore CS0109
}
