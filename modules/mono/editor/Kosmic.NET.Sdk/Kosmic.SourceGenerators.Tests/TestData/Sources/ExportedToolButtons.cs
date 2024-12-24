using Godot;
using System;

[Tool]
public partial class ExportedToolButtons : KosmicObject
{
    [ExportToolButton("Click me!")]
    public Callable MyButton1 => Callable.From(() => { KS.Print("Clicked MyButton1!"); });

    [ExportToolButton("Click me!", Icon = "ColorRect")]
    public Callable MyButton2 => Callable.From(() => { KS.Print("Clicked MyButton2!"); });
}
