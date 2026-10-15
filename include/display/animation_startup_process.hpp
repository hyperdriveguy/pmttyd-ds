#pragma once
#include "animation_initialize.hpp"
#include "animation_pool_memory.hpp"
#include "event_native_runtime.hpp"
#include "native_file_cache.hpp"
namespace ttyd {
// Owns a cold animation subsystem's actual VM/SDK arena/work/cache aliases.
// This is not whole-Game startup: source Entry/typed file metadata, live CPU
// output/tangent consumers and broader platform/field providers remain separate.
class AnimationStartupProcess {
public:
 static std::unique_ptr<AnimationStartupProcess> cold(
  std::shared_ptr<NativeFileCache>,std::shared_ptr<BattlePosePool>,
  const EventNativePlatform&,const char* originalImage,uint32_t gpAddress,
  uint32_t arenaAddress,uint32_t arenaBytes);
 AnimationStartupProcess(const AnimationStartupProcess&)=delete;
 AnimationStartupProcess& operator=(const AnimationStartupProcess&)=delete;
 bool advance(); // Retains the actual cold parent/child on false; never reset.
 bool ready() const{return aliases_ && aliases_->matchesWork();}
 uint32_t pending() const{return cursor_.pending;}
 EventNativeRuntime& vm(){return *vm_;}
 const EventNativeRuntime& vm() const{return *vm_;}
 const AnimationPoolMemory* animationMemory() const{return aliases_.get();}
private:
 AnimationStartupProcess(std::shared_ptr<NativeFileCache>,std::shared_ptr<BattlePosePool>,const EventNativePlatform&,uint32_t);
 AnimationInitializeServices services();
 bool child(BattleSequenceCall&);
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
 std::unique_ptr<AnimationPoolMemory> aliases_;
};
}
