#include "game.hpp"
#include <cassert>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <unistd.h>
using namespace ttyd;
int main(int argc,char**) {
    char directory[]="/tmp/pm-harbor-XXXXXX";assert(mkdtemp(directory));const std::filesystem::path root=directory;
    if(argc>1)assert(loadHarborStoryAssets("romfs"));
    else {
        std::filesystem::create_directory(root/"messages");
        std::ofstream global(root/"messages/global.txt",std::ios::binary);global.put(0);global.close();
        std::ofstream room(root/"messages/gor_00.txt",std::ios::binary);
        for(int id=16;id<=21;++id){char key[20];std::snprintf(key,sizeof(key),"mac_0_%03d",id);room.write(key,std::strlen(key)+1);const char text[]="Synthetic test<k>";room.write(text,sizeof(text));}
        room.put(0);room.close();
        std::ofstream font(root/"font.pmfont",std::ios::binary);const char magic[8]="PMFONT1";const uint32_t count=256;
        font.write(magic,8);font.write(reinterpret_cast<const char*>(&count),4);
        for(uint16_t i=0;i<256;++i)font.write(reinterpret_cast<const char*>(&i),2);
        const FontGlyph glyph{0,0,10,22,{0,0}};for(int i=0;i<256;++i)font.write(reinterpret_cast<const char*>(&glyph),8);font.close();
        assert(loadHarborStoryAssets(directory));
    }
    clearCollision();Game game;Progress p;p.room=1;p.originalStory=3;p.position={0,-.999f};game.restore(p);
    game.tick({});assert(!game.harborStory.triggered);game.progress.position.z=-1;game.tick({});
    assert(game.harborStory.triggered && game.scene==Scene::Dialogue && game.harborStory.speaker==0x14b28);
    assert((game.harborStory.speakerRoot==std::array<float,3>{175,15,-300}));
    std::vector<std::string> ids;std::vector<uint32_t> speakers;
    for(int t=0;t<10000 && !game.harborStory.finished;++t) {
        const auto& message=game.harborStory.message;
        if(message.active() && (ids.empty() || ids.back()!=message.id())){
            ids.push_back(message.id());speakers.push_back(game.harborStory.speaker);
            for(const auto& a:game.harborStory.actors)if(a.key==game.harborStory.speaker)
                assert((game.harborStory.speakerRoot==std::array<float,3>{a.x,a.y,a.z}));
        }
        Input input;input.confirm=true;game.tick(input);
        if(game.harborStory.locked())assert(!game.saveRequested && game.progress.originalStory==3);
    }
    assert(game.harborStory.finished && game.progress.originalStory==4 && game.scene==Scene::Explore && game.saveRequested);
    assert(ids==std::vector<std::string>({"mac_0_016","mac_0_017","mac_0_018","mac_0_019","mac_0_020","mac_0_021"}));
    assert(speakers==std::vector<uint32_t>({0x14b28,0x14b40,0x14b28,0x14b40,0x14b5c,0x14b28}));
    assert(game.harborStory.actors[0].x==55 && game.harborStory.actors[1].x==105);
    const auto before=game.harborStory.stage;game.tick({});assert(game.harborStory.stage==before);
    p.originalStory.reset();game.restore(p);game.tick({});assert(!game.harborStory.triggered);
    {std::ofstream ground(root/"ground",std::ios::binary);const uint32_t count=2;
        const float plane[]={-10,0,-10,10,0,-10,-10,0,10,-10,0,10,10,0,-10,10,0,10};
        ground.write(reinterpret_cast<const char*>(&count),4);ground.write(reinterpret_cast<const char*>(plane),sizeof(plane));}
    assert(loadGround((root/"ground").c_str()));
    p.originalStory=3;p.position={-2,-1};Game air;air.restore(p);Input launch;launch.confirm=true;launch.jumpHeld=true;
    air.tick(launch);assert(air.harborStory.locked() && air.fieldAirMotion().airborne);
    for(int n=0;n<90;++n)air.tick({});assert(!air.fieldAirMotion().airborne && air.harborStory.locked());
    std::filesystem::remove_all(root);
    std::cout<<"original harbor trigger, six ordered speakers/messages, concurrent movement and story4 boundary passed\n";
}
