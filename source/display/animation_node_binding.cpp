#include "animation_node_binding.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {float real(uint32_t w){float f;std::memcpy(&f,&w,4);return f;}uint32_t bits(float f){uint32_t w;std::memcpy(&w,&f,4);return w;}}
AnimationNodeBinding::AnimationNodeBinding(EventAddressSpace& m,std::function<bool()> current):memory_(m),current_(std::move(current)),sdk_(m,current_){}
// Borrow durable controller-local matrices through SDK slot/pair services.
// 48-byte matrices contain 12 floats; a two-float pair starts at most byte 40
// and on a 4-byte boundary. SDK owns captured inputs and partial guest outputs.
bool AnimationNodeBinding::matrix(const AnimationSourceMatrixCall& q,AnimationNodeLocals& local){
 AnimationSdkLocals view;view.user=&local;view.count=local.size();
 view.pair=[](void* u,uint32_t slot,unsigned offset,std::array<uint32_t,2>& out){auto& l=*static_cast<AnimationNodeLocals*>(u);if(slot>=l.size() || offset>40 || offset%4)return false;out={bits(l[slot][offset/4]),bits(l[slot][offset/4+1])};return true;};
 view.store=[](void* u,uint32_t slot,const BattleActMatrix& m){auto& l=*static_cast<AnimationNodeLocals*>(u);if(slot>=l.size())return false;l[slot]=m;return true;};
 return sdk_.advance(q,view);
}
AnimationSourceNodeServices AnimationNodeBinding::source(){AnimationSourceNodeServices s;s.user=this;
 s.word=[](void* u,uint32_t a)->std::optional<uint32_t>{auto& b=*static_cast<AnimationNodeBinding*>(u);uint32_t v;return b.current_ && b.current_() && b.memory_.readWord(a,v)?std::optional<uint32_t>(v):std::nullopt;};
 s.store=[](void* u,uint32_t a,uint32_t v){auto& b=*static_cast<AnimationNodeBinding*>(u);return b.current_ && b.current_() && b.memory_.writeWord(a,v);};
 // Native finite binary32 Multiply/Divide only. Unused NaN fields may pass
 // parent gates; reached nonfinite operands/results hold without publishing 0.
 // These admission limits are not original node validation or full Gekko math.
 s.number=[](void* u,AnimationNodeNumber n,uint32_t a,uint32_t b)->std::optional<uint32_t>{auto& owner=*static_cast<AnimationNodeBinding*>(u);const float x=real(a),y=real(b);if(!owner.current_ || !owner.current_() || !std::isfinite(x) || !std::isfinite(y))return {};volatile float out=n==AnimationNodeNumber::Multiply?x*y:x/y;return std::isfinite(float(out))?std::optional<uint32_t>(bits(float(out))):std::nullopt;};
 s.matrix=[](void* u,const AnimationSourceMatrixCall& q,AnimationNodeLocals& local){return static_cast<AnimationNodeBinding*>(u)->matrix(q,local);};return s;
}
// Borrowed VM/current predicate and this stable binding must outlive pending
// node AND SDK cursors. Budget 128 is native scheduling,not source timing.
bool AnimationNodeBinding::advance(const AnimationGroupNode& q,unsigned visits){return current_ && current_() && resumeAnimationSourceNode(q,source(),cursor_,visits);}
}
