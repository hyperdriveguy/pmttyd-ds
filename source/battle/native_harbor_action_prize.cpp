#include "native_harbor_action_prize.hpp"
#include "event_native_runtime.hpp"
#include "battle_action_services.hpp"
#include "battle_event_callbacks.hpp"
namespace ttyd {
std::optional<int32_t> nativeHarborActionPrize(EventNativeRuntime& vm,EventEntry& event){
 constexpr uint32_t BattleGlobal=0x8041eb90,Registry=0x20,LastHitCode=0x278,PrizeByte=0x1cbd;
 uint32_t entry=0;if(!vm.entryAddresses.addressOf(&event,entry)||!entry)return {};
 uint32_t captured=0;if(!vm.memory.readWord(BattleGlobal,captured)||!captured)return {};
 uint32_t rawActor=0;if(event.argumentCount<3||!vm.memory.readWord(event.arguments,rawActor))return {};
 const auto values=vm.interpreterServices().operands;
 const auto actor=getEventValue(event,int32_t(rawActor),values);
 uint32_t unit=0;
 if(actor!=-1&&actor!=-5){
  const auto id=translateBattleActor(&event,actor,nativeBattleActorServices(vm));
  uint32_t current=0;if(!vm.memory.readWord(BattleGlobal,current))return {};
  if(id>=0&&id<64&&!vm.memory.readWord(current+Registry+4u*uint32_t(id),unit))return {};
 }
 uint32_t rawInput=0;if(!vm.memory.readWord(event.arguments+4,rawInput))return {};
 const auto input=getEventValue(event,int32_t(rawInput),values);
 uint32_t destination=0;if(!vm.memory.readWord(event.arguments+8,destination))return {};
 ActionPrizeActor view;if(unit&&!vm.memory.readWord(unit+LastHitCode,view.lastHitCode))return {};
 uint8_t grade=0;
 // Source low-code30 returns before reading the captured work's byte.
 if(!(unit&&(view.lastHitCode&255u)==30u)&&!vm.memory.readByte(captured+PrizeByte,grade))return {};
 ActionPrizeServices services;services.user=&grade;
 services.prizeByte=[](void*p,void*){return *static_cast<uint8_t*>(p);};
 const auto result=actionPrizeLevel(&captured,unit?&view:nullptr,input,services);
 setEventValue(event,int32_t(destination),result,values);return 2;
}
}
