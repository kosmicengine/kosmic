using Godot;
using System;
using GodotTools.ProjectEditor;

namespace KosmicTools
{
    public static class CsProjOperations
    {
        public static string GenerateGameProject(string dir, string name)
        {
            try
            {
                return ProjectGenerator.GenAndSaveGameProject(dir, name);
            }
            catch (Exception e)
            {
                KS.PushError(e.ToString());
                return string.Empty;
            }
        }
    }
}
