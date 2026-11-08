#include "event_native_runtime.hpp"
#include "event_heap_allocation.hpp"
#include <cassert>
#include <vector>
#include <iostream>
using namespace ttyd;
struct Fixture {
 EventNativeRuntime vm{0x81000000,{}};std::vector<uint32_t>flushes;unsigned calls=0;bool testMutation=true;
 Fixture(){
  assert(vm.memory.registerRam(0x8041e844,std::vector<uint8_t>(4)));
  assert(vm.memory.registerRam(0x8041f070,std::vector<uint8_t>(4)));
  assert(vm.memory.registerRam(0x1000,std::vector<uint8_t>(12)));
  assert(vm.memory.registerRam(0x2000,std::vector<uint8_t>(0x20000,0xa7)));
  auto put=[&](uint32_t a,uint32_t v){assert(vm.memory.writeWord(a,v));};
  put(0x8041e844,0);put(0x8041f070,0x1000);put(0x1000,0x20000);put(0x1004,0x2000);put(0x1008,0);put(0x2000,0);put(0x2004,0);put(0x2008,0x20000);
  vm.platform.user=this;
  vm.platform.flushBlock=[](void*p,uint32_t a){auto&f=*static_cast<Fixture*>(p);f.flushes.push_back(a);uint32_t v=0;assert(f.vm.memory.readWord(a,v));
   if(f.testMutation){assert(f.vm.memory.registerRam(0xa0000000,std::vector<uint8_t>(32)));assert(f.vm.memory.unregisterRam(0xa0000000));}};
  vm.platform.systemCall=[](void*p){++static_cast<Fixture*>(p)->calls;};
 }
 std::vector<uint8_t>snapshot(){std::vector<uint8_t>v(0x20000);for(unsigned i=0;i<v.size();++i)assert(vm.memory.readByte(0x2000+i,v[i]));return v;}
};
int main(){
 for(uint32_t n:{0u,1u,31u,32u,33u,64u,65537u,0x10000000u}){
  Fixture source,native;
  const auto expected=eventHeapAllocate(0,n,source.vm.memoryBinding.services());
  const auto actual=native.vm.memoryBinding.allocateHeap(0,n);
  assert(actual==expected&&source.snapshot()==native.snapshot()&&source.flushes==native.flushes&&source.calls==native.calls);
 }
 Fixture source,native;source.testMutation=native.testMutation=false;
 assert(native.vm.memory.withRawRamAccess([&](auto&){assert(native.vm.memoryBinding.allocateHeap(0,33)==eventHeapAllocate(0,33,source.vm.memoryBinding.services()));return true;}));
 assert(source.snapshot()==native.snapshot()&&source.flushes==native.flushes&&source.calls==native.calls);
 std::cout<<"Native heap borrow exact bytes/flush order/reentrant platform/nested borrow PASS\n";
}
