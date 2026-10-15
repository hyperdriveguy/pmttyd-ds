#include "harbor_tutorial.hpp"
#include <cassert>
using namespace ttyd;
int main(){
 assert(loadHarborTutorialAssets("romfs"));HarborTutorial scene;
 assert(scene.beginFresh(0,10,10,4));
 assert(!scene.classMenuDrawBinding({}));
 for(unsigned n=0;n<1500 && !scene.introComplete;++n){
  if(scene.pendingService==0x80106980)scene.animationComplete(1);
  scene.tick((n+1)*1000/60,n%4==0?0x100:0);
 }
 assert(scene.introComplete && scene.initializeClassMenuResources() && scene.openClassMenu());
 const auto first=scene.classMenuDrawBinding({});assert(first && first->owner && first->allocation && first->current());
 const auto& memory=first->display.draw.memory.memory;
 assert(memory.word(memory.user,0x84001c78)==first->allocation);
 assert(first->display.draw.memory.byte(first->display.draw.memory.user,first->allocation+0x14)==0);
 std::optional<uint32_t> cursor;
 assert(first->display.cursorLookup(first->display.user,0x8400171c,14,cursor) && cursor);
 assert(memory.word(memory.user,first->allocation)==*cursor);
 // No-match is completed lookup with unwritten output, not a provider wait.
 cursor.reset();assert(first->display.cursorLookup(first->display.user,0x8400171c,0x7fffffff,cursor) && !cursor);
 // Genuine source Init releases/reallocates W, and the SDK reuses that address.
 // A retained old key is invalid even though its guest address/context match.
 assert(scene.openClassMenu());const auto second=scene.classMenuDrawBinding({});
 assert(second && second->current() && !first->current());
 assert(second->allocation==first->allocation && second->owner!=first->owner);
 assert(memory.word(memory.user,second->allocation+0xc)==0);
 assert(scene.endClassMenuResources());
}
