#include "field_light_registry_owner.hpp"
#include "event_native_runtime.hpp"
#include "event_heap_allocation.hpp"
#include "native_field_animation.hpp"
#include "event_script_assets.hpp"
#include "fixtures/harbor_light_golden.hpp"
#include <cassert>
#include <cstring>
#include <string>
#include <vector>
using namespace ttyd;
namespace {
using namespace FieldLightRegistrySource;
constexpr uint32_t HeapBase=0x86000000,InputBase=0x87000000;
constexpr uint32_t NameInput=InputBase,PositionInput=InputBase+0x80,VectorInput=InputBase+0x90,ColorInput=InputBase+0xa0;
uint32_t bits(float value){uint32_t out=0;std::memcpy(&out,&value,4);return out;}
uint32_t word(EventAddressSpace& memory,uint32_t address){uint32_t value=0;assert(memory.readWord(address,value));return value;}
void text(EventAddressSpace& memory,uint32_t address,const char* value){for(unsigned n=0;;++n){assert(memory.writeByte(address+n,uint8_t(value[n])));if(!value[n])break;}}
std::vector<uint8_t> bytes(EventAddressSpace& memory,uint32_t address,uint32_t size){std::vector<uint8_t> out(size);for(uint32_t n=0;n<size;++n)assert(memory.readByte(address+n,out[n]));return out;}
struct Flush {
 bool available=false;unsigned calls=0;
 std::array<uint8_t*,2> pointers{};
 static bool run(void* user,uint8_t* data,uint32_t size){auto& f=*static_cast<Flush*>(user);assert(size==Capacity*RecordBytes && reinterpret_cast<uintptr_t>(data)%32==0);++f.calls;if(!f.pointers[0])f.pointers[0]=data;else if(data!=f.pointers[0])f.pointers[1]=data;return f.available;}
};
}
void nativeLightOwner(const char* root){
 FieldLightRequest request,otherRequest;
 EventNativeRuntime vm{0x81000000,{}};EventScriptAssets definitions;const std::string base=root;
 assert(definitions.load((base+"/field-light-registry.pmevt").c_str()) && vm.memory.importScripts(definitions));
 assert(vm.memory.registerRam(ActiveGroup,std::vector<uint8_t>(4)) && vm.memory.registerRam(InputBase,std::vector<uint8_t>(256)));
 Flush flush;FieldLightHeapServices heap{&flush,Flush::run};auto owner=FieldLightRegistryOwner::bind(vm.memory,HeapBase,heap);assert(owner);
 assert(!FieldLightRegistryOwner::bind(vm.memory,HeapBase,heap));
 assert(!owner->initialize(request) && owner->pending() && !owner->ready());
 assert(word(vm.memory,Work)==Capacity && word(vm.memory,Work+4)==0);
 MapLightQuery q;q.function=0x8001bad0;q.maximum=8;uint32_t pending=0;assert(!owner->query(q,pending) && pending==q.function);
 assert(vm.memory.writeByte(HeapBase+7,0xee));flush.available=true;assert(!owner->initialize(otherRequest) && flush.calls==1);
 assert(owner->initialize(request) && owner->ready() && !owner->pending() && flush.calls==3 && flush.pointers[0]!=flush.pointers[1]);
 assert(word(vm.memory,Work+4)==HeapBase && word(vm.memory,Work+12)==HeapBase+Capacity*RecordBytes);
 uint8_t marker=0;assert(vm.memory.readByte(HeapBase+7,marker) && marker==0xee); // Held cache child never re-zeroed.
 assert(owner->preset(1) && owner->preset(1)->flags()==0x1040 && word(vm.memory,Presets+RecordBytes+Position)==bits(-600));
 for(unsigned index=0;index<4;++index){assert(owner->preset(index)->color()==0xffffffff && word(vm.memory,Presets+index*RecordBytes+Reference)==0);}
 assert(owner->find(request,0x8041f8c4)==0); // Static PAPER is not an ordinary scene entry.
 text(vm.memory,NameInput,"lamp");for(unsigned n=0;n<3;++n){assert(vm.memory.writeWord(PositionInput+4*n,bits(float(n+1))) && vm.memory.writeWord(VectorInput+4*n,bits(float(n+4))));}assert(vm.memory.writeWord(ColorInput,0x123456ff));
 FieldLightEntryArguments a{NameInput,1,PositionInput,VectorInput,ColorInput,0,2,{bits(0),bits(1)},0};
 assert(owner->enter(request,a)==0 && owner->find(request,NameInput)==HeapBase);
 const auto* light=owner->record(HeapBase);assert(light && light->flags()==0x1081 && light->color()==0x123456ff);
 q={};q.function=0x8001bad0;q.maximum=8;q.mask=0x80;assert(owner->query(q,pending) && q.count==1 && q.output[0]==light && !pending);
 assert(vm.memory.writeWord(HeapBase+Rgba,0xabcdef01) && light->color()==0xabcdef01); // Same physical renderer/source bytes.
 assert(owner->enter(request,a)==1);assert(owner->enter(request,a)==2);
 std::array<char,NameBytes> name{};for(unsigned n=0;n<name.size();++n){uint8_t b=0;assert(vm.memory.readByte(HeapBase+2*RecordBytes+Name+n,b));name[n]=char(b);if(!b)break;}assert(std::string(name.data())=="lamp_0000_0001");
 // Other signed groups select field bank0; exact1 owns the other allocation.
 assert(vm.memory.writeWord(ActiveGroup,1) && owner->enter(request,a)==0 && owner->find(request,NameInput)==owner->bankAddress(1));
 assert(owner->reset(request) && owner->find(request,NameInput)==0 && light->flags()==0x1081 && owner->preset(1)->flags()==0x1040);
 assert(vm.memory.writeWord(ActiveGroup,2) && owner->find(request,NameInput)==HeapBase);
 // Wrong physical mapping holds even if every replacement byte is identical.
 auto copy=bytes(vm.memory,HeapBase,Capacity*RecordBytes);auto* backing=const_cast<uint8_t*>(light->bytes.data());
 assert(vm.memory.unregisterExternalRam(HeapBase) && vm.memory.registerRam(HeapBase,copy));
 assert(!owner->record(HeapBase) && !owner->enter(request,a) && !owner->query(q,pending));
 assert(vm.memory.unregisterRam(HeapBase) && vm.memory.registerExternalRam(HeapBase,backing,Capacity*RecordBytes));
 assert(owner->record(HeapBase)==light);
 // A failed raw name read remains a retained source compare; consumers/reset
 // cannot discard it. Repair the same operand backing and finish that request.
 constexpr uint32_t MissingName=0x88000000;
 assert(!owner->find(request,MissingName) && owner->pending() && !owner->reset(request) && !owner->query(q,pending));
 assert(vm.memory.registerRam(MissingName,std::vector<uint8_t>{'n','o',0,0}));assert(!owner->find(otherRequest,MissingName) && owner->pending());assert(owner->find(request,MissingName)==0 && !owner->pending());
 // Typed native admission rejects out-of-range attenuation and a full bank
 // before clear/format/publication; the complete core separately tests overrun.
 auto invalid=a;invalid.attenuation=4;const auto before=bytes(vm.memory,HeapBase,Capacity*RecordBytes);assert(!owner->enter(request,invalid) && bytes(vm.memory,HeapBase,Capacity*RecordBytes)==before && !owner->pending());
 for(unsigned index=0;index<Capacity;++index){assert(vm.memory.writeHalf(HeapBase+index*RecordBytes,1));text(vm.memory,HeapBase+index*RecordBytes+Name,"busy");}
 const auto full=bytes(vm.memory,HeapBase,Capacity*RecordBytes);const auto scratch=bytes(vm.memory,owner->scratchAddress(),256);
 assert(!owner->enter(request,a) && !owner->pending() && bytes(vm.memory,HeapBase,Capacity*RecordBytes)==full && bytes(vm.memory,owner->scratchAddress(),256)==scratch);
 assert(owner->reset(request) && light->flags()==0 && owner->preset(1)->flags()==0x1040);
 // Process allocations and descriptor pointers survive room-style reset.
 assert(word(vm.memory,Work)==Capacity && word(vm.memory,Work+4)==HeapBase && flush.calls==3);
}
void nativeHarborLights(const char* root){
 using namespace harbor_light_fixture;
 FieldLightRequest request,otherRequest;
 EventNativeRuntime vm{0x81000000,{}};const std::string base=root;
 EventScriptAssets definitions,input;assert(definitions.load((base+"/field-light-registry.pmevt").c_str()) && input.load((base+"/field-harbor-lights-input.pmevt").c_str()));
 assert(vm.memory.importScripts(definitions) && vm.memory.importScripts(input));
 assert(vm.memory.registerRam(ActiveGroup,std::vector<uint8_t>(4)) && vm.memory.registerRam(MapEntry,std::vector<uint8_t>(0x100)));
 assert(vm.memory.writeWord(MapEntry+FieldMapLightSource::TableField,Table));
 Flush flush;flush.available=true;auto owner=FieldLightRegistryOwner::bind(vm.memory,HeapBase,{&flush,Flush::run});assert(owner && owner->initialize(request));
 assert(owner->loadMapLights(request,MapEntry) && !owner->pending());
 for(unsigned index=0;index<Lights.size();++index){
  const auto address=HeapBase+index*RecordBytes;const auto* record=owner->record(address);assert(record && record->flags()==(index?0x1001:0x8001) && record->color()==Lights[index].color);
  for(unsigned byte=0;;++byte){uint8_t value=0;assert(vm.memory.readByte(address+Name+byte,value) && value==uint8_t(Lights[index].name[byte]));if(!value)break;}
  for(unsigned axis=0;axis<3;++axis){assert(word(vm.memory,address+Position+4*axis)==Lights[index].position[axis] && word(vm.memory,address+Reference+4*axis)==Lights[index].position[axis]);}
  assert(word(vm.memory,address+Vector)==0x27fce83b && word(vm.memory,address+Vector+4)==0x43340000 && word(vm.memory,address+Vector+8)==0);
  assert(word(vm.memory,address+Scalar0)==0 && word(vm.memory,address+Scalar1)==Lights[index].scalar1);
  assert(word(vm.memory,address+Coefficients)==bits(1) && word(vm.memory,address+Coefficients+4)==0 && word(vm.memory,address+Coefficients+8)==0);
 }
 MapLightQuery query;query.function=0x8001bad0;query.maximum=8;uint32_t pending=0;assert(owner->query(query,pending) && query.count==4 && !pending);
 for(unsigned index=0;index<4;++index)assert(query.output[index]==owner->record(HeapBase+(index+1)*RecordBytes));
 assert(owner->reset(request) && owner->record(HeapBase)->flags()==0 && owner->preset(1)->flags()==0x1040);
 // Preserve completed rows while a later source field is unavailable. A later
 // fresh table count governs termination, without replaying the first Entry.
 const auto second=word(vm.memory,Table+8);std::vector<uint8_t> nameWord;
 assert(vm.memory.extractRam(second,4,nameWord));
 assert(!owner->loadMapLights(request,MapEntry) && owner->pending() && word(vm.memory,HeapBase+Position)==Lights[0].position[0]);
 assert(!owner->query(query,pending) && !owner->reset(request));
 assert(vm.memory.registerRam(second,nameWord) && vm.memory.writeWord(Table,2));
 assert(!owner->loadMapLights(otherRequest,MapEntry) && owner->pending());
 assert(owner->loadMapLights(request,MapEntry) && !owner->pending());
 assert(owner->record(HeapBase)->flags()==0x8001 && owner->record(HeapBase+RecordBytes)->flags()==0x1001 && owner->record(HeapBase+2*RecordBytes)->flags()==0);
 assert(owner->reset(request));
 // Unknown kinds retain default point type. A nonmatching magic skips FLAGS
 // even when that source word is unmapped; it is not a mandatory preload read.
 const auto first=word(vm.memory,Table+4);constexpr uint32_t UnknownName=0x87000000;
 assert(vm.memory.registerRam(UnknownName,std::vector<uint8_t>{'o','t','h','e','r',0,0,0}));
 assert(vm.memory.writeWord(Table,1) && vm.memory.writeWord(first+FieldMapLightSource::Kind,UnknownName) && vm.memory.writeWord(first+FieldMapLightSource::Magic,0));
 std::vector<uint8_t> flags;assert(vm.memory.extractRam(first+FieldMapLightSource::Flags,4,flags));
 assert(owner->loadMapLights(request,MapEntry) && owner->record(HeapBase)->flags()==0x1001);
 // Teardown removes only this owner's physical mappings, never an external
 // replacement that happens to have the same original guest address.
 auto replacement=bytes(vm.memory,HeapBase,Capacity*RecordBytes);
 assert(vm.memory.unregisterExternalRam(HeapBase) && vm.memory.registerExternalRam(HeapBase,replacement.data(),uint32_t(replacement.size())));
 owner.reset();assert(bytes(vm.memory,HeapBase,Capacity*RecordBytes)==replacement);
 assert(vm.memory.unregisterExternalRam(HeapBase,replacement.data()));
}
void nativeLightSdkConstants(const char* root){
 FieldLightRequest request;EventNativeRuntime vm{0x81000000,{}};EventScriptAssets definitions;
 assert(definitions.load((std::string(root)+"/field-light-registry.pmevt").c_str()) && vm.memory.importScripts(definitions));
 std::vector<uint8_t> one;assert(vm.memory.extractRam(0x80429358,4,one) && vm.memory.writeWord(0x8042935c,bits(2)));
 Flush flush;flush.available=true;auto owner=FieldLightRegistryOwner::bind(vm.memory,HeapBase,{&flush,Flush::run});assert(owner && owner->initialize(request));
 const auto craft=Presets+RecordBytes+Position;
 assert(word(vm.memory,craft)==bits(-198) && word(vm.memory,craft+4)==bits(102) && word(vm.memory,craft+8)==bits(102));
}
void originalMapLightBridge(const char* root){
 EventNativeRuntime vm{0x81000000,{}};EventScriptAssets definitions;auto& m=vm.memory;
 assert(definitions.load((std::string(root)+"/field-light-registry.pmevt").c_str()) && m.importScripts(definitions));
 assert(m.registerRam(ActiveGroup,std::vector<uint8_t>(4)) && m.registerRam(InputBase,std::vector<uint8_t>(256)));
 Flush flush;flush.available=true;FieldLightRequest init;
 auto owner=FieldLightRegistryOwner::bind(m,HeapBase,{&flush,Flush::run});assert(owner && owner->initialize(init));
 EventNativeRuntime foreign{0x82000000,{}};assert(owner->readyFor(m) && !owner->readyFor(foreign.memory));
 assert(word(m,CraftAmbient)==0 && word(m,PaperAmbient)==0);
 for(unsigned n=0;n<3;++n)assert(m.writeWord(PositionInput+4*n,bits(float(n+1))) && m.writeWord(VectorInput+4*n,0));
 assert(m.writeWord(ColorInput,0x123456ff));
 constexpr uint32_t MissingName=0x88000000;
 FieldMapSourceCall call;call.function=Entry;call.arguments={MissingName,1,PositionInput,VectorInput,ColorInput,0,0,0};call.scalars={bits(0),bits(1),0};
 assert(!owner->service(call) && !call.result && !owner->record(HeapBase)->flags());
 auto other=call;assert(!owner->service(other));
 call.arguments[0]=NameInput;assert(!owner->service(call));call.arguments[0]=MissingName;
 assert(m.registerRam(MissingName,std::vector<uint8_t>{'l','a','m','p',0}));
 assert(owner->service(call) && call.result==0 && owner->record(HeapBase)->flags()==0x1001);
 call.result.reset();assert(owner->service(call) && call.result==1);
 const auto duplicate=bytes(m,HeapBase+RecordBytes+Name,10);assert(std::string(duplicate.begin(),duplicate.end())==std::string("lamp_0000\0",10));
 assert(m.writeWord(ActiveGroup,1));call.result.reset();assert(owner->service(call) && call.result==0);
 assert(m.writeWord(CraftAmbient,0x11223344) && m.writeWord(PaperAmbient,0x55667788));
 FieldMapSourceCall reset;reset.function=Reinitialize;assert(owner->service(reset) && !reset.result);
 assert(!owner->record(owner->bankAddress(1))->flags() && owner->record(HeapBase)->flags()==0x1001);
 assert(word(m,CraftAmbient)==0x11223344 && word(m,PaperAmbient)==0x55667788);
 FieldMapSourceCall unknown;unknown.function=0x8000f36c;assert(!owner->service(unknown));
}

// Exercise original heap0 allocation against the actual cold process VM. The
// owner must preserve SDK headers, source publication order and a held flush.
void sourceHeapLightBanks(const char* root){
 constexpr uint32_t Scratch=0x8d0c0000,BankBytes=Capacity*RecordBytes;
 constexpr uint32_t SdkDescriptors=0x8041f070,HeapHandles=0x8041e844;
 constexpr uint32_t DescriptorBytes=12,HeaderBytes=32,NodeNext=4,NodeSize=8;
 constexpr unsigned SdkListVisitLimit=4096;
 struct Backend {
  std::shared_ptr<NativeFieldAnimation> field;
  std::array<uint32_t,2> addresses{};
  unsigned allocations=0,flushes=0;
  bool available=false,flush=false,alive=true,retireOnAllocation=false;
  EventNativeRuntime& vm(){return field->process().vm();}
  bool current(){
   if(!alive || !field->process().heapGlobalsCurrent())return false;
   auto& m=vm().memory;const auto descriptors=word(m,SdkDescriptors),handle=word(m,HeapHandles);
   uint32_t node=word(m,descriptors+DescriptorBytes*handle+NodeSize);unsigned found=0;
   // A changed head or severed SDK allocated list invalidates the provider.
   for(unsigned budget=0;node && budget<SdkListVisitLimit;++budget){
    for(unsigned i=0;i<allocations;++i)if(addresses[i] && node+HeaderBytes==addresses[i]){
     if(word(m,node+NodeSize)<BankBytes+HeaderBytes)return false;
     ++found;
    }
    node=word(m,node+NodeNext);
   }
   unsigned expected=0;for(unsigned i=0;i<allocations;++i)expected+=addresses[i]!=0;
   return found==expected;
  }
  FieldLightHeapServices services(){
   FieldLightHeapServices h;h.user=this;
   h.current=[](void* p){return static_cast<Backend*>(p)->current();};
   h.allocate=[](void* p,uint32_t heap,uint32_t size,uint32_t& out){
    auto& b=*static_cast<Backend*>(p);assert(heap==0 && size==BankBytes && b.allocations<2);
    if(!b.available)return false;
    out=eventHeapAllocate(int32_t(heap),size,b.vm().memoryBinding.services());
    b.addresses[b.allocations++]=out;
    if(b.retireOnAllocation){b.alive=false;b.retireOnAllocation=false;}
    return true;
   };
   h.flush=[](void* p,uint8_t* bytes,uint32_t size){
    auto& b=*static_cast<Backend*>(p);assert(bytes && size==BankBytes && reinterpret_cast<uintptr_t>(bytes)%32==0);
    ++b.flushes;return b.flush;
   };return h;
  }
 } backend;
 EventNativePlatform platform;platform.flushBlock=[](void*,uint32_t){};platform.systemCall=[](void*){};
 platform.truncate=[](void*,float f){return int32_t(f);};
 platform.subtractDouble=[](void*,double a,double b){volatile double v=a-b;return v;};
 platform.roundDoubleFloat=[](void*,double d){volatile float v=float(d);return v;};
 backend.field=NativeFieldAnimation::create(root,NativeFileCache::create(),platform);assert(backend.field);
 auto& m=backend.vm().memory;EventScriptAssets definitions;
 assert(definitions.load((std::string(root)+"/field-light-registry.pmevt").c_str()));
 // Shared source literals may already exist; retain their original backing.
 for(const auto& segment:definitions.segments()){
  uint32_t begin=0;std::vector<uint8_t> missing;
  auto publish=[&](){if(!missing.empty()){assert(m.registerRam(begin,std::move(missing)));missing.clear();}};
  for(uint32_t i=0;i<segment.size;++i){uint8_t expected=0,actual=0;const auto at=segment.address+i;
   assert(definitions.readByte(at,expected));
   if(m.readByte(at,actual)){assert(actual==expected);publish();}
   else{if(missing.empty())begin=at;missing.push_back(expected);}
  }publish();
 }
 assert(m.registerRam(ActiveGroup,std::vector<uint8_t>(4)));
 auto owner=FieldLightRegistryOwner::bind(m,Scratch,backend.services());FieldLightRequest request,foreign;
 assert(owner && owner->scratchAddress()==Scratch && !owner->bankAddress(0) && !owner->bankAddress(1));
 assert(!owner->initialize(request) && owner->pending() && backend.allocations==0 && word(m,Work)==Capacity);
 backend.available=true;
 assert(!owner->initialize(request) && backend.allocations==1 && backend.flushes==1);
 const auto first=owner->bankAddress(0);assert(first==backend.addresses[0] && first && first!=Scratch && word(m,Work+4)==0);
 assert(word(m,first-HeaderBytes+NodeSize)>=BankBytes+HeaderBytes); // Unchanged actual SDK header.
 assert(m.writeByte(first+7,0xed));
 assert(!owner->initialize(foreign) && backend.flushes==1);
 assert(!owner->initialize(request) && backend.allocations==1 && backend.flushes==2);
 uint8_t marker=0;assert(m.readByte(first+7,marker) && marker==0xed);
 backend.flush=true;assert(owner->initialize(request) && owner->ready() && backend.allocations==2);
 const auto second=owner->bankAddress(1);assert(second==backend.addresses[1] && second!=first && second!=first+BankBytes);
 assert(word(m,Work+4)==first && word(m,Work+12)==second && owner->record(first) && owner->record(second));
 assert(m.readByte(first+7,marker) && marker==0xed); // No wrapper clear replay.
 const auto descriptor=word(m,SdkDescriptors)+DescriptorBytes*word(m,HeapHandles),head=word(m,descriptor+NodeSize);
 assert(m.writeWord(descriptor+8,0) && !owner->ready());
 assert(m.writeWord(descriptor+8,head) && owner->ready());
 backend.alive=false;assert(!owner->ready() && !owner->reset(request));backend.alive=true;assert(owner->ready());
 auto replacement=bytes(m,first,BankBytes);assert(m.unregisterExternalRam(first) && m.registerRam(first,replacement));
 assert(!owner->ready());owner.reset();assert(bytes(m,first,BankBytes)==replacement);
 // Exhaustion is acknowledged once for each original call, with no fabricated
 // bank or repeated allocation. Source Init publishes both nulls and finishes
 // presets; native record/query consumers still reject the absent banks.
 backend.addresses={};backend.allocations=0;backend.flushes=0;
 const auto freeHead=word(m,descriptor+4);assert(m.writeWord(descriptor+4,0));
 owner=FieldLightRegistryOwner::bind(m,Scratch,backend.services());assert(owner);
 backend.retireOnAllocation=true;
 assert(!owner->initialize(request) && backend.allocations==1 && owner->pending());
 backend.alive=true;
 assert(owner->initialize(request));
 assert(backend.allocations==2 && backend.flushes==0 && !owner->bankAddress(0) && !owner->bankAddress(1));
 assert(word(m,Work+4)==0 && word(m,Work+12)==0 && !owner->record(0));
 assert(owner->initialize(request) && backend.allocations==2 && !owner->reset(request));
 assert(m.writeWord(descriptor+4,freeHead));
}

int main(){sourceHeapLightBanks("romfs");originalMapLightBridge("romfs");nativeLightOwner("romfs");nativeHarborLights("romfs");nativeLightSdkConstants("romfs");}
