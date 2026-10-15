#pragma once
#include "battle_main.hpp"
#include <memory>
// Native metadata retains a single source invocation; it does not own the
// children's partial work or simulation cadence. Fixed memory/service/lifecycle
// bindings and nonreentry protect pending indices without guest-state rollback.
namespace ttyd {
// Native continuation metadata only. Providers and actual guest memory must
// outlive a held call. A false provider retains its own partial child work.
// Completed calls start afresh on the next invocation; the simulation owner
// supplies the once-per-pass cadence, not rendering or a wall-clock counter.
class BattleMainCursor {
public:
    BattleMainCursor();
    ~BattleMainCursor();
    BattleMainCursor(const BattleMainCursor&)=delete;
    BattleMainCursor& operator=(const BattleMainCursor&)=delete;
    bool active() const;
    uint32_t pending() const;
    // Reset is refused while a provider is executing, including reentry.
    bool reset();
private:
    struct State;
    std::unique_ptr<State> state_;
    bool invoking_=false;
    friend std::optional<int32_t> resumePackedBattleMain(const BattleMainServices&,BattleMainCursor&);
    friend bool resumePackedBattleMainWrapper(const BattleMainServices&,BattleMainCursor&);
    friend struct BattleMainRunner;
};
std::optional<int32_t> resumePackedBattleMain(const BattleMainServices&,BattleMainCursor&);
// Null OUTER owner completes without upper work. Otherwise upper independently
// captures B and runs all eleven children before its explicit result and outer
// end-bit publication. Different retained outer/inner owners are not flattened.
bool resumePackedBattleMainWrapper(const BattleMainServices&,BattleMainCursor&);

struct BattleSequenceDispatchRequest {
    uint32_t function=0,battle=0;
    int32_t mode=0; // Used only by BattleCheckAllPinchStatus.
};
struct BattleSequenceDispatchServices {
    BattleActorServices actors;
    void* user=nullptr;
    // Native admission of a whole retained child, not its original r3 scalar.
    bool (*request)(void*,const BattleSequenceDispatchRequest&)=nullptr;
};
class BattleSequenceDispatchCursor {
public:
    BattleSequenceDispatchCursor();
    ~BattleSequenceDispatchCursor();
    BattleSequenceDispatchCursor(const BattleSequenceDispatchCursor&)=delete;
    BattleSequenceDispatchCursor& operator=(const BattleSequenceDispatchCursor&)=delete;
    bool active() const;
    uint32_t pending() const;
    bool reset();
private:
    struct State;
    std::unique_ptr<State> state_;
    bool invoking_=false;
    friend bool resumePackedBattleSequenceManager(const BattleSequenceDispatchServices&,BattleSequenceDispatchCursor&);
};
// Whole8011f1ec: fresh global B and one bank capture, exact route order. A
// child's guest bank/global changes cannot select another route during a hold.
bool resumePackedBattleSequenceManager(const BattleSequenceDispatchServices&,BattleSequenceDispatchCursor&);
}
