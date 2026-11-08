#include "map_group_flag_continuation.hpp"
namespace ttyd {
namespace {
constexpr uint32_t Work=0x8041e660,Active=0x8041e664,QueuePending=0x8041e670,QueueIndex=0x8041e674,Queue=0x803c97b0;
constexpr uint32_t GroupBytes=0x2f4,EntryBytes=0x178,ObjectBytes=0x134,QueueStride=0x24;
constexpr uint32_t Compare=0x8026d464,CopyBounded=0x8026d5b8,FlagOff=0x8001eee8,FlagOn=0x8001f1fc;
int32_t signedWord(uint32_t v){return v<0x80000000u?int32_t(v):int32_t(int64_t(v)-0x100000000ll);}
enum class Step{Group,Work,Outer,Array,Inner,Joint,Name,Compare,NextObject,NextEntry,QueueIndex,QueueClear,QueueCopy,QueueReload,QueuePending,QueueAdvance,QueueWrap,Apply,Done};
bool same(const MapGroupFlagServices&a,const MapGroupFlagServices&b){return a.user==b.user&&a.current==b.current&&a.call==b.call&&a.lifetime==b.lifetime&&!a.lifetime.owner_before(b.lifetime)&&!b.lifetime.owner_before(a.lifetime)&&a.memory.user==b.memory.user&&a.memory.word==b.memory.word&&a.memory.store==b.memory.store;}
}
struct MapGroupFlagCursor::State {
 MapGroupFlagServices owner;Step step=Step::Group;bool on=false,invalid=false,invoking=false;
 uint32_t name=0,mask=0,group=0,work=0,entry=0,object=0,outer=0,inner=0,joint=0,objectName=0,index=0,queueRecord=0,nextIndex=0,pending=0;
 std::optional<MapGroupFlagRequest> held;
 uint32_t pc(uint32_t off)const{return (on?0x8001edd0u:0x8001ecb8u)+off;}
 bool current(){if(invalid||!owner.current(owner.user)){invalid=true;return false;}return true;}
 bool read(uint32_t address,uint32_t&v,uint32_t off){pending=pc(off);return owner.memory.word(owner.memory.user,address,v);}
 bool store(uint32_t address,uint32_t v,uint32_t off){pending=pc(off);return owner.memory.store(owner.memory.user,address,v);}
 bool call(uint32_t off,uint32_t function,std::array<uint32_t,3>args,bool result=false){
  pending=function;if(!held){MapGroupFlagRequest q;q.callsite=pc(off);q.count=function==Compare?2:3;q.source.function=function;for(unsigned i=0;i<3;++i)q.source.arguments[i]=args[i];held=std::move(q);}
  return owner.call&&owner.call(owner.user,*held)&&current()&&(!result||held->source.result.has_value());
 }
 void accepted(){held.reset();pending=0;}
};
uint32_t MapGroupFlagCursor::pending()const{return state?state->pending:0;}
bool MapGroupFlagCursor::complete()const{return state&&state->step==Step::Done&&!state->invalid;}
bool resumePackedMapGroupFlag(bool on,uint32_t name,uint32_t mask,const MapGroupFlagServices&s,MapGroupFlagCursor&c){
 if(!c.state){if(!s.lifetime||!s.current||!s.memory.word||!s.memory.store)return false;c.state=std::make_shared<MapGroupFlagCursor::State>();c.state->owner=s;c.state->on=on;c.state->name=name;c.state->mask=mask;}
 auto&p=*c.state;if(p.invoking||!same(p.owner,s)||p.on!=on||!p.current())return false;
 p.invoking=true;struct Guard{bool&v;~Guard(){v=false;}}guard{p.invoking};
 while(p.current())switch(p.step){
 case Step::Group:if(!p.read(Active,p.group,0x18))return false;p.step=Step::Work;break;
 case Step::Work:if(!p.read(Work,p.work,0x1c))return false;p.work+=p.group*GroupBytes;p.entry=p.work;p.step=p.name?Step::Outer:Step::QueueIndex;break;
 case Step::Outer:{uint32_t n=0;if(!p.read(p.work,n,0x84))return false;p.step=signedWord(p.outer)<signedWord(n)?Step::Array:Step::QueueIndex;break;}
 case Step::Array:if(!p.read(p.entry+0x154,p.object,0x40))return false;p.inner=0;p.step=Step::Inner;break;
 case Step::Inner:{uint32_t n=0;if(!p.read(p.entry+0x150,n,0x70))return false;p.step=signedWord(p.inner)<signedWord(n)?Step::Joint:Step::NextEntry;break;}
 case Step::Joint:if(!p.read(p.object+8,p.joint,0x4c))return false;p.step=Step::Name;break;
 case Step::Name:if(!p.read(p.joint,p.objectName,0x54))return false;p.step=Step::Compare;break;
 case Step::Compare:
  if(!p.call(0x58,Compare,{p.objectName,p.name,0},true))return false;
  p.step=*p.held->source.result?Step::NextObject:Step::Apply;p.accepted();break;
 case Step::NextObject:++p.inner;p.object+=ObjectBytes;p.step=Step::Inner;break;
 case Step::NextEntry:++p.outer;p.entry+=EntryBytes;p.step=Step::Outer;break;
 case Step::QueueIndex:if(!p.read(QueueIndex,p.index,0x9c))return false;p.queueRecord=Queue+p.index*QueueStride;p.step=Step::QueueClear;break;
 case Step::QueueClear:if(!p.store(p.queueRecord+0x20,0,0xbc))return false;p.step=Step::QueueCopy;break;
 case Step::QueueCopy:if(!p.call(0xc0,CopyBounded,{p.queueRecord,p.name,31}))return false;p.accepted();p.step=Step::QueueReload;break;
 case Step::QueueReload:if(!p.read(QueueIndex,p.index,0xc4))return false;p.nextIndex=p.index+1;p.step=Step::QueuePending;break;
 case Step::QueuePending:if(!p.store(QueuePending,1,0xcc))return false;p.step=Step::QueueAdvance;break;
 case Step::QueueAdvance:if(!p.store(QueueIndex,p.nextIndex,0xd8))return false;p.step=Step::QueueWrap;break;
 case Step::QueueWrap:{const auto sign=p.nextIndex>>31;const auto shifted=(p.nextIndex<<27)-sign;const auto wrapped=((shifted<<5)|(shifted>>27))+sign;if(!p.store(QueueIndex,wrapped,0xec))return false;p.step=Step::Done;break;}
 case Step::Apply:if(!p.call(0x100,p.on?FlagOn:FlagOff,{p.object,p.mask,0}))return false;p.accepted();p.step=Step::Done;break;
 case Step::Done:p.pending=0;return true;
 }
 return false;
}
}
