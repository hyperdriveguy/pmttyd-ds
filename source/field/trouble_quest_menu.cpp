#include "trouble_quest_menu.hpp"
#include <algorithm>
namespace ttyd {
namespace {
constexpr const char* Names[]={"iraiYubi","iraiArrowUp","iraiArrowDown","iraiLButton","iraiRButton"};
}
TroubleQuestMenuWork originalTroubleQuestList(int32_t story) {
 TroubleQuestMenuWork w;
 for(int32_t i=0;i<6;++i)w.quests[i]=i+1;
 for(const auto row:std::array<std::array<int32_t,3>,6>{{{{56,6,3}},{{112,9,3}},{{164,12,3}},{{215,15,3}},{{339,21,3}},{{373,24,3}}}})
  if(story>=row[0])for(int32_t i=0;i<row[2];++i)w.quests[row[1]+i]=row[1]+i+1;
 if(story>=261){w.quests[18]=20;w.quests[19]=21;}
 if(story>=281)w.quests[20]=19;
 if(story>=401)for(int32_t i=27;i<30;++i)w.quests[i]=i+1;
 for(int32_t quest:w.quests)if(quest)++w.count; // Source counts, never compacts/sorts.
 return w;
}
bool TroubleQuestMenu::begin(int32_t story,const TroubleQuestMenuServices& s) {
 if(owner_ || !s.ready || !s.activeQuest || !s.flag || !s.iconEnter || !s.iconFlags || !s.iconScale ||
    !s.iconPosition || !s.iconDelete || !s.sound || !s.publish || !s.display || !s.ready(s.user))return false;
 work_=originalTroubleQuestList(story);services_=s;owner_=this;return true;
}
void TroubleQuestMenu::present() {
 auto& w=work_;auto& s=services_;
 w.scroll=(float(w.top*30)-w.scroll)/6.f+w.scroll;
 w.pointerX=-129.f;w.pointerY=float(120-(w.cursor-w.top)*30);
 s.iconPosition(s.user,Names[0],w.pointerX,w.pointerY,0);s.iconFlags(s.user,Names[0],0,2);
 s.iconPosition(s.user,Names[1],231,178,0);s.iconPosition(s.user,Names[2],231,-190,0);
 s.iconPosition(s.user,Names[3],261,146,0);s.iconPosition(s.user,Names[4],261,-190,0);
 const bool first=w.top==0,last=w.top>=w.count-10;
 for(unsigned i:{1u,3u})s.iconFlags(s.user,Names[i],first?2:0,first?0:2);
 for(unsigned i:{2u,4u})s.iconFlags(s.user,Names[i],last?2:0,last?0:2);
 s.display(s.user,w);
}
std::optional<int32_t> TroubleQuestMenu::invoke(TroubleQuestMenuInput input) {
 if(!canAdvance())return {};
 auto& w=work_;auto& s=services_;
 if(w.state==0) {
  s.iconEnter(s.user,Names[0],504);s.iconFlags(s.user,Names[0],4,0);
  s.iconEnter(s.user,Names[1],524);s.iconFlags(s.user,Names[1],8,0);
  s.iconEnter(s.user,Names[2],524);
  s.iconEnter(s.user,Names[3],134);s.iconScale(s.user,Names[3],.8f);
  s.iconEnter(s.user,Names[4],136);s.iconScale(s.user,Names[4],.8f);
  for(const char* name:Names)s.iconFlags(s.user,name,2,0);
  w.state=1;s.sound(s.user,42);s.display(s.user,w);return 0;
 }
 if(w.state==100 || w.state==101) {
  s.publish(s.user,w.state==100?w.quests[w.cursor]:-1);w.state=0;
  for(const char* name:Names)s.iconDelete(s.user,name);
  s.sound(s.user,43);return 2;
 }
 if(w.state!=1){s.display(s.user,w);return 0;}
 if(input.buttons&0x100) {
  const auto active=s.activeQuest(s.user);if(!active)return {};
  if(*active==0) {
   const auto complete=s.flag(s.user,5374+w.quests[w.cursor]);if(!complete)return {};
   if(!*complete){s.sound(s.user,18);w.state=100;s.display(s.user,w);return 0;}
  }
 }
 if(input.buttons&0x200){s.sound(s.user,19);w.state=101;s.display(s.user,w);return 0;}
 const int32_t old=w.cursor;
 if((input.directionRepeat&0x2000) && (w.cursor<w.count-1 || (input.directionTrigger&0x2000))) {
  ++w.cursor;s.sound(s.user,5);
 }
 if((input.directionRepeat&0x1000) && (w.cursor>0 || (input.directionTrigger&0x1000))) {
  --w.cursor;s.sound(s.user,5);
 }
 if(w.cursor>=w.count){w.cursor=0;w.top=0;}
 if(w.cursor<0){w.cursor=w.count-1;if(w.count>10)w.top=w.count-10;}
 if(w.cursor>old && w.cursor-w.top>7 && w.top<w.count-10)++w.top;
 if(w.cursor<old && w.cursor-w.top<3 && w.top>0)--w.top;
 if((input.buttons&0x40) && w.top>0){const int32_t n=std::min<int32_t>(w.top,10);w.top-=n;w.cursor-=n;s.sound(s.user,5);}
 if((input.buttons&0x20) && w.top<w.count-10){const int32_t n=std::min<int32_t>(w.count-10-w.top,10);w.top+=n;w.cursor+=n;s.sound(s.user,5);}
 if(w.cursor!=w.previous)w.counter94=0;
 w.previous=w.cursor;present();return 0;
}
}
