using Kosmic;

public partial class EventSignals : KosmicObject
{
    [Signal]
    public delegate void MySignalEventHandler(string str, int num);

    private struct MyStruct { }

    [Signal]
    private delegate void {|KS0201:MyInvalidSignal|}();

    [Signal]
    private delegate void MyInvalidParameterTypeSignalEventHandler(MyStruct {|KS0202:myStruct|});

    [Signal]
    private delegate MyStruct {|KS0203:MyInvalidReturnTypeSignalEventHandler|}();
}
