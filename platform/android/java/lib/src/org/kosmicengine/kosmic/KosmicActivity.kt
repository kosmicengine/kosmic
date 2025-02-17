/**************************************************************************/
/*  KosmicActivity.kt                                                      */
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

package org.kosmicengine.kosmic

import android.app.Activity
import android.content.Intent
import android.content.pm.PackageManager
import android.os.Bundle
import android.util.Log
import androidx.annotation.CallSuper
import androidx.annotation.LayoutRes
import androidx.fragment.app.FragmentActivity
import org.kosmicengine.kosmic.utils.PermissionsUtil
import org.kosmicengine.kosmic.utils.ProcessPhoenix

/**
 * Base abstract activity for Android apps intending to use Godot as the primary screen.
 *
 * Also a reference implementation for how to setup and use the [KosmicFragment] fragment
 * within an Android app.
 */
abstract class KosmicActivity : FragmentActivity(), KosmicHost {

	companion object {
		private val TAG = KosmicActivity::class.java.simpleName

		@JvmStatic
		protected val EXTRA_NEW_LAUNCH = "new_launch_requested"
	}

	/**
	 * Interaction with the [Godot] object is delegated to the [KosmicFragment] class.
	 */
	protected var kosmicFragment: KosmicFragment? = null
		private set

	override fun onCreate(savedInstanceState: Bundle?) {
		super.onCreate(savedInstanceState)
		setContentView(getKosmicAppLayout())

		handleStartIntent(intent, true)

		val currentFragment = supportFragmentManager.findFragmentById(R.id.kosmic_fragment_container)
		if (currentFragment is KosmicFragment) {
			Log.v(TAG, "Reusing existing Godot fragment instance.")
			kosmicFragment = currentFragment
		} else {
			Log.v(TAG, "Creating new Godot fragment instance.")
			kosmicFragment = initKosmicInstance()
			supportFragmentManager.beginTransaction().replace(R.id.kosmic_fragment_container, kosmicFragment!!).setPrimaryNavigationFragment(kosmicFragment).commitNowAllowingStateLoss()
		}
	}

	@LayoutRes
	protected open fun getKosmicAppLayout() = R.layout.kosmic_app_layout

	override fun onDestroy() {
		Log.v(TAG, "Destroying KosmicActivity $this...")
		super.onDestroy()
	}

	override fun onKosmicForceQuit(instance: Godot) {
		runOnUiThread { terminateKosmicInstance(instance) }
	}

	private fun terminateKosmicInstance(instance: Godot) {
		kosmicFragment?.let {
			if (instance === it.kosmic) {
				Log.v(TAG, "Force quitting Godot instance")
				ProcessPhoenix.forceQuit(this)
			}
		}
	}

	override fun onKosmicRestartRequested(instance: Godot) {
		runOnUiThread {
			kosmicFragment?.let {
				if (instance === it.kosmic) {
					// It's very hard to properly de-initialize Godot on Android to restart the game
					// from scratch. Therefore, we need to kill the whole app process and relaunch it.
					//
					// Restarting only the activity, wouldn't be enough unless it did proper cleanup (including
					// releasing and reloading native libs or resetting their state somehow and clearing static data).
					Log.v(TAG, "Restarting Godot instance...")
					ProcessPhoenix.triggerRebirth(this)
				}
			}
		}
	}

	override fun onNewIntent(newIntent: Intent) {
		super.onNewIntent(newIntent)
		intent = newIntent

		handleStartIntent(newIntent, false)

		kosmicFragment?.onNewIntent(newIntent)
	}

	private fun handleStartIntent(intent: Intent, newLaunch: Boolean) {
		if (!newLaunch) {
			val newLaunchRequested = intent.getBooleanExtra(EXTRA_NEW_LAUNCH, false)
			if (newLaunchRequested) {
				Log.d(TAG, "New launch requested, restarting..")
				val restartIntent = Intent(intent).putExtra(EXTRA_NEW_LAUNCH, false)
				ProcessPhoenix.triggerRebirth(this, restartIntent)
				return
			}
		}
	}

	@CallSuper
	override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
		super.onActivityResult(requestCode, resultCode, data)
		kosmicFragment?.onActivityResult(requestCode, resultCode, data)
	}

	@CallSuper
	override fun onRequestPermissionsResult(requestCode: Int, permissions: Array<String>, grantResults: IntArray) {
		super.onRequestPermissionsResult(requestCode, permissions, grantResults)
		kosmicFragment?.onRequestPermissionsResult(requestCode, permissions, grantResults)

		// Logging the result of permission requests
		if (requestCode == PermissionsUtil.REQUEST_ALL_PERMISSION_REQ_CODE || requestCode == PermissionsUtil.REQUEST_SINGLE_PERMISSION_REQ_CODE) {
			Log.d(TAG, "Received permissions request result..")
			for (i in permissions.indices) {
				val permissionGranted = grantResults[i] == PackageManager.PERMISSION_GRANTED
				Log.d(TAG, "Permission ${permissions[i]} ${if (permissionGranted) { "granted"} else { "denied" }}")
			}
		}
	}

	override fun onBackPressed() {
		kosmicFragment?.onBackPressed() ?: super.onBackPressed()
	}

	override fun getActivity(): Activity? {
		return this
	}

	override fun getKosmic(): Kosmic? {
		return kosmicFragment?.kosmic
	}

	/**
	 * Used to initialize the Godot fragment instance in [onCreate].
	 */
	protected open fun initKosmicInstance(): KosmicFragment {
		return KosmicFragment()
	}
}
