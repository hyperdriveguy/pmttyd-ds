#include "platform.hpp"
namespace ttyd {
bool Renderer::itemGetWindows(ItemGetWindows& owner,uint32_t counter) {
 bool success=true;
 for(unsigned slot=0;slot<2;++slot)if(owner.displaySlots()&(1u<<slot)) {
  // Callback mutates the retained rectangle; frame uses the prior snapshot.
  success=drawGetWindow(owner.display(slot),owner.palette0,counter) && success;
 }
 selectorClip(nullptr);return success;
}
bool Renderer::coinGetWindow(CoinGetWindow& owner,uint32_t counter) {
 const bool success=!owner.displayed() || drawGetWindow(owner.display(),owner.palette0,counter);
 selectorClip(nullptr);return success;
}
bool Renderer::drawGetWindow(const ItemGetWindowDisplay& display,uint32_t palette0,uint32_t counter) {
 bool success=true;const ItemSelectionClip full{0,0,608,480},helpClip{0,384,608,56};
 const auto& w=display.frame;const auto& r=w.rectangle;const auto& seq=w.sequence;
  const std::array<uint8_t,3> color=display.help?std::array<uint8_t,3>{240,240,240}:std::array<uint8_t,3>{225,215,155};
  const auto frames=itemSelectionFramePlan(r.x,r.y,r.width,r.height,w.decoration,seq.rotation,seq.scale,color,seq.alpha);
  selectorClip(nullptr);
  for(unsigned f=0;f<frames.count;++f)success=sourceWindowFrame(frames.frames[f]) && success;
  if(!display.content)return success;
  success=sourceWindowFont(display.text,WindowFontConsumer::MessageDirect,display.draw,
      {display.draw.color,palette0,2,255,false},display.help?&helpClip:&full,display.compiled) && success;
  if(display.help) {
   const auto icons=itemSelectionHelpIconDraws(w,counter);
   for(unsigned n=0;n<icons.count;++n){const auto& i=icons.icons[n];success=selectorIcon(i.icon,counter,i.x,i.y,i.scale,i.flags) && success;}
  }
 selectorClip(nullptr);return success;
}
}
