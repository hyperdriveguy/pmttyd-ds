#include "field_camera_publication.hpp"
#include <cmath>
namespace ttyd {
// Only source camera 4 perspective mode before battle-mode 2 is adapted.
// Candidate-copy validation keeps old camera intact on nonfinite/singular input.
// Engine units=native world*100: convert eye/aim, near/far, view translation and
// projection Z translation; do not rescale view orientation or projection lens.
bool publishNativeFieldCamera(CameraManager& bank,const FieldCameraFrame& frame){
    auto* camera=bank.camera(4);if(!camera || camera->mode==2 || camera->projectionKind!=0)return false;
    for(const auto& row:frame.view)for(float value:row)if(!std::isfinite(value))return false;
    for(float value:frame.projection)if(!std::isfinite(value))return false;
    for(float value:{frame.eye.x,frame.eye.y,frame.eye.z,frame.at.x,frame.at.y,frame.at.z,frame.fov,frame.nearPlane,frame.farPlane,frame.yaw})if(!std::isfinite(value))return false;
    if(frame.fov<=0 || frame.fov>=180 || frame.nearPlane<=0 || frame.farPlane<=frame.nearPlane || frame.projection[0]==0 || frame.projection[2]==0)return false;
    const auto& v=frame.view;
    const float determinant=v[0][0]*(v[1][1]*v[2][2]-v[1][2]*v[2][1])-v[0][1]*(v[1][0]*v[2][2]-v[1][2]*v[2][0])+v[0][2]*(v[1][0]*v[2][1]-v[1][1]*v[2][0]);
    if(!std::isfinite(determinant) || determinant==0)return false;
    auto next=*camera;
    next.eye={frame.eye.x*100,frame.eye.y*100,frame.eye.z*100};next.at={frame.at.x*100,frame.at.y*100,frame.at.z*100};
    next.fov=frame.fov;next.nearPlane=frame.nearPlane*100;next.farPlane=frame.farPlane*100;next.yaw=frame.yaw;
    const float dx=next.eye[0]-next.at[0],dy=next.eye[1]-next.at[1],dz=next.eye[2]-next.at[2];next.distance=std::sqrt(dx*dx+dy*dy+dz*dz);
    next.matrices.view=frame.view;for(auto& row:next.matrices.view)row[3]*=100;
    const auto& p=frame.projection;next.matrices.projection={{{p[0],0,p[1],0},{0,p[2],p[3],0},{0,0,p[4],p[5]*100},{0,0,-1,0}}};
    for(float value:{next.eye[0],next.eye[1],next.eye[2],next.at[0],next.at[1],next.at[2],next.nearPlane,next.farPlane,next.distance})if(!std::isfinite(value))return false;
    for(const auto& row:next.matrices.view)for(float value:row)if(!std::isfinite(value))return false;
    for(const auto& row:next.matrices.projection)for(float value:row)if(!std::isfinite(value))return false;
    next.viewPublished=true;*camera=next;return true;
}
}
