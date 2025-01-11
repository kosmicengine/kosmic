using Kosmic;
using Kosmic.NativeInterop;

partial class ExportedToolButtons
{
#pragma warning disable CS0109 // Disable warning about redundant 'new' keyword
    /// <summary>
    /// Cached StringNames for the properties and fields contained in this class, for fast lookup.
    /// </summary>
    public new class PropertyName : global::Kosmic.KosmicObject.PropertyName {
        /// <summary>
        /// Cached name for the 'MyButton1' property.
        /// </summary>
        public new static readonly global::Kosmic.StringName @MyButton1 = "MyButton1";
        /// <summary>
        /// Cached name for the 'MyButton2' property.
        /// </summary>
        public new static readonly global::Kosmic.StringName @MyButton2 = "MyButton2";
    }
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override bool GetKosmicClassPropertyValue(in kosmic_string_name name, out kosmic_variant value)
    {
        if (name == PropertyName.@MyButton1) {
            value = global::Kosmic.NativeInterop.VariantUtils.CreateFrom<global::Kosmic.Callable>(this.@MyButton1);
            return true;
        }
        if (name == PropertyName.@MyButton2) {
            value = global::Kosmic.NativeInterop.VariantUtils.CreateFrom<global::Kosmic.Callable>(this.@MyButton2);
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
        properties.Add(new(type: (global::Kosmic.Variant.Type)25, name: PropertyName.@MyButton1, hint: (global::Kosmic.PropertyHint)39, hintString: "Click me!", usage: (global::Kosmic.PropertyUsageFlags)4, exported: true));
        properties.Add(new(type: (global::Kosmic.Variant.Type)25, name: PropertyName.@MyButton2, hint: (global::Kosmic.PropertyHint)39, hintString: "Click me!,ColorRect", usage: (global::Kosmic.PropertyUsageFlags)4, exported: true));
        return properties;
    }
#pragma warning restore CS0109
}
