/**************************************************************************/
/*  dom_keys.inc                                                          */
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

#include "core/os/keyboard.h"

// See https://w3c.github.io/uievents-code/#code-value-tables
Key dom_code2kosmic_scancode(EM_UTF8 const p_code[32], EM_UTF8 const p_key[32], bool p_physical) {
#define DOM2KOSMIC(p_str, p_kosmic_code)                                                                \
	if (memcmp((const void *)p_str, (void *)(p_physical ? p_code : p_key), strlen(p_str) + 1) == 0) { \
		return Key::p_kosmic_code;                                                                     \
	}

	// Numpad section.
	DOM2KOSMIC("NumLock", NUMLOCK);
	DOM2KOSMIC("Numpad0", KP_0);
	DOM2KOSMIC("Numpad1", KP_1);
	DOM2KOSMIC("Numpad2", KP_2);
	DOM2KOSMIC("Numpad3", KP_3);
	DOM2KOSMIC("Numpad4", KP_4);
	DOM2KOSMIC("Numpad5", KP_5);
	DOM2KOSMIC("Numpad6", KP_6);
	DOM2KOSMIC("Numpad7", KP_7);
	DOM2KOSMIC("Numpad8", KP_8);
	DOM2KOSMIC("Numpad9", KP_9);
	DOM2KOSMIC("NumpadAdd", KP_ADD);
	DOM2KOSMIC("NumpadBackspace", BACKSPACE);
	DOM2KOSMIC("Clear", CLEAR); // NumLock on macOS.
	DOM2KOSMIC("NumpadClear", CLEAR);
	DOM2KOSMIC("NumpadClearEntry", CLEAR);
	//DOM2KOSMIC("NumpadComma", UNKNOWN);
	DOM2KOSMIC("NumpadDecimal", KP_PERIOD);
	DOM2KOSMIC("NumpadDivide", KP_DIVIDE);
	DOM2KOSMIC("NumpadEnter", KP_ENTER);
	DOM2KOSMIC("NumpadEqual", EQUAL);
	//DOM2KOSMIC("NumpadHash", UNKNOWN);
	//DOM2KOSMIC("NumpadMemoryAdd", UNKNOWN);
	//DOM2KOSMIC("NumpadMemoryClear", UNKNOWN);
	//DOM2KOSMIC("NumpadMemoryRecall", UNKNOWN);
	//DOM2KOSMIC("NumpadMemoryStore", UNKNOWN);
	//DOM2KOSMIC("NumpadMemorySubtract", UNKNOWN);
	DOM2KOSMIC("NumpadMultiply", KP_MULTIPLY);
	DOM2KOSMIC("NumpadParenLeft", PARENLEFT);
	DOM2KOSMIC("NumpadParenRight", PARENRIGHT);
	DOM2KOSMIC("NumpadStar", KP_MULTIPLY); // or ASTERISK ?
	DOM2KOSMIC("NumpadSubtract", KP_SUBTRACT);

	// Alphanumeric section.
	DOM2KOSMIC("Backquote", QUOTELEFT);
	DOM2KOSMIC("Backslash", BACKSLASH);
	DOM2KOSMIC("BracketLeft", BRACKETLEFT);
	DOM2KOSMIC("BracketRight", BRACKETRIGHT);
	DOM2KOSMIC("Comma", COMMA);
	DOM2KOSMIC("Digit0", KEY_0);
	DOM2KOSMIC("Digit1", KEY_1);
	DOM2KOSMIC("Digit2", KEY_2);
	DOM2KOSMIC("Digit3", KEY_3);
	DOM2KOSMIC("Digit4", KEY_4);
	DOM2KOSMIC("Digit5", KEY_5);
	DOM2KOSMIC("Digit6", KEY_6);
	DOM2KOSMIC("Digit7", KEY_7);
	DOM2KOSMIC("Digit8", KEY_8);
	DOM2KOSMIC("Digit9", KEY_9);
	DOM2KOSMIC("Equal", EQUAL);
	DOM2KOSMIC("IntlBackslash", BACKSLASH);
	//DOM2KOSMIC("IntlRo", UNKNOWN);
	DOM2KOSMIC("IntlYen", YEN);

	DOM2KOSMIC("KeyA", A);
	DOM2KOSMIC("KeyB", B);
	DOM2KOSMIC("KeyC", C);
	DOM2KOSMIC("KeyD", D);
	DOM2KOSMIC("KeyE", E);
	DOM2KOSMIC("KeyF", F);
	DOM2KOSMIC("KeyG", G);
	DOM2KOSMIC("KeyH", H);
	DOM2KOSMIC("KeyI", I);
	DOM2KOSMIC("KeyJ", J);
	DOM2KOSMIC("KeyK", K);
	DOM2KOSMIC("KeyL", L);
	DOM2KOSMIC("KeyM", M);
	DOM2KOSMIC("KeyN", N);
	DOM2KOSMIC("KeyO", O);
	DOM2KOSMIC("KeyP", P);
	DOM2KOSMIC("KeyQ", Q);
	DOM2KOSMIC("KeyR", R);
	DOM2KOSMIC("KeyS", S);
	DOM2KOSMIC("KeyT", T);
	DOM2KOSMIC("KeyU", U);
	DOM2KOSMIC("KeyV", V);
	DOM2KOSMIC("KeyW", W);
	DOM2KOSMIC("KeyX", X);
	DOM2KOSMIC("KeyY", Y);
	DOM2KOSMIC("KeyZ", Z);

	DOM2KOSMIC("Minus", MINUS);
	DOM2KOSMIC("Period", PERIOD);
	DOM2KOSMIC("Quote", APOSTROPHE);
	DOM2KOSMIC("Semicolon", SEMICOLON);
	DOM2KOSMIC("Slash", SLASH);

	// Functional keys in the Alphanumeric section.
	DOM2KOSMIC("Alt", ALT);
	DOM2KOSMIC("AltLeft", ALT);
	DOM2KOSMIC("AltRight", ALT);
	DOM2KOSMIC("Backspace", BACKSPACE);
	DOM2KOSMIC("CapsLock", CAPSLOCK);
	DOM2KOSMIC("ContextMenu", MENU);
	DOM2KOSMIC("Control", CTRL);
	DOM2KOSMIC("ControlLeft", CTRL);
	DOM2KOSMIC("ControlRight", CTRL);
	DOM2KOSMIC("Enter", ENTER);
	DOM2KOSMIC("Meta", META);
	DOM2KOSMIC("MetaLeft", META);
	DOM2KOSMIC("MetaRight", META);
	DOM2KOSMIC("OSLeft", META); // Command on macOS.
	DOM2KOSMIC("OSRight", META); // Command on macOS.
	DOM2KOSMIC("Shift", SHIFT);
	DOM2KOSMIC("ShiftLeft", SHIFT);
	DOM2KOSMIC("ShiftRight", SHIFT);
	DOM2KOSMIC("Space", SPACE);
	DOM2KOSMIC("Tab", TAB);

	// ControlPad section.
	DOM2KOSMIC("Delete", KEY_DELETE);
	DOM2KOSMIC("End", END);
	DOM2KOSMIC("Help", HELP);
	DOM2KOSMIC("Home", HOME);
	DOM2KOSMIC("Insert", INSERT);
	DOM2KOSMIC("PageDown", PAGEDOWN);
	DOM2KOSMIC("PageUp", PAGEUP);

	// ArrowPad section.
	DOM2KOSMIC("ArrowDown", DOWN);
	DOM2KOSMIC("ArrowLeft", LEFT);
	DOM2KOSMIC("ArrowRight", RIGHT);
	DOM2KOSMIC("ArrowUp", UP);

	// Function section.
	DOM2KOSMIC("Escape", ESCAPE);
	DOM2KOSMIC("F1", F1);
	DOM2KOSMIC("F2", F2);
	DOM2KOSMIC("F3", F3);
	DOM2KOSMIC("F4", F4);
	DOM2KOSMIC("F5", F5);
	DOM2KOSMIC("F6", F6);
	DOM2KOSMIC("F7", F7);
	DOM2KOSMIC("F8", F8);
	DOM2KOSMIC("F9", F9);
	DOM2KOSMIC("F10", F10);
	DOM2KOSMIC("F11", F11);
	DOM2KOSMIC("F12", F12);
	//DOM2KOSMIC("Fn", UNKNOWN); // never actually fired, but included in the standard draft.
	//DOM2KOSMIC("FnLock", UNKNOWN);
	DOM2KOSMIC("PrintScreen", PRINT);
	DOM2KOSMIC("ScrollLock", SCROLLLOCK);
	DOM2KOSMIC("Pause", PAUSE);

	// Media keys section.
	DOM2KOSMIC("BrowserBack", BACK);
	DOM2KOSMIC("BrowserFavorites", FAVORITES);
	DOM2KOSMIC("BrowserForward", FORWARD);
	DOM2KOSMIC("BrowserHome", OPENURL);
	DOM2KOSMIC("BrowserRefresh", REFRESH);
	DOM2KOSMIC("BrowserSearch", SEARCH);
	DOM2KOSMIC("BrowserStop", STOP);
	//DOM2KOSMIC("Eject", UNKNOWN);
	DOM2KOSMIC("LaunchApp1", LAUNCH0);
	DOM2KOSMIC("LaunchApp2", LAUNCH1);
	DOM2KOSMIC("LaunchMail", LAUNCHMAIL);
	DOM2KOSMIC("MediaPlayPause", MEDIAPLAY);
	DOM2KOSMIC("MediaSelect", LAUNCHMEDIA);
	DOM2KOSMIC("MediaStop", MEDIASTOP);
	DOM2KOSMIC("MediaTrackNext", MEDIANEXT);
	DOM2KOSMIC("MediaTrackPrevious", MEDIAPREVIOUS);
	//DOM2KOSMIC("Power", UNKNOWN);
	//DOM2KOSMIC("Sleep", UNKNOWN);
	DOM2KOSMIC("AudioVolumeDown", VOLUMEDOWN);
	DOM2KOSMIC("AudioVolumeMute", VOLUMEMUTE);
	DOM2KOSMIC("AudioVolumeUp", VOLUMEUP);
	//DOM2KOSMIC("WakeUp", UNKNOWN);

	// Printable ASCII.
	uint8_t b0 = (uint8_t)p_key[0];
	uint8_t b1 = (uint8_t)p_key[1];
	if (b0 >= 0x20 && b0 < 0x7F) { // ASCII.
		if (b0 > 0x60 && b0 < 0x7B) { // Lowercase ASCII.
			b0 -= 32;
		}
		return (Key)b0;
	} else if (b0 == 0xC2 && b1 == 0xA5) {
		return Key::YEN;
	} else if (b0 == 0xC2 && b1 == 0xA7) {
		return Key::SECTION;
	}

	return Key::NONE;
#undef DOM2KOSMIC
}

KeyLocation dom_code2kosmic_key_location(EM_UTF8 const p_code[32]) {
#define DOM2KOSMIC(m_str, m_kosmic_code)                                         \
	if (memcmp((const void *)m_str, (void *)p_code, strlen(m_str) + 1) == 0) { \
		return KeyLocation::m_kosmic_code;                                      \
	}

	DOM2KOSMIC("AltLeft", LEFT);
	DOM2KOSMIC("AltRight", RIGHT);
	DOM2KOSMIC("ControlLeft", LEFT);
	DOM2KOSMIC("ControlRight", RIGHT);
	DOM2KOSMIC("MetaLeft", LEFT);
	DOM2KOSMIC("MetaRight", RIGHT);
	DOM2KOSMIC("OSLeft", LEFT);
	DOM2KOSMIC("OSRight", RIGHT);
	DOM2KOSMIC("ShiftLeft", LEFT);
	DOM2KOSMIC("ShiftRight", RIGHT);

	return KeyLocation::UNSPECIFIED;
#undef DOM2KOSMIC
}
