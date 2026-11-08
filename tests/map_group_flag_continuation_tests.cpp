#include "map_group_flag_continuation.hpp"
#include <cassert>
#include <map>
#include <tuple>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Work=0x81000000,Objects=0x82000000,Joints=0x83000000,Name=0x84000000;
constexpr uint32_t WorkGlobal=0x8041e660,Active=0x8041e664,Pending=0x8041e670,Index=0x8041e674,Queue=0x803c97b0;
constexpr uint32_t Compare=0x8026d464,Copy=0x8026d5b8,On=0x8001f1fc,Off=0x8001eee8;
struct Event{char kind;uint32_t a,b,c,d,e;bool operator==(const Event&q)const{return std::tie(kind,a,b,c,d,e)==std::tie(q.kind,q.a,q.b,q.c,q.d,q.e);}};
struct Fixture {
 std::map<uint32_t,uint32_t> words;
 std::vector<Event> events;
 std::shared_ptr<int> life=std::make_shared<int>(0);
 bool alive=true,mutate=false,on=false,recursive=false;
 unsigned hold=UINT32_MAX,waits=0;
 MapGroupFlagCursor cursor;
 Fixture(bool enabled,int match,uint32_t index):on(enabled){
  words[Active]=1;words[WorkGlobal]=Work;words[Pending]=0;words[Index]=index;const auto group=Work+0x2f4;words[group]=2;
  for(unsigned entry=0;entry<2;++entry){const auto e=group+entry*0x178,objects=Objects+entry*0x1000;words[e+0x150]=2;words[e+0x154]=objects;
   for(unsigned object=0;object<2;++object){const auto n=entry*2+object;const auto j=Joints+16*n;words[objects+object*0x134+8]=j;words[j]=int(n)==match?Name:Name+16+n;}}
  words[Queue+index*0x24+0x20]=0xa5a5a5a5;
 }
 bool admit(){if(events.size()==hold&&waits++<2)return false;return true;}
 uint32_t word(uint32_t a){assert(words.count(a));const auto v=words.at(a);events.push_back({'r',a,v,0,0,0});return v;}
 void store(uint32_t a,uint32_t v){assert(words.count(a));words[a]=v;events.push_back({'w',a,v,0,0,0});}
 void effect(MapGroupFlagRequest&q){
  const auto&a=q.source.arguments;assert(q.count==(q.source.function==Compare?2u:3u));events.push_back({'c',q.source.function,q.callsite,a[0],a[1],a[2]});
  if(q.source.function==Compare){q.source.result=uint32_t(a[0]!=a[1]);if(mutate){store(WorkGlobal,0xdeadbeef);if(*q.source.result)store(Work+0x2f4+0x150,0);}}
  if(mutate&&q.source.function==Copy){store(Index,31);}
 }
 MapGroupFlagServices services(){MapGroupFlagServices s;s.user=this;s.lifetime=life;s.current=[](void*p){return static_cast<Fixture*>(p)->alive;};s.memory.user=this;
  s.memory.word=[](void*p,uint32_t a,uint32_t&v){auto&f=*static_cast<Fixture*>(p);if(!f.admit())return false;v=f.word(a);return true;};
  s.memory.store=[](void*p,uint32_t a,uint32_t v){auto&f=*static_cast<Fixture*>(p);if(!f.admit())return false;f.store(a,v);return true;};
  s.call=[](void*p,MapGroupFlagRequest&q){auto&f=*static_cast<Fixture*>(p);if(f.recursive){f.recursive=false;assert(!resumePackedMapGroupFlag(f.on,Name,3,f.services(),f.cursor));}if(!f.admit())return false;f.effect(q);return true;};return s;
 }
 void reference(uint32_t mask){
  const auto root=on?0x8001edd0u:0x8001ecb8u;
  auto call=[&](uint32_t off,uint32_t fn,std::array<uint32_t,3>a){MapGroupFlagRequest q;q.callsite=root+off;q.count=fn==Compare?2:3;q.source.function=fn;for(unsigned i=0;i<3;++i)q.source.arguments[i]=a[i];effect(q);return q.source.result.value_or(0);};
  const auto active=word(Active),work=word(WorkGlobal)+active*0x2f4;uint32_t found=0,entry=work,outer=0;
  while(int32_t(outer)<int32_t(word(work))){uint32_t object=word(entry+0x154),inner=0;
   while(int32_t(inner)<int32_t(word(entry+0x150))){const auto joint=word(object+8),name=word(joint);if(!call(0x58,Compare,{name,Name,0})){found=object;break;}++inner;object+=0x134;}
   if(found)break;
   ++outer;entry+=0x178;
  }
  if(found){call(0x100,on?On:Off,{found,mask,0});return;}
  const auto record=Queue+word(Index)*0x24;store(record+0x20,0);call(0xc0,Copy,{record,Name,31});
  const auto index=word(Index);store(Pending,1);const auto next=index+1;store(Index,next);
  const auto sign=next>>31,shifted=(next<<27)-sign;store(Index,((shifted<<5)|(shifted>>27))+sign);
 }
};
void differential(){
 for(bool on:{false,true})for(int match:{-1,0,1,3})for(uint32_t index:{0u,31u,0xfffffffeu})for(bool mutate:{false,true}){
  Fixture reference(on,match,index);reference.mutate=mutate;reference.reference(0x40000002);
  for(unsigned hold=0;hold<=reference.events.size();++hold){Fixture f(on,match,index);f.mutate=mutate;f.hold=hold;f.recursive=true;
   for(unsigned retry=0;!resumePackedMapGroupFlag(on,Name,0x40000002,f.services(),f.cursor);++retry)assert(retry<3);
   assert(f.cursor.complete()&&!f.cursor.pending());assert(f.events==reference.events&&f.words==reference.words);
   const auto count=f.events.size();assert(resumePackedMapGroupFlag(on,7,8,f.services(),f.cursor)&&f.events.size()==count);
  }
 }
}
void signedCounts(){
 for(bool on:{false,true})for(bool outer:{false,true}){Fixture reference(on,-1,0);if(outer)reference.words[Work+0x2f4]=UINT32_MAX;else reference.words[Work+0x2f4+0x150]=UINT32_MAX;reference.reference(2);
  Fixture f(on,-1,0);if(outer)f.words[Work+0x2f4]=UINT32_MAX;else f.words[Work+0x2f4+0x150]=UINT32_MAX;assert(resumePackedMapGroupFlag(on,Name,2,f.services(),f.cursor));assert(f.events==reference.events&&f.words==reference.words);}
}
void nullAndRetired(){
 Fixture f(true,-1,0);auto s=f.services();s.call=[](void*,MapGroupFlagRequest&q){assert(q.source.function==Copy&&q.source.arguments[1]==0);return false;};
 assert(!resumePackedMapGroupFlag(true,0,7,s,f.cursor));assert(f.events.size()==4&&f.cursor.pending()==Copy); // group/base/index read and source queue flag clear; no fabricated null copy.
 auto copy=f.words;assert(!resumePackedMapGroupFlag(true,0,7,s,f.cursor)&&f.words==copy);
 Fixture lost(false,0,0);lost.hold=3;assert(!resumePackedMapGroupFlag(false,Name,1,lost.services(),lost.cursor));lost.alive=false;const auto before=lost.events;assert(!resumePackedMapGroupFlag(false,Name,1,lost.services(),lost.cursor));assert(lost.events==before);lost.alive=true;assert(!resumePackedMapGroupFlag(false,Name,1,lost.services(),lost.cursor));
 Fixture invalid(true,0,0);auto noWord=invalid.services();noWord.memory.word=nullptr;assert(!resumePackedMapGroupFlag(true,Name,1,noWord,invalid.cursor)&&!invalid.cursor.state);
}
}
int main(){differential();signedCounts();nullAndRetired();}
