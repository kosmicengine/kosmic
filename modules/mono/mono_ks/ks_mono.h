/**************************************************************************/
/*  ks_mono.h                                                             */
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

#ifndef KS_MONO_H
#define KS_MONO_H

#include "../kosmicsharp_defs.h"

#include "core/io/config_file.h"

#ifndef KS_CLR_STDCALL
#ifdef WIN32
#define KS_CLR_STDCALL __stdcall
#else
#define KS_CLR_STDCALL
#endif
#endif

namespace ksmono {

#ifdef TOOLS_ENABLED
struct PluginCallbacks {
	using FuncLoadProjectAssemblyCallback = bool(KS_CLR_STDCALL *)(const char16_t *, String *);
	using FuncLoadToolsAssemblyCallback = Object *(KS_CLR_STDCALL *)(const char16_t *, const void **, int32_t);
	using FuncUnloadProjectPluginCallback = bool(KS_CLR_STDCALL *)();
	FuncLoadProjectAssemblyCallback LoadProjectAssemblyCallback = nullptr;
	FuncLoadToolsAssemblyCallback LoadToolsAssemblyCallback = nullptr;
	FuncUnloadProjectPluginCallback UnloadProjectPluginCallback = nullptr;
};
#endif

} // namespace ksmono

class KSMono {
	bool initialized = false;
	bool runtime_initialized = false;
	bool finalizing_scripts_domain = false;

	void *hostfxr_dll_handle = nullptr;
	void *coreclr_dll_handle = nullptr;

	String project_assembly_path;
	uint64_t project_assembly_modified_time = 0;
#ifdef KS_MONO_HOT_RELOAD
	int project_load_failure_count = 0;
#endif

#ifdef TOOLS_ENABLED
	bool _load_project_assembly();
	void _try_load_project_assembly();
#endif

#ifdef DEBUG_METHODS_ENABLED
	uint64_t api_core_hash = 0;
#endif
#ifdef TOOLS_ENABLED
	uint64_t api_editor_hash = 0;
#endif
	void _init_kosmic_api_hashes();

#ifdef TOOLS_ENABLED
	gdmono::PluginCallbacks plugin_callbacks;
#endif

protected:
	static KSMono *singleton;

public:
#ifdef DEBUG_METHODS_ENABLED
	uint64_t get_api_core_hash() {
		if (api_core_hash == 0) {
			api_core_hash = ClassDB::get_api_hash(ClassDB::API_CORE);
		}
		return api_core_hash;
	}
#ifdef TOOLS_ENABLED
	uint64_t get_api_editor_hash() {
		if (api_editor_hash == 0) {
			api_editor_hash = ClassDB::get_api_hash(ClassDB::API_EDITOR);
		}
		return api_editor_hash;
	}
#endif // TOOLS_ENABLED
#endif // DEBUG_METHODS_ENABLED

	_FORCE_INLINE_ static String get_expected_api_build_config() {
#ifdef TOOLS_ENABLED
		return "Debug";
#else
#ifdef DEBUG_ENABLED
		return "Debug";
#else
		return "Release";
#endif
#endif
	}

	static KSMono *get_singleton() {
		return singleton;
	}

	_FORCE_INLINE_ bool is_initialized() const {
		return initialized;
	}
	_FORCE_INLINE_ bool is_runtime_initialized() const {
		return runtime_initialized;
	}
	_FORCE_INLINE_ bool is_finalizing_scripts_domain() {
		return finalizing_scripts_domain;
	}

	_FORCE_INLINE_ const String &get_project_assembly_path() const {
		return project_assembly_path;
	}
	_FORCE_INLINE_ uint64_t get_project_assembly_modified_time() const {
		return project_assembly_modified_time;
	}

#ifdef TOOLS_ENABLED
	const gdmono::PluginCallbacks &get_plugin_callbacks() {
		return plugin_callbacks;
	}
#endif

#ifdef KS_MONO_HOT_RELOAD
	void reload_failure();
	Error reload_project_assemblies();
#endif

	bool should_initialize();

	void initialize();

	KSMono();
	~KSMono();
};

namespace mono_bind {

class KosmicSharp : public Object {
	KSCLASS(KosmicSharp, Object);

protected:
	static KosmicSharp *singleton;

public:
	static KosmicSharp *get_singleton() { return singleton; }

	void reload_assemblies(bool p_soft_reload);

	KosmicSharp();
	~KosmicSharp();
};

} // namespace mono_bind

#endif // KS_MONO_H
