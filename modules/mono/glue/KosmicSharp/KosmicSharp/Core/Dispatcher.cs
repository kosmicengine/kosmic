using System;
using System.Runtime.InteropServices;
using Kosmic.NativeInterop;

namespace Kosmic
{
    public static class Dispatcher
    {
        internal static GodotTaskScheduler DefaultGodotTaskScheduler;

        internal static void InitializeDefaultGodotTaskScheduler()
        {
            DefaultGodotTaskScheduler?.Dispose();
            DefaultGodotTaskScheduler = new GodotTaskScheduler();
        }

        public static KosmicSynchronizationContext SynchronizationContext => DefaultGodotTaskScheduler.Context;
    }
}
