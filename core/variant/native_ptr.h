/**************************************************************************/
/*  native_ptr.h                                                          */
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

#ifndef NATIVE_PTR_H
#define NATIVE_PTR_H

#include "core/math/audio_frame.h"
#include "core/variant/method_ptrcall.h"
#include "core/variant/type_info.h"

template <typename T>
struct KSExtensionConstPtr {
	const T *data = nullptr;
	KSExtensionConstPtr(const T *p_assign) { data = p_assign; }
	static const char *get_name() { return "const void"; }
	operator const T *() const { return data; }
	operator Variant() const { return uint64_t(data); }
};

template <typename T>
struct KSExtensionPtr {
	T *data = nullptr;
	KSExtensionPtr(T *p_assign) { data = p_assign; }
	static const char *get_name() { return "void"; }
	operator T *() const { return data; }
	operator Variant() const { return uint64_t(data); }
};

#define KSVIRTUAL_NATIVE_PTR(m_type)                                                                          \
	template <>                                                                                               \
	struct KSExtensionConstPtr<const m_type> {                                                                \
		const m_type *data = nullptr;                                                                         \
		KSExtensionConstPtr() {}                                                                              \
		KSExtensionConstPtr(const m_type *p_assign) {                                                         \
			data = p_assign;                                                                                  \
		}                                                                                                     \
		static const char *get_name() {                                                                       \
			return "const " #m_type;                                                                          \
		}                                                                                                     \
		operator const m_type *() const {                                                                     \
			return data;                                                                                      \
		}                                                                                                     \
		operator Variant() const {                                                                            \
			return uint64_t(data);                                                                            \
		}                                                                                                     \
	};                                                                                                        \
	template <>                                                                                               \
	struct VariantCaster<KSExtensionConstPtr<const m_type>> {                                                 \
		static _FORCE_INLINE_ KSExtensionConstPtr<const m_type> cast(const Variant &p_variant) {              \
			return KSExtensionConstPtr<const m_type>((const m_type *)p_variant.operator uint64_t());          \
		}                                                                                                     \
	};                                                                                                        \
	template <>                                                                                               \
	struct VariantInternalAccessor<KSExtensionConstPtr<const m_type>> {                                       \
		static _FORCE_INLINE_ const KSExtensionConstPtr<const m_type> &get(const Variant *v) {                \
			return *reinterpret_cast<const KSExtensionConstPtr<const m_type> *>(VariantInternal::get_int(v)); \
		}                                                                                                     \
		static _FORCE_INLINE_ void set(Variant *v, const KSExtensionConstPtr<const m_type> &p_value) {        \
			*VariantInternal::get_int(v) = uint64_t(p_value.data);                                            \
		}                                                                                                     \
	};                                                                                                        \
	template <>                                                                                               \
	struct KSExtensionPtr<m_type> {                                                                           \
		m_type *data = nullptr;                                                                               \
		KSExtensionPtr() {}                                                                                   \
		KSExtensionPtr(m_type *p_assign) {                                                                    \
			data = p_assign;                                                                                  \
		}                                                                                                     \
		static const char *get_name() {                                                                       \
			return #m_type;                                                                                   \
		}                                                                                                     \
		operator m_type *() const {                                                                           \
			return data;                                                                                      \
		}                                                                                                     \
		operator Variant() const {                                                                            \
			return uint64_t(data);                                                                            \
		}                                                                                                     \
	};                                                                                                        \
	template <>                                                                                               \
	struct VariantCaster<KSExtensionPtr<m_type>> {                                                            \
		static _FORCE_INLINE_ KSExtensionPtr<m_type> cast(const Variant &p_variant) {                         \
			return KSExtensionPtr<m_type>((m_type *)p_variant.operator uint64_t());                           \
		}                                                                                                     \
	};                                                                                                        \
	template <>                                                                                               \
	struct VariantInternalAccessor<KSExtensionPtr<m_type>> {                                                  \
		static _FORCE_INLINE_ const KSExtensionPtr<m_type> &get(const Variant *v) {                           \
			return *reinterpret_cast<const KSExtensionPtr<m_type> *>(VariantInternal::get_int(v));            \
		}                                                                                                     \
		static _FORCE_INLINE_ void set(Variant *v, const KSExtensionPtr<m_type> &p_value) {                   \
			*VariantInternal::get_int(v) = uint64_t(p_value.data);                                            \
		}                                                                                                     \
	};

template <typename T>
struct GetTypeInfo<KSExtensionConstPtr<T>> {
	static const Variant::Type VARIANT_TYPE = Variant::NIL;
	static const KosmicTypeInfo::Metadata METADATA = KosmicTypeInfo::METADATA_NONE;
	static inline PropertyInfo get_class_info() {
		return PropertyInfo(Variant::INT, String(), PROPERTY_HINT_INT_IS_POINTER, KSExtensionConstPtr<T>::get_name());
	}
};

template <typename T>
struct GetTypeInfo<KSExtensionPtr<T>> {
	static const Variant::Type VARIANT_TYPE = Variant::NIL;
	static const KosmicTypeInfo::Metadata METADATA = KosmicTypeInfo::METADATA_NONE;
	static inline PropertyInfo get_class_info() {
		return PropertyInfo(Variant::INT, String(), PROPERTY_HINT_INT_IS_POINTER, KSExtensionPtr<T>::get_name());
	}
};

template <typename T>
struct PtrToArg<KSExtensionConstPtr<T>> {
	_FORCE_INLINE_ static KSExtensionConstPtr<T> convert(const void *p_ptr) {
		return KSExtensionConstPtr<T>(reinterpret_cast<const T *>(p_ptr));
	}
	typedef const T *EncodeT;
	_FORCE_INLINE_ static void encode(KSExtensionConstPtr<T> p_val, void *p_ptr) {
		*((const T **)p_ptr) = p_val.data;
	}
};
template <typename T>
struct PtrToArg<KSExtensionPtr<T>> {
	_FORCE_INLINE_ static KSExtensionPtr<T> convert(const void *p_ptr) {
		return KSExtensionPtr<T>(reinterpret_cast<const T *>(p_ptr));
	}
	typedef T *EncodeT;
	_FORCE_INLINE_ static void encode(KSExtensionPtr<T> p_val, void *p_ptr) {
		*((T **)p_ptr) = p_val.data;
	}
};

KSVIRTUAL_NATIVE_PTR(void)
KSVIRTUAL_NATIVE_PTR(AudioFrame)
KSVIRTUAL_NATIVE_PTR(bool)
KSVIRTUAL_NATIVE_PTR(char)
KSVIRTUAL_NATIVE_PTR(char16_t)
KSVIRTUAL_NATIVE_PTR(char32_t)
KSVIRTUAL_NATIVE_PTR(wchar_t)
KSVIRTUAL_NATIVE_PTR(uint8_t)
KSVIRTUAL_NATIVE_PTR(uint8_t *)
KSVIRTUAL_NATIVE_PTR(int8_t)
KSVIRTUAL_NATIVE_PTR(uint16_t)
KSVIRTUAL_NATIVE_PTR(int16_t)
KSVIRTUAL_NATIVE_PTR(uint32_t)
KSVIRTUAL_NATIVE_PTR(int32_t)
KSVIRTUAL_NATIVE_PTR(int64_t)
KSVIRTUAL_NATIVE_PTR(uint64_t)
KSVIRTUAL_NATIVE_PTR(float)
KSVIRTUAL_NATIVE_PTR(double)

#endif // NATIVE_PTR_H
