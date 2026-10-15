#include "animation_buffer_refresh.hpp"
#include <cassert>
#include <functional>
#include <map>
#include <vector>
using namespace ttyd;
struct Buffers {
 uint32_t work=0x81000000,poses=0x82000000,table=0x83000000,file=0x84000000,holder=0x85000000,model=0x86000000,base=0x87000000,hold=0;
 std::map<uint32_t,uint32_t> words;
 std::vector<BattleSequenceCall> calls;
 std::vector<std::pair<uint32_t,uint32_t>> stores;
 std::function<void(uint32_t,uint32_t)> onStore;
 std::function<void(BattleSequenceCall&)> onCall;
 Buffers(){words[0x80415dc0]=work;words[work]=table;words[work+0x10]=poses;words[work+0x14]=3;words[work+0x104]=base;words[work+0x108]=base;words[work+0x10c]=0;words[table+8]=file;words[file+0xa0]=holder;words[holder]=model;
  for(unsigned i=0;i<3;++i){words[poses+i*0x170]=1;words[poses+i*0x170+0xc]=i;words[poses+i*0x170+0x10]=0;}
  words[model+0xf0]=3;words[model+0xf8]=2;words[model+0x13c]=7;words[model+0x140]=5;words[model+0x12c]=2;}
 BattleSequenceEntryServices services(){BattleSequenceEntryServices s;s.user=this;
 s.word=[](void* p,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<Buffers*>(p);auto at=f.words.find(a);return at==f.words.end()?std::nullopt:std::optional<uint32_t>(at->second);};
 s.store=[](void* p,uint32_t a,uint32_t v){auto& f=*static_cast<Buffers*>(p);f.words[a]=v;f.stores.push_back({a,v});if(f.onStore)f.onStore(a,v);return true;};
 s.call=[](void* p,BattleSequenceCall& c){auto& f=*static_cast<Buffers*>(p);f.calls.push_back(c);if(c.function==f.hold)return false;if(c.function==0x800300f0)c.result=f.base;if(f.onCall)f.onCall(c);return true;};return s;}
 unsigned count(uint32_t fn){unsigned n=0;for(auto& c:calls)n+=c.function==fn;return n;}
};
int main(){
 for(uint32_t bytes:{0u,1u,31u,32u,33u,0xffffffffu}){Buffers f;AnimationBufferCursor c;assert(runAnimationBufferFunction(0x80042ac8,bytes,f.services(),c));assert(c.result==f.base && f.words[f.work+0x108]==f.base+((bytes+31)&~31u) && f.calls.empty());}
 {Buffers f;AnimationBufferCursor c;f.words[f.work+0x14]=0;f.words[f.work+0x108]=f.base+0x17ffc0;assert(runAnimationBufferFunction(0x80042ac8,32,f.services(),c) && c.result==f.base+0x17ffc0);}
 for(unsigned delta:{0u,32u}){Buffers f;AnimationBufferCursor c;f.words[f.work+0x14]=0;f.words[f.work+0x108]=f.base+0x17ffe0+delta;assert(runAnimationBufferFunction(0x80042ac8,32,f.services(),c) && c.result==f.base && f.words[f.work+0x108]==f.base+32);}
 {Buffers f;AnimationBufferCursor c;f.words[f.work+0x104]=0xfffffff0;f.words[f.work+0x108]=0xfffffff0;assert(runAnimationBufferFunction(0x80042ac8,32,f.services(),c) && c.result==0xfffffff0 && f.words[f.work+0x108]==16);}
 for(uint32_t mode:{0u,1u,2u}){Buffers f;AnimationBufferCursor c;f.words[f.work+0x10c]=mode;assert(runAnimationBufferFunction(0x80041c84,0,f.services(),c));
  for(unsigned i=0;i<3;++i){auto p=f.poses+i*0x170;bool selected=mode?i!=0:i!=1;assert((f.words.count(p+0x48)!=0)==selected);if(!selected)continue;
   assert(f.words[p+0x58]==f.words[p+0x5c] && f.words[p+0x68]==f.words[p+0x6c] && f.words[p+0x3c]==0xffffffff);unsigned invalidations=0;for(auto v:f.stores)invalidations+=v.first==p+0x3c;assert(invalidations==2);}
  assert(f.calls.empty());}
 {Buffers f;AnimationBufferCursor c;f.words[f.work+0x104]=0;f.hold=0x800300f0;assert(!runAnimationBufferFunction(0x80041c58,0,f.services(),c) && pendingAnimationBufferFunction(c)==f.hold && f.words[f.work+0x10c]==1);
  const auto newer=f.work+0x1000;f.words[0x80415dc0]=newer;f.words[newer+0x14]=0;f.hold=0;assert(runAnimationBufferFunction(0x80041c58,0,f.services(),c) && f.words[newer+0x104]==f.base && f.words[newer+0x108]==f.base && f.words[f.work+0x104]==0);assert(f.calls.back().args[0]==0 && f.calls.back().args[1]==0x180000);}
 {Buffers f;AnimationBufferCursor c;f.words[f.work+0x14]=1;unsigned allocations=0;
  f.onStore=[&](uint32_t a,uint32_t){if(a==f.poses+0x48){++allocations;f.words[f.model+0xf0]=9;}};
  assert(runAnimationBufferFunction(0x80041c84,0,f.services(),c));assert(allocations==1 && f.words[f.poses+0x4c]==f.base+64 && f.words[f.poses+0x50]==f.base+192);}
 {Buffers f;AnimationBufferCursor c;f.words[f.work+0x14]=1;f.words[f.work+0x108]=f.base+0x17ffe0;unsigned invalidations=0;
  f.onStore=[&](uint32_t a,uint32_t){if(a==f.poses+0x3c && ++invalidations==3){f.words[f.work+0x14]=0;f.words[f.poses]=0;}};
  assert(runAnimationBufferFunction(0x80042ac8,0x17ffe0,f.services(),c) && c.result==f.base && f.words[f.work+0x108]==f.base+0x17ffe0);
  assert(invalidations==4 && f.count(0x80041ee0)==0); // Real recursive child plus caller's own cache store.
 }
 {Buffers f;AnimationBufferCursor c;f.words[f.work+0x104]=0;f.words[f.work+0x14]=0;f.words[f.work+0x108]=0x180000;f.hold=0x800300f0;
  assert(!runAnimationBufferFunction(0x80042ac8,1,f.services(),c));auto stored=f.words[f.work+0x108];assert(stored==0x180020);assert(!runAnimationBufferFunction(0x80042ac8,1,f.services(),c) && f.words[f.work+0x108]==stored);f.hold=0;assert(runAnimationBufferFunction(0x80042ac8,1,f.services(),c) && c.result==f.base);}
 {Buffers f;AnimationBufferCursor c;f.words[f.poses]=0x80000000;assert(runAnimationBufferFunction(0x80041c58,0,f.services(),c) && f.words[f.work+0x10c]==1);assert(!f.words.count(f.poses+0x48));}
 {Buffers f;AnimationBufferCursor c;f.words[f.work+0x14]=0;
  assert(!runAnimationBufferFunction(0x80042ac8,0x180000,f.services(),c));const auto stores=f.stores.size();
  const auto pending=pendingAnimationBufferFunction(c);assert(stores<1000 && (pending==0x80041c84 || pending==0x80042ac8 || pending==0x80042e5c));
  assert(!runAnimationBufferFunction(0x80042ac8,0x180000,f.services(),c) && f.stores.size()==stores);}

}
