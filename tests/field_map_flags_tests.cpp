#include "game.hpp"
#include <algorithm>
#include <cassert>
#include <cstdio>
using namespace ttyd;
static uint32_t vertices(const char* path){FILE* f=std::fopen(path,"rb");assert(f);char magic[8];uint32_t n=0;assert(std::fread(magic,8,1,f)==1 && std::fread(&n,4,1,f)==1);std::fclose(f);return n;}
int main(){
 assert(loadFieldMapFlagAssets("romfs") && loadFieldDoorDataAssets("romfs"));
 FieldMapFlags flags;flags.synchronize(2);const auto count=vertices("romfs/gor_02.mapflags");assert(flags.ready(count));
 auto visible=[&](){uint32_t n=0;flags.visibleRanges(0,count,[&](uint32_t first,uint32_t size){assert(!(first%3) && !(size%3));n+=size;});return n;};
 assert(visible()==count && flags.disabledHits().empty());
 FieldDoorDataWorld door;assert(door.initialize(2,uint8_t(0)) && door.initializeHouse05Map(flags));
 assert(flags.mapFlags("S_ie_05_naka")==1 && flags.hitEnabled("A_ie_05C")==false);
 const auto hidden=visible();assert(hidden && hidden<count);const auto disabled=flags.disabledHits();assert(!disabled.empty());
 assert(std::is_sorted(disabled.begin(),disabled.end()));
 assert(flags.mapFlags("S_ie_04_naka")==0 && flags.hitEnabled("A_ie_04C")==true);
 // A completed source init must not replay over a later event mutation.
 assert(flags.mapFlag("S_ie_05_naka",1,false) && flags.hitEnabled("A_ie_05C",true));
 assert(door.initializeHouse05Map(flags));assert(visible()==count && flags.disabledHits().empty());
 assert(!flags.mapFlag("missing",1,true) && !flags.hitEnabled("S_ie_05_naka",false));
 flags.synchronize(0);assert(!flags.ready(count));flags.synchronize(2);
 assert(door.initializeHouse05Map(flags) && visible()==hidden && flags.disabledHits()==disabled);
 // The actual Game NPC/init tail owns the original two requests.
 assert(loadFieldActorCatalog("romfs") && loadHarborStoryAssets("romfs"));
 assert(loadGround("romfs/gor_02.ground") && loadCollision("romfs/gor_02.walls") && loadHead("romfs/gor_02.head"));
 Game game(false);game.progress.room=2;game.progress.originalStory=23;game.progress.position={5.2f,2.f};
 initializeTroubleQuestState(game.progress.originalNpcTalkState);game.tick({});
 assert(game.fieldMapFlags.mapFlags("S_ie_05_naka")==1 && game.fieldMapFlags.disabledHits()==disabled);
 // Actual source elevated box surface centroid, as an isolated query fixture.
 const Vec2 floor{-1.211952f,-4.162082f};float height=0;GroundContact contact;
 const auto originalFloor=0x12e490u;
 assert(!groundAt(floor,.35f,height,&contact) || contact.sourceNode!=originalFloor);
 assert(game.fieldMapFlags.hitEnabled("A_ie_05C",true));game.tick({});
 assert(groundAt(floor,.35f,height,&contact) && contact.sourceNode==originalFloor);
 assert(game.fieldMapFlags.hitEnabled("A_ie_05C",false));game.tick({});
 assert(!groundAt(floor,.35f,height,&contact) || contact.sourceNode!=originalFloor);
 game.scene=Scene::Victory;game.progress.room=0;game.tick({});assert(game.fieldMapFlags.disabledHits().empty());
 game.progress.room=2;game.tick({});assert(game.fieldMapFlags.disabledHits().empty());
 game.scene=Scene::Explore;game.tick({});assert(game.fieldMapFlags.disabledHits()==disabled);
}
