#include "animation_render_binding.hpp"
namespace ttyd {
AnimationRenderBinding::AnimationRenderBinding(EventAddressSpace& m,const AnimationSourceRenderChildren& s,std::function<bool()> current):memory_(m),children_(s),current_(std::move(current)){}
AnimationSourceRenderServices AnimationRenderBinding::source(){
 AnimationSourceRenderServices s;s.user=this;s.children.user=this;
 s.word=[](void* u,uint32_t a)->std::optional<uint32_t>{auto& b=*static_cast<AnimationRenderBinding*>(u);uint32_t v;return b.current_ && b.current_() && b.memory_.readWord(a,v)?std::optional<uint32_t>(v):std::nullopt;};
 s.byte=[](void* u,uint32_t a)->std::optional<uint8_t>{auto& b=*static_cast<AnimationRenderBinding*>(u);uint8_t v;return b.current_ && b.current_() && b.memory_.readByte(a,v)?std::optional<uint8_t>(v):std::nullopt;};
 s.half=[](void* u,uint32_t a)->std::optional<uint16_t>{auto& b=*static_cast<AnimationRenderBinding*>(u);uint16_t v;return b.current_ && b.current_() && b.memory_.readHalf(a,v)?std::optional<uint16_t>(v):std::nullopt;};
 s.children.call=[](void* u,const AnimationRenderCall& q,AnimationRenderLocals& locals,uint32_t& result){auto& b=*static_cast<AnimationRenderBinding*>(u);return b.current_ && b.current_() && b.children_.call && b.children_.call(b.children_.user,q,locals,result);};
 s.children.fifo=[](void* u,uint32_t a,uint16_t v){auto& b=*static_cast<AnimationRenderBinding*>(u);return b.current_ && b.current_() && b.children_.fifo && b.children_.fifo(b.children_.user,a,v);};return s;
}
bool AnimationRenderBinding::advance(uint32_t shape,unsigned visits){return current_ && current_() && resumeAnimationSourceRender(shape,source(),cursor_,visits);}
}
