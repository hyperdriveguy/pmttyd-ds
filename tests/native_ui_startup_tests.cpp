#include <memory>
#include "native_ui_startup.hpp"
#include "native_source_font_memory.hpp"
#include "event_heap_allocation.hpp"
#include "field_party.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <iostream>
using namespace ttyd;
namespace {
uint32_t word(EventAddressSpace&m,uint32_t a){uint32_t v;assert(m.readWord(a,v));return v;}
EventNativePlatform platform(){EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};p.truncate=[](void*,float x){assert(std::isfinite(x)&&double(x)>=-2147483648.0&&double(x)<2147483648.0);return int32_t(x);};return p;}
struct Gate {bool heap=false,read=false,reenter=true;NativeUiStartup*ui=nullptr;unsigned visits=0;static bool admit(void*p,NativeUiStartupBoundary b,uint32_t,uint32_t){auto&g=*static_cast<Gate*>(p);++g.visits;if(g.reenter&&g.ui)assert(!g.ui->resumeFont());return b==NativeUiStartupBoundary::HeapTransfer?g.heap:g.read;}};
std::shared_ptr<NativeFieldAnimation>actual(){auto n=NativeFieldAnimation::create("romfs",NativeFileCache::create(),platform());assert(n&&n->ready());return n;}
void sourceClosure(){
 auto native=actual();auto&vm=native->process().vm();const auto gp=word(vm.memory,0x80415fd0);assert(word(vm.memory,gp+0x16c)==0);
 Gate gate;auto life=std::make_shared<int>(1);auto ui=NativeUiStartup::create(native,"romfs",{&gate,life,Gate::admit});assert(ui);gate.ui=ui.get();assert(!ui->fontObject()&&!ui->fontMemory());
 assert(!ui->resumeFont()&&ui->pending()==0x800300f0);auto c=ui->counts();assert(c.gpStores==1&&c.opens==8&&c.heapAllocations==1&&c.heapTransfers==0&&c.reads==0&&word(vm.memory,gp+0x16c)==1);
 const auto descriptors=word(vm.memory,0x8041f070);const auto handle=word(vm.memory,0x8041e844);const auto allocated=word(vm.memory,descriptors+12*handle+8);const auto base=allocated+32;
 std::array<uint32_t,3>header{word(vm.memory,allocated),word(vm.memory,allocated+4),word(vm.memory,allocated+8)};
 // Source allocator already ran. A foreign external alias refuses transfer;
 // fixing that ownership never reexecutes allocator/clear/list publication.
 std::vector<uint8_t>saved;assert(vm.memory.extractRam(base,66560,saved));for(auto b:saved)assert(b==0);assert(vm.memory.registerExternalRam(base,saved.data(),saved.size()));gate.heap=true;
 assert(!ui->resumeFont()&&ui->counts().heapAllocations==1&&ui->counts().heapTransfers==0);assert(vm.memory.unregisterExternalRam(base)&&vm.memory.registerRam(base,saved));
 assert(!ui->resumeFont()&&ui->pending()==0x80144e18);c=ui->counts();assert(c.heapTransfers==1&&c.heapAllocations==1&&c.reads==1&&c.closes==0&&ui->word(base)==std::optional<uint32_t>(0));
 assert(!ui->resumeFont()&&!ui->resumeFont()&&ui->counts().reads==1&&ui->counts().opens==8); // no native FILE read prefix replay
 gate.read=true;assert(ui->resumeFont()&&ui->pending()==0);c=ui->counts();assert(c.gpStores==1&&c.opens==8&&c.reads==1&&c.closes==1&&c.heapAllocations==1&&c.heapTransfers==1);
 const auto object=ui->fontObject();assert(object&&object->font==base&&object->object==0x8041e97c&&object->image==base+1024&&object->width==512&&object->height==256);
 const auto memory=ui->fontMemory();assert(memory&&memory->byte(base+77)==9&&ui->word(base)==0x464f4e54&&ui->word(base+8)==66560);
 assert((header==std::array<uint32_t,3>{word(vm.memory,allocated),word(vm.memory,allocated+4),word(vm.memory,allocated+8)}));
 assert(ui->resumeFont()&&ui->counts().gpStores==1&&ui->counts().heapAllocations==1);assert(vm.memory.writeWord(gp+0x16c,2)&&ui->language()==2&&ui->resumeFont()&&word(vm.memory,gp+0x16c)==2); // no per-draw or later-language reset
 EventScriptAssets image;assert(image.load("romfs/source-ui-startup/memory.pmevt"));EventNativeRuntime second(0x81100000,platform());assert(second.memory.importScripts(image));auto secondBinding=memory->bind(second.memory);assert(secondBinding&&word(second.memory,0x8041e978)==base);uint8_t width;assert(second.memory.readByte(base+77,width)&&width==9);assert(second.memory.writeByte(base+77,8)&&memory->byte(base+77)==8);assert(vm.memory.readByte(base+77,width)&&width==8);assert(second.memory.writeByte(base+77,9));
 std::weak_ptr<NativeFieldAnimation>weak=native;auto pin=ui->lifetime();native.reset();assert(!weak.expired()&&ui->fontObject());ui->retire();assert(!ui->current()&&!ui->fontObject()&&!ui->resumeFont());secondBinding.reset();ui.reset();assert(!weak.expired());pin.reset();assert(weak.expired());
}
void replacedDeliveryNoAck(){
 auto n=actual();Gate gate;gate.heap=true;auto life=std::make_shared<int>(1);auto ui=NativeUiStartup::create(n,"romfs",{&gate,life,Gate::admit});assert(ui&&!ui->resumeFont()&&ui->pending()==0x80144e18);
 auto&vm=n->process().vm();const auto d=word(vm.memory,0x8041f070),h=word(vm.memory,0x8041e844),base=word(vm.memory,d+12*h+8)+32;
 auto equal=std::vector<uint8_t>(66560,0);assert(vm.memory.unregisterExternalRam(base)&&vm.memory.registerExternalRam(base,equal.data(),equal.size()));gate.read=true;
 assert(!ui->resumeFont()&&ui->pending()==0x80144e18&&!ui->fontObject());const auto c=ui->counts();assert(c.reads==1&&c.heapAllocations==1&&c.closes==0&&equal[0]==0&&ui->word(base)==0);
 assert(!ui->resumeFont()&&ui->counts().reads==1&&ui->counts().closes==0);ui->retire();ui.reset();
}
void actualMissingNumeric(){auto n=actual();auto ui=NativeUiStartup::create(n,"romfs");assert(ui);n->process().vm().platform.truncate=nullptr;assert(!ui->resumeFont()&&ui->pending()==0x802b7580&&!ui->fontObject());const auto c=ui->counts();n->process().vm().platform.truncate=platform().truncate;assert(ui->resumeFont()&&ui->counts().heapAllocations==c.heapAllocations&&ui->counts().reads==c.reads&&ui->fontObject());}
void actualPoseLifetime(){
 auto native=actual();auto ui=NativeUiStartup::create(native,"romfs");assert(ui&&ui->resumeFont());
 const auto original=ui->fontObject();const auto count=ui->counts();
 auto&vm=native->process().vm();const auto gp=word(vm.memory,0x80415fd0);assert(vm.memory.writeWord(gp+0x16c,2));
 assert(loadFieldPlayerPoseAssets("romfs"));FieldPartyRegistry parties;FieldMarioBattleState mario;
 assert(parties.configureNativeAnimation(native,fieldPlayerPoseResources())&&parties.initializeMario(mario,{},0));
 assert(native->tick(false)&&ui->language()==2&&ui->fontObject()&&ui->resumeFont());
 parties.reinitializeMario(mario);assert(ui->language()==2);
 // Retire actual role views before original source Release invalidates poses.
 for(auto&view:parties.poses)view.reset();
 for(auto handle:mario.poseHandles)assert(native->release(handle));
 assert(ui->current()&&ui->fontObject()->font==original->font&&ui->language()==2&&ui->resumeFont());
 assert(ui->counts().gpStores==count.gpStores&&ui->counts().heapAllocations==count.heapAllocations);
 assert(!NativeUiStartup::create(native,"romfs")); // not another cold incarnation
 auto old=ui->fontMemory();auto pin=ui->lifetime();ui->retire();assert(!ui->fontObject());
 auto fresh=actual();auto next=NativeUiStartup::create(fresh,"romfs");assert(next&&next->resumeFont()&&next->language()==1&&next->fontMemory()!=old);
}
void replacedCompletedObject(){
 auto n=actual();auto ui=NativeUiStartup::create(n,"romfs");assert(ui&&ui->resumeFont()&&ui->current());
 auto m=ui->fontMemory();auto object=ui->fontObject();assert(m&&object);
 assert(m->storeByte(object->object,m->byte(object->object).value()^1));
 assert(!ui->current()&&!ui->fontObject()&&!ui->fontMemory()&&!ui->resumeFont());
}
void knownHeapMemory(){
 auto native=actual();auto&vm=native->process().vm();EventScriptAssets image;assert(image.load("romfs/source-ui-startup/memory.pmevt")&&vm.memory.importScripts(image));auto m=NativeSourceFontMemory::create();auto first=m->bind(vm.memory);assert(first);
 EventNativeRuntime second(0x81100000,platform());assert(second.memory.importScripts(image));auto other=m->bind(second.memory);assert(other);
 const auto base=eventHeapAllocate(0,65,vm.memoryBinding.services());assert(base);auto bytes=std::make_shared<std::vector<uint8_t>>();assert(vm.memory.extractRam(base,96,*bytes));
 // Native known-prefix transfer rollback while a borrowing VM blocks mapping.
 assert(second.memory.withRawRamAccess([&](auto&){assert(!m->adoptHeapAllocation(base,bytes->data(),96,65,bytes));uint8_t v;assert(!vm.memory.readByte(base,v));return true;}));
 assert(m->adoptHeapAllocation(base,bytes->data(),96,65,bytes));assert(m->byte(base)==0&&!m->byte(base+65)&&m->owns(base+80));
 auto delivered=std::vector<uint8_t>{1,2,3,4};assert(!m->installRead(base,delivered));assert(second.memory.withRawRamAccess([&](auto&){assert(!m->deliverHeapRead(base,delivered));return true;}));assert(m->byte(base)==0);
 auto same=*bytes;assert(second.memory.unregisterExternalRam(base)&&second.memory.registerExternalRam(base,same.data(),65));
 assert(!m->deliverHeapRead(base,delivered)&&m->word(base)==0&&same[0]==0);
 assert(second.memory.unregisterExternalRam(base)&&second.memory.registerExternalRam(base,bytes->data(),65));
 assert(second.memory.withRawRamAccess([&](auto&ram){assert(ram.matchesBacking(base,65,bytes->data()));assert(!ram.matchesBacking(base,65,same.data())&&!ram.matchesBacking(base,0,bytes->data())&&!ram.matchesBacking(base,66,bytes->data())&&!ram.matchesBacking(0xfffffff0,32,bytes->data())&&!ram.matchesBacking(base,65,nullptr));return true;}));
 assert(m->deliverHeapRead(base,delivered)&&m->word(base)==0x01020304&&m->byte(base+64)==0&&!m->byte(base+65));assert(word(second.memory,base)==0x01020304&&!m->deliverHeapRead(base,delivered));
}
}
int main(){sourceClosure();replacedDeliveryNoAck();actualMissingNumeric();actualPoseLifetime();replacedCompletedObject();knownHeapMemory();std::cout<<"actual native UI font startup prefix tests PASS\n";}
