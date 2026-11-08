#pragma once
#include "field_mario_battle_transfer.hpp"
#include "field_party.hpp"
#include "field_movement.hpp"
#include "mario_presentation.hpp"
#include "native_facade_registration.hpp"
#include "battle_sequence_entry.hpp"
namespace ttyd {
struct NativeFieldMarioBattleTransferBinding {
 std::shared_ptr<const void> lifetime;
 NativeFacadeRegistration::Proof host,facade;
 void* user=nullptr;
 // Proofs are checked before this callback or any borrowed typed owner.
 bool(*current)(void*)=nullptr;
 FieldMarioBattleState* mario=nullptr;
 FieldPartyRegistry* parties=nullptr;
 MarioPresentation* presentation=nullptr;
 FieldMovement* movement=nullptr;
 std::optional<uint16_t>(*motion)(void*)=nullptr;
 // Actual field engine-space binary32 words, not native render coordinates.
 std::optional<uint32_t>(*position)(void*,unsigned axis)=nullptr;
 bool(*storePosition)(void*,unsigned axis,uint32_t)=nullptr;
 // Only reached original special action/ability/appearance children. Null
 // means unavailable. It must retain its own partial work and private output.
 bool(*child)(void*,FieldMarioBattleTransferRequest&)=nullptr;
};
// Finite native source-field view, not a fabricated complete Mario allocation.
// The actual bound owners remain stationary; copied/restored facades must use
// distinct registration proofs. Saved XYZ become known only after real stores.
class NativeFieldMarioBattleTransfer {
public:
 static std::unique_ptr<NativeFieldMarioBattleTransfer> create(NativeFieldMarioBattleTransferBinding);
 bool call(BattleSequenceCall&);
 uint32_t pending()const;
 bool current();
private:
 explicit NativeFieldMarioBattleTransfer(NativeFieldMarioBattleTransferBinding);
 NativeFieldMarioBattleTransferBinding binding_;
 FieldMarioBattleTransfer prepare_{MarioFieldBattlePrepare},post_{MarioFieldBattlePost};
 std::array<std::optional<uint32_t>,3> saved_{};
 bool invoking_=false,checking_=false,stale_=false;
 FieldMarioBattleTransferServices services();
 std::optional<uint32_t> word(uint32_t);
 std::optional<uint16_t> half(uint32_t);
 std::optional<uint8_t> byte(uint32_t);
 bool store(uint32_t,uint32_t);
 bool child(FieldMarioBattleTransferRequest&);
 std::optional<int32_t> partyId();
 FieldPartyActor* actor(uint32_t,unsigned&offset);
};
}
