#pragma once
#include "effect_pool.hpp"
#include <memory>
namespace ttyd {
class NativeUiStartup;
// Source callbacks belong to a retained producer/queue owner. current is a pure
// lifetime proof; true update means the COMPLETE original effect callback ran.
struct NativeEffectUpdate {
 void* user=nullptr;std::shared_ptr<const void> lifetime;
 bool (*current)(void*)=nullptr;
 bool (*update)(void*,uint32_t callback,uint32_t effect)=nullptr;
};
struct NativeEffectRetirement {
 void* user=nullptr;std::shared_ptr<const void> lifetime;
 // Must prove constructor, callback and queued display consumers have drained.
 // AutoRelease itself supplies none of those cancellations/acknowledgments.
 bool (*quiescent)(void*)=nullptr;
};
// Borrow the actual UI/animation process VM, source game heaps and file manager.
// Cold attachment installs only absent source ranges; successful RAM/history
// stays with that VM, including after this native facade retires. No private VM,
// heap recreation, implicit primary-texture setup or per-effect destructor.
class NativeEffectProcess {
public:
 static std::shared_ptr<NativeEffectProcess> create(std::shared_ptr<NativeUiStartup>,const char* romfsRoot);
 ~NativeEffectProcess();
 NativeEffectProcess(const NativeEffectProcess&)=delete;
 NativeEffectProcess& operator=(const NativeEffectProcess&)=delete;
 bool initialize(); // Original effInit once; later calls never repeat allocations.
 bool current()const;
 bool poolReady()const;
 uint32_t pending()const;
 // One actual source-driver invocation, not a rendering frame. Owner stays
 // stationary; visit IDs strictly increase. Retries/completed duplicate visits
 // do not repeat callbacks/cache work. No update provider is needed for empty
 // records, but any reached nonnull callback holds until genuinely bound.
 bool main(const void* owner,uint64_t visit,const NativeEffectUpdate&);
 bool mainPending()const;
 bool busy()const;
 bool initializeFade(); // Original fadeInit may precede effect pool Init.
 bool beginFadeTextures();
 bool pumpFadeTextures(uint32_t byteBudget);
 bool fadeTexturesReady()const;
 bool initializeEnvironment(); // Original envInit once on the same process.
 bool beginEnvironmentTextures();
 bool pumpEnvironmentTextures(uint32_t byteBudget);
 bool environmentTexturesReady()const;
 bool beginTextures(); // Original primary + mandatory N64 job admission.
 bool pumpTextures(uint32_t byteBudget); // Independent actual DVD callbacks.
 bool texturesReady()const;
 bool pumpFiles(uint32_t byteBudget); // Native cooperative IO policy, not source timing.
 bool autoRelease(uint32_t exactContext,const NativeEffectRetirement&);
 // Only available after Init. Borrowed callbacks outlive all held constructors;
 // callers retain this process and actual source actor/camera/audio owners.
 EffectPoolServices constructionServices();
 std::shared_ptr<NativeUiStartup> ui()const;
 // Stop new admission only. No source free/reset/cancel; VM RAM remains live.
 bool retire();
private:
 NativeEffectProcess();struct State;std::unique_ptr<State> state_;bool invoking_=false;
 EffectPoolServices services();
};
}
