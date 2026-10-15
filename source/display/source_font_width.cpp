#include "source_font_width.hpp"
#include <cstring>
namespace ttyd {namespace {
float real(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
float add(float a,float b){volatile float n=a+b;return n;}
//4330 double construction uses unsigned or sign-XOR low words,then original
// bias subtraction rounds once to float. add() preserves a separate float step.
float converted(uint32_t hi,uint32_t lo,uint32_t n,bool sign){uint64_t raw=uint64_t(hi)<<32|lo,encoded=0x4330000000000000ULL|(n^(sign?0x80000000u:0u));double bias,value;std::memcpy(&bias,&raw,8);std::memcpy(&value,&encoded,8);volatile float out=value-bias;return out;}
bool ownerSame(const std::shared_ptr<const void>&a,const std::shared_ptr<const void>&b){return a==b&&!a.owner_before(b)&&!b.owner_before(a);}
bool same(const SourceFontWidthServices&a,const SourceFontWidthServices&b){const auto&s=a.common;const auto&t=b.common;const auto&i=s.actors;const auto&j=t.actors;const auto&m=i.memory;const auto&n=j.memory;
 return ownerSame(s.lifetime,t.lifetime)&&s.user==t.user&&s.current==t.current&&s.camera==t.camera&&s.matrix==t.matrix&&s.loadMatrix==t.loadMatrix&&s.tevColor==t.tevColor&&s.glyph==t.glyph&&s.truncate==t.truncate&&s.nullByte==t.nullByte&&s.nullHalf==t.nullHalf&&a.call==b.call&&a.fma==b.fma&&i.user==j.user&&i.byte==j.byte&&i.audienceLifecycle.user==j.audienceLifecycle.user&&i.audienceLifecycle.event==j.audienceLifecycle.event&&m.user==n.user&&m.word==n.word&&m.half==n.half&&m.storeWord==n.storeWord&&m.storeHalf==n.storeHalf&&m.storeByte==n.storeByte&&m.flushBlock==n.flushBlock&&m.systemCall==n.systemCall;}
struct Running{bool&v;explicit Running(bool&f):v(f){v=true;}~Running(){v=false;}};
}
// Private frame is byte-addressed BE storage. Optional disengagement means an
// unwritten byte,not byte0; reads of1..4 bytes require every lane known. Writes
// preserve aliases among scanf arguments/results,icon dimensions and strings.
std::optional<uint32_t> SourceFontWidthPrivate::read(uint32_t at,uint32_t size)const{if(!size||size>4||uint64_t(at)+size>bytes.size())return {};uint32_t value=0;for(uint32_t i=0;i<size;++i){if(!bytes[at+i])return {};value=value<<8|*bytes[at+i];}return value;}
bool SourceFontWidthPrivate::write(uint32_t at,uint32_t size,uint32_t value){if(!size||size>4||uint64_t(at)+size>bytes.size())return false;for(uint32_t i=0;i<size;++i)bytes[at+i]=uint8_t(value>>(8*(size-i-1)));return true;}
// One borrowed text object and its strong owner remain stable. Total actor
// reads require admitted RAM; optional text/private reads can hold. Late source
// GP/language/spacing reads stay late,not folded into a font-metric snapshot.
struct SourceFontWidthCursor::State {
 SourceFontWidthServices services;BattleActorServices actorView;SourceMatrixFontText text,view;const SourceMatrixFontText*borrowed=nullptr;
 SourceFontWidthPrivate frame;SourceMatrixFontCommandCursor command;std::optional<SourceFontWidthCall> request;
 uint32_t linesAddress=0,phase=0,pending=0,offset=0,commandStart=0,current=0,maximum=0,lines=0,code=0,entry=0,gp=0,extra=0,width=0;
 uint16_t glyph=0xffff;bool invalid=false,null=false;float a=0,b=0,c=0,currentReal=0,result=0;
 bool valid(){if(invalid)return false;const auto&s=services.common;if(!s.current||!s.current(s.user)){invalid=true;return false;}return true;}
 uint32_t word(uint32_t at){if(!valid())return 0;const auto&m=services.common.actors.memory;const auto v=m.word(m.user,at);return valid()?v:0;}
 uint16_t half(uint32_t at){if(!valid())return 0;const auto&m=services.common.actors.memory;const auto v=m.half(m.user,at);return valid()?v:0;}
 uint8_t byte(uint32_t at){if(!valid())return 0;const auto&s=services.common.actors;const auto v=s.byte(s.user,at);return valid()?v:0;}
 std::optional<uint8_t> read(uint32_t at){if(!valid())return {};const auto&s=services.common;const auto v=null?(s.nullByte?s.nullByte(s.user,at):std::nullopt):text.byte(text.user,at);return valid()?v:std::nullopt;}
 std::optional<uint16_t> textHalf(uint32_t at){if(!valid())return {};const auto&s=services.common;const auto v=null?(s.nullHalf?s.nullHalf(s.user,at):std::nullopt):(text.half?text.half(text.user,at):std::nullopt);return valid()?v:std::nullopt;}
 void bind(){actorView=services.common.actors;actorView.user=this;actorView.byte=[](void*p,uint32_t a){return static_cast<State*>(p)->byte(a);};actorView.memory.user=this;actorView.memory.word=[](void*p,uint32_t a){return static_cast<State*>(p)->word(a);};actorView.memory.half=[](void*p,uint32_t a){return static_cast<State*>(p)->half(a);};view.user=this;view.lifetime=text.lifetime;view.byte=[](void*p,uint32_t a){return static_cast<State*>(p)->read(a);};view.half=[](void*p,uint32_t a){return static_cast<State*>(p)->textHalf(a);};}
 std::optional<int32_t> call(uint32_t fn,uint32_t pc,std::initializer_list<uint32_t>args){pending=fn;if(!request){request=SourceFontWidthCall{};request->function=fn;request->callsite=pc;request->frame=&frame;request->count=uint8_t(args.size());unsigned i=0;for(auto n:args)request->arguments[i++]=n;}if(!services.call||!valid())return {};const auto v=services.call(services.common.user,*request);if(!valid()||!v)return {};request.reset();return v;}
};
SourceFontWidthCursor::SourceFontWidthCursor()=default;SourceFontWidthCursor::~SourceFontWidthCursor()=default;
bool SourceFontWidthCursor::active()const{return bool(state_);}bool SourceFontWidthCursor::stale()const{return state_&&state_->invalid;}uint32_t SourceFontWidthCursor::pending()const{return state_?state_->pending:0;}bool SourceFontWidthCursor::reset(){if(invoking_)return false;state_.reset();return true;}
std::optional<uint32_t> resumeSourceFontWidthLine(const SourceMatrixFontText&text,uint32_t lines,const SourceFontWidthServices&s,SourceFontWidthCursor&cursor){
 const auto&m=s.common.actors.memory;if(cursor.invoking_||!s.common.lifetime||!text.lifetime||!s.common.current||!text.byte||!m.word||!m.half||!s.common.actors.byte)return {};
 if(cursor.state_){const auto&c=*cursor.state_;if(c.borrowed!=&text||c.linesAddress!=lines||!same(c.services,s)||c.text.user!=text.user||c.text.byte!=text.byte||c.text.half!=text.half||!ownerSame(c.text.lifetime,text.lifetime))return {};}
 Running running(cursor.invoking_);if(!cursor.state_){cursor.state_=std::make_unique<SourceFontWidthCursor::State>();auto&c=*cursor.state_;c.services=s;c.text=text;c.borrowed=&text;c.linesAddress=lines;c.bind();}
 auto&c=*cursor.state_;
 for(;;){if(!c.valid())return {};switch(c.phase){
 // Separate EOF-test byte,lead predicate,true LHZ or fresh single byte reads.
 // Newline compares LOW16 widths,clears current and counts newline characters;
 // glyphffff skips width. This width parser differs from centered raw drawing.
 case 0:{c.pending=0x80075804;const auto v=c.read(c.offset);if(!v)return {};if(!*v){c.phase=20;continue;}c.code=*v;c.glyph=0xffff;c.phase=sourceMatrixFontLead(c.code,c.actorView)?1:2;continue;}
 case 1:{c.pending=0x8007558c;const auto v=c.textHalf(c.offset);if(!v)return {};c.glyph=sourceMatrixFontMap(*v,c.actorView);c.offset+=2;c.phase=18;continue;}
 case 2:{c.pending=0x800755a0;const auto v=c.read(c.offset);if(!v)return {};if(*v=='<'){c.commandStart=c.offset+1;c.phase=3;continue;}if(*v=='\n'){if(uint16_t(c.maximum)<uint16_t(c.current))c.maximum=c.current;c.current=0;++c.lines;++c.offset;c.phase=18;continue;}c.phase=17;continue;}
 // Command name c0..13f and args40..bf share one retained140-byte frame.
 // Merge ONLY written bytes; a new command can retain previous argument bytes.
 // Null command switches to actual guest-null providers at offset0.
 case 3:{c.pending=0x80081aa0;const auto v=resumeSourceMatrixFontCommand(c.commandStart,c.view,s.common,c.command);if(!c.valid()||!v){if(c.command.stale())c.invalid=true;return {};}
  for(unsigned i=0;i<128;++i){if(v->name[i])c.frame.bytes[0xc0+i]=v->name[i];if(v->arguments[i])c.frame.bytes[0x40+i]=v->arguments[i];}if(v->null){c.null=true;c.offset=0;}else c.offset=v->offset;c.phase=4;continue;}
 // strcmp(icon literal80420408),then CameraGet even though result is unused.
 // Numeric leading ASCII0..9 selects scanf format802c2bbc; otherwise2bc8
 // parses a name. Scanf result0 is complete,not missing output initialization.
 case 4:{const auto v=c.call(0x8026d464,0x80075604,{0xc0,0x80420408});if(!v)return {};c.phase=*v==0?5:14;continue;}
 case 5:{c.pending=0x8000f2e4;if(!s.common.camera)return {};const auto v=s.common.camera(s.common.user);if(!c.valid()||!v)return {};c.phase=6;continue;}
 case 6:{c.pending=0x80075614;const auto v=c.frame.read(0x40,1);if(!v)return {};c.phase=*v>='0'&&*v<='9'?7:8;continue;}
 case 7:if(!c.call(0x8026c344,0x80075650,{0x40,0x802c2bbc,0x1c,0x0c,0x18,0x14,0x10}))return {};c.phase=10;continue;
 case 8:if(!c.call(0x8026c344,0x8007567c,{0x40,0x802c2bc8,0x20,0x0c,0x18,0x14,0x10}))return {};c.phase=9;continue;
 // Named icon negative lookup floors0. iconGetSize outputs width at private
 // a,height8 and uses low16 ID; other frame cells remain caller-written only.
 case 9:{const auto v=c.call(0x80081a1c,0x80075684,{0x20});if(!v)return {};c.frame.write(0x1c,4,*v<0?0:uint32_t(*v));c.phase=10;continue;}
 case 10:{c.pending=0x8007569c;const auto id=c.frame.read(0x1c,4);if(!id)return {};c.code=*id&0xffff;c.phase=11;continue;}
 case 11:if(!c.call(0x8001a19c,0x800756ac,{0xa,8,c.code}))return {};c.phase=12;continue;
 case 12:{c.pending=0x800756b0;const auto width=c.frame.read(0xa,2);if(!width)return {};c.width=*width;c.phase=121;continue;}
 // Capture unsigned icon width,signed extra and current LOW16 with source
 // unsigned/signed biases2b90/98 before later scale0c read. Required FMA
 // computes width*scale+extra; separate float addition then fctiwz follow.
 case 121:{c.pending=0x800756b8;const auto extra=c.frame.read(0x10,4);if(!extra)return {};c.extra=*extra;const auto uhi=c.word(0x802c2b90),ulo=c.word(0x802c2b94),shi=c.word(0x802c2b98),slo=c.word(0x802c2b9c);c.a=converted(uhi,ulo,c.width,false);c.c=converted(shi,slo,c.extra,true);c.currentReal=converted(uhi,ulo,uint16_t(c.current),false);c.phase=122;continue;}
 case 122:{c.pending=0x800756f0;const auto scale=c.frame.read(0xc,4);if(!scale)return {};c.b=real(*scale);c.phase=13;continue;}
 case 13:{c.pending=0x80075708;if(!s.fma)return {};const auto v=s.fma(s.common.user,c.a,c.b,c.c);if(!c.valid()||!v)return {};c.result=add(c.currentReal,*v);c.phase=131;continue;}
 case 131:{c.pending=0x80075714;if(!s.common.truncate)return {};const auto v=s.common.truncate(s.common.user,c.result);if(!c.valid()||!v)return {};c.current=uint32_t(*v);c.phase=15;continue;}
 // Only name starting R skips the following newline test. The original
 // signed-byte slash comparison cannot equal2f72; do not normalize /r here.
 case 14:{c.pending=0x80075724;const auto v=c.frame.read(0xc0,1);if(!v)return {};c.phase=*v=='R'?18:15;continue;}
 case 15:{c.pending=0x80075744;const auto v=c.read(c.offset);if(!v)return {};if(*v=='\n')++c.offset;c.phase=18;continue;}
 case 17:{c.pending=0x80075758;const auto v=c.read(c.offset);if(!v)return {};++c.offset;c.glyph=sourceMatrixFontMap(*v,c.actorView);c.phase=18;continue;}
 case 18:
  if(c.glyph==0xffff){c.phase=0;continue;}
  c.entry=sourceMatrixFontWidth(c.glyph,c.actorView);c.gp=c.word(0x80415fd0);c.width=c.byte(c.entry+1);c.phase=19;continue;
 case 19:{const auto language=c.word(c.gp+0x16c);const auto value=c.current+c.width;const auto spacing=c.word(0x8041601c+4*language);c.current=(spacing+value)&0xffff;c.phase=0;continue;}
 // EOF compares low16 and removes FRESH language spacing only for nonzero
 // maximum. lines counts reached newline branches; optional output is actual
 // guest STH. Wrapper passes null; private frame never substitutes that address.
 case 20:
  if(uint16_t(c.maximum)<uint16_t(c.current))c.maximum=c.current;
  if(uint16_t(c.maximum)){const auto gp=c.word(0x80415fd0),language=c.word(gp+0x16c),spacing=c.word(0x8041601c+4*language);c.maximum=(c.maximum-spacing)&0xffff;}
  c.phase=21;continue;
 case 21:
  if(c.linesAddress){c.pending=0x80075850;if(!m.storeHalf)return {};m.storeHalf(m.user,c.linesAddress,uint16_t(c.lines));if(!c.valid())return {};}
  {const auto out=c.maximum;cursor.state_.reset();return out;}
 default:return {};
 }}
}
std::optional<uint32_t> resumeSourceFontWidth(const SourceMatrixFontText&t,const SourceFontWidthServices&s,SourceFontWidthCursor&c){return resumeSourceFontWidthLine(t,0,s,c);}
}
