#include "source_font_setup.hpp"
#include <cstring>
namespace ttyd {namespace {
int32_t sw(uint32_t n){return n<0x80000000?int32_t(n):int32_t(int64_t(n)-0x100000000LL);}
float real(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
bool same(const SourceFontSetupServices&a,const SourceFontSetupServices&b){const auto&i=a.actors;const auto&j=b.actors;const auto&m=i.memory;const auto&n=j.memory;return a.lifetime==b.lifetime&&!a.lifetime.owner_before(b.lifetime)&&!b.lifetime.owner_before(a.lifetime)&&a.user==b.user&&a.current==b.current&&a.request==b.request&&i.user==j.user&&i.byte==j.byte&&i.audienceLifecycle.user==j.audienceLifecycle.user&&i.audienceLifecycle.event==j.audienceLifecycle.event&&m.user==n.user&&m.word==n.word&&m.half==n.half&&m.storeWord==n.storeWord&&m.storeHalf==n.storeHalf&&m.storeByte==n.storeByte&&m.flushBlock==n.flushBlock&&m.systemCall==n.systemCall;}
struct Running{bool&v;explicit Running(bool&f):v(f){v=true;}~Running(){v=false;}};
}
std::optional<uint8_t> SourceFontPath::byte(uint32_t offset)const{const auto i=cells.find(offset);return i==cells.end()?std::nullopt:std::optional<uint8_t>(i->second);}
struct SourceFontSetupCursor::State {
 SourceFontSetupServices services;BattleActorServices readers;uint32_t entry=0,argument=0,selector=0,phase=0,pending=0,root=0;
 uint32_t selected=0,selectedLength=0,maximum=0,probe=0,handle=0,heap=0,font=0,block=0,index=0;
 bool invalid=false,queued=false;SourceFontPath path;SourceFontSetupCall call;
 bool valid(){if(invalid)return false;if(!services.current||!services.current(services.user)){invalid=true;return false;}return true;}
 uint32_t word(uint32_t at){if(!valid())return 0;const auto&m=services.actors.memory;auto v=m.word(m.user,at);return valid()?v:0;}
 uint16_t half(uint32_t at){if(!valid())return 0;const auto&m=services.actors.memory;auto v=m.half(m.user,at);return valid()?v:0;}
 void store(uint32_t at,uint32_t value){if(valid()){const auto&m=services.actors.memory;m.storeWord(m.user,at,value);valid();}}
 void byte(uint32_t at,uint8_t value){if(valid()){const auto&m=services.actors.memory;m.storeByte(m.user,at,value);valid();}}
 void bind(){readers=services.actors;readers.memory.user=this;readers.memory.word=[](void*p,uint32_t at){return static_cast<State*>(p)->word(at);};readers.memory.half=[](void*p,uint32_t at){return static_cast<State*>(p)->half(at);};}
 template<class F>std::optional<uint32_t> request(uint32_t function,uint32_t site,F fill){pending=function;if(!valid())return {};if(!queued){call={};call.function=function;call.callsite=site;fill(call);queued=true;}if(!valid()||!services.request)return {};const auto v=services.request(services.user,call);if(!valid()||!v)return {};queued=false;return v;}
 std::optional<uint32_t> request(uint32_t function,uint32_t site){return request(function,site,[](auto&){});}
};
SourceFontSetupCursor::SourceFontSetupCursor()=default;SourceFontSetupCursor::~SourceFontSetupCursor()=default;
bool SourceFontSetupCursor::active()const{return bool(state_);}bool SourceFontSetupCursor::stale()const{return state_&&state_->invalid;}uint32_t SourceFontSetupCursor::pending()const{return state_?state_->pending:0;}bool SourceFontSetupCursor::reset(){if(invoking_)return false;state_.reset();return true;}
bool resumeSourceFontSetupEntry(uint32_t entry,uint32_t argument,const SourceFontSetupServices&s,SourceFontSetupCursor&cursor){
 if(entry!=0x80075004&&entry!=0x80077bdc)return false;
 const auto&m=s.actors.memory;if(cursor.invoking_||!s.lifetime||!s.current||!m.word||!m.half||!m.storeWord||!m.storeByte)return false;
 if(cursor.state_){const auto&c=*cursor.state_;if(c.entry!=entry||c.argument!=argument||!same(c.services,s))return false;}
 Running running(cursor.invoking_);if(!cursor.state_){cursor.state_=std::make_unique<SourceFontSetupCursor::State>();auto&c=*cursor.state_;c.services=s;c.entry=entry;c.argument=argument;c.bind();}
 auto&c=*cursor.state_;
 for(;;){if(!c.valid())return false;switch(c.phase){
 case 0:c.selector=c.entry==0x80077bdc?uint16_t(c.word(c.word(0x80415fd0)+0x16c)):c.argument;c.phase=1;continue;
 case 1:if(c.word(0x8041e978)){cursor.state_.reset();return true;}c.phase=2;continue;
 case 2:{auto v=c.request(0x80007388,0x80075024);if(!v)return false;c.root=*v;c.phase=3;continue;}
 case 3:if(!c.request(0x8026a0b8,0x80075048,[&](auto&q){const auto name=c.word(0x80416038+((c.selector<<2)&0x3fffc));q.integers={0x802c2ba0,c.root,name};q.formatDestination=&c.path;}))return false;c.phase=4;continue;
 case 4:{auto v=c.request(0x80144e88,0x80075058,[&](auto&q){q.path=&c.path;q.integers={2,0};});if(!v)return false;c.selected=*v;c.phase=5;continue;}
 case 5:{auto v=c.request(0x80144dcc,0x80075060,[&](auto&q){q.integers={c.selected};});if(!v)return false;c.selectedLength=*v;c.maximum=*v;c.probe=0;c.phase=6;continue;}
 case 6:{auto v=c.request(0x80007388,0x8007507c);if(!v)return false;c.root=*v;c.phase=7;continue;}
 case 7:if(!c.request(0x8026a0b8,0x80075094,[&](auto&q){q.integers={0x802c2bac,c.root,c.word(0x80416038+4*c.probe)};q.formatDestination=&c.path;}))return false;c.phase=8;continue;
 case 8:{auto v=c.request(0x80144e88,0x800750a4,[&](auto&q){q.path=&c.path;q.integers={2,0};});if(!v)return false;c.handle=*v;c.phase=c.handle?9:11;continue;}
 case 9:{auto v=c.request(0x80144dcc,0x800750b0,[&](auto&q){q.integers={c.handle};});if(!v)return false;if(sw(c.maximum)<sw(*v))c.maximum=*v;c.phase=10;continue;}
 case 10:if(!c.request(0x80144dd4,0x800750c4,[&](auto&q){q.integers={c.handle};}))return false;c.phase=11;continue;
 case 11:if(++c.probe<7)c.phase=6;else c.phase=12;continue;
 case 12:{auto v=c.request(0x800300f0,0x800750e4,[&](auto&q){q.integers={0,(c.maximum+31)&0xffffffe0};});if(!v)return false;c.heap=*v;c.store(0x8041e978,c.heap);c.phase=13;continue;}
 case 13:if(!c.request(0x80144e18,0x80075100,[&](auto&q){q.integers={c.selected,c.heap,(c.selectedLength+31)&0xffffffe0,0};}))return false;c.phase=14;continue;
 case 14:if(!c.request(0x80144dd4,0x80075108,[&](auto&q){q.integers={c.selected};}))return false;c.font=c.word(0x8041e978);c.block=c.font+0x20;c.index=0;c.phase=15;continue;
 case 15:
  // Live header count is reloaded on EVERY original loop condition.
  if(c.index<c.word(c.font+0xc)){const auto tag=c.word(c.block);if(tag==0x494e4631)c.store(0x8041e9a0,c.block);else if(tag==0x474c5931)c.store(0x8041e99c,c.block);c.block+=c.word(c.block+4);++c.index;continue;}c.phase=16;continue;
 case 16:if(!c.request(0x802b72b0,0x800751b8,[&](auto&q){const auto gly=c.word(0x8041e99c);const auto width=c.half(gly+0x1a),height=c.half(gly+0x1c);q.integers={0x8041e97c,gly+0x20,width,height,0,0,0,0};}))return false;c.phase=17;continue;
 case 17:if(!c.request(0x802b7544,0x800751e0,[&](auto&q){const auto zero=real(c.word(0x804203f8));q.integers={0x8041e97c,1,1,0,0,0};q.reals={zero,zero,zero};}))return false;c.phase=18;continue;
 case 18:{const auto glyph=sourceMatrixFontMap(0x20,c.readers);const auto entry=sourceMatrixFontWidth(glyph,c.readers);c.byte(entry+1,7);c.phase=19;continue;}
 case 19:{const auto glyph=sourceMatrixFontMap(0x8140,c.readers);const auto entry=sourceMatrixFontWidth(glyph,c.readers);c.byte(entry+1,9);if(!c.valid())return false;cursor.state_.reset();return true;}
 default:return false;
 }}
}
bool resumeSourceFontSetup(uint32_t selector,const SourceFontSetupServices&s,SourceFontSetupCursor&c){return resumeSourceFontSetupEntry(0x80075004,selector,s,c);}
bool resumeSourceFontManagerSetup(const SourceFontSetupServices&s,SourceFontSetupCursor&c){return resumeSourceFontSetupEntry(0x80077bdc,0,s,c);}
}
