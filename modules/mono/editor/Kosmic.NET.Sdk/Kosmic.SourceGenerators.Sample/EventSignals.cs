namespace Kosmic.SourceGenerators.Sample;

public partial class EventSignals : KosmicObject
{
    [Signal]
    public delegate void MySignalEventHandler(string str, int num);
}
