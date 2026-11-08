#include "field_camera_road_owner.hpp"
namespace ttyd {
FieldCameraRoadOwner::FieldCameraRoadOwner(EventAddressSpace& m,uint32_t s,const FieldCameraEntryBorrow& c):memory_(m),stackAddress_(s),camera_(c){}
std::unique_ptr<FieldCameraRoadOwner> FieldCameraRoadOwner::bind(EventAddressSpace& m,uint32_t stack,const FieldCameraEntryBorrow& camera){
 if(!stack || (stack&(StackGuardBytes-1)) || uint64_t(stack)+StackBytes>0x100000000ULL)return {};
 auto o=std::unique_ptr<FieldCameraRoadOwner>(new FieldCameraRoadOwner(m,stack,camera));
 auto map=[&](uint32_t address,uint8_t* backing,uint32_t bytes){if(!m.registerExternalRam(address,backing,bytes))return false;o->mappings_[o->mapped_++]=address;return true;};
 if(!map(RoadWork,o->road_.data(),RoadBytes) || !map(ShiftWork,o->shift_.data(),ShiftBytes) || !map(stack,o->stack_.data(),StackBytes) || !o->current())return {};
 return o;
}
FieldCameraRoadOwner::~FieldCameraRoadOwner(){
 const std::array<const uint8_t*,3> backing{{road_.data(),shift_.data(),stack_.data()}};
 while(mapped_){--mapped_;memory_.unregisterExternalRam(mappings_[mapped_],backing[mapped_]);}
}
bool FieldCameraRoadOwner::current() const{
 if(mapped_!=mappings_.size() || !memory_.hasRamBacking(RoadWork,road_.data(),RoadBytes) || !memory_.hasRamBacking(ShiftWork,shift_.data(),ShiftBytes) || !memory_.hasRamBacking(stackAddress_,stack_.data(),StackBytes) || !memory_.hasRamBacking(CameraSlot,camera_.slotBacking,sizeof(uint32_t)) || !memory_.hasRamBacking(camera_.entryAddress,camera_.entryBacking,CameraBytes))return false;
 uint32_t road=0,shift=0,entry=0;
 return memory_.readWord(RoadPointer,road) && road==RoadWork && memory_.readWord(ShiftPointer,shift) && shift==ShiftWork && memory_.readWord(CameraSlot,entry) && entry==camera_.entryAddress;
}
bool FieldCameraRoadOwner::service(FieldMapSourceCall& call,const FieldMapSourceServices& services,unsigned visits){
 if(!accepts(call.function) || !current())return false;
 if(call_){if(call_!=&call || call.function!=operands_.function || call.arguments!=operands_.arguments || call.scalars!=operands_.scalars)return false;}
 else{if(call.arguments[0]!=FieldCamera)return false;call_=&call;operands_=call;}
 std::array<uint32_t,5> args{};for(unsigned n=0;n<args.size();++n)args[n]=operands_.arguments[n];
 const auto result=resumeFieldMapSource(call.function,args,stackAddress_+StackBytes-StackGuardBytes,services,cursor_,visits);
 if(!result)return false;
 call.result=*result;call_=nullptr;return true;
}
}
