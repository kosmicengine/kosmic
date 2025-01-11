using Kosmic;

public partial class Generic<T> : KosmicObject
{
    private int _field;
}

// Generic again but different generic parameters
public partial class {|KS0003:Generic|}<T, R> : KosmicObject
{
    private int _field;
}

// Generic again but without generic parameters
public partial class {|KS0003:Generic|} : KosmicObject
{
    private int _field;
}
