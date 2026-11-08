#include "harbor_source_unit_construction.hpp"
#include "battle_event_callbacks.hpp"
#include "battle_packed_display_entry.hpp"
#include "battle_packed_part.hpp"
#include "battle_packed_pose_lookup.hpp"
#include "battle_packed_unit_equipment.hpp"
#include "battle_packed_unit_initialization.hpp"
#include "battle_packed_unit_vectors.hpp"
#include "battle_unit_rotation_events.hpp"
#include "battle_unit_resources.hpp"
#include <cstdlib>
#include <cstring>
namespace ttyd {
namespace {
void required(bool ok){if(!ok)std::abort();}
struct Guard{bool& flag;explicit Guard(bool& f):flag(f){flag=true;}~Guard(){flag=false;}};
}
struct HarborSourceUnitConstruction::State {
 HarborSourceUnitConstructionBinding binding;
 BattleUnitEntryCursor entry;BattleDisplayEntryCursor display;
 BattleMarioJumpBackend math;
 std::shared_ptr<const uint8_t> battleLifetime;
 std::vector<std::pair<uint32_t,std::shared_ptr<const uint8_t>>> allocations;
 struct NameView{uint32_t address,size;};std::vector<NameView> names;
 uint32_t definition=0,pending=0;bool begun=false,done=false;
 mutable bool invalid=false,checking=false;
 bool current()const{
  if(invalid || checking)return false;
  Guard guard(checking);
  if(!binding.current()){invalid=true;return false;}
  if(!binding.pool->current() || binding.pool->allocationLifetime(binding.battle)!=battleLifetime || word(0x8041eb90)!=binding.battle){invalid=true;return false;}
  for(const auto& allocation:allocations)if(binding.pool->allocationLifetime(allocation.first)!=allocation.second){invalid=true;return false;}
  if(binding.animation)for(const auto& name:names)
   if(!binding.vm->memory.sharesRamWith(binding.animation->process().vm().memory,name.address,name.size)){invalid=true;return false;}
  return true;
 }
 BattleActorServices actors()const{return nativeBattleActorServices(*binding.vm);}
 uint32_t word(uint32_t a)const{uint32_t v;required(binding.vm->memory.readWord(a,v));return v;}
 float real(uint32_t a)const{const auto bits=word(a);float v;std::memcpy(&v,&bits,4);return v;}
 void real(uint32_t a,float v){uint32_t bits;std::memcpy(&bits,&v,4);required(binding.vm->memory.writeWord(a,bits));}
 std::optional<std::string> text(uint32_t a)const{
  constexpr unsigned MaxModelName=255;std::string result;
  for(unsigned i=0;i<=MaxModelName;++i){uint8_t b;if(uint64_t(a)+i>UINT32_MAX || !binding.vm->memory.readByte(a+i,b))return {};
   if(!b)return result;
   result.push_back(char(b));}return {};
 }
 bool shareName(uint32_t name,const std::string& value){
  auto& target=binding.animation->process().vm().memory;
  bool equal=true;for(unsigned i=0;i<=value.size();++i){uint8_t b;
   if(!target.readByte(name+i,b) || b!=(i<value.size()?uint8_t(value[i]):0)){equal=false;break;}}
  if(equal)return true;
  if(!target.unmappedRange(name,value.size()+1) || !binding.vm->memory.shareRamTo(target,name,uint32_t(value.size()+1)))return false;
  names.push_back({name,uint32_t(value.size()+1)});return true;
 }
 BattleUnitEntryContinuationServices services(){
  BattleUnitEntryContinuationServices s;s.actors=actors();s.user=this;s.lifetime=binding.lifetime;
  s.current=[](void* p){return static_cast<State*>(p)->current();};
  s.call=[](void* p,const BattleUnitEntryRequest& q){return static_cast<State*>(p)->call(q);};return s;
 }
 std::optional<BattleUnitEntryOutput> call(const BattleUnitEntryRequest& q){
  pending=q.function;if(!current())return {};BattleUnitEntryOutput out;
  const auto a=q.integers[0],b=q.integers[1],c=q.integers[2];const auto memory=actors();
  if(initializePackedBattleUnitHelper(q.function,a,memory))return out;
  switch(q.function){
  case 0x800f88cc:out.value=battleRegistryUnit(a,int32_t(b),memory);return out;
  case 0x800f88b4:setBattleRegistryUnit(a,int32_t(b),c,memory);return out;
  case 0x800f8934:{const auto address=binding.pool->allocate(binding.pool->root(),a);if(!address)return {};
   out.value=*address;if(*address)allocations.emplace_back(*address,binding.pool->allocationLifetime(*address));return out;}
  case 0x800050b4:out.value=eventFillMemory(a,int32_t(b),c,memory.memory);return out;
  case 0x800d5eb0:out.value=word(0x8041eb00);return out;
  case 0x80128c98:out.value=packedBodyPartId(a,memory.memory);return out;
  case 0x80128cd8:out.value=packedUnitPart(a,int32_t(b),memory.memory);return out;
  case 0x8012772c:out.value=packedBattlePoseName(a,int32_t(b),memory.memory);return out;
  case 0x80103cd0:out.value=searchPackedBattlePoseTable(a,int32_t(b),memory.memory);return out;
  case 0x8026d5fc:
   // Original strcpy has no local bound. The source pose table/name/buffer
   // domain is admitted by the source resource owner, as for the parent.
   for(uint32_t i=0;;++i){uint8_t value;required(binding.vm->memory.readByte(b+i,value));required(binding.vm->memory.writeByte(a+i,value));if(!value)break;}
   out.value=a;return out;
  case 0x8004176c:{
   if(!binding.animation || !binding.resources)return {};
   const auto model=text(a);if(!model)return {};
   const auto resource=binding.resources->find(*model);if(!resource || !shareName(a,*model))return {};
   BattlePoseClock clock;if(!binding.animation->clock(clock))return {};
   const auto handle=binding.animation->pool()->enterSourceOriginal(resource,int32_t(b),clock,a);
   if(!handle)return {};
   out.value=uint32_t(*handle);return out;}
  case 0x801011fc:if(!resumePackedBattleDisplayEntry(a,services(),display))return {};return out;
  case 0x80127134:{PackedUnitResourceServices r;r.memory=memory;r.user=this;
   r.pouch=[](void* p){return static_cast<State*>(p)->word(0x8041eb00);};
   r.partyIndex=[](void*,int32_t kind){return battlePartyIndex(kind);};
   importPackedUnitResources(binding.battle,a,r);return out;}
  case 0x800374ac:{auto creation=binding.vm->creationServices();out.event=&createRootEvent(binding.vm->schedule,a,int32_t(b),c,creation);return out;}
  case 0x801288b4:{BattleUnitRotationEventServices rotation;rotation.units=nativeBattleUnitEventServices(*binding.vm);
   rotation.angle=nativeBattleMarioJumpServices(math);setPackedBattleUnitRotate(a,q.vector,rotation);return out;}
  default:break;
  }
  PackedUnitVectorServices vectors;vectors.memory=memory.memory;vectors.user=this;
  vectors.reviseAngle=[](void* p,float value){return revisePackedJumpAngle(value,nativeBattleMarioJumpServices(static_cast<State*>(p)->math));};
  if(setPackedUnitVector(q.function,a,q.vector,vectors))return out;
  return {};
 }
};
HarborSourceUnitConstruction::HarborSourceUnitConstruction()=default;
HarborSourceUnitConstruction::~HarborSourceUnitConstruction(){
 if(!state_ || !state_->binding.animation)return;
 // Caller guarantees source VM/map outlive this view owner. A replacement
 // target mapping is preserved; only still-shared aliases belong to us.
 auto& s=*state_;auto& target=s.binding.animation->process().vm().memory;
 for(const auto& name:s.names)if(s.binding.vm->memory.sharesRamWith(target,name.address,name.size))target.unregisterExternalRam(name.address);
}
std::unique_ptr<HarborSourceUnitConstruction> HarborSourceUnitConstruction::create(HarborSourceUnitConstructionBinding b){
 if(!b.vm || !b.pool || !b.battle || !b.lifetime || !b.current)return {};
 auto result=std::unique_ptr<HarborSourceUnitConstruction>(new HarborSourceUnitConstruction);result->state_=std::make_unique<State>();
 auto& s=*result->state_;s.binding=std::move(b);s.battleLifetime=s.binding.pool->allocationLifetime(s.binding.battle);s.math.runtime=s.binding.vm;
 if(!s.battleLifetime || !result->current())return {};
 return result;
}
std::optional<uint32_t> HarborSourceUnitConstruction::resume(uint32_t definition){
 if(invoking_ || !current())return {};
 auto& s=*state_;if(s.begun && s.definition!=definition)return {};
 Guard guard(invoking_);s.begun=true;s.definition=definition;
 const auto value=resumePackedBattleUnitEntry(s.binding.battle,definition,s.services(),s.entry);
 if(value){s.done=true;s.pending=0;}return value;
}
bool HarborSourceUnitConstruction::next(){
 if(invoking_ || !current() || !state_->done)return false;
 auto& s=*state_;required(s.entry.reset() && s.display.reset());s.begun=s.done=false;s.definition=0;return true;
}
uint32_t HarborSourceUnitConstruction::pending()const{return state_?state_->pending:0;}
bool HarborSourceUnitConstruction::current()const{return state_ && state_->current();}
}
