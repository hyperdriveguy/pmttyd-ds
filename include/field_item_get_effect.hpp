#pragma once
#include "actor_animation.hpp"
#include <array>
#include <cstdint>
namespace ttyd {
struct ItemGetEffectPass {
 ActorMatrix model;
 uint32_t color=0;
 uint8_t alpha=0;
};
struct ItemGetEffectDraw {
 std::array<ItemGetEffectPass,2> passes;
 // Source texmap0/1 and TEV color registers. Texture pixels stay grayscale.
 std::array<uint32_t,2> textures{{44,42}};
 std::array<uint32_t,2> colors{{0xe1e1e1ff,0xeb7d06ff}};
 std::array<ActorVector,4> vertices{{{{-32,32,0}},{{32,32,0}},{{32,0,0}},{{-32,0,0}}}};
 std::array<std::array<float,2>,4> uv{{{{0,0}},{{1,0}},{{1,1}},{{0,1}}}};
};
class FieldItemGetEffect6 {
public:
 void begin(ActorVector originalRoot);
 void softDelete(){if(active_)softDelete_=true;}
 bool tick(); // One admitted source effect-driver invocation, not render time.
 ItemGetEffectDraw draw(float cameraYaw,uint32_t shadowColor) const;
 bool active() const{return active_;}
 int32_t age() const{return age_;}
 int32_t lifetime() const{return lifetime_;}
 uint8_t alpha() const{return alpha_;}
 float scale() const{return scale_;}
 uint32_t layer() const{return layer_;}
 void layer(uint32_t value){layer_=value;}
private:
 ActorVector root_{};
 bool active_=false,softDelete_=false;
 int32_t age_=0,lifetime_=1000;
 uint8_t alpha_=255;
 float scale_=0;
 uint32_t layer_=4;
};
}
