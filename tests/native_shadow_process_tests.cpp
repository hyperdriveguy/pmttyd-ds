#include "native_shadow_process.hpp"
#include "native_ui_startup.hpp"
#include "native_field_animation.hpp"
#include <cassert>
#include <cmath>
using namespace ttyd;
namespace {
uint32_t word(EventAddressSpace& m,uint32_t a){uint32_t v=0;assert(m.readWord(a,v));return v;}
EventNativePlatform platform(){EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};p.truncate=[](void*,float f){assert(std::isfinite(f));return int32_t(f);};p.subtractDouble=[](void*,double a,double b){volatile double v=a-b;return v;};p.roundDoubleFloat=[](void*,double v){volatile float f=float(v);return f;};return p;}
struct Fixture {
 std::shared_ptr<NativeFieldAnimation> field;
 std::shared_ptr<NativeUiStartup> ui;
 std::shared_ptr<NativeShadowProcess> shadows;
 Fixture(const char* root){field=NativeFieldAnimation::create(root,NativeFileCache::create(),platform());assert(field&&field->process().heapGlobalsCurrent());ui=NativeUiStartup::create(field,root);assert(ui&&ui->resumeFont());}
 EventNativeRuntime& vm(){return field->process().vm();}
};
void actualNativeBootstrap(const char* root){
 Fixture f(root);auto& vm=f.vm();auto& m=vm.memory;
 const auto pool=word(m,0x8041e808),descriptor=word(m,0x8041f070),heap0=word(m,0x8041e844),heap4=word(m,0x8041e854);
 const auto start=word(m,0x8041e840),end=word(m,0x8041e82c);assert(heap0!=heap4);
 const auto uiCounts=f.ui->counts();const auto font=f.ui->fontObject();assert(font);
 assert(m.writeWord(SourceSmartArena::FirstAllocation,0x12345678));
 f.shadows=NativeShadowProcess::create(f.ui,root);assert(f.shadows&&f.shadows->current()&&!f.shadows->ready());
 assert(word(m,SourceSmartArena::FirstAllocation)==0x12345678 && f.field->process().heapGlobalsCurrent());
 assert(!f.shadows->initialize(1)&&f.shadows->busy());
 // Missing actual SDK fctiwz holds AFTER real heap4/pool/ramp prefixes.
 vm.platform.truncate=nullptr;assert(!f.shadows->initialize()&&f.shadows->pending()==0x802b7580);
 const auto before=f.shadows->counts();const auto header=word(m,SourceShadowProcess::Primary+0xfc);
 assert(before.heap4Allocations==1 && before.heap0Allocations==2 && before.objects==1 && before.lods==0);
 for(unsigned i=0;i<3;++i)assert(!f.shadows->initialize()&&f.shadows->counts().heap0Allocations==before.heap0Allocations && word(m,SourceShadowProcess::Primary+0xfc)==header);
 vm.platform.truncate=platform().truncate;assert(f.shadows->initialize()&&f.shadows->ready()&&!f.shadows->busy());
 const auto c=f.shadows->counts();assert(c.heap4Allocations==1&&c.heap0Allocations==3&&c.objects==2&&c.lods==2&&c.fills==4&&c.flushes==3);
 assert(header==SourceSmartArena::Work+4 && word(m,header+4)==160000 && word(m,header)>=start+32 && word(m,header)+160000<=end);
 assert(word(m,SourceSmartArena::Work)==word(m,header) && word(m,SourceSmartArena::FirstAllocation)==1);
 assert(word(m,descriptor+12*heap4+8)==word(m,header)-32 && word(m,0x8041e808)==pool && word(m,0x804151dc)==100);
 assert(f.ui->current()&&f.ui->fontObject()->font==font->font&&f.ui->counts().reads==uiCounts.reads&&f.ui->counts().heapAllocations==uiCounts.heapAllocations);
 assert(!NativeShadowProcess::create(f.ui,root)); // No second arena/incarnation.
 assert(f.shadows->initialize()&&f.shadows->counts().heap4Allocations==1);
 // Mutable actual gate/first flag survive completed bootstrap calls.
 assert(m.writeWord(SourceShadowProcess::Primary+0x10c,0)&&m.writeWord(SourceSmartArena::FirstAllocation,0));
 FieldMapSourceCall gate;gate.function=SourceShadowProcess::Projection;assert(f.shadows->shadow()->service(gate)&&gate.result==0);
 FieldMapSourceCall next;next.function=SourceSmartArena::Allocate;next.arguments[0]=1;while(!f.shadows->smartArena()->service(next,37)){}
 assert(next.result && *next.result!=header && word(m,SourceSmartArena::FirstAllocation)==1 && f.field->process().heapGlobalsCurrent());
 assert(f.shadows->initialize()&&word(m,SourceShadowProcess::Primary+0x10c)==0);
 // SDK heap loss cannot become a successful shadow/resource receipt.
 const auto allocated=word(m,descriptor+12*heap4+8);assert(m.writeWord(descriptor+12*heap4+8,0));assert(!f.shadows->current()&&!f.shadows->initialize());assert(m.writeWord(descriptor+12*heap4+8,allocated)&&f.shadows->ready());
 f.ui->retire();assert(!f.shadows->current()&&!f.shadows->initialize());
}
void replacedHeapGlobals(const char* root){
 Fixture f(root);f.shadows=NativeShadowProcess::create(f.ui,root);assert(f.shadows&&f.shadows->initialize());auto& m=f.vm().memory;
 std::vector<uint8_t> same(80);for(unsigned i=0;i<same.size();++i)assert(m.readByte(0x8041e808+i,same[i]));
 assert(m.unregisterExternalRam(0x8041e808)&&m.registerRam(0x8041e808,same));
 assert(!f.field->process().heapGlobalsCurrent()&&!f.shadows->current()&&!f.shadows->initialize());
 f.shadows.reset();assert(word(m,SourceSmartArena::FirstAllocation)==1); // Borrowed mapping survives teardown.
}
}
int main(){actualNativeBootstrap("romfs");replacedHeapGlobals("romfs");}
