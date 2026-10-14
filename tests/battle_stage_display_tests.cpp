#include "battle_stage_display.hpp"
#include "native_shadow_program.hpp"
#include "native_display_state.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <unordered_map>
#include <vector>
using namespace ttyd;
struct Fixture {
    uint32_t battle=0x84000000;std::unordered_map<uint32_t,uint32_t> words;std::unordered_map<uint32_t,uint8_t> bytes;
    CameraState camera;NativeShadowProgram program;NativeDisplayState display;
    unsigned cameras=0,mathCalls=0;uint32_t holdGX=0;bool holdPrimitive=false,holdMath=false;
    std::vector<ShadowGXRequest> requests;std::vector<BattleStagePrimitive> primitives;
    Fixture(){camera.viewPublished=true;camera.matrices.view={{{1,0,0,0},{0,1,0,0},{0,0,1,0}}};program.afterDummy();display.afterDummy();}
    uint32_t work()const{return battle+0x163fc;}
    void real(uint32_t a,float x){uint32_t u;std::memcpy(&u,&x,4);words[a]=u;}
    BattleStageDisplayServices services(){
        BattleStageDisplayServices s;s.user=this;s.actors.user=this;s.actors.memory.user=this;
        s.actors.memory.word=[](void* u,uint32_t a){auto& f=*static_cast<Fixture*>(u);return a==0x8041eb90?f.battle:f.words[a];};
        s.actors.byte=[](void* u,uint32_t a){return static_cast<Fixture*>(u)->bytes[a];};
        s.camera=[](void* u,uint32_t i)->const CameraState*{assert(i==4);auto& f=*static_cast<Fixture*>(u);++f.cameras;return &f.camera;};
        s.gx=[](void* u,const ShadowGXRequest& r){auto& f=*static_cast<Fixture*>(u);if(uint32_t(r.kind)==f.holdGX)return false;
            if(r.kind==ShadowGX::Depth || r.kind==ShadowGX::Blend || r.kind==ShadowGX::DepthLocation || r.kind==ShadowGX::Alpha){
                DisplayStateRequest q;q.kind=r.kind==ShadowGX::Depth?DisplayGXState::ZMode:r.kind==ShadowGX::Blend?DisplayGXState::BlendMode:r.kind==ShadowGX::DepthLocation?DisplayGXState::ZCompareLocation:DisplayGXState::AlphaCompare;
                for(unsigned i=0;i<5;++i)q.values[i]=r.values[i];assert(f.display.request(q,{nullptr,[](void*,const NativeDisplayPipeline& p){return p.rasterAdmitted();}}));
            }else if(r.kind!=ShadowGX::PositionMatrix){auto next=f.program.request(r);assert(next);f.program=*next;}
            f.requests.push_back(r);return true;};
        s.primitive=[](void* u,const BattleStagePrimitive& r){auto& f=*static_cast<Fixture*>(u);if(f.holdPrimitive)return false;f.primitives.push_back(r);return true;};
        s.math=[](void* u,BattleStageMath& r){auto& f=*static_cast<Fixture*>(u);if(f.holdMath)return false;++f.mathCalls;return nativeBattleStageMath(r);};return s;
    }
};
int main(){
    Fixture f;f.bytes[f.work()+0xb08]=140;f.bytes[f.work()+0xae8]=0;f.holdGX=uint32_t(ShadowGX::TevOrder);
    BattleStageDisplayCursor c;assert(!resumePackedBattleStageDisplay(0x80138da8,f.services(),c) && c.pending==f.holdGX);
    assert(f.cameras==1 && f.primitives.empty());const auto setup=f.requests.size();
    f.camera.matrices.view[0][3]=123;f.holdGX=0;f.holdPrimitive=true;
    assert(!resumePackedBattleStageDisplay(0x80138da8,f.services(),c) && c.pending==0x802b5a6c && f.cameras==1);
    assert(c.primitive.vertices[0].color[3]==140 && f.requests.size()==setup+2);
    // Held quad captures alpha once. Following squares reread the retained
    // stage owner even if global BattleWork changes while the provider holds.
    const auto owner=f.work();f.bytes[owner+0xb08]=37;f.bytes[owner+0xae8]=9;f.battle+=0x100000;
    f.holdPrimitive=false;assert(resumePackedBattleStageDisplay(0x80138da8,f.services(),c));
    assert(f.primitives.size()==3 && f.primitives[0].vertices[0].color[3]==140 && f.primitives[1].vertices[0].color[3]==37 && f.primitives[2].vertices[0].color[3]==9);
    for(unsigned j=0;j<3;++j){const auto& q=f.primitives[j];assert(q.mode==0x80 && q.count==4);const float x=j==1?640:-640;
        assert((q.vertices[0].position==std::array<float,3>{x,640,105}));assert((q.vertices[2].position==std::array<float,3>{-x,-640,105}));}
    unsigned matrix=0;std::vector<int32_t> compares;
    for(const auto& q:f.requests){if(q.kind==ShadowGX::PositionMatrix){++matrix;assert(q.matrix[0][3]==0);}if(q.kind==ShadowGX::Depth)compares.push_back(q.values[1]);}
    assert(matrix==1 && compares==std::vector<int32_t>({6,1,1}) && f.program.alpha==NativeShadowProgram::Raster && f.display.state()->compare==1);
    // Actual inactive storage still issues the full, distinct light prefix.
    f=Fixture{};c={};assert(resumePackedBattleStageDisplay(0x80138da8,f.services(),c));f.requests.clear();f.primitives.clear();c={};assert(resumePackedBattleStageDisplay(0x801389a0,f.services(),c));
    assert(f.primitives.empty() && f.requests.size()==20 && f.program.alpha==NativeShadowProgram::Zero && f.display.state()->sourceBlend==2);
    assert((f.requests[3].values==std::array<int32_t,7>({1,2,1,5,0,0,0}) && f.requests[4].kind==ShadowGX::Depth && f.requests.back().kind==ShadowGX::PositionMatrix));
    // One active entry emits the actual eighteen rotated beam triangles.
    f=Fixture{};c={};assert(resumePackedBattleStageDisplay(0x80138da8,f.services(),c));f.requests.clear();f.primitives.clear();c={};const auto w=f.work();f.words[w+4]=1;
    f.real(w+0x88,0);f.real(w+0x8c,0);f.real(w+0x90,1);f.real(w+0x40,2);f.real(w+0xb8,1);
    f.bytes[w+0x28]=12;f.bytes[w+0x29]=34;f.bytes[w+0x2a]=56;f.holdMath=true;
    assert(!resumePackedBattleStageDisplay(0x801389a0,f.services(),c) && c.pending==0x8029e1c0 && f.primitives.empty());
    const auto calls=f.requests.size();f.holdMath=false;f.holdPrimitive=true;
    assert(!resumePackedBattleStageDisplay(0x801389a0,f.services(),c) && c.pending==0x802b5a6c && f.mathCalls==3);
    const auto captured=c.primitive;f.bytes[w+0x28]=99;f.real(w+0x88,1);f.holdPrimitive=false;
    assert(resumePackedBattleStageDisplay(0x801389a0,f.services(),c) && f.requests.size()==calls && f.mathCalls==54 && f.primitives.size()==18);
    assert(f.primitives[0].vertices[0].color==captured.vertices[0].color && f.primitives[1].vertices[0].color[0]==99);
    for(unsigned i=0;i<18;++i){assert(f.primitives[i].mode==0x90 && f.primitives[i].count==3 && f.primitives[i].vertices[0].color[3]==0);
        if(i==1){assert(std::fabs(f.primitives[i].vertices[1].position[0]-f.primitives[i-1].vertices[2].position[0]-2)<1e-6f);}
        if(i>1)assert(f.primitives[i].vertices[1].position==f.primitives[i-1].vertices[2].position);}
    BattleStageMath math;math.function=0x8029e1c0;math.axis={0,0,1};math.angle=.3490658402442932f;
    assert(nativeBattleStageMath(math) && std::fabs(math.matrix[0]-.93969262f)<1e-6f && std::fabs(math.matrix[1]+.34202014f)<1e-6f);
    math.axis={};math.angle=0;assert(!nativeBattleStageMath(math)); // No invented degenerate identity.
    math={};math.function=0x8029ebe4;math.vector={};assert(!nativeBattleStageMath(math));
}
