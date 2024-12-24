using System;

namespace Kosmic.SourceGenerators.Sample
{
    public partial class AllWriteOnly : KosmicObject
    {
        private bool _writeOnlyBackingField = false;
        public bool WriteOnlyProperty { set => _writeOnlyBackingField = value; }
    }
}
