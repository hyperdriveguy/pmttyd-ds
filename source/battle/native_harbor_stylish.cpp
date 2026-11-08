#include "native_harbor_stylish.hpp"
#include "event_native_runtime.hpp"
#include "battle_event_callbacks.hpp"
#include <array>
namespace ttyd {
namespace {
constexpr uint32_t BattleGlobal=0x8041eb90,Registry=0x20;
constexpr uint32_t Counter=0x271c,Lower=0x2720,Upper=0x2724,End=0x2728,Actor=0x272c,Result=0x2730,Scan=0x2734;
constexpr uint32_t UnitFlags=0x27c,Automatic=0x307,DebugFlags=0x163f4;
constexpr uint32_t GeneralPad=0xf2c,TriggerHistory=0x18,HistoryEntries=30,PhysicalA=0x100;
constexpr int32_t NoOutput=-250000000;
constexpr uint32_t ActorSuppression=0x10,StylishDebugDisplay=4;
constexpr int32_t NoTry=1,Success=2,TooEarly=-1,TooLate=-2;
int32_t signedWord(uint32_t w){return w<=0x7fffffff?int32_t(w):int32_t(int64_t(w)-0x100000000LL);}
std::optional<uint32_t>word(EventNativeRuntime&vm,uint32_t a){uint32_t w;if(!vm.memory.readWord(a,w))return {};return w;}
}
std::optional<int32_t>nativeHarborStylishStart(EventNativeRuntime&vm,EventEntry&e,bool first,bool physicalCurrent,NativeHarborStylishCursor&c){
 if(c.invoking||c.poisoned||!(e.flags&1)||e.argumentCount<5)return {};
 c.invoking=true;struct Guard{bool&v;~Guard(){v=false;}}guard{c.invoking};
 const auto b=word(vm,BattleGlobal);c.pending=BattleGlobal;if(!b||!*b)return {};
 if(c.started&&(c.battle!=*b||c.event!=e.id||c.command!=e.currentCommand))return {};
 if(!c.started&&!first)return {};
 std::array<uint32_t,7>w;
 for(unsigned i=0;i<w.size();++i){c.pending=*b+Counter+4*i;auto q=word(vm,c.pending);if(!q)return {};w[i]=*q;}
 if(!c.started){
  std::array<int32_t,5>v;const auto values=vm.interpreterServices().operands;
  for(unsigned i=0;i<v.size();++i){c.pending=e.arguments+4*i;const auto raw=word(vm,c.pending);if(!raw)return {};v[i]=getEventValue(e,signedWord(*raw),values);
   if(i==0)v[0]=translateBattleActor(&e,v[0],nativeBattleActorServices(vm));}
  // Source Start stores counter,lo,hi,end,actor,result,scan in this order.
  w={0,uint32_t(v[1]),uint32_t(v[2]),uint32_t(v[3]),uint32_t(v[0]),0,uint32_t(v[4])};
 }
 const int32_t actor=signedWord(w[4]);uint32_t unit=0;
 if(actor>=0&&actor<64){c.pending=*b+Registry+4u*uint32_t(actor);const auto q=word(vm,c.pending);if(!q)return {};unit=*q;}
 int32_t result=0;const uint32_t next=w[0]+1u;const int32_t count=signedWord(next),lo=signedWord(w[1]),hi=signedWord(w[2]),end=signedWord(w[3]);
 if(!unit)result=NoTry;
 else {
  c.pending=unit+UnitFlags;const auto flags=word(vm,c.pending);if(!flags)return {};
  if(*flags&ActorSuppression)result=NoTry;
  else {
   c.pending=*b+DebugFlags;const auto debug=word(vm,c.pending);if(!debug)return {};
   if((*debug&StylishDebugDisplay)&&lo<count&&count<=hi){c.pending=0x80010bc4;return {};}
   uint8_t automatic;c.pending=unit+Automatic;if(!vm.memory.readByte(c.pending,automatic))return {};
   const bool autoPressed=automatic&&lo<count&&count<=hi;
   const int32_t scan=signedWord(w[6]);
   // Only actual reached trigger cells are required. Original debug/pad
   // providers cannot be synthesized from an absent physical producer.
   if(!physicalCurrent){c.pending=0x8011a06c;return {};}
   if(count<=1&&scan>0){
    if(uint32_t(scan)>HistoryEntries){c.pending=0x8011a028;return {};}
    for(int32_t i=0;i<scan;++i){c.pending=*b+GeneralPad+TriggerHistory+4u*uint32_t(i);const auto h=word(vm,c.pending);if(!h)return {};if(*h&PhysicalA){result=TooEarly;break;}}
   }
   if(!result){c.pending=*b+GeneralPad+TriggerHistory;const auto h=word(vm,c.pending);if(!h)return {};
    if((*h&PhysicalA)||autoPressed)result=count<=lo?TooEarly:count<=hi?Success:TooLate;
    else if(count>=end)result=NoTry;
   }
  }
 }
 // Native quiescent RAM admission has completed. Any unexpected store loss
 // poisons this cursor; a caller never retries a partially applied prefix.
 auto store=[&](uint32_t at,uint32_t v){c.pending=at;if(!vm.memory.writeWord(at,v)){c.poisoned=true;return false;}return true;};
 if(!c.started){for(unsigned i=0;i<w.size();++i)if(!store(*b+Counter+4*i,w[i]))return {};c.started=true;c.battle=*b;c.event=e.id;c.command=e.currentCommand;}
 if(!store(*b+Counter,next))return {};
 if(result&&!store(*b+Result,uint32_t(result)))return {};
 c.pending=0;return result?1:0;
}
std::optional<int32_t>nativeHarborStylishResult(EventNativeRuntime&vm,EventEntry&e){
 if(!(e.flags&1)||e.argumentCount<2)return {};
 const auto b=word(vm,BattleGlobal),a=word(vm,e.arguments),d=word(vm,e.arguments+4);if(!b||!a||!d)return {};
 // Whole source BattleAcrobatGetResult80192d04 loads result then counter.
 const auto result=word(vm,*b+Result),count=word(vm,*b+Counter);if(!result||!count)return {};
 const auto values=vm.interpreterServices().operands;
 if(signedWord(*a)!=NoOutput)setEventValue(e,signedWord(*a),signedWord(*result),values);
 if(signedWord(*d)!=NoOutput)setEventValue(e,signedWord(*d),signedWord(*count),values);
 return 2;
}
bool nativeHarborPointIncrementZeroDomain(EventNativeRuntime&vm){
 // 802034a0 captures B; actual GetWork8020524c reloads B and returnsB+1616c.
 // Neither pointer is dereferenced before the genuine pouch gate/earlyreturn.
 const auto captured=word(vm,BattleGlobal),fresh=word(vm,BattleGlobal);
 if(!captured||!fresh)return false;
 constexpr uint32_t PouchGlobal=0x8041eb00,StarPowerGate=0x8c;
 const auto pouch=word(vm,PouchGlobal);if(!pouch)return false;
 uint16_t gate;if(!vm.memory.readHalf(*pouch+StarPowerGate,gate))return false;
 return gate==0;
}

}
