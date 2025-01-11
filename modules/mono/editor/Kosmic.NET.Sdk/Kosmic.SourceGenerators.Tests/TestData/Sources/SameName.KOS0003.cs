using Kosmic;

namespace NamespaceA
{
    partial class SameName : KosmicObject
    {
        private int _field;
    }
}

// SameName again but different namespace
namespace NamespaceB
{
    partial class {|KS0003:SameName|} : KosmicObject
    {
        private int _field;
    }
}
