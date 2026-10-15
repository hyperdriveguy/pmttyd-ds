#include "field_npc_registry_owner.hpp"
#include "event_address_space.hpp"
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
std::optional<int32_t> FieldNpcRegistryBinding::enter(uint32_t name,uint32_t model,BattlePoseClock clock){
 std::string nameBytes,modelBytes,luigi;
 uint32_t base=0,array=0,count=0,gp=0,mode=0,unused=0;
 if(!text(name,nameBytes,32) || !text(model,modelBytes,64) || !registry(base,array,count) || !activeActors())return {};
 uint32_t freeActor=0;for(unsigned i=0;i<count;++i){uint32_t flags=0;const bool ok=memory_.readWord(array+i*0x340,flags);assert(ok);(void)ok;if(!(flags&1)){freeActor=array+i*0x340;break;}}if(!freeActor)return {};
 // Source strcpy/resource requests cannot alias the slot cleared immediately
 // before them in this synchronous native admission domain.
 auto overlaps=[&](uint32_t a,size_t n){return uint64_t(a)<uint64_t(freeActor)+0x340 && uint64_t(freeActor)<uint64_t(a)+n;};
 if(overlaps(name,nameBytes.size()+1) || overlaps(model,modelBytes.size()+1))return {};
 if(!memory_.readWord(0x80415fd0,gp) || uint64_t(gp)+0x47>UINT32_MAX || !memory_.readWord(gp+0x14,mode) || !memory_.readWord(gp+0x38,unused) || !memory_.readWord(gp+0x3c,unused))return {};
 if(!text(0x802c1468,luigi,64) || !owner_->poses_.canEnter(modelBytes,mode?1:0))return {};
 const uint32_t constants[]={0x802c11f8,0x802c11fc,0x802c1200,0x802c1204,0x802c1208,0x802c120c,0x8041fc94,0x8041fc7c,0x8041fc6c};
 for(auto a:constants)if(!memory_.readWord(a,unused))return {};
 Call call{this,clock};return enterPackedNpc(name,model,services(call));
}
}
