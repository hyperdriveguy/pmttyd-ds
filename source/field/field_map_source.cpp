#include "field_map_source.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
float scalar(uint32_t bits){float f;std::memcpy(&f,&bits,sizeof(f));return f;}
uint32_t bits(float f){uint32_t out;std::memcpy(&out,&f,sizeof(out));return out;}
int32_t signedWord(uint32_t v){return v<=INT32_MAX?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
void compare(FieldMapSourceCursor& c,unsigned field,int64_t a,int64_t b){const auto i=4*field;c.condition[i]=a<b;c.condition[i+1]=a>b;c.condition[i+2]=a==b;c.condition[i+3]=false;}
uint32_t rotate(uint32_t v,unsigned n){return n?(v<<n)|(v>>(32-n)):v;}
bool compatible(const FieldMapSourceServices& a,const FieldMapSourceServices& b){
#define PIN(f) (!a.f || a.f==b.f)
 return a.user==b.user && PIN(word) && PIN(byte) && PIN(store) && PIN(storeHalf) && PIN(storeByte) && PIN(call);
#undef PIN
}
bool root(uint32_t f){using namespace FieldMapSource;return f==Init || f==Find || f==Entry || f==EntrySub || f==Object || f==Count || f==CountSub || f==Recalculate;}
}
std::optional<uint32_t> resumeFieldMapSource(uint32_t function,const std::array<uint32_t,5>& args,uint32_t stackTop,const FieldMapSourceServices& s,FieldMapSourceCursor& c,unsigned visits){
 if(c.active && (c.function!=function || !compatible(c.owner,s)))return {};
 if(!c.active){if(!root(function) || !stackTop)return {};c={};c.active=true;c.function=c.pc=function;c.registers[1]=stackTop;c.registers[2]=FieldMapSource::SourceR2;c.registers[13]=FieldMapSource::SourceR13;for(unsigned i=0;i<args.size();++i)c.registers[3+i]=args[i];}
 c.owner=s;auto& r=c.registers;auto& f=c.floats;
 auto word=[&](uint32_t a,uint32_t& v){uint32_t raw=0;if(!s.word || !s.word(s.user,a,raw))return false;v=raw;return true;};
 auto byte=[&](uint32_t a,uint32_t& v){uint8_t raw=0;if(!s.byte || !s.byte(s.user,a,raw))return false;v=raw;return true;};
 auto store=[&](uint32_t a,uint32_t v){return s.store && s.store(s.user,a,v);};
 auto external=[&](uint32_t target,uint32_t next){
  if(!c.queued){c.request={};c.request.function=target;for(unsigned i=0;i<c.request.arguments.size();++i)c.request.arguments[i]=r[3+i];for(unsigned i=0;i<c.request.scalars.size();++i)c.request.scalars[i]=f[1+i];c.lr=next;c.queued=true;}
  if(!s.call || !s.call(s.user,c.request))return false;
  if(c.request.result)r[3]=*c.request.result;
  if(c.request.floatingResult)f[1]=*c.request.floatingResult;
  c.queued=false;c.pc=next;return true;
 };
 while(visits--){
  if(!c.pc){const auto result=r[3];c={};return result;}
  switch(c.pc){
#include "field_map_source_program.inc"
  default:return {};
  }
 }
 return {};
}
}
