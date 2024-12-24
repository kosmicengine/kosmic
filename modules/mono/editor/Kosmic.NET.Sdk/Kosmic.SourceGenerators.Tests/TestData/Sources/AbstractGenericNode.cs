using Godot;

public abstract partial class AbstractGenericNode<[MustBeVariant] T> : Node
{
    [Export] // This should be included, but without type hints.
    public Kosmic.Collections.Array<T> MyArray { get; set; } = new();
}
