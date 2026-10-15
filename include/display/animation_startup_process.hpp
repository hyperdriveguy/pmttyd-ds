#pragma once
#include "animation_initialize.hpp"
#include "animation_pool_memory.hpp"
#include "event_native_runtime.hpp"
#include "native_file_cache.hpp"
#include "animation_pose_entry.hpp"
#include "animation_buffer_refresh.hpp"
#include "animation_tangent_memory.hpp"
#include "animation_scratch_memory.hpp"
#include "animation_native_main.hpp"
#include "animation_mesh_publication.hpp"
#include "animation_native_selection.hpp"
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
 ~AnimationStartupProcess();
 AnimationStartupProcess(const AnimationStartupProcess&)=delete;
 AnimationStartupProcess& operator=(const AnimationStartupProcess&)=delete;
 bool advance(); // Retains the actual cold parent/child on false; never reset.
 // Actual source entry + sidecar adoption. Source name address stays live;
 // repeat the same request/resource across holds. -2 is a completed model
 // failure, empty is unavailable. No teardown/release contract is implied.
 std::optional<int32_t> enter(uint32_t name,uint32_t mode,std::shared_ptr<const BattlePoseResource>);
 uint32_t entryPending() const{return entry_.pending;}
 // Configure once while quiescent. current(pass) must prove the same immutable
 // producer/camera/file pass; no camera is required for null/flag8-clear exits.
 // privateCapture additionally promises current is independent of all private
 // publication candidate/cursor writes. Raw/disjoint admission remains dynamic.
 bool configureMain(std::shared_ptr<const CameraManager>,AnimationFacingAddressPolicy,std::function<bool(uint64_t)> current,bool privateCapture=false);
 bool main(uint32_t handle,uint64_t pass,unsigned visits=64);
 bool reservePresentation(uint32_t handle,uint64_t pass);
 bool present(uint32_t handle,uint64_t pass,unsigned budget=8192);
 bool presentationPending() const{return presentation_ && presentation_->pending();}
 bool selectionPending() const{return selection_ && selection_->pending();}
 uint32_t selectionPhase() const{return selection_?selection_->phase():0;}
 // Exclusive source selector; resume identical inputs after false. May set up
 // its own reserved phase0 presentation, but cannot overlap source Main.
 bool setAnimation(uint32_t handle,uint32_t selector,uint32_t force,unsigned visits=64);
 bool producerPending() const{return mainPending() || presentationPending() || selectionPending();}
 const AnimationMeshPublication* presentationOwner() const{return presentation_.get();}
 bool mainPending() const{return main_ && main_->pending();}
 uint64_t mainConfiguration() const{return mainConfiguration_;}
 const AnimationNativeMain* mainOwner() const{return main_.get();}
 AnimationQuantizationState& quantization(){return quantization_;}
 const AnimationQuantizationState& quantization() const{return quantization_;}
 bool ready() const{return aliases_ && aliases_->matchesWork() && tangentMemory_ && tangentMemory_->current() && scratch_ && scratch_->current();}
 uint32_t pending() const{return cursor_.pending;}
 EventNativeRuntime& vm(){return *vm_;}
 const EventNativeRuntime& vm() const{return *vm_;}
 const AnimationPoolMemory* animationMemory() const{return aliases_.get();}
private:
 friend class AnimationNativeMain;
 friend class AnimationNativeSelection;
 friend class AnimationMeshPublication;
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
 AnimationQuantizationState quantization_;
 std::shared_ptr<AnimationScratchMemory> scratch_; // Retire before arrays/VM.
 uint64_t mainConfiguration_=0;
 std::unique_ptr<AnimationNativeSelection> selection_;
 std::unique_ptr<AnimationMeshPublication> presentation_;
 std::unique_ptr<AnimationNativeMain> main_; // Retire frames before scratch/VM.
};
}
