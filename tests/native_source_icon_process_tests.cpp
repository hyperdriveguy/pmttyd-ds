#include <memory>
#include "native_source_icon_process.hpp"
#include "native_ui_startup.hpp"
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iostream>
using namespace ttyd;
namespace {
const char*startup(){const auto*p=std::getenv("TTYD_ICON_STARTUP_PACK");return p?p:"romfs/source-icon-startup/startup.bin";}
EventNativePlatform platform(){EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};p.truncate=[](void*,float x){assert(std::isfinite(x)&&double(x)>=-2147483648.0&&double(x)<2147483648.0);return int32_t(x);};p.subtractDouble=[](void*,double a,double b){volatile double v=a-b;return v;};p.roundDoubleFloat=[](void*,double a){volatile float v=float(a);return v;};return p;}
struct F {std::shared_ptr<NativeFieldAnimation>process;std::shared_ptr<NativeUiStartup>ui;std::shared_ptr<NativeSourceIconProcess>icons;
 F(NativeSourceIconAdmission policy={}){process=NativeFieldAnimation::create("romfs",NativeFileCache::create(),platform());assert(process&&process->ready());ui=NativeUiStartup::create(process,"romfs");assert(ui);icons=NativeSourceIconProcess::create(process,"romfs/source-icons",startup(),std::move(policy));assert(icons);}
};
void actual(){F f;assert(f.icons->current()&&!f.icons->ready()&&!f.icons->word(0x8041e630));assert(f.icons->begin());auto c=f.icons->counts();assert(c.allocations==4&&c.transfers==4&&c.opens==2&&c.jobs==2&&c.clears==2&&!c.callbacks&&!c.reads);assert(f.icons->word(0x8041e638)==0&&f.icons->word(0x8041e63c)==0);const auto tpl=f.icons->word(0x8041e630),bin=f.icons->word(0x8041e634);assert(tpl&&*tpl&&bin&&*bin&&*tpl!=*bin);assert(f.icons->word(*tpl)==0); // Actual allocator state; DVD not delivered.
 assert(f.icons->pump()&&f.icons->ready());c=f.icons->counts();assert(c.deliveries==2&&c.reads==2&&c.callbacks==2&&c.closes==2);assert(f.icons->word(0x8041e638)==1&&f.icons->word(0x8041e63c)==1&&f.icons->word(*tpl+4)==613&&f.icons->word(*tpl+8)==*tpl+12);auto a=f.icons->assets();assert(a);for(unsigned i=0;i<8192;++i)assert(f.icons->byte(*bin+i)==a->roundedBin()[i]);assert(f.icons->word(0x80415080)==a->word(0x80415080));auto memory=f.icons->memory();assert(memory);
 // Cross-VM view exposes actual pointers/relocations, not SourceIconResources
 // synthetic directory/palette globals or index+1 object handles.
 auto second=std::make_shared<EventNativeRuntime>(0x83000000,platform());auto binding=memory->bind(second->memory,second,second.get(),+[](void*p){uint32_t gp;return static_cast<EventNativeRuntime*>(p)->memory.readWord(0x80415fd0,gp)&&gp==0x83000000;});assert(binding);uint32_t v;assert(second->memory.readWord(0x8041e630,v)&&v==*tpl);assert(second->memory.readWord(*tpl+8,v)&&v==*tpl+12);assert(second->memory.writeWord(*bin,0x12345678)&&f.icons->word(*bin)==0x12345678);
 assert(f.icons->pump()&&f.icons->counts().reads==2&&f.icons->counts().callbacks==2);
 assert(!NativeSourceIconProcess::create(f.process,"romfs/source-icons",startup()));binding->retire();f.icons->retire();assert(!f.icons->ready()&&!f.icons->current()&&!memory->current()&&f.icons->owns(*bin)&&!f.icons->word(*bin));assert(!NativeSourceIconProcess::create(f.process,"romfs/source-icons",startup()));assert(f.process->ready());}
void holds(){struct Gate {unsigned remaining=2;NativeSourceIconBoundary at=NativeSourceIconBoundary::HeapTransfer;std::shared_ptr<NativeSourceIconProcess>*owner=nullptr;bool active=true;};for(auto boundary:{NativeSourceIconBoundary::HeapTransfer,NativeSourceIconBoundary::Delivery,NativeSourceIconBoundary::Callback}){auto gate=std::make_shared<Gate>();gate->at=boundary;NativeSourceIconAdmission a;a.lifetime=gate;a.admit=[gate](NativeSourceIconBoundary at,uint32_t,uint32_t){if(gate->owner){assert(!(*gate->owner)->begin()&&!(*gate->owner)->pump()&&!(*gate->owner)->current());(*gate->owner)->retire();}if(gate->active&&at==gate->at&&gate->remaining){--gate->remaining;return false;}return true;};F f(a);gate->owner=&f.icons;
 assert(!f.icons->pump());auto first=f.icons->counts();if(boundary==NativeSourceIconBoundary::HeapTransfer)assert(first.allocations==1&&!first.transfers);else if(boundary==NativeSourceIconBoundary::Delivery)assert(first.reads==1&&!first.deliveries&&!first.callbacks);else assert(first.reads==1&&first.deliveries==1&&!first.callbacks);
 assert(!f.icons->pump());for(unsigned i=0;i<10&&!f.icons->pump();++i){}assert(f.icons->ready());auto c=f.icons->counts();assert(c.allocations==4&&c.transfers==4&&c.jobs==2&&c.reads==2&&c.deliveries==2&&c.callbacks==2&&c.closes==2);gate->active=false;f.icons->retire();}}
void lifetime(){F f;assert(f.icons->begin());auto&vm=f.process->process().vm();uint32_t gp;assert(vm.memory.readWord(0x80415fd0,gp));assert(vm.gpAddresses.unregisterGp(gp)&&vm.gpAddresses.registerGp(gp,vm.gp)&&vm.gpAddresses.writeWord(0x80415fd0,gp));assert(!f.icons->pump()&&!f.icons->current()&&!f.icons->word(0x8041e630));f.icons->retire();assert(!NativeSourceIconProcess::create(f.process,"romfs/source-icons",startup()));
 F other;std::weak_ptr<NativeFieldAnimation>p=other.process;auto memory=other.icons->memory();auto icons=other.icons;other.icons.reset();other.ui.reset();other.process.reset();assert(!p.expired()&&icons->pump()&&icons->ready());icons->retire();icons.reset();assert(!memory->current());memory.reset();assert(p.expired());}
}
int main(){actual();holds();lifetime();std::cout<<"actual native icon heap/DVD/callback ownership tests PASS\n";}
