#pragma once
#include "battle_audience_init_continuation.hpp"
#include "battle_audience_noise_continuation.hpp"
#include "harbor_source_map_pool.hpp"
#include "native_battle_sound.hpp"
#include <functional>
namespace ttyd {
enum class HarborAudienceNoisePrivatePolicy {
  PreserveUnknown,
  // Separate reviewable adaptation, never source initialization evidence.
  // Only native private byte0 is initialized; all other bytes stay unknown.
  EmptyColdPrivateName
};
struct HarborSourceAudienceNoiseBinding {
  EventNativeRuntime *vm = nullptr;
  HarborSourceMapPool *pool = nullptr;
  uint32_t battle = 0;
  std::shared_ptr<const void> lifetime;
  std::function<bool()> current;
  std::shared_ptr<const EventScriptAssets> sourceData;
  std::shared_ptr<NativeBattleSound> sound;
  std::shared_ptr<PositionalSfxRegistry> registry;
  HarborAudienceNoisePrivatePolicy policy =
      HarborAudienceNoisePrivatePolicy::PreserveUnknown;
};
class HarborSourceAudienceNoise {
public:
  static std::unique_ptr<HarborSourceAudienceNoise>
      create(HarborSourceAudienceNoiseBinding);
  ~HarborSourceAudienceNoise();
  HarborSourceAudienceNoise(const HarborSourceAudienceNoise &) = delete;
  HarborSourceAudienceNoise &
  operator=(const HarborSourceAudienceNoise &) = delete;
  // Only actual cold-parent call801a8584 is admitted. Metadata is not proof
  // that gameplay reached it; the integrating source parent supplies that
  // proof.
  bool resume(const BattleAudienceSetupRequest &);
  bool complete() const;
  uint32_t pending() const;
  bool retire();

private:
  HarborSourceAudienceNoise();
  struct State;
  std::unique_ptr<State> state_;
  bool invoking_ = false;
};
} // namespace ttyd
