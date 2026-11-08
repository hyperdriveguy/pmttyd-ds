#include "source_mario_set_direction.hpp"
#include <cstring>
#include <tuple>
namespace ttyd {namespace {
constexpr uint32_t MarioPointer=0x8041e900,GlobalPointer=0x80415fd0,Revise=0x80096564,ToMove=0x800961f8;
constexpr uint32_t Zero=0x80420a60,Full=0x80420a68,CameraBasis=0x80420a90,PositiveRound=0x80420a88,NegativeRound=0x80420a8c,Quantum=0x80420a94;
constexpr uint32_t WrapperBasis=0x80421b1c,WrapperFull=0x80421b44,WrapperZero=0x80421adc;
float scalar(uint32_t w){float f;std::memcpy(&f,&w,4);return f;}uint32_t bits(float f){uint32_t w;std::memcpy(&w,&f,4);return w;}
int64_t signedWord(uint32_t w){return w<=0x7fffffff?int64_t(w):int64_t(w)-0x100000000LL;}
int64_t floorShift(int64_t v,unsigned n){const int64_t scale=int64_t(1)<<n;return v>=0?v/scale:-((-v+scale-1)/scale);}
struct Guard{bool&v;explicit Guard(bool&b):v(b){v=true;}~Guard(){v=false;}};
bool same(const SourceMarioDirectionServices&a,const SourceMarioDirectionServices&b){return a.lifetime.get()==b.lifetime.get()&&!a.lifetime.owner_before(b.lifetime)&&!b.lifetime.owner_before(a.lifetime)&&std::tie(a.user,a.current,a.word,a.doubleWord,a.storeWord,a.eventFloat,a.eventValue,a.number,a.truncate)==std::tie(b.user,b.current,b.word,b.doubleWord,b.storeWord,b.eventFloat,b.eventValue,b.number,b.truncate);}
struct Binding {
 SourceMarioDirectionServices s;bool bound=false,poison=false;uint32_t pending=0;
 bool admit(const SourceMarioDirectionServices&v){if(poison)return false;if(!bound){if(!v.lifetime||!v.current)return false;s=v;bound=true;}else if(!same(s,v))return false;return current();}
 bool current(){if(poison||!s.current(s.user)){poison=true;return false;}return true;}
 bool word(uint32_t a,uint32_t&out){pending=a;if(!s.word)return false;auto v=s.word(s.user,a);if(!v)return false;out=*v;return true;}
 bool load(uint32_t a,float&out){uint32_t w=0;if(!word(a,w))return false;out=scalar(w);return true;}
 bool wide(uint32_t a,uint64_t&out){pending=a;if(!s.doubleWord)return false;auto v=s.doubleWord(s.user,a);if(!v)return false;out=*v;return true;}
 bool store(uint32_t a,uint32_t v){pending=a;return s.storeWord&&s.storeWord(s.user,a,v);}
 bool num(SourceMarioDirectionNumberKind kind,uint32_t pc,std::array<float,3>values,float&out,uint32_t w=0,uint64_t bias=0){pending=pc;if(!s.number)return false;SourceMarioDirectionNumber q{kind,pc,values,w,bias};auto v=s.number(s.user,q);if(!v)return false;out=*v;return true;}
};using N=SourceMarioDirectionNumberKind;
}
struct SourceMarioDirectionHelper::State:Binding {
 enum Phase{Start,Pointer,Half,Camera,Subtract,LoadZero,Add,SelectRound,QuantumRead,Fma,Bias,FullRead,Truncate,ZeroRead,Convert,Divide,LowTest,LowAdd,FreshFull,HighTest,HighSubtract,Done};Phase phase=Start;
 bool args=false;uint32_t function=0,actor=0,integer=0;uint64_t bias=0;float input=0,value=0,full=0,zero=0,half=0,camera=0,round=0,quantum=0,delta=0;
};
SourceMarioDirectionHelper::SourceMarioDirectionHelper():state_(std::make_unique<State>()){}SourceMarioDirectionHelper::~SourceMarioDirectionHelper()=default;
bool SourceMarioDirectionHelper::complete()const{return state_->phase==State::Done;}uint32_t SourceMarioDirectionHelper::pending()const{return state_->pending;}
bool SourceMarioDirectionHelper::reset(){if(invoking_||(state_->bound&&!complete()))return false;state_=std::make_unique<State>();return true;}
std::optional<float>SourceMarioDirectionHelper::resume(uint32_t function,float value,const SourceMarioDirectionServices&s){
 if(invoking_)return {};Guard guard(invoking_);auto&c=*state_;if(function!=Revise&&function!=ToMove)return {};if(!c.admit(s))return {};
 if(!c.args){c.function=function;c.input=value;c.value=value;c.args=true;}else if(c.function!=function||bits(c.input)!=bits(value))return {};
 for(unsigned visits=0;;++visits){if(visits==4096)return {};if(!c.current())return {};switch(c.phase){
 case State::Start:c.phase=function==ToMove?State::Pointer:State::FullRead;break;
 case State::Pointer:if(!c.word(MarioPointer,c.actor))return {};c.phase=State::Half;break;
 case State::Half:if(!c.load(CameraBasis,c.half))return {};c.phase=State::Camera;break;
 case State::Camera:if(!c.load(c.actor+0x19c,c.camera))return {};c.phase=State::Subtract;break;
 case State::Subtract:if(!c.num(N::Subtract,0x8009621c,{c.half,c.input,0},c.delta))return {};c.phase=State::LoadZero;break;
 case State::LoadZero:if(!c.load(Zero,c.zero))return {};c.phase=State::Add;break;
 case State::Add:if(!c.num(N::Add,0x80096224,{c.delta,c.camera,0},c.value))return {};c.phase=State::SelectRound;break;
 case State::SelectRound:if(!c.load(c.value>=c.zero?PositiveRound:NegativeRound,c.round))return {};c.phase=State::QuantumRead;break;
 case State::QuantumRead:if(!c.load(Quantum,c.quantum))return {};c.phase=State::Fma;break;
 case State::Fma:if(!c.num(N::FusedMultiplyAdd,0x80096250,{c.quantum,c.value,c.round},c.value))return {};c.phase=State::Bias;break;
 case State::Bias:if(!c.wide(0x802c3ec0,c.bias))return {};c.phase=State::FullRead;break;
 case State::FullRead:if(!c.load(Full,c.full))return {};c.phase=function==ToMove?State::Truncate:State::ZeroRead;break;
 case State::Truncate:{c.pending=0x8009625c;if(!s.truncate)return {};auto v=s.truncate(s.user,c.value);if(!v)return {};c.integer=uint32_t(*v);c.phase=State::ZeroRead;break;}
 case State::ZeroRead:if(!c.load(Zero,c.zero))return {};c.phase=function==ToMove?State::Convert:State::LowTest;break;
 case State::Convert:if(!c.num(N::SignedWordMinusDouble,0x80096278,{},c.value,c.integer,c.bias))return {};c.phase=State::Divide;break;
 case State::Divide:if(!c.num(N::Divide,0x8009627c,{c.value,c.quantum,0},c.value))return {};c.phase=State::LowTest;break;
 case State::LowTest:c.phase=c.value<c.zero?State::LowAdd:State::FreshFull;break;
 case State::LowAdd:if(!c.num(N::Add,function==ToMove?0x80096284:0x80096570,{c.value,c.full,0},c.value))return {};c.phase=State::LowTest;break;
 case State::FreshFull:if(!c.load(Full,c.full))return {};c.phase=State::HighTest;break;
 case State::HighTest:c.phase=c.value>=c.full?State::HighSubtract:State::Done;break;
 case State::HighSubtract:if(!c.num(N::Subtract,function==ToMove?0x80096298:0x80096584,{c.value,c.full,0},c.value))return {};c.phase=State::HighTest;break;
 case State::Done:c.pending=0;return c.value;
 }}
}
struct SourceMarioSetDirection::State:Binding {
 enum Phase{Args,Pointer,RawAngle,Angle,RawTime,Time,Global,Hz,Frames,RawMode,Mode,First,FirstHalf,CounterZero,LatchZero,InitialAngle,SaveInitial,TargetTest,TargetFull,TargetSubtract,Latch,LatchZeroConstant,LatchTravel,LatchFacing,DurationTest,HalfForDelta,Delta,NormalizeDelta,FreshHalf,DeltaTest,FullForDelta,AdjustDelta,Counter,Initial,Bias,Difference,ConvertCounter,ConvertFrames,Ratio,Multiply,Add,Normalize,StoreAngle,ReadAngle,StoreCopy,ReadConvertAngle,ConvertAngle,StoreTravel,ReadTravel,StoreFacing,FreshCounter,StoreCounter,CounterTest,LatchOne,InstantHalf,InstantDelta,Done};Phase phase=Args;
 EventEntry*entry=nullptr;uint32_t address=0,id=0,command=0,args=0,actor=0,raw=0,global=0,hz=0,counter=0,latch=0;bool first=false,eventBound=false;uint64_t bias=0;int32_t milliseconds=0,frames=0,mode=0,result=0;
 float target=0,half=0,full=0,initial=0,angle=0,delta=0,convertedCounter=0,convertedFrames=0,ratio=0,value=0;
 SourceMarioDirectionHelper helper;
};
SourceMarioSetDirection::SourceMarioSetDirection():state_(std::make_unique<State>()){}SourceMarioSetDirection::~SourceMarioSetDirection()=default;
bool SourceMarioSetDirection::complete()const{return state_->phase==State::Done;}uint32_t SourceMarioSetDirection::pending()const{return state_->helper.pending()?state_->helper.pending():state_->pending;}
bool SourceMarioSetDirection::reset(){if(invoking_||(state_->bound&&!complete()))return false;state_=std::make_unique<State>();return true;}
std::optional<int32_t>SourceMarioSetDirection::resume(EventEntry&e,uint32_t address,bool first,const SourceMarioDirectionServices&s){
 if(invoking_)return {};Guard guard(invoking_);auto&c=*state_;if(!c.admit(s))return {};
 if(!c.eventBound){c.entry=&e;c.address=address;c.id=e.id;c.command=e.currentCommand;c.first=first;c.eventBound=true;}else if(c.entry!=&e||c.address!=address||c.id!=e.id||c.command!=e.currentCommand)return {};
 auto ef=[&]{c.pending=0x80030840;if(!s.eventFloat)return false;auto v=s.eventFloat(s.user,e,c.raw);if(!v)return false;c.target=*v;return true;};
 auto ev=[&](int32_t&out){c.pending=0x80031260;if(!s.eventValue)return false;auto v=s.eventValue(s.user,e,c.raw);if(!v)return false;out=*v;return true;};
 auto child=[&](uint32_t fn,float in,float&out){c.pending=fn;auto v=c.helper.resume(fn,in,s);if(!v)return false;out=*v;return c.helper.reset();};
 for(;;){if(!c.current())return {};switch(c.phase){
 case State::Args:if(!c.word(address+0x18,c.args))return {};c.phase=State::Pointer;break;
 case State::Pointer:if(!c.word(MarioPointer,c.actor))return {};c.phase=State::RawAngle;break;
 case State::RawAngle:if(!c.word(c.args,c.raw))return {};c.phase=State::Angle;break;
 case State::Angle:if(!ef())return {};c.phase=State::RawTime;break;
 case State::RawTime:if(!c.word(c.args+4,c.raw))return {};c.phase=State::Time;break;
 case State::Time:if(!ev(c.milliseconds))return {};c.phase=State::Global;break;
 case State::Global:if(!c.word(GlobalPointer,c.global))return {};c.phase=State::Hz;break;
 case State::Hz:if(!c.word(c.global+4,c.hz))return {};c.phase=State::Frames;break;
 case State::Frames:{uint32_t product=uint32_t(uint64_t(uint32_t(c.milliseconds))*c.hz);int64_t high=floorShift(signedWord(product)*int64_t(0x10624dd3),32);int64_t shift=floorShift(high,6);c.frames=int32_t(shift+(shift<0));c.phase=State::RawMode;break;}
 case State::RawMode:if(!c.word(c.args+8,c.raw))return {};c.phase=State::Mode;break;
 case State::Mode:if(!ev(c.mode))return {};c.phase=State::First;break;
 case State::First:c.phase=c.first?State::FirstHalf:State::Latch;break;
 case State::FirstHalf:if(!c.load(WrapperBasis,c.half))return {};c.phase=State::CounterZero;break;
 case State::CounterZero:if(!c.store(address+0x78,0))return {};c.phase=State::LatchZero;break;
 case State::LatchZero:if(!c.store(address+0x80,0))return {};c.phase=State::InitialAngle;break;
 case State::InitialAngle:if(!c.load(c.actor+0x1ac,c.initial))return {};c.phase=State::SaveInitial;break;
 case State::SaveInitial:if(!c.store(address+0x7c,bits(c.initial)))return {};c.phase=State::TargetTest;break;
 case State::TargetTest:c.phase=c.target>c.half?State::TargetFull:State::Latch;break;
 case State::TargetFull:if(!c.load(WrapperFull,c.full))return {};c.phase=State::TargetSubtract;break;
 case State::TargetSubtract:if(!c.num(N::Subtract,0x800e6e80,{c.target,c.full,0},c.target))return {};c.phase=State::Latch;break;
 case State::Latch:if(!c.word(address+0x80,c.latch))return {};c.phase=c.latch==1?State::LatchZeroConstant:State::DurationTest;break;
 case State::LatchZeroConstant:if(!c.load(WrapperZero,c.value))return {};c.result=2;c.phase=State::LatchTravel;break;
 case State::LatchTravel:if(!c.store(c.actor+0x1a4,bits(c.value)))return {};c.phase=State::LatchFacing;break;
 case State::LatchFacing:if(!c.store(c.actor+0x1a0,bits(c.value)))return {};c.phase=State::Done;break;
 case State::DurationTest:c.phase=c.frames>0?State::HalfForDelta:State::InstantHalf;break;
 case State::HalfForDelta:if(!c.load(WrapperBasis,c.half))return {};c.phase=State::Delta;break;
 case State::Delta:if(!c.num(N::Subtract,0x800e6eb0,{c.half,c.target,0},c.angle))return {};c.phase=State::NormalizeDelta;break;
 case State::NormalizeDelta:if(!child(Revise,c.angle,c.angle))return {};c.phase=State::FreshHalf;break;
 case State::FreshHalf:if(!c.load(WrapperBasis,c.half))return {};c.phase=State::DeltaTest;break;
 case State::DeltaTest:c.phase=c.angle>c.half?State::FullForDelta:State::Counter;break;
 case State::FullForDelta:if(!c.load(WrapperFull,c.full))return {};c.phase=State::AdjustDelta;break;
 case State::AdjustDelta:if(!c.num(N::Subtract,0x800e6ec8,{c.angle,c.full,0},c.angle))return {};c.phase=State::Counter;break;
 case State::Counter:if(!c.word(address+0x78,c.counter))return {};c.phase=State::Initial;break;
 case State::Initial:if(!c.load(address+0x7c,c.initial))return {};c.phase=State::Bias;break;
 case State::Bias:if(!c.wide(0x802e3eb8,c.bias))return {};c.phase=State::Difference;break;
 case State::Difference:if(!c.num(N::Subtract,0x800e6eec,{c.angle,c.initial,0},c.delta))return {};c.phase=State::ConvertCounter;break;
 case State::ConvertCounter:if(!c.num(N::SignedWordMinusDouble,0x800e6efc,{},c.convertedCounter,c.counter,c.bias))return {};c.phase=State::ConvertFrames;break;
 case State::ConvertFrames:if(!c.num(N::SignedWordMinusDouble,0x800e6f08,{},c.convertedFrames,uint32_t(c.frames),c.bias))return {};c.phase=State::Ratio;break;
 case State::Ratio:if(!c.num(N::Divide,0x800e6f0c,{c.convertedCounter,c.convertedFrames,0},c.ratio))return {};c.phase=State::Multiply;break;
 case State::Multiply:if(!c.num(N::Multiply,0x800e6f10,{c.delta,c.ratio,0},c.value))return {};c.phase=State::Add;break;
 case State::Add:if(!c.num(N::Add,0x800e6f14,{c.initial,c.value,0},c.angle))return {};c.phase=State::Normalize;break;
 case State::Normalize:if(!child(Revise,c.angle,c.angle))return {};c.phase=State::StoreAngle;break;
 case State::StoreAngle:if(!c.store(c.actor+0x1ac,bits(c.angle)))return {};c.phase=State::ReadAngle;break;
 case State::ReadAngle:if(!c.load(c.actor+0x1ac,c.value))return {};c.phase=State::StoreCopy;break;
 case State::StoreCopy:if(!c.store(c.actor+0x1b0,bits(c.value)))return {};c.phase=State::ReadConvertAngle;break;
 case State::ReadConvertAngle:if(!c.load(c.actor+0x1ac,c.value))return {};c.phase=State::ConvertAngle;break;
 case State::ConvertAngle:if(!child(ToMove,c.value,c.value))return {};c.phase=State::StoreTravel;break;
 case State::StoreTravel:if(!c.store(c.actor+0x1a4,bits(c.value)))return {};c.phase=State::ReadTravel;break;
 case State::ReadTravel:if(!c.load(c.actor+0x1a4,c.value))return {};c.phase=State::StoreFacing;break;
 case State::StoreFacing:if(!c.store(c.actor+0x1a0,bits(c.value)))return {};c.phase=c.frames>0?State::FreshCounter:State::Done;break;
 case State::FreshCounter:if(!c.word(address+0x78,c.counter))return {};c.counter+=1;c.phase=State::StoreCounter;break;
 case State::StoreCounter:if(!c.store(address+0x78,c.counter))return {};c.phase=State::CounterTest;break;
 case State::CounterTest:if(signedWord(c.counter)>c.frames){if(c.mode==0){c.result=1;c.phase=State::Done;}else c.phase=State::LatchOne;}else{c.result=0;c.phase=State::Done;}break;
 case State::LatchOne:if(!c.store(address+0x80,1))return {};c.result=0;c.phase=State::Done;break;
 case State::InstantHalf:if(!c.load(WrapperBasis,c.half))return {};c.phase=State::InstantDelta;break;
 case State::InstantDelta:if(!c.num(N::Subtract,0x800e6f74,{c.half,c.target,0},c.angle))return {};c.result=2;c.phase=State::Normalize;break;
 case State::Done:c.pending=0;return c.result;
 }}
}
}
