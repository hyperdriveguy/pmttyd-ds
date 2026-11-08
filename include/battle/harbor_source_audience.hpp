#pragma once
#include "battle_audience_init_continuation.hpp"
#include "battle_audience_turn_end_continuation.hpp"
#include "event_native_runtime.hpp"
#include "harbor_source_map_pool.hpp"
#include "native_file_cache.hpp"
#include <functional>
namespace ttyd {
// Native catalog admission before the actual shared cache's first bind. This
// stages unchanged original raw TPLs; it neither acquires them nor runs Init.
bool stageHarborAudienceFiles(NativeFileCache &, const char *romfsRoot);
struct HarborSourceAudienceBinding {
  EventNativeRuntime *vm = nullptr;
  HarborSourceMapPool *pool = nullptr;
  uint32_t battle = 0;
  std::shared_ptr<const void> lifetime;
  std::function<bool()> current;
  std::shared_ptr<NativeFileCache> files;
  // Actual immutable imported pack for source table/string identity. No native
  // fabricated root, guest scratch address or mutable-byte adoption.
  std::shared_ptr<const EventScriptAssets> sourceData;
  // Whole retained audio/positive population children. Missing remains held at
  // its original call. Providers and captured resource owners stay stable.
  std::function<std::optional<BattleAudienceSetupOutput>(
      const BattleAudienceSetupRequest &)>
      child;
  // Original AddAudienceNum suffix from801a176c, after actual early captures.
  std::function<std::optional<uint32_t>(const BattleAudienceAddNumInvocation &)>
      addAudience;
};
class HarborSourceAudience {
public:
  static std::unique_ptr<HarborSourceAudience>
      create(HarborSourceAudienceBinding);
  ~HarborSourceAudience();
  HarborSourceAudience(const HarborSourceAudience &) = delete;
  HarborSourceAudience &operator=(const HarborSourceAudience &) = delete;
  bool resume();
  bool complete() const;
  bool current() const;
  uint32_t pending() const;
  bool retire(); // Native metadata only; no guest End/fileFree/reset.
private:
  HarborSourceAudience();
  struct State;
  std::unique_ptr<State> state_;
  bool invoking_ = false;
};
} // namespace ttyd
