#pragma once
#include "event_memory_opcodes.hpp"
#include "source_icon_gx.hpp"
#include <memory>
namespace ttyd {
struct HitN64DrawServices {
    EventMemoryServices memory;void* user=nullptr;
    uint8_t (*byte)(void*,uint32_t)=nullptr;
    float (*multiply)(void*,float,float)=nullptr;
    std::optional<int32_t> (*truncate)(void*,float)=nullptr;
    // Axis-vector function8029e1c0 uses xyz as its original borrowed axis.
    bool (*matrix)(void*,BattleAudienceMatrixRequest&)=nullptr;
    // Texture construction801d78b0 returns an actual retained native object
    // token for its original output object, not a fabricated guest stack pointer.
    std::optional<uint32_t> (*call)(void*,const SourceIconGXCall&)=nullptr;
    // Atomic original tri2 FIFO: six vertices, three U16 array indices each.
    bool (*indices)(void*,const std::array<uint16_t,18>&)=nullptr;
};
// operation1 whole callback/2 particle child share one cursor type. Reject
// changed camera/effect/providers or borrowed-base ADDRESS on hold;read base
// only at reached concat. Successful invocation resets;GPU lifetime is separate.
class HitN64DrawCursor {
public:
    HitN64DrawCursor();~HitN64DrawCursor();
    HitN64DrawCursor(const HitN64DrawCursor&)=delete;
    HitN64DrawCursor& operator=(const HitN64DrawCursor&)=delete;
    bool active()const;uint32_t pending()const;bool reset();
private:
    struct State;std::unique_ptr<State> state_;bool invoking_=false;
    friend bool resumeHitN64Draw(int32_t,uint32_t,const HitN64DrawServices&,HitN64DrawCursor&);
    friend bool resumeHitN64ParticleDraw(uint32_t,const BattleActMatrix&,const HitN64DrawServices&,HitN64DrawCursor&);
};
// Whole effHitDisp801c5d6c/450, with retained main_dl child per source pass.
// SDK matrices, actual cameras, original N64 texture object, GX and FIFO remain
// required execution providers. No fallback camera or material is manufactured.
bool resumeHitN64Draw(int32_t camera,uint32_t effect,const HitN64DrawServices&,HitN64DrawCursor&);
// Whole main_dl801c5b58/214 plus whole effSetVtxDescN64801d771c/c8 and tri2
//801d76a0/50. Borrowed base identity/storage must survive a held invocation.
bool resumeHitN64ParticleDraw(uint32_t effect,const BattleActMatrix& borrowedBase,const HitN64DrawServices&,HitN64DrawCursor&);
}
