package org.kosmicengine.kosmic

import android.app.Service
import android.content.Intent
import android.os.Binder
import android.os.IBinder
import android.util.Log

/**
 * Godot service responsible for hosting the Godot engine instance.
 *
 * Note: Still in development, so it's made private and inaccessible until completed.
 */
private class KosmicService : Service() {

	companion object {
		private val TAG = KosmicService::class.java.simpleName
	}

	private var boundIntent: Intent? = null
	private val kosmic by lazy {
		Kosmic(applicationContext)
	}

	override fun onCreate() {
		super.onCreate()
	}

	override fun onDestroy() {
		super.onDestroy()
	}

	override fun onBind(intent: Intent?): IBinder? {
		if (boundIntent != null) {
			Log.d(TAG, "KosmicService already bound")
			return null
		}

		boundIntent = intent
		return KosmicHandle(kosmic)
	}

	override fun onRebind(intent: Intent?) {
		super.onRebind(intent)
	}

	override fun onUnbind(intent: Intent?): Boolean {
		return super.onUnbind(intent)
	}

	override fun onTaskRemoved(rootIntent: Intent?) {
		super.onTaskRemoved(rootIntent)
	}

	class KosmicHandle(val kosmic: Kosmic) : Binder()
}
