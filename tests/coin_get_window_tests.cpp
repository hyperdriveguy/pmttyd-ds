#include "item_get_windows.hpp"
#include "field_camera_events.hpp"
#include "field_item_get_effect.hpp"
#include <cassert>
#include <cmath>
using namespace ttyd;
int main() {
 MessageFont font;assert(font.load("romfs/font.pmfont") && loadItemGetWindowAssets("romfs"));
 CoinGetWindow window;assert(window.prepare(font));
 assert(window.title()=="You got <col c00000ff>20</col> coins!");
 int32_t parameter[]={20,0};assert(window.enter(0x80313dc8,parameter)==-1);
 assert(window.enter(0x80415e84,nullptr)==-1 && !window.retained());
 assert(window.enter(0x80415e84,parameter)==0 && window.retained() && !window.prepare(font));
 assert(window.enter(0x80415e84,parameter)==-1);
 int32_t rate=60;ItemSelectionSequenceServices sequence{&rate,[](void* p){return *static_cast<int32_t*>(p);},
  [](void*,int32_t mode,int32_t step,int32_t count,float a,float b){return cameraInterpolation(mode,step,count,a,b).value_or(a);}};
 window.managerTick(sequence);assert(window.displayed() && window.action(0)==1);
 const auto hidden=window.display();assert(!hidden.content && hidden.frame.rectangle.width==350 && hidden.frame.rectangle.height==42);
 parameter[1]=1;const auto opening=window.display();
 assert(opening.content && window.action(0)==1 && opening.draw.y==112 && opening.draw.color==0x000000ff);
 assert(opening.text==window.title().c_str() && opening.compiled && opening.compiled->plain=="You got 20 coins!");
 struct Draw {std::vector<WindowFontQuad> quads;unsigned rng=0;} draw;
 WindowFontDrawServices s{&draw,[](void* p,const WindowFontQuad& q){static_cast<Draw*>(p)->quads.push_back(q);},
  [](void* p,uint32_t bound){assert(bound==64);++static_cast<Draw*>(p)->rng;return 0u;}};
 assert(drawColorWindowFont(font,*opening.compiled,{0x000000ff,window.palette0,2,255,false},s,0,112));
 assert(draw.quads.size()==17 && draw.rng==34);
 assert(draw.quads[8].rgba==0xc00000ff && draw.quads[9].rgba==0xc00000ff && draw.quads[10].rgba==window.palette0);
 const auto* compiled=opening.compiled;
 for(unsigned i=0;i<20;++i){window.managerTick(sequence);assert(window.display().compiled==compiled);}
 parameter[0]=1;assert(window.display().compiled->plain=="You got 1 coin!");
 parameter[1]=0;assert(!window.display().content);
 window.closeAutoDelete(0);assert(window.window().sequence.flags&8);
 for(unsigned i=0;i<9;++i)window.managerTick(sequence);
 assert(!window.retained() && !window.displayed() && window.action(0)==1); // Source deleted slot retains4; native retirement must be separate.
 const auto retired=window.window().sequence;
 assert(window.enter(0x80415e84,parameter)==0);
 assert(window.window().sequence.alpha==retired.alpha && window.window().sequence.scale==retired.scale);
 // Geometry changes only when source parameter flag enables display, and the
 // visible frame snapshots its rectangle before that callback's writes.
 MessageFont wide=font;for(auto& glyph:wide.glyphs)glyph.width=40;
 CoinGetWindow large;assert(large.prepare(wide) && large.enter(0x80415e84,parameter)==0);
 large.managerTick(sequence);assert(!large.display().content && large.window().rectangle.width==350);
 parameter[1]=1;const auto old=large.display();assert(old.content && old.frame.rectangle.width==350 && large.window().rectangle.width>350);
 const auto resized=large.display();assert(resized.frame.rectangle.width==large.window().rectangle.width);
 assert(large.window().rectangle.x==int32_t(-float(large.window().rectangle.width)/2));
 // Kind7 shares original bank/update/quad shader, retaining its distinct scale
 // and pink color rather than relabeling a kind6 orange effect.
 FieldItemGetEffect6 effect;effect.beginCoin({0,62,0});assert(effect.kind()==7);
 for(unsigned i=0;i<11;++i)effect.tick();auto coin=effect.draw(0,0);
 assert(coin.colors[1]==0xe85bd0ff && coin.textures[0]==44 && coin.textures[1]==42);
 const auto origin=coin.passes[1].model.point({0,0,0}),edge=coin.passes[1].model.point({1,0,0});
 assert(std::fabs(edge[0]-origin[0]-.75f)<.00001f);
 effect.begin({0,62,0});assert(effect.kind()==6);for(unsigned i=0;i<11;++i)effect.tick();
 assert(effect.draw(0,0).colors[1]==0xeb7d06ff);
}
