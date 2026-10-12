#include "battle_position.hpp"
#include <cassert>
#include <cmath>

namespace ttyd {
namespace {
void copy(const JumpVector& p,float* x,float* y,float* z) {assert(x && y && z);*x=p.x;*y=p.y;*z=p.z;}
float mul(float a,float b) {return a*b;}
float add(float a,float b) {return a+b;}
}
void getHomePosition(const BattlePositionUnit& u,float* x,float* y,float* z) {assert(u.home);copy(*u.home,x,y,z);}
void getVisiblePosition(const BattlePositionUnit& u,float* x,float* y,float* z) {assert(u.visible);copy(*u.visible,x,y,z);}
void getPartOffsetPosition(const BattlePositionPart& p,float* x,float* y,float* z) {copy(p.offset,x,y,z);}
void getPartWorldPosition(const BattlePositionPart& p,float* x,float* y,float* z) {
    const uint32_t attributes=p.attributes;auto* owner=p.owner;
    if(attributes&0x10000000) {copy(p.absolute,x,y,z);return;}
    assert(owner && owner->facing);JumpVector unit,offset;
    getVisiblePosition(*owner,&unit.x,&unit.y,&unit.z);
    getPartOffsetPosition(p,&offset.x,&offset.y,&offset.z);
    const float facing=static_cast<float>(*owner->facing);
    *x=std::fma(offset.x,facing,unit.x);*y=add(unit.y,offset.y);*z=add(unit.z,offset.z);
}
void getHitPosition(const BattlePositionUnit& u,const BattlePositionPart& p,float* x,float* y,float* z,const BattleHitPositionServices& s) {
    assert(u.belong && s.sideFactor);const float side=static_cast<float>(s.sideFactor(s.user,*u.belong));
    getPartWorldPosition(p,x,y,z);assert(u.scale);
    for(unsigned i=0;i<2;++i) {
        const float dx=mul(p.hitOffsets[i].x,*u.scale);*x=std::fma(side,dx,*x);
        const float dy=mul(p.hitOffsets[i].y,*u.scale);*y=std::fma(1.f,dy,*y);
        *z=std::fma(p.hitOffsets[i].z,*u.scale,*z);
    }
}
BattlePositionPart* getPositionPart(const BattlePositionUnit& u,int32_t id) {
    auto* head=u.head;if(id<0)return head;
    assert(head);if(!head->next)return head;
    for(auto* p=head;p;p=p->next)if(p->id==id)return p;
    return nullptr;
}
int hitPositionEvent(const std::array<int32_t,5>& operands,const BattlePositionEventServices& event,const BattleHitPositionServices& hit) {
    assert(event.resolve && event.translate && event.unit && event.part);
    const int32_t id=event.translate(event.user,event.resolve(event.user,operands[0]));
    const int32_t partId=event.resolve(event.user,operands[1]);
    auto* u=event.unit(event.user,id);auto* p=event.part(event.user,id,partId);assert(u && p);
    JumpVector position;getHitPosition(*u,*p,&position.x,&position.y,&position.z,hit);
    const float values[]={position.x,position.y,position.z};
    for(unsigned i=0;i<3;++i)if(operands[i+2]!=-250000000) {
        assert(event.truncate && event.store);
        const int32_t value=event.truncate(event.user,values[i]);event.store(event.user,operands[i+2],value);
    }
    return 2;
}
}
