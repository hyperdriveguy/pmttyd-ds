#include "game.hpp"
#include "save.hpp"
#include <cmath>
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
    bytes[8]^=1;CHECK(!decodeSave(bytes,restored));
    p.position.x=std::numeric_limits<float>::infinity();CHECK(!decodeSave(encodeSave(p),restored));
    p.position={0,0};CHECK(!decodeSave(encodeSave(p),restored));
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
    CHECK(!groundAt({8,2},0,h));CHECK(!groundAt({0,2},0,h));
    CHECK(groundAt({0,2},.3f,h));CHECK(std::fabs(h-.5f)<.001f);
    CHECK(groundAt({-3,2},3,h));CHECK(h==3);
    Game g;g.progress.position={-3,2};g.restore(g.progress);CHECK(std::fabs(g.groundHeight-.125f)<.001f);
    Input right;right.x=1;for(int n=0;n<12;++n)g.tick(right);
    CHECK(g.progress.position.x>-2.3f);CHECK(g.groundHeight>.2f);
    clearCollision();std::remove(path);
}
int main() {
    try {movement();dialogueAndTouch();timing();battles();saves();collisionImport();groundImport();std::puts("Core scenarios passed");}
    catch(const std::exception& e){std::fprintf(stderr,"%s\n",e.what());return 1;}
}
