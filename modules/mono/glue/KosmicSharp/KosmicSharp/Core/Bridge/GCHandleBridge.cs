using System;
using System.Runtime.InteropServices;
using Kosmic.NativeInterop;

namespace Kosmic.Bridge
{
    internal static class GCHandleBridge
    {
        [UnmanagedCallersOnly]
        internal static void FreeGCHandle(IntPtr gcHandlePtr)
        {
            try
            {
                CustomGCHandle.Free(GCHandle.FromIntPtr(gcHandlePtr));
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
            }
        }

        // Returns true, if releasing the provided handle is necessary for assembly unloading to succeed.
        // This check is not perfect and only intended to prevent things in GodotTools from being reloaded.
        [UnmanagedCallersOnly]
        internal static kosmic_bool GCHandleIsTargetCollectible(IntPtr gcHandlePtr)
        {
            try
            {
                var target = GCHandle.FromIntPtr(gcHandlePtr).Target;

                if (target is Delegate @delegate)
                    return DelegateUtils.IsDelegateCollectible(@delegate).ToGodotBool();

                return target.GetType().IsCollectible.ToGodotBool();
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
                return kosmic_bool.True;
            }
        }
    }
}
