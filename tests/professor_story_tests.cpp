#include "professor_story.hpp"
#include "game.hpp"
#include "secondary_party_source.hpp"
#include "paper_fence_warp.hpp"
#include <limits>
#include <cassert>
#include <iostream>
using namespace ttyd;
int main() {
    assert(secondaryPartyUsesOther(true,1,2,0,149.999f));
    assert(!secondaryPartyUsesOther(true,1,2,0,150.f));
    assert(!secondaryPartyUsesOther(false,1,2,0,0));
    assert(!secondaryPartyUsesOther(true,3,3,0,0));
    assert(!secondaryPartyUsesOther(true,1,2,3,0));
    assert(secondaryPartyUsesOther(true,1,2,0,std::numeric_limits<float>::quiet_NaN()));
    assert(loadProfessorStoryAssets("romfs"));ProfessorStory s;
    assert(s.begin(0,{-215,0,-95},{-245,0,-95}));
    for(uint64_t t=0;t<1000;t+=17)s.tick(t,0x100,true);
    assert(s.messageOrder.empty() && s.actors[2].position[0]==-115 && s.actors[2].position[2]==-225);
    float jumpPeak=0;
    for(uint64_t t=1000;t<400000 && s.locked();t+=17){s.tick(t,0x100,false);jumpPeak=std::max(jumpPeak,s.actors[2].position[1]);}
    assert(!s.failed && s.finished && s.story==9 && s.keysEnabled && s.temporaryProfessor);
    const char* suffixes[]={"066_kur","066_01","067","067_01","067_02","067_03","067_04","067_05","067_06","068_kur","069","069_01","069_02","069_03","070_kur","071","072_kur","073","073_01","074_kur","075","076_kur","077","078_kur","079","080","081","081_01_kur","082"};
    assert(s.messageOrder.size()==29);
    for(unsigned i=0;i<29;++i)assert(s.messageOrder[i]==std::string("mac_0_")+suffixes[i]);
    assert(s.actors[0].pose=="M_S_1");
    assert(s.actors[2].position[0]==-140 && s.actors[2].position[2]==-225);
    assert(s.presentationRequests.size()==119 && s.cameras.size()==17);
    assert(jumpPeak>29 && jumpPeak<=30.001f);
    assert(!s.itemVisible && std::fabs(s.itemScale-.5498046875f)<.00001f && s.music==0);
    const uint64_t fenceStart=s.now+17;
    assert(s.beginFence(fenceStart,{-215,0,-15},{-245,0,-15}));
    for(uint64_t t=fenceStart;t<fenceStart+1000;t+=17)s.tick(t,0x100,true);
    assert(s.messageOrder.empty() && s.temporaryProfessor && !s.captureRequested);
    for(uint64_t t=s.now+17;t<fenceStart+60000 && !s.captureRequested;t+=17)s.tick(t,0x100,false);
    assert(s.captureRequested && !s.captureReady && !s.fenceCollisionOff && !s.temporaryProfessor);
    assert(s.messageOrder.size()==1 && s.messageOrder[0]=="mac_0_083");
    s.captureBounds={100,100,80,60};s.captureReady=true;
    uint64_t animationStart=0;
    for(uint64_t t=s.now+17;t<fenceStart+100000 && s.story==9;t+=17) {
        s.tick(t,0x100,false);
        if(s.fencePose=="A_1") {
            animationStart=s.fencePoseAt;
            if(double(t-animationStart)*60<140000)assert(!s.fenceCollisionOff);
        }
    }
    assert(animationStart && s.fenceCollisionOff && s.fenceHidden && !s.fenceVisible);
    assert(!s.failed && s.story==10 && s.running() && !s.locked() && s.keysEnabled);
    assert(s.messageOrder.size()==2 && s.messageOrder[1]=="mac_0_084");
    assert(s.actors[2].position[0]==-220 && s.actors[2].position[2]==305);
    for(unsigned i=0;i<60;++i)s.tick(s.now+17,0,false);
    assert(s.messageOrder.size()==2); // Lecture remains a live crossing poll.
    s.actors[0].position={-260,0,260};
    for(unsigned i=0;i<10000 && !s.selection.active();++i)s.tick(s.now+17,0x100,false);
    assert(!s.failed && s.locked() && s.story==10 && s.selection.active());
    assert(s.messageOrder.size()==4 && s.messageOrder[2]=="mac_0_084_01" && s.messageOrder[3]=="mac_0_084_02");
    for(unsigned i=0;i<10000 && s.running();++i) {
        MessageSelectionInput input;
        if(s.selection.active() && s.selection.state==1) {
            const int desired=s.selectionOrder.size()==1?0:1;
            input.buttons=0x100|(s.selection.current==desired?0u:desired==0?8u:4u);
        }
        s.tick(s.now+17,0x100,false,input);
    }
    assert(!s.failed && s.finished && s.story==11 && s.keysEnabled);
    assert((s.selectionResults==std::vector<int>{0,1}));
    assert(s.messageOrder.size()==6 && s.messageOrder[4]=="mac_0_084_03" && s.messageOrder[5]=="mac_0_084_04");
    assert(s.message.id()=="mac_0_084_02" && s.message.rawText().find("<o>")!=std::string::npos);
    for(int first:{0,1}) {
        ProfessorStory lesson;assert(lesson.beginLecture(0,{-260,0,200},{-290,0,200}));
        lesson.tick(17,0,false);lesson.actors[0].position[2]=260;
        float goombellaHop=0;
        for(unsigned i=0;i<12000 && !lesson.pendingService;++i) {
            MessageSelectionInput input;
            if(lesson.selection.active() && lesson.selection.state==1) {
                const int desired=lesson.selectionOrder.size()==1?first:0;
                input.buttons=0x100|(lesson.selection.current==desired?0u:desired==0?8u:4u);
            }
            lesson.tick(lesson.now+17,0x100,false,input);
            goombellaHop=std::max(goombellaHop,lesson.actors[1].position[1]);
        }
        assert(!lesson.failed && lesson.story==10 && lesson.pendingService && lesson.battleSetup==3 && lesson.goombellaNpc);
        assert(lesson.selectionResults.size()==(first==1?1u:2u));
        assert(lesson.messageOrder.back()=="mac_0_084_06" && lesson.speaker==1);
        assert(!lesson.actors[1].hidden && goombellaHop>19 && goombellaHop<=20.001f);
    }
    ActorAnimationPack paper;assert(paper.load("romfs/professor-fence.pmanim"));
    const auto* paperPose=paper.pose("A_1");assert(paperPose && paperPose->end==140 && paperPose->keys.size()==7);
    ActorAnimationSample paperSample;assert(paperPose->evaluate(0,paperSample));
    std::array<ActorVector,4> corners{};
    for(size_t i=0;i<paperPose->bindings.size();++i) {
        const auto& v=paperSample.vertices[i];corners[paperPose->bindings[i].position]={v.x,v.y,v.z};
    }
    const CameraView identity{{{1,0,0,0},{0,1,0,0},{0,0,1,0}}};
    const ActorVector original{.25f,.35f,2};
    const auto unchanged=paperFenceWarp(original,identity,{0,0,1,1},corners);
    for(unsigned i=0;i<3;++i)assert(std::fabs(unchanged[i]-original[i])<.00001f);
    assert(paperPose->evaluate(47,paperSample));
    assert(paperSample.vertices[0].z!=0 || paperSample.vertices[0].y!=-24);
    clearCollision();assert(loadFieldDoors("romfs/doors.pmdoor"));
    assert(loadCollision("romfs/gor_02.walls") && loadGround("romfs/gor_02.ground") && loadHead("romfs/gor_02.head"));
    Progress p;p.room=2;p.originalStory=8;p.position={-2.15f,-.15f};
    Game g;g.restore(p);Input forward;forward.z=-1;
    assert(!walkable({-2.6472f,2.3427f},0)); // Original A_saku_04 blocks the route.
    for(int i=0;i<24;++i)g.tick(forward);
    Input a;a.confirm=true;g.tick(a);
    assert(g.fieldDoor.active() && g.professorStory.locked());
    for(int i=0;i<24000 && (g.fieldDoor.active() || g.professorStory.locked());++i)g.tick(a);
    assert(g.professorStory.finished && !g.professorStory.failed && g.progress.originalStory==9);
    assert(g.scene==Scene::Explore && g.professorStory.messageOrder.size()==29 && !g.saveRequested);
    const auto professorStart=g.professorStory.actors[2].position;
    Input leave;leave.z=1;
    for(unsigned i=0;i<100;++i)g.tick(leave);
    assert(g.professorFollow.initialized && !g.professorFollow.trail.empty());
    assert(g.professorStory.actors[2].position!=professorStart);
    assert(g.professorStory.temporaryProfessor && !g.saveRequested && g.progress.originalStory==9);
    Input align;align.x=-1;
    for(unsigned i=0;i<200 && g.progress.position.x> -2.15f;++i)g.tick(align);
    for(unsigned i=0;i<200 && g.wallContact.sourceNode!=fieldDoors()[1].closeNode;++i)g.tick(leave);
    g.tick(a);assert(g.fieldDoor.active() && g.professorStory.fenceStarted);
    for(unsigned i=0;i<12000 && g.professorStory.story==9;++i) {
        if(g.professorStory.captureRequested){g.professorStory.captureReady=true;g.professorStory.captureBounds={100,100,80,60};}
        g.tick(a);
    }
    assert(g.progress.originalStory==10 && !g.fieldDoor.active() && !(g.fieldDoor.interiorMask&2));
    assert(g.professorStory.fenceCollisionOff && !g.professorStory.temporaryProfessor && !g.saveRequested);
    assert(walkable({-2.6472f,2.3427f},0));
    for(unsigned i=0;i<20;++i)g.tick(align);
    for(unsigned i=0;i<600 && !g.professorStory.selection.active();++i) {
        Input cross=g.professorStory.locked()?a:leave;g.tick(cross);
    }
    assert(g.professorStory.selection.active() && g.professorStory.messageOrder.size()==4 && g.progress.originalStory==10);
    Progress reload;reload.room=2;reload.originalStory=10;reload.position={-2.6f,2};
    g.restore(reload);assert(g.professorStory.running() && !g.professorStory.locked());
    for(unsigned i=0;i<1200 && !g.professorStory.selection.active();++i)g.tick(g.professorStory.locked()?a:leave);
    assert(g.professorStory.selection.active() && g.progress.originalStory==10);
    reload.originalStory=11;g.restore(reload);
    assert(!g.professorStory.running() && !g.professorStory.actors[2].hidden);
    assert((g.professorStory.actors[2].position==std::array<float,3>{-220,0,305}) && g.professorStory.actors[2].yaw==270);
    reload.originalFlag1207=true;g.restore(reload);assert(g.professorStory.actors[2].hidden && g.professorStory.professorDeparted);
    ProfessorStory pipe;assert(pipe.departThroughPipe(0,11,false));
    assert(pipe.finished && pipe.story==11 && pipe.professorDeparted && !pipe.professorControlEnabled);
    assert(pipe.temporaryProfessor && pipe.actors[2].hidden && pipe.actors[2].position[1]==-1000);
    const auto requests=pipe.presentationRequests.size();assert(requests==3);
    assert(pipe.departThroughPipe(0,11,true) && pipe.presentationRequests.empty() && pipe.story==11);
    ProfessorStory imported;assert(imported.beginLecture(0,{-260,0,200},{-290,0,200},true));
    imported.tick(17,0,false);assert(imported.professorDeparted);
    clearCollision();
    std::cout<<"Original REL first-talk: 29 ordered messages, concurrent waits and story9 passed\n";
}
