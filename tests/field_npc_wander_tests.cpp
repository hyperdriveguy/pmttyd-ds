#include "game.hpp"
#include "field_actors.hpp"
#include "camera_road_assets.hpp"
#include <cassert>
#include <cmath>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <cstring>
#include <unistd.h>
using namespace ttyd;
int main() {
    assert(loadFieldActorCatalog("romfs") && loadCameraRoadAssets("romfs"));
    assert(loadCollision("romfs/gor_02.walls") && loadGround("romfs/gor_02.ground") && loadHead("romfs/gor_02.head"));
    const FieldActorRecord* actor=nullptr;
    for(const auto& a:fieldActorCatalog().actors())if(a.wander.enabled){actor=&a;break;}
    assert(actor && actor->sourceTalkPresent && actor->talkRules.empty());
    assert(actor->position[0]==-370 && actor->position[2]==130 && actor->wander.radius==70 && actor->wander.runThreshold==20);
    Progress saved;saved.room=2;saved.originalStory=8;saved.position={1.2f,-4};Game game;game.restore(saved);
    game.tick({});const auto* initial=game.fieldNpcState(actor->key);assert(initial && initial->initialized);
    assert(initial->body.root.x==-370 && initial->body.root.z==130 && initial->movement.dt==0 && initial->wander.phase==1);
    assert(initial->body.flags==0x40480603 && initial->pose=="S_1");
    game.tick({});assert(game.fieldNpcState(actor->key)->body.root.x==-370);
    game.tick({});assert(game.fieldNpcState(actor->key)->body.root.x!=-370 && game.fieldNpcState(actor->key)->pose=="R_1");
    unsigned moving=0,idle=0,sounds=0;float maximum=0;
    for(int n=0;n<1600;++n) {
        game.tick({});const auto* npc=game.fieldNpcState(actor->key);assert(npc && !npc->paused && npc->body.root.y==0);
        maximum=std::max(maximum,std::hypot(npc->body.root.x+370,npc->body.root.z-130));
        moving+=npc->pose=="R_1";idle+=npc->pose=="S_1";
        for(const auto& sound:game.fieldNpcSounds()){assert(sound.key==actor->wander.soundLeft || sound.key==actor->wander.soundRight);++sounds;}
        assert(npc->movement.components.x==0 && npc->movement.components.z==0);
    }
    assert(moving>100 && idle>100 && sounds>3 && maximum<=70.61f);
    Game pause;auto near=saved;near.position={-3.4f,1.3f};pause.restore(near);
    for(int n=0;n<360;++n)pause.tick({});
    const auto* stopped=pause.fieldNpcState(actor->key);
    assert(stopped && stopped->paused && (stopped->body.state&0x20000000));
    const auto count=stopped->wander.count,phase=stopped->wander.phase;const auto root=stopped->body.root;
    const auto heading=stopped->movement.moveHeading;const auto savedYaw=stopped->savedYaw;
    for(int n=0;n<30;++n)pause.tick({});
    stopped=pause.fieldNpcState(actor->key);assert(stopped->wander.count==count && stopped->wander.phase==phase);
    assert(stopped->body.root.x==root.x && stopped->body.root.z==root.z && stopped->pose=="S_1");
    assert(stopped->movement.dt>0 && stopped->movement.moveHeading==heading);
    const float facing=stopped->movement.yaw;pause.progress.position.z+=.05f;pause.tick({});
    assert(pause.fieldNpcState(actor->key)->paused && pause.fieldNpcState(actor->key)->movement.yaw==facing); // Source turns only on initial admission.
    Input press;press.confirm=true;pause.tick(press);assert(!pause.airborne() && !pause.fieldActorTalk.message.active()); // No invented quest fallback.
    pause.fieldKeysOff=true;pause.progress.position={1.2f,-4};pause.tick({});
    assert(pause.fieldNpcState(actor->key)->paused && pause.fieldNpcState(actor->key)->wander.count==count);
    pause.fieldKeysOff=false;pause.tick({});
    stopped=pause.fieldNpcState(actor->key);assert(!stopped->paused && stopped->movement.yaw==savedYaw);
    assert(stopped->wander.count==count);pause.tick({});assert(pause.fieldNpcState(actor->key)->wander.count==count-1);
    auto changed=near;changed.room=0;changed.position={0,0};pause.restore(changed);pause.tick({});assert(!pause.fieldNpcState(actor->key));
    // Mechanism fixture: unsupported source winner must not redirect A to
    // farther Chu, whose original message provider remains available.
    assert(loadHarborStoryAssets("romfs"));Game nearest;nearest.restore(saved);for(int n=0;n<360;++n)nearest.tick({});
    std::ifstream pack("romfs/field-actors.pmnpc",std::ios::binary);std::vector<uint8_t> bytes((std::istreambuf_iterator<char>(pack)),{});
    auto word=[&](size_t p){uint32_t value;std::memcpy(&value,bytes.data()+p,4);return value;};
    size_t row=16+word(8)*100;
    for(unsigned n=0;n<word(12);++n) {
        const size_t tail=row+204+word(row+200)*40;
        if(word(row)==actor->key) {
            const float position[]={130,0,-385};std::memcpy(bytes.data()+row+40,position,12);
            std::memset(bytes.data()+tail+4,0,184); // Static harness body, original event present but no playback rules.
        }
        row=tail+188;
    }
    char temporary[]="/tmp/ttyd-npc-nearest-XXXXXX";assert(mkdtemp(temporary));const std::filesystem::path fixture(temporary);
    std::filesystem::create_directory(fixture/"messages");
    for(const char* name:{"global.txt","gor_02.txt"})std::filesystem::copy_file(std::filesystem::path("romfs/messages")/name,fixture/"messages"/name);
    std::filesystem::copy_file("romfs/npc-angle.bin",fixture/"npc-angle.bin");
    {std::ofstream out(fixture/"field-actors.pmnpc",std::ios::binary);out.write(reinterpret_cast<const char*>(bytes.data()),bytes.size());}
    assert(loadFieldActorCatalog(temporary));nearest.tick({});
    assert(nearest.nearbyFieldActor() && fieldActorCatalog().models()[nearest.nearbyFieldActor()->model].asset=="field-chu");
    nearest.tick(press);nearest.tick({});assert(!nearest.fieldActorTalk.message.active() && !nearest.airborne());
    assert(nearest.progress.originalStory==8 && std::strstr(nearest.prompt(),"event unavailable"));
    assert(loadFieldActorCatalog("romfs"));std::filesystem::remove_all(fixture);
    std::cout<<"Original NPC event order, live wander, radius, pose, sound and retained pause/resume passed\n";
}
