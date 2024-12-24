using System;
using System.Runtime.InteropServices;
using Kosmic.NativeInterop;

namespace Kosmic.Bridge
{
    internal static class CSharpInstanceBridge
    {
        [UnmanagedCallersOnly]
        internal static unsafe kosmic_bool Call(IntPtr godotObjectGCHandle, kosmic_string_name* method,
            kosmic_variant** args, int argCount, kosmic_variant_call_error* refCallError, kosmic_variant* ret)
        {
            try
            {
                var godotObject = (KosmicObject)GCHandle.FromIntPtr(godotObjectGCHandle).Target;

                if (godotObject == null)
                {
                    *ret = default;
                    (*refCallError).Error = kosmic_variant_call_error_error.KOSMIC_CALL_ERROR_CALL_ERROR_INSTANCE_IS_NULL;
                    return kosmic_bool.False;
                }

                bool methodInvoked = godotObject.InvokeKosmicClassMethod(CustomUnsafe.AsRef(method),
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
        internal static unsafe kosmic_bool Set(IntPtr godotObjectGCHandle, kosmic_string_name* name, kosmic_variant* value)
        {
            try
            {
                var godotObject = (KosmicObject)GCHandle.FromIntPtr(godotObjectGCHandle).Target;

                if (godotObject == null)
                    throw new InvalidOperationException();

                if (godotObject.SetKosmicClassPropertyValue(CustomUnsafe.AsRef(name), CustomUnsafe.AsRef(value)))
                {
                    return kosmic_bool.True;
                }

                var nameManaged = StringName.CreateTakingOwnershipOfDisposableValue(
                    NativeFuncs.kosmicsharp_string_name_new_copy(CustomUnsafe.AsRef(name)));

                Variant valueManaged = Variant.CreateCopyingBorrowed(*value);

                return godotObject._Set(nameManaged, valueManaged).ToGodotBool();
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
                return kosmic_bool.False;
            }
        }

        [UnmanagedCallersOnly]
        internal static unsafe kosmic_bool Get(IntPtr godotObjectGCHandle, kosmic_string_name* name,
            kosmic_variant* outRet)
        {
            try
            {
                var godotObject = (KosmicObject)GCHandle.FromIntPtr(godotObjectGCHandle).Target;

                if (godotObject == null)
                    throw new InvalidOperationException();

                // Properties
                if (godotObject.GetKosmicClassPropertyValue(CustomUnsafe.AsRef(name), out kosmic_variant outRetValue))
                {
                    *outRet = outRetValue;
                    return kosmic_bool.True;
                }

                // Signals
                if (godotObject.HasKosmicClassSignal(CustomUnsafe.AsRef(name)))
                {
                    kosmic_signal signal = new kosmic_signal(NativeFuncs.kosmicsharp_string_name_new_copy(*name), godotObject.GetInstanceId());
                    *outRet = VariantUtils.CreateFromSignalTakingOwnershipOfDisposableValue(signal);
                    return kosmic_bool.True;
                }

                // Methods
                if (godotObject.HasKosmicClassMethod(CustomUnsafe.AsRef(name)))
                {
                    kosmic_callable method = new kosmic_callable(NativeFuncs.kosmicsharp_string_name_new_copy(*name), godotObject.GetInstanceId());
                    *outRet = VariantUtils.CreateFromCallableTakingOwnershipOfDisposableValue(method);
                    return kosmic_bool.True;
                }

                var nameManaged = StringName.CreateTakingOwnershipOfDisposableValue(
                    NativeFuncs.kosmicsharp_string_name_new_copy(CustomUnsafe.AsRef(name)));

                Variant ret = godotObject._Get(nameManaged);

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
        internal static void CallDispose(IntPtr godotObjectGCHandle, kosmic_bool okIfNull)
        {
            try
            {
                var godotObject = (KosmicObject)GCHandle.FromIntPtr(godotObjectGCHandle).Target;

                if (okIfNull.ToBool())
                    godotObject?.Dispose();
                else
                    godotObject!.Dispose();
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
            }
        }

        [UnmanagedCallersOnly]
        internal static unsafe void CallToString(IntPtr godotObjectGCHandle, kosmic_string* outRes, kosmic_bool* outValid)
        {
            try
            {
                var self = (KosmicObject)GCHandle.FromIntPtr(godotObjectGCHandle).Target;

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
        internal static unsafe kosmic_bool HasMethodUnknownParams(IntPtr godotObjectGCHandle, kosmic_string_name* method)
        {
            try
            {
                var godotObject = (KosmicObject)GCHandle.FromIntPtr(godotObjectGCHandle).Target;

                if (godotObject == null)
                    return kosmic_bool.False;

                return godotObject.HasKosmicClassMethod(CustomUnsafe.AsRef(method)).ToGodotBool();
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
                return kosmic_bool.False;
            }
        }

        [UnmanagedCallersOnly]
        internal static unsafe void SerializeState(
            IntPtr godotObjectGCHandle,
            kosmic_dictionary* propertiesState,
            kosmic_dictionary* signalEventsState
        )
        {
            try
            {
                var godotObject = (KosmicObject)GCHandle.FromIntPtr(godotObjectGCHandle).Target;

                if (godotObject == null)
                    return;

                // Call OnBeforeSerialize

                // ReSharper disable once SuspiciousTypeConversion.Global
                if (godotObject is ISerializationListener serializationListener)
                    serializationListener.OnBeforeSerialize();

                // Save instance state

                using var info = KosmicSerializationInfo.CreateCopyingBorrowed(
                    *propertiesState, *signalEventsState);

                godotObject.SaveKosmicObjectData(info);
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
            }
        }

        [UnmanagedCallersOnly]
        internal static unsafe void DeserializeState(
            IntPtr godotObjectGCHandle,
            kosmic_dictionary* propertiesState,
            kosmic_dictionary* signalEventsState
        )
        {
            try
            {
                var godotObject = (KosmicObject)GCHandle.FromIntPtr(godotObjectGCHandle).Target;

                if (godotObject == null)
                    return;

                // Restore instance state

                using var info = KosmicSerializationInfo.CreateCopyingBorrowed(
                    *propertiesState, *signalEventsState);

                godotObject.RestoreKosmicObjectData(info);

                // Call OnAfterDeserialize

                // ReSharper disable once SuspiciousTypeConversion.Global
                if (godotObject is ISerializationListener serializationListener)
                    serializationListener.OnAfterDeserialize();
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
            }
        }
    }
}
