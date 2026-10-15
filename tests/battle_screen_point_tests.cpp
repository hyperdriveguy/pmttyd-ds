#include "battle_screen_point.hpp"
#include "camera_depth.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
uint32_t bits(float value){uint32_t result;std::memcpy(&result,&value,4);return result;}
struct Fixture {
    std::map<uint32_t,uint32_t> words;
    std::vector<uint32_t> reads,calls;
    uint32_t camera=0x83000000;
    bool readyCamera=true,readyView=true,readyProject=true,mutate=false;
    CameraDepthSnapshot matrices;
    Fixture(){
        words[0x80422244]=bits(304);words[0x80422248]=bits(240);words[0x80422240]=bits(0);
        position({1,2,3});
        matrices.view={{{1,0,0,0},{0,1,0,0},{0,0,1,0}}};
        matrices.projection={{{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}}};
    }
    void position(BattleScreenPoint point){for(unsigned i=0;i<3;++i)words[0x84000000+4*i]=bits(point[i]);}
    BattleScreenPointServices services(){
        BattleScreenPointServices s;s.memory.user=this;s.user=this;
        s.memory.word=[](void* p,uint32_t address){auto& f=*static_cast<Fixture*>(p);f.reads.push_back(address);return f.words.at(address);};
        s.camera=[](void* p,uint32_t id)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);f.calls.push_back(id);assert(f.reads.empty());if(!f.readyCamera)return {};if(f.mutate)f.position({2,3,4});return f.camera;};
        s.view=[](void* p,uint32_t at,const BattleScreenPoint& in,BattleScreenPoint& out){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(at);assert(f.reads.size()==3);if(!f.readyView)return false;const auto value=cameraDepthView(f.matrices,in);out=value;if(f.mutate){f.camera=0x85000000;f.position({100,200,300});}return true;};
        s.project=[](void* p,uint32_t at,const BattleScreenPoint& in,BattleScreenPoint& out){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(at);assert(f.reads.size()==3);if(!f.readyProject)return false;const auto value=cameraDepthProject(f.matrices,in);out=value;if(f.mutate){f.words[0x80422244]=bits(12);f.words[0x80422248]=bits(-7);f.words[0x80422240]=bits(9);}return true;};
        return s;
    }
};
}
int main(){
    Fixture identity;BattleScreenPoint out{9,8,7};
    assert(packedBattleScreenPoint(0x84000000,out,identity.services()));
    assert((out==BattleScreenPoint{304,480,0}));
    assert((identity.calls==std::vector<uint32_t>{4,0x8300011c,0x8300015c}));
    assert((identity.reads==std::vector<uint32_t>{0x84000000,0x84000004,0x84000008,0x80422244,0x80422248,0x80422240}));

    // Capture camera once; copy position after camera lookup, before view.
    // Read constants freshly after projection, rather than caching304/240/0.
    Fixture mutation;mutation.mutate=true;out={9,8,7};
    assert(packedBattleScreenPoint(0x84000000,out,mutation.services()));
    assert((out==BattleScreenPoint{24,-21,9}));
    assert((mutation.calls==std::vector<uint32_t>{4,0x8300011c,0x8300015c}));

    // The explicit test math policy uses existing view/project adapters on
    // supplied matrices. Perspective division precedes304/240 scaling, and
    // output Z is original0 regardless of transformed depth.
    Fixture perspective;perspective.position({2,4,-4});
    perspective.matrices.view={{{0,-1,0,1},{1,0,0,-1},{0,0,1,0}}};
    perspective.matrices.projection={{{2,0,0,0},{0,3,0,0},{0,0,-1,-2},{0,0,-1,0}}};
    assert(packedBattleScreenPoint(0x84000000,out,perspective.services()));
    assert((out==BattleScreenPoint{-456,180,0}));
    Fixture ortho;ortho.position({-3,2,500});
    ortho.matrices.projection={{{.5f,0,0,1},{0,.25f,0,-1},{0,0,-.125f,2},{0,0,0,1}}};
    assert(packedBattleScreenPoint(0x84000000,out,ortho.services()));
    assert((out==BattleScreenPoint{-152,-120,0})); // No viewport-origin addition.

    for(unsigned boundary=0;boundary<3;++boundary){
        Fixture held;held.readyCamera=boundary!=0;held.readyView=boundary!=1;held.readyProject=boundary!=2;out={9,8,7};
        assert(!packedBattleScreenPoint(0x84000000,out,held.services()));
        assert((out==BattleScreenPoint{9,8,7}));
        assert(held.calls.size()==boundary+1);
        assert(held.reads.size()==(boundary?3u:0u));
    }
    Fixture absent;auto services=absent.services();services.camera=nullptr;out={9,8,7};
    assert(!packedBattleScreenPoint(0x84000000,out,services) && absent.calls.empty() && absent.reads.empty());
    std::cout<<"Original screen-point camera capture, copy/provider order and projection policy passed\n";
}
