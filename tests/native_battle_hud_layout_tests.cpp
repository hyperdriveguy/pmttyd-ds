#include "native_battle_hud_layout.hpp"
#include "message_icons.hpp"
#include "message_window.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
#include <limits>
using namespace ttyd;
namespace {
bool near(float a,float b){return std::abs(a-b)<.0001f;}
const NativeBattleHudIconDraw&find(const NativeBattleHudPlan&p,uint32_t id,unsigned n=0){
 for(std::size_t i=0;i<p.count;++i)if(p.icons[i].icon==id){if(!n)return p.icons[i];--n;}
 assert(false);return p.icons[0];
}
bool contains(const NativeBattleHudPlan&p,uint32_t id){
 for(std::size_t i=0;i<p.count;++i)if(p.icons[i].icon==id)return true;
 return false;
}
struct Rect{float left,top,right,bottom;};
Rect rect(const NativeBattleHudIconDraw&q){
 const auto*icon=messageIcon(q.icon);assert(icon);
 const auto&f=icon->frame(0);
 // Actual messageIconImage native mapping: one .5 conversion, bottom anchor.
 const float w=f.width*q.sourceScale*MessageWindowScale;
 const float h=f.height*q.sourceScale*.5f;
 return {q.centerX-w*.5f,q.bottomY-h,q.centerX+w*.5f,q.bottomY};
}
}
int main(){
 assert(loadMessageIcons("romfs/message-icons.bin"));
 NativeBattleHudReadings in{20,20,25,25,237,17,nullptr};
 const auto plan=planNativeBattleHud(in);assert(plan);
 const auto hp=rect(find(*plan,472)),fp=rect(find(*plan,473));
 assert(near(hp.right-hp.left,84)&&near(hp.bottom-hp.top,16));
 assert(near(hp.left,60)&&near(hp.right,144)&&near(hp.top,4)&&near(hp.bottom,20));
 assert(near(fp.left,150)&&near(fp.right,234)&&near(fp.top,4)&&near(fp.bottom,20));
 const auto portrait=rect(find(*plan,427));
 assert(near(portrait.right-portrait.left,26)&&near(portrait.top,-6)&&near(portrait.bottom,20));
 const auto slash=rect(find(*plan,480));assert(near(slash.bottom,17)&&near(slash.bottom-slash.top,12));
 const auto large=rect(find(*plan,481)),small=rect(find(*plan,482));
 assert(near(large.right-large.left,16)&&near(large.bottom-large.top,16));
 assert(near(small.right-small.left,12)&&near(small.bottom-small.top,12));
 const auto counter=rect(find(*plan,475));assert(near(counter.left,284)&&near(counter.right,386)&&near(counter.top,4)&&near(counter.bottom,20));
 // Every numeral in these source-range pairs/counters remains on its banner.
 for(std::size_t i=0;i<plan->count;++i){const auto&q=plan->icons[i];if(q.icon<481||q.icon>500)continue;
  const auto b=rect(q);const auto&bar=q.centerX<145?hp:q.centerX<235?fp:counter;
  assert(b.left>=bar.left&&b.right<=bar.right&&b.top>=bar.top&&b.bottom<=bar.bottom);
  assert(q.sourceScale==1);
 }
 assert(!contains(*plan,474)&&!contains(*plan,442));
 NativeBattleStatusSnapshot status;status.partnerPortrait=428;
 status.values[NativeBattleStatusSnapshot::PartnerHp]=30;
 status.values[NativeBattleStatusSnapshot::PartnerMaxHp]=30;
 status.gaugeCount=8;
 for(unsigned i=0;i<8;++i)status.gauge[i]={uint16_t(447+i),int16_t(32*i),12};
 in.sourceStatus=&status;
 const auto complete=planNativeBattleHud(in);assert(complete&&contains(*complete,428));
 const auto bar=rect(find(*complete,474));
 assert(near(bar.left,154)&&near(bar.right,302)&&near(bar.top,22)&&near(bar.bottom,38));
 const auto marker=rect(find(*complete,442));assert(near(marker.right-marker.left,23.4f)&&near(marker.bottom,40));
 for(unsigned i=0;i<8;++i){const auto&q=find(*complete,447+i);const auto b=rect(q);
  assert(near(q.centerX,178+16*i)&&near(q.bottomY,35.5f)&&q.sourceScale==1);
  assert(b.left>=bar.left&&b.right<=bar.right&&b.top>=bar.top&&b.bottom<=bar.bottom);
 }
 // A partial AP glow is intentionally larger than the banner vertically;
 // retain original48px art/offset rather than clipping it into the full stars.
 status.gaugeCount=9;
 for(unsigned i=8;i>0;--i)status.gauge[i]=status.gauge[i-1];
 status.gauge[0]={471,224,0};
 const auto partial=planNativeBattleHud(in);assert(partial);
 const auto glow=rect(find(*partial,471));assert(near(glow.left,278)&&near(glow.right,302)&&near(glow.top,17.5f)&&near(glow.bottom,41.5f));
 for(int n:{-1,0,1,9,10,99,100,999999,std::numeric_limits<int>::max()}){
  in.hp=in.maxHp=in.fp=in.maxFp=in.coins=in.starPoints=n;
  const auto p=planNativeBattleHud(in);assert(p&&p->count<=NativeBattleHudMaximumIcons);
 }
 status.gaugeCount=10;assert(!planNativeBattleHud(in));
 NativeBattleHudReadings unknown;
 const auto missing=planNativeBattleHud(unknown);assert(missing&&missing->count==3);
 assert(!contains(*missing,473)&&!contains(*missing,475)&&!contains(*missing,474));
 std::cout<<"PASS original HUD source scale/bottom baseline; actual atlas dimensions; resource/counter numeral bounds; eight AP stars and original partial glow; unavailable data stays unavailable\n";
}
