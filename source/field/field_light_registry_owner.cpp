#include "field_light_registry_owner.hpp"
#include "event_address_space.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <limits>
namespace ttyd {
namespace {
using namespace FieldLightRegistrySource;
constexpr uint32_t BankBytes=Capacity*RecordBytes, ScratchBytes=256, MapScratchBytes=32;
constexpr uint32_t CacheLineBytes=32, SdkZero=0x8042935c;
constexpr uint32_t WordBytes=sizeof(uint32_t), VectorBytes=3*WordBytes;
static_assert(sizeof(MapLightRecord)==RecordBytes,"Source record byte stride");
uint32_t bits(float value){uint32_t out=0;std::memcpy(&out,&value,sizeof(out));return out;}
float scalar(uint32_t value){float out=0;std::memcpy(&out,&value,sizeof(out));return out;}
int32_t signedWord(uint32_t value){return value<=INT32_MAX?int32_t(value):int32_t(int64_t(value)-0x100000000LL);}
bool overlaps(uint32_t a,uint32_t n,uint32_t b,uint32_t m){return uint64_t(a)<uint64_t(b)+m && uint64_t(b)<uint64_t(a)+n;}
}
FieldLightRegistryOwner::FieldLightRegistryOwner(EventAddressSpace& memory,uint32_t base,const FieldLightHeapServices& heap):memory_(memory),allocationBase_(base),heap_(heap){}
uint32_t FieldLightRegistryOwner::bankAddress(unsigned group) const{return allocationBase_+group*BankBytes;}
uint32_t FieldLightRegistryOwner::scratchAddress() const{return allocationBase_+2*BankBytes;}
std::unique_ptr<FieldLightRegistryOwner> FieldLightRegistryOwner::bind(EventAddressSpace& memory,uint32_t base,const FieldLightHeapServices& heap){
 if(!base || !heap.flush || (base&(CacheLineBytes-1)) || uint64_t(base)+2*BankBytes+ScratchBytes+MapScratchBytes>0x100000000ULL)return {};
 auto owner=std::unique_ptr<FieldLightRegistryOwner>(new FieldLightRegistryOwner(memory,base,heap));
 auto map=[&](uint32_t address,uint8_t* bytes,uint32_t size){
  if(!memory.registerExternalRam(address,bytes,size))return false;
  owner->mappings_[owner->mapped_++]=address;return true;
 };
 if(!map(Work,owner->descriptors_.data(),uint32_t(owner->descriptors_.size())) ||
    !map(Presets,reinterpret_cast<uint8_t*>(owner->presets_.data()),uint32_t(sizeof(owner->presets_))) ||
    !map(base,reinterpret_cast<uint8_t*>(owner->banks_[0].data()),BankBytes) ||
    !map(base+BankBytes,reinterpret_cast<uint8_t*>(owner->banks_[1].data()),BankBytes) ||
    !map(owner->scratchAddress(),owner->scratch_.data(),uint32_t(owner->scratch_.size())))return {};
 return owner;
}
FieldLightRegistryOwner::~FieldLightRegistryOwner(){
 const std::array<const uint8_t*,5> backing{{descriptors_.data(),reinterpret_cast<const uint8_t*>(presets_.data()),reinterpret_cast<const uint8_t*>(banks_[0].data()),reinterpret_cast<const uint8_t*>(banks_[1].data()),scratch_.data()}};
 while(mapped_){--mapped_;memory_.unregisterExternalRam(mappings_[mapped_],backing[mapped_]);}
}
bool FieldLightRegistryOwner::current() const{
 if(mapped_!=mappings_.size())return false;
 return memory_.withRawRamAccess([&](EventAddressSpace::RawRamAccess& raw){
  return raw.aliases(Work,descriptors_.data(),uint32_t(descriptors_.size())) &&
   raw.aliases(Presets,reinterpret_cast<const uint8_t*>(presets_.data()),uint32_t(sizeof(presets_))) &&
   raw.aliases(bankAddress(0),reinterpret_cast<const uint8_t*>(banks_[0].data()),BankBytes) &&
   raw.aliases(bankAddress(1),reinterpret_cast<const uint8_t*>(banks_[1].data()),BankBytes) &&
   raw.aliases(scratchAddress(),scratch_.data(),uint32_t(scratch_.size()));
 });
}
bool FieldLightRegistryOwner::owns(uint32_t address,uint32_t size) const{
 for(unsigned group=0;group<2;++group)if(address>=bankAddress(group) && uint64_t(address)+size<=uint64_t(bankAddress(group))+BankBytes)return true;
 return address>=Presets && uint64_t(address)+size<=uint64_t(Presets)+sizeof(presets_);
}
bool FieldLightRegistryOwner::readString(uint32_t address,char* output,uint32_t capacity,uint32_t& length) const{
 for(length=0;length<capacity;++length){uint8_t byte=0;if(uint64_t(address)+length>UINT32_MAX || !memory_.readByte(address+length,byte))return false;output[length]=char(byte);if(!byte)return true;}
 return false;
}
bool FieldLightRegistryOwner::selected(unsigned& group) const{
 uint32_t active=0,count=0,array=0;
 if(!memory_.readWord(ActiveGroup,active))return false;
 group=active==1?1:0;
 return memory_.readWord(Work+DescriptorBytes*group,count) && count==Capacity &&
  memory_.readWord(Work+DescriptorBytes*group+WordBytes,array) && array==bankAddress(group);
}
FieldLightRegistryServices FieldLightRegistryOwner::services(){
 FieldLightRegistryServices s;s.user=this;
 s.group=[](void* user,int32_t& out){auto& o=*static_cast<FieldLightRegistryOwner*>(user);uint32_t value=0;if(!o.memory_.readWord(ActiveGroup,value))return false;out=signedWord(value);return true;};
 s.word=[](void* user,uint32_t address,uint32_t& out){return static_cast<FieldLightRegistryOwner*>(user)->memory_.readWord(address,out);};
 s.half=[](void* user,uint32_t address,uint16_t& out){return static_cast<FieldLightRegistryOwner*>(user)->memory_.readHalf(address,out);};
 s.store=[](void* user,uint32_t address,uint32_t value){return static_cast<FieldLightRegistryOwner*>(user)->memory_.writeWord(address,value);};
 s.storeHalf=[](void* user,uint32_t address,uint16_t value){return static_cast<FieldLightRegistryOwner*>(user)->memory_.writeHalf(address,value);};
 s.allocate=[](void* user,uint32_t heap,uint32_t size,uint32_t& out){
  auto& o=*static_cast<FieldLightRegistryOwner*>(user);
  if(heap || size!=BankBytes || o.allocations_>=o.banks_.size())return false;
  const auto index=o.allocations_;auto* bytes=reinterpret_cast<uint8_t*>(o.banks_[index].data());
  if(!o.zeroed_[index]){std::memset(bytes,0,BankBytes);o.zeroed_[index]=true;}
  if(!o.heap_.flush(o.heap_.user,bytes,size) || !o.current())return false;
  out=o.bankAddress(index);++o.allocations_;return true;
 };
 s.clear=[](void* user,uint32_t address,uint32_t size){
  auto& o=*static_cast<FieldLightRegistryOwner*>(user);
  if(!o.owns(address,size) || !o.current())return false;
  for(unsigned group=0;group<o.banks_.size();++group){
   if(address>=o.bankAddress(group) && uint64_t(address)+size<=uint64_t(o.bankAddress(group))+BankBytes){
    auto* bytes=reinterpret_cast<uint8_t*>(o.banks_[group].data());
    std::memset(bytes+address-o.bankAddress(group),0,size);return true;
   }
  }
  auto* bytes=reinterpret_cast<uint8_t*>(o.presets_.data());
  std::memset(bytes+address-Presets,0,size);return true;
 };
 s.copy=[](void* user,uint32_t destination,uint32_t source){
  auto& o=*static_cast<FieldLightRegistryOwner*>(user);std::array<char,NameBytes> name{};uint32_t length=0;
  if(!o.owns(destination,NameBytes) || !o.readString(source,name.data(),uint32_t(name.size()),length))return false;
  if(source!=destination && overlaps(source,length+1,destination,length+1))return false;
  for(uint32_t index=0;index<=length;++index)if(!o.memory_.writeByte(destination+index,uint8_t(name[index])))return false;
  return true;
 };
 s.compare=[](void* user,uint32_t left,uint32_t right,int32_t& out){
  auto& o=*static_cast<FieldLightRegistryOwner*>(user);std::array<char,ScratchBytes> a{},b{};uint32_t na=0,nb=0;
  if(!o.readString(left,a.data(),uint32_t(a.size()),na) || !o.readString(right,b.data(),uint32_t(b.size()),nb))return false;
  out=std::strcmp(a.data(),b.data());return true;
 };
 s.format=[](void* user,uint32_t destination,uint32_t format,uint32_t name,int32_t suffix){
  auto& o=*static_cast<FieldLightRegistryOwner*>(user);constexpr char ExpectedFormat[]="%s_%04d";
  std::array<char,ScratchBytes> prefix{},result{};std::array<char,sizeof(ExpectedFormat)> pattern{};uint32_t size=0,length=0;
  if(destination!=o.scratchAddress() || !o.readString(format,pattern.data(),uint32_t(pattern.size()),size) ||
     std::strcmp(pattern.data(),ExpectedFormat) || !o.readString(name,prefix.data(),uint32_t(prefix.size()),length))return false;
  // Capture the stable prefix before writing. Original formatting copies an
  // identical in-place prefix; overlapping snprintf input is never used here.
  const int count=std::snprintf(result.data(),result.size(),ExpectedFormat,prefix.data(),int(suffix));
  if(count<0 || unsigned(count)>=result.size())return false;
  for(int index=0;index<=count;++index)if(!o.memory_.writeByte(destination+uint32_t(index),uint8_t(result[unsigned(index)])))return false;
  return true;
 };
 s.scale=[](void* user,uint32_t scale,std::array<uint32_t,12>& matrix){
  auto& o=*static_cast<FieldLightRegistryOwner*>(user);uint32_t zero=0;
  if(!o.memory_.readWord(SdkZero,zero) || !std::isfinite(scalar(scale)) || !std::isfinite(scalar(zero)))return false;
  matrix.fill(zero);for(unsigned axis=0;axis<3;++axis)matrix[5*axis]=scale;return true;
 };
 s.multiply=[](void* user,const std::array<uint32_t,12>& matrix,uint32_t source,uint32_t destination){
  auto& o=*static_cast<FieldLightRegistryOwner*>(user);std::array<float,3> input{},output{};
  if(!o.owns(destination,VectorBytes) || !o.memory_.contiguousRam(destination,VectorBytes))return false;
  for(unsigned axis=0;axis<input.size();++axis){uint32_t value=0;if(!o.memory_.readWord(source+WordBytes*axis,value) || !std::isfinite(scalar(value)))return false;input[axis]=scalar(value);}
  for(unsigned row=0;row<output.size();++row){
   const auto offset=4*row;
   for(unsigned col=0;col<4;++col)if(!std::isfinite(scalar(matrix[offset+col])))return false;
   const float xy0=scalar(matrix[offset])*input[0],xy1=scalar(matrix[offset+1])*input[1];
   const float xz=std::fma(scalar(matrix[offset+2]),input[2],xy0);
   const float yt=std::fma(scalar(matrix[offset+3]),1.0f,xy1);
   output[row]=xz+yt;if(!std::isfinite(output[row]))return false;
  }
  for(unsigned axis=0;axis<output.size();++axis)if(!o.memory_.writeWord(destination+WordBytes*axis,bits(output[axis])))return false;
  return true;
 };
 return s;
}
bool FieldLightRegistryOwner::admitRequest(FieldLightRequest& request){
 if(activeRequest_ && activeRequest_!=&request)return false;
 activeRequest_=&request;return true;
}
void FieldLightRegistryOwner::releaseRequest(){if(!pending())activeRequest_=nullptr;}
bool FieldLightRegistryOwner::initialize(FieldLightRequest& request){
 if(!current())return false;
 if(ready_)return !pending();
 if(!admitRequest(request))return false;
 auto s=services();const bool complete=initializeFieldLights(s,cursor_);
 if(complete)ready_=true;
 releaseRequest();return complete;
}
bool FieldLightRegistryOwner::reset(FieldLightRequest& request){
 if(!current() || !ready_ || (pending() && cursor_.operation!=FieldLightOperation::Reset))return false;
 if(!pending()){unsigned group=0;if(!selected(group))return false;}
 if(!admitRequest(request))return false;
 auto s=services();const bool complete=resetFieldLights(s,cursor_);releaseRequest();return complete;
}
bool FieldLightRegistryOwner::admitEntry(const FieldLightEntryArguments& a) const{
 unsigned group=0;if(!selected(group) || a.attenuation>=4 || !memory_.contiguousRam(a.position,VectorBytes) || !memory_.contiguousRam(a.vector,VectorBytes) || !memory_.contiguousRam(a.color,WordBytes))return false;
 // Typed map-producer domain: stable input spans outside the process light
 // records. Complete alias/malformed-capacity source behavior stays in core.
 if(owns(a.position,VectorBytes) || owns(a.vector,VectorBytes) || owns(a.color,WordBytes))return false;
 std::array<char,ScratchBytes> candidate{};uint32_t length=0;
 if(!readString(a.name,candidate.data(),NameBytes,length))return false;
 unsigned free=Capacity;std::array<std::array<char,NameBytes>,Capacity> names{};
 for(unsigned index=0;index<Capacity;++index){const auto& record=banks_[group][index];if(!(record.flags()&Active)){if(free==Capacity)free=index;continue;}
  uint32_t size=0;if(!readString(bankAddress(group)+index*RecordBytes+Name,names[index].data(),NameBytes,size))return false;
 }
 if(free==Capacity)return false;
 for(uint32_t suffix=0;;++suffix){
  bool duplicate=false;for(unsigned index=0;index<Capacity;++index)if((banks_[group][index].flags()&Active) && !std::strcmp(names[index].data(),candidate.data())){duplicate=true;break;}
  if(!duplicate)return true;
  std::array<char,ScratchBytes> result{};const int count=std::snprintf(result.data(),result.size(),"%s_%04d",candidate.data(),int(suffix));
  if(count<0 || unsigned(count)>=NameBytes)return false;
  candidate=result;
 }
}
std::optional<int32_t> FieldLightRegistryOwner::enter(FieldLightRequest& request,FieldLightEntryArguments arguments){
 if(!admitRequest(request))return {};
 auto result=enterImpl(arguments,false);releaseRequest();return result;
}
std::optional<int32_t> FieldLightRegistryOwner::enterImpl(FieldLightEntryArguments arguments,bool mapChild){
 const bool retained=cursor_.operation!=FieldLightOperation::Idle;
 if(!current() || !ready_ || (mapCursor_.active && !mapChild) || (retained && cursor_.operation!=FieldLightOperation::Enter))return {};
 if(!retained && !admitEntry(arguments))return {};
 arguments.scratch=scratchAddress();auto s=services();return enterFieldLight(arguments,s,cursor_);
}
bool FieldLightRegistryOwner::loadMapLights(FieldLightRequest& request,uint32_t mapEntry){
 if(!current() || !ready_ || (!mapCursor_.active && cursor_.operation!=FieldLightOperation::Idle))return false;
 if(!admitRequest(request))return false;
 const auto registry=services();FieldMapLightServices s;
 s.user=this;s.word=registry.word;s.store=registry.store;s.compare=registry.compare;
 s.scale=[](void* user,uint32_t source,uint32_t scale,uint32_t destination){
  auto& o=*static_cast<FieldLightRegistryOwner*>(user);const float factor=scalar(scale);std::array<uint32_t,3> result{};
  if(destination!=o.scratchAddress()+ScratchBytes || !std::isfinite(factor))return false;
  for(unsigned axis=0;axis<result.size();++axis){uint32_t input=0;if(!o.memory_.readWord(source+WordBytes*axis,input) || !std::isfinite(scalar(input)))return false;const float value=scalar(input)*factor;if(!std::isfinite(value))return false;result[axis]=bits(value);}
  for(unsigned axis=0;axis<result.size();++axis)if(!o.memory_.writeWord(destination+WordBytes*axis,result[axis]))return false;
  return true;
 };
 s.entry=[](void* user,const FieldLightEntryArguments& args,int32_t& result){auto& o=*static_cast<FieldLightRegistryOwner*>(user);auto value=o.enterImpl(args,true);if(!value)return false;result=*value;return true;};
 const auto temporary=scratchAddress()+ScratchBytes;
 const bool complete=loadFieldMapLights(mapEntry,temporary,temporary+VectorBytes,scratchAddress(),s,mapCursor_);releaseRequest();return complete;
}
std::optional<uint32_t> FieldLightRegistryOwner::find(FieldLightRequest& request,uint32_t name){
 if(!current() || !ready_ || mapCursor_.active || (pending() && cursor_.operation!=FieldLightOperation::Find))return {};
 if(!pending()){unsigned group=0;if(!selected(group))return {};}
 if(!admitRequest(request))return {};
 auto s=services();auto result=findFieldLight(name,s,cursor_);releaseRequest();return result;
}
const MapLightRecord* FieldLightRegistryOwner::record(uint32_t address) const{
 if(!current() || !ready_ || pending())return nullptr;
 for(unsigned group=0;group<2;++group)if(address>=bankAddress(group) && address<bankAddress(group)+BankBytes && (address-bankAddress(group))%RecordBytes==0)return &banks_[group][(address-bankAddress(group))/RecordBytes];
 return nullptr;
}
const MapLightRecord* FieldLightRegistryOwner::preset(unsigned index) const{return current() && ready_ && !pending() && index<presets_.size()?&presets_[index]:nullptr;}
bool FieldLightRegistryOwner::query(MapLightQuery& query,uint32_t& pendingFunction) const{
 pendingFunction=query.function;unsigned group=0;
 if(!current() || !ready_ || pending() || !selected(group))return false;
 return mapLightBankQuery(MapLightBankView{true,int32_t(Capacity),banks_[group].data()},query,pendingFunction);
}
}
