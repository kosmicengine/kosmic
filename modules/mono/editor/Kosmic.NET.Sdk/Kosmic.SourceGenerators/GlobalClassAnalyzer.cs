using System.Collections.Immutable;
using System.Linq;
using Microsoft.CodeAnalysis;
using Microsoft.CodeAnalysis.CSharp;
using Microsoft.CodeAnalysis.Diagnostics;

namespace Kosmic.SourceGenerators
{
    [DiagnosticAnalyzer(LanguageNames.CSharp)]
    public sealed class GlobalClassAnalyzer : DiagnosticAnalyzer
    {
        public override ImmutableArray<DiagnosticDescriptor> SupportedDiagnostics
            => ImmutableArray.Create(
                Common.GlobalClassMustDeriveFromKosmicObjectRule,
                Common.GlobalClassMustNotBeGenericRule);

        public override void Initialize(AnalysisContext context)
        {
            context.ConfigureGeneratedCodeAnalysis(GeneratedCodeAnalysisFlags.None);
            context.EnableConcurrentExecution();
            context.RegisterSyntaxNodeAction(AnalyzeNode, SyntaxKind.ClassDeclaration);
        }

        private static void AnalyzeNode(SyntaxNodeAnalysisContext context)
        {
            // Return if not a type symbol or the type is not a global class.
            if (context.ContainingSymbol is not INamedTypeSymbol typeSymbol ||
                !typeSymbol.GetAttributes().Any(a => a.AttributeClass?.IsKosmicGlobalClassAttribute() ?? false))
                return;

            if (typeSymbol.IsGenericType)
            {
                context.ReportDiagnostic(Diagnostic.Create(
                    Common.GlobalClassMustNotBeGenericRule,
                    typeSymbol.Locations.FirstLocationWithSourceTreeOrDefault(),
                    typeSymbol.ToDisplayString()
                ));
            }

            if (!typeSymbol.InheritsFrom("KosmicSharp", KosmicClasses.KosmicObject))
            {
                context.ReportDiagnostic(Diagnostic.Create(
                    Common.GlobalClassMustDeriveFromKosmicObjectRule,
                    typeSymbol.Locations.FirstLocationWithSourceTreeOrDefault(),
                    typeSymbol.ToDisplayString()
                ));
            }
        }
    }
}
