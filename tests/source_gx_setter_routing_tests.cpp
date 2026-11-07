#include <memory>
#include "source_gx_setter_routing.hpp"
#include "display_queue.hpp"
#include "battle_texture_plane.hpp"
#include "source_font_draw_start.hpp"
#include "source_icon_gx.hpp"
#include "window_primitive_continuation.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
namespace {
struct Fixture {
 bool live=true,source=true;std::shared_ptr<int>process=std::make_shared<int>(1),scope=std::make_shared<int>(2);
 std::shared_ptr<SourceGxInheritedState>history=std::make_shared<SourceGxInheritedState>();
 std::unique_ptr<SourceGxSetterRouting>route;std::function<void()>probe;
 Fixture(){assert(history->bind({process,this,[](void*p){return static_cast<Fixture*>(p)->live;},[](void*){return true;}}));assert(history->initializeCold());route=std::make_unique<SourceGxSetterRouting>(history,scope,[this]{if(probe)probe();return source;});}
};
void shapes(){Fixture f;auto original=f.route->capture();assert(original);
 DisplayStateRequest d{DisplayGXState::AlphaUpdate,{0}};assert(f.route->accepted(d)==SourceGxObserved::Recorded);assert(!f.route->capture()&&original->current());d.values[0]=1;assert(f.route->accepted(d)==SourceGxObserved::Recorded&&f.route->capture());
 BattleTexturePlaneRequest p;p.function=BattleTexturePlaneCall::Cull;p.integers={0};assert(f.route->accepted(p)==SourceGxObserved::Recorded);
 SourceFontStateCall s;s.function=0x802b8880;s.callsite=0x80074c20;s.integers={0};assert(f.route->accepted(s)==SourceGxObserved::Ignored&&f.route->capture());
 SourceFontDrawStartFog g;g.type=1;assert(f.route->accepted(g)==SourceGxObserved::Recorded&&!f.route->capture());g.type=0;assert(f.route->accepted(g)==SourceGxObserved::Recorded&&f.route->capture());
 SourceIconGXCall icon;icon.function=0x802b8c98;icon.integers={0,0,1};assert(f.route->accepted(icon)==SourceGxObserved::Recorded&&!f.route->capture());icon.integers[2]=0;assert(f.route->accepted(icon)==SourceGxObserved::Recorded&&f.route->capture());
 icon={};icon.function=0x802b8ff4;icon.integers={0,0x12345678};assert(f.route->accepted(icon)==SourceGxObserved::Recorded);auto ip=f.route->capture();assert(ip);bool found=false;for(unsigned i=0;i<ip->size();++i)if(ip->evidence()[i].function==icon.function){found=true;assert(ip->evidence()[i].integers[1]==0x12345678);}assert(found);
 WindowPrimitiveRequest w;w.function=0x802b8ff4;w.callsite=0x8007b534;w.integers={0};w.color=0xaabbccdd;w.reals={0,0,0,0};assert(f.route->accepted(w)==SourceGxObserved::Recorded);assert(f.route->capture());
 w.kind=WindowPrimitiveRequestKind::FifoFloat;assert(f.route->accepted(w)==SourceGxObserved::Ignored);p.kind=BattleTexturePlaneKind::FifoByte;assert(f.route->accepted(p)==SourceGxObserved::Ignored);
 // Held and rejected actual providers MUST NOT call accepted. A genuinely
 // successful unknown whole effect invalidates future evidence, never the
 // already copied packet; it does not request replay of the successful child.
 SourceGxInheritedCall unknown;unknown.function=0x800217c8;bool actualAccepted=false;auto before=f.route->capture();if(actualAccepted)f.route->accepted(unknown);assert(f.route->capture());actualAccepted=true;if(actualAccepted)assert(f.route->accepted(unknown)==SourceGxObserved::Invalidated);assert(!f.route->capture()&&before->current()&&original->current());assert(f.history->initializeCold()&&!f.route->capture());
}
void lifecycle(){Fixture f;auto packet=f.route->capture();assert(packet);f.source=false;assert(!f.route->capture());assert(packet->current());DisplayStateRequest q{DisplayGXState::ColorUpdate,{0}};assert(f.route->accepted(q)==SourceGxObserved::Unavailable);f.source=true;assert(f.route->capture());
 unsigned visits=0;f.probe=[&]{++visits;assert(!f.route->capture());assert(f.route->accepted(q)==SourceGxObserved::Unavailable);};assert(f.route->compatibleNow()&&visits==2);f.probe={};assert(f.route->capture());
 f.probe=[&]{f.route->retire();};assert(!f.route->capture());f.probe={};assert(!f.route->capture()&&packet->current());f.history->retire();assert(!packet->current());
 SourceGxSetterRouting absent({},f.scope,[]{return true;});assert(!absent.capture());assert(absent.accepted(q)==SourceGxObserved::Unavailable);
}
void actualQueue(){
 Fixture f;DisplayQueue queue;assert(queue.enter(4,9,0x100,0,0)==DisplayAdmission::Registered);assert(queue.enter(8,9,0x80121ce8,0,0)==DisplayAdmission::Registered);queue.sort();
 struct Bridge{Fixture*f;unsigned states=0,holds=0,calls=0;bool wait=true;};Bridge b{&f};DisplayDispatchServices s;s.user=&b;
 s.state=[](void*p,const DisplayStateRequest&q){auto&b=*static_cast<Bridge*>(p);if(b.wait&&q.kind==DisplayGXState::AlphaUpdate){++b.holds;return false;}++b.states;const auto result=b.f->route->accepted(q);assert(result!=SourceGxObserved::Unavailable&&result!=SourceGxObserved::Invalidated);return true;};
 s.callback=[](void*p,uint32_t camera,uint32_t,uint32_t){auto&b=*static_cast<Bridge*>(p);++b.calls;assert(camera==4?!b.f->route->capture():bool(b.f->route->capture()));return true;};
 DisplayDispatchCursor c;assert(!queue.dispatch(4,s,c));assert(b.states==1&&b.holds==1&&f.route->capture());assert(!queue.dispatch(4,s,c));assert(b.states==1&&b.holds==2);b.wait=false;assert(queue.dispatch(4,s,c)&&b.calls==1&&!f.route->capture());auto cold=f.history->coldComplete();assert(cold);assert(queue.dispatch(8,s,c)&&b.calls==2&&f.route->capture());
}
}
int main(){shapes();lifecycle();actualQueue();std::cout<<"source GX setter routing tests passed\n";}
