#include "source_mario_set_pose.hpp"
#include <cassert>
#include <map>
#include <vector>
#include <string>
#include <iostream>
using namespace ttyd;
namespace {
constexpr uint32_t MP=0x8041e900,A=0x6000,E=0x8000,ARGS=0x9000,T=0x8032dd60;
struct World{
 std::shared_ptr<const void>life=std::make_shared<int>(1);std::map<uint32_t,uint32_t>w;std::vector<std::string>trace;size_t index=0,stall=~size_t(0);int waits=0;bool alive=true,late=false,reenter=false;uint32_t dotResult=0x1234;SourceMarioSetPose*active=nullptr;EventEntry event{};
 World(){event.id=7;event.currentCommand=0x9050664c;w={{MP,A},{E+0x18,ARGS},{ARGS,0x1234},{A,0},{A+4,0x10000000},{A+0xc,0},{A+0x2e,0},{T,0x1111},{T+4,0x1234},{T+8,0}};}
 bool admit(){if(reenter){reenter=false;assert(active);assert(!active->reset());assert(!active->resume(event,E,services()));}if(index==stall&&waits++<2)return false;++index;return true;}
 static bool current(void*p){return static_cast<World*>(p)->alive;}
 static std::optional<uint32_t>word(void*p,uint32_t a){auto&x=*static_cast<World*>(p);if(!x.admit())return {};if(!x.w.count(a))return {};auto v=x.w[a];x.trace.push_back("R"+std::to_string(a)+":"+std::to_string(v));return v;}
 static std::optional<uint16_t>half(void*p,uint32_t a){auto v=word(p,a);if(!v)return {};return uint16_t(*v);}
 static bool store(void*p,uint32_t a,uint32_t v){auto&x=*static_cast<World*>(p);if(!x.admit())return false;x.w[a]=v;x.trace.push_back("W"+std::to_string(a)+":"+std::to_string(v));if(x.late&&a==A+4)x.w[a]|=0x01000000;if(x.late&&a==A+0x18)x.w[A+0xc]=0x80;return true;}
 static std::optional<int32_t>value(void*p,EventEntry&,uint32_t v){auto&x=*static_cast<World*>(p);if(!x.admit())return {};x.trace.push_back("V"+std::to_string(v));return int32_t(v);}
 static std::optional<int32_t>compare(void*p,uint32_t a,uint32_t b){auto&x=*static_cast<World*>(p);if(!x.admit())return {};x.trace.push_back("C"+std::to_string(a)+":"+std::to_string(b));return a==b?0:1;}
 static std::optional<uint32_t>dot(void*p,uint32_t a){auto&x=*static_cast<World*>(p);if(!x.admit())return {};x.trace.push_back("D"+std::to_string(a));return x.dotResult;}
 SourceMarioPoseServices services(){return {life,this,current,word,half,store,value,compare,dot};}
};
void reference(World&w){auto args=*World::word(&w,E+0x18);auto p=*World::word(&w,MP);auto m=*World::half(&w,p+0x2e);if(m==24||m==25||m==22||m==21)return;auto pose=uint32_t(*World::value(&w,w.event,*World::word(&w,args)));auto main=*World::word(&w,p);bool dot=main&0x80000000,member=false;if(dot){pose=*World::dot(&w,pose);if(!pose)return;}else{for(uint32_t table=T;;table+=4){auto candidate=*World::word(&w,table);if(!candidate)break;if(*World::compare(&w,pose,candidate)==0){member=true;break;}}}World::store(&w,p+0x18,pose);auto dirty=*World::word(&w,p+0xc);World::store(&w,p+0xc,dirty|0x1000);if(!dot){auto f=*World::word(&w,p+4);World::store(&w,p+4,member?f&~0x10000000u:f|0x10000000u);}auto f=*World::word(&w,p+4);if(f&0x01000000){dirty=*World::word(&w,p+0xc);World::store(&w,p+0xc,dirty|0x8000);}}
void parity(){for(unsigned motion:{0u,21u,22u,24u,25u})for(uint32_t flags:{0u,0x80000000u})for(uint32_t requested:{0x1111u,0x1234u,0x4321u})for(uint32_t dot:{0u,0x2222u})for(bool late:{false,true}){World r;r.w[A+0x2e]=motion;r.w[A]=flags;r.w[ARGS]=requested;r.dotResult=dot;r.late=late;reference(r);for(size_t wait=0;wait<=r.index;++wait){World w;w.w[A+0x2e]=motion;w.w[A]=flags;w.w[ARGS]=requested;w.dotResult=dot;w.late=late;w.stall=wait;SourceMarioSetPose owner;w.active=&owner;w.reenter=true;std::optional<int32_t>v;unsigned tries=0;do{v=owner.resume(w.event,E,w.services());assert(++tries<10);}while(!v);assert(*v==2&&w.trace==r.trace&&w.w==r.w);auto count=w.index;assert(owner.resume(w.event,E,w.services())==2&&w.index==count);}}}
void guards(){World w;w.w[A+0x2e]=24;auto s=w.services();s.eventValue=nullptr;s.compare=nullptr;s.dotPose=nullptr;s.storeWord=nullptr;SourceMarioSetPose blocked;assert(blocked.resume(w.event,E,s)==2&&w.trace.size()==3);World h;auto hs=h.services();hs.compare=nullptr;SourceMarioSetPose c;assert(!c.resume(h.event,E,hs)&&c.pending()==0x8026d464);auto n=h.index;auto changed=hs;changed.word=nullptr;assert(!c.resume(h.event,E,changed)&&h.index==n);h.event.currentCommand+=4;assert(!c.resume(h.event,E,hs)&&h.index==n);h.event.currentCommand-=4;h.alive=false;assert(!c.resume(h.event,E,hs));h.alive=true;assert(!c.resume(h.event,E,hs)&&h.index==n&&!c.reset());}
}
int main(){parity();guards();std::cout<<"Whole source SetPose trace/every wait twice, motion early-return, actual table and Dot/null branches, fresh dirty/flags pass\n";return 0;}
