#include "game.hpp"
#include "save.hpp"
#include "visibility.hpp"
#include "uv_animation.hpp"
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
    elevated.room=4;CHECK(decodeSave(encodeSave(elevated),restored));CHECK(restored.room==4);
    elevated.room=256;CHECK(!decodeSave(encodeSave(elevated),restored));
    bytes[8]^=1;CHECK(!decodeSave(bytes,restored));
    p.position.x=std::numeric_limits<float>::infinity();CHECK(!decodeSave(encodeSave(p),restored));
    p.room=5;CHECK(!decodeSave(encodeSave(p),restored));p.room=0;
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
    CHECK(!walkable({7,3}));
    auto levelWall=[&](float low,float high) {
        std::ofstream file(path,std::ios::binary);uint32_t tagged=0x80000001;
        Vec2 levels{low,high};file.write(reinterpret_cast<char*>(&tagged),4);
        file.write(reinterpret_cast<char*>(&wall),sizeof(wall));file.write(reinterpret_cast<char*>(&levels),sizeof(levels));
        file.close();CHECK(loadCollision(path));
    };
    levelWall(0,.1f);CHECK(walkable({.1f,2.5f},0)); // Original harbor riser height.
    levelWall(0,.3f);CHECK(!walkable({.1f,2.5f},0)); // Taller obstacle remains solid.
    CHECK(walkable({.1f,2.5f},.3f));
    levelWall(1.5f,2);CHECK(walkable({.1f,2.5f},0)); // Overhead geometry.
    clearCollision();std::remove(path);
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
    GroundContact legacy{123,456};CHECK(groundAt({-3,2},0,h,&legacy));CHECK(legacy.attributes==0 && legacy.sourceNode==0);
    CHECK(!groundAt({std::numeric_limits<float>::quiet_NaN(),2},0,h));
    CHECK(!groundAt({8,2},0,h));CHECK(!groundAt({0,2},0,h));
    CHECK(groundAt({0,2},.3f,h));CHECK(std::fabs(h-.5f)<.001f);
    CHECK(groundAt({-3,2},3,h));CHECK(h==3);
    Game g;g.progress.position={-3,2};g.restore(g.progress);CHECK(std::fabs(g.groundHeight-.125f)<.001f);
    Input right;right.x=1;for(int n=0;n<12;++n)g.tick(right);
    CHECK(g.progress.position.x>-2.3f);CHECK(g.groundHeight>.2f);
    {
        std::ofstream file(path,std::ios::binary);uint32_t tagged=0x80000002;
        file.write(reinterpret_cast<char*>(&tagged),4);
        for(int n=0;n<2;++n) {
            GroundContact contact{uint32_t(0x200+n),uint32_t(100+n)};
            file.write(reinterpret_cast<char*>(triangles+n*9),36);
            file.write(reinterpret_cast<char*>(&contact),8);
        }
    }
    CHECK(loadGround(path));GroundContact contact;
    CHECK(groundAt({-3,2},0,h,&contact));CHECK(contact.attributes==0x200 && contact.sourceNode==100);
    CHECK(groundAt({-3,2},3,h,&contact));CHECK(contact.attributes==0x201 && contact.sourceNode==101);
    CHECK(!groundAt({8,2},0,h,&contact));CHECK(contact.attributes==0x201 && h==3);
    {std::ofstream file(path,std::ios::binary);uint32_t tagged=0x80000002;file.write(reinterpret_cast<char*>(&tagged),4);}
    CHECK(!loadGround(path));CHECK(groundAt({-3,2},0,h,&contact));CHECK(contact.attributes==0x200 && contact.sourceNode==100);
    clearCollision();std::remove(path);
}
void visibility() {
    Point3 eye{0,1,4},actor{0,1,0};
    Point3 a{-1,0,2},b{1,0,2},c{0,2,2};
    CHECK(segmentHitsTriangle(eye,actor,a,b,c));CHECK(segmentHitsTriangle(eye,actor,c,b,a));
    CHECK(segmentHitsTriangle(actor,eye,a,b,c));
    CHECK(!segmentHitsTriangle({3,1,4},{3,1,0},a,b,c));
    CHECK(!segmentHitsTriangle(eye,{0,1,3},a,b,c));
    CHECK(!segmentHitsTriangle({0,1,2},actor,a,b,c)); // At-camera contact.
    CHECK(!segmentHitsTriangle(eye,{0,1,2},a,b,c)); // At-actor contact.
    CHECK(!segmentHitsTriangle({0,1,2},{1,1,2},a,b,c)); // Parallel.
    CHECK(!segmentHitsTriangle(eye,actor,a,a,a));
    Bounds3 bounds{{-1,0,2},{1,2,2}};
    CHECK(segmentHitsBounds(eye,actor,bounds));CHECK(segmentHitsBounds(actor,eye,bounds));
    CHECK(!segmentHitsBounds(eye,{0,1,3},bounds));
    CHECK(!segmentHitsBounds({3,1,4},{3,1,0},bounds));
    CHECK(segmentHitsBounds({0,1,2},{0,1,2},bounds));
    CHECK(!segmentHitsBounds(eye,actor,{{1,2,3},{0,1,2}}));
    auto nan=std::numeric_limits<float>::quiet_NaN();
    CHECK(!segmentHitsBounds({nan,1,4},actor,bounds));
    CHECK(!segmentHitsTriangle({nan,1,4},actor,a,b,c));
}
void collisionRollback() {
    char directory[]="/tmp/ttyd-transition-XXXXXX";CHECK(mkdtemp(directory));
    std::string source=std::string(directory)+"/source",target=std::string(directory)+"/target";
    auto writeWalls=[](const std::string& prefix,float x) {
        std::ofstream file(prefix+".walls",std::ios::binary);uint32_t count=1;Rect wall{x,1,x,3};
        file.write(reinterpret_cast<char*>(&count),4);file.write(reinterpret_cast<char*>(&wall),sizeof(wall));
    };
    auto writeFloor=[](const std::string& prefix,uint32_t attributes) {
        std::ofstream file(prefix+".ground",std::ios::binary);uint32_t count=0x80000002;
        float triangles[]={-3,0,1, 3,0,1, -3,0,3, 3,0,1, 3,0,3, -3,0,3};
        file.write(reinterpret_cast<char*>(&count),4);
        for(int n=0;n<2;++n) {
            GroundContact contact{attributes,uint32_t(100+n)};
            file.write(reinterpret_cast<char*>(triangles+n*9),36);file.write(reinterpret_cast<char*>(&contact),8);
        }
    };
    writeWalls(source,0);writeFloor(source,0x200);writeWalls(target,2);
    {CollisionTransition tx;CHECK(tx.load(source.c_str()));tx.commit();}
    const Rect originalBounds=collisionBounds();float h=99;
    CHECK(!walkable({0,2}));CHECK(walkable({2,2}));CHECK(groundAt({1,2},0,h));CHECK(h==0);
    GroundContact contact;CHECK(groundAt({1,2},0,h,&contact));CHECK(contact.attributes==0x200);
    {std::ofstream bad(target+".ground");bad<<"truncated";}
    {
        CollisionTransition tx;CHECK(!tx.load(target.c_str()));tx.commit();
        // Rollback must work even if the previous room cannot be read again.
        std::remove((source+".walls").c_str());std::remove((source+".ground").c_str());
    }
    CHECK(!walkable({0,2}));CHECK(walkable({2,2}));CHECK(groundAt({1,2},0,h));CHECK(h==0);
    CHECK(groundAt({1,2},0,h,&contact));CHECK(contact.attributes==0x200);
    CHECK(collisionBounds().x0==originalBounds.x0);CHECK(collisionBounds().z1==originalBounds.z1);
    writeFloor(target,0x400);
    {
        CollisionTransition tx;CHECK(tx.load(target.c_str()));CHECK(walkable({0,2}));CHECK(!walkable({2,2}));
        // Model a rejected arrival or graphics load after collision staging succeeds.
        CHECK(!groundAt({20,2},0,h));
        {CollisionTransition nested;CHECK(!nested.load((source+"-missing").c_str()));}
        CHECK(!walkable({2,2}));
    }
    CHECK(!walkable({0,2}));CHECK(walkable({2,2}));
    {CollisionTransition tx;CHECK(tx.load(target.c_str()));tx.commit();}
    CHECK(walkable({0,2}));CHECK(!walkable({2,2}));CHECK(groundAt({1,2},0,h));
    CHECK(groundAt({1,2},0,h,&contact));CHECK(contact.attributes==0x400);
    clearCollision();std::remove((target+".walls").c_str());std::remove((target+".ground").c_str());rmdir(directory);
}
void uvAnimation() {
    UVTrack track;track.frames={{{1,0,0,0,1,0}},{{1,0,.5f,0,1,.25f}},{{2,0,0,0,3,0}}};
    UVBinding binding{0,.2f,.3f};
    auto a=animatedUV(track,binding,0),b=animatedUV(track,binding,1),c=animatedUV(track,binding,3);
    CHECK(std::fabs(a[0]-.2f)<.0001f && std::fabs(a[1]-.7f)<.0001f);
    CHECK(std::fabs(b[0]-.7f)<.0001f && std::fabs(b[1]-.45f)<.0001f);CHECK(a==c);
    char filename[]="/tmp/ttyd-uv-XXXXXX";int fd=mkstemp(filename);CHECK(fd>=0);close(fd);
    auto write=[&](uint32_t version,uint32_t frames,uint32_t vertex,float value,bool extra) {
        std::ofstream f(filename,std::ios::binary);f.write("UVAN",4);uint32_t count=1,bindings=1;
        f.write((char*)&version,4);f.write((char*)&count,4);f.write((char*)&frames,4);f.write((char*)&bindings,4);
        std::array<float,6> matrix{{1,0,value,0,1,0}};f.write((char*)matrix.data(),24);
        UVBinding item{vertex,.2f,.3f};f.write((char*)&item,sizeof(item));if(extra)f.put('x');
    };
    std::vector<UVTrack> loaded;
    write(1,1,0,0,false);CHECK(loadUVAnimation(filename,1,loaded));CHECK(loaded.size()==1);
    for(int mode=0;mode<5;++mode) {
        write(mode==0?2:1,mode==1?0:1,mode==2?1:0,mode==3?std::numeric_limits<float>::quiet_NaN():0,mode==4);
        CHECK(!loadUVAnimation(filename,1,loaded));CHECK(loaded.size()==1);CHECK(loaded[0].frames.size()==1);
    }
    std::remove(filename);
}
void meshAnimation() {
    MeshBinding b{0,1,2,3};MeshTrack track;
    track.frames={{{1,0,0,0,0,1,0,0,0,0,1,0}},{{0,-1,0,2,1,0,0,3,0,0,2,1}}};
    CHECK((animatedPosition(track,b,0)==std::array<float,3>{1,2,3}));
    CHECK((animatedPosition(track,b,1)==std::array<float,3>{0,4,7}));
    CHECK(animatedPosition(track,b,0)==animatedPosition(track,b,2));
}
void indexedOriginalCollision() {
    size_t queries=0;
    for(uint32_t room=0;room<5;++room) {
        std::string prefix=std::string("romfs/")+roomAsset(room);
        CHECK(loadCollision((prefix+".walls").c_str()));CHECK(loadGround((prefix+".ground").c_str()));
        std::ifstream vertexFile(prefix+".vertices",std::ios::binary|std::ios::ate);
        CHECK(vertexFile.good());const auto vertexBytes=vertexFile.tellg();
        CHECK(vertexBytes>0 && vertexBytes%36==0);const uint32_t vertexCount=vertexBytes/36;
        std::vector<UVTrack> uv;CHECK(loadUVAnimation((prefix+".uvanim").c_str(),vertexCount,uv));
        CHECK(uv.size()==(room==2?3u:room==1?4u:0u));
        std::vector<MeshTrack> mesh;CHECK(loadMeshAnimation((prefix+".meshanim").c_str(),vertexCount,mesh));
        CHECK(mesh.size()==(room==0?2u:room==1?3u:0u));size_t animated=0;
        for(const auto& track:mesh)for(auto binding:track.bindings) {
            auto first=animatedPosition(track,binding,0),loop=animatedPosition(track,binding,track.frames.size());
            CHECK(first==loop);CHECK(std::isfinite(first[0]) && std::isfinite(first[1]) && std::isfinite(first[2]));++animated;
        }
        CHECK(animated==(room==0?12u:room==1?4581u:0u));
        for(const auto& track:uv)for(auto binding:track.bindings) {
            auto first=animatedUV(track,binding,0),loop=animatedUV(track,binding,track.frames.size());
            CHECK(first==loop);CHECK(std::isfinite(first[0]) && std::isfinite(first[1]));
        }
        std::ifstream walls(prefix+".walls",std::ios::binary),ground(prefix+".ground",std::ios::binary);
        uint32_t nw=0,ng=0;walls.read(reinterpret_cast<char*>(&nw),4);ground.read(reinterpret_cast<char*>(&ng),4);
        CHECK(ng&0x80000000);ng&=0x7fffffff;
        CHECK(nw&0x80000000);nw&=0x7fffffff;
        std::vector<std::array<float,6>> segments(nw);std::vector<std::array<float,9>> triangles(ng);
        std::vector<GroundContact> contacts(ng);
        walls.read(reinterpret_cast<char*>(segments.data()),nw*24);
        for(uint32_t n=0;n<ng;++n) {
            ground.read(reinterpret_cast<char*>(triangles[n].data()),36);
            ground.read(reinterpret_cast<char*>(&contacts[n]),8);
        }
        CHECK(walls.good());CHECK(ground.good());
        for(int x=-18;x<=18;++x)for(int z=-16;z<=16;++z)for(float level:{0.f,1.2f,3.f}) {
            Vec2 p{x*.5f,z*.5f};bool free=true,found=false;float floor=-1e30f;
            for(auto w:segments) {
                if(w[5]<level+.25f || w[4]>level+1.2f)continue;
                float dx=w[2]-w[0],dz=w[3]-w[1],den=dx*dx+dz*dz;if(den<1e-8f)continue;
                float t=std::max(0.f,std::min(1.f,((p.x-w[0])*dx+(p.z-w[1])*dz)/den));
                float a=p.x-w[0]-t*dx,b=p.z-w[1]-t*dz;if(a*a+b*b<.28f*.28f)free=false;
            }
            CHECK(walkable(p,level)==free);
            GroundContact expected;
            for(size_t n=0;n<triangles.size();++n) {
                auto v=triangles[n];
                if(p.x<std::min({v[0],v[3],v[6]}) || p.x>std::max({v[0],v[3],v[6]}) || p.z<std::min({v[2],v[5],v[8]}) || p.z>std::max({v[2],v[5],v[8]}))continue;
                float den=(v[5]-v[8])*(v[0]-v[6])+(v[6]-v[3])*(v[2]-v[8]);if(std::fabs(den)<1e-8f)continue;
                float a=((v[5]-v[8])*(p.x-v[6])+(v[6]-v[3])*(p.z-v[8]))/den;
                float b=((v[8]-v[2])*(p.x-v[6])+(v[0]-v[6])*(p.z-v[8]))/den;
                float y=a*v[1]+b*v[4]+(1-a-b)*v[7];
                if(a>=-.0001f && b>=-.0001f && a+b<=1.0001f && y<=level+.25f && y>=level-.5f && y>floor){found=true;floor=y;expected=contacts[n];}
            }
            float actual=0;GroundContact contact;CHECK(groundAt(p,level,actual,&contact)==found);
            if(found) {CHECK(std::fabs(actual-floor)<.001f);CHECK(contact.attributes==expected.attributes);CHECK(contact.sourceNode==expected.sourceNode);}
            ++queries;
        }
    }
    clearCollision();std::printf("Indexed original collision matched %zu full-scan probes\n",queries);
}
void originalRooms() {
    CHECK(loadWarps("romfs/rooms.bin"));
    std::ifstream file("romfs/rooms.bin",std::ios::binary);uint32_t count=0;file.read(reinterpret_cast<char*>(&count),4);
    CHECK(count==8);std::vector<RoadWarp> roads(count);file.read(reinterpret_cast<char*>(roads.data()),count*sizeof(RoadWarp));CHECK(file.good());
    for(size_t n=0;n<roads.size();++n) {
        const auto& road=roads[n];std::string prefix=std::string("romfs/")+roomAsset(road.target);
        CHECK(loadCollision((prefix+".walls").c_str()));CHECK(loadGround((prefix+".ground").c_str()));
        Game g;g.progress.room=road.source;g.progress.hp=7;g.warpRequest=n;
        CHECK(g.warpTarget()==road.target);CHECK(g.finishWarp());
        CHECK(g.progress.room==road.target);CHECK(g.progress.hp==7);CHECK(g.warpRequest==-1);
        CHECK(g.warpCooldown==45);CHECK(g.saveRequested);
        CHECK(g.cameraX==g.progress.position.x);CHECK(g.cameraZ==g.progress.position.z);
        CHECK(g.cameraHeight==g.groundHeight);CHECK(walkable(g.progress.position,g.groundHeight));
        CHECK(g.groundContact.attributes==g.partnerContact.attributes);
        CHECK(g.groundContact.sourceNode==g.partnerContact.sourceNode);
        CHECK(std::fabs(g.progress.position.x-road.spawnX)<.001f);CHECK(std::fabs(g.progress.position.z-road.spawnZ)<.001f);
        CHECK(decodeSave(encodeSave(g.progress),g.progress));
        for(int frame=0;frame<10;++frame)g.tick({});CHECK(g.warpRequest==-1);
        if(road.source==0 && road.target==1) {
            // Harbor's entrance overlaps short posts and lower floor surfaces.
            // A downward floor change must not place Mario inside their walls.
            Input down;down.z=1;
            for(int frame=0;frame<60;++frame) {
                g.tick(down);CHECK(walkable(g.progress.position,g.groundHeight));
            }
            Input up;up.z=-1;
            for(int frame=0;frame<120 && g.warpRequest<0;++frame)g.tick(up);
            CHECK(g.warpTarget()==0);
            Game stairs;stairs.progress.room=0;stairs.warpRequest=n;CHECK(stairs.finishWarp());
            auto walk=[&](float x,float z,int frames) {
                Input i;i.x=x;i.z=z;
                for(int frame=0;frame<frames;++frame) {
                    stairs.tick(i);CHECK(walkable(stairs.progress.position,stairs.groundHeight));
                    CHECK(walkable(stairs.partner,stairs.partnerHeight));
                }
            };
            walk(1,0,30);walk(1,1,50);walk(-1,0,60);walk(0,1,10);
            CHECK(std::fabs(stairs.progress.position.x)<1 && stairs.progress.position.z>-2.5f);
            CHECK(std::fabs(stairs.groundHeight-.15f)<.001f);
            CHECK(std::fabs(stairs.partnerHeight-.15f)<.001f);
            walk(0,-1,10);walk(1,0,60);walk(-1,-1,50);walk(-1,0,30);walk(0,-1,60);
            CHECK(stairs.warpTarget()==0);
        }
    }
    clearCollision();
}
int main(int argc,char**) {
    try {if(argc>1) {indexedOriginalCollision();originalRooms();std::puts("Original room arrivals passed");return 0;}movement();dialogueAndTouch();timing();battles();saves();collisionImport();groundImport();collisionRollback();visibility();uvAnimation();meshAnimation();std::puts("Core scenarios passed");}
    catch(const std::exception& e){std::fprintf(stderr,"%s\n",e.what());return 1;}
}
