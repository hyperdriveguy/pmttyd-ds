#include "battle_menu_audience_native.hpp"
#include "battle_audience_item_on.hpp"
#include "battle_audience_item_on_native.hpp"
#include "battle_weapon_menu_main.hpp"
#include <cassert>
#include <functional>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
struct Fixture {
 static constexpr uint32_t B=0x10000000,C=0x11000000,U=0x20000000,Private=0x30000000;
 struct Access {uint32_t kind,address,value;bool operator==(const Access& b)const{return kind==b.kind&&address==b.address&&value==b.value;}};
 std::map<uint32_t,uint8_t> bytes;
 std::vector<Access> trace;
 bool lease=true,release=false,reenter=false;unsigned stop=0,triggerCalls=0,marioCalls=0,endCalls=0;
 uint32_t pressed=0x400;
 BattleMenuAudienceNativeCursor* cursor=nullptr;
 std::function<void(Fixture&)> onTrigger,onMario;
 std::function<void(Fixture&,uint32_t)> onStore;
 Fixture(){seed(0x8041eb90,B);seed(0x804249cc,0x41a00000);seed(B+0x1c5c,3);}
 uint8_t byte(uint32_t a)const{const auto i=bytes.find(a);return i==bytes.end()?0:i->second;}
 uint32_t raw(uint32_t a)const{uint32_t n=0;for(unsigned j=0;j<4;++j)n=n<<8|byte(a+j);return n;}
 void seed(uint32_t a,uint32_t n){for(unsigned j=0;j<4;++j)bytes[a+j]=uint8_t(n>>(24-8*j));}
 void half(uint32_t a,uint16_t n){bytes[a]=uint8_t(n>>8);bytes[a+1]=uint8_t(n);}
 uint32_t word(uint32_t a){auto n=raw(a);if(a<Private||a>=Private+16)trace.push_back({0,a,n});return n;}
 void store(uint32_t a,uint32_t n){seed(a,n);if(a<Private||a>=Private+16)trace.push_back({1,a,n});if(onStore)onStore(*this,a);}
 uint32_t member(unsigned i,uint32_t b=B)const{return b+0x2858+0x1bc+i*0x134;}
 uint32_t item(unsigned i,uint32_t b=B)const{return b+0x2858+0xf25c+i*0x48;}
 void holder(unsigned i,int state=3,uint32_t flags=1,uint32_t b=B){auto r=member(i,b);seed(r,flags);half(r+0x1c,uint16_t(i));seed(item(i,b)+4,uint32_t(state));seed(item(i,b)+0x10,236);seed(r+0x48,0x41200000);seed(r+0x4c,0x41a00000);seed(r+0x50,0x41f00000);}
 BattleActorServices actors(){BattleActorServices a;a.user=this;a.memory.user=this;
  a.byte=[](void* p,uint32_t at){auto& f=*static_cast<Fixture*>(p);auto n=f.byte(at);f.trace.push_back({2,at,n});return n;};
  a.memory.word=[](void* p,uint32_t at){return static_cast<Fixture*>(p)->word(at);};
  a.memory.half=[](void* p,uint32_t at){auto& f=*static_cast<Fixture*>(p);auto n=uint16_t(uint16_t(f.byte(at))<<8|f.byte(at+1));f.trace.push_back({3,at,n});return n;};
  a.memory.storeWord=[](void* p,uint32_t at,uint32_t n){static_cast<Fixture*>(p)->store(at,n);};
  a.memory.storeHalf=[](void* p,uint32_t at,uint16_t n){auto& f=*static_cast<Fixture*>(p);f.half(at,n);f.trace.push_back({4,at,n});};
  a.memory.storeByte=[](void* p,uint32_t at,uint8_t n){auto& f=*static_cast<Fixture*>(p);f.bytes[at]=n;f.trace.push_back({5,at,n});};return a;
 }
 bool held(unsigned stage){
  if(reenter){assert(cursor);assert(!cursor->reset());assert(!resumeNativeBattleMenuAudience(services(),*cursor));}
  return stop==stage&&!release;
 }
 BattleMenuAudienceNativeServices services(){BattleMenuAudienceNativeServices s;s.actors=actors();s.user=this;
  s.allocationCurrent=[](void* p){return static_cast<Fixture*>(p)->lease;};
  s.trigger=[](void* p,uint32_t mask)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);assert(mask==0x400);if(f.held(1))return {}; ++f.triggerCalls;if(f.onTrigger)f.onTrigger(f);return f.pressed&mask;};
  s.mario=[](void* p,uint32_t b)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);assert(b==B);if(f.held(2))return {}; ++f.marioCalls;if(f.onMario)f.onMario(f);return U;};
  s.endActMenu=[](void* p,uint32_t b){auto& f=*static_cast<Fixture*>(p);assert(b==B);if(f.held(3))return false;++f.endCalls;f.store(B+0x1c5c,6);f.store(B+0xef4,0x80);return true;};return s;
 }
 bool reference(){BattleMenuAudienceServices s;s.memory=actors();s.user=this;s.privateItem=Private;
  s.trigger=[](void* p,uint32_t mask){auto& f=*static_cast<Fixture*>(p);++f.triggerCalls;if(f.onTrigger)f.onTrigger(f);return f.pressed&mask;};
  s.mario=[](void* p,uint32_t b){auto& f=*static_cast<Fixture*>(p);assert(b==B);++f.marioCalls;if(f.onMario)f.onMario(f);return U;};
  s.itemOn=[](void* p,uint32_t out,int32_t a,int32_t b,int32_t c,int32_t d){assert(!a&&!b&&!c&&!d);auto& f=*static_cast<Fixture*>(p);packedBattleAudienceItemOn({out,0,0,0,0},f.actors());return true;};
  s.setTarget=[](void* p,int32_t target){auto& f=*static_cast<Fixture*>(p);packedBattleAudienceSetTarget(target,f.actors());};
  s.endActMenuReady=[](void* p,uint32_t b){auto& f=*static_cast<Fixture*>(p);assert(b==B);++f.endCalls;f.store(B+0x1c5c,6);f.store(B+0xef4,0x80);return true;};
  s.lightOff=[](void*,int32_t a,int32_t b){assert(a==0&&b==1);};
  BattleMenuAudienceCursor c;auto result=resumePackedBattleMenuAudienceCheck(s,c);assert(result);return *result;
 }
 auto visibleBytes()const{auto result=bytes;for(unsigned j=0;j<16;++j)result.erase(Private+j);return result;}
};
void compare(Fixture baseline){
 // Four-output footer signature retains the actual extra cache reads even
 // though the original footer consumes only the disjoint private index.
 Fixture query=baseline,reference=baseline;
 packedBattleAudienceItemOn({Fixture::Private,Fixture::Private+4,Fixture::Private+8,Fixture::Private+12,0},reference.actors());
 const auto selected=nativeBattleAudienceItemOn(query.actors(),BattleAudiencePrivateOutputs::IndexAndPosition);
 assert(uint32_t(selected.index)==reference.raw(Fixture::Private));
 assert(query.visibleBytes()==reference.visibleBytes()&&query.trace==reference.trace);
 if(selected.index!=-1){assert(selected.position);for(unsigned j=0;j<3;++j)assert((*selected.position)[j]==reference.raw(Fixture::Private+4+4*j));}
 else assert(!selected.position);

 Fixture actual=baseline;const bool expected=baseline.reference();BattleMenuAudienceNativeCursor c;
 auto result=resumeNativeBattleMenuAudience(actual.services(),c);assert(result&&*result==expected&&!c.active());
 assert(actual.visibleBytes()==baseline.visibleBytes()&&actual.trace==baseline.trace);
 assert(actual.triggerCalls==baseline.triggerCalls&&actual.marioCalls==baseline.marioCalls&&actual.endCalls==baseline.endCalls);
}
}
int main(){
 Fixture empty;compare(empty);
 for(uint32_t pressed:{0u,0x100u,0x400u}){Fixture f;f.pressed=pressed;f.holder(0);compare(f);}
 Fixture blocked;blocked.seed(Fixture::B+0xef4,0x40000000);blocked.holder(0);compare(blocked);
 Fixture mario; mario.seed(Fixture::U+0x104,0x10);mario.holder(0);compare(mario);
 for(int state:{-9,0,4,5,6,7,8})for(uint32_t flags:{0u,1u,3u,0x101u}){Fixture f;f.holder(0,state,flags);f.holder(199);compare(f);}
 Fixture good;good.holder(0);good.seed(good.item(0)+0x10,123);good.holder(1);compare(good); // X keeps original first-eligible good item.
 Fixture cancel;cancel.holder(0);cancel.holder(2,7);cancel.bytes[cancel.member(2)+0x19]=7;compare(cancel);
 Fixture fresh;fresh.holder(0,3,1,Fixture::C);fresh.onTrigger=[](Fixture& f){f.seed(0x8041eb90,Fixture::C);};compare(fresh);
 Fixture changed;changed.holder(0);changed.onStore=[](Fixture& f,uint32_t a){if(a==Fixture::B+0x2858+0x138bc)f.seed(a,99);};compare(changed);
 for(unsigned stop:{1u,2u,3u}){
  Fixture f;f.holder(0);Fixture baseline=f;baseline.reference();f.stop=stop;BattleMenuAudienceNativeCursor c;f.cursor=&c;
  assert(!resumeNativeBattleMenuAudience(f.services(),c)&&c.active());auto prefix=f.trace;auto bytes=f.bytes;
  assert(!resumeNativeBattleMenuAudience(f.services(),c)&&f.trace==prefix&&f.bytes==bytes);
  auto wrong=f.services();wrong.user=&baseline;assert(!resumeNativeBattleMenuAudience(wrong,c));
  wrong=f.services();wrong.actors.audienceLifecycle.user=&baseline;assert(!resumeNativeBattleMenuAudience(wrong,c));
  assert(f.trace==prefix&&f.bytes==bytes);
  f.release=true;f.reenter=true;auto result=resumeNativeBattleMenuAudience(f.services(),c);assert(result&&*result);
  assert(f.visibleBytes()==baseline.visibleBytes()&&f.trace==baseline.trace&&f.triggerCalls==1&&f.marioCalls==1&&f.endCalls==1);
 }
 Fixture lost;lost.holder(0);lost.stop=3;BattleMenuAudienceNativeCursor c;assert(!resumeNativeBattleMenuAudience(lost.services(),c));auto before=lost.trace;
 lost.lease=false;assert(!resumeNativeBattleMenuAudience(lost.services(),c)&&c.stale());lost.lease=true;lost.release=true;
 assert(!resumeNativeBattleMenuAudience(lost.services(),c)&&lost.trace==before);assert(c.reset());
 Fixture invalid;invalid.holder(0);invalid.onMario=[](Fixture& f){f.lease=false;};BattleMenuAudienceNativeCursor ic;
 assert(!resumeNativeBattleMenuAudience(invalid.services(),ic)&&ic.stale()&&invalid.endCalls==0);
 return 0;
}
