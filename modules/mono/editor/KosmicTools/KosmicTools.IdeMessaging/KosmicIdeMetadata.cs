using System.Diagnostics.CodeAnalysis;

namespace KosmicTools.IdeMessaging
{
    public readonly struct KosmicIdeMetadata
    {
        public int Port { get; }
        public string EditorExecutablePath { get; }

        public const string DefaultFileName = "ide_messaging_meta.txt";

        public KosmicIdeMetadata(int port, string editorExecutablePath)
        {
            Port = port;
            EditorExecutablePath = editorExecutablePath;
        }

        public static bool operator ==(KosmicIdeMetadata a, KosmicIdeMetadata b)
        {
            return a.Port == b.Port && a.EditorExecutablePath == b.EditorExecutablePath;
        }

        public static bool operator !=(KosmicIdeMetadata a, KosmicIdeMetadata b)
        {
            return !(a == b);
        }

        public override bool Equals([NotNullWhen(true)] object? obj)
        {
            return obj is KosmicIdeMetadata metadata && metadata == this;
        }

        public bool Equals(KosmicIdeMetadata other)
        {
            return Port == other.Port && EditorExecutablePath == other.EditorExecutablePath;
        }

        public override int GetHashCode()
        {
            unchecked
            {
                return (Port * 397) ^ (EditorExecutablePath != null ? EditorExecutablePath.GetHashCode() : 0);
            }
        }
    }
}
