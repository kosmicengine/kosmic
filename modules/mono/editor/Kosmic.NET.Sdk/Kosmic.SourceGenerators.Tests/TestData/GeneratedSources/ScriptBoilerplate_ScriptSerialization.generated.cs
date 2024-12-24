using Godot;
using Kosmic.NativeInterop;

partial class ScriptBoilerplate
{
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override void SaveKosmicObjectData(global::Kosmic.Bridge.KosmicSerializationInfo info)
    {
        base.SaveKosmicObjectData(info);
        info.AddProperty(PropertyName.@_nodePath, global::Kosmic.Variant.From<global::Kosmic.NodePath>(this.@_nodePath));
        info.AddProperty(PropertyName.@_velocity, global::Kosmic.Variant.From<int>(this.@_velocity));
    }
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override void RestoreKosmicObjectData(global::Kosmic.Bridge.KosmicSerializationInfo info)
    {
        base.RestoreKosmicObjectData(info);
        if (info.TryGetProperty(PropertyName.@_nodePath, out var _value__nodePath))
            this.@_nodePath = _value__nodePath.As<global::Kosmic.NodePath>();
        if (info.TryGetProperty(PropertyName.@_velocity, out var _value__velocity))
            this.@_velocity = _value__velocity.As<int>();
    }
}
