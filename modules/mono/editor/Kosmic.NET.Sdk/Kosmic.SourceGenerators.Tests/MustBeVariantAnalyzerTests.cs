using Xunit;

namespace Kosmic.SourceGenerators.Tests;

public class MustBeVariantAnalyzerTests
{
    [Fact]
    public async void GenericTypeArgumentMustBeVariantTest()
    {
        const string MustBeVariantKS0301 = "MustBeVariant.KS0301.cs";
        await CSharpAnalyzerVerifier<MustBeVariantAnalyzer>.Verify(MustBeVariantKS0301);
    }

    [Fact]
    public async void GenericTypeParameterMustBeVariantAnnotatedTest()
    {
        const string MustBeVariantKS0302 = "MustBeVariant.KS0302.cs";
        await CSharpAnalyzerVerifier<MustBeVariantAnalyzer>.Verify(MustBeVariantKS0302);
    }
}
