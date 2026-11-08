#include "source_mario_move_position.hpp"
#include <cstring>
#include <tuple>
namespace ttyd {
namespace {
constexpr uint32_t MarioPointer=0x8041e900,GlobalPointer=0x80415fd0;
constexpr uint32_t MaskedSpeedScale=0x80421b0c,One=0x80421ad8,SignedBias=0x802e3eb8;
constexpr uint32_t ForceFlag=0x20,MaskedSpeedFlag=0x02000000;
float scalar(uint32_t w){float f;std::memcpy(&f,&w,4);return f;}
uint32_t bits(float f){uint32_t w;std::memcpy(&w,&f,4);return w;}
struct Guard{bool&v;explicit Guard(bool&b):v(b){v=true;}~Guard(){v=false;}};
bool same(const SourceMarioMovePositionServices&a,const SourceMarioMovePositionServices&b){return a.lifetime.get()==b.lifetime.get()&&!a.lifetime.owner_before(b.lifetime)&&!b.lifetime.owner_before(a.lifetime)&&std::tie(a.user,a.current,a.word,a.half,a.doubleWord,a.storeWord,a.storeHalf,a.eventFloat,a.number,a.truncate,a.motion)==std::tie(b.user,b.current,b.word,b.half,b.doubleWord,b.storeWord,b.storeHalf,b.eventFloat,b.number,b.truncate,b.motion);}
}
struct SourceMarioMovePosition::State {
 enum Phase {Args,Parent,RawX,FloatX,RawZ,FloatZ,RawRate,FloatRate,First,
  DistX,DistZ,Distance,AngleX,AngleZ,Angle,StoreAngle,Flags,HalfConstant,HalfMultiply,
  Global,Hz,Bias,OneConstant,FreshDirection,ConvertHz,ScaleRate,OneRate,DistanceRate,AddDuration,Truncate,
  ForceParent,ForceFlags,ForceFlagStore,Face,Heading,Counter,Speed,MotionRead,MotionCheck,Dash,MotionCall,
  RestoreFace,RestoreHeading,RestoreCounter,RestoreSpeed,Poll,FinalX,FinalZ,Done};
 Phase phase=Args;SourceMarioMovePositionServices services;EventEntry*entry=nullptr;
 uint32_t address=0,id=0,command=0,args=0,parent=0,force=0,raw=0,flags=0,global=0,hz=0,pending=0;
 uint64_t bias=0;uint16_t motion=0;bool first=false,bound=false,poison=false;
 float x=0,z=0,rate=0,sx=0,sz=0,distance=0,angle=0,constant=0,one=0,direction=0,frequency=0,oneRate=0,distanceRate=0,duration=0;
 int32_t ticks=0,action=0,result=0;
};
SourceMarioMovePosition::SourceMarioMovePosition():state_(std::make_unique<State>()){}
SourceMarioMovePosition::~SourceMarioMovePosition()=default;
bool SourceMarioMovePosition::complete()const{return state_->phase==State::Done;}
uint32_t SourceMarioMovePosition::pending()const{return state_->pending;}
bool SourceMarioMovePosition::reset(){if(invoking_||(state_->bound&&!complete()))return false;state_=std::make_unique<State>();return true;}
std::optional<int32_t>SourceMarioMovePosition::resume(EventEntry&e,uint32_t address,bool first,const SourceMarioMovePositionServices&s){
 if(invoking_)return {};Guard guard(invoking_);auto&c=*state_;
 if(c.poison)return {};
 if(!c.bound){
  if(!s.lifetime||!s.current)return {};
  if(!s.current(s.user)){c.poison=true;return {};}
  c.services=s;c.entry=&e;c.address=address;c.id=e.id;c.command=e.currentCommand;c.first=first;c.bound=true;
 }else{
  if(!same(c.services,s)||c.entry!=&e||c.address!=address)return {};
  if(!s.current(s.user)){c.poison=true;return {};}
  if(c.id!=e.id||c.command!=e.currentCommand)return {};
 }
 auto word=[&](uint32_t a,uint32_t&out){c.pending=a;if(!s.word)return false;const auto v=s.word(s.user,a);if(!v)return false;out=*v;return true;};
 auto load=[&](uint32_t a,float&out){uint32_t v=0;if(!word(a,v))return false;out=scalar(v);return true;};
 auto store=[&](uint32_t a,uint32_t v){c.pending=a;return s.storeWord&&s.storeWord(s.user,a,v);};
 auto halfStore=[&](uint32_t a,uint16_t v){c.pending=a;return s.storeHalf&&s.storeHalf(s.user,a,v);};
 auto eventFloat=[&](float&out){c.pending=0x80030840;if(!s.eventFloat)return false;const auto v=s.eventFloat(s.user,e,c.raw);if(!v)return false;out=*v;return true;};
 auto number=[&](SourceMarioMoveNumberKind kind,uint32_t pc,std::array<float,4> values,float&out,uint32_t function=0){c.pending=function?function:pc;SourceMarioMoveNumber q;q.kind=kind;q.function=function;q.callsite=pc;q.values=values;q.word=c.hz;q.doubleBits=c.bias;if(!s.number)return false;const auto v=s.number(s.user,q);if(!v)return false;out=*v;return true;};
 for(;;){if(!s.current(s.user)){c.poison=true;return {};}
 switch(c.phase){
 case State::Args:if(!word(c.address+0x18,c.args))return {};c.phase=State::Parent;break;
 case State::Parent:if(!word(MarioPointer,c.parent))return {};c.phase=State::RawX;break;
 case State::RawX:if(!word(c.args,c.raw))return {};c.phase=State::FloatX;break;
 case State::FloatX:if(!eventFloat(c.x))return {};c.phase=State::RawZ;break;
 case State::RawZ:if(!word(c.args+4,c.raw))return {};c.phase=State::FloatZ;break;
 case State::FloatZ:if(!eventFloat(c.z))return {};c.phase=State::RawRate;break;
 case State::RawRate:if(!word(c.args+8,c.raw))return {};c.phase=State::FloatRate;break;
 case State::FloatRate:if(!eventFloat(c.rate))return {};c.phase=State::First;break;
 case State::First:c.phase=c.first?State::DistX:State::Poll;break;
 case State::DistX:if(!load(c.parent+0x8c,c.sx))return {};c.phase=State::DistZ;break;
 case State::DistZ:if(!load(c.parent+0x94,c.sz))return {};c.phase=State::Distance;break;
 case State::Distance:if(!number(SourceMarioMoveNumberKind::Distance,0x800e5fe8,{c.sx,c.sz,c.x,c.z},c.distance,0x8000722c))return {};c.phase=State::AngleX;break;
 case State::AngleX:if(!load(c.parent+0x8c,c.sx))return {};c.phase=State::AngleZ;break;
 case State::AngleZ:if(!load(c.parent+0x94,c.sz))return {};c.phase=State::Angle;break;
 case State::Angle:if(!number(SourceMarioMoveNumberKind::Angle,0x800e6000,{c.sx,c.sz,c.x,c.z},c.angle,0x8000703c))return {};c.phase=State::StoreAngle;break;
 case State::StoreAngle:if(!store(c.parent+0x1a4,bits(c.angle)))return {};c.phase=State::Flags;break;
 case State::Flags:if(!word(c.parent,c.flags))return {};c.phase=c.flags&MaskedSpeedFlag?State::HalfConstant:State::Global;break;
 case State::HalfConstant:if(!load(MaskedSpeedScale,c.constant))return {};c.phase=State::HalfMultiply;break;
 case State::HalfMultiply:if(!number(SourceMarioMoveNumberKind::Multiply,0x800e6018,{c.rate,c.constant,0,0},c.rate))return {};c.phase=State::Global;break;
 case State::Global:if(!word(GlobalPointer,c.global))return {};c.phase=State::Hz;break;
 case State::Hz:if(!word(c.global+4,c.hz))return {};c.phase=State::Bias;break;
 case State::Bias:{c.pending=SignedBias;if(!s.doubleWord)return {};const auto v=s.doubleWord(s.user,SignedBias);if(!v)return {};c.bias=*v;c.phase=State::OneConstant;break;}
 case State::OneConstant:if(!load(One,c.one))return {};c.phase=State::FreshDirection;break;
 case State::FreshDirection:if(!load(c.parent+0x1a4,c.direction))return {};c.phase=State::ConvertHz;break;
 case State::ConvertHz:if(!number(SourceMarioMoveNumberKind::SignedWordMinusDouble,0x800e6048,{},c.frequency))return {};c.phase=State::ScaleRate;break;
 case State::ScaleRate:if(!number(SourceMarioMoveNumberKind::Divide,0x800e604c,{c.rate,c.frequency,0,0},c.rate))return {};c.phase=State::OneRate;break;
 case State::OneRate:if(!number(SourceMarioMoveNumberKind::Divide,0x800e6050,{c.one,c.rate,0,0},c.oneRate))return {};c.phase=State::DistanceRate;break;
 case State::DistanceRate:if(!number(SourceMarioMoveNumberKind::Divide,0x800e6054,{c.distance,c.rate,0,0},c.distanceRate))return {};c.phase=State::AddDuration;break;
 case State::AddDuration:if(!number(SourceMarioMoveNumberKind::Add,0x800e6058,{c.oneRate,c.distanceRate,0,0},c.duration))return {};c.phase=State::Truncate;break;
 case State::Truncate:{c.pending=0x800e6060;if(!s.truncate)return {};const auto v=s.truncate(s.user,c.duration);if(!v)return {};c.ticks=*v;c.phase=State::ForceParent;break;}
 case State::ForceParent:if(!word(MarioPointer,c.force))return {};c.phase=State::ForceFlags;break;
 case State::ForceFlags:if(!word(c.force,c.flags))return {};c.phase=State::ForceFlagStore;break;
 case State::ForceFlagStore:if(!store(c.force,c.flags|ForceFlag))return {};c.phase=State::Face;break;
 case State::Face:if(!store(c.force+0x1a0,bits(c.direction)))return {};c.phase=State::Heading;break;
 case State::Heading:if(!store(c.force+0x1a4,bits(c.direction)))return {};c.phase=State::Counter;break;
 case State::Counter:if(!halfStore(c.force+0x4e,uint16_t(c.ticks)))return {};c.phase=State::Speed;break;
 case State::Speed:if(!store(c.force+0x180,bits(c.rate)))return {};c.phase=State::MotionRead;break;
 case State::MotionRead:{c.pending=c.force+0x2e;if(!s.half)return {};const auto v=s.half(s.user,c.pending);if(!v)return {};c.motion=*v;c.phase=State::MotionCheck;break;}
 case State::MotionCheck:c.phase=c.motion==25||c.motion==21||c.motion==22||c.motion==26?State::Poll:State::Dash;break;
 case State::Dash:if(!load(c.force+0x188,c.constant))return {};c.action=c.rate>=c.constant?2:1;c.phase=State::MotionCall;break;
 case State::MotionCall:c.pending=0x8009455c;if(!s.motion||!s.motion(s.user,c.action))return {};c.phase=State::RestoreFace;break;
 case State::RestoreFace:if(!store(c.force+0x1a0,bits(c.direction)))return {};c.phase=State::RestoreHeading;break;
 case State::RestoreHeading:if(!store(c.force+0x1a4,bits(c.direction)))return {};c.phase=State::RestoreCounter;break;
 case State::RestoreCounter:if(!halfStore(c.force+0x4e,uint16_t(c.ticks)))return {};c.phase=State::RestoreSpeed;break;
 case State::RestoreSpeed:if(!store(c.force+0x180,bits(c.rate)))return {};c.phase=State::Poll;break;
 case State::Poll:if(!word(c.parent,c.flags))return {};if(c.flags&ForceFlag){c.result=0;c.phase=State::Done;}else c.phase=State::FinalX;break;
 case State::FinalX:if(!store(c.parent+0x8c,bits(c.x)))return {};c.phase=State::FinalZ;break;
 case State::FinalZ:if(!store(c.parent+0x94,bits(c.z)))return {};c.result=2;c.phase=State::Done;break;
 case State::Done:c.pending=0;return c.result;
 }
 }
}
}
