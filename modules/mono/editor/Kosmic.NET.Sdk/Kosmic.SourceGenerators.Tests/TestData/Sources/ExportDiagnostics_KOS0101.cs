using Kosmic;

public partial class ExportDiagnostics_KS0101 : Node
{
    [Export]
    public static string {|KS0101:StaticField|};

    [Export]
    public static int {|KS0101:StaticProperty|} { get; set; }
}
