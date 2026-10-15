#include "camera_road_assets.hpp"
#include "platform.hpp"
#include "field_pipe.hpp"
#include "save.hpp"
#include <algorithm>
#include <atomic>
#include <cstdio>
#include <cstring>
#include <sys/stat.h>
// Native service composition. Game advances on fixed simulation ticks; this
// file owns HID polling, GPU/resource transactions and filesystem workers.
// See docs/code/runtime.md for input and room/save ownership across a frame.
namespace {
double seconds() { return svcGetSystemTick()/double(SYSCLOCK_ARM11); }
bool stageRoomCollision(uint32_t room,ttyd::CollisionTransition& transaction) {
    const char* name=ttyd::roomAsset(room);if(!name)return false;
    char path[96];std::snprintf(path,sizeof(path),"romfs:/%s",name);
    return transaction.load(path,room);
}
bool loadRoomCollision(uint32_t room) {
    ttyd::CollisionTransition transaction;
    if(!stageRoomCollision(room,transaction))return false;
    transaction.commit();return true;
}
// Polling can run without a simulation tick. Retain the newest analog/held
// state but OR edges until one tick consumes them; a touch keeps its coordinates.
void merge(ttyd::Input& pending,const ttyd::Input& i) {
    pending.x=i.x;pending.z=i.z;pending.jumpHeld=i.jumpHeld;pending.cancelHeld=i.cancelHeld;
    pending.confirm|=i.confirm;pending.cancel|=i.cancel;pending.left|=i.left;pending.right|=i.right;
    pending.menuConfirm|=i.menuConfirm;
    pending.confirmReleased|=i.confirmReleased;pending.cancelReleased|=i.cancelReleased;
    pending.physicalSnapshot=i.physicalSnapshot;pending.physicalButtons=i.physicalButtons;
    if(i.touchX>=0) {pending.touchX=i.touchX;pending.touchY=i.touchY;}
}
// Only one SD write is outstanding. Copy the original pouch as well as the
// Progress shell: simulation may mutate live inventory while the worker writes.
// Release/acquire on done publishes success; ticket rejects stale UI completions.
struct SaveWorker {
    Thread thread=nullptr;
    std::string path;ttyd::Progress checkpoint;
    std::atomic<bool> done{false};bool success=false;uint64_t ticket=0;
    bool start(const std::string& destination,const ttyd::Progress& progress,uint64_t id) {
        if(thread)return false;
        path=destination;checkpoint=ttyd::snapshotSaveProgress(progress);ticket=id;done.store(false,std::memory_order_relaxed);
        // libctru worker: 16 KiB stack, priority 0x31, core -2 (default affinity),
        // joinable. These are native service settings, not original game constants.
        thread=threadCreate([](void* p) {
            auto& self=*static_cast<SaveWorker*>(p);self.success=ttyd::writeSave(self.path,self.checkpoint);
            self.done.store(true,std::memory_order_release);
        },this,16384,0x31,-2,false);
        return thread!=nullptr;
    }
    bool poll(uint64_t& id,bool& result) {
        if(!thread || !done.load(std::memory_order_acquire))return false;
        threadJoin(thread,UINT64_MAX);threadFree(thread);thread=nullptr;
        id=ticket;result=success;return true;
    }
    void finish(){if(thread){threadJoin(thread,UINT64_MAX);threadFree(thread);thread=nullptr;}}
    ~SaveWorker(){finish();}
};
}
int main() {
    gfxInitDefault();gfxSet3D(false);
    bool newModel=false;
    if(R_SUCCEEDED(APT_CheckNew3DS(&newModel)) && newModel) osSetSpeedupEnable(true);
    const Result romfs=romfsInit();
    auto ownedRenderer=std::make_unique<ttyd::Renderer>();auto& renderer=*ownedRenderer;
    if(R_FAILED(romfs) || !renderer.init()) {
        renderer.shutdown();consoleInit(GFX_TOP,nullptr);
        std::printf("TT YD prototype could not initialize.\nCheck packaged RomFS and available memory.\nPress START to exit.\n");
        while(aptMainLoop()) {hidScanInput();if(hidKeysDown()&KEY_START)break;gspWaitForVBlank();}
        if(R_SUCCEEDED(romfs)) romfsExit();
        gfxExit();
        return 1;
    }
    const char* failedAsset=nullptr;
    auto requireAsset=[&](const char* name,bool ready){if(!ready)failedAsset=name;return ready;};
    if(!requireAsset("initial room collision",loadRoomCollision(0)) ||
       !requireAsset("room warps",ttyd::loadWarps("romfs:/rooms.bin")) ||
       !requireAsset("field pipes",ttyd::loadFieldPipes("romfs:/field-pipes.bin")) ||
       !requireAsset("field doors",ttyd::loadFieldDoors("romfs:/doors.pmdoor")) ||
       !requireAsset("Trouble Center events",ttyd::loadNativeTroubleBoardAssets("romfs:")) ||
       !requireAsset("camera roads",ttyd::loadCameraRoadAssets("romfs:")) ||
       !requireAsset("harbor tutorial",ttyd::loadHarborTutorialAssets("romfs:")) ||
       !requireAsset("harbor encounter",ttyd::loadHarborEncounterAssets("romfs:")) ||
       !requireAsset("harbor story",ttyd::loadHarborStoryAssets("romfs:")) ||
       !requireAsset("field actor catalog",ttyd::loadFieldActorCatalog("romfs:")) ||
       !requireAsset("save objects",ttyd::loadFieldSaveObjectAssets("romfs:")) ||
       !requireAsset("professor story",ttyd::loadProfessorStoryAssets("romfs:")) ||
       !requireAsset("underground story",ttyd::loadUndergroundStoryAssets("romfs:")) ||
       !requireAsset("trio introduction",ttyd::loadTrioBattleAssets("romfs:")) ||
       !requireAsset("trio attacks",ttyd::loadTrioAttackAssets("romfs:"))) {
        if(failedAsset)svcOutputDebugString(failedAsset,int32_t(std::strlen(failedAsset)));
        renderer.shutdown();consoleInit(GFX_TOP,nullptr);
        std::printf("Development asset load failed:\n%s\nPress START to exit.\n",failedAsset?failedAsset:"unknown");
        while(aptMainLoop()){hidScanInput();if(hidKeysDown()&KEY_START)break;gfxFlushBuffers();gfxSwapBuffers();gspWaitForVBlank();}
        romfsExit();gfxExit();return 1;
    }
    ttyd::Audio audio;audio.init();
    auto ownedGame=std::make_unique<ttyd::Game>();auto& game=*ownedGame;ttyd::Progress saved;
    const std::string saveDirectory="sdmc:/3ds/ttyd-2ds";
    std::string savePath=ttyd::saveSlotPath(saveDirectory,0);
    mkdir("sdmc:/3ds",0777);mkdir(saveDirectory.c_str(),0777);
    ttyd::Frontend frontend;ttyd::OpeningDemo demo;ttyd::OpeningSequence opening;
    bool frontReady=demo.load("romfs:") && opening.load("romfs:");
    demo.begin(0);ttyd::scanSaveSlots(frontend,saveDirectory);
    std::string restoreFailure;
    // Stage collision first, validate a separate Game against that staged room,
    // then stage rendering. Failure rolls collision back through the transaction
    // destructor; commit only after every owner accepts the checkpoint.
    auto restoreProgress=[&](const ttyd::Progress& checkpoint) {
        ttyd::CollisionTransition transaction;float height=checkpoint.elevation;
        restoreFailure.clear();
        if(!stageRoomCollision(checkpoint.room,transaction)){restoreFailure="Checkpoint collision unavailable";return false;}
        auto ownedCandidate=std::make_unique<ttyd::Game>();auto& candidate=*ownedCandidate;candidate.restore(ttyd::snapshotSaveProgress(checkpoint));candidate.retainPhysicalInput(game);
        if(!candidate.checkpointGround(height)){restoreFailure="Checkpoint ground unavailable";return false;}
        if(!ttyd::walkable(checkpoint.position,height)){restoreFailure="Checkpoint position blocked";return false;}
        if(!renderer.changeRoom(checkpoint.room)){restoreFailure=renderer.resourceFailure().empty()?"Checkpoint scene unavailable":renderer.resourceFailure();return false;}
        candidate.cameraManager->sourceRenderDimensions=game.cameraManager->sourceRenderDimensions; // Source GP render fields survive a committed checkpoint handoff.
        candidate.cameraManager->stockExpPosition=game.cameraManager->stockExpPosition; // Preserve source process globals only after checkpoint admission.
        candidate.battleSequenceProcess=game.battleSequenceProcess; // Process8041e968 survives admitted checkpoint handoff.
        candidate.soundFlags=game.soundFlags; // Audio boot owner survives a successfully staged checkpoint.
        game=std::move(candidate);transaction.commit();return true;
    };
    game.restore(game.progress);
#ifndef TTYD_DEBUG_SHORT_BOOT
    renderer.beginFrontend();
#endif
#ifdef TTYD_DEBUG_SHORT_BOOT
    const auto loaded=ttyd::loadSave(savePath,saved);
    if(loaded==ttyd::LoadResult::Loaded || loaded==ttyd::LoadResult::Recovered)restoreProgress(saved);
    else restoreFailure=loaded==ttyd::LoadResult::Invalid?"Development checkpoint invalid":"Development checkpoint missing";
    frontend.screen=ttyd::FrontScreen::Playing;
#endif
    const char* notice=frontReady?"":"Original opening resources unavailable";
    // Notice lifetimes are native UI ticks: 240=4 seconds, 180=3 seconds at 60 Hz.
    // Story/demo clocks below use integer milliseconds (ticks * 1000 / 60).
    int noticeTicks=frontReady?0:240;
    uint64_t frontTicks=0;
    ttyd::Metrics metrics;
    double previous=seconds(),accumulator=0,window=previous;int frames=0;
    ttyd::Input pending;bool pendingStart=false;
    SaveWorker saveWorker;
    while(aptMainLoop()) {
        uint64_t completedTicket;bool saveSuccess;
        if(saveWorker.poll(completedTicket,saveSuccess) && game.saveObjects)
            game.saveObjects->presentation.completeWrite(completedTicket,saveSuccess);
        double now=seconds();double delta=std::max(0.,now-previous);previous=now;
        // Resume/menu interruptions must not cause seconds of battle advancement.
        // Native scheduling policy: >250 ms is an interruption. Discard backlog
        // and pending edges, then admit one 60 Hz tick instead of fast-forwarding.
        if(delta>.25) {accumulator=0;pending={};delta=1./60;}
        accumulator+=delta;
        hidScanInput();uint32_t down=hidKeysDown(),held=hidKeysHeld(),up=hidKeysUp();
        if((down&KEY_START) && frontend.screen==ttyd::FrontScreen::Playing) break;
        ttyd::Input i;circlePosition circle;hidCircleRead(&circle);
        // Native Circle Pad adapter: 18-unit dead zone, 156-unit normalization.
        // The later PAD bridge maps this normalized value to the source +/-70 range.
        i.x=std::abs(circle.dx)>18?circle.dx/156.f:0;
        i.z=std::abs(circle.dy)>18?-circle.dy/156.f:0;
        i.confirm=down&KEY_A;i.cancel=down&KEY_X; // X requests partner dialogue in field.
        if(game.scene!=ttyd::Scene::Explore || frontend.screen!=ttyd::FrontScreen::Playing) i.cancel=down&KEY_B;
        i.left=down&KEY_DLEFT;i.right=down&KEY_DRIGHT;i.menuConfirm=down&KEY_Y;
        i.jumpHeld=held&KEY_A;
        i.cancelHeld=held&KEY_B;i.confirmReleased=up&KEY_A;i.cancelReleased=up&KEY_B;
        i.physicalSnapshot=true;
        // GameCube PAD ABI: A/B/X/Y=0x100/200/400/800, Start=0x1000,
        // L/R=0x40/20, D-pad left/right/down/up=1/2/4/8. 3DS key bits differ.
        i.physicalButtons=((held&KEY_A)?0x100:0)|((held&KEY_B)?0x200:0)|((held&KEY_X)?0x400:0)|
            ((held&KEY_Y)?0x800:0)|((held&KEY_START)?0x1000:0)|((held&KEY_L)?0x40:0)|((held&KEY_R)?0x20:0)|
            ((held&KEY_DLEFT)?1:0)|((held&KEY_DRIGHT)?2:0)|((held&KEY_DDOWN)?4:0)|((held&KEY_DUP)?8:0);
        if(down&KEY_TOUCH) {touchPosition touch;hidTouchRead(&touch);i.touchX=touch.px;i.touchY=touch.py;}
        merge(pending,i);pendingStart|=bool(down&KEY_START);
        const double simulationStart=seconds();metrics.steps=0;
        // Catch-up cap is five SIMULATION ticks per render iteration. Only the
        // first tick receives accumulated edges; subsequent ticks keep held state.
        while(accumulator>=1./60 && metrics.steps<5) {
            if(frontend.screen==ttyd::FrontScreen::Playing){
                // Scoped native sequence transition: genuine GPU synchronization and
                // complete field Exit; battle Init remains a separate provider wait.
                // Existing demo gameplay is still an adapter, not full source seqMain.
                ttyd::OriginalSequenceServices sequence;sequence.user=&game;
                sequence.drawSync=[](void*){
                    // Blocking FrameBegin waits the previous GX command queue.
                    // Close this empty frame before the ordinary renderer begins.
                    if(!C3D_FrameBegin(0))return false;
                    C3D_FrameEnd(0);return true;
                };
                sequence.callback=[](void* user,uint32_t identity,ttyd::OriginalSequenceWork&){
                    if(identity!=0x80007390)return false;
                    ttyd::originalFieldSequenceExit(static_cast<ttyd::Game*>(user)->fieldSystemFlags);return true;
                };
                if(!game.trioAttack.outerMainPending() && (game.originalSequence->state.requested!=game.originalSequence->state.current || game.originalSequence->pending()))game.originalSequence->main(sequence);
                const bool outerUpdate=game.trioAttack.outerMainPending();
                if(!outerUpdate){audio.bindFieldMarioMotion(game);renderer.bindFieldItemGet(game,audio);renderer.advanceTutorialAnimations(game);}
                game.tick(pending,&sequence);
                if(!outerUpdate){renderer.advanceFieldIcons();audio.tickKeyItemAudio(ttyd::TickRate);renderer.advanceFieldItemPresentation();}}
            else {
                ++frontTicks;
                // PAD publication is global across scene ownership. Preserve
                // held-button history when the fresh field Game is restored,
                // so the final dialogue edge cannot become a second jump edge.
                if(pending.physicalSnapshot) {
                    std::array<ttyd::RawPad,4> pads{};pads[0].buttons=uint16_t(pending.physicalButtons);
                    pads[0].mainX=int8_t(std::clamp(pending.x,-1.f,1.f)*70.f);
                    pads[0].mainY=int8_t(std::clamp(-pending.z,-1.f,1.f)*70.f);
                    game.publishPhysicalInput(pads);
                }
                ttyd::FrontInput fi;fi.confirm=pending.confirm;fi.cancel=pending.cancel;
                fi.start=pendingStart;pendingStart=false;
                fi.previous=pending.left;fi.next=pending.right;
                // Native four-slot menu hitbox: x [10,310), y [42,190),
                // rows 37 pixels high. This layout is an adaptation for the 3DS.
                if(pending.touchX>=10 && pending.touchX<310 && pending.touchY>=42 && pending.touchY<190)
                    fi.touchedSlot=(pending.touchY-42)/37;
                if(frontend.screen==ttyd::FrontScreen::Title)frontend.title.admitMusic(audio.frontendTrackReady(4));
                const auto previousFrontend=frontend.screen;
                frontend.tick(fi);
                if(previousFrontend==ttyd::FrontScreen::Title && frontend.screen==ttyd::FrontScreen::Demo)demo.begin(frontTicks*1000/60);
                if(frontend.screen==ttyd::FrontScreen::Demo && frontReady){demo.tick(frontTicks*1000/60,(pending.confirm?0x100u:0u)|(pending.cancel?0x200u:0u)|(fi.start?0x1000u:0u));if(demo.finished){frontend.demoComplete();frontend.title.bgm.begin(demo.bgm.snapshot());}}
                if(frontend.newRequested){frontend.newRequested=false;savePath=ttyd::saveSlotPath(saveDirectory,frontend.selected);opening.bindSceneServices();opening.begin(frontTicks*1000/60);}
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
                        ttyd::initializeTroubleQuestState(fresh.originalNpcTalkState); // Explicit original swInit caller.
                        fresh.originalPouch=ttyd::createProloguePouch(); // Explicit native new-file handoff using the recovered initial-stage pouch snapshot.
                        if(renderer.prepareOpeningDeparture(opening.harbor,0) && restoreProgress(fresh) && game.retainOpeningDeparture(opening.harbor)){frontend.openingComplete();game.saveRequested=true;notice="";noticeTicks=0;}
                        else {notice="Original harbor could not load";noticeTicks=240;}
                    }
                }
            }
            ++metrics.steps;accumulator-=1./60;
            pending={};pending.x=i.x;pending.z=i.z;pending.jumpHeld=i.jumpHeld;pending.cancelHeld=i.cancelHeld;
            pending.physicalSnapshot=i.physicalSnapshot;pending.physicalButtons=i.physicalButtons;
            if(frontend.screen==ttyd::FrontScreen::Playing && game.progress.sound) audio.play(game);
            game.troubleBoardSounds.clear(); // Consume once per native audio visit, including muted/unavailable playback.
            if(noticeTicks>0)--noticeTicks;
        }
        // Native overload policy drops whole remaining ticks (and the fraction),
        // recording the count rather than changing action-command tick duration.
        if(accumulator>=1./60) {metrics.droppedTicks+=uint64_t(accumulator*60);accumulator=0;}
        metrics.simulationMs=(seconds()-simulationStart)*1000;
        // Pipe/road/reload work happens outside Game::tick. Loading may block;
        // reset wall-clock accumulation afterwards so IO time cannot age gameplay.
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
        // Save-block writes carry an immutable checkpoint and presentation ticket.
        // Ordinary save requests below wait for this worker to relinquish the file.
        if(game.saveObjects) {
            uint64_t ticket;ttyd::SaveBlockSwitches switches;
            if(game.saveObjects->presentation.takeWriteRequest(ticket,switches)) {
                auto checkpoint=game.progress;
                checkpoint.originalStory=switches.story;checkpoint.originalFlag233=switches.firstHit;
                checkpoint.originalFlag236=switches.postStoryHit;checkpoint.originalSaveMode=switches.mode;
                if(!saveWorker.start(savePath,checkpoint,ticket))game.saveObjects->presentation.completeWrite(ticket,false);
            }
        }
        if(game.saveRequested && !game.saveLocked() && !saveWorker.thread) {
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
        if(frontend.screen==ttyd::FrontScreen::Playing && game.harborTutorial.running())audio.updateFrontend(12,game.progress.sound);
        else if(frontend.screen==ttyd::FrontScreen::Playing)audio.update(game.trioBattleActive?(game.trioAttack.rewardPresentationActive()?ttyd::Scene::Victory:ttyd::Scene::Battle):game.scene,game.progress.sound,game.harborStory.enabled && game.harborStory.triggered,
                     game.professorStory.locked() && !game.trioBattleActive?game.professorStory.music:0,game.progress.room,
                     game.trioBattleActive?(game.trioCommands.state==ttyd::TrioBattleCommands::State::Command || game.trioCommands.state==ttyd::TrioBattleCommands::State::Target):game.phase==ttyd::Phase::Select);
        else if(frontend.screen==ttyd::FrontScreen::Demo)audio.updateFrontend(demo,game.progress.sound);
        else if(frontend.screen==ttyd::FrontScreen::Title)audio.updateFrontend(frontend.title,game.progress.sound);
        else if(frontend.screen==ttyd::FrontScreen::Opening)audio.updateFrontend(opening,game.progress.sound);
        else audio.updateFrontend(4,game.progress.sound);
        audio.updateOpeningHousePcm(frontend.screen==ttyd::FrontScreen::Opening && opening.part==ttyd::OpeningPart::House?&opening.house.sounds:nullptr,game.progress.sound);
        metrics.renderMs=C3D_GetProcessingTime();metrics.gpuMs=C3D_GetDrawingTime();
        const char* displayedNotice=noticeTicks>0?notice:"";
#ifdef TTYD_DEBUG_SHORT_BOOT
        // Persistent harness diagnostics, separate from original story dialogue.
        if(!restoreFailure.empty())displayedNotice=restoreFailure.c_str();
#endif
        if(frontend.screen==ttyd::FrontScreen::Playing)renderer.render(game,metrics,displayedNotice,audio.available());
        else renderer.renderFrontend(frontend,demo,opening,displayedNotice);
        game.fade.presented();
        ++frames;
        if(now-window>=1) {metrics.fps=frames/(now-window);window=now;frames=0;}
    }
    saveWorker.finish(); // Complete an already accepted write before closing services.
    // A battle is transient; save only the exploration checkpoint.
    if(frontend.screen==ttyd::FrontScreen::Playing && !game.progress.originalStory && !game.harborEncounter.running() && !game.saveLocked() && !game.professorStory.temporaryProfessor && !game.professorStory.running() && game.scene==ttyd::Scene::Explore && !game.airborne() && !game.water.active() && !game.fade.active() && !game.arriving && game.warpRequest<0 && game.pipeRequest<0) ttyd::writeSave(savePath,game.progress);
    audio.shutdown();renderer.shutdown();romfsExit();gfxExit();return 0;
}
