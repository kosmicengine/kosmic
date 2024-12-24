/**************************************************************************/
/*  register_scene_types.cpp                                              */
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

#include "register_scene_types.h"

#include "core/config/project_settings.h"
#include "core/object/class_db.h"
#include "core/os/os.h"
#include "scene/animation/animation_blend_space_1d.h"
#include "scene/animation/animation_blend_space_2d.h"
#include "scene/animation/animation_blend_tree.h"
#include "scene/animation/animation_mixer.h"
#include "scene/animation/animation_node_extension.h"
#include "scene/animation/animation_node_state_machine.h"
#include "scene/animation/animation_player.h"
#include "scene/animation/animation_tree.h"
#include "scene/animation/tween.h"
#include "scene/audio/audio_stream_player.h"
#include "scene/debugger/scene_debugger.h"
#include "scene/gui/aspect_ratio_container.h"
#include "scene/gui/box_container.h"
#include "scene/gui/button.h"
#include "scene/gui/center_container.h"
#include "scene/gui/check_box.h"
#include "scene/gui/check_button.h"
#include "scene/gui/code_edit.h"
#include "scene/gui/color_picker.h"
#include "scene/gui/color_rect.h"
#include "scene/gui/control.h"
#include "scene/gui/dialogs.h"
#include "scene/gui/file_dialog.h"
#include "scene/gui/flow_container.h"
#include "scene/gui/graph_edit.h"
#include "scene/gui/graph_frame.h"
#include "scene/gui/graph_node.h"
#include "scene/gui/grid_container.h"
#include "scene/gui/item_list.h"
#include "scene/gui/label.h"
#include "scene/gui/line_edit.h"
#include "scene/gui/link_button.h"
#include "scene/gui/margin_container.h"
#include "scene/gui/menu_bar.h"
#include "scene/gui/menu_button.h"
#include "scene/gui/nine_patch_rect.h"
#include "scene/gui/option_button.h"
#include "scene/gui/panel.h"
#include "scene/gui/panel_container.h"
#include "scene/gui/popup_menu.h"
#include "scene/gui/progress_bar.h"
#include "scene/gui/reference_rect.h"
#include "scene/gui/rich_text_effect.h"
#include "scene/gui/rich_text_label.h"
#include "scene/gui/scroll_bar.h"
#include "scene/gui/scroll_container.h"
#include "scene/gui/separator.h"
#include "scene/gui/slider.h"
#include "scene/gui/spin_box.h"
#include "scene/gui/split_container.h"
#include "scene/gui/subviewport_container.h"
#include "scene/gui/tab_bar.h"
#include "scene/gui/tab_container.h"
#include "scene/gui/text_edit.h"
#include "scene/gui/texture_button.h"
#include "scene/gui/texture_progress_bar.h"
#include "scene/gui/texture_rect.h"
#include "scene/gui/tree.h"
#include "scene/gui/video_stream_player.h"
#include "scene/main/canvas_item.h"
#include "scene/main/canvas_layer.h"
#include "scene/main/http_request.h"
#include "scene/main/instance_placeholder.h"
#include "scene/main/missing_node.h"
#include "scene/main/multiplayer_api.h"
#include "scene/main/resource_preloader.h"
#include "scene/main/scene_tree.h"
#include "scene/main/shader_globals_override.h"
#include "scene/main/status_indicator.h"
#include "scene/main/timer.h"
#include "scene/main/viewport.h"
#include "scene/main/window.h"
#include "scene/resources/animated_texture.h"
#include "scene/resources/animation_library.h"
#include "scene/resources/atlas_texture.h"
#include "scene/resources/audio_stream_polyphonic.h"
#include "scene/resources/audio_stream_wav.h"
#include "scene/resources/bit_map.h"
#include "scene/resources/bone_map.h"
#include "scene/resources/camera_attributes.h"
#include "scene/resources/camera_texture.h"
#include "scene/resources/canvas_item_material.h"
#include "scene/resources/color_palette.h"
#include "scene/resources/compositor.h"
#include "scene/resources/compressed_texture.h"
#include "scene/resources/curve_texture.h"
#include "scene/resources/environment.h"
#include "scene/resources/external_texture.h"
#include "scene/resources/font.h"
#include "scene/resources/gradient.h"
#include "scene/resources/gradient_texture.h"
#include "scene/resources/image_texture.h"
#include "scene/resources/immediate_mesh.h"
#include "scene/resources/label_settings.h"
#include "scene/resources/material.h"
#include "scene/resources/mesh_data_tool.h"
#include "scene/resources/mesh_texture.h"
#include "scene/resources/multimesh.h"
#include "scene/resources/navigation_mesh.h"
#include "scene/resources/packed_scene.h"
#include "scene/resources/particle_process_material.h"
#include "scene/resources/physics_material.h"
#include "scene/resources/placeholder_textures.h"
#include "scene/resources/portable_compressed_texture.h"
#include "scene/resources/resource_format_text.h"
#include "scene/resources/shader_include.h"
#include "scene/resources/skeleton_profile.h"
#include "scene/resources/sky.h"
#include "scene/resources/style_box.h"
#include "scene/resources/style_box_flat.h"
#include "scene/resources/style_box_line.h"
#include "scene/resources/style_box_texture.h"
#include "scene/resources/surface_tool.h"
#include "scene/resources/syntax_highlighter.h"
#include "scene/resources/text_line.h"
#include "scene/resources/text_paragraph.h"
#include "scene/resources/texture.h"
#include "scene/resources/texture_rd.h"
#include "scene/resources/theme.h"
#include "scene/resources/video_stream.h"
#include "scene/resources/visual_shader.h"
#include "scene/resources/visual_shader_nodes.h"
#include "scene/resources/visual_shader_particle_nodes.h"
#include "scene/resources/visual_shader_sdf_nodes.h"
#include "scene/resources/world_2d.h"
#include "scene/theme/theme_db.h"

// 2D
#include "scene/2d/animated_sprite_2d.h"
#include "scene/2d/audio_listener_2d.h"
#include "scene/2d/audio_stream_player_2d.h"
#include "scene/2d/back_buffer_copy.h"
#include "scene/2d/camera_2d.h"
#include "scene/2d/canvas_group.h"
#include "scene/2d/canvas_modulate.h"
#include "scene/2d/cpu_particles_2d.h"
#include "scene/2d/gpu_particles_2d.h"
#include "scene/2d/light_2d.h"
#include "scene/2d/light_occluder_2d.h"
#include "scene/2d/line_2d.h"
#include "scene/2d/marker_2d.h"
#include "scene/2d/mesh_instance_2d.h"
#include "scene/2d/multimesh_instance_2d.h"
#include "scene/2d/navigation_agent_2d.h"
#include "scene/2d/navigation_link_2d.h"
#include "scene/2d/navigation_obstacle_2d.h"
#include "scene/2d/navigation_region_2d.h"
#include "scene/2d/parallax_2d.h"
#include "scene/2d/parallax_background.h"
#include "scene/2d/parallax_layer.h"
#include "scene/2d/path_2d.h"
#include "scene/2d/physics/animatable_body_2d.h"
#include "scene/2d/physics/area_2d.h"
#include "scene/2d/physics/character_body_2d.h"
#include "scene/2d/physics/collision_polygon_2d.h"
#include "scene/2d/physics/collision_shape_2d.h"
#include "scene/2d/physics/joints/damped_spring_joint_2d.h"
#include "scene/2d/physics/joints/groove_joint_2d.h"
#include "scene/2d/physics/joints/joint_2d.h"
#include "scene/2d/physics/joints/pin_joint_2d.h"
#include "scene/2d/physics/kinematic_collision_2d.h"
#include "scene/2d/physics/physical_bone_2d.h"
#include "scene/2d/physics/physics_body_2d.h"
#include "scene/2d/physics/ray_cast_2d.h"
#include "scene/2d/physics/rigid_body_2d.h"
#include "scene/2d/physics/shape_cast_2d.h"
#include "scene/2d/physics/static_body_2d.h"
#include "scene/2d/polygon_2d.h"
#include "scene/2d/remote_transform_2d.h"
#include "scene/2d/skeleton_2d.h"
#include "scene/2d/sprite_2d.h"
#include "scene/2d/tile_map.h"
#include "scene/2d/tile_map_layer.h"
#include "scene/2d/touch_screen_button.h"
#include "scene/2d/visible_on_screen_notifier_2d.h"
#include "scene/resources/2d/capsule_shape_2d.h"
#include "scene/resources/2d/circle_shape_2d.h"
#include "scene/resources/2d/concave_polygon_shape_2d.h"
#include "scene/resources/2d/convex_polygon_shape_2d.h"
#include "scene/resources/2d/navigation_mesh_source_geometry_data_2d.h"
#include "scene/resources/2d/navigation_polygon.h"
#include "scene/resources/2d/polygon_path_finder.h"
#include "scene/resources/2d/rectangle_shape_2d.h"
#include "scene/resources/2d/segment_shape_2d.h"
#include "scene/resources/2d/separation_ray_shape_2d.h"
#include "scene/resources/2d/skeleton/skeleton_modification_2d.h"
#include "scene/resources/2d/skeleton/skeleton_modification_2d_ccdik.h"
#include "scene/resources/2d/skeleton/skeleton_modification_2d_fabrik.h"
#include "scene/resources/2d/skeleton/skeleton_modification_2d_jiggle.h"
#include "scene/resources/2d/skeleton/skeleton_modification_2d_lookat.h"
#include "scene/resources/2d/skeleton/skeleton_modification_2d_physicalbones.h"
#include "scene/resources/2d/skeleton/skeleton_modification_2d_stackholder.h"
#include "scene/resources/2d/skeleton/skeleton_modification_2d_twoboneik.h"
#include "scene/resources/2d/skeleton/skeleton_modification_stack_2d.h"
#include "scene/resources/2d/tile_set.h"
#include "scene/resources/2d/world_boundary_shape_2d.h"

#ifndef _3D_DISABLED
#include "scene/3d/audio_listener_3d.h"
#include "scene/3d/audio_stream_player_3d.h"
#include "scene/3d/bone_attachment_3d.h"
#include "scene/3d/camera_3d.h"
#include "scene/3d/cpu_particles_3d.h"
#include "scene/3d/decal.h"
#include "scene/3d/fog_volume.h"
#include "scene/3d/gpu_particles_3d.h"
#include "scene/3d/gpu_particles_collision_3d.h"
#include "scene/3d/importer_mesh_instance_3d.h"
#include "scene/3d/label_3d.h"
#include "scene/3d/light_3d.h"
#include "scene/3d/lightmap_gi.h"
#include "scene/3d/lightmap_probe.h"
#include "scene/3d/look_at_modifier_3d.h"
#include "scene/3d/marker_3d.h"
#include "scene/3d/mesh_instance_3d.h"
#include "scene/3d/multimesh_instance_3d.h"
#include "scene/3d/navigation_agent_3d.h"
#include "scene/3d/navigation_link_3d.h"
#include "scene/3d/navigation_obstacle_3d.h"
#include "scene/3d/navigation_region_3d.h"
#include "scene/3d/node_3d.h"
#include "scene/3d/occluder_instance_3d.h"
#include "scene/3d/path_3d.h"
#include "scene/3d/physical_bone_simulator_3d.h"
#include "scene/3d/physics/animatable_body_3d.h"
#include "scene/3d/physics/area_3d.h"
#include "scene/3d/physics/character_body_3d.h"
#include "scene/3d/physics/collision_polygon_3d.h"
#include "scene/3d/physics/collision_shape_3d.h"
#include "scene/3d/physics/joints/cone_twist_joint_3d.h"
#include "scene/3d/physics/joints/generic_6dof_joint_3d.h"
#include "scene/3d/physics/joints/hinge_joint_3d.h"
#include "scene/3d/physics/joints/joint_3d.h"
#include "scene/3d/physics/joints/pin_joint_3d.h"
#include "scene/3d/physics/joints/slider_joint_3d.h"
#include "scene/3d/physics/kinematic_collision_3d.h"
#include "scene/3d/physics/physical_bone_3d.h"
#include "scene/3d/physics/physics_body_3d.h"
#include "scene/3d/physics/ray_cast_3d.h"
#include "scene/3d/physics/rigid_body_3d.h"
#include "scene/3d/physics/shape_cast_3d.h"
#include "scene/3d/physics/spring_arm_3d.h"
#include "scene/3d/physics/static_body_3d.h"
#include "scene/3d/physics/vehicle_body_3d.h"
#include "scene/3d/reflection_probe.h"
#include "scene/3d/remote_transform_3d.h"
#include "scene/3d/retarget_modifier_3d.h"
#include "scene/3d/skeleton_3d.h"
#include "scene/3d/skeleton_ik_3d.h"
#include "scene/3d/skeleton_modifier_3d.h"
#include "scene/3d/soft_body_3d.h"
#include "scene/3d/sprite_3d.h"
#include "scene/3d/visible_on_screen_notifier_3d.h"
#include "scene/3d/voxel_gi.h"
#include "scene/3d/world_environment.h"
#include "scene/3d/xr_body_modifier_3d.h"
#include "scene/3d/xr_face_modifier_3d.h"
#include "scene/3d/xr_hand_modifier_3d.h"
#include "scene/3d/xr_nodes.h"
#include "scene/animation/root_motion_view.h"
#include "scene/resources/3d/box_shape_3d.h"
#include "scene/resources/3d/capsule_shape_3d.h"
#include "scene/resources/3d/concave_polygon_shape_3d.h"
#include "scene/resources/3d/convex_polygon_shape_3d.h"
#include "scene/resources/3d/cylinder_shape_3d.h"
#include "scene/resources/3d/fog_material.h"
#include "scene/resources/3d/height_map_shape_3d.h"
#include "scene/resources/3d/importer_mesh.h"
#include "scene/resources/3d/mesh_library.h"
#include "scene/resources/3d/navigation_mesh_source_geometry_data_3d.h"
#include "scene/resources/3d/primitive_meshes.h"
#include "scene/resources/3d/separation_ray_shape_3d.h"
#include "scene/resources/3d/sky_material.h"
#include "scene/resources/3d/sphere_shape_3d.h"
#include "scene/resources/3d/world_3d.h"
#include "scene/resources/3d/world_boundary_shape_3d.h"
#endif // _3D_DISABLED

static Ref<ResourceFormatSaverText> resource_saver_text;
static Ref<ResourceFormatLoaderText> resource_loader_text;

static Ref<ResourceFormatLoaderCompressedTexture2D> resource_loader_stream_texture;
static Ref<ResourceFormatLoaderCompressedTextureLayered> resource_loader_texture_layered;
static Ref<ResourceFormatLoaderCompressedTexture3D> resource_loader_texture_3d;

static Ref<ResourceFormatSaverShader> resource_saver_shader;
static Ref<ResourceFormatLoaderShader> resource_loader_shader;

static Ref<ResourceFormatSaverShaderInclude> resource_saver_shader_include;
static Ref<ResourceFormatLoaderShaderInclude> resource_loader_shader_include;

void register_scene_types() {
	OS::get_singleton()->benchmark_begin_measure("Scene", "Register Types");

	SceneStringNames::create();

	OS::get_singleton()->yield(); // may take time to init

	Node::init_node_hrcr();

	resource_loader_stream_texture.instantiate();
	ResourceLoader::add_resource_format_loader(resource_loader_stream_texture);

	resource_loader_texture_layered.instantiate();
	ResourceLoader::add_resource_format_loader(resource_loader_texture_layered);

	resource_loader_texture_3d.instantiate();
	ResourceLoader::add_resource_format_loader(resource_loader_texture_3d);

	resource_saver_text.instantiate();
	ResourceSaver::add_resource_format_saver(resource_saver_text, true);

	resource_loader_text.instantiate();
	ResourceLoader::add_resource_format_loader(resource_loader_text, true);

	resource_saver_shader.instantiate();
	ResourceSaver::add_resource_format_saver(resource_saver_shader, true);

	resource_loader_shader.instantiate();
	ResourceLoader::add_resource_format_loader(resource_loader_shader, true);

	resource_saver_shader_include.instantiate();
	ResourceSaver::add_resource_format_saver(resource_saver_shader_include, true);

	resource_loader_shader_include.instantiate();
	ResourceLoader::add_resource_format_loader(resource_loader_shader_include, true);

	OS::get_singleton()->yield(); // may take time to init

	KSREGISTER_CLASS(Object);

	KSREGISTER_CLASS(Node);
	KSREGISTER_VIRTUAL_CLASS(MissingNode);
	KSREGISTER_ABSTRACT_CLASS(InstancePlaceholder);

	KSREGISTER_ABSTRACT_CLASS(Viewport);
	KSREGISTER_CLASS(SubViewport);
	KSREGISTER_CLASS(ViewportTexture);

	KSREGISTER_VIRTUAL_CLASS(CompositorEffect);

	KSREGISTER_ABSTRACT_CLASS(MultiplayerPeer);
	KSREGISTER_CLASS(MultiplayerPeerExtension);
	KSREGISTER_ABSTRACT_CLASS(MultiplayerAPI);
	KSREGISTER_CLASS(MultiplayerAPIExtension);

	KSREGISTER_CLASS(HTTPRequest);
	KSREGISTER_CLASS(Timer);
	KSREGISTER_CLASS(CanvasLayer);
	KSREGISTER_CLASS(CanvasModulate);
	KSREGISTER_CLASS(ResourcePreloader);
	KSREGISTER_CLASS(Window);

	KSREGISTER_CLASS(StatusIndicator);

	/* REGISTER GUI */

	KSREGISTER_CLASS(ButtonGroup);
	KSREGISTER_VIRTUAL_CLASS(BaseButton);

	OS::get_singleton()->yield(); // may take time to init

	KSREGISTER_CLASS(Control);
	KSREGISTER_CLASS(Button);
	KSREGISTER_CLASS(Label);
	KSREGISTER_ABSTRACT_CLASS(ScrollBar);
	KSREGISTER_CLASS(HScrollBar);
	KSREGISTER_CLASS(VScrollBar);
	KSREGISTER_CLASS(ProgressBar);
	KSREGISTER_ABSTRACT_CLASS(Slider);
	KSREGISTER_CLASS(HSlider);
	KSREGISTER_CLASS(VSlider);
	KSREGISTER_CLASS(Popup);
	KSREGISTER_CLASS(PopupPanel);
	KSREGISTER_CLASS(CheckBox);
	KSREGISTER_CLASS(CheckButton);
	KSREGISTER_CLASS(LinkButton);
	KSREGISTER_CLASS(Panel);
	KSREGISTER_VIRTUAL_CLASS(Range);

	OS::get_singleton()->yield(); // may take time to init

	KSREGISTER_CLASS(TextureRect);
	KSREGISTER_CLASS(ColorRect);
	KSREGISTER_CLASS(NinePatchRect);
	KSREGISTER_CLASS(ReferenceRect);
	KSREGISTER_CLASS(AspectRatioContainer);
	KSREGISTER_CLASS(TabContainer);
	KSREGISTER_CLASS(TabBar);
	KSREGISTER_ABSTRACT_CLASS(Separator);
	KSREGISTER_CLASS(HSeparator);
	KSREGISTER_CLASS(VSeparator);
	KSREGISTER_CLASS(TextureButton);
	KSREGISTER_CLASS(Container);
	KSREGISTER_CLASS(BoxContainer);
	KSREGISTER_CLASS(HBoxContainer);
	KSREGISTER_CLASS(VBoxContainer);
	KSREGISTER_CLASS(GridContainer);
	KSREGISTER_CLASS(CenterContainer);
	KSREGISTER_CLASS(ScrollContainer);
	KSREGISTER_CLASS(PanelContainer);
	KSREGISTER_CLASS(FlowContainer);
	KSREGISTER_CLASS(HFlowContainer);
	KSREGISTER_CLASS(VFlowContainer);
	KSREGISTER_CLASS(MarginContainer);

	OS::get_singleton()->yield(); // may take time to init

	KSREGISTER_CLASS(TextureProgressBar);
	KSREGISTER_CLASS(ItemList);

	KSREGISTER_CLASS(LineEdit);
	KSREGISTER_CLASS(VideoStreamPlayer);
	KSREGISTER_VIRTUAL_CLASS(VideoStreamPlayback);
	KSREGISTER_VIRTUAL_CLASS(VideoStream);

#ifndef ADVANCED_GUI_DISABLED
	KSREGISTER_CLASS(FileDialog);

	KSREGISTER_CLASS(PopupMenu);
	KSREGISTER_CLASS(Tree);

	KSREGISTER_CLASS(TextEdit);
	KSREGISTER_CLASS(CodeEdit);
	KSREGISTER_CLASS(SyntaxHighlighter);
	KSREGISTER_CLASS(CodeHighlighter);

	KSREGISTER_ABSTRACT_CLASS(TreeItem);
	KSREGISTER_CLASS(MenuBar);
	KSREGISTER_CLASS(MenuButton);
	KSREGISTER_CLASS(OptionButton);
	KSREGISTER_CLASS(SpinBox);
	KSREGISTER_CLASS(ColorPicker);
	KSREGISTER_CLASS(ColorPickerButton);
	KSREGISTER_CLASS(RichTextLabel);
	KSREGISTER_CLASS(RichTextEffect);
	KSREGISTER_CLASS(CharFXTransform);

	KSREGISTER_CLASS(AcceptDialog);
	KSREGISTER_CLASS(ConfirmationDialog);

	KSREGISTER_CLASS(SubViewportContainer);
	KSREGISTER_CLASS(SplitContainer);
	KSREGISTER_CLASS(HSplitContainer);
	KSREGISTER_CLASS(VSplitContainer);

	KSREGISTER_CLASS(GraphElement);
	KSREGISTER_CLASS(GraphNode);
	KSREGISTER_CLASS(GraphFrame);
	KSREGISTER_CLASS(GraphEdit);

	OS::get_singleton()->yield(); // may take time to init

	bool swap_cancel_ok = false;
	if (DisplayServer::get_singleton()) {
		swap_cancel_ok = GLOBAL_DEF_NOVAL("gui/common/swap_cancel_ok", bool(DisplayServer::get_singleton()->get_swap_cancel_ok()));
	}
	AcceptDialog::set_swap_cancel_ok(swap_cancel_ok);
#endif

	int root_dir = GLOBAL_GET("internationalization/rendering/root_node_layout_direction");
	Control::set_root_layout_direction(root_dir);
	Window::set_root_layout_direction(root_dir);

	/* REGISTER ANIMATION */
	KSREGISTER_CLASS(Tween);
	KSREGISTER_ABSTRACT_CLASS(Tweener);
	KSREGISTER_CLASS(PropertyTweener);
	KSREGISTER_CLASS(IntervalTweener);
	KSREGISTER_CLASS(CallbackTweener);
	KSREGISTER_CLASS(MethodTweener);

	KSREGISTER_ABSTRACT_CLASS(AnimationMixer);
	KSREGISTER_CLASS(AnimationPlayer);
	KSREGISTER_CLASS(AnimationTree);
	KSREGISTER_CLASS(AnimationNode);
	KSREGISTER_CLASS(AnimationRootNode);
	KSREGISTER_CLASS(AnimationNodeBlendTree);
	KSREGISTER_CLASS(AnimationNodeBlendSpace1D);
	KSREGISTER_CLASS(AnimationNodeBlendSpace2D);
	KSREGISTER_CLASS(AnimationNodeStateMachine);
	KSREGISTER_CLASS(AnimationNodeStateMachinePlayback);
	KSREGISTER_VIRTUAL_CLASS(AnimationNodeExtension);

	KSREGISTER_INTERNAL_CLASS(AnimationNodeStartState);
	KSREGISTER_INTERNAL_CLASS(AnimationNodeEndState);

	KSREGISTER_CLASS(AnimationNodeSync);
	KSREGISTER_CLASS(AnimationNodeStateMachineTransition);
	KSREGISTER_CLASS(AnimationNodeOutput);
	KSREGISTER_CLASS(AnimationNodeOneShot);
	KSREGISTER_CLASS(AnimationNodeAnimation);
	KSREGISTER_CLASS(AnimationNodeAdd2);
	KSREGISTER_CLASS(AnimationNodeAdd3);
	KSREGISTER_CLASS(AnimationNodeBlend2);
	KSREGISTER_CLASS(AnimationNodeBlend3);
	KSREGISTER_CLASS(AnimationNodeSub2);
	KSREGISTER_CLASS(AnimationNodeTimeScale);
	KSREGISTER_CLASS(AnimationNodeTimeSeek);
	KSREGISTER_CLASS(AnimationNodeTransition);

	KSREGISTER_CLASS(ShaderGlobalsOverride); // can be used in any shader

	OS::get_singleton()->yield(); // may take time to init

	/* REGISTER 3D */

#ifndef _3D_DISABLED
	KSREGISTER_CLASS(Node3D);
	KSREGISTER_ABSTRACT_CLASS(Node3DGizmo);
	KSREGISTER_CLASS(Skin);
	KSREGISTER_ABSTRACT_CLASS(SkinReference);
	KSREGISTER_CLASS(Skeleton3D);
	KSREGISTER_CLASS(ImporterMesh);
	KSREGISTER_CLASS(ImporterMeshInstance3D);
	KSREGISTER_VIRTUAL_CLASS(VisualInstance3D);
	KSREGISTER_VIRTUAL_CLASS(GeometryInstance3D);
	KSREGISTER_CLASS(Camera3D);
	KSREGISTER_CLASS(AudioListener3D);
	KSREGISTER_CLASS(XRCamera3D);
	KSREGISTER_CLASS(XRNode3D);
	KSREGISTER_CLASS(XRController3D);
	KSREGISTER_CLASS(XRAnchor3D);
	KSREGISTER_CLASS(XROrigin3D);
	KSREGISTER_CLASS(XRBodyModifier3D);
	KSREGISTER_CLASS(XRHandModifier3D);
	KSREGISTER_CLASS(XRFaceModifier3D);
	KSREGISTER_CLASS(MeshInstance3D);
	KSREGISTER_CLASS(OccluderInstance3D);
	KSREGISTER_ABSTRACT_CLASS(Occluder3D);
	KSREGISTER_CLASS(ArrayOccluder3D);
	KSREGISTER_CLASS(QuadOccluder3D);
	KSREGISTER_CLASS(BoxOccluder3D);
	KSREGISTER_CLASS(SphereOccluder3D);
	KSREGISTER_CLASS(PolygonOccluder3D);
	KSREGISTER_ABSTRACT_CLASS(SpriteBase3D);
	KSREGISTER_CLASS(Sprite3D);
	KSREGISTER_CLASS(AnimatedSprite3D);
	KSREGISTER_CLASS(Label3D);
	KSREGISTER_ABSTRACT_CLASS(Light3D);
	KSREGISTER_CLASS(DirectionalLight3D);
	KSREGISTER_CLASS(OmniLight3D);
	KSREGISTER_CLASS(SpotLight3D);
	KSREGISTER_CLASS(ReflectionProbe);
	KSREGISTER_CLASS(Decal);
	KSREGISTER_CLASS(VoxelGI);
	KSREGISTER_CLASS(VoxelGIData);
	KSREGISTER_CLASS(LightmapGI);
	KSREGISTER_CLASS(LightmapGIData);
	KSREGISTER_CLASS(LightmapProbe);
	KSREGISTER_ABSTRACT_CLASS(Lightmapper);
	KSREGISTER_CLASS(GPUParticles3D);
	KSREGISTER_ABSTRACT_CLASS(GPUParticlesCollision3D);
	KSREGISTER_CLASS(GPUParticlesCollisionBox3D);
	KSREGISTER_CLASS(GPUParticlesCollisionSphere3D);
	KSREGISTER_CLASS(GPUParticlesCollisionSDF3D);
	KSREGISTER_CLASS(GPUParticlesCollisionHeightField3D);
	KSREGISTER_ABSTRACT_CLASS(GPUParticlesAttractor3D);
	KSREGISTER_CLASS(GPUParticlesAttractorBox3D);
	KSREGISTER_CLASS(GPUParticlesAttractorSphere3D);
	KSREGISTER_CLASS(GPUParticlesAttractorVectorField3D);
	KSREGISTER_CLASS(CPUParticles3D);
	KSREGISTER_CLASS(Marker3D);
	KSREGISTER_CLASS(RootMotionView);
	KSREGISTER_VIRTUAL_CLASS(SkeletonModifier3D);
	KSREGISTER_CLASS(RetargetModifier3D);

	OS::get_singleton()->yield(); // may take time to init

	KSREGISTER_ABSTRACT_CLASS(CollisionObject3D);
	KSREGISTER_ABSTRACT_CLASS(PhysicsBody3D);
	KSREGISTER_CLASS(StaticBody3D);
	KSREGISTER_CLASS(AnimatableBody3D);
	KSREGISTER_CLASS(RigidBody3D);
	KSREGISTER_CLASS(KinematicCollision3D);
	KSREGISTER_CLASS(CharacterBody3D);
	KSREGISTER_CLASS(SpringArm3D);

	KSREGISTER_CLASS(PhysicalBoneSimulator3D);
	KSREGISTER_CLASS(PhysicalBone3D);
	KSREGISTER_CLASS(SoftBody3D);

	KSREGISTER_CLASS(SkeletonIK3D);
	KSREGISTER_CLASS(BoneAttachment3D);
	KSREGISTER_CLASS(LookAtModifier3D);

	KSREGISTER_CLASS(VehicleBody3D);
	KSREGISTER_CLASS(VehicleWheel3D);
	KSREGISTER_CLASS(Area3D);
	KSREGISTER_CLASS(CollisionShape3D);
	KSREGISTER_CLASS(CollisionPolygon3D);
	KSREGISTER_CLASS(RayCast3D);
	KSREGISTER_CLASS(ShapeCast3D);
	KSREGISTER_CLASS(MultiMeshInstance3D);

	KSREGISTER_CLASS(Curve3D);
	KSREGISTER_CLASS(Path3D);
	KSREGISTER_CLASS(PathFollow3D);
	KSREGISTER_CLASS(VisibleOnScreenNotifier3D);
	KSREGISTER_CLASS(VisibleOnScreenEnabler3D);
	KSREGISTER_CLASS(WorldEnvironment);
	KSREGISTER_CLASS(FogVolume);
	KSREGISTER_CLASS(FogMaterial);
	KSREGISTER_CLASS(RemoteTransform3D);

	KSREGISTER_ABSTRACT_CLASS(Joint3D);
	KSREGISTER_CLASS(PinJoint3D);
	KSREGISTER_CLASS(HingeJoint3D);
	KSREGISTER_CLASS(SliderJoint3D);
	KSREGISTER_CLASS(ConeTwistJoint3D);
	KSREGISTER_CLASS(Generic6DOFJoint3D);

	KSREGISTER_CLASS(NavigationRegion3D);
	KSREGISTER_CLASS(NavigationAgent3D);
	KSREGISTER_CLASS(NavigationObstacle3D);
	KSREGISTER_CLASS(NavigationLink3D);

	OS::get_singleton()->yield(); // may take time to init
#endif // _3D_DISABLED

	/* REGISTER SHADER */

	KSREGISTER_CLASS(Shader);
	KSREGISTER_CLASS(VisualShader);
	KSREGISTER_CLASS(ShaderInclude);
	KSREGISTER_ABSTRACT_CLASS(VisualShaderNode);
	KSREGISTER_CLASS(VisualShaderNodeCustom);
	KSREGISTER_CLASS(VisualShaderNodeInput);
	KSREGISTER_ABSTRACT_CLASS(VisualShaderNodeOutput);
	KSREGISTER_ABSTRACT_CLASS(VisualShaderNodeResizableBase);
	KSREGISTER_ABSTRACT_CLASS(VisualShaderNodeGroupBase);
	KSREGISTER_ABSTRACT_CLASS(VisualShaderNodeConstant);
	KSREGISTER_ABSTRACT_CLASS(VisualShaderNodeVectorBase);
	KSREGISTER_CLASS(VisualShaderNodeFrame);
#ifndef DISABLE_DEPRECATED
	KSREGISTER_CLASS(VisualShaderNodeComment); // Deprecated, just for compatibility.
#endif
	KSREGISTER_CLASS(VisualShaderNodeFloatConstant);
	KSREGISTER_CLASS(VisualShaderNodeIntConstant);
	KSREGISTER_CLASS(VisualShaderNodeUIntConstant);
	KSREGISTER_CLASS(VisualShaderNodeBooleanConstant);
	KSREGISTER_CLASS(VisualShaderNodeColorConstant);
	KSREGISTER_CLASS(VisualShaderNodeVec2Constant);
	KSREGISTER_CLASS(VisualShaderNodeVec3Constant);
	KSREGISTER_CLASS(VisualShaderNodeVec4Constant);
	KSREGISTER_CLASS(VisualShaderNodeTransformConstant);
	KSREGISTER_CLASS(VisualShaderNodeFloatOp);
	KSREGISTER_CLASS(VisualShaderNodeIntOp);
	KSREGISTER_CLASS(VisualShaderNodeUIntOp);
	KSREGISTER_CLASS(VisualShaderNodeVectorOp);
	KSREGISTER_CLASS(VisualShaderNodeColorOp);
	KSREGISTER_CLASS(VisualShaderNodeTransformOp);
	KSREGISTER_CLASS(VisualShaderNodeTransformVecMult);
	KSREGISTER_CLASS(VisualShaderNodeFloatFunc);
	KSREGISTER_CLASS(VisualShaderNodeIntFunc);
	KSREGISTER_CLASS(VisualShaderNodeUIntFunc);
	KSREGISTER_CLASS(VisualShaderNodeVectorFunc);
	KSREGISTER_CLASS(VisualShaderNodeColorFunc);
	KSREGISTER_CLASS(VisualShaderNodeTransformFunc);
	KSREGISTER_CLASS(VisualShaderNodeUVFunc);
	KSREGISTER_CLASS(VisualShaderNodeUVPolarCoord);
	KSREGISTER_CLASS(VisualShaderNodeDotProduct);
	KSREGISTER_CLASS(VisualShaderNodeVectorLen);
	KSREGISTER_CLASS(VisualShaderNodeDeterminant);
	KSREGISTER_CLASS(VisualShaderNodeDerivativeFunc);
	KSREGISTER_CLASS(VisualShaderNodeClamp);
	KSREGISTER_CLASS(VisualShaderNodeFaceForward);
	KSREGISTER_CLASS(VisualShaderNodeOuterProduct);
	KSREGISTER_CLASS(VisualShaderNodeSmoothStep);
	KSREGISTER_CLASS(VisualShaderNodeStep);
	KSREGISTER_CLASS(VisualShaderNodeVectorDistance);
	KSREGISTER_CLASS(VisualShaderNodeVectorRefract);
	KSREGISTER_CLASS(VisualShaderNodeMix);
	KSREGISTER_CLASS(VisualShaderNodeVectorCompose);
	KSREGISTER_CLASS(VisualShaderNodeTransformCompose);
	KSREGISTER_CLASS(VisualShaderNodeVectorDecompose);
	KSREGISTER_CLASS(VisualShaderNodeTransformDecompose);
	KSREGISTER_CLASS(VisualShaderNodeTexture);
	KSREGISTER_CLASS(VisualShaderNodeCurveTexture);
	KSREGISTER_CLASS(VisualShaderNodeCurveXYZTexture);
	KSREGISTER_ABSTRACT_CLASS(VisualShaderNodeSample3D);
	KSREGISTER_CLASS(VisualShaderNodeTexture2DArray);
	KSREGISTER_CLASS(VisualShaderNodeTexture3D);
	KSREGISTER_CLASS(VisualShaderNodeCubemap);
	KSREGISTER_ABSTRACT_CLASS(VisualShaderNodeParameter);
	KSREGISTER_CLASS(VisualShaderNodeParameterRef);
	KSREGISTER_CLASS(VisualShaderNodeFloatParameter);
	KSREGISTER_CLASS(VisualShaderNodeIntParameter);
	KSREGISTER_CLASS(VisualShaderNodeUIntParameter);
	KSREGISTER_CLASS(VisualShaderNodeBooleanParameter);
	KSREGISTER_CLASS(VisualShaderNodeColorParameter);
	KSREGISTER_CLASS(VisualShaderNodeVec2Parameter);
	KSREGISTER_CLASS(VisualShaderNodeVec3Parameter);
	KSREGISTER_CLASS(VisualShaderNodeVec4Parameter);
	KSREGISTER_CLASS(VisualShaderNodeTransformParameter);
	KSREGISTER_ABSTRACT_CLASS(VisualShaderNodeTextureParameter);
	KSREGISTER_CLASS(VisualShaderNodeTexture2DParameter);
	KSREGISTER_CLASS(VisualShaderNodeTextureParameterTriplanar);
	KSREGISTER_CLASS(VisualShaderNodeTexture2DArrayParameter);
	KSREGISTER_CLASS(VisualShaderNodeTexture3DParameter);
	KSREGISTER_CLASS(VisualShaderNodeCubemapParameter);
	KSREGISTER_CLASS(VisualShaderNodeLinearSceneDepth);
	KSREGISTER_CLASS(VisualShaderNodeWorldPositionFromDepth);
	KSREGISTER_CLASS(VisualShaderNodeScreenNormalWorldSpace);
	KSREGISTER_CLASS(VisualShaderNodeIf);
	KSREGISTER_CLASS(VisualShaderNodeSwitch);
	KSREGISTER_CLASS(VisualShaderNodeFresnel);
	KSREGISTER_CLASS(VisualShaderNodeExpression);
	KSREGISTER_CLASS(VisualShaderNodeGlobalExpression);
	KSREGISTER_CLASS(VisualShaderNodeIs);
	KSREGISTER_CLASS(VisualShaderNodeCompare);
	KSREGISTER_CLASS(VisualShaderNodeMultiplyAdd);
	KSREGISTER_CLASS(VisualShaderNodeBillboard);
	KSREGISTER_CLASS(VisualShaderNodeDistanceFade);
	KSREGISTER_CLASS(VisualShaderNodeProximityFade);
	KSREGISTER_CLASS(VisualShaderNodeRandomRange);
	KSREGISTER_CLASS(VisualShaderNodeRemap);
	KSREGISTER_CLASS(VisualShaderNodeRotationByAxis);
	KSREGISTER_ABSTRACT_CLASS(VisualShaderNodeVarying);
	KSREGISTER_CLASS(VisualShaderNodeVaryingSetter);
	KSREGISTER_CLASS(VisualShaderNodeVaryingGetter);
	KSREGISTER_CLASS(VisualShaderNodeReroute);

	KSREGISTER_CLASS(VisualShaderNodeSDFToScreenUV);
	KSREGISTER_CLASS(VisualShaderNodeScreenUVToSDF);
	KSREGISTER_CLASS(VisualShaderNodeTextureSDF);
	KSREGISTER_CLASS(VisualShaderNodeTextureSDFNormal);
	KSREGISTER_CLASS(VisualShaderNodeSDFRaymarch);

	KSREGISTER_CLASS(VisualShaderNodeParticleOutput);
	KSREGISTER_ABSTRACT_CLASS(VisualShaderNodeParticleEmitter);
	KSREGISTER_CLASS(VisualShaderNodeParticleSphereEmitter);
	KSREGISTER_CLASS(VisualShaderNodeParticleBoxEmitter);
	KSREGISTER_CLASS(VisualShaderNodeParticleRingEmitter);
	KSREGISTER_CLASS(VisualShaderNodeParticleMeshEmitter);
	KSREGISTER_CLASS(VisualShaderNodeParticleMultiplyByAxisAngle);
	KSREGISTER_CLASS(VisualShaderNodeParticleConeVelocity);
	KSREGISTER_CLASS(VisualShaderNodeParticleRandomness);
	KSREGISTER_CLASS(VisualShaderNodeParticleAccelerator);
	KSREGISTER_CLASS(VisualShaderNodeParticleEmit);

	KSREGISTER_VIRTUAL_CLASS(Material);
	KSREGISTER_CLASS(PlaceholderMaterial);
	KSREGISTER_CLASS(ShaderMaterial);
	KSREGISTER_ABSTRACT_CLASS(CanvasItem);
	KSREGISTER_CLASS(CanvasTexture);
	KSREGISTER_CLASS(CanvasItemMaterial);
	SceneTree::add_idle_callback(CanvasItemMaterial::flush_changes);
	CanvasItemMaterial::init_shaders();

	/* REGISTER 2D */

	KSREGISTER_CLASS(Node2D);
	KSREGISTER_CLASS(CanvasGroup);
	KSREGISTER_CLASS(CPUParticles2D);
	KSREGISTER_CLASS(GPUParticles2D);
	KSREGISTER_CLASS(Sprite2D);
	KSREGISTER_CLASS(SpriteFrames);
	KSREGISTER_CLASS(AnimatedSprite2D);
	KSREGISTER_CLASS(Marker2D);
	KSREGISTER_CLASS(Line2D);
	KSREGISTER_CLASS(MeshInstance2D);
	KSREGISTER_CLASS(MultiMeshInstance2D);
	KSREGISTER_ABSTRACT_CLASS(CollisionObject2D);
	KSREGISTER_ABSTRACT_CLASS(PhysicsBody2D);
	KSREGISTER_CLASS(StaticBody2D);
	KSREGISTER_CLASS(AnimatableBody2D);
	KSREGISTER_CLASS(RigidBody2D);
	KSREGISTER_CLASS(CharacterBody2D);
	KSREGISTER_CLASS(KinematicCollision2D);
	KSREGISTER_CLASS(Area2D);
	KSREGISTER_CLASS(CollisionShape2D);
	KSREGISTER_CLASS(CollisionPolygon2D);
	KSREGISTER_CLASS(RayCast2D);
	KSREGISTER_CLASS(ShapeCast2D);
	KSREGISTER_CLASS(VisibleOnScreenNotifier2D);
	KSREGISTER_CLASS(VisibleOnScreenEnabler2D);
	KSREGISTER_CLASS(Polygon2D);
	KSREGISTER_CLASS(Skeleton2D);
	KSREGISTER_CLASS(Bone2D);
	KSREGISTER_ABSTRACT_CLASS(Light2D);
	KSREGISTER_CLASS(PointLight2D);
	KSREGISTER_CLASS(DirectionalLight2D);
	KSREGISTER_CLASS(LightOccluder2D);
	KSREGISTER_CLASS(OccluderPolygon2D);
	KSREGISTER_CLASS(BackBufferCopy);

	OS::get_singleton()->yield(); // may take time to init

	KSREGISTER_CLASS(Camera2D);
	KSREGISTER_CLASS(AudioListener2D);
	KSREGISTER_ABSTRACT_CLASS(Joint2D);
	KSREGISTER_CLASS(PinJoint2D);
	KSREGISTER_CLASS(GrooveJoint2D);
	KSREGISTER_CLASS(DampedSpringJoint2D);
	KSREGISTER_CLASS(TileSet);
	KSREGISTER_ABSTRACT_CLASS(TileSetSource);
	KSREGISTER_CLASS(TileSetAtlasSource);
	KSREGISTER_CLASS(TileSetScenesCollectionSource);
	KSREGISTER_CLASS(TileMapPattern);
	KSREGISTER_CLASS(TileData);
	KSREGISTER_CLASS(TileMap);
	KSREGISTER_CLASS(TileMapLayer);
	KSREGISTER_CLASS(Parallax2D);
	KSREGISTER_CLASS(ParallaxBackground);
	KSREGISTER_CLASS(ParallaxLayer);
	KSREGISTER_CLASS(TouchScreenButton);
	KSREGISTER_CLASS(RemoteTransform2D);

	KSREGISTER_CLASS(SkeletonModificationStack2D);
	KSREGISTER_CLASS(SkeletonModification2D);
	KSREGISTER_CLASS(SkeletonModification2DLookAt);
	KSREGISTER_CLASS(SkeletonModification2DCCDIK);
	KSREGISTER_CLASS(SkeletonModification2DFABRIK);
	KSREGISTER_CLASS(SkeletonModification2DJiggle);
	KSREGISTER_CLASS(SkeletonModification2DTwoBoneIK);
	KSREGISTER_CLASS(SkeletonModification2DStackHolder);

	KSREGISTER_CLASS(PhysicalBone2D);
	KSREGISTER_CLASS(SkeletonModification2DPhysicalBones);

	OS::get_singleton()->yield(); // may take time to init

	/* REGISTER RESOURCES */

	KSREGISTER_ABSTRACT_CLASS(Shader);
	KSREGISTER_CLASS(ParticleProcessMaterial);
	SceneTree::add_idle_callback(ParticleProcessMaterial::flush_changes);
	ParticleProcessMaterial::init_shaders();

	KSREGISTER_VIRTUAL_CLASS(Mesh);
	KSREGISTER_CLASS(MeshConvexDecompositionSettings);
	KSREGISTER_CLASS(ArrayMesh);
	KSREGISTER_CLASS(PlaceholderMesh);
	KSREGISTER_CLASS(ImmediateMesh);
	KSREGISTER_CLASS(MultiMesh);
	KSREGISTER_CLASS(SurfaceTool);
	KSREGISTER_CLASS(MeshDataTool);

#ifndef _3D_DISABLED
	KSREGISTER_CLASS(AudioStreamPlayer3D);
	KSREGISTER_VIRTUAL_CLASS(PrimitiveMesh);
	KSREGISTER_CLASS(BoxMesh);
	KSREGISTER_CLASS(CapsuleMesh);
	KSREGISTER_CLASS(CylinderMesh);
	KSREGISTER_CLASS(PlaneMesh);
	KSREGISTER_CLASS(PrismMesh);
	KSREGISTER_CLASS(QuadMesh);
	KSREGISTER_CLASS(SphereMesh);
	KSREGISTER_CLASS(TextMesh);
	KSREGISTER_CLASS(TorusMesh);
	KSREGISTER_CLASS(TubeTrailMesh);
	KSREGISTER_CLASS(RibbonTrailMesh);
	KSREGISTER_CLASS(PointMesh);
	KSREGISTER_ABSTRACT_CLASS(BaseMaterial3D);
	KSREGISTER_CLASS(StandardMaterial3D);
	KSREGISTER_CLASS(ORMMaterial3D);
	KSREGISTER_CLASS(ProceduralSkyMaterial);
	KSREGISTER_CLASS(PanoramaSkyMaterial);
	KSREGISTER_CLASS(PhysicalSkyMaterial);
	SceneTree::add_idle_callback(BaseMaterial3D::flush_changes);
	BaseMaterial3D::init_shaders();

	KSREGISTER_CLASS(MeshLibrary);
	KSREGISTER_CLASS(NavigationMeshSourceGeometryData3D);

	OS::get_singleton()->yield(); // may take time to init

	KSREGISTER_ABSTRACT_CLASS(Shape3D);
	KSREGISTER_CLASS(SeparationRayShape3D);
	KSREGISTER_CLASS(SphereShape3D);
	KSREGISTER_CLASS(BoxShape3D);
	KSREGISTER_CLASS(CapsuleShape3D);
	KSREGISTER_CLASS(CylinderShape3D);
	KSREGISTER_CLASS(HeightMapShape3D);
	KSREGISTER_CLASS(WorldBoundaryShape3D);
	KSREGISTER_CLASS(ConvexPolygonShape3D);
	KSREGISTER_CLASS(ConcavePolygonShape3D);
	KSREGISTER_CLASS(World3D);

	OS::get_singleton()->yield(); // may take time to init
#endif // _3D_DISABLED

	KSREGISTER_CLASS(PhysicsMaterial);
	KSREGISTER_CLASS(Compositor);
	KSREGISTER_CLASS(Environment);
	KSREGISTER_VIRTUAL_CLASS(CameraAttributes);
	KSREGISTER_CLASS(CameraAttributesPhysical);
	KSREGISTER_CLASS(CameraAttributesPractical);
	KSREGISTER_CLASS(World2D);
	KSREGISTER_VIRTUAL_CLASS(Texture);
	KSREGISTER_VIRTUAL_CLASS(Texture2D);
	KSREGISTER_CLASS(Sky);
	KSREGISTER_CLASS(CompressedTexture2D);
	KSREGISTER_CLASS(PortableCompressedTexture2D);
	KSREGISTER_CLASS(ImageTexture);
	KSREGISTER_CLASS(AtlasTexture);
	KSREGISTER_CLASS(MeshTexture);
	KSREGISTER_CLASS(CurveTexture);
	KSREGISTER_CLASS(CurveXYZTexture);
	KSREGISTER_CLASS(GradientTexture1D);
	KSREGISTER_CLASS(GradientTexture2D);
	KSREGISTER_CLASS(AnimatedTexture);
	KSREGISTER_CLASS(CameraTexture);
	KSREGISTER_CLASS(ExternalTexture);
	KSREGISTER_VIRTUAL_CLASS(TextureLayered);
	KSREGISTER_ABSTRACT_CLASS(ImageTextureLayered);
	KSREGISTER_VIRTUAL_CLASS(Texture3D);
	KSREGISTER_CLASS(ImageTexture3D);
	KSREGISTER_CLASS(CompressedTexture3D);
	KSREGISTER_CLASS(Cubemap);
	KSREGISTER_CLASS(CubemapArray);
	KSREGISTER_CLASS(Texture2DArray);
	KSREGISTER_ABSTRACT_CLASS(CompressedTextureLayered);
	KSREGISTER_CLASS(CompressedCubemap);
	KSREGISTER_CLASS(CompressedCubemapArray);
	KSREGISTER_CLASS(CompressedTexture2DArray);
	KSREGISTER_CLASS(PlaceholderTexture2D);
	KSREGISTER_CLASS(PlaceholderTexture3D);
	KSREGISTER_ABSTRACT_CLASS(PlaceholderTextureLayered);
	KSREGISTER_CLASS(PlaceholderTexture2DArray);
	KSREGISTER_CLASS(PlaceholderCubemap);
	KSREGISTER_CLASS(PlaceholderCubemapArray);

	// These classes are part of renderer_rd
	KSREGISTER_CLASS(Texture2DRD);
	KSREGISTER_ABSTRACT_CLASS(TextureLayeredRD);
	KSREGISTER_CLASS(Texture2DArrayRD);
	KSREGISTER_CLASS(TextureCubemapRD);
	KSREGISTER_CLASS(TextureCubemapArrayRD);
	KSREGISTER_CLASS(Texture3DRD);

	KSREGISTER_CLASS(Animation);
	KSREGISTER_CLASS(AnimationLibrary);

	KSREGISTER_ABSTRACT_CLASS(Font);
	KSREGISTER_CLASS(FontFile);
	KSREGISTER_CLASS(FontVariation);
	KSREGISTER_CLASS(SystemFont);
	KSREGISTER_CLASS(ColorPalette);

	KSREGISTER_CLASS(Curve);

	KSREGISTER_CLASS(LabelSettings);

	KSREGISTER_CLASS(TextLine);
	KSREGISTER_CLASS(TextParagraph);

	KSREGISTER_VIRTUAL_CLASS(StyleBox);
	KSREGISTER_CLASS(StyleBoxEmpty);
	KSREGISTER_CLASS(StyleBoxTexture);
	KSREGISTER_CLASS(StyleBoxFlat);
	KSREGISTER_CLASS(StyleBoxLine);
	KSREGISTER_CLASS(Theme);

	KSREGISTER_CLASS(BitMap);
	KSREGISTER_CLASS(Gradient);

	KSREGISTER_CLASS(SkeletonProfile);
	KSREGISTER_CLASS(SkeletonProfileHumanoid);
	KSREGISTER_CLASS(BoneMap);

	OS::get_singleton()->yield(); // may take time to init

	KSREGISTER_CLASS(AudioStreamPlayer);
	KSREGISTER_CLASS(AudioStreamWAV);
	KSREGISTER_CLASS(AudioStreamPolyphonic);
	KSREGISTER_ABSTRACT_CLASS(AudioStreamPlaybackPolyphonic);

	OS::get_singleton()->yield(); // may take time to init

	KSREGISTER_CLASS(AudioStreamPlayer2D);
	KSREGISTER_ABSTRACT_CLASS(Shape2D);
	KSREGISTER_CLASS(WorldBoundaryShape2D);
	KSREGISTER_CLASS(SegmentShape2D);
	KSREGISTER_CLASS(SeparationRayShape2D);
	KSREGISTER_CLASS(CircleShape2D);
	KSREGISTER_CLASS(RectangleShape2D);
	KSREGISTER_CLASS(CapsuleShape2D);
	KSREGISTER_CLASS(ConvexPolygonShape2D);
	KSREGISTER_CLASS(ConcavePolygonShape2D);
	KSREGISTER_CLASS(Curve2D);
	KSREGISTER_CLASS(Path2D);
	KSREGISTER_CLASS(PathFollow2D);
	KSREGISTER_CLASS(PolygonPathFinder);

	KSREGISTER_CLASS(NavigationMesh);
	KSREGISTER_CLASS(NavigationMeshSourceGeometryData2D);
	KSREGISTER_CLASS(NavigationPolygon);
	KSREGISTER_CLASS(NavigationRegion2D);
	KSREGISTER_CLASS(NavigationAgent2D);
	KSREGISTER_CLASS(NavigationObstacle2D);
	KSREGISTER_CLASS(NavigationLink2D);

	OS::get_singleton()->yield(); // may take time to init

	KSREGISTER_ABSTRACT_CLASS(SceneState);
	KSREGISTER_CLASS(PackedScene);

	KSREGISTER_CLASS(SceneTree);
	KSREGISTER_ABSTRACT_CLASS(SceneTreeTimer); // sorry, you can't create it

#ifndef DISABLE_DEPRECATED
	// Dropped in 4.0, near approximation.
	ClassDB::add_compatibility_class("AnimationTreePlayer", "AnimationTree");
	ClassDB::add_compatibility_class("BakedLightmap", "LightmapGI");
	ClassDB::add_compatibility_class("BakedLightmapData", "LightmapGIData");
	ClassDB::add_compatibility_class("BitmapFont", "FontFile");
	ClassDB::add_compatibility_class("DynamicFont", "FontFile");
	ClassDB::add_compatibility_class("DynamicFontData", "FontFile");
	ClassDB::add_compatibility_class("Navigation3D", "Node3D");
	ClassDB::add_compatibility_class("Navigation2D", "Node2D");
	ClassDB::add_compatibility_class("OpenSimplexNoise", "FastNoiseLite");
	ClassDB::add_compatibility_class("ProximityGroup", "Node3D");
	ClassDB::add_compatibility_class("ToolButton", "Button");
	ClassDB::add_compatibility_class("YSort", "Node2D");
	// Portal and room occlusion was replaced by raster occlusion (OccluderInstance3D node).
	ClassDB::add_compatibility_class("Portal", "Node3D");
	ClassDB::add_compatibility_class("Room", "Node3D");
	ClassDB::add_compatibility_class("RoomManager", "Node3D");
	ClassDB::add_compatibility_class("RoomGroup", "Node3D");
	ClassDB::add_compatibility_class("Occluder", "Node3D");
	// The OccluderShapeSphere resource (used in the old Occluder node) is not present anymore.
	ClassDB::add_compatibility_class("OccluderShapeSphere", "Resource");

	// Renamed in 4.0.
	// Keep alphabetical ordering to easily locate classes and avoid duplicates.
	ClassDB::add_compatibility_class("AnimatedSprite", "AnimatedSprite2D");
	ClassDB::add_compatibility_class("Area", "Area3D");
	ClassDB::add_compatibility_class("ARVRCamera", "XRCamera3D");
	ClassDB::add_compatibility_class("ARVRController", "XRController3D");
	ClassDB::add_compatibility_class("ARVRAnchor", "XRAnchor3D");
	ClassDB::add_compatibility_class("ARVRInterface", "XRInterface");
	ClassDB::add_compatibility_class("ARVROrigin", "XROrigin3D");
	ClassDB::add_compatibility_class("ARVRPositionalTracker", "XRPositionalTracker");
	ClassDB::add_compatibility_class("ARVRServer", "XRServer");
	ClassDB::add_compatibility_class("AStar", "AStar3D");
	ClassDB::add_compatibility_class("BoneAttachment", "BoneAttachment3D");
	ClassDB::add_compatibility_class("BoxShape", "BoxShape3D");
	ClassDB::add_compatibility_class("Camera", "Camera3D");
	ClassDB::add_compatibility_class("CapsuleShape", "CapsuleShape3D");
	ClassDB::add_compatibility_class("ClippedCamera", "ClippedCamera3D");
	ClassDB::add_compatibility_class("CollisionObject", "CollisionObject3D");
	ClassDB::add_compatibility_class("CollisionPolygon", "CollisionPolygon3D");
	ClassDB::add_compatibility_class("CollisionShape", "CollisionShape3D");
	ClassDB::add_compatibility_class("ConcavePolygonShape", "ConcavePolygonShape3D");
	ClassDB::add_compatibility_class("ConeTwistJoint", "ConeTwistJoint3D");
	ClassDB::add_compatibility_class("ConvexPolygonShape", "ConvexPolygonShape3D");
	ClassDB::add_compatibility_class("CPUParticles", "CPUParticles3D");
	ClassDB::add_compatibility_class("CSGBox", "CSGBox3D");
	ClassDB::add_compatibility_class("CSGCombiner", "CSGCombiner3D");
	ClassDB::add_compatibility_class("CSGCylinder", "CSGCylinder3D");
	ClassDB::add_compatibility_class("CSGMesh", "CSGMesh3D");
	ClassDB::add_compatibility_class("CSGPolygon", "CSGPolygon3D");
	ClassDB::add_compatibility_class("CSGPrimitive", "CSGPrimitive3D");
	ClassDB::add_compatibility_class("CSGShape", "CSGShape3D");
	ClassDB::add_compatibility_class("CSGSphere", "CSGSphere3D");
	ClassDB::add_compatibility_class("CSGTorus", "CSGTorus3D");
	ClassDB::add_compatibility_class("CubeMesh", "BoxMesh");
	ClassDB::add_compatibility_class("CylinderShape", "CylinderShape3D");
	ClassDB::add_compatibility_class("DirectionalLight", "DirectionalLight3D");
	ClassDB::add_compatibility_class("EditorSpatialGizmo", "EditorNode3DGizmo");
	ClassDB::add_compatibility_class("EditorSpatialGizmoPlugin", "EditorNode3DGizmoPlugin");
	ClassDB::add_compatibility_class("Generic6DOFJoint", "Generic6DOFJoint3D");
	ClassDB::add_compatibility_class("GIProbe", "VoxelGI");
	ClassDB::add_compatibility_class("GIProbeData", "VoxelGIData");
	ClassDB::add_compatibility_class("GradientTexture", "GradientTexture1D");
	ClassDB::add_compatibility_class("HeightMapShape", "HeightMapShape3D");
	ClassDB::add_compatibility_class("HingeJoint", "HingeJoint3D");
	ClassDB::add_compatibility_class("Joint", "Joint3D");
	ClassDB::add_compatibility_class("KinematicBody", "CharacterBody3D");
	ClassDB::add_compatibility_class("KinematicBody2D", "CharacterBody2D");
	ClassDB::add_compatibility_class("KinematicCollision", "KinematicCollision3D");
	ClassDB::add_compatibility_class("Light", "Light3D");
	ClassDB::add_compatibility_class("Light2D", "PointLight2D");
	ClassDB::add_compatibility_class("LineShape2D", "WorldBoundaryShape2D");
	ClassDB::add_compatibility_class("Listener", "AudioListener3D");
	ClassDB::add_compatibility_class("MeshInstance", "MeshInstance3D");
	ClassDB::add_compatibility_class("MultiMeshInstance", "MultiMeshInstance3D");
	ClassDB::add_compatibility_class("NavigationAgent", "NavigationAgent3D");
	ClassDB::add_compatibility_class("NavigationMeshInstance", "NavigationRegion3D");
	ClassDB::add_compatibility_class("NavigationObstacle", "NavigationObstacle3D");
	ClassDB::add_compatibility_class("NavigationPolygonInstance", "NavigationRegion2D");
	ClassDB::add_compatibility_class("NavigationRegion", "NavigationRegion3D");
	ClassDB::add_compatibility_class("Navigation2DServer", "NavigationServer2D");
	ClassDB::add_compatibility_class("NavigationServer", "NavigationServer3D");
	ClassDB::add_compatibility_class("OmniLight", "OmniLight3D");
	ClassDB::add_compatibility_class("PanoramaSky", "Sky");
	ClassDB::add_compatibility_class("Particles", "GPUParticles3D");
	ClassDB::add_compatibility_class("Particles2D", "GPUParticles2D");
	ClassDB::add_compatibility_class("ParticlesMaterial", "ParticleProcessMaterial");
	ClassDB::add_compatibility_class("Path", "Path3D");
	ClassDB::add_compatibility_class("PathFollow", "PathFollow3D");
	ClassDB::add_compatibility_class("PhysicalBone", "PhysicalBone3D");
	ClassDB::add_compatibility_class("Physics2DDirectBodyState", "PhysicsDirectBodyState2D");
	ClassDB::add_compatibility_class("Physics2DDirectSpaceState", "PhysicsDirectSpaceState2D");
	ClassDB::add_compatibility_class("Physics2DServer", "PhysicsServer2D");
	ClassDB::add_compatibility_class("Physics2DShapeQueryParameters", "PhysicsShapeQueryParameters2D");
	ClassDB::add_compatibility_class("Physics2DTestMotionResult", "PhysicsTestMotionResult2D");
	ClassDB::add_compatibility_class("PhysicsBody", "PhysicsBody3D");
	ClassDB::add_compatibility_class("PhysicsDirectBodyState", "PhysicsDirectBodyState3D");
	ClassDB::add_compatibility_class("PhysicsDirectSpaceState", "PhysicsDirectSpaceState3D");
	ClassDB::add_compatibility_class("PhysicsServer", "PhysicsServer3D");
	ClassDB::add_compatibility_class("PhysicsShapeQueryParameters", "PhysicsShapeQueryParameters3D");
	ClassDB::add_compatibility_class("PinJoint", "PinJoint3D");
	ClassDB::add_compatibility_class("PlaneShape", "WorldBoundaryShape3D");
	ClassDB::add_compatibility_class("Position2D", "Marker2D");
	ClassDB::add_compatibility_class("Position3D", "Marker3D");
	ClassDB::add_compatibility_class("ProceduralSky", "Sky");
	ClassDB::add_compatibility_class("RayCast", "RayCast3D");
	ClassDB::add_compatibility_class("RayShape", "SeparationRayShape3D");
	ClassDB::add_compatibility_class("RayShape2D", "SeparationRayShape2D");
	ClassDB::add_compatibility_class("RemoteTransform", "RemoteTransform3D");
	ClassDB::add_compatibility_class("RigidBody", "RigidBody3D");
	ClassDB::add_compatibility_class("RigidDynamicBody2D", "RigidBody2D");
	ClassDB::add_compatibility_class("RigidDynamicBody3D", "RigidBody3D");
	ClassDB::add_compatibility_class("Shape", "Shape3D");
	ClassDB::add_compatibility_class("ShortCut", "Shortcut");
	ClassDB::add_compatibility_class("Skeleton", "Skeleton3D");
	ClassDB::add_compatibility_class("SkeletonIK", "SkeletonIK3D");
	ClassDB::add_compatibility_class("SliderJoint", "SliderJoint3D");
	ClassDB::add_compatibility_class("SoftBody", "SoftBody3D");
	ClassDB::add_compatibility_class("SoftDynamicBody3D", "SoftBody3D");
	ClassDB::add_compatibility_class("Spatial", "Node3D");
	ClassDB::add_compatibility_class("SpatialGizmo", "Node3DGizmo");
	ClassDB::add_compatibility_class("SpatialMaterial", "StandardMaterial3D");
	ClassDB::add_compatibility_class("SphereShape", "SphereShape3D");
	ClassDB::add_compatibility_class("SpotLight", "SpotLight3D");
	ClassDB::add_compatibility_class("SpringArm", "SpringArm3D");
	ClassDB::add_compatibility_class("Sprite", "Sprite2D");
	ClassDB::add_compatibility_class("StaticBody", "StaticBody3D");
	ClassDB::add_compatibility_class("StreamTexture", "CompressedTexture2D");
	ClassDB::add_compatibility_class("TextureProgress", "TextureProgressBar");
	ClassDB::add_compatibility_class("VehicleBody", "VehicleBody3D");
	ClassDB::add_compatibility_class("VehicleWheel", "VehicleWheel3D");
	ClassDB::add_compatibility_class("VideoPlayer", "VideoStreamPlayer");
	ClassDB::add_compatibility_class("ViewportContainer", "SubViewportContainer");
	ClassDB::add_compatibility_class("Viewport", "SubViewport");
	ClassDB::add_compatibility_class("VisibilityEnabler", "VisibleOnScreenEnabler3D");
	ClassDB::add_compatibility_class("VisibilityNotifier", "VisibleOnScreenNotifier3D");
	ClassDB::add_compatibility_class("VisibilityNotifier2D", "VisibleOnScreenNotifier2D");
	ClassDB::add_compatibility_class("VisibilityNotifier3D", "VisibleOnScreenNotifier3D");
	ClassDB::add_compatibility_class("VisualServer", "RenderingServer");
	ClassDB::add_compatibility_class("World", "World3D");

	// VisualShader classes.
	ClassDB::add_compatibility_class("VisualShaderNodeScalarConstant", "VisualShaderNodeFloatConstant");
	ClassDB::add_compatibility_class("VisualShaderNodeScalarFunc", "VisualShaderNodeFloatFunc");
	ClassDB::add_compatibility_class("VisualShaderNodeScalarOp", "VisualShaderNodeFloatOp");
	ClassDB::add_compatibility_class("VisualShaderNodeScalarClamp", "VisualShaderNodeClamp");
	ClassDB::add_compatibility_class("VisualShaderNodeVectorClamp", "VisualShaderNodeClamp");
	ClassDB::add_compatibility_class("VisualShaderNodeScalarInterp", "VisualShaderNodeMix");
	ClassDB::add_compatibility_class("VisualShaderNodeVectorInterp", "VisualShaderNodeMix");
	ClassDB::add_compatibility_class("VisualShaderNodeVectorScalarMix", "VisualShaderNodeMix");
	ClassDB::add_compatibility_class("VisualShaderNodeScalarSmoothStep", "VisualShaderNodeSmoothStep");
	ClassDB::add_compatibility_class("VisualShaderNodeVectorSmoothStep", "VisualShaderNodeSmoothStep");
	ClassDB::add_compatibility_class("VisualShaderNodeVectorScalarSmoothStep", "VisualShaderNodeSmoothStep");
	ClassDB::add_compatibility_class("VisualShaderNodeVectorScalarStep", "VisualShaderNodeStep");
	ClassDB::add_compatibility_class("VisualShaderNodeScalarSwitch", "VisualShaderNodeSwitch");
	ClassDB::add_compatibility_class("VisualShaderNodeScalarTransformMult", "VisualShaderNodeTransformOp");
	ClassDB::add_compatibility_class("VisualShaderNodeScalarDerivativeFunc", "VisualShaderNodeDerivativeFunc");
	ClassDB::add_compatibility_class("VisualShaderNodeVectorDerivativeFunc", "VisualShaderNodeDerivativeFunc");

	ClassDB::add_compatibility_class("VisualShaderNodeBooleanUniform", "VisualShaderNodeBooleanParameter");
	ClassDB::add_compatibility_class("VisualShaderNodeColorUniform", "VisualShaderNodeColorParameter");
	ClassDB::add_compatibility_class("VisualShaderNodeScalarUniform", "VisualShaderNodeFloatParameter");
	ClassDB::add_compatibility_class("VisualShaderNodeCubeMapUniform", "VisualShaderNodeCubeMapParameter");
	ClassDB::add_compatibility_class("VisualShaderNodeTextureUniform", "VisualShaderNodeTexture2DParameter");
	ClassDB::add_compatibility_class("VisualShaderNodeTextureUniformTriplanar", "VisualShaderNodeTextureParameterTriplanar");
	ClassDB::add_compatibility_class("VisualShaderNodeTransformUniform", "VisualShaderNodeTransformParameter");
	ClassDB::add_compatibility_class("VisualShaderNodeVec3Uniform", "VisualShaderNodeVec3Parameter");
	ClassDB::add_compatibility_class("VisualShaderNodeUniform", "VisualShaderNodeParameter");
	ClassDB::add_compatibility_class("VisualShaderNodeUniformRef", "VisualShaderNodeParameterRef");

	// Renamed during 4.0 alpha, added to ease transition between alphas.
	ClassDB::add_compatibility_class("AudioStreamOGGVorbis", "AudioStreamOggVorbis");
	ClassDB::add_compatibility_class("AudioStreamSample", "AudioStreamWAV");
	ClassDB::add_compatibility_class("OGGPacketSequence", "OggPacketSequence");
	ClassDB::add_compatibility_class("StreamCubemap", "CompressedCubemap");
	ClassDB::add_compatibility_class("StreamCubemapArray", "CompressedCubemapArray");
	ClassDB::add_compatibility_class("StreamTexture2D", "CompressedTexture2D");
	ClassDB::add_compatibility_class("StreamTexture2DArray", "CompressedTexture2DArray");
	ClassDB::add_compatibility_class("StreamTexture3D", "CompressedTexture3D");
	ClassDB::add_compatibility_class("StreamTextureLayered", "CompressedTextureLayered");
	ClassDB::add_compatibility_class("VisualShaderNodeFloatUniform", "VisualShaderNodeFloatParameter");
#endif /* DISABLE_DEPRECATED */

	OS::get_singleton()->yield(); // may take time to init

	for (int i = 0; i < 20; i++) {
		GLOBAL_DEF_BASIC(vformat("%s/layer_%d", PNAME("layer_names/2d_render"), i + 1), "");
		GLOBAL_DEF_BASIC(vformat("%s/layer_%d", PNAME("layer_names/3d_render"), i + 1), "");
	}

	for (int i = 0; i < 32; i++) {
		GLOBAL_DEF_BASIC(vformat("%s/layer_%d", PNAME("layer_names/2d_physics"), i + 1), "");
		GLOBAL_DEF_BASIC(vformat("%s/layer_%d", PNAME("layer_names/2d_navigation"), i + 1), "");
		GLOBAL_DEF_BASIC(vformat("%s/layer_%d", PNAME("layer_names/3d_physics"), i + 1), "");
		GLOBAL_DEF_BASIC(vformat("%s/layer_%d", PNAME("layer_names/3d_navigation"), i + 1), "");
	}

	for (int i = 0; i < 32; i++) {
		GLOBAL_DEF_BASIC(vformat("%s/layer_%d", PNAME("layer_names/avoidance"), i + 1), "");
	}

	if (RenderingServer::get_singleton()) {
		// RenderingServer needs to exist for this to succeed.
		ColorPicker::init_shaders();
		GraphEdit::init_shaders();
	}

	SceneDebugger::initialize();

	OS::get_singleton()->benchmark_end_measure("Scene", "Register Types");
}

void unregister_scene_types() {
	OS::get_singleton()->benchmark_begin_measure("Scene", "Unregister Types");

	SceneDebugger::deinitialize();

	ResourceLoader::remove_resource_format_loader(resource_loader_texture_layered);
	resource_loader_texture_layered.unref();

	ResourceLoader::remove_resource_format_loader(resource_loader_texture_3d);
	resource_loader_texture_3d.unref();

	ResourceLoader::remove_resource_format_loader(resource_loader_stream_texture);
	resource_loader_stream_texture.unref();

	ResourceSaver::remove_resource_format_saver(resource_saver_text);
	resource_saver_text.unref();

	ResourceLoader::remove_resource_format_loader(resource_loader_text);
	resource_loader_text.unref();

	ResourceSaver::remove_resource_format_saver(resource_saver_shader);
	resource_saver_shader.unref();

	ResourceLoader::remove_resource_format_loader(resource_loader_shader);
	resource_loader_shader.unref();

	ResourceSaver::remove_resource_format_saver(resource_saver_shader_include);
	resource_saver_shader_include.unref();

	ResourceLoader::remove_resource_format_loader(resource_loader_shader_include);
	resource_loader_shader_include.unref();

	// StandardMaterial3D is not initialized when 3D is disabled, so it shouldn't be cleaned up either
#ifndef _3D_DISABLED
	BaseMaterial3D::finish_shaders();
	PhysicalSkyMaterial::cleanup_shader();
	PanoramaSkyMaterial::cleanup_shader();
	ProceduralSkyMaterial::cleanup_shader();
	FogMaterial::cleanup_shader();
#endif // _3D_DISABLED

	ParticleProcessMaterial::finish_shaders();
	CanvasItemMaterial::finish_shaders();
	ColorPicker::finish_shaders();
	GraphEdit::finish_shaders();
	SceneStringNames::free();

	OS::get_singleton()->benchmark_end_measure("Scene", "Unregister Types");
}

void register_scene_singletons() {
	OS::get_singleton()->benchmark_begin_measure("Scene", "Register Singletons");

	KSREGISTER_CLASS(ThemeDB);

	Engine::get_singleton()->add_singleton(Engine::Singleton("ThemeDB", ThemeDB::get_singleton()));

	OS::get_singleton()->benchmark_end_measure("Scene", "Register Singletons");
}
