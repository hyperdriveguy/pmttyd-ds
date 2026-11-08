#pragma once
#include "event_memory_binding.hpp"
#include "prologue_pouch.hpp"
// Shared native retained byte owner. VM facade attachment must preserve the
// arena and pointer-cell history; destroying a binding must not cold-reset them.
// canAllocate is no-effects admission, not the source allocator implementation.
namespace ttyd {
// Native guest allocation policy, not a claim about original heap addresses.
// One primary arena owns every allocation and paired free, including the nested
// battle pool. Cold construction is distinct from attaching a new VM facade.
class BattleSequenceStorage {
public:
 BattleSequenceStorage(const BattleSequenceStorage&)=delete;
 BattleSequenceStorage& operator=(const BattleSequenceStorage&)=delete;
 static std::shared_ptr<BattleSequenceStorage> cold(std::shared_ptr<ProloguePouch>,uint32_t arenaAddress,uint32_t arenaBytes,uint32_t pouchAddress);
 bool canAllocate(uint32_t root,uint32_t bytes,const EventAddressSpace&) const;
 uint32_t arenaAddress() const{return arenaAddress_;}
 uint32_t pouchAddress() const{return pouchAddress_;}
private:
 friend class BattleSequenceBinding;
 friend class BattleSequenceNestedPoolReceipt;
 bool aliases(const EventAddressSpace&)const;
 bool ownsPrimaryAllocation(uint32_t,uint32_t,const EventAddressSpace&)const;
 void unregisterOwnAliases(EventAddressSpace&,const std::vector<uint32_t>&)const;
 BattleSequenceStorage(std::shared_ptr<ProloguePouch>,uint32_t,uint32_t,uint32_t);
 std::shared_ptr<ProloguePouch> pouch_;
 uint32_t arenaAddress_,pouchAddress_;
 std::vector<uint8_t> arena_;
 std::array<uint8_t,20> globals_{}; // 8041e808..e81b primary/battle/lengths, retained.
 std::array<uint8_t,4> pouchPointer_{},battlePointer_{};
};
}
