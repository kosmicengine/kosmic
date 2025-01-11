using Kosmic;
using Kosmic.NativeInterop;

partial class EventSignals
{
#pragma warning disable CS0109 // Disable warning about redundant 'new' keyword
    /// <summary>
    /// Cached StringNames for the signals contained in this class, for fast lookup.
    /// </summary>
    public new class SignalName : global::Kosmic.KosmicObject.SignalName {
        /// <summary>
        /// Cached name for the 'MySignal' signal.
        /// </summary>
        public new static readonly global::Kosmic.StringName @MySignal = "MySignal";
    }
    /// <summary>
    /// Get the signal information for all the signals declared in this class.
    /// This method is used by Godot to register the available signals in the editor.
    /// Do not call this method.
    /// </summary>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    internal new static global::System.Collections.Generic.List<global::Kosmic.Bridge.MethodInfo> GetKosmicSignalList()
    {
        var signals = new global::System.Collections.Generic.List<global::Kosmic.Bridge.MethodInfo>(1);
        signals.Add(new(name: SignalName.@MySignal, returnVal: new(type: (global::Kosmic.Variant.Type)0, name: "", hint: (global::Kosmic.PropertyHint)0, hintString: "", usage: (global::Kosmic.PropertyUsageFlags)6, exported: false), flags: (global::Kosmic.MethodFlags)1, arguments: new() { new(type: (global::Kosmic.Variant.Type)4, name: "str", hint: (global::Kosmic.PropertyHint)0, hintString: "", usage: (global::Kosmic.PropertyUsageFlags)6, exported: false), new(type: (global::Kosmic.Variant.Type)2, name: "num", hint: (global::Kosmic.PropertyHint)0, hintString: "", usage: (global::Kosmic.PropertyUsageFlags)6, exported: false),  }, defaultArguments: null));
        return signals;
    }
#pragma warning restore CS0109
    private global::EventSignals.MySignalEventHandler backing_MySignal;
    /// <inheritdoc cref="global::EventSignals.MySignalEventHandler"/>
    public event global::EventSignals.MySignalEventHandler @MySignal {
        add => backing_MySignal += value;
        remove => backing_MySignal -= value;
}
    protected void EmitSignalMySignal(string @str, int @num)
    {
        EmitSignal(SignalName.MySignal, @str, @num);
    }
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override void RaiseKosmicClassSignalCallbacks(in kosmic_string_name signal, NativeVariantPtrArgs args)
    {
        if (signal == SignalName.@MySignal && args.Count == 2) {
            backing_MySignal?.Invoke(global::Kosmic.NativeInterop.VariantUtils.ConvertTo<string>(args[0]), global::Kosmic.NativeInterop.VariantUtils.ConvertTo<int>(args[1]));
            return;
        }
        base.RaiseKosmicClassSignalCallbacks(signal, args);
    }
    /// <inheritdoc/>
    [global::System.ComponentModel.EditorBrowsable(global::System.ComponentModel.EditorBrowsableState.Never)]
    protected override bool HasKosmicClassSignal(in kosmic_string_name signal)
    {
        if (signal == SignalName.@MySignal) {
           return true;
        }
        return base.HasKosmicClassSignal(signal);
    }
}
