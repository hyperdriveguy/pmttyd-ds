#pragma once
#include <array>
#include <cstdint>
#include <optional>
namespace ttyd {
struct FieldActorTalkState;
struct TroubleQuestServices;
// Native finite knowledge store for Trouble Center GSF5374..5421 and
// GSW1469..1474. Four legacy named flags remain canonical in talk state.
struct TroubleQuestExtraState {
 static constexpr uint64_t Mask=((uint64_t(1)<<48)-1) &
  ~((uint64_t(1)<<2) | (uint64_t(7)<<31));
 uint64_t known=0,values=0;
 std::array<uint8_t,6> bytes{};
 uint8_t byteKnown=0;
 bool present() const{return known || byteKnown;}
 bool valid() const;
};
std::optional<bool> troubleQuestFlag(const FieldActorTalkState&,int32_t flag);
bool setTroubleQuestFlag(FieldActorTalkState&,int32_t flag,bool value);
std::optional<uint8_t> troubleQuestByte(const FieldActorTalkState&,int32_t byte);
bool setTroubleQuestByte(FieldActorTalkState&,int32_t byte,int32_t value);
// Explicit fresh swInit caller only; never infer cleared banks from old saves.
void initializeTroubleQuestState(FieldActorTalkState&);
// Original reset callback writers; item/cancellation services are supplied
// separately by the actual pouch owner.
TroubleQuestServices troubleQuestSwitchServices(FieldActorTalkState&);
}
