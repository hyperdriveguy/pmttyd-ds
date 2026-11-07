#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include <optional>
namespace ttyd {
struct SourceStatusValueCall {
  uint32_t function = 0, callsite = 0;
  std::array<uint32_t, 1> arguments{};
  uint8_t count = 0;
};
// Logical output offsets in the original argument record. A provider owns real
// guest bytes or the actual parent's private cells; no invented stack address.
struct SourceStatusValueOutput {
  void *user = nullptr;
  std::shared_ptr<const void> lifetime;
  std::optional<uint16_t> (*half)(void *, uint32_t offset) = nullptr;
  // Atomic one-STH admission: false has no effect, true performs exactly once.
  bool (*storeHalf)(void *, uint32_t offset, uint16_t) = nullptr;
};
struct SourceStatusValueServices {
  void *user = nullptr;
  std::shared_ptr<const void> lifetime;
  bool (*current)(void *) = nullptr;
  std::optional<uint32_t> (*word)(void *, uint32_t) = nullptr;
  std::optional<uint16_t> (*half)(void *, uint32_t) = nullptr;
  std::optional<uint8_t> (*byte)(void *, uint32_t) = nullptr;
  // Whole original child, including real zero/null returns. Unavailable holds.
  std::optional<uint32_t> (*call)(void *,
                                  const SourceStatusValueCall &) = nullptr;
};
class SourceStatusValuesCursor {
public:
  SourceStatusValuesCursor();
  ~SourceStatusValuesCursor();
  SourceStatusValuesCursor(const SourceStatusValuesCursor &) = delete;
  SourceStatusValuesCursor &
  operator=(const SourceStatusValuesCursor &) = delete;
  bool active() const;
  bool stale() const;
  uint32_t pending() const;
  bool reset();

private:
  struct State;
  std::unique_ptr<State> state_;
  bool invoking_ = false;
  friend bool resumeSourceStatusValues(const SourceStatusValueOutput &,
                                       const SourceStatusValueServices &,
                                       SourceStatusValuesCursor &);
};
// Whole original statusGetValue8013c9ac/178. Partial output writes survive
// holds; publish a HUD snapshot only after this complete invocation returns.
bool resumeSourceStatusValues(const SourceStatusValueOutput &,
                              const SourceStatusValueServices &,
                              SourceStatusValuesCursor &);
} // namespace ttyd
