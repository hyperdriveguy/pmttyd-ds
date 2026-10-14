#pragma once
#include "animation_draw_traversal.hpp"
#include "animation_node_draw.hpp"
namespace ttyd {
struct AnimationTPLImage {
    uint32_t index=0,imageHeader=0,paletteHeader=0,data=0,format=0,wrapS=0,wrapT=0,minFilter=0,magFilter=0,paletteData=0,paletteFormat=0;
    uint16_t width=0,height=0,paletteCount=0;float bias=0;uint8_t edge=0,minLOD=0,maxLOD=0;
};
class AnimationTextureResource {
public:
    static std::shared_ptr<const AnimationTextureResource> load(const char*);
    bool read(FILE*);
    std::optional<AnimationTPLImage> image(uint32_t i) const {return i<images_.size()?std::optional<AnimationTPLImage>(images_[i]):std::nullopt;}
    uint32_t count() const{return uint32_t(images_.size());}
private:std::vector<AnimationTPLImage> images_;
};
struct AnimationTextureRequest {
    uint32_t function=0,coordinate=0,slot=0,descriptor=0,state=0,wrapS=0,wrapT=0;
    AnimationTPLImage image;std::array<int32_t,7> arguments{};BattleActMatrix matrix{};
    const BattlePose* pose=nullptr;
};
struct AnimationTextureServices {
    void* user=nullptr;
    bool (*get)(void*,const BattlePose&,uint32_t,AnimationTPLImage&)=nullptr;
    bool (*gx)(void*,const AnimationTextureRequest&)=nullptr;
    bool (*matrix)(void*,BattleAudienceMatrixRequest&)=nullptr;
};
struct AnimationTextureCursor {
    bool active=false,queued=false,matrixQueued=false,ci=false;uint32_t phase=0,pending=0,draw=0,slots=0,j=0,slot=0,state=0,index=0,overrideWrap=0;
    uint32_t step=0,count=0,mask=0,result=0;const BattlePose* pose=nullptr;
    std::shared_ptr<const BattlePoseResource> resource;AnimationTPLImage image;
    AnimationTextureRequest request;BattleAudienceMatrixRequest math;
    std::array<AnimationNodeOperation,12> operations{};std::array<BattleActMatrix,8> matrices{};
};
bool resumeAnimationTexture(const BattlePose&,uint32_t draw,const AnimationTextureServices&,AnimationTextureCursor&);
// Native whole-image atlas identity; use the selected converted package's
// palette, never a texture-name-only match across differently packed atlases.
std::optional<uint16_t> animationTextureCell(const AnimationTextureRequest&);
// Native converted atlas layouts; logical 1024-square coordinates are cropped
// vertically and may be downsampled uniformly to half width.
bool animationAtlasDimensions(uint32_t width,uint32_t height,float uvScale);
}
