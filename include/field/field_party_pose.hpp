#pragma once
#include "battle_pose_pool.hpp"
#include <cassert>
namespace ttyd {
// Legacy standalone views retain local values. Fresh original sessions alias
// actual source scalar words, never a periodically synchronized duplicate.
// bind changes only the target; it does not publish local defaults. Assignment
// copies VALUE into the destination owner, not another wrapper's binding.
class FieldPoseWord {
public:
 FieldPoseWord(uint32_t value=0):local_(value){}
 operator uint32_t() const{return pose_?pose_->word(offset_):local_;}
 FieldPoseWord& operator=(uint32_t value){if(pose_)pose_->word(offset_,value);else local_=value;return *this;}
 FieldPoseWord& operator=(const FieldPoseWord& other){return *this=uint32_t(other);}
 FieldPoseWord& operator|=(uint32_t value){return *this=uint32_t(*this)|value;}
 void bind(BattlePose* p,unsigned offset){pose_=p;offset_=offset;}
private:
 uint32_t local_;BattlePose* pose_=nullptr;unsigned offset_=0;
};
// Source words18(high)/1c(low), BE accessor. Assignment writes low then high
// in that explicit order. Legacy local storage is used only without backing.
class FieldPoseTimestamp {
public:
 operator uint64_t() const{return pose_?pose_->timestamp():local_;}
 FieldPoseTimestamp& operator=(uint64_t value){if(pose_){pose_->word(0x1c,uint32_t(value));pose_->word(0x18,uint32_t(value>>32));}else local_=value;return *this;}
 FieldPoseTimestamp& operator=(const FieldPoseTimestamp& other){return *this=uint64_t(other);}
 void bind(BattlePose* pose){pose_=pose;}
private:
 uint64_t local_=0;BattlePose* pose_=nullptr;
};
// Typed flags0/materiale8/lightec/timestamp/shape directly alias one live
// BattlePose. Resource stays immutable. Manual release of this handle must
// retire views first; wrapper destruction is not original pose AutoRelease.
struct FieldPartyPose {
private:
 std::shared_ptr<BattlePosePool> backing_;
 int32_t handle_=-1;
 ActorAnimationSample localShape_;
 void bind(){if(backing_){auto* p=backing_->pose(handle_);assert(p);flags.bind(p,0);materialFlags.bind(p,0xe8);materialLightFlags.bind(p,0xec);enteredMilliseconds.bind(p);}}
public:
 FieldPoseWord flags{1},materialFlags,materialLightFlags;
 uint32_t sourceResource=0;
 int32_t mode=2,archiveType=0;
 FieldPoseTimestamp enteredMilliseconds;
 std::shared_ptr<const ActorAnimationPack> resource;
 ActorAnimationSample& shape;
 FieldPartyPose():shape(localShape_){}
 FieldPartyPose(std::shared_ptr<BattlePosePool> pool,int32_t handle,uint32_t source):backing_(std::move(pool)),handle_(handle),sourceResource(source),resource(backing_->pose(handle)->resource->bank),shape(backing_->pose(handle)->shape){bind();archiveType=backing_->pose(handle)->archiveType;}
 // Individual copy clones its backing namespace; registry copies instead clone
 // once and rebind all role views to that ONE clone. Shape reference must
 // target copied storage, not original Game's CPU workspace.
 FieldPartyPose(const FieldPartyPose& other):backing_(other.backing_?std::make_shared<BattlePosePool>(*other.backing_):nullptr),handle_(other.handle_),sourceResource(other.sourceResource),mode(other.mode),archiveType(other.archiveType),resource(other.resource),shape(backing_?backing_->pose(handle_)->shape:localShape_){bind();flags=uint32_t(other.flags);materialFlags=uint32_t(other.materialFlags);materialLightFlags=uint32_t(other.materialLightFlags);enteredMilliseconds=uint64_t(other.enteredMilliseconds);if(!backing_)shape=other.shape;}
 void paperOff(){flags=uint32_t(flags)&~8u;}
 BattlePose* original() const{return backing_?backing_->pose(handle_):nullptr;}
};
// Original token order a_mario,a_mario_r,e_mario,c_pkr.
bool loadFieldPlayerPoseAssets(const char* root);
std::array<std::shared_ptr<const BattlePoseResource>,4> fieldPlayerPoseResources();
}
