using Xunit;

namespace Kosmic.SourceGenerators.Tests;

public class ExportDiagnosticsTests
{
    [Fact]
    public async void StaticMembers()
    {
        await CSharpSourceGeneratorVerifier<ScriptPropertyDefValGenerator>.Verify(
            "ExportDiagnostics_KS0101.cs",
            "ExportDiagnostics_KS0101_ScriptPropertyDefVal.generated.cs"
        );
    }

    [Fact]
    public async void TypeIsNotSupported()
    {
        await CSharpSourceGeneratorVerifier<ScriptPropertyDefValGenerator>.Verify(
            "ExportDiagnostics_KS0102.cs",
            "ExportDiagnostics_KS0102_ScriptPropertyDefVal.generated.cs"
        );
    }

    [Fact]
    public async void ReadOnly()
    {
        await CSharpSourceGeneratorVerifier<ScriptPropertyDefValGenerator>.Verify(
            "ExportDiagnostics_KS0103.cs",
            "ExportDiagnostics_KS0103_ScriptPropertyDefVal.generated.cs"
        );
    }

    [Fact]
    public async void WriteOnly()
    {
        await CSharpSourceGeneratorVerifier<ScriptPropertyDefValGenerator>.Verify(
            "ExportDiagnostics_KS0104.cs",
            "ExportDiagnostics_KS0104_ScriptPropertyDefVal.generated.cs"
        );
    }

    [Fact]
    public async void Indexer()
    {
        await CSharpSourceGeneratorVerifier<ScriptPropertyDefValGenerator>.Verify(
            "ExportDiagnostics_KS0105.cs",
            "ExportDiagnostics_KS0105_ScriptPropertyDefVal.generated.cs"
        );
    }

    [Fact]
    public async void ExplicitInterfaceImplementation()
    {
        await CSharpSourceGeneratorVerifier<ScriptPropertyDefValGenerator>.Verify(
            new string[] { "ExportDiagnostics_KS0106.cs" },
            new string[]
            {
                "ExportDiagnostics_KS0106_OK_ScriptPropertyDefVal.generated.cs",
                "ExportDiagnostics_KS0106_KO_ScriptPropertyDefVal.generated.cs",
            }
        );
    }

    [Fact]
    public async void NodeExports()
    {
        await CSharpSourceGeneratorVerifier<ScriptPropertyDefValGenerator>.Verify(
            new string[] { "ExportDiagnostics_KS0107.cs" },
            new string[]
            {
                "ExportDiagnostics_KS0107_OK_ScriptPropertyDefVal.generated.cs",
                "ExportDiagnostics_KS0107_KO_ScriptPropertyDefVal.generated.cs",
            }
        );
    }

    [Fact]
    public async void ExportToolButtonInNonToolClass()
    {
        await CSharpSourceGeneratorVerifier<ScriptPropertiesGenerator>.Verify(
            new string[] { "ExportDiagnostics_KS0108.cs" },
            new string[] { "ExportDiagnostics_KS0108_ScriptProperties.generated.cs" }
        );
    }

    [Fact]
    public async void ExportAndExportToolButtonOnSameMember()
    {
        await CSharpSourceGeneratorVerifier<ScriptPropertiesGenerator>.Verify(
            new string[] { "ExportDiagnostics_KS0109.cs" },
            new string[] { "ExportDiagnostics_KS0109_ScriptProperties.generated.cs" }
        );
    }

    [Fact]
    public async void ExportToolButtonOnNonCallable()
    {
        await CSharpSourceGeneratorVerifier<ScriptPropertiesGenerator>.Verify(
            new string[] { "ExportDiagnostics_KS0110.cs" },
            new string[] { "ExportDiagnostics_KS0110_ScriptProperties.generated.cs" }
        );
    }
}
