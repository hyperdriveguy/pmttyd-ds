#include "native_harbor_completion.hpp"
#include "harbor_tutorial_packed.hpp"
#include "native_battle_sound.hpp"
#include "battle_menu_file_cache.hpp"
#include "battle_menu_resources.hpp"
#include "battle_packed_unit_resources.hpp"
#include "battle_unit_resources.hpp"
#include "event_lifecycle.hpp"
#include <utility>
namespace ttyd {
namespace {
constexpr uint32_t BattleGlobal=0x8041eb90,PouchGlobal=0x8041eb00;
constexpr uint32_t RegistryOffset=0x20,RegistryCount=64,InformationOffset=0x2738;
constexpr uint32_t UnitKind=8,UnitAlliance=0xc,UnitFlags=0x1c;
constexpr uint32_t CurrentHp=0x10c,CurrentFp=0x112,SnapshotOffset=0x162a0;
constexpr uint32_t Victory=1,AllowZeroHpReturn=0x40;
struct Guard{bool& flag;explicit Guard(bool& f):flag(f){flag=true;}~Guard(){flag=false;}};
}
struct NativeHarborCompletion::State {
 NativeHarborCompletionBinding binding;NativeHarborCompletionServices services;
 std::unique_ptr<BattleMenuFileBinding> menu;BattleMenuResourceCursor menuCursor;
 enum class Phase{Admit,Presentation,Music,Events,Export,Sound,Menus,Done};
 Phase phase=Phase::Admit;bool bound=false,poisoned=false,checking=false;
 uint32_t battle=0,pouch=0,mario=0,party=0,flags=0,pending=0;
 std::shared_ptr<const uint8_t> battleAllocation;ProloguePouch exported;
 bool current(){
  if(checking || poisoned)return false;
  Guard guard(checking);
  if(!binding.vm||!binding.packed||!binding.lifetime||!binding.current||!binding.current())return false;
  if(!bound)return true;
  return services.lifetime&&services.current&&services.current(services.user);
 }
 bool addresses(){uint32_t b=0,p=0;return current()&&binding.vm->memory.readWord(BattleGlobal,b)&&b==battle&&binding.packed->sourceAllocationLifetime(b)==battleAllocation&&binding.vm->memory.readWord(PouchGlobal,p)&&p==pouch;}
 bool known(uint32_t a,uint32_t count){if(uint64_t(a)+count>0x100000000ULL)return false;uint8_t v=0;for(uint32_t i=0;i<count;++i)if(!binding.vm->memory.readByte(a+i,v))return false;return true;}
 bool admit(){
  auto& vm=*binding.vm;
  if(binding.result!=int32_t(Victory)||vm.schedule.managerActive||vm.gp.battleContext!=1||!binding.menuFiles||!binding.menuFiles->ready())return false;
  if(!vm.memory.readWord(BattleGlobal,battle)||!battle||!vm.memory.readWord(PouchGlobal,pouch)||!pouch||!known(pouch,uint32_t(exported.bytes.size())))return false;
  battleAllocation=binding.packed->sourceAllocationLifetime(battle);
  if(!battleAllocation&&battle!=HarborTutorialPacked::Battle)return false;
  for(uint32_t i=0;i<RegistryCount;++i){uint32_t u=0;if(!vm.memory.readWord(battle+RegistryOffset+4*i,u))return false;
   if(u&&(!known(u+UnitKind,4)||!known(u+UnitAlliance,1)||!known(u+UnitFlags,4)))return false;
  }
  const auto actors=nativeBattleActorServices(vm);
  mario=battleMarioUnit(battle,actors);party=battlePartyUnit(battle,actors);
  if(!mario||!known(mario+CurrentHp,2)||!known(mario+CurrentFp,2))return false;
  uint32_t information=0,setup=0;
  if(!vm.memory.readWord(battle+InformationOffset,information)||!information||!vm.memory.readWord(information+0xc,setup)||!setup||!vm.memory.readWord(setup+4,flags))return false;
  if(party){uint32_t kind=0;if(!vm.memory.readWord(party+UnitKind,kind)||!known(party+CurrentHp,2)||!known(party+0x114,0x2c)||!known(battle+SnapshotOffset+(kind-224)*0x2c,0x2c))return false;}
  uint32_t descriptor=0;if(!vm.memory.readWord(battle+0x163f8,descriptor))return false;
  menu=std::make_unique<BattleMenuFileBinding>(binding.menuFiles,actors);
  return addresses();
 }
 bool exportResources(){
  auto& vm=*binding.vm;if(!addresses())return false;
  // Admission owns this serialized native domain. Preserve the original
  // ending mask40 condition; ordinary victory does not heal resources.
  uint16_t hp=0;if(!vm.memory.readHalf(mario+CurrentHp,hp))return false;
  if((flags&AllowZeroHpReturn)&&int16_t(hp)<=0&&!vm.memory.writeHalf(mario+CurrentHp,1))return false;
  PackedUnitResourceServices s;s.memory=nativeBattleActorServices(vm);s.user=this;
  s.pouch=[](void* p){return static_cast<State*>(p)->pouch;};
  s.partyIndex=[](void*,int32_t kind){return battlePartyIndex(kind);};
  exportPackedUnitResources(battle,mario,s);if(party)exportPackedUnitResources(battle,party,s);
  for(unsigned i=0;i<exported.bytes.size();++i)if(!vm.memory.readByte(pouch+i,exported.bytes[i])){poisoned=true;return false;}
  return true;
 }
};
NativeHarborCompletion::NativeHarborCompletion(NativeHarborCompletionBinding binding):state_(std::make_unique<State>()){state_->binding=std::move(binding);}
NativeHarborCompletion::~NativeHarborCompletion()=default;
bool NativeHarborCompletion::resume(const NativeHarborCompletionServices& s){
 if(invoking_)return false;
 Guard guard(invoking_);auto& x=*state_;
 if(!s.lifetime||!s.current||!s.retirePresentation||!s.retireMusic)return false;
 if(x.bound&&(x.services.lifetime!=s.lifetime||x.services.lifetime.owner_before(s.lifetime)||s.lifetime.owner_before(x.services.lifetime)||x.services.user!=s.user||x.services.current!=s.current||x.services.retirePresentation!=s.retirePresentation||x.services.retireMusic!=s.retireMusic||x.services.publicSound.user!=s.publicSound.user||x.services.publicSound.descriptor!=s.publicSound.descriptor||x.services.publicSound.stop!=s.publicSound.stop))return false;
 if(!x.bound){x.services=s;x.bound=true;}
 if(!x.current())return false;
 using P=State::Phase;
 for(;;){
  if(x.phase!=P::Admit&&!x.addresses())return false;
  switch(x.phase){
  case P::Admit:if(!x.admit())return false;x.phase=P::Presentation;break;
  case P::Presentation:if(!x.services.retirePresentation(x.services.user))return false;x.phase=P::Music;break;
  case P::Music:if(!x.services.retireMusic(x.services.user))return false;x.phase=P::Events;break;
  case P::Events:{auto& vm=*x.binding.vm;if(vm.schedule.managerActive||vm.gp.battleContext!=1)return false;
   auto& context=vm.battle;if(context.capacity<0||(context.capacity&&!context.entries))return false;
   for(int32_t i=0;i<context.capacity;++i)if((context.entries[i].flags&1)&&context.entries[i].unresolvedLinkMask)return false;
   for(int32_t i=0;i<context.capacity;++i){if(!x.addresses())return false;if(context.entries[i].flags&1)deleteEvent(context.entries[i],vm.creationServices());}
   x.phase=P::Export;break;}
  case P::Export:if(!x.exportResources())return false;x.phase=P::Sound;break;
  case P::Sound:x.pending=0x800d95f8;if(x.binding.packed->sourceSound&&!x.binding.packed->sourceSound->retireOwned(x.services.publicSound))return false;x.phase=P::Menus;break;
  case P::Menus:x.pending=0x80119d74;if(!x.binding.packed->retireNativeClassUi())return false;
   if(!endPackedBattleMenuResources(x.menu->services(),x.menuCursor))return false;
   x.phase=P::Done;x.pending=0;break;
  case P::Done:return true;
  }
 }
}
std::optional<NativeHarborCompletionReceipt>NativeHarborCompletion::receipt()const{
 if(invoking_)return {};
 Guard guard(invoking_);auto& x=*state_;
 if(x.phase!=State::Phase::Done||!x.addresses())return {};
 return NativeHarborCompletionReceipt{x.binding.result,x.exported,x.binding.lifetime};
}
uint32_t NativeHarborCompletion::pending()const{return state_->pending;}
}
