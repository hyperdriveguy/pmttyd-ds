#pragma once
#include "animation_source_texture.hpp"
#include "animation_source_render.hpp"
#include "animation_sdk_matrix_binding.hpp"
namespace ttyd {
struct AnimationTextureBackend {
 void* user=nullptr;
 // GX object/TLUT/generator/matrix and diagnostic services remain explicit.
 bool (*call)(void*,const AnimationTextureCall&,AnimationTextureLocals&,uint32_t&)=nullptr;
};
// Raw texture parent, actual-VM TEXGet and shared SDK matrix implementation.
// Native arithmetic admits finite binary32 and the original byte-conversion
// bias only. Unsupported numerics remain held; no source finite guard is added.
class AnimationTextureBinding {
public:
 AnimationTextureBinding(EventAddressSpace&,const AnimationTextureBackend&,
  const AnimationSourceRenderChildren& fallback,std::function<bool()> current);
 AnimationTextureBinding(const AnimationTextureBinding&)=delete;
 AnimationTextureBinding& operator=(const AnimationTextureBinding&)=delete;
 AnimationSourceRenderChildren services();
 bool active() const{return cursor_.active;}
private:
 bool current()const;AnimationSourceTextureServices source();
 bool call(const AnimationRenderCall&,AnimationRenderLocals&,uint32_t&);
 bool matrix(const AnimationSourceMatrixCall&,AnimationTextureLocals&);
 EventAddressSpace& memory_;AnimationTextureBackend backend_;AnimationSourceRenderChildren fallback_;
 std::function<bool()> current_;AnimationSdkMatrixBinding sdk_;
 AnimationSourceTextureCursor cursor_;AnimationRenderCall request_;AnimationRenderLocals* caller_=nullptr;
};
}
