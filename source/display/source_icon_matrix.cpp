#include "source_icon_matrix.hpp"
#include <cstring>
namespace ttyd {
namespace {
// Original global addresses hold directory/bin/TPL pointers and shared GP.
// The native resource provider maps them to synthetic immutable guest domains.
constexpr uint32_t Directory=0x8041e634,Bin=0x8041e638,Tpl=0x8041e63c,GP=0x80415fd0;
float real(uint32_t bits){float value;std::memcpy(&value,&bits,4);return value;}
}
// Only original iconDispGx2/Col identities are admitted. Capture request once;
// caller retains this request/continuation until completion. Missing word/half
// is a hold; an actual zero loaded-buffer pointer takes the original early return.
bool resumeSourceIconMatrix(const BattleAudienceIconRequest& request,const SourceIconMatrixServices& s,SourceIconMatrixCursor& c){
    if(request.function!=0x8001a6e0 && request.function!=0x8001a4bc)return false;
    if(!c.active){c={};c.active=true;c.request=request;}
    // The containing audience cursor retains this request until completion.
    if(c.request.function!=request.function)return false;
    auto word=[&](uint32_t address){return s.word?s.word(s.user,address):std::optional<uint32_t>{};};
    auto half=[&](uint32_t address){return s.half?s.half(s.user,address):std::optional<uint16_t>{};};
    if(c.phase==0){
        c.pending=0x8000f2e4;if(!s.currentCamera)return false;
        const auto camera=s.currentCamera(s.user);if(!camera)return false;
        c.camera=*camera;c.phase=1;
    }
    if(c.phase==1){
        c.pending=c.request.function;
        const auto bin=word(Bin);if(!bin)return false;
        if(!*bin){c.active=false;c.pending=0;return true;}
        const auto tpl=word(Tpl);if(!tpl)return false;
        if(!*tpl){c.active=false;c.pending=0;return true;}
        c.icon.flags=uint16_t(c.request.flags);c.icon.id=uint16_t(c.request.icon);
        // Col copies supplied color; ordinary matrix icon copies original DATA.
        const auto color=c.request.function==0x8001a4bc?std::optional<uint32_t>(c.request.rgba):word(0x8041f864);
        if(!color)return false;
        const auto metadata=word(Directory);if(!metadata)return false;
        c.icon.rgba=*color;c.metadata=*metadata;c.phase=2;
    }
    if(c.phase==2){
        uint32_t selected=c.icon.id;
        // Local name story below actually reads GP+16c LANGUAGE selection,
        // not plot progress. Icons 73/103 use separate localized U16 tables.
        if(selected==73 || selected==103){
            const auto gp=word(GP);if(!gp)return false;
            const auto story=word(*gp+0x16c);if(!story)return false;
            const auto id=half((selected==73?0x80415040u:0x8041504cu)+(*story<<1));if(!id)return false;
            selected=*id;
        }
        // Source rlwinm retains the narrowed ID after the doubleword shift.
        const auto offset=half(c.metadata+((selected<<1)&0x1fffeu)+2);if(!offset)return false;
        c.descriptor=c.metadata+*offset;c.phase=3;
    }
    if(c.phase==3){
        // Descriptor header:+2 width,+4 height,+6 frame count; frame rows
        // are U16 texture/+8 and delay/+10, 4-byte stride. Clock GP+1c is
        // signed for remainder: negative clocks retain original branch behavior.
        const auto width=half(c.descriptor+2),height=half(c.descriptor+4),texture=half(c.descriptor+8),delay=half(c.descriptor+10),count=half(c.descriptor+6);
        if(!width || !height || !texture || !delay || !count)return false;
        auto icon=c.icon;icon.width=*width;icon.height=*height;icon.texture=*texture;icon.delay=*delay;
        if(*count>1){
            uint32_t total=0;
            for(uint32_t i=0;i<*count;++i){const auto duration=half(c.descriptor+10+4*i);if(!duration)return false;total+=*duration;}
            // Authentic descriptors have positive totals. Gekko divw zero and
            // signed-overflow domains are deliberately not guessed here.
            if(!total || total>0x7fffffffu)return false;
            const auto gp=word(GP);if(!gp)return false;
            const auto clock=word(*gp+0x1c);if(!clock)return false;
            int32_t remaining=int32_t(*clock)%int32_t(total);
            for(uint32_t i=0;i<*count;++i){
                const auto duration=half(c.descriptor+10+4*i);if(!duration)return false;
                if(remaining<int32_t(*duration)){
                    const auto selected=half(c.descriptor+8+4*i);if(!selected)return false;
                    icon.texture=*selected;break;
                }
                remaining-=*duration;
            }
        }
        c.icon=icon;c.phase=4;
    }
    // Camera+11c is 3x4 view, 12 binary32 words. PSMTXConcat8029dd64
    // computes view*supplied model before iconGX800197f8, not after drawing.
    if(c.phase==4){
        c.matrix={};c.matrix.function=0x8029dd64;
        for(uint32_t i=0;i<12;++i){const auto bits=word(c.camera+0x11c+4*i);if(!bits)return false;c.matrix.left[i]=real(*bits);}
        c.matrix.right=c.request.matrix;c.phase=5;
    }
    if(c.phase==5){
        c.pending=0x8029dd64;if(!s.matrix || !s.matrix(s.user,c.matrix))return false;
        c.phase=6;
    }
    if(c.phase==6){
        c.pending=0x800197f8;
        if(s.draw){if(!s.draw(s.user,c.matrix.output,c.icon))return false;}
        else if(!s.gx || !resumeSourceIconGX(c.matrix.output,c.icon,*s.gx,c.gxCursor)){
            if(c.gxCursor.active)c.pending=c.gxCursor.pending;
            return false;
        }
        c.active=false;c.pending=0;return true;
    }
    return false;
}
}
