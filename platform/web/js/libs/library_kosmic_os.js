/**************************************************************************/
/*  library_kosmic_os.js                                                   */
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

const IDHandler = {
	$IDHandler: {
		_last_id: 0,
		_references: {},

		get: function (p_id) {
			return IDHandler._references[p_id];
		},

		add: function (p_data) {
			const id = ++IDHandler._last_id;
			IDHandler._references[id] = p_data;
			return id;
		},

		remove: function (p_id) {
			delete IDHandler._references[p_id];
		},
	},
};

autoAddDeps(IDHandler, '$IDHandler');
mergeInto(LibraryManager.library, IDHandler);

const KosmicConfig = {
	$KosmicConfig__postset: 'Module["initConfig"] = KosmicConfig.init_config;',
	$KosmicConfig__deps: ['$KosmicRuntime'],
	$KosmicConfig: {
		canvas: null,
		locale: 'en',
		canvas_resize_policy: 2, // Adaptive
		virtual_keyboard: false,
		persistent_drops: false,
		on_execute: null,
		on_exit: null,

		init_config: function (p_opts) {
			KosmicConfig.canvas_resize_policy = p_opts['canvasResizePolicy'];
			KosmicConfig.canvas = p_opts['canvas'];
			KosmicConfig.locale = p_opts['locale'] || KosmicConfig.locale;
			KosmicConfig.virtual_keyboard = p_opts['virtualKeyboard'];
			KosmicConfig.persistent_drops = !!p_opts['persistentDrops'];
			KosmicConfig.on_execute = p_opts['onExecute'];
			KosmicConfig.on_exit = p_opts['onExit'];
			if (p_opts['focusCanvas']) {
				KosmicConfig.canvas.focus();
			}
		},

		locate_file: function (file) {
			return Module['locateFile'](file);
		},
		clear: function () {
			KosmicConfig.canvas = null;
			KosmicConfig.locale = 'en';
			KosmicConfig.canvas_resize_policy = 2;
			KosmicConfig.virtual_keyboard = false;
			KosmicConfig.persistent_drops = false;
			KosmicConfig.on_execute = null;
			KosmicConfig.on_exit = null;
		},
	},

	kosmic_js_config_canvas_id_get__proxy: 'sync',
	kosmic_js_config_canvas_id_get__sig: 'vii',
	kosmic_js_config_canvas_id_get: function (p_ptr, p_ptr_max) {
		KosmicRuntime.stringToHeap(`#${KosmicConfig.canvas.id}`, p_ptr, p_ptr_max);
	},

	kosmic_js_config_locale_get__proxy: 'sync',
	kosmic_js_config_locale_get__sig: 'vii',
	kosmic_js_config_locale_get: function (p_ptr, p_ptr_max) {
		KosmicRuntime.stringToHeap(KosmicConfig.locale, p_ptr, p_ptr_max);
	},
};

autoAddDeps(KosmicConfig, '$KosmicConfig');
mergeInto(LibraryManager.library, KosmicConfig);

const KosmicFS = {
	$KosmicFS__deps: ['$FS', '$IDBFS', '$KosmicRuntime'],
	$KosmicFS__postset: [
		'Module["initFS"] = KosmicFS.init;',
		'Module["copyToFS"] = KosmicFS.copy_to_fs;',
	].join(''),
	$KosmicFS: {
		// ERRNO_CODES works every odd version of emscripten, but this will break too eventually.
		ENOENT: 44,
		_idbfs: false,
		_syncing: false,
		_mount_points: [],

		is_persistent: function () {
			return KosmicFS._idbfs ? 1 : 0;
		},

		// Initialize godot file system, setting up persistent paths.
		// Returns a promise that resolves when the FS is ready.
		// We keep track of mount_points, so that we can properly close the IDBFS
		// since emscripten is not doing it by itself. (emscripten GH#12516).
		init: function (persistentPaths) {
			KosmicFS._idbfs = false;
			if (!Array.isArray(persistentPaths)) {
				return Promise.reject(new Error('Persistent paths must be an array'));
			}
			if (!persistentPaths.length) {
				return Promise.resolve();
			}
			KosmicFS._mount_points = persistentPaths.slice();

			function createRecursive(dir) {
				try {
					FS.stat(dir);
				} catch (e) {
					if (e.errno !== KosmicFS.ENOENT) {
						// Let mkdirTree throw in case, we cannot trust the above check.
						KosmicRuntime.error(e);
					}
					FS.mkdirTree(dir);
				}
			}

			KosmicFS._mount_points.forEach(function (path) {
				createRecursive(path);
				FS.mount(IDBFS, {}, path);
			});
			return new Promise(function (resolve, reject) {
				FS.syncfs(true, function (err) {
					if (err) {
						KosmicFS._mount_points = [];
						KosmicFS._idbfs = false;
						KosmicRuntime.print(`IndexedDB not available: ${err.message}`);
					} else {
						KosmicFS._idbfs = true;
					}
					resolve(err);
				});
			});
		},

		// Deinit godot file system, making sure to unmount file systems, and close IDBFS(s).
		deinit: function () {
			KosmicFS._mount_points.forEach(function (path) {
				try {
					FS.unmount(path);
				} catch (e) {
					KosmicRuntime.print('Already unmounted', e);
				}
				if (KosmicFS._idbfs && IDBFS.dbs[path]) {
					IDBFS.dbs[path].close();
					delete IDBFS.dbs[path];
				}
			});
			KosmicFS._mount_points = [];
			KosmicFS._idbfs = false;
			KosmicFS._syncing = false;
		},

		sync: function () {
			if (KosmicFS._syncing) {
				KosmicRuntime.error('Already syncing!');
				return Promise.resolve();
			}
			KosmicFS._syncing = true;
			return new Promise(function (resolve, reject) {
				FS.syncfs(false, function (error) {
					if (error) {
						KosmicRuntime.error(`Failed to save IDB file system: ${error.message}`);
					}
					KosmicFS._syncing = false;
					resolve(error);
				});
			});
		},

		// Copies a buffer to the internal file system. Creating directories recursively.
		copy_to_fs: function (path, buffer) {
			const idx = path.lastIndexOf('/');
			let dir = '/';
			if (idx > 0) {
				dir = path.slice(0, idx);
			}
			try {
				FS.stat(dir);
			} catch (e) {
				if (e.errno !== KosmicFS.ENOENT) {
					// Let mkdirTree throw in case, we cannot trust the above check.
					KosmicRuntime.error(e);
				}
				FS.mkdirTree(dir);
			}
			FS.writeFile(path, new Uint8Array(buffer));
		},
	},
};
mergeInto(LibraryManager.library, KosmicFS);

const KosmicOS = {
	$KosmicOS__deps: ['$KosmicRuntime', '$KosmicConfig', '$KosmicFS'],
	$KosmicOS__postset: [
		'Module["request_quit"] = function() { KosmicOS.request_quit() };',
		'Module["onExit"] = KosmicOS.cleanup;',
		'KosmicOS._fs_sync_promise = Promise.resolve();',
	].join(''),
	$KosmicOS: {
		request_quit: function () {},
		_async_cbs: [],
		_fs_sync_promise: null,

		atexit: function (p_promise_cb) {
			KosmicOS._async_cbs.push(p_promise_cb);
		},

		cleanup: function (exit_code) {
			const cb = KosmicConfig.on_exit;
			KosmicFS.deinit();
			KosmicConfig.clear();
			if (cb) {
				cb(exit_code);
			}
		},

		finish_async: function (callback) {
			KosmicOS._fs_sync_promise.then(function (err) {
				const promises = [];
				KosmicOS._async_cbs.forEach(function (cb) {
					promises.push(new Promise(cb));
				});
				return Promise.all(promises);
			}).then(function () {
				return KosmicFS.sync(); // Final FS sync.
			}).then(function (err) {
				// Always deferred.
				setTimeout(function () {
					callback();
				}, 0);
			});
		},
	},

	kosmic_js_os_finish_async__proxy: 'sync',
	kosmic_js_os_finish_async__sig: 'vi',
	kosmic_js_os_finish_async: function (p_callback) {
		const func = KosmicRuntime.get_func(p_callback);
		KosmicOS.finish_async(func);
	},

	kosmic_js_os_request_quit_cb__proxy: 'sync',
	kosmic_js_os_request_quit_cb__sig: 'vi',
	kosmic_js_os_request_quit_cb: function (p_callback) {
		KosmicOS.request_quit = KosmicRuntime.get_func(p_callback);
	},

	kosmic_js_os_fs_is_persistent__proxy: 'sync',
	kosmic_js_os_fs_is_persistent__sig: 'i',
	kosmic_js_os_fs_is_persistent: function () {
		return KosmicFS.is_persistent();
	},

	kosmic_js_os_fs_sync__proxy: 'sync',
	kosmic_js_os_fs_sync__sig: 'vi',
	kosmic_js_os_fs_sync: function (callback) {
		const func = KosmicRuntime.get_func(callback);
		KosmicOS._fs_sync_promise = KosmicFS.sync();
		KosmicOS._fs_sync_promise.then(function (err) {
			func();
		});
	},

	kosmic_js_os_has_feature__proxy: 'sync',
	kosmic_js_os_has_feature__sig: 'ii',
	kosmic_js_os_has_feature: function (p_ftr) {
		const ftr = KosmicRuntime.parseString(p_ftr);
		const ua = navigator.userAgent;
		if (ftr === 'web_macos') {
			return (ua.indexOf('Mac') !== -1) ? 1 : 0;
		}
		if (ftr === 'web_windows') {
			return (ua.indexOf('Windows') !== -1) ? 1 : 0;
		}
		if (ftr === 'web_android') {
			return (ua.indexOf('Android') !== -1) ? 1 : 0;
		}
		if (ftr === 'web_ios') {
			return ((ua.indexOf('iPhone') !== -1) || (ua.indexOf('iPad') !== -1) || (ua.indexOf('iPod') !== -1)) ? 1 : 0;
		}
		if (ftr === 'web_linuxbsd') {
			return ((ua.indexOf('CrOS') !== -1) || (ua.indexOf('BSD') !== -1) || (ua.indexOf('Linux') !== -1) || (ua.indexOf('X11') !== -1)) ? 1 : 0;
		}
		return 0;
	},

	kosmic_js_os_execute__proxy: 'sync',
	kosmic_js_os_execute__sig: 'ii',
	kosmic_js_os_execute: function (p_json) {
		const json_args = KosmicRuntime.parseString(p_json);
		const args = JSON.parse(json_args);
		if (KosmicConfig.on_execute) {
			KosmicConfig.on_execute(args);
			return 0;
		}
		return 1;
	},

	kosmic_js_os_shell_open__proxy: 'sync',
	kosmic_js_os_shell_open__sig: 'vi',
	kosmic_js_os_shell_open: function (p_uri) {
		window.open(KosmicRuntime.parseString(p_uri), '_blank');
	},

	kosmic_js_os_hw_concurrency_get__proxy: 'sync',
	kosmic_js_os_hw_concurrency_get__sig: 'i',
	kosmic_js_os_hw_concurrency_get: function () {
		// TODO Godot core needs fixing to avoid spawning too many threads (> 24).
		const concurrency = navigator.hardwareConcurrency || 1;
		return concurrency < 2 ? concurrency : 2;
	},

	kosmic_js_os_download_buffer__proxy: 'sync',
	kosmic_js_os_download_buffer__sig: 'viiii',
	kosmic_js_os_download_buffer: function (p_ptr, p_size, p_name, p_mime) {
		const buf = KosmicRuntime.heapSlice(HEAP8, p_ptr, p_size);
		const name = KosmicRuntime.parseString(p_name);
		const mime = KosmicRuntime.parseString(p_mime);
		const blob = new Blob([buf], { type: mime });
		const url = window.URL.createObjectURL(blob);
		const a = document.createElement('a');
		a.href = url;
		a.download = name;
		a.style.display = 'none';
		document.body.appendChild(a);
		a.click();
		a.remove();
		window.URL.revokeObjectURL(url);
	},
};

autoAddDeps(KosmicOS, '$KosmicOS');
mergeInto(LibraryManager.library, KosmicOS);

/*
 * Godot event listeners.
 * Keeps track of registered event listeners so it can remove them on shutdown.
 */
const KosmicEventListeners = {
	$KosmicEventListeners__deps: ['$KosmicOS'],
	$KosmicEventListeners__postset: 'KosmicOS.atexit(function(resolve, reject) { KosmicEventListeners.clear(); resolve(); });',
	$KosmicEventListeners: {
		handlers: [],

		has: function (target, event, method, capture) {
			return KosmicEventListeners.handlers.findIndex(function (e) {
				return e.target === target && e.event === event && e.method === method && e.capture === capture;
			}) !== -1;
		},

		add: function (target, event, method, capture) {
			if (KosmicEventListeners.has(target, event, method, capture)) {
				return;
			}
			function Handler(p_target, p_event, p_method, p_capture) {
				this.target = p_target;
				this.event = p_event;
				this.method = p_method;
				this.capture = p_capture;
			}
			KosmicEventListeners.handlers.push(new Handler(target, event, method, capture));
			target.addEventListener(event, method, capture);
		},

		clear: function () {
			KosmicEventListeners.handlers.forEach(function (h) {
				h.target.removeEventListener(h.event, h.method, h.capture);
			});
			KosmicEventListeners.handlers.length = 0;
		},
	},
};
mergeInto(LibraryManager.library, KosmicEventListeners);

const KosmicPWA = {

	$KosmicPWA__deps: ['$KosmicRuntime', '$KosmicEventListeners'],
	$KosmicPWA: {
		hasUpdate: false,

		updateState: function (cb, reg) {
			if (!reg) {
				return;
			}
			if (!reg.active) {
				return;
			}
			if (reg.waiting) {
				KosmicPWA.hasUpdate = true;
				cb();
			}
			KosmicEventListeners.add(reg, 'updatefound', function () {
				const installing = reg.installing;
				KosmicEventListeners.add(installing, 'statechange', function () {
					if (installing.state === 'installed') {
						KosmicPWA.hasUpdate = true;
						cb();
					}
				});
			});
		},
	},

	kosmic_js_pwa_cb__proxy: 'sync',
	kosmic_js_pwa_cb__sig: 'vi',
	kosmic_js_pwa_cb: function (p_update_cb) {
		if ('serviceWorker' in navigator) {
			try {
				const cb = KosmicRuntime.get_func(p_update_cb);
				navigator.serviceWorker.getRegistration().then(KosmicPWA.updateState.bind(null, cb));
			} catch (e) {
				KosmicRuntime.error('Failed to assign PWA callback', e);
			}
		}
	},

	kosmic_js_pwa_update__proxy: 'sync',
	kosmic_js_pwa_update__sig: 'i',
	kosmic_js_pwa_update: function () {
		if ('serviceWorker' in navigator && KosmicPWA.hasUpdate) {
			try {
				navigator.serviceWorker.getRegistration().then(function (reg) {
					if (!reg || !reg.waiting) {
						return;
					}
					reg.waiting.postMessage('update');
				});
			} catch (e) {
				KosmicRuntime.error(e);
				return 1;
			}
			return 0;
		}
		return 1;
	},
};

autoAddDeps(KosmicPWA, '$KosmicPWA');
mergeInto(LibraryManager.library, KosmicPWA);
