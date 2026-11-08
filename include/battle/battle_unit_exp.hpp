#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>
namespace ttyd {
struct EventEntry;
struct BattleUnitExpServices {
  void *user = nullptr;
  std::shared_ptr<const void> lifetime;
  bool (*current)(void *) = nullptr;
  std::optional<uint32_t> (*word)(void *, uint32_t) = nullptr;
  std::optional<uint8_t> (*byte)(void *, uint32_t) = nullptr;
  std::optional<uint16_t> (*half)(void *, uint32_t) = nullptr;
  std::optional<uint64_t> (*doubleBits)(void *, uint32_t) = nullptr;
  std::optional<int32_t> (*value)(void *, EventEntry &, int32_t) = nullptr;
  std::optional<int32_t> (*translate)(void *, EventEntry &, int32_t) = nullptr;
  std::optional<float> (*subtractDoubleFloat)(void *, double, double) = nullptr;
  std::optional<float> (*multiply)(void *, float, float) = nullptr;
  std::optional<int32_t> (*truncate)(void *, float) = nullptr;
  bool (*store)(void *, EventEntry &, int32_t, int32_t) = nullptr;
  bool (*writeWord)(void *, uint32_t, uint32_t) = nullptr;
};
class BattleUnitExpCursor {
public:
  uint32_t pending() const { return pending_; }

private:
  friend std::optional<int32_t>
  resumeBattleGetExpEvent(EventEntry &, const BattleUnitExpServices &,
                          BattleUnitExpCursor &);
  friend std::optional<int32_t>
  resumeBattleStoreExpEvent(EventEntry &, const BattleUnitExpServices &,
                            BattleUnitExpCursor &);
  struct Operation {
    uint32_t kind;
    std::array<uint64_t, 2> arguments;
    uint64_t result;
  };
  std::vector<Operation> operations_;
  BattleUnitExpServices services_;
  const EventEntry *event_ = nullptr;
  uint32_t id_ = 0, command_ = 0, args_ = 0, pending_ = 0, mode_ = 0,
           address_ = 0, result_ = 0;
  int32_t destination_ = 0;
  bool invoking_ = false, prepared_ = false, complete_ = false;
};
// Whole source GetExp8010d9bc +BtlUnit_GetExp80126d20. No null-unit fake0.
std::optional<int32_t> resumeBattleGetExpEvent(EventEntry &,
                                               const BattleUnitExpServices &,
                                               BattleUnitExpCursor &);
// Whole StoreExp8010d980 +BattleStoreExp800f7bfc:actual B+f00 accumulator.
std::optional<int32_t> resumeBattleStoreExpEvent(EventEntry &,
                                                 const BattleUnitExpServices &,
                                                 BattleUnitExpCursor &);
} // namespace ttyd
