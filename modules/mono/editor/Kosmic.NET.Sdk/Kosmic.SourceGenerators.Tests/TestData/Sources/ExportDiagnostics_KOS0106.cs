using Kosmic;

public interface MyInterface
{
    public int MyProperty { get; set; }
}

public partial class ExportDiagnostics_KS0106_OK : Node, MyInterface
{
    [Export]
    public int MyProperty { get; set; }
}

public partial class ExportDiagnostics_KS0106_KO : Node, MyInterface
{
    [Export]
    int MyInterface.{|KS0106:MyProperty|} { get; set; }
}
