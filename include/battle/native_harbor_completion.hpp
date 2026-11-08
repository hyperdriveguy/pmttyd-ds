#pragma once
#include "positional_sfx_registry.hpp"
#include "prologue_pouch.hpp"
#include <functional>
#include <memory>
#include <optional>
namespace ttyd {
class EventNativeRuntime;class HarborTutorialPacked;class BattleMenuFileCache;
struct NativeHarborCompletionBinding {
 EventNativeRuntime* vm=nullptr;HarborTutorialPacked* packed=nullptr;
 std::shared_ptr<const void> lifetime;
 std::function<bool()> current;
 std::shared_ptr<BattleMenuFileCache> menuFiles;
 int32_t result=0;
};
// Actual native services are called outside rendering, after synchronization.
// True means the operation completed. False must retain its own partial work.
struct NativeHarborCompletionServices {
 std::shared_ptr<const void> lifetime;void* user=nullptr;
 bool(*current)(void*)=nullptr;
 bool(*retirePresentation)(void*)=nullptr;
 bool(*retireMusic)(void*)=nullptr;
 PositionalSfxStopServices publicSound;
};
struct NativeHarborCompletionReceipt {
 int32_t result=0;
 ProloguePouch pouch;
 std::shared_ptr<const void> battleLifetime;
};
// Native policy over actual battle owners. Not whole original btlseqEnd or
// source sequence-return completion. Only original victory result1 is admitted.
class NativeHarborCompletion {
public:
 explicit NativeHarborCompletion(NativeHarborCompletionBinding);
 ~NativeHarborCompletion();
 NativeHarborCompletion(const NativeHarborCompletion&)=delete;
 NativeHarborCompletion& operator=(const NativeHarborCompletion&)=delete;
 bool resume(const NativeHarborCompletionServices&);
 std::optional<NativeHarborCompletionReceipt> receipt()const;
 uint32_t pending()const;
private:
 struct State;std::unique_ptr<State> state_;
 mutable bool invoking_=false;
};
}
