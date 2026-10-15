#pragma once
#include "animation_startup_process.hpp"
#include "mario_presentation.hpp"
#include "animation_pose_release.hpp"
#include "native_npc_inner_services.hpp"
namespace ttyd {
struct FieldNpcAnimationServices;
struct FieldNpcDrawRequest;
class AnimationNativeInner;
struct NativeNpcInnerBackend;
// Native simulation service over the real cold VM, source files and one pose
// namespace. Creation precedes Mario's three Entries. Copies of the typed pool
// deliberately do not inherit these borrowing services.
class NativeFieldAnimation {
public:
 static std::shared_ptr<NativeFieldAnimation> create(const char* root,std::shared_ptr<NativeFileCache>,const EventNativePlatform&);
 ~NativeFieldAnimation();
 std::shared_ptr<BattlePosePool> pool() const{return pool_;}
 AnimationStartupProcess& process(){return *process_;}
 bool ready() const;
 bool clock(BattlePoseClock&) const;
 // One simulation visit, including paused visits if the caller supplies them.
 // Native timebase is162MHz/4; rational60Hz simulation inputs are explicit port
 // policy. Source retrace prefix owns bank/freeze arithmetic, not render calls.
 bool tick(bool battle,bool paused=false);
 bool mario(const MarioPresentation&,const std::array<int32_t,3>& handles,unsigned budget=8192);
 bool resumeMario(unsigned budget=8192);
 bool presentationPending() const{return marioActive_ || (process_ && process_->presentationPending());}
 bool producerPending() const{return npcOwner_ || presentationPending() || (process_ && process_->producerPending());}
 bool configurePresentation(std::shared_ptr<const CameraManager>);
 // Caller retains the complete field callback/queue/camera pass until end.
 // current is pure, non-reentrant and independent of animation source stores.
 // Owner token is identity only; its lifetime and predicate outlive the lease.
 bool beginNpcPass(const void* owner,uint64_t pass,std::shared_ptr<const CameraManager>,std::function<bool()> current);
 bool endNpcPass(const void* owner,uint64_t pass);
 bool npcPassPending() const{return npcOwner_!=nullptr;}
 FieldNpcAnimationServices npcAnimationServices();
 bool configureNpcInnerBackend(const NativeNpcInnerBackend&);
 bool configureNpcInner(const NativeNpcInnerServices&); // Quiescent provider, outlives pass/holds.
 bool npcDrawPending() const{return npcDraw_.active;}
 AnimationDrawWrapperFrontier npcDrawFrontier() const{return npcDrawFrontier_;}

 void publicationBudget(unsigned budget){budget_=budget;} // Native scheduling policy, not source timing.
 unsigned publicationBudget() const{return budget_;}
 float marioYaw() const{return publishedYaw_;}
 bool marioReflect() const{return publishedReflect_;}
 void setMarioRoot(ActorVector root){if(!producerPending())nextRoot_=root;}
 ActorVector marioRoot() const{return publishedRoot_;}
 // Retained source Release; ordinary Entry uses real fileFree. Smart buffers
 // require their own provider and retain the original smartFree barrier.
 bool release(int32_t handle);
 // Raw source Main only; native shape publication is a distinct next owner.
 // Caller configures process Main and pins its producer pass first.
 bool main(uint32_t handle,uint64_t pass,unsigned visits=64);
 uint32_t releasePending() const{return releasing_.pending;}
 const BattlePose* marioPose() const{return sampled_>=0?pool_->pose(sampled_):nullptr;}
 unsigned marioBank() const{return bank_;}
private:
 friend class AnimationNativeInner;
 NativeFieldAnimation()=default;
 std::optional<int32_t> enter(std::shared_ptr<const BattlePoseResource>,int32_t,const char*);
 std::shared_ptr<BattlePosePool> pool_;
 std::shared_ptr<NativeFileCache> files_;
 std::unique_ptr<AnimationStartupProcess> process_;
 std::string name_;std::shared_ptr<const BattlePoseResource> entering_;int32_t mode_=0;
 AnimationReleaseCursor releasing_;uint64_t releasingGeneration_=0;
 std::shared_ptr<const CameraManager> cameras_;uint64_t configuredMain_=0;
 ActorVector nextRoot_{},requestRoot_{},publishedRoot_{};
 bool marioActive_=false,setupDone_=false,selectionDone_=false;uint64_t marioPass_=0,requestSerial_=0;
 uint32_t requestBank_=0;int32_t requestHandle_=-1;uint16_t requestStart_=0;std::string requestName_;
 float requestYaw_=0,publishedYaw_=0;bool requestReflect_=false,publishedReflect_=false;
 const void* npcOwner_=nullptr;uint64_t npcPass_=0;std::function<bool()> npcCurrent_;
 bool drawNpc(const FieldNpcDrawRequest&);
 bool npcDrawCurrent() const;
 std::unique_ptr<AnimationNativeInner> npcInnerBinding_;
 NativeNpcInnerServices npcInner_;AnimationDrawWrapperCursor npcDraw_;
 AnimationDrawWrapperFrontier npcDrawFrontier_=AnimationDrawWrapperFrontier::Pending;
 uint32_t npcDrawHandle_=0;uint64_t npcDrawGeneration_=0;
 std::shared_ptr<const BattlePoseResource> npcDrawResource_;
 std::shared_ptr<AnimationScratchPose> npcDrawBuffers_;
 unsigned budget_=8192;
 uint64_t raw_=0,uploaded_=UINT64_MAX;int32_t selected_=-1,sampled_=-1;unsigned bank_=0;
};
}
