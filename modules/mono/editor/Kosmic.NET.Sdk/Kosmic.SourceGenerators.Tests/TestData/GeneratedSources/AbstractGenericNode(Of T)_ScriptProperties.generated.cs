using Kosmic;
using Kosmic.NativeInterop;

partial class AbstractGenericNode<T>
{
#pragma warning disable CS0109 // Disable warning about redundant 'new' keyword
    /// <summary>
    /// Cached StringNames for the properties and fields contained in this class, for fast lookup.
    /// </summary>
    public new class PropertyName : global::Kosmic.Node.PropertyName {
        /// <summary>
        /// Cached name for the 'MyArray' property.
        /// </summary>
        public new static readonly global::Kosmic.StringName @MyArray = "MyArray";
    }
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override bool SetKosmicClassPropertyValue(in kosmic_string_name name, in kosmic_variant value)
    {
        if (name == PropertyName.@MyArray) {
            this.@MyArray = global::Kosmic.NativeInterop.VariantUtils.ConvertToArray<T>(value);
            return true;
        }
        return base.SetKosmicClassPropertyValue(name, value);
    }
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override bool GetKosmicClassPropertyValue(in kosmic_string_name name, out kosmic_variant value)
    {
        if (name == PropertyName.@MyArray) {
            value = global::Kosmic.NativeInterop.VariantUtils.CreateFromArray(this.@MyArray);
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
        properties.Add(new(type: (global::Kosmic.Variant.Type)28, name: PropertyName.@MyArray, hint: (global::Kosmic.PropertyHint)0, hintString: "", usage: (global::Kosmic.PropertyUsageFlags)4102, exported: true));
        return properties;
    }
#pragma warning restore CS0109
}
