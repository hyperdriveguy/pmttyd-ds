#include "field_mario_preparation.hpp"
#include "field_area_event.hpp"
#include "animation_group_prefetch.hpp"
#include "game.hpp"
#include <cassert>
#include <map>
#include <cstring>
using namespace ttyd;
struct World {
 uintptr_t current=0x1000;std::map<uintptr_t,uint32_t> words;std::map<uintptr_t,uint8_t> bytes;std::map<uintptr_t,uint16_t> halves;
 std::vector<std::pair<char,uintptr_t>> trace;bool switchOnWrite=false,mutateCounter=false;
 FieldMarioPreparationWorld services(){FieldMarioPreparationWorld w;auto& s=w.storage;s.user=this;
 s.mario=[](void* p){auto& w=*static_cast<World*>(p);w.trace.push_back({'m',w.current});return w.current;};
 s.read=[](void* p,uintptr_t a){auto& w=*static_cast<World*>(p);w.trace.push_back({'r',a});return w.words.at(a);};
 s.write=[](void* p,uintptr_t a,uint32_t v){auto& w=*static_cast<World*>(p);w.trace.push_back({'w',a});w.words[a]=v;if(w.switchOnWrite)w.current=0x3000;};
 s.camera=[](void* p,int32_t i)->uintptr_t{assert(i==4);static_cast<World*>(p)->trace.push_back({'c',4});return 0x4000;};
 s.half=[](void* p,uintptr_t a){auto& w=*static_cast<World*>(p);w.trace.push_back({'h',a});return w.halves.at(a);};
 w.admit=[](void*,uint32_t){return true;};
 w.byte=[](void* p,uintptr_t a){auto& w=*static_cast<World*>(p);w.trace.push_back({'b',a});return w.bytes.at(a);};
 w.storeByte=[](void* p,uintptr_t a,uint8_t v){auto& w=*static_cast<World*>(p);w.trace.push_back({'B',a});w.bytes[a]=w.mutateCounter?7:v;};
 w.storeHalf=[](void* p,uintptr_t a,uint16_t v){auto& w=*static_cast<World*>(p);w.trace.push_back({'H',a});w.halves[a]=v;};return w;}
};
int main(){
 for(unsigned n=0;n<256;++n){World a;a.words[0x1000]=0xa0000010;a.bytes[0x1039]=n;const auto next=uint8_t(n+1);assert(fieldMarioKeyOff(a.services())==(next<128?int(next):int(next)-256));assert(a.words.at(0x1000)==0xa0000018 && a.bytes.at(0x1039)==next);
  World b;b.words[0x1000]=0xa0000018;b.bytes[0x1039]=n;const auto previous=uint8_t(n-1);assert(fieldMarioKeyOn(b.services())==(previous<128?int(previous):int(previous)-256));assert(b.words.at(0x1000)==(previous?0xa0000018:0xa0000010));}
 World alias;alias.words[0x2000]=0x100;alias.bytes[0x2039]=5;EventEntry e;e.arguments=0x80;
 FieldMarioPreparationOperands o;o.user=&alias;
 o.word=[](void* p,uint32_t a){assert(a==0x80);static_cast<World*>(p)->trace.push_back({'a',a});return 0u;};
 o.value=[](void* p,EventEntry& e,int32_t r){assert(r==0);auto& w=*static_cast<World*>(p);w.trace.push_back({'v',0});w.current=0x2000;w.switchOnWrite=true;e.arguments=0xdead;return 0;};
 assert(fieldMarioKeyEvent(e,o,alias.services())==2 && alias.words.at(0x2000)==0x108 && alias.bytes.at(0x2039)==6);
 assert((alias.trace==std::vector<std::pair<char,uintptr_t>>{{'m',0x1000},{'a',0x80},{'v',0},{'m',0x2000},{'r',0x2000},{'w',0x2000},{'b',0x2039},{'B',0x2039},{'b',0x2039}}));
 World reread;reread.words[0x1000]=8;reread.bytes[0x1039]=1;reread.mutateCounter=true;assert(fieldMarioKeyOn(reread.services())==7 && reread.words.at(0x1000)==8);
 for(bool on:{false,true})for(unsigned form:{0,1,2,255})for(bool small:{false,true}) {
  World w;w.words[0x1000]=(small?0x4000000:0)|0x2000000|0x1234;w.bytes[0x103c]=form;w.words[0x1004]=0xc0028000;w.halves[0x4000]=0x82b5;w.words[0x11b8]=0xdeadbeef;w.words[0x11bc]=0x12345678;
  assert(fieldMarioBackgroundEvent(on,w.services())==2);
  assert(bool(w.words[0x1000]&0x2000000)==on && (w.words[0x1000]&0x1234)==0x1234);
  assert(w.words[0x1184]==0x3f800000 && w.words[0x1188]==(form==1?0x40000000:0x40100000));
  assert(w.words[0x11b8]==(form==2?0xdeadbeef:form==1?0x41a00000:small?0x41200000:0x41a00000));
  assert(w.words[0x11bc]==(form==2?0x12345678:form==1?0x423c0000:small?0x41cf3333:0x42140000));
  assert(w.halves[0x4000]==uint16_t(on?0x82b5:0x82a5) && w.words[0x1004]==(on?0xc0028000:0xc0008000));
  assert(w.trace[w.trace.size()-4]==std::make_pair('H',uintptr_t(0x4000)) && w.trace[w.trace.size()-3]==std::make_pair('m',uintptr_t(0x1000)));
 }
 // Freshly reloaded Mario after main store owns dimensions; follow reloads again.
 World fresh;fresh.words[0x1000]=0;fresh.words[0x3000]=0;fresh.bytes[0x303c]=2;fresh.words[0x3004]=0;fresh.halves[0x4000]=0;fresh.switchOnWrite=true;
 assert(fieldMarioBackgroundEvent(true,fresh.services())==2 && fresh.words[0x1000]==0x2000000 && fresh.words[0x3188]==0x40100000 && !fresh.words.count(0x1188) && fresh.words[0x3004]==0x20000);
 World camera;camera.halves[0x4000]=0x4001;camera.words[0x1004]=0x20;assert(fieldMarioCameraYOffEvent(camera.services())==2 && camera.halves[0x4000]==0x4011 && camera.words[0x1004]==0x20020);
 // Native item acquisition and Bero use one real Mario counter/flag owner.
 Game game(false);assert(game.applyFieldItemStateRequest(ItemGetRequest::KeyOff));auto native=game.fieldMarioPreparationWorld();
 assert(fieldMarioKeyOff(native)==2 && game.fieldMarioBattleState().keyDepth==2 && game.fieldKeysOff);
 assert(fieldMarioKeyOn(native)==1 && game.fieldKeysOff);assert(game.applyFieldItemStateRequest(ItemGetRequest::KeyOn));
 assert(!game.fieldKeysOff && !(game.fieldMarioBattleState().flags&8) && game.fieldMarioBattleState().keyDepth==0);
 assert(fieldMarioBackgroundEvent(false,native)==2); // Genuine ordinary fields and consumers now bound.
 native.storage.write(native.storage.user,native.storage.mario(native.storage.user),0x4000000);
 assert(fieldMarioBackgroundEvent(false,native)==0); // Small/transformed owners remain unsupported.
 // Unchanged original parent/child reaches the next actual party request.
 assert(loadFieldAreaEventAssets("romfs") && loadFieldMapFlagAssets("romfs") && loadFieldBeroInfoAssets("romfs"));
 AnimationGroupPrefetch cache;assert(cache.open("romfs/field-prefetch"));FieldAreaEvent area;assert(area.begin(2,0));FieldAreaGpState state{};
 std::memcpy(state.bytes.data()+0x11c-FieldAreaGpState::Begin,"w_bero",7);state.bytes[0x176-FieldAreaGpState::Begin]=1;state.bytes[0x177-FieldAreaGpState::Begin]=146;assert(area.appendAreaInit(state));
 FieldMapFlags map;map.synchronize(2);FieldBeroInfo info;info.synchronize(2);FieldBeroExec exec;
 World actor;actor.words[0x1000]=0x2000000;actor.words[0x1004]=0x20000;actor.bytes[0x1039]=0;actor.bytes[0x103c]=0;actor.halves[0x4000]=0x10;
 uint32_t next=0;FieldAreaEventServices s;s.prefetch=&cache;s.map=&map;s.beroInfo=&info;s.beroExec=&exec;s.mario=actor.services();s.user=&next;
 s.call=[](void* p,FieldAreaNativeCall& r){*static_cast<uint32_t*>(p)=r.function;return 0;};
 for(unsigned t=1;t<40 && !next;++t)area.tick(t*17,s);
 assert(!area.failed && next==0x800e3058 && area.pendingFunction==next && area.liveEventCount()==2 && exec.read()==1);
 assert(actor.bytes.at(0x1039)==1 && actor.words.at(0x1000)==8 && actor.words.at(0x1004)==0 && actor.halves.at(0x4000)==0);
}
