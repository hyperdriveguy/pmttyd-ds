#include "animation_fan.hpp"
#include "animation_texture.hpp"
#include <cassert>
#include <cmath>
#include <fstream>
#include <iterator>
#include <limits>
#include <vector>
using namespace ttyd;
struct Output {
    bool hold=false;unsigned attempts=0;std::vector<std::vector<AnimationFanVertex>> fans;AnimationFanTexture texture;
    static bool submit(void* u,const AnimationFanBatch& b){auto& o=*static_cast<Output*>(u);++o.attempts;if(o.hold)return false;o.fans.emplace_back(b.vertices,b.vertices+b.count);o.texture=b.texture;return true;}
};
static AnimationShapeCall call(const BattlePose& p,uint32_t draw,uint32_t function,std::array<uint32_t,7> args={}){
    AnimationShapeCall r;r.resource=p.resource->draw;r.pose=&p;r.draw=draw;r.function=function;r.arguments=args;return r;
}
static bool configure(AnimationFanStream& stream,const BattlePose& p,uint32_t shape,uint32_t draw,const AnimationFanTexture& texture,AnimationFanSubmit out){
    const auto& d=*p.resource->draw;if(!stream.request(call(p,draw,0x802b4520),texture,out))return false;
    const uint32_t base[]{d.word(shape+0x40),d.word(shape+0x48),d.word(0x164)+4*d.word(shape+0x50),d.word(0x18c)+8*d.word(shape+0x58+8*d.byte(draw+0x30))};
    const uint32_t attr[]{9,10,11,13};
    for(unsigned i=0;i<4;++i){
        if(!stream.request(call(p,draw,0x802b3c08,{attr[i],3}),texture,out) ||
           !stream.request(call(p,draw,0x802b4558,{0,attr[i],i==1?0u:1u,i==2?5u:4u}),texture,out) ||
           !stream.request(call(p,draw,0x802b4db0,{attr[i],base[i],i<2?12u:i==2?4u:8u}),texture,out))return false;
    }return true;
}
static AnimationShapeCall vertex(const BattlePose& p,uint32_t draw,uint32_t first,uint32_t v){
    const auto& d=*p.resource->draw;auto r=call(p,draw,0xcc008000,{4});
    constexpr uint32_t ptr[]{0x158,0x160,0x168};
    for(unsigned i=0;i<3;++i)r.indices[i]=uint16_t(d.word(d.word(ptr[i])+4*(first+d.word(draw+0x40+4*i)+v)));
    const auto k=d.byte(draw+0x30);r.indices[3]=uint16_t(d.word(d.word(0x16c+4*k)+4*(first+d.word(draw+0x4c+4*k)+v)));
    return r;
}
int main(){
    const char* banks[]{"trio-attack-mario","trio-attack-goombella","professor"};
    const unsigned expectedFans[]{70,88,12},expectedVertices[]{297,308,48};
    for(unsigned b=0;b<3;++b){
        const std::string prefix=std::string("romfs/")+banks[b];auto resource=loadBattlePoseResource((prefix+".pose-directory.bin").c_str(),(prefix+".pmanim").c_str());assert(resource && resource->draw);
        BattlePosePool pool;auto h=pool.enter(resource,1,{});assert(h && pool.main(*h,{}));auto& p=*pool.pose(*h);const auto& d=*resource->draw;
        // Geometry census uses an explicit cell0 texture fixture; palette
        // selection is separately checked below against the real Goombella cell.
        AnimationFanTexture texture{resource,0,float(1u<<b),60,{}};AnimationFanStream stream;Output output;AnimationFanSubmit submit{&output,Output::submit};
        for(uint32_t si=0;si<d.word(0xe8);++si){const auto shape=d.word(0x14c)+168*si;
            for(uint32_t di=0;di<d.word(shape+0x9c);++di){const auto draw=d.word(0x19c)+108*(d.word(shape+0x98)+di);assert(configure(stream,p,shape,draw,texture,submit));
                for(uint32_t f=0;f<d.word(draw+0x3c);++f){const auto run=d.word(0x150)+8*(d.word(draw+0x38)+f),first=d.word(run),count=d.word(run+4)&65535;
                    assert(stream.request(call(p,draw,0x802b5a6c,{0xa0,0,count}),texture,submit));
                    for(uint32_t v=0;v<count;++v){const auto r=vertex(p,draw,first,v);assert(stream.request(r,texture,submit));}
                    assert(!stream.active() && output.fans.back().size()==count);
                    for(uint32_t v=0;v<count;++v){const auto r=vertex(p,draw,first,v);const auto& actual=output.fans.back()[v];
                        assert(actual.position==p.shape.positions[d.word(shape+0x40)+r.indices[0]] && actual.normal==p.shape.normals[d.word(shape+0x48)+r.indices[1]]);
                        const uint32_t uv=d.word(0x18c)+8*(d.word(shape+0x58+8*d.byte(draw+0x30))+r.indices[3]);
                        assert(std::fabs(actual.uv[0]-(2+124*d.real(uv))/1024)<1e-7f);
                        assert(std::fabs(actual.uv[1]-(1-(2+124*d.real(uv+4))*texture.uvScale/1024))<1e-7f);
                    }
                }
            }
        }
        assert(stream.submittedFans()==expectedFans[b] && stream.submittedVertices()==expectedVertices[b]);
    }
    auto resource=loadBattlePoseResource("romfs/trio-attack-goombella.pose-directory.bin","romfs/trio-attack-goombella.pmanim");BattlePosePool pool;auto h=pool.enter(resource,1,{});assert(h && pool.main(*h,{}));auto& p=*pool.pose(*h);const auto& d=*resource->draw;
    const auto shape=d.word(0x14c)+168*15,draw=d.word(0x19c)+108*15;AnimationFanStream stream;Output output;AnimationFanSubmit submit{&output,Output::submit};
    AnimationTextureRequest image;image.pose=&p;image.slot=d.word(draw+0x10);image.state=p.shape.textures[image.slot];image.descriptor=d.word(d.word(0x194)+8*image.slot)+image.state;image.image.index=d.word(d.word(0x198)+64*image.descriptor+4);auto cell=animationTextureCell(image);assert(cell && *cell==0);
    AnimationFanTexture texture{resource,*cell,2,30,BattleActMatrix{1,0,.25f,-.25f,0,1,-.125f,.125f,0,0,1,0}}; // AB11 constant columns cancel, z=0 would be wrong.
    assert(configure(stream,p,shape,draw,texture,submit));assert(stream.request(call(p,draw,0x802b5a6c,{0xa0,0,3}),texture,submit));
    const auto first=d.word(d.word(0x150)+8*66);auto a=vertex(p,draw,first,0),b=vertex(p,draw,first,1),c=vertex(p,draw,first,2);
    assert(a.indices[0]==7 && a.indices[1]==235 && a.indices[2]==235 && a.indices[3]==7);
    assert(stream.request(a,texture,submit) && stream.request(b,texture,submit));output.hold=true;assert(!stream.request(c,texture,submit) && stream.stagedVertices()==3 && stream.submittedFans()==0);
    const auto old=p.shape.positions[114];p.shape.positions[114][0]+=999;texture.cell=10;
    auto changed=c;++changed.indices[0];assert(!stream.request(changed,texture,submit) && output.attempts==1);
    output.hold=false;assert(stream.request(c,texture,submit) && stream.submittedFans()==1 && stream.submittedVertices()==3 && output.fans[0][0].position==old && output.texture.cell==0);
    assert(std::fabs(output.fans[0][0].uv[0]-(2+124*.5930250883f)/1024)<1e-7f);p.shape.positions[114]=old;
    assert(!stream.request(c,texture,submit)); // No duplicate final FIFO acknowledgement without a new Begin.
    assert(stream.request(call(p,draw,0x802b5a6c,{0xa0,0,0}),texture,submit) && !stream.active());
    assert(!stream.request(call(p,draw,0x802b5a6c,{0xa0,0,65}),texture,submit));texture.resource.reset();assert(!stream.request(call(p,draw,0x802b5a6c,{0xa0,0,3}),texture,submit));texture.resource=resource;
    texture.generator=60;texture.matrix=BattleActMatrix{};texture.cell=10;
    assert(stream.request(call(p,draw,0x802b5a6c,{0xa0,0,3}),texture,submit));
    auto invalid=a;invalid.indices[1]=65535;assert(!stream.request(invalid,texture,submit) && stream.stagedVertices()==0);
    const auto oldNormal=p.shape.normals[235];p.shape.normals[235][0]=std::numeric_limits<float>::quiet_NaN();assert(!stream.request(a,texture,submit));p.shape.normals[235]=oldNormal;
    assert(stream.request(a,texture,submit) && stream.request(b,texture,submit) && stream.request(c,texture,submit));assert(output.texture.cell==10);
    assert(std::fabs(output.fans.back()[0].uv[1]-(1-(128+2+124*.25f)*2/1024))<1e-7f); // Identity60 ignores stale zero matrix, one V correction.
    stream.reset();assert(!stream.request(a,texture,submit) && !stream.submittedFans());
    // Original RGBA bytes, not flattened animation colors or host-endian words.
    std::ifstream file("romfs/trio-attack-goombella.draw-model.bin",std::ios::binary);std::vector<uint8_t> bytes((std::istreambuf_iterator<char>(file)),{});const auto co=d.word(0x164)+4*235;bytes[co]=0x12;bytes[co+1]=0x34;bytes[co+2]=0x56;bytes[co+3]=0x78;
    FILE* f=std::tmpfile();assert(f && std::fwrite(bytes.data(),bytes.size(),1,f)==1);std::rewind(f);auto raw=std::make_shared<AnimationDrawResource>();assert(raw->read(f));std::fclose(f);auto altered=std::make_shared<BattlePoseResource>(*resource);altered->draw=raw;p.resource=altered;texture.resource=altered;texture.generator=60;texture.cell=0;
    assert(configure(stream,p,shape,draw,texture,submit));assert(stream.request(call(p,draw,0x802b5a6c,{0xa0,0,3}),texture,submit));assert(stream.request(vertex(p,draw,first,0),texture,submit) && stream.request(vertex(p,draw,first,1),texture,submit) && stream.request(vertex(p,draw,first,2),texture,submit));
    assert((output.fans.back()[0].color==std::array<uint8_t,4>{0x12,0x34,0x56,0x78}));
}
