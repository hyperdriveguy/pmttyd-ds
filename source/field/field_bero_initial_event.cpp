#include "field_bero_initial_event.hpp"
#include "field_bero_info.hpp"
#include "event_native_runtime.hpp"
#include "event_memory_bootstrap.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
uint32_t bits(float v){uint32_t out;std::memcpy(&out,&v,4);return out;}
int32_t signedWord(uint32_t v){return v<=INT32_MAX?int32_t(v):int32_t(int64_t(v)-0x100000000LL);}
}
struct FieldBeroInitialRuntime {
 FieldBeroInitialEvent* owner;
 FieldBeroInfo info; // Entry snapshot owns the catalog and incoming-name storage.
 FieldBeroPositionServices geometry;
 FieldPositionWorld world;
 EventNativeRuntime vm;
 std::optional<std::array<int32_t,16>> completedBank;
 bool globalsPublished=false;
 // Native VM GP base 81000000; original bus clock 243MHz is converted by
 // eventClockRate for millisecond timestamps, independently of render frames.
 FieldBeroInitialRuntime(FieldBeroInitialEvent* p,const FieldBeroInfo& i):owner(p),info(i),vm(0x81000000,platform(this)){}
 static EventNativePlatform platform(FieldBeroInitialRuntime* r) {
  EventNativePlatform s;s.user=r;s.clock=[](void*)->uint32_t{return 243000000;};
  s.truncate=[](void*,float v){assert(std::isfinite(v) && double(v)>=INT32_MIN && double(v)<2147483648.);return int32_t(v);};
  s.divideWord=[](void*,int32_t a,int32_t b){assert(b);return a/b;};
  s.flushBlock=[](void*,uint32_t){};s.systemCall=[](void*){};
  s.format=[](void* p,const EventDiagnosticCall&){static_cast<FieldBeroInitialRuntime*>(p)->owner->failed=true;};
  s.call=[](void* p,EventEntry& e,uint32_t fn,int32_t){return static_cast<FieldBeroInitialRuntime*>(p)->call(e,fn);};return s;
 }
 uint32_t word(uint32_t a){uint32_t v=0;if(!vm.memory.readWord(a,v))owner->failed=true;return v;}
 int32_t call(EventEntry& e,uint32_t fn) {
  owner->pendingFunction=fn;const auto op=vm.interpreterServices().operands;
  // US callback identities: get_into_info, get_info_length, normal-position.
  // Return0 holds USER_FUNC; return2 consumes it. Unknown callbacks also hold.
  if(fn==0x8007a214)return info.intoInfo(e)?2:0;
  if(fn==0x8007a76c) {
   const auto args=e.arguments;const auto index=getEventValue(e,signedWord(word(args)),op);
   const auto length=info.length(index);if(!length || !fieldBeroStartPublication({*length,0,0}))return 0;
   setEventFloat(e,signedWord(word(args+4)),*length,op);return owner->failed?0:2;
  }
  if(fn==0x80079b74) {
   if(!world.mario || !world.write || !world.half)return 0;
   const auto args=e.arguments;const auto capturedMario=world.mario(world.user);if(!capturedMario)return 0;
   const auto mode=getEventValue(e,signedWord(word(args)),op);
   const auto distance=getEventFloat(e,signedWord(word(args+4)),op);
   std::array<int32_t,3> xyz;for(unsigned n=0;n<3;++n)xyz[n]=signedWord(word(uint32_t(e.locals[6])+4*n));
   std::string name;for(unsigned n=0;n<64;++n){uint8_t byte=0;if(!info.readByte(uint32_t(e.locals[3])+n,byte))return 0;if(!byte)break;name.push_back(char(byte));}
   if(owner->failed || name.size()==64)return 0;
   const auto result=fieldBeroNormalPosition(xyz,name,e.locals[5],mode,distance,geometry);
   if(!result || !fieldBeroStartPublication(result->start))return 0;
   // Original mutable globals store start XYZ then finish XYZ in descending
   // addresses f8/f4/f0/ec/e8/e4. Preserve order, then run recovery stores.
   const uint32_t addresses[]={0x8041e9f8,0x8041e9f4,0x8041e9f0,0x8041e9ec,0x8041e9e8,0x8041e9e4};
   const float values[]={result->start.x,result->start.y,result->start.z,result->finish.x,result->finish.y,result->finish.z};
   for(unsigned n=0;n<6;++n)if(!vm.memory.writeWord(addresses[n],bits(values[n]))){owner->failed=true;return 0;}
   globalsPublished=true;
   // Actual recovery leaf gets Mario again, before the captured motion read.
   const auto recoveryMario=world.mario(world.user);assert(recoveryMario);
   world.write(world.user,recoveryMario+0x2e4,bits(result->finish.x));
   world.write(world.user,recoveryMario+0x2e8,bits(result->recovery.y));
   world.write(world.user,recoveryMario+0x2ec,bits(result->finish.z));
   // Read motion25 on the earlier captured owner; only then reacquire Mario
   // for +2e0=1. The recovery lookup above does not replace this captured read.
   if(world.half(world.user,capturedMario+0x2e)==25) {
    const auto reloadMario=world.mario(world.user);assert(reloadMario);world.write(world.user,reloadMario+0x2e0,1);
   }
   owner->position=result;return 2;
  }
  FieldPositionOperands operands;operands.user=this;
  operands.word=[](void* p,uint32_t a){return static_cast<FieldBeroInitialRuntime*>(p)->word(a);};
  operands.scalar=[](void* p,EventEntry& entry,int32_t raw){auto& r=*static_cast<FieldBeroInitialRuntime*>(p);return getEventFloat(entry,raw,r.vm.interpreterServices().operands);};
  // US set-pos, party-set-pos and camera-road-reset callbacks respectively.
  // World offsets/ordered effects are delegated to the shared leaf helpers.
  if(fn==0x800e7180){if(!world.mario || !world.write)return 0;return fieldMarioSetPositionEvent(e,operands,world);}
  if(fn==0x800eb558){if(!world.party || !world.currentParty || !world.write)return 0;return fieldPartySetPositionEvent(e,operands,world);}
  if(fn==0x800a785c) {
   if(!world.mario || !world.camera || !world.read || !world.write || !world.roadReset || !world.shiftReset)return 0;
   const auto status=fieldCameraRoadResetEvent(world);
   // Pinned original script has only RETURN/END after this final callback.
   // Preserve its actual bank before the source deletion clears entry storage.
   completedBank=e.locals;return status;
  }
  return 0; // Unadmitted original branch/callback stays pending.
 }
 // Private 512KiB heap [82000000,82080000), plus original allocator/context
 // cells and 24-byte Bero globals. 81000004=60 is VM scheduling configuration.
 // Root8030f060 priority1 runs unchanged initial_pos, not an invented script.
 bool initialize(uint64_t milliseconds,const std::array<int32_t,16>& bank) {
  if(!vm.memory.registerRam(0x82000000,std::vector<uint8_t>(0x80000)) ||
     !vm.memory.registerRam(0x8041f070,std::vector<uint8_t>(16)) || !vm.memory.registerRam(0x8041e428,std::vector<uint8_t>(4)) ||
     !vm.memory.registerRam(0x8041e844,std::vector<uint8_t>(4)) || !info.importScripts(vm.memory) ||
     !vm.memory.registerRam(0x8041e9e4,std::vector<uint8_t>(24)))return false;
  const auto memory=vm.memoryBinding.services();const auto start=eventSdkInitializeAllocator(0x82000000,0x82080000,1,memory);
  if(eventSdkCreateHeap(start,0x82080000,memory)!=0 || !vm.memory.writeWord(0x8041e844,0) || !vm.memory.writeWord(0x81000004,60))return false;
  vm.gp.timestamp=milliseconds*eventClockRate(243000000);vm.initializeContexts();
  auto& entry=createRootEvent(vm.schedule,0x8030f060,1,0,vm.creationServices());entry.locals=bank;owner->eventId=entry.id;return true;
 }
};
// Refuse an active child or unknown/unregistered/non-normal entry. A completed
// or failed child may be replaced; the facade itself cannot move while retained.
bool FieldBeroInitialEvent::begin(const FieldBeroInfo& info,const std::array<int32_t,16>& bank,uint64_t milliseconds) {
 if((started && !finished && !failed) || !info.known() || !info.registered() || (uint32_t(info.publication()->at(3))&0xfff)!=0)return false;
 runtime_=std::make_shared<FieldBeroInitialRuntime>(this,info);locals.reset();position.reset();finished=failed=false;pendingFunction=0;
 if(!runtime_->initialize(milliseconds,bank)){runtime_.reset();return false;}started=true;return true;
}
void FieldBeroInitialEvent::tick(uint64_t milliseconds,const FieldBeroPositionServices& geometry,const FieldPositionWorld& world) {
 if(!runtime_ || !started || finished || failed)return;
 runtime_->owner=this;runtime_->geometry=geometry;runtime_->world=world;pendingFunction=0;
 runtime_->vm.gp.timestamp=milliseconds*eventClockRate(243000000);runtime_->vm.runManager();
 if(!runtime_->vm.runtime.allocationCount && runtime_->completedBank && !failed){locals=runtime_->completedBank;finished=true;pendingFunction=0;}
}
// Expose aligned words only after geometry published the six globals; RAM
// mapping alone does not make zero-initialized bytes an original result.
bool FieldBeroInitialEvent::readGlobal(uint32_t address,uint32_t& value) const {
 return runtime_ && runtime_->globalsPublished && address>=0x8041e9e4 && address<0x8041e9fc && !(address&3) && runtime_->vm.memory.readWord(address,value);
}
}
