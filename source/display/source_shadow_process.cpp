#include "source_shadow_process.hpp"
namespace ttyd {
namespace {struct Invocation {bool& flag;explicit Invocation(bool& f):flag(f){flag=true;}~Invocation(){flag=false;}};}
SourceShadowProcess::SourceShadowProcess(EventAddressSpace& m,uint32_t s,const SourceShadowProcessBackend& b):memory_(m),stackAddress_(s),backend_(b){}
std::unique_ptr<SourceShadowProcess> SourceShadowProcess::bind(EventAddressSpace& m,uint32_t stack,const SourceShadowProcessBackend& b){
 if(!stack || (stack&(StackGuardBytes-1)) || uint64_t(stack)+StackBytes>0x100000000ULL || !b.current || !b.call)return {};
 auto o=std::unique_ptr<SourceShadowProcess>(new SourceShadowProcess(m,stack,b));
 auto map=[&](uint32_t a,uint8_t* p,uint32_t n){if(!m.registerExternalRam(a,p,n))return false;o->mappings_[o->mapped_++]=a;return true;};
 if(!map(Primary,o->primary_.data(),PrimaryBytes) || !map(ProjectionWork,o->projection_.data(),AuxiliaryBytes) || !map(DepthWork,o->depth_.data(),AuxiliaryBytes) || !map(RampObjects,o->ramps_.data(),RampObjectBytes) || !map(stack,o->stack_.data(),StackBytes) || !o->current())return {};
 return o;
}
SourceShadowProcess::~SourceShadowProcess(){const std::array<const uint8_t*,5> p{{primary_.data(),projection_.data(),depth_.data(),ramps_.data(),stack_.data()}};while(mapped_){--mapped_;memory_.unregisterExternalRam(mappings_[mapped_],p[mapped_]);}}
bool SourceShadowProcess::current()const{
 if(mapped_!=mappings_.size() || !memory_.hasRamBacking(Primary,primary_.data(),PrimaryBytes) || !memory_.hasRamBacking(ProjectionWork,projection_.data(),AuxiliaryBytes) || !memory_.hasRamBacking(DepthWork,depth_.data(),AuxiliaryBytes) || !memory_.hasRamBacking(RampObjects,ramps_.data(),RampObjectBytes) || !memory_.hasRamBacking(stackAddress_,stack_.data(),StackBytes))return false;
 uint32_t a=0,b=0,c=0;
 return memory_.readWord(PrimaryPointer,a) && a==Primary && memory_.readWord(ProjectionPointer,b) && b==ProjectionWork && memory_.readWord(DepthPointer,c) && c==DepthWork && backend_.current(backend_.user);
}
FieldMapSourceServices SourceShadowProcess::services(){FieldMapSourceServices s;s.user=this;
 s.word=[](void* u,uint32_t a,uint32_t& v){return static_cast<SourceShadowProcess*>(u)->memory_.readWord(a,v);};
 s.byte=[](void* u,uint32_t a,uint8_t& v){return static_cast<SourceShadowProcess*>(u)->memory_.readByte(a,v);};
 s.half=[](void* u,uint32_t a,uint16_t& v){return static_cast<SourceShadowProcess*>(u)->memory_.readHalf(a,v);};
 s.store=[](void* u,uint32_t a,uint32_t v){return static_cast<SourceShadowProcess*>(u)->memory_.writeWord(a,v);};
 s.storeByte=[](void* u,uint32_t a,uint8_t v){return static_cast<SourceShadowProcess*>(u)->memory_.writeByte(a,v);};
 s.storeHalf=[](void* u,uint32_t a,uint16_t v){return static_cast<SourceShadowProcess*>(u)->memory_.writeHalf(a,v);};
 s.call=[](void* u,FieldMapSourceCall& q){return static_cast<SourceShadowProcess*>(u)->child(q);};return s;
}
bool SourceShadowProcess::child(FieldMapSourceCall& q){if(!current())return false;const bool done=backend_.call(backend_.user,q);return current() && done;}
bool SourceShadowProcess::initialize(unsigned visits){
 if(invoking_ || !current())return false;
 if(initialized_)return true;
 Invocation guard(invoking_);
 const auto result=resumeFieldMapSource(Init,{},stackAddress_+StackBytes-StackGuardBytes,services(),initializing_,visits);
 if(!result || !current())return false;
 initialized_=true;return true;
}
bool SourceShadowProcess::service(FieldMapSourceCall& call,unsigned visits){
 if(invoking_ || !ready() || !accepts(call.function))return false;
 if(call_){if(call_!=&call || call.function!=operands_.function || call.arguments!=operands_.arguments || call.scalars!=operands_.scalars)return false;}
 else{call_=&call;operands_=call;}
 Invocation guard(invoking_);
 std::array<uint32_t,5> args{};for(unsigned n=0;n<args.size();++n)args[n]=operands_.arguments[n];
 const auto result=resumeFieldMapSource(call.function,args,stackAddress_+StackBytes-StackGuardBytes,services(),query_,visits);
 if(!result || !current())return false;
 call.result=*result;call_=nullptr;return true;
}
}
