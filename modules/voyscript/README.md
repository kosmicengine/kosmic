# Basic VoyScript module architecture
This provides some basic information in how VoyScript is implemented and integrates with the rest of the engine. You can learn more about VoyScript in the [documentation](https://docs.kosmicengine.org/en/latest/tutorials/scripting/voyscript/index.html). It describes the syntax and user facing systems and concepts, and can be used as a reference for what user expectations are.


## General design

VoyScript is:

1. A [gradually typed](https://en.wikipedia.org/wiki/Gradual_typing) language. Type hints are optional and help with static analysis and performance. However, typed code must easily interoperate with untyped code.
2. A tightly designed language. Features are added because they are _needed_, and not because they can be added or are interesting to develop.
3. Primarily an interpreted scripting language: it is compiled to VoyScript byte code and interpreted in a VoyScript virtual machine. It is meant to be easy to use and develop gameplay in. It is not meant for CPU-intensive algorithms or data processing, and is not optimized for it. For that, [C#](https://docs.kosmicengine.org/en/stable/tutorials/scripting/c_sharp/c_sharp_basics.html) or [KSExtension](https://docs.kosmicengine.org/en/stable/tutorials/scripting/ksextension/what_is_ksextension.html) may be used.


## Integration into Kosmic

VoyScript is integrated into Kosmic as a module. Since modules are optional, this means that Kosmic may be built without VoyScript and work perfectly fine without it!

The VoyScript module interfaces with Kosmic's codebase by inheriting from the engine's scripting-related classes. New languages inherit from [`ScriptLanguage`](/core/object/script_language.h), and are registered in Kosmic's [`ScriptServer`](/core/object/script_language.h). Scripts, referring to a file containing code, are represented in the engine by the `Script` class. Instances of that script, which are used at runtime when actually executing the code, inherit from [`ScriptInstance`](/core/object/script_instance.h).

To access Kosmic's internal classes, VoyScript uses [`ClassDB`](/core/object/class_db.h). `ClassDB` is where Kosmic registers classes, methods and properties that it wants exposed to its scripting system. This is how VoyScript understands that `Node2D` is a class it can use, and that it has a `get_parent()` method.

[Built-in VoyScript methods](https://docs.kosmicengine.org/en/latest/classes/class_@voyscript.html#methods) are defined and exported by [`VoyScriptUtilityFunctions`](voyscript_utility_functions.h), whereas [global scope methods](https://docs.kosmicengine.org/en/latest/classes/class_%2540globalscope.html) are registered in [`Variant::_register_variant_utility_functions()`](/core/variant/variant_utility.cpp).


## Compilation

Scripts can be at different stages of compilation. The process isn't entirely linear, but consists of this general order: tokenizing, parsing, analyzing, and finally compiling. This process is the same for scripts in the editor and scripts in an exported game. Scripts are stored as text files in both cases, and the compilation process must happen in full before the bytecode can be passed to the virtual machine and run.

The main class of the VoyScript module is the [`VoyScript`](voyscript.h) class, which represents a class defined in VoyScript. Each `.voy` file is called a _class file_ because it implicitly defines a class in VoyScript, and thus results in an associated `VoyScript` object. However, VoyScript classes may define [_inner classes_](https://docs.kosmicengine.org/en/stable/tutorials/scripting/voyscript/voyscript_basics.html#inner-classes), and those are also represented by further `VoyScript` objects, even though they are not in files of their own.

The `VoyScript` class contains all the information related to the corresponding VoyScript class: its name and path, its members like variables, functions, symbols, signals, implicit methods like initializers, etc. This is the main class that the compilation step deals with.

A secondary class is `VoyScriptInstance`, defined in the same file, containing _runtime_ information for an instance of a `VoyScript`, and is more related to the execution of a script by the virtual machine.


### Loading source code

This mostly happens by calling `VoyScript::load_source_code()` on a `VoyScript` object. Parsing only requires a `String`, so it is entirely possible to parse a script without a `VoyScript` object!


### Tokenizing (see [`VoyScriptTokenizer`](voyscript_tokenizer.h))

Tokenizing is the process of converting the source code `String` into a sequence of tokens, which represent language constructs (such as `for` or `if`), identifiers, literals, etc. This happens almost exclusively during the parsing process, which asks for the next token in order to make sense of the source code. The tokenizer is only used outside of the parsing process in very rare exceptions.


### Parsing (see [`VoyScriptParser`](voyscript_parser.h))

The parser takes a sequence of tokens and builds [the abstract syntax tree (AST)](https://en.wikipedia.org/wiki/Abstract_syntax_tree) of the VoyScript program. The AST is used in the analyzing and compilation steps, and the source code `String` and sequence of tokens are discarded. The AST-building process finds syntax errors in a VoyScript program and reports them to the user.

The parser class also defines all the possible nodes of the AST as subtypes of `VoyScriptParser::Node`, not to be confused with Kosmic's scene tree `Node`. For example, `VoyScriptParser::IfNode` has two children nodes, one for the code in the `if` block, and one for the code in the `else` block. A `VoyScriptParser::FunctionNode` contains children nodes for its name, parameters, return type, body, etc. The parser also defines typechecking data structures like `VoyScriptParser::Datatype`.

The parser was [intentionally designed](https://kosmicengine.org/article/voyscript-progress-report-writing-new-parser/#less-lookahead) with a look-ahead of a single token. This means that the parser only has access to the current token and the previous token (or, if you prefer, the current token and the next token). This parsing limitation ensures that VoyScript will remain syntactically simple and accessible, and that the parsing process cannot become overly complex.


### Analysis and typechecking (see [`VoyScriptAnalyzer`](voyscript_analyzer.h))

The analyzer takes in the AST of a program and verifies that "everything checks out". For example, when analyzing a method call with three parameters, it will check whether the function definition also contains three parameters. If the code is typed, it will check that argument and parameter types are compatible.

There are two types of functions in the analyzer: `reduce` functions and `resolve` functions. Their parameters always include the AST node that they are attempting to reduce or resolve.
- The `reduce` functions work on VoyScript expressions, which return values, and thus their main goal is to populate the `VoyScriptParser::Datatype` of the underlying AST node. The datatype is then used to typecheck code that depends on this expression, and gives the compiler necessary information to generate appropriate, safe, and optimized bytecode.
For example, function calls are handled with `reduce_call()`, which must figure out what function is being called and check that the passed arguments match the function's parameters. The type of the underlying `CallNode` will be the return type of the function.
Another example is `reduce_identifier()`, which does _a lot_ of work: given the string of its `IdentifierNode`, it must figure out what that identifier refers to. It could be a local variable, class name, global or class function, function parameter, class or superclass member, or any number of other things. It has to check many different places to find this information!
A secondary goal of the `reduce` functions is to perform [constant folding](https://en.wikipedia.org/wiki/Constant_folding): to determine whether an expression is constant, and if it is, compute its _reduced value_ at this time so it does not need to be computed over and over at runtime!
- The resolve functions work on AST nodes that represent statements, and don't necessarily have values. Their goal is to do work related to program control flow, resolve their child AST nodes, deal with scoping, etc. One of the simplest examples is `resolve_if()`, which reduces the `if` condition, then resolves the `if` body and `else` body if it exists.
The `resolve_for()` function does more work than simply resolving its code block. With `for i in range(10)`, for example, it must also declare and type the new variable `i` within the scope of its code block, as well as make sure `range(10)` is iterable, among other things.
To understand classes and inheritance without introducing cyclic dependency problems that would come from immediate full class code analysis, the analyzer often asks only for class _interfaces_: it needs to know what member variables and methods exist as well as their types, but no more.
This is done through `resolve_class_interface()`, which populates `ClassNode`'s `Datatype` with that information. It first checks for superclass information with `resolve_class_inheritance()`, then populates its member information by calling `resolve_class_member()` on each member. Since this step is only about the class _interface_, methods are resolved with `resolve_function_signature()`, which gets all relevant typing information without resolving the function body!
The remaining steps of resolution, including member variable initialization code, method code, etc, can happen at a later time.

In fully untyped code, very little static analysis is possible. For example, the analyzer cannot know whether `my_var.some_member` exists when it does not know the type of `my_var`. Therefore, it cannot emit a warning or error because `some_member` _could_ exist - or it could not. The analyzer must trust the programmer. If an error does occur, it will be at runtime.
However, VoyScript is gradually typed, so all of these analyses must work when parts of the code are typed and others untyped. Static analysis in a gradually typed language is a best-effort situation: suppose there is a typed variable `var x : int`, and an untyped `var y = "some string"`. We can obviously tell this isn't going to work, but the analyzer will accept the assignment `x = y` without warnings or errors: it only knows that `y` is untyped and can therefore be anything, including the `int` that `x` expects. It must once again trust the programmer to have written code that works. In this instance, the code will error at runtime.
In both these cases, the analyzer handles the uncertainty of untyped code by calling `mark_node_unsafe()` on the respective AST node. This means it didn't have enough information to know whether the code was fully safe or necessarily wrong. Lines with unsafe AST nodes are represented by gray line numbers in the VoyScript editor. Green line numbers indicate a line of code without any unsafe nodes.

This analysis step is also where dependencies are introduced and that information stored for use later. If class `A` extends class `B` or contains a member with type `B` from some other script file, then the analyzer will attempt to load that second script. If `B` contains references to `A`, then a _cyclic_ dependency is introduced. This is OK in many cases, but impossible to resolve in others.

Clearly, the analyzer is where a lot of the "magic" happens! It determines what constitutes proper code that can actually be compiled, and provides as many safety guarantees as possible with the typing information it is provided with. The more typed the code, the safer and more optimized it will be!


#### Cyclic dependencies and member resolution

Cyclic dependencies from inheritance (`A extends B, B extends A`) are not supported in any programming language. Other cyclic dependencies are supported, such as `A extends B` and `B` uses, contains, or preloads, members of type `A`.

To see why cyclic dependencies are complicated, suppose there is one between classes `A <-> B`. Partially through the analysis of `A`, we will need information about `B`, and therefore trigger its analysis. However, the analysis of `B` will eventually need information from `A`, which is incomplete because we never finished analyzing it. This would result in members not being found when they actually exist!

VoyScript supports cyclic dependencies due to a few features of the analyzer:

1. Class interface resolution: when analyzing code of class `A` that depends on some other class `B`, we don't need to resolve the _code_ of `B` (its member initializers, function code, etc). We only need to know what members and methods the class has, as well as their types. These are the only things one class can use to work with, or _interface_ with, another. Because of inheritance, a class's interface depends on its superclass as well, so recursive interface resolution is needed. More details can be found in `VoyScriptAnalyzer::resolve_class_interface()`.
2. Out of order member resolution: the analyzer may not even need an entire class interface to be resolved in order to figure out a specific type! For example, if class `A` contains code that references `B.is_alive`, then the analyzer doesn't need to immediately resolve `B`'s entire interface. It may simply check whether `is_alive` exists in `B`, and reduce it for its type information, on-demand.
A fundamental cyclic dependency problem occurs when the types of two different member variables are mutually dependent. This is commonly checked by a pattern that declares a temporary datatype with `VoyScriptParser::DataType resolving_datatype;`, followed by `resolving_datatype.kind = VoyScriptParser::DataType::RESOLVING;`. If the analyzer attempts to resolve a member on-demand that is already tagged as resolving, then a cyclic dependency problem has been found and can be reported.


### Compiling (see [`VoyScriptCompiler`](voyscript_compiler.h))

Compiling is the final step in making a VoyScript executable in the [virtual machine](voyscript_vm.h) (VM). The compiler takes a `VoyScript` object and an AST, and uses another class, [`VoyScriptByteCodeGenerator`](voyscript_byte_codegen.h), to generate bytecode corresponding to the class. In doing this, it creates the objects that the VM understands how to run, like [`VoyScriptFunction`](voyscript_function.h), and completes a few extra tasks needed for compilation, such as populating runtime class member information.

Importantly, the compilation process of a class, specifically the `VoyScriptCompiler::_compile_class()` method, _cannot_ depend on information obtained by calling `VoyScriptCompiler::_compile_class()` on another class, for the same cyclic dependency reasons explained in the previous section.
Any information that can only be obtained or populated during the compilation step, when `VoyScript` objects become available, must be handled before `VoyScriptCompiler::_compile_class()` is called. This process is centralized in `VoyScriptCompiler::_prepare_compilation()` which works as the compile-time equivalent of `VoyScriptAnalyzer::resolve_class_interface()`: it populates a `VoyScript`'s "interface" exclusively with information from the analysis step, and without processing other external classes. This information may then be referenced by other classes without introducing problematic cycles.

The more typing information a VoyScript has, the more optimized the compiled bytecode can be. For example, if `my_var` is untyped, the bytecode for `my_var.some_member` will need to go through several layers of indirection to figure out the type of `my_var` at runtime, and from there determine how to obtain `some_member`. This varies depending on whether `my_var` is a dictionary, a script, or a native class. If the type of `my_var` was known at compile time, the bytecode can directly call the type-specific method for obtaining a member.
Similar optimizations are possible for `my_var.some_func()`. With untyped VoyScript, the VM will need to resolve `my_var`'s type at runtime, then, depending on the type, use different methods to resolve the function and call it. When the function is fully resolved during static analysis, native function pointers or VoyScript function objects can be compiled into the bytecode and directly called by the VM, removing several layers of indirection.

Typed code is safer code and faster code!


## Loading scripts

VoyScripts can be loaded in a couple of different ways. The main method, used almost everywhere in the engine, is to load scripts through the `ResourceLoader` singleton. In this way, VoyScripts are resources like any others: `ResourceLoader::load()` will simply reroute to `ResourceFormatLoaderVoyScript::load()`, found in `voyscript.h/cpp`(voyscript.h). This generates a VoyScript object which is compiled and ready to use.

The other method is to manually load the source code, then pass it to a parser, then to an analyzer and then to a compiler. The previous approach does this behind the scenes, alongside some smart caching of scripts and other functionalities. It is used in the [VoyScript test runner infrastructure](tests/voyscript_test_runner.h).


### Full and shallow scripts

The `ResourceFormatLoaderVoyScript::load()` method simply calls `VoyScriptCache::get_full_script()`. The [`VoyScriptCache`](voyscript_cache.h) is, as it sounds, a cache for VoyScripts. Its two main methods, `get_shallow_script()` and `get_full_script()`, get and cache, respectively, scripts that have been merely parsed, and scripts which have been statically analyzed and fully compiled. Another internal class, `VoyScriptParserRef`, found in the same file, provides even more granularity over the different steps of the parsing process, and is used extensively in the analyzer.

Shallow, or "just parsed" scripts, provide information such as defined classes, class members, and so forth. This is sufficient for many purposes, like obtaining a class interface or checking whether a member exists on a specific class. Full scripts, on the other hand, have been analyzed and compiled and are ready to use.

The distinction between full and shallow scripts is very important, as shallow scripts cannot create cyclic dependency problems, whereas full scripts can. The analyzer, for example, never asks for full scripts. Choosing when to request a shallow vs a full script is an important but subtle decision.

In practice, full scripts are simply scripts where `VoyScript::reload()` has been called. This critical function is the primary way in which scripts get compiled in Kosmic, and essentially does all the compilation steps covered so far in order. Whenever a script is loaded, or updated and reloaded in Kosmic, it will end up going through `VoyScript::reload()`, except in very rare circumstances like the test runner. It is an excellent place to start reading and understanding the VoyScript module!


## Special types of scripts

Certain types of VoyScripts behave slightly differently. For example, autoloads are loaded with `ResourceLoader::load()` during `Main::start()`, very soon after Kosmic is launched. Many systems aren't initialized at that time, so error reporting is often significantly reduced and may not even show up in the editor.

Tool scripts, declared with the `@tool` annotation on a VoyScript file, run in the editor itself as opposed to just when the game is launched. This leads to a significant increase in complexity, as many things that can be changed in the editor may affect a currently executing tool script.


## Other

There are many other classes in the VoyScript module. Here is a brief overview of some of them:

- Declaration of VoyScript warnings in [`VoyScriptWarning`](voyscript_warning.h).
- [`VoyScriptFunction`](voyscript_function.h), which represents an executable VoyScript function. The relevant file contains both static as well as runtime information.
- The [virtual machine](voyscript_vm.cpp) is essentially defined as calling `VoyScriptFunction::call()`.
- Editor-related functions can be found in parts of `VoyScriptLanguage`, originally declared in [`voyscript.h`](voyscript.h) but defined in [`voyscript_editor.cpp`](voyscript_editor.cpp). Code highlighting can be found in [`VoyScriptSyntaxHighlighter`](editor/voyscript_highlighter.h).
- VoyScript decompilation is found in [`voyscript_disassembler.cpp`](voyscript_disassembler.h), defined as `VoyScriptFunction::disassemble()`.
- Documentation generation from VoyScript comments in [`VoyScriptDocGen`](editor/voyscript_docgen.h)
