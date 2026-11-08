#include "field_map_source.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
float scalar(uint32_t bits){float f;std::memcpy(&f,&bits,sizeof(f));return f;}
uint32_t bits(float f){uint32_t out;std::memcpy(&out,&f,sizeof(out));return out;}
int32_t signedWord(uint32_t v){return v<=INT32_MAX?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
void compare(FieldMapSourceCursor& c,unsigned field,int64_t a,int64_t b){const auto i=4*field;c.condition[i]=a<b;c.condition[i+1]=a>b;c.condition[i+2]=a==b;c.condition[i+3]=false;}
uint64_t doubleBits(double value){uint64_t out;std::memcpy(&out,&value,sizeof(out));return out;}
double doubleScalar(uint64_t value){double out;std::memcpy(&out,&value,sizeof(out));return out;}
uint32_t signHalf(uint32_t value){return value&0x8000?value|0xffff0000u:value&0xffff;}
unsigned leadingZeros(uint32_t value){unsigned count=0;for(uint32_t bit=0x80000000u;bit && !(value&bit);bit>>=1)++count;return count;}
uint32_t rotate(uint32_t v,unsigned n){return n?(v<<n)|(v>>(32-n)):v;}
bool compatible(const FieldMapSourceServices& a,const FieldMapSourceServices& b){
#define PIN(f) (!a.f || a.f==b.f)
 return a.user==b.user && PIN(word) && PIN(byte) && PIN(half) && PIN(store) && PIN(storeHalf) && PIN(storeByte) && PIN(call);
#undef PIN
}
bool root(uint32_t f){switch(f){
 case 0x802b708c: // GXGetTexBufferSize
 case 0x8002dcec: // shadowInit
 case 0x8002bc7c: // shadowGetCharShadowProjection
 case 0x8000f36c: // camLoadRoad
 case 0x8000e104: // camRoadSetup
 case 0x8000f310: // camUnLoadRoad
 case 0x8000e8c0: // camSetTypePersp
 case 0x800f3978: // camShiftInit
 case 0x8002b464: // mapGetDvdRoot
 case 0x80018658: // hitEntry
 case 0x800186f4: // hitEntrySub
 case 0x8001896c: // _hitEnt
 case 0x800165a8: // hitCalcVtxPosition
 case 0x8002af18: // mapInit
 case 0x8001f878: // mapGetMapObj
 case 0x8002a46c: // mapEntry
 case 0x8002a508: // mapEntrySub
 case 0x8002b120: // mapGetJoints
 case 0x8002b16c: // mapGetJointsSub
 case 0x8002a780: // _mapEnt
 case 0x80028130: // mapReCalcMatrix
 case 0x80028d08: // mapLoad
 case 0x80028f18: // _mapLoad
 case 0x80028d78: // mapUnLoad
 case 0x8002fe60: // _mapAllocTail
 case 0x8002fdc4: // _mapFree
 case 0x800782e4: // arcOpen
 case 0x80078444: // arcDataCheck
 case 0x800783b8: // arcDelete
 case 0x80037b6c: // UnpackTexPalette
 case 0x8001f920: // mapSearchDmdJoint
 case 0x8001fd48: // mapSearchDmdJointSub
 case 0x8001f958: // mapSearchDmdJointSub2
 case 0x80037b5c: // TEXGet
 case 0x8002ad08: // mapBuildTexture
 case 0x8002af5c: // mapGetBoundingBox
 case 0x8002a228: // makeDisplayList
 case 0x8002b438: // mapGetWork
 case 0x80144e88: // DVDMgrOpen
 case 0x80144e18: // DVDMgrRead
 case 0x80144dcc: // DVDMgrGetLength
 case 0x80144dd4: // DVDMgrClose
 case 0x80145024: // DVDMgrMain
 case 0x80145288: // compare
 case 0x800064fc: // qqsort
 case 0x80006680: // fsort
 return true;default:return false;}}
}
std::optional<uint32_t> resumeFieldMapSource(uint32_t function,const std::array<uint32_t,5>& args,uint32_t stackTop,const FieldMapSourceServices& s,FieldMapSourceCursor& c,unsigned visits){
 if(c.active && (c.function!=function || !compatible(c.owner,s)))return {};
 if(!c.active){if(!root(function) || !stackTop)return {};c={};c.active=true;c.function=c.pc=function;c.registers[1]=stackTop;c.registers[2]=FieldMapSource::SourceR2;c.registers[13]=FieldMapSource::SourceR13;for(unsigned i=0;i<args.size();++i)c.registers[3+i]=args[i];}
 c.owner=s;auto& r=c.registers;auto& f=c.floats;
 auto word=[&](uint32_t a,uint32_t& v){uint32_t raw=0;if(!s.word || !s.word(s.user,a,raw))return false;v=raw;return true;};
 auto byte=[&](uint32_t a,uint32_t& v){uint8_t raw=0;if(!s.byte || !s.byte(s.user,a,raw))return false;v=raw;return true;};
 auto half=[&](uint32_t a,uint32_t& v){uint16_t raw=0;if(!s.half || !s.half(s.user,a,raw))return false;v=raw;return true;};
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
