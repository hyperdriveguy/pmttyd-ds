#include "source_mario_speed.hpp"
#include <cstring>
#include <tuple>
namespace ttyd {namespace {
constexpr uint32_t MarioPointer=0x8041e900,Dash=0x80087ce4,Walk=0x80087d84;
constexpr uint32_t FormFlag=0x00100000,BackgroundFlag=0x02000000,Half=0x8042088c;
constexpr uint32_t WalkBase=0x184,DashBase=0x188,Scale=0x228,StickX=0x252,StickY=0x253;
constexpr int32_t HalfStickSquared=55*55;
float scalar(uint32_t w){float f;std::memcpy(&f,&w,4);return f;}int32_t signedByte(uint8_t b){return b<128?b:int32_t(b)-256;}
struct Guard{bool&v;explicit Guard(bool&b):v(b){v=true;}~Guard(){v=false;}};
bool same(const SourceMarioSpeedServices&a,const SourceMarioSpeedServices&b){return a.lifetime.get()==b.lifetime.get()&&!a.lifetime.owner_before(b.lifetime)&&!b.lifetime.owner_before(a.lifetime)&&std::tie(a.user,a.current,a.word,a.byte,a.multiply)==std::tie(b.user,b.current,b.word,b.byte,b.multiply);}
}
struct SourceMarioSpeed::State {
 enum Phase{Pointer,Flags,Base,Branch,StickFirst,StickSecond,WalkReload,StickTest,BgPointer,BgFlags,HalfRead,HalfMultiply,ScaleRead,ScaleMultiply,Done};Phase phase=Pointer;
 SourceMarioSpeedServices services;bool bound=false,poison=false;uint32_t function=0,actor=0,flags=0,bgActor=0,pending=0;uint8_t stickX=0,stickY=0;float value=0,half=0,scale=0;
};
SourceMarioSpeed::SourceMarioSpeed():state_(std::make_unique<State>()){}SourceMarioSpeed::~SourceMarioSpeed()=default;
bool SourceMarioSpeed::complete()const{return state_->phase==State::Done;}uint32_t SourceMarioSpeed::pending()const{return state_->pending;}
bool SourceMarioSpeed::reset(){if(invoking_||(state_->bound&&!complete()))return false;state_=std::make_unique<State>();return true;}
std::optional<float>SourceMarioSpeed::resume(uint32_t function,const SourceMarioSpeedServices&s){
 if(invoking_||(function!=Dash&&function!=Walk))return {};Guard guard(invoking_);auto&c=*state_;if(c.poison)return {};
 if(!c.bound){if(!s.lifetime||!s.current)return {};c.services=s;c.function=function;c.bound=true;}else if(c.function!=function||!same(c.services,s))return {};
 auto word=[&](uint32_t a,uint32_t&out){c.pending=a;if(!s.word)return false;auto v=s.word(s.user,a);if(!v)return false;out=*v;return true;};
 auto load=[&](uint32_t a,float&out){uint32_t v=0;if(!word(a,v))return false;out=scalar(v);return true;};
 auto byte=[&](uint32_t a,uint8_t&out){c.pending=a;if(!s.byte)return false;auto v=s.byte(s.user,a);if(!v)return false;out=*v;return true;};
 auto mul=[&](uint32_t pc,float factor){c.pending=pc;if(!s.multiply)return false;auto v=s.multiply(s.user,pc,c.value,factor);if(!v)return false;c.value=*v;return true;};
 for(;;){if(!s.current(s.user)){c.poison=true;return {};}switch(c.phase){
 case State::Pointer:if(!word(MarioPointer,c.actor))return {};c.phase=State::Flags;break;
 case State::Flags:if(!word(c.actor,c.flags))return {};c.phase=State::Base;break;
 case State::Base:if(!load(c.actor+(function==Dash?DashBase:WalkBase),c.value))return {};c.phase=State::Branch;break;
 case State::Branch:c.phase=c.flags&FormFlag?State::StickFirst:State::BgPointer;break;
 case State::StickFirst:if(!byte(c.actor+StickX,c.stickX))return {};c.phase=State::StickSecond;break;
 case State::StickSecond:if(!byte(c.actor+StickY,c.stickY))return {};c.phase=function==Dash?State::WalkReload:State::StickTest;break;
 case State::WalkReload:if(!load(c.actor+WalkBase,c.value))return {};c.phase=State::StickTest;break;
 case State::StickTest:{auto x=signedByte(c.stickX),y=signedByte(c.stickY);c.phase=x*x+y*y>HalfStickSquared?State::ScaleRead:State::HalfRead;break;}
 case State::BgPointer:if(!word(MarioPointer,c.bgActor))return {};c.phase=State::BgFlags;break;
 case State::BgFlags:{uint32_t f=0;if(!word(c.bgActor,f))return {};c.phase=f&BackgroundFlag?State::HalfRead:State::ScaleRead;break;}
 case State::HalfRead:if(!load(Half,c.half))return {};c.phase=State::HalfMultiply;break;
 case State::HalfMultiply:if(!mul(function==Dash?(c.flags&FormFlag?0x80087d40:0x80087d58):(c.flags&FormFlag?0x80087ddc:0x80087df4),c.half))return {};c.phase=State::ScaleRead;break;
 case State::ScaleRead:if(!load(c.actor+Scale,c.scale))return {};c.phase=State::ScaleMultiply;break;
 case State::ScaleMultiply:if(!mul(function==Dash?0x80087d60:0x80087dfc,c.scale))return {};c.phase=State::Done;break;
 case State::Done:c.pending=0;return c.value;
 }}
}
}
