using Kosmic;

public partial class ExportDiagnostics_KS0102 : Node
{
    public struct MyStruct { }

    [Export]
    public MyStruct {|KS0102:StructField|};

    [Export]
    public MyStruct {|KS0102:StructProperty|} { get; set; }
}
