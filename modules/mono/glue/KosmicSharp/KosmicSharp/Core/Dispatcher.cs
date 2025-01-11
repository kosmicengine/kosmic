using System;
using System.Runtime.InteropServices;
using Kosmic.NativeInterop;

namespace Kosmic
{
    public static class Dispatcher
    {
        internal static KosmicTaskScheduler DefaultKosmicTaskScheduler;

        internal static void InitializeDefaultKosmicTaskScheduler()
        {
            DefaultKosmicTaskScheduler?.Dispose();
            DefaultKosmicTaskScheduler = new KosmicTaskScheduler();
        }

        public static KosmicSynchronizationContext SynchronizationContext => DefaultKosmicTaskScheduler.Context;
    }
}
