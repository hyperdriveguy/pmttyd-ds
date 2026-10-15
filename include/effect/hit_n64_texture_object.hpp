#pragma once
#include "event_memory_opcodes.hpp"
#include "source_icon_gx.hpp"
#include <memory>
namespace ttyd {
struct HitN64TextureObjectServices {
    EventMemoryServices memory;void* user=nullptr;
    // GXInitTexObj integers{pixels,width,height,format,wrapS,wrapT,mipmap};
    // UnpackTexObj integers{actualPalette,index}. Output storage is private
    // retained native texture-object ownership, returned as an actual opaque
    // token; this is not the original function's incidental r3 return.
    std::optional<uint32_t> (*construct)(void*,const SourceIconGXCall&)=nullptr;
};
class HitN64TextureObjectCursor {
public:
    HitN64TextureObjectCursor();~HitN64TextureObjectCursor();
    HitN64TextureObjectCursor(const HitN64TextureObjectCursor&)=delete;
    HitN64TextureObjectCursor& operator=(const HitN64TextureObjectCursor&)=delete;
    bool active()const;uint32_t pending()const;bool reset();
private:
    struct State;std::unique_ptr<State> state_;bool invoking_=false;
    friend std::optional<uint32_t> resumeHitN64TextureObject(uint32_t,const HitN64TextureObjectServices&,HitN64TextureObjectCursor&);
};
// Whole effGetTexObjN64801d78b0/60. Real state+4 selects original palette path
// versus original1x1I4 fallback; never fabricate state0 for unavailable assets.
std::optional<uint32_t> resumeHitN64TextureObject(uint32_t index,const HitN64TextureObjectServices&,HitN64TextureObjectCursor&);
}
