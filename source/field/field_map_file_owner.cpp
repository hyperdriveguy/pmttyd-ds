#include "field_map_file_owner.hpp"
#include "event_ram_access.hpp"
#include <cstring>
#include <vector>
namespace ttyd {
namespace {
constexpr uint32_t ConvertPath=0x8029fdbc,Open=0x802a00b0,Read=0x802a03c0,Close=0x802a0178;
constexpr uint32_t Yield=0x8029ccf4,Clear=0x800050b4,Copy=0x8000519c,CopyString=0x8026d5fc;
constexpr uint32_t FileInfo=0x40,LogicalLength=0x34,State=0x90;
constexpr uint16_t Used=0x8000;
constexpr unsigned MaxPathBytes=64;
struct StagedFile {const char* name;uint32_t logical,transfer;};
// Exact US harbor FST lengths. The editable transfer files retain the original
// bytes through the next32-byte boundary, including30 actual disc-tail bytes.
constexpr std::array<StagedFile,3> Catalog{{{"d",1039618,1039648},{"t",525792,525792},{"c",2048,2048}}};
}
FieldMapFileOwner::FieldMapFileOwner(EventAddressSpace& m,uint32_t t,uint32_t s,const std::string& root):memory_(m),tableAddress_(t),stackAddress_(s),root_(root){}
std::unique_ptr<FieldMapFileOwner> FieldMapFileOwner::bind(EventAddressSpace& m,uint32_t t,uint32_t s,const std::string& root){
 if(!t || !s || (t&(CacheLine-1)) || (s&(CacheLine-1)) || root.empty() || uint64_t(t)+TableBytes>0x100000000ULL || uint64_t(s)+StackBytes>0x100000000ULL)return {};
 // Admit all complete staged transfers before unchecked source acquisition.
 for(const auto& item:Catalog){const auto name=root+"/maps/gor_00/"+item.name;FILE* f=std::fopen(name.c_str(),"rb");if(!f)return {};const bool ok=std::fseek(f,0,SEEK_END)==0 && std::ftell(f)==long(item.transfer);std::fclose(f);if(!ok)return {};}
 auto o=std::unique_ptr<FieldMapFileOwner>(new FieldMapFileOwner(m,t,s,root));
 auto map=[&](uint32_t a,uint8_t* b,uint32_t n){if(!m.registerExternalRam(a,b,n))return false;o->mappings_[o->mapped_++]=a;return true;};
 if(!map(Globals,o->globals_.data(),GlobalsBytes) || !map(t,o->table_.data(),TableBytes) || !map(SortScratch,o->sort_.data(),SortScratchBytes) || !map(SortGlobals,o->sortGlobals_.data(),SortGlobalsBytes) || !map(s,o->stack_.data(),StackBytes) || !o->current())return {};
 // Native platform bootstrap: one cold process table and enabled cooperative
 // worker. This does not claim to execute OSCreateThread/DVDMgrInit.
 if(!m.writeWord(TablePointer,t) || !m.writeWord(Globals,1))return {};
 return o;
}
FieldMapFileOwner::~FieldMapFileOwner(){
 for(auto* f:files_)if(f)std::fclose(f);
 const std::array<const uint8_t*,5> backing{{globals_.data(),table_.data(),sort_.data(),sortGlobals_.data(),stack_.data()}};
 while(mapped_){--mapped_;memory_.unregisterExternalRam(mappings_[mapped_],backing[mapped_]);}
}
bool FieldMapFileOwner::current() const{
 return mapped_==mappings_.size() && memory_.hasRamBacking(Globals,globals_.data(),GlobalsBytes) && memory_.hasRamBacking(tableAddress_,table_.data(),TableBytes) && memory_.hasRamBacking(SortScratch,sort_.data(),SortScratchBytes) && memory_.hasRamBacking(SortGlobals,sortGlobals_.data(),SortGlobalsBytes) && memory_.hasRamBacking(stackAddress_,stack_.data(),StackBytes);
}
bool FieldMapFileOwner::text(uint32_t address,std::string& out) const{
 out.clear();for(unsigned i=0;i<MaxPathBytes;++i){uint8_t b=0;if(!memory_.readByte(address+i,b))return false;if(!b)return true;out+=char(b);}return false;
}
std::optional<unsigned> FieldMapFileOwner::path(uint32_t address) const{
 std::string value;if(!text(address,value))return {};
 for(unsigned i=0;i<Catalog.size();++i)if(value==std::string("././m/gor_00/")+Catalog[i].name || value==std::string("./m/gor_00/")+Catalog[i].name)return i;
 return {};
}
std::optional<unsigned> FieldMapFileOwner::slot(uint32_t info) const{
 if(info<tableAddress_+FileInfo)return {};
 const auto delta=info-tableAddress_-FileInfo;
 if(delta%SlotBytes || delta/SlotBytes>=Slots)return {};
 return delta/SlotBytes;
}
bool FieldMapFileOwner::accepts(uint32_t f){return f==ConvertPath || f==Open || f==Read || f==Close || f==Yield || f==Clear || f==Copy || f==CopyString;}
bool FieldMapFileOwner::service(FieldMapSourceCall& q){
 if(!current())return false;
 uint32_t table=0;if(!memory_.readWord(TablePointer,table) || table!=tableAddress_)return false;
 const auto& a=q.arguments;
 switch(q.function){
 case ConvertPath:{const auto index=path(a[0]);q.result=index?uint32_t(*index):UINT32_MAX;return true;}
 case Open:{
  const auto index=path(a[0]),entry=slot(a[1]);if(!index || !entry || files_[*entry])return false;
  const uint32_t handle=tableAddress_+*entry*SlotBytes;uint16_t state=0;
  if(!memory_.readHalf(handle+State,state) || !(state&Used) || !memory_.contiguousRam(a[1],LogicalLength+4))return false;
  const auto name=root_+"/maps/gor_00/"+Catalog[*index].name;FILE* f=std::fopen(name.c_str(),"rb");if(!f){q.result=0;return true;}
  files_[*entry]=f;memory_.writeWord(a[1]+LogicalLength,Catalog[*index].logical);q.result=1;return true;
 }
 case Read:{
  const auto entry=slot(a[0]);if(!entry || !files_[*entry] || a[4]!=2 || !memory_.contiguousRam(a[1],a[2]))return false;
  const auto index=path(tableAddress_+*entry*SlotBytes);if(!index || uint64_t(a[3])+a[2]>Catalog[*index].transfer)return false;
  // Complete native transfer before source Main is allowed to publish flags.
  // Storage mappings stay stationary throughout this synchronous native IO.
  std::vector<uint8_t> bytes(a[2]);FILE* f=files_[*entry];
  if(std::fseek(f,long(a[3]),SEEK_SET)!=0 || std::fread(bytes.data(),1,bytes.size(),f)!=bytes.size()){q.result=UINT32_MAX;return true;}
  if(!memory_.withRawRamAccess([&](EventAddressSpace::RawRamAccess& raw){
   if(!raw.bind(a[1],a[2]))return false;
   for(unsigned i=0;i<bytes.size();++i)raw.writeByte(a[1]+i,bytes[i]);
   return true;
  }))return false;
  q.result=a[2];return true;
 }
 case Close:{const auto entry=slot(a[0]);if(!entry || !files_[*entry])return false;std::fclose(files_[*entry]);files_[*entry]=nullptr;q.result=1;return true;}
 case Yield:
  // Parent source Read remains at its real yield until one complete worker
  // iteration (including source sorting/completion/retirement) has returned.
  return pump();
 case Clear:case Copy:{
  if(!a[2]){q.result=a[0];return true;}
  if(!memory_.contiguousRam(a[0],a[2]) || (q.function==Copy && !memory_.contiguousRam(a[1],a[2])))return false;
  if(q.function==Clear){for(uint32_t i=0;i<a[2];++i)memory_.writeByte(a[0]+i,uint8_t(a[1]));}
  else if(!memory_.withRawRamAccess([&](EventAddressSpace::RawRamAccess& raw){
   return raw.bind(a[0],a[2]) && raw.bind(a[1],a[2]) && raw.copy(a[0],a[1],a[2]);
  }))return false; // Complete physical/virtual disjointness admission.
  q.result=a[0];return true;
 }
 case CopyString:{std::string value;if(!text(a[1],value) || !memory_.contiguousRam(a[0],uint32_t(value.size()+1)))return false;for(unsigned i=0;i<=value.size();++i)memory_.writeByte(a[0]+i,i==value.size()?0:uint8_t(value[i]));q.result=a[0];return true;}
 default:return false;
 }
}
FieldMapSourceServices FieldMapFileOwner::services(){FieldMapSourceServices s;s.user=this;
 s.word=[](void* u,uint32_t a,uint32_t& v){return static_cast<FieldMapFileOwner*>(u)->memory_.readWord(a,v);};
 s.half=[](void* u,uint32_t a,uint16_t& v){return static_cast<FieldMapFileOwner*>(u)->memory_.readHalf(a,v);};
 s.byte=[](void* u,uint32_t a,uint8_t& v){return static_cast<FieldMapFileOwner*>(u)->memory_.readByte(a,v);};
 s.store=[](void* u,uint32_t a,uint32_t v){return static_cast<FieldMapFileOwner*>(u)->memory_.writeWord(a,v);};
 s.storeHalf=[](void* u,uint32_t a,uint16_t v){return static_cast<FieldMapFileOwner*>(u)->memory_.writeHalf(a,v);};
 s.storeByte=[](void* u,uint32_t a,uint8_t v){return static_cast<FieldMapFileOwner*>(u)->memory_.writeByte(a,v);};
 s.call=[](void* u,FieldMapSourceCall& q){return static_cast<FieldMapFileOwner*>(u)->service(q);};return s;
}
bool FieldMapFileOwner::pump(unsigned visits){
 if(!current())return false;
 uint32_t active=0,table=0;
 if(!memory_.readWord(Globals,active) || active!=1 || !memory_.readWord(TablePointer,table) || table!=tableAddress_)return false;
 return resumeFieldMapSource(FieldMapSource::FileWorker,{},stackAddress_+StackBytes-CacheLine,services(),worker_,visits).has_value();
}
}
