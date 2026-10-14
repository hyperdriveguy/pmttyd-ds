#include "shadow_work.hpp"
#include <cassert>
#include <cmath>
#include <limits>
using namespace ttyd;
int main(){
    ShadowWork shadow;DisplayQueue queue;ShadowMainCursor cursor;
    assert(shadow.count()==0 && shadow.type==2 && shadow.height==20 && shadow.collisionEnabled==1 && shadow.color==0x6e);
    assert(shadow.profileIndex==1 && shadow.selectedProfile()==&shadow.profiles[1]);
    assert(shadow.profiles[1].projectionKind==1 && shadow.profiles[1].sizeIndex==0 && shadow.profileSizes[0]==400);
    shadow.profiles[1].at[0]=71;
    const auto first=shadow.enter({1,2,3},4);assert(first && *first==0);
    auto* record=shadow.record(0);assert(record && record->flags==1 && record->type==2 && record->height==20 && record->radius==4);
    assert(record->collision==-1 && record->camera==4 && record->scale==1 && record->inner==.2f && record->outer==.4f && record->alpha==255);
    record->matrix[0]=71;record->padding[2]=83;record->flags=3;
    shadow.flags=0xf;shadow.counter=11;shadow.auxiliary={ShadowAuxiliaryState{3,17},ShadowAuxiliaryState{7,19}};
    assert(shadow.main(queue,cursor) && shadow.count()==0 && queue.count()==8);
    assert(shadow.flags==14 && shadow.counter==0 && shadow.auxiliary[0].flags==2 && shadow.auxiliary[1].flags==6 && shadow.auxiliary[1].counter==0);
    assert(shadow.profiles[1].at[0]==71); // Main resets request ownership, not mutable profiles.
    constexpr uint8_t cameras[]{4,1,5,7,2,2,2,2},classes[]{9,9,9,9,5,0,2,7};
    for(unsigned i=0;i<8;++i){const auto* r=queue.record(i);assert(r->camera==cameras[i] && r->drawClass==classes[i] && r->callback==0x8002d3d8 && r->userdata==0);}
    shadow.type=0x103;shadow.height=31;shadow.collisionEnabled=0;
    assert(shadow.enter({4,5,6},0)==0);assert(record->flags==0 && record->type==3 && record->height==31 && record->matrix[0]==71 && record->padding[2]==83);
    for(unsigned i=1;i<64;++i)assert(shadow.enter({0,0,0},float(i))==i);
    assert(!shadow.enter({0,0,0},1) && shadow.count()==64);
    queue.reinitialize();shadow.preparationGate=0;assert(shadow.main(queue,cursor) && queue.count()==7);
    assert(shadow.enter({std::numeric_limits<float>::quiet_NaN(),-0.f,0},std::numeric_limits<float>::infinity())==0);
    assert(std::isnan(record->position[0]) && std::signbit(record->position[1]) && std::isinf(record->radius));
    // Partial registration preserves collected requests until every Entry returns.
    queue.reinitialize();for(unsigned i=0;i<1020;++i)assert(queue.enter(0,0,0,0,0)==DisplayAdmission::Registered);
    shadow.preparationGate=1;assert(!shadow.main(queue,cursor) && cursor.step==4 && shadow.count()==1);
    queue.reinitialize();assert(shadow.main(queue,cursor) && queue.count()==4 && shadow.count()==0);
    assert(queue.record(0)->camera==2 && queue.record(0)->drawClass==5);
}
