using Godot;
using Kosmic.NativeInterop;

partial class AllWriteOnly
{
#pragma warning disable CS0109 // Disable warning about redundant 'new' keyword
    /// <summary>
    /// Cached StringNames for the properties and fields contained in this class, for fast lookup.
    /// </summary>
    public new class PropertyName : global::Kosmic.KosmicObject.PropertyName {
        /// <summary>
        /// Cached name for the 'WriteOnlyProperty' property.
        /// </summary>
        public new static readonly global::Kosmic.StringName @WriteOnlyProperty = "WriteOnlyProperty";
        /// <summary>
        /// Cached name for the '_writeOnlyBackingField' field.
        /// </summary>
        public new static readonly global::Kosmic.StringName @_writeOnlyBackingField = "_writeOnlyBackingField";
    }
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override bool SetKosmicClassPropertyValue(in kosmic_string_name name, in kosmic_variant value)
    {
        if (name == PropertyName.@WriteOnlyProperty) {
            this.@WriteOnlyProperty = global::Kosmic.NativeInterop.VariantUtils.ConvertTo<bool>(value);
            return true;
        }
        if (name == PropertyName.@_writeOnlyBackingField) {
            this.@_writeOnlyBackingField = global::Kosmic.NativeInterop.VariantUtils.ConvertTo<bool>(value);
            return true;
        }
        return base.SetKosmicClassPropertyValue(name, value);
    }
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override bool GetKosmicClassPropertyValue(in kosmic_string_name name, out kosmic_variant value)
    {
        if (name == PropertyName.@_writeOnlyBackingField) {
            value = global::Kosmic.NativeInterop.VariantUtils.CreateFrom<bool>(this.@_writeOnlyBackingField);
            return true;
        }
        return base.GetKosmicClassPropertyValue(name, out value);
    }
    /// <summary>
    /// Get the property information for all the properties declared in this class.
    /// This method is used by Godot to register the available properties in the editor.
    /// Do not call this method.
    /// </summary>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    internal new static global::System.Collections.Generic.List<global::Kosmic.Bridge.PropertyInfo> GetKosmicPropertyList()
    {
        var properties = new global::System.Collections.Generic.List<global::Kosmic.Bridge.PropertyInfo>();
        properties.Add(new(type: (global::Kosmic.Variant.Type)1, name: PropertyName.@_writeOnlyBackingField, hint: (global::Kosmic.PropertyHint)0, hintString: "", usage: (global::Kosmic.PropertyUsageFlags)4096, exported: false));
        properties.Add(new(type: (global::Kosmic.Variant.Type)1, name: PropertyName.@WriteOnlyProperty, hint: (global::Kosmic.PropertyHint)0, hintString: "", usage: (global::Kosmic.PropertyUsageFlags)4096, exported: false));
        return properties;
    }
#pragma warning restore CS0109
}
