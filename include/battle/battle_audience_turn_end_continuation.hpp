#pragma once
#include <memory>
#include "battle_audience_target_continuation.hpp"
namespace ttyd {
// Actual AddAudienceNum entry captures and original operands after its three
// early-return gates. A nonempty provider owns the remaining source body from
//801a176c, including all allocation/removal/RNG effects, without recapturing B/A.
struct BattleAudienceAddNumInvocation {
 uint32_t battle=0,audience=0,pouch=0;
 int32_t amount=0;
 // Source pouch + 0x8c is a halfword capacity, not a truncated byte.
 uint16_t capacity=0;
};
// nullopt means hold for numeric/RNG children; engaged zero is a real result.
// addAudience false retains its own partial suffix effects and exact invocation.
// Keep every provider/user stable; resetting does not roll back prefix writes.
struct BattleAudienceTurnEndServices {
 BattleActorServices actors;
 void* user=nullptr;
 std::optional<int32_t> (*divide)(void*,int32_t numerator,int32_t denominator)=nullptr;
 std::optional<int32_t> (*truncate)(void*,float)=nullptr;
 std::optional<int32_t> (*random)(void*,int32_t bound)=nullptr;
 bool (*addAudience)(void*,const BattleAudienceAddNumInvocation&)=nullptr;
};
class BattleAudienceTurnEndCursor {
public:
 BattleAudienceTurnEndCursor();~BattleAudienceTurnEndCursor();
 BattleAudienceTurnEndCursor(const BattleAudienceTurnEndCursor&)=delete;
 BattleAudienceTurnEndCursor& operator=(const BattleAudienceTurnEndCursor&)=delete;
 bool active() const;uint32_t pending() const;bool reset();
private:
 struct State;std::unique_ptr<State> state_;bool invoking_=false;
 friend bool resumePackedBattleAudienceTurnEnd(const BattleAudienceTurnEndServices&,BattleAudienceTurnEndCursor&);
};
// Whole ordinary no-argument source8019f298/168. Captures original globals and
// pointers in source order; composes retained Target and exact phase-list /
// NumToTarget / GetExist leaves. No empty crowd, RNG, camera or numeric result
// is manufactured. Source AddAudienceNum early returns need no suffix provider.
bool resumePackedBattleAudienceTurnEnd(const BattleAudienceTurnEndServices&,BattleAudienceTurnEndCursor&);
}
