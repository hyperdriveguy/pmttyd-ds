#include "native_environment_sound.hpp"
#include <array>
#include <utility>
namespace ttyd {
namespace {
constexpr uint32_t Fade=0x800d7470,FadeTable=0x80416520,ClosedControl=0x200;
struct Guard{bool&v;explicit Guard(bool&b):v(b){v=true;}~Guard(){v=false;}};
template<class A,class B>bool sameOwner(const A&a,const B&b){return a.get()==b.get()&&!a.owner_before(b)&&!b.owner_before(a);}
}
struct NativeEnvironmentSoundState {
 NativeEnvironmentSoundBinding binding;
 std::array<std::optional<uint16_t>,2>handles{};
 bool initialized=false,attempted=false,stale=false,checking=false,invoking=false;
 explicit NativeEnvironmentSoundState(NativeEnvironmentSoundBinding b):binding(std::move(b)){}
 bool current(){if(stale||checking)return false;Guard guard(checking);if(!binding.host.current()){stale=true;return false;}if(!binding.current(binding.user)){stale=true;return false;}return true;}
};
NativeEnvironmentSound::NativeEnvironmentSound(std::shared_ptr<NativeEnvironmentSoundState>s):state_(std::move(s)){}
std::shared_ptr<NativeEnvironmentSound>NativeEnvironmentSound::prepare(NativeEnvironmentSoundBinding b){
 if(!b.lifetime||!b.host.current()||!b.current||!b.word)return {};
 auto state=std::make_shared<NativeEnvironmentSoundState>(std::move(b));if(!state->current())return {};
 return std::shared_ptr<NativeEnvironmentSound>(new NativeEnvironmentSound(std::move(state)));
}
std::optional<NativeEnvironmentColdProof>NativeEnvironmentSound::initializeNativeCold(){
 auto&s=*state_;if(s.invoking||!s.current()||s.initialized||s.attempted)return {};Guard guard(s.invoking);
 // Actual native process-constructor contribution of psndInit's two stores:
 // 800dc5ec -> 803df700, then 800dc5f4 -> 803df702. No guest BSS is invented.
 s.handles[0]=0;s.handles[1]=0;s.initialized=true;
 return NativeEnvironmentColdProof(state_);
}
bool NativeEnvironmentSound::current(const NativeEnvironmentColdProof&p){
 const auto owner=p.state_.lock();return sameOwner(owner,state_)&&state_->initialized&&state_->current();
}
std::optional<int32_t>NativeEnvironmentSound::off(const NativeEnvironmentColdProof&p,uint32_t control,NativeEnvironmentOffCursor&c){
 auto&s=*state_;if(s.invoking)return {};s.attempted=true;
 if(!current(p))return {};Guard guard(s.invoking);
 if(c.phase_){const auto owner=c.owner_.lock();if(!sameOwner(owner,state_)||control!=c.control_)return {};}
 else{c.owner_=state_;c.control_=control;c.pending_=Fade;c.phase_=1;}
 for(;;)switch(c.phase_){
 case 1:
  // Closed actual reached domain only. Other groups/flags are unimplemented.
  if(c.control_!=ClosedControl||!s.handles[0])return {};c.handle_=*s.handles[0];c.phase_=2;break;
 case 2:{c.pending_=FadeTable;const auto word=s.binding.word(s.binding.user,FadeTable);
  if(!word||!current(p))return {};c.table_=*word;c.phase_=3;break;}
 case 3:
  // Source loads the table BEFORE its captured handle-zero branch. No fade,
  // native voice stop or store is reached for the zero handle return0 path.
  if(c.handle_){c.pending_=Fade;return {};}
  c.phase_=4;c.pending_=0;return 0;
 case 4:return 0;
 default:return {};
 }
}
void NativeEnvironmentSound::invalidate(){state_->stale=true;state_->handles={};}
NativeFieldEnvironmentOff::NativeFieldEnvironmentOff(EventNativeRuntime&vm,std::shared_ptr<NativeEnvironmentSound>s,NativeEnvironmentColdProof p):vm_(&vm),vmProof_(vm.nativeFacadeProof()),environment_(std::move(s)),cold_(std::move(p)){}
std::unique_ptr<NativeFieldEnvironmentOff>NativeFieldEnvironmentOff::create(EventNativeRuntime&vm,std::shared_ptr<NativeEnvironmentSound>s,NativeEnvironmentColdProof p){
 if(!s||!s->current(p))return {};return std::unique_ptr<NativeFieldEnvironmentOff>(new NativeFieldEnvironmentOff(vm,std::move(s),std::move(p)));
}
std::optional<int32_t>NativeFieldEnvironmentOff::invoke(uint32_t fn,EventEntry&e,bool){
 if(stale_||invoking_||fn!=NativeEnvironmentOffEventFunction||!vmProof_.current()||!environment_->current(cold_))return {};Guard guard(invoking_);
 if(!(e.flags&1))return {};
 auto&c=calls_[e.id];if(!c.entry){c.entry=&e;c.id=e.id;c.command=e.currentCommand;if(!vm_->entryAddresses.addressOf(&e,c.address)){calls_.erase(e.id);return {};}}
 if(c.entry!=&e||c.id!=e.id||c.command!=e.currentCommand)return {};pending_=fn;
 for(;;)switch(c.phase){
 case 0:if(!vm_->memory.readWord(c.address+0x18,c.args))return {};++c.phase;break;
 case 1:if(!vm_->memory.readWord(c.args,c.raw))return {};++c.phase;break;
 case 2:{pending_=Fade;if(!environment_->off(cold_,c.raw,c.child)){pending_=c.child.pending()?c.child.pending():Fade;return {};}
  // The original wrapper ignores psndENVOff's integer return (including0).
  // It never resolves raw flags via evtGetValue and itself returns2.
  calls_.erase(e.id);pending_=0;return 2;}
 default:return {};
 }
}
void NativeFieldEnvironmentOff::retire(const EventEntry&e){if(invoking_){stale_=true;return;}const auto i=calls_.find(e.id);if(i!=calls_.end()&&i->second.entry==&e)calls_.erase(i);if(calls_.empty())pending_=0;}
}
