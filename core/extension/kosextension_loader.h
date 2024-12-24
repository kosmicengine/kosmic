/**************************************************************************/
/*  ksextension_loader.h                                                  */
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

#ifndef KSEXTENSION_LOADER_H
#define KSEXTENSION_LOADER_H

#include "core/object/ref_counted.h"

class KSExtension;

class KSExtensionLoader : public RefCounted {
public:
	virtual Error open_library(const String &p_path) = 0;
	virtual Error initialize(KSExtensionInterfaceGetProcAddress p_get_proc_address, const Ref<KSExtension> &p_extension, KSExtensionInitialization *r_initialization) = 0;
	virtual void close_library() = 0;
	virtual bool is_library_open() const = 0;
	virtual bool has_library_changed() const = 0;
	virtual bool library_exists() const = 0;
};

#endif // KSEXTENSION_LOADER_H
