using Godot;
using Kosmic.Collections;

public partial class ExportDiagnostics_GD0107_OK : Node
{
    [Export]
    public Node NodeField;

    [Export]
    public Node[] SystemArrayOfNodesField;

    [Export]
    public Array<Node> GodotArrayOfNodesField;

    [Export]
    public Dictionary<Node, string> KosmicDictionaryWithNodeAsKeyField;

    [Export]
    public Dictionary<string, Node> KosmicDictionaryWithNodeAsValueField;

    [Export]
    public Node NodeProperty { get; set; }

    [Export]
    public Node[] SystemArrayOfNodesProperty { get; set; }

    [Export]
    public Array<Node> GodotArrayOfNodesProperty { get; set; }

    [Export]
    public Dictionary<Node, string> KosmicDictionaryWithNodeAsKeyProperty { get; set; }

    [Export]
    public Dictionary<string, Node> KosmicDictionaryWithNodeAsValueProperty { get; set; }
}

public partial class ExportDiagnostics_GD0107_KO : Resource
{
    [Export]
    public Node {|GD0107:NodeField|};

    [Export]
    public Node[] {|GD0107:SystemArrayOfNodesField|};

    [Export]
    public Array<Node> {|GD0107:GodotArrayOfNodesField|};

    [Export]
    public Dictionary<Node, string> {|GD0107:KosmicDictionaryWithNodeAsKeyField|};

    [Export]
    public Dictionary<string, Node> {|GD0107:KosmicDictionaryWithNodeAsValueField|};

    [Export]
    public Node {|GD0107:NodeProperty|} { get; set; }

    [Export]
    public Node[] {|GD0107:SystemArrayOfNodesProperty|} { get; set; }

    [Export]
    public Array<Node> {|GD0107:GodotArrayOfNodesProperty|} { get; set; }

    [Export]
    public Dictionary<Node, string> {|GD0107:KosmicDictionaryWithNodeAsKeyProperty|} { get; set; }

    [Export]
    public Dictionary<string, Node> {|GD0107:KosmicDictionaryWithNodeAsValueProperty|} { get; set; }
}
