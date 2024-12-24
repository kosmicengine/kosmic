using System;

namespace Kosmic.SourceGenerators.Sample;

public partial class NestedClass : KosmicObject
{
    public partial class NestedClass2 : KosmicObject
    {
        public partial class NestedClass3 : KosmicObject
        {
            [Signal]
            public delegate void MySignalEventHandler(string str, int num);

            [Export] private String _fieldString = "foo";
            [Export] private String PropertyString { get; set; } = "foo";

            private void Method()
            {
            }
        }
    }
}
