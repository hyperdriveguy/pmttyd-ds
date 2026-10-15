#pragma once
#include <cmath>
namespace ttyd {
// mot_upstairs8009b158: three equal vertical updates to stored targetY.
// remaining 3 counts simulation visits; begin does not advance one. Stored
// target differs from accumulated height so repeated additions retain rounding.
struct FieldStep {
    float start=0,target=0,height=0,velocity=0;
    int remaining=0;
    bool active() const{return remaining>0;}
    void begin(float from,float to){start=height=from;target=to;velocity=(to-from)/3;remaining=3;}
    void tick(){if(active()){height+=velocity;--remaining;}}
};
}
