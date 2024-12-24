namespace Kosmic.SourceGenerators.Sample
{
    public partial class AllReadOnly : KosmicObject
    {
        public readonly string ReadonlyField = "foo";
        public string ReadonlyAutoProperty { get; } = "foo";
        public string ReadonlyProperty { get => "foo"; }
        public string InitonlyAutoProperty { get; init; }
    }
}
