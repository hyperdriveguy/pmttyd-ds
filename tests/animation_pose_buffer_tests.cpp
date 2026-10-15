#include "animation_buffer_refresh.hpp"
#include <cassert>
#include <functional>
#include <map>
#include <vector>
using namespace ttyd;
struct PoseBuffers {
 uint32_t work=0x81000000,poses=0x82000000,table=0x83000000,file=0x84000000,holder=0x85000000,model=0x86000000,base=0x87000000,hold=0;
 std::map<uint32_t,uint32_t> words;
 std::vector<BattleSequenceCall> calls;
 std::vector<std::pair<uint32_t,uint32_t>> stores;
 std::function<void(uint32_t,uint32_t)> onStore;
 std::function<void(BattleSequenceCall&)> onCall;
 PoseBuffers(){words[0x80415dc0]=work;words[work]=table;words[work+0x10]=poses;words[work+0x14]=3;words[work+0x104]=base;words[work+0x108]=base;words[work+0x10c]=0;words[table+8]=file;words[file+0xa0]=holder;words[holder]=model;
  for(unsigned i=0;i<3;++i){words[poses+i*0x170]=1;words[poses+i*0x170+0xc]=i;words[poses+i*0x170+0x10]=0;}
  words[model+0xf0]=3;words[model+0xf8]=2;words[model+0x13c]=7;words[model+0x140]=5;words[model+0x12c]=2;}
 BattleSequenceEntryServices services(){BattleSequenceEntryServices s;s.user=this;
 s.word=[](void* p,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<PoseBuffers*>(p);auto at=f.words.find(a);return at==f.words.end()?std::nullopt:std::optional<uint32_t>(at->second);};
 s.store=[](void* p,uint32_t a,uint32_t v){auto& f=*static_cast<PoseBuffers*>(p);f.words[a]=v;f.stores.push_back({a,v});if(f.onStore)f.onStore(a,v);return true;};
 s.call=[](void* p,BattleSequenceCall& c){auto& f=*static_cast<PoseBuffers*>(p);f.calls.push_back(c);if(c.function==f.hold)return false;if(c.function==0x800300f0)c.result=f.base;if(f.onCall)f.onCall(c);return true;};return s;}
 unsigned count(uint32_t fn){unsigned n=0;for(auto& c:calls)n+=c.function==fn;return n;}
};

void runAnimationPoseBufferFixture(){
 // All eight source requests, one final invalidation, no clearing data arrays.
 {PoseBuffers f;AnimationBufferCursor c;f.words[f.work+0x108]=f.base+3;
  assert(runAnimationBufferFunction(0x80041ee0,f.poses,f.services(),c));
  constexpr uint32_t offsets[]={0x48,0x4c,0x50,0x54,0x58,0x60,0x64,0x68};
  constexpr uint32_t sizes[]={36,36,24,24,7,20,20,48};uint32_t cursor=f.base+3;
  for(unsigned i=0;i<8;++i){assert(f.words[f.poses+offsets[i]]==cursor);cursor+=(sizes[i]+31)&~31u;}
  assert(f.words[f.work+0x108]==cursor && f.words[f.poses+0x58]==f.words[f.poses+0x5c] && f.words[f.poses+0x68]==f.words[f.poses+0x6c]);
  unsigned cache=0;for(auto w:f.stores)cache+=w.first==f.poses+0x3c;assert(cache==1 && f.calls.empty());}
 // Captured return cursor precedes header/count lookup; publication aliases
 // reread live values, not the child's stale return value.
 {PoseBuffers f;AnimationBufferCursor c;auto s=f.services();auto readWord=s.word;
  struct Intercept{PoseBuffers* f;decltype(readWord) read;bool changed=false;} ctx{&f,readWord};s.user=&ctx;
  s.word=[](void* p,uint32_t a)->std::optional<uint32_t>{auto& x=*static_cast<Intercept*>(p);if(a==x.f->model+0xf0 && !x.changed){x.changed=true;x.f->words[x.f->work+0x108]=x.f->base+0x100;}return x.read(x.f,a);};
  s.store=[](void* p,uint32_t a,uint32_t v){auto& f=*static_cast<Intercept*>(p)->f;f.words[a]=v;if(a==f.poses+0x58)f.words[a]=0x1234;if(a==f.poses+0x68)f.words[a]=0x5678;return true;};
  assert(runAnimationBufferFunction(0x80041ee0,f.poses,s,c));assert(f.words[f.poses+0x48]==f.base && f.words[f.poses+0x4c]==f.base+0x140 && f.words[f.poses+0x5c]==0x1234 && f.words[f.poses+0x6c]==0x5678);}
 // First overflow must reset+recursive Restore rather than inline Rebuild.
 // The source loop's live mode filter skips mode1 in field context.
 for(uint32_t mode:{0u,1u}){PoseBuffers f;AnimationBufferCursor c;f.words[f.work+0x10c]=mode;f.words[f.work+0x108]=f.base+0x17ffe0;
  bool changed=false;f.onStore=[&](uint32_t a,uint32_t){if(a==f.work+0x108 && !changed){changed=true;f.words[f.work+0x14]=0;}};
  assert(runAnimationBufferFunction(0x80041ee0,f.poses,f.services(),c));assert(f.words[f.poses+0x48]==f.base && f.calls.empty());}
 // Missing allocation retains its call and captured pose across a changed
 // caller argument. Resume publishes through current W, then completes.
 {PoseBuffers f;AnimationBufferCursor c;f.words[f.work+0x104]=0;f.words[f.work+0x108]=0x180000;f.words[f.work+0x14]=0;f.hold=0x800300f0;
  assert(!runAnimationBufferFunction(0x80041ee0,f.poses,f.services(),c));assert(pendingAnimationBufferFunction(c)==f.hold);auto writes=f.stores.size();
  assert(!runAnimationBufferFunction(0x80041ee0,f.poses+0x170,f.services(),c) && writes==f.stores.size());f.hold=0;
  assert(runAnimationBufferFunction(0x80041ee0,f.poses+0x170,f.services(),c) && f.words[f.poses+0x48]==f.base && !f.words.count(f.poses+0x170+0x48));}
 // Complete real recursive selected-pose recovery, preserving native bound
 // for truly nonterminating source requests rather than manufacturing success.
 for(uint32_t mode:{0u,1u}){PoseBuffers f;AnimationBufferCursor c;f.words[f.work+0x14]=1;f.words[f.poses+0xc]=2;f.words[f.work+0x10c]=mode;f.words[f.work+0x108]=f.base+0x17ffe0;
  assert(runAnimationBufferFunction(0x80041ee0,f.poses,f.services(),c));unsigned cache=0;for(auto w:f.stores)cache+=w.first==f.poses+0x3c;assert(cache==3 && f.calls.empty());}
 // Second overflow reaches real Refresh, then testAlloc with the retained
 // aligned request even after the loaded MODEL changes during recovery.
 {PoseBuffers f;AnimationBufferCursor c;f.words[f.work+0x14]=1;f.words[f.work+0x108]=f.base+0x17ffe0;f.words[f.model+0xf0]=0x1fffc;unsigned cache=0;bool first=true;
  f.onStore=[&](uint32_t a,uint32_t){if(a==f.work+0x108 && first){first=false;f.words[f.model+0xf0]=3;}if(a==f.poses+0x3c && ++cache==3){f.words[f.work+0x14]=0;f.words[f.poses]=0;}};
  assert(runAnimationBufferFunction(0x80041ee0,f.poses,f.services(),c));assert(f.words[f.poses+0x48]==f.base && cache==5 && f.calls.empty());}
 {PoseBuffers f;AnimationBufferCursor c;f.words[f.work+0x14]=0;f.words[f.model+0xf0]=0x20000;
  assert(!runAnimationBufferFunction(0x80041ee0,f.poses,f.services(),c));auto writes=f.stores.size();assert(!runAnimationBufferFunction(0x80041ee0,f.poses,f.services(),c) && writes==f.stores.size());}
}
int main(){runAnimationPoseBufferFixture();}
