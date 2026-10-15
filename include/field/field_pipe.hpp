#pragma once
#include "field_road.hpp"
namespace ttyd {
// Private48-byte record: five U32 identities, six native float coordinates,
// original direction word. case6 ground+signedY>30; case10 front contact.
struct FieldPipe {
    uint32_t source=0,target=0,node=0,arrivalNode=0,caseType=0;
    float x=0,y=0,z=0,arrivalX=0,arrivalY=0,arrivalZ=0;
    uint32_t direction=0;
};
bool loadFieldPipes(const char*);
// Original case6 uses Mario+1e8 contact + signed mainY strictly greater than30.
// Native ground-contact mapping is provisional until that producer is recovered.
// Case10 uses front contact. Both retain the common case-system gate.
inline bool fieldPipeAdmitted(const FieldPipe& p,uint32_t ground,uint32_t front,int8_t mainY,const RoadCaseAdmission& gate) {
    if(!gate.permits())return false;
    if(p.caseType==6)return p.node && ground==p.node && mainY>30;
    if(p.caseType==10)return p.node && front==p.node;
    return false;
}
}
