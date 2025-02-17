/**************************************************************************/
/*  test_resource_uid.h                                                   */
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

#ifndef TEST_RESOURCE_UID_H
#define TEST_RESOURCE_UID_H

#include "core/io/resource_uid.h"

#include "thirdparty/doctest/doctest.h"

#include "tests/test_macros.h"

namespace TestResourceUID {

TEST_CASE("[ResourceUID] Must encode/decode maximum/minimum UID correctly") {
	CHECK_MESSAGE(ResourceUID::get_singleton()->id_to_text(0x7fffffffffffffff) == "uid://d4n4ub6itg400", "Maximum UID must encode correctly.");
	CHECK_MESSAGE(ResourceUID::get_singleton()->text_to_id("uid://d4n4ub6itg400") == 0x7fffffffffffffff, "Maximum UID must decode correctly.");

	CHECK_MESSAGE(ResourceUID::get_singleton()->id_to_text(0) == "uid://a", "Minimum UID must encode correctly.");
	CHECK_MESSAGE(ResourceUID::get_singleton()->text_to_id("uid://a") == 0, "Minimum UID must decode correctly.");
}

} // namespace TestResourceUID

#endif // TEST_RESOURCE_UID_H
