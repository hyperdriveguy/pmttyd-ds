#include "source_smart_arena.hpp"
namespace ttyd {
namespace {struct Invocation {bool& flag;explicit Invocation(bool& f):flag(f){flag=true;}~Invocation(){flag=false;}};}
SourceSmartArena::SourceSmartArena(EventAddressSpace& m,uint32_t s,const SourceSmartArenaBackend& b):memory_(m),stackAddress_(s),backend_(b){}
std::unique_ptr<SourceSmartArena> SourceSmartArena::bind(EventAddressSpace& m,uint32_t stack,const SourceSmartArenaBackend& b,uint8_t* flag){
 if(!stack || (stack&(StackGuardBytes-1)) || uint64_t(stack)+StackBytes>0x100000000ULL || !b.current || !b.call)return {};
 auto o=std::unique_ptr<SourceSmartArena>(new SourceSmartArena(m,stack,b));
 o->borrowedFlag_=flag!=nullptr;if(flag)o->flagBacking_=flag;
 auto map=[&](uint32_t a,uint8_t* p,uint32_t n){if(!m.registerExternalRam(a,p,n))return false;o->mappedBacking_[o->mapped_]=p;o->mappings_[o->mapped_++]=a;return true;};
 if(!map(Work,o->work_.data(),WorkBytes) || !(flag?m.hasRamBacking(FirstAllocation,flag,4):map(FirstAllocation,o->first_.data(),4)) || !map(stack,o->stack_.data(),StackBytes) || !o->current())return {};
 return o;
}
SourceSmartArena::~SourceSmartArena(){while(mapped_){--mapped_;memory_.unregisterExternalRam(mappings_[mapped_],mappedBacking_[mapped_]);}}
bool SourceSmartArena::current()const{
 if(mapped_!=(borrowedFlag_?2u:3u) || !memory_.hasRamBacking(Work,work_.data(),WorkBytes) || !memory_.hasRamBacking(FirstAllocation,flagBacking_,4) || !memory_.hasRamBacking(stackAddress_,stack_.data(),StackBytes))return false;
 uint32_t pointer=0;return memory_.readWord(Pointer,pointer) && pointer==Work && backend_.current(backend_.user);
}
FieldMapSourceServices SourceSmartArena::services(){FieldMapSourceServices s;s.user=this;
 s.word=[](void* u,uint32_t a,uint32_t& v){return static_cast<SourceSmartArena*>(u)->memory_.readWord(a,v);};
 s.byte=[](void* u,uint32_t a,uint8_t& v){return static_cast<SourceSmartArena*>(u)->memory_.readByte(a,v);};
 s.half=[](void* u,uint32_t a,uint16_t& v){return static_cast<SourceSmartArena*>(u)->memory_.readHalf(a,v);};
 s.store=[](void* u,uint32_t a,uint32_t v){return static_cast<SourceSmartArena*>(u)->memory_.writeWord(a,v);};
 s.storeByte=[](void* u,uint32_t a,uint8_t v){return static_cast<SourceSmartArena*>(u)->memory_.writeByte(a,v);};
 s.storeHalf=[](void* u,uint32_t a,uint16_t v){return static_cast<SourceSmartArena*>(u)->memory_.writeHalf(a,v);};
 s.call=[](void* u,FieldMapSourceCall& q){return static_cast<SourceSmartArena*>(u)->child(q);};return s;
}
bool SourceSmartArena::child(FieldMapSourceCall& q){if(!current())return false;const bool done=backend_.call(backend_.user,q);return current() && done;}
bool SourceSmartArena::initialize(unsigned visits){
 if(invoking_ || !current())return false;
 if(initialized_)return true;
 Invocation guard(invoking_);const auto result=resumeFieldMapSource(Init,{},stackAddress_+StackBytes-StackGuardBytes,services(),cursor_,visits);
 if(!result || !current())return false;
 initialized_=true;return true;
}
bool SourceSmartArena::service(FieldMapSourceCall& call,unsigned visits){
 if(invoking_ || !ready() || !accepts(call.function))return false;
 if(call_){if(call_!=&call || call.function!=operands_.function || call.arguments!=operands_.arguments || call.scalars!=operands_.scalars)return false;}
 else{call_=&call;operands_=call;}
 Invocation guard(invoking_);std::array<uint32_t,5> args{};for(unsigned n=0;n<args.size();++n)args[n]=operands_.arguments[n];
 const auto result=resumeFieldMapSource(call.function,args,stackAddress_+StackBytes-StackGuardBytes,services(),cursor_,visits);
 if(!result || !current())return false;
 call.result=*result;call_=nullptr;return true;
}
}
