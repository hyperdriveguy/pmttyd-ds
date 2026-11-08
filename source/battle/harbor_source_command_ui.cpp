#include "harbor_source_command_ui.hpp"
#include "battle_command_input_continuation.hpp"
#include "battle_act_menu_continuation.hpp"
#include "battle_action_class_table_continuation.hpp"
#include "battle_weapon_menu_continuation.hpp"
#include "battle_weapon_ui_continuation.hpp"
#include "battle_menu_audience_native.hpp"
#include "battle_packed_selection.hpp"
#include "battle_packed_status.hpp"
#include "battle_action_help.hpp"
#include "battle_command_pose.hpp"
#include "battle_packed_part.hpp"
#include "battle_weapon_table_continuation.hpp"
#include "battle_weapon_cost_continuation.hpp"
#include "battle_packed_pouch.hpp"
#include "battle_packed_choice.hpp"
#include "battle_packed_takeoff.hpp"
#include "battle_attack_selection.hpp"
#include <utility>
namespace ttyd {
namespace {
constexpr uint32_t kGlobalPouch=0x8041eb00,kGlobalMapHeap=0x8041e80c;
constexpr uint32_t kMenuOffset=0x171c,kTriggerOffset=0xf44;
constexpr uint32_t kRepeatOffset=0xfbc,kHeldOffset=0x1034;
int32_t signedWord(uint32_t n){return n<0x80000000u?int32_t(n):int32_t(int64_t(n)-0x100000000LL);}
struct Invocation {bool& value;explicit Invocation(bool& v):value(v){value=true;}~Invocation(){value=false;}};
}
struct HarborSourceCommandUi::State {
 HarborSourceCommandUiBinding binding;
 BattleCommandInputContinuationCursor input;
 BattleActMenuContinuationCursor act,actEnd;
 BattleActUiContinuationCursor classUi;
 BattleActionClassTableCursor classes;
 BattleWeaponMenuContinuationCursor weapon;
 BattleWeaponUiCursor weaponUi;
 BattleMenuAudienceNativeCursor audience;
 BattleWeaponTableContinuationCursor weaponTable;
 BattleWeaponEvaluationCursor weaponEligibility,weaponDecision,weaponTargets,weaponAffordability;
 PackedWeaponCostCursor weaponFpCost;
 // Native provider-record identity only. The outer entry's actual runtime
 // lock, not this record, protects source VM accesses through each hold.
 std::shared_ptr<const uint8_t> costProvider=std::make_shared<const uint8_t>(0);
 bool invoking=false,poisoned=false;
 uint32_t pending=0;
 explicit State(HarborSourceCommandUiBinding b):binding(std::move(b)){}
 bool current(){if(poisoned)return false;if(!binding.current||!binding.current()){poisoned=true;return false;}return true;}
 const BattleActorServices& actors()const{return binding.actors;}
 uint32_t word(uint32_t a)const{return actors().memory.word(actors().memory.user,a);}
 uint8_t byte(uint32_t a)const{return actors().byte(actors().user,a);}
 void store(uint32_t a,uint32_t n)const{actors().memory.storeWord(actors().memory.user,a,n);}
 std::optional<uint32_t> external(uint32_t fn,uint32_t site,std::array<uint32_t,4> args={},uint8_t count=0){
  pending=fn;if(!binding.child||!current())return {};
  const auto result=binding.child(binding.user,{fn,site,args,count});
  if(!current())return {};
  if(result)pending=0;
  return result;
 }
 std::optional<uint32_t> knownWord(uint32_t a){return binding.word?binding.word(binding.user,a):std::nullopt;}
 PackedWeaponCostServices costServices(){
  PackedWeaponCostServices services;services.user=this;services.lifetime=costProvider;
  services.current=[](void* u){return static_cast<State*>(u)->current();};
  services.word=[](void* u,uint32_t a){return static_cast<State*>(u)->knownWord(a);};
  services.byte=[](void* u,uint32_t a)->std::optional<uint8_t>{auto& s=*static_cast<State*>(u);return s.binding.byte?s.binding.byte(s.binding.byteUser,a):std::nullopt;};
  services.equippedCount=[](void* u,uint32_t badge)->std::optional<uint32_t>{auto& s=*static_cast<State*>(u);s.pending=0x800d3300;const auto pouch=s.knownWord(kGlobalPouch);if(!pouch)return {};return uint32_t(packedPouchEquippedBadgeCount(*pouch,int32_t(badge),s.actors().memory));};
  services.power=[](void* u,double a,double b)->std::optional<double>{auto& s=*static_cast<State*>(u);s.pending=0x80272620;return s.binding.power?s.binding.power(s.binding.user,a,b):std::nullopt;};
  services.truncate=[](void* u,double a)->std::optional<int32_t>{auto& s=*static_cast<State*>(u);s.pending=0x80126a70;return s.binding.truncate?s.binding.truncate(s.binding.user,a):std::nullopt;};
  return services;
 }
 BattleWeaponEvaluationServices evaluationServices(){return {actors(),this,[](void* u){return static_cast<State*>(u)->current();},[](void* u,const BattleWeaponEvaluationRequest& q)->std::optional<uint32_t>{
  auto& s=*static_cast<State*>(u);s.pending=q.function;const auto& a=q.arguments;
  switch(q.function){
  case 0x80126c64:return uint32_t(packedUnitFp(a[0],false,s.actors()));
  case 0x80126968:return resumePackedWeaponCost(a[0],a[1],s.costServices(),s.weaponFpCost);
  case 0x800d3c44:{const auto pouch=s.knownWord(kGlobalPouch);if(!pouch)return {};const auto ap=s.actors().memory.half(s.actors().memory.user,*pouch+0x7a);return uint32_t(ap<32768?int32_t(ap):int32_t(ap)-65536);}
  case 0x80127454:return uint32_t(packedEnemyBelong(a[0],s.actors()));
  case 0x80128144:return uint32_t(s.byte(a[0]+0xc));
  case 0x801268fc:{const auto value=resumePackedBattleWeaponCostCheck(a[0],a[1],s.evaluationServices(),s.weaponAffordability);return value?std::optional<uint32_t>(uint32_t(*value)):std::nullopt;}
  case 0x800ff5c0:return s.binding.evaluationChild?s.binding.evaluationChild(s.binding.evaluationUser,q):std::nullopt;
  default:return s.request(q.function,q.callsite,{a[0],a[1],a[2],a[3]},q.count,q.privateCursorOutput);
  }
 },binding.privateSamplingBuffer};}
 BattleWeaponTableContinuationServices weaponTableServices(){return {actors(),this,[](void* u){return static_cast<State*>(u)->current();},[](void* u,const BattleWeaponTableContinuationRequest& q)->std::optional<uint32_t>{
  auto& s=*static_cast<State*>(u);s.pending=q.function;const auto& a=q.arguments;
  if(q.privateCountOutput)return {}; // Actual party callback needs its own retained native output adapter.
  switch(q.function){
  case 0x800d5eb0:return s.knownWord(kGlobalPouch);
  case 0x800d3300:{const auto pouch=s.knownWord(kGlobalPouch);if(!pouch)return {};return uint32_t(packedPouchEquippedBadgeCount(*pouch,int32_t(a[0]),s.actors().memory));}
  case 0x80124c64:case 0x80124c10:{
   const auto pouch=s.knownWord(kGlobalPouch);if(!pouch)return {};
   const auto raw=s.byte(*pouch+(q.function==0x80124c64?0x98:0x99));
   const auto level=raw<128?int32_t(raw):int32_t(raw)-256;
   const auto base=q.function==0x80124c64?167u:164u;return level==3?base+2:level==2?base+1:base;
  }
  case 0x8012733c:return packedUnitData(a[0],int32_t(a[1]),s.actors());
  default:return s.request(q.function,q.callsite,{a[0],a[1],a[2]},q.count,q.privateCursorOutput);
  }
 }};}
 BattleActMenuContinuationServices actServices(){return {actors(),this,[](void* u,const BattleActMenuContinuationRequest& q){auto& s=*static_cast<State*>(u);return s.request(uint32_t(q.function),q.callsite,q.arguments,q.count,q.privateCursorOutput);}};}
 BattleWeaponMenuContinuationServices weaponServices(){return {actors(),this,[](void* u,const BattleWeaponMenuContinuationRequest& q){auto& s=*static_cast<State*>(u);return s.request(q.function,q.callsite,q.arguments,q.count,q.privateCursorOutput);}};}
 std::optional<uint32_t> mapAllocate(uint32_t bytes){
  // The real source MapAlloc wrapper captures its root on first entry, not on
  // each resumed provider call. Class UI's callback therefore owns this request.
  if(!classMapRequest){pending=0x8002ff58;const auto root=knownWord(kGlobalMapHeap);if(!root)return {};classMapRequest=HarborSourceCommandUiRequest{0x8002ff58,0x800f8948,{*root,bytes},2};}
  const auto& q=*classMapRequest;const auto result=external(q.function,q.callsite,q.arguments,q.count);if(result)classMapRequest.reset();return result;
 }
 bool mapFree(uint32_t address){
  if(!classMapRequest){pending=0x8002fdc4;const auto root=knownWord(kGlobalMapHeap);if(!root)return false;classMapRequest=HarborSourceCommandUiRequest{0x8002fdc4,0x800f8920,{*root,address},2};}
  const auto& q=*classMapRequest;const auto result=external(q.function,q.callsite,q.arguments,q.count);if(result)classMapRequest.reset();return result.has_value();
 }
 std::optional<HarborSourceCommandUiRequest> classMapRequest;
 std::optional<HarborSourceCommandUiRequest> targetAnimation;
 bool endingTarget=false;uint32_t targetEndMenu=0;
 std::optional<uint32_t> targetEndCursor;
 std::optional<uint32_t> targetSetup(uint32_t b){
  // Whole80120fe4/58: unit lookup, source flag store, body-ID/part
  // lookups, then original SetAnimationType(part,40). Its source-visible
  // flag store must not replay while the name/resource child is unavailable.
  if(!targetAnimation){
   const auto unit=battleRegistryUnit(b,signedWord(word(b+0x420)),actors());
   store(b+0x1720,word(b+0x1720)|0x10000);
   const auto id=packedBattleBodyPartId(unit,actors().memory);
   const auto part=packedUnitPart(unit,id,actors().memory);
   targetAnimation=HarborSourceCommandUiRequest{0x801276ac,0x80121024,{part,40},2};
  }
  const auto& q=*targetAnimation;const auto result=external(q.function,q.callsite,q.arguments,q.count);
  if(result)targetAnimation.reset();
  return result;
 }
 std::optional<uint32_t> targetEnd(uint32_t b){
  // Whole801209e8/5c: private cursor output is deliberately not consumed.
  // CameraMode may hold; the two later flag stores are independent fresh RMWs.
  if(!endingTarget){
   targetEndMenu=b+0x171c;const auto type=word(b+0x1c5c);
   BattlePackedSelectionServices selection;selection.memory=actors();uint32_t output=0;
   if(findPackedCommandCursor(targetEndMenu,type,output,selection))targetEndCursor=output;
   endingTarget=true;
  }
  const auto result=external(0x8012c028,0x80120a14,{0,0},2);if(!result)return {};
  store(targetEndMenu+4,word(targetEndMenu+4)&~0x10000u);
  store(targetEndMenu+4,word(targetEndMenu+4)&~0x1000000u);
  endingTarget=false;targetEndCursor.reset();return 0;
 }
 BattleActUiServices classUiServices(){return {actors(),this,[](void* u,uint32_t n){return static_cast<State*>(u)->mapAllocate(n);},[](void* u,uint32_t a){return static_cast<State*>(u)->mapFree(a);}};}
 BattleWeaponUiServices weaponUiServices(){return {actors(),this,[](void* u){return static_cast<State*>(u)->current();},[](void* u,const BattleWeaponUiRequest& q){return static_cast<State*>(u)->external(q.function,q.callsite,{q.arguments[0],q.arguments[1]},q.count);}};}
 BattleActionClassServices classServices(){
  return {actors(),this,[](void* u,uint32_t b,int32_t id)->std::optional<uint32_t>{auto& s=*static_cast<State*>(u);return battleRegistryUnit(b,id,s.actors());},
   [](void* u){return static_cast<State*>(u)->knownWord(kGlobalPouch);},
   [](void* u,uint32_t key){return static_cast<State*>(u)->external(0x80081b8c,0,{key},1);}};
 }
 BattleMenuAudienceNativeServices audienceServices(){return {actors(),this,
  [](void* u){return static_cast<State*>(u)->current();},
  [](void* u,uint32_t mask)->std::optional<uint32_t>{auto& s=*static_cast<State*>(u);return s.word(s.binding.battle+kTriggerOffset)&mask;},
  [](void* u,uint32_t b)->std::optional<uint32_t>{auto& s=*static_cast<State*>(u);return battleMarioUnit(b,s.actors());},
  [](void* u,uint32_t b){auto& s=*static_cast<State*>(u);const auto done=resumePackedBattleActMenuEnd(b,s.actServices(),s.actEnd);if(!done)s.pending=s.actEnd.pending();return done;}};}
 std::optional<uint32_t> request(uint32_t fn,uint32_t site,const std::array<uint32_t,4>& a,uint8_t count,std::optional<uint32_t>* cursor=nullptr){
  pending=fn;if(!current())return {};
  switch(fn){
  case 0x800f88cc:return battleRegistryUnit(a[0],signedWord(a[1]),actors());
  case 0x800f87a8:return battleMarioUnit(a[0],actors());
  case 0x800f8728:return battlePartyUnit(a[0],actors());
  case 0x80127814:return checkPackedBattleStatus(a[0],signedWord(a[1]),actors())?1u:0u;
  case 0x80124cb8:{
   uint32_t address=0;BattlePackedSelectionServices selection;selection.memory=actors();
   if(!cursor)return {};
   if(findPackedCommandCursor(a[0],a[1],address,selection))*cursor=address;
   return 0; // Original no-match leaves the private output unwritten.
  }
  case 0x80119df8:return packedBattleMenuKeyAllowed(a[0],actors())?1u:0u;
  case 0x8011a06c:return word(binding.battle+kTriggerOffset)&a[0];
  case 0x80119f60:return word(binding.battle+kRepeatOffset)&a[0];
  case 0x80119fe0:return word(binding.battle+kHeldOffset)&a[0];
  case 0x8011f5f0:{
   const auto b=currentBattleAddress(actors());
   if(!byte(b+0x1c74))store(b+0x1c70,word(b+kMenuOffset));
   else if(word(b+0x1c70)==word(b+kMenuOffset)){store(b+0x1c70,0);actors().memory.storeByte(actors().memory.user,b+0x1c74,0);}
   return 0;
  }
  case 0x80216954:packedBattleActionHelpSetKey(a[0],actors());return 0;
  case 0x8021690c:packedBattleActionHelpSetDisplay(signedWord(a[0]),actors());return 0;
  case 0x80122794:{const bool done=resumePackedBattleActMenuSetup(a[0],a[1],actServices(),act);if(!done){return {};}return 0;}
  case 0x8012230c:{const auto result=resumePackedBattleActMenuMain(a[0],actServices(),act);if(!result){return {};}return uint32_t(*result);}
  case 0x8012214c:{const auto result=resumePackedBattleChangePosition(a[0],actServices(),actEnd);if(!result){return {};}return uint32_t(*result);}
  case 0x80124dc8:{const auto result=resumePackedBattleActionClassTable(a[0],classServices(),classes);if(!result&&classes.pending())pending=classes.pending();return result;}
  case 0x80116cdc:{const bool done=resumePackedBattleActUiInit(a[0],a[1],a[2],a[3],classUiServices(),classUi);if(!done)return {};return 0;}
  case 0x80116c78:{const bool done=resumePackedBattleActUiEnd(a[0],classUiServices(),classUi);if(!done)return {};return 0;}
  case 0x80122068:{const bool done=resumePackedBattleWeaponMenuSetup(a[0],a[1],a[2],weaponServices(),weapon);if(!done){return {};}return 0;}
  case 0x80121d0c:{const auto result=resumePackedBattleWeaponMenuMain(a[0],a[1],weaponServices(),weapon);if(!result){return {};}return uint32_t(*result);}
  case 0x80116070:{const bool done=resumePackedBattleWeaponUiInit(a[0],a[1],a[2],a[3],weaponUiServices(),weaponUi);if(!done){return {};}return 0;}
  case 0x8011600c:{const bool done=resumePackedBattleWeaponUiEnd(a[0],weaponUiServices(),weaponUi);if(!done){return {};}return 0;}
  case 0x800ff5a0:{BattleWeaponEvaluationRequest q;q.function=fn;q.callsite=site;q.arguments[0]=a[0];q.count=1;return binding.evaluationChild?binding.evaluationChild(binding.evaluationUser,q):std::nullopt;}
  case 0x80123ec0:{const auto result=resumePackedBattleWeaponTable(a[0],a[1],weaponTableServices(),weaponTable);return result?std::optional<uint32_t>(uint32_t(*result)):std::nullopt;}
  case 0x80123db0:{const bool ready=resumePackedBattleWeaponEligibility(a[0],a[1],evaluationServices(),weaponEligibility);return ready?std::optional<uint32_t>(0u):std::nullopt;}
  case 0x80123d00:{const auto result=resumePackedBattleWeaponTargetCount(a[0],a[1],a[2],evaluationServices(),weaponTargets);return result?std::optional<uint32_t>(uint32_t(*result)):std::nullopt;}
  case 0x80123210:{const auto result=resumePackedBattleWeaponDecision(a[0],a[1],a[2],evaluationServices(),weaponDecision);return result?std::optional<uint32_t>(uint32_t(*result)):std::nullopt;}
  case 0x80120fe4:return targetSetup(a[0]);
  case 0x801209e8:return targetEnd(a[0]);
  case 0x8011f524:{const auto result=resumeNativeBattleMenuAudience(audienceServices(),audience);if(!result){return {};}return *result?1u:0u;}
  // These complete original bodies are blr, not pending display acknowledgments.
  case 0x80116cd8:case 0x8011606c:
  case 0x80138988:case 0x8013898c:case 0x80138990:case 0x80138994:
  case 0x80138998:case 0x8013899c:return 0;
  default:return external(fn,site,a,count);
  }
 }
};
HarborSourceCommandUi::HarborSourceCommandUi(HarborSourceCommandUiBinding b):state_(std::make_unique<State>(std::move(b))){}
HarborSourceCommandUi::~HarborSourceCommandUi()=default;
std::optional<int32_t> HarborSourceCommandUi::commandInput(){
 auto& s=*state_;
 if(s.invoking||s.poisoned)return {};
 const auto owner=s.binding.runtime.lock();if(!owner){s.poisoned=true;return {};}
 Invocation invocation(s.invoking);
 if(!s.current())return {};
 BattleCommandInputContinuationServices services{s.actors(),&s,[](void* user,const BattleCommandInputContinuationRequest& q){auto& state=*static_cast<State*>(user);return state.request(q.function,q.callsite,{q.arguments[0],q.arguments[1],q.arguments[2]},q.count,q.privateCursorOutput);}};
 const auto result=resumePackedBattleCommandInput(s.binding.battle,services,s.input);
 if(!s.current())return {};
 if(result)s.pending=0;
 else if(!s.pending)s.pending=s.input.pending();
 return result;
}
bool HarborSourceCommandUi::pending()const{return state_->input.active();}
uint32_t HarborSourceCommandUi::pendingFunction()const{return state_->pending;}
bool HarborSourceCommandUi::stale()const{return state_->poisoned;}
uint32_t HarborSourceCommandUi::battleAddress()const{return state_->binding.battle;}
}
