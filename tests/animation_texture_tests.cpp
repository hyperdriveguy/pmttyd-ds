#include "animation_texture.hpp"
#include <cassert>
#include <cmath>
#include <vector>
#include <fstream>
#include <iterator>
#include <cstring>
using namespace ttyd;
struct Trace {
    uint32_t hold=0;bool ci=false;int mathHold=-1;unsigned gets=0;
    std::vector<AnimationTextureRequest> calls;std::vector<BattleAudienceMatrixRequest> math;
    static bool get(void* u,const BattlePose& p,uint32_t index,AnimationTPLImage& out){auto& t=*static_cast<Trace*>(u);++t.gets;const auto i=p.resource->textures->image(index);assert(i);out=*i;if(t.ci){out.paletteHeader=123;out.paletteCount=16;out.paletteData=456;}return true;}
    static bool gx(void* u,const AnimationTextureRequest& r){auto& t=*static_cast<Trace*>(u);if(r.function==t.hold)return false;t.calls.push_back(r);return true;}
    static bool matrix(void* u,BattleAudienceMatrixRequest& r){auto& t=*static_cast<Trace*>(u);if(t.mathHold==int(t.math.size()))return false;BattleAudienceAuxServices s;bindAudienceAuxMathAdapter(s);assert(s.matrix(nullptr,r));t.math.push_back(r);return true;}
};
int main(){
    const auto resource=loadBattlePoseResource("romfs/trio-attack-goombella.pose-directory.bin","romfs/trio-attack-goombella.pmanim");assert(resource && resource->draw && resource->textures);
    const auto image=resource->textures->image(44);assert(image && image->width==40 && image->height==32 && image->format==14 && !image->paletteHeader && image->minFilter==1 && image->magFilter==1 && image->bias==0);
    assert(!resource->textures->image(resource->textures->count()));
    BattlePosePool pool;const auto handle=pool.enter(resource,1,{});assert(handle && pool.main(*handle,{}));auto& pose=*pool.pose(*handle);
    const auto draw=resource->draw->word(0x19c)+108*15;Trace t;t.hold=0x802b7880;
    AnimationTextureServices s;s.user=&t;s.get=Trace::get;s.gx=Trace::gx;s.matrix=Trace::matrix;AnimationTextureCursor c;
    assert(!resumeAnimationTexture(pose,draw,s,c) && c.pending==0x802b7880 && t.gets==2 && t.calls.size()==2 && t.math.empty());
    assert(t.calls[0].function==0x802b72b0 && t.calls[0].descriptor==44 && t.calls[0].slot==9 && !t.calls[0].wrapS && !t.calls[0].wrapT);
    pose.shape.textures[9]=1;t.hold=0;
    assert(resumeAnimationTexture(pose,draw,s,c) && t.gets==2 && t.calls.size()==5 && t.math.size()==1);
    assert(t.calls[2].function==0x802b7880 && t.calls[2].state==0 && animationTextureCell(t.calls[2])==0);
    assert(t.math[0].function==0x8029e230 && t.math[0].xyz==ActorVector({0,0,0}));
    assert(t.calls[3].function==0x802b4e04 && t.calls[3].arguments[3]==30 && t.calls[4].function==0x802ba3d0 && t.calls[4].matrix[0]==1 && t.calls[4].matrix[5]==1);
    // Explicit source state1 needs the selected package's admitted palette.
    assert(!animationTextureCell(AnimationTextureRequest{0}));
    assert(pool.selectName(*handle,"PKR_D_1",true,{}) && pool.main(*handle,{}));pose.shape.textures[9]=1;t={};c={};
    assert(resumeAnimationTexture(pose,draw,s,c) && t.calls[2].descriptor==45 && animationTextureCell(t.calls[2])==10);
    // Captured finishing queue request: Frankly descriptor0 Z_1, slot11,
    // original image17 -> package cell9. Its atlas retains full 1024 width.
    const auto frankly=loadBattlePoseResource("romfs/professor.pose-directory.bin","romfs/professor.pmanim");
    assert(frankly && frankly->modelName=="c_kuri_h");
    BattlePosePool npcs;const auto npc=npcs.enter(frankly,1,{});assert(npc && npcs.main(*npc,{}));
    const auto* professor=npcs.pose(*npc);assert(professor && professor->word(0x14)==0);
    AnimationTextureRequest held;held.function=0x802b7880;held.pose=professor;held.slot=11;held.state=0;held.descriptor=17;
    held.image=*frankly->textures->image(17);assert(animationTextureCell(held)==9);
    assert(loadBattlePoseResource("romfs/professor.pose-directory.bin","romfs/professor.pmanim")==frankly);
    // Use actual conversion sidecars rather than inferring atlas scale from a
    // model name or substituting Mario's resident layout.
    auto atlasScale=[](const char* path,const char* magic){std::ifstream f(path,std::ios::binary);char tag[8];float scale=0;f.read(tag,8);f.read(reinterpret_cast<char*>(&scale),4);assert(f && std::memcmp(tag,magic,8)==0 && f.peek()==EOF);return scale;};
    const auto franklyScale=atlasScale("romfs/professor.bin","PMPNPC1");
    const auto marioScale=atlasScale("romfs/trio-attack-mario.bin","PMSUV1\0");
    const auto partnerScale=atlasScale("romfs/trio-attack-goombella.bin","PMSUV1\0");
    assert(franklyScale==4 && animationAtlasDimensions(1024,256,franklyScale));
    assert(animationAtlasDimensions(512,512,marioScale) && animationAtlasDimensions(512,256,partnerScale));
    assert(!animationAtlasDimensions(512,512,franklyScale) && !animationAtlasDimensions(1024,256,partnerScale));
    assert(!animationAtlasDimensions(256,64,franklyScale) && !animationAtlasDimensions(1024,256,0));
    // Original CI branch request order, supplied by an explicit test provider.
    t={};t.ci=true;c={};assert(resumeAnimationTexture(pose,draw,s,c));
    const uint32_t expected[]{0x802b78d4,0x802b74fc,0x802b7544,0x802b790c,0x802b7880,0x802b4e04,0x802ba3d0};
    assert(t.gets==2 && t.calls.size()==7);for(unsigned i=0;i<7;++i)assert(t.calls[i].function==expected[i]);
    // Capture all five live lanes before the SDK graph. A held concat never
    // recaptures a changed pose or replays its preceding translation/rotation.
    pose.shape.surfaces.resize(pose.shape.textures.size());pose.shape.surfaces[9].transform={.1f,.4f,2,3,45};t={};t.mathHold=2;c={};
    assert(!resumeAnimationTexture(pose,draw,s,c) && c.pending==0x8029dd64 && t.math.size()==2);
    pose.shape.surfaces[9].transform={9,9,9,9,9};t.mathHold=-1;
    assert(resumeAnimationTexture(pose,draw,s,c) && t.math.size()==9);
    assert(t.math[1].radians==(-45.f)*.01745329238474369f && t.math[5].xyz[1]==-2.4f);
    const auto& m=t.calls.back().matrix;assert(std::fabs(m[0]-1.41421356f)<.000001f && std::fabs(m[1]-2.12132034f)<.000001f && std::fabs(m[3]+1.83345238f)<.000002f);
    // Literal sy gate: zero translation and sy0 suppress translation, but
    // still request Scale(1,0,1), followed by generator-before-matrix upload.
    pose.shape.surfaces[9].transform={0,0,1,0,0};t={};c={};assert(resumeAnimationTexture(pose,draw,s,c) && t.math.size()==1 && t.math[0].function==0x8029e2b0);
    // Explicit two-slot fixture: load objects in reversed slot order, skip
    // kind1's image, then generate coordinates for BOTH after object loading.
    std::ifstream original("romfs/trio-attack-goombella.draw-model.bin",std::ios::binary);
    std::vector<uint8_t> bytes((std::istreambuf_iterator<char>(original)),{});
    auto put=[&](uint32_t o,uint32_t v){for(unsigned i=0;i<4;++i)bytes[o+i]=uint8_t(v>>(24-8*i));};
    pose.shape.surfaces.clear();pose.shape.textures[9]=0;
    const auto& rawDraw=*resource->draw;const auto other=rawDraw.word(rawDraw.word(0x194)+8*8)+pose.shape.textures[8];
    put(draw,2);bytes[draw+0x31]=0;put(draw+0x10,8);put(draw+0x14,9);put(rawDraw.word(0x198)+64*other+8,1);
    FILE* fixture=std::tmpfile();assert(fixture && std::fwrite(bytes.data(),bytes.size(),1,fixture)==1);std::rewind(fixture);
    auto changed=std::make_shared<AnimationDrawResource>();assert(changed->read(fixture));std::fclose(fixture);
    auto owner=std::make_shared<BattlePoseResource>(*resource);owner->draw=changed;pose.resource=owner;t={};c={};
    assert(resumeAnimationTexture(pose,draw,s,c) && t.gets==2 && t.calls.size()==7 && t.math.size()==2);
    assert(t.calls[2].function==0x802b7880 && t.calls[2].slot==9 && t.calls[3].function==0x802b4e04 && t.calls[3].slot==9);
    assert(t.calls[5].function==0x802b4e04 && t.calls[5].slot==8 && t.calls[5].arguments[3]==33 && t.calls[6].function==0x802ba3d0);
    pose.resource=resource;
    FILE* bad=std::tmpfile();assert(bad);const uint8_t raw[12]{};assert(std::fwrite(raw,12,1,bad)==1);std::rewind(bad);AnimationTextureResource invalid;assert(!invalid.read(bad));std::fclose(bad);
}
