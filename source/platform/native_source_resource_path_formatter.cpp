#include "native_source_resource_path_formatter.hpp"
#include <cstdio>
#include <climits>
#include <string>
namespace ttyd {namespace {
struct Running{bool&value;explicit Running(bool&v):value(v){value=true;}~Running(){value=false;}};
const char*expected(uint32_t site){switch(site){case 0x80075048:return "%s/f/%s.bfn";case 0x80075094:return "%s/font/%s.bfn";case 0x8007fc30:return "%s/f/msgWindow.tpl";case 0x8001b730:return "%s/icon.tpl";case 0x8001b7a0:return "%s/icon.bin";default:return nullptr;}}
}
struct NativeSourceResourcePathFormatter::State {
 NativeResourcePathKind kind=NativeResourcePathKind::Font;uint32_t site=0;std::array<uint32_t,3>args{};uint8_t count=0,input=0;
 const void*destination=nullptr;std::map<uint32_t,uint8_t>*cells=nullptr;
 std::array<std::string,3>text;std::optional<uint32_t>pending;
};
NativeSourceResourcePathFormatter::NativeSourceResourcePathFormatter()=default;
NativeSourceResourcePathFormatter::~NativeSourceResourcePathFormatter()=default;
bool NativeSourceResourcePathFormatter::current(){if(!bound_||invalid_)return false;if(!services_.current(services_.user))invalid_=true;return !invalid_;}
bool NativeSourceResourcePathFormatter::bind(NativeResourcePathFormatterServices s){
 if(bound_||invalid_||invoking_||!s.lifetime||!s.current||!s.byte||!s.immutableInput||!s.privateDestination||!s.stringBudget||s.stringBudget>uint32_t(INT_MAX)||!s.outputCapacity||s.outputCapacity>uint32_t(INT_MAX))return false;
 Running running(invoking_);services_=std::move(s);bound_=true;return current();
}
bool NativeSourceResourcePathFormatter::active()const{return bool(state_);}
bool NativeSourceResourcePathFormatter::stale()const{return invalid_;}
std::optional<uint32_t>NativeSourceResourcePathFormatter::pendingAddress()const{return state_?state_->pending:std::nullopt;}
bool NativeSourceResourcePathFormatter::reset(){if(invoking_)return false;state_.reset();return true;}
void NativeSourceResourcePathFormatter::retire(){invalid_=true;}
std::optional<uint32_t>NativeSourceResourcePathFormatter::request(const SourceFontSetupCall&q){
 if(q.function!=0x8026a0b8||!q.formatDestination||q.path||!q.reals.empty()||q.integers.size()!=3||
    !((q.callsite==0x80075048&&q.integers[0]==0x802c2ba0)||(q.callsite==0x80075094&&q.integers[0]==0x802c2bac)))return {};
 return format(NativeResourcePathKind::Font,q.callsite,{q.integers[0],q.integers[1],q.integers[2]},3,q.formatDestination,&q.formatDestination->cells);
}
std::optional<uint32_t>NativeSourceResourcePathFormatter::request(const SourceWindowTextureSetupCall&q){
 if(q.function!=0x8026a0b8||q.callsite!=0x8007fc30||q.count!=2||!q.destination||q.path||q.arguments[0]!=0x802c3094||q.arguments[2]||q.arguments[3]||q.arguments[4])return {};
 return format(NativeResourcePathKind::Window,q.callsite,{q.arguments[0],q.arguments[1],0},2,q.destination,&q.destination->cells);
}
std::optional<uint32_t>NativeSourceResourcePathFormatter::request(const SourceIconSetupCall&q){
 if(q.function!=0x8026a0b8||q.count!=2||!q.destination||q.path||q.arguments[2]||q.arguments[3]||q.arguments[4]||
    !((q.callsite==0x8001b730&&q.arguments[0]==0x802bf858)||(q.callsite==0x8001b7a0&&q.arguments[0]==0x802bf864)))return {};
 return format(NativeResourcePathKind::Icon,q.callsite,{q.arguments[0],q.arguments[1],0},2,q.destination,&q.destination->cells);
}
std::optional<uint32_t>NativeSourceResourcePathFormatter::format(NativeResourcePathKind kind,uint32_t site,const std::array<uint32_t,3>&args,uint8_t count,const void*destination,std::map<uint32_t,uint8_t>*cells){
 if(invoking_||!bound_||invalid_)return {};
 const auto lifetime=services_.lifetime;Running running(invoking_);
 if(!current())return {};
 if(state_){const auto&c=*state_;if(c.kind!=kind||c.site!=site||c.args!=args||c.count!=count||c.destination!=destination||c.cells!=cells)return {};}
 else{state_=std::make_unique<State>();state_->kind=kind;state_->site=site;state_->args=args;state_->count=count;state_->destination=destination;state_->cells=cells;}
 auto&c=*state_;
 const auto isolated=[&](){const bool ready=services_.privateDestination(services_.user,kind,destination,services_.outputCapacity);return current()&&ready;};
 if(!isolated())return {};
 while(c.input<c.count){
  const uint32_t base=c.args[c.input];if(!base)return {}; // source-null%s outside finite domain
  auto&text=c.text[c.input];bool terminated=false;
  while(text.size()<services_.stringBudget){
   const uint64_t at=uint64_t(base)+text.size();if(at>0xffffffffULL)return {};
   c.pending=uint32_t(at);const auto extent=uint32_t(text.size()+1);
   const bool owned=services_.immutableInput(services_.user,base,extent);if(!current()||!owned)return {};
   const auto value=services_.byte(services_.user,uint32_t(at));if(!current()||!value)return {};
   if(!*value){terminated=true;break;}text.push_back(char(*value));
  }
  if(!terminated)return {};
  ++c.input;c.pending.reset();
 }
 if(c.text[0]!=expected(site))return {};
 const uint64_t size=uint64_t(c.text[0].size())-2*(count-1)+c.text[1].size()+(count==3?c.text[2].size():0);
 if(size>=services_.outputCapacity||size>uint64_t(INT_MAX))return {};
 const auto inputs=[&](){for(uint8_t i=0;i<c.count;++i){const bool owned=services_.immutableInput(services_.user,c.args[i],uint32_t(c.text[i].size()+1));if(!current()||!owned)return false;}return true;};
 if(!inputs()||!isolated())return {};
 std::vector<char>buffer(size_t(size+1));
 const int written=count==3?std::snprintf(buffer.data(),buffer.size(),c.text[0].c_str(),c.text[1].c_str(),c.text[2].c_str()):std::snprintf(buffer.data(),buffer.size(),c.text[0].c_str(),c.text[1].c_str());
 if(written<0||uint64_t(written)!=size||size_t(written)>=buffer.size())return {};
 auto candidate=*cells;for(uint32_t i=0;i<=uint32_t(written);++i)candidate[i]=uint8_t(buffer[i]);
 // All allocations, native formatting and callbacks precede target mutation.
 // Pure owner proofs cannot mutate the source/destination or retire after this
 // admission. No guest SP, original writer trace or generalprintf is implied.
 if(!inputs()||!isolated())return {};
 cells->swap(candidate);const auto result=uint32_t(written);state_.reset();return result;
}
}
