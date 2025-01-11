#pragma warning disable IDE1006 // Naming rule violation
// ReSharper disable InconsistentNaming

using System;
using System.Diagnostics.CodeAnalysis;
using System.Runtime.CompilerServices;
using Kosmic;
using Kosmic.NativeInterop;
using Kosmic.SourceGenerators.Internal;
using KosmicTools.IdeMessaging.Requests;

namespace KosmicTools.Internals
{
    [GenerateUnmanagedCallbacks(typeof(InternalUnmanagedCallbacks))]
    internal static partial class Internal
    {
        public const string CSharpLanguageType = "CSharpScript";
        public const string CSharpLanguageExtension = ".cs";

        public static string FullExportTemplatesDir
        {
            get
            {
                kosmic_icall_Internal_FullExportTemplatesDir(out kosmic_string dest);
                using (dest)
                    return Marshaling.ConvertStringToManaged(dest);
            }
        }

        public static string SimplifyKosmicPath(this string path) => Kosmic.StringExtensions.SimplifyPath(path);

        public static bool IsMacOSAppBundleInstalled(string bundleId)
        {
            using kosmic_string bundleIdIn = Marshaling.ConvertStringToNative(bundleId);
            return kosmic_icall_Internal_IsMacOSAppBundleInstalled(bundleIdIn);
        }

        public static bool LipOCreateFile(string outputPath, string[] files)
        {
            using kosmic_string outputPathIn = Marshaling.ConvertStringToNative(outputPath);
            using kosmic_packed_string_array filesIn = Marshaling.ConvertSystemArrayToNativePackedStringArray(files);
            return kosmic_icall_Internal_LipOCreateFile(outputPathIn, filesIn);
        }

        public static bool KosmicIs32Bits() => kosmic_icall_Internal_KosmicIs32Bits();

        public static bool KosmicIsRealTDouble() => kosmic_icall_Internal_KosmicIsRealTDouble();

        public static void KosmicMainIteration() => kosmic_icall_Internal_KosmicMainIteration();

        public static bool IsAssembliesReloadingNeeded() => kosmic_icall_Internal_IsAssembliesReloadingNeeded();

        public static void ReloadAssemblies(bool softReload) => kosmic_icall_Internal_ReloadAssemblies(softReload);

        public static void EditorDebuggerNodeReloadScripts() => kosmic_icall_Internal_EditorDebuggerNodeReloadScripts();

        public static bool ScriptEditorEdit(Resource resource, int line, int col, bool grabFocus = true) =>
            kosmic_icall_Internal_ScriptEditorEdit(resource.NativeInstance, line, col, grabFocus);

        public static void EditorNodeShowScriptScreen() => kosmic_icall_Internal_EditorNodeShowScriptScreen();

        public static void EditorRunPlay() => kosmic_icall_Internal_EditorRunPlay();

        public static void EditorRunStop() => kosmic_icall_Internal_EditorRunStop();

        public static void EditorPlugin_AddControlToEditorRunBar(Control control) =>
            kosmic_icall_Internal_EditorPlugin_AddControlToEditorRunBar(control.NativeInstance);

        public static void ScriptEditorDebugger_ReloadScripts() =>
            kosmic_icall_Internal_ScriptEditorDebugger_ReloadScripts();

        public static string[] CodeCompletionRequest(CodeCompletionRequest.CompletionKind kind,
            string scriptFile)
        {
            using kosmic_string scriptFileIn = Marshaling.ConvertStringToNative(scriptFile);
            kosmic_icall_Internal_CodeCompletionRequest((int)kind, scriptFileIn, out kosmic_packed_string_array res);
            using (res)
                return Marshaling.ConvertNativePackedStringArrayToSystemArray(res);
        }

        #region Internal

        private static bool initialized = false;

        // ReSharper disable once ParameterOnlyUsedForPreconditionCheck.Global
        internal static unsafe void Initialize(IntPtr unmanagedCallbacks, int unmanagedCallbacksSize)
        {
            if (initialized)
                throw new InvalidOperationException("Already initialized.");
            initialized = true;

            if (unmanagedCallbacksSize != sizeof(InternalUnmanagedCallbacks))
                throw new ArgumentException("Unmanaged callbacks size mismatch.", nameof(unmanagedCallbacksSize));

            _unmanagedCallbacks = Unsafe.AsRef<InternalUnmanagedCallbacks>((void*)unmanagedCallbacks);
        }

        private partial struct InternalUnmanagedCallbacks
        {
        }

        /*
         * IMPORTANT:
         * The order of the methods defined in NativeFuncs must match the order
         * in the array defined at the bottom of 'editor/editor_internal_calls.cpp'.
         */

        public static partial void kosmic_icall_KosmicSharpDirs_ResMetadataDir(out kosmic_string r_dest);

        public static partial void kosmic_icall_KosmicSharpDirs_MonoUserDir(out kosmic_string r_dest);

        public static partial void kosmic_icall_KosmicSharpDirs_BuildLogsDirs(out kosmic_string r_dest);

        public static partial void kosmic_icall_KosmicSharpDirs_DataEditorToolsDir(out kosmic_string r_dest);

        public static partial void kosmic_icall_KosmicSharpDirs_CSharpProjectName(out kosmic_string r_dest);

        public static partial void kosmic_icall_EditorProgress_Create(in kosmic_string task, in kosmic_string label,
            int amount, bool canCancel);

        public static partial void kosmic_icall_EditorProgress_Dispose(in kosmic_string task);

        public static partial bool kosmic_icall_EditorProgress_Step(in kosmic_string task, in kosmic_string state,
            int step,
            bool forceRefresh);

        private static partial void kosmic_icall_Internal_FullExportTemplatesDir(out kosmic_string dest);

        private static partial bool kosmic_icall_Internal_IsMacOSAppBundleInstalled(in kosmic_string bundleId);

        private static partial bool kosmic_icall_Internal_LipOCreateFile(in kosmic_string outputPath, in kosmic_packed_string_array files);

        private static partial bool kosmic_icall_Internal_KosmicIs32Bits();

        private static partial bool kosmic_icall_Internal_KosmicIsRealTDouble();

        private static partial void kosmic_icall_Internal_KosmicMainIteration();

        private static partial bool kosmic_icall_Internal_IsAssembliesReloadingNeeded();

        private static partial void kosmic_icall_Internal_ReloadAssemblies(bool softReload);

        private static partial void kosmic_icall_Internal_EditorDebuggerNodeReloadScripts();

        private static partial bool kosmic_icall_Internal_ScriptEditorEdit(IntPtr resource, int line, int col,
            bool grabFocus);

        private static partial void kosmic_icall_Internal_EditorNodeShowScriptScreen();

        private static partial void kosmic_icall_Internal_EditorRunPlay();

        private static partial void kosmic_icall_Internal_EditorRunStop();

        private static partial void kosmic_icall_Internal_EditorPlugin_AddControlToEditorRunBar(IntPtr p_control);

        private static partial void kosmic_icall_Internal_ScriptEditorDebugger_ReloadScripts();

        private static partial void kosmic_icall_Internal_CodeCompletionRequest(int kind, in kosmic_string scriptFile,
            out kosmic_packed_string_array res);

        public static partial float kosmic_icall_Globals_EditorScale();

        public static partial void kosmic_icall_Globals_GlobalDef(in kosmic_string setting, in kosmic_variant defaultValue,
            bool restartIfChanged, out kosmic_variant result);

        public static partial void kosmic_icall_Globals_EditorDef(in kosmic_string setting, in kosmic_variant defaultValue,
            bool restartIfChanged, out kosmic_variant result);

        public static partial void
            kosmic_icall_Globals_EditorDefShortcut(in kosmic_string setting, in kosmic_string name, Key keycode, kosmic_bool physical, out kosmic_variant result);

        public static partial void
            kosmic_icall_Globals_EditorGetShortcut(in kosmic_string setting, out kosmic_variant result);

        public static partial void
            kosmic_icall_Globals_EditorShortcutOverride(in kosmic_string setting, in kosmic_string feature, Key keycode, kosmic_bool physical);

        public static partial void kosmic_icall_Globals_TTR(in kosmic_string text, out kosmic_string dest);

        public static partial void kosmic_icall_Utils_OS_GetPlatformName(out kosmic_string dest);

        public static partial bool kosmic_icall_Utils_OS_UnixFileHasExecutableAccess(in kosmic_string filePath);

        #endregion
    }
}
