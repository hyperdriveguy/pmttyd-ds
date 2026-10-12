#include "platform.hpp"
#include "save.hpp"
#include <algorithm>
#include <cstdio>
#include <sys/stat.h>
namespace {
double seconds() { return svcGetSystemTick()/double(SYSCLOCK_ARM11); }
bool stageRoomCollision(uint32_t room,ttyd::CollisionTransition& transaction) {
    const char* name=ttyd::roomAsset(room);if(!name)return false;
    char path[96];std::snprintf(path,sizeof(path),"romfs:/%s",name);
    return transaction.load(path);
}
bool loadRoomCollision(uint32_t room) {
    ttyd::CollisionTransition transaction;
    if(!stageRoomCollision(room,transaction))return false;
    transaction.commit();return true;
}
void merge(ttyd::Input& pending,const ttyd::Input& i) {
    pending.x=i.x;pending.z=i.z;pending.jumpHeld=i.jumpHeld;
    pending.confirm|=i.confirm;pending.cancel|=i.cancel;pending.left|=i.left;pending.right|=i.right;
    pending.menuConfirm|=i.menuConfirm;
    if(i.touchX>=0) {pending.touchX=i.touchX;pending.touchY=i.touchY;}
}
}
int main() {
    gfxInitDefault();gfxSet3D(false);
    bool newModel=false;
    if(R_SUCCEEDED(APT_CheckNew3DS(&newModel)) && newModel) osSetSpeedupEnable(true);
    const Result romfs=romfsInit();
    ttyd::Renderer renderer;
    if(R_FAILED(romfs) || !renderer.init()) {
        renderer.shutdown();consoleInit(GFX_TOP,nullptr);
        std::printf("TT YD prototype could not initialize.\nCheck packaged RomFS and available memory.\nPress START to exit.\n");
        while(aptMainLoop()) {hidScanInput();if(hidKeysDown()&KEY_START)break;gspWaitForVBlank();}
        if(R_SUCCEEDED(romfs)) romfsExit();
        gfxExit();
        return 1;
    }
    if(!loadRoomCollision(0) || !ttyd::loadWarps("romfs:/rooms.bin")) {
        renderer.shutdown();romfsExit();gfxExit();return 1;
    }
    ttyd::Audio audio;audio.init();
    ttyd::Game game;ttyd::Progress saved;
    const char* savePath="sdmc:/3ds/ttyd-2ds/progress.bin";
    mkdir("sdmc:/3ds",0777);mkdir("sdmc:/3ds/ttyd-2ds",0777);
    const auto loaded=ttyd::loadSave(savePath,saved);
    float savedHeight=saved.elevation;bool restored=false;
    if(loaded==ttyd::LoadResult::Loaded || loaded==ttyd::LoadResult::Recovered) {
        ttyd::CollisionTransition transaction;
        if(stageRoomCollision(saved.room,transaction) && ttyd::groundAt(saved.position,saved.elevation,savedHeight) &&
           ttyd::walkable(saved.position,savedHeight) && (saved.room==0 || renderer.changeRoom(saved.room))) {
            game.restore(saved);transaction.commit();restored=true;
        }
    }
    if(!restored)game.restore(game.progress);
    const bool rejected=(loaded==ttyd::LoadResult::Loaded || loaded==ttyd::LoadResult::Recovered) && !restored;
    const char* notice=rejected?"Saved room unavailable; started in plaza":loaded==ttyd::LoadResult::Recovered?"Recovered backup save":loaded==ttyd::LoadResult::Invalid?"Invalid save; started a new game":!newModel?"New 2DS XL is the performance target":"Development slice / provisional mechanics";
    int noticeTicks=240;
    ttyd::Metrics metrics;
    double previous=seconds(),accumulator=0,window=previous;int frames=0;
    ttyd::Input pending;
    while(aptMainLoop()) {
        double now=seconds();double delta=std::max(0.,now-previous);previous=now;
        // Resume/menu interruptions must not cause seconds of battle advancement.
        if(delta>.25) {accumulator=0;pending={};delta=1./60;}
        accumulator+=delta;
        hidScanInput();uint32_t down=hidKeysDown(),held=hidKeysHeld();
        if(down&KEY_START) break;
        ttyd::Input i;circlePosition circle;hidCircleRead(&circle);
        i.x=std::abs(circle.dx)>18?circle.dx/156.f:0;
        i.z=std::abs(circle.dy)>18?-circle.dy/156.f:0;
        i.confirm=down&KEY_A;i.cancel=down&KEY_X; // X requests partner dialogue in field.
        if(game.scene!=ttyd::Scene::Explore) i.cancel=down&KEY_B;
        i.left=down&KEY_DLEFT;i.right=down&KEY_DRIGHT;i.menuConfirm=down&KEY_Y;
        i.jumpHeld=held&KEY_A;
        if(down&KEY_TOUCH) {touchPosition touch;hidTouchRead(&touch);i.touchX=touch.px;i.touchY=touch.py;}
        merge(pending,i);
        const double simulationStart=seconds();metrics.steps=0;
        while(accumulator>=1./60 && metrics.steps<5) {
            game.tick(pending);++metrics.steps;accumulator-=1./60;
            pending={};pending.x=i.x;pending.z=i.z;pending.jumpHeld=i.jumpHeld;
            if(game.progress.sound) audio.play(game.sound);
            if(noticeTicks>0)--noticeTicks;
        }
        if(accumulator>=1./60) {metrics.droppedTicks+=uint64_t(accumulator*60);accumulator=0;}
        metrics.simulationMs=(seconds()-simulationStart)*1000;
        if(game.warpRequest>=0 && game.fade.ready()) {
            ttyd::CollisionTransition transaction;
            // Target ID comes from the original, validated road table.
            ttyd::Game candidate=game;
            // Read target from the fixed record requested by the simulation.
            uint32_t target=game.warpTarget();
            bool ok=stageRoomCollision(target,transaction) && candidate.finishWarp();
            if(ok)ok=renderer.changeRoom(target);
            if(ok) {transaction.commit();game=std::move(candidate);game.beginRoadArrival();game.fade.startIn();notice="";noticeTicks=0;}
            else {game.cancelRoad();notice="Room transition failed";noticeTicks=180;}
            accumulator=0;pending={};previous=seconds();
        }
        if(game.saveRequested) {
            notice=ttyd::writeSave(savePath,game.progress)?"Progress saved":"Save failed; progress kept in memory";
            noticeTicks=180;game.saveRequested=false;
        }
        if(game.reloadRequested) {
            game.reloadRequested=false;ttyd::Progress checkpoint;
            const auto result=ttyd::loadSave(savePath,checkpoint);bool ok=false;
            if((result==ttyd::LoadResult::Loaded || result==ttyd::LoadResult::Recovered) && checkpoint.hp>0) {
                ttyd::CollisionTransition transaction;float h=checkpoint.elevation;
                if(stageRoomCollision(checkpoint.room,transaction) && ttyd::groundAt(checkpoint.position,h,h) &&
                   ttyd::walkable(checkpoint.position,h) && renderer.changeRoom(checkpoint.room)) {
                    game.restore(checkpoint);transaction.commit();ok=true;
                }
            }
            notice=ok?"Progress loaded":"Save unavailable";noticeTicks=180;
            accumulator=0;pending={};previous=seconds();
        }
        audio.update(game.scene,game.progress.sound);
        metrics.renderMs=C3D_GetProcessingTime();metrics.gpuMs=C3D_GetDrawingTime();
        renderer.render(game,metrics,noticeTicks>0?notice:"",audio.available());
        game.fade.presented();
        ++frames;
        if(now-window>=1) {metrics.fps=frames/(now-window);window=now;frames=0;}
    }
    // A battle is transient; save only the exploration checkpoint.
    if(game.scene==ttyd::Scene::Explore && !game.airborne() && !game.water.active() && !game.fade.active() && !game.arriving && game.warpRequest<0) ttyd::writeSave(savePath,game.progress);
    audio.shutdown();renderer.shutdown();romfsExit();gfxExit();return 0;
}
