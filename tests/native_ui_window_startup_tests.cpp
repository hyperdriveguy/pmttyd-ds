#include <memory>
#include "native_ui_startup.hpp"
#include "field_party.hpp"
#include "event_heap_allocation.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
using namespace ttyd;
namespace {
uint32_t word(EventAddressSpace&m,uint32_t a){uint32_t v;assert(m.readWord(a,v));return v;}
EventNativePlatform platform(){EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};p.truncate=[](void*,float x){assert(std::isfinite(x)&&double(x)>=-2147483648.0&&double(x)<2147483648.0);return int32_t(x);};p.subtractDouble=[](void*,double a,double b){volatile double v=a-b;return v;};p.roundDoubleFloat=[](void*,double v){volatile float f=float(v);return f;};return p;}
std::shared_ptr<NativeFieldAnimation>actual(){auto n=NativeFieldAnimation::create("romfs",NativeFileCache::create(),platform());assert(n&&n->ready());return n;}
struct Gate {bool archive=true,heap=true,job=true,delivery=true,callback=true;NativeUiStartup*ui=nullptr;static bool admit(void*p,NativeUiStartupBoundary at,uint32_t,uint32_t n){auto&g=*static_cast<Gate*>(p);if(g.ui)assert(!g.ui->resumeWindow());switch(at){case NativeUiStartupBoundary::WindowArchive:return g.archive;case NativeUiStartupBoundary::WindowJob:return g.job;case NativeUiStartupBoundary::WindowCallback:return g.callback;case NativeUiStartupBoundary::HeapTransfer:return n!=321984||g.heap;case NativeUiStartupBoundary::DvdDelivery:return n!=321984||g.delivery;}return false;}};
void realWindowAndMario(){
 auto n=actual();Gate g;g.heap=g.job=g.delivery=g.callback=false;auto life=std::make_shared<int>(1);auto ui=NativeUiStartup::create(n,"romfs",{&g,life,Gate::admit});assert(ui&&ui->resumeFont());g.ui=ui.get();
 auto&vm=n->process().vm();const auto gp=word(vm.memory,0x80415fd0);assert(vm.memory.writeWord(gp+0x16c,2));
 assert(!ui->beginWindow()&&ui->pending()==0x800300f0);auto c=ui->windowCounts();assert(c.archiveDescriptors==4&&c.opens==1&&c.heapAllocations==1&&!c.heapTransfers&&!c.jobs&&!c.reads&&!ui->windowMemory()&&!ui->windowReady());assert(ui->word(0x8041ea1c)==0);
 g.heap=true;assert(!ui->beginWindow()&&ui->pending()==0x80144de4);c=ui->windowCounts();assert(c.heapTransfers==1&&!c.jobs&&!c.reads);assert(!ui->beginWindow()&&ui->windowCounts().opens==1&&ui->windowCounts().heapAllocations==1);
 g.job=true;assert(ui->beginWindow()&&ui->windowCounts().jobs==1&&!ui->windowReady());assert(ui->beginWindow()&&!ui->windowCounts().reads&&!ui->windowCounts().callbacks);const auto palette=ui->word(0x8041ea18);assert(palette&&*palette&&ui->word(*palette)==0&&ui->word(0x8041ea24)==0);
 assert(!ui->pumpWindow()&&ui->windowCounts().reads==1&&!ui->windowCounts().callbacks&&ui->word(*palette)==0);assert(!ui->pumpWindow()&&ui->windowCounts().reads==1);
 g.delivery=true;assert(!ui->pumpWindow()&&ui->word(*palette)==0x0020af30&&!ui->windowCounts().callbacks&&ui->word(0x8041ea1c)==0);
 g.callback=true;assert(ui->pumpWindow()&&ui->windowReady()&&ui->windowMemory());c=ui->windowCounts();assert(c.jobs==1&&c.reads==1&&c.callbacks==1&&c.closes==1&&c.heapAllocations==2&&c.heapTransfers==2&&c.objectClears==51);
 assert(ui->word(0x8041ea1c)==1&&ui->word(*palette+4)==51&&ui->word(*palette+8).value()>0x80000000u);auto object=ui->windowObject(0);assert(object&&object->palette==*palette&&object->address==ui->word(0x8041ea24)&&ui->windowMemory()->word(object->address+20)==14&&!ui->windowObject(51));
 assert(object->image.index==0&&object->image.width==72&&object->image.height==72&&object->image.format==14&&!object->image.paletteHeader&&object->image.minFilter==1&&object->image.magFilter==1);
 for(uint32_t i=0;i<51;++i){const auto v=ui->windowObject(i);assert(v&&(v->image.format==0||v->image.format==14)&&ui->windowMemory()->word(v->address+20)==v->image.format&&ui->windowMemory()->word(v->address+12)==((v->image.data>>5)&0x1fffff));}
 const auto format=object->image.imageHeader+4;assert(ui->windowMemory()->storeWord(format,0x123));assert(!ui->windowObject(0)&&ui->windowReady());assert(ui->windowMemory()->storeWord(format,14)&&ui->windowObject(0));
 const auto shape=ui->windowMemory()->word(object->address+8);assert(shape&&((*shape&1023)+1)==72&&(((*shape>>10)&1023)+1)==72);
 assert(ui->language()==2&&ui->resumeWindow()&&ui->windowCounts().callbacks==1&&ui->counts().gpStores==1&&ui->counts().reads==1);
 assert(loadFieldPlayerPoseAssets("romfs"));FieldPartyRegistry parties;FieldMarioBattleState mario;assert(parties.configureNativeAnimation(n,fieldPlayerPoseResources())&&parties.initializeMario(mario,{},0));assert(n->tick(false)&&ui->language()==2&&ui->windowReady());
 EventScriptAssets font,window;assert(font.load("romfs/source-ui-startup/memory.pmevt")&&window.load("romfs/source-window-startup/memory.pmevt"));EventNativeRuntime second(0x81100000,platform());assert(second.memory.importScripts(font)&&second.memory.importScripts(window));auto fm=ui->fontMemory();auto fb=fm->bind(second.memory);assert(fb);auto wm=ui->windowMemory();auto wb=wm->bind(second.memory,ui,ui.get(),[](void*p){return static_cast<NativeUiStartup*>(p)->current();});assert(wb&&word(second.memory,0x8041ea1c)==1&&word(second.memory,object->address+20)==14);
 assert(second.memory.writeByte(object->address,object->bytes[0]^1));assert(ui->windowReady()&&!ui->windowObject(0)&&ui->windowMemory()&&ui->resumeWindow());assert(second.memory.writeByte(object->address,object->bytes[0])&&ui->windowReady());
 ui->retire();assert(!wb->current()&&!wm->current()&&!ui->windowReady());wb.reset();fb.reset();
}
void sourceResourceFootprint(){
 auto n=actual();Gate g;g.job=false;auto ui=NativeUiStartup::create(n,"romfs",{&g,std::make_shared<int>(1),Gate::admit});assert(ui&&!ui->beginWindow());
 const auto gp=word(n->process().vm().memory,0x80415fd0);assert(!ui->ownsSourceResource(gp+0x16c)&&!ui->sourceResourceWord(gp+0x16c)&&!ui->ownsSourceResource(0x60030098));
 assert(ui->ownsSourceResource(0x8041ea1c,4)&&ui->sourceResourceWord(0x8041ea1c)==0&&!ui->windowReady()&&!ui->windowObject(0));
 assert(ui->sourceResourceByte(0x802c3080)=='f'&&ui->sourceResourceHalf(0x802c3080)==0x666f&&ui->sourceResourceWord(0x802c3080)==0x666f6e74);
 const auto palette=ui->sourceResourceWord(0x8041ea18);assert(palette&&*palette&&ui->ownsSourceResource(*palette,321984)&&ui->sourceResourceWord(*palette)==0);
 assert(ui->ownsSourceResource(0x8041f497,2)&&!ui->sourceResourceHalf(0x8041f497)&&!ui->sourceResourceWord(0x8041f495));
 assert(!ui->ownsSourceResource(0,0)&&!ui->ownsSourceResource(0xffffffff,2)&&!ui->sourceResourceHalf(0xffffffff)&&!ui->sourceResourceWord(0xfffffffe));
 auto fm=ui->fontMemory();assert(fm&&fm->storeWord(0x80429578,0x12345678)&&ui->sourceResourceWord(0x80429578)==0x12345678);
 ui->retire();assert(ui->ownsSourceResource(0x8041ea1c,4)&&ui->ownsSourceResource(*palette)&&!ui->sourceResourceWord(0x8041ea1c)&&!ui->sourceResourceByte(*palette));
}
void heldMath(){auto n=actual();auto ui=NativeUiStartup::create(n,"romfs");assert(ui&&ui->resumeFont());n->process().vm().platform.subtractDouble=nullptr;assert(!ui->resumeWindow()&&ui->word(0x8041ea1c)==0);auto c=ui->windowCounts();assert(c.callbacks==1&&c.closes==1&&c.objectClears==1&&c.reads==1&&c.heapAllocations==2);assert(!ui->resumeWindow()&&ui->windowCounts().callbacks==1&&ui->windowCounts().objectClears==1);n->process().vm().platform.subtractDouble=platform().subtractDouble;assert(ui->resumeWindow()&&ui->windowCounts().objectClears==51&&ui->windowCounts().reads==1);}
void alteredRemainingNeverCompletes(){
 auto n=actual();Gate g;g.delivery=false;auto ui=NativeUiStartup::create(n,"romfs",{&g,std::make_shared<int>(1),Gate::admit});assert(ui&&ui->beginWindow()&&!ui->pumpWindow());auto&vm=n->process().vm();const uint32_t d=0x60030098;assert(vm.memory.writeWord(d+0x80,321984+32));g.delivery=true;
 assert(!ui->pumpWindow()&&ui->pending()==0x80144de4&&word(vm.memory,d+0x80)==32&&ui->word(0x8041ea1c)==0&&!ui->windowCounts().callbacks&&!ui->windowCounts().closes);
 uint16_t flags;assert(vm.memory.readHalf(d+0x90,flags)&&(flags&1)&&!(flags&2));assert(!ui->pumpWindow()&&ui->windowCounts().reads==1);ui->retire();
}
void entryAliasCannotAdmitJob(){
 auto n=actual();Gate g;g.job=false;auto ui=NativeUiStartup::create(n,"romfs",{&g,std::make_shared<int>(1),Gate::admit});assert(ui&&!ui->beginWindow()&&ui->pending()==0x80144de4);auto&vm=n->process().vm();const uint32_t d=0x60030098;assert(word(vm.memory,d+0x6c)==d);
 std::vector<uint8_t>equal(152);for(unsigned i=0;i<152;++i)assert(vm.memory.readByte(d+i,equal[i]));assert(vm.memory.unregisterExternalRam(d)&&vm.memory.registerExternalRam(d,equal.data(),152));g.job=true;
 assert(!ui->beginWindow()&&!ui->windowCounts().jobs&&!ui->windowCounts().reads&&ui->windowCounts().opens==1&&word(vm.memory,d+0x90)==0x80000002);assert(!ui->beginWindow()&&!ui->windowCounts().jobs);ui->retire();ui.reset();
}
void realWindowHeapTransfer(){
 auto n=actual();auto&vm=n->process().vm();EventScriptAssets image;assert(image.load("romfs/source-window-startup/memory.pmevt")&&vm.memory.importScripts(image));auto memory=NativeSourceWindowMemory::create(n,n.get(),[](void*p){return static_cast<NativeFieldAnimation*>(p)->ready();});assert(memory);auto first=memory->bind(vm.memory,n,n.get(),[](void*p){return static_cast<NativeFieldAnimation*>(p)->ready();});assert(first);
 EventNativeRuntime second(0x81100000,platform());assert(second.memory.importScripts(image));auto peer=memory->bind(second.memory,n,n.get(),[](void*p){return static_cast<NativeFieldAnimation*>(p)->ready();});assert(peer);
 const auto base=eventHeapAllocate(0,65,vm.memoryBinding.services());auto backing=std::make_shared<std::vector<uint8_t>>();assert(base&&vm.memory.extractRam(base,96,*backing));
 assert(second.memory.withRawRamAccess([&](auto&){assert(!memory->adoptHeapAllocation(base,backing->data(),96,65,backing));uint8_t v;assert(!vm.memory.readByte(base,v));return true;}));
 assert(memory->adoptHeapAllocation(base,backing->data(),96,65,backing)&&memory->byte(base)==0&&!memory->byte(base+65));assert(memory->storeByte(base+90,33)&&memory->byte(base+90)==33);
 auto equal=*backing;assert(second.memory.unregisterExternalRam(base)&&second.memory.registerExternalRam(base,equal.data(),65));const std::vector<uint8_t>actualBytes{1,2,3,4};assert(!memory->deliverHeapRead(base,actualBytes)&&memory->byte(base)==0&&equal[0]==0);assert(second.memory.unregisterExternalRam(base)&&second.memory.registerExternalRam(base,backing->data(),65));
 assert(memory->deliverHeapRead(base,actualBytes)&&memory->word(base)==0x01020304&&word(second.memory,base)==0x01020304&&!memory->byte(base+65)&&memory->byte(base+90)==33&&!memory->deliverHeapRead(base,actualBytes));peer.reset();first.reset();
}
void actualArchiveFrontiers(){
 {auto n=actual();Gate g;g.archive=false;auto ui=NativeUiStartup::create(n,"romfs",{&g,std::make_shared<int>(1),Gate::admit});assert(ui&&!ui->beginWindow());auto&vm=n->process().vm();assert(vm.memory.writeHalf(0x803dbff0,1));g.archive=true;assert(!ui->beginWindow()&&ui->pending()==0x80078198&&ui->windowCounts().archiveDescriptors==0&&!ui->windowCounts().opens);assert(vm.memory.writeHalf(0x803dbff0,0));assert(!ui->beginWindow()&&ui->pending()==0x80078198&&!ui->windowCounts().opens);}
 {auto n=actual();Gate g;g.archive=false;auto ui=NativeUiStartup::create(n,"romfs",{&g,std::make_shared<int>(1),Gate::admit});assert(ui&&!ui->beginWindow());auto&vm=n->process().vm();std::vector<uint8_t>original;assert(vm.memory.extractRam(0x803dbff0,160,original)&&vm.memory.registerRam(0x803dbff0,std::vector<uint8_t>(original.begin(),original.begin()+80)));g.archive=true;assert(!ui->beginWindow()&&ui->windowCounts().archiveDescriptors==2&&!ui->windowCounts().opens);assert(vm.memory.registerRam(0x803dc040,std::vector<uint8_t>(original.begin()+80,original.end())));assert(ui->resumeWindow()&&ui->windowCounts().archiveDescriptors==4&&ui->windowReady());}
}
}
int main(){sourceResourceFootprint();realWindowAndMario();heldMath();alteredRemainingNeverCompletes();entryAliasCannotAdmitJob();realWindowHeapTransfer();actualArchiveFrontiers();std::cout<<"actual native Window startup/job/callback/Mario tests PASS\n";}
