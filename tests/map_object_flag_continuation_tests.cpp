#include "map_object_flag_continuation.hpp"
#include "map_group_flag_continuation.hpp"
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <map>
#include <tuple>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Root=0x81000000,Stride=0x200,StackLow=0x87000000,StackTop=0x87010000,StackHigh=0x87020000;
struct Event{bool write;uint32_t address,value;bool operator==(const Event&q)const{return write==q.write&&address==q.address&&value==q.value;}};
struct Fixture{
 std::map<uint32_t,uint32_t> memory;std::vector<Event> events;
 std::shared_ptr<int>life=std::make_shared<int>(0);MapObjectFlagCursor cursor;
 bool alive=true,reenter=false,on=false;uint32_t mask=0;unsigned hold=UINT32_MAX,waits=0;
 Fixture(){for(unsigned i=0;i<20;++i){const auto a=Root+i*Stride;memory[a]=0xa5a5a5a5;memory[a+0x114]=i%3;memory[a+0xe4]=0;memory[a+0xe8]=0;}
  // Long child/sibling paths cross every source hand-unrolling boundary.
  for(unsigned i=0;i<9;++i)memory[Root+i*Stride+0xe4]=Root+(i+1)*Stride;
  for(unsigned i=0;i<8;++i)memory[Root+i*Stride+0xe8]=Root+(i+11)*Stride;
 }
 bool admit(){if(events.size()==hold&&waits++<2)return false;return true;}
 uint32_t word(uint32_t a){assert(memory.count(a));const auto v=memory.at(a);if(a<StackLow||a>=StackHigh)events.push_back({false,a,v});return v;}
 void store(uint32_t a,uint32_t v){if(a<StackLow||a>=StackHigh){assert(memory.count(a));events.push_back({true,a,v});}memory[a]=v;}
 MapObjectFlagServices services(){MapObjectFlagServices s;s.user=this;s.lifetime=life;s.current=[](void*p){return static_cast<Fixture*>(p)->alive;};s.memory.user=this;
  s.memory.word=[](void*p,uint32_t a,uint32_t&v){auto&f=*static_cast<Fixture*>(p);if(f.reenter){f.reenter=false;assert(!resumePackedMapObjectFlag(f.on,Root,f.mask,0,f.services(),f.cursor));}if(!f.admit())return false;v=f.word(a);return true;};
  s.memory.store=[](void*p,uint32_t a,uint32_t v){auto&f=*static_cast<Fixture*>(p);if(!f.admit())return false;f.store(a,v);return true;};return s;
 }
};
uint32_t be(const unsigned char*p){return uint32_t(p[0])<<24|uint32_t(p[1])<<16|uint32_t(p[2])<<8|p[3];}
std::vector<uint32_t> original(bool on){
 const char*iso=std::getenv("TTYD_ISO");if(!iso)iso="/home/carson/games/roms/G8ME01.iso";FILE*f=std::fopen(iso,"rb");assert(f);unsigned char buffer[256];assert(std::fseek(f,0x420,SEEK_SET)==0&&std::fread(buffer,1,4,f)==4);const auto dol=be(buffer);assert(std::fseek(f,dol,SEEK_SET)==0&&std::fread(buffer,1,256,f)==256);
 const uint32_t address=on?0x8001f1fc:0x8001eee8,length=on?0x440:0x314;uint32_t file=0;
 for(unsigned i=0;i<18;++i){const auto start=be(buffer+0x48+4*i),size=be(buffer+0x90+4*i);if(start<=address&&address+length<=start+size){file=dol+be(buffer+4*i)+address-start;break;}}
 assert(file&&std::fseek(f,file,SEEK_SET)==0);std::vector<unsigned char>raw(length);assert(std::fread(raw.data(),1,length,f)==length);std::fclose(f);std::vector<uint32_t>words;for(unsigned i=0;i<length;i+=4)words.push_back(be(raw.data()+i));return words;
}
// TEST ONLY instruction oracle over the pinned original two bodies. It never
// enters production code. Source-private register saves use isolated stack RAM;
// comparison retains every original MapObj load/store, including alias order.
void oracle(Fixture&f,bool on,uint32_t mask,int32_t follow){
 const auto code=original(on);const uint32_t base=on?0x8001f1fc:0x8001eee8;uint32_t pc=base,lr=0,registers[32]={};bool eq=false;
 registers[1]=StackTop;registers[3]=Root;registers[4]=mask;registers[5]=uint32_t(follow);
 auto&r=registers;
 for(unsigned visits=0;pc&&visits<1000000;++visits){assert(pc>=base&&(pc-base)/4<code.size());const auto w=code[(pc-base)/4],op=w>>26,rt=(w>>21)&31,ra=(w>>16)&31,rb=(w>>11)&31;const auto imm=int32_t(int16_t(w&65535));const auto old=pc;pc+=4;
  switch(op){
  case 14:r[rt]=(ra?r[ra]:0)+uint32_t(imm);break;
  case 32:r[rt]=f.word(r[ra]+uint32_t(imm));break;
  case 36:f.store(r[ra]+uint32_t(imm),r[rt]);break;
  case 37:{const auto a=r[ra]+uint32_t(imm);f.store(a,r[rt]);r[ra]=a;break;}
  case 46:for(unsigned i=rt;i<32;++i)r[i]=f.word(r[ra]+uint32_t(imm)+4*(i-rt));break;
  case 47:for(unsigned i=rt;i<32;++i)f.store(r[ra]+uint32_t(imm)+4*(i-rt),r[i]);break;
  case 10:case 11:eq=r[ra]==uint32_t(op==10?uint32_t(w&65535):uint32_t(imm));break;
  case 16:{const auto bo=(w>>21)&31,bi=(w>>16)&31;assert(bi==2&&(bo==4||bo==12));if(eq==(bo==12))pc=old+uint32_t(int32_t(int16_t(w&0xfffc)));break;}
  case 18:{int32_t d=int32_t(w&0x03fffffc);if(d&0x02000000)d-=0x04000000;if(w&1)lr=pc;pc=old+uint32_t(d);break;}
  case 19:assert(w==0x4e800020);pc=lr;break;
  case 21:{const auto shift=(w>>11)&31,begin=(w>>6)&31,end=(w>>1)&31;uint32_t m=0;for(unsigned b=0;b<32;++b)if(begin<=end?(b>=begin&&b<=end):(b>=begin||b<=end))m|=uint32_t(1)<<(31-b);const auto v=shift?(r[rt]<<shift)|(r[rt]>>(32-shift)):r[rt];r[ra]=v&m;if(w&1)eq=r[ra]==0;break;}
  case 31:{const auto xo=(w>>1)&1023;
   if(xo==339){assert(((w>>16)&31)==8);r[rt]=lr;}
   else if(xo==467){assert(((w>>16)&31)==8);lr=r[rt];}
   else if(xo==444)r[ra]=r[rt]|r[rb];
   else if(xo==28)r[ra]=r[rt]&r[rb];
   else if(xo==124)r[ra]=~(r[rt]|r[rb]);
   else assert(false);
   break;}
  default:assert(false);
  }
 }
 assert(pc==0);
}
void all(){
 for(bool on:{false,true})for(uint32_t mask:{0u,1u,0x100u,0x102u,0xffffffffu})for(int32_t siblings:{0,1,-1}){
  Fixture reference;oracle(reference,on,mask,siblings);
  for(unsigned hold=0;hold<=reference.events.size();++hold){Fixture f;f.on=on;f.mask=mask;f.hold=hold;f.reenter=true;unsigned retries=0;
   while(!resumePackedMapObjectFlag(on,Root,mask,siblings,f.services(),f.cursor,17))assert(++retries<100);
   assert(f.events==reference.events&&f.cursor.complete()&&!f.cursor.pending());
   for(const auto&[address,value]:f.memory)assert(reference.memory.at(address)==value);
  }
 }
}
void composedParent(){
 for(bool on:{false,true}){
  struct Context{Fixture f;MapGroupFlagCursor parent;unsigned compares=0;bool on=false;}c;c.on=on;c.f.on=on;c.f.mask=0x102;c.f.hold=10;
  constexpr uint32_t Work=0x85000000,Name=0x86000000,Joint=0x86000100;
  c.f.memory[0x8041e660]=Work;c.f.memory[0x8041e664]=1;c.f.memory[Work+0x2f4]=1;c.f.memory[Work+0x2f4+0x150]=1;c.f.memory[Work+0x2f4+0x154]=Root;c.f.memory[Root+8]=Joint;c.f.memory[Joint]=Name;
  MapGroupFlagServices s;s.user=&c;s.memory=c.f.services().memory;s.lifetime=c.f.life;s.current=[](void*p){return static_cast<Context*>(p)->f.alive;};
  s.call=[](void*p,MapGroupFlagRequest&q){auto&c=*static_cast<Context*>(p);if(q.source.function==0x8026d464){++c.compares;assert(q.source.arguments[0]==q.source.arguments[1]);q.source.result=0;return true;}
   assert(q.source.function==(c.on?0x8001f1fcu:0x8001eee8u));return resumePackedMapObjectFlag(c.on,q.source.arguments[0],q.source.arguments[1],int32_t(q.source.arguments[2]),c.f.services(),c.f.cursor);
  };
  unsigned attempts=0;while(!resumePackedMapGroupFlag(on,Name,0x102,s,c.parent))assert(++attempts<4);
  Fixture reference;oracle(reference,on,0x102,0);assert(c.compares==1&&c.f.cursor.complete()&&c.parent.complete());
  std::vector<Event> actual;for(const auto&e:c.f.events)if(e.address>=Root&&e.address<Root+20*Stride&&e.address!=Root+8)actual.push_back(e);
  assert(actual==reference.events);
 }
}
void retirement(){Fixture f;f.hold=2;assert(!resumePackedMapObjectFlag(false,Root,1,0,f.services(),f.cursor));const auto events=f.events;f.alive=false;assert(!resumePackedMapObjectFlag(false,Root,1,0,f.services(),f.cursor));assert(f.events==events);f.alive=true;assert(!resumePackedMapObjectFlag(false,Root,1,0,f.services(),f.cursor));}
}
int main(){all();composedParent();retirement();}
