using Kosmic;
using Kosmic.Collections;

public partial class ExportDiagnostics_KS0107_OK : Node
{
    [Export]
    public Node NodeField;

    [Export]
    public Node[] SystemArrayOfNodesField;

    [Export]
    public Array<Node> KosmicArrayOfNodesField;

    [Export]
    public Dictionary<Node, string> KosmicDictionaryWithNodeAsKeyField;

    [Export]
    public Dictionary<string, Node> KosmicDictionaryWithNodeAsValueField;

    [Export]
    public Node NodeProperty { get; set; }

    [Export]
    public Node[] SystemArrayOfNodesProperty { get; set; }

    [Export]
    public Array<Node> KosmicArrayOfNodesProperty { get; set; }

    [Export]
    public Dictionary<Node, string> KosmicDictionaryWithNodeAsKeyProperty { get; set; }

    [Export]
    public Dictionary<string, Node> KosmicDictionaryWithNodeAsValueProperty { get; set; }
}

public partial class ExportDiagnostics_KS0107_KO : Resource
{
    [Export]
    public Node {|KS0107:NodeField|};

    [Export]
    public Node[] {|KS0107:SystemArrayOfNodesField|};

    [Export]
    public Array<Node> {|KS0107:KosmicArrayOfNodesField|};

    [Export]
    public Dictionary<Node, string> {|KS0107:KosmicDictionaryWithNodeAsKeyField|};

    [Export]
    public Dictionary<string, Node> {|KS0107:KosmicDictionaryWithNodeAsValueField|};

    [Export]
    public Node {|KS0107:NodeProperty|} { get; set; }

    [Export]
    public Node[] {|KS0107:SystemArrayOfNodesProperty|} { get; set; }

    [Export]
    public Array<Node> {|KS0107:KosmicArrayOfNodesProperty|} { get; set; }

    [Export]
    public Dictionary<Node, string> {|KS0107:KosmicDictionaryWithNodeAsKeyProperty|} { get; set; }

    [Export]
    public Dictionary<string, Node> {|KS0107:KosmicDictionaryWithNodeAsValueProperty|} { get; set; }
}
