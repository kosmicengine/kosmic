/**************************************************************************/
/*  gd_mono_cache.h                                                       */
/**************************************************************************/
/*                         This file is part of:                          */
/*                             KOSMIC ENGINE                              */
/*                        https://kosmicengine.org                        */
/**************************************************************************/
/* Copyright (c) 2024-present Kosmic Engine contributors (see AUTHORS.md).*/
/* Copyright (c) 2014-2024 Godot Engine contributors (see AUTHORS.md).    */
/* Copyright (c) 2007-2014 Juan Linietsky, Ariel Manzur.                  */
/*                                                                        */
/* Permission is hereby granted, free of charge, to any person obtaining  */
/* a copy of this software and associated documentation files (the        */
/* "Software"), to deal in the Software without restriction, including    */
/* without limitation the rights to use, copy, modify, merge, publish,    */
/* distribute, sublicense, and/or sell copies of the Software, and to     */
/* permit persons to whom the Software is furnished to do so, subject to  */
/* the following conditions:                                              */
/*                                                                        */
/* The above copyright notice and this permission notice shall be         */
/* included in all copies or substantial portions of the Software.        */
/*                                                                        */
/* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,        */
/* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF     */
/* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. */
/* IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY   */
/* CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,   */
/* TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE      */
/* SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                 */
/**************************************************************************/

#ifndef KS_MONO_CACHE_H
#define KS_MONO_CACHE_H

#include "../csharp_script.h"
#include "../interop_types.h"
#include "../mono_gc_handle.h"

#include "core/object/object.h"
#include "core/string/string_name.h"
#include "core/string/ustring.h"
#include "core/variant/callable.h"
#include "core/variant/dictionary.h"
#include "core/variant/variant.h"

#include <stdint.h>

class CSharpScript;

namespace KSMonoCache {

#ifndef KS_CLR_STDCALL
#ifdef WIN32
#define KS_CLR_STDCALL __stdcall
#else
#define KS_CLR_STDCALL
#endif
#endif

struct kosmicsharp_property_info {
	kosmic_string_name name; // Not owned
	kosmic_string hint_string;
	Variant::Type type;
	PropertyHint hint;
	PropertyUsageFlags usage;
	bool exported;
};

struct kosmicsharp_property_def_val_pair {
	kosmic_string_name name; // Not owned
	kosmic_variant value;
};

struct ManagedCallbacks {
	using Callback_ScriptManagerBridge_GetPropertyInfoList_Add = void(KS_CLR_STDCALL *)(CSharpScript *p_script, const String *, void *p_props, int32_t p_count);
	using Callback_ScriptManagerBridge_GetPropertyDefaultValues_Add = void(KS_CLR_STDCALL *)(CSharpScript *p_script, void *p_def_vals, int32_t p_count);

	using FuncSignalAwaiter_SignalCallback = void(KS_CLR_STDCALL *)(GCHandleIntPtr, const Variant **, int32_t, bool *);
	using FuncDelegateUtils_InvokeWithVariantArgs = void(KS_CLR_STDCALL *)(GCHandleIntPtr, void *, const Variant **, int32_t, const Variant *);
	using FuncDelegateUtils_DelegateEquals = bool(KS_CLR_STDCALL *)(GCHandleIntPtr, GCHandleIntPtr);
	using FuncDelegateUtils_DelegateHash = int32_t(KS_CLR_STDCALL *)(GCHandleIntPtr);
	using FuncDelegateUtils_GetArgumentCount = int32_t(KS_CLR_STDCALL *)(GCHandleIntPtr, bool *);
	using FuncDelegateUtils_TrySerializeDelegateWithGCHandle = bool(KS_CLR_STDCALL *)(GCHandleIntPtr, const Array *);
	using FuncDelegateUtils_TryDeserializeDelegateWithGCHandle = bool(KS_CLR_STDCALL *)(const Array *, GCHandleIntPtr *);
	using FuncScriptManagerBridge_FrameCallback = void(KS_CLR_STDCALL *)();
	using FuncScriptManagerBridge_CreateManagedForKosmicObjectBinding = GCHandleIntPtr(KS_CLR_STDCALL *)(const StringName *, Object *);
	using FuncScriptManagerBridge_CreateManagedForKosmicObjectScriptInstance = bool(KS_CLR_STDCALL *)(const CSharpScript *, Object *, const Variant **, int32_t);
	using FuncScriptManagerBridge_GetScriptNativeName = void(KS_CLR_STDCALL *)(const CSharpScript *, StringName *);
	using FuncScriptManagerBridge_GetGlobalClassName = void(KS_CLR_STDCALL *)(const String *, String *, String *, String *);
	using FuncScriptManagerBridge_SetKosmicObjectPtr = void(KS_CLR_STDCALL *)(GCHandleIntPtr, Object *);
	using FuncScriptManagerBridge_RaiseEventSignal = void(KS_CLR_STDCALL *)(GCHandleIntPtr, const StringName *, const Variant **, int32_t, bool *);
	using FuncScriptManagerBridge_ScriptIsOrInherits = bool(KS_CLR_STDCALL *)(const CSharpScript *, const CSharpScript *);
	using FuncScriptManagerBridge_AddScriptBridge = bool(KS_CLR_STDCALL *)(const CSharpScript *, const String *);
	using FuncScriptManagerBridge_GetOrCreateScriptBridgeForPath = void(KS_CLR_STDCALL *)(const String *, Ref<CSharpScript> *);
	using FuncScriptManagerBridge_RemoveScriptBridge = void(KS_CLR_STDCALL *)(const CSharpScript *);
	using FuncScriptManagerBridge_TryReloadRegisteredScriptWithClass = bool(KS_CLR_STDCALL *)(const CSharpScript *);
	using FuncScriptManagerBridge_UpdateScriptClassInfo = void(KS_CLR_STDCALL *)(const CSharpScript *, CSharpScript::TypeInfo *, Array *, Dictionary *, Dictionary *, Ref<CSharpScript> *);
	using FuncScriptManagerBridge_SwapGCHandleForType = bool(KS_CLR_STDCALL *)(GCHandleIntPtr, GCHandleIntPtr *, bool);
	using FuncScriptManagerBridge_GetPropertyInfoList = void(KS_CLR_STDCALL *)(CSharpScript *, Callback_ScriptManagerBridge_GetPropertyInfoList_Add);
	using FuncScriptManagerBridge_GetPropertyDefaultValues = void(KS_CLR_STDCALL *)(CSharpScript *, Callback_ScriptManagerBridge_GetPropertyDefaultValues_Add);
	using FuncScriptManagerBridge_CallStatic = bool(KS_CLR_STDCALL *)(const CSharpScript *, const StringName *, const Variant **, int32_t, Callable::CallError *, Variant *);
	using FuncCSharpInstanceBridge_Call = bool(KS_CLR_STDCALL *)(GCHandleIntPtr, const StringName *, const Variant **, int32_t, Callable::CallError *, Variant *);
	using FuncCSharpInstanceBridge_Set = bool(KS_CLR_STDCALL *)(GCHandleIntPtr, const StringName *, const Variant *);
	using FuncCSharpInstanceBridge_Get = bool(KS_CLR_STDCALL *)(GCHandleIntPtr, const StringName *, Variant *);
	using FuncCSharpInstanceBridge_CallDispose = void(KS_CLR_STDCALL *)(GCHandleIntPtr, bool);
	using FuncCSharpInstanceBridge_CallToString = void(KS_CLR_STDCALL *)(GCHandleIntPtr, String *, bool *);
	using FuncCSharpInstanceBridge_HasMethodUnknownParams = bool(KS_CLR_STDCALL *)(GCHandleIntPtr, const StringName *);
	using FuncCSharpInstanceBridge_SerializeState = void(KS_CLR_STDCALL *)(GCHandleIntPtr, const Dictionary *, const Dictionary *);
	using FuncCSharpInstanceBridge_DeserializeState = void(KS_CLR_STDCALL *)(GCHandleIntPtr, const Dictionary *, const Dictionary *);
	using FuncGCHandleBridge_FreeGCHandle = void(KS_CLR_STDCALL *)(GCHandleIntPtr);
	using FuncGCHandleBridge_GCHandleIsTargetCollectible = bool(KS_CLR_STDCALL *)(GCHandleIntPtr);
	using FuncDebuggingUtils_GetCurrentStackInfo = void(KS_CLR_STDCALL *)(Vector<ScriptLanguage::StackInfo> *);
	using FuncDisposablesTracker_OnKosmicShuttingDown = void(KS_CLR_STDCALL *)();
	using FuncKS_OnCoreApiAssemblyLoaded = void(KS_CLR_STDCALL *)(bool);

	FuncSignalAwaiter_SignalCallback SignalAwaiter_SignalCallback;
	FuncDelegateUtils_InvokeWithVariantArgs DelegateUtils_InvokeWithVariantArgs;
	FuncDelegateUtils_DelegateEquals DelegateUtils_DelegateEquals;
	FuncDelegateUtils_DelegateHash DelegateUtils_DelegateHash;
	FuncDelegateUtils_GetArgumentCount DelegateUtils_GetArgumentCount;
	FuncDelegateUtils_TrySerializeDelegateWithGCHandle DelegateUtils_TrySerializeDelegateWithGCHandle;
	FuncDelegateUtils_TryDeserializeDelegateWithGCHandle DelegateUtils_TryDeserializeDelegateWithGCHandle;
	FuncScriptManagerBridge_FrameCallback ScriptManagerBridge_FrameCallback;
	FuncScriptManagerBridge_CreateManagedForKosmicObjectBinding ScriptManagerBridge_CreateManagedForKosmicObjectBinding;
	FuncScriptManagerBridge_CreateManagedForKosmicObjectScriptInstance ScriptManagerBridge_CreateManagedForKosmicObjectScriptInstance;
	FuncScriptManagerBridge_GetScriptNativeName ScriptManagerBridge_GetScriptNativeName;
	FuncScriptManagerBridge_GetGlobalClassName ScriptManagerBridge_GetGlobalClassName;
	FuncScriptManagerBridge_SetKosmicObjectPtr ScriptManagerBridge_SetKosmicObjectPtr;
	FuncScriptManagerBridge_RaiseEventSignal ScriptManagerBridge_RaiseEventSignal;
	FuncScriptManagerBridge_ScriptIsOrInherits ScriptManagerBridge_ScriptIsOrInherits;
	FuncScriptManagerBridge_AddScriptBridge ScriptManagerBridge_AddScriptBridge;
	FuncScriptManagerBridge_GetOrCreateScriptBridgeForPath ScriptManagerBridge_GetOrCreateScriptBridgeForPath;
	FuncScriptManagerBridge_RemoveScriptBridge ScriptManagerBridge_RemoveScriptBridge;
	FuncScriptManagerBridge_TryReloadRegisteredScriptWithClass ScriptManagerBridge_TryReloadRegisteredScriptWithClass;
	FuncScriptManagerBridge_UpdateScriptClassInfo ScriptManagerBridge_UpdateScriptClassInfo;
	FuncScriptManagerBridge_SwapGCHandleForType ScriptManagerBridge_SwapGCHandleForType;
	FuncScriptManagerBridge_GetPropertyInfoList ScriptManagerBridge_GetPropertyInfoList;
	FuncScriptManagerBridge_GetPropertyDefaultValues ScriptManagerBridge_GetPropertyDefaultValues;
	FuncScriptManagerBridge_CallStatic ScriptManagerBridge_CallStatic;
	FuncCSharpInstanceBridge_Call CSharpInstanceBridge_Call;
	FuncCSharpInstanceBridge_Set CSharpInstanceBridge_Set;
	FuncCSharpInstanceBridge_Get CSharpInstanceBridge_Get;
	FuncCSharpInstanceBridge_CallDispose CSharpInstanceBridge_CallDispose;
	FuncCSharpInstanceBridge_CallToString CSharpInstanceBridge_CallToString;
	FuncCSharpInstanceBridge_HasMethodUnknownParams CSharpInstanceBridge_HasMethodUnknownParams;
	FuncCSharpInstanceBridge_SerializeState CSharpInstanceBridge_SerializeState;
	FuncCSharpInstanceBridge_DeserializeState CSharpInstanceBridge_DeserializeState;
	FuncGCHandleBridge_FreeGCHandle GCHandleBridge_FreeGCHandle;
	FuncGCHandleBridge_GCHandleIsTargetCollectible GCHandleBridge_GCHandleIsTargetCollectible;
	FuncDebuggingUtils_GetCurrentStackInfo DebuggingUtils_GetCurrentStackInfo;
	FuncDisposablesTracker_OnKosmicShuttingDown DisposablesTracker_OnKosmicShuttingDown;
	FuncKS_OnCoreApiAssemblyLoaded KS_OnCoreApiAssemblyLoaded;
};

extern ManagedCallbacks managed_callbacks;
extern bool kosmic_api_cache_updated;

void update_kosmic_api_cache(const ManagedCallbacks &p_managed_callbacks);

} // namespace KSMonoCache

#endif // KS_MONO_CACHE_H
