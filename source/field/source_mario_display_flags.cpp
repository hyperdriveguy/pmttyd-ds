#include "source_mario_display_flags.hpp"
#include <tuple>
namespace ttyd {namespace {
constexpr uint32_t MarioPointer=0x8041e900,ExtraFlags=4;
struct Guard{bool&v;explicit Guard(bool&b):v(b){v=true;}~Guard(){v=false;}};
bool same(const SourceMarioDisplayFlagServices&a,const SourceMarioDisplayFlagServices&b){return a.lifetime.get()==b.lifetime.get()&&!a.lifetime.owner_before(b.lifetime)&&!b.lifetime.owner_before(a.lifetime)&&std::tie(a.user,a.current,a.word,a.storeWord,a.eventValue)==std::tie(b.user,b.current,b.word,b.storeWord,b.eventValue);}
}
struct SourceMarioDisplayFlags::State {
 enum Phase{Args,Pointer,RawOn,On,RawMask,Mask,Flags,Store,Done};Phase phase=Args;
 SourceMarioDisplayFlagServices services;EventEntry*entry=nullptr;bool bound=false,poison=false;uint32_t address=0,id=0,command=0,args=0,actor=0,raw=0,flags=0,mask=0,pending=0;int32_t on=0;
};
SourceMarioDisplayFlags::SourceMarioDisplayFlags():state_(std::make_unique<State>()){}SourceMarioDisplayFlags::~SourceMarioDisplayFlags()=default;
bool SourceMarioDisplayFlags::complete()const{return state_->phase==State::Done;}uint32_t SourceMarioDisplayFlags::pending()const{return state_->pending;}
bool SourceMarioDisplayFlags::reset(){if(invoking_||(state_->bound&&!complete()))return false;state_=std::make_unique<State>();return true;}
std::optional<int32_t>SourceMarioDisplayFlags::resume(EventEntry&e,uint32_t address,const SourceMarioDisplayFlagServices&s){
 if(invoking_)return {};Guard guard(invoking_);auto&c=*state_;if(c.poison)return {};
 if(!c.bound){if(!s.lifetime||!s.current)return {};if(!s.current(s.user)){c.poison=true;return {};}c.services=s;c.entry=&e;c.address=address;c.id=e.id;c.command=e.currentCommand;c.bound=true;}
 else{if(!same(c.services,s)||c.entry!=&e||c.address!=address)return {};if(!s.current(s.user)){c.poison=true;return {};}if(c.id!=e.id||c.command!=e.currentCommand)return {};}
 auto word=[&](uint32_t a,uint32_t&out){c.pending=a;if(!s.word)return false;auto v=s.word(s.user,a);if(!v)return false;out=*v;return true;};
 auto value=[&](int32_t&out){c.pending=0x80031260;if(!s.eventValue)return false;auto v=s.eventValue(s.user,e,c.raw);if(!v)return false;out=*v;return true;};
 for(;;){if(!s.current(s.user)){c.poison=true;return {};}switch(c.phase){
 case State::Args:if(!word(address+0x18,c.args))return {};c.phase=State::Pointer;break;
 case State::Pointer:if(!word(MarioPointer,c.actor))return {};c.phase=State::RawOn;break;
 case State::RawOn:if(!word(c.args,c.raw))return {};c.phase=State::On;break;
 case State::On:if(!value(c.on))return {};c.phase=State::RawMask;break;
 case State::RawMask:if(!word(c.args+4,c.raw))return {};c.phase=State::Mask;break;
 case State::Mask:{int32_t mask=0;if(!value(mask))return {};c.mask=uint32_t(mask);c.phase=State::Flags;break;}
 case State::Flags:if(!word(c.actor+ExtraFlags,c.flags))return {};c.phase=State::Store;break;
 case State::Store:c.pending=c.actor+ExtraFlags;if(!s.storeWord||!s.storeWord(s.user,c.pending,c.on?c.flags|c.mask:c.flags&~c.mask))return {};c.phase=State::Done;break;
 case State::Done:c.pending=0;return 2;
 }}
}
}
