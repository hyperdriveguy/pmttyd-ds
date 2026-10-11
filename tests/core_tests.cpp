#include "game.hpp"
#include "save.hpp"
#include <cmath>
#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <limits>
#include <stdexcept>
#include <string>
#include <unistd.h>
using namespace ttyd;
#define CHECK(x) do { if(!(x)) throw std::runtime_error(std::string(__FILE__)+":"+std::to_string(__LINE__)+" " #x); } while(0)
void advance(Game& g,int ticks) {while(ticks-->0)g.tick({});}
Input confirm() {Input i;i.confirm=true;return i;}
void attack(Game& g,bool success) {
    CHECK(g.phase==Phase::Select);g.selection=0;g.tick(confirm());
    advance(g,success?BattleRules::JumpImpact-1:BattleRules::JumpImpact);
    if(success)g.tick(confirm());
    CHECK(g.phase==Phase::Impact);
}
void movement() {
    Game g;Input right;right.x=1;
    advance(g,1);for(int n=0;n<1000;++n)g.tick(right);
    CHECK(walkable(g.progress.position));CHECK(g.progress.position.x<=8.72f);
    Game obstacle;obstacle.progress.position={0,1.2f};Input north;north.z=-1;
    for(int n=0;n<120;++n)obstacle.tick(north);
    CHECK(obstacle.progress.position.z>=.88f);
    CHECK(!walkable({std::numeric_limits<float>::quiet_NaN(),1}));
    CHECK(!walkable({0,0}));
    Game jump;jump.progress.position={0,2};jump.tick(confirm());advance(jump,18);CHECK(jump.jumpHeight>.9f);advance(jump,18);CHECK(std::fabs(jump.jumpHeight)<.01f);
    Game follow;follow.progress.position={-3,3};follow.partner={-6,3};advance(follow,100);CHECK(follow.partner.x>-4.1f);CHECK(walkable(follow.partner));
}
void dialogueAndTouch() {
    Game g;g.progress.position=CrumpPosition;g.tick(confirm());CHECK(g.scene==Scene::Dialogue);
    g.tick(confirm());g.tick(confirm());CHECK(g.scene==Scene::Battle);
    Game npc;npc.progress.position=NpcPosition;npc.tick(confirm());CHECK(npc.scene==Scene::Dialogue);
    Input close;close.touchX=20;close.touchY=180;npc.tick(close);CHECK(npc.scene==Scene::Explore);
    Game tabs;Input touch;touch.touchX=240;touch.touchY=55;tabs.tick(touch);CHECK(tabs.menu==Menu::Settings);
    Input y;y.menuConfirm=true;tabs.tick(y);CHECK(!tabs.progress.sound);CHECK(tabs.saveRequested);
}
void timing() {
    for(int frame:{1,42,43,47,48}) {
        Game g;g.startBattle();g.tick(confirm());advance(g,frame-1);g.tick(confirm());
        advance(g,BattleRules::JumpImpact-frame);
        CHECK(g.lastDamage==(frame>=43?2:1));
    }
    // An early press cannot be repaired by mashing later.
    Game mash;mash.startBattle();mash.tick(confirm());mash.tick(confirm());advance(mash,46);mash.tick(confirm());CHECK(mash.lastDamage==1);
    for(int release:{53,54,60,66}) {
        Game g;g.startBattle();g.selection=1;g.tick(confirm());
        Input held;held.hammerHeld=true;g.tick(held);advance(g,release-2);
        Input up;up.hammerReleased=true;g.tick(up);
        CHECK(g.phase==Phase::Impact);CHECK(g.lastDamage==(release>=54?2:1));
    }
    Game timeout;timeout.startBattle();timeout.selection=1;timeout.tick(confirm());advance(timeout,66);CHECK(timeout.phase==Phase::Impact);CHECK(timeout.lastDamage==1);
}
void battles() {
    Game g;g.startBattle();
    for(int turn=0;turn<3;++turn) {
        attack(g,true);advance(g,42);
        if(turn==2)break;
        CHECK(g.phase==Phase::EnemyWindup);advance(g,53);g.tick(confirm());CHECK(g.guarded);CHECK(g.progress.hp==10);advance(g,36);
    }
    CHECK(g.scene==Scene::Victory);g.tick(confirm());CHECK(g.scene==Scene::Explore);CHECK(g.progress.crumpDefeated);CHECK(g.saveRequested);
    Game loss;loss.startBattle();loss.progress.hp=1;attack(loss,false);advance(loss,42+54+36);CHECK(loss.scene==Scene::Defeat);
    loss.tick(confirm());CHECK(loss.scene==Scene::Battle);CHECK(loss.progress.hp==10);CHECK(loss.enemyHP==5);
    Game item;item.startBattle();item.selection=2;item.tick(confirm());CHECK(item.phase==Phase::Select);CHECK(item.progress.mushrooms==1);
    item.progress.hp=4;item.tick(confirm());CHECK(item.progress.hp==9);CHECK(item.progress.mushrooms==0);advance(item,42+54+36);CHECK(item.phase==Phase::Select);
}
void saves() {
    Progress p;p.position={2,2};p.crumpDefeated=true;p.hp=7;p.sound=false;
    auto bytes=encodeSave(p);Progress restored;CHECK(decodeSave(bytes,restored));CHECK(restored.hp==7);CHECK(restored.crumpDefeated);CHECK(!restored.sound);
    // Version 1 plaza saves migrate without losing progress.
    auto legacy=encodeSave(p);legacy[4]=1;
    uint32_t sum=2166136261u;for(int n=0;n<28;++n){sum^=legacy[n];sum*=16777619u;}
    for(int n=0;n<4;++n)legacy[28+n]=(sum>>(8*n))&255;
    CHECK(decodeSave(legacy,restored));CHECK(restored.room==0);CHECK(restored.elevation==0);
    Progress elevated=p;elevated.room=1;elevated.elevation=1.19f;
    CHECK(decodeSave(encodeSave(elevated),restored));CHECK(restored.room==1);CHECK(std::fabs(restored.elevation-1.2f)<.001f);
    elevated.room=256;CHECK(!decodeSave(encodeSave(elevated),restored));
    bytes[8]^=1;CHECK(!decodeSave(bytes,restored));
    p.position.x=std::numeric_limits<float>::infinity();CHECK(!decodeSave(encodeSave(p),restored));
    p.room=4;CHECK(!decodeSave(encodeSave(p),restored));p.room=0;
    p.position={2,2};p.hp=11;CHECK(!decodeSave(encodeSave(p),restored));p.hp=7;
    char directory[]="/tmp/ttyd-tests-XXXXXX";CHECK(mkdtemp(directory));std::string path=std::string(directory)+"/progress.bin";
    CHECK(loadSave(path,restored)==LoadResult::Missing);CHECK(writeSave(path,p));CHECK(loadSave(path,restored)==LoadResult::Loaded);
    p.hp=4;CHECK(writeSave(path,p));CHECK(loadSave(path,restored)==LoadResult::Loaded);CHECK(restored.hp==4);
    {std::ofstream corrupt(path);corrupt<<"broken";}
    CHECK(loadSave(path,restored)==LoadResult::Recovered);CHECK(restored.hp==7);
    CHECK(!writeSave(std::string(directory)+"/missing/save",p));
    std::remove(path.c_str());std::remove((path+".bak").c_str());rmdir(directory);
}
void collisionImport() {
    char path[]="/tmp/ttyd-walls-XXXXXX";int fd=mkstemp(path);CHECK(fd>=0);
    uint32_t count=1;Rect wall{0,2,0,3.5f};
    CHECK(write(fd,&count,4)==4);CHECK(write(fd,&wall,sizeof(wall))==sizeof(wall));close(fd);
    CHECK(loadCollision(path));CHECK(!walkable({.1f,2.5f}));CHECK(walkable({1,2.5f}));
    CHECK(!walkable({7,3}));clearCollision();std::remove(path);
    CHECK(!loadCollision("/does/not/exist"));
}
void groundImport() {
    char path[]="/tmp/ttyd-ground-XXXXXX";int fd=mkstemp(path);CHECK(fd>=0);
    uint32_t count=2;
    // Sloping floor, plus a roof that must not pull the actor upward.
    float triangles[18]={-4,0,1, 4,1,1, -4,0,4, -4,3,1, 4,3,1, -4,3,4};
    CHECK(write(fd,&count,4)==4);CHECK(write(fd,triangles,sizeof(triangles))==sizeof(triangles));close(fd);
    CHECK(loadGround(path));float h=0;
    CHECK(groundAt({-3,2},0,h));CHECK(std::fabs(h-.125f)<.001f);
    CHECK(!groundAt({std::numeric_limits<float>::quiet_NaN(),2},0,h));
    CHECK(!groundAt({8,2},0,h));CHECK(!groundAt({0,2},0,h));
    CHECK(groundAt({0,2},.3f,h));CHECK(std::fabs(h-.5f)<.001f);
    CHECK(groundAt({-3,2},3,h));CHECK(h==3);
    Game g;g.progress.position={-3,2};g.restore(g.progress);CHECK(std::fabs(g.groundHeight-.125f)<.001f);
    Input right;right.x=1;for(int n=0;n<12;++n)g.tick(right);
    CHECK(g.progress.position.x>-2.3f);CHECK(g.groundHeight>.2f);
    clearCollision();std::remove(path);
}
void indexedOriginalCollision() {
    size_t queries=0;
    for(uint32_t room=0;room<4;++room) {
        std::string prefix=std::string("romfs/")+roomAsset(room);
        CHECK(loadCollision((prefix+".walls").c_str()));CHECK(loadGround((prefix+".ground").c_str()));
        std::ifstream walls(prefix+".walls",std::ios::binary),ground(prefix+".ground",std::ios::binary);
        uint32_t nw=0,ng=0;walls.read(reinterpret_cast<char*>(&nw),4);ground.read(reinterpret_cast<char*>(&ng),4);
        CHECK(nw&0x80000000);nw&=0x7fffffff;
        std::vector<std::array<float,6>> segments(nw);std::vector<std::array<float,9>> triangles(ng);
        walls.read(reinterpret_cast<char*>(segments.data()),nw*24);ground.read(reinterpret_cast<char*>(triangles.data()),ng*36);
        CHECK(walls.good());CHECK(ground.good());
        for(int x=-18;x<=18;++x)for(int z=-16;z<=16;++z)for(float level:{0.f,1.2f,3.f}) {
            Vec2 p{x*.5f,z*.5f};bool free=true,found=false;float floor=-1e30f;
            for(auto w:segments) {
                if(w[5]<level+.1f || w[4]>level+1.2f)continue;
                float dx=w[2]-w[0],dz=w[3]-w[1],den=dx*dx+dz*dz;if(den<1e-8f)continue;
                float t=std::max(0.f,std::min(1.f,((p.x-w[0])*dx+(p.z-w[1])*dz)/den));
                float a=p.x-w[0]-t*dx,b=p.z-w[1]-t*dz;if(a*a+b*b<.28f*.28f)free=false;
            }
            CHECK(walkable(p,level)==free);
            for(auto v:triangles) {
                if(p.x<std::min({v[0],v[3],v[6]}) || p.x>std::max({v[0],v[3],v[6]}) || p.z<std::min({v[2],v[5],v[8]}) || p.z>std::max({v[2],v[5],v[8]}))continue;
                float den=(v[5]-v[8])*(v[0]-v[6])+(v[6]-v[3])*(v[2]-v[8]);if(std::fabs(den)<1e-8f)continue;
                float a=((v[5]-v[8])*(p.x-v[6])+(v[6]-v[3])*(p.z-v[8]))/den;
                float b=((v[8]-v[2])*(p.x-v[6])+(v[0]-v[6])*(p.z-v[8]))/den;
                float y=a*v[1]+b*v[4]+(1-a-b)*v[7];
                if(a>=-.0001f && b>=-.0001f && a+b<=1.0001f && y<=level+.25f && y>=level-.5f && y>floor){found=true;floor=y;}
            }
            float actual=0;CHECK(groundAt(p,level,actual)==found);if(found)CHECK(std::fabs(actual-floor)<.001f);
            ++queries;
        }
    }
    clearCollision();std::printf("Indexed original collision matched %zu full-scan probes\n",queries);
}
void originalRooms() {
    CHECK(loadWarps("romfs/rooms.bin"));
    std::ifstream file("romfs/rooms.bin",std::ios::binary);uint32_t count=0;file.read(reinterpret_cast<char*>(&count),4);
    CHECK(count==6);std::vector<RoadWarp> roads(count);file.read(reinterpret_cast<char*>(roads.data()),count*sizeof(RoadWarp));CHECK(file.good());
    for(size_t n=0;n<roads.size();++n) {
        const auto& road=roads[n];std::string prefix=std::string("romfs/")+roomAsset(road.target);
        CHECK(loadCollision((prefix+".walls").c_str()));CHECK(loadGround((prefix+".ground").c_str()));
        Game g;g.progress.room=road.source;g.progress.hp=7;g.warpRequest=n;
        CHECK(g.warpTarget()==road.target);CHECK(g.finishWarp());
        CHECK(g.progress.room==road.target);CHECK(g.progress.hp==7);CHECK(g.warpRequest==-1);
        CHECK(g.warpCooldown==45);CHECK(g.saveRequested);
        CHECK(g.cameraX==g.progress.position.x);CHECK(g.cameraZ==g.progress.position.z);
        CHECK(g.cameraHeight==g.groundHeight);CHECK(walkable(g.progress.position,g.groundHeight));
        CHECK(std::fabs(g.progress.position.x-road.spawnX)<.001f);CHECK(std::fabs(g.progress.position.z-road.spawnZ)<.001f);
        CHECK(decodeSave(encodeSave(g.progress),g.progress));
        for(int frame=0;frame<10;++frame)g.tick({});CHECK(g.warpRequest==-1);
    }
    clearCollision();
}
int main(int argc,char**) {
    try {if(argc>1) {indexedOriginalCollision();originalRooms();std::puts("Original room arrivals passed");return 0;}movement();dialogueAndTouch();timing();battles();saves();collisionImport();groundImport();std::puts("Core scenarios passed");}
    catch(const std::exception& e){std::fprintf(stderr,"%s\n",e.what());return 1;}
}
