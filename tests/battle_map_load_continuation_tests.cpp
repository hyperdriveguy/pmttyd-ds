#include "battle_map_load_continuation.hpp"
#include <cassert>
#include <map>
#include <string>
#include <tuple>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Work=0x81000000,Alternate=0x82000000,WorkGlobal=0x8041e660,Group=0x8041e664,Heap=0x8041e808,GroupBytes=0x2f4,EntryBytes=0x178;
constexpr uint32_t Free=0x8002fdc4,Archive=0x80078444,Smart=0x801526d4,Clear=0x800050b4,Load=0x80028f18,Sync=0x80005ce4;
using Call=std::tuple<uint32_t,uint32_t,std::array<uint32_t,3>,unsigned>;
struct Fixture {
 std::map<uint32_t,uint32_t> words;
 std::vector<std::tuple<char,uint32_t,uint32_t>> memory;
 std::vector<Call> calls;
 std::shared_ptr<int> life=std::make_shared<int>(0);
 bool alive=true,mutate=false,changed=false,reenter=false,shrink=false,shrunk=false;
 unsigned hold=UINT32_MAX,waits=0,accepted=0;
 BattleMapLoadCursor cursor;
 explicit Fixture(int32_t count=2){
  for(auto base:{Work,Alternate})for(unsigned i=0;i<4*GroupBytes;i+=4)words[base+i]=0;
  words[WorkGlobal]=Work;words[Group]=7;words[Heap]=0x83000000;words[Work]=uint32_t(count);
  for(unsigned i=0;i<2;++i){const auto e=Work+i*EntryBytes;words[e+0x16c]=0x9000+4*i;words[e+0x7c]=0xa000+4*i;words[e+0x80]=100+i;words[e+0x84]=0xb000+4*i;words[e+0x88]=200+i;words[e+0x8c]=0xc000+4*i;words[e+0x90]=300+i;}
  words[Alternate+GroupBytes+EntryBytes+0x16c]=0xdd00;
 }
 uint32_t word(uint32_t a){assert(words.count(a));const auto v=words.at(a);memory.emplace_back('r',a,v);return v;}
 void store(uint32_t a,uint32_t v){assert(words.count(a));words[a]=v;memory.emplace_back('w',a,v);}
 void effect(BattleMapLoadRequest&q){
  calls.emplace_back(q.function,q.callsite,q.arguments,q.count);++accepted;
  if(q.function==Archive)q.result=q.arguments[0]==0xa004?1:0;
  if(q.function==Smart)q.result=q.arguments[0]+0x100000;
  if(q.function==Clear)for(uint32_t i=0;i<q.arguments[2];i+=4)store(q.arguments[0]+i,0);
  if(shrink&&!shrunk&&q.function==Free){store(Work,1);shrunk=true;}
  if(mutate&&!changed&&q.function==Free){store(WorkGlobal,Alternate);store(Group,1);changed=true;}
 }
 BattleMapLoadServices services(){BattleMapLoadServices s;s.user=this;s.lifetime=life;s.current=[](void*p){return static_cast<Fixture*>(p)->alive;};s.memory.user=this;
  s.memory.word=[](void*p,uint32_t a){return static_cast<Fixture*>(p)->word(a);};s.memory.storeWord=[](void*p,uint32_t a,uint32_t v){static_cast<Fixture*>(p)->store(a,v);};
  s.call=[](void*p,BattleMapLoadRequest&q){auto&f=*static_cast<Fixture*>(p);
   if(f.reenter){f.reenter=false;assert(!resumePackedBattleMapLoad(0,0,f.services(),f.cursor));}
   if(f.accepted==f.hold&&f.waits++<2)return false;
   f.effect(q);return true;
  };return s;
 }
 // Straight-line original instruction behavior is independent of retained steps.
 void reference(uint32_t base,uint32_t foreground){
  auto call=[&](uint32_t site,uint32_t fn,unsigned n,std::array<uint32_t,3>a){BattleMapLoadRequest q;q.function=fn;q.callsite=site;q.count=n;q.arguments=a;effect(q);return q.result.value_or(0);};
  call(0x80028944,Sync,0,{});const auto captured=word(WorkGlobal);store(Group,0);
  uint32_t offset=0,entry=captured,index=0;
  while(int32_t(index)<int32_t(word(captured))){
   const auto group=word(Group),work=word(WorkGlobal),tail=work+group*GroupBytes+offset;
   const auto pointer=word(tail+0x16c);
   if(pointer)call(0x80028984,Free,2,{word(Heap),pointer,0});
   store(tail+0x168,0);store(tail+0x16c,0);store(entry+0x170,0);store(entry+0x178,0);store(entry+0x174,0);
   const auto d=word(entry+0x7c);
   if(d){
    if(!call(0x800289ac,Archive,1,{d,0,0})){
     const auto d0=word(entry+0x7c),d1=word(entry+0x80);const auto smartD=call(0x800289c0,Smart,2,{d0,d1,0});store(entry+0x170,smartD);
     const auto t0=word(entry+0x84),t1=word(entry+0x88);const auto smartT=call(0x800289d0,Smart,2,{t0,t1,0});store(entry+0x178,smartT);
     call(0x800289e0,Free,2,{word(Heap),word(entry+0x84),0});call(0x800289ec,Free,2,{word(Heap),word(entry+0x7c),0});store(entry+0x84,0);store(entry+0x7c,0);
    }else{
     const auto a0=word(entry+0x8c),a1=word(entry+0x90);const auto smart=call(0x80028a08,Smart,2,{a0,a1,0});store(entry+0x174,smart);
     call(0x80028a18,Free,2,{word(Heap),word(entry+0x8c),0});store(entry+0x8c,0);
    }
   }
   offset+=EntryBytes;entry+=EntryBytes;++index;
  }
  const auto bank=word(WorkGlobal)+GroupBytes;store(Group,1);call(0x80028a58,Clear,3,{bank,0,GroupBytes});call(0x80028a68,Load,3,{bank,0,base});store(bank,1);
  if(foreground){call(0x80028a88,Load,3,{bank,1,foreground});store(bank,2);}
 }
};
void cases(){
 for(int32_t count:{-1,0,1,2})for(bool fore:{false,true})for(bool mutate:{false,true})for(bool empty:{false,true})for(bool shrink:{false,true}){
  Fixture reference(count);reference.mutate=mutate;reference.shrink=shrink;if(empty){reference.words[Work+0x16c]=0;reference.words[Work+0x7c]=0;}reference.reference(0x95400000,fore?0x95400010:0);
  for(unsigned hold=0;hold<=reference.calls.size();++hold){Fixture f(count);f.mutate=mutate;f.shrink=shrink;if(empty){f.words[Work+0x16c]=0;f.words[Work+0x7c]=0;}f.hold=hold;f.reenter=true;
   for(unsigned retry=0;!resumePackedBattleMapLoad(0x95400000,fore?0x95400010:0,f.services(),f.cursor);++retry){assert(retry<3);assert(f.cursor.pending());}
   assert(f.cursor.complete()&&!f.cursor.pending());assert(f.calls==reference.calls&&f.words==reference.words);
   // Compare every source read/store; held children cannot replay operand loads.
   assert(f.memory==reference.memory);
   const auto size=f.calls.size();assert(resumePackedBattleMapLoad(7,8,f.services(),f.cursor));assert(f.calls.size()==size);
  }
 }
}
void capturedNames(){
 Fixture f(0);f.hold=0;assert(!resumePackedBattleMapLoad(0x1110,0x2220,f.services(),f.cursor));
 assert(!resumePackedBattleMapLoad(7,0,f.services(),f.cursor));
 assert(resumePackedBattleMapLoad(7,0,f.services(),f.cursor));
 assert(std::get<2>(f.calls[f.calls.size()-2])[2]==0x1110);
 assert(std::get<2>(f.calls.back())[2]==0x2220);
}
void unavailableAndRetirement(){
 Fixture f;auto s=f.services();s.call=nullptr;assert(!resumePackedBattleMapLoad(1,2,s,f.cursor));assert(f.calls.empty()&&f.memory.empty()&&f.cursor.pending()==Sync);
 s.call=f.services().call; // Provider table replacement is unavailable after capture.
 assert(!resumePackedBattleMapLoad(1,2,s,f.cursor));assert(f.calls.empty());
 Fixture retired;retired.hold=1;assert(!resumePackedBattleMapLoad(1,2,retired.services(),retired.cursor));assert(retired.calls.size()==1);retired.alive=false;
 const auto before=retired.memory;assert(!resumePackedBattleMapLoad(1,2,retired.services(),retired.cursor));assert(retired.memory==before);retired.alive=true;assert(!resumePackedBattleMapLoad(1,2,retired.services(),retired.cursor));
 Fixture missing;auto noMemory=missing.services();noMemory.memory.word=nullptr;assert(!resumePackedBattleMapLoad(1,2,noMemory,missing.cursor)&&!missing.cursor.state);
}
}
int main(){cases();capturedNames();unavailableAndRetirement();}
