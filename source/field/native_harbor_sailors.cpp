#include "native_harbor_sailors.hpp"
#include "battle_event_callbacks.hpp"
#include "field_battle_information.hpp"
#include "event_lifecycle.hpp"
#include "battle_actor.hpp"
#include <cassert>
namespace ttyd {
namespace {
constexpr uint32_t SailorRows[]={0x905044c8,0x905047a8};
constexpr unsigned TagOrdinals[]={2,12};
constexpr uint32_t TribeTable=0x803316d0,TribeStride=0x54,TribeCount=328;
constexpr uint32_t InvalidBattleName=0x802c3b10,RegistryTable=0x803057ec;
struct Guard{bool& x;explicit Guard(bool&v):x(v){x=true;}~Guard(){x=false;}};
}
NativeHarborSailors::NativeHarborSailors(EventNativeRuntime&vm,FieldNpcRegistryBinding&r,std::shared_ptr<FieldNpcRegistryOwner>o,std::shared_ptr<NativeFieldAnimation>a,std::shared_ptr<const EventScriptAssets>s):vm_(vm),vmProof_(vm.nativeFacadeProof()),registry_(r),owner_(std::move(o)),animation_(std::move(a)),source_(std::move(s)){}
std::unique_ptr<NativeHarborSailors>NativeHarborSailors::prepare(EventNativeRuntime&vm,FieldNpcRegistryBinding&r,std::shared_ptr<FieldNpcRegistryOwner>o,std::shared_ptr<NativeFieldAnimation>a,std::shared_ptr<const EventScriptAssets>s,const FieldMapNpcAssets&tags,uint32_t base,uint32_t area){
 if(!o||r.owner_!=o||&r.memory_!=&vm.memory||!o->initialized()||!a||!a->ready()||o->poses().sharedPool()!=a->pool()||!s||tags.count()!=17)return {};
 auto p=std::unique_ptr<NativeHarborSailors>(new NativeHarborSailors(vm,r,std::move(o),std::move(a),std::move(s)));
 if(!vm.memory.readWord(0x80415fd0,p->gp_)||!p->current()||!tags.mapTo(vm.memory,base))return {};
 p->inputBase_=base;p->mapped_=true;
 for(unsigned i=0;i<2;++i){auto input=tags.input(TagOrdinals[i],base,area);if(!input)return {};p->rows_[i].input=*input;p->rows_[i].setup=SailorRows[i];}
 auto& target=p->animation_->process().vm().memory;
 if(!p->owner_->aliases(target)){p->animationRegistry_=FieldNpcRegistryBinding::bind(p->owner_,target);if(!p->animationRegistry_)return {};}
 p->history_=FieldNpcRegularEventHistory::bind(p->owner_,vm);
 p->groups_=AnimationGroupRequestBinding::bind(p->animation_->process().vm().memory,p->animation_->files());
 if(!p->history_||!p->groups_||!p->capture())return {};
 return p;
}
NativeHarborSailors::~NativeHarborSailors(){
 // No constructor cancellation/actor deletion is synthesized here. Registry
 // retains unfinished Entry state; its actual resource owner remains required.
 if(mapped_&&vmProof_.current())vm_.memory.unregisterRam(inputBase_);
 auto& target=animation_->process().vm().memory;
 for(auto i=aliases_.rbegin();i!=aliases_.rend();++i)target.unregisterExternalRam(i->first);
}
bool NativeHarborSailors::current()const{
 if(!vmProof_.current()||!animation_||!animation_->ready())return false;
 uint32_t gp=0,mode=0,field=0;
 return vm_.memory.readWord(0x80415fd0,gp)&&gp==gp_&&vm_.memory.readWord(gp+0x14,mode)&&!mode&&vm_.memory.readWord(gp+0x168,field)&&field==owner_->fieldWorkAddress()&&owner_->aliases(vm_.memory);
}
bool NativeHarborSailors::word(uint32_t a,uint32_t&v)const{return current()&&vm_.memory.readWord(a,v);}
bool NativeHarborSailors::readable(uint32_t a,unsigned n)const{if(uint64_t(a)+n>0x100000000ULL)return false;uint8_t v=0;for(unsigned i=0;i<n;++i)if(!vm_.memory.readByte(a+i,v))return false;return true;}
bool NativeHarborSailors::text(uint32_t a,std::string&s,unsigned n)const{s.clear();for(unsigned i=0;i<n;++i){uint8_t v=0;if(uint64_t(a)+i>UINT32_MAX||!vm_.memory.readByte(a+i,v))return false;if(!v)return true;s+=char(v);}return false;}
bool NativeHarborSailors::capture(){
 for(auto&r:rows_){
  uint32_t original=0;if(!source_->readWord(r.setup,r.name)||!word(r.setup,original)||original!=r.name||!source_->readWord(r.setup+0x10,r.script)||!word(r.setup+0x10,original)||original!=r.script||!r.script||!admitOriginalNpcSetupScript(*source_,vm_.memory,r.script))return false;
  std::string wanted,tag,tribe;
  if(!text(r.name,wanted)||!text(r.input.name,tag)||wanted!=tag||!text(r.input.tribe,tribe))return false;
  for(unsigned i=0;i<TribeCount;++i){uint32_t name=0;const auto at=TribeTable+i*TribeStride;if(!word(at,name)||!name)return false;std::string candidate;if(!text(name,candidate))return false;if(candidate!=tribe)continue;r.tribe=at;if(!word(at+4,r.model))return false;break;}
  if(!r.tribe||!r.model)return false;
  const auto existing=registry_.lookup(r.input.name);if(!existing||*existing)return false;
 }
 return true;
}
bool NativeHarborSailors::aliasModel(){
 auto&r=rows_[ordinal_];std::string name;if(!text(r.model,name))return false;
 auto& target=animation_->process().vm().memory;
 if(&target==&vm_.memory)return true;
 for(const auto&a:aliases_)if(a.first==r.model)return vm_.memory.sharesRamWith(target,a.first,a.second);
 const auto size=uint32_t(name.size()+1);
 if(!vm_.memory.shareRamTo(target,r.model,size))return false;
 aliases_.push_back({r.model,size});return true;
}
// All non-retained source tail children are admitted before entering it. The
// two actual source records have BTLNO_INVALID: no battle RNG is reached.
bool NativeHarborSailors::preflight(){
 auto&r=rows_[ordinal_];std::string actual,invalid,expected;
 if(!text(r.name,expected)||!text(r.input.name,actual)||actual!=expected)return false;
 const auto named=registry_.lookup(r.input.name);if(!named||*named!=r.actor)return false;
 if(!text(r.input.tribe,expected))return false;
 bool matched=false;for(unsigned i=0;i<TribeCount;++i){uint32_t name=0;const auto row=TribeTable+i*TribeStride;if(!word(row,name)||!name)return false;if(!text(name,actual))return false;if(actual==expected){if(row!=r.tribe)return false;matched=true;break;}}if(!matched)return false;
 for(unsigned i=0;i<4;++i){uint32_t child=0;if(!word(r.actor+0x330+4*i,child)||child)return false;}
 uint32_t model=0;if(!word(r.tribe+4,model)||model!=r.model)return false;
 if(!text(r.input.battle,actual)||!text(InvalidBattleName,invalid)||actual!=invalid||!readable(r.tribe,TribeStride))return false;
 for(auto a:{r.input.position,r.input.territoryBase,r.input.loiter,r.input.homing})if(!readable(a,12))return false;
 if(!readable(r.input.direction,4))return false;for(auto a:r.input.ranges)if(!readable(a,4))return false;
 for(auto a:{r.input.ai,r.input.territory,r.input.area})if(!text(a,actual))return false;
 auto scan=[&](uint32_t table,unsigned stride,unsigned limit){for(unsigned i=0;i<limit;++i){uint32_t name=0;if(!word(table+i*stride,name))return false;if(!name)return true;if(!readable(table+i*stride,stride)||!text(name,actual,256))return false;}return false;};
 if(!scan(0x80338270,0x24,57)||!scan(0x80416580,8,4))return false;
 bool end=false;for(unsigned i=0;i<34;++i){uint32_t row=0,name=0;if(!word(RegistryTable+4*i,row))return false;if(!row){end=true;break;}if(!word(row,name)||!text(name,actual))return false;}if(!end)return false;
 uint32_t zero=0;if(!word(0x804207b4,zero)||!readable(r.actor,0x340))return false;
 for(unsigned off:{8u,0x10u,0x14u}){uint32_t name=0;if(!word(r.tribe+off,name))return false;if(name&&!text(name,actual,32))return false;}
 uint32_t handle=0;if(!word(r.actor+0x104,handle)||handle>=256)return false;const auto* pose=animation_->pool()->pose(int32_t(handle));if(!pose)return false;
 uint32_t name=0;if(!word(r.tribe+8,name))return false;if(name&&(!text(name,actual,32)||!owner_->poses().animationReady(pose->resource->modelName,actual)))return false;
 return true;
}
PackedMapNpcServices NativeHarborSailors::tailServices(){
 PackedMapNpcServices s;s.user=this;s.memory.user=this;
 s.memory.word=[](void*p,uint32_t a){auto&b=*static_cast<NativeHarborSailors*>(p);uint32_t v=0;const bool ok=b.vm_.memory.readWord(a,v);assert(ok);(void)ok;return v;};
 s.memory.half=[](void*p,uint32_t a){uint16_t v=0;const bool ok=static_cast<NativeHarborSailors*>(p)->vm_.memory.readHalf(a,v);assert(ok);(void)ok;return v;};
 s.memory.storeWord=[](void*p,uint32_t a,uint32_t v){const bool ok=static_cast<NativeHarborSailors*>(p)->vm_.memory.writeWord(a,v);assert(ok);(void)ok;};
 s.memory.storeHalf=[](void*p,uint32_t a,uint16_t v){const bool ok=static_cast<NativeHarborSailors*>(p)->vm_.memory.writeHalf(a,v);assert(ok);(void)ok;};
 s.memory.storeByte=[](void*p,uint32_t a,uint8_t v){const bool ok=static_cast<NativeHarborSailors*>(p)->vm_.memory.writeByte(a,v);assert(ok);(void)ok;};
 s.compare=[](void*p,uint32_t a,uint32_t b){auto&m=static_cast<NativeHarborSailors*>(p)->vm_.memory;for(;;++a,++b){uint8_t x=0,y=0;const bool ok=m.readByte(a,x)&&m.readByte(b,y);assert(ok);(void)ok;if(x!=y||!x)return int32_t(x)-int32_t(y);}};
 s.copyString=[](void*p,uint32_t a,uint32_t b){auto&m=static_cast<NativeHarborSailors*>(p)->vm_.memory;uint8_t v=0;do{const bool ok=m.readByte(b++,v)&&m.writeByte(a++,v);assert(ok);(void)ok;}while(v);};
 s.entry=[](void*p,uint32_t name,uint32_t model){auto&b=*static_cast<NativeHarborSailors*>(p);const auto&r=b.rows_[b.ordinal_];assert(r.entry.complete()&&name==r.input.name&&model==r.model);(void)r;(void)name;(void)model;};
 s.lookup=[](void*p,uint32_t name){auto&b=*static_cast<NativeHarborSailors*>(p);auto a=b.registry_.lookup(name);assert(a&&*a);return *a;};
 s.area=[](void*p,uint32_t name){auto&b=*static_cast<NativeHarborSailors*>(p);std::string area;const bool ok=b.text(name,area);assert(ok);(void)ok;return packedBattleAreaData(area.c_str(),nativeBattleActorServices(b.vm_));};
 s.battle=[](void*p,uint32_t actor,int32_t index){auto&b=*static_cast<NativeHarborSailors*>(p);assert(index==-1);PackedNpcBattleSetupServices services;services.memory=nativeBattleActorServices(b.vm_);setPackedNpcBattleInformation(actor,index,services);};
 s.animation=[](void*p,int32_t handle,uint32_t name,int32_t mode){auto&b=*static_cast<NativeHarborSailors*>(p);std::string animation;BattlePoseClock clock;const bool ok=b.text(name,animation,32)&&mode==1&&b.animation_->clock(clock)&&b.owner_->poses().selectName(handle,animation,true,clock);assert(ok);(void)ok;};return s;
}
bool NativeHarborSailors::finishRow(){
 auto&r=rows_[ordinal_];if(!preflight())return false;
 const auto actor=registerPackedMapNpc(r.input,tailServices());if(actor!=r.actor)return false;
 // Native constructor policy: use the ORIGINAL setup's regular script and
 // the real source event allocator; no fabricated ID or successful Init claim.
 if(!vm_.memory.writeWord(r.actor+0x124,r.script))return false;
 phase_=4;return true;
}
bool NativeHarborSailors::advance(){
 if(invoking_||!current())return false;Guard guard(invoking_);
 while(ordinal_<rows_.size()){
  auto&r=rows_[ordinal_];
  if(!phase_){pending_=0x800536e4;if(!aliasModel())return false;phase_=1;}
  if(phase_==1){auto loaded=groups_->advance(r.model,0);if(!loaded)return false;if(!*loaded){animation_->files()->advanceAsync(4096);return false;}phase_=2;}
  if(phase_==2){BattlePoseClock clock;if(!animation_->clock(clock)||!registry_.resumeEnter(r.input.name,r.model,clock,r.entry,true))return false;auto a=registry_.lookup(r.input.name);if(!a||!*a)return false;r.actor=*a;phase_=3;}
  if(phase_==3&&!finishRow())return false;
  if(phase_==4){uint32_t script=0,original=0;if(!word(r.setup+0x10,script)||script!=r.script||!source_->readWord(r.setup+0x10,original)||original!=script)return false;pending_=0x800374ac;if(vm_.schedule.managerActive||vm_.gp.battleContext||vm_.field.capacity<0||vm_.field.capacity>256||!vm_.field.entries)return false;
   bool free=false;for(int i=0;i<vm_.field.capacity;++i)free|=!(vm_.field.entries[i].flags&1);if(!free)return false;
   auto&e=createRootEvent(vm_.schedule,r.script,0,0x20,vm_.creationServices());e.rawBytes[0]=1;e.field170=r.actor;r.event=e.id;
   const bool ok=vm_.memory.writeWord(r.actor+0x11c,r.event);assert(ok);(void)ok;++ordinal_;phase_=0;
  }
 }
 pending_=0;return true;
}
bool NativeHarborSailors::ready()const{return current()&&ordinal_==rows_.size();}
std::optional<uint32_t>NativeHarborSailors::actor(unsigned n)const{if(!ready()||n>=rows_.size())return {};return rows_[n].actor;}
std::optional<uint32_t>NativeHarborSailors::regularEvent(unsigned n)const{if(!ready()||n>=rows_.size()||!history_->owns(rows_[n].event,rows_[n].actor))return {};return rows_[n].event;}
}
