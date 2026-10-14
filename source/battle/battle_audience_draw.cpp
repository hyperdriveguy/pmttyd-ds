#include "battle_audience_draw.hpp"
#include <cstring>
namespace ttyd {
namespace {
uint32_t base(const BattleActorServices& a){return currentBattleAddress(a)+0x2858;}
uint32_t record(int32_t i,const BattleActorServices& a){return base(a)+0x1bc+uint32_t(i)*0x134;}
float scalar(uint32_t p,const BattleActorServices& a){const auto b=a.memory.word(a.memory.user,p);float v;std::memcpy(&v,&b,4);return v;}
uint32_t bits(float v){uint32_t b;std::memcpy(&b,&v,4);return b;}
}
std::optional<int32_t> packedBattleAudienceFront(int32_t slot,const BattleAudienceDrawServices& s){
    (void)base(s.actors);const auto r=record(slot,s.actors);
    if(!s.camera || !s.vectors || !s.fma)return {};
    const auto camera=s.camera(s.user,4);if(!camera)return {};
    const auto values=s.vectors(s.user,*camera);if(!values)return {};
    const float dy=(*values)[4]-(*values)[1],dx=(*values)[3]-(*values)[0];
    const float y=dy*scalar(r+0x94,s.actors),dz=(*values)[5]-(*values)[2];
    const float xy=s.fma(s.user,dx,scalar(r+0x90,s.actors),y);
    const float dot=s.fma(s.user,dz,scalar(r+0x98,s.actors),xy);
    return dot>=scalar(0x80424988,s.actors)?0:1;
}
bool drawPackedBattleAudience(const BattleAudienceDrawServices& s){
    const auto& a=s.actors;const auto& m=a.memory;const auto owner=base(a);
    if(!s.camera)return false;const auto camera=s.camera(s.user,4);if(!camera)return false;
    const auto gx=[&](uint32_t fn,std::array<int32_t,7> args={}){return s.request && s.request(s.user,{fn,args});};
    const BattleAudienceGXRequest setup[]{
        {0x802b5c8c,{0}},{0x802b4520,{}},{0x802b3c08,{9,1}},{0x802b3c08,{13,1}},
        {0x802b4558,{0,9,1,4,0}},{0x802b4558,{0,13,1,4,0}},
        {0x802b6f3c,{1}},{0x802b6f78,{4,0,1,0,0,2,2}},
        {0x802b5084,{1}},{0x802b4e04,{0,1,4,60,0,125}},
        {0x802b8fcc,{1}},{0x802b89e8,{0,0,0,0,1,0}},
        {0x802b8960,{0,15,12,8,10}},{0x802b8a50,{0,0,0,0,1,0}},
        {0x802b89a4,{0,7,5,4,7}},{0x802b8e30,{0,0,0,4}}
    };
    for(const auto& request:setup)if(!s.request || !s.request(s.user,request))return false;
    auto r=record(0,a);const float half=scalar(0x80424a18,a),negative=-half;
    std::optional<BattleAudienceTexture> loaded;
    std::optional<uint32_t> flip; // Original GPR27 is initially unwritten, unlike FPR27.
    for(unsigned slot=0;slot<200;++slot,r+=0x134){
        if(!s.mapMode)return false;const auto mode=s.mapMode(s.user);if(!mode)return false;
        const auto alpha=a.byte(a.user,r+0x44);
        if(*mode==2?alpha==255:alpha!=255)continue;
        const auto flags=m.word(m.user,r);if(!(flags&1) || !(flags&4))continue;
        uint32_t bank=0;bool found=false;
        const auto type=a.byte(a.user,r+0x1b);
        if(type<=11){bank=m.word(m.user,owner+0xc);found=true;}
        else for(unsigned i=0;i<2;++i)if(type==a.byte(a.user,owner+0x18+i)){bank=m.word(m.user,owner+0x10+4*i);found=true;break;}
        if(found){
            if(!s.texture)return false;const auto texture=s.texture(s.user,bank,a.byte(a.user,r+0x1a));if(!texture)return false;
            if(!gx(0x802b7880,{0}))return false;loaded=*texture;
            const auto width=float(uint16_t(texture->width));
            const auto widthType=a.byte(a.user,r+0x1b);
            const float widthBase=scalar(0x803b4dfc+32*widthType,a)*width;
            const float widthRatio=widthBase*scalar(0x80424a1c,a);
            m.storeWord(m.user,r+0x54,bits(scalar(r+0x60,a)*widthRatio));
            const auto height=float(uint16_t(texture->height));
            const auto heightType=a.byte(a.user,r+0x1b);
            const float heightBase=scalar(0x803b4e00+32*heightType,a)*height;
            const float heightRatio=heightBase*scalar(0x80424a1c,a);
            m.storeWord(m.user,r+0x58,bits(scalar(r+0x64,a)*heightRatio));
        }
        const float sx=scalar(r+0x54,a),sy=scalar(r+0x58,a),sz=scalar(r+0x5c,a);
        const std::array<float,3> pivot{scalar(r+0xa8,a)*sx,scalar(r+0xac,a)*sy,scalar(r+0xb0,a)*sz};
        if(!s.scale)return false;auto scaled=s.scale(s.user,sx,sy,sz);if(!scaled)return false;
        const float angleX=scalar(r+0x9c,a)*scalar(0x80424a20,a);
        const float angleY=scalar(r+0xa0,a)*scalar(0x80424a20,a);
        const float angleZ=scalar(r+0xa4,a)*scalar(0x80424a20,a);
        if(!s.sine || !s.cosine || !s.fma)return false;
        const auto sx64=s.sine(s.user,double(angleX));if(!sx64)return false;const float sinX=float(*sx64);
        const auto cx64=s.cosine(s.user,double(angleX));if(!cx64)return false;const float cosX=float(*cx64);
        const auto sy64=s.sine(s.user,double(angleY));if(!sy64)return false;const float sinY=float(*sy64);
        const auto cy64=s.cosine(s.user,double(angleY));if(!cy64)return false;const float cosY=float(*cy64);
        const auto sz64=s.sine(s.user,double(angleZ));if(!sz64)return false;const float sinZ=float(*sz64);
        const auto cz64=s.cosine(s.user,double(angleZ));if(!cz64)return false;const float cosZ=float(*cz64);
        const float xy=sinX*sinY,xc=sinX*cosY;
        const float xyZ=xy*sinZ,xcZ=xc*sinZ;
        BattleActMatrix rotation{
            s.fma(s.user,cosY,cosZ,-xyZ),s.fma(s.user,-cosY,sinZ,-(xy*cosZ)),-cosX*sinY,scalar(0x80424988,a),
            cosX*sinZ,cosX*cosZ,-sinX,scalar(0x80424988,a),
            s.fma(s.user,sinY,cosZ,xcZ),s.fma(s.user,-sinY,sinZ,xc*cosZ),cosX*cosY,scalar(0x80424988,a)
        };
        const auto nx=m.word(m.user,0x802fa018),ny=m.word(m.user,0x802fa01c);
        m.storeWord(m.user,r+0x90,nx);m.storeWord(m.user,r+0x94,ny);m.storeWord(m.user,r+0x98,m.word(m.user,0x802fa020));
        if(!s.vector)return false;const auto normal=s.vector(s.user,rotation,{scalar(r+0x90,a),scalar(r+0x94,a),scalar(r+0x98,a)});if(!normal)return false;
        for(unsigned i=0;i<3;++i)m.storeWord(m.user,r+0x90+4*i,bits((*normal)[i]));
        (*scaled)[3]+=pivot[0];(*scaled)[7]+=pivot[1];(*scaled)[11]+=pivot[2];
        if(!s.concat)return false;const auto rotated=s.concat(s.user,rotation,*scaled);if(!rotated)return false;auto world=*rotated;
        world[3]+=scalar(r+0xcc,a)+(-pivot[0]+scalar(r+0x48,a));
        world[11]+=scalar(r+0xd4,a)+(-pivot[2]+scalar(r+0x50,a));
        float y=scalar(r+0xd0,a)+(-pivot[1]+scalar(r+0x4c,a));
        if(a.byte(a.user,r+0x1b)==2)y+=scalar(r+0x130,a);
        world[7]+=y;
        if(!s.view)return false;const auto view=s.view(s.user,*camera);if(!view)return false;
        const auto matrix=s.concat(s.user,*view,world);if(!matrix)return false;
        if(!gx(0x802ba2fc,{0}))return false;
        std::array<float,32> uv{};for(unsigned i=0;i<32;++i)uv[i]=scalar(0x802fa024+4*i,a);
        const auto front=packedBattleAudienceFront(int32_t(slot),s);if(!front)return false;
        unsigned uvBase=m.word(m.user,r+0x24)==m.word(m.user,m.word(m.user,r+0x20)+0x24)?8:0;
        const uint32_t color=uint32_t(a.byte(a.user,r+0x41))<<24|uint32_t(a.byte(a.user,r+0x42))<<16|
            uint32_t(a.byte(a.user,r+0x43))<<8|a.byte(a.user,r+0x44);
        const float full=scalar(0x80424a24,a);
        float angle=scalar(r+0xe8,a);
        if(angle>=full){if(!s.truncate)return false;const auto q=s.truncate(s.user,angle/full);if(!q)return false;
            m.storeWord(m.user,r+0xe8,bits(s.fma(s.user,-full,float(*q),angle)));}
        angle=scalar(r+0xe8,a);
        if(angle<scalar(0x80424988,a)){if(!s.truncate)return false;const auto q=s.truncate(s.user,angle/full);if(!q)return false;
            const uint32_t multiplier=1u-uint32_t(*q);int32_t signedMultiplier;std::memcpy(&signedMultiplier,&multiplier,4);
            m.storeWord(m.user,r+0xe8,bits(s.fma(s.user,full,float(signedMultiplier),angle)));}
        angle=scalar(r+0xe8,a);const float zero=scalar(0x80424988,a),middle=scalar(0x80424a28,a);
        if(angle>zero && angle<=middle)flip=1;
        if(angle==zero || (angle>middle && angle<full))flip=0;
        if(uint8_t(*front)==1){
            const auto table=m.word(m.user,r+0x20),animation=m.word(m.user,r+0x24);
            if(animation!=m.word(m.user,table+0x24) && animation!=m.word(m.user,table+0x20) && animation!=m.word(m.user,table+0x1c)){
                if(!flip)return false;*flip^=1;
            }
        }
        if(!flip)return false;if(*flip==1)uvBase+=4;
        if(!gx(0x802b6e54,{4,int32_t(color)}) || !gx(0x802b5a6c,{0x80,0,4}))return false;
        const float top=scalar(0x80424a00,a);
        BattleAudienceQuad quad;quad.slot=slot;quad.matrix=*matrix;quad.rgba=color;
        quad.points={BattleAudiencePoint{negative+scalar(r+0x6c,a),top+scalar(r+0x70,a),scalar(r+0x74,a),uv[2*uvBase],uv[2*uvBase+1]},
            {half+scalar(r+0x6c,a),top+scalar(r+0x70,a),scalar(r+0x74,a),uv[2*uvBase+2],uv[2*uvBase+3]},
            {half,zero,zero,uv[2*uvBase+4],uv[2*uvBase+5]},
            {negative,zero,zero,uv[2*uvBase+6],uv[2*uvBase+7]}};
        if(!loaded || !s.quad)return false;quad.texture=*loaded;
        if(!s.quad(s.user,quad))return false;
    }
    return true;
}
}
