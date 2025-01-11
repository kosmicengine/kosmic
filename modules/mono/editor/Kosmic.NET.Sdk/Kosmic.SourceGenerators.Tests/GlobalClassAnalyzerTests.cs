using Xunit;

namespace Kosmic.SourceGenerators.Tests;

public class GlobalClassAnalyzerTests
{
    [Fact]
    public async void GlobalClassMustDeriveFromKosmicObjectTest()
    {
        const string GlobalClassKS0401 = "GlobalClass.KS0401.cs";
        await CSharpAnalyzerVerifier<GlobalClassAnalyzer>.Verify(GlobalClassKS0401);
    }

    [Fact]
    public async void GlobalClassMustNotBeGenericTest()
    {
        const string GlobalClassKS0402 = "GlobalClass.KS0402.cs";
        await CSharpAnalyzerVerifier<GlobalClassAnalyzer>.Verify(GlobalClassKS0402);
    }
}
