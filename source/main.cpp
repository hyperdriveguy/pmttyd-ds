#include "camera_road_assets.hpp"
#include "platform.hpp"
#include "field_pipe.hpp"
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
    pending.x=i.x;pending.z=i.z;pending.jumpHeld=i.jumpHeld;pending.cancelHeld=i.cancelHeld;
    pending.confirm|=i.confirm;pending.cancel|=i.cancel;pending.left|=i.left;pending.right|=i.right;
    pending.menuConfirm|=i.menuConfirm;
    pending.confirmReleased|=i.confirmReleased;pending.cancelReleased|=i.cancelReleased;
    pending.physicalSnapshot=i.physicalSnapshot;pending.physicalButtons=i.physicalButtons;
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
    if(!loadRoomCollision(0) || !(ttyd::loadWarps("romfs:/rooms.bin") && ttyd::loadFieldPipes("romfs:/field-pipes.bin")) || !ttyd::loadFieldDoors("romfs:/doors.pmdoor") || !ttyd::loadCameraRoadAssets("romfs:") || !ttyd::loadHarborStoryAssets("romfs:") || !ttyd::loadFieldActorCatalog("romfs:") || !ttyd::loadProfessorStoryAssets("romfs:")) {
        renderer.shutdown();romfsExit();gfxExit();return 1;
    }
    ttyd::Audio audio;audio.init();
    auto ownedGame=std::make_unique<ttyd::Game>();auto& game=*ownedGame;ttyd::Progress saved;
    const std::string saveDirectory="sdmc:/3ds/ttyd-2ds";
    std::string savePath=ttyd::saveSlotPath(saveDirectory,0);
    mkdir("sdmc:/3ds",0777);mkdir(saveDirectory.c_str(),0777);
    ttyd::Frontend frontend;ttyd::OpeningDemo demo;ttyd::OpeningSequence opening;
    bool frontReady=demo.load("romfs:") && opening.load("romfs:");
    demo.begin(0);ttyd::scanSaveSlots(frontend,saveDirectory);
    auto restoreProgress=[&](const ttyd::Progress& checkpoint) {
        ttyd::CollisionTransition transaction;float height=checkpoint.elevation;
        if(!stageRoomCollision(checkpoint.room,transaction))return false;
        auto ownedCandidate=std::make_unique<ttyd::Game>();auto& candidate=*ownedCandidate;candidate.restore(checkpoint);
        if(!candidate.checkpointGround(height) || !ttyd::walkable(checkpoint.position,height) || !renderer.changeRoom(checkpoint.room))return false;
        game=std::move(candidate);transaction.commit();return true;
    };
    game.restore(game.progress);
#ifndef TTYD_DEBUG_SHORT_BOOT
    renderer.beginFrontend();
#endif
#ifdef TTYD_DEBUG_SHORT_BOOT
    const auto loaded=ttyd::loadSave(savePath,saved);
    if(loaded==ttyd::LoadResult::Loaded || loaded==ttyd::LoadResult::Recovered)restoreProgress(saved);
    frontend.screen=ttyd::FrontScreen::Playing;
#endif
    const char* notice=frontReady?"":"Original opening resources unavailable";
    int noticeTicks=frontReady?0:240;
    uint64_t frontTicks=0;
    ttyd::Metrics metrics;
    double previous=seconds(),accumulator=0,window=previous;int frames=0;
    ttyd::Input pending;bool pendingStart=false;
    while(aptMainLoop()) {
        double now=seconds();double delta=std::max(0.,now-previous);previous=now;
        // Resume/menu interruptions must not cause seconds of battle advancement.
        if(delta>.25) {accumulator=0;pending={};delta=1./60;}
        accumulator+=delta;
        hidScanInput();uint32_t down=hidKeysDown(),held=hidKeysHeld(),up=hidKeysUp();
        if((down&KEY_START) && frontend.screen==ttyd::FrontScreen::Playing) break;
        ttyd::Input i;circlePosition circle;hidCircleRead(&circle);
        i.x=std::abs(circle.dx)>18?circle.dx/156.f:0;
        i.z=std::abs(circle.dy)>18?-circle.dy/156.f:0;
        i.confirm=down&KEY_A;i.cancel=down&KEY_X; // X requests partner dialogue in field.
        if(game.scene!=ttyd::Scene::Explore || frontend.screen!=ttyd::FrontScreen::Playing) i.cancel=down&KEY_B;
        i.left=down&KEY_DLEFT;i.right=down&KEY_DRIGHT;i.menuConfirm=down&KEY_Y;
        i.jumpHeld=held&KEY_A;
        i.cancelHeld=held&KEY_B;i.confirmReleased=up&KEY_A;i.cancelReleased=up&KEY_B;
        i.physicalSnapshot=true;
        i.physicalButtons=((held&KEY_A)?0x100:0)|((held&KEY_B)?0x200:0)|((held&KEY_X)?0x400:0)|
            ((held&KEY_Y)?0x800:0)|((held&KEY_START)?0x1000:0)|((held&KEY_L)?0x40:0)|((held&KEY_R)?0x20:0)|
            ((held&KEY_DLEFT)?1:0)|((held&KEY_DRIGHT)?2:0)|((held&KEY_DDOWN)?4:0)|((held&KEY_DUP)?8:0);
        if(down&KEY_TOUCH) {touchPosition touch;hidTouchRead(&touch);i.touchX=touch.px;i.touchY=touch.py;}
        merge(pending,i);pendingStart|=bool(down&KEY_START);
        const double simulationStart=seconds();metrics.steps=0;
        while(accumulator>=1./60 && metrics.steps<5) {
            if(frontend.screen==ttyd::FrontScreen::Playing)game.tick(pending);
            else {
                ++frontTicks;
                ttyd::FrontInput fi;fi.confirm=pending.confirm;fi.cancel=pending.cancel;
                fi.start=pendingStart;pendingStart=false;
                fi.previous=pending.left;fi.next=pending.right;
                if(pending.touchX>=10 && pending.touchX<310 && pending.touchY>=42 && pending.touchY<190)
                    fi.touchedSlot=(pending.touchY-42)/37;
                frontend.tick(fi);
                if(frontend.screen==ttyd::FrontScreen::Demo && frontReady){demo.tick(frontTicks*1000/60,(pending.confirm?0x100u:0u)|(pending.cancel?0x200u:0u));if(demo.finished)frontend.demoComplete();}
                if(frontend.newRequested){frontend.newRequested=false;savePath=ttyd::saveSlotPath(saveDirectory,frontend.selected);opening.begin(frontTicks*1000/60);}
                if(frontend.loadRequested) {
                    savePath=ttyd::saveSlotPath(saveDirectory,frontend.selected);
                    if(restoreProgress(frontend.slots[frontend.selected].progress)){frontend.acceptLoad();notice="";noticeTicks=0;}
                    else {frontend.rejectLoad();notice="Saved checkpoint unavailable";noticeTicks=240;}
                }
                if(frontend.screen==ttyd::FrontScreen::Opening && frontReady) {
                    // Opening is a separate input owner. Never advance field simulation here.
                    opening.tick(frontTicks*1000/60,(pending.confirm?0x100u:0u)|(pending.cancel?0x200u:0u)|(fi.start?0x1000u:0u));
                    if(opening.complete()) {
                        ttyd::Progress fresh;fresh.room=1;fresh.position={0,2.25f};fresh.elevation=.15f;fresh.originalStory=opening.story;
                        if(restoreProgress(fresh)){frontend.openingComplete();game.saveRequested=true;notice="";noticeTicks=0;}
                        else {notice="Original harbor could not load";noticeTicks=240;}
                    }
                }
            }
            ++metrics.steps;accumulator-=1./60;
            pending={};pending.x=i.x;pending.z=i.z;pending.jumpHeld=i.jumpHeld;pending.cancelHeld=i.cancelHeld;
            pending.physicalSnapshot=i.physicalSnapshot;pending.physicalButtons=i.physicalButtons;
            if(frontend.screen==ttyd::FrontScreen::Playing && game.progress.sound) audio.play(game);
            if(noticeTicks>0)--noticeTicks;
        }
        if(accumulator>=1./60) {metrics.droppedTicks+=uint64_t(accumulator*60);accumulator=0;}
        metrics.simulationMs=(seconds()-simulationStart)*1000;
        if(game.pipeRequest>=0 && game.fade.ready()) {
            ttyd::CollisionTransition transaction;
            auto ownedCandidate=std::make_unique<ttyd::Game>(game);auto& candidate=*ownedCandidate;
            const uint32_t target=game.pipeTarget();
            bool ok=stageRoomCollision(target,transaction) && candidate.finishPipeWarp();
            if(ok)ok=renderer.changeRoom(target);
            if(ok){transaction.commit();game=std::move(candidate);game.fade.startIn();notice="";noticeTicks=0;}
            else {game.cancelPipeWarp();notice="Pipe transition failed";noticeTicks=180;}
            accumulator=0;pending={};previous=seconds();
        }
        if(game.warpRequest>=0 && game.fade.ready()) {
            ttyd::CollisionTransition transaction;
            // Target ID comes from the original, validated road table.
            auto ownedCandidate=std::make_unique<ttyd::Game>(game);auto& candidate=*ownedCandidate;
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
                ok=restoreProgress(checkpoint);
            }
            notice=ok?"Progress loaded":"Save unavailable";noticeTicks=180;
            accumulator=0;pending={};previous=seconds();
        }
        if(frontend.screen==ttyd::FrontScreen::Playing)audio.update(game.scene,game.progress.sound,game.harborStory.enabled && game.harborStory.triggered,
                     game.professorStory.locked()?game.professorStory.music:0,game.progress.room);
        else audio.updateFrontend(frontend.screen==ttyd::FrontScreen::Demo?(demo.bookComplete?6:5):frontend.screen==ttyd::FrontScreen::Opening?opening.music():4,game.progress.sound);
        metrics.renderMs=C3D_GetProcessingTime();metrics.gpuMs=C3D_GetDrawingTime();
        if(frontend.screen==ttyd::FrontScreen::Playing)renderer.render(game,metrics,noticeTicks>0?notice:"",audio.available());
        else renderer.renderFrontend(frontend,demo,opening,noticeTicks>0?notice:"");
        game.fade.presented();
        ++frames;
        if(now-window>=1) {metrics.fps=frames/(now-window);window=now;frames=0;}
    }
    // A battle is transient; save only the exploration checkpoint.
    if(frontend.screen==ttyd::FrontScreen::Playing && !game.professorStory.temporaryProfessor && !game.professorStory.running() && game.scene==ttyd::Scene::Explore && !game.airborne() && !game.water.active() && !game.fade.active() && !game.arriving && game.warpRequest<0 && game.pipeRequest<0) ttyd::writeSave(savePath,game.progress);
    audio.shutdown();renderer.shutdown();romfsExit();gfxExit();return 0;
}
