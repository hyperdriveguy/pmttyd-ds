#include "source_mario_display_flags.hpp"
#include <cassert>
#include <map>
#include <vector>
#include <string>
#include <iostream>
using namespace ttyd;
namespace {
constexpr uint32_t A=0x6000,B=0x7000,E=0x8000,ARGS=0x9000,MP=0x8041e900;
struct World{
 std::shared_ptr<const void>life=std::make_shared<int>(1);std::map<uint32_t,uint32_t>w{{MP,A},{E+0x18,ARGS},{ARGS,1},{ARGS+4,2},{A+4,0x80},{B+4,0x40}};std::vector<std::string>trace;size_t index=0,stall=~size_t(0);int waits=0;bool alive=true,late=false,reenter=false;SourceMarioDisplayFlags*active=nullptr;EventEntry event{};
 World(){event.id=12;event.currentCommand=0x9050672c;}
 bool admit(){if(reenter){reenter=false;assert(active);assert(!active->reset());assert(!active->resume(event,E,services()));}if(index==stall&&waits++<2)return false;++index;return true;}
 static bool current(void*p){return static_cast<World*>(p)->alive;}
 static std::optional<uint32_t>word(void*p,uint32_t a){auto&x=*static_cast<World*>(p);if(!x.admit())return {};if(!x.w.count(a))return {};auto v=x.w[a];x.trace.push_back("R"+std::to_string(a)+":"+std::to_string(v));return v;}
 static bool store(void*p,uint32_t a,uint32_t v){auto&x=*static_cast<World*>(p);if(!x.admit())return false;x.trace.push_back("W"+std::to_string(a)+":"+std::to_string(v));x.w[a]=v;return true;}
 static std::optional<int32_t>value(void*p,EventEntry&,uint32_t v){auto&x=*static_cast<World*>(p);if(!x.admit())return {};x.trace.push_back("V"+std::to_string(v));if(x.late){x.w[MP]=B;x.w[A+4]=0x8000;}return int32_t(v);}
 SourceMarioDisplayFlagServices services(){return {life,this,current,word,store,value};}
};
void reference(World&w){auto args=*World::word(&w,E+0x18);auto a=*World::word(&w,MP);auto on=*World::value(&w,w.event,*World::word(&w,args));auto mask=uint32_t(*World::value(&w,w.event,*World::word(&w,args+4)));auto flags=*World::word(&w,a+4);World::store(&w,a+4,on?flags|mask:flags&~mask);}
void parity(){for(int on:{0,1,-1,2})for(uint32_t mask:{0u,2u,0xffffffffu,0x80000000u})for(bool late:{false,true}){World r;r.w[ARGS]=uint32_t(on);r.w[ARGS+4]=mask;r.late=late;reference(r);for(size_t wait=0;wait<=r.index;++wait){World w;w.w[ARGS]=uint32_t(on);w.w[ARGS+4]=mask;w.late=late;w.stall=wait;SourceMarioDisplayFlags owner;w.active=&owner;w.reenter=true;unsigned tries=0;while(!owner.resume(w.event,E,w.services()))assert(++tries<10);assert(w.trace==r.trace&&w.w==r.w);auto count=w.index;assert(owner.resume(w.event,E,w.services())==2&&w.index==count);assert(owner.reset());}}}
void guards(){World w;auto s=w.services();s.eventValue=nullptr;SourceMarioDisplayFlags c;assert(!c.resume(w.event,E,s)&&c.pending()==0x80031260);auto n=w.index;auto changed=s;changed.word=nullptr;assert(!c.resume(w.event,E,changed)&&w.index==n);w.event.id+=1;assert(!c.resume(w.event,E,s)&&w.index==n);w.event.id-=1;w.alive=false;assert(!c.resume(w.event,E,s));w.alive=true;assert(!c.resume(w.event,E,s)&&w.index==n&&!c.reset());}
}
int main(){parity();guards();std::cout<<"Whole source Mario displayflags traces, nonzero truth/masks, every wait twice, captured actor/fresh flags and identity guards pass\n";return 0;}
