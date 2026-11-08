#include "source_battle_voice.hpp"
#include <cassert>
#include <map>
#include <algorithm>
#include <string>
#include <vector>
#include <cstdio>
using namespace ttyd;
namespace {
struct Fixture {
 bool alive=true,small=true,large=true;int hold=-1,left=2,accepted=0,onCount=0,draws=0;int32_t pitch=0;uint32_t volume=0,key=0;
 std::vector<std::string> trace;std::shared_ptr<int> life=std::make_shared<int>(1);
 SourceBattleVoiceServices services;SourceBattleVoiceCursor cursor;EventEntry event;
 std::map<uint32_t,uint32_t> words{{0x8041eb90,0x5000},{0x6000,1},{0x6004,1},{0x80416ad8,0x7000},{0x80416aec,0x7100}};
 bool step(const std::string& label){if(accepted==hold && left){--left;return false;}++accepted;trace.push_back(label);return true;}
 Fixture(){event.id=1;event.currentCommand=0x8000;event.arguments=0x6000;auto& s=services;s.user=this;s.lifetime=life;
  s.current=[](void* p){return static_cast<Fixture*>(p)->alive;};
  s.word=[](void* p,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);if(!f.step("word"+std::to_string(a)))return {};const auto it=f.words.find(a);if(it==f.words.end())return {};return it->second;};
  s.value=[](void* p,EventEntry&,int32_t value)->std::optional<int32_t>{auto& f=*static_cast<Fixture*>(p);if(!f.step("value"+std::to_string(value)))return {};return value;};
  s.translate=[](void* p,EventEntry&,int32_t value)->std::optional<int32_t>{auto& f=*static_cast<Fixture*>(p);if(!f.step("translate"+std::to_string(value)))return {};return value;};
  s.unit=[](void* p,uint32_t b,int32_t id)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);assert(b==0x5000 && id==1);if(!f.step("unit"))return {};return 0x9000;};
  s.random=[](void* p,int32_t bound)->std::optional<int32_t>{auto& f=*static_cast<Fixture*>(p);if(!f.step("random"+std::to_string(bound)))return {};++f.draws;return bound==41?40:2;};
  s.position=[](void* p,uint32_t unit)->std::optional<std::array<float,3>>{auto& f=*static_cast<Fixture*>(p);assert(unit==0x9000);if(!f.step("position"))return {};return std::array<float,3>{1,2,3};};
  s.checkStatus=[](void* p,uint32_t unit,int32_t id)->std::optional<int32_t>{auto& f=*static_cast<Fixture*>(p);assert(unit==0x9000);if(!f.step("check"+std::to_string(id)))return {};return id==11?f.small:f.large;};
  s.getStatus=[](void* p,uint32_t,int32_t id)->std::optional<StatusValue>{auto& f=*static_cast<Fixture*>(p);if(!f.step("status"+std::to_string(id)))return {};return StatusValue{1,uint8_t(id==11?-2:3)};};
  s.on=[](void* p,uint32_t key,const std::array<float,3>& pos)->std::optional<int32_t>{auto& f=*static_cast<Fixture*>(p);assert((pos==std::array<float,3>{1,2,3}));if(!f.step("on"))return {};++f.onCount;f.key=key;return 0x127;};
  s.pitch=[](void* p,int32_t handle,int32_t pitch){auto& f=*static_cast<Fixture*>(p);assert(handle==0x127);if(!f.step("pitch"))return false;f.pitch=pitch;return true;};
  s.volume=[](void* p,int32_t handle)->std::optional<uint8_t>{auto& f=*static_cast<Fixture*>(p);assert(handle==0x127);if(!f.step("volume"))return {};return 73;};
  s.setVolume=[](void* p,int32_t handle,uint32_t volume){auto& f=*static_cast<Fixture*>(p);assert(handle==0x127);if(!f.step("setVolume"))return false;f.volume=volume;return true;};
 }
 void run(){int32_t result=-9;for(unsigned i=0;i<4;++i)if(resumeSourceBattleVoice(event,services,cursor,result)){assert(result==2);return;}assert(false);}
};
}
int main(){
 for(int mode:{0,1,2,3}){
  Fixture reference;reference.words[0x6004]=uint32_t(mode);reference.run();
  const int count=reference.accepted;
  for(int hold=0;hold<count;++hold){Fixture f;f.words[0x6004]=uint32_t(mode);f.hold=hold;f.run();assert(f.trace==reference.trace && f.draws==reference.draws && f.onCount==reference.onCount && f.pitch==reference.pitch && f.volume==reference.volume && f.key==reference.key);}
  if(mode==1 || mode==2){assert(reference.onCount==1 && reference.draws==2 && reference.pitch==-2367 && reference.volume==73);int32_t result=0;const auto size=reference.trace.size();assert(resumeSourceBattleVoice(reference.event,reference.services,reference.cursor,result));assert(reference.trace.size()==size);}
  else assert(!reference.onCount && !reference.draws);
 }
 for(uint32_t volume:{0xf1194d80u,0xf1194d81u,0u,0xffffffffu}){
  Fixture f;f.small=false;f.large=false;int32_t handle=0;
  assert(resumeSourceBattleUnitSound(0x9000,0x7000,volume,32767,f.services,f.cursor,handle));
  assert(handle==0x127 && f.pitch==-32749 && f.volume==(volume==0xf1194d80u?73:volume&255));
 }
 Fixture noop;noop.words[0x6004]=0;noop.services.random=nullptr;noop.services.on=nullptr;noop.run();assert(!noop.draws && !noop.onCount);
 for(unsigned flags=0;flags<4;++flags){Fixture f;f.small=(flags&1)!=0;f.large=(flags&2)!=0;f.run();assert(f.pitch==(f.large?-2367:f.small?1725:20));}
 Fixture retired;retired.hold=16;int32_t result=-9;resumeSourceBattleVoice(retired.event,retired.services,retired.cursor,result);const auto count=retired.accepted;retired.alive=false;assert(!resumeSourceBattleVoice(retired.event,retired.services,retired.cursor,result));assert(retired.accepted==count && result==-9);
 Fixture replaced;replaced.hold=1;assert(!resumeSourceBattleVoice(replaced.event,replaced.services,replaced.cursor,result));auto services=replaced.services;services.user=&retired;assert(!resumeSourceBattleVoice(replaced.event,services,replaced.cursor,result));
 std::puts("Retained voice modes/status precedence/raw sentinel/word wrap/every-provider double holds pass");
}
