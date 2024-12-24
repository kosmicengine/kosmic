using Godot;
using Kosmic.NativeInterop;

partial struct OuterClass
{
partial class NestedClass
{
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override void SaveKosmicObjectData(global::Kosmic.Bridge.KosmicSerializationInfo info)
    {
        base.SaveKosmicObjectData(info);
    }
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override void RestoreKosmicObjectData(global::Kosmic.Bridge.KosmicSerializationInfo info)
    {
        base.RestoreKosmicObjectData(info);
    }
}
}
