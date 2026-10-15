#include "battle_audience_item_on_native.hpp"
#include <cassert>
#include <cstring>
// Source field offsets/masks in these notes are hex; counts/item IDs and
// byte sizes are decimal unless prefixed. Addresses are original source tokens.
namespace ttyd {
namespace {
int32_t signedWord(uint32_t n){return n<0x80000000u?int32_t(n):int32_t(int64_t(n)-0x100000000LL);}
float scalar(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
}
// Capture cache base and member start from TWO actual current-B reads. Member
// stride134hex,200 slots; require mask0x1 and exclude0x100/0x2. Signed held half1c
// rejects ONLY-1. Item stride48hex uses another fresh B; signed state<=4 or7
// admits,including original negative-state domain. No harmful-only filter.
BattleAudienceNativeItemOn nativeBattleAudienceItemOn(
    const BattleActorServices& a,BattleAudiencePrivateOutputs outputs){
 const auto& m=a.memory;assert(m.word&&m.half&&m.storeWord);
 const auto word=[&](uint32_t at){return m.word(m.user,at);};
 const auto base=currentBattleAddress(a)+0x2858;
 auto member=currentBattleAddress(a)+0x2858+0x1bc;
 for(uint32_t i=0;i<200;++i,member+=0x134){
  const auto flags=word(member);if(!(flags&1)||(flags&0x100)||(flags&2))continue;
  const auto raw=m.half(m.user,member+0x1c);
  const auto held=raw<0x8000?int32_t(raw):int32_t(raw)-65536;
  if(held==-1)continue;
  const auto item=currentBattleAddress(a)+0x2858+uint32_t(held)*0x48+0xf25c;
  const auto state=signedWord(word(item+4));if(state>4&&state!=7)continue;
  // Cache index138bc and XYZ138c0/4/8 belong to the first captured base.
  // Z is actual member50 plus source249cc offset3. Four-output signature
  // rereads cache XYZ AFTER stores,not detached member values. No match leaves
  // cache unchanged and returns private index-1 with absent position.
  m.storeWord(m.user,base+0x138bc,i);
  BattleAudienceNativeItemOn result;result.index=int32_t(i);
  m.storeWord(m.user,base+0x138c0,word(member+0x48));
  m.storeWord(m.user,base+0x138c4,word(member+0x4c));
  const auto z=scalar(word(member+0x50)),offset=scalar(word(0x804249cc));
  m.storeWord(m.user,base+0x138c8,bits(z+offset));
  if(outputs==BattleAudiencePrivateOutputs::IndexAndPosition){
   std::array<uint32_t,3> position;
   position[0]=word(base+0x138c0);
   position[1]=word(base+0x138c4);
   position[2]=word(base+0x138c8);
   result.position=position;
  }
  return result;
 }
 return {};
}
}
