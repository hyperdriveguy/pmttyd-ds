#include "game.hpp"
#include <cassert>
#include <limits>
using namespace ttyd;
int main() {
 FieldActorRecord rule;rule.initialization=FieldActorInit::Borodo2;FieldActorTalkState state;
 assert(!fieldActorInitialPosition(rule,state));
 state.quest1420=0;assert((fieldActorInitialPosition(rule,state)==std::array<float,3>{-475,0,-180}));
 state.quest1420=4;assert(!fieldActorInitialPosition(rule,state));
 state.flag5406=false;assert((fieldActorInitialPosition(rule,state)==std::array<float,3>{-475,0,-180}));
 state.flag5406=true;assert((fieldActorInitialPosition(rule,state)==std::array<float,3>{0,-1000,0}));
 assert(loadFieldActorCatalog("romfs") && loadHarborStoryAssets("romfs"));
 assert(loadGround("romfs/gor_02.ground") && loadCollision("romfs/gor_02.walls") && loadHead("romfs/gor_02.head"));
 const FieldActorRecord* actor=nullptr;
 for(const auto& a:fieldActorCatalog().actors())if(a.initialization==FieldActorInit::Borodo2){assert(!actor);actor=&a;}
 assert(actor && actor->eventName==std::string("\x83\x7b\x83\x8d\x83\x68\x81\x5b\x82\x51",10));
 assert(fieldActorCatalog().models()[actor->model].originalName=="c_borodo_n");
 Game game(false);game.progress.room=2;game.progress.originalStory=23;game.progress.position={2.5f,0};
 assert(!game.setFieldNpcPosition(actor->eventName,{1,2,3}));
 game.tick({});assert(!game.fieldNpcState(actor->key)->initialized); // No fabricated task state.
 initializeTroubleQuestState(game.progress.originalNpcTalkState);game.tick({});
 const auto* live=game.fieldNpcState(actor->key);assert(live && live->initialized);
 assert(live->body.root.x==-475 && live->body.root.z==-180);
 const auto yaw=live->movement.yaw;const auto flags=live->body.flags;const auto clock=live->poseAt;
 assert(game.setFieldNpcPosition(actor->eventName,{350,25,120}));
 live=game.fieldNpcState(actor->key);
 assert(live->body.previous.x==350 && live->body.previous.y==25 && live->body.root.z==120 && live->movement.root.y==25);
 assert(live->movement.yaw==yaw && live->body.flags==flags && live->poseAt==clock);
 for(int n=0;n<20;++n)game.tick({});
 live=game.fieldNpcState(actor->key);assert(live->body.root.x==350 && live->body.root.y==25 && live->body.root.z==120);
 assert(!game.setFieldNpcPosition("not-an-original-actor",{0,0,0}));
 assert(!game.setFieldNpcPosition(actor->eventName,{std::numeric_limits<float>::quiet_NaN(),0,0}));
 game.progress.originalNpcTalkState.quest1420=4;game.progress.originalNpcTalkState.flag5406=true;
 // Changing a switch does not replay init over an explicitly moved live actor.
 game.tick({});assert(game.fieldNpcState(actor->key)->body.root.x==350);
 Game completed(false);completed.progress=game.progress;completed.tick({});
 assert(completed.fieldNpcState(actor->key)->body.root.y==-1000);
}
