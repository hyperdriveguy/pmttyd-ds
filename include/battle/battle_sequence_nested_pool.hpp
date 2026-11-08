#pragma once
#include <memory>
#include <cstdint>
namespace ttyd {
class BattleSequenceBinding;
class EventNativeRuntime;
class HarborSourceMapPool;
// Opaque receipt for a completed actual N_battleMapAlloc80030078 branch.
// Source End/free remains unbound. Retirement removes admission only; it does
// not clear/remap/reset/free any guest or source process state.
class BattleSequenceNestedPoolReceipt {
public:
 ~BattleSequenceNestedPoolReceipt();
 BattleSequenceNestedPoolReceipt(const BattleSequenceNestedPoolReceipt&)=delete;
 BattleSequenceNestedPoolReceipt&operator=(const BattleSequenceNestedPoolReceipt&)=delete;
 bool current()const;
 bool accepts(const EventNativeRuntime&)const;
 uint32_t root()const;
 void retire();
private:
 struct State;std::unique_ptr<State>state_;
 BattleSequenceNestedPoolReceipt();
 static std::shared_ptr<BattleSequenceNestedPoolReceipt> prepare(BattleSequenceBinding&);
 // Called only after the actual source allocator returns. A missing receipt
 // cannot turn that completed void source call into a replayable wait.
 bool complete();
 bool claim(const EventNativeRuntime&)const;
 friend class BattleSequenceBinding;
 friend class HarborSourceMapPool;
};
}
