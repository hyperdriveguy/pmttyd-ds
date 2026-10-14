#include "trouble_quest_presentation.hpp"
#include <cassert>
#include <vector>
using namespace ttyd;
int main() {
 MessageFont font;assert(font.load("romfs/font.pmfont") && loadMessageIcons("romfs/message-icons.bin"));
 struct State {int32_t quest=0;bool complete=false;std::vector<int32_t> sounds;} state;
 TroubleQuestStateServices s{&state,[](void*){return true;},[](void* p){return std::optional<int32_t>(static_cast<State*>(p)->quest);},
  [](void* p,int32_t id){assert(id>=5375 && id<=5404);return std::optional<bool>(static_cast<State*>(p)->complete);},
  [](void* p,int32_t cue){static_cast<State*>(p)->sounds.push_back(cue);}};
 TroubleQuestPresentation p;auto absent=s;absent.sound=nullptr;assert(!p.begin(8,font,"romfs/messages/gor_02.txt",absent));
 assert(p.begin(8,font,"romfs/messages/gor_02.txt",s) && !p.begin(8,font,"romfs/messages/gor_02.txt",s));
 assert(p.invoke({})==0 && p.displayed() && !p.result());
 assert(p.icons()[0].flags==0x1017 && p.icons()[0].frame==0 && p.icons()[0].delay==2);
 assert(p.icons()[1].flags==0x1b && p.icons()[2].flags==0x13 && p.icons()[3].scale==.8f && p.icons()[4].scale==.8f);
 for(const auto& icon:p.icons())assert(!icon.visible());
 p.iconMain();assert(p.icons()[0].frame==0 && p.icons()[0].delay==1); // Hidden initialization visit still advances.
 assert(p.invoke({})==0 && p.icons()[0].visible());p.iconMain();
 assert(p.icons()[0].frame==1 && p.icons()[0].delay==2); // Actual first visible source frame.
 assert(!p.icons()[1].visible() && !p.icons()[2].visible()); // Six-row menu has no paging arrows/buttons.
 TroubleQuestPresentation copy=p;assert(!copy.canAdvance() && !copy.displayWork() && !copy.invoke({0x100}));
 const auto before=p.icons()[0].frame;copy.iconMain();assert(p.icons()[0].frame==before);
 for(int n=0;n<16;++n)p.iconMain();assert(p.icons()[0].frame==9 && p.icons()[0].delay==28);
 for(int n=0;n<27;++n)p.iconMain();assert(p.icons()[0].frame==9 && p.icons()[0].delay==1);
 p.iconMain();assert(p.icons()[0].frame==0 && p.icons()[0].delay==2); //46 visits, with no draw advancement.
 p.iconMain(false);assert(p.icons()[0].delay==2);
 assert(p.invoke({0,0x2000,0x2000})==0 && p.work().cursor==1 && p.icons()[0].y==90);
 assert(p.invoke({0x100})==0 && !p.result() && p.displayed());
 assert(p.invoke({})==2 && p.result()==2 && !p.displayed());
 for(const auto& icon:p.icons())assert(!(icon.flags&1));
 p.iconMain();assert(p.icons()[0].frame==0);
 assert((state.sounds==std::vector<int32_t>{42,5,18,43}));
 // Source No re-enters same menu work, with a fresh local icon phase.
 assert(p.invoke({})==0 && p.work().cursor==1 && p.icons()[0].frame==0 && p.icons()[0].delay==2);
 state.quest=2;assert(p.invoke({0x300})==0 && p.displayed()); // Invalid A allows B.
 assert(p.invoke({})==2 && p.result()==-1 && state.sounds.back()==43);
}
