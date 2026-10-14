#include "field_party_appearance.hpp"
#include <cassert>
#include <vector>
#include <string>
using namespace ttyd;
struct Fixture {
    int auxiliary=-1,displayReads=0;
    std::vector<bool> hits;
    std::vector<std::string> calls;
    std::vector<float> headings;
    FieldPartyAppearanceServices services() {
        FieldPartyAppearanceServices s;s.user=this;
        s.root=[](void* p){static_cast<Fixture*>(p)->calls.push_back("root");return FieldWallVector{100,10,200};};
        s.display=[](void* p){auto& f=*static_cast<Fixture*>(p);f.calls.push_back("display");return float(++f.displayReads*5);};
        s.auxiliary=[](void* p){return static_cast<Fixture*>(p)->auxiliary;};
        s.revise=[](void* p,float value){assert(value==185);static_cast<Fixture*>(p)->calls.push_back("revise");return 185.f;};
        s.heading=[](void* p,float display,float offset,bool second){auto& f=*static_cast<Fixture*>(p);f.headings.push_back(display+offset);f.calls.push_back(second?"heading2":"heading");return display+offset;};
        s.sincos=[](void* p,float){static_cast<Fixture*>(p)->calls.push_back("sincos");return FieldWallVector{1,0,0};};
        s.move=[](void* p,FieldWallVector& root,float distance,float){assert(distance==-30);static_cast<Fixture*>(p)->calls.push_back("move");root.x+=distance;};
        s.ray=[](void* p,FieldWallVector origin,FieldWallVector direction,float length){auto& f=*static_cast<Fixture*>(p);
            assert(origin.y==47 && origin.z==200);
            if(direction.y==-1){assert(length==38 && origin.x==70);f.calls.push_back("down");}
            else {assert(length==56 && origin.x==100);f.calls.push_back("horizontal");}
            assert(!f.hits.empty());const bool hit=f.hits.front();f.hits.erase(f.hits.begin());return FieldPartyAppearanceHit{hit,999};};
        return s;
    }
};
int main() {
    for(bool hit:{false,true}) {
        Fixture f;f.hits={false,hit};auto s=f.services();FieldWallVector out;
        assert(s.ready() && fieldPartyAppearPosition2(26,0,out,s)==hit);
        assert(out.x==70 && out.y==(hit?999:10) && out.z==200 && f.displayReads==4);
        assert((f.headings==std::vector<float>{195,20}));
        assert((f.calls==std::vector<std::string>{"display","revise","display","root","display","heading","sincos","horizontal","display","heading","move","down"}));
    }
    Fixture blocked;blocked.hits={true};auto s=blocked.services();FieldWallVector out;
    assert(!fieldPartyAppearPosition2(26,0,out,s) && out.x==100 && out.y==10 && blocked.displayReads==3);
    for(int slot:{0,1}) {
        Fixture first;first.auxiliary=0;first.hits={false,false};s=first.services();
        assert(!fieldPartyAppearPosition2(26,slot,out,s) && out.x==70 && out.y==10 && first.hits.empty());
        assert((first.headings==std::vector<float>{-80,slot==0?30.f:-10.f})); // Floor failure never tries side2.
        Fixture second;second.auxiliary=1;second.hits={true,false,true};s=second.services();
        assert(fieldPartyAppearPosition2(26,slot,out,s) && out.x==70 && out.y==999 && second.hits.empty());
        assert((second.headings==std::vector<float>{-80,100,slot==0?-10.f:30.f}));
    }
    Fixture both;both.auxiliary=0;both.hits={true,true};s=both.services();
    assert(!fieldPartyAppearPosition2(26,0,out,s) && out.x==100 && out.y==10);
    assert(fieldPartyHeading(0,0,0,0)==270 && fieldPartyHeading(180,0,0,0)==90);
    assert(fieldPartyHeading(0,0,0,0,20,true)==250 && fieldPartyHeading(180,0,0,0,20,true)==110);
    assert(fieldPartyHeading(99,0,0x100000,260)==90 && fieldPartyHeading(99,0,0x100000,280)==270);
    FieldWallVector moved{};fieldPartyMove(moved,-30,270);assert(std::fabs(moved.x-30)<.001f);
    const auto direction=fieldPartySinCos(180);assert(std::fabs(direction.x)<.001f && direction.z==1);
}
