#include "native_field_mario_get_position.hpp"
#include "event_memory_bootstrap.hpp"
#include "event_lifecycle.hpp"
#include "field_party.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>
using namespace ttyd;
namespace {
uint32_t bits(float f){uint32_t w;std::memcpy(&w,&f,4);return w;}
struct Fixture {
 NativeFacadeRegistration host;
 std::unique_ptr<EventNativeRuntime>vm;
 std::unique_ptr<NativeFieldMarioGetPosition>getter;
 EventEntry*entry=nullptr;
 std::array<std::optional<uint32_t>,3>xyz{bits(-25.9999f),bits(15.25f),bits(-100.0009f)};
 unsigned borrowed=0,coordinates=0;bool recurse=false,recursiveRejected=false;
 static EventNativePlatform platform(){EventNativePlatform p;p.clock=[](void*)->uint32_t{return 243000000;};p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};p.truncate=[](void*,float f){assert(std::isfinite(f)&&double(f)>=-2147483648.&&double(f)<2147483648.);return int32_t(f);};return p;}
 Fixture(){
  vm=std::make_unique<EventNativeRuntime>(0x81000000,platform());
  for(auto p:{std::pair<uint32_t,uint32_t>{0x8041f070,16},{0x8041e428,4},{0x8041e844,4},{0x82000000,0x80000},{0x83000000,32}})assert(vm->memory.registerRam(p.first,std::vector<uint8_t>(p.second)));
  const auto s=vm->memoryBinding.services();auto start=eventSdkInitializeAllocator(0x82000000,0x82080000,1,s);assert(eventSdkCreateHeap(start,0x82080000,s)==0);assert(vm->memory.writeWord(0x8041e844,0));vm->initializeContexts();
  assert(vm->memory.writeWord(0x83000000,1));entry=&createRootEvent(vm->schedule,0x83000000,10,0,vm->creationServices());entry->arguments=0x83000010;entry->argumentCount=4;
  assert(vm->memory.writeWord(entry->arguments,0));for(unsigned i=0;i<3;++i)assert(vm->memory.writeWord(entry->arguments+4+4*i,uint32_t(-30000000+int(i))));
  NativeFieldMarioGetPositionBinding b;b.lifetime=std::make_shared<int>(1);b.host=host.proof();b.user=this;
  b.current=[](void*p,const EventNativeRuntime&v){auto&f=*static_cast<Fixture*>(p);++f.borrowed;if(f.recurse&&f.getter)assert(!f.getter->current());return &v==f.vm.get();};
  b.position=[](void*p,unsigned axis){auto&f=*static_cast<Fixture*>(p);++f.coordinates;if(f.recurse&&f.getter){assert(!f.getter->invoke(NativeFieldMarioGetPositionFunction,*f.entry,false));f.recursiveRejected=true;}return f.xyz[axis];};
  getter=NativeFieldMarioGetPosition::create(*vm,b);assert(getter);
 }
 std::optional<int32_t>call(){return getter->invoke(NativeFieldMarioGetPositionFunction,*entry,true);}
};
void exactOutput(){Fixture f;const auto old=f.entry->locals;f.xyz[1].reset();assert(!f.call()&&f.entry->locals==old);f.xyz[1]=bits(15.25f);f.recurse=true;assert(f.call()==2&&f.recursiveRejected);const auto s=f.vm->interpreterServices().operands;assert(getEventFloat(*f.entry,-30000000,s)==-25.9990234375f);assert(getEventFloat(*f.entry,-29999999,s)==15.25f);assert(getEventFloat(*f.entry,-29999998,s)==-100.f);}
void missingBeforePrefix(){Fixture f;const auto old=f.entry->locals;for(float v:{std::numeric_limits<float>::infinity(),std::numeric_limits<float>::quiet_NaN(),2097152.f,-2097153.f}){f.xyz[0]=bits(v);assert(!f.call()&&f.entry->locals==old);}f.xyz[0]=bits(-2097152.f);assert(f.call()==2);const auto fresh=f.entry->locals;assert(f.vm->memory.writeWord(f.entry->arguments,1));const auto count=f.coordinates;assert(!f.call()&&f.coordinates==count&&f.entry->locals==fresh);assert(f.vm->memory.writeWord(f.entry->arguments,0));assert(f.vm->memory.writeWord(f.entry->arguments+4,uint32_t(-40000000)));assert(!f.call()&&f.entry->locals==fresh);}
void ownedDomain(){Fixture f;const auto old=f.entry->locals;assert(f.vm->memory.registerRam(FieldMarioOwnerAddress+0x8c,std::vector<uint8_t>(12)));const auto count=f.coordinates;assert(!f.call()&&f.coordinates==count&&f.entry->locals==old);}
void lifetime(){Fixture f;const auto before=f.borrowed;f.host.retire();assert(!f.call()&&f.borrowed==before);Fixture other;auto retained=std::move(other.getter);other.vm.reset();const auto borrowed=other.borrowed;assert(!retained->current()&&other.borrowed==borrowed);}
}
int main(){exactOutput();missingBeforePrefix();ownedDomain();lifetime();std::puts("Finite native Mario XYZ preadmission, actual source Q1024 locals, unavailable-prefix/reentry/lifetime checks pass");return 0;}
