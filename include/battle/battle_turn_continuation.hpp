#pragma once
#include "battle_actor.hpp"
#include <array>
#include <memory>
#include <optional>

namespace ttyd {
// Original child calls only. Values that were source stack outputs are native
// private outputs, never invented guest addresses. An engaged zero is a valid
// source return (including failed event/effect allocations).
struct BattleTurnRequest {
    // Original guest callee token. Positional arrays carry the reached
    // child's operands, not a generic EVT argument list or guest stack image.
    // See docs/code/battle-turn.md for dispatch/output domains.
    uint32_t function=0;
    std::array<uint32_t,8> integers{};
    std::array<float,4> reals{};
};
struct BattleTurnOutput {
    // Interpretation belongs to the callee: integer/handle/guest EventEntry
    // address, with position or status outputs in typed native fields below.
    uint32_t value=0;
    std::array<float,3> position{};
    uint8_t duration=0,strength=0;
};
struct BattleTurnServices {
    BattleActorServices actors;
    void* user=nullptr;
    // A held child owns its partial work and must not replay acknowledged work.
    std::optional<BattleTurnOutput> (*call)(void*,const BattleTurnRequest&)=nullptr;
};
// Pin one battle pointer and service identity until completion. pending() is
// a guest function diagnostic, not a frame counter. reset refuses reentry and
// drops native continuation only; earlier guest/provider effects remain.
class BattleTurnCursor {
public:
    BattleTurnCursor();
    ~BattleTurnCursor();
    BattleTurnCursor(const BattleTurnCursor&)=delete;
    BattleTurnCursor& operator=(const BattleTurnCursor&)=delete;
    bool active()const;
    uint32_t pending()const;
    bool reset();
private:
    struct State;
    std::unique_ptr<State> state_;
    bool invoking_=false;
    friend bool resumePackedBattleTurn(uint32_t,const BattleTurnServices&,BattleTurnCursor&);
};
// Complete original btlseqTurn8011c510 parent, all78 dispatch entries. One
// invocation may immediately traverse several empty phases. The simulation
// owner supplies cadence; rendering and a held child never start a fresh call.
bool resumePackedBattleTurn(uint32_t battle,const BattleTurnServices&,BattleTurnCursor&);
}
