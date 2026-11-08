#include "battle_target_display_continuation.hpp"
#include <cstring>
namespace ttyd {namespace {
constexpr uint32_t Entry=0x80120a44,SmallData=0x80427440;
constexpr uint32_t SignedBias=0x802efe78,UnsignedBias=0x802efe80;
constexpr uint32_t CursorPtr=0x80124cb8,GetUnit=0x800f88cc,GetPart=0x80128cd8;
constexpr uint32_t Project=0x801009b4,Icon=0x8001a918,Yoshi=0x800d2db4,Message=0x80081b8c;
constexpr uint32_t Width=0x80075528,Window=0x8007b44c,FontStart=0x80077b4c,FontString=0x800773d0,Camera=0x8012c028;
constexpr uint32_t SingleTargetMask=0x01000000,AttributeMask=2,CursorIcon=503;
constexpr uint32_t TargetBase=0x428,TargetOrder=0xa6d,TargetCount=0xa6c,TargetStride=0x24;
constexpr uint32_t YoshiKind=226,LineSpacing=32,FrontIconFlags=16,AttributeIconFlags=24;
constexpr uint32_t CommandWorkOffset=0x171c,CommandTypeOffset=0x1c5c;
constexpr uint32_t CommandWeaponTable=0x80,CommandWeaponStride=0x1c,WeaponTargetsOffset=0x64;
constexpr uint32_t SelectedTargetOffset=0xedf,UnitAttributesOffset=0x104,UnitKindOffset=8,PartNameOffset=8;
struct Path {
 uint32_t project,icon,yoshi,message,width,widthSub,widthAdd,spacingSub,windowYSub,window,fontStart,textSpacingSub,textYSub,fontString;
 int32_t color;
};
constexpr Path Paths[]={
 {0x80120b48,0x80120b74,0x80120b84,0x80120b94,0x80120ba0,0x80120bd4,0x80120be4,0,0,0x80120bf0,0x80120bf4,0,0,0x80120c04,-20112},
 {0x80120c14,0x80120c40,0x80120c50,0x80120c60,0x80120c6c,0x80120ca0,0x80120cb0,0,0,0x80120cbc,0x80120cc0,0,0,0x80120cd0,-20108},
 {0x80120d90,0x80120dbc,0x80120dcc,0x80120ddc,0x80120de8,0x80120e24,0x80120e38,0x80120e40,0x80120e50,0x80120e58,0x80120e5c,0x80120e88,0x80120e8c,0x80120e90,-20104},
 {0x80120ea0,0x80120ecc,0x80120edc,0x80120eec,0x80120ef8,0x80120f34,0x80120f48,0x80120f50,0x80120f60,0x80120f68,0x80120f6c,0x80120f98,0x80120f9c,0x80120fa0,-20100}
};
int32_t signedByte(uint8_t n){return n<128?int32_t(n):int32_t(n)-256;}
int32_t signedHalf(uint16_t n){return n<32768?int32_t(n):int32_t(n)-65536;}
float scalar(uint32_t n){float f;std::memcpy(&f,&n,sizeof f);return f;}
double magic(uint32_t n,bool sign){const uint64_t raw=(uint64_t(0x43300000)<<32)|(sign?n^0x80000000u:n);double d;std::memcpy(&d,&raw,sizeof d);return d;}
bool same(const BattleTargetDisplayServices&a,const BattleTargetDisplayServices&b){return a.user==b.user&&a.current==b.current&&a.word==b.word&&a.half==b.half&&a.byte==b.byte&&a.request==b.request&&a.subtractToSingle==b.subtractToSingle&&a.add==b.add;}
enum class Step {Start,Cursor,Loop,Row,Unit,Part,Coordinate,CoordinateResult,Projected,IconDone,Name,WidthDone,WidthConverted,WidthAdded,SpacingConverted,WindowY,WindowDone,FontStarted,TextSpacing,TextY,TextDone,Done,Call,Subtract,Add};
struct Running{bool&flag;explicit Running(bool&f):flag(f){flag=true;}~Running(){flag=false;}};
}
struct BattleTargetDisplayCursor::State {
 BattleTargetDisplayServices services{};uint32_t battle=0;bool invalid=false,single=false;
 Step step=Step::Start,next=Step::Start;uint32_t pending=0,callsite=0;
 std::optional<BattleTargetDisplayRequest> request;BattleTargetDisplayReply reply{};
 uint32_t row=0,unit=0,part=0,name=0,width=0,index=0,spacing=0,coordinate=0,pathIndex=0,color=0;
 double coordinateBias=0,bias=0,left=0,right=0;float result=0,widthBase=0,windowWidth=0,originY=0,windowX=0,windowHeight=0,textX=0;
 std::array<float,3> point{};
 bool valid(){if(invalid)return false;if(!services.current(services.user)){invalid=true;return false;}return true;}
 uint32_t word(uint32_t a)const{return services.word(services.user,a);}
 int32_t half(uint32_t a)const{return signedHalf(services.half(services.user,a));}
 int32_t byte(uint32_t a)const{return signedByte(services.byte(services.user,a));}
 float constant(int32_t offset)const{return scalar(word(SmallData+uint32_t(offset)));}
 double sourceDouble(uint32_t a)const{const auto hi=word(a),lo=word(a+4);const uint64_t raw=(uint64_t(hi)<<32)|lo;double d;std::memcpy(&d,&raw,sizeof d);return d;}
 const Path&path()const{return Paths[pathIndex];}
 void call(uint32_t function,uint32_t pc,Step after,std::array<uint32_t,3> args={},BattleTargetPrivate transport=BattleTargetPrivate::None){
  request=BattleTargetDisplayRequest{};request->function=function;request->callsite=pc;request->integers=args;request->transport=transport;pending=function;callsite=pc;next=after;step=Step::Call;
 }
 void subtract(double a,double b,uint32_t pc,Step after){left=a;right=b;pending=Entry;callsite=pc;next=after;step=Step::Subtract;}
 void add(float a,float b,uint32_t pc,Step after){left=a;right=b;pending=Entry;callsite=pc;next=after;step=Step::Add;}
 void window(float y,float round){call(Window,path().window,Step::WindowDone,{0,0,0},BattleTargetPrivate::WindowColor);request->color=color;request->scalars={windowX,y,windowWidth,windowHeight,round};}
 void text(float y){call(FontString,path().fontString,Step::TextDone,{name,0,0});request->scalars={textX,y,0,0,0};}
};
BattleTargetDisplayCursor::BattleTargetDisplayCursor()=default;
BattleTargetDisplayCursor::~BattleTargetDisplayCursor()=default;
bool BattleTargetDisplayCursor::active()const{return bool(state_);}
bool BattleTargetDisplayCursor::stale()const{return state_&&state_->invalid;}
uint32_t BattleTargetDisplayCursor::pending()const{return state_?state_->pending:0;}
uint32_t BattleTargetDisplayCursor::pendingCallsite()const{return state_?state_->callsite:0;}
bool BattleTargetDisplayCursor::reset(){if(invoking_)return false;state_.reset();return true;}
bool resumePackedBattleTargetDisplay(uint32_t battle,const BattleTargetDisplayServices&s,BattleTargetDisplayCursor&cursor){
 if(cursor.invoking_||!s.current||!s.word||!s.half||!s.byte)return false;
 if(cursor.state_&&(cursor.state_->battle!=battle||!same(cursor.state_->services,s)))return false;
 Running running(cursor.invoking_);
 if(!cursor.state_){cursor.state_=std::make_unique<BattleTargetDisplayCursor::State>();cursor.state_->battle=battle;cursor.state_->services=s;}
 auto&c=*cursor.state_;
 for(;;){if(!c.valid())return false;switch(c.step){
 case Step::Start:{const auto selected=c.word(battle+CommandTypeOffset);c.call(CursorPtr,0x80120a70,Step::Cursor,{battle+CommandWorkOffset,selected,0},BattleTargetPrivate::CursorOutput);continue;}
 case Step::Cursor:{const auto ordinal=c.word(c.reply.value);const auto weapon=c.word(battle+CommandWorkOffset+CommandWeaponTable+ordinal*CommandWeaponStride);c.single=(c.word(weapon+WeaponTargetsOffset)&SingleTargetMask)!=0;
  if(c.single){c.index=uint32_t(c.byte(battle+SelectedTargetOffset));c.step=Step::Row;}else{c.coordinateBias=c.sourceDouble(SignedBias);c.step=Step::Loop;}continue;}
 case Step::Loop:
  if(int32_t(c.index)>=c.byte(battle+TargetBase+TargetCount)){c.call(Camera,0x80120fc4,Step::Done);continue;}
  c.step=Step::Row;continue;
 case Step::Row:{const auto order=c.byte(battle+TargetBase+TargetOrder+c.index);c.row=battle+TargetBase+4+uint32_t(order)*TargetStride;
  const auto id=uint32_t(c.half(c.row));c.call(GetUnit,c.single?0x80120ac0:0x80120d14,Step::Unit,{battle,id,0});continue;}
 case Step::Unit:{c.unit=c.reply.value;const auto id=uint32_t(c.half(c.row+2));c.call(GetPart,c.single?0x80120acc:0x80120d20,Step::Part,{c.unit,id,0});continue;}
 case Step::Part:c.part=c.reply.value;c.coordinate=0;c.step=Step::Coordinate;continue;
 case Step::Coordinate:{const auto value=uint32_t(c.half(c.row+4+2*c.coordinate));if(c.single&&c.coordinate==0)c.coordinateBias=c.sourceDouble(SignedBias);
  constexpr uint32_t singlePc[]={0x80120af8,0x80120b14,0x80120b2c};constexpr uint32_t allPc[]={0x80120d40,0x80120d5c,0x80120d74};
  c.subtract(magic(value,true),c.coordinateBias,c.single?singlePc[c.coordinate]:allPc[c.coordinate],Step::CoordinateResult);continue;}
 case Step::CoordinateResult:
  c.point[c.coordinate]=c.result;if(++c.coordinate<3){c.step=Step::Coordinate;continue;}
  c.pathIndex=(c.single?0:2)+((c.word(c.unit+UnitAttributesOffset)&AttributeMask)?1:0);
  c.call(Project,c.path().project,Step::Projected,{},BattleTargetPrivate::ProjectPoint);c.request->point=c.point;continue;
 case Step::Projected:{auto point=c.reply.point;point[2]=c.constant(-20072);const auto scale=c.constant(-20068);
  c.call(Icon,c.path().icon,Step::IconDone,{(c.pathIndex&1)?AttributeIconFlags:FrontIconFlags,CursorIcon,0},BattleTargetPrivate::IconPoint);c.request->point=point;c.request->scalars[0]=scale;continue;}
 case Step::IconDone:
  if(c.word(c.unit+UnitKindOffset)==YoshiKind)c.call(Yoshi,c.path().yoshi,Step::Name);
  else{const auto key=c.word(c.part+PartNameOffset);c.call(Message,c.path().message,Step::Name,{key,0,0});}continue;
 case Step::Name:c.name=c.reply.value;c.call(Width,c.path().width,Step::WidthDone,{c.name,0,0});continue;
 case Step::WidthDone:
  c.width=c.reply.value&0xffffu;
  if(c.single){c.color=c.word(SmallData+uint32_t(c.path().color));c.bias=c.sourceDouble(UnsignedBias);c.widthBase=c.constant(-20008);}
  else{c.bias=c.sourceDouble(UnsignedBias);c.color=c.word(SmallData+uint32_t(c.path().color));}
  c.subtract(magic(c.width,false),c.bias,c.path().widthSub,Step::WidthConverted);continue;
 case Step::WidthConverted:
  if(c.single){c.windowX=c.constant(-20016);c.originY=c.constant(-20012);}
  else{c.widthBase=c.constant(-20008);c.bias=c.sourceDouble(SignedBias);}
  c.add(c.widthBase,c.result,c.path().widthAdd,Step::WidthAdded);continue;
 case Step::WidthAdded:
  c.windowWidth=c.result;
  if(c.single){c.windowHeight=c.constant(-20044);const auto round=c.constant(-20084);c.window(c.originY,round);}
  else{c.originY=c.constant(-19996);c.subtract(magic(c.spacing,true),c.bias,c.path().spacingSub,Step::SpacingConverted);}continue;
 case Step::SpacingConverted:
  c.windowX=c.constant(-20016);c.windowHeight=c.constant(-20044);c.subtract(c.originY,c.result,c.path().windowYSub,Step::WindowY);continue;
 case Step::WindowY:{const auto y=c.result;const auto round=c.constant(-20084);c.window(y,round);continue;}
 case Step::WindowDone:c.call(FontStart,c.path().fontStart,Step::FontStarted);continue;
 case Step::FontStarted:
  if(c.single){c.textX=c.constant(-20004);const auto y=c.constant(-20000);c.text(y);}
  else{c.bias=c.sourceDouble(SignedBias);c.originY=c.constant(-19992);c.textX=c.constant(-20004);c.subtract(magic(c.spacing,true),c.bias,c.path().textSpacingSub,Step::TextSpacing);}continue;
 case Step::TextSpacing:c.subtract(c.originY,c.result,c.path().textYSub,Step::TextY);continue;
 case Step::TextY:c.text(c.result);continue;
 case Step::TextDone:
  if(c.single)c.call(Camera,0x80120fc4,Step::Done);
  else{++c.index;c.spacing+=LineSpacing;c.step=Step::Loop;}continue;
 case Step::Done:cursor.state_.reset();return true;
 case Step::Call:{if(!s.request)return false;const auto reply=s.request(s.user,*c.request);if(!c.valid()||!reply)return false;c.reply=*reply;c.request.reset();c.step=c.next;continue;}
 case Step::Subtract:{if(!s.subtractToSingle)return false;const auto value=s.subtractToSingle(s.user,c.left,c.right);if(!c.valid()||!value)return false;c.result=*value;c.step=c.next;continue;}
 case Step::Add:{if(!s.add)return false;const auto value=s.add(s.user,float(c.left),float(c.right));if(!c.valid()||!value)return false;c.result=*value;c.step=c.next;continue;}
 }}
}
}
