// meta-description: Basic import script template (no comments)

#if TOOLS
using _BINDINGS_NAMESPACE_;
using System;

[Tool]
public partial class _CLASS_ : _BASE_
{
    public override KosmicObject _PostImport(Node scene)
    {
        return scene;
    }
}
#endif
