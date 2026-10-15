#pragma once
#include <memory>
#include <array>
#include <optional>
#include "battle_actor.hpp"
namespace ttyd {
struct BattleCommandInputContinuationRequest {
 uint32_t function=0,callsite=0;
 std::array<uint32_t,3> arguments{};
 uint8_t count=0;
 // Only GetCursorPtr receives the same native SP08 output owner. Source SP08
 // starts unwritten. No fabricated guest address/zero is supplied as argument3.
 std::optional<uint32_t>* privateCursorOutput=nullptr;
};
struct BattleCommandInputContinuationServices {
 BattleActorServices actors;
 void* user=nullptr;
 // Exact original child; engaged0 is a completed scalar0, nullopt holds.
 // Child owns any partial effects. Output/request borrows end with invocation.
 std::optional<uint32_t> (*call)(void*,const BattleCommandInputContinuationRequest&)=nullptr;
};
// One source simulation invocation, not a modal UI loop until a choice.
// Pin B and callback identities; caller pins all actual allocations/context.
// Reset/reentry cannot replay acknowledged child effects safely.
class BattleCommandInputContinuationCursor {
public:
 BattleCommandInputContinuationCursor();~BattleCommandInputContinuationCursor();
 BattleCommandInputContinuationCursor(const BattleCommandInputContinuationCursor&)=delete;
 BattleCommandInputContinuationCursor& operator=(const BattleCommandInputContinuationCursor&)=delete;
 bool active()const;uint32_t pending()const;uint32_t callsite()const;bool reset();
private:
 struct State;std::unique_ptr<State> state_;bool invoking_=false;
 friend std::optional<int32_t> resumePackedBattleCommandInput(uint32_t,const BattleCommandInputContinuationServices&,BattleCommandInputContinuationCursor&);
};
// Whole original801229d8/838 including state11. Engaged0 completes one source
// simulation invocation, not a held until-confirmation UI. Caller owns cadence.
std::optional<int32_t> resumePackedBattleCommandInput(uint32_t battle,const BattleCommandInputContinuationServices&,BattleCommandInputContinuationCursor&);
}
