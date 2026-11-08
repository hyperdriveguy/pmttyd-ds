#include "source_mario_force_consumer.hpp"
#include <cstring>
#include <tuple>
namespace ttyd {
namespace {
constexpr uint32_t MarioPointer=0x8041e900,ForceFlag=0x20,Zero=0x80420a60,One=0x80420a74;
constexpr uint32_t TwoPi=0x8041f458,FullDegrees=0x8041f45c;
float scalar(uint32_t w){float f;std::memcpy(&f,&w,4);return f;}uint32_t bits(float f){uint32_t w;std::memcpy(&w,&f,4);return w;}
int32_t signedHalf(uint16_t v){return v<0x8000?int32_t(v):int32_t(v)-0x10000;}
struct Guard{bool&v;explicit Guard(bool&b):v(b){v=true;}~Guard(){v=false;}};
bool same(const SourceMarioForceServices&a,const SourceMarioForceServices&b){return a.lifetime.get()==b.lifetime.get()&&!a.lifetime.owner_before(b.lifetime)&&!b.lifetime.owner_before(a.lifetime)&&std::tie(a.user,a.current,a.word,a.half,a.storeWord,a.storeHalf,a.speed,a.trig,a.number)==std::tie(b.user,b.current,b.word,b.half,b.storeWord,b.storeHalf,b.speed,b.trig,b.number);}
struct Binding {
 SourceMarioForceServices services;bool bound=false,poison=false;uint32_t pending=0;
 bool admit(const SourceMarioForceServices&s){if(poison)return false;if(!bound){if(!s.lifetime||!s.current)return false;services=s;bound=true;}else if(!same(services,s))return false;if(!s.current(s.user)){poison=true;return false;}return true;}
 bool current(){if(poison||!services.current(services.user)){poison=true;return false;}return true;}
 bool word(uint32_t a,uint32_t&out){pending=a;if(!services.word)return false;const auto v=services.word(services.user,a);if(!v)return false;out=*v;return true;}
 bool load(uint32_t a,float&out){uint32_t w=0;if(!word(a,w))return false;out=scalar(w);return true;}
 bool store(uint32_t a,uint32_t w){pending=a;return services.storeWord&&services.storeWord(services.user,a,w);}
 bool halfStore(uint32_t a,uint16_t h){pending=a;return services.storeHalf&&services.storeHalf(services.user,a,h);}
};
}
struct SourceMarioForceMoveMain::State:Binding {
 enum Phase{Parent,Flags,Count,DecrementStore,Branch,LoadZero,ClearCounter,LoadOne,FreshFlags,ClearForce,ClearSpeed,Input,Choose,Speed,StoreSpeed,Done};Phase phase=Parent;
 uint32_t parent=0,flags=0,function=0;uint16_t decrement=0;float zero=0,one=0,input=0,speed=0;
};
SourceMarioForceMoveMain::SourceMarioForceMoveMain():state_(std::make_unique<State>()){}
SourceMarioForceMoveMain::~SourceMarioForceMoveMain()=default;
bool SourceMarioForceMoveMain::complete()const{return state_->phase==State::Done;}
uint32_t SourceMarioForceMoveMain::pending()const{return state_->pending;}
bool SourceMarioForceMoveMain::reset(){if(invoking_||(state_->bound&&!complete()))return false;state_=std::make_unique<State>();return true;}
bool SourceMarioForceMoveMain::resume(const SourceMarioForceServices&s){
 if(invoking_)return false;Guard guard(invoking_);auto&c=*state_;if(!c.admit(s))return false;
 for(;;){if(!c.current())return false;switch(c.phase){
 case State::Parent:if(!c.word(MarioPointer,c.parent))return false;c.phase=State::Flags;break;
 case State::Flags:if(!c.word(c.parent,c.flags))return false;c.phase=c.flags&ForceFlag?State::Count:State::Done;break;
 case State::Count:{c.pending=c.parent+0x4e;if(!s.half)return false;const auto v=s.half(s.user,c.pending);if(!v)return false;c.decrement=uint16_t(signedHalf(*v)-1);c.phase=State::DecrementStore;break;}
 case State::DecrementStore:if(!c.halfStore(c.parent+0x4e,c.decrement))return false;c.phase=State::Branch;break;
 case State::Branch:c.phase=signedHalf(c.decrement)>0?State::Done:State::LoadZero;break;
 case State::LoadZero:if(!c.load(Zero,c.zero))return false;c.phase=State::ClearCounter;break;
 case State::ClearCounter:if(!c.halfStore(c.parent+0x4e,0))return false;c.phase=State::LoadOne;break;
 case State::LoadOne:if(!c.load(One,c.one))return false;c.phase=State::FreshFlags;break;
 case State::FreshFlags:if(!c.word(c.parent,c.flags))return false;c.phase=State::ClearForce;break;
 case State::ClearForce:if(!c.store(c.parent,c.flags&~ForceFlag))return false;c.phase=State::ClearSpeed;break;
 case State::ClearSpeed:if(!c.store(c.parent+0x180,bits(c.zero)))return false;c.phase=State::Input;break;
 case State::Input:if(!c.load(c.parent+0x194,c.input))return false;c.phase=State::Choose;break;
 case State::Choose:if(c.input>c.one)c.function=0x80087ce4;else if(c.input>c.zero)c.function=0x80087d84;else{c.phase=State::Done;break;}c.phase=State::Speed;break;
 case State::Speed:{c.pending=c.function;if(!s.speed)return false;const auto v=s.speed(s.user,c.function);if(!v)return false;c.speed=*v;c.phase=State::StoreSpeed;break;}
 case State::StoreSpeed:if(!c.store(c.parent+0x180,bits(c.speed)))return false;c.phase=State::Done;break;
 case State::Done:c.pending=0;return true;
 }}
}
struct SourceMarioMovePos::State:Binding {
 enum Phase{Pi,Degrees,Multiply,Divide,Sin,RoundSin,Cos,X,RoundCos,FmaX,StoreX,Z,FmaZ,StoreZ,Done};Phase phase=Pi;
 bool arguments=false;uint32_t x=0,z=0;float speed=0,angle=0,pi=0,degrees=0,product=0,radians=0,sine=0,cosine=0,oldX=0,oldZ=0,outX=0,outZ=0;double wide=0;
};
SourceMarioMovePos::SourceMarioMovePos():state_(std::make_unique<State>()){}
SourceMarioMovePos::~SourceMarioMovePos()=default;
bool SourceMarioMovePos::complete()const{return state_->phase==State::Done;}
uint32_t SourceMarioMovePos::pending()const{return state_->pending;}
bool SourceMarioMovePos::reset(){if(invoking_||(state_->bound&&!complete()))return false;state_=std::make_unique<State>();return true;}
bool SourceMarioMovePos::resume(uint32_t x,uint32_t z,float speed,float angle,const SourceMarioForceServices&s){
 if(invoking_)return false;Guard guard(invoking_);auto&c=*state_;if(!c.admit(s))return false;
 if(!c.arguments){c.x=x;c.z=z;c.speed=speed;c.angle=angle;c.arguments=true;}
 else if(c.x!=x||c.z!=z||bits(c.speed)!=bits(speed)||bits(c.angle)!=bits(angle))return false;
 auto number=[&](SourceMarioForceNumberKind kind,uint32_t pc,std::array<float,3>values,float&out){c.pending=pc;if(!s.number)return false;SourceMarioForceNumber q;q.kind=kind;q.callsite=pc;q.values=values;q.wide=c.wide;const auto v=s.number(s.user,q);if(!v)return false;out=*v;return true;};
 auto trig=[&](uint32_t fn){c.pending=fn;if(!s.trig)return false;const auto v=s.trig(s.user,fn,c.radians);if(!v)return false;c.wide=*v;return true;};
 for(;;){if(!c.current())return false;switch(c.phase){
 case State::Pi:if(!c.load(TwoPi,c.pi))return false;c.phase=State::Degrees;break;
 case State::Degrees:if(!c.load(FullDegrees,c.degrees))return false;c.phase=State::Multiply;break;
 case State::Multiply:if(!number(SourceMarioForceNumberKind::Multiply,0x80005e20,{c.pi,c.angle,0},c.product))return false;c.phase=State::Divide;break;
 case State::Divide:if(!number(SourceMarioForceNumberKind::Divide,0x80005e28,{c.product,c.degrees,0},c.radians))return false;c.phase=State::Sin;break;
 case State::Sin:if(!trig(0x80272410))return false;c.phase=State::RoundSin;break;
 case State::RoundSin:if(!number(SourceMarioForceNumberKind::RoundSingle,0x80005e34,{},c.sine))return false;c.phase=State::Cos;break;
 case State::Cos:if(!trig(0x80271ea8))return false;c.phase=State::X;break;
 case State::X:if(!c.load(c.x,c.oldX))return false;c.phase=State::RoundCos;break;
 case State::RoundCos:if(!number(SourceMarioForceNumberKind::RoundSingle,0x80005e44,{},c.cosine))return false;c.phase=State::FmaX;break;
 case State::FmaX:if(!number(SourceMarioForceNumberKind::FusedMultiplyAdd,0x80005e48,{c.speed,c.sine,c.oldX},c.outX))return false;c.phase=State::StoreX;break;
 case State::StoreX:if(!c.store(c.x,bits(c.outX)))return false;c.phase=State::Z;break;
 case State::Z:if(!c.load(c.z,c.oldZ))return false;c.phase=State::FmaZ;break;
 case State::FmaZ:if(!number(SourceMarioForceNumberKind::FusedNegativeMultiplySubtract,0x80005e54,{c.speed,c.cosine,c.oldZ},c.outZ))return false;c.phase=State::StoreZ;break;
 case State::StoreZ:if(!c.store(c.z,bits(c.outZ)))return false;c.phase=State::Done;break;
 case State::Done:c.pending=0;return true;
 }}
}
}
