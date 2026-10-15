#pragma once
#include "battle_pose_pool.hpp"
#include <map>
namespace ttyd {
// Retained field-owned CPU pose/resource namespace. Installation requires real
// original MODEL/TPL data and descriptor0; it does not acknowledge GPU/facing
// Main readiness or a missing source file. Unavailable native data stays pending.
class FieldPosePool {
public:
 bool install(std::shared_ptr<const BattlePoseResource>);
 bool load(const char* directory,const char* pack);
 bool canEnter(const std::string& model,int32_t mode) const;
 bool animationReady(const std::string& model,const std::string& animation) const;
 std::optional<int32_t> enter(const std::string& model,int32_t mode,BattlePoseClock);
 bool paperOn(int32_t handle){return poses_.paperOn(handle);}
 bool materialLightFlagOn(int32_t handle,uint32_t flags){return poses_.materialLightFlagOn(handle,flags);}
 std::optional<int32_t> vivianType(int32_t handle) const{return poses_.vivianType(handle);}
 std::optional<float> height(int32_t handle) const{return poses_.height(handle);}
 std::optional<float> radius(int32_t handle) const{return poses_.radius(handle);}
 bool release(int32_t handle){return poses_.release(handle);}
 bool selectName(int32_t handle,const std::string& name,bool force,BattlePoseClock clock){return poses_.selectName(handle,name,force,clock);}
 bool evaluate(int32_t handle,BattlePoseClock clock){return poses_.evaluate(handle,clock);}
 bool main(int32_t handle,BattlePoseClock clock){return poses_.main(handle,clock);}
 const BattlePosePool& poses() const{return poses_;}
private:
 std::map<std::string,std::shared_ptr<const BattlePoseResource>> resources_;
 BattlePosePool poses_;
};
}
