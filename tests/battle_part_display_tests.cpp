#include "battle_part_display.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    static constexpr uint32_t P=0x10000,U=0x20000,B=0x30000,H=P+0x21c;
    std::map<uint32_t,uint8_t> memory;CameraState camera;
    unsigned matrixCalls=0,scalarCalls=0,statusCalls=0,callbacks=0;
    int holdMatrix=-1;bool status4=false,status16=false,holdMaterial=false,holdDraw=false,hook=false;
    std::vector<BattleAudienceMatrixRequest> matrices;std::vector<BattlePartMaterialRequest> material;std::vector<BattlePartDrawRequest> draws;
    Fixture(){word(0x8041eb90,B);word(P+0x1c0,9);word(P+0x4ec,U);word(P+0x4f4,0x12345678);word(U+0x1c,0x01000000);
        word(0x8042223c,0x3c8efa35);real(0x80422240,0);real(0x8042224c,1);real(0x80422250,2);real(0x80422258,270);real(0x8042225c,90);real(0x80422260,-1);real(0x80422268,-5);
        for(unsigned o:{0x90u,0x94u,0x98u,0x9cu,0xa0u,0xa4u,0x114u})real(U+o,1);
        for(unsigned o:{0x60u,0x64u,0x68u,0x6cu,0x70u,0x74u})real(P+o,1);
        real(U+0x3c,10);real(U+0x40,20);real(U+0x44,30);real(P+0x24,5);real(P+0x28,6);real(P+0x2c,7);
        memory[P+0xbd]=1;memory[U+0x189]=1;memory[U+0x312]=255;
    }
    uint32_t word(uint32_t a){uint32_t v=0;for(unsigned i=0;i<4;++i)v=v<<8|memory[a+i];return v;}
    void word(uint32_t a,uint32_t v){for(unsigned i=0;i<4;++i)memory[a+i]=v>>(24-8*i);}
    void real(uint32_t a,float v){uint32_t b;std::memcpy(&b,&v,4);word(a,b);}
    float real(uint32_t a){auto b=word(a);float v;std::memcpy(&v,&b,4);return v;}
    BattlePartDisplayServices services(){BattlePartDisplayServices s;s.user=this;s.actors.user=this;s.actors.memory.user=this;
        s.actors.memory.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->word(a);};
        s.actors.memory.storeWord=[](void* p,uint32_t a,uint32_t v){static_cast<Fixture*>(p)->word(a,v);};
        s.actors.memory.storeByte=[](void* p,uint32_t a,uint8_t v){static_cast<Fixture*>(p)->memory[a]=v;};
        s.actors.byte=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->memory[a];};
        s.currentCamera=[](void* p)->const CameraState*{return &static_cast<Fixture*>(p)->camera;};
        s.matrix=[](void* p,BattleAudienceMatrixRequest& r){auto& f=*static_cast<Fixture*>(p);if(int(f.matrixCalls)==f.holdMatrix)return false;
            BattleAudienceAuxServices math;bindAudienceAuxMathAdapter(math);if(!math.matrix(nullptr,r))return false;f.matrices.push_back(r);++f.matrixCalls;return true;};
        s.scalar=[](void* p,const BattlePartScalarRequest& r)->std::optional<float>{auto& f=*static_cast<Fixture*>(p);++f.scalarCalls;
            if(r.function==0x80100ed0 || r.function==0x80100ffc || r.function==0x8000703c)return 0;
            if(r.function==0x80007328){float v=std::fmod(r.value,360.f);if(v<0)v+=360;return v;}return {};};
        s.status=[](void* p,uint32_t u,int32_t id)->std::optional<bool>{auto& f=*static_cast<Fixture*>(p);assert(u==U);++f.statusCalls;return id==4?f.status4:f.status16;};
        s.material=[](void* p,const BattlePartMaterialRequest& r){auto& f=*static_cast<Fixture*>(p);if(f.holdMaterial)return false;f.material.push_back(r);return true;};
        s.callback=[](void* p,uint32_t fn,uint32_t part,uint32_t mode){auto& f=*static_cast<Fixture*>(p);assert(part==P);++f.callbacks;
            if(mode==0){assert(fn==0x8001);f.word(P+0x210,0x8002);f.word(P+0x1c0,17);}else {assert(fn==0x8002);f.word(P+0x218,0xaabbccdd);}return true;};
        s.draw=[](void* p,const BattlePartDrawRequest& r,uint32_t& pending){auto& f=*static_cast<Fixture*>(p);if(f.holdDraw && r.pass==2){pending=0x8003bc68;return false;}f.draws.push_back(r);return true;};return s;
    }
};
int main(){
    assert(nativeBattlePartPhaseOffset(0,true)==0 && nativeBattlePartPhaseOffset(0,false)==0);
    // lhz phase32768 means302/294 degrees, rather than a negative signed phase.
    assert(std::fabs(nativeBattlePartPhaseOffset(32768,true)-float(2*std::sin(302*3.14159265358979323846/180.)+2))<.000001f);
    assert(std::fabs(nativeBattlePartPhaseOffset(32768,false)-float(4*std::sin(294*3.14159265358979323846/180.)+4))<.000001f);

    Fixture f;BattlePartDisplayCursor c;for(unsigned r=0;r<10;++r)for(unsigned w=0;w<17;++w)f.word(Fixture::H+0xc+r*0x44+w*4,0x10000*r+w);
    f.word(Fixture::H+4,0xaaaa);f.word(Fixture::H+8,0xbbbb);
    assert(resumePackedBattlePartDisplay(Fixture::P,f.services(),c));
    assert(f.matrixCalls==38 && f.draws.size()==3 && f.statusCalls==1 && f.material.size()==3);
    for(unsigned i=0;i<3;++i){const auto& d=f.draws[i];assert(d.pass==i+1 && d.handle==9 && d.angle==180 && d.scale==2 && d.matrix[3]==15 && d.matrix[7]==26 && d.matrix[11]==37);}
    assert(f.real(Fixture::H+0x10+11*4)==32 && f.word(Fixture::H+0x44)==0x12345678 && f.word(Fixture::H+0x48)==0xaaaa && f.word(Fixture::H+0x4c)==0xbbbb);
    for(unsigned r=1;r<10;++r)for(unsigned w=0;w<17;++w)assert(f.word(Fixture::H+0xc+r*0x44+w*4)==0x10000*(r-1)+w);
    // A held initial SDK call pins operands while following calls remain fresh.
    Fixture held;held.holdMatrix=0;BattlePartDisplayCursor h;assert(!resumePackedBattlePartDisplay(Fixture::P,held.services(),h));
    held.real(Fixture::U+0x78,100);held.holdMatrix=-1;held.holdDraw=true;
    assert(!resumePackedBattlePartDisplay(Fixture::P,held.services(),h) && h.pending==0x8003bc68 && held.matrixCalls==37 && held.draws.size()==1);
    assert(held.matrices[0].xyz[0]==0 && held.matrices[1].xyz[0]==100);
    const auto cached=held.draws[0].matrix;held.real(Fixture::U+0x3c,999);held.word(Fixture::P+0x1c0,27);held.holdDraw=false;
    assert(resumePackedBattlePartDisplay(Fixture::P,held.services(),h) && held.matrixCalls==38 && held.draws[1].handle==9 && held.draws[2].handle==27 && held.draws[1].matrix==cached);
    // Material status4 uses the NEW red for blue; alpha survives unscaled.
    Fixture poison;poison.word(Fixture::P+0x4f4,0xffffff89);poison.status4=true;poison.holdMaterial=true;BattlePartDisplayCursor p;
    assert(!resumePackedBattlePartDisplay(Fixture::P,poison.services(),p) && p.pending==0x800406b4 && poison.word(Fixture::P+0x218)==0xa0ff6489);
    poison.word(Fixture::P+0x218,0);poison.holdMaterial=false;
    assert(resumePackedBattlePartDisplay(Fixture::P,poison.services(),p) && poison.material[0].value==0xa0ff6489 && poison.statusCalls==2);
    // Fresh callbacks/handles after pre-hook, source history color after post.
    Fixture hooks;hooks.word(Fixture::P+0x210,0x8001);hooks.status16=true;BattlePartDisplayCursor k;
    assert(resumePackedBattlePartDisplay(Fixture::P,hooks.services(),k) && hooks.callbacks==2 && hooks.material[2].handle==17 && hooks.material[2].function==0x80040730);
    assert(hooks.draws[0].handle==17 && hooks.word(Fixture::H+0x44)==0xaabbccdd);
    // History shifts once even if its final matrix Copy is unavailable.
    Fixture history;history.holdMatrix=37;history.word(Fixture::P+0x1ac,0x04000000);history.word(Fixture::H,1);history.word(Fixture::H+0xc,1);history.word(Fixture::H+0x44,0x77889900);BattlePartDisplayCursor t;
    assert(!resumePackedBattlePartDisplay(Fixture::P,history.services(),t) && t.phase==27 && history.word(Fixture::H+0x88)==0x77889900);
    history.word(Fixture::H+4,123);history.word(Fixture::H+8,456);history.holdMatrix=-1;
    assert(resumePackedBattlePartDisplay(Fixture::P,history.services(),t) && history.word(Fixture::H+0xc)==0 && history.word(Fixture::H+0x48)==123 && history.word(Fixture::H+0x4c)==456 && history.draws.size()==3);
    // Inclusive mirror gate rebuilds unit Z scale; identities/calls aren't elided.
    Fixture mirror;mirror.word(Fixture::U+0x1c,0);BattlePartDisplayCursor v;assert(resumePackedBattlePartDisplay(Fixture::P,mirror.services(),v) && mirror.matrixCalls==47 && mirror.draws[0].matrix[10]==-1);
    // Source switch order is observable with noncommuting axis rotations.
    constexpr unsigned orders[6][3]{{1,2,0},{1,0,2},{0,1,2},{0,2,1},{2,0,1},{2,1,0}};
    for(unsigned order=0;order<6;++order){Fixture r;r.memory[Fixture::P+0x4fc]=order;
        r.real(Fixture::P+0x3c,23);r.real(Fixture::P+0x40,47);r.real(Fixture::P+0x44,81);
        BattlePartDisplayCursor rc;assert(resumePackedBattlePartDisplay(Fixture::P,r.services(),rc));
        for(unsigned i=0;i<3;++i)assert(r.matrices[28+i].left==r.matrices[18+orders[order][i]].output);
    }
    Fixture special;special.word(Fixture::P+0x1ac,0x20000000);BattlePartDisplayCursor sc;
    assert(!resumePackedBattlePartDisplay(Fixture::P,special.services(),sc) && special.matrixCalls==16 && special.draws.empty() && sc.pending==0x80101774);
    Fixture negative;negative.word(Fixture::P+0x1c0,0xffffffff);BattlePartDisplayCursor n;assert(resumePackedBattlePartDisplay(Fixture::P,negative.services(),n) && negative.matrixCalls==0 && negative.material.empty());
    // Only part Y performs the extra correction add; unit signed-zero is retained.
    Fixture zero;zero.real(Fixture::U+0x60,-0.f);zero.real(Fixture::U+0x6c,-0.f);BattlePartDisplayCursor z;assert(resumePackedBattlePartDisplay(Fixture::P,zero.services(),z) && std::signbit(zero.matrices[4].radians));
}
