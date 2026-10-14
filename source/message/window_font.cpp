#include "window_font.hpp"
#include <algorithm>
#include <cassert>
#include <cmath>
namespace ttyd {
namespace {
int16_t signedHalf(uint32_t value) {
    const uint32_t bits=value&0xffff;return bits<0x8000?int16_t(bits):int16_t(int32_t(bits)-65536);
}
int32_t signedWord(uint32_t bits) {return bits<=0x7fffffff?int32_t(bits):int32_t(int64_t(bits)-0x100000000LL);}
uint32_t effective(uint32_t rgba,uint8_t alpha) {return (rgba&0xffffff00u)|((rgba&255)*alpha/255);}
}
bool measurePlainWindowFont(const MessageFont& font,const char* text,uint32_t spacing,WindowFontMeasurement& out) {
    if(!text)return false;
    WindowFontMeasurement next;uint16_t current=0,maximum=0;
    for(const auto* p=reinterpret_cast<const uint8_t*>(text);*p;++p) {
        if(*p=='<')return false;
        if(*p=='\n'){maximum=std::max(maximum,current);current=0;++next.lineFeeds;continue;}
        const uint16_t code=font.mapping[*p];if(code==0xffff)continue;
        if(code>=font.glyphs.size())return false;
        current=uint16_t(uint32_t(current)+font.glyphs[code].width+spacing);++next.glyphs;
    }
    maximum=std::max(maximum,current);next.width=maximum?uint16_t(uint32_t(maximum)-spacing):0;
    out=next;return true;
}
bool windowFontQuadIntersects(const WindowFontQuad& q,WindowFontTransform t,WindowFontClip clip) {
    const float left=t.x+q.pitchX+(float(q.originX)+q.left)*t.scaleX,right=t.x+q.pitchX+(float(q.originX)+q.right)*t.scaleX;
    const float top=t.y+q.pitchY+(float(q.originY)+q.top)*t.scaleY,bottom=t.y+q.pitchY+(float(q.originY)+q.bottom)*t.scaleY;
    return std::max(left,right)>clip.left && std::min(left,right)<clip.right &&
        std::max(top,bottom)>clip.bottom && std::min(top,bottom)<clip.top;
}
bool drawPlainWindowFont(const MessageFont& font,const char* text,WindowFontConsumer consumer,
    const WindowFontStyle& style,const WindowFontDrawServices& services,int32_t x,int32_t y) {
    if(consumer==WindowFontConsumer::StringPitch)return false;
    WindowFontMeasurement checked;if(!measurePlainWindowFont(font,text,style.spacing,checked))return false;
    assert(services.emitQuad && services.random);
    int32_t penX=consumer==WindowFontConsumer::MessageDirect?x:0;
    int32_t penY=consumer==WindowFontConsumer::MessageDirect?y:0;
    const int32_t startX=penX;float floatX=0,floatY=0;
    for(const auto* p=reinterpret_cast<const uint8_t*>(text);*p;++p) {
        if(*p=='\n'){
            if(consumer==WindowFontConsumer::StringMatrix){floatX=0;floatY-=28.f;}
            else {penX=startX;penY=signedWord(uint32_t(penY)-28);}
            continue;
        }
        const uint16_t code=font.mapping[*p];if(code==0xffff)continue;
        const auto& glyph=font.glyphs[code];
        const int16_t localX=consumer==WindowFontConsumer::StringMatrix?signedHalf(uint32_t(int32_t(floatX))):
            consumer==WindowFontConsumer::MessageMatrix?0:signedHalf(uint32_t(penX));
        const int16_t localY=consumer==WindowFontConsumer::StringMatrix?signedHalf(uint32_t(int32_t(floatY))):
            consumer==WindowFontConsumer::MessageMatrix?0:signedHalf(uint32_t(penY));
        const auto emit=[&](int32_t dx,int32_t dy,uint32_t color) {
            const int16_t left=signedHalf(uint32_t(int32_t(localX))+uint32_t(dx));
            const int16_t requestY=signedHalf(uint32_t(int32_t(localY))+uint32_t(dy));
            const int16_t top=signedHalf(uint32_t(int32_t(requestY))-2);
            WindowFontQuad q{left,top,signedHalf(uint32_t(int32_t(float(left)+float(glyph.width)))),
                signedHalf(uint32_t(int32_t(float(top)-float(glyph.height)))),glyph.x,glyph.y,
                uint16_t(glyph.x+glyph.width),uint16_t(glyph.y+glyph.height),effective(color,style.alpha)};
            if(consumer==WindowFontConsumer::MessageMatrix){q.originX=penX;q.originY=penY;}
            // Original inner quad consumes these even when noise UVs are unused.
            services.random(services.user,64);services.random(services.user,64);
            services.emitQuad(services.user,q);
        };
        if(style.edge){emit(-2,0,style.palette0);emit(2,0,style.palette0);emit(0,-2,style.palette0);emit(0,2,style.palette0);}
        emit(0,0,style.color);
        if(consumer==WindowFontConsumer::StringMatrix)floatX+=float(signedWord(uint32_t(glyph.width)+style.spacing));
        else {penX=signedWord(uint32_t(penX)+glyph.width);penX=signedWord(uint32_t(penX)+style.spacing);}
    }
    return true;
}
bool drawPlainWindowFontPitch(const MessageFont& font,const char* text,const WindowFontStyle& style,
 const WindowFontDrawServices& services,float x,float y,float scaleX,float scaleY) {
 (void)scaleY; // Native per-glyph matrix applies captured Y scale.
 if(!text || style.edge || !services.emitQuad || !services.random)return false;
 for(const auto* p=reinterpret_cast<const uint8_t*>(text);*p;++p) {
  if(*p=='<' || *p=='\n')return false; // Original command/line consumers is a separate provider.
  const uint16_t code=font.mapping[*p];if(code!=0xffff && code>=font.glyphs.size())return false;
 }
 float pen=x;
 for(const auto* p=reinterpret_cast<const uint8_t*>(text);*p;++p) {
  const uint16_t code=font.mapping[*p];if(code==0xffff)continue;
  const auto& glyph=font.glyphs[code];
  WindowFontQuad q{0,-2,int16_t(glyph.width),int16_t(-2-int(glyph.height)),glyph.x,glyph.y,
   uint16_t(glyph.x+glyph.width),uint16_t(glyph.y+glyph.height),effective(style.color,style.alpha)};
  q.pitchX=pen;q.pitchY=y;
  services.random(services.user,64);services.random(services.user,64);services.emitQuad(services.user,q);
  pen=std::fma(scaleX,float(signedWord(uint32_t(glyph.width)+style.spacing)),pen);
 }
 return true;
}

}
