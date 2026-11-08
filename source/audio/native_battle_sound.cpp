#include "native_battle_sound.hpp"
#include "event_native_runtime.hpp"
#include "battle_timing_a.hpp"
#include "battle_event_callbacks.hpp"
#include "battle_packed_status.hpp"
#include "positional_sfx_start.hpp"
#include <cmath>
#include <cstring>
#include <map>
#include <string>
namespace ttyd {
namespace {
constexpr uint32_t SoundRows=0x80316b28,SoundRowBytes=20,SoundRowCount=4242;
constexpr uint32_t SourceMilliseconds=0x804218cc;
constexpr uint32_t GpRate=4,GpStory=0x174;
constexpr uint32_t UnitPosition=0x3c,UnitStatus=0x118,UnitStatusBytes=30;
constexpr uint16_t PendingFlag=1;
constexpr unsigned NameBudget=4096;
}
struct NativeBattleSound::State {
 NativeBattleSoundBinding binding;SourceBattleVoiceServices services;
 std::map<uint64_t,SourceBattleVoiceCursor> voices;uint32_t pending=0;
 bool starting=false,poisoned=false;
 bool current()const{return !poisoned && binding.vm && binding.random && binding.registry && binding.lifetime && binding.current && binding.current(binding.user);}
 std::optional<uint32_t> word(uint32_t address)const{uint32_t v=0;if(!binding.vm->memory.readWord(address,v))return {};return v;}
 std::optional<std::string> text(uint32_t address)const{
  std::string result;for(unsigned i=0;i<NameBudget;++i){uint8_t v=0;if(!binding.vm->memory.readByte(address+i,v))return {};if(!v)return result;result+=char(v);}return {};
 }
 std::optional<StatusValue> status(uint32_t unit,int32_t id)const{
  // Native admission before the existing exact whole GetStatus reader; no
  // missing byte is supplied as zero. The actual reader keeps source order.
  for(unsigned i=0;i<UnitStatusBytes;++i){uint8_t value;if(!binding.vm->memory.readByte(unit+UnitStatus+i,value))return {};}
  return getPackedBattleStatus(unit,id,nativeBattleActorServices(*binding.vm));
 }
 std::optional<int32_t> start(uint32_t key,const std::optional<std::array<float,3>>& position){
  pending=0x800d9934;if(!current() || starting)return {};
  // This finite binding has no started-engine stop owner. Refuse before
  // entering the nonretained original On implementation, not after mutations.
  for(const auto& slot:binding.registry->slots)
   if(slot.key!=-1 && !(slot.flags&PendingFlag) && slot.engine!=-1)return {};
  struct Snapshot {
   std::map<uint32_t,PositionalSfxRow> rows;
   std::map<std::pair<uint32_t,uint32_t>,int32_t> comparisons;
   int32_t story=0,rate=0;float milliseconds=0;State* state=nullptr;
  } snapshot;
  snapshot.state=this;
  auto row=[&](uint32_t index)->std::optional<PositionalSfxRow>{
   PositionalSfxRow value;for(unsigned i=0;i<SoundRowBytes;++i)
    if(!binding.vm->memory.readByte(SoundRows+SoundRowBytes*index+i,value.bytes[i]))return {};
   snapshot.rows[index]=value;return value;
  };
  uint32_t index=key&0x1fff;
  if(int32_t(key)<0){
   const auto requested=text(key);if(!requested)return {};
   for(index=0;index<SoundRowCount;++index){const auto value=row(index);if(!value)return {};
    const auto original=text(value->word(0));if(!original)return {};
    const int32_t compared=original->compare(*requested);snapshot.comparisons[{value->word(0),key}]=compared;
    if(compared==0)break;
   }
   if(index==SoundRowCount)index=0;
  }
  if(!row(index))return {};
  const auto gp=binding.vm->gpAddresses.currentAddress();const auto story=word(gp+GpStory),rate=word(gp+GpRate),milliseconds=word(SourceMilliseconds);
  if(!story || !rate || !milliseconds || !binding.vm->platform.truncate)return {};
  snapshot.story=int32_t(*story);snapshot.rate=int32_t(*rate);std::memcpy(&snapshot.milliseconds,&*milliseconds,4);
  // Source delay is U16, row delay U8 and preference U16. With delay0 used by
  // these wrappers, the actual truncation input is always finite/signed32.
  if(!std::isfinite(snapshot.milliseconds) || snapshot.milliseconds==0 || snapshot.rate==0 || !current())return {};
  const float period=snapshot.milliseconds/float(snapshot.rate);
  if(!std::isfinite(period) || period==0)return {};
  PositionalSfxStartServices source;source.user=&snapshot;
  source.row=[](void* p,uint32_t i)->std::optional<PositionalSfxRow>{auto& x=*static_cast<Snapshot*>(p);const auto it=x.rows.find(i);return it==x.rows.end()?std::nullopt:std::optional<PositionalSfxRow>(it->second);};
  source.compareName=[](void* p,uint32_t a,uint32_t b)->std::optional<int32_t>{auto& x=*static_cast<Snapshot*>(p);const auto it=x.comparisons.find({a,b});return it==x.comparisons.end()?std::nullopt:std::optional<int32_t>(it->second);};
  source.story=[](void* p)->std::optional<int32_t>{return static_cast<Snapshot*>(p)->story;};
  source.rate=[](void* p)->std::optional<int32_t>{return static_cast<Snapshot*>(p)->rate;};
  source.truncate=[](void* p,float value)->std::optional<int32_t>{auto& x=*static_cast<Snapshot*>(p);return x.state->binding.vm->platform.truncate(x.state->binding.vm->platform.user,value);};
  source.milliseconds=snapshot.milliseconds;
  PositionalSfxStartRequest request;request.key=int32_t(key);request.position=position;
  starting=true;const auto result=startPositionalSfx(*binding.registry,request,source);starting=false;
  // All reached providers above are captured and total during this serialized
  // call. No caller may blindly replay On after a violated admission invariant.
  if(result)pending=0;else poisoned=true;
  return result;
 }
};
NativeBattleSound::NativeBattleSound(const NativeBattleSoundBinding& binding):state_(std::make_unique<State>()){
 auto& x=*state_;x.binding=binding;auto& s=x.services;s.user=&x;s.lifetime=binding.lifetime;
 s.current=[](void* p){return static_cast<State*>(p)->current();};
 s.word=[](void* p,uint32_t a){return static_cast<State*>(p)->word(a);};
 s.value=[](void* p,EventEntry& e,int32_t raw)->std::optional<int32_t>{auto& x=*static_cast<State*>(p);return getEventValue(e,raw,x.binding.vm->interpreterServices().operands);};
 s.translate=[](void* p,EventEntry& e,int32_t raw)->std::optional<int32_t>{auto& x=*static_cast<State*>(p);return translateBattleActor(&e,raw,nativeBattleActorServices(*x.binding.vm));};
 s.unit=[](void* p,uint32_t b,int32_t id)->std::optional<uint32_t>{auto& x=*static_cast<State*>(p);if(id<0 || id>=64)return 0;return x.word(b+0x20+4u*uint32_t(id));};
 s.random=[](void* p,int32_t bound)->std::optional<int32_t>{return static_cast<State*>(p)->binding.random->bounded(bound);};
 s.position=[](void* p,uint32_t unit)->std::optional<std::array<float,3>>{auto& x=*static_cast<State*>(p);std::array<float,3> value;for(unsigned i=0;i<3;++i){const auto v=x.word(unit+UnitPosition+4*i);if(!v)return {};std::memcpy(&value[i],&*v,4);}return value;};
 s.checkStatus=[](void* p,uint32_t unit,int32_t id)->std::optional<int32_t>{auto value=static_cast<State*>(p)->status(unit,id);if(!value)return {};return value->duration!=0?1:0;};
 s.getStatus=[](void* p,uint32_t unit,int32_t id){return static_cast<State*>(p)->status(unit,id);};
 s.on=[](void* p,uint32_t key,const std::array<float,3>& position){return static_cast<State*>(p)->start(key,position);};
 s.pitch=[](void* p,int32_t handle,int32_t pitch){auto& x=*static_cast<State*>(p);if(handle==-1)return true;const auto i=uint32_t(handle)&255;if(i>=x.binding.registry->slots.size())return false;auto& slot=x.binding.registry->slots[i];if(slot.key==-1)return true;if(slot.flags&PendingFlag){slot.pitch=uint16_t(pitch);return true;}const auto descriptor=x.word(SoundRows+SoundRowBytes*(uint32_t(slot.key)&0x1fff)+4);return descriptor && (*descriptor&0x80000000);};
 s.volume=[](void* p,int32_t handle){return static_cast<State*>(p)->binding.registry->getVolume(handle);};
 s.setVolume=[](void* p,int32_t handle,uint32_t value){return static_cast<State*>(p)->binding.registry->setVolume(handle,int32_t(value));};
}
NativeBattleSound::~NativeBattleSound()=default;
std::optional<int32_t> NativeBattleSound::voice(EventEntry& e){auto& x=*state_;if(!x.current())return {};const uint64_t key=uint64_t(e.id)<<32|e.currentCommand;auto& cursor=x.voices[key];int32_t result=0;if(!resumeSourceBattleVoice(e,x.services,cursor,result)){x.pending=cursor.pending();return {};}x.voices.erase(key);x.pending=0;return result;}
std::optional<int32_t> NativeBattleSound::menu(uint32_t key){return state_->start(key,{});}
std::optional<int32_t> NativeBattleSound::position(uint32_t key,const std::array<float,3>& position){return state_->start(key,position);}
uint32_t NativeBattleSound::pending()const{return state_->pending;}
bool NativeBattleSound::usesRegistry(const PositionalSfxRegistry* registry)const{return state_->binding.registry.get()==registry;}
bool NativeBattleSound::usesRuntime(const EventNativeRuntime* vm)const{return state_->binding.vm==vm;}
}
