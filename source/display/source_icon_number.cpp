#include "source_icon_number.hpp"
#include <cmath>
#include <cstring>
#include <limits>
namespace ttyd {
namespace {
float real(uint32_t n){float f;std::memcpy(&f,&n,4);return f;}
bool same(const SourceIconMatrixServices& a,const SourceIconMatrixServices& b){
 return a.user==b.user && a.word==b.word && a.half==b.half && a.currentCamera==b.currentCamera &&
     a.matrix==b.matrix && a.draw==b.draw && a.gx==b.gx;
}
bool same(const SourceIconGXServices& a,const SourceIconGXServices& b){
 return a.user==b.user && a.word==b.word && a.half==b.half && a.matrix==b.matrix && a.call==b.call && a.quad==b.quad;
}
float converted(int32_t n,double bias){
 const uint64_t bits=(uint64_t(0x43300000)<<32)|(uint32_t(n)^0x80000000u);
 double d;std::memcpy(&d,&bits,8);return float(d-bias);
}
enum class Phase {Spacing,Digits,BiasHigh,BiasLow,Zero,Translation,Concat,Color,Digit,Draw,Advance};
}
struct SourceIconNumberCursor::State {
 SourceIconNumberServices owner;SourceIconGXServices gx;
 const CameraViewMatrix* base=nullptr;const uint32_t* rgba=nullptr;
 int32_t number=0,total=0;uint32_t mode=0,biasHigh=0;
 float spacing=0;double bias=0;
 std::array<int32_t,5> digits{};unsigned count=0,index=0;
 Phase phase=Phase::Spacing;bool mathPending=false;
 BattleAudienceMatrixRequest math;BattleActMatrix translated{},combined{};
 BattleAudienceIconRequest icon;SourceIconMatrixCursor child;
};
SourceIconNumberCursor::SourceIconNumberCursor()=default;
SourceIconNumberCursor::~SourceIconNumberCursor(){reset();}
bool SourceIconNumberCursor::active() const{return bool(state_);}
void SourceIconNumberCursor::reset(){delete state_;state_=nullptr;}
bool resumeSourceIconNumber(const CameraViewMatrix& base,int32_t number,uint32_t mode,
 const uint32_t& rgba,const SourceIconNumberServices& s,SourceIconNumberCursor& cursor,int32_t& width){
 if(cursor.state_){const auto& c=*cursor.state_;
  if(c.base!=&base || c.rgba!=&rgba || c.number!=number || c.mode!=mode || !same(c.owner.icons,s.icons) ||
      (s.icons.gx && !same(c.gx,*s.icons.gx)))return false;
 }else{
  cursor.state_=new SourceIconNumberCursor::State;auto& c=*cursor.state_;
  c.owner=s;if(s.icons.gx)c.gx=*s.icons.gx;c.base=&base;c.rgba=&rgba;c.number=number;c.mode=mode;
 }
 auto& c=*cursor.state_;const auto& i=s.icons;
 const auto word=[&](uint32_t a){return i.word?i.word(i.user,a):std::optional<uint32_t>{};};
 for(;;)switch(c.phase){
 case Phase::Spacing:{const auto n=word(mode==0?0x8041f880:0x8041f884);if(!n)return false;
  c.spacing=real(*n);c.phase=Phase::Digits;break;}
 case Phase::Digits:{
  int32_t remainder=c.number;const int32_t highest=remainder/100000;remainder-=highest*100000;
  for(unsigned j=5;j>0;--j){int32_t divisor=1;for(unsigned k=1;k<j;++k)divisor*=10;
   c.digits[j-1]=remainder/divisor;remainder-=c.digits[j-1]*divisor;}
  c.count=highest?5:1;if(!highest)for(unsigned j=5;j>1;--j)if(c.digits[j-1]){c.count=j;break;}
  c.phase=Phase::BiasHigh;break;}
 case Phase::BiasHigh:{const auto n=word(0x802bf848);if(!n)return false;c.biasHigh=*n;c.phase=Phase::BiasLow;break;}
 case Phase::BiasLow:{const auto n=word(0x802bf84c);if(!n)return false;
  const uint64_t bits=(uint64_t(c.biasHigh)<<32)|*n;std::memcpy(&c.bias,&bits,8);c.phase=Phase::Zero;break;}
 case Phase::Zero:{const auto n=word(0x8041f87c);if(!n)return false;
  c.math={};c.math.function=0x8029e230;c.math.xyz={converted(-int32_t(c.index),c.bias)*c.spacing,real(*n),real(*n)};
  c.mathPending=true;c.phase=Phase::Translation;break;}
 case Phase::Translation:
  if(!i.matrix || !i.matrix(i.user,c.math))return false;
  c.translated=c.math.output;c.mathPending=false;c.phase=Phase::Concat;break;
 case Phase::Concat:
  if(!c.mathPending){c.math={};c.math.function=0x8029dd64;c.math.right=c.translated;
   for(unsigned r=0;r<3;++r)for(unsigned col=0;col<4;++col){c.math.left[4*r+col]=(*c.base)[r][col];}
   c.mathPending=true;}
  if(!i.matrix || !i.matrix(i.user,c.math))return false;
  c.combined=c.math.output;c.mathPending=false;c.phase=Phase::Color;break;
 case Phase::Color:c.icon={};c.icon.function=0x8001a4bc;c.icon.flags=16;c.icon.matrix=c.combined;c.icon.rgba=*c.rgba;c.phase=Phase::Digit;break;
 case Phase::Digit:{
  const uint32_t address=(c.mode==0?0x80415084u:0x80415098u)+(uint32_t(c.digits[c.index])<<1);
  const auto id=i.half?i.half(i.user,address):std::optional<uint16_t>{};if(!id)return false;
  c.icon.icon=*id;c.phase=Phase::Draw;break;}
 case Phase::Draw:
  if(!resumeSourceIconMatrix(c.icon,i,c.child))return false;
  c.phase=Phase::Advance;break;
 case Phase::Advance:{
  const float n=converted(c.total,c.bias)+c.spacing;
  if(!std::isfinite(n) || double(n)<std::numeric_limits<int32_t>::min() || double(n)>std::numeric_limits<int32_t>::max())return false;
  c.total=int32_t(std::trunc(n));++c.index;
  if(c.index==c.count){width=c.total;cursor.reset();return true;}
  c.phase=Phase::Zero;break;}
 }
}
}
