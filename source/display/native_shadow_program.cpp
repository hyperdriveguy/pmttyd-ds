#include "native_shadow_program.hpp"
namespace ttyd {
std::optional<NativeShadowProgram> NativeShadowProgram::request(const ShadowGXRequest& r) const{
    if(!seeded)return {};
    auto next=*this;
    auto args=[&](std::initializer_list<int32_t> values){std::array<int32_t,7> expected{};unsigned i=0;for(auto value:values)expected[i++]=value;return r.values==expected;};
    auto slot=[](int32_t attribute){return attribute==9?0:attribute==11?1:attribute==13?2:-1;};
    switch(r.kind){
    case ShadowGX::NumChans:if(!args({1}))return {};next.chans=1;break;
    case ShadowGX::ChanCtrl:
        if(!args({4,0,0,0,0,2,2}) && !args({4,0,0,1,0,2,2}) && !args({4,0,1,0,0,2,2}) && !args({4,0,1,1,0,2,2}))return {};
        next.materialVertex=r.values[3]==1;break;
    case ShadowGX::TevColor:if(!args({1}))return {};next.registerAlpha=uint8_t(r.color);break;
    case ShadowGX::ChanMaterial:if(!args({4}))return {};next.material=r.color;break;
    case ShadowGX::NumTexGens:if(!args({0}) && !args({1}))return {};next.texGens=r.values[0];break;
    case ShadowGX::NumTevStages:if(!args({1}))return {};break;
    case ShadowGX::TevOrder:if(!args({0,255,255,4}) && !args({0,0,0,4}))return {};break;
    case ShadowGX::TevColorOp:case ShadowGX::TevAlphaOp:if(!args({0,0,0,0,1,0}))return {};break;
    case ShadowGX::TevColorIn:
        if(args({0,15,12,8,10})){next.audienceColor=true;next.textureRGB=false;next.rasterRGB=true;}
        else if(args({0,15,10,8,15})){next.textureRGB=true;next.audienceColor=false;next.rasterRGB=true;}
        else if(args({0,15,15,15,10}) && next.material){next.audienceColor=false;next.textureRGB=false;next.rasterRGB=true;}
        else return {};
        break;
    case ShadowGX::TevAlphaIn:
        if(args({0,7,1,5,7}))next.alpha=RegisterRaster;
        else if(args({0,7,4,5,7}) && next.texture)next.alpha=TextureRaster;
        else if(args({0,7,5,4,7}))next.alpha=TextureRaster;
        else return {};
        break;
    case ShadowGX::TextureBind:if(!args({0}))return {};next.texture=r.texture;break;
    case ShadowGX::TexCoordGen:if(!args({0,1,4,60,0,125}))return {};break;
    case ShadowGX::Fog:
        if(!args({0}) || r.floats!=std::array<float,4>{})return {};
        next.fogOff=true;next.fogColor=r.color;break;
    case ShadowGX::CurrentMatrix:if(!args({0}))return {};break; // Slot0; actual matrix upload belongs to geometry.
    case ShadowGX::Cull:if(!args({0}) && !args({1}) && !args({2}))return {};next.cull=r.values[0];break;
    case ShadowGX::Array:{
        int index=slot(r.values[0]);if(index<0 || r.values[2]!=(index==1?4:12))return {};
        uint32_t address=uint32_t(r.values[1]);
        if(index==0 && address!=0x80304b00 && address!=0x80304ce0)return {};
        if(index==1 && address!=0x80415140 && address!=0x80415160)return {};
        if(!args({r.values[0],r.values[1],r.values[2]}))return {};
        if(index==2)return {};
        next.arrays[index]=address;next.strides[index]=r.values[2];break;
    }
    case ShadowGX::ClearDesc:if(!args({}))return {};next.descriptors={};break;
    case ShadowGX::Desc:{
        int index=slot(r.values[0]);if(index<0 || (r.values[1]!=1 && r.values[1]!=2))return {};
        if(!args({r.values[0],r.values[1]}))return {};
        next.descriptors[index]=r.values[1];break;
    }
    case ShadowGX::Format:{
        int index=slot(r.values[1]);if(index<0 || !args({0,r.values[1],1,index==1?5:4,0}))return {};
        next.formats[index]=index==1?5:4;break;
    }
    default:return {};
    }
    return next;
}
}
