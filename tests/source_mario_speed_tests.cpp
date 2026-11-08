#include "source_mario_speed.hpp"
#include <cassert>
#include <cstring>
#include <map>
#include <vector>
#include <string>
#include <iostream>
using namespace ttyd;
namespace {
uint32_t bits(float f){uint32_t w;std::memcpy(&w,&f,4);return w;}float scalar(uint32_t w){float f;std::memcpy(&f,&w,4);return f;}
constexpr uint32_t MP=0x8041e900,A=0x6000,B=0x7000,D=0x80087ce4,W=0x80087d84;
struct World{
 std::shared_ptr<const void>life=std::make_shared<int>(1);std::map<uint32_t,uint32_t>words;std::map<uint32_t,uint8_t>bytes;std::vector<std::string>trace;size_t index=0,stall=~size_t(0);int waits=0;bool alive=true,late=false,reenter=false;SourceMarioSpeed*active=nullptr;
 World(){words={{MP,A},{A,0},{B,0x02000000},{A+0x184,bits(3)},{A+0x188,bits(6)},{A+0x228,bits(1.5)},{0x8042088c,bits(.5)}};bytes={{A+0x252,0},{A+0x253,0}};}
 bool admit(){if(reenter){reenter=false;assert(active);assert(!active->reset());assert(!active->resume(D,services()));}if(index==stall&&waits++<2)return false;++index;return true;}
 static bool current(void*p){return static_cast<World*>(p)->alive;}
 static std::optional<uint32_t>word(void*p,uint32_t a){auto&x=*static_cast<World*>(p);if(!x.admit())return {};if(!x.words.count(a))return {};auto v=x.words[a];x.trace.push_back("R"+std::to_string(a)+":"+std::to_string(v));if(x.late&&(a==A+0x184||a==A+0x188))x.words[MP]=B;return v;}
 static std::optional<uint8_t>byte(void*p,uint32_t a){auto&x=*static_cast<World*>(p);if(!x.admit())return {};if(!x.bytes.count(a))return {};auto v=x.bytes[a];x.trace.push_back("B"+std::to_string(a)+":"+std::to_string(v));return v;}
 static std::optional<float>multiply(void*p,uint32_t pc,float a,float b){auto&x=*static_cast<World*>(p);if(!x.admit())return {};x.trace.push_back("M"+std::to_string(pc)+":"+std::to_string(bits(a))+":"+std::to_string(bits(b)));if(x.late)x.words[A+0x228]=bits(2);return a*b;}
 SourceMarioSpeedServices services(){return {life,this,current,word,byte,multiply};}
};
float reference(World&w,uint32_t fn){auto a=*World::word(&w,MP);auto flags=*World::word(&w,a);auto base=scalar(*World::word(&w,a+(fn==D?0x188:0x184)));bool half;if(flags&0x00100000){auto sx=*World::byte(&w,a+0x252);auto sy=*World::byte(&w,a+0x253);if(fn==D)base=scalar(*World::word(&w,a+0x184));int x=sx<128?sx:int(sx)-256,y=sy<128?sy:int(sy)-256;half=x*x+y*y<=3025;}else{auto other=*World::word(&w,MP);half=(*World::word(&w,other)&0x02000000)!=0;}if(half){auto factor=scalar(*World::word(&w,0x8042088c));base=*World::multiply(&w,fn==D?(flags&0x00100000?0x80087d40:0x80087d58):(flags&0x00100000?0x80087ddc:0x80087df4),base,factor);}auto factor=scalar(*World::word(&w,a+0x228));return *World::multiply(&w,fn==D?0x80087d60:0x80087dfc,base,factor);}
void parity(){for(auto fn:{D,W})for(uint32_t flags:{0u,0x00100000u,0x02000000u,0x02100000u})for(int x:{-128,-55,0,55,56,127})for(int y:{0,1,-55})for(bool late:{false,true}){World r;r.words[A]=flags;r.bytes[A+0x252]=uint8_t(x);r.bytes[A+0x253]=uint8_t(y);r.late=late;auto expected=reference(r,fn);for(size_t wait=0;wait<=r.index;++wait){World w;w.words[A]=flags;w.bytes[A+0x252]=uint8_t(x);w.bytes[A+0x253]=uint8_t(y);w.late=late;w.stall=wait;SourceMarioSpeed c;w.active=&c;w.reenter=true;std::optional<float>v;unsigned tries=0;do{v=c.resume(fn,w.services());assert(++tries<10);}while(!v);assert(bits(*v)==bits(expected)&&w.trace==r.trace);auto count=w.index;assert(c.resume(fn,w.services())==v&&w.index==count);assert(c.reset());}}}
void guards(){World w;auto s=w.services();s.byte=nullptr;SourceMarioSpeed ordinary;assert(ordinary.resume(D,s)==9);World h;h.words[A]=0x00100000;auto hs=h.services();hs.byte=nullptr;SourceMarioSpeed hold;assert(!hold.resume(D,hs)&&hold.pending()==A+0x252);auto n=h.index;auto changed=hs;changed.multiply=nullptr;assert(!hold.resume(D,changed)&&h.index==n);assert(!hold.resume(W,hs)&&h.index==n);h.alive=false;assert(!hold.resume(D,hs));h.alive=true;assert(!hold.resume(D,hs)&&h.index==n&&!hold.reset());}
}
int main(){parity();guards();std::cout<<"Whole Dash/Walk/Bgmode source trace, signed-stick threshold, fresh pointer/scale, every wait twice and identities pass\n";return 0;}
