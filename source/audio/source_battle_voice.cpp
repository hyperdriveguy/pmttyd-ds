#include "source_battle_voice.hpp"
#include <tuple>
namespace ttyd {
namespace {
constexpr uint32_t BattleGlobal=0x8041eb90;
constexpr uint32_t FirstVoiceTable=0x80416ad0,SecondVoiceTable=0x80416ae4;
constexpr uint32_t DefaultVolumeSentinel=0xf1194d80;
constexpr int32_t SmallStatus=11,LargeStatus=10;
constexpr int32_t StatusPitchMultiplier=-341,PitchDrawBound=41,PitchCentre=20;
constexpr uint32_t PreserveVolumeBias=0x0ee70000,PreserveVolumeCompare=0x4d80;
int32_t signedByte(uint8_t v){return v<128?int32_t(v):int32_t(v)-256;}
int32_t signedHalf(uint32_t v){v&=65535;return v<32768?int32_t(v):int32_t(v)-65536;}
auto identity(const SourceBattleVoiceServices& s){return std::make_tuple(s.user,s.lifetime.get(),s.current,s.word,s.value,s.translate,s.unit,s.random,s.position,s.checkStatus,s.getStatus,s.on,s.pitch,s.volume,s.setVolume);}
}
struct SourceBattleVoiceState {
 enum class Step {Battle,SelectorWord,SelectorValue,ModeWord,ModeValue,Translate,Unit,Mode,VoiceRandom,VoiceKey,Position,CheckSmall,GetSmall,CheckLarge,GetLarge,PitchRandom,Start,Pitch,Volume,SetVolume,Done};
 SourceBattleVoiceServices services;EventEntry* event=nullptr;uint32_t eventId=0,command=0,args=0;
 Step step=Step::Battle;uint32_t pending=0,battle=0,raw=0,unit=0,key=0;int32_t selector=0,mode=0,translated=0,draw=0,offset=0,pitch=0,handle=0;uint32_t volume=0;
 std::array<float,3> position{};bool invoking=false,voice=true;uint32_t rawVolume=DefaultVolumeSentinel;int32_t belong=0;
};
uint32_t SourceBattleVoiceCursor::pending()const{return state_?state_->pending:0;}
namespace {
bool drive(EventEntry* event,const SourceBattleVoiceServices& s,SourceBattleVoiceState& c,int32_t& result){
 using Step=SourceBattleVoiceState::Step;
 if(c.invoking)return false;
 struct Guard {bool& flag;Guard(bool& x):flag(x){flag=true;}~Guard(){flag=false;}} guard(c.invoking);
 auto alive=[&]{return s.current(s.user);};
 for(;;){if(!alive())return false;
  switch(c.step){
  case Step::Battle:{c.pending=0x80104520;auto v=s.word?s.word(s.user,BattleGlobal):std::nullopt;if(!v)return false;c.battle=*v;c.step=Step::SelectorWord;break;}
  case Step::SelectorWord:{c.pending=0x80104524;auto v=s.word?s.word(s.user,c.args):std::nullopt;if(!v)return false;c.raw=*v;c.step=Step::SelectorValue;break;}
  case Step::SelectorValue:{c.pending=0x80031260;auto v=s.value?s.value(s.user,*event,int32_t(c.raw)):std::nullopt;if(!v)return false;c.selector=*v;c.step=Step::ModeWord;break;}
  case Step::ModeWord:{c.pending=0x8010452c;auto v=s.word?s.word(s.user,c.args+4):std::nullopt;if(!v)return false;c.raw=*v;c.step=Step::ModeValue;break;}
  case Step::ModeValue:{c.pending=0x80031260;auto v=s.value?s.value(s.user,*event,int32_t(c.raw)):std::nullopt;if(!v)return false;c.mode=*v;c.step=Step::Translate;break;}
  case Step::Translate:{c.pending=0x801253ec;auto v=s.translate?s.translate(s.user,*event,c.selector):std::nullopt;if(!v)return false;c.translated=*v;c.step=Step::Unit;break;}
  case Step::Unit:{c.pending=0x800f88cc;auto v=s.unit?s.unit(s.user,c.battle,c.translated):std::nullopt;if(!v)return false;c.unit=*v;c.step=Step::Mode;break;}
  case Step::Mode:c.step=(c.mode==1 || c.mode==2)?Step::VoiceRandom:Step::Done;break;
  case Step::VoiceRandom:{c.pending=0x80005da0;auto v=s.random?s.random(s.user,c.mode==1?5:10):std::nullopt;if(!v)return false;c.draw=*v;c.step=Step::VoiceKey;break;}
  case Step::VoiceKey:{c.pending=c.mode==1?0x80104588:0x801045b4;auto v=s.word?s.word(s.user,(c.mode==1?FirstVoiceTable:SecondVoiceTable)+(uint32_t(c.draw)<<2)):std::nullopt;if(!v)return false;c.key=*v;c.step=Step::Position;break;}
  case Step::Position:{c.pending=0x80128c7c;auto v=s.position?s.position(s.user,c.unit):std::nullopt;if(!v)return false;c.position=*v;c.step=Step::CheckSmall;break;}
  case Step::CheckSmall:{c.pending=0x80127814;auto v=s.checkStatus?s.checkStatus(s.user,c.unit,SmallStatus):std::nullopt;if(!v)return false;c.step=*v?Step::GetSmall:Step::CheckLarge;break;}
  case Step::GetSmall:{c.pending=0x80127f34;auto v=s.getStatus?s.getStatus(s.user,c.unit,SmallStatus):std::nullopt;if(!v)return false;c.offset=signedHalf(uint32_t(2*signedByte(v->strength)-1)*uint32_t(StatusPitchMultiplier));c.step=Step::CheckLarge;break;}
  case Step::CheckLarge:{c.pending=0x80127814;auto v=s.checkStatus?s.checkStatus(s.user,c.unit,LargeStatus):std::nullopt;if(!v)return false;c.step=*v?Step::GetLarge:Step::PitchRandom;break;}
  case Step::GetLarge:{c.pending=0x80127f34;auto v=s.getStatus?s.getStatus(s.user,c.unit,LargeStatus):std::nullopt;if(!v)return false;c.offset=signedHalf(uint32_t(2*signedByte(v->strength)+1)*uint32_t(StatusPitchMultiplier));c.step=Step::PitchRandom;break;}
  case Step::PitchRandom:{c.pending=0x80005da0;auto v=s.random?s.random(s.user,PitchDrawBound):std::nullopt;if(!v)return false;c.pitch=signedHalf(uint32_t(*v)+uint32_t(c.belong)+uint32_t(c.offset)-uint32_t(PitchCentre));c.step=Step::Start;break;}
  case Step::Start:{c.pending=0x800d9848;auto v=s.on?s.on(s.user,c.key,c.position):std::nullopt;if(!v)return false;c.handle=*v;c.step=Step::Pitch;break;}
  case Step::Pitch:c.pending=0x800d9744;if(!s.pitch || !s.pitch(s.user,c.handle,c.pitch))return false;if(c.rawVolume+PreserveVolumeBias==PreserveVolumeCompare)c.step=Step::Volume;else{c.volume=c.rawVolume&255;c.step=Step::SetVolume;}break;
  case Step::Volume:{c.pending=0x800d9558;auto v=s.volume?s.volume(s.user,c.handle):std::nullopt;if(!v)return false;c.volume=*v;c.step=Step::SetVolume;break;}
  case Step::SetVolume:c.pending=0x800d97d0;if(!s.setVolume || !s.setVolume(s.user,c.handle,c.volume))return false;c.step=Step::Done;break;
  case Step::Done:c.pending=0;result=c.voice?2:c.handle;return true;
  }
 }
}
} // namespace
bool resumeSourceBattleVoice(EventEntry& e,const SourceBattleVoiceServices& s,SourceBattleVoiceCursor& cursor,int32_t& result){
 if(!s.lifetime || !s.current || !s.current(s.user))return false;
 if(!cursor.state_){cursor.state_=std::make_shared<SourceBattleVoiceState>();auto& c=*cursor.state_;c.services=s;c.event=&e;c.eventId=e.id;c.command=e.currentCommand;c.args=e.arguments;}
 auto held=cursor.state_;auto& c=*held;
 if(!c.voice || c.event!=&e || c.eventId!=e.id || c.command!=e.currentCommand || identity(c.services)!=identity(s))return false;
 return drive(&e,s,c,result);
}
bool resumeSourceBattleUnitSound(uint32_t unit,uint32_t key,uint32_t rawVolume,int32_t belong,const SourceBattleVoiceServices& s,SourceBattleVoiceCursor& cursor,int32_t& result){
 if(!s.lifetime || !s.current || !s.current(s.user))return false;
 if(!cursor.state_){cursor.state_=std::make_shared<SourceBattleVoiceState>();auto& c=*cursor.state_;c.services=s;c.voice=false;c.unit=unit;c.key=key;c.rawVolume=rawVolume;c.belong=belong;c.step=SourceBattleVoiceState::Step::Position;}
 auto held=cursor.state_;auto& c=*held;
 if(c.voice || c.unit!=unit || c.key!=key || c.rawVolume!=rawVolume || c.belong!=belong || identity(c.services)!=identity(s))return false;
 return drive(nullptr,s,c,result);
}
}
