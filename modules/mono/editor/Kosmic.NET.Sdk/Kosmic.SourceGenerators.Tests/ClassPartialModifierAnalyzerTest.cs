using System.Threading.Tasks;
using Xunit;

namespace Kosmic.SourceGenerators.Tests;

public class ClassPartialModifierTest
{
    [Fact]
    public async Task ClassPartialModifierCodeFixTest()
    {
        await CSharpCodeFixVerifier<ClassPartialModifierCodeFixProvider, ClassPartialModifierAnalyzer>
            .Verify("ClassPartialModifier.KS0001.cs", "ClassPartialModifier.KS0001.fixed.cs");
    }

    [Fact]
    public async void OuterClassPartialModifierAnalyzerTest()
    {
        await CSharpAnalyzerVerifier<ClassPartialModifierAnalyzer>.Verify("OuterClassPartialModifierAnalyzer.KS0002.cs");
    }
}
