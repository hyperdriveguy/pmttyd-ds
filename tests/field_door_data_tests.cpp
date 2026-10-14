#include "game.hpp"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <unistd.h>
using namespace ttyd;
int main() {
 assert(loadFieldDoorDataAssets("romfs"));
 FieldDoorDataWorld world;assert(!world.initialize(2,{}));assert(!world.workAddress(11));
 assert(world.initialize(2,uint8_t(0)));assert(world.copies().size()==1);
 const auto original=world.copies()[0];assert(original.work==11 && original.address!=original.source);
 for(unsigned n=0;n<18;++n){uint32_t word=0;assert(world.readWord(original.source+4*n,word) && word==original.words[n]);}
 std::vector<std::string> normal;assert(world.npcNames(original.address,normal) && normal.size()==3);
 assert(normal.back()==std::string("\x93\x90\x91\xaf\x92\x63\x82\x51",8));
 // Init does not replay over a later source mutation or switch write.
 assert(world.initialize(2,uint8_t(1)));std::vector<std::string> names;
 assert(world.npcNames(original.address,names) && names==normal);
 FieldDoorDataWorld active;assert(active.initialize(2,uint8_t(1)));
 const auto alternate=active.copies()[0].words[13];assert(active.npcNames(active.workAddress(11),names) && names.size()==2);
 assert(std::vector<std::string>(normal.begin(),normal.end()-1)==names);
 assert(world.setParameter(original.address,0,alternate));assert(world.npcNames(original.address,names) && names.size()==2);
 uint32_t unchanged=0;assert(world.readWord(original.source+0x34,unchanged) && unchanged==original.words[13]);
 assert(world.setParameter(0,7,0)); // Exact unused-selector no-access body.
 assert(!world.setParameter(0,0,alternate));
 world.synchronize(0);assert(!world.workAddress(11) && !world.readWord(original.address,unchanged));
 assert(world.initialize(2,uint8_t(0)));assert(world.workAddress(11)!=original.address);
 assert(!world.setParameter(original.address,0,alternate));
 assert(world.npcNames(world.workAddress(11),names) && names==normal);
 // Rejected descriptors retain the working immutable bank and live copies.
 char directory[]="/tmp/ttyd-door-data-XXXXXX";assert(mkdtemp(directory));
 const std::filesystem::path fixture(directory);
 std::filesystem::copy_file("romfs/door-data.pmevt",fixture/"door-data.pmevt");
 std::ifstream input("romfs/door-data.bin",std::ios::binary);std::vector<char> bytes((std::istreambuf_iterator<char>(input)),{});
 assert(bytes.size()==36);bytes[20]=32; // Out-of-domain GW index.
 {std::ofstream output(fixture/"door-data.bin",std::ios::binary);output.write(bytes.data(),bytes.size());}
 const auto liveAddress=world.workAddress(11);assert(!loadFieldDoorDataAssets(directory));
 assert(world.workAddress(11)==liveAddress && world.npcNames(liveAddress,names) && names==normal);
 std::filesystem::remove_all(fixture);
 // Held NPC collision visits still invalidate area-owned addresses first.
 assert(loadFieldActorCatalog("romfs") && loadHarborStoryAssets("romfs"));
 assert(loadGround("romfs/gor_02.ground") && loadCollision("romfs/gor_02.walls") && loadHead("romfs/gor_02.head"));
 Game held(false);held.progress.room=2;held.progress.originalStory=23;held.progress.position={2.5f,.04f};
 initializeTroubleQuestState(held.progress.originalNpcTalkState);held.tick({});
 const auto old=held.fieldDoorData.workAddress(11);assert(old);
 held.scene=Scene::Victory;held.progress.room=0;held.tick({});
 assert(!held.fieldDoorData.workAddress(11) && !held.fieldDoorData.readWord(old,unchanged));
 held.scene=Scene::Explore;held.progress.room=2;held.tick({});
 const auto replacement=held.fieldDoorData.workAddress(11);assert(replacement && replacement!=old);
 assert(loadFieldActorCatalog("romfs"));held.scene=Scene::Victory;held.tick({});
 assert(!held.fieldDoorData.workAddress(11));held.scene=Scene::Explore;held.tick({});
 assert(held.fieldDoorData.workAddress(11)!=replacement);
}
