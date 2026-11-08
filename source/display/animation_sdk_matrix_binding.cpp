#include "animation_sdk_matrix_binding.hpp"
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
// Concat pair capture order (hex byte offsets): A00,B00,B08,B10,A10,
// tail8041e458,B18,A08,A18,B20,B28,A20,A28. Source 2 denotes live(0,1)
// tail,not a third matrix. All 13 pairs precede ANY output. Stores below are
// 00,10,08,18,20,28; this differs from sequential PSMTXCopy pair ordering.
constexpr Load Captures[]{{0,0},{1,0},{1,8},{1,0x10},{0,0x10},{2,0},{1,0x18},{0,8},{0,0x18},{1,0x20},{1,0x28},{0,0x20},{0,0x28}};
constexpr unsigned Stores[]{0,0x10,8,0x18,0x20,0x28};
}
AnimationSdkMatrixBinding::AnimationSdkMatrixBinding(EventAddressSpace& m,std::function<bool()> current,AnimationSdkGuestPairs guest):memory_(m),current_(std::move(current)),guest_(guest){}
// Local references are slot IDs; guest references wrap address+offset. A pair
// is 8 bytes, starts on a 4-byte local boundary and fits within 48-byte matrices.
// Semantic range endpoints get exclusive dispatch,never a raw-RAM fallback.
bool AnimationSdkMatrixBinding::pair(AnimationNodeRef ref,unsigned offset,std::array<uint32_t,2>& words,const AnimationSdkLocals& local){
 if(!current_ || !current_())return false;
 if(!ref.guest)return ref.value<local.count && offset<=animation_sdk::LastPairOffset && offset%4==0 && local.pair && local.pair(local.user,ref.value,offset,words);
 const uint32_t a=ref.value+offset;
 if(guest_.owns && (guest_.owns(guest_.user,a) || guest_.owns(guest_.user,a+7)))return guest_.pair && guest_.pair(guest_.user,a,words);
 // One complete contiguous raw pair per synchronous borrow; no pointer or
 // borrow survives a provider hold. A partial pair acknowledges neither lane.
 return memory_.withRawRamAccess([&](auto&){return current_() && memory_.contiguousRam(a,8) && memory_.readWord(a,words[0]) && memory_.readWord(a+4,words[1]);});
}
// Snapshot request BITS and local storage identity once. A held store/result
// never recaptures inputs. current must remain pure and protect the same pass.
bool AnimationSdkMatrixBinding::advance(const AnimationSourceMatrixCall& incoming,const AnimationSdkLocals& storage){
 if(!current_ || !current_())return false;
 if(!sdkActive_){sdkActive_=true;sdk_=incoming;locals_=storage;stage_=load_=store_=0;}
 if(!same(incoming,sdk_) || locals_.user!=storage.user || locals_.count!=storage.count || locals_.pair!=storage.pair || locals_.store!=storage.store)return false;
 const auto& q=sdk_;const auto& local=locals_;
 auto word=[&](uint32_t a,uint32_t& v){return current_() && memory_.readWord(a,v);};
 if(q.function==animation_sdk::Concat){
  while(stage_==0 && load_<13){const auto op=Captures[load_];std::array<uint32_t,2> words;
   if(!pair(op.source==2?AnimationNodeRef{true,animation_sdk::ConcatTail}:op.source?q.right:q.left,op.offset,words,local))return false;
   if(op.source==2)tail_=words;else {auto& to=op.source?right_:left_;to[op.offset/4]=real(words[0]);to[op.offset/4+1]=real(words[1]);}++load_;
  }
  if(stage_==0){
   if(!finite(left_) || !finite(right_) || !std::isfinite(real(tail_[0])) || !std::isfinite(real(tail_[1])))return false;
   // Row-major 3x4: first product narrows,then ordered FMAs. Columns 2/3
   // retain EXTRA FMA(tail[0/1],translation,w),even if algebra could simplify.
   // Finite admission is native policy,not proof of full Gekko/FPSCR parity.
   for(unsigned row=0;row<3;++row)for(unsigned col=0;col<4;++col){const unsigned i=row*4;volatile float u=right_[col]*left_[i];const float v=std::fma(right_[4+col],left_[i+1],float(u));const float w=std::fma(right_[8+col],left_[i+2],v);output_[i+col]=col<2?w:std::fma(real(tail_[col-2]),left_[i+3],w);}
   stage_=1;
  }
 }else{
  if(q.output.guest || q.output.value>=local.count)return false;
  // Non-Concat writers publish only LOCAL matrices. RotRad calls double
  // sin THEN cos,narrowing each before SDK ZERO/ONE loads. Scale loads ZERO
  // only; Identity/Trans/RotTrig also load ONE. These are mutable SDK literals,
  // distinct from node constants. Reached nonfinite operands/results hold.
  if(stage_==0){for(float x:q.xyz)if(!std::isfinite(x))return false;if(!std::isfinite(q.radians))return false;
   if(q.function==animation_sdk::RotateRadians){sine_=float(std::sin(double(q.radians)));cosine_=float(std::cos(double(q.radians)));}stage_=1;
  }
  if(stage_==1){if(!word(animation_sdk::Zero,zero_))return false;stage_=2;}
  if(stage_==2){if(q.function!=animation_sdk::Scale && !word(animation_sdk::One,one_))return false;stage_=3;}
  if(stage_==3){const float z=real(zero_),o=real(one_);switch(q.function){
   case animation_sdk::Identity:output_={o,z,z,z,z,o,z,z,z,z,o,z};break;
   case animation_sdk::Translate:output_={o,z,z,q.xyz[0],z,o,z,q.xyz[1],z,z,o,q.xyz[2]};break;
   case animation_sdk::Scale:output_={q.xyz[0],z,z,z,z,q.xyz[1],z,z,z,z,q.xyz[2],z};break;
   // OR 20hex folds ASCII X/Y/Z to 120/121/122. Other axes complete with
   // NO store after trig/constants,matching the SDK default return branch.
   case animation_sdk::RotateRadians:{const float s=sine_,c=cosine_,neg=real(bits(s)^0x80000000u);switch(q.axis|0x20){case 120:output_={o,z,z,z,z,c,neg,z,z,s,c,z};break;case 121:output_={c,z,s,z,z,o,z,z,neg,z,c,z};break;case 122:output_={c,neg,z,z,s,c,z,z,z,z,o,z};break;default:sdkActive_=false;return true;}break;}
   default:return false;
  }stage_=4;}
 }
 if(!finite(output_))return false;
 // Guest output retires whole 8-byte pairs in source order. Earlier pairs
 // remain committed if a later pair is missing; semantic outputs are refused.
 // Local output publishes once via its owner,after all numeric work succeeds.
 if(q.output.guest){while(store_<6){const auto offset=Stores[store_];const uint32_t a=q.output.value+offset;const auto x=bits(output_[offset/4]),y=bits(output_[offset/4+1]);
   if(guest_.owns && (guest_.owns(guest_.user,a) || guest_.owns(guest_.user,a+7)))return false;
   if(!memory_.withRawRamAccess([&](auto&){return current_() && memory_.contiguousRam(a,8) && memory_.writeWord(a,x) && memory_.writeWord(a+4,y);}))return false;
   ++store_;
  }
 }else {if(q.output.value>=local.count)return false;if(!local.store || !local.store(local.user,q.output.value,output_))return false;}
 sdkActive_=false;return true;
}
}
