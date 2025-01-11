using Kosmic;
using Kosmic.NativeInterop;

partial struct OuterClass
{
partial class NestedClass
{
#pragma warning disable CS0109 // Disable warning about redundant 'new' keyword
    /// <summary>
    /// Cached StringNames for the methods contained in this class, for fast lookup.
    /// </summary>
    public new class MethodName : global::Kosmic.RefCounted.MethodName {
        /// <summary>
        /// Cached name for the '_Get' method.
        /// </summary>
        public new static readonly global::Kosmic.StringName @_Get = "_Get";
    }
    /// <summary>
    /// Get the method information for all the methods declared in this class.
    /// This method is used by Godot to register the available methods in the editor.
    /// Do not call this method.
    /// </summary>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    internal new static global::System.Collections.Generic.List<global::Kosmic.Bridge.MethodInfo> GetKosmicMethodList()
    {
        var methods = new global::System.Collections.Generic.List<global::Kosmic.Bridge.MethodInfo>(1);
        methods.Add(new(name: MethodName.@_Get, returnVal: new(type: (global::Kosmic.Variant.Type)0, name: "", hint: (global::Kosmic.PropertyHint)0, hintString: "", usage: (global::Kosmic.PropertyUsageFlags)131078, exported: false), flags: (global::Kosmic.MethodFlags)1, arguments: new() { new(type: (global::Kosmic.Variant.Type)21, name: "property", hint: (global::Kosmic.PropertyHint)0, hintString: "", usage: (global::Kosmic.PropertyUsageFlags)6, exported: false),  }, defaultArguments: null));
        return methods;
    }
#pragma warning restore CS0109
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override bool InvokeKosmicClassMethod(in kosmic_string_name method, NativeVariantPtrArgs args, out kosmic_variant ret)
    {
        if (method == MethodName.@_Get && args.Count == 1) {
            var callRet = @_Get(global::Kosmic.NativeInterop.VariantUtils.ConvertTo<global::Kosmic.StringName>(args[0]));
            ret = global::Kosmic.NativeInterop.VariantUtils.CreateFrom<global::Kosmic.Variant>(callRet);
            return true;
        }
        return base.InvokeKosmicClassMethod(method, args, out ret);
    }
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override bool HasKosmicClassMethod(in kosmic_string_name method)
    {
        if (method == MethodName.@_Get) {
           return true;
        }
        return base.HasKosmicClassMethod(method);
    }
}
}
