#include "source_string_font.hpp"
#include <cstring>
namespace ttyd {namespace {
float real(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
float add(float a,float b){volatile float v=a+b;return v;}
float mul(float a,float b){volatile float v=a*b;return v;}
float converted(uint32_t hi,uint32_t lo,uint32_t value,bool sign){uint64_t raw=uint64_t(hi)<<32|lo,bits=0x4330000000000000ULL|(value^(sign?0x80000000u:0u));double bias,number;std::memcpy(&bias,&raw,8);std::memcpy(&number,&bits,8);volatile float out=number-bias;return out;}
int32_t sh(uint32_t n){n&=65535;return n<32768?int32_t(n):int32_t(n)-65536;}
int32_t sw(uint32_t n){return n<0x80000000u?int32_t(n):int32_t(int64_t(n)-0x100000000LL);}
bool owner(const std::shared_ptr<const void>&a,const std::shared_ptr<const void>&b){return a==b&&!a.owner_before(b)&&!b.owner_before(a);}
bool same(const SourceStringFontServices&a,const SourceStringFontServices&b){
 const auto&s=a.common;const auto&t=b.common;const auto&i=s.actors;const auto&j=t.actors;const auto&m=i.memory;const auto&n=j.memory;
 return a.width==b.width&&owner(s.lifetime,t.lifetime)&&s.user==t.user&&s.current==t.current&&s.camera==t.camera&&s.matrix==t.matrix&&s.loadMatrix==t.loadMatrix&&s.tevColor==t.tevColor&&s.glyph==t.glyph&&s.truncate==t.truncate&&s.nullByte==t.nullByte&&s.nullHalf==t.nullHalf&&i.user==j.user&&i.byte==j.byte&&i.audienceLifecycle.user==j.audienceLifecycle.user&&i.audienceLifecycle.event==j.audienceLifecycle.event&&m.user==n.user&&m.word==n.word&&m.half==n.half&&m.storeWord==n.storeWord&&m.storeHalf==n.storeHalf&&m.storeByte==n.storeByte&&m.flushBlock==n.flushBlock&&m.systemCall==n.systemCall;
}
struct Running{bool&v;explicit Running(bool&b):v(b){v=true;}~Running(){v=false;}};
}
struct SourceStringFontCursor::State {
 SourceStringFontServices services;SourceMatrixFontText text;BattleActorServices actors;
 const SourceMatrixFontText*borrowedText=nullptr;const BattleActMatrix*borrowedBase=nullptr;
 BattleActMatrix base{},transformed{};bool captured=false,invalid=false;
 uint32_t phase=0,pending=0,camera=0,offset=0,code=0,width=0,saved=0,color=0,integerX=0;
 uint16_t glyph=0;float pen=0;uint32_t signedHi=0,signedLo=0;
 std::optional<BattleAudienceMatrixRequest>matrix;
 std::optional<SourceMatrixFontGlyph>glyphRequest;
 bool valid(){if(invalid)return false;const auto&s=services.common;if(!s.current||!s.current(s.user))invalid=true;return !invalid;}
 uint32_t word(uint32_t a){if(!valid())return 0;const auto&m=services.common.actors.memory;auto v=m.word(m.user,a);return valid()?v:0;}
 uint8_t byte(uint32_t a){if(!valid())return 0;const auto&s=services.common.actors;auto v=s.byte(s.user,a);return valid()?v:0;}
 uint16_t half(uint32_t a){if(!valid())return 0;const auto&m=services.common.actors.memory;auto v=m.half(m.user,a);return valid()?v:0;}
 void store(uint32_t a,uint32_t v){if(!valid())return;const auto&m=services.common.actors.memory;m.storeWord(m.user,a,v);valid();}
 void bind(){actors=services.common.actors;actors.user=this;actors.byte=[](void*p,uint32_t a){return static_cast<State*>(p)->byte(a);};actors.memory.user=this;actors.memory.word=[](void*p,uint32_t a){return static_cast<State*>(p)->word(a);};actors.memory.half=[](void*p,uint32_t a){return static_cast<State*>(p)->half(a);};}
 std::optional<uint8_t>read(uint32_t pc){pending=pc;const auto v=text.byte(text.user,offset);return valid()?v:std::nullopt;}
 bool tev(){pending=0x802b8ab8;const auto&s=services.common;if(!s.tevColor)return false;const bool done=s.tevColor(s.user,1,color);return valid()&&done;}
 bool convert(uint32_t pc){pending=pc;const auto&s=services.common;if(!s.truncate)return false;const auto v=s.truncate(s.user,pen);if(!valid()||!v)return false;integerX=uint32_t(*v);return true;}
 bool emit(int32_t x,int32_t y){pending=0x80074720;if(!glyphRequest)glyphRequest=SourceMatrixFontGlyph{glyph,code,x,y};const auto&s=services.common;if(!s.glyph)return false;const bool done=s.glyph(s.user,*glyphRequest);if(!valid()||!done)return false;glyphRequest.reset();return true;}
};
SourceStringFontCursor::SourceStringFontCursor()=default;SourceStringFontCursor::~SourceStringFontCursor()=default;
bool SourceStringFontCursor::active()const{return bool(state_);}bool SourceStringFontCursor::stale()const{return state_&&state_->invalid;}uint32_t SourceStringFontCursor::pending()const{return state_?state_->pending:0;}bool SourceStringFontCursor::reset(){if(invoking_)return false;state_.reset();return true;}
bool resumeSourceStringFont(const BattleActMatrix&base,const SourceMatrixFontText&text,const SourceStringFontServices&s,SourceStringFontCursor&cursor){
 const auto&common=s.common;const auto&m=common.actors.memory;
 if(cursor.invoking_||!common.lifetime||!text.lifetime||!common.current||!text.byte||!m.word||!m.half||!m.storeWord||!common.actors.byte)return false;
 if(cursor.state_){const auto&c=*cursor.state_;if(c.borrowedBase!=&base||c.borrowedText!=&text||(c.captured&&std::memcmp(c.base.data(),base.data(),sizeof(base)))||!same(c.services,s)||c.text.user!=text.user||c.text.byte!=text.byte||c.text.half!=text.half||!owner(c.text.lifetime,text.lifetime))return false;}
 Running running(cursor.invoking_);if(!cursor.state_){cursor.state_=std::make_unique<SourceStringFontCursor::State>();auto&c=*cursor.state_;c.services=s;c.text=text;c.borrowedText=&text;c.borrowedBase=&base;c.bind();}
 auto&c=*cursor.state_;
 for(;;){if(!c.valid())return false;switch(c.phase){
 case 0:{c.pending=0x8000f2e4;if(!common.camera)return false;const auto v=common.camera(common.user);if(!c.valid()||!v)return false;c.camera=*v;c.phase=1;continue;}
 case 1:{c.pending=0x8007554c;if(!s.width)return false;const auto v=s.width(common.user,c.text,0);if(!c.valid()||!v)return false;c.width=*v&65535;c.phase=2;continue;}
 case 2:{const auto hi=c.word(0x802c2b90),lo=c.word(0x802c2b94);const float half=real(c.word(0x8042041c));c.pen=mul(half,-converted(hi,lo,c.width,false));c.phase=3;continue;}
 case 3:{c.pending=0x8029dd64;if(!c.matrix){c.matrix=BattleAudienceMatrixRequest{};c.matrix->function=0x8029dd64;for(unsigned i=0;i<12;++i)c.matrix->left[i]=real(c.word(c.camera+0x11c+4*i));c.base=*c.borrowedBase;c.captured=true;c.matrix->right=c.base;}if(!common.matrix)return false;const bool done=common.matrix(common.user,*c.matrix);if(!c.valid()||!done)return false;c.transformed=c.matrix->output;c.matrix.reset();c.phase=4;continue;}
 case 4:{c.pending=0x802ba2fc;if(!common.loadMatrix)return false;const bool done=common.loadMatrix(common.user,c.transformed,0);if(!c.valid()||!done)return false;c.signedHi=c.word(0x802c2b98);c.signedLo=c.word(0x802c2b9c);c.phase=5;continue;}
 case 5:{const auto v=c.read(0x80076cac);if(!v)return false;if(!*v){cursor.state_.reset();return true;}c.code=*v;c.phase=sourceMatrixFontLead(c.code,c.actors)?6:7;continue;}
 case 6:{c.pending=0x80076abc;if(!c.text.half)return false;const auto v=c.text.half(c.text.user,c.offset);if(!c.valid()||!v)return false;c.code=*v;c.glyph=sourceMatrixFontMap(c.code,c.actors);c.offset+=2;c.phase=8;continue;}
 case 7:{const auto v=c.read(0x80076ad0);if(!v)return false;c.code=*v;++c.offset;c.glyph=sourceMatrixFontMap(c.code,c.actors);c.phase=8;continue;}
 case 8:if(c.word(0x8041e9a4)){const auto palette=c.word(0x80416010);c.saved=c.word(0x8041e9ac);const auto alpha=c.byte(0x8041e9a8);c.store(0x8041e9ac,palette);c.color=(palette&0xffffff00)|((palette&255)*alpha/255);c.phase=9;}else c.phase=18;continue;
 case 9:if(!c.tev())return false;c.phase=10;continue;
 case 10:if(!c.convert(0x80076b44))return false;c.integerX=uint32_t(sh(c.integerX));c.phase=11;continue;
 case 11:if(!c.emit(sh(c.integerX-2),0))return false;c.phase=12;continue;
 case 12:if(!c.emit(sh(c.integerX+2),0))return false;c.phase=13;continue;
 case 13:if(!c.convert(0x80076b7c))return false;c.integerX=uint32_t(sh(c.integerX));c.phase=14;continue;
 case 14:if(!c.emit(sh(c.integerX),-2))return false;c.phase=15;continue;
 case 15:if(!c.emit(sh(c.integerX),2))return false;{const auto alpha=c.byte(0x8041e9a8);c.store(0x8041e9ac,c.saved);c.color=(c.saved&0xffffff00)|((c.saved&255)*alpha/255);}c.phase=16;continue;
 case 16:if(!c.tev())return false;c.phase=18;continue;
 case 18:if(!c.convert(0x80076bf8))return false;c.phase=19;continue;
 case 19:if(!c.emit(sw(c.integerX),0))return false;c.phase=20;continue;
 case 20:{const auto entry=sourceMatrixFontWidth(c.glyph,c.actors);const auto gp=c.word(0x80415fd0);const auto width=c.byte(entry+1);const auto language=c.word(gp+0x16c);const auto spacing=c.word(0x8041601c+4*language);c.pen=add(c.pen,converted(c.signedHi,c.signedLo,spacing+width,true));c.phase=5;continue;}
 default:return false;
 }}
}
}
