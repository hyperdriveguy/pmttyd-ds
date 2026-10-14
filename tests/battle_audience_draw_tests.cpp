#include "battle_audience_draw.hpp"
#include "battle_audience_backend.hpp"
#include "event_script_assets.hpp"
#include "trio_attack.hpp"
#include "game.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
using namespace ttyd;
struct Fixture {
    static constexpr uint32_t B=0x10000,A=B+0x2858;
    EventScriptAssets original;BattleAudienceResources assets;BattleAudienceBackend backend;
    std::map<uint32_t,uint8_t> bytes;std::vector<BattleAudienceQuad> quads;
    uint8_t mode=1;unsigned cameraCalls=0;bool available=true;
    std::array<float,6> camera{0,0,100,0,0,0};
    Fixture(){assert(original.load("romfs/trio-attack-events.pmevt") && assets.loadFrames("romfs"));backend.assets=&assets;
        word(0x8041eb90,B);word(A+0xc,1);word(A+0x10,2);bytes[A+0x18]=12;}
    uint32_t r(unsigned i){return A+0x1bc+0x134*i;}
    uint32_t word(uint32_t p){if(bytes.count(p)||bytes.count(p+1)||bytes.count(p+2)||bytes.count(p+3)){uint32_t v=0;for(unsigned i=0;i<4;++i)v=v<<8|bytes[p+i];return v;}
        uint32_t v=0;if(p>=0x80000000)assert(original.readWord(p,v));return v;}
    uint8_t byte(uint32_t p){return p>=0x80000000?uint8_t(word(p&~3u)>>(24-8*(p&3))):bytes[p];}
    void word(uint32_t p,uint32_t v){for(unsigned i=0;i<4;++i)bytes[p+i]=v>>(24-8*i);}
    void real(uint32_t p,float v){uint32_t b;std::memcpy(&b,&v,4);word(p,b);}
    float real(uint32_t p){auto b=word(p);float v;std::memcpy(&v,&b,4);return v;}
    void member(unsigned i,uint8_t type=10,uint8_t alpha=255){const auto p=r(i);word(p,5);bytes[p+0x1b]=type;bytes[p+0x1a]=173;bytes[p+0x44]=alpha;
        word(p+0x20,0x60000);word(p+0x24,0x70000);for(unsigned j=0;j<10;++j)word(0x60000+4*j,0x71000+4*j);
        real(p+0x48,20);real(p+0x4c,30);real(p+0x50,40);real(p+0x5c,1);real(p+0x60,1);real(p+0x64,1);real(p+0x70,0);real(p+0xe8,270);
    }
    BattleAudienceDrawServices services(){BattleAudienceDrawServices s;s.user=this;s.actors.user=this;s.actors.memory.user=this;bindAudienceMathAdapters(s);
        s.actors.byte=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->byte(a);};
        s.actors.memory.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->word(a);};
        s.actors.memory.storeWord=[](void* p,uint32_t a,uint32_t v){static_cast<Fixture*>(p)->word(a,v);};
        s.camera=[](void* p,uint32_t i)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);++f.cameraCalls;assert(i==4);return 4;};
        s.vectors=[](void* p,uint32_t)->std::optional<std::array<float,6>>{return static_cast<Fixture*>(p)->camera;};
        s.view=[](void*,uint32_t)->std::optional<BattleActMatrix>{return BattleActMatrix{1,0,0,0,0,1,0,0,0,0,1,0};};
        s.mapMode=[](void* p)->std::optional<uint8_t>{return static_cast<Fixture*>(p)->mode;};
        s.texture=[](void* p,uint32_t bank,uint8_t index)->std::optional<BattleAudienceTexture>{auto& f=*static_cast<Fixture*>(p);if(!f.available)return {};const auto* image=f.assets.frame(bank,index);if(!image)return {};return BattleAudienceTexture{bank,index,image->width,image->height};};
        s.request=[](void* p,const BattleAudienceGXRequest& q){return static_cast<Fixture*>(p)->backend.request(q);};
        s.quad=[](void* p,const BattleAudienceQuad& q){auto& f=*static_cast<Fixture*>(p);f.quads.push_back(q);return f.backend.emitQuad(q);};return s;}
};
int main(){
    Fixture f;f.member(0);f.member(1,10,128);auto s=f.services();
    assert(drawPackedBattleAudience(s) && f.quads.size()==1 && f.backend.vertices.size()==6);
    // Every original UV template preserves the entire source40x40 quad and
    // stays within its own atlas cell, including the overshooting end pose.
    for(unsigned group=0;group<4;++group){
        auto templateQuad=f.quads[0];templateQuad.matrix={1,0,0,0,0,1,0,0,0,0,1,0};templateQuad.rgba=0x11223377;
        for(unsigned i=0;i<4;++i){uint32_t u=0,v=0;assert(f.original.readWord(0x802fa024+group*32+i*8,u) && f.original.readWord(0x802fa028+group*32+i*8,v));
            std::memcpy(&templateQuad.points[i].u,&u,4);std::memcpy(&templateQuad.points[i].v,&v,4);}
        f.backend.begin();assert(f.backend.emitQuad(templateQuad));float area=0;
        const auto* region=f.assets.frame(templateQuad.texture.bank,templateQuad.texture.index);assert(region);
        for(size_t i=0;i<f.backend.vertices.size();i+=3){const auto& a=f.backend.vertices[i];const auto& b=f.backend.vertices[i+1];const auto& c=f.backend.vertices[i+2];
            area+=std::fabs(a.x*(b.y-c.y)+b.x*(c.y-a.y)+c.x*(a.y-b.y))*.5f;}
        assert(std::fabs(area-1600)<.01f);
        for(const auto& vertex:f.backend.vertices){assert(vertex.rgba==templateQuad.rgba && vertex.u>=region->uv[0] && vertex.u<=region->uv[2] && vertex.v<=region->uv[1] && vertex.v>=region->uv[3]);}
    }
    const auto& q=f.quads[0];assert(q.slot==0 && q.texture.bank==1 && q.texture.index==173 && q.rgba==255);
    assert(q.points[0].x==-20 && q.points[0].y==40 && q.points[2].x==20 && q.points[2].y==0);
    assert(q.matrix[3]==20 && q.matrix[7]==30 && q.matrix[11]==40);
    assert(f.real(f.r(0)+0x54)==1 && f.real(f.r(0)+0x58)==1 && f.real(f.r(0)+0x98)==-1);
    f.mode=2;f.backend.begin();f.quads.clear();assert(drawPackedBattleAudience(s) && f.quads.size()==1 && f.quads[0].slot==1 && f.quads[0].rgba==128);
    // No ad hoc GetExist filter: flags1+4 with bit2/100 still draw; missing1/4 skips.
    Fixture flags;for(unsigned i=0;i<4;++i)flags.member(i);flags.word(flags.r(0),1);flags.word(flags.r(1),4);flags.word(flags.r(2),7);flags.word(flags.r(3),0x105);
    assert(drawPackedBattleAudience(flags.services()) && flags.quads.size()==2 && flags.quads[0].slot==2 && flags.quads[1].slot==3);
    // Original rotation/normal producer, pivot and hover translations; top warp affects top vertices only.
    Fixture transformed;transformed.member(0,2);transformed.real(transformed.r(0)+0x9c,90);transformed.real(transformed.r(0)+0x130,7);
    transformed.real(transformed.r(0)+0x6c,2);transformed.real(transformed.r(0)+0x74,4);
    assert(drawPackedBattleAudience(transformed.services()));const auto& rotated=transformed.quads[0];
    assert(std::fabs(transformed.real(transformed.r(0)+0x94)-1)<1e-5);
    assert(std::fabs(rotated.matrix[7]-37)<1e-5 && rotated.points[0].x==-18 && rotated.points[0].z==4 && rotated.points[2].z==0);
    // Back/front toggles the ordinary UV group; exempt animations retain their group.
    Fixture back;back.member(0);back.camera={0,0,-100,0,0,0};assert(drawPackedBattleAudience(back.services()));
    assert(back.quads[0].points[0].u==1 && back.quads[0].points[1].u==0);
    // End-animation UVs extend to1.075: native clamp subdivision retains ALL area.
    Fixture ending;ending.member(0);ending.word(ending.r(0)+0x24,ending.word(0x60000+0x24));assert(drawPackedBattleAudience(ending.services()));
    assert(ending.quads[0].points[0].v>1 && ending.backend.vertices.size()>6);
    double area=0;for(unsigned i=0;i<ending.backend.vertices.size();i+=3){const auto& a=ending.backend.vertices[i];const auto& b=ending.backend.vertices[i+1];const auto& c=ending.backend.vertices[i+2];area+=std::abs((b.x-a.x)*(c.y-a.y)-(b.y-a.y)*(c.x-a.x))*.5;
        const auto* frame=ending.assets.frame(1,173);for(unsigned j=0;j<3;++j){const auto& v=ending.backend.vertices[i+j];assert(v.u>=frame->uv[0] && v.u<=frame->uv[2] && v.v>=frame->uv[3] && v.v<=frame->uv[1]);}}
    assert(std::fabs(area-1600)<.001);
    // Negative exact multiples retain original GPR flip after normalization to360.
    Fixture angles;angles.member(0);angles.member(1);angles.real(angles.r(0)+0xe8,45);angles.real(angles.r(1)+0xe8,-360);
    assert(drawPackedBattleAudience(angles.services()) && angles.real(angles.r(1)+0xe8)==360 && angles.quads[1].points[0].u==1);
    Fixture unwritten;unwritten.member(0);unwritten.real(unwritten.r(0)+0xe8,-360);assert(!drawPackedBattleAudience(unwritten.services()) && unwritten.real(unwritten.r(0)+0xe8)==360 && unwritten.quads.empty());
    // Guest matching uses source bank slots; an unmatched guest retains prior texture and own scale.
    Fixture guest;guest.member(0);guest.member(1,12);guest.bytes[guest.r(1)+0x1a]=0;
    assert(drawPackedBattleAudience(guest.services()) && guest.quads[1].texture.bank==2);
    Fixture retained;retained.member(0);retained.member(1,13);retained.real(retained.r(1)+0x54,2);retained.real(retained.r(1)+0x58,3);
    assert(drawPackedBattleAudience(retained.services()) && retained.quads[1].texture.bank==1 && retained.real(retained.r(1)+0x54)==2);
    Fixture absent;absent.member(0);absent.available=false;assert(!drawPackedBattleAudience(absent.services()) && absent.quads.empty());
    assert(loadProfessorStoryAssets("romfs") && loadUndergroundStoryAssets("romfs") && loadTrioBattleAssets("romfs") && loadTrioAttackAssets("romfs"));
    ProfessorStory scene;assert(scene.beginTrioBattleIntro(0));TrioBattleCommands menu;assert(menu.begin(scene));menu.input(0x100,0);menu.input(0x100,0);
    auto pouch=createProloguePouch();assert(pouch);pouch->half(0x8c,1);pouch->bytes[0x84]=0x41;pouch->bytes[0x85]=pouch->bytes[0x86]=pouch->bytes[0x87]=0;
    TrioAttack native;assert(native.begin(menu,scene,10,pouch));Fixture packets;
    assert(native.prepareAudienceDrawing(packets.services()) && packets.quads.size()==8);
    const auto memberAddress=0x84002858+0x1bc+packets.quads[0].slot*0x134;
    uint32_t instruction=0,again=0,appearance=0;assert(native.readWord(memberAddress+0x28,instruction));
    assert(native.readWord(memberAddress+0x18,appearance) && ((appearance>>8)&255)==packets.quads[0].texture.index);
    packets.mode=2;assert(native.prepareAudienceDrawing(packets.services()));
    assert(native.readWord(memberAddress+0x28,again) && again==instruction); // Same timestamp/second pass does not advance script.
    native.tick(17,scene);packets.mode=1;packets.quads.clear();packets.backend.begin();
    assert(native.prepareAudienceDrawing(packets.services()) && !packets.quads.empty());
    assert(native.readWord(memberAddress+0x18,appearance) && ((appearance>>8)&255)!=0); // Next original opcode supplies its frame.


}
