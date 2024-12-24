/**************************************************************************/
/*  gltf_skeleton.h                                                       */
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

#ifndef GLTF_SKELETON_H
#define GLTF_SKELETON_H

#include "../gltf_defines.h"

#include "core/io/resource.h"
#include "scene/3d/bone_attachment_3d.h"
#include "scene/3d/skeleton_3d.h"

class GLTFSkeleton : public Resource {
	KSCLASS(GLTFSkeleton, Resource);
	friend class GLTFDocument;
	friend class SkinTool;
	friend class FBXDocument;

private:
	// The *synthesized* skeletons joints
	Vector<GLTFNodeIndex> joints;

	// The roots of the skeleton. If there are multiple, each root must have the
	// same parent (ie roots are siblings)
	Vector<GLTFNodeIndex> roots;

	// The created Skeleton3D for the scene
	Skeleton3D *kosmic_skeleton = nullptr;

	// Set of unique bone names for the skeleton
	HashSet<String> unique_names;

	HashMap<int32_t, GLTFNodeIndex> kosmic_bone_node;

	Vector<BoneAttachment3D *> bone_attachments;

protected:
	static void _bind_methods();

public:
	Vector<GLTFNodeIndex> get_joints();
	void set_joints(Vector<GLTFNodeIndex> p_joints);

	Vector<GLTFNodeIndex> get_roots();
	void set_roots(Vector<GLTFNodeIndex> p_roots);

	Skeleton3D *get_kosmic_skeleton();

	// Skeleton *get_kosmic_skeleton() {
	// 	return kosmic_skeleton;
	// }
	// void set_kosmic_skeleton(Skeleton p_*kosmic_skeleton) {
	// 	kosmic_skeleton = p_kosmic_skeleton;
	// }

	TypedArray<String> get_unique_names();
	void set_unique_names(TypedArray<String> p_unique_names);

	//RBMap<int32_t, GLTFNodeIndex> get_kosmic_bone_node() {
	//	return kosmic_bone_node;
	//}
	//void set_kosmic_bone_node(const RBMap<int32_t, GLTFNodeIndex> &p_kosmic_bone_node) {
	//	kosmic_bone_node = p_kosmic_bone_node;
	//}
	Dictionary get_kosmic_bone_node();
	void set_kosmic_bone_node(Dictionary p_indict);

	//Dictionary get_kosmic_bone_node() {
	//	return VariantConversion::to_dict(kosmic_bone_node);
	//}
	//void set_kosmic_bone_node(Dictionary p_indict) {
	//	VariantConversion::set_from_dict(kosmic_bone_node, p_indict);
	//}

	BoneAttachment3D *get_bone_attachment(int idx);

	int32_t get_bone_attachment_count();
};

#endif // GLTF_SKELETON_H
