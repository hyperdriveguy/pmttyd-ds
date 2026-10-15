#include "animation_texture.hpp"
#include <cstring>
#include <cmath>
namespace ttyd {
std::shared_ptr<const AnimationTextureResource> AnimationTextureResource::load(const char* path){
    FILE* f=path?std::fopen(path,"rb"):nullptr;if(!f)return {};auto r=std::make_shared<AnimationTextureResource>();const bool ok=r->read(f);std::fclose(f);return ok?r:nullptr;
}
// TPL is BE: magic0020af30(hex), count/table at 4/8; table rows 8 bytes,
// image headers36, palette headers12. Native cap 16MiB/4096 images. read clears
// old images and may leave partial results on false; load returns no owner then.
bool AnimationTextureResource::read(FILE* file){
    images_.clear();if(!file || std::fseek(file,0,SEEK_END))return false;const auto length=std::ftell(file);
    if(length<12 || length>16*1024*1024 || std::fseek(file,0,SEEK_SET))return false;
    std::vector<uint8_t> b(size_t(length),0);if(std::fread(b.data(),b.size(),1,file)!=1)return false;
    auto span=[&](uint32_t o,uint64_t n){return o<=b.size() && n<=b.size()-o;};
    auto half=[&](uint32_t o){return uint16_t(uint16_t(b[o])<<8|b[o+1]);};
    auto word=[&](uint32_t o){uint32_t v;std::memcpy(&v,b.data()+o,4);
#if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
        return __builtin_bswap32(v);
#else
        return v;
#endif
    };
    if(word(0)!=0x0020af30 || word(4)>4096 || !span(word(8),8ull*word(4)))return false;
    for(uint32_t n=0;n<word(4);++n){const auto table=word(8)+8*n,h=word(table),p=word(table+4);if(!span(h,36) || (p && !span(p,12)))return false;
        AnimationTPLImage i;i.index=n;i.imageHeader=h;i.paletteHeader=p;i.height=half(h);i.width=half(h+2);i.format=word(h+4);i.data=word(h+8);i.wrapS=word(h+12);i.wrapT=word(h+16);i.minFilter=word(h+20);i.magFilter=word(h+24);
        auto bits=word(h+28);std::memcpy(&i.bias,&bits,4);i.edge=b[h+32];i.minLOD=b[h+33];i.maxLOD=b[h+34];
        if(!i.width || !i.height || !std::isfinite(i.bias) || i.wrapS>2 || i.wrapT>2 || i.minLOD>i.maxLOD)return false;
        // GX tiles: formats I4/CI4/CMPR(0/8/14)8x8x32 bytes;
        // I8/IA4/CI8(1/2/9)8x4x32; IA8/RGB565/RGB5A3/CI14(3/4/5/10)
        // 4x4x32; RGBA8(6)4x4x64. Bounds here cover base image bytes.
        unsigned tw=4,th=4,bytes=32;
        switch(i.format){case 0:case 8:case 14:tw=th=8;break;case 1:case 2:case 9:tw=8;break;case 3:case 4:case 5:case 10:break;case 6:bytes=64;break;default:return false;}
        if(!span(i.data,uint64_t((i.width+tw-1)/tw)*((i.height+th-1)/th)*bytes))return false;
        if(p){i.paletteCount=half(p);i.paletteFormat=word(p+4);i.paletteData=word(p+8);if(!span(i.paletteData,2ull*i.paletteCount))return false;}
        images_.push_back(i);
    }
    return true;
}
bool animationAtlasDimensions(uint32_t width,uint32_t height,float uvScale){
    return (width==512 || width==1024) && (uvScale==1 || uvScale==2 || uvScale==4 || uvScale==8) && height==width/uvScale;
}
// Selected pose descriptor+slot+state+original TPL image must agree before
// using a converted cell. Equal texture names across packages are insufficient.
std::optional<uint16_t> animationTextureCell(const AnimationTextureRequest& r){
    if(!r.pose || !r.pose->resource || !r.pose->resource->draw || !r.pose->resource->bank)return {};
    const auto& resource=*r.pose->resource;const auto selected=r.pose->word(0x14);if(selected>=resource.names.size())return {};
    const auto* p=resource.bank->pose(resource.names[selected].c_str());if(!p || r.slot>=p->textures.size() || r.state>255)return {};
    const auto& d=*resource.draw;
    if(r.slot>=d.word(0x130) || d.word(d.word(0x194)+8*r.slot)+r.state!=r.descriptor || r.descriptor>=d.word(0x134) || (d.word(d.word(0x198)+64*r.descriptor+8)!=0 || d.word(d.word(0x198)+64*r.descriptor+4)!=r.image.index))return {};
    const auto cell=p->textures[r.slot].cells[r.state];return cell<64?std::optional<uint16_t>(cell):std::nullopt;
}
// Two ordered walks: first load original textures, then coordinate matrices.
// Slots traverse reverse draw order. Keep two original TPL image queries; the
// palette decision is captured before the second getter and must not be merged.
bool resumeAnimationTexture(const BattlePose& pose,uint32_t draw,const AnimationTextureServices& s,AnimationTextureCursor& c){
    if(!c.active){if(!pose.resource || !pose.resource->draw)return false;c={};c.active=true;c.pose=&pose;c.resource=pose.resource;c.draw=draw;const auto& d=*c.resource->draw;
        const auto first=d.word(0x19c);if(draw<first || (draw-first)%108 || (draw-first)/108>=d.word(0x138)){c.pending=0x8003dcfc;return false;}
        c.slots=d.word(draw);if(c.slots>8)return false;}
    if(c.pose!=&pose || c.resource!=pose.resource){c.pending=0x8003dcfc;return false;}
    const auto& d=*c.resource->draw;
    auto next=[&](uint32_t p){c.queued=false;c.phase=p;};
    auto send=[&](uint32_t fn){if(!c.queued){c.request={};auto& r=c.request;r.function=fn;r.coordinate=c.j;r.slot=c.slot;r.descriptor=c.index;r.state=c.state;r.image=c.image;r.pose=&pose;
            const bool original=int32_t(c.overrideWrap)<0;r.wrapS=original?c.image.wrapS:(c.overrideWrap&4)?2:(c.overrideWrap&1);r.wrapT=original?c.image.wrapT:(c.overrideWrap&8)?2:((c.overrideWrap>>1)&1);
            if(fn==0x802b4e04)r.arguments={int32_t(c.j),1,int32_t(c.j+4),int32_t(c.mask?30+3*c.j:60),0,125,0};
            if(fn==0x802ba3d0){r.arguments={int32_t(30+3*c.j),1};r.matrix=c.matrices[c.result];}
            c.queued=true;
        }c.pending=fn;return s.gx && s.gx(s.user,c.request);};
    for(;;)switch(c.phase){
    case 0:
        if(c.j>=c.slots){c.j=0;next(12);break;}
        c.slot=d.word(c.draw+0x10+4*(c.slots-c.j-1));c.pending=0x8003dcfc;
        if(c.slot>=pose.shape.textures.size() || c.slot>=d.word(0x130))return false;
        c.state=pose.shape.textures[c.slot];c.index=d.word(d.word(0x194)+8*c.slot)+c.state;c.overrideWrap=d.word(d.word(0x194)+8*c.slot+4);
        if(int32_t(c.index)<0 || c.index>=d.word(0x134)){c.pending=0x8026a2a8;return false;} // Diagnostic/fallback owner remains explicit.
        c.phase=d.word(d.word(0x198)+64*c.index+8)==0?1:10;break;
    case 1:c.pending=0x80037b5c;if(!s.get || !s.get(s.user,pose,d.word(d.word(0x198)+64*c.index+4),c.image))return false;c.ci=c.image.paletteHeader!=0;next(2);break;
    case 2:c.pending=0x80037b5c;if(!s.get || !s.get(s.user,pose,d.word(d.word(0x198)+64*c.index+4),c.image))return false;next(c.ci?3:5);break;
    case 3:if(!send(0x802b78d4))return false;next(4);break;
    case 4:if(!send(0x802b74fc))return false;next(6);break;
    case 5:if(!send(0x802b72b0))return false;next(6);break;
    case 6:if(!send(0x802b7544))return false;next(c.ci?7:8);break;
    case 7:if(!send(0x802b790c))return false;next(8);break;
    case 8:if(!send(0x802b7880))return false;next(10);break;
    case 10:++c.j;next(0);break;
        // Surface transform is U/V translation, U/V scale, rotation. Rotation
        // pivots about(.5,.5); V translation retains (1-v)-scaleV even when
        // it looks reducible. Mask1/2/4 selects rotation/translation/scale.
    case 12:{
        if(c.j>=c.slots){c.active=false;c.pending=0;return true;}
        c.slot=d.word(c.draw+0x10+4*(c.slots-c.j-1));c.pending=0x8003dcfc;if(c.slot>=d.word(0x12c))return false;
        std::array<float,5> t;
        if(!pose.shape.surfaces.empty()){if(c.slot>=pose.shape.surfaces.size())return false;t=pose.shape.surfaces[c.slot].transform;}
        else for(unsigned i=0;i<5;++i)t[i]=d.real(d.word(0x190)+24*c.slot+4+4*i);
        c.step=c.count=c.mask=0;c.matrixQueued=false;
        auto op=[&](uint32_t fn,unsigned target,unsigned left=0,unsigned right=0){auto& r=c.operations[c.count++];r={};r.function=fn;r.target=target;r.left=left;r.right=right;};
        auto trans=[&](unsigned target,std::array<float,3> xyz){op(0x8029e230,target);c.operations[c.count-1].xyz=xyz;};
        if(t[4]!=0){trans(3,{.5f,.5f,0});op(0x8029dff0,4);c.operations[c.count-1].radians=(-t[4])*.01745329238474369f;op(0x8029dd64,5,3,4);trans(3,{-.5f,-.5f,0});op(0x8029dd64,0,5,3);c.mask|=1;}
        if(t[0]!=0 || t[1]!=0 || t[3]!=0){trans(1,{t[0],(1-t[1])-t[3],0});c.mask|=2;}
        if(t[2]!=1 || t[3]!=1){op(0x8029e2b0,2);c.operations[c.count-1].xyz={t[2],t[3],1};c.mask|=4;}
        bool first=true;for(unsigned i=0;i<3;++i)if(c.mask&(1u<<i)){if(first){c.result=i;first=false;}else{op(0x8029dd64,6,c.result,i);c.result=6;}}
        next(13);break;}
    case 13:
        while(c.step<c.count){const auto& op=c.operations[c.step];if(!c.matrixQueued){c.math={};c.math.function=op.function;c.math.xyz=op.xyz;c.math.axis=122;c.math.radians=op.radians;c.math.left=c.matrices[op.left];c.math.right=c.matrices[op.right];c.matrixQueued=true;}
            c.pending=c.math.function;if(!s.matrix || !s.matrix(s.user,c.math))return false;c.matrices[op.target]=c.math.output;c.matrixQueued=false;++c.step;}
        next(14);break;
    // Generator setup precedes matrix upload. Matrix30+3*j, identity60,
    // postmatrix125 retain GX coordinate identity; axis122 is ASCII z.
    case 14:if(!send(0x802b4e04))return false;next(c.mask?15:16);break;
    case 15:if(!send(0x802ba3d0))return false;next(16);break;
    case 16:++c.j;next(12);break;
    default:c.pending=0x8003dcfc;return false;
    }
}
}
