#include <memory>
#include "event_gp_language_alias.hpp"
#include "event_address_space.hpp"
#include "physical_input.hpp"
#include <cassert>
#include <functional>
#include <iostream>
using namespace ttyd;
struct Vm {
 EventEntryAddresses entries;EventContextAddresses contexts{entries};EventRuntimeGlobals runtime{0,1,1,0};EventSchedule schedule;
 EventRuntimeAddresses globals{runtime,schedule};EventGpAddresses gp;EventAddressSpace memory{entries,contexts,globals,gp};EventGlobals state{0,60,0x1122334455667788};
 uint32_t base;explicit Vm(uint32_t at):base(at){assert(gp.registerGp(base,state));assert(gp.writeWord(0x80415fd0,base));}
 uint32_t word(uint32_t off){uint32_t n=0xdeadbeef;assert(memory.readWord(base+off,n));return n;}
};
struct Process {Vm vm{0x81000000};bool alive=true;};
std::shared_ptr<EventGpLanguageAlias> alias(const std::shared_ptr<Process>&source,std::function<bool()>extra={}){
 std::weak_ptr<Process>w=source;return std::make_shared<EventGpLanguageAlias>(source->vm.gp,source->vm.base,source,[w,extra]{auto p=w.lock();return p&&p->alive&&(!extra||extra());});
}
int main(){
 auto process=std::make_shared<Process>();Vm dest(0x82000000);assert(process->vm.gp.writeWord(process->vm.base+0x16c,1));auto shared=alias(process);assert(dest.gp.bindLanguageAlias(dest.base,shared));assert(!dest.gp.bindLanguageAlias(dest.base,shared));assert(dest.word(0x16c)==1);
 assert(process->vm.memory.writeWord(process->vm.base+0x16c,2)&&dest.word(0x16c)==2);assert(dest.memory.writeWord(dest.base+0x16c,1)&&process->vm.word(0x16c)==1);
 std::array<uint8_t,4>dims{2,0x60,1,0xe0};assert(dest.gp.bindRenderDimensions(dest.base,dims));assert(dest.memory.writeWord(dest.base+0x168,0xaabbccdd));assert(dest.memory.writeWord(dest.base+0x174,0x12345678));
 assert(dest.memory.writeHalf(dest.base+0x16f,0x9988));assert(process->vm.word(0x16c)==0x99&&dims[0]==0x88);assert(dest.memory.writeWord(dest.base+0x16a,0x11223344));assert(dest.word(0x168)==0xaabb1122&&process->vm.word(0x16c)==0x33440099);uint16_t h;assert(dest.memory.readHalf(dest.base+0x16b,h)&&h==0x2233);
 assert(dest.memory.writeByte(dest.base+0x16e,0x66)&&process->vm.word(0x16c)==0x33446699);assert(dest.word(0x174)==0x12345678&&dest.state.timestamp==0x1122334455667788&&dest.state.refreshRate==60&&dest.state.battleContext==0);
 PhysicalInputProducer input;input.ready=9;input.channels[0].buttons=0x1234;assert(dest.gp.bindPhysicalInput(dest.base,input));assert(dest.word(0x1324)==9&&dest.word(0x1328)==0x1234);assert(dest.memory.writeWord(dest.base+0x1328,0x4321)&&input.channels[0].buttons==0x4321);assert(process->vm.word(0x1328)==0);
 assert(dest.gp.currentAddress()==dest.base&&process->vm.gp.currentAddress()==process->vm.base);
 // Every unaligned byte/half/word crossing either alias boundary agrees with
 // a reference BE span and updates only the true source language lanes.
 for(unsigned n:{1u,2u,4u})for(unsigned off=0x169;off<0x173;++off){
  assert(process->vm.memory.writeWord(process->vm.base+0x16c,0x11223344));assert(dest.memory.writeWord(dest.base+0x168,0xaabbccdd));dims={0xee,0xff,0x55,0x66};
  uint32_t expected=0;for(unsigned i=0;i<n;++i){uint8_t b;assert(dest.memory.readByte(dest.base+off+i,b));expected=expected<<8|b;}
  uint32_t got;if(n==1){uint8_t b;assert(dest.memory.readByte(dest.base+off,b));got=b;}else if(n==2){assert(dest.memory.readHalf(dest.base+off,h));got=h;}else assert(dest.memory.readWord(dest.base+off,got));assert(got==expected);
  bool done=n==1?dest.memory.writeByte(dest.base+off,0xa5):n==2?dest.memory.writeHalf(dest.base+off,0xa5b6):dest.memory.writeWord(dest.base+off,0xa5b6c7d8);assert(done);
  const uint32_t value=n==1?0xa5:n==2?0xa5b6:0xa5b6c7d8;for(unsigned i=0;i<n;++i){uint8_t b;assert(dest.memory.readByte(dest.base+off+i,b)&&b==uint8_t(value>>(8*(n-i-1))));}
 }
 // One proof per aliased transaction; no callbacks after successful admission.
 auto atomicSource=std::make_shared<Process>();Vm atomic(0x83000000);unsigned checks=0;auto atomicAlias=alias(atomicSource,[&]{++checks;return true;});assert(atomic.gp.bindLanguageAlias(atomic.base,atomicAlias));checks=0;assert(atomic.memory.writeHalf(atomic.base+0x16b,0xaabb)&&checks==1);checks=0;assert(atomic.memory.readHalf(atomic.base+0x16b,h)&&checks==1);
 // Stale operations leave both source word and adjacent destination unchanged.
 assert(atomic.memory.writeWord(atomic.base+0x168,0x12345678));const auto before=atomicSource->vm.word(0x16c);atomicSource->alive=false;assert(!atomic.memory.writeWord(atomic.base+0x16a,0xdeadbeef));assert(atomic.word(0x168)==0x12345678&&atomicSource->vm.word(0x16c)==before);uint32_t out=0xdeadbeef;assert(!atomic.memory.readWord(atomic.base+0x16c,out)&&out==0xdeadbeef);uint8_t b=0xab;assert(!atomic.memory.readByte(atomic.base+0x16c,b)&&b==0xab);assert(!atomic.memory.registerRam(atomic.base+0x16c,{9,9,9,9}));atomicSource->alive=true;assert(!atomicAlias->current());
 // A preexisting external hole cannot become fallback for stale alias bytes.
 Vm early(0x84000000);Vm later(0x85000000);auto src=std::make_shared<Process>();
 std::array<uint8_t,4>raw{7,7,7,7};assert(later.memory.registerExternalRam(early.base+0x16c,raw.data(),4));EventGlobals overlap;assert(later.gp.registerGp(early.base,overlap));auto holder=std::make_shared<int>(1);bool live=true;auto first=std::make_shared<EventGpLanguageAlias>(src->vm.gp,src->vm.base,src,[&]{return live;});assert(later.gp.bindLanguageAlias(early.base,first));live=false;assert(!later.memory.readWord(early.base+0x16c,out)&&!later.memory.writeHalf(early.base+0x16c,0x9999)&&raw[0]==7);
 // Register identity rejects actual GP End/Init ABA, even at same address.
 auto abaSource=std::make_shared<Process>();Vm aba(0x86000000);auto a=alias(abaSource);assert(aba.gp.bindLanguageAlias(aba.base,a));assert(abaSource->vm.gp.unregisterGp(abaSource->vm.base));assert(abaSource->vm.gp.registerGp(abaSource->vm.base,abaSource->vm.state));assert(!a->current()&&!aba.memory.readWord(aba.base+0x16c,out));
 auto switchedSource=std::make_shared<Process>();Vm switched(0x8b000000);auto switchedAlias=alias(switchedSource);assert(switched.gp.bindLanguageAlias(switched.base,switchedAlias));assert(switchedSource->vm.gp.writeWord(0x80415fd0,0));assert(!switchedAlias->current());assert(switchedSource->vm.gp.writeWord(0x80415fd0,switchedSource->vm.base));assert(!switchedAlias->current());
 // Source may not itself be aliased; self and precreated cycles are rejected.
 auto self=alias(process);assert(!process->vm.gp.bindLanguageAlias(process->vm.base,self));self->retire();auto left=std::make_shared<Process>();auto right=std::make_shared<Vm>(0x87000000);auto fromLeft=alias(left);auto fromRight=std::make_shared<EventGpLanguageAlias>(right->gp,right->base,right,[]{return true;});assert(right->gp.bindLanguageAlias(right->base,fromLeft));assert(!left->vm.gp.bindLanguageAlias(left->vm.base,fromRight));auto chain=std::make_shared<EventGpLanguageAlias>(right->gp,right->base,right,[]{return true;});Vm last(0x88000000);assert(!last.gp.bindLanguageAlias(last.base,chain));
 // Actual process remains strongly owned until views retire; no borrowed source
 // pointer can outlive its proof holder. Explicit current loss still holds.
 std::weak_ptr<Process>weak=process;process.reset();assert(!weak.expired());shared->retire();assert(weak.expired()&&!dest.memory.readWord(dest.base+0x16c,out));
 auto recursiveSource=std::make_shared<Process>();Vm recursiveDest(0x89000000);bool probe=false;std::shared_ptr<EventGpLanguageAlias>recursive;recursive=alias(recursiveSource,[&]{if(probe){uint32_t v;assert(!recursive->read(v)&&!recursive->modify(0xff,1));assert(!recursiveDest.gp.readWord(recursiveDest.base+0x168,v));assert(!recursiveDest.gp.unregisterGp(recursiveDest.base));assert(!recursiveDest.gp.registerGp(0x8a000000,recursiveDest.state));}return true;});assert(recursiveDest.gp.bindLanguageAlias(recursiveDest.base,recursive));probe=true;assert(recursiveDest.memory.writeWord(recursiveDest.base+0x16c,2));
 auto removedSource=std::make_shared<Process>();Vm removedDest(0x8c000000);bool removeNow=false;auto removed=alias(removedSource,[&]{if(removeNow)assert(removedSource->vm.gp.unregisterGp(removedSource->vm.base));return true;});assert(removedDest.gp.bindLanguageAlias(removedDest.base,removed));assert(removedDest.memory.writeWord(removedDest.base+0x168,0x01020304));removeNow=true;assert(!removedDest.memory.writeHalf(removedDest.base+0x16b,0xabcd)&&removedDest.word(0x168)==0x01020304);
 auto retiringSource=std::make_shared<Process>();Vm retiringDest(0x8d000000);bool retireNow=false;std::shared_ptr<EventGpLanguageAlias>retiring;retiring=alias(retiringSource,[&]{if(retireNow)retiring->retire();return true;});assert(retiringDest.gp.bindLanguageAlias(retiringDest.base,retiring));retireNow=true;assert(!retiringDest.memory.writeWord(retiringDest.base+0x16c,2));retiring->retire();
 auto missing=std::make_shared<EventGpLanguageAlias>(retiringSource->vm.gp,retiringSource->vm.base,retiringSource,std::function<bool()>{});assert(!missing->current());
 std::cout<<"Actual GP language alias bidirectional widths/atomic boundaries/lifetime/ABA PASS\n";
}
