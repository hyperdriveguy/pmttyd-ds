#include "platform.hpp"
namespace ttyd {
namespace {constexpr uint32_t OriginalClassQueuedDraw=0x8012210c,OriginalWeaponQueuedDraw=0x80121ce8;}
bool Renderer::resumeHarborCommandDisplay(const Game&game,const HarborSourceDisplayInvocation&q){
 if(!harborSourceFrameActive||game.trioBattleActive||!game.harborTutorial.running())return false;
 const auto factory=game.harborTutorial.sourceMenuFactory();if(!factory||!factory->current())return false;
 if(q.callback==OriginalClassQueuedDraw)return resumeHarborClassMenu(game,q);
 if(q.callback==OriginalWeaponQueuedDraw)return resumeHarborWeaponMenu(game,q);
 return false; // Actual Target and other source draw children remain unbound.
}
void Renderer::resumeHarborSourceCamera(const Game&game){
 if(!harborSourceFrameActive||game.trioBattleActive||!game.harborTutorial.running())return;
 const auto factory=game.harborTutorial.sourceMenuFactory();if(factory)factory->resumeDisplay();
}
}
