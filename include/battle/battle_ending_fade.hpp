#pragma once
#include <cstdint>
namespace ttyd {
// Used original fade10 scalar owner. Source GP refresh rate is supplied; the
// native black overlay adapts the original embedded white1px texture to3DS.
struct BattleEndingFade {
    bool active=false,finished=false;
    int32_t duration=0;
    float elapsed=0,ratio=0,textureOffset=0;
    void enter(int32_t milliseconds){active=true;finished=false;duration=milliseconds;elapsed=ratio=textureOffset=0;}
    void tick(int32_t refreshRate){
        if(!active || finished)return;
        if(duration==0){ratio=1;finished=true;return;}
        elapsed+=1000.f/float(refreshRate);ratio=elapsed/float(duration);textureOffset-=10;
        if(ratio>1){ratio=1;finished=true;}
    }
    uint8_t alpha() const {return uint8_t(ratio*255.f);}
};
}
