#include "platform.hpp"
#include "opening_voyage_callbacks.hpp"
namespace ttyd {
// Original US language0 texture 21 uses 440x238 source dimensions. Native
// presentation halves them to 220x119 around horizontal center200; 59.5
// is the resulting half-height, not an animation or collision constant.
bool Renderer::frontVoyageLogo(const OpeningVoyageLogo& logo){
    if(!logo.active || logo.complete)return true;
    // US language0 selects mariost.tpl21 in the original draw palette table.
    // frontImage already packages that unchanged original as title-logo.
    if(!frontImage("title-logo",0,0,0,0))return false;
    logo.admitResources();
    // Original source position(0,100+work.Y,0), unit scale and whiteRGBA.
    // Source440x238 image/608x480 viewport adapted to half-size native content.
    return frontImage("title-logo",90,120-(100+logo.yOffset)*.5f-59.5f,220,119,uint8_t(logo.alpha));
}
}
