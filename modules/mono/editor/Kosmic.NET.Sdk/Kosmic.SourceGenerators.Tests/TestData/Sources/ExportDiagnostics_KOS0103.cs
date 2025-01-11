using Kosmic;

public partial class ExportDiagnostics_KS0103 : Node
{
    [Export]
    public readonly string {|KS0103:ReadOnlyField|};

    [Export]
    public string {|KS0103:ReadOnlyProperty|} { get; }
}
