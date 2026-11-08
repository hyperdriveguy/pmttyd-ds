#include "source_mario_set_pose.hpp"
#include <tuple>
namespace ttyd {namespace {
constexpr uint32_t MarioPointer=0x8041e900,PoseTable=0x8032dd60,DotFlag=0x80000000;
constexpr uint32_t Main=0,Extra=4,Dirty=0xc,Pose=0x18,Motion=0x2e;
constexpr uint32_t UnlistedPoseFlag=0x10000000,PoseChanged=0x1000,SpecialPoseFlag=0x01000000,SpecialChanged=0x8000;
struct Guard{bool&v;explicit Guard(bool&b):v(b){v=true;}~Guard(){v=false;}};
bool same(const SourceMarioPoseServices&a,const SourceMarioPoseServices&b){return a.lifetime.get()==b.lifetime.get()&&!a.lifetime.owner_before(b.lifetime)&&!b.lifetime.owner_before(a.lifetime)&&std::tie(a.user,a.current,a.word,a.half,a.storeWord,a.eventValue,a.compare,a.dotPose)==std::tie(b.user,b.current,b.word,b.half,b.storeWord,b.eventValue,b.compare,b.dotPose);}
}
struct SourceMarioSetPose::State {
 enum Phase{Args,Pointer,MotionRead,Raw,Value,Flags,Dot,TableRead,Compare,PoseStore,DirtyRead,DirtyStore,ExtraRead,ExtraStore,FreshExtra,SpecialDirty,SpecialStore,Done};Phase phase=Args;
 SourceMarioPoseServices services;EventEntry*entry=nullptr;bool bound=false,poison=false,dot=false,member=false;uint32_t address=0,id=0,command=0,args=0,actor=0,raw=0,pose=0,table=PoseTable,tablePose=0,value=0,pending=0;uint16_t motion=0;
};
SourceMarioSetPose::SourceMarioSetPose():state_(std::make_unique<State>()){}SourceMarioSetPose::~SourceMarioSetPose()=default;
bool SourceMarioSetPose::complete()const{return state_->phase==State::Done;}uint32_t SourceMarioSetPose::pending()const{return state_->pending;}
bool SourceMarioSetPose::reset(){if(invoking_||(state_->bound&&!complete()))return false;state_=std::make_unique<State>();return true;}
std::optional<int32_t>SourceMarioSetPose::resume(EventEntry&e,uint32_t address,const SourceMarioPoseServices&s){
 if(invoking_)return {};Guard guard(invoking_);auto&c=*state_;if(c.poison)return {};
 if(!c.bound){if(!s.lifetime||!s.current)return {};if(!s.current(s.user)){c.poison=true;return {};}c.services=s;c.entry=&e;c.address=address;c.id=e.id;c.command=e.currentCommand;c.bound=true;}
 else{if(!same(c.services,s)||c.entry!=&e||c.address!=address)return {};if(!s.current(s.user)){c.poison=true;return {};}if(c.id!=e.id||c.command!=e.currentCommand)return {};}
 auto word=[&](uint32_t a,uint32_t&out){c.pending=a;if(!s.word)return false;auto v=s.word(s.user,a);if(!v)return false;out=*v;return true;};
 auto store=[&](uint32_t a,uint32_t v){c.pending=a;return s.storeWord&&s.storeWord(s.user,a,v);};
 for(unsigned visits=0;;++visits){if(visits==4096)return {};if(!s.current(s.user)){c.poison=true;return {};}switch(c.phase){
 case State::Args:if(!word(address+0x18,c.args))return {};c.phase=State::Pointer;break;
 case State::Pointer:if(!word(MarioPointer,c.actor))return {};c.phase=State::MotionRead;break;
 case State::MotionRead:{c.pending=c.actor+Motion;if(!s.half)return {};auto v=s.half(s.user,c.pending);if(!v)return {};c.motion=*v;c.phase=c.motion==24||c.motion==25||c.motion==22||c.motion==21?State::Done:State::Raw;break;}
 case State::Raw:if(!word(c.args,c.raw))return {};c.phase=State::Value;break;
 case State::Value:{c.pending=0x80031260;if(!s.eventValue)return {};auto v=s.eventValue(s.user,e,c.raw);if(!v)return {};c.pose=uint32_t(*v);c.phase=State::Flags;break;}
 case State::Flags:if(!word(c.actor+Main,c.value))return {};c.dot=(c.value&DotFlag)!=0;c.phase=c.dot?State::Dot:State::TableRead;break;
 case State::Dot:{c.pending=0x80058f9c;if(!s.dotPose)return {};auto v=s.dotPose(s.user,c.pose);if(!v)return {};c.pose=*v;c.phase=c.pose?State::PoseStore:State::Done;break;}
 case State::TableRead:if(!word(c.table,c.tablePose))return {};c.phase=c.tablePose?State::Compare:State::PoseStore;break;
 case State::Compare:{c.pending=0x8026d464;if(!s.compare)return {};auto v=s.compare(s.user,c.pose,c.tablePose);if(!v)return {};if(*v==0){c.member=true;c.phase=State::PoseStore;}else{c.table+=4;c.phase=State::TableRead;}break;}
 case State::PoseStore:if(!store(c.actor+Pose,c.pose))return {};c.phase=State::DirtyRead;break;
 case State::DirtyRead:if(!word(c.actor+Dirty,c.value))return {};c.phase=State::DirtyStore;break;
 case State::DirtyStore:if(!store(c.actor+Dirty,c.value|PoseChanged))return {};c.phase=c.dot?State::FreshExtra:State::ExtraRead;break;
 case State::ExtraRead:if(!word(c.actor+Extra,c.value))return {};c.phase=State::ExtraStore;break;
 case State::ExtraStore:if(!store(c.actor+Extra,c.member?c.value&~UnlistedPoseFlag:c.value|UnlistedPoseFlag))return {};c.phase=State::FreshExtra;break;
 case State::FreshExtra:if(!word(c.actor+Extra,c.value))return {};c.phase=c.value&SpecialPoseFlag?State::SpecialDirty:State::Done;break;
 case State::SpecialDirty:if(!word(c.actor+Dirty,c.value))return {};c.phase=State::SpecialStore;break;
 case State::SpecialStore:if(!store(c.actor+Dirty,c.value|SpecialChanged))return {};c.phase=State::Done;break;
 case State::Done:c.pending=0;return 2;
 }}
}
}
