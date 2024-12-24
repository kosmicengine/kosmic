/**************************************************************************/
/*  webrtc_peer_connection_extension.cpp                                  */
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

#include "webrtc_peer_connection_extension.h"

void WebRTCPeerConnectionExtension::_bind_methods() {
	KSVIRTUAL_BIND(_get_connection_state);
	KSVIRTUAL_BIND(_get_gathering_state);
	KSVIRTUAL_BIND(_get_signaling_state);
	KSVIRTUAL_BIND(_initialize, "p_config");
	KSVIRTUAL_BIND(_create_data_channel, "p_label", "p_config");
	KSVIRTUAL_BIND(_create_offer);
	KSVIRTUAL_BIND(_set_remote_description, "p_type", "p_sdp");
	KSVIRTUAL_BIND(_set_local_description, "p_type", "p_sdp");
	KSVIRTUAL_BIND(_add_ice_candidate, "p_sdp_mid_name", "p_sdp_mline_index", "p_sdp_name");
	KSVIRTUAL_BIND(_poll);
	KSVIRTUAL_BIND(_close);
}
