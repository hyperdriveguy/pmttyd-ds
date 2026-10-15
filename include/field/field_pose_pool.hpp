#pragma once
#include "battle_pose_pool.hpp"
#include <map>
namespace ttyd {
// Retained field resource wrapper over standalone or shared CPU poses. Requires real
// original MODEL/TPL data and descriptor0; it does not acknowledge GPU/facing
// Main readiness or a missing source file. Unavailable native data stays pending.
struct FieldPoseEntryCursor {
 const class FieldPosePool* owner=nullptr;
 std::shared_ptr<const BattlePoseResource> resource;
 std::string model;int32_t mode=0,result=0;bool admitted=false,complete=false;
};
class FieldPosePool {
public:
 // Caller pool shares global handles/model/texture caches with field roles
 // and battle. Null creates standalone legacy namespace; no live migration.
 explicit FieldPosePool(std::shared_ptr<BattlePosePool> pool={}):poses_(pool?std::move(pool):std::make_shared<BattlePosePool>()){}
 bool install(std::shared_ptr<const BattlePoseResource>);
 bool load(const char* directory,const char* pack);
 // Admission checks CPU pose/model/texture capacity only; successful entry
 // does not establish original NPC Main, world placement or GPU residency.
 bool canEnter(const std::string& model,int32_t mode) const;
 bool animationReady(const std::string& model,const std::string& animation) const;
 std::optional<int32_t> enter(const std::string& model,int32_t mode,BattlePoseClock);
 // One initial admission, then resume the exact pinned native Entry directly.
 // In-progress source cache publication must not be rejected as a new request.
 std::optional<int32_t> resumeEnter(const std::string&,int32_t,BattlePoseClock,FieldPoseEntryCursor&);
 bool paperOn(int32_t handle){return poses_->paperOn(handle);}
 bool materialLightFlagOn(int32_t handle,uint32_t flags){return poses_->materialLightFlagOn(handle,flags);}
 std::optional<int32_t> vivianType(int32_t handle) const{return poses_->vivianType(handle);}
 std::optional<float> height(int32_t handle) const{return poses_->height(handle);}
 std::optional<float> radius(int32_t handle) const{return poses_->radius(handle);}
 bool release(int32_t handle){return poses_->release(handle);}
 bool selectName(int32_t handle,const std::string& name,bool force,BattlePoseClock clock){return poses_->selectName(handle,name,force,clock);}
 bool evaluate(int32_t handle,BattlePoseClock clock){return poses_->evaluate(handle,clock);}
 bool main(int32_t handle,BattlePoseClock clock){return poses_->main(handle,clock);}
 const BattlePosePool& poses() const{return *poses_;}
 // Exposes the SAME pool identity; resource catalog remains this wrapper's
 // immutable bank map. No periodically synchronized duplicate pose records.
 std::shared_ptr<BattlePosePool> sharedPool() const{return poses_;}
private:
 std::map<std::string,std::shared_ptr<const BattlePoseResource>> resources_;
 std::shared_ptr<BattlePosePool> poses_;
};
}
