#include "battle_weapon_source_font.hpp"
#include <cstring>
#include <limits>
namespace ttyd {namespace {
// Original US entry points. These remain source identities, not native GPU IDs.
constexpr uint32_t JutDrawStart=0x80074bfc;
constexpr uint32_t DisableOutline=0x80077aa8;
constexpr uint32_t EnableOutline=0x80077ab4;
constexpr uint32_t DefaultStart=0x80077b4c;
constexpr uint32_t DrawColor=0x800777e0;
constexpr uint32_t PaletteColor=0x80077844;
constexpr uint32_t UniformScale=0x80077758;
constexpr uint32_t MessageWidth=0x80075528;
constexpr uint32_t CenteredString=0x80076a10;
constexpr uint32_t MatrixString=0x80076cdc;
constexpr uint32_t ScalarString=0x800773d0;
constexpr uint64_t GuestAddressSpaceSize=uint64_t{1}<<32;
struct Guard{bool&v;explicit Guard(bool&b):v(b){v=true;}~Guard(){v=false;}};
bool sameText(const std::optional<SourceMatrixFontText>&a,const std::optional<SourceMatrixFontText>&b){return bool(a)==bool(b)&&(!a||(a->user==b->user&&a->lifetime==b->lifetime&&!a->lifetime.owner_before(b->lifetime)&&!b->lifetime.owner_before(a->lifetime)&&a->byte==b->byte&&a->half==b->half));}
bool same(const BattleWeaponSourceFontCall&a,const BattleWeaponSourceFontCall&b){return sameText(a.sourceText,b.sourceText)&& a.function==b.function&&a.integers==b.integers&&a.privateText==b.privateText&&a.reals.size()==b.reals.size()&&(a.reals.empty()||!std::memcmp(a.reals.data(),b.reals.data(),a.reals.size()*sizeof(float)))&&bool(a.matrix)==bool(b.matrix)&&(!a.matrix||!std::memcmp(a.matrix->data(),b.matrix->data(),sizeof(BattleActMatrix)));}
}
struct BattleWeaponSourceFont::State {
 BattleWeaponSourceFontBinding b;BattleFontGlyphSink sink;BattleFontGxState gx;
 SourceFontStateServices stateServices;SourceFontDrawStartServices startServices;
 SourceMatrixFontServices common;SourceFontWidthServices widthServices;
 SourceStringFontServices stringServices;SourceScalarFontServices scalarServices;
 SourceFontStateCursor stateCursor;SourceFontDrawStartCursor startCursor;
 SourceFontWidthCursor widthCursor;SourceMatrixFontCursor matrixCursor;
 SourceStringFontCursor stringCursor;SourceScalarFontCursor scalarCursor;
 std::optional<BattleWeaponSourceFontCall>held;SourceMatrixFontText text;
 uint32_t color=0;bool invalid=false;
 bool source(){if(invalid)return false;if(!b.sourceCurrent()||!b.publicationCurrent()||!b.font.current())invalid=true;return !invalid;}
 std::optional<uint8_t>textByte(uint32_t offset){
  if(!source()||!held)return {};
  if(held->sourceText){const auto&t=*held->sourceText;const auto v=t.byte?t.byte(t.user,offset):std::nullopt;return source()?v:std::nullopt;}
  if(held->privateText)return offset<held->privateText->size()?std::optional<uint8_t>((*held->privateText)[offset]):std::nullopt;
  if(held->integers.size()!=1||uint64_t(held->integers[0])+offset>UINT32_MAX)return {};
  const auto v=b.byte(held->integers[0]+offset);return source()?v:std::nullopt;
 }
 std::optional<uint16_t>textHalf(uint32_t offset){
  if(!source()||!held)return {};
  if(held->sourceText){const auto&t=*held->sourceText;const auto v=t.half?t.half(t.user,offset):std::nullopt;return source()?v:std::nullopt;}
  if(held->privateText){if(uint64_t(offset)+2>held->privateText->size())return {};return uint16_t(uint16_t((*held->privateText)[offset])<<8|(*held->privateText)[offset+1]);}
  if(held->integers.size()!=1||uint64_t(held->integers[0])+offset+2>GuestAddressSpaceSize)return {};
  const auto v=b.half(held->integers[0]+offset);return source()?v:std::nullopt;
 }
 bool gxRequest(const SourceFontStateCall&q){
  if(!source())return false;
  if(q.function==JutDrawStart){if(!q.color||!q.integers.empty()||q.matrix)return false;return resumeSourceFontDrawStart(*q.color,startServices,startCursor);}
  if(!gx.request(q))return false;
  b.gx->accepted(q);return source();
 }
 bool bind(){
  common.actors=b.actors;common.user=this;common.lifetime=b.resources;common.current=[](void*p){return static_cast<State*>(p)->source();};
  common.camera=[](void*p){auto&s=*static_cast<State*>(p);return s.source()?s.b.camera():std::nullopt;};
  common.matrix=[](void*p,BattleAudienceMatrixRequest&q){auto&s=*static_cast<State*>(p);return s.source()&&s.b.matrix(q)&&s.source();};
  common.loadMatrix=[](void*p,const BattleActMatrix&m,uint32_t index){auto&s=*static_cast<State*>(p);return s.source()&&s.gx.loadMatrix(m,index)&&s.source();};
  common.tevColor=[](void*p,uint32_t index,uint32_t rgba){auto&s=*static_cast<State*>(p);return s.source()&&s.gx.tevColor(index,rgba)&&s.source();};
  common.glyph=[](void*p,const SourceMatrixFontGlyph&q){auto&s=*static_cast<State*>(p);return s.source()&&s.sink.glyph(q)&&s.source();};
  common.truncate=[](void*p,float v){auto&s=*static_cast<State*>(p);return s.source()?s.b.truncate(v):std::nullopt;};
  common.nullByte=[](void*p,uint32_t offset){auto&s=*static_cast<State*>(p);return s.source()?s.b.byte(offset):std::nullopt;};
  common.nullHalf=[](void*p,uint32_t offset){auto&s=*static_cast<State*>(p);return s.source()?s.b.half(offset):std::nullopt;};
  text.user=this;text.lifetime=b.resources;text.byte=[](void*p,uint32_t a){return static_cast<State*>(p)->textByte(a);};text.half=[](void*p,uint32_t a){return static_cast<State*>(p)->textHalf(a);};
  stateServices.actors=b.actors;stateServices.user=this;stateServices.lifetime=b.resources;stateServices.current=common.current;stateServices.matrix=common.matrix;
  stateServices.request=[](void*p,const SourceFontStateCall&q){return static_cast<State*>(p)->gxRequest(q);};
  startServices.common=stateServices;startServices.camera=common.camera;startServices.fog=[](void*p,const SourceFontDrawStartFog&q){auto&s=*static_cast<State*>(p);if(!s.source()||!s.gx.fog(q))return false;s.b.gx->accepted(q);return s.source();};
  widthServices.common=common;widthServices.fma=[](void*p,float a,float c,float d){auto&s=*static_cast<State*>(p);return s.source()?s.b.fma(a,c,d):std::nullopt;};
  widthServices.call=[](void*p,const SourceFontWidthCall&q)->std::optional<int32_t>{auto&s=*static_cast<State*>(p);return s.source()&&s.b.widthCall?s.b.widthCall(s.b.widthUser,q):std::nullopt;};
  stringServices.common=common;stringServices.width=[](void*p,const SourceMatrixFontText&t,uint32_t lines){auto&s=*static_cast<State*>(p);return s.source()?resumeSourceFontWidthLine(t,lines,s.widthServices,s.widthCursor):std::nullopt;};
  scalarServices.common=common;scalarServices.fma=widthServices.fma;
  BattleFontGlyphBinding out;out.font=b.font;out.actors=b.actors;out.resources=b.resources;out.capacity=b.capacity;out.raster=b.raster;out.sourceCurrent=[this]{return source();};out.publicationCurrent=b.publicationCurrent;
  out.ordinaryProfile=[this]{return source()&&gx.ordinaryProfile()&&b.ordinaryCurrent();};out.random=b.random;out.orderedUser=b.orderedUser;out.orderedAdmit=b.reserve;out.orderedEmit=b.commit;
  if(!sink.bind(std::move(out)))return false;
  BattleFontGxBinding g;g.sink=&sink;g.font=b.font;g.resources=b.resources;g.sourceCurrent=[this]{return source();};g.publicationCurrent=b.publicationCurrent;g.staticTextureCurrent=b.staticTextureCurrent;return gx.bind(std::move(g));
 }
 bool shape(const BattleWeaponSourceFontCall&q){
  const bool plain=!q.matrix&&!q.privateText&&!q.sourceText;
  const unsigned domains=unsigned(bool(q.privateText))+unsigned(bool(q.sourceText))+unsigned(q.integers.size()==1);
  const bool text=domains==1&&(q.privateText||q.sourceText?q.integers.empty():q.integers.size()==1)&&(!q.sourceText||(q.sourceText->lifetime&&q.sourceText->byte));
  switch(q.function){
   case DisableOutline:case EnableOutline:case DefaultStart:return plain&&q.integers.empty()&&q.reals.empty();
   case DrawColor:case PaletteColor:return plain&&q.integers.size()==1&&q.reals.empty();
   case UniformScale:return plain&&q.integers.empty()&&q.reals.size()==1;
   case MessageWidth:return !q.matrix&&text&&q.reals.empty();
   case CenteredString:case MatrixString:return q.matrix&&q.reals.empty()&&text;
   case ScalarString:return !q.matrix&&q.reals.size()==2&&text;
   default:return false;
  }
 }
 std::optional<uint32_t>resume(){
  const auto&q=*held;
  if(q.function==MessageWidth)return resumeSourceFontWidth(text,widthServices,widthCursor);
  if(q.function==CenteredString)return resumeSourceStringFont(*q.matrix,text,stringServices,stringCursor)?std::optional<uint32_t>(0):std::nullopt;
  if(q.function==MatrixString)return resumeSourceMatrixFont(*q.matrix,text,common,matrixCursor)?std::optional<uint32_t>(0):std::nullopt;
  if(q.function==ScalarString)return resumeSourceScalarFont(q.reals[0],q.reals[1],text,scalarServices,scalarCursor)?std::optional<uint32_t>(0):std::nullopt;
  SourceFontStateInput input;input.function=q.function;
  if(q.function==DrawColor)input.privateColor=&color;
  if(q.function==PaletteColor)input.argument=q.integers[0];
  if(q.function==UniformScale)std::memcpy(&input.scaleBits,&q.reals[0],4);
  return resumeSourceFontState(input,stateServices,stateCursor);
 }
};
BattleWeaponSourceFont::~BattleWeaponSourceFont()=default;
std::unique_ptr<BattleWeaponSourceFont>BattleWeaponSourceFont::create(BattleWeaponSourceFontBinding b){
 if(!b.resources||!b.font.current()||!b.capacity||!b.sourceCurrent||!b.publicationCurrent||!b.byte||!b.half||!b.camera||!b.matrix||!b.truncate||!b.fma||!b.staticTextureCurrent||!b.ordinaryCurrent||!b.random||!b.reserve||!b.commit||!b.gx)return {};
 auto result=std::unique_ptr<BattleWeaponSourceFont>(new BattleWeaponSourceFont);result->state_=std::make_unique<State>();result->state_->b=std::move(b);if(!result->state_->source()||!result->state_->bind())return {};return result;
}
std::optional<uint32_t>BattleWeaponSourceFont::request(const BattleWeaponSourceFontCall&q){
 if(invoking_||!state_)return {};
 Guard guard(invoking_);auto&s=*state_;if(!s.source()||!s.shape(q)||(s.held&&!same(*s.held,q)))return {};
 if(!s.held){s.held=q;if(q.function==DrawColor)s.color=q.integers[0];}
 const auto value=s.resume();if(value&&s.source())s.held.reset();else return {};return value;
}
uint32_t BattleWeaponSourceFont::pending()const{if(!state_)return 0;const auto&s=*state_;for(auto n:{s.startCursor.pending(),s.stateCursor.pending(),s.widthCursor.pending(),s.matrixCursor.pending(),s.stringCursor.pending(),s.scalarCursor.pending()})if(n)return n;return 0;}
void BattleWeaponSourceFont::retire(){if(!state_)return;state_->invalid=true;state_->gx.retire();state_->sink.retire();}
}
