/**************************************************************************/
/*  library_kosmic_display.js                                              */
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

const KosmicDisplayVK = {

	$KosmicDisplayVK__deps: ['$KosmicRuntime', '$KosmicConfig', '$KosmicEventListeners'],
	$KosmicDisplayVK__postset: 'KosmicOS.atexit(function(resolve, reject) { KosmicDisplayVK.clear(); resolve(); });',
	$KosmicDisplayVK: {
		textinput: null,
		textarea: null,

		available: function () {
			return KosmicConfig.virtual_keyboard && 'ontouchstart' in window;
		},

		init: function (input_cb) {
			function create(what) {
				const elem = document.createElement(what);
				elem.style.display = 'none';
				elem.style.position = 'absolute';
				elem.style.zIndex = '-1';
				elem.style.background = 'transparent';
				elem.style.padding = '0px';
				elem.style.margin = '0px';
				elem.style.overflow = 'hidden';
				elem.style.width = '0px';
				elem.style.height = '0px';
				elem.style.border = '0px';
				elem.style.outline = 'none';
				elem.readonly = true;
				elem.disabled = true;
				KosmicEventListeners.add(elem, 'input', function (evt) {
					const c_str = KosmicRuntime.allocString(elem.value);
					input_cb(c_str, elem.selectionEnd);
					KosmicRuntime.free(c_str);
				}, false);
				KosmicEventListeners.add(elem, 'blur', function (evt) {
					elem.style.display = 'none';
					elem.readonly = true;
					elem.disabled = true;
				}, false);
				KosmicConfig.canvas.insertAdjacentElement('beforebegin', elem);
				return elem;
			}
			KosmicDisplayVK.textinput = create('input');
			KosmicDisplayVK.textarea = create('textarea');
			KosmicDisplayVK.updateSize();
		},
		show: function (text, type, start, end) {
			if (!KosmicDisplayVK.textinput || !KosmicDisplayVK.textarea) {
				return;
			}
			if (KosmicDisplayVK.textinput.style.display !== '' || KosmicDisplayVK.textarea.style.display !== '') {
				KosmicDisplayVK.hide();
			}
			KosmicDisplayVK.updateSize();

			let elem = KosmicDisplayVK.textinput;
			switch (type) {
			case 0: // KEYBOARD_TYPE_DEFAULT
				elem.type = 'text';
				elem.inputmode = '';
				break;
			case 1: // KEYBOARD_TYPE_MULTILINE
				elem = KosmicDisplayVK.textarea;
				break;
			case 2: // KEYBOARD_TYPE_NUMBER
				elem.type = 'text';
				elem.inputmode = 'numeric';
				break;
			case 3: // KEYBOARD_TYPE_NUMBER_DECIMAL
				elem.type = 'text';
				elem.inputmode = 'decimal';
				break;
			case 4: // KEYBOARD_TYPE_PHONE
				elem.type = 'tel';
				elem.inputmode = '';
				break;
			case 5: // KEYBOARD_TYPE_EMAIL_ADDRESS
				elem.type = 'email';
				elem.inputmode = '';
				break;
			case 6: // KEYBOARD_TYPE_PASSWORD
				elem.type = 'password';
				elem.inputmode = '';
				break;
			case 7: // KEYBOARD_TYPE_URL
				elem.type = 'url';
				elem.inputmode = '';
				break;
			default:
				elem.type = 'text';
				elem.inputmode = '';
				break;
			}

			elem.readonly = false;
			elem.disabled = false;
			elem.value = text;
			elem.style.display = 'block';
			elem.focus();
			elem.setSelectionRange(start, end);
		},
		hide: function () {
			if (!KosmicDisplayVK.textinput || !KosmicDisplayVK.textarea) {
				return;
			}
			[KosmicDisplayVK.textinput, KosmicDisplayVK.textarea].forEach(function (elem) {
				elem.blur();
				elem.style.display = 'none';
				elem.value = '';
			});
		},
		updateSize: function () {
			if (!KosmicDisplayVK.textinput || !KosmicDisplayVK.textarea) {
				return;
			}
			const rect = KosmicConfig.canvas.getBoundingClientRect();
			function update(elem) {
				elem.style.left = `${rect.left}px`;
				elem.style.top = `${rect.top}px`;
				elem.style.width = `${rect.width}px`;
				elem.style.height = `${rect.height}px`;
			}
			update(KosmicDisplayVK.textinput);
			update(KosmicDisplayVK.textarea);
		},
		clear: function () {
			if (KosmicDisplayVK.textinput) {
				KosmicDisplayVK.textinput.remove();
				KosmicDisplayVK.textinput = null;
			}
			if (KosmicDisplayVK.textarea) {
				KosmicDisplayVK.textarea.remove();
				KosmicDisplayVK.textarea = null;
			}
		},
	},
};
mergeInto(LibraryManager.library, KosmicDisplayVK);

/*
 * Display server cursor helper.
 * Keeps track of cursor status and custom shapes.
 */
const KosmicDisplayCursor = {
	$KosmicDisplayCursor__deps: ['$KosmicOS', '$KosmicConfig'],
	$KosmicDisplayCursor__postset: 'KosmicOS.atexit(function(resolve, reject) { KosmicDisplayCursor.clear(); resolve(); });',
	$KosmicDisplayCursor: {
		shape: 'default',
		visible: true,
		cursors: {},
		set_style: function (style) {
			KosmicConfig.canvas.style.cursor = style;
		},
		set_shape: function (shape) {
			KosmicDisplayCursor.shape = shape;
			let css = shape;
			if (shape in KosmicDisplayCursor.cursors) {
				const c = KosmicDisplayCursor.cursors[shape];
				css = `url("${c.url}") ${c.x} ${c.y}, default`;
			}
			if (KosmicDisplayCursor.visible) {
				KosmicDisplayCursor.set_style(css);
			}
		},
		clear: function () {
			KosmicDisplayCursor.set_style('');
			KosmicDisplayCursor.shape = 'default';
			KosmicDisplayCursor.visible = true;
			Object.keys(KosmicDisplayCursor.cursors).forEach(function (key) {
				URL.revokeObjectURL(KosmicDisplayCursor.cursors[key]);
				delete KosmicDisplayCursor.cursors[key];
			});
		},
		lockPointer: function () {
			const canvas = KosmicConfig.canvas;
			if (canvas.requestPointerLock) {
				canvas.requestPointerLock();
			}
		},
		releasePointer: function () {
			if (document.exitPointerLock) {
				document.exitPointerLock();
			}
		},
		isPointerLocked: function () {
			return document.pointerLockElement === KosmicConfig.canvas;
		},
	},
};
mergeInto(LibraryManager.library, KosmicDisplayCursor);

const KosmicDisplayScreen = {
	$KosmicDisplayScreen__deps: ['$KosmicConfig', '$KosmicOS', '$GL', 'emscripten_webgl_get_current_context'],
	$KosmicDisplayScreen: {
		desired_size: [0, 0],
		hidpi: true,
		getPixelRatio: function () {
			return KosmicDisplayScreen.hidpi ? window.devicePixelRatio || 1 : 1;
		},
		isFullscreen: function () {
			const elem = document.fullscreenElement || document.mozFullscreenElement
				|| document.webkitFullscreenElement || document.msFullscreenElement;
			if (elem) {
				return elem === KosmicConfig.canvas;
			}
			// But maybe knowing the element is not supported.
			return document.fullscreen || document.mozFullScreen
				|| document.webkitIsFullscreen;
		},
		hasFullscreen: function () {
			return document.fullscreenEnabled || document.mozFullScreenEnabled
				|| document.webkitFullscreenEnabled;
		},
		requestFullscreen: function () {
			if (!KosmicDisplayScreen.hasFullscreen()) {
				return 1;
			}
			const canvas = KosmicConfig.canvas;
			try {
				const promise = (canvas.requestFullscreen || canvas.msRequestFullscreen
					|| canvas.mozRequestFullScreen || canvas.mozRequestFullscreen
					|| canvas.webkitRequestFullscreen
				).call(canvas);
				// Some browsers (Safari) return undefined.
				// For the standard ones, we need to catch it.
				if (promise) {
					promise.catch(function () {
						// nothing to do.
					});
				}
			} catch (e) {
				return 1;
			}
			return 0;
		},
		exitFullscreen: function () {
			if (!KosmicDisplayScreen.isFullscreen()) {
				return 0;
			}
			try {
				const promise = document.exitFullscreen();
				if (promise) {
					promise.catch(function () {
						// nothing to do.
					});
				}
			} catch (e) {
				return 1;
			}
			return 0;
		},
		_updateGL: function () {
			const gl_context_handle = _emscripten_webgl_get_current_context();
			const gl = GL.getContext(gl_context_handle);
			if (gl) {
				GL.resizeOffscreenFramebuffer(gl);
			}
		},
		updateSize: function () {
			const isFullscreen = KosmicDisplayScreen.isFullscreen();
			const wantsFullWindow = KosmicConfig.canvas_resize_policy === 2;
			const noResize = KosmicConfig.canvas_resize_policy === 0;
			const dWidth = KosmicDisplayScreen.desired_size[0];
			const dHeight = KosmicDisplayScreen.desired_size[1];
			const canvas = KosmicConfig.canvas;
			let width = dWidth;
			let height = dHeight;
			if (noResize) {
				// Don't resize canvas, just update GL if needed.
				if (canvas.width !== width || canvas.height !== height) {
					KosmicDisplayScreen.desired_size = [canvas.width, canvas.height];
					KosmicDisplayScreen._updateGL();
					return 1;
				}
				return 0;
			}
			const scale = KosmicDisplayScreen.getPixelRatio();
			if (isFullscreen || wantsFullWindow) {
				// We need to match screen size.
				width = window.innerWidth * scale;
				height = window.innerHeight * scale;
			}
			const csw = `${width / scale}px`;
			const csh = `${height / scale}px`;
			if (canvas.style.width !== csw || canvas.style.height !== csh || canvas.width !== width || canvas.height !== height) {
				// Size doesn't match.
				// Resize canvas, set correct CSS pixel size, update GL.
				canvas.width = width;
				canvas.height = height;
				canvas.style.width = csw;
				canvas.style.height = csh;
				KosmicDisplayScreen._updateGL();
				return 1;
			}
			return 0;
		},
	},
};
mergeInto(LibraryManager.library, KosmicDisplayScreen);

/**
 * Display server interface.
 *
 * Exposes all the functions needed by DisplayServer implementation.
 */
const KosmicDisplay = {
	$KosmicDisplay__deps: ['$KosmicConfig', '$KosmicRuntime', '$KosmicDisplayCursor', '$KosmicEventListeners', '$KosmicDisplayScreen', '$KosmicDisplayVK'],
	$KosmicDisplay: {
		window_icon: '',
		getDPI: function () {
			// devicePixelRatio is given in dppx
			// https://drafts.csswg.org/css-values/#resolution
			// > due to the 1:96 fixed ratio of CSS *in* to CSS *px*, 1dppx is equivalent to 96dpi.
			const dpi = Math.round(window.devicePixelRatio * 96);
			return dpi >= 96 ? dpi : 96;
		},
	},

	kosmic_js_display_is_swap_ok_cancel__proxy: 'sync',
	kosmic_js_display_is_swap_ok_cancel__sig: 'i',
	kosmic_js_display_is_swap_ok_cancel: function () {
		const win = (['Windows', 'Win64', 'Win32', 'WinCE']);
		const plat = navigator.platform || '';
		if (win.indexOf(plat) !== -1) {
			return 1;
		}
		return 0;
	},

	kosmic_js_tts_is_speaking__proxy: 'sync',
	kosmic_js_tts_is_speaking__sig: 'i',
	kosmic_js_tts_is_speaking: function () {
		return window.speechSynthesis.speaking;
	},

	kosmic_js_tts_is_paused__proxy: 'sync',
	kosmic_js_tts_is_paused__sig: 'i',
	kosmic_js_tts_is_paused: function () {
		return window.speechSynthesis.paused;
	},

	kosmic_js_tts_get_voices__proxy: 'sync',
	kosmic_js_tts_get_voices__sig: 'vi',
	kosmic_js_tts_get_voices: function (p_callback) {
		const func = KosmicRuntime.get_func(p_callback);
		try {
			const arr = [];
			const voices = window.speechSynthesis.getVoices();
			for (let i = 0; i < voices.length; i++) {
				arr.push(`${voices[i].lang};${voices[i].name}`);
			}
			const c_ptr = KosmicRuntime.allocStringArray(arr);
			func(arr.length, c_ptr);
			KosmicRuntime.freeStringArray(c_ptr, arr.length);
		} catch (e) {
			// Fail graciously.
		}
	},

	kosmic_js_tts_speak__proxy: 'sync',
	kosmic_js_tts_speak__sig: 'viiiffii',
	kosmic_js_tts_speak: function (p_text, p_voice, p_volume, p_pitch, p_rate, p_utterance_id, p_callback) {
		const func = KosmicRuntime.get_func(p_callback);

		function listener_end(evt) {
			evt.currentTarget.cb(1 /* TTS_UTTERANCE_ENDED */, evt.currentTarget.id, 0);
		}

		function listener_start(evt) {
			evt.currentTarget.cb(0 /* TTS_UTTERANCE_STARTED */, evt.currentTarget.id, 0);
		}

		function listener_error(evt) {
			evt.currentTarget.cb(2 /* TTS_UTTERANCE_CANCELED */, evt.currentTarget.id, 0);
		}

		function listener_bound(evt) {
			evt.currentTarget.cb(3 /* TTS_UTTERANCE_BOUNDARY */, evt.currentTarget.id, evt.charIndex);
		}

		const utterance = new SpeechSynthesisUtterance(KosmicRuntime.parseString(p_text));
		utterance.rate = p_rate;
		utterance.pitch = p_pitch;
		utterance.volume = p_volume / 100.0;
		utterance.addEventListener('end', listener_end);
		utterance.addEventListener('start', listener_start);
		utterance.addEventListener('error', listener_error);
		utterance.addEventListener('boundary', listener_bound);
		utterance.id = p_utterance_id;
		utterance.cb = func;
		const voice = KosmicRuntime.parseString(p_voice);
		const voices = window.speechSynthesis.getVoices();
		for (let i = 0; i < voices.length; i++) {
			if (voices[i].name === voice) {
				utterance.voice = voices[i];
				break;
			}
		}
		window.speechSynthesis.resume();
		window.speechSynthesis.speak(utterance);
	},

	kosmic_js_tts_pause__proxy: 'sync',
	kosmic_js_tts_pause__sig: 'v',
	kosmic_js_tts_pause: function () {
		window.speechSynthesis.pause();
	},

	kosmic_js_tts_resume__proxy: 'sync',
	kosmic_js_tts_resume__sig: 'v',
	kosmic_js_tts_resume: function () {
		window.speechSynthesis.resume();
	},

	kosmic_js_tts_stop__proxy: 'sync',
	kosmic_js_tts_stop__sig: 'v',
	kosmic_js_tts_stop: function () {
		window.speechSynthesis.cancel();
		window.speechSynthesis.resume();
	},

	kosmic_js_display_alert__proxy: 'sync',
	kosmic_js_display_alert__sig: 'vi',
	kosmic_js_display_alert: function (p_text) {
		window.alert(KosmicRuntime.parseString(p_text)); // eslint-disable-line no-alert
	},

	kosmic_js_display_screen_dpi_get__proxy: 'sync',
	kosmic_js_display_screen_dpi_get__sig: 'i',
	kosmic_js_display_screen_dpi_get: function () {
		return KosmicDisplay.getDPI();
	},

	kosmic_js_display_pixel_ratio_get__proxy: 'sync',
	kosmic_js_display_pixel_ratio_get__sig: 'f',
	kosmic_js_display_pixel_ratio_get: function () {
		return KosmicDisplayScreen.getPixelRatio();
	},

	kosmic_js_display_fullscreen_request__proxy: 'sync',
	kosmic_js_display_fullscreen_request__sig: 'i',
	kosmic_js_display_fullscreen_request: function () {
		return KosmicDisplayScreen.requestFullscreen();
	},

	kosmic_js_display_fullscreen_exit__proxy: 'sync',
	kosmic_js_display_fullscreen_exit__sig: 'i',
	kosmic_js_display_fullscreen_exit: function () {
		return KosmicDisplayScreen.exitFullscreen();
	},

	kosmic_js_display_desired_size_set__proxy: 'sync',
	kosmic_js_display_desired_size_set__sig: 'vii',
	kosmic_js_display_desired_size_set: function (width, height) {
		KosmicDisplayScreen.desired_size = [width, height];
		KosmicDisplayScreen.updateSize();
	},

	kosmic_js_display_size_update__proxy: 'sync',
	kosmic_js_display_size_update__sig: 'i',
	kosmic_js_display_size_update: function () {
		const updated = KosmicDisplayScreen.updateSize();
		if (updated) {
			KosmicDisplayVK.updateSize();
		}
		return updated;
	},

	kosmic_js_display_screen_size_get__proxy: 'sync',
	kosmic_js_display_screen_size_get__sig: 'vii',
	kosmic_js_display_screen_size_get: function (width, height) {
		const scale = KosmicDisplayScreen.getPixelRatio();
		KosmicRuntime.setHeapValue(width, window.screen.width * scale, 'i32');
		KosmicRuntime.setHeapValue(height, window.screen.height * scale, 'i32');
	},

	kosmic_js_display_window_size_get__proxy: 'sync',
	kosmic_js_display_window_size_get__sig: 'vii',
	kosmic_js_display_window_size_get: function (p_width, p_height) {
		KosmicRuntime.setHeapValue(p_width, KosmicConfig.canvas.width, 'i32');
		KosmicRuntime.setHeapValue(p_height, KosmicConfig.canvas.height, 'i32');
	},

	kosmic_js_display_has_webgl__proxy: 'sync',
	kosmic_js_display_has_webgl__sig: 'ii',
	kosmic_js_display_has_webgl: function (p_version) {
		if (p_version !== 1 && p_version !== 2) {
			return false;
		}
		try {
			return !!document.createElement('canvas').getContext(p_version === 2 ? 'webgl2' : 'webgl');
		} catch (e) { /* Not available */ }
		return false;
	},

	/*
	 * Canvas
	 */
	kosmic_js_display_canvas_focus__proxy: 'sync',
	kosmic_js_display_canvas_focus__sig: 'v',
	kosmic_js_display_canvas_focus: function () {
		KosmicConfig.canvas.focus();
	},

	kosmic_js_display_canvas_is_focused__proxy: 'sync',
	kosmic_js_display_canvas_is_focused__sig: 'i',
	kosmic_js_display_canvas_is_focused: function () {
		return document.activeElement === KosmicConfig.canvas;
	},

	/*
	 * Touchscreen
	 */
	kosmic_js_display_touchscreen_is_available__proxy: 'sync',
	kosmic_js_display_touchscreen_is_available__sig: 'i',
	kosmic_js_display_touchscreen_is_available: function () {
		return 'ontouchstart' in window;
	},

	/*
	 * Clipboard
	 */
	kosmic_js_display_clipboard_set__proxy: 'sync',
	kosmic_js_display_clipboard_set__sig: 'ii',
	kosmic_js_display_clipboard_set: function (p_text) {
		const text = KosmicRuntime.parseString(p_text);
		if (!navigator.clipboard || !navigator.clipboard.writeText) {
			return 1;
		}
		navigator.clipboard.writeText(text).catch(function (e) {
			// Setting OS clipboard is only possible from an input callback.
			KosmicRuntime.error('Setting OS clipboard is only possible from an input callback for the Web platform. Exception:', e);
		});
		return 0;
	},

	kosmic_js_display_clipboard_get__proxy: 'sync',
	kosmic_js_display_clipboard_get__sig: 'ii',
	kosmic_js_display_clipboard_get: function (callback) {
		const func = KosmicRuntime.get_func(callback);
		try {
			navigator.clipboard.readText().then(function (result) {
				const ptr = KosmicRuntime.allocString(result);
				func(ptr);
				KosmicRuntime.free(ptr);
			}).catch(function (e) {
				// Fail graciously.
			});
		} catch (e) {
			// Fail graciously.
		}
	},

	/*
	 * Window
	 */
	kosmic_js_display_window_title_set__proxy: 'sync',
	kosmic_js_display_window_title_set__sig: 'vi',
	kosmic_js_display_window_title_set: function (p_data) {
		document.title = KosmicRuntime.parseString(p_data);
	},

	kosmic_js_display_window_icon_set__proxy: 'sync',
	kosmic_js_display_window_icon_set__sig: 'vii',
	kosmic_js_display_window_icon_set: function (p_ptr, p_len) {
		let link = document.getElementById('-gd-engine-icon');
		const old_icon = KosmicDisplay.window_icon;
		if (p_ptr) {
			if (link === null) {
				link = document.createElement('link');
				link.rel = 'icon';
				link.id = '-gd-engine-icon';
				document.head.appendChild(link);
			}
			const png = new Blob([KosmicRuntime.heapSlice(HEAPU8, p_ptr, p_len)], { type: 'image/png' });
			KosmicDisplay.window_icon = URL.createObjectURL(png);
			link.href = KosmicDisplay.window_icon;
		} else {
			if (link) {
				link.remove();
			}
			KosmicDisplay.window_icon = null;
		}
		if (old_icon) {
			URL.revokeObjectURL(old_icon);
		}
	},

	/*
	 * Cursor
	 */
	kosmic_js_display_cursor_set_visible__proxy: 'sync',
	kosmic_js_display_cursor_set_visible__sig: 'vi',
	kosmic_js_display_cursor_set_visible: function (p_visible) {
		const visible = p_visible !== 0;
		if (visible === KosmicDisplayCursor.visible) {
			return;
		}
		KosmicDisplayCursor.visible = visible;
		if (visible) {
			KosmicDisplayCursor.set_shape(KosmicDisplayCursor.shape);
		} else {
			KosmicDisplayCursor.set_style('none');
		}
	},

	kosmic_js_display_cursor_is_hidden__proxy: 'sync',
	kosmic_js_display_cursor_is_hidden__sig: 'i',
	kosmic_js_display_cursor_is_hidden: function () {
		return !KosmicDisplayCursor.visible;
	},

	kosmic_js_display_cursor_set_shape__proxy: 'sync',
	kosmic_js_display_cursor_set_shape__sig: 'vi',
	kosmic_js_display_cursor_set_shape: function (p_string) {
		KosmicDisplayCursor.set_shape(KosmicRuntime.parseString(p_string));
	},

	kosmic_js_display_cursor_set_custom_shape__proxy: 'sync',
	kosmic_js_display_cursor_set_custom_shape__sig: 'viiiii',
	kosmic_js_display_cursor_set_custom_shape: function (p_shape, p_ptr, p_len, p_hotspot_x, p_hotspot_y) {
		const shape = KosmicRuntime.parseString(p_shape);
		const old_shape = KosmicDisplayCursor.cursors[shape];
		if (p_len > 0) {
			const png = new Blob([KosmicRuntime.heapSlice(HEAPU8, p_ptr, p_len)], { type: 'image/png' });
			const url = URL.createObjectURL(png);
			KosmicDisplayCursor.cursors[shape] = {
				url: url,
				x: p_hotspot_x,
				y: p_hotspot_y,
			};
		} else {
			delete KosmicDisplayCursor.cursors[shape];
		}
		if (shape === KosmicDisplayCursor.shape) {
			KosmicDisplayCursor.set_shape(KosmicDisplayCursor.shape);
		}
		if (old_shape) {
			URL.revokeObjectURL(old_shape.url);
		}
	},

	kosmic_js_display_cursor_lock_set__proxy: 'sync',
	kosmic_js_display_cursor_lock_set__sig: 'vi',
	kosmic_js_display_cursor_lock_set: function (p_lock) {
		if (p_lock) {
			KosmicDisplayCursor.lockPointer();
		} else {
			KosmicDisplayCursor.releasePointer();
		}
	},

	kosmic_js_display_cursor_is_locked__proxy: 'sync',
	kosmic_js_display_cursor_is_locked__sig: 'i',
	kosmic_js_display_cursor_is_locked: function () {
		return KosmicDisplayCursor.isPointerLocked() ? 1 : 0;
	},

	/*
	 * Listeners
	 */
	kosmic_js_display_fullscreen_cb__proxy: 'sync',
	kosmic_js_display_fullscreen_cb__sig: 'vi',
	kosmic_js_display_fullscreen_cb: function (callback) {
		const canvas = KosmicConfig.canvas;
		const func = KosmicRuntime.get_func(callback);
		function change_cb(evt) {
			if (evt.target === canvas) {
				func(KosmicDisplayScreen.isFullscreen());
			}
		}
		KosmicEventListeners.add(document, 'fullscreenchange', change_cb, false);
		KosmicEventListeners.add(document, 'mozfullscreenchange', change_cb, false);
		KosmicEventListeners.add(document, 'webkitfullscreenchange', change_cb, false);
	},

	kosmic_js_display_window_blur_cb__proxy: 'sync',
	kosmic_js_display_window_blur_cb__sig: 'vi',
	kosmic_js_display_window_blur_cb: function (callback) {
		const func = KosmicRuntime.get_func(callback);
		KosmicEventListeners.add(window, 'blur', function () {
			func();
		}, false);
	},

	kosmic_js_display_notification_cb__proxy: 'sync',
	kosmic_js_display_notification_cb__sig: 'viiiii',
	kosmic_js_display_notification_cb: function (callback, p_enter, p_exit, p_in, p_out) {
		const canvas = KosmicConfig.canvas;
		const func = KosmicRuntime.get_func(callback);
		const notif = [p_enter, p_exit, p_in, p_out];
		['mouseover', 'mouseleave', 'focus', 'blur'].forEach(function (evt_name, idx) {
			KosmicEventListeners.add(canvas, evt_name, function () {
				func(notif[idx]);
			}, true);
		});
	},

	kosmic_js_display_setup_canvas__proxy: 'sync',
	kosmic_js_display_setup_canvas__sig: 'viiii',
	kosmic_js_display_setup_canvas: function (p_width, p_height, p_fullscreen, p_hidpi) {
		const canvas = KosmicConfig.canvas;
		KosmicEventListeners.add(canvas, 'contextmenu', function (ev) {
			ev.preventDefault();
		}, false);
		KosmicEventListeners.add(canvas, 'webglcontextlost', function (ev) {
			alert('WebGL context lost, please reload the page'); // eslint-disable-line no-alert
			ev.preventDefault();
		}, false);
		KosmicDisplayScreen.hidpi = !!p_hidpi;
		switch (KosmicConfig.canvas_resize_policy) {
		case 0: // None
			KosmicDisplayScreen.desired_size = [canvas.width, canvas.height];
			break;
		case 1: // Project
			KosmicDisplayScreen.desired_size = [p_width, p_height];
			break;
		default: // Full window
			// Ensure we display in the right place, the size will be handled by updateSize
			canvas.style.position = 'absolute';
			canvas.style.top = 0;
			canvas.style.left = 0;
			break;
		}
		KosmicDisplayScreen.updateSize();
		if (p_fullscreen) {
			KosmicDisplayScreen.requestFullscreen();
		}
	},

	/*
	 * Virtual Keyboard
	 */
	kosmic_js_display_vk_show__proxy: 'sync',
	kosmic_js_display_vk_show__sig: 'viiii',
	kosmic_js_display_vk_show: function (p_text, p_type, p_start, p_end) {
		const text = KosmicRuntime.parseString(p_text);
		const start = p_start > 0 ? p_start : 0;
		const end = p_end > 0 ? p_end : start;
		KosmicDisplayVK.show(text, p_type, start, end);
	},

	kosmic_js_display_vk_hide__proxy: 'sync',
	kosmic_js_display_vk_hide__sig: 'v',
	kosmic_js_display_vk_hide: function () {
		KosmicDisplayVK.hide();
	},

	kosmic_js_display_vk_available__proxy: 'sync',
	kosmic_js_display_vk_available__sig: 'i',
	kosmic_js_display_vk_available: function () {
		return KosmicDisplayVK.available();
	},

	kosmic_js_display_tts_available__proxy: 'sync',
	kosmic_js_display_tts_available__sig: 'i',
	kosmic_js_display_tts_available: function () {
		return 'speechSynthesis' in window;
	},

	kosmic_js_display_vk_cb__proxy: 'sync',
	kosmic_js_display_vk_cb__sig: 'vi',
	kosmic_js_display_vk_cb: function (p_input_cb) {
		const input_cb = KosmicRuntime.get_func(p_input_cb);
		if (KosmicDisplayVK.available()) {
			KosmicDisplayVK.init(input_cb);
		}
	},
};

autoAddDeps(KosmicDisplay, '$KosmicDisplay');
mergeInto(LibraryManager.library, KosmicDisplay);
