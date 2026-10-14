#include "positional_sfx_registry.hpp"
#include <cassert>
#include <vector>
using namespace ttyd;
struct Backend {
    std::vector<PositionalSfxStopKind> kinds;
    uint32_t descriptor=0;int32_t engine=0;bool available=true;
    PositionalSfxStopServices services() {
        return {this,[](void* p,uint32_t index)->std::optional<uint32_t>{assert(index==0x123);return static_cast<Backend*>(p)->descriptor;},
            [](void* p,PositionalSfxStopKind kind,int32_t engine){auto& b=*static_cast<Backend*>(p);b.kinds.push_back(kind);b.engine=engine;return b.available;}};
    }
};
int main() {
    PositionalSfxRegistry r;assert(r.check(-1)==-1 && r.check(0)==-1 && !r.check(40));
    for(auto& s:r.slots){assert(s.key==-1 && s.engine==-1 && s.generation==0 && !s.flags);}
    auto& s=r.slots[0];s.key=0;s.generation=0;s.volume=27;
    assert(r.check(0)==0 && r.check(0x1000000)==0 && r.check(0x100)==-1);
    s.generation=65535;assert(r.check(0xffff00)==0 && r.check(0)==-1);
    // Non-Chk accessors intentionally accept stale encoded generations.
    assert(r.getVolume(0)==27 && r.setVolume(0,256) && s.volume==0);
    assert(r.setVolume(0,-1) && s.volume==255 && r.setDistance(0,65536) && s.distance==0);
    s.distance=88;s.pitch=99;s.flags=0xffff;s.engine=123;s.position={1,2,3};r.allocationGeneration=42;
    r.initializePublicSlots();assert(s.key==-1 && s.engine==-1 && s.flags==0 && s.generation==65535);
    assert(s.volume==255 && s.distance==88 && s.pitch==99 && s.position[2]==3 && r.allocationGeneration==42);
    assert(r.setVolume(0,18) && s.volume==255 && r.getVolume(0)==0 && !r.getVolume(255));
    assert(r.off(-1,{}) && r.off(0,{}) && !r.off(40,{}));
    s.key=0x123;s.engine=91;s.flags=1;assert(r.off(0,{}) && s.key==-1 && s.engine==-1 && s.generation==65535);
    s.key=0x123;s.engine=-1;s.flags=0;assert(r.off(0,{}) && s.key==-1);
    Backend b;const auto services=b.services();
    for(const auto desc:{0xc0000000u,0x40000000u,0u}) {
        s.key=0x6123;s.engine=91;s.flags=0;b.descriptor=desc;
        assert(r.off(0,services) && s.key==-1 && s.engine==-1 && b.engine==91);
    }
    assert((b.kinds==std::vector<PositionalSfxStopKind>{PositionalSfxStopKind::Stream,PositionalSfxStopKind::Song,PositionalSfxStopKind::Effect}));
    s.key=0x123;s.engine=91;b.available=false;assert(!r.off(0,services) && s.key==0x123 && s.engine==91);
    assert(!r.off(0,{}) && s.key==0x123);
}
