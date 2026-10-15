#include "source_font_draw_start.hpp"
#include <cstring>
namespace ttyd {namespace {
float real(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
struct Fixed{uint32_t function,site,count;std::array<uint32_t,7>args;};
// Original {function,callsite,operand count,operands}, in source order.
// First18 GX setters precede the late color read: indirect/cull/depth/alpha/
// blend, descriptors/formats, channel and texgen. Attributes9/11/13/14 mean
// POS/CLR0/TEX0/TEX1; direct1 and VAT0 formats retain exact source tuples.
constexpr Fixed early[]{
 {0x802b8880,0x80074c20,1,{0}},{0x802b5c8c,0x80074c28,1,{0}},
 {0x802b940c,0x80074c30,1,{1}},{0x802b8d60,0x80074c48,5,{7,0,0,7,0}},
 {0x802b932c,0x80074c5c,4,{1,4,5,7}},{0x802b93d8,0x80074c6c,3,{1,7,0}},
 {0x802b4520,0x80074c70,0,{}},
 {0x802b3c08,0x80074c7c,2,{9,1}},{0x802b3c08,0x80074c88,2,{11,1}},
 {0x802b3c08,0x80074c94,2,{13,1}},{0x802b3c08,0x80074ca0,2,{14,1}},
 {0x802b4558,0x80074cb8,5,{0,9,1,3,0}},{0x802b4558,0x80074cd0,5,{0,11,1,5,0}},
 {0x802b4558,0x80074ce8,5,{0,13,1,2,0}},{0x802b4558,0x80074d00,5,{0,14,1,0,0}},
 {0x802b6f78,0x80074d20,7,{4,0,1,1,0,0,2}},{0x802b6f3c,0x80074d28,1,{1}},
 {0x802b5084,0x80074d30,1,{1}}
};
// One TEV stage/order/ops/inputs, then LoadTexObj of literal static object
// e97c into slot0. No dereference of e97c as a pointer cell or atlas-ID alias.
constexpr Fixed late[]{
 {0x802b8fcc,0x80074d4c,1,{1}},{0x802b8e30,0x80074d60,4,{0,0,0,255}},
 {0x802b89e8,0x80074d7c,6,{0,0,0,0,1,0}},{0x802b8a50,0x80074d98,6,{0,0,0,0,1,0}},
 {0x802b8960,0x80074db0,5,{0,15,2,12,15}},{0x802b89a4,0x80074dc8,5,{0,7,1,4,7}},
 {0x802b7880,0x80074dd4,2,{0x8041e97c,0}}
};
bool same(const SourceFontDrawStartServices&a,const SourceFontDrawStartServices&b){const auto&x=a.common;const auto&y=b.common;const auto&m=x.actors;const auto&n=y.actors;
 return a.camera==b.camera&&a.fog==b.fog&&x.user==y.user&&x.current==y.current&&x.lifetime.get()==y.lifetime.get()&&!x.lifetime.owner_before(y.lifetime)&&!y.lifetime.owner_before(x.lifetime)&&x.request==y.request&&x.matrix==y.matrix&&m.user==n.user&&m.byte==n.byte&&m.audienceLifecycle.user==n.audienceLifecycle.user&&m.audienceLifecycle.event==n.audienceLifecycle.event&&m.memory.user==n.memory.user&&m.memory.word==n.memory.word&&m.memory.half==n.memory.half&&m.memory.storeWord==n.memory.storeWord&&m.memory.storeHalf==n.memory.storeHalf&&m.memory.storeByte==n.memory.storeByte&&m.memory.flushBlock==n.memory.flushBlock&&m.memory.systemCall==n.memory.systemCall;
}
struct Running{bool&v;explicit Running(bool&f):v(f){v=true;}~Running(){v=false;}};
}
struct SourceFontDrawStartCursor::State {
 SourceFontDrawStartInput input;SourceFontDrawStartServices services;uint32_t phase=0,index=0,pending=0,camera=0;
 bool invalid=false;std::optional<SourceFontStateCall>held;std::optional<BattleAudienceMatrixRequest>math;std::optional<SourceFontDrawStartFog>fog;
 BattleActMatrix textureMatrix{};
 bool valid(){if(invalid)return false;const auto&s=services.common;if(!s.current||!s.current(s.user)){invalid=true;return false;}return true;}
 uint32_t word(uint32_t a){if(!valid())return 0;const auto&m=services.common.actors.memory;const auto v=m.word(m.user,a);valid();return v;}
 template<class Build>bool request(uint32_t fn,uint32_t site,Build build){pending=fn;if(!held){held=SourceFontStateCall{};held->function=fn;held->callsite=site;build(*held);}const auto&s=services.common;if(!valid()||!s.request)return false;const bool done=s.request(s.user,*held);if(!valid()||!done)return false;held.reset();return true;}
 bool fixed(const Fixed&q){return request(q.function,q.site,[&](auto&r){r.integers.assign(q.args.begin(),q.args.begin()+q.count);});}
};
SourceFontDrawStartCursor::SourceFontDrawStartCursor()=default;SourceFontDrawStartCursor::~SourceFontDrawStartCursor()=default;
bool SourceFontDrawStartCursor::active()const{return bool(state_);}bool SourceFontDrawStartCursor::stale()const{return state_&&state_->invalid;}uint32_t SourceFontDrawStartCursor::pending()const{return state_?state_->pending:0;}bool SourceFontDrawStartCursor::reset(){if(invoking_)return false;state_.reset();return true;}
// One whole33-child invocation. Pin all service and shared-owner identities;
// held request/math/fog objects keep operands stable. Missing children hold;
// current loss poisons, and successful prefix effects never replay.
bool resumeSourceFontDrawStart(const SourceFontDrawStartInput&input,const SourceFontDrawStartServices&s,SourceFontDrawStartCursor&cursor){
 const auto&common=s.common;if(cursor.invoking_||!common.current||!common.lifetime||!common.actors.memory.word)return false;
 if(cursor.state_){const auto&c=*cursor.state_;if(c.input.guestColor!=input.guestColor||c.input.privateColor!=input.privateColor||!same(c.services,s))return false;}
 Running running(cursor.invoking_);if(!cursor.state_){cursor.state_=std::make_unique<SourceFontDrawStartCursor::State>();cursor.state_->input=input;cursor.state_->services=s;}
 auto&c=*cursor.state_;
 for(;;){if(!c.valid())return false;switch(c.phase){
 case 0:{c.pending=0x8000f2e4;if(!s.camera)return false;const auto v=s.camera(common.user);if(!c.valid()||!v)return false;c.camera=*v;c.phase=1;continue;}
 case 1:
  if(c.index==std::size(early)){c.phase=2;continue;}
  if(!c.fixed(early[c.index]))return false;
  ++c.index;continue;
 // Only now read the caller color (private native word or actual guest word).
 // A change before this read is visible; a held TEV child keeps captured RGBA.
 case 2:
  if(!c.request(0x802b8ab8,0x80074d44,[&](auto&q){q.integers={1};q.color=input.privateColor?*input.privateColor:c.word(input.guestColor);}))return false;
  c.index=0;c.phase=3;continue;
 case 3:
  if(c.index==std::size(late)){c.phase=4;continue;}
  if(!c.fixed(late[c.index]))return false;
  ++c.index;continue;
 // Fresh GP+16c AFTER texture load: zero chooses Y1/1024, any nonzero
 // Y1/256; X1/512,Z1. Keep the actual SDK scale output across its hold.
 case 4:{c.pending=0x8029e2b0;if(!c.math){c.math=BattleAudienceMatrixRequest{};c.math->function=0x8029e2b0;const auto language=c.word(c.word(0x80415fd0)+0x16c);const auto x=real(c.word(0x804203ec)),y=real(c.word(language==0?0x804203f0:0x804203f4)),z=real(c.word(0x804203e4));c.math->xyz={x,y,z};}if(!c.valid()||!common.matrix)return false;const bool done=common.matrix(common.user,*c.math);if(!c.valid()||!done)return false;c.textureMatrix=c.math->output;c.math.reset();c.phase=5;continue;}
 case 5:
  if(!c.request(0x802ba3d0,0x80074e20,[&](auto&q){q.integers={30,1};q.matrix=c.textureMatrix;}))return false;
  c.phase=6;continue;
 case 6:
  if(!c.request(0x802b4e04,0x80074e3c,[](auto&q){q.integers={0,1,4,30,0,125};}))return false;
  c.phase=7;continue;
 // Original camera pointer was captured at entry; its12 view words at+11c
 // are read NOW into the retained LoadPosMtx request, not snapshotted at entry.
 case 7:
  if(!c.request(0x802ba2fc,0x80074e48,[&](auto&q){q.integers={0};BattleActMatrix view;for(unsigned i=0;i<12;++i)view[i]=real(c.word(c.camera+0x11c+4*i));q.matrix=view;}))return false;
  c.phase=8;continue;
 case 8:
  if(!c.request(0x802ba39c,0x80074e50,[](auto&q){q.integers={0};}))return false;
  c.phase=9;continue;
 // Final Fog uses fresh source positive zero203f8 and actual BSS color29578.
 // Zero BSS needs a real persistent loader/storage owner, not a per-read default.
 case 9:{c.pending=0x802b8ff4;if(!c.fog){c.fog=SourceFontDrawStartFog{};const auto zero=real(c.word(0x804203f8));const auto color=c.word(0x80429578);c.fog->reals.fill(zero);c.fog->color=color;}if(!c.valid()||!s.fog)return false;const bool done=s.fog(common.user,*c.fog);if(!c.valid()||!done)return false;cursor.state_.reset();return true;}
 default:return false;
 }}
}
bool resumeSourceFontDrawStart(const uint32_t&color,const SourceFontDrawStartServices&s,SourceFontDrawStartCursor&cursor){return resumeSourceFontDrawStart(SourceFontDrawStartInput{0,&color},s,cursor);}
}
