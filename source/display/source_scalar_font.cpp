#include "source_scalar_font.hpp"
#include <cstring>
namespace ttyd {namespace {
uint32_t bits(float f){uint32_t n;std::memcpy(&n,&f,4);return n;}
float real(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
float sub(float a,float b){volatile float f=a-b;return f;}
float mul(float a,float b){volatile float f=a*b;return f;}
bool ownerSame(const std::shared_ptr<const void>&a,const std::shared_ptr<const void>&b){return a==b&&!a.owner_before(b)&&!b.owner_before(a);}
bool same(const SourceScalarFontServices&a,const SourceScalarFontServices&b){
 const auto&s=a.common;const auto&t=b.common;const auto&i=s.actors;const auto&j=t.actors;const auto&m=i.memory;const auto&n=j.memory;
 return ownerSame(s.lifetime,t.lifetime)&&s.user==t.user&&s.current==t.current&&s.camera==t.camera&&s.matrix==t.matrix&&s.loadMatrix==t.loadMatrix&&s.tevColor==t.tevColor&&s.glyph==t.glyph&&s.truncate==t.truncate&&s.nullByte==t.nullByte&&s.nullHalf==t.nullHalf&&a.fma==b.fma&&
 i.user==j.user&&i.byte==j.byte&&i.audienceLifecycle.user==j.audienceLifecycle.user&&i.audienceLifecycle.event==j.audienceLifecycle.event&&m.user==n.user&&m.word==n.word&&m.half==n.half&&m.storeWord==n.storeWord&&m.storeHalf==n.storeHalf&&m.storeByte==n.storeByte&&m.flushBlock==n.flushBlock&&m.systemCall==n.systemCall;
}
float converted(uint32_t hi,uint32_t lo,uint32_t integer,bool sign){uint64_t raw=uint64_t(hi)<<32|lo,encoded=0x4330000000000000ULL|(integer^(sign?0x80000000u:0u));double bias,value;std::memcpy(&bias,&raw,8);std::memcpy(&value,&encoded,8);volatile float out=value-bias;return out;}
struct Running{bool&v;explicit Running(bool&f):v(f){v=true;}~Running(){v=false;}};
}
struct SourceScalarFontCursor::State {
 SourceScalarFontServices services;BattleActorServices actorView;SourceMatrixFontText text,view;const SourceMatrixFontText*borrowed=nullptr;
 uint32_t xBits=0,yBits=0,phase=0,pending=0,camera=0,offset=0,code=0,width=0,savedColor=0,color=0,commandStart=0;
 uint16_t glyph=0xffff;bool invalid=false,null=false;
 std::array<float,3> position{};float pitch=0,pen=0,translation=0,a=0,b=0,c=0;
 BattleActMatrix scale{},transformed{};std::optional<BattleAudienceMatrixRequest> matrix;
 std::optional<SourceMatrixFontGlyph> glyphRequest;std::optional<uint32_t> colorRequest;
 SourceMatrixFontCommandCursor command;
 std::array<std::optional<uint8_t>,128> names{},arguments{};
 bool valid(){const auto&s=services.common;if(invalid)return false;if(!s.current||!s.current(s.user)){invalid=true;return false;}return true;}
 uint32_t word(uint32_t address){if(!valid())return 0;const auto&m=services.common.actors.memory;auto v=m.word(m.user,address);return valid()?v:0;}
 uint8_t byte(uint32_t address){if(!valid())return 0;const auto&s=services.common.actors;auto v=s.byte(s.user,address);return valid()?v:0;}
 void store(uint32_t address,uint32_t value){if(valid()){const auto&m=services.common.actors.memory;m.storeWord(m.user,address,value);valid();}}
 float constant(int32_t offset){return real(word(0x80427440u+uint32_t(offset)));}
 std::optional<uint8_t> read(uint32_t at){if(!valid())return {};const auto&s=services.common;const auto v=null?(s.nullByte?s.nullByte(s.user,at):std::nullopt):(text.byte?text.byte(text.user,at):std::nullopt);return valid()?v:std::nullopt;}
 std::optional<uint16_t> half(uint32_t at){if(!valid())return {};const auto&s=services.common;const auto v=null?(s.nullHalf?s.nullHalf(s.user,at):std::nullopt):(text.half?text.half(text.user,at):std::nullopt);return valid()?v:std::nullopt;}
 void bind(){
  actorView=services.common.actors;actorView.user=this;actorView.byte=[](void*p,uint32_t at){return static_cast<State*>(p)->byte(at);};
  actorView.memory.user=this;actorView.memory.word=[](void*p,uint32_t at){return static_cast<State*>(p)->word(at);};
  actorView.memory.half=[](void*p,uint32_t at)->uint16_t{auto&c=*static_cast<State*>(p);if(!c.valid())return 0;const auto&m=c.services.common.actors.memory;auto v=m.half(m.user,at);return c.valid()?v:0;};
  actorView.memory.storeWord=[](void*p,uint32_t at,uint32_t value){static_cast<State*>(p)->store(at,value);};
  view.user=this;view.lifetime=text.lifetime;view.byte=[](void*p,uint32_t at){return static_cast<State*>(p)->read(at);};view.half=[](void*p,uint32_t at){return static_cast<State*>(p)->half(at);};}
 template<class F>bool math(uint32_t function,F fill){pending=function;if(!matrix){matrix=BattleAudienceMatrixRequest{};matrix->function=function;fill(*matrix);}const auto&s=services.common;if(!valid()||!s.matrix)return false;bool done=s.matrix(s.user,*matrix);return valid()&&done;}
 bool load(){pending=0x802ba2fc;const auto&s=services.common;if(!s.loadMatrix)return false;bool done=s.loadMatrix(s.user,transformed,0);return valid()&&done;}
 bool tev(uint32_t rgba){pending=0x802b8ab8;if(!colorRequest)colorRequest=rgba;const auto&s=services.common;if(!s.tevColor)return false;bool done=s.tevColor(s.user,1,*colorRequest);if(!valid()||!done)return false;colorRequest.reset();return true;}
 bool emit(int32_t x,int32_t y){pending=0x80074720;if(!glyphRequest)glyphRequest=SourceMatrixFontGlyph{glyph,code,x,y};const auto&s=services.common;if(!s.glyph)return false;bool done=s.glyph(s.user,*glyphRequest);if(!valid()||!done)return false;glyphRequest.reset();return true;}
 std::optional<float> fused(uint32_t pc){pending=pc;if(!services.fma||!valid())return {};const auto v=services.fma(services.common.user,a,b,c);return valid()?v:std::nullopt;}
};
SourceScalarFontCursor::SourceScalarFontCursor()=default;SourceScalarFontCursor::~SourceScalarFontCursor()=default;
bool SourceScalarFontCursor::active()const{return bool(state_);}bool SourceScalarFontCursor::stale()const{return state_&&state_->invalid;}
uint32_t SourceScalarFontCursor::pending()const{return state_?state_->pending:0;}
bool SourceScalarFontCursor::reset(){if(invoking_)return false;state_.reset();return true;}
bool resumeSourceScalarFont(float x,float y,const SourceMatrixFontText&text,const SourceScalarFontServices&s,SourceScalarFontCursor&cursor){
 const auto&common=s.common;const auto&m=common.actors.memory;
 if(cursor.invoking_||!common.lifetime||!text.lifetime||!common.current||!text.byte||!m.word||!m.half||!m.storeWord||!common.actors.byte)return false;
 if(cursor.state_){const auto&c=*cursor.state_;if(c.xBits!=bits(x)||c.yBits!=bits(y)||c.borrowed!=&text||!same(c.services,s)||c.text.user!=text.user||c.text.byte!=text.byte||c.text.half!=text.half||!ownerSame(c.text.lifetime,text.lifetime))return false;}
 Running running(cursor.invoking_);if(!cursor.state_){cursor.state_=std::make_unique<SourceScalarFontCursor::State>();auto&c=*cursor.state_;c.services=s;c.text=text;c.borrowed=&text;c.xBits=bits(x);c.yBits=bits(y);c.bind();}
 auto&c=*cursor.state_;
 for(;;){if(!c.valid())return false;switch(c.phase){
 case 0:
  // Wrapper copies all three original words even though X/Y are overwritten.
  (void)c.word(0x802c2b78);(void)c.word(0x802c2b7c);c.position={x,y,real(c.word(0x802c2b80))};c.pitch=c.constant(-28688);c.phase=1;continue;
 case 1:{c.pending=0x8000f2e4;if(!common.camera)return false;const auto v=common.camera(common.user);if(!c.valid()||!v)return false;c.camera=*v;c.pen=c.position[0];c.phase=2;continue;}
 case 2:
  if(!c.math(0x8029e2b0,[&](auto&q){q.xyz={real(c.word(0x8041e9b0)),real(c.word(0x8041e9b4)),real(c.word(0x8041e9b8))};}))return false;
  c.scale=c.matrix->output;c.matrix.reset();c.phase=3;continue;
 case 3:{c.pending=0x80077338;const auto v=c.read(c.offset);if(!v)return false;if(!*v){cursor.state_.reset();return true;}c.code=*v;c.glyph=0xffff;c.phase=sourceMatrixFontLead(c.code,c.actorView)?4:5;continue;}
 case 4:{c.pending=0x800770a0;const auto v=c.half(c.offset);if(!v)return false;c.code=*v;c.glyph=sourceMatrixFontMap(c.code,c.actorView);c.offset+=2;c.phase=8;continue;}
 case 5:{c.pending=0x800770b4;const auto v=c.read(c.offset);if(!v)return false;if(*v=='<'){c.commandStart=c.offset+1;c.phase=6;}else{c.code=*v;++c.offset;c.phase=7;}continue;}
 case 6:{c.pending=0x80081aa0;const auto v=resumeSourceMatrixFontCommand(c.commandStart,c.view,common,c.command);if(!c.valid()||!v){if(c.command.stale())c.invalid=true;return false;}
  for(unsigned i=0;i<128;++i){if(v->name[i])c.names[i]=v->name[i];if(v->arguments[i])c.arguments[i]=v->arguments[i];}
  if(v->null){c.null=true;c.offset=0;}else c.offset=v->offset;c.phase=8;continue;}
 case 7:c.glyph=sourceMatrixFontMap(c.code,c.actorView);c.phase=8;continue;
 case 8:
  if(c.glyph==0xffff){c.phase=3;continue;}
  {const auto entry=sourceMatrixFontWidth(c.glyph,c.actorView);const auto zero=c.constant(-28744);c.width=c.byte(entry+1);
   if(c.pitch<zero){c.translation=c.pen;c.phase=11;continue;}
   const auto hi=c.word(0x802c2b90),lo=c.word(0x802c2b94);const auto half=c.constant(-28708),scale=real(c.word(0x8041e9b0));
   c.a=scale;c.b=mul(half,sub(c.pitch,converted(hi,lo,c.width,false)));c.c=c.pen;c.phase=10;continue;}
 case 10:{const auto v=c.fused(0x800771b4);if(!v)return false;c.translation=*v;c.phase=11;continue;}
 case 11:
  if(!c.math(0x8029e264,[&](auto&q){q.left=c.scale;q.xyz={c.translation,c.position[1],c.position[2]};}))return false;
  c.transformed=c.matrix->output;c.matrix.reset();c.phase=12;continue;
 case 12:
  if(!c.math(0x8029dd64,[&](auto&q){for(unsigned i=0;i<12;++i)q.left[i]=real(c.word(c.camera+0x11c+4*i));q.right=c.transformed;}))return false;
  c.transformed=c.matrix->output;c.matrix.reset();c.phase=13;continue;
 case 13:if(!c.load())return false;c.phase=14;continue;
 case 14:
  if(c.word(0x8041e9a4)){const auto palette=c.word(0x80416010);c.savedColor=c.word(0x8041e9ac);const auto alpha=c.byte(0x8041e9a8);c.store(0x8041e9ac,palette);c.color=(palette&0xffffff00)|((palette&255)*alpha/255);c.phase=15;}else c.phase=22;continue;
 case 15:if(!c.tev(c.color))return false;c.phase=16;continue;
 case 16:if(!c.emit(-2,0))return false;c.phase=17;continue;
 case 17:if(!c.emit(2,0))return false;c.phase=18;continue;
 case 18:if(!c.emit(0,-2))return false;c.phase=19;continue;
 case 19:if(!c.emit(0,2))return false;c.phase=20;continue;
 case 20:{const auto alpha=c.byte(0x8041e9a8);c.store(0x8041e9ac,c.savedColor);c.color=(c.savedColor&0xffffff00)|((c.savedColor&255)*alpha/255);c.phase=21;continue;}
 case 21:if(!c.tev(c.color))return false;c.phase=22;continue;
 case 22:if(!c.emit(0,0))return false;c.phase=23;continue;
 case 23:
  if(c.pitch<c.constant(-28744)){
   const auto gp=c.word(0x80415fd0),language=c.word(gp+0x16c),hi=c.word(0x802c2b98),lo=c.word(0x802c2b9c);
   const auto spacing=c.word(0x8041601c+4*language);const auto scale=real(c.word(0x8041e9b0));c.a=scale;c.b=converted(hi,lo,spacing+c.width,true);c.c=c.pen;c.phase=24;
  }else{c.a=c.pitch;c.b=real(c.word(0x8041e9b0));c.c=c.pen;c.phase=25;}continue;
 case 24:{const auto v=c.fused(0x80077328);if(!v)return false;c.pen=*v;c.phase=3;continue;}
 case 25:{const auto v=c.fused(0x80077334);if(!v)return false;c.pen=*v;c.phase=3;continue;}
 default:return false;
 }}
}
}
