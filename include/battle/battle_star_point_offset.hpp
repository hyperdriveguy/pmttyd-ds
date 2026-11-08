#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>
namespace ttyd {
struct EventEntry;
struct BattleStarPointOffsetServices {
  void *user = nullptr;
  std::shared_ptr<const void> lifetime;
  bool (*current)(void *) = nullptr;
  std::optional<uint32_t> (*word)(void *, uint32_t) = nullptr;
  std::optional<uint64_t> (*doubleBits)(void *, uint32_t) = nullptr;
  std::optional<int32_t> (*value)(void *, EventEntry &, int32_t) = nullptr;
  std::optional<int32_t> (*translate)(void *, EventEntry &, int32_t) = nullptr;
  std::optional<float> (*subtractDoubleFloat)(void *, double, double) = nullptr;
  std::optional<float> (*add)(void *, float, float) = nullptr;
  std::optional<int32_t> (*truncate)(void *, float) = nullptr;
  bool (*store)(void *, EventEntry &, int32_t, int32_t) = nullptr;
};
class BattleStarPointOffsetCursor {
public:
  uint32_t pending() const { return pending_; }

private:
  friend std::optional<int32_t>
  resumeBattleStarPointOffset(EventEntry &,
                              const BattleStarPointOffsetServices &,
                              BattleStarPointOffsetCursor &);
  struct Operation {
    uint32_t kind;
    std::array<uint64_t, 2> arguments;
    uint64_t result;
  };
  std::vector<Operation> operations_;
  BattleStarPointOffsetServices services_;
  const EventEntry *event_ = nullptr;
  uint32_t id_ = 0, command_ = 0, args_ = 0, pending_ = 0;
  bool invoking_ = false, complete_ = false;
};
// Whole original _add_star_point_disp_offset80113f88/174, including its three
// ordered setters. Typed private conversion bits never become guest addresses.
std::optional<int32_t>
resumeBattleStarPointOffset(EventEntry &, const BattleStarPointOffsetServices &,
                            BattleStarPointOffsetCursor &);
} // namespace ttyd
