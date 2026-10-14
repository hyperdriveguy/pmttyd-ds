#include "item_get_windows.hpp"
#include "item_presentation.hpp"
#include "field_camera_events.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
int main() {
 MessageFont font;assert(font.load("romfs/font.pmfont"));assert(loadItemPresentations("romfs/item-presentations.bin"));
 assert(loadItemGetWindowAssets("romfs"));ItemGetWindows w;assert(w.prepare(font));
 assert(w.title()=="You got a <col c00000ff>Box</col>!");
 WindowFontMeasurement m;assert(measureColorWindowFont(font,w.title().c_str(),2,m));
 WindowFontMeasurement plain;assert(measurePlainWindowFont(font,"You got a Box!",2,plain));assert(m.width==plain.width && m.glyphs==plain.glyphs && m.lineFeeds==0);
 assert(measureColorWindowFont(font,"Box<col ff>\nBox",2,m));assert(m.lineFeeds==0);
 assert(measureColorWindowFont(font,"Box\nBox",2,m));assert(m.lineFeeds==1);
 struct Probe {std::vector<WindowFontQuad> q;unsigned rng=0;} probe;
 WindowFontDrawServices s{&probe,[](void* p,const WindowFontQuad& q){static_cast<Probe*>(p)->q.push_back(q);},[](void* p,uint32_t b){assert(b==64);++static_cast<Probe*>(p)->rng;return 0u;}};
 assert(drawColorWindowFont(font,w.title().c_str(),{0x000000ff,0x1e1414ff,2,255,false},s,-100,112));
 assert(probe.q.size()==14 && probe.rng==28);for(unsigned i=0;i<10;++i)assert(probe.q[i].rgba==0x000000ff);
 for(unsigned i=10;i<13;++i)assert(probe.q[i].rgba==0xc00000ff);assert(probe.q[13].rgba==0x1e1414ff);
 assert(probe.q[0].left==-100 && probe.q[0].top==110);
 probe={};assert(!drawColorWindowFont(font,"Box<col nope>",{0x000000ff,0x1e1414ff,2,255,false},s,0,0));assert(!probe.rng && probe.q.empty());
 assert(!measureColorWindowFont(font,"<icon 307>",2,m));
 assert(w.enter(0x80313e18,"Goomba3Box")==-1);assert(!w.retained()); // Unbound failure window.
 const int name=w.enter(0x80313dc8,"Goomba3Box"),help=w.enter(0x80313df0,"Goomba3Box");assert(name==0 && help==1);
 assert(w.action(name)==0 && w.window(name).sequence.action==1 && w.window(name).sequence.flags==1);
 assert(!w.prepare(font) && w.enter(0x80313dc8,"other")==-1);
 int32_t rate=60;ItemSelectionSequenceServices seq{&rate,[](void* p){return *static_cast<int32_t*>(p);},[](void*,int32_t kind,int32_t tick,int32_t count,float a,float b){return *cameraInterpolation(kind,tick,count,a,b);}};
 w.managerTick(0,seq);assert(w.displaySlots()==3 && w.action(name)==1);
 const auto first=w.display(name);assert(!first.content && first.frame.rectangle.height==42 && w.window(name).rectangle.height==42);
 assert(!w.display(help).content && w.window(help).helpText==nullptr);
 for(unsigned i=0;i<8;++i)w.managerTick(0,seq);
 assert(w.window(name).sequence.action==0 && w.action(name)==1); // Completion retains transition.
 w.managerTick(0,seq);assert(w.action(name)==0 && w.window(help).helpLines==0);
 const auto title=w.display(name);assert(title.content && !title.help && title.draw.color==0x000000ff && title.draw.y==112);
 const auto description=w.display(help);assert(description.content && description.help && description.draw.color==w.palette0);
 assert(w.window(help).helpText==w.description() && w.window(help).helpLines==0);
 w.managerTick(0x300,seq);assert(w.window(help).helpLines==2 && w.window(help).helpPage==0);
 assert(loadItemGetWindowAssets("romfs"));assert(w.description()==description.text); // Reload retains old resource.
 w.closeAutoDelete(name);w.closeAutoDelete(help);assert(w.retained() && w.window(name).sequence.flags==11 && w.window(name).sequence.clock==0);
 for(unsigned i=0;i<9;++i)w.managerTick(0,seq);
 assert(!w.retained() && w.displaySlots()==0 && w.action(name)==1); // Deleted manager retains4, event must not await it.
 const auto retired=w.window(name).sequence;
 assert(w.enter(0x80313dc8,"reuse")==0);
 assert(w.window(0).sequence.alpha==retired.alpha && w.window(0).sequence.scale==retired.scale && w.window(0).sequence.rotation==retired.rotation);
 assert(w.window(0).helpText==nullptr && w.window(0).helpPage==0 && w.window(0).sequence.flags==1);
 w.closeAutoDelete(0);for(unsigned i=0;i<9;++i)w.managerTick(0,seq);
 assert(w.prepare(font));assert(w.enter(0x80313dc8,"new")==0);
 ColorWindowText compiled;assert(compileColorWindowText(font,"A<col ff112280>B</col>C",compiled));
 probe={};assert(drawColorWindowFont(font,compiled,{0x000000ff,0x112233ff,2,128,true},s,0,0));
 assert(probe.q.size()==15 && probe.rng==30);
 assert(probe.q[4].rgba==0x00000080 && probe.q[9].rgba==0xff112240 && probe.q[14].rgba==0x11223380);
 for(unsigned i=0;i<15;++i)if(i%5!=4)assert(probe.q[i].rgba==0x11223380);

 MessageFont wide=font;for(auto& glyph:wide.glyphs)glyph.width=40;
 ItemGetWindows wideWindow;assert(wideWindow.prepare(wide));assert(wideWindow.enter(0x80313dc8,"wide")==0);
 wideWindow.managerTick(0,seq);const auto oldFrame=wideWindow.display(0);
 assert(oldFrame.frame.rectangle.width==350 && !oldFrame.content);
 assert(wideWindow.window(0).rectangle.width>350 && wideWindow.window(0).rectangle.x==int32_t(-float(wideWindow.window(0).rectangle.width)/2));
 const auto resizedFrame=wideWindow.display(0);assert(resizedFrame.frame.rectangle.width==wideWindow.window(0).rectangle.width);

 std::cout<<"Original item95 title colors, window gates, help publication and AutoDelete ownership passed\n";
}
