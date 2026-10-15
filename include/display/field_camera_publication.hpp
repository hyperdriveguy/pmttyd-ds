#pragma once
#include "camera_manager.hpp"
#include "field_camera_state.hpp"
namespace ttyd {
// Publish the current native field frame in source units before battle mode2.
// Retained flags, roll/extra, up, aspect and viewport remain separately owned.
// false publishes nothing. Only eye/aim/lens/yaw/matrices and publication
// state change; source flags/roll/up/aspect/viewport retain separate ownership.
bool publishNativeFieldCamera(CameraManager&,const FieldCameraFrame&);
}
