using Kosmic;

public class {|KS0002:OuterOuterClassPartialModifierAnalyzer|}
{
    public class {|KS0002:OuterClassPartialModifierAnalyzer|}
    {
        // MyNode is contained in a non-partial type so the source generators
        // can't enhance this type to work with Kosmic.
        public partial class MyNode : Node { }
    }
}
