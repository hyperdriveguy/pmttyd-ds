#include "source_camera_bootstrap.hpp"
#include <cstring>
namespace ttyd {
namespace {
double scalar(uint64_t b){double v;std::memcpy(&v,&b,sizeof(v));return v;}
uint64_t bits(double v){uint64_t b;std::memcpy(&b,&v,sizeof(b));return b;}
float single(uint32_t b){float v;std::memcpy(&v,&b,sizeof(v));return v;}
uint32_t singleBits(double d){volatile float v=float(d);float f=v;uint32_t b;std::memcpy(&b,&f,sizeof(b));return b;}
uint64_t rounded(double d){return bits(double(single(singleBits(d))));}
bool root(uint32_t f){using namespace SourceCameraBootstrap;return f==Init||f==Entry||f==Perspective||f==Ortho||f==RenderMode||f==SelectedModeCopy;}
bool compatible(const SourceCameraServices& a,const SourceCameraServices& b){
#define PIN(f) (!a.f || a.f==b.f)
 return a.user==b.user&&PIN(word)&&PIN(half)&&PIN(store)&&PIN(storeHalf)&&PIN(call);
#undef PIN
}
}
std::optional<uint32_t> resumeSourceCamera(const SourceCameraCall& input,uint32_t stackTop,const SourceCameraServices& s,SourceCameraCursor& c,unsigned visits){
 if(c.active&&(c.input.function!=input.function||c.input.arguments!=input.arguments||c.input.scalars!=input.scalars||!compatible(c.owner,s)))return {};
 if(!c.active){if(!root(input.function)||!stackTop)return {};c={};c.active=true;c.function=c.pc=input.function;c.input=input;c.registers[1]=stackTop;c.registers[2]=SourceCameraBootstrap::SourceR2;c.registers[13]=SourceCameraBootstrap::SourceR13;for(unsigned i=0;i<input.arguments.size();++i)c.registers[3+i]=input.arguments[i];for(unsigned i=0;i<input.scalars.size();++i)c.floats[1+i]=input.scalars[i];if(input.function==SourceCameraBootstrap::SelectedModeCopy)c.registers[31]=input.arguments[0];}
 c.owner=s;auto& r=c.registers;auto& f=c.floats;
 auto word=[&](uint32_t a,uint32_t& v){uint32_t t;if(!s.word||!s.word(s.user,a,t))return false;v=t;return true;};
 auto half=[&](uint32_t a,uint32_t& v){uint16_t t;if(!s.half||!s.half(s.user,a,t))return false;v=t;return true;};
 auto store=[&](uint32_t a,uint32_t v){return s.store&&s.store(s.user,a,v);};
 auto external=[&](uint32_t target,uint32_t next){
  if(!c.queued){c.request={};c.request.function=target;for(unsigned i=0;i<c.request.arguments.size();++i)c.request.arguments[i]=r[3+i];for(unsigned i=0;i<c.request.scalars.size();++i)c.request.scalars[i]=f[1+i];c.lr=next;c.queued=true;}
  if(!s.call||!s.call(s.user,c.request))return false;
  if(c.request.result)r[3]=*c.request.result;
  if(c.request.floatingResult)f[1]=*c.request.floatingResult;
  c.queued=false;c.pc=next;return true;
 };
 while(visits--){if(!c.pc){auto v=r[3];c={};return v;}switch(c.pc){
#include "source_camera_bootstrap_program.inc"
 default:return {};
 }}return {};
}
}
