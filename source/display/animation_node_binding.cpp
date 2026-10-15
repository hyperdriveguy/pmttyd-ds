#include "animation_node_binding.hpp"
#include "event_ram_access.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
float real(uint32_t w){float f;std::memcpy(&f,&w,4);return f;}uint32_t bits(float f){uint32_t w;std::memcpy(&w,&f,4);return w;}
bool same(AnimationNodeRef a,AnimationNodeRef b){return a.guest==b.guest && a.value==b.value;}
bool same(const AnimationSourceMatrixCall& a,const AnimationSourceMatrixCall& b){return a.function==b.function && a.axis==b.axis && same(a.left,b.left) && same(a.right,b.right) && same(a.output,b.output) && !std::memcmp(a.xyz.data(),b.xyz.data(),12) && !std::memcmp(&a.radians,&b.radians,4);}
bool finite(const BattleActMatrix& m){for(float f:m)if(!std::isfinite(f))return false;return true;}
struct Load{unsigned source,offset;};
constexpr Load Captures[]{{0,0},{1,0},{1,8},{1,0x10},{0,0x10},{2,0},{1,0x18},{0,8},{0,0x18},{1,0x20},{1,0x28},{0,0x20},{0,0x28}};
constexpr unsigned Stores[]{0,0x10,8,0x18,0x20,0x28};
}
AnimationNodeBinding::AnimationNodeBinding(EventAddressSpace& m,std::function<bool()> current):memory_(m),current_(std::move(current)){}
bool AnimationNodeBinding::pair(AnimationNodeRef ref,unsigned offset,std::array<uint32_t,2>& words,AnimationNodeLocals& local){
 if(!current_ || !current_())return false;
 if(!ref.guest){if(ref.value>=local.size())return false;words={bits(local[ref.value][offset/4]),bits(local[ref.value][offset/4+1])};return true;}
 const uint32_t a=ref.value+offset;
 return memory_.withRawRamAccess([&](auto&){return current_() && memory_.contiguousRam(a,8) && memory_.readWord(a,words[0]) && memory_.readWord(a+4,words[1]);});
}
bool AnimationNodeBinding::matrix(const AnimationSourceMatrixCall& q,AnimationNodeLocals& local){
 if(!current_ || !current_())return false;
 if(!sdkActive_){sdkActive_=true;sdk_=q;locals_=&local;stage_=load_=store_=0;}
 if(!same(q,sdk_) || locals_!=&local)return false;
 auto word=[&](uint32_t a,uint32_t& v){return current_() && memory_.readWord(a,v);};
 if(q.function==0x8029dd64){
  while(stage_==0 && load_<13){const auto op=Captures[load_];std::array<uint32_t,2> words;
   if(!pair(op.source==2?AnimationNodeRef{true,0x8041e458}:op.source?q.right:q.left,op.offset,words,local))return false;
   if(op.source==2)tail_=words;else {auto& to=op.source?right_:left_;to[op.offset/4]=real(words[0]);to[op.offset/4+1]=real(words[1]);}++load_;
  }
  if(stage_==0){
   if(!finite(left_) || !finite(right_) || !std::isfinite(real(tail_[0])) || !std::isfinite(real(tail_[1])))return false;
   for(unsigned row=0;row<3;++row)for(unsigned col=0;col<4;++col){const unsigned i=row*4;volatile float u=right_[col]*left_[i];const float v=std::fma(right_[4+col],left_[i+1],float(u));const float w=std::fma(right_[8+col],left_[i+2],v);output_[i+col]=col<2?w:std::fma(real(tail_[col-2]),left_[i+3],w);}
   stage_=1;
  }
 }else{
  if(q.output.guest || q.output.value>=local.size())return false;
  if(stage_==0){for(float x:q.xyz)if(!std::isfinite(x))return false;if(!std::isfinite(q.radians))return false;
   if(q.function==0x8029dff0){sine_=float(std::sin(double(q.radians)));cosine_=float(std::cos(double(q.radians)));}stage_=1;
  }
  if(stage_==1){if(!word(0x8042935c,zero_))return false;stage_=2;}
  if(stage_==2){if(q.function!=0x8029e2b0 && !word(0x80429358,one_))return false;stage_=3;}
  if(stage_==3){const float z=real(zero_),o=real(one_);switch(q.function){
   case 0x8029dd04:output_={o,z,z,z,z,o,z,z,z,z,o,z};break;
   case 0x8029e230:output_={o,z,z,q.xyz[0],z,o,z,q.xyz[1],z,z,o,q.xyz[2]};break;
   case 0x8029e2b0:output_={q.xyz[0],z,z,z,z,q.xyz[1],z,z,z,z,q.xyz[2],z};break;
   case 0x8029dff0:{const float s=sine_,c=cosine_,neg=real(bits(s)^0x80000000u);switch(q.axis|0x20){case 120:output_={o,z,z,z,z,c,neg,z,z,s,c,z};break;case 121:output_={c,z,s,z,z,o,z,z,neg,z,c,z};break;case 122:output_={c,neg,z,z,s,c,z,z,z,z,o,z};break;default:sdkActive_=false;return true;}break;}
   default:return false;
  }stage_=4;}
 }
 if(!finite(output_))return false;
 if(q.output.guest){while(store_<6){const auto offset=Stores[store_];const uint32_t a=q.output.value+offset;const auto x=bits(output_[offset/4]),y=bits(output_[offset/4+1]);
   if(!memory_.withRawRamAccess([&](auto&){return current_() && memory_.contiguousRam(a,8) && memory_.writeWord(a,x) && memory_.writeWord(a+4,y);}))return false;
   ++store_;
  }
 }else {if(q.output.value>=local.size())return false;local[q.output.value]=output_;}
 sdkActive_=false;return true;
}
AnimationSourceNodeServices AnimationNodeBinding::source(){AnimationSourceNodeServices s;s.user=this;
 s.word=[](void* u,uint32_t a)->std::optional<uint32_t>{auto& b=*static_cast<AnimationNodeBinding*>(u);uint32_t v;return b.current_ && b.current_() && b.memory_.readWord(a,v)?std::optional<uint32_t>(v):std::nullopt;};
 s.store=[](void* u,uint32_t a,uint32_t v){auto& b=*static_cast<AnimationNodeBinding*>(u);return b.current_ && b.current_() && b.memory_.writeWord(a,v);};
 s.number=[](void* u,AnimationNodeNumber n,uint32_t a,uint32_t b)->std::optional<uint32_t>{auto& owner=*static_cast<AnimationNodeBinding*>(u);const float x=real(a),y=real(b);if(!owner.current_ || !owner.current_() || !std::isfinite(x) || !std::isfinite(y))return {};volatile float out=n==AnimationNodeNumber::Multiply?x*y:x/y;return std::isfinite(float(out))?std::optional<uint32_t>(bits(float(out))):std::nullopt;};
 s.matrix=[](void* u,const AnimationSourceMatrixCall& q,AnimationNodeLocals& local){return static_cast<AnimationNodeBinding*>(u)->matrix(q,local);};return s;
}
bool AnimationNodeBinding::advance(const AnimationGroupNode& q,unsigned visits){return current_ && current_() && resumeAnimationSourceNode(q,source(),cursor_,visits);}
}
