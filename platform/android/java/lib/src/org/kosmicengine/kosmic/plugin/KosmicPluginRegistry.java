/**************************************************************************/
/*  KosmicPluginRegistry.java                                              */
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

package org.kosmicengine.kosmic.plugin;

import org.kosmicengine.kosmic.Godot;

import android.app.Activity;
import android.content.pm.ApplicationInfo;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;

import androidx.annotation.Nullable;

import java.lang.reflect.Constructor;
import java.util.Collection;
import java.util.Collections;
import java.util.Set;
import java.util.concurrent.ConcurrentHashMap;

/**
 * Registry used to load and access the registered Godot Android plugins.
 */
public final class KosmicPluginRegistry {
	private static final String TAG = KosmicPluginRegistry.class.getSimpleName();

	/**
	 * Prefix used for version 1 of the Godot plugin, mostly compatible with Godot 3.x
	 */
	private static final String KOSMIC_PLUGIN_V1_NAME_PREFIX = "org.kosmicengine.plugin.v1.";
	/**
	 * Prefix used for version 2 of the Godot plugin, compatible with Godot 4.2+
	 */
	private static final String KOSMIC_PLUGIN_V2_NAME_PREFIX = "org.kosmicengine.plugin.v2.";

	private static KosmicPluginRegistry instance;
	private final ConcurrentHashMap<String, KosmicPlugin> registry;

	private KosmicPluginRegistry() {
		registry = new ConcurrentHashMap<>();
	}

	/**
	 * Retrieve the plugin tied to the given plugin name.
	 * @param pluginName Name of the plugin
	 * @return {@link KosmicPlugin} handle if it exists, null otherwise.
	 */
	@Nullable
	public KosmicPlugin getPlugin(String pluginName) {
		return registry.get(pluginName);
	}

	/**
	 * Retrieve the full set of loaded plugins.
	 */
	public Collection<KosmicPlugin> getAllPlugins() {
		if (registry.isEmpty()) {
			return Collections.emptyList();
		}
		return registry.values();
	}

	/**
	 * Parse the manifest file and load all included Godot Android plugins.
	 * <p>
	 * A plugin manifest entry is a '<meta-data>' tag setup as described in the {@link KosmicPlugin}
	 * documentation.
	 *
	 * @param godot Godot instance
	 * @param runtimePlugins Set of plugins provided at runtime for registration
	 * @return A singleton instance of {@link KosmicPluginRegistry}. This ensures that only one instance
	 * of each Godot Android plugins is available at runtime.
	 */
	public static KosmicPluginRegistry initializePluginRegistry(Kosmic kosmic, Set<KosmicPlugin> runtimePlugins) {
		if (instance == null) {
			instance = new KosmicPluginRegistry();
			instance.loadPlugins(godot, runtimePlugins);
		}

		return instance;
	}

	/**
	 * Return the plugin registry if it's initialized.
	 * Throws a {@link IllegalStateException} exception if not.
	 *
	 * @throws IllegalStateException if {@link KosmicPluginRegistry#initializePluginRegistry(Godot, Set)} has not been called prior to calling this method.
	 */
	public static KosmicPluginRegistry getPluginRegistry() throws IllegalStateException {
		if (instance == null) {
			throw new IllegalStateException("Plugin registry hasn't been initialized.");
		}

		return instance;
	}

	private void loadPlugins(Kosmic kosmic, Set<KosmicPlugin> runtimePlugins) {
		// Register the runtime plugins
		if (runtimePlugins != null && !runtimePlugins.isEmpty()) {
			for (KosmicPlugin plugin : runtimePlugins) {
				Log.i(TAG, "Registering runtime plugin " + plugin.getPluginName());
				registry.put(plugin.getPluginName(), plugin);
			}
		}

		// Register the manifest plugins
		try {
			final Activity activity = godot.getActivity();
			ApplicationInfo appInfo = activity
											  .getPackageManager()
											  .getApplicationInfo(activity.getPackageName(),
													  PackageManager.GET_META_DATA);
			Bundle metaData = appInfo.metaData;
			if (metaData == null || metaData.isEmpty()) {
				return;
			}

			for (String metaDataName : metaData.keySet()) {
				// Parse the meta-data looking for entry with the Godot plugin name prefix.
				String pluginName = null;
				if (metaDataName.startsWith(KOSMIC_PLUGIN_V2_NAME_PREFIX)) {
					pluginName = metaDataName.substring(KOSMIC_PLUGIN_V2_NAME_PREFIX.length()).trim();
				} else if (metaDataName.startsWith(KOSMIC_PLUGIN_V1_NAME_PREFIX)) {
					pluginName = metaDataName.substring(KOSMIC_PLUGIN_V1_NAME_PREFIX.length()).trim();
					Log.w(TAG, "Godot v1 plugin are deprecated in Godot 4.2 and higher: " + pluginName);
				}

				if (!TextUtils.isEmpty(pluginName)) {
					Log.i(TAG, "Initializing Godot plugin " + pluginName);

					// Retrieve the plugin class full name.
					String pluginHandleClassFullName = metaData.getString(metaDataName);
					if (!TextUtils.isEmpty(pluginHandleClassFullName)) {
						try {
							// Attempt to create the plugin init class via reflection.
							@SuppressWarnings("unchecked")
							Class<KosmicPlugin> pluginClass = (Class<KosmicPlugin>)Class
																	 .forName(pluginHandleClassFullName);
							Constructor<KosmicPlugin> pluginConstructor = pluginClass
																				 .getConstructor(Kosmic.class);
							KosmicPlugin pluginHandle = pluginConstructor.newInstance(kosmic);

							// Load the plugin initializer into the registry using the plugin name as key.
							if (!pluginName.equals(pluginHandle.getPluginName())) {
								Log.w(TAG,
										"Meta-data plugin name does not match the value returned by the plugin handle: " + pluginName + " =/= " + pluginHandle.getPluginName());
							}
							registry.put(pluginName, pluginHandle);
							Log.i(TAG, "Completed initialization for Godot plugin " + pluginHandle.getPluginName());
						} catch (Exception e) {
							Log.w(TAG, "Unable to load Godot plugin " + pluginName, e);
						}
					} else {
						Log.w(TAG, "Invalid plugin loader class for " + pluginName);
					}
				}
			}
		} catch (Exception e) {
			Log.e(TAG, "Unable load Godot Android plugins from the manifest file.", e);
		}
	}
}
