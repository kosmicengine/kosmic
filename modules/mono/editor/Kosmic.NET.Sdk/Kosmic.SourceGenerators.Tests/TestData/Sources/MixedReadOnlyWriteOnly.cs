using Kosmic;

public partial class MixedReadOnlyWriteOnly : KosmicObject
{
    public readonly string ReadOnlyField = "foo";
    public string ReadOnlyAutoProperty { get; } = "foo";
    public string ReadOnlyProperty { get => "foo"; }
    public string InitOnlyAutoProperty { get; init; }

    bool _writeOnlyBackingField = false;
    public bool WriteOnlyProperty { set => _writeOnlyBackingField = value; }
}
