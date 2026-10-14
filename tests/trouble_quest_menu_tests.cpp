#include "trouble_quest_menu.hpp"
#include <cassert>
#include <string>
#include <vector>
using namespace ttyd;
struct Harness {
 std::optional<int32_t> active=0;
 std::optional<bool> completed=false;
 std::vector<std::string> calls;
 int32_t flagRead=0,published=0;
 unsigned displays=0;
};
TroubleQuestMenuServices services(Harness& h) {
 return {&h,[](void*){return true;},[](void* p){return static_cast<Harness*>(p)->active;},
 [](void* p,int32_t flag){auto& h=*static_cast<Harness*>(p);h.flagRead=flag;return h.completed;},
 [](void* p,const char* n,int32_t id){static_cast<Harness*>(p)->calls.push_back(std::string("enter:")+n+":"+std::to_string(id));},
 [](void* p,const char* n,uint32_t on,uint32_t off){static_cast<Harness*>(p)->calls.push_back(std::string("flags:")+n+":"+std::to_string(on)+":"+std::to_string(off));},
 [](void* p,const char* n,float scale){assert(scale==.8f);static_cast<Harness*>(p)->calls.push_back(std::string("scale:")+n);},
 [](void* p,const char* n,float x,float y,float z){assert(z==0);if(std::string(n)=="iraiArrowUp")assert(x==231 && y==178);static_cast<Harness*>(p)->calls.push_back(std::string("pos:")+n);},
 [](void* p,const char* n){static_cast<Harness*>(p)->calls.push_back(std::string("delete:")+n);},
 [](void* p,int32_t id){static_cast<Harness*>(p)->calls.push_back("sound:"+std::to_string(id));},
 [](void* p,int32_t id){auto& h=*static_cast<Harness*>(p);h.published=id;h.calls.push_back("publish:"+std::to_string(id));},
 [](void* p,TroubleQuestMenuWork&){auto& h=*static_cast<Harness*>(p);++h.displays;h.calls.push_back("display");}};
}
int main() {
 for(const auto row:std::vector<std::pair<int32_t,int32_t>>{{-1,6},{55,6},{56,9},{111,9},{112,12},{163,12},{164,15},{214,15},{215,18},{260,18},{261,20},{280,20},{281,21},{338,21},{339,24},{372,24},{373,27},{400,27},{401,30},{INT32_MAX,30}}) {
  const auto w=originalTroubleQuestList(row.first);assert(w.count==row.second);
  for(int i=0;i<w.count;++i)assert(w.quests[i]==(i==18?20:i==19?21:i==20?19:i+1));
  for(int i=w.count;i<30;++i)assert(w.quests[i]==0);
 }
 Harness h;TroubleQuestMenu menu;auto s=services(h);s.publish=nullptr;assert(!menu.begin(8,s) && h.calls.empty());
 assert(menu.begin(8,services(h)) && !menu.begin(8,services(h)));
 assert(menu.invoke({0x100})==0 && menu.work().state==1 && h.displays==1); // Init does not process A.
 assert(h.calls.front()=="enter:iraiYubi:504" && h.calls[1]=="flags:iraiYubi:4:0" && h.calls.back()=="display");
 assert(h.calls[h.calls.size()-2]=="sound:42");
 TroubleQuestMenu copy=menu;assert(!copy.canAdvance() && !copy.invoke({0x100}));
 assert(menu.invoke({0,0x2000,0x2000})==0 && menu.work().cursor==1 && menu.work().pointerY==90);
 h.active.reset();const auto count=h.calls.size();assert(!menu.invoke({0x100}) && h.calls.size()==count);
 h.active=2;h.completed=false;h.flagRead=0;assert(menu.invoke({0x100})==0 && menu.work().state==1 && !h.flagRead);
 h.active=0;h.completed=true;assert(menu.invoke({0x100})==0 && menu.work().state==1 && h.flagRead==5376);
 h.completed.reset();assert(!menu.invoke({0x100}) && menu.work().state==1);
 h.completed=false;h.calls.clear();const auto draws=h.displays;
 assert(menu.invoke({0x300})==0 && menu.work().state==100 && h.published==0); // Valid A wins over B.
 assert((h.calls==std::vector<std::string>{"sound:18","display"}) && h.displays==draws+1);
 assert(menu.invoke({})==2 && h.published==2 && menu.work().state==0 && h.displays==draws+1);
 assert(h.calls[2]=="publish:2" && h.calls[3]=="delete:iraiYubi" && h.calls.back()=="sound:43");
 assert(menu.invoke({})==0 && menu.work().cursor==1); // Confirmation No can reopen same work; not new list.
 h.completed=true;assert(menu.invoke({0x300})==0 && menu.work().state==101); // Invalid A permits B.
 assert(menu.invoke({})==2 && h.published==-1);
 Harness big;TroubleQuestMenu pages;assert(pages.begin(401,services(big)) && pages.invoke({})==0);
 assert(pages.invoke({0,0x1000,0x1000})==0 && pages.work().cursor==29 && pages.work().top==20);
 assert(pages.invoke({0,0x2000,0})==0 && pages.work().cursor==29); // Held repeat at edge cannot wrap.
 assert(pages.invoke({0,0x3000,0x2000})==0 && pages.work().cursor==29 && pages.work().top==20); // Both repeats precede wrapping.
 assert(pages.invoke({0,0x2000,0x2000})==0 && pages.work().cursor==0 && pages.work().top==0);
 assert(pages.invoke({0x20})==0 && pages.work().cursor==10 && pages.work().top==10);
 assert(pages.invoke({0x20})==0 && pages.work().cursor==20 && pages.work().top==20);
 assert(pages.invoke({0x20})==0 && pages.work().cursor==20 && pages.work().top==20);
 assert(pages.invoke({0x40})==0 && pages.work().cursor==10 && pages.work().top==10);
 assert(pages.invoke({0x40})==0 && pages.work().cursor==0 && pages.work().top==0);
 for(int n=0;n<8;++n)assert(pages.invoke({0,0x2000,0})==0);
 assert(pages.work().cursor==8 && pages.work().top==1 && pages.work().scroll>0);
 assert(pages.work().pointerX==-129 && pages.work().pointerY==-90);
}
