#include "field_bero_position.hpp"
#include "event_manager.hpp"
#include <cassert>
#include <cmath>
#include <limits>
#include <vector>
using namespace ttyd;
struct Geometry {
 std::vector<int> calls;std::vector<float> angles;
 bool resolved=true,hit=true;FieldWallVector origin;
 FieldBeroPositionServices services() {
  return {this,[](void* p,const std::string& name,FieldWallVector& out){auto& g=*static_cast<Geometry*>(p);g.calls.push_back(1);assert(name=="w_bero");out={100,5,200};return true;},
   [](void* p,FieldWallVector origin,FieldWallVector direction,float length){auto& g=*static_cast<Geometry*>(p);g.calls.push_back(4);g.origin=origin;
    assert(direction.x==0 && direction.y==-1 && direction.z==0 && length==1000000);return FieldBeroFloor{g.resolved,g.hit,-2.25f};},
   [](void* p,float angle){auto& g=*static_cast<Geometry*>(p);g.calls.push_back(2);g.angles.push_back(angle);return 1.f;},
   [](void* p,float angle){auto& g=*static_cast<Geometry*>(p);g.calls.push_back(3);g.angles.push_back(angle);return 0.f;},
   [](void*,float a,float b,float c){return -std::fma(a,b,-c);},
   [](void*,float a,float b,float c){return std::fma(a,b,c);}};
 }
};
int main() {
 Geometry g;const auto result=fieldBeroNormalPosition({100000,8,9},"w_bero",0,1,30,g.services());assert(result);
 assert((g.calls==std::vector<int>{1,2,3,4,2,3}));assert(g.angles[0]==0x1.921fb6p+1f);
 assert(g.origin.x==70 && g.origin.y==5 && g.origin.z==200);
 assert(result->start.x==70 && result->start.y==-2.25f && result->start.z==200);
 assert(result->finish.x==130 && result->finish.y==-2.25f && result->finish.z==200 && result->recovery.y==34.75f);
 Geometry length;const auto longer=fieldBeroNormalPosition({100000,8,9},"w_bero",0,1,45,length.services());assert(longer);
 assert(longer->start.x==result->start.x && longer->finish.x==145); // Fixed30 start, source30+distance final.
 Geometry miss;miss.hit=false;const auto noFloor=fieldBeroNormalPosition({100000,8,9},"w_bero",0,1,30,miss.services());assert(noFloor && noFloor->start.y==5);
 Geometry pending;pending.resolved=false;assert(!fieldBeroNormalPosition({100000,8,9},"w_bero",0,1,30,pending.services()));
 Geometry zero;const auto stationary=fieldBeroNormalPosition({100000,8,9},"w_bero",0,1,-0.f,zero.services());assert(stationary);
 assert(zero.calls==std::vector<int>{1} && stationary->start.x==100 && stationary->start.y==5);
 assert(stationary->finish.x==100000 && stationary->finish.y==8 && stationary->finish.z==9 && stationary->recovery.y==45);
 Geometry authored;const auto raw=fieldBeroNormalPosition({50,8,90},"unused",0,2,30,authored.services());assert(raw && authored.calls.front()==2 && authored.angles.front()==0);
 Geometry negative;assert(fieldBeroNormalPosition({50,8,90},"unused",-2,0,30,negative.services()));assert(negative.angles.front()==-0x1.921fb6p+0f);
 assert(!fieldBeroNormalPosition({100000,0,0},"missing",0,1,30,{}));
 const auto encoded=fieldBeroStartPublication({1.234567f,-1.234567f,0});assert(encoded);
 assert(encoded->words[0]==-229998736 && encoded->words[1]==-230001264 && encoded->words[2]==-230000000);
 assert(encoded->position.x==1.234375f && encoded->position.y==-1.234375f && encoded->position.z==0);
 EventEntry e;e.locals.fill(77);e.localFlags.fill(~0u);const auto original=e;
 assert(publishFieldBeroStart(e,*encoded));for(unsigned n=0;n<16;++n)assert(e.locals[n]==(n>=10 && n<=12?encoded->words[n-10]:77));assert(e.localFlags==original.localFlags);
 assert(!fieldBeroStartPublication({std::numeric_limits<float>::infinity(),0,0}));
 assert(!fieldBeroStartPublication({2097152,0,0})); // Source fctiwz range is a native admission boundary.
}
