#include "source_mario_change_motion.hpp"
#include <tuple>
namespace ttyd {namespace {
constexpr uint32_t MarioPointer=0x8041e900,MotionTable=0x80310030;
constexpr uint32_t Main=0,Extra=4,Dirty=0xc,Motion=0x2e,Previous=0x30;
constexpr uint32_t SuppressChange=0x10000000,PreviousReset=0x08000000,PreviousResetSmall=4;
constexpr uint32_t StaySound=0x27c,OtherStaySound=0x280;
struct Guard{bool&v;explicit Guard(bool&b):v(b){v=true;}~Guard(){v=false;}};
bool same(const SourceMarioMotionServices&a,const SourceMarioMotionServices&b){return a.lifetime.get()==b.lifetime.get()&&!a.lifetime.owner_before(b.lifetime)&&!b.lifetime.owner_before(a.lifetime)&&std::tie(a.user,a.current,a.word,a.half,a.storeWord,a.storeHalf,a.call)==std::tie(b.user,b.current,b.word,b.half,b.storeWord,b.storeHalf,b.call);}
struct Binding {
 SourceMarioMotionServices s;bool bound=false,poison=false;uint32_t pending=0;
 bool admit(const SourceMarioMotionServices&v){if(poison)return false;if(!bound){if(!v.lifetime||!v.current)return false;s=v;bound=true;}else if(!same(s,v))return false;return current();}
 bool current(){if(poison||!s.current(s.user)){poison=true;return false;}return true;}
 bool word(uint32_t a,uint32_t&v){pending=a;if(!s.word)return false;const auto r=s.word(s.user,a);if(!r)return false;v=*r;return true;}
 bool half(uint32_t a,uint16_t&v){pending=a;if(!s.half)return false;const auto r=s.half(s.user,a);if(!r)return false;v=*r;return true;}
 bool store(uint32_t a,uint32_t v){pending=a;return s.storeWord&&s.storeWord(s.user,a,v);}
 bool storeHalf(uint32_t a,uint16_t v){pending=a;return s.storeHalf&&s.storeHalf(s.user,a,v);}
 bool call(const SourceMarioMotionCall&q){pending=q.function;return s.call&&s.call(s.user,q);}
};
}
struct SourceMarioChangeMotion::State:Binding {
 enum Phase{Pointer,Flags,PreviousRead,ExtraFirst,ExtraFirstStore,ExtraSecond,ExtraSecondStore,CurrentMotion,StorePrevious,StoreMotion,FreshPrevious,PostRead,Post,DirtyRead,DirtyStore,Done};Phase phase=Pointer;
 bool arguments=false;uint32_t requested=0,actor=0,value=0,entry=0;uint16_t previous=0,motion=0;SourceMarioMotionCall request;
};
SourceMarioChangeMotion::SourceMarioChangeMotion():state_(std::make_unique<State>()){}SourceMarioChangeMotion::~SourceMarioChangeMotion()=default;
bool SourceMarioChangeMotion::complete()const{return state_->phase==State::Done;}uint32_t SourceMarioChangeMotion::pending()const{return state_->pending;}
bool SourceMarioChangeMotion::reset(){if(invoking_||(state_->bound&&!complete()))return false;state_=std::make_unique<State>();return true;}
bool SourceMarioChangeMotion::resume(uint32_t requested,const SourceMarioMotionServices&s){
 if(invoking_)return false;Guard guard(invoking_);auto&c=*state_;if(!c.admit(s))return false;if(!c.arguments){c.requested=requested;c.arguments=true;}else if(c.requested!=requested)return false;
 for(;;){if(!c.current())return false;switch(c.phase){
 case State::Pointer:if(!c.word(MarioPointer,c.actor))return false;c.phase=State::Flags;break;
 case State::Flags:if(!c.word(c.actor+Main,c.value))return false;c.phase=c.value&SuppressChange?State::Done:State::PreviousRead;break;
 case State::PreviousRead:if(!c.half(c.actor+Previous,c.previous))return false;c.phase=c.previous==16||c.previous==17?State::ExtraFirst:State::CurrentMotion;break;
 case State::ExtraFirst:if(!c.word(c.actor+Extra,c.value))return false;c.phase=c.value&PreviousReset?State::ExtraFirstStore:State::ExtraSecond;break;
 case State::ExtraFirstStore:if(!c.store(c.actor+Extra,c.value&~PreviousReset))return false;c.phase=State::ExtraSecond;break;
 case State::ExtraSecond:if(!c.word(c.actor+Extra,c.value))return false;c.phase=c.value&PreviousResetSmall?State::ExtraSecondStore:State::CurrentMotion;break;
 case State::ExtraSecondStore:if(!c.store(c.actor+Extra,c.value&~PreviousResetSmall))return false;c.phase=State::CurrentMotion;break;
 case State::CurrentMotion:if(!c.half(c.actor+Motion,c.motion))return false;c.phase=State::StorePrevious;break;
 case State::StorePrevious:if(!c.storeHalf(c.actor+Previous,c.motion))return false;c.phase=State::StoreMotion;break;
 case State::StoreMotion:if(!c.storeHalf(c.actor+Motion,uint16_t(c.requested)))return false;c.phase=State::FreshPrevious;break;
 case State::FreshPrevious:if(!c.half(c.actor+Previous,c.previous))return false;c.entry=MotionTable+12u*c.previous;c.phase=State::PostRead;break;
 case State::PostRead:if(!c.word(c.entry+8,c.value))return false;c.request={c.value,0x800945f4,c.entry};c.phase=c.value?State::Post:State::DirtyRead;break;
 case State::Post:if(!c.call(c.request))return false;c.phase=State::DirtyRead;break;
 case State::DirtyRead:if(!c.word(c.actor+Dirty,c.value))return false;c.phase=State::DirtyStore;break;
 case State::DirtyStore:if(!c.store(c.actor+Dirty,c.value|1))return false;c.phase=State::Done;break;
 case State::Done:c.pending=0;return true;
 }}
}
struct SourceMarioStayPost::State:Binding {
 enum Phase{FirstPointer,MotionRead,SoundPointer,Handle,Off,Clear,Done};Phase phase=FirstPointer;
 uint32_t actor=0,handle=0,offset=StaySound;uint16_t motion=0;bool special=false;SourceMarioMotionCall request;
};
SourceMarioStayPost::SourceMarioStayPost():state_(std::make_unique<State>()){}SourceMarioStayPost::~SourceMarioStayPost()=default;
bool SourceMarioStayPost::complete()const{return state_->phase==State::Done;}uint32_t SourceMarioStayPost::pending()const{return state_->pending;}
bool SourceMarioStayPost::reset(){if(invoking_||(state_->bound&&!complete()))return false;state_=std::make_unique<State>();return true;}
bool SourceMarioStayPost::resume(const SourceMarioMotionServices&s){
 if(invoking_)return false;Guard guard(invoking_);auto&c=*state_;if(!c.admit(s))return false;
 auto next=[&]{if(c.special||c.offset==OtherStaySound)c.phase=State::Done;else{c.offset=OtherStaySound;c.phase=State::SoundPointer;}};
 for(;;){if(!c.current())return false;switch(c.phase){
 case State::FirstPointer:if(!c.word(MarioPointer,c.actor))return false;c.phase=State::MotionRead;break;
 case State::MotionRead:if(!c.half(c.actor+Motion,c.motion))return false;c.special=c.motion==19||c.motion==20;c.phase=State::SoundPointer;break;
 case State::SoundPointer:if(!c.word(MarioPointer,c.actor))return false;c.phase=State::Handle;break;
 case State::Handle:if(!c.word(c.actor+c.offset,c.handle))return false;if(c.handle==0xffffffffu){next();break;}c.request={0x800d95f8,c.special?0x800a6b9cu:c.offset==StaySound?0x800a6b50u:0x800a6b74u,c.handle};c.phase=State::Off;break;
 case State::Off:if(!c.call(c.request))return false;c.phase=State::Clear;break;
 case State::Clear:if(!c.store(c.actor+c.offset,0xffffffffu))return false;next();break;
 case State::Done:c.pending=0;return true;
 }}
}
}
