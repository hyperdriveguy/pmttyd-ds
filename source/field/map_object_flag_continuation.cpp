#include "map_object_flag_continuation.hpp"
#include <vector>
namespace ttyd {
namespace {
constexpr uint32_t ConditionalMask=0x100,ConditionalWord=0x114,Child=0xe4,Sibling=0xe8;
enum class Step{Conditional,Flags,Store,Child,Sibling,Return};
struct Frame{uint32_t object=0,flags=0;bool siblings=false;Step step=Step::Conditional;};
bool same(const MapObjectFlagServices&a,const MapObjectFlagServices&b){return a.user==b.user&&a.current==b.current&&a.lifetime==b.lifetime&&!a.lifetime.owner_before(b.lifetime)&&!b.lifetime.owner_before(a.lifetime)&&a.memory.user==b.memory.user&&a.memory.word==b.memory.word&&a.memory.store==b.memory.store;}
}
struct MapObjectFlagCursor::State {
 MapObjectFlagServices owner;std::vector<Frame> frames;
 uint32_t mask=0,pending=0;bool on=false,invalid=false,invoking=false,done=false;
 bool current(){if(invalid||!owner.current(owner.user)){invalid=true;return false;}return true;}
};
uint32_t MapObjectFlagCursor::pending()const{return state?state->pending:0;}
bool MapObjectFlagCursor::complete()const{return state&&state->done&&!state->invalid;}
bool resumePackedMapObjectFlag(bool on,uint32_t object,uint32_t mask,int32_t siblings,const MapObjectFlagServices&s,MapObjectFlagCursor&c,unsigned budget){
 if(!c.state){if(!s.lifetime||!s.current||!s.memory.word||!s.memory.store)return false;c.state=std::make_shared<MapObjectFlagCursor::State>();c.state->owner=s;c.state->mask=mask;c.state->on=on;c.state->frames.push_back({object,0,siblings!=0,Step::Conditional});}
 const auto pin=c.state;auto&p=*pin;if(p.invoking||p.on!=on||!same(p.owner,s)||!p.current())return false;
 p.invoking=true;struct Guard{bool&v;~Guard(){v=false;}}guard{p.invoking};
 while(p.current()){
  if(p.frames.empty()){p.done=true;p.pending=0;return true;}
  auto&f=p.frames.back();
  // These source PCs identify the corresponding root transfer class. Compiler
  // unrolling has no external calls; native frames retain the same exact order.
  p.pending=p.on?0x8001f21c:0x8001ef08;
  if(!budget)return false;
  --budget;
  switch(f.step){
  case Step::Conditional:
   if(p.on&&(p.mask&ConditionalMask)){uint32_t v=0;if(!s.memory.word(s.memory.user,f.object+ConditionalWord,v))return false;f.step=v?Step::Flags:Step::Child;}
   else f.step=Step::Flags;
   break;
  case Step::Flags:if(!s.memory.word(s.memory.user,f.object,f.flags))return false;f.flags=p.on?(f.flags|p.mask):(f.flags&~p.mask);f.step=Step::Store;break;
  case Step::Store:if(!s.memory.store(s.memory.user,f.object,f.flags))return false;f.step=Step::Child;break;
  case Step::Child:{uint32_t child=0;if(!s.memory.word(s.memory.user,f.object+Child,child))return false;f.step=Step::Sibling;if(child)p.frames.push_back({child,0,true,Step::Conditional});break;}
  case Step::Sibling:
   if(f.siblings){uint32_t sibling=0;if(!s.memory.word(s.memory.user,f.object+Sibling,sibling))return false;f.step=Step::Return;if(sibling)p.frames.push_back({sibling,0,true,Step::Conditional});}
   else f.step=Step::Return;
   break;
  case Step::Return:p.frames.pop_back();break;
  }
 }
 return false;
}
}
