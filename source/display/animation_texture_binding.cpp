#include "animation_texture_binding.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
float real(uint32_t w){float f;std::memcpy(&f,&w,4);return f;}uint32_t bits(float f){uint32_t w;std::memcpy(&w,&f,4);return w;}
bool same(const AnimationRenderCall& a,const AnimationRenderCall& b){if(a.function!=b.function || a.count!=b.count || a.count>a.arguments.size())return false;for(unsigned i=0;i<a.count;++i)if(a.arguments[i].kind!=b.arguments[i].kind || a.arguments[i].value!=b.arguments[i].value)return false;return true;}
}
AnimationTextureBinding::AnimationTextureBinding(EventAddressSpace& m,const AnimationTextureBackend& b,const AnimationSourceRenderChildren& f,std::function<bool()> current):memory_(m),backend_(b),fallback_(f),current_(std::move(current)),sdk_(m,current_){}
bool AnimationTextureBinding::current()const{return current_ && current_();}
bool AnimationTextureBinding::matrix(const AnimationSourceMatrixCall& q,AnimationTextureLocals& local){
 AnimationSdkLocals view;view.user=&local;view.count=local.matrixAvailable.size();
 view.pair=[](void* u,uint32_t slot,unsigned offset,std::array<uint32_t,2>& out){auto& l=*static_cast<AnimationTextureLocals*>(u);if(slot>=l.matrixAvailable.size() || !l.matrixAvailable[slot] || offset>animation_sdk::LastPairOffset || offset%4)return false;out={bits(l.matrices[slot][offset/4]),bits(l.matrices[slot][offset/4+1])};return true;};
 view.store=[](void* u,uint32_t slot,const BattleActMatrix& value){auto& l=*static_cast<AnimationTextureLocals*>(u);if(slot>=l.matrixAvailable.size())return false;l.matrices[slot]=value;l.matrixAvailable[slot]=true;return true;};return sdk_.advance(q,view);
}
AnimationSourceTextureServices AnimationTextureBinding::source(){
 AnimationSourceTextureServices s;s.user=this;
 s.word=[](void* u,uint32_t a)->std::optional<uint32_t>{auto& b=*static_cast<AnimationTextureBinding*>(u);uint32_t v;return b.current() && b.memory_.readWord(a,v)?std::optional<uint32_t>(v):std::nullopt;};
 s.doubleWord=[](void* u,uint32_t a)->std::optional<std::array<uint32_t,2>>{auto& b=*static_cast<AnimationTextureBinding*>(u);std::array<uint32_t,2> pair;if(!b.current())return {};const bool ok=b.memory_.withRawRamAccess([&](auto&){return b.current() && b.memory_.contiguousRam(a,8) && b.memory_.readWord(a,pair[0]) && b.memory_.readWord(a+4,pair[1]);});return ok?std::optional<std::array<uint32_t,2>>(pair):std::nullopt;};
 s.byte=[](void* u,uint32_t a)->std::optional<uint8_t>{auto& b=*static_cast<AnimationTextureBinding*>(u);uint8_t v;return b.current() && b.memory_.readByte(a,v)?std::optional<uint8_t>(v):std::nullopt;};
 s.half=[](void* u,uint32_t a)->std::optional<uint16_t>{auto& b=*static_cast<AnimationTextureBinding*>(u);uint16_t v;return b.current() && b.memory_.readHalf(a,v)?std::optional<uint16_t>(v):std::nullopt;};
 s.number=[](void* u,AnimationTextureArithmetic op,std::array<uint32_t,3> a)->std::optional<uint32_t>{auto& b=*static_cast<AnimationTextureBinding*>(u);if(!b.current())return {};
  if(op==AnimationTextureArithmetic::ByteToFloat){if(a[0]>255 || a[1]!=0x43300000 || a[2]!=0)return {};return bits(float(a[0]));}
  const float x=real(a[0]),y=real(a[1]);if(!std::isfinite(x) || !std::isfinite(y))return {};volatile float out=op==AnimationTextureArithmetic::Multiply?x*y:x-y;return std::isfinite(float(out))?std::optional<uint32_t>(bits(float(out))):std::nullopt;
 };
 s.matrix=[](void* u,const AnimationSourceMatrixCall& q,AnimationTextureLocals& l){auto& b=*static_cast<AnimationTextureBinding*>(u);return b.current() && b.matrix(q,l);};
 s.call=[](void* u,const AnimationTextureCall& q,AnimationTextureLocals& l,uint32_t& result){auto& b=*static_cast<AnimationTextureBinding*>(u);if(!b.current())return false;
  if(q.function==animation_texture_source::Get){uint32_t table;if(q.count!=2 || q.arguments[0].kind!=AnimationTextureRefKind::Guest || q.arguments[1].kind!=AnimationTextureRefKind::Scalar || !b.memory_.readWord(q.arguments[0].value+8,table))return false;result=table+8*q.arguments[1].value;return true;}
  return b.backend_.call && b.backend_.call(b.backend_.user,q,l,result);
 };return s;
}
bool AnimationTextureBinding::call(const AnimationRenderCall& q,AnimationRenderLocals& locals,uint32_t& result){
 if(!current())return false;
 if(cursor_.active && (caller_!=&locals || !same(q,request_)))return false;
 if(q.function!=animation_texture_source::Function)return fallback_.call && fallback_.call(fallback_.user,q,locals,result);
 using K=AnimationRenderKind;
 if(q.count!=7)return false;
 for(unsigned i=0;i<7;++i)if(q.arguments[i].kind!=(i==0 || i==4?K::Scalar:K::Guest))return false;
 if(!cursor_.active){request_=q;caller_=&locals;}
 const auto& a=request_.arguments;
 const AnimationSourceTextureInput in{a[0].value,a[1].value,a[2].value,a[3].value,a[4].value,a[5].value,a[6].value};
 return resumeAnimationSourceTexture(in,source(),cursor_);
}
AnimationSourceRenderChildren AnimationTextureBinding::services(){return {this,[](void* u,const AnimationRenderCall& q,AnimationRenderLocals& l,uint32_t& r){return static_cast<AnimationTextureBinding*>(u)->call(q,l,r);},[](void* u,uint32_t a,uint16_t v){auto& b=*static_cast<AnimationTextureBinding*>(u);return b.current() && !b.cursor_.active && b.fallback_.fifo && b.fallback_.fifo(b.fallback_.user,a,v);}};}
}
