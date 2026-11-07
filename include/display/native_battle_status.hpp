#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include <optional>
namespace ttyd {
struct EventNativeRuntime;
struct NativeBattleStatusSnapshot {
  std::array<int16_t, 11> values{};
  // Values are the complete original statusGetValue output, indexed offset/2.
  std::optional<uint16_t> partnerPortrait;
  struct GaugeIcon {
    uint16_t icon = 0;
    int16_t x = 0, y = 0;
  };
  std::array<GaugeIcon, 9> gauge{};
  uint8_t gaugeCount = 0;
};
// The VM is borrowed from its real battle owner. That owner must destroy this
// reader before its VM; lifetime/current pin its actual allocation incarnation.
// Unknown source reads retain this invocation; no partial snapshot is
// published.
class NativeBattleStatusReader {
public:
  NativeBattleStatusReader(EventNativeRuntime &, std::shared_ptr<const void>,
                           void *, bool (*current)(void *));
  ~NativeBattleStatusReader();
  NativeBattleStatusReader(const NativeBattleStatusReader &) = delete;
  NativeBattleStatusReader &
  operator=(const NativeBattleStatusReader &) = delete;
  std::optional<NativeBattleStatusSnapshot> snapshot();
  uint32_t pending() const;

private:
  struct State;
  std::unique_ptr<State> state_;
};
} // namespace ttyd
