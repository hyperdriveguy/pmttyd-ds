#include "animation_startup_process.hpp"
#include <cassert>
#include <cstring>
using namespace ttyd;
namespace {
uint32_t bits(float f){uint32_t v=0;std::memcpy(&v,&f,4);return v;}
uint32_t word(EventAddressSpace& m,uint32_t a){uint32_t v=0;assert(m.readWord(a,v));return v;}
void kernels(){
 AnimationTangentTable table;for(unsigned i=0;i<178;++i)table.word(i,bits(nativeAnimationTangents()[i]));
 ActorSparseUpdate update{0,16,0,0,0};const float baseline=actorNodeDelta(update,.25f,2,&table);
 assert(baseline==actorNodeDelta(update,.25f,2));table.word(89,bits(4));
 assert(actorNodeDelta(update,.25f,2,&table)!=baseline);
 ActorAnimationPose p;p.positions={{{0,0,0}}};p.normals={{{0,1,0}}};p.nodes.resize(24);p.nodes[3]=p.nodes[4]=p.nodes[5]=1;p.groups={{-1,-1,0,0,1}};
 p.keys.resize(2);p.keys[0].time=0;p.keys[1].time=2;p.keys[1].nodes={update};p.end=2;
 ActorAnimationCache c;allocateActorAnimationCache(p,c);ActorAnimationSample out;
 assert(sampleActorAnimationCache(p,c,out,0,1,.25f,-1,0,{},&table));const auto custom=out.nodes[0];
 table.word(89,bits(0));assert(sampleActorAnimationCache(p,c,out,0,1,.25f,0,.25f,{},&table));assert(custom!=out.nodes[0]);
 ActorAnimationSample supplied,legacy;table.word(89,bits(4));
 assert(p.evaluateSelected(.5f,supplied,&table) && p.evaluateSelected(.5f,legacy) && supplied.nodes!=legacy.nodes);
 // Outgoing no-cubic gate bypasses even unsupported incoming table index.
 update.b=-128;update.c=89;assert(actorNodeDelta(update,.25f,2,&table)==0);
}
}
namespace {
void transactionCases(const std::shared_ptr<BattlePosePool>& original,EventAddressSpace& source){
 auto pool=std::make_shared<BattlePosePool>(*original);EventNativeRuntime vm(0x81000000,{});auto& memory=vm.memory;
 auto copy=[&](uint32_t address,uint32_t size){std::vector<uint8_t> bytes(size);for(unsigned i=0;i<size;++i)assert(source.readByte(address+i,bytes[i]));assert(memory.registerRam(address,std::move(bytes)));};
 // Exact copies of the already-established owner's bytes, not a fabricated
 // cold heap/header/file history. This fixture tests alias transactions only.
 const auto work=word(source,0x80415dc0);copy(0x80415dc0,4);copy(work,0x110);
 copy(word(source,work),64*16);copy(word(source,work+8),64*12);copy(word(source,work+0x10),256*0x170);copy(AnimationTangentMemory::Address,178*4);
 auto arrays=AnimationPoolMemory::bind(pool,memory);assert(arrays);
 const auto elapsed=pool->pose(0)->word(0x20);assert(!pool->evaluate(0,{0,50}) && pool->pose(0)->word(0x20)==elapsed);
 assert(!pool->enter(pool->pose(0)->resource,2,{}));
 constexpr auto Address=AnimationTangentMemory::Address;const auto* table=pool->animationTangents();assert(table);
 EventNativeRuntime other(0x81100000,{});assert(other.memory.registerRam(Address,std::vector<uint8_t>(table->state.begin(),table->state.end())));
 assert(!AnimationTangentMemory::bind(pool,other.memory)); // Same bytes in a different VM are not this source owner.
 const auto first=table->word(0);assert(memory.writeWord(Address,first^1) && !AnimationTangentMemory::bind(pool,memory) && word(memory,Address)==(first^1) && table->word(0)==first);
 assert(memory.writeWord(Address,first));
 // A readable external span cannot be extracted as owned RAM. Failure leaves
 // its mapping/data and the retained table untouched.
 std::vector<uint8_t> borrowed;assert(memory.unregisterRam(Address,&borrowed) && memory.registerExternalRam(Address,borrowed.data(),uint32_t(borrowed.size())));
 assert(!AnimationTangentMemory::bind(pool,memory) && word(memory,Address)==first && table->word(0)==first);
 assert(memory.unregisterExternalRam(Address) && memory.registerRam(Address,std::move(borrowed)));
 auto alias=AnimationTangentMemory::bind(pool,memory);assert(alias && alias->current() && pool->animationTangents()==table);
 assert(memory.writeWord(Address,first^1) && table->word(0)==(first^1));
 assert(memory.writeWord(0x80415dc0,0) && !alias->current());assert(memory.writeWord(0x80415dc0,work) && alias->current());
 assert(alias->detach() && !alias->current() && word(memory,Address)==(first^1) && table->word(0)==(first^1));
 assert(!pool->evaluate(0,{0,50}) && pool->pose(0)->word(0x20)==elapsed);
 assert(memory.writeWord(Address,first) && !AnimationTangentMemory::bind(pool,memory) && table->word(0)==(first^1));
 assert(memory.writeWord(Address,first^1));alias=AnimationTangentMemory::bind(pool,memory);assert(alias && alias->current() && table->word(0)==(first^1));
 assert(alias->detach() && arrays->detach());
}
}
void runAnimationTangentMemoryFixture(const char* root){
 kernels();auto pool=std::make_shared<BattlePosePool>();auto files=NativeFileCache::create();assert(stagePrologueAnimationFiles(*files,root));
 EventNativePlatform platform;platform.flushBlock=[](void*,uint32_t){};platform.systemCall=[](void*){};
 auto process=AnimationStartupProcess::cold(files,pool,platform,(std::string(root)+"/animation-boot/memory.pmevt").c_str(),0x81000000,0x82000000,12*1024*1024);assert(process && !pool->animationTangents() && process->advance() && process->ready());
 auto& m=process->vm().memory;const auto* table=pool->animationTangents();assert(table);
 for(unsigned i=0;i<178;++i)assert(table->word(i)==word(m,AnimationTangentMemory::Address+4*i));
 assert(!AnimationTangentMemory::bind(pool,m)); // No competing owner.
 const auto stem=std::string(root)+"/field-player-c_pkr";auto resource=loadBattlePoseResource((stem+".pose-directory.bin").c_str(),(stem+".pmanim").c_str());assert(resource);
 std::vector<uint8_t> name(resource->modelName.begin(),resource->modelName.end());name.push_back(0);assert(m.registerRam(0x81800000,std::move(name)) && m.registerRam(0x800000f8,{0x09,0xa7,0xec,0x80}));
 assert(process->enter(0x81800000,2,resource)==0 && pool->selectName(0,"PKR_W_1",true,{0,0}));
 bool observed=false;
 for(unsigned tick=1;tick<1000 && !observed;++tick){
  assert(pool->evaluate(0,{0,tick}));const auto* p=pool->pose(0);const auto* animation=p->resource->bank->pose(p->resource->names[p->word(0x14)].c_str());assert(animation);
  const auto next=p->word(0x2c);if(p->word(0x28)>=next || next>=animation->keys.size() || p->scalar(0x30)==0)continue;
  for(const auto& r:animation->keys[next].nodes){if(r.b<-89 || r.b>88 || r.c<=-89 || r.c>=89)continue;
   const uint32_t index=unsigned(int(r.b)+89),address=AnimationTangentMemory::Address+4*index,original=word(m,address);const auto before=p->shape.nodes;
   assert(m.writeWord(address,bits(1234)) && table->word(index)==bits(1234));
   assert(pool->evaluate(0,{0,tick}));observed=pool->pose(0)->shape.nodes!=before;
   assert(m.writeWord(address,original) && pool->evaluate(0,{0,tick}) && pool->pose(0)->shape.nodes==before);
   if(observed)break;
  }
 }
 assert(observed); // Actual Goombella cubic evaluation consumes current source bytes.
 transactionCases(pool,m);
 auto isolated=std::make_unique<BattlePosePool>(*pool);assert(isolated->animationTangents() && isolated->animationTangents()!=table);
 const auto old=table->word(89);assert(m.writeWord(AnimationTangentMemory::Address+89*4,bits(7)) && table->word(89)==bits(7) && isolated->animationTangents()->word(89)==old);
 // Change only table byte lanes, then destroy the VM owner: retained CPU bytes
 // must survive and may not regenerate from the detached legacy static table.
 assert(m.writeByte(AnimationTangentMemory::Address+89*4+3,1));const auto final=table->word(89);
 process.reset();assert(pool->animationTangents()==table && table->word(89)==final && pool->evaluate(0,{0,17}));
}
int main(){runAnimationTangentMemoryFixture("romfs");}
