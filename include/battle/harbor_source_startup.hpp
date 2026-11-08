#pragma once
#include "battle_cold_init_continuation.hpp"
#include "battle_menu_resources.hpp"
#include "battle_unit_setup_continuation.hpp"
#include "event_native_runtime.hpp"
#include "harbor_source_map_pool.hpp"
#include "harbor_source_audience.hpp"
#include "harbor_source_audience_noise.hpp"
#include "harbor_source_unit_construction.hpp"
#include <functional>

namespace ttyd {
struct HarborSourceStartupBinding {
    EventNativeRuntime* vm = nullptr;
    HarborSourceMapPool* pool = nullptr;
    BattleMenuResourceServices menu;
    std::shared_ptr<const void> lifetime;
    std::function<bool()> current;
    std::shared_ptr<NativeFieldAnimation> animation;
    std::shared_ptr<const HarborTutorialPoseResources> poseResources;
    // Actual stage/audience/break-slot owners retain their own partial work.
    std::function<std::optional<uint32_t>(const BattleColdInitRequest&)> coldChild;
    // Other actual equipment providers. Returning nullopt
    // preserves this exact request; no legacy actor registration is substituted.
    std::function<std::optional<BattleUnitSetupOutput>(const BattleUnitSetupRequest&)> unitChild;
    // Already-staged actual shared file owner/source pack. Missing frontend
    // owners remain held by the retained source audience parent.
    std::shared_ptr<NativeFileCache> audienceFiles;
    std::shared_ptr<const EventScriptAssets> audienceSourceData;
    std::function<std::optional<BattleAudienceSetupOutput>(const BattleAudienceSetupRequest&)> audienceChild;
    std::function<std::optional<uint32_t>(const BattleAudienceAddNumInvocation&)> audienceAdd;
    std::shared_ptr<NativeBattleSound> audienceSound;
    std::shared_ptr<PositionalSfxRegistry> audienceRegistry;
    // Explicit separate native policy, default preserves original unknowns.
    HarborAudienceNoisePrivatePolicy audienceNoisePolicy = HarborAudienceNoisePrivatePolicy::PreserveUnknown;
};
class HarborSourceStartup {
public:
    static std::unique_ptr<HarborSourceStartup> create(HarborSourceStartupBinding);
    ~HarborSourceStartup();
    HarborSourceStartup(const HarborSourceStartup&) = delete;
    HarborSourceStartup& operator=(const HarborSourceStartup&) = delete;
    bool resumeCold(uint32_t actualFieldInformation);
    bool resumeUnitSetup(uint32_t actualBattle);
    bool coldComplete() const;
    bool unitSetupComplete() const;
    uint32_t battleAddress() const;
    uint32_t pending() const;
    bool current() const;
private:
    HarborSourceStartup();
    struct State;
    std::unique_ptr<State> state_;
    bool invoking_ = false;
};
}
