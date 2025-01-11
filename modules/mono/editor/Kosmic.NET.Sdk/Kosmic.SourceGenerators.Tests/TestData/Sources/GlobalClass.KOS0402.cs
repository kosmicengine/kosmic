using Kosmic;

// This works because it inherits from KosmicObject and it doesn't have any generic type parameter.
[GlobalClass]
public partial class CustomGlobalClass : KosmicObject
{

}

// This raises a KS0402 diagnostic error: global classes can't have any generic type parameter
[GlobalClass]
public partial class {|KS0402:CustomGlobalClass|}<T> : KosmicObject
{

}
