#pragma once
#include <cstdint>
// duration/elapsed are milliseconds; tick uses live source simulation refresh
// rate, not display FPS. No zero/negative-rate guard exists. Duration0 completes
// immediately without changing slot28;ordinary tick subtracts10 from it.
// textureOffset retains source slot28;used fade10 GPU path has an identity
// texture matrix,so this scalar does not drive texture scrolling on that path.
// ratio==1 does not finish until a later tick makes it>1; alpha truncates255*ratio.
// enter resets scalar history but performs no camera/event/field-return work.
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
