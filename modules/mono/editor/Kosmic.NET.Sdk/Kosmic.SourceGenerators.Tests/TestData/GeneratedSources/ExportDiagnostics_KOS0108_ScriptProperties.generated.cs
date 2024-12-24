using Godot;
using Kosmic.NativeInterop;

partial class ExportDiagnostics_GD0108
{
#pragma warning disable CS0109 // Disable warning about redundant 'new' keyword
    /// <summary>
    /// Cached StringNames for the properties and fields contained in this class, for fast lookup.
    /// </summary>
    public new class PropertyName : global::Kosmic.Node.PropertyName {
        /// <summary>
        /// Cached name for the 'MyButton' field.
        /// </summary>
        public new static readonly global::Kosmic.StringName @MyButton = "MyButton";
    }
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override bool SetKosmicClassPropertyValue(in kosmic_string_name name, in kosmic_variant value)
    {
        if (name == PropertyName.@MyButton) {
            this.@MyButton = global::Kosmic.NativeInterop.VariantUtils.ConvertTo<global::Kosmic.Callable>(value);
            return true;
        }
        return base.SetKosmicClassPropertyValue(name, value);
    }
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override bool GetKosmicClassPropertyValue(in kosmic_string_name name, out kosmic_variant value)
    {
        if (name == PropertyName.@MyButton) {
            value = global::Kosmic.NativeInterop.VariantUtils.CreateFrom<global::Kosmic.Callable>(this.@MyButton);
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
        return properties;
    }
#pragma warning restore CS0109
}
