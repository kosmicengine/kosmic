using Kosmic;
using Kosmic.NativeInterop;
using System.Diagnostics.CodeAnalysis;
using System.Runtime.CompilerServices;

namespace KosmicTools.Internals
{
    public static class Globals
    {
        public static float EditorScale => Internal.kosmic_icall_Globals_EditorScale();

        // ReSharper disable once UnusedMethodReturnValue.Global
        public static Variant GlobalDef(string setting, Variant defaultValue, bool restartIfChanged = false)
        {
            using kosmic_string settingIn = Marshaling.ConvertStringToNative(setting);
            using kosmic_variant defaultValueIn = defaultValue.CopyNativeVariant();
            Internal.kosmic_icall_Globals_GlobalDef(settingIn, defaultValueIn, restartIfChanged,
                out kosmic_variant result);
            return Variant.CreateTakingOwnershipOfDisposableValue(result);
        }

        // ReSharper disable once UnusedMethodReturnValue.Global
        public static Variant EditorDef(string setting, Variant defaultValue, bool restartIfChanged = false)
        {
            using kosmic_string settingIn = Marshaling.ConvertStringToNative(setting);
            using kosmic_variant defaultValueIn = defaultValue.CopyNativeVariant();
            Internal.kosmic_icall_Globals_EditorDef(settingIn, defaultValueIn, restartIfChanged,
                out kosmic_variant result);
            return Variant.CreateTakingOwnershipOfDisposableValue(result);
        }

        public static Shortcut EditorDefShortcut(string setting, string name, Key keycode = Key.None, bool physical = false)
        {
            using kosmic_string settingIn = Marshaling.ConvertStringToNative(setting);
            using kosmic_string nameIn = Marshaling.ConvertStringToNative(name);
            Internal.kosmic_icall_Globals_EditorDefShortcut(settingIn, nameIn, keycode, physical.ToKosmicBool(), out kosmic_variant result);
            return (Shortcut)Variant.CreateTakingOwnershipOfDisposableValue(result);
        }

        public static Shortcut EditorGetShortcut(string setting)
        {
            using kosmic_string settingIn = Marshaling.ConvertStringToNative(setting);
            Internal.kosmic_icall_Globals_EditorGetShortcut(settingIn, out kosmic_variant result);
            return (Shortcut)Variant.CreateTakingOwnershipOfDisposableValue(result);
        }

        public static void EditorShortcutOverride(string setting, string feature, Key keycode = Key.None, bool physical = false)
        {
            using kosmic_string settingIn = Marshaling.ConvertStringToNative(setting);
            using kosmic_string featureIn = Marshaling.ConvertStringToNative(feature);
            Internal.kosmic_icall_Globals_EditorShortcutOverride(settingIn, featureIn, keycode, physical.ToKosmicBool());
        }

        [SuppressMessage("ReSharper", "InconsistentNaming")]
        public static string TTR(this string text)
        {
            using kosmic_string textIn = Marshaling.ConvertStringToNative(text);
            Internal.kosmic_icall_Globals_TTR(textIn, out kosmic_string dest);
            using (dest)
                return Marshaling.ConvertStringToManaged(dest);
        }
    }
}
