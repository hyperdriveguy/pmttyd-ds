#pragma once
#include <cstdint>
namespace ttyd {
// Native room-change presentation gate. Opaque must be PRESENTED at least
// once before ready permits blocking resource replacement, even if simulation
// reaches Opaque during multi-tick catch-up. Alpha spans0..255 by integer ratio.
struct TransitionFade {
    // Original default map-change duration300ms at NTSC60Hz.
    static constexpr int Duration=18;
    enum class Phase { Idle, Out, Opaque, In };
    Phase phase=Phase::Idle;
    int ticks=0;
    bool opaquePresented=false;
    bool active() const {return phase!=Phase::Idle;}
    bool ready() const {return phase==Phase::Opaque && opaquePresented;}
    void startOut() {phase=Phase::Out;ticks=0;opaquePresented=false;}
    void startIn() {phase=Phase::In;ticks=Duration;opaquePresented=false;}
    void presented() {if(phase==Phase::Opaque)opaquePresented=true;}
    void tick() {
        if(phase==Phase::Out && ++ticks>=Duration){ticks=Duration;phase=Phase::Opaque;}
        else if(phase==Phase::In && --ticks<=0){ticks=0;phase=Phase::Idle;}
    }
    uint8_t alpha() const {return uint8_t(ticks*255/Duration);}
};
}
