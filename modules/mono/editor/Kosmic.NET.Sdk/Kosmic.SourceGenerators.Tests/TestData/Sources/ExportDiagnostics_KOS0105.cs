using System;
using Kosmic;

public partial class ExportDiagnostics_KS0105 : Node
{
    [Export]
    public int {|KS0105:this|}[int index]
    {
        get { return index; }
        set { }
    }
}
