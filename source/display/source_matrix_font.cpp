#include "source_matrix_font.hpp"
#include <cstring>
namespace ttyd {namespace {
int32_t sw(uint32_t n){return n<0x80000000u?int32_t(n):int32_t(int64_t(n)-0x100000000LL);}
int32_t sh(uint32_t n){n&=0xffff;return n<0x8000?int32_t(n):int32_t(n)-65536;}
float scalar(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
float add(float a,float b){volatile float v=a+b;return v;}
float sub(float a,float b){volatile float v=a-b;return v;}
uint32_t word(const BattleActorServices&s,uint32_t a){return s.memory.word(s.memory.user,a);}
uint16_t half(const BattleActorServices&s,uint32_t a){return s.memory.half(s.memory.user,a);}
uint32_t widthEntry(const BattleActorServices&s,uint16_t glyph){const auto f=word(s,0x8041e978);auto count=word(s,f+0xc),block=f+0x20;while(count--){if(word(s,block)==0x57494431){const auto start=half(s,block+8);if(glyph>=start&&glyph<=half(s,block+0xa))return block+0xc+2*(glyph-start);}block+=word(s,block+4);}return 0;}
uint16_t mapCode(const BattleActorServices&s,uint32_t input){const auto font=word(s,0x8041e978),info=word(s,0x8041e9a0);auto count=word(s,font+0xc),block=font+0x20;const auto fallback=half(s,info+0x12),code=uint16_t(input);while(count--){if(word(s,block)==0x4d415031){const auto first=half(s,block+0xa);if(code>=first&&code<=half(s,block+0xc)){switch(half(s,block+8)){case 0:return uint16_t(input-first);case 1:{const auto lo=uint8_t(input),hi=uint8_t(input>>8);const auto t=uint32_t(lo)-(lo<128?64u:65u);if(!half(s,block+0xe))return uint16_t(188u*(uint32_t(hi)-136)+702+t);return uint16_t(188u*(uint32_t(hi)-136)-94+half(s,block+0x10)+t);}case 2:return half(s,block+0x10+2*(code-first));case 3:{int32_t low=0,high=half(s,block+0xe);while(high>=low){const auto mid=sw(uint32_t(low)+uint32_t(high))/2;const auto candidate=half(s,block+0x10+4*uint32_t(mid));if(code==candidate)return half(s,block+0x12+4*uint32_t(mid));if(code<candidate)high=mid-1;else low=mid+1;}return fallback;}default:return fallback;}}}block+=word(s,block+4);}return fallback;}
bool memorySame(const BattleActorServices&a,const BattleActorServices&b){return a.user==b.user&&a.byte==b.byte&&a.audienceLifecycle.user==b.audienceLifecycle.user&&a.audienceLifecycle.event==b.audienceLifecycle.event&&a.memory.user==b.memory.user&&a.memory.word==b.memory.word&&a.memory.half==b.memory.half&&a.memory.storeWord==b.memory.storeWord&&a.memory.storeHalf==b.memory.storeHalf&&a.memory.storeByte==b.memory.storeByte&&a.memory.flushBlock==b.memory.flushBlock&&a.memory.systemCall==b.memory.systemCall;}
bool ownerSame(const std::shared_ptr<const void>&a,const std::shared_ptr<const void>&b){return a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);}
bool same(const SourceMatrixFontServices&a,const SourceMatrixFontServices&b){return memorySame(a.actors,b.actors)&&ownerSame(a.lifetime,b.lifetime)&&a.user==b.user&&a.current==b.current&&a.camera==b.camera&&a.matrix==b.matrix&&a.loadMatrix==b.loadMatrix&&a.tevColor==b.tevColor&&a.glyph==b.glyph&&a.truncate==b.truncate&&a.nullByte==b.nullByte&&a.nullHalf==b.nullHalf;}
struct Running{bool&v;explicit Running(bool&f):v(f){v=true;}~Running(){v=false;}};
}
bool sourceMatrixFontLead(uint32_t code,const BattleActorServices&s){const auto language=word(s,word(s,0x80415fd0)+0x16c);const auto b=uint8_t(code);return language==0&&((b>=0x81&&b<=0x9f)||(b>=0xe0&&b<=0xfc));}
uint16_t sourceMatrixFontMap(uint32_t code,const BattleActorServices&s){return mapCode(s,code);}
uint32_t sourceMatrixFontWidth(uint16_t glyph,const BattleActorServices&s){return widthEntry(s,glyph);}
struct SourceMatrixFontCommandCursor::State {
 SourceMatrixFontServices services;SourceMatrixFontText text;const SourceMatrixFontText*borrowed=nullptr;
 uint32_t start=0,offset=0,count=0,nameIndex=0,argIndex=0;bool argument=false,invalid=false;
 SourceMatrixFontCommandResult output;
 bool valid(){if(invalid)return false;if(!services.current||!services.current(services.user)){invalid=true;return false;}return true;}
};
SourceMatrixFontCommandCursor::SourceMatrixFontCommandCursor()=default;SourceMatrixFontCommandCursor::~SourceMatrixFontCommandCursor()=default;
bool SourceMatrixFontCommandCursor::active()const{return bool(state_);}bool SourceMatrixFontCommandCursor::stale()const{return state_&&state_->invalid;}bool SourceMatrixFontCommandCursor::reset(){if(invoking_)return false;state_.reset();return true;}
std::optional<SourceMatrixFontCommandResult> resumeSourceMatrixFontCommand(uint32_t start,const SourceMatrixFontText&text,const SourceMatrixFontServices&s,SourceMatrixFontCommandCursor&cursor){
 if(cursor.invoking_||!text.byte||!text.lifetime||!s.current||!s.lifetime)return {};
 if(cursor.state_){const auto&c=*cursor.state_;if(c.start!=start||c.borrowed!=&text||!same(c.services,s)||c.text.user!=text.user||c.text.byte!=text.byte||c.text.half!=text.half||!ownerSame(c.text.lifetime,text.lifetime))return {};}
 Running running(cursor.invoking_);if(!cursor.state_){cursor.state_=std::make_unique<SourceMatrixFontCommandCursor::State>();auto&c=*cursor.state_;c.services=s;c.text=text;c.borrowed=&text;c.start=start;c.offset=start;}
 auto&c=*cursor.state_;while(c.valid()){
  if(c.count==128){c.output.null=true;auto out=c.output;cursor.state_.reset();return out;}
  const auto v=c.text.byte(c.text.user,c.offset);if(!c.valid()||!v)return {};
  if(*v=='>'){auto&out=c.argument?c.output.arguments:c.output.name;const auto index=c.argument?c.argIndex:c.nameIndex;out[index]=0;c.output.offset=c.offset+1;auto result=c.output;cursor.state_.reset();return result;}
  if(*v==' '&&!c.argument){c.output.name[c.nameIndex]=0;c.argument=true;}
  else{auto&out=c.argument?c.output.arguments:c.output.name;auto&index=c.argument?c.argIndex:c.nameIndex;out[index++]=*v;}
  ++c.offset;++c.count;
 }return {};
}
struct SourceMatrixFontCursor::State {
 SourceMatrixFontServices services;SourceMatrixFontText text,nullText;const SourceMatrixFontText*borrowedText=nullptr;const BattleActMatrix*borrowedBase=nullptr;
 BattleActMatrix base{},transformed{};std::optional<BattleAudienceMatrixRequest> math;std::optional<SourceMatrixFontGlyph> glyphRequest;
 uint32_t phase=0,pending=0,camera=0,offset=0,code=0,glyph=0xffff,savedColor=0,effectiveColor=0,outline=0,convertedX=0,convertedY=0;
 float x=0,y=0;bool baseCaptured=false,invalid=false,null=false;std::optional<float> conversion;std::optional<uint32_t> heldColor;
 SourceMatrixFontCommandCursor command;uint32_t commandStart=0;std::array<std::optional<uint8_t>,128> name{};
 bool valid(){if(invalid)return false;if(!services.current||!services.current(services.user)){invalid=true;return false;}return true;}
 uint32_t word(uint32_t a)const{return ttyd::word(services.actors,a);}
 uint8_t byte(uint32_t a)const{return services.actors.byte(services.actors.user,a);}
 void store(uint32_t a,uint32_t v)const{services.actors.memory.storeWord(services.actors.memory.user,a,v);}
 float constant(int32_t n)const{return scalar(word(0x80427440u+uint32_t(n)));}
 std::optional<uint8_t> read(){pending=0x80077004;const auto v=null?(services.nullByte?services.nullByte(services.user,offset):std::nullopt):text.byte(text.user,offset);if(!valid())return {};return v;}
 std::optional<int32_t> truncate(uint32_t pc,float value){pending=pc;if(!conversion)conversion=value;if(!services.truncate)return {};const auto v=services.truncate(services.user,*conversion);if(!valid()||!v)return {};conversion.reset();return v;}
 bool color(uint32_t rgba){pending=0x802b8ab8;if(!heldColor)heldColor=rgba;if(!services.tevColor)return false;const bool done=services.tevColor(services.user,1,*heldColor);if(!valid()||!done)return false;heldColor.reset();return true;}
 bool emit(int32_t xPos,int32_t yPos){pending=0x80074720;if(!glyphRequest)glyphRequest=SourceMatrixFontGlyph{uint16_t(glyph),code,xPos,yPos};if(!services.glyph)return false;const bool done=services.glyph(services.user,*glyphRequest);if(!valid()||!done)return false;glyphRequest.reset();return true;}
};
SourceMatrixFontCursor::SourceMatrixFontCursor()=default;SourceMatrixFontCursor::~SourceMatrixFontCursor()=default;
bool SourceMatrixFontCursor::active()const{return bool(state_);}bool SourceMatrixFontCursor::stale()const{return state_&&state_->invalid;}uint32_t SourceMatrixFontCursor::pending()const{return state_?state_->pending:0;}bool SourceMatrixFontCursor::reset(){if(invoking_)return false;state_.reset();return true;}
bool resumeSourceMatrixFont(const BattleActMatrix&base,const SourceMatrixFontText&text,const SourceMatrixFontServices&s,SourceMatrixFontCursor&cursor){
 const auto&m=s.actors.memory;if(cursor.invoking_||!s.lifetime||!text.lifetime||!s.current||!text.byte||!m.word||!m.half||!m.storeWord||!s.actors.byte)return false;
 if(cursor.state_){const auto&c=*cursor.state_;if(c.borrowedBase!=&base||c.borrowedText!=&text||(c.baseCaptured&&std::memcmp(c.base.data(),base.data(),sizeof(base)))||!same(c.services,s)||c.text.user!=text.user||c.text.byte!=text.byte||c.text.half!=text.half||!ownerSame(c.text.lifetime,text.lifetime))return false;}
 Running running(cursor.invoking_);if(!cursor.state_){cursor.state_=std::make_unique<SourceMatrixFontCursor::State>();auto&c=*cursor.state_;c.services=s;c.text=text;c.borrowedText=&text;c.borrowedBase=&base;c.nullText.user=&c;c.nullText.lifetime=s.lifetime;c.nullText.byte=[](void*p,uint32_t at)->std::optional<uint8_t>{auto&state=*static_cast<SourceMatrixFontCursor::State*>(p);return state.services.nullByte?state.services.nullByte(state.services.user,at):std::nullopt;};c.nullText.half=[](void*p,uint32_t at)->std::optional<uint16_t>{auto&state=*static_cast<SourceMatrixFontCursor::State*>(p);return state.services.nullHalf?state.services.nullHalf(state.services.user,at):std::nullopt;};}
 auto&c=*cursor.state_;
 for(;;){if(!c.valid())return false;switch(c.phase){
 case 0:{c.pending=0x8000f2e4;if(!s.camera)return false;const auto v=s.camera(s.user);if(!c.valid()||!v)return false;c.camera=*v;c.x=c.constant(-28744);c.y=c.x;c.phase=1;continue;}
 case 1:{c.pending=0x8029dd64;if(!c.math){c.math=BattleAudienceMatrixRequest{};c.math->function=0x8029dd64;for(unsigned i=0;i<12;++i)c.math->left[i]=scalar(c.word(c.camera+0x11c+4*i));c.base=*c.borrowedBase;c.baseCaptured=true;c.math->right=c.base;}if(!s.matrix)return false;const bool done=s.matrix(s.user,*c.math);if(!c.valid()||!done)return false;c.transformed=c.math->output;c.math.reset();c.phase=2;continue;}
 case 2:{c.pending=0x802ba2fc;if(!s.loadMatrix)return false;const bool done=s.loadMatrix(s.user,c.transformed,0);if(!c.valid()||!done)return false;c.phase=3;continue;}
 case 3:{const auto v=c.read();if(!v)return false;if(!*v){cursor.state_.reset();return true;}c.code=*v;c.glyph=0xffff;c.phase=sourceMatrixFontLead(c.code,s.actors)?4:5;continue;}
 case 4:{c.pending=0x80076d48;const auto&view=c.null?c.nullText:text;if(!view.half)return false;const auto v=view.half(view.user,c.offset);if(!c.valid()||!v)return false;c.code=*v;c.glyph=mapCode(s.actors,c.code);c.offset+=2;c.phase=8;continue;}

 case 5:{const auto v=c.read();if(!v)return false;if(*v=='\n'){const auto pitch=c.constant(-28684);++c.offset;c.x=c.constant(-28744);c.y=sub(c.y,pitch);c.phase=8;continue;}if(*v=='<'){++c.offset;c.commandStart=c.offset;c.phase=6;continue;}c.phase=7;continue;}
 case 6:{c.pending=0x80081aa0;const auto out=resumeSourceMatrixFontCommand(c.commandStart,c.null?c.nullText:*c.borrowedText,s,c.command);if(!c.valid()||!out){if(c.command.stale())c.invalid=true;return false;}for(size_t i=0;i<c.name.size();++i)if(out->name[i])c.name[i]=out->name[i];if(out->null){c.null=true;c.offset=0;}else c.offset=out->offset;c.phase=9;continue;}

 case 9:
  if(!c.name[0]){c.pending=0x80076da0;return false;}
  if(*c.name[0]=='R'){c.phase=8;continue;}
  // Source byte '/' cannot also equal0x2f72; there is no '/r' exception.
  {const auto v=c.read();if(!v)return false;if(*v=='\n')++c.offset;c.phase=8;continue;}
 case 7:{const auto v=c.read();if(!v)return false;c.code=*v;++c.offset;c.glyph=mapCode(s.actors,c.code);c.phase=8;continue;}
 case 8:
  if(uint16_t(c.glyph)==0xffff){c.phase=3;continue;}
  if(c.word(0x8041e9a4)){const auto palette=c.word(0x80416010),saved=c.word(0x8041e9ac);c.savedColor=saved;const auto alpha=c.byte(0x8041e9a8);c.store(0x8041e9ac,palette);c.effectiveColor=(palette&0xffffff00)|((palette&255)*alpha/255);c.phase=10;continue;}
  c.phase=20;continue;
 case 10:if(!c.color(c.effectiveColor))return false;c.phase=11;continue;
 case 11:{const auto v=c.truncate(0x80076e58,c.x);if(!v)return false;c.convertedX=uint32_t(*v);c.phase=12;continue;}
 case 12:{const auto v=c.truncate(0x80076e60,c.y);if(!v)return false;c.convertedY=uint32_t(*v);c.phase=13;continue;}
 case 13:if(!c.emit(sh(c.convertedX-2),sh(c.convertedY)))return false;c.phase=14;continue;
 case 14:if(!c.emit(sh(c.convertedX+2),sh(c.convertedY)))return false;c.phase=15;continue;
 case 15:{const auto v=c.truncate(0x80076ea0,c.y);if(!v)return false;c.convertedY=uint32_t(*v);c.phase=16;continue;}
 case 16:{const auto v=c.truncate(0x80076ea8,c.x);if(!v)return false;c.convertedX=uint32_t(*v);c.phase=17;continue;}
 case 17:if(!c.emit(sh(c.convertedX),sh(c.convertedY-2)))return false;c.phase=18;continue;
 case 18:if(!c.emit(sh(c.convertedX),sh(c.convertedY+2)))return false;{const auto alpha=c.byte(0x8041e9a8);c.store(0x8041e9ac,c.savedColor);c.effectiveColor=(c.savedColor&0xffffff00)|((c.savedColor&255)*alpha/255);}c.phase=19;continue;
 case 19:if(!c.color(c.effectiveColor))return false;c.phase=20;continue;
 case 20:{const auto v=c.truncate(0x80076f34,c.x);if(!v)return false;c.convertedX=uint32_t(*v);c.phase=21;continue;}
 case 21:{const auto v=c.truncate(0x80076f3c,c.y);if(!v)return false;c.convertedY=uint32_t(*v);c.phase=22;continue;}
 case 22:if(!c.emit(sw(c.convertedX),sw(c.convertedY)))return false;c.phase=23;continue;
 case 23:{const auto entry=widthEntry(s.actors,uint16_t(c.glyph));const auto gp=c.word(0x80415fd0),language=c.word(gp+0x16c);const auto width=c.byte(entry+1);const auto spacing=c.word(0x8041601c+4*language);const auto hi=c.word(0x802c2b98),lo=c.word(0x802c2b9c);const uint64_t raw=(uint64_t(hi)<<32)|lo;double bias;std::memcpy(&bias,&raw,8);const uint64_t encoded=(uint64_t(0x43300000)<<32)|((spacing+width)^0x80000000u);double advance;std::memcpy(&advance,&encoded,8);c.x=add(c.x,float(advance-bias));c.phase=3;continue;}
 default:return false;
 }}
}
bool drawSourceMatrixFontAtomicGlyph(const SourceMatrixFontGlyph&q,const SourceMatrixFontAtomicGlyphServices&s){
 if(!s.invoking||*s.invoking)return false;
 Running running(*s.invoking);
 const auto&m=s.actors.memory;if(!s.font||!s.lifetime||!s.current||!s.current(s.user)||!m.word||!m.half||!s.actors.byte||!s.color||!s.admit||!s.random||!s.emit||q.sourceCode==0||q.sourceCode>255||q.sourceCode=='<'||q.sourceCode=='\n')return false;
 const auto byte=uint8_t(q.sourceCode);if(s.font->mapping[byte]!=q.glyph||q.glyph>=s.font->glyphs.size())return false;
 const auto&glyph=s.font->glyphs[q.glyph];const auto entry=widthEntry(s.actors,q.glyph);if(!entry)return false;
 const auto bearing=s.actors.byte(s.actors.user,entry),width=s.actors.byte(s.actors.user,entry+1);const auto descriptor=word(s.actors,0x8041e99c);
 const auto first=half(s.actors,descriptor+8),cellWidth=half(s.actors,descriptor+0xc),height=half(s.actors,descriptor+0xe),columns=half(s.actors,descriptor+0x16),rows=half(s.actors,descriptor+0x18);
 // Explicit native finite US atlas domain, not guards invented in original.
 if(!columns||!rows||height<2||q.glyph<first)return false;
 const auto difference=q.glyph-first,globalRow=difference/columns,col=difference%columns,row=globalRow%rows;
 const auto x=uint32_t(col)*cellWidth+bearing,y=2+uint32_t(row)*height;
 if(x>65535||y>65535||glyph.x!=x||glyph.y!=y||glyph.width!=width||glyph.height!=height-2)return false;
 const auto color=s.color(s.user);if(!color||!s.current(s.user))return false;
 // Pure geometry preflight does not execute source RNG. The matching
 // existing consumer performs the real two random calls and actual emission.
 const auto left=sh(uint32_t(q.x)),top=sh(uint32_t(sh(uint32_t(q.y)))-2);
 WindowFontQuad quad{int16_t(left),int16_t(top),int16_t(sh(uint32_t(left)+glyph.width)),int16_t(sh(uint32_t(top)-glyph.height)),glyph.x,glyph.y,uint16_t(glyph.x+glyph.width),uint16_t(glyph.y+glyph.height),*color};
 if(!s.admit(s.user,quad)||!s.current(s.user))return false;
 const char text[2]{char(byte),0};WindowFontStyle style;style.color=*color;style.alpha=255;style.edge=false;style.spacing=0;
 WindowFontDrawServices actual;actual.user=s.user;actual.random=s.random;actual.emitQuad=s.emit;
 // After whole admission the actual immutable owner forbids rejection,
 // mutation/reentry or failure in these void RNG/quad callbacks.
 return drawPlainWindowFont(*s.font,text,WindowFontConsumer::MessageDirect,style,actual,q.x,q.y);
}
}
