#pragma once
#include "battle_actor.hpp"
#include <array>
#include <memory>
#include <optional>
namespace ttyd {
// Actual linked calls: no opaque branch/state handler. A held callee retains
// its own acknowledged effects; engaged zero is a real original return value.
struct BattleAudienceSetupPrivateCopy {
  uint32_t destination = 0;
  std::array<std::optional<uint8_t>, 48> bytes{};
};
struct BattleAudienceSetupRequest {
  uint32_t function = 0, callsite = 0;
  std::array<uint32_t, 3> arguments{};
  // Only memcpy8000519c: arguments[1] has no guest-word interpretation.
  const BattleAudienceSetupPrivateCopy *privateCopy = nullptr;
};
struct BattleAudienceSetupOutput {
  uint32_t value = 0;
  // Only pouchGetAudienceNum800d3bac returns its original f1 binary32 here.
  uint32_t singleBits = 0;
};
enum class BattleAudienceSetupNumeric {
  SubtractDoubleToSingle,
  AddSingle,
  MultiplySingle,
  FmaSingle,
  TruncateSingle,
  DivideSigned
};
struct BattleAudienceSetupNumberRequest {
  BattleAudienceSetupNumeric operation{};
  uint32_t source = 0;
  // Double subtraction operands are raw binary64; single operands raw
  // binary32; signed division operands original wrapping words.
  std::array<uint64_t, 3> operands{};
};
struct BattleAudienceSetupServices {
  BattleActorServices actors;
  std::shared_ptr<const void> lifetime;
  void *user = nullptr;
  bool (*current)(void *) = nullptr;
  std::optional<BattleAudienceSetupOutput> (*call)(
      void *, const BattleAudienceSetupRequest &) = nullptr;
  // Returns binary32 result bits or the original signed integer result word.
  std::optional<uint32_t> (*number)(
      void *, const BattleAudienceSetupNumberRequest &) = nullptr;
};
// These are complete source void parents. Source-private pointers are native
// tagged offsets, never manufactured guest SP addresses. Unknown private words
// hold when reached as an actual observable operand; reset writes no guest RAM.
#define TTYD_AUDIENCE_INIT_CURSOR(Name, Fn)                                    \
  class Name {                                                                 \
  public:                                                                      \
    Name();                                                                    \
    ~Name();                                                                   \
    Name(const Name &) = delete;                                               \
    Name &operator=(const Name &) = delete;                                    \
    bool active() const;                                                       \
    uint32_t pending() const;                                                  \
    bool reset();                                                              \
                                                                               \
  private:                                                                     \
    struct State;                                                              \
    std::unique_ptr<State> state_;                                             \
    bool invoking_ = false;                                                    \
    friend bool Fn(const BattleAudienceSetupServices &, Name &);               \
  };                                                                           \
  bool Fn(const BattleAudienceSetupServices &, Name &);
TTYD_AUDIENCE_INIT_CURSOR(BattleAudienceSettingCursor,
                          resumePackedBattleAudienceSetting)
TTYD_AUDIENCE_INIT_CURSOR(BattleAudienceInitCursor,
                          resumePackedBattleAudienceInit)
#undef TTYD_AUDIENCE_INIT_CURSOR
} // namespace ttyd
