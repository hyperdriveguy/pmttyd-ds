#pragma once
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>
namespace ttyd {
struct EventEntry;
struct BattleUnitCoinServices {
  void *user = nullptr;
  std::shared_ptr<const void> lifetime;
  bool (*current)(void *) = nullptr;
  std::optional<uint32_t> (*word)(void *, uint32_t) = nullptr;
  std::optional<uint8_t> (*byte)(void *, uint32_t) = nullptr;
  std::optional<int32_t> (*value)(void *, EventEntry &, int32_t) = nullptr;
  std::optional<int32_t> (*translate)(void *, EventEntry &, int32_t) = nullptr;
  std::optional<int32_t> (*random)(void *, int32_t) = nullptr;
  // Atomic whole original setter: false means no write was acknowledged.
  bool (*store)(void *, EventEntry &, int32_t, int32_t) = nullptr;
  bool (*writeWord)(void *, uint32_t, uint32_t) = nullptr;
};
class BattleUnitCoinCursor {
public:
  uint32_t pending() const { return pending_; }

private:
  friend std::optional<int32_t>
  resumeBattleStoreCoinEvent(EventEntry &, const BattleUnitCoinServices &,
                             BattleUnitCoinCursor &);
  friend std::optional<int32_t>
  resumeBattleGetCoinEvent(EventEntry &, const BattleUnitCoinServices &,
                           BattleUnitCoinCursor &);
  struct Read {
    uint32_t kind, address, value;
  };
  std::vector<Read> reads_;
  BattleUnitCoinServices services_;
  const EventEntry *event_ = nullptr;
  uint32_t id_ = 0, command_ = 0, args_ = 0, pending_ = 0, mode_ = 0,
           address_ = 0, sum_ = 0;
  int32_t destination_ = 0, coins_ = 0;
  bool invoking_ = false, prepared_ = false, complete_ = false;
};
// Whole original GetCoin8010d914 + unit getter80126ca8 and registry lookup;
// fresh count/probability loads and each actual RNG result survive all holds.
// Engaged2 is original EVT completion; unavailable is not a zero coin result.
std::optional<int32_t> resumeBattleGetCoinEvent(EventEntry &,
                                                const BattleUnitCoinServices &,
                                                BattleUnitCoinCursor &);
// Whole StoreCoin8010d8d8 and BattleStoreCoin800f7c0c. Writes the actual
// Battle-information accumulator; never a pouch or victory substitute.
std::optional<int32_t>
resumeBattleStoreCoinEvent(EventEntry &, const BattleUnitCoinServices &,
                           BattleUnitCoinCursor &);

} // namespace ttyd
