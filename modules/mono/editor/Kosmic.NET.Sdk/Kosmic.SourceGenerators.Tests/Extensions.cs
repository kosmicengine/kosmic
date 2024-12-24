using System.Reflection;
using Microsoft.CodeAnalysis;

namespace Kosmic.SourceGenerators.Tests;

public static class Extensions
{
    public static MetadataReference CreateMetadataReference(this Assembly assembly)
    {
        return MetadataReference.CreateFromFile(assembly.Location);
    }
}
