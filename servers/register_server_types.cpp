/**************************************************************************/
/*  register_server_types.cpp                                             */
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

#include "register_server_types.h"

#include "core/config/engine.h"
#include "core/config/project_settings.h"

#include "audio/audio_effect.h"
#include "audio/audio_stream.h"
#include "audio/effects/audio_effect_amplify.h"
#include "audio/effects/audio_effect_capture.h"
#include "audio/effects/audio_effect_chorus.h"
#include "audio/effects/audio_effect_compressor.h"
#include "audio/effects/audio_effect_delay.h"
#include "audio/effects/audio_effect_distortion.h"
#include "audio/effects/audio_effect_eq.h"
#include "audio/effects/audio_effect_filter.h"
#include "audio/effects/audio_effect_hard_limiter.h"
#include "audio/effects/audio_effect_limiter.h"
#include "audio/effects/audio_effect_panner.h"
#include "audio/effects/audio_effect_phaser.h"
#include "audio/effects/audio_effect_pitch_shift.h"
#include "audio/effects/audio_effect_record.h"
#include "audio/effects/audio_effect_reverb.h"
#include "audio/effects/audio_effect_spectrum_analyzer.h"
#include "audio/effects/audio_effect_stereo_enhance.h"
#include "audio/effects/audio_stream_generator.h"
#include "audio_server.h"
#include "camera/camera_feed.h"
#include "camera_server.h"
#include "debugger/servers_debugger.h"
#include "display/native_menu.h"
#include "display_server.h"
#include "movie_writer/movie_writer.h"
#include "movie_writer/movie_writer_mjpeg.h"
#include "movie_writer/movie_writer_pngwav.h"
#include "rendering/renderer_rd/framebuffer_cache_rd.h"
#include "rendering/renderer_rd/storage_rd/render_data_rd.h"
#include "rendering/renderer_rd/storage_rd/render_scene_buffers_rd.h"
#include "rendering/renderer_rd/storage_rd/render_scene_data_rd.h"
#include "rendering/renderer_rd/uniform_set_cache_rd.h"
#include "rendering/rendering_device.h"
#include "rendering/rendering_device_binds.h"
#include "rendering/shader_include_db.h"
#include "rendering/storage/render_data.h"
#include "rendering/storage/render_scene_buffers.h"
#include "rendering/storage/render_scene_data.h"
#include "rendering_server.h"
#include "servers/rendering/shader_types.h"
#include "text/text_server_dummy.h"
#include "text/text_server_extension.h"
#include "text_server.h"

// 2D physics and navigation.
#include "navigation_server_2d.h"
#include "physics_server_2d.h"
#include "physics_server_2d_dummy.h"
#include "servers/extensions/physics_server_2d_extension.h"

// 3D physics and navigation (3D navigation is needed for 2D).
#include "navigation_server_3d.h"
#ifndef _3D_DISABLED
#include "physics_server_3d.h"
#include "physics_server_3d_dummy.h"
#include "servers/extensions/physics_server_3d_extension.h"
#include "xr/xr_body_tracker.h"
#include "xr/xr_controller_tracker.h"
#include "xr/xr_face_tracker.h"
#include "xr/xr_hand_tracker.h"
#include "xr/xr_interface.h"
#include "xr/xr_interface_extension.h"
#include "xr/xr_positional_tracker.h"
#include "xr_server.h"
#endif // _3D_DISABLED

ShaderTypes *shader_types = nullptr;

#ifndef _3D_DISABLED
static PhysicsServer3D *_create_dummy_physics_server_3d() {
	return memnew(PhysicsServer3DDummy);
}
#endif // _3D_DISABLED

static PhysicsServer2D *_create_dummy_physics_server_2d() {
	return memnew(PhysicsServer2DDummy);
}

static bool has_server_feature_callback(const String &p_feature) {
	if (RenderingServer::get_singleton()) {
		if (RenderingServer::get_singleton()->has_os_feature(p_feature)) {
			return true;
		}
	}

	return false;
}

static MovieWriterMJPEG *writer_mjpeg = nullptr;
static MovieWriterPNGWAV *writer_pngwav = nullptr;

void register_server_types() {
	OS::get_singleton()->benchmark_begin_measure("Servers", "Register Extensions");

	shader_types = memnew(ShaderTypes);

	KSREGISTER_CLASS(TextServerManager);
	KSREGISTER_ABSTRACT_CLASS(TextServer);
	KSREGISTER_CLASS(TextServerExtension);
	KSREGISTER_CLASS(TextServerDummy);

	KSREGISTER_NATIVE_STRUCT(Glyph, "int start = -1;int end = -1;uint8_t count = 0;uint8_t repeat = 1;uint16_t flags = 0;float x_off = 0.f;float y_off = 0.f;float advance = 0.f;RID font_rid;int font_size = 0;int32_t index = 0");
	KSREGISTER_NATIVE_STRUCT(CaretInfo, "Rect2 leading_caret;Rect2 trailing_caret;TextServer::Direction leading_direction;TextServer::Direction trailing_direction");

	Engine::get_singleton()->add_singleton(Engine::Singleton("TextServerManager", TextServerManager::get_singleton(), "TextServerManager"));

	OS::get_singleton()->set_has_server_feature_callback(has_server_feature_callback);

	KSREGISTER_ABSTRACT_CLASS(DisplayServer);
	KSREGISTER_ABSTRACT_CLASS(RenderingServer);
	KSREGISTER_CLASS(AudioServer);

	KSREGISTER_CLASS(NativeMenu);

	KSREGISTER_CLASS(CameraServer);

	KSREGISTER_ABSTRACT_CLASS(RenderingDevice);

	KSREGISTER_CLASS(AudioStream);
	KSREGISTER_CLASS(AudioStreamPlayback);
	KSREGISTER_VIRTUAL_CLASS(AudioStreamPlaybackResampled);
	KSREGISTER_CLASS(AudioStreamMicrophone);
	KSREGISTER_CLASS(AudioStreamRandomizer);
	KSREGISTER_CLASS(AudioSample);
	KSREGISTER_CLASS(AudioSamplePlayback);
	KSREGISTER_VIRTUAL_CLASS(AudioEffect);
	KSREGISTER_VIRTUAL_CLASS(AudioEffectInstance);
	KSREGISTER_CLASS(AudioEffectEQ);
	KSREGISTER_CLASS(AudioEffectFilter);
	KSREGISTER_CLASS(AudioBusLayout);

	KSREGISTER_CLASS(AudioStreamGenerator);
	KSREGISTER_ABSTRACT_CLASS(AudioStreamGeneratorPlayback);

	{
		//audio effects
		KSREGISTER_CLASS(AudioEffectAmplify);

		KSREGISTER_CLASS(AudioEffectReverb);

		KSREGISTER_CLASS(AudioEffectLowPassFilter);
		KSREGISTER_CLASS(AudioEffectHighPassFilter);
		KSREGISTER_CLASS(AudioEffectBandPassFilter);
		KSREGISTER_CLASS(AudioEffectNotchFilter);
		KSREGISTER_CLASS(AudioEffectBandLimitFilter);
		KSREGISTER_CLASS(AudioEffectLowShelfFilter);
		KSREGISTER_CLASS(AudioEffectHighShelfFilter);

		KSREGISTER_CLASS(AudioEffectEQ6);
		KSREGISTER_CLASS(AudioEffectEQ10);
		KSREGISTER_CLASS(AudioEffectEQ21);

		KSREGISTER_CLASS(AudioEffectDistortion);

		KSREGISTER_CLASS(AudioEffectStereoEnhance);

		KSREGISTER_CLASS(AudioEffectPanner);
		KSREGISTER_CLASS(AudioEffectChorus);
		KSREGISTER_CLASS(AudioEffectDelay);
		KSREGISTER_CLASS(AudioEffectCompressor);
		KSREGISTER_CLASS(AudioEffectLimiter);
		KSREGISTER_CLASS(AudioEffectHardLimiter);
		KSREGISTER_CLASS(AudioEffectPitchShift);
		KSREGISTER_CLASS(AudioEffectPhaser);

		KSREGISTER_CLASS(AudioEffectRecord);
		KSREGISTER_CLASS(AudioEffectSpectrumAnalyzer);
		KSREGISTER_ABSTRACT_CLASS(AudioEffectSpectrumAnalyzerInstance);

		KSREGISTER_CLASS(AudioEffectCapture);
	}

	KSREGISTER_ABSTRACT_CLASS(RenderingDevice);
	KSREGISTER_CLASS(ShaderIncludeDB);
	KSREGISTER_CLASS(RDTextureFormat);
	KSREGISTER_CLASS(RDTextureView);
	KSREGISTER_CLASS(RDAttachmentFormat);
	KSREGISTER_CLASS(RDFramebufferPass);
	KSREGISTER_CLASS(RDSamplerState);
	KSREGISTER_CLASS(RDVertexAttribute);
	KSREGISTER_CLASS(RDUniform);
	KSREGISTER_CLASS(RDPipelineRasterizationState);
	KSREGISTER_CLASS(RDPipelineMultisampleState);
	KSREGISTER_CLASS(RDPipelineDepthStencilState);
	KSREGISTER_CLASS(RDPipelineColorBlendStateAttachment);
	KSREGISTER_CLASS(RDPipelineColorBlendState);
	KSREGISTER_CLASS(RDShaderSource);
	KSREGISTER_CLASS(RDShaderSPIRV);
	KSREGISTER_CLASS(RDShaderFile);
	KSREGISTER_CLASS(RDPipelineSpecializationConstant);

	KSREGISTER_ABSTRACT_CLASS(RenderData);
	KSREGISTER_CLASS(RenderDataExtension);
	KSREGISTER_CLASS(RenderDataRD);

	KSREGISTER_ABSTRACT_CLASS(RenderSceneData);
	KSREGISTER_CLASS(RenderSceneDataExtension);
	KSREGISTER_CLASS(RenderSceneDataRD);

	KSREGISTER_CLASS(RenderSceneBuffersConfiguration);
	KSREGISTER_ABSTRACT_CLASS(RenderSceneBuffers);
	KSREGISTER_CLASS(RenderSceneBuffersExtension);
	KSREGISTER_CLASS(RenderSceneBuffersRD);

	KSREGISTER_CLASS(FramebufferCacheRD);
	KSREGISTER_CLASS(UniformSetCacheRD);

	KSREGISTER_CLASS(CameraFeed);

	KSREGISTER_VIRTUAL_CLASS(MovieWriter);

	ServersDebugger::initialize();

	// Physics 2D
	KSREGISTER_CLASS(PhysicsServer2DManager);
	Engine::get_singleton()->add_singleton(Engine::Singleton("PhysicsServer2DManager", PhysicsServer2DManager::get_singleton(), "PhysicsServer2DManager"));

	KSREGISTER_ABSTRACT_CLASS(PhysicsServer2D);
	KSREGISTER_VIRTUAL_CLASS(PhysicsServer2DExtension);
	KSREGISTER_VIRTUAL_CLASS(PhysicsDirectBodyState2DExtension);
	KSREGISTER_VIRTUAL_CLASS(PhysicsDirectSpaceState2DExtension);

	KSREGISTER_NATIVE_STRUCT(PhysicsServer2DExtensionRayResult, "Vector2 position;Vector2 normal;RID rid;ObjectID collider_id;Object *collider;int shape");
	KSREGISTER_NATIVE_STRUCT(PhysicsServer2DExtensionShapeResult, "RID rid;ObjectID collider_id;Object *collider;int shape");
	KSREGISTER_NATIVE_STRUCT(PhysicsServer2DExtensionShapeRestInfo, "Vector2 point;Vector2 normal;RID rid;ObjectID collider_id;int shape;Vector2 linear_velocity");
	KSREGISTER_NATIVE_STRUCT(PhysicsServer2DExtensionMotionResult, "Vector2 travel;Vector2 remainder;Vector2 collision_point;Vector2 collision_normal;Vector2 collider_velocity;real_t collision_depth;real_t collision_safe_fraction;real_t collision_unsafe_fraction;int collision_local_shape;ObjectID collider_id;RID collider;int collider_shape");

	KSREGISTER_ABSTRACT_CLASS(PhysicsDirectBodyState2D);
	KSREGISTER_ABSTRACT_CLASS(PhysicsDirectSpaceState2D);
	KSREGISTER_CLASS(PhysicsRayQueryParameters2D);
	KSREGISTER_CLASS(PhysicsPointQueryParameters2D);
	KSREGISTER_CLASS(PhysicsShapeQueryParameters2D);
	KSREGISTER_CLASS(PhysicsTestMotionParameters2D);
	KSREGISTER_CLASS(PhysicsTestMotionResult2D);

	GLOBAL_DEF(PropertyInfo(Variant::STRING, PhysicsServer2DManager::setting_property_name, PROPERTY_HINT_ENUM, "DEFAULT"), "DEFAULT");

	PhysicsServer2DManager::get_singleton()->register_server("Dummy", callable_mp_static(_create_dummy_physics_server_2d));

	KSREGISTER_ABSTRACT_CLASS(NavigationServer2D);
	KSREGISTER_CLASS(NavigationPathQueryParameters2D);
	KSREGISTER_CLASS(NavigationPathQueryResult2D);

#ifndef _3D_DISABLED
	// Physics 3D
	KSREGISTER_CLASS(PhysicsServer3DManager);
	Engine::get_singleton()->add_singleton(Engine::Singleton("PhysicsServer3DManager", PhysicsServer3DManager::get_singleton(), "PhysicsServer3DManager"));

	KSREGISTER_ABSTRACT_CLASS(PhysicsServer3D);
	KSREGISTER_VIRTUAL_CLASS(PhysicsServer3DExtension);
	KSREGISTER_VIRTUAL_CLASS(PhysicsDirectBodyState3DExtension);
	KSREGISTER_VIRTUAL_CLASS(PhysicsDirectSpaceState3DExtension)
	KSREGISTER_VIRTUAL_CLASS(PhysicsServer3DRenderingServerHandler)

	KSREGISTER_NATIVE_STRUCT(PhysicsServer3DExtensionRayResult, "Vector3 position;Vector3 normal;RID rid;ObjectID collider_id;Object *collider;int shape;int face_index");
	KSREGISTER_NATIVE_STRUCT(PhysicsServer3DExtensionShapeResult, "RID rid;ObjectID collider_id;Object *collider;int shape");
	KSREGISTER_NATIVE_STRUCT(PhysicsServer3DExtensionShapeRestInfo, "Vector3 point;Vector3 normal;RID rid;ObjectID collider_id;int shape;Vector3 linear_velocity");
	KSREGISTER_NATIVE_STRUCT(PhysicsServer3DExtensionMotionCollision, "Vector3 position;Vector3 normal;Vector3 collider_velocity;Vector3 collider_angular_velocity;real_t depth;int local_shape;ObjectID collider_id;RID collider;int collider_shape");
	KSREGISTER_NATIVE_STRUCT(PhysicsServer3DExtensionMotionResult, "Vector3 travel;Vector3 remainder;real_t collision_depth;real_t collision_safe_fraction;real_t collision_unsafe_fraction;PhysicsServer3DExtensionMotionCollision collisions[32];int collision_count");

	KSREGISTER_ABSTRACT_CLASS(PhysicsDirectBodyState3D);
	KSREGISTER_ABSTRACT_CLASS(PhysicsDirectSpaceState3D);
	KSREGISTER_CLASS(PhysicsRayQueryParameters3D);
	KSREGISTER_CLASS(PhysicsPointQueryParameters3D);
	KSREGISTER_CLASS(PhysicsShapeQueryParameters3D);
	KSREGISTER_CLASS(PhysicsTestMotionParameters3D);
	KSREGISTER_CLASS(PhysicsTestMotionResult3D);

	GLOBAL_DEF(PropertyInfo(Variant::STRING, PhysicsServer3DManager::setting_property_name, PROPERTY_HINT_ENUM, "DEFAULT"), "DEFAULT");

	PhysicsServer3DManager::get_singleton()->register_server("Dummy", callable_mp_static(_create_dummy_physics_server_3d));

	KSREGISTER_ABSTRACT_CLASS(XRInterface);
	KSREGISTER_CLASS(XRVRS);
	KSREGISTER_CLASS(XRBodyTracker);
	KSREGISTER_CLASS(XRControllerTracker);
	KSREGISTER_CLASS(XRFaceTracker);
	KSREGISTER_CLASS(XRHandTracker);
	KSREGISTER_CLASS(XRInterfaceExtension); // can't register this as virtual because we need a creation function for our extensions.
	KSREGISTER_CLASS(XRPose);
	KSREGISTER_CLASS(XRPositionalTracker);
	KSREGISTER_CLASS(XRServer);
	KSREGISTER_ABSTRACT_CLASS(XRTracker);
#endif // _3D_DISABLED

	KSREGISTER_ABSTRACT_CLASS(NavigationServer3D);
	KSREGISTER_CLASS(NavigationPathQueryParameters3D);
	KSREGISTER_CLASS(NavigationPathQueryResult3D);

	writer_mjpeg = memnew(MovieWriterMJPEG);
	MovieWriter::add_writer(writer_mjpeg);

	writer_pngwav = memnew(MovieWriterPNGWAV);
	MovieWriter::add_writer(writer_pngwav);

	OS::get_singleton()->benchmark_end_measure("Servers", "Register Extensions");
}

void unregister_server_types() {
	OS::get_singleton()->benchmark_begin_measure("Servers", "Unregister Extensions");

	ServersDebugger::deinitialize();
	memdelete(shader_types);
	memdelete(writer_mjpeg);
	memdelete(writer_pngwav);

	OS::get_singleton()->benchmark_end_measure("Servers", "Unregister Extensions");
}

void register_server_singletons() {
	OS::get_singleton()->benchmark_begin_measure("Servers", "Register Singletons");

	Engine::get_singleton()->add_singleton(Engine::Singleton("AudioServer", AudioServer::get_singleton(), "AudioServer"));
	Engine::get_singleton()->add_singleton(Engine::Singleton("CameraServer", CameraServer::get_singleton(), "CameraServer"));
	Engine::get_singleton()->add_singleton(Engine::Singleton("DisplayServer", DisplayServer::get_singleton(), "DisplayServer"));
	Engine::get_singleton()->add_singleton(Engine::Singleton("NativeMenu", NativeMenu::get_singleton(), "NativeMenu"));
	Engine::get_singleton()->add_singleton(Engine::Singleton("NavigationServer2D", NavigationServer2D::get_singleton(), "NavigationServer2D"));
	Engine::get_singleton()->add_singleton(Engine::Singleton("NavigationServer3D", NavigationServer3D::get_singleton(), "NavigationServer3D"));
	Engine::get_singleton()->add_singleton(Engine::Singleton("RenderingServer", RenderingServer::get_singleton(), "RenderingServer"));

	Engine::get_singleton()->add_singleton(Engine::Singleton("PhysicsServer2D", PhysicsServer2D::get_singleton(), "PhysicsServer2D"));
#ifndef _3D_DISABLED
	Engine::get_singleton()->add_singleton(Engine::Singleton("PhysicsServer3D", PhysicsServer3D::get_singleton(), "PhysicsServer3D"));
	Engine::get_singleton()->add_singleton(Engine::Singleton("XRServer", XRServer::get_singleton(), "XRServer"));
#endif // _3D_DISABLED

	OS::get_singleton()->benchmark_end_measure("Servers", "Register Singletons");
}
