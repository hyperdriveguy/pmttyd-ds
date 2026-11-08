#include "source_map_material.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
float scalar(uint32_t bits){float value;std::memcpy(&value,&bits,sizeof(value));return value;}
uint32_t bits(float value){uint32_t out;std::memcpy(&out,&value,sizeof(out));return out;}
double real(uint64_t bits){double value;std::memcpy(&value,&bits,sizeof(value));return value;}
uint64_t realBits(double value){uint64_t out;std::memcpy(&out,&value,sizeof(out));return out;}
int32_t signedWord(uint32_t v){return v<=INT32_MAX?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
uint32_t signHalf(uint32_t v){return v&0x8000?v|0xffff0000u:v&0xffff;}
uint32_t rotate(uint32_t v,unsigned n){return n?(v<<n)|(v>>(32-n)):v;}
void compare(MapMaterialSourceCursor& c,unsigned field,int64_t a,int64_t b){const auto i=4*field;c.condition[i]=a<b;c.condition[i+1]=a>b;c.condition[i+2]=a==b;c.condition[i+3]=false;}
bool compatible(const MapMaterialSourceServices& a,const MapMaterialSourceServices& b){
 const auto& x=a.memory;const auto& y=b.memory;
 return x.user==y.user && x.word==y.word && x.byte==y.byte && x.half==y.half && x.store==y.store && x.storeByte==y.storeByte && x.storeHalf==y.storeHalf && a.call==b.call;
}
bool root(uint32_t value){switch(value){
 case 0x80020e54:case 0x8002493c:case 0x8001ba60:case 0x8001bcf8:
 case 0x8002b430:case 0x8001bad0:case 0x8001bccc:case 0x800064fc:case 0x80006680:
 case 0x8000f2e4:case 0x80037b5c:return true;
 default:return false;
}}
}
std::optional<uint32_t> resumeMapMaterialSource(uint32_t function,const std::array<uint32_t,5>& args,uint32_t stackTop,const MapMaterialSourceServices& services,MapMaterialSourceCursor& c,unsigned visits){
 if(c.active && (c.function!=function || c.arguments!=args || c.stackTop!=stackTop || !compatible(c.owner,services)))return {};
 if(!c.active){if(!root(function)||!stackTop)return {};c={};c.active=true;c.function=c.pc=function;c.arguments=args;c.stackTop=stackTop;c.registers[1]=stackTop;c.registers[2]=FieldMapSource::SourceR2;c.registers[13]=FieldMapSource::SourceR13;for(unsigned i=0;i<args.size();++i)c.registers[3+i]=args[i];}
 c.visitsConsumed=0;c.owner=services;
 const auto& s=services.memory;auto& r=c.registers;auto& f=c.floats;
 auto word=[&](uint32_t a,uint32_t& value){uint32_t raw=0;if(!s.word||!s.word(s.user,a,raw))return false;value=raw;return true;};
 auto byte=[&](uint32_t a,uint32_t& value){uint8_t raw=0;if(!s.byte||!s.byte(s.user,a,raw))return false;value=raw;return true;};
 auto half=[&](uint32_t a,uint32_t& value){uint16_t raw=0;if(!s.half||!s.half(s.user,a,raw))return false;value=raw;return true;};
 auto store=[&](uint32_t a,uint32_t value){return s.store&&s.store(s.user,a,value);};
 auto external=[&](uint32_t target,uint32_t next){
  if(!c.queued){c.request={};c.request.function=target;for(unsigned i=0;i<c.request.arguments.size();++i)c.request.arguments[i]=r[3+i];for(unsigned i=0;i<c.request.reals.size();++i)c.request.reals[i]=f[1+i];c.lr=next;c.queued=true;}
  if(!services.call||!services.call(s.user,c.request))return false;
  if(c.request.result)r[3]=*c.request.result;
  if(c.request.realResult)f[1]=*c.request.realResult;
  c.queued=false;c.pc=next;return true;
 };
 while(visits--){++c.visitsConsumed;
  if(!c.pc){const auto result=r[3];c={};return result;}
  switch(c.pc){
#include "source_map_material_program.inc"
  default:return {};
  }
 }
 return {};
}
}
