#pragma once
#include "animation_initialize.hpp"
#include "animation_pool_memory.hpp"
#include "event_native_runtime.hpp"
#include "native_file_cache.hpp"
#include "animation_pose_entry.hpp"
#include "animation_buffer_refresh.hpp"
#include "animation_tangent_memory.hpp"
namespace ttyd {
// Owns a cold animation subsystem's actual VM/SDK arena/work/cache aliases.
// Runs cold startup and original Entry with real file/buffer owners. Live CPU
// scratch consumers and whole-Game platform/field wiring remain separate.
class AnimationStartupProcess {
public:
 static std::unique_ptr<AnimationStartupProcess> cold(
  std::shared_ptr<NativeFileCache>,std::shared_ptr<BattlePosePool>,
  const EventNativePlatform&,const char* originalImage,uint32_t gpAddress,
  uint32_t arenaAddress,uint32_t arenaBytes);
 AnimationStartupProcess(const AnimationStartupProcess&)=delete;
 AnimationStartupProcess& operator=(const AnimationStartupProcess&)=delete;
 bool advance(); // Retains the actual cold parent/child on false; never reset.
 // Actual source entry + sidecar adoption. Source name address stays live;
 // repeat the same request/resource across holds. -2 is a completed model
 // failure, empty is unavailable. No teardown/release contract is implied.
 std::optional<int32_t> enter(uint32_t name,uint32_t mode,std::shared_ptr<const BattlePoseResource>);
 uint32_t entryPending() const{return entry_.pending;}
 bool ready() const{return aliases_ && aliases_->matchesWork() && tangentMemory_ && tangentMemory_->current();}
 uint32_t pending() const{return cursor_.pending;}
 EventNativeRuntime& vm(){return *vm_;}
 const EventNativeRuntime& vm() const{return *vm_;}
 const AnimationPoolMemory* animationMemory() const{return aliases_.get();}
private:
 AnimationStartupProcess(std::shared_ptr<NativeFileCache>,std::shared_ptr<BattlePosePool>,const EventNativePlatform&,uint32_t);
 AnimationInitializeServices services();
 bool child(BattleSequenceCall&);
 bool entryChild(AnimationEntryCall&);
 bool adoptEntry();
 bool text(uint32_t,std::string&) const;
 // Reverse destruction: retire aliases, then VM, then physical file/pool owners.
 std::shared_ptr<BattlePosePool> pool_;
 std::shared_ptr<NativeFileCache> files_;
 std::unique_ptr<EventNativeRuntime> vm_;
 AnimationInitializeCursor cursor_;
 bool initialized_=false;
 // vsprintf completed before a retained _fileAlloc/Yield child.
 bool formatted_=false;
 uint32_t formatAddress_=0;
 std::string filename_;
 AnimationEntryCursor entry_;
 AnimationBufferCursor entryBuffers_;
 std::shared_ptr<const BattlePoseResource> entryResource_;
 bool entryComplete_=false;
 bool entryFormatted_=false;
 std::string entryFilename_;
 std::unique_ptr<AnimationPoolMemory> aliases_;
 std::unique_ptr<AnimationTangentMemory> tangentMemory_; // Retires before arrays and VM.
};
}
