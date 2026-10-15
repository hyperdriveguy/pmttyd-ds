#include "native_shadow_program.hpp"
namespace ttyd {
// Return a validated candidate; caller publishes only after backend admission.
// GX numeric arguments remain source identities, not PICA enum values. Slot
// mapping9/11/13 is position/color 0/texcoord0 throughout the declarations.
std::optional<NativeShadowProgram> NativeShadowProgram::request(const ShadowGXRequest& r) const{
    if(!seeded)return {};
    auto next=*this;
    auto args=[&](std::initializer_list<int32_t> values){std::array<int32_t,7> expected{};unsigned i=0;for(auto value:values)expected[i++]=value;return r.values==expected;};
    auto slot=[](int32_t attribute){return attribute==9?0:attribute==11?1:attribute==13?2:-1;};
    // The admitted second GX stage multiplies PREV by retained K0. Each
    // setter retains its own completed state; publication requires all fields.
    // Eight setters contribute bits 1..128: order, color op, alpha op, color in,
    // alpha in, swap, K-color select, K-alpha select. Mask 255 requires all;
    // register selectors12/28 pick K0 color/alpha for the second GX stage.
    if(r.values[0]==1){
        uint32_t bit=0;
        switch(r.kind){
        case ShadowGX::TevOrder:if(!args({1,255,255,255}))return {};bit=1;break;
        case ShadowGX::TevColorOp:if(!args({1,0,0,0,1,0}))return {};bit=2;break;
        case ShadowGX::TevAlphaOp:if(!args({1,0,0,0,1,0}))return {};bit=4;break;
        case ShadowGX::TevColorIn:if(!args({1,15,0,14,15}))return {};bit=8;break;
        case ShadowGX::TevAlphaIn:if(!args({1,7,0,6,7}))return {};bit=16;break;
        case ShadowGX::TevSwapMode:if(!args({1,0,0}))return {};bit=32;break;
        case ShadowGX::TevKColorSel:if(!args({1,12}))return {};bit=64;break;
        case ShadowGX::TevKAlphaSel:if(!args({1,28}))return {};bit=128;break;
        default:break;
        }
        if(bit){next.constantStageMask|=bit;return next;}
    }
    switch(r.kind){
    case ShadowGX::NumChans:if(!args({1}))return {};next.chans=1;break;
    case ShadowGX::ChanAmbient:if(!args({4}))return {};next.ambient=r.color;break;
    // Channel 0 color, 2 alpha, 4 both. Argument3 selects material source
    // (0 register, 1 vertex); argument1 retains source lighting-enable.
    // Color/alpha material ownership is independent.
    case ShadowGX::ChanCtrl:{
        const auto channel=r.values[0];if(channel!=0 && channel!=2 && channel!=4)return {};
        auto expected=r.values;expected[0]=4;
        if(expected!=std::array<int32_t,7>{4,0,0,0,0,2,2} && expected!=std::array<int32_t,7>{4,0,0,1,0,2,2} &&
           expected!=std::array<int32_t,7>{4,0,1,0,0,2,2} && expected!=std::array<int32_t,7>{4,0,1,1,0,2,2} && expected!=std::array<int32_t,7>{4,0,0,0,0,0,2})return {};
        if(channel==0 || channel==4){next.materialVertex=r.values[3]==1;next.channelControl[0]=r.values;}
        if(channel==2 || channel==4){next.alphaMaterialVertex=r.values[3]==1;next.channelControl[1]=r.values;}
        break;}

    case ShadowGX::TevKColor:if(!args({0}))return {};next.tevKColor0=r.color;break;
    case ShadowGX::TevColor:if(!args({1}))return {};next.registerAlpha=uint8_t(r.color);break;
    case ShadowGX::ChanMaterial:if(!args({4}))return {};next.material=r.color;break;
    case ShadowGX::NumTexGens:if(!args({0}) && !args({1}))return {};next.texGens=r.values[0];break;
    case ShadowGX::NumTevStages:
        if(args({2})){if(next.constantStageMask!=255 || !next.tevKColor0)return {};next.tevStages=2;}
        else if(args({1}))next.tevStages=1;else return {};
        break;
    case ShadowGX::NumIndStages:if(!args({0}))return {};next.indirectStages=0;break;
    case ShadowGX::TevOp:
        if(args({0,4}) && (next.material || next.materialVertex)){
            next.audienceColor=false;next.textureRGB=false;next.rasterRGB=true;next.alpha=Raster;
        }else if(args({0,0}) && next.texture){
            next.audienceColor=false;next.textureRGB=true;next.rasterRGB=true;next.alpha=TextureRaster;
        }else return {};break;
    case ShadowGX::TevSwapMode:if(!args({0,0,0}))return {};break;
    case ShadowGX::TevOrder:if(!args({0,255,255,4}) && !args({0,0,0,4}))return {};break;
    case ShadowGX::TevColorOp:case ShadowGX::TevAlphaOp:if(!args({0,0,0,0,1,0}))return {};break;
    // Original color input IDs: 15 ZERO, 14 KONST, 12 ONE, 10 RASC, 8 TEXC,
    // 0 PREV. Alpha IDs below: 7 ZERO, 6 KONST, 5 RASA, 4 TEXA, 1 A0.
    // Retain register/raster/texture alpha sources separately from RGB.
    case ShadowGX::TevColorIn:
        if(args({0,15,12,8,10})){next.audienceColor=true;next.textureRGB=false;next.rasterRGB=true;}
        else if(args({0,15,10,8,15})){next.textureRGB=true;next.audienceColor=false;next.rasterRGB=true;}
        else if(args({0,15,15,15,10}) && (next.material || next.materialVertex)){next.audienceColor=false;next.textureRGB=false;next.rasterRGB=true;}
        else return {};
        break;
    case ShadowGX::TevAlphaIn:
        if(args({0,7,7,7,5}))next.alpha=Raster;
        else if(args({0,7,7,7,7}))next.alpha=Zero;
        else if(args({0,7,1,5,7}))next.alpha=RegisterRaster;
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
    // Position arrays 80304b00/80304ce0 and palettes80415140/80415160
    // are guest resource identities. Strides12=XYZ float3, 4=RGBA bytes.
    // Descriptor 1 DIRECT/2 INDEX8; format 3 S16, 4 F32, 5 RGBA8 below.
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
        int index=slot(r.values[1]);if(index<0)return {};
        const int32_t format=index==0 && r.values[3]==3?3:index==1?5:4;
        if(!args({0,r.values[1],1,format,0}))return {};
        next.formats[index]=format;break;
    }
    default:return {};
    }
    return next;
}
}
