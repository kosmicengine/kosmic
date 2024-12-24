using Godot;

// This works because it inherits from KosmicObject.
[GlobalClass]
public partial class CustomGlobalClass1 : KosmicObject
{

}

// This works because it inherits from an object that inherits from KosmicObject
[GlobalClass]
public partial class CustomGlobalClass2 : Node
{

}

// This raises a GD0401 diagnostic error: global classes must inherit from KosmicObject
[GlobalClass]
public partial class {|GD0401:CustomGlobalClass3|}
{

}
