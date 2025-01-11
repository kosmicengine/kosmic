using System;
using System.Runtime.InteropServices;
using Kosmic.NativeInterop;

namespace Kosmic.Bridge
{
    internal static class CSharpInstanceBridge
    {
        [UnmanagedCallersOnly]
        internal static unsafe kosmic_bool Call(IntPtr kosmicObjectGCHandle, kosmic_string_name* method,
            kosmic_variant** args, int argCount, kosmic_variant_call_error* refCallError, kosmic_variant* ret)
        {
            try
            {
                var kosmicObject = (KosmicObject)GCHandle.FromIntPtr(kosmicObjectGCHandle).Target;

                if (kosmicObject == null)
                {
                    *ret = default;
                    (*refCallError).Error = kosmic_variant_call_error_error.KOSMIC_CALL_ERROR_CALL_ERROR_INSTANCE_IS_NULL;
                    return kosmic_bool.False;
                }

                bool methodInvoked = kosmicObject.InvokeKosmicClassMethod(CustomUnsafe.AsRef(method),
                    new NativeVariantPtrArgs(args, argCount), out kosmic_variant retValue);

                if (!methodInvoked)
                {
                    *ret = default;
                    // This is important, as it tells Object::call that no method was called.
                    // Otherwise, it would prevent Object::call from calling native methods.
                    (*refCallError).Error = kosmic_variant_call_error_error.KOSMIC_CALL_ERROR_CALL_ERROR_INVALID_METHOD;
                    return kosmic_bool.False;
                }

                *ret = retValue;
                return kosmic_bool.True;
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
                *ret = default;
                return kosmic_bool.False;
            }
        }

        [UnmanagedCallersOnly]
        internal static unsafe kosmic_bool Set(IntPtr kosmicObjectGCHandle, kosmic_string_name* name, kosmic_variant* value)
        {
            try
            {
                var kosmicObject = (KosmicObject)GCHandle.FromIntPtr(kosmicObjectGCHandle).Target;

                if (kosmicObject == null)
                    throw new InvalidOperationException();

                if (kosmicObject.SetKosmicClassPropertyValue(CustomUnsafe.AsRef(name), CustomUnsafe.AsRef(value)))
                {
                    return kosmic_bool.True;
                }

                var nameManaged = StringName.CreateTakingOwnershipOfDisposableValue(
                    NativeFuncs.kosmicsharp_string_name_new_copy(CustomUnsafe.AsRef(name)));

                Variant valueManaged = Variant.CreateCopyingBorrowed(*value);

                return kosmicObject._Set(nameManaged, valueManaged).ToKosmicBool();
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
                return kosmic_bool.False;
            }
        }

        [UnmanagedCallersOnly]
        internal static unsafe kosmic_bool Get(IntPtr kosmicObjectGCHandle, kosmic_string_name* name,
            kosmic_variant* outRet)
        {
            try
            {
                var kosmicObject = (KosmicObject)GCHandle.FromIntPtr(kosmicObjectGCHandle).Target;

                if (kosmicObject == null)
                    throw new InvalidOperationException();

                // Properties
                if (kosmicObject.GetKosmicClassPropertyValue(CustomUnsafe.AsRef(name), out kosmic_variant outRetValue))
                {
                    *outRet = outRetValue;
                    return kosmic_bool.True;
                }

                // Signals
                if (kosmicObject.HasKosmicClassSignal(CustomUnsafe.AsRef(name)))
                {
                    kosmic_signal signal = new kosmic_signal(NativeFuncs.kosmicsharp_string_name_new_copy(*name), kosmicObject.GetInstanceId());
                    *outRet = VariantUtils.CreateFromSignalTakingOwnershipOfDisposableValue(signal);
                    return kosmic_bool.True;
                }

                // Methods
                if (kosmicObject.HasKosmicClassMethod(CustomUnsafe.AsRef(name)))
                {
                    kosmic_callable method = new kosmic_callable(NativeFuncs.kosmicsharp_string_name_new_copy(*name), kosmicObject.GetInstanceId());
                    *outRet = VariantUtils.CreateFromCallableTakingOwnershipOfDisposableValue(method);
                    return kosmic_bool.True;
                }

                var nameManaged = StringName.CreateTakingOwnershipOfDisposableValue(
                    NativeFuncs.kosmicsharp_string_name_new_copy(CustomUnsafe.AsRef(name)));

                Variant ret = kosmicObject._Get(nameManaged);

                if (ret.VariantType == Variant.Type.Nil)
                {
                    *outRet = default;
                    return kosmic_bool.False;
                }

                *outRet = ret.CopyNativeVariant();
                return kosmic_bool.True;
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
                *outRet = default;
                return kosmic_bool.False;
            }
        }

        [UnmanagedCallersOnly]
        internal static void CallDispose(IntPtr kosmicObjectGCHandle, kosmic_bool okIfNull)
        {
            try
            {
                var kosmicObject = (KosmicObject)GCHandle.FromIntPtr(kosmicObjectGCHandle).Target;

                if (okIfNull.ToBool())
                    kosmicObject?.Dispose();
                else
                    kosmicObject!.Dispose();
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
            }
        }

        [UnmanagedCallersOnly]
        internal static unsafe void CallToString(IntPtr kosmicObjectGCHandle, kosmic_string* outRes, kosmic_bool* outValid)
        {
            try
            {
                var self = (KosmicObject)GCHandle.FromIntPtr(kosmicObjectGCHandle).Target;

                if (self == null)
                {
                    *outRes = default;
                    *outValid = kosmic_bool.False;
                    return;
                }

                var resultStr = self.ToString();

                if (resultStr == null)
                {
                    *outRes = default;
                    *outValid = kosmic_bool.False;
                    return;
                }

                *outRes = Marshaling.ConvertStringToNative(resultStr);
                *outValid = kosmic_bool.True;
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
                *outRes = default;
                *outValid = kosmic_bool.False;
            }
        }

        [UnmanagedCallersOnly]
        internal static unsafe kosmic_bool HasMethodUnknownParams(IntPtr kosmicObjectGCHandle, kosmic_string_name* method)
        {
            try
            {
                var kosmicObject = (KosmicObject)GCHandle.FromIntPtr(kosmicObjectGCHandle).Target;

                if (kosmicObject == null)
                    return kosmic_bool.False;

                return kosmicObject.HasKosmicClassMethod(CustomUnsafe.AsRef(method)).ToKosmicBool();
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
                return kosmic_bool.False;
            }
        }

        [UnmanagedCallersOnly]
        internal static unsafe void SerializeState(
            IntPtr kosmicObjectGCHandle,
            kosmic_dictionary* propertiesState,
            kosmic_dictionary* signalEventsState
        )
        {
            try
            {
                var kosmicObject = (KosmicObject)GCHandle.FromIntPtr(kosmicObjectGCHandle).Target;

                if (kosmicObject == null)
                    return;

                // Call OnBeforeSerialize

                // ReSharper disable once SuspiciousTypeConversion.Global
                if (kosmicObject is ISerializationListener serializationListener)
                    serializationListener.OnBeforeSerialize();

                // Save instance state

                using var info = KosmicSerializationInfo.CreateCopyingBorrowed(
                    *propertiesState, *signalEventsState);

                kosmicObject.SaveKosmicObjectData(info);
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
            }
        }

        [UnmanagedCallersOnly]
        internal static unsafe void DeserializeState(
            IntPtr kosmicObjectGCHandle,
            kosmic_dictionary* propertiesState,
            kosmic_dictionary* signalEventsState
        )
        {
            try
            {
                var kosmicObject = (KosmicObject)GCHandle.FromIntPtr(kosmicObjectGCHandle).Target;

                if (kosmicObject == null)
                    return;

                // Restore instance state

                using var info = KosmicSerializationInfo.CreateCopyingBorrowed(
                    *propertiesState, *signalEventsState);

                kosmicObject.RestoreKosmicObjectData(info);

                // Call OnAfterDeserialize

                // ReSharper disable once SuspiciousTypeConversion.Global
                if (kosmicObject is ISerializationListener serializationListener)
                    serializationListener.OnAfterDeserialize();
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
            }
        }
    }
}
