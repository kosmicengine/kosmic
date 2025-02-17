/**************************************************************************/
/*  KosmicRenderer.java                                                    */
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

package org.kosmicengine.kosmic.gl;

import org.kosmicengine.kosmic.KosmicLib;
import org.kosmicengine.kosmic.plugin.KosmicPlugin;
import org.kosmicengine.kosmic.plugin.KosmicPluginRegistry;

import android.util.Log;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

/**
 * Godot's GL renderer implementation.
 */
public class KosmicRenderer implements GLSurfaceView.Renderer {
	private final String TAG = KosmicRenderer.class.getSimpleName();

	private final KosmicPluginRegistry pluginRegistry;
	private boolean activityJustResumed = false;

	public KosmicRenderer() {
		this.pluginRegistry = KosmicPluginRegistry.getPluginRegistry();
	}

	public boolean onDrawFrame(GL10 gl) {
		if (activityJustResumed) {
			KosmicLib.onRendererResumed();
			activityJustResumed = false;
		}

		boolean swapBuffers = KosmicLib.step();
		for (KosmicPlugin plugin : pluginRegistry.getAllPlugins()) {
			plugin.onGLDrawFrame(gl);
		}

		return swapBuffers;
	}

	@Override
	public void onRenderThreadExiting() {
		Log.d(TAG, "Destroying Kosmic Engine");
		KosmicLib.ondestroy();
	}

	public void onSurfaceChanged(GL10 gl, int width, int height) {
		KosmicLib.resize(null, width, height);
		for (KosmicPlugin plugin : pluginRegistry.getAllPlugins()) {
			plugin.onGLSurfaceChanged(gl, width, height);
		}
	}

	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		KosmicLib.newcontext(null);
		for (KosmicPlugin plugin : pluginRegistry.getAllPlugins()) {
			plugin.onGLSurfaceCreated(gl, config);
		}
	}

	public void onActivityResumed() {
		// We defer invoking KosmicLib.onRendererResumed() until the first draw frame call.
		// This ensures we have a valid GL context and surface when we do so.
		activityJustResumed = true;
	}

	public void onActivityPaused() {
		KosmicLib.onRendererPaused();
	}
}
