#pragma once
#include <memory>
#include <array>
#include <optional>
#include "battle_actor.hpp"
namespace ttyd {
struct BattleMoveRequest {
 uint32_t function=0;
 std::array<uint32_t,8> integers{};
};
struct BattleMoveServices {
 BattleActorServices actors;
 void* user=nullptr;
 // Exact original child only. nullopt is unavailable; engaged0 is an actual
 // source result (including a barrier result0). Void results are ignored.
 // The same request object and operands survive every held child.
 std::optional<uint32_t> (*call)(void*,const BattleMoveRequest&)=nullptr;
};
// One original invocation. Provider/B identities are fixed, but actual memory,
// unit/event allocations and child lifetimes are caller-owned. Reset abandons
// retained metadata without undoing source effects or safely replaying them.
class BattleMoveCursor {
public:
 BattleMoveCursor();~BattleMoveCursor();
 BattleMoveCursor(const BattleMoveCursor&)=delete;
 BattleMoveCursor& operator=(const BattleMoveCursor&)=delete;
 bool active()const;uint32_t pending()const;bool reset();
private:
 struct State;std::unique_ptr<State> state_;bool invoking_=false;
 friend bool resumePackedBattleMove(uint32_t,const BattleMoveServices&,BattleMoveCursor&);
};
class BattlePhaseCursor {
public:
 BattlePhaseCursor();~BattlePhaseCursor();
 BattlePhaseCursor(const BattlePhaseCursor&)=delete;
 BattlePhaseCursor& operator=(const BattlePhaseCursor&)=delete;
 bool active()const;uint32_t pending()const;bool reset();
private:
 struct State;std::unique_ptr<State> state_;bool invoking_=false;
 friend bool resumePackedBattlePhase(uint32_t,const BattleMoveServices&,BattlePhaseCursor&);
};
// Whole source Move8011bc24/690 and Phase8011c2b4/9c. Own all source parent
// scans/transitions and immediate Move redispatches. Phase composes real Move,
// not an opaque state handler. Actual children (including Act8011b2dc) remain
// explicitly retained caller owners; no legacy handoff is acknowledged here.
bool resumePackedBattleMove(uint32_t battle,const BattleMoveServices&,BattleMoveCursor&);
bool resumePackedBattlePhase(uint32_t battle,const BattleMoveServices&,BattlePhaseCursor&);
}
