using System;
using System.Runtime.CompilerServices;
using Kosmic.NativeInterop;

namespace Kosmic;

#nullable enable

// TODO: Disabled because it is a false positive, see https://github.com/dotnet/roslyn-analyzers/issues/6151
#pragma warning disable CA1001 // Types that own disposable fields should be disposable
public partial struct Variant : IDisposable
#pragma warning restore CA1001
{
    internal kosmic_variant.movable NativeVar;
    private object? _obj;
    private Disposer? _disposer;

    private sealed class Disposer : IDisposable
    {
        private kosmic_variant.movable _native;

        private WeakReference<IDisposable>? _weakReferenceToSelf;

        public Disposer(in kosmic_variant.movable nativeVar)
        {
            _native = nativeVar;
            _weakReferenceToSelf = DisposablesTracker.RegisterDisposable(this);
        }

        ~Disposer()
        {
            Dispose(false);
        }

        public void Dispose()
        {
            Dispose(true);
            GC.SuppressFinalize(this);
        }

        private void Dispose(bool disposing)
        {
            _native.DangerousSelfRef.Dispose();

            if (_weakReferenceToSelf != null)
            {
                DisposablesTracker.UnregisterDisposable(_weakReferenceToSelf);
            }
        }
    }

    private Variant(in kosmic_variant nativeVar)
    {
        NativeVar = (kosmic_variant.movable)nativeVar;
        _obj = null;

        switch (nativeVar.Type)
        {
            case Type.Nil:
            case Type.Bool:
            case Type.Int:
            case Type.Float:
            case Type.Vector2:
            case Type.Vector2I:
            case Type.Rect2:
            case Type.Rect2I:
            case Type.Vector3:
            case Type.Vector3I:
            case Type.Vector4:
            case Type.Vector4I:
            case Type.Plane:
            case Type.Quaternion:
            case Type.Color:
            case Type.Rid:
                _disposer = null;
                break;
            default:
            {
                _disposer = new Disposer(NativeVar);
                break;
            }
        }
    }

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    // Explicit name to make it very clear
    public static Variant CreateTakingOwnershipOfDisposableValue(in kosmic_variant nativeValueToOwn) =>
        new(nativeValueToOwn);

    // Explicit name to make it very clear
    public static Variant CreateCopyingBorrowed(in kosmic_variant nativeValueToOwn) =>
        new(NativeFuncs.kosmicsharp_variant_new_copy(nativeValueToOwn));

    /// <summary>
    /// Constructs a new <see cref="Kosmic.NativeInterop.kosmic_variant"/> from this instance.
    /// The caller is responsible of disposing the new instance to avoid memory leaks.
    /// </summary>
    public kosmic_variant CopyNativeVariant() =>
        NativeFuncs.kosmicsharp_variant_new_copy((kosmic_variant)NativeVar);

    public void Dispose()
    {
        _disposer?.Dispose();
        NativeVar = default;
        _obj = null;
    }

    // TODO: Consider renaming Variant.Type to VariantType and this property to Type. VariantType would also avoid ambiguity with System.Type.
    public Type VariantType => NativeVar.DangerousSelfRef.Type;

    public override string ToString() => AsString();

    public object? Obj =>
        _obj ??= NativeVar.DangerousSelfRef.Type switch
        {
            Type.Bool => AsBool(),
            Type.Int => AsInt64(),
            Type.Float => AsDouble(),
            Type.String => AsString(),
            Type.Vector2 => AsVector2(),
            Type.Vector2I => AsVector2I(),
            Type.Rect2 => AsRect2(),
            Type.Rect2I => AsRect2I(),
            Type.Vector3 => AsVector3(),
            Type.Vector3I => AsVector3I(),
            Type.Transform2D => AsTransform2D(),
            Type.Vector4 => AsVector4(),
            Type.Vector4I => AsVector4I(),
            Type.Plane => AsPlane(),
            Type.Quaternion => AsQuaternion(),
            Type.Aabb => AsAabb(),
            Type.Basis => AsBasis(),
            Type.Transform3D => AsTransform3D(),
            Type.Projection => AsProjection(),
            Type.Color => AsColor(),
            Type.StringName => AsStringName(),
            Type.NodePath => AsNodePath(),
            Type.Rid => AsRid(),
            Type.Object => AsKosmicObject(),
            Type.Callable => AsCallable(),
            Type.Signal => AsSignal(),
            Type.Dictionary => AsKosmicDictionary(),
            Type.Array => AsGodotArray(),
            Type.PackedByteArray => AsByteArray(),
            Type.PackedInt32Array => AsInt32Array(),
            Type.PackedInt64Array => AsInt64Array(),
            Type.PackedFloat32Array => AsFloat32Array(),
            Type.PackedFloat64Array => AsFloat64Array(),
            Type.PackedStringArray => AsStringArray(),
            Type.PackedVector2Array => AsVector2Array(),
            Type.PackedVector3Array => AsVector3Array(),
            Type.PackedVector4Array => AsVector4Array(),
            Type.PackedColorArray => AsColorArray(),
            Type.Nil => null,
            Type.Max or _ =>
                throw new InvalidOperationException($"Invalid Variant type: {NativeVar.DangerousSelfRef.Type}"),
        };

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant From<[MustBeVariant] T>(in T from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFrom(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public T As<[MustBeVariant] T>() =>
        VariantUtils.ConvertTo<T>(NativeVar.DangerousSelfRef);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public bool AsBool() =>
        VariantUtils.ConvertToBool((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public char AsChar() =>
        (char)VariantUtils.ConvertToUInt16((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public sbyte AsSByte() =>
        VariantUtils.ConvertToInt8((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public short AsInt16() =>
        VariantUtils.ConvertToInt16((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public int AsInt32() =>
        VariantUtils.ConvertToInt32((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public long AsInt64() =>
        VariantUtils.ConvertToInt64((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public byte AsByte() =>
        VariantUtils.ConvertToUInt8((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public ushort AsUInt16() =>
        VariantUtils.ConvertToUInt16((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public uint AsUInt32() =>
        VariantUtils.ConvertToUInt32((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public ulong AsUInt64() =>
        VariantUtils.ConvertToUInt64((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public float AsSingle() =>
        VariantUtils.ConvertToFloat32((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public double AsDouble() =>
        VariantUtils.ConvertToFloat64((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public string AsString() =>
        VariantUtils.ConvertToString((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Vector2 AsVector2() =>
        VariantUtils.ConvertToVector2((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Vector2I AsVector2I() =>
        VariantUtils.ConvertToVector2I((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Rect2 AsRect2() =>
        VariantUtils.ConvertToRect2((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Rect2I AsRect2I() =>
        VariantUtils.ConvertToRect2I((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Transform2D AsTransform2D() =>
        VariantUtils.ConvertToTransform2D((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Vector3 AsVector3() =>
        VariantUtils.ConvertToVector3((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Vector3I AsVector3I() =>
        VariantUtils.ConvertToVector3I((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Basis AsBasis() =>
        VariantUtils.ConvertToBasis((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Quaternion AsQuaternion() =>
        VariantUtils.ConvertToQuaternion((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Transform3D AsTransform3D() =>
        VariantUtils.ConvertToTransform3D((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Vector4 AsVector4() =>
        VariantUtils.ConvertToVector4((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Vector4I AsVector4I() =>
        VariantUtils.ConvertToVector4I((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Projection AsProjection() =>
        VariantUtils.ConvertToProjection((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Aabb AsAabb() =>
        VariantUtils.ConvertToAabb((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Color AsColor() =>
        VariantUtils.ConvertToColor((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Plane AsPlane() =>
        VariantUtils.ConvertToPlane((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Callable AsCallable() =>
        VariantUtils.ConvertToCallable((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Signal AsSignal() =>
        VariantUtils.ConvertToSignal((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public byte[] AsByteArray() =>
        VariantUtils.ConvertAsPackedByteArrayToSystemArray((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public int[] AsInt32Array() =>
        VariantUtils.ConvertAsPackedInt32ArrayToSystemArray((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public long[] AsInt64Array() =>
        VariantUtils.ConvertAsPackedInt64ArrayToSystemArray((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public float[] AsFloat32Array() =>
        VariantUtils.ConvertAsPackedFloat32ArrayToSystemArray((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public double[] AsFloat64Array() =>
        VariantUtils.ConvertAsPackedFloat64ArrayToSystemArray((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public string[] AsStringArray() =>
        VariantUtils.ConvertAsPackedStringArrayToSystemArray((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Vector2[] AsVector2Array() =>
        VariantUtils.ConvertAsPackedVector2ArrayToSystemArray((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Vector3[] AsVector3Array() =>
        VariantUtils.ConvertAsPackedVector3ArrayToSystemArray((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Vector4[] AsVector4Array() =>
        VariantUtils.ConvertAsPackedVector4ArrayToSystemArray((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Color[] AsColorArray() =>
        VariantUtils.ConvertAsPackedColorArrayToSystemArray((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public T[] AsKosmicObjectArray<T>()
        where T : KosmicObject =>
        VariantUtils.ConvertToSystemArrayOfKosmicObject<T>((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Collections.Dictionary<TKey, TValue> AsKosmicDictionary<[MustBeVariant] TKey, [MustBeVariant] TValue>() =>
        VariantUtils.ConvertToDictionary<TKey, TValue>((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Collections.Array<T> AsGodotArray<[MustBeVariant] T>() =>
        VariantUtils.ConvertToArray<T>((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public StringName[] AsSystemArrayOfStringName() =>
        VariantUtils.ConvertToSystemArrayOfStringName((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public NodePath[] AsSystemArrayOfNodePath() =>
        VariantUtils.ConvertToSystemArrayOfNodePath((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Rid[] AsSystemArrayOfRid() =>
        VariantUtils.ConvertToSystemArrayOfRid((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public KosmicObject AsKosmicObject() =>
        VariantUtils.ConvertToKosmicObject((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public StringName AsStringName() =>
        VariantUtils.ConvertToStringName((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public NodePath AsNodePath() =>
        VariantUtils.ConvertToNodePath((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Rid AsRid() =>
        VariantUtils.ConvertToRid((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Collections.Dictionary AsKosmicDictionary() =>
        VariantUtils.ConvertToDictionary((kosmic_variant)NativeVar);

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public Collections.Array AsGodotArray() =>
        VariantUtils.ConvertToArray((kosmic_variant)NativeVar);

    // Explicit conversion operators to supported types

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator bool(Variant from) => from.AsBool();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator char(Variant from) => from.AsChar();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator sbyte(Variant from) => from.AsSByte();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator short(Variant from) => from.AsInt16();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator int(Variant from) => from.AsInt32();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator long(Variant from) => from.AsInt64();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator byte(Variant from) => from.AsByte();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator ushort(Variant from) => from.AsUInt16();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator uint(Variant from) => from.AsUInt32();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator ulong(Variant from) => from.AsUInt64();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator float(Variant from) => from.AsSingle();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator double(Variant from) => from.AsDouble();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator string(Variant from) => from.AsString();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator Vector2(Variant from) => from.AsVector2();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator Vector2I(Variant from) => from.AsVector2I();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator Rect2(Variant from) => from.AsRect2();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator Rect2I(Variant from) => from.AsRect2I();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator Transform2D(Variant from) => from.AsTransform2D();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator Vector3(Variant from) => from.AsVector3();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator Vector3I(Variant from) => from.AsVector3I();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator Basis(Variant from) => from.AsBasis();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator Quaternion(Variant from) => from.AsQuaternion();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator Transform3D(Variant from) => from.AsTransform3D();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator Vector4(Variant from) => from.AsVector4();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator Vector4I(Variant from) => from.AsVector4I();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator Projection(Variant from) => from.AsProjection();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator Aabb(Variant from) => from.AsAabb();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator Color(Variant from) => from.AsColor();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator Plane(Variant from) => from.AsPlane();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator Callable(Variant from) => from.AsCallable();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator Signal(Variant from) => from.AsSignal();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator byte[](Variant from) => from.AsByteArray();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator int[](Variant from) => from.AsInt32Array();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator long[](Variant from) => from.AsInt64Array();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator float[](Variant from) => from.AsFloat32Array();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator double[](Variant from) => from.AsFloat64Array();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator string[](Variant from) => from.AsStringArray();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator Vector2[](Variant from) => from.AsVector2Array();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator Vector3[](Variant from) => from.AsVector3Array();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator Vector4[](Variant from) => from.AsVector4Array();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator Color[](Variant from) => from.AsColorArray();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator StringName[](Variant from) => from.AsSystemArrayOfStringName();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator NodePath[](Variant from) => from.AsSystemArrayOfNodePath();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator Rid[](Variant from) => from.AsSystemArrayOfRid();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator KosmicObject(Variant from) => from.AsKosmicObject();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator StringName(Variant from) => from.AsStringName();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator NodePath(Variant from) => from.AsNodePath();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator Rid(Variant from) => from.AsRid();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator Collections.Dictionary(Variant from) => from.AsKosmicDictionary();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static explicit operator Collections.Array(Variant from) => from.AsGodotArray();

    // While we provide implicit conversion operators, normal methods are still needed for
    // casts that are not done implicitly (e.g.: raw array to Span, enum to integer, etc).

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(bool from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(char from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(sbyte from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(short from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(int from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(long from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(byte from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(ushort from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(uint from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(ulong from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(float from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(double from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(string from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Vector2 from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Vector2I from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Rect2 from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Rect2I from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Transform2D from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Vector3 from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Vector3I from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Basis from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Quaternion from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Transform3D from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Vector4 from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Vector4I from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Projection from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Aabb from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Color from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Plane from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Callable from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Signal from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Span<byte> from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Span<int> from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Span<long> from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Span<float> from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Span<double> from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Span<string> from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Span<Vector2> from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Span<Vector3> from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Span<Vector4> from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Span<Color> from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(KosmicObject[] from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom<[MustBeVariant] TKey, [MustBeVariant] TValue>(Collections.Dictionary<TKey, TValue> from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromDictionary(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom<[MustBeVariant] T>(Collections.Array<T> from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromArray(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Span<StringName> from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Span<NodePath> from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Span<Rid> from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(KosmicObject from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(StringName from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(NodePath from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Rid from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Collections.Dictionary from) => from;

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static Variant CreateFrom(Collections.Array from) => from;

    // Implicit conversion operators

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(bool from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromBool(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(char from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromInt(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(sbyte from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromInt(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(short from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromInt(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(int from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromInt(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(long from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromInt(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(byte from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromInt(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(ushort from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromInt(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(uint from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromInt(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(ulong from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromInt(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(float from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromFloat(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(double from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromFloat(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(string from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromString(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Vector2 from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromVector2(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Vector2I from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromVector2I(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Rect2 from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromRect2(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Rect2I from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromRect2I(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Transform2D from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromTransform2D(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Vector3 from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromVector3(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Vector3I from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromVector3I(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Basis from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromBasis(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Quaternion from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromQuaternion(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Transform3D from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromTransform3D(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Vector4 from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromVector4(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Vector4I from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromVector4I(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Projection from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromProjection(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Aabb from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromAabb(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Color from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromColor(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Plane from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromPlane(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Callable from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromCallable(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Signal from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromSignal(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(byte[] from) =>
        (Variant)from.AsSpan();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(int[] from) =>
        (Variant)from.AsSpan();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(long[] from) =>
        (Variant)from.AsSpan();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(float[] from) =>
        (Variant)from.AsSpan();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(double[] from) =>
        (Variant)from.AsSpan();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(string[] from) =>
        (Variant)from.AsSpan();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Vector2[] from) =>
        (Variant)from.AsSpan();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Vector3[] from) =>
        (Variant)from.AsSpan();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Vector4[] from) =>
        (Variant)from.AsSpan();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Color[] from) =>
        (Variant)from.AsSpan();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(KosmicObject[] from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromSystemArrayOfKosmicObject(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(StringName[] from) =>
        (Variant)from.AsSpan();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(NodePath[] from) =>
        (Variant)from.AsSpan();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Rid[] from) =>
        (Variant)from.AsSpan();

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Span<byte> from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromPackedByteArray(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Span<int> from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromPackedInt32Array(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Span<long> from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromPackedInt64Array(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Span<float> from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromPackedFloat32Array(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Span<double> from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromPackedFloat64Array(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Span<string> from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromPackedStringArray(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Span<Vector2> from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromPackedVector2Array(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Span<Vector3> from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromPackedVector3Array(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Span<Vector4> from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromPackedVector4Array(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Span<Color> from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromPackedColorArray(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Span<StringName> from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromSystemArrayOfStringName(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Span<NodePath> from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromSystemArrayOfNodePath(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Span<Rid> from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromSystemArrayOfRid(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(KosmicObject from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromKosmicObject(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(StringName from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromStringName(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(NodePath from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromNodePath(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Rid from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromRid(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Collections.Dictionary from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromDictionary(from));

    [MethodImpl(MethodImplOptions.AggressiveInlining)]
    public static implicit operator Variant(Collections.Array from) =>
        CreateTakingOwnershipOfDisposableValue(VariantUtils.CreateFromArray(from));
}
