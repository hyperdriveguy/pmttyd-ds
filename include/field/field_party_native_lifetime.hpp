#pragma once
#include "field_party.hpp"
#include "field_party_lifetime.hpp"
#include "event_script_assets.hpp"
namespace ttyd {
// Native actor/table/Mario views over the retained source controllers. The
// caller keeps registry, Mario and this adapter stationary through completion.
// Encoded actor tokens are native address policy, never original heap addresses.
class NativeFieldPartyLifetime {
public:
 // Base resource admits exact original kind1/8/11/13 records. A live removal
 // additionally requires actual native animation/pool/release owners; negative/
 // null removals can complete without inventing legacy source fileFree.
 static std::unique_ptr<NativeFieldPartyLifetime> create(const char* root,FieldPartyRegistry&,FieldMarioBattleState&);
 ~NativeFieldPartyLifetime()=default;
 NativeFieldPartyLifetime(const NativeFieldPartyLifetime&)=delete;
 NativeFieldPartyLifetime& operator=(const NativeFieldPartyLifetime&)=delete;
 // Completion includes actual source pose/file release and actor retirement.
 // False can follow public table clearing; keep this adapter until it finishes.
 bool kill(int32_t index,bool forget=false);
 bool removeAll(); // Original marioPartyKill80141198, including fresh queries.
 // Bind actual EventEntry+18/args reads from this same live VM. Typed Mario and
 // party views remain local; no fake EventEntry or evaluated slot is supplied.
 bool event(uint32_t entry,bool clear,EventAddressSpace&,int32_t& result);
 bool active() const{return removal_.active || outer_.active || event_.active;}
 uint32_t pending() const;
 // Captured actor remains backed after table publication and before memFree.
 bool retiring() const{return bool(retiring_);}
 // Native token policy:two stride200 slots,each backed188-byte source record.
 // Tokens stay readable via retiring_ after public table clear,until memFree.
 static constexpr uint32_t ActorBase=0x85800000,ActorStride=0x200;
private:
 NativeFieldPartyLifetime(FieldPartyRegistry& p,FieldMarioBattleState& m):parties_(p),mario_(m){}
 FieldPartyRegistry& parties_;FieldMarioBattleState& mario_;
 std::shared_ptr<NativeFieldAnimation> native_;
 std::shared_ptr<BattlePosePool> pool_;
 // Expiring lease avoids touching a borrowed registry during whole-context
 // teardown (Game destroys some field members before its battle wrapper).
 std::shared_ptr<int> removalLease_=std::make_shared<int>(0);
 EventScriptAssets definitions_;
 FieldPartyRemovalCursor removal_;
 FieldMarioPartyRemovalCursor outer_;
 FieldPartyRemovalEventCursor event_;
 EventAddressSpace* eventMemory_=nullptr;
 std::unique_ptr<FieldPartyActor> retiring_;
 FieldPartyActor* captured_=nullptr;
 uint32_t capturedAddress_=0;int32_t releasePose_=-1;bool releaseCaptured_=false;
 BattleSequenceEntryServices services();
 FieldPartyActor* actor(uint32_t address,unsigned bytes,unsigned& offset);
 std::optional<uint32_t> word(uint32_t);
 std::optional<uint8_t> byte(uint32_t);
 bool store(uint32_t,uint32_t);
 bool storeByte(uint32_t,uint8_t);
 bool call(BattleSequenceCall&);
 bool nativeOwner() const;
 bool killImpl(int32_t,bool);
};
}
