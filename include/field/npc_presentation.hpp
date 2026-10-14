#pragma once
#include "actor_animation.hpp"
#include "field_camera_state.hpp"
#include "message_playback.hpp"
namespace ttyd {
class NpcAngleTable {
public:
    bool load(const char*);
    float angle(float fromX,float fromZ,float toX,float toZ) const;
private:
    std::array<float,91> values{};
};
float npcReviseAngle(float);
// The local NPC pack must retain the original bank's descriptor0 first.
const ActorAnimationPose* selectNpcAnimation(const ActorAnimationPack&,const char* name);
struct NpcCamera {CameraPoint eye,at;};
struct NpcCameraService {void* user=nullptr;NpcCamera (*read)(void*)=nullptr;};
struct NpcMatrixInput {
    ActorVector root{},pivot{},scale{{1,1,1}},rotation{}; // X/Y/Z additive rotation, not event yaw.
    uint32_t flags=0;
    float yaw=0,drawScale=2;
};
class NpcPoseClock {
public:
    std::string pose,basePose;
    uint64_t poseAt=0,messageSerial=0,requestSerial=0;
    float update(const std::string& base,const std::string& talk,const MessagePlayback* message,uint64_t now);
};
class NpcPresentation {
public:
    // Fresh entry initializes angles0; sign is published only by Main.
    uint32_t flags=9;
    float incoming=0,retained=0,target=0,sign=0;
    ActorMatrix matrix;
    bool prepare(const NpcMatrixInput&,const NpcAngleTable&,const NpcCameraService&);
    ActorVector point(const ActorMeshVertex& v) const {
        auto p=matrix.point({v.x,v.y,v.z});for(auto& f:p)f*=.01f;return p;
    }
    static CameraPoint talkAnchor(const ActorVector& root,float sourceHeight) {
        return {root[0]*.01f,(root[1]+sourceHeight)*.01f,root[2]*.01f};
    }
};
}
