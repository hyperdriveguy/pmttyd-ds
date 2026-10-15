#include "field_npc_registry_owner.hpp"
#include "event_address_space.hpp"
#include "field_battle_information.hpp"
#include <cassert>
#include <cstring>
namespace ttyd {
namespace {
constexpr uint32_t Registry=0x8041e8ac,Sizes[]={36*0x340,36*0x94,16*0x340,0x580};
uint32_t bits(float f){uint32_t v;std::memcpy(&v,&f,4);return v;}
}
FieldNpcRegistryOwner::FieldNpcRegistryOwner(uint32_t base){
 for(unsigned i=0;i<4;++i){addresses_[i]=base;allocations_[i].resize(Sizes[i]);base+=Sizes[i];}
}
std::shared_ptr<FieldNpcRegistryOwner> FieldNpcRegistryOwner::cold(uint32_t base){
 uint64_t end=base;for(auto n:Sizes)end+=n;
 if(base<0x10000 || (base&15) || end>0x100000000ULL)return {};
 return std::shared_ptr<FieldNpcRegistryOwner>(new FieldNpcRegistryOwner(base));
}
uint8_t* FieldNpcRegistryOwner::span(uint32_t a,uint32_t n){
 for(unsigned i=0;i<4;++i)if(a>=addresses_[i] && uint64_t(a)-addresses_[i]+n<=allocations_[i].size())return allocations_[i].data()+(a-addresses_[i]);
 return nullptr;
}
FieldNpcRegistryBinding::FieldNpcRegistryBinding(std::shared_ptr<FieldNpcRegistryOwner> o,EventAddressSpace& m):owner_(std::move(o)),memory_(m){}
FieldNpcRegistryBinding::~FieldNpcRegistryBinding(){for(auto base:mapped_){const bool removed=memory_.unregisterExternalRam(base);assert(removed);(void)removed;}}
std::unique_ptr<FieldNpcRegistryBinding> FieldNpcRegistryBinding::bind(std::shared_ptr<FieldNpcRegistryOwner> owner,EventAddressSpace& memory){
 if(!owner)return {};
 auto binding=std::unique_ptr<FieldNpcRegistryBinding>(new FieldNpcRegistryBinding(std::move(owner),memory));auto& o=*binding->owner_;
 if(!memory.registerExternalRam(Registry,o.registry_.data(),o.registry_.size()))return {};
 binding->mapped_.push_back(Registry);
 for(unsigned i=0;i<4;++i){if(!memory.registerExternalRam(o.addresses_[i],o.allocations_[i].data(),Sizes[i]))return {};binding->mapped_.push_back(o.addresses_[i]);}
 return binding;
}
bool FieldNpcRegistryBinding::text(uint32_t a,std::string& out,unsigned limit) const{
 out.clear();for(unsigned i=0;i<limit;++i){uint8_t c;if(uint64_t(a)+i>UINT32_MAX || !memory_.readByte(a+i,c))return false;if(!c)return true;out.push_back(char(c));}return false;
}
struct FieldNpcRegistryBinding::Call {FieldNpcRegistryBinding* binding;BattlePoseClock clock;unsigned allocation=0;};
PackedNpcRegistryServices FieldNpcRegistryBinding::services(Call& c){
 PackedNpcRegistryServices s;s.user=&c;s.memory.user=&c;
 s.memory.word=[](void* p,uint32_t a){uint32_t v=0;const bool ok=static_cast<Call*>(p)->binding->memory_.readWord(a,v);assert(ok);(void)ok;return v;};
 s.memory.half=[](void* p,uint32_t a){uint16_t v=0;const bool ok=static_cast<Call*>(p)->binding->memory_.readHalf(a,v);assert(ok);(void)ok;return v;};
 s.memory.storeWord=[](void* p,uint32_t a,uint32_t v){const bool ok=static_cast<Call*>(p)->binding->memory_.writeWord(a,v);assert(ok);(void)ok;};
 s.memory.storeHalf=[](void* p,uint32_t a,uint16_t v){const bool ok=static_cast<Call*>(p)->binding->memory_.writeHalf(a,v);assert(ok);(void)ok;};
 s.memory.storeByte=[](void* p,uint32_t a,uint8_t v){const bool ok=static_cast<Call*>(p)->binding->memory_.writeByte(a,v);assert(ok);(void)ok;};
 s.allocate=[](void* p,int32_t heap,uint32_t n){auto& c=*static_cast<Call*>(p);assert(heap==0 && c.allocation<4 && n==Sizes[c.allocation]);(void)heap;(void)n;return c.binding->owner_->addresses_[c.allocation++];};
 s.clear=[](void* p,uint32_t a,uint32_t n){auto* dst=static_cast<Call*>(p)->binding->owner_->span(a,n);assert(dst);std::memset(dst,0,n);};
 s.compare=[](void* p,uint32_t a,uint32_t b)->int32_t{auto& m=static_cast<Call*>(p)->binding->memory_;for(;;++a,++b){uint8_t x=0,y=0;const bool ok=m.readByte(a,x) && m.readByte(b,y);assert(ok);(void)ok;if(x!=y || !x)return int32_t(x)-int32_t(y);}};
 s.copyString=[](void* p,uint32_t a,uint32_t b){auto& m=static_cast<Call*>(p)->binding->memory_;uint8_t v=0;do{const bool ok=m.readByte(b++,v) && m.writeByte(a++,v);assert(ok);(void)ok;}while(v);};
 s.poseEntry=[](void* p,uint32_t a,int32_t mode)->int32_t{auto& c=*static_cast<Call*>(p);std::string model;const bool ok=c.binding->text(a,model,64);assert(ok);(void)ok;const auto h=c.binding->owner_->poses_.enter(model,mode,c.clock);assert(h);return *h;};
 s.paperOn=[](void* p,int32_t h){const bool ok=static_cast<Call*>(p)->binding->owner_->poses_.paperOn(h);assert(ok);(void)ok;};
 s.heightBits=[](void* p,int32_t h){const auto v=static_cast<Call*>(p)->binding->owner_->poses_.height(h);assert(v);return bits(*v);};
 s.radiusBits=[](void* p,int32_t h){const auto v=static_cast<Call*>(p)->binding->owner_->poses_.radius(h);assert(v);return bits(*v);};
 s.vivianType=[](void* p,int32_t h)->int32_t{const auto v=static_cast<Call*>(p)->binding->owner_->poses_.vivianType(h);assert(v);return *v;};
 s.lightFlagOn=[](void* p,int32_t h,uint32_t flags){const bool ok=static_cast<Call*>(p)->binding->owner_->poses_.materialLightFlagOn(h,flags);assert(ok);(void)ok;};
 return s;
}
bool FieldNpcRegistryBinding::initialize(){
 if(owner_->initialized_)return false;
 uint32_t gp=0,v=0;if(!memory_.readWord(0x80415fd0,gp) || uint64_t(gp)+0x16b>UINT32_MAX || !memory_.readWord(gp+0x168,v))return false;
 // Cold BSS is retained by this owner; all four allocations are already
 // mapped before npcInit effects. Native allocation bytes are not a source
 // initialization claim for the auxiliary buffer which npcInit never clears.
 Call call{this,{}};initializePackedNpcRegistry(services(call));assert(call.allocation==4);owner_->initialized_=true;return true;
}
bool FieldNpcRegistryBinding::registry(uint32_t& base,uint32_t& array,uint32_t& count) const{
 if(!owner_->initialized_)return false;
 uint32_t gp=0,mode=0;if(!memory_.readWord(0x80415fd0,gp) || uint64_t(gp)+0x17>UINT32_MAX || !memory_.readWord(gp+0x14,mode))return false;
 base=0x8041e8b4+(mode?0x14:0);if(!memory_.readWord(base+4,count) || !memory_.readWord(base+0xc,array))return false;
 return count<=(mode?16u:36u) && array==owner_->addresses_[mode?2:0];
}
std::optional<std::vector<uint32_t>> FieldNpcRegistryBinding::activeActors() const{
 uint32_t base=0,array=0,count=0;if(!registry(base,array,count))return {};
 std::vector<uint32_t> actors;
 for(unsigned i=0;i<count;++i){uint32_t flags=0;const auto a=array+i*0x340;if(!memory_.readWord(a,flags))return {};if(flags&1){std::string name;if(!text(a+8,name,32))return {};actors.push_back(a);}}
 return actors;
}
std::optional<uint32_t> FieldNpcRegistryBinding::lookup(uint32_t name,PackedNpcLookup kind){
 std::string bytes;if(!text(name,bytes,32) || !activeActors())return {};
 Call call{this,{}};return packedNpcNameToPointer(name,kind,services(call));
}
bool FieldNpcRegistryBinding::canEnter(uint32_t name,uint32_t model) const{
 std::string nameBytes,modelBytes,luigi;
 uint32_t base=0,array=0,count=0,gp=0,mode=0,unused=0;
 if(!text(name,nameBytes,32) || !text(model,modelBytes,64) || !registry(base,array,count) || !activeActors())return false;
 uint32_t freeActor=0;for(unsigned i=0;i<count;++i){uint32_t flags=0;const bool ok=memory_.readWord(array+i*0x340,flags);assert(ok);(void)ok;if(!(flags&1)){freeActor=array+i*0x340;break;}}if(!freeActor)return false;
 // Source strcpy/resource requests cannot alias the slot cleared immediately
 // before them in this synchronous native admission domain.
 auto overlaps=[&](uint32_t a,size_t n){return uint64_t(a)<uint64_t(freeActor)+0x340 && uint64_t(freeActor)<uint64_t(a)+n;};
 if(overlaps(name,nameBytes.size()+1) || overlaps(model,modelBytes.size()+1))return false;
 if(!memory_.readWord(0x80415fd0,gp) || uint64_t(gp)+0x47>UINT32_MAX || !memory_.readWord(gp+0x14,mode) || !memory_.readWord(gp+0x38,unused) || !memory_.readWord(gp+0x3c,unused))return false;
 if(!text(0x802c1468,luigi,64) || !owner_->poses_.canEnter(modelBytes,mode?1:0))return false;
 const uint32_t constants[]={0x802c11f8,0x802c11fc,0x802c1200,0x802c1204,0x802c1208,0x802c120c,0x8041fc94,0x8041fc7c,0x8041fc6c};
 for(auto a:constants)if(!memory_.readWord(a,unused))return false;
 return true;
}
std::optional<int32_t> FieldNpcRegistryBinding::enter(uint32_t name,uint32_t model,BattlePoseClock clock){
 if(!canEnter(name,model))return {};
 Call call{this,clock};return enterPackedNpc(name,model,services(call));
}


std::optional<uint32_t> FieldNpcRegistryBinding::registerMapNpc(const PackedMapNpcInput& i,BattlePoseClock clock,const FieldMapNpcBattleAdmission& battle){
 // Preflight source tables/strings without effects. Parsed input lives outside
 // owner allocations, just as the original source stack does.
 auto readable=[&](uint32_t a,unsigned n){uint8_t v;for(unsigned k=0;k<n;++k)if(uint64_t(a)+k>UINT32_MAX || !memory_.readByte(a+k,v))return false;return true;};
 auto external=[&](uint32_t a,unsigned n){for(unsigned k=0;k<4;++k)if(uint64_t(a)<uint64_t(owner_->addresses_[k])+Sizes[k] && uint64_t(owner_->addresses_[k])<uint64_t(a)+n)return false;return !(uint64_t(a)<uint64_t(Registry)+0x30 && uint64_t(Registry)<uint64_t(a)+n);};
 std::string names[5],areaName,invalid;
 const uint32_t strings[]={i.name,i.tribe,i.ai,i.territory,i.battle};
 for(unsigned k=0;k<5;++k)if(!text(strings[k],names[k],64) || !external(strings[k],unsigned(names[k].size()+1)))return {};
 if(!text(i.area,areaName,64) || !external(i.area,unsigned(areaName.size()+1)) || !text(0x802c3b10,invalid,64))return {};
 const uint32_t vectors[]={i.position,i.territoryBase,i.loiter,i.homing};for(auto a:vectors)if(!readable(a,12) || !external(a,12))return {};
 if(!readable(i.direction,4) || !external(i.direction,4))return {};
 for(auto a:i.ranges)if(!readable(a,4) || !external(a,4))return {};
 auto find=[&](uint32_t table,unsigned stride,unsigned limit,const std::string& key)->std::optional<uint32_t>{for(unsigned n=0;n<limit;++n){const uint64_t at=uint64_t(table)+uint64_t(stride)*n;if(at+3>UINT32_MAX)return {};uint32_t a=0;if(!memory_.readWord(uint32_t(at),a))return {};if(!a)return 0;std::string actual;if(!text(a,actual,256))return {};if(actual==key)return uint32_t(at);}return {};};
 auto tribe=find(0x803316d0,0x54,328,names[1]);if(!tribe || !*tribe || !readable(*tribe,0x54))return {};
 uint32_t model=0;memory_.readWord(*tribe+4,model);std::string modelName;if(!text(model,modelName,64) || !canEnter(i.name,model))return {};
 auto ai=find(0x80338270,0x24,57,names[2]);if(!ai || (*ai && !readable(*ai,0x24)))return {};auto territory=find(0x80416580,8,4,names[3]);if(!territory || (*territory && !readable(*territory,8)))return {};
 auto target=lookup(i.name);if(!target)return {};const bool duplicate=*target!=0;
 if(!duplicate){uint32_t registryBase=0,array=0,count=0;if(!registry(registryBase,array,count))return {};for(unsigned n=0;n<count;++n){uint32_t flags=0;memory_.readWord(array+0x340*n,flags);if(!(flags&1)){target=array+0x340*n;break;}}}
 if(!*target)return {};
 std::string poseNames[3];for(unsigned k=0;k<3;++k){uint32_t a=0;memory_.readWord(*tribe+(k==0?8:k==1?0x10:0x14),a);if(a && !text(a,poseNames[k],32))return {};}
 std::string targetModel=modelName;if(duplicate){uint32_t handle=0;memory_.readWord(*target+0x104,handle);if(handle>=256)return {};const auto* pose=owner_->poses_.poses().pose(int32_t(handle));if(!pose)return {};targetModel=pose->resource->modelName;
  for(unsigned n=0;n<4;++n){uint32_t a=0;memory_.readWord(*target+0x330+4*n,a);if(a && !owner_->span(a,0x340))return {};}
 }
 if(!poseNames[0].empty() && !owner_->poses_.animationReady(targetModel,poseNames[0]))return {};
 uint32_t unused=0;if(!memory_.readWord(0x804207b4,unused))return {};
 // Original areaDataPtr always runs, even for BTLNO_INVALID. Native admission
 // proves its complete lookup inputs before entering the source effect span.
 uint32_t area=0;bool ended=false;
 for(unsigned n=0;n<34;++n){uint32_t row=0,name=0;if(!memory_.readWord(0x803057ec+4*n,row))return {};if(!row){ended=true;break;}if(!memory_.readWord(row,name))return {};std::string actual;if(!text(name,actual,64))return {};if(actual==areaName){area=row;ended=true;break;}}
 if(!ended)return {};
 if(names[4]!=invalid){uint32_t numbers=0;if(!area || !memory_.readWord(area+12,numbers) || !numbers)return {};auto row=find(numbers,8,4096,names[4]);if(!row)return {};uint32_t value=0;if(*row && !memory_.readWord(*row+4,value))return {};const auto index=value<=0x7fffffff?int32_t(value):int32_t(int64_t(value)-0x100000000LL);if(index!=-1 && (!battle.admit || !battle.admit(battle.user,*target,index)))return {};}
 struct MapCall {FieldNpcRegistryBinding* binding;Call native;const FieldMapNpcBattleAdmission* battle;};MapCall call{this,{this,clock},&battle};const auto primitive=services(call.native);PackedMapNpcServices s;s.user=&call;s.memory=primitive.memory;
 s.compare=[](void* p,uint32_t a,uint32_t b)->int32_t{auto& c=*static_cast<MapCall*>(p);return c.binding->services(c.native).compare(&c.native,a,b);};
 s.copyString=[](void* p,uint32_t a,uint32_t b){auto& c=*static_cast<MapCall*>(p);c.binding->services(c.native).copyString(&c.native,a,b);};
 s.entry=[](void* p,uint32_t a,uint32_t b){auto& c=*static_cast<MapCall*>(p);const auto h=c.binding->enter(a,b,c.native.clock);assert(h);(void)h;};
 s.lookup=[](void* p,uint32_t a){auto& c=*static_cast<MapCall*>(p);const auto result=c.binding->lookup(a,PackedNpcLookup::Release);assert(result);return *result;};
 // Capture-free callbacks construct this same facade locally; original numeric
 // battle selection still freshly resolves its own GP/area owner.
 s.area=[](void* p,uint32_t a){auto& c=*static_cast<MapCall*>(p);std::string name;const bool ok=c.binding->text(a,name,64);assert(ok);(void)ok;BattleActorServices b;b.memory=c.binding->services(c.native).memory;b.user=&c;b.byte=[](void* p,uint32_t a){uint8_t v=0;const bool ok=static_cast<MapCall*>(p)->binding->memory_.readByte(a,v);assert(ok);(void)ok;return v;};return packedBattleAreaData(name.c_str(),b);};
 s.battle=[](void* p,uint32_t actor,int32_t index){auto& c=*static_cast<MapCall*>(p);PackedNpcBattleSetupServices b;b.memory.memory=c.binding->services(c.native).memory;b.memory.user=&c;b.memory.byte=[](void* p,uint32_t a){uint8_t v=0;const bool ok=static_cast<MapCall*>(p)->binding->memory_.readByte(a,v);assert(ok);(void)ok;return v;};b.user=c.battle->user;b.random=c.battle->random;b.savedFlag=c.battle->savedFlag;setPackedNpcBattleInformation(actor,index,b);};
 s.animation=[](void* p,int32_t h,uint32_t a,int32_t mode){auto& c=*static_cast<MapCall*>(p);std::string name;const bool ready=c.binding->text(a,name,32);assert(ready && mode==1);(void)ready;(void)mode;const bool ok=c.binding->owner_->poses_.selectName(h,name,true,c.native.clock);assert(ok);(void)ok;};
 return registerPackedMapNpc(i,s);
}
}
