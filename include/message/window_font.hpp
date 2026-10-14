#pragma once
#include "message_playback.hpp"
namespace ttyd {
struct WindowFontMeasurement {uint16_t width=0,lineFeeds=0;size_t glyphs=0;};
// Valid original US single-byte plain text. Tagged/multibyte streams need their
// original consumers; rejection never strips/replaces their control tokens.
bool measurePlainWindowFont(const MessageFont&,const char*,uint32_t spacing,WindowFontMeasurement&);
struct WindowFontQuad {
    int16_t left=0,top=0,right=0,bottom=0;
    uint16_t u0=0,v0=0,u1=0,v1=0;
    uint32_t rgba=0;
    int32_t originX=0,originY=0; // Matrix-message per-glyph translation before wrapper T*S.
    float pitchX=0,pitchY=0; // Proportional scalar-string physical pen, outside glyph scale.
};
struct WindowFontTransform {float x=0,y=0,scaleX=1,scaleY=1;};
struct WindowFontClip {float left=0,top=0,right=0,bottom=0;}; // Source Y up.
// Native visibility optimization only; source quad/RNG requests still execute.
bool windowFontQuadIntersects(const WindowFontQuad&,WindowFontTransform,WindowFontClip);
struct WindowFontDrawServices {
    void* user=nullptr;
    void (*emitQuad)(void*,const WindowFontQuad&)=nullptr;
    uint32_t (*random)(void*,uint32_t bound)=nullptr;
};
enum class WindowFontConsumer {StringMatrix,MessageMatrix,MessageDirect,StringPitch};
struct WindowFontStyle {
    uint32_t color=0x1e1414ff,palette0=0x1e1414ff,spacing=2;
    uint8_t alpha=255;
    bool edge=false;
};
// Finite original nonoutlined US plain proportional scalar-string consumer.
bool drawPlainWindowFontPitch(const MessageFont&,const char*,const WindowFontStyle&,
    const WindowFontDrawServices&,float x,float y,float scaleX,float scaleY);
// Local glyph requests: matrix consumers start at0; direct starts supplied XY.
// Native caller owns camera/wrapper matrix, scissor, texture and GPU state.
bool drawPlainWindowFont(const MessageFont&,const char*,WindowFontConsumer,
    const WindowFontStyle&,const WindowFontDrawServices&,int32_t x=0,int32_t y=0);
}
