using System;
using System.Runtime.InteropServices;
using Kosmic.NativeInterop;

namespace Kosmic
{
    public class SignalAwaiter : IAwaiter<Variant[]>, IAwaitable<Variant[]>
    {
        private bool _completed;
        private Variant[] _result;
        private Action _continuation;

        public SignalAwaiter(KosmicObject source, StringName signal, KosmicObject target)
        {
            var awaiterGcHandle = CustomGCHandle.AllocStrong(this);
            using kosmic_string_name signalSrc = NativeFuncs.kosmicsharp_string_name_new_copy(
                (kosmic_string_name)(signal?.NativeValue ?? default));
            NativeFuncs.kosmicsharp_internal_signal_awaiter_connect(KosmicObject.GetPtr(source), in signalSrc,
                KosmicObject.GetPtr(target), GCHandle.ToIntPtr(awaiterGcHandle));
        }

        public bool IsCompleted => _completed;

        public void OnCompleted(Action continuation)
        {
            _continuation = continuation;
        }

        public Variant[] GetResult() => _result;

        public IAwaiter<Variant[]> GetAwaiter() => this;

        [UnmanagedCallersOnly]
        internal static unsafe void SignalCallback(IntPtr awaiterGCHandlePtr, kosmic_variant** args, int argCount,
            kosmic_bool* outAwaiterIsNull)
        {
            try
            {
                var awaiter = (SignalAwaiter)GCHandle.FromIntPtr(awaiterGCHandlePtr).Target;

                if (awaiter == null)
                {
                    *outAwaiterIsNull = kosmic_bool.True;
                    return;
                }

                *outAwaiterIsNull = kosmic_bool.False;

                awaiter._completed = true;

                Variant[] signalArgs = new Variant[argCount];

                for (int i = 0; i < argCount; i++)
                    signalArgs[i] = Variant.CreateCopyingBorrowed(*args[i]);

                awaiter._result = signalArgs;

                awaiter._continuation?.Invoke();
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
                *outAwaiterIsNull = kosmic_bool.False;
            }
        }
    }
}
