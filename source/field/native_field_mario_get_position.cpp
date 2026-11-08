#include "native_field_mario_get_position.hpp"
#include "field_story_position.hpp"
#include "field_party.hpp"
#include <cmath>
#include <cstring>
#include <utility>
namespace ttyd {
namespace {
constexpr uint32_t LocalZero=uint32_t(-30000000);
struct Guard{bool&value;explicit Guard(bool&v):value(v){value=true;}~Guard(){value=false;}};
float scalar(uint32_t bits){float result;std::memcpy(&result,&bits,sizeof result);return result;}
bool encodable(uint32_t bits){
 const auto value=scalar(bits),scaled=value*1024.f;
 return std::isfinite(value)&&std::isfinite(scaled)&&double(scaled)>=-2147483648.0&&double(scaled)<2147483648.0;
}
}
NativeFieldMarioGetPosition::NativeFieldMarioGetPosition(EventNativeRuntime&vm,NativeFieldMarioGetPositionBinding b):vm_(&vm),vmProof_(vm.nativeFacadeProof()),binding_(std::move(b)){}
std::unique_ptr<NativeFieldMarioGetPosition>NativeFieldMarioGetPosition::create(EventNativeRuntime&vm,NativeFieldMarioGetPositionBinding b){
 if(!b.lifetime||!b.host.current()||!b.current||!b.position)return {};
 auto owner=std::unique_ptr<NativeFieldMarioGetPosition>(new NativeFieldMarioGetPosition(vm,std::move(b)));if(!owner->current())return {};return owner;
}
bool NativeFieldMarioGetPosition::current(){
 if(stale_||checking_)return false;Guard guard(checking_);
 if(!vmProof_.current()||!binding_.host.current()){stale_=true;return false;}
 if(!binding_.current(binding_.user,*vm_)){stale_=true;return false;}return true;
}
std::optional<int32_t>NativeFieldMarioGetPosition::invoke(uint32_t fn,EventEntry&e,bool){
 if(invoking_||fn!=NativeFieldMarioGetPositionFunction||!current())return {};Guard guard(invoking_);
 // Native preflight, not a claim about original speculative loads. This exact
 // fixed-LW domain cannot alias the sampled native XYZ or original arg cells.
 if(!vm_->memory.unmappedRange(FieldMarioOwnerAddress+0x8c,3*sizeof(uint32_t)))return {};
 uint32_t address=0;if(!vm_->entryAddresses.addressOf(&e,address)||!(e.flags&1)||e.argumentCount!=4)return {};
 uint32_t actual=0;if(!vm_->memory.readWord(address+0x18,actual)||actual!=e.arguments)return {};
 const std::array<uint32_t,4>expected{0,LocalZero,LocalZero+1,LocalZero+2};
 for(unsigned i=0;i<expected.size();++i)if(!vm_->memory.readWord(e.arguments+4*i,actual)||actual!=expected[i])return {};
 for(unsigned axis=0;axis<sample_.size();++axis){
  if(!current())return {};const auto word=binding_.position(binding_.user,axis);
  if(!word||!encodable(*word)||!current())return {};sample_[axis]=*word;
 }
 if(!current())return {};
 FieldStoryPositionServices source;source.user=this;source.values=vm_->interpreterServices().operands;
 // Wrap the same actual VM operands, adding only three admitted native lanes.
 struct View{NativeFieldMarioGetPosition*owner;EventOperandServices original;};
 View view{this,source.values};source.values.user=&view;
 source.values.contextAddress=[](void*p){auto&v=*static_cast<View*>(p);return v.original.contextAddress(v.original.user);};
 source.values.entryAddress=[](void*p,EventEntry&entry){auto&v=*static_cast<View*>(p);return v.original.entryAddress(v.original.user,entry);};
 source.values.globalAddress=[](void*p){auto&v=*static_cast<View*>(p);return v.original.globalAddress(v.original.user);};
 source.values.readWord=[](void*p,uint32_t a){auto&v=*static_cast<View*>(p);for(unsigned axis=0;axis<3;++axis)if(a==FieldMarioOwnerAddress+0x8c+4*axis)return v.owner->sample_[axis];return v.original.readWord(v.original.user,a);};
 source.values.writeWord=[](void*p,uint32_t a,uint32_t w){auto&v=*static_cast<View*>(p);v.original.writeWord(v.original.user,a,w);};
 source.values.readByte=[](void*p,uint32_t a){auto&v=*static_cast<View*>(p);return v.original.readByte(v.original.user,a);};
 source.values.writeByte=[](void*p,uint32_t a,uint8_t b){auto&v=*static_cast<View*>(p);v.original.writeByte(v.original.user,a,b);};
 source.values.truncate=[](void*p,float f){auto&v=*static_cast<View*>(p);return v.original.truncate(v.original.user,f);};
 source.mario=[](void*){return uint32_t(FieldMarioOwnerAddress);};
 // Mode0 reaches no subtraction child. Fixed local destinations use actual
 // source evtSetFloat; they do not mutate the native sampled coordinates.
 const auto result=fieldMarioGetPosition(e,source);
 if(!current())return {};return result;
}
}
