#include "harbor_tutorial_packed.hpp"
#include "harbor_source_command_ui.hpp"
#include <utility>
#include "battle_packed_sampling.hpp"
namespace ttyd { namespace {
constexpr uint32_t CurrentBattleCell=0x8041eb90;
constexpr uint32_t NativeClassRows=7,NativeWeaponRows=64,NativeMenuLabelBytes=4096;
bool label(EventAddressSpace& memory,uint32_t at,std::string& result){
 if(!at)return true;
 for(uint32_t n=0;n<NativeMenuLabelBytes;++n){uint8_t b=0;if(!memory.readByte(at+n,b))return false;if(!b)return true;result.push_back(char(b));}
 return false;
}
bool classProjection(uint32_t b,EventNativeRuntime& vm,HarborTutorial& scene){
 const auto menu=b+0x171c;uint32_t cursor=0;
 if(!findPackedCommandCursor(menu,14,cursor,nativeBattlePackedSelectionServices(vm)))return false;
 uint32_t count=0,selection=0;if(!vm.memory.readWord(cursor+8,count)||!vm.memory.readWord(cursor,selection)||count>NativeClassRows)return false;
 std::vector<HarborTutorialClassRow> rows;
 for(uint32_t n=0;n<count;++n){const auto at=menu+20*n;HarborTutorialClassRow row;uint32_t enabled=0;
  if(!vm.memory.readWord(at+8,row.type)||!vm.memory.readWord(at+0xc,enabled)||!vm.memory.readWord(at+0x10,row.nameAddress)||!vm.memory.readHalf(at+0x14,row.icon)||!vm.memory.readWord(at+0x18,row.reason)||!label(vm.memory,row.nameAddress,row.label))return false;
  row.enabled=enabled!=0;rows.push_back(std::move(row));
 }
 scene.classRows=std::move(rows);scene.classRowIndex=int32_t(selection);return true;
}
bool weaponProjection(uint32_t b,EventNativeRuntime& vm,HarborTutorial& scene){
 const auto menu=b+0x171c;uint32_t type=0,cursor=0,count=0,selection=0;
 if(!vm.memory.readWord(b+0x1c5c,type)||!findPackedCommandCursor(menu,type,cursor,nativeBattlePackedSelectionServices(vm))||!vm.memory.readWord(cursor+8,count)||!vm.memory.readWord(cursor,selection)||count>NativeWeaponRows)return false;
 std::vector<HarborTutorialWeaponRow> rows;
 for(uint32_t n=0;n<count;++n){const auto at=menu+0x80+28*n;HarborTutorialWeaponRow row;uint32_t enabled=0,ordinal=0,item=0,reason=0;
  if(!vm.memory.readWord(at,row.weapon)||!vm.memory.readWord(at+4,enabled)||!vm.memory.readWord(at+8,row.nameAddress)||!vm.memory.readHalf(at+12,row.icon)||!vm.memory.readWord(at+16,ordinal)||!vm.memory.readWord(at+20,item)||!vm.memory.readWord(at+24,reason)||!label(vm.memory,row.nameAddress,row.label))return false;
  row.enabled=enabled!=0;row.ordinal=int32_t(ordinal);row.item=int32_t(item);row.reason=int32_t(reason);rows.push_back(std::move(row));
 }
 scene.weaponRows=std::move(rows);scene.weaponMenuType=type;scene.weaponRowIndex=int32_t(selection);return true;
}
}}
#include "harbor_source_map_pool.hpp"
namespace ttyd {
bool HarborTutorialPacked::bindSourceCommandUi(HarborSourceCommandUiBinding binding){
 if(sourceCommandUi || binding.runtime.expired() || !binding.current || !binding.child || !binding.word)return false;
 // Storage is this actual battle VM; external hooks supply original children,
 // never an alternative registry, fixed cursor or synthetic menu state.
 const auto actualBattle=battleAddress();if(!actualBattle)return false;
 const auto originalCurrent=std::move(binding.current);
 binding.current=[packed=this,weak=binding.runtime,originalCurrent,actualBattle](){
  const auto runtime=weak.lock();if(!runtime)return false;
  uint32_t currentBattle=0;
  return originalCurrent() && packed->readWord(CurrentBattleCell,currentBattle) && currentBattle==actualBattle;
 };
 binding.actors=nativeBattleActorServices(vm);binding.battle=actualBattle;
 binding.privateSamplingBuffer=MenuRam+0x40; // Actual native VM scratch, disjoint from live B+428.
 if(!binding.evaluationChild){binding.evaluationUser=this;binding.evaluationChild=[](void* u,const BattleWeaponEvaluationRequest& q){return static_cast<HarborTutorialPacked*>(u)->sourceCommandEvaluationChild(q);};}
 if(!binding.byte){binding.byteUser=this;binding.byte=[](void* u,uint32_t a)->std::optional<uint8_t>{uint8_t byte=0;if(!static_cast<HarborTutorialPacked*>(u)->vm.memory.readByte(a,byte))return {};return byte;};}
 sourceCommandUi=std::make_shared<HarborSourceCommandUi>(std::move(binding));return true;
}
std::optional<int32_t> HarborTutorialPacked::resumeSourceCommandInput(HarborTutorial& scene){
 if(!sourceCommandUi)return {};
 const auto result=sourceCommandUi->commandInput();
 scene.commandInputPending=sourceCommandUi->pendingFunction();
 if(sourceCommandUi->stale())return {};
 // Presentation projections only. Source Setup/End own these bits; drawing
 // alone owns work+14 readiness. No native presented/confirmed flag is stored.
 const auto b=sourceCommandUi->battleAddress();
 const auto flags=word(b+0x1720);
 scene.classMenuOpen=(flags&1)!=0;
 scene.weaponMenuOpen=(flags&0x10)!=0;
 scene.weaponTargetMenuOpen=(flags&0x10000)!=0;
 // Diagnostic projection failure does not replay a completed source Input.
 // Real drawing reads the actual source rows independently.
 if(scene.classMenuOpen)classProjection(b,vm,scene);
 if(scene.weaponMenuOpen)weaponProjection(b,vm,scene);
 if(result&&*result==1)commandFinalized=true;
 return result;
}
bool HarborTutorialPacked::sourceCommandInputPending()const{return sourceCommandUi&&sourceCommandUi->pending();}
std::optional<uint32_t> HarborTutorialPacked::sourceCommandEvaluationChild(const BattleWeaponEvaluationRequest& q){
 // Complete synchronous source sampler/update are admitted only with all
 // their reached numeric services. Never return unavailable after they write.
 const auto& p=vm.platform;
 if(!p.truncate||!p.fmaFloat||!p.addFloat||!p.multiplyFloat||!p.signedWordFloat||!p.divideFloat||!p.subtractFloat||!p.negativeMultiplySubtractFloat)return {};
 const auto services=nativeBattleSamplingServices(vm);const auto& a=q.arguments;
 if(q.function==0x800ff5c0&&q.count==7){
  // Monosiri is a genuine unbound provider. This conservative preadmission
  // rejects its attribute domain before the sampler's first source store.
  if(a[5]&1)return {};
  return uint32_t(samplePackedBattleCandidates({nativeBattleActorServices(vm),a[0]},a[1],int32_t(a[2]),int32_t(a[3]),a[4],a[5],int32_t(a[6]),services));
 }
 if(q.function==0x800ff5a0&&q.count==1){updatePackedBattleCandidates({nativeBattleActorServices(vm),a[0]},services);return 0u;}
 return {};
}
std::optional<uint32_t> HarborTutorialPacked::sourceCommandMapChild(const HarborSourceCommandUiRequest& q){
 if(q.count!=2 || !sourceMapPool_ || !sourceMapPool_->current())return {};
 const auto heap=q.arguments[0],value=q.arguments[1];
 if(q.function==0x8002ff58){
  const auto address=sourceMapPool_->allocate(heap,value);if(!address)return {};
  if(*address){classUiAllocations.insert(*address);classUiLifetimes[*address]=sourceMapPool_->allocationLifetime(*address);}
  return address;
 }
 if(q.function==0x8002fdc4){
  if(heap!=sourceMapPool_->root())return {};
  const auto token=sourceMapPool_->allocationLifetime(value);
  const auto found=classUiLifetimes.find(value);
  if(!token || found==classUiLifetimes.end() || found->second!=token)return {};
  // The pool owns the actual allocation incarnation. Retire draw admission
  // before real free; caller freezes the admitted actual VM map group.
  classUiAllocations.erase(value);classUiLifetimes.erase(found);
  return sourceMapPool_->free(heap,value)?std::optional<uint32_t>(0u):std::nullopt;
 }
 return {};
}
}
