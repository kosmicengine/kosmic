/**************************************************************************/
/*  mono_gc_handle.cpp                                                    */
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

#include "mono_gc_handle.h"

#include "mono_ks/ks_mono.h"
#include "mono_ks/ks_mono_cache.h"

void MonoGCHandleData::release() {
#ifdef DEBUG_ENABLED
	CRASH_COND(handle.value && KSMono::get_singleton() == nullptr);
#endif

	if (handle.value && KSMonoCache::kosmic_api_cache_updated &&
			KSMono::get_singleton()->is_runtime_initialized()) {
		free_gchandle(handle);
		handle.value = nullptr;
	}
}
void MonoGCHandleData::free_gchandle(GCHandleIntPtr p_gchandle) {
	CRASH_COND(!KSMonoCache::kosmic_api_cache_updated);
	KSMonoCache::managed_callbacks.GCHandleBridge_FreeGCHandle(p_gchandle);
}
