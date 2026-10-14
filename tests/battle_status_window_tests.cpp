#include "battle_status_window.hpp"
#include <cassert>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
 std::map<uint32_t,uint32_t> words{{0x8041eb90,0x10000},{0x1000,0}};std::vector<uint32_t> reads;
 EventEntry event;bool visible=false,recapture=false;unsigned opens=0,closes=0,checks=0;
 Fixture(){event.arguments=0x1000;}
 BattleStatusWindowServices services(){BattleStatusWindowServices s;s.user=this;s.units.memory.user=this;s.units.values.user=this;
 s.units.memory.word=[](void* p,uint32_t at){auto& f=*static_cast<Fixture*>(p);f.reads.push_back(at);return f.words[at];};
 s.units.memory.storeWord=[](void* p,uint32_t at,uint32_t value){static_cast<Fixture*>(p)->words[at]=value;};
 s.units.values.resolve=[](void* p,EventEntry&,int32_t value){auto& f=*static_cast<Fixture*>(p);if(f.recapture)f.words[0x8041eb90]=0x20000;return value;};
 s.isOpen=[](void* p){auto& f=*static_cast<Fixture*>(p);++f.checks;return f.visible;};s.open=[](void* p){auto& f=*static_cast<Fixture*>(p);++f.opens;f.visible=true;};s.close=[](void* p){auto& f=*static_cast<Fixture*>(p);++f.closes;f.visible=false;};return s;}
};
int main(){
 for(bool show:{false,true})for(bool current:{false,true}){Fixture f;f.visible=current;f.words[0x1274c]=show;assert(packedBattleStatusWindowCheck(f.services())==int(show) && f.checks==1 && f.opens==unsigned(show && !current) && f.closes==unsigned(!show && current));}
 for(auto blocker:{0x273cu,0x2740u}){Fixture f;f.words[0x10000+blocker]=0xffffffff;f.words[0x1274c]=1;f.words[0x12748]=1;f.words[0x12744]=1;assert(!packedBattleStatusWindowCheck(f.services()));}
 Fixture show;show.words[0x12748]=1;assert(!packedBattleStatusWindowCheck(show.services()));show.words[0x12744]=1;assert(packedBattleStatusWindowCheck(show.services())==1);packedBattleStatusWindowSet(0x2744,false,show.services());assert(!show.visible && show.words[0x12748]==1 && !show.words[0x12744]);packedBattleStatusWindowSet(0x2744,true,show.services());assert(show.visible);
 for(int argument:{0,1,2,-1,3}){Fixture f;f.words[0x1000]=uint32_t(argument);f.words[0x12748]=77;f.words[0x12744]=88;assert(battleStatusWindowOnOffEvent(f.event,f.services())==2 && f.words[0x12748]==uint32_t(argument==0 || argument==1?argument:77) && f.words[0x12744]==uint32_t(argument==2?1:88) && f.checks==1);}
 Fixture reload;reload.words[0x1000]=1;reload.recapture=true;reload.words[0x2274c]=1;assert(battleStatusWindowOnOffEvent(reload.event,reload.services())==2 && reload.words[0x12748]==1 && reload.words[0x22748]==0 && reload.visible);
}
