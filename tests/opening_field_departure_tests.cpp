#include "game.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
using namespace ttyd;
static OpeningHarborScene handoff() {
 OpeningHarborScene s;s.begin(100);s.advance(580);s.messageComplete(0);s.advance(700);s.messageComplete(1);
 s.advance(784);assert(s.paperCapturePending);s.captureComplete();s.advance(844);s.presented(60);s.advance(845);
 s.advance(988);assert(s.departureAt==989);s.advance(1168);assert(!s.storyPublished);s.advance(1169);assert(s.storyPublished && s.visible && !s.complete);
 return s;
}
int main() {
 const auto original=handoff();OpeningFieldDeparture owner;
 auto before=original;before.storyPublished=false;assert(!owner.begin(before,0));
 before=original;before.paperDone=false;assert(!owner.begin(before,0));
 assert(owner.begin(original,50));assert(owner.scene.now==original.now);
 const float initial=owner.scene.x;owner.advance(51,1);assert(owner.active && owner.scene.x<initial && owner.scene.now==original.now+1);
 owner.advance(349,1);assert(owner.active && owner.scene.visible);owner.advance(350,1);assert(!owner.active && owner.scene.complete && owner.scene.x==-1260);
 OpeningFieldDeparture dense,sparse;assert(dense.begin(original,0) && sparse.begin(original,0));
 for(unsigned i=0;i<=299;++i)dense.advance(i,1);sparse.advance(299,1);
 assert(dense.active && sparse.active && dense.scene.x==sparse.scene.x && dense.scene.parts==sparse.scene.parts && dense.scene.rock==sparse.scene.rock);
 owner={};assert(owner.begin(original,0));owner.advance(1,0);assert(!owner.active);
 Game game;Progress fresh;fresh.room=1;fresh.position={0,2.25f};fresh.originalStory=3;game.restore(fresh);
 assert(game.scene==Scene::Explore && game.retainOpeningDeparture(original));
 const float start=game.progress.position.x;Input left;left.x=-1;
 for(unsigned i=0;i<20;++i)game.tick(left);
 assert(game.openingDeparture.active && game.progress.position.x<start && game.openingDeparture.scene.now==original.now+20);
 // A field input owner can lock Mario independently of the existing child.
 game.scene=Scene::Dialogue;const float locked=game.progress.position.x;
 for(unsigned i=20;i<299;++i)game.tick(left);
 assert(game.progress.position.x==locked && game.openingDeparture.active && game.openingDeparture.scene.visible);
 game.tick(left);assert(!game.openingDeparture.active && game.openingDeparture.scene.complete && game.progress.originalStory==3);
 std::cout<<"Original +180 field release, live input, independent child clock, +480 retirement and unload passed\n";
}
