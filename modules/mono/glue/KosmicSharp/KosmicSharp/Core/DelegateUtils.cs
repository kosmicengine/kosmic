#nullable enable

using System;
using System.Collections.Generic;
using System.Diagnostics.CodeAnalysis;
using System.Globalization;
using System.IO;
using System.Reflection;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;
using Kosmic.NativeInterop;

namespace Kosmic
{
    internal static class DelegateUtils
    {
        [UnmanagedCallersOnly]
        internal static kosmic_bool DelegateEquals(IntPtr delegateGCHandleA, IntPtr delegateGCHandleB)
        {
            try
            {
                var @delegateA = (Delegate?)GCHandle.FromIntPtr(delegateGCHandleA).Target;
                var @delegateB = (Delegate?)GCHandle.FromIntPtr(delegateGCHandleB).Target;
                return (@delegateA! == @delegateB!).ToKosmicBool();
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
                return kosmic_bool.False;
            }
        }

        [UnmanagedCallersOnly]
        internal static int DelegateHash(IntPtr delegateGCHandle)
        {
            try
            {
                var @delegate = (Delegate?)GCHandle.FromIntPtr(delegateGCHandle).Target;
                return @delegate?.GetHashCode() ?? 0;
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
                return 0;
            }
        }

        [UnmanagedCallersOnly]
        internal static unsafe int GetArgumentCount(IntPtr delegateGCHandle, kosmic_bool* outIsValid)
        {
            try
            {
                var @delegate = (Delegate?)GCHandle.FromIntPtr(delegateGCHandle).Target;
                int? argCount = @delegate?.Method?.GetParameters().Length;
                if (argCount is null)
                {
                    *outIsValid = kosmic_bool.False;
                    return 0;
                }
                *outIsValid = kosmic_bool.True;
                return argCount.Value;
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
                *outIsValid = kosmic_bool.False;
                return 0;
            }
        }

        [UnmanagedCallersOnly]
        internal static unsafe void InvokeWithVariantArgs(IntPtr delegateGCHandle, void* trampoline,
            kosmic_variant** args, int argc, kosmic_variant* outRet)
        {
            try
            {
                if (trampoline == null)
                {
                    throw new ArgumentNullException(nameof(trampoline),
                        "Cannot dynamically invoke delegate because the trampoline is null.");
                }

                var @delegate = (Delegate)GCHandle.FromIntPtr(delegateGCHandle).Target!;
                var trampolineFn = (delegate* managed<object, NativeVariantPtrArgs, out kosmic_variant, void>)trampoline;

                trampolineFn(@delegate, new NativeVariantPtrArgs(args, argc), out kosmic_variant ret);

                *outRet = ret;
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
                *outRet = default;
            }
        }

        // TODO: Check if we should be using BindingFlags.DeclaredOnly (would give better reflection performance).

        private enum TargetKind : uint
        {
            Static,
            KosmicObject,
            CompilerGenerated
        }

        internal static bool TrySerializeDelegate(Delegate @delegate, Collections.Array serializedData)
        {
            if (@delegate is null)
            {
                return false;
            }

            if (@delegate is MulticastDelegate multicastDelegate)
            {
                bool someDelegatesSerialized = false;

                Delegate[] invocationList = multicastDelegate.GetInvocationList();

                if (invocationList.Length > 1)
                {
                    var multiCastData = new Collections.Array();

                    foreach (Delegate oneDelegate in invocationList)
                        someDelegatesSerialized |= TrySerializeDelegate(oneDelegate, multiCastData);

                    if (!someDelegatesSerialized)
                        return false;

                    serializedData.Add(multiCastData);
                    return true;
                }
            }

            if (TrySerializeSingleDelegate(@delegate, out byte[]? buffer))
            {
                serializedData.Add((Span<byte>)buffer);
                return true;
            }

            return false;
        }

        private static bool TrySerializeSingleDelegate(Delegate @delegate, [MaybeNullWhen(false)] out byte[] buffer)
        {
            buffer = null;

            object? target = @delegate.Target;

            switch (target)
            {
                case null:
                {
                    using (var stream = new MemoryStream())
                    using (var writer = new BinaryWriter(stream))
                    {
                        writer.Write((ulong)TargetKind.Static);

                        SerializeType(writer, @delegate.GetType());

                        if (!TrySerializeMethodInfo(writer, @delegate.Method))
                            return false;

                        buffer = stream.ToArray();
                        return true;
                    }
                }
                case KosmicObject kosmicObject:
                {
                    using (var stream = new MemoryStream())
                    using (var writer = new BinaryWriter(stream))
                    {
                        writer.Write((ulong)TargetKind.KosmicObject);
                        // ReSharper disable once RedundantCast
                        writer.Write((ulong)kosmicObject.GetInstanceId());

                        SerializeType(writer, @delegate.GetType());

                        if (!TrySerializeMethodInfo(writer, @delegate.Method))
                            return false;

                        buffer = stream.ToArray();
                        return true;
                    }
                }
                default:
                {
                    Type targetType = target.GetType();

                    if (targetType.IsDefined(typeof(CompilerGeneratedAttribute), true))
                    {
                        // Compiler generated. Probably a closure. Try to serialize it.

                        using (var stream = new MemoryStream())
                        using (var writer = new BinaryWriter(stream))
                        {
                            writer.Write((ulong)TargetKind.CompilerGenerated);
                            SerializeType(writer, targetType);

                            SerializeType(writer, @delegate.GetType());

                            if (!TrySerializeMethodInfo(writer, @delegate.Method))
                                return false;

                            FieldInfo[] fields = targetType.GetFields(BindingFlags.Instance | BindingFlags.Public);

                            writer.Write(fields.Length);

                            foreach (FieldInfo field in fields)
                            {
                                Type fieldType = field.FieldType;

                                Variant.Type variantType = KS.TypeToVariantType(fieldType);

                                if (variantType == Variant.Type.Nil)
                                    return false;

                                static byte[] VarToBytes(in kosmic_variant var)
                                {
                                    NativeFuncs.kosmicsharp_var_to_bytes(var, kosmic_bool.True, out var varBytes);
                                    using (varBytes)
                                        return Marshaling.ConvertNativePackedByteArrayToSystemArray(varBytes);
                                }

                                writer.Write(field.Name);

                                var fieldValue = field.GetValue(target);
                                using var fieldValueVariant = RuntimeTypeConversionHelper.ConvertToVariant(fieldValue);
                                byte[] valueBuffer = VarToBytes(fieldValueVariant);
                                writer.Write(valueBuffer.Length);
                                writer.Write(valueBuffer);
                            }

                            buffer = stream.ToArray();
                            return true;
                        }
                    }

                    return false;
                }
            }
        }

        private static bool TrySerializeMethodInfo(BinaryWriter writer, MethodInfo methodInfo)
        {
            SerializeType(writer, methodInfo.DeclaringType);

            writer.Write(methodInfo.Name);

            int flags = 0;

            if (methodInfo.IsPublic)
                flags |= (int)BindingFlags.Public;
            else
                flags |= (int)BindingFlags.NonPublic;

            if (methodInfo.IsStatic)
                flags |= (int)BindingFlags.Static;
            else
                flags |= (int)BindingFlags.Instance;

            writer.Write(flags);

            Type returnType = methodInfo.ReturnType;
            bool hasReturn = methodInfo.ReturnType != typeof(void);

            writer.Write(hasReturn);
            if (hasReturn)
                SerializeType(writer, returnType);

            ParameterInfo[] parameters = methodInfo.GetParameters();

            writer.Write(parameters.Length);

            if (parameters.Length > 0)
            {
                for (int i = 0; i < parameters.Length; i++)
                    SerializeType(writer, parameters[i].ParameterType);
            }

            return true;
        }

        private static void SerializeType(BinaryWriter writer, Type? type)
        {
            if (type == null)
            {
                int genericArgumentsCount = -1;
                writer.Write(genericArgumentsCount);
            }
            else if (type.IsGenericType)
            {
                Type genericTypeDef = type.GetGenericTypeDefinition();
                Type[] genericArgs = type.GetGenericArguments();

                int genericArgumentsCount = genericArgs.Length;
                writer.Write(genericArgumentsCount);

                writer.Write(genericTypeDef.Assembly.GetName().Name ?? "");
                writer.Write(genericTypeDef.FullName ?? genericTypeDef.ToString());

                for (int i = 0; i < genericArgs.Length; i++)
                    SerializeType(writer, genericArgs[i]);
            }
            else
            {
                int genericArgumentsCount = 0;
                writer.Write(genericArgumentsCount);

                writer.Write(type.Assembly.GetName().Name ?? "");
                writer.Write(type.FullName ?? type.ToString());
            }
        }

        [UnmanagedCallersOnly]
        internal static unsafe kosmic_bool TrySerializeDelegateWithGCHandle(IntPtr delegateGCHandle,
            kosmic_array* nSerializedData)
        {
            try
            {
                var serializedData = Collections.Array.CreateTakingOwnershipOfDisposableValue(
                    NativeFuncs.kosmicsharp_array_new_copy(*nSerializedData));

                var @delegate = (Delegate)GCHandle.FromIntPtr(delegateGCHandle).Target!;

                return TrySerializeDelegate(@delegate, serializedData)
                    .ToKosmicBool();
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
                return kosmic_bool.False;
            }
        }

        [UnmanagedCallersOnly]
        internal static unsafe kosmic_bool TryDeserializeDelegateWithGCHandle(kosmic_array* nSerializedData,
            IntPtr* delegateGCHandle)
        {
            try
            {
                var serializedData = Collections.Array.CreateTakingOwnershipOfDisposableValue(
                    NativeFuncs.kosmicsharp_array_new_copy(*nSerializedData));

                if (TryDeserializeDelegate(serializedData, out Delegate? @delegate))
                {
                    *delegateGCHandle = GCHandle.ToIntPtr(CustomGCHandle.AllocStrong(@delegate));
                    return kosmic_bool.True;
                }
                else
                {
                    *delegateGCHandle = IntPtr.Zero;
                    return kosmic_bool.False;
                }
            }
            catch (Exception e)
            {
                ExceptionUtils.LogException(e);
                *delegateGCHandle = default;
                return kosmic_bool.False;
            }
        }

        internal static bool TryDeserializeDelegate(Collections.Array serializedData,
            [MaybeNullWhen(false)] out Delegate @delegate)
        {
            @delegate = null;

            if (serializedData.Count == 1)
            {
                var elem = serializedData[0].Obj;

                if (elem == null)
                    return false;

                if (elem is Collections.Array multiCastData)
                    return TryDeserializeDelegate(multiCastData, out @delegate);

                return TryDeserializeSingleDelegate((byte[])elem, out @delegate);
            }

            var delegates = new List<Delegate>(serializedData.Count);

            foreach (Variant variantElem in serializedData)
            {
                var elem = variantElem.Obj;

                if (elem == null)
                    continue;

                if (elem is Collections.Array multiCastData)
                {
                    if (TryDeserializeDelegate(multiCastData, out Delegate? oneDelegate))
                        delegates.Add(oneDelegate);
                }
                else
                {
                    if (TryDeserializeSingleDelegate((byte[])elem, out Delegate? oneDelegate))
                        delegates.Add(oneDelegate);
                }
            }

            if (delegates.Count <= 0)
                return false;

            @delegate = delegates.Count == 1 ? delegates[0] : Delegate.Combine(delegates.ToArray())!;
            return true;
        }

        private static bool TryDeserializeSingleDelegate(byte[] buffer, [MaybeNullWhen(false)] out Delegate @delegate)
        {
            @delegate = null;

            using (var stream = new MemoryStream(buffer, writable: false))
            using (var reader = new BinaryReader(stream))
            {
                var targetKind = (TargetKind)reader.ReadUInt64();

                switch (targetKind)
                {
                    case TargetKind.Static:
                    {
                        Type? delegateType = DeserializeType(reader);
                        if (delegateType == null)
                            return false;

                        if (!TryDeserializeMethodInfo(reader, out MethodInfo? methodInfo))
                            return false;

                        @delegate = Delegate.CreateDelegate(delegateType, null, methodInfo, throwOnBindFailure: false);

                        if (@delegate == null)
                            return false;

                        return true;
                    }
                    case TargetKind.KosmicObject:
                    {
                        ulong objectId = reader.ReadUInt64();
                        KosmicObject? kosmicObject = KosmicObject.InstanceFromId(objectId);
                        if (kosmicObject == null)
                            return false;

                        Type? delegateType = DeserializeType(reader);
                        if (delegateType == null)
                            return false;

                        if (!TryDeserializeMethodInfo(reader, out MethodInfo? methodInfo))
                            return false;

                        @delegate = Delegate.CreateDelegate(delegateType, kosmicObject, methodInfo,
                            throwOnBindFailure: false);

                        if (@delegate == null)
                            return false;

                        return true;
                    }
                    case TargetKind.CompilerGenerated:
                    {
                        Type? targetType = DeserializeType(reader);
                        if (targetType == null)
                            return false;

                        Type? delegateType = DeserializeType(reader);
                        if (delegateType == null)
                            return false;

                        if (!TryDeserializeMethodInfo(reader, out MethodInfo? methodInfo))
                            return false;

                        int fieldCount = reader.ReadInt32();

                        object recreatedTarget = Activator.CreateInstance(targetType)!;

                        for (int i = 0; i < fieldCount; i++)
                        {
                            string name = reader.ReadString();
                            int valueBufferLength = reader.ReadInt32();
                            byte[] valueBuffer = reader.ReadBytes(valueBufferLength);

                            FieldInfo? fieldInfo = targetType.GetField(name,
                                BindingFlags.Instance | BindingFlags.Public);

                            if (fieldInfo != null)
                            {
                                var variantValue = KS.BytesToVarWithObjects(valueBuffer);
                                object? managedValue = RuntimeTypeConversionHelper.ConvertToObjectOfType(
                                    (kosmic_variant)variantValue.NativeVar, fieldInfo.FieldType);
                                fieldInfo.SetValue(recreatedTarget, managedValue);
                            }
                        }

                        @delegate = Delegate.CreateDelegate(delegateType, recreatedTarget, methodInfo,
                            throwOnBindFailure: false);

                        if (@delegate == null)
                            return false;

                        return true;
                    }
                    default:
                        return false;
                }
            }
        }

        private static bool TryDeserializeMethodInfo(BinaryReader reader,
            [MaybeNullWhen(false)] out MethodInfo methodInfo)
        {
            methodInfo = null;

            Type? declaringType = DeserializeType(reader);

            if (declaringType == null)
                return false;

            string methodName = reader.ReadString();

            BindingFlags flags = (BindingFlags)reader.ReadInt32();

            bool hasReturn = reader.ReadBoolean();
            Type? returnType = hasReturn ? DeserializeType(reader) : typeof(void);

            int parametersCount = reader.ReadInt32();
            var parameterTypes = parametersCount == 0 ? Type.EmptyTypes : new Type[parametersCount];

            for (int i = 0; i < parametersCount; i++)
            {
                Type? parameterType = DeserializeType(reader);
                if (parameterType == null)
                    return false;
                parameterTypes[i] = parameterType;
            }

#pragma warning disable REFL045 // These flags are insufficient to match any members
            // TODO: Suppressing invalid warning, remove when issue is fixed
            // https://github.com/DotNetAnalyzers/ReflectionAnalyzers/issues/209
            methodInfo = declaringType.GetMethod(methodName, flags, null, parameterTypes, null);
#pragma warning restore REFL045
            return methodInfo != null && methodInfo.ReturnType == returnType;
        }

        private static Type? DeserializeType(BinaryReader reader)
        {
            int genericArgumentsCount = reader.ReadInt32();

            if (genericArgumentsCount == -1)
                return null;

            string assemblyName = reader.ReadString();

            if (assemblyName.Length == 0)
            {
                KS.PushError($"Missing assembly name of type when attempting to deserialize delegate");
                return null;
            }

            string typeFullName = reader.ReadString();
            var type = ReflectionUtils.FindTypeInLoadedAssemblies(assemblyName, typeFullName);

            if (type == null)
                return null; // Type not found

            if (genericArgumentsCount != 0)
            {
                var genericArgumentTypes = new Type[genericArgumentsCount];

                for (int i = 0; i < genericArgumentsCount; i++)
                {
                    Type? genericArgumentType = DeserializeType(reader);
                    if (genericArgumentType == null)
                        return null;
                    genericArgumentTypes[i] = genericArgumentType;
                }

                type = type.MakeGenericType(genericArgumentTypes);
            }

            return type;
        }

        // Returns true, if unloading the delegate is necessary for assembly unloading to succeed.
        // This check is not perfect and only intended to prevent things in KosmicTools from being reloaded.
        internal static bool IsDelegateCollectible(Delegate @delegate)
        {
            if (@delegate.GetType().IsCollectible)
                return true;

            if (@delegate is MulticastDelegate multicastDelegate)
            {
                Delegate[] invocationList = multicastDelegate.GetInvocationList();

                if (invocationList.Length > 1)
                {
                    foreach (Delegate oneDelegate in invocationList)
                        if (IsDelegateCollectible(oneDelegate))
                            return true;

                    return false;
                }
            }

            if (@delegate.Method.IsCollectible)
                return true;

            object? target = @delegate.Target;

            if (target is not null && target.GetType().IsCollectible)
                return true;

            return false;
        }

        internal static class RuntimeTypeConversionHelper
        {
            public static kosmic_variant ConvertToVariant(object? obj)
            {
                if (obj == null)
                    return default;

                switch (obj)
                {
                    case bool @bool:
                        return VariantUtils.CreateFrom(@bool);
                    case char @char:
                        return VariantUtils.CreateFrom(@char);
                    case sbyte int8:
                        return VariantUtils.CreateFrom(int8);
                    case short int16:
                        return VariantUtils.CreateFrom(int16);
                    case int int32:
                        return VariantUtils.CreateFrom(int32);
                    case long int64:
                        return VariantUtils.CreateFrom(int64);
                    case byte uint8:
                        return VariantUtils.CreateFrom(uint8);
                    case ushort uint16:
                        return VariantUtils.CreateFrom(uint16);
                    case uint uint32:
                        return VariantUtils.CreateFrom(uint32);
                    case ulong uint64:
                        return VariantUtils.CreateFrom(uint64);
                    case float @float:
                        return VariantUtils.CreateFrom(@float);
                    case double @double:
                        return VariantUtils.CreateFrom(@double);
                    case Vector2 vector2:
                        return VariantUtils.CreateFrom(vector2);
                    case Vector2I vector2I:
                        return VariantUtils.CreateFrom(vector2I);
                    case Rect2 rect2:
                        return VariantUtils.CreateFrom(rect2);
                    case Rect2I rect2I:
                        return VariantUtils.CreateFrom(rect2I);
                    case Transform2D transform2D:
                        return VariantUtils.CreateFrom(transform2D);
                    case Vector3 vector3:
                        return VariantUtils.CreateFrom(vector3);
                    case Vector3I vector3I:
                        return VariantUtils.CreateFrom(vector3I);
                    case Vector4 vector4:
                        return VariantUtils.CreateFrom(vector4);
                    case Vector4I vector4I:
                        return VariantUtils.CreateFrom(vector4I);
                    case Basis basis:
                        return VariantUtils.CreateFrom(basis);
                    case Quaternion quaternion:
                        return VariantUtils.CreateFrom(quaternion);
                    case Transform3D transform3D:
                        return VariantUtils.CreateFrom(transform3D);
                    case Projection projection:
                        return VariantUtils.CreateFrom(projection);
                    case Aabb aabb:
                        return VariantUtils.CreateFrom(aabb);
                    case Color color:
                        return VariantUtils.CreateFrom(color);
                    case Plane plane:
                        return VariantUtils.CreateFrom(plane);
                    case Callable callable:
                        return VariantUtils.CreateFrom(callable);
                    case Signal signal:
                        return VariantUtils.CreateFrom(signal);
                    case string @string:
                        return VariantUtils.CreateFrom(@string);
                    case byte[] byteArray:
                        return VariantUtils.CreateFrom(byteArray);
                    case int[] int32Array:
                        return VariantUtils.CreateFrom(int32Array);
                    case long[] int64Array:
                        return VariantUtils.CreateFrom(int64Array);
                    case float[] floatArray:
                        return VariantUtils.CreateFrom(floatArray);
                    case double[] doubleArray:
                        return VariantUtils.CreateFrom(doubleArray);
                    case string[] stringArray:
                        return VariantUtils.CreateFrom(stringArray);
                    case Vector2[] vector2Array:
                        return VariantUtils.CreateFrom(vector2Array);
                    case Vector3[] vector3Array:
                        return VariantUtils.CreateFrom(vector3Array);
                    case Color[] colorArray:
                        return VariantUtils.CreateFrom(colorArray);
                    case StringName[] stringNameArray:
                        return VariantUtils.CreateFrom(stringNameArray);
                    case NodePath[] nodePathArray:
                        return VariantUtils.CreateFrom(nodePathArray);
                    case Rid[] ridArray:
                        return VariantUtils.CreateFrom(ridArray);
                    case KosmicObject[] kosmicObjectArray:
                        return VariantUtils.CreateFrom(kosmicObjectArray);
                    case StringName stringName:
                        return VariantUtils.CreateFrom(stringName);
                    case NodePath nodePath:
                        return VariantUtils.CreateFrom(nodePath);
                    case Rid rid:
                        return VariantUtils.CreateFrom(rid);
                    case Collections.Dictionary kosmicDictionary:
                        return VariantUtils.CreateFrom(kosmicDictionary);
                    case Collections.Array kosmicArray:
                        return VariantUtils.CreateFrom(kosmicArray);
                    case Variant variant:
                        return VariantUtils.CreateFrom(variant);
                    case KosmicObject kosmicObject:
                        return VariantUtils.CreateFrom(kosmicObject);
                    case Enum @enum:
                        return VariantUtils.CreateFrom(Convert.ToInt64(@enum, CultureInfo.InvariantCulture));
                    case Collections.IGenericKosmicDictionary kosmicDictionary:
                        return VariantUtils.CreateFrom(kosmicDictionary.UnderlyingDictionary);
                    case Collections.IGenericKosmicArray kosmicArray:
                        return VariantUtils.CreateFrom(kosmicArray.UnderlyingArray);
                }

                KS.PushError("Attempted to convert an unmarshallable managed type to Variant. Name: '" +
                             obj.GetType().FullName + ".");
                return new kosmic_variant();
            }

            private delegate object? ConvertToSystemObjectFunc(in kosmic_variant managed);

            private static readonly System.Collections.Generic.Dictionary<Type, ConvertToSystemObjectFunc>
                _toSystemObjectFuncByType = new()
                {
                    [typeof(bool)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<bool>(variant),
                    [typeof(char)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<char>(variant),
                    [typeof(sbyte)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<sbyte>(variant),
                    [typeof(short)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<short>(variant),
                    [typeof(int)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<int>(variant),
                    [typeof(long)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<long>(variant),
                    [typeof(byte)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<byte>(variant),
                    [typeof(ushort)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<ushort>(variant),
                    [typeof(uint)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<uint>(variant),
                    [typeof(ulong)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<ulong>(variant),
                    [typeof(float)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<float>(variant),
                    [typeof(double)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<double>(variant),
                    [typeof(Vector2)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<Vector2>(variant),
                    [typeof(Vector2I)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<Vector2I>(variant),
                    [typeof(Rect2)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<Rect2>(variant),
                    [typeof(Rect2I)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<Rect2I>(variant),
                    [typeof(Transform2D)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<Transform2D>(variant),
                    [typeof(Vector3)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<Vector3>(variant),
                    [typeof(Vector3I)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<Vector3I>(variant),
                    [typeof(Basis)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<Basis>(variant),
                    [typeof(Quaternion)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<Quaternion>(variant),
                    [typeof(Transform3D)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<Transform3D>(variant),
                    [typeof(Vector4)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<Vector4>(variant),
                    [typeof(Vector4I)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<Vector4I>(variant),
                    [typeof(Aabb)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<Aabb>(variant),
                    [typeof(Color)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<Color>(variant),
                    [typeof(Plane)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<Plane>(variant),
                    [typeof(Callable)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<Callable>(variant),
                    [typeof(Signal)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<Signal>(variant),
                    [typeof(string)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<string>(variant),
                    [typeof(byte[])] = (in kosmic_variant variant) => VariantUtils.ConvertTo<byte[]>(variant),
                    [typeof(int[])] = (in kosmic_variant variant) => VariantUtils.ConvertTo<int[]>(variant),
                    [typeof(long[])] = (in kosmic_variant variant) => VariantUtils.ConvertTo<long[]>(variant),
                    [typeof(float[])] = (in kosmic_variant variant) => VariantUtils.ConvertTo<float[]>(variant),
                    [typeof(double[])] = (in kosmic_variant variant) => VariantUtils.ConvertTo<double[]>(variant),
                    [typeof(string[])] = (in kosmic_variant variant) => VariantUtils.ConvertTo<string[]>(variant),
                    [typeof(Vector2[])] = (in kosmic_variant variant) => VariantUtils.ConvertTo<Vector2[]>(variant),
                    [typeof(Vector3[])] = (in kosmic_variant variant) => VariantUtils.ConvertTo<Vector3[]>(variant),
                    [typeof(Color[])] = (in kosmic_variant variant) => VariantUtils.ConvertTo<Color[]>(variant),
                    [typeof(StringName[])] =
                        (in kosmic_variant variant) => VariantUtils.ConvertTo<StringName[]>(variant),
                    [typeof(NodePath[])] = (in kosmic_variant variant) => VariantUtils.ConvertTo<NodePath[]>(variant),
                    [typeof(Rid[])] = (in kosmic_variant variant) => VariantUtils.ConvertTo<Rid[]>(variant),
                    [typeof(StringName)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<StringName>(variant),
                    [typeof(NodePath)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<NodePath>(variant),
                    [typeof(Rid)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<Rid>(variant),
                    [typeof(Kosmic.Collections.Dictionary)] = (in kosmic_variant variant) =>
                        VariantUtils.ConvertTo<Kosmic.Collections.Dictionary>(variant),
                    [typeof(Kosmic.Collections.Array)] =
                        (in kosmic_variant variant) => VariantUtils.ConvertTo<Kosmic.Collections.Array>(variant),
                    [typeof(Variant)] = (in kosmic_variant variant) => VariantUtils.ConvertTo<Variant>(variant),
                };

            public static object? ConvertToObjectOfType(in kosmic_variant variant, Type type)
            {
                if (_toSystemObjectFuncByType.TryGetValue(type, out var func))
                    return func(variant);

                if (typeof(KosmicObject).IsAssignableFrom(type))
                    return VariantUtils.ConvertTo<KosmicObject>(variant);

                if (typeof(KosmicObject[]).IsAssignableFrom(type))
                {
                    static KosmicObject[] ConvertToSystemArrayOfKosmicObject(in kosmic_array nativeArray, Type type)
                    {
                        var array = Collections.Array.CreateTakingOwnershipOfDisposableValue(
                            NativeFuncs.kosmicsharp_array_new_copy(nativeArray));

                        int length = array.Count;
                        var ret = (KosmicObject[])Activator.CreateInstance(type, length)!;

                        for (int i = 0; i < length; i++)
                            ret[i] = array[i].AsKosmicObject();

                        return ret;
                    }

                    using var kosmicArray = NativeFuncs.kosmicsharp_variant_as_array(variant);
                    return ConvertToSystemArrayOfKosmicObject(kosmicArray, type);
                }

                if (type.IsEnum)
                {
                    var enumUnderlyingType = type.GetEnumUnderlyingType();

                    switch (Type.GetTypeCode(enumUnderlyingType))
                    {
                        case TypeCode.SByte:
                            return Enum.ToObject(type, VariantUtils.ConvertToInt8(variant));
                        case TypeCode.Int16:
                            return Enum.ToObject(type, VariantUtils.ConvertToInt16(variant));
                        case TypeCode.Int32:
                            return Enum.ToObject(type, VariantUtils.ConvertToInt32(variant));
                        case TypeCode.Int64:
                            return Enum.ToObject(type, VariantUtils.ConvertToInt64(variant));
                        case TypeCode.Byte:
                            return Enum.ToObject(type, VariantUtils.ConvertToUInt8(variant));
                        case TypeCode.UInt16:
                            return Enum.ToObject(type, VariantUtils.ConvertToUInt16(variant));
                        case TypeCode.UInt32:
                            return Enum.ToObject(type, VariantUtils.ConvertToUInt32(variant));
                        case TypeCode.UInt64:
                            return Enum.ToObject(type, VariantUtils.ConvertToUInt64(variant));
                        default:
                        {
                            KS.PushError(
                                "Attempted to convert Variant to enum value of unsupported underlying type. Name: " +
                                type.FullName + " : " + enumUnderlyingType.FullName + ".");
                            return null;
                        }
                    }
                }

                if (type.IsGenericType)
                {
                    var genericTypeDef = type.GetGenericTypeDefinition();

                    if (genericTypeDef == typeof(Kosmic.Collections.Dictionary<,>))
                    {
                        var ctor = type.GetConstructor(new[] { typeof(Kosmic.Collections.Dictionary) });

                        if (ctor == null)
                            throw new InvalidOperationException("Dictionary constructor not found");

                        return ctor.Invoke(new object?[]
                        {
                            VariantUtils.ConvertTo<Kosmic.Collections.Dictionary>(variant)
                        });
                    }

                    if (genericTypeDef == typeof(Kosmic.Collections.Array<>))
                    {
                        var ctor = type.GetConstructor(new[] { typeof(Kosmic.Collections.Array) });

                        if (ctor == null)
                            throw new InvalidOperationException("Array constructor not found");

                        return ctor.Invoke(new object?[]
                        {
                            VariantUtils.ConvertTo<Kosmic.Collections.Array>(variant)
                        });
                    }
                }

                KS.PushError($"Attempted to convert Variant to unsupported type. Name: {type.FullName}.");
                return null;
            }
        }
    }
}
