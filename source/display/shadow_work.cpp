#include "shadow_work.hpp"
namespace ttyd {
std::optional<uint32_t> ShadowWork::enter(std::array<float,3> position,float radius){
    if(count_>=records_.size())return {};
    const auto slot=count_;auto& record=records_[slot];
    record.flags=collisionEnabled?1:0;record.type=uint8_t(type);record.variant=0;
    record.position=position;record.radius=radius;record.height=height;
    record.collision=-1;record.camera=4;record.scale=1;record.inner=.2f;record.outer=.4f;record.alpha=255;
    ++count_;return slot;
}
bool ShadowWork::main(DisplayQueue& queue,ShadowMainCursor& cursor){
    static constexpr uint32_t cameras[]{4,1,5,7,2,2,2,2};
    static constexpr int32_t classes[]{9,9,9,9,5,0,2,7};
    if(!cursor.active){cursor.step=0;cursor.active=true;}
    while(cursor.step<8){
        const auto step=cursor.step;
        if(step==4 && !preparationGate){++cursor.step;continue;}
        if(queue.enter(cameras[step],classes[step],0x8002d3d8,0,0.f)==DisplayAdmission::Unavailable)return false;
        ++cursor.step;
    }
    count_=0;flags&=~1u;counter=0;
    for(auto& state:auxiliary){state.flags&=~1u;state.counter=0;}
    cursor.active=false;return true;
}
}
