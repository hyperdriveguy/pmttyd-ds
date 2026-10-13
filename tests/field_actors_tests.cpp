#include "field_actors.hpp"
#include "actor_animation.hpp"
#include "game.hpp"
#include "camera_road_assets.hpp"
#include <cassert>
#include <cstring>
#include <iostream>
#include <limits>
#include <unistd.h>
using namespace ttyd;
namespace {
template<class T> void append(std::vector<uint8_t>& bytes,const T& value) {
    const auto* p=reinterpret_cast<const uint8_t*>(&value);bytes.insert(bytes.end(),p,p+sizeof(value));
}
void text(std::vector<uint8_t>& bytes,const char* value){std::array<char,32> s{};std::strncpy(s.data(),value,31);append(bytes,s);}
std::vector<uint8_t> fixture() {
    std::vector<uint8_t> b;const std::array<char,8> magic{'P','M','N','P','C','0','0','1'};append(b,magic);
    append(b,uint32_t(1));append(b,uint32_t(2));text(b,"fixture-model");append(b,2.f);
    for(int story:{8,10}) {
        for(uint32_t v:{17u,12u,0u,story==8?2u:0u,story==8?0u:2u,1u,0u})append(b,v);
        append(b,int32_t(story));append(b,int32_t(story));
        append(b,std::array<float,3>{-135,0,-240});append(b,90.f);append(b,2.f);text(b,"S_1");append(b,std::array<char,32>{});
    }
    return b;
}
bool read(FieldActorAssets& a,const std::vector<uint8_t>& b) {
    FILE* f=std::tmpfile();assert(f);assert(std::fwrite(b.data(),1,b.size(),f)==b.size());std::rewind(f);
    const bool ok=a.read(f);std::fclose(f);return ok;
}
template<class T> void replace(std::vector<uint8_t>& b,size_t offset,T v){assert(offset+sizeof(v)<=b.size());std::memcpy(b.data()+offset,&v,sizeof(v));}
}
int main(int argc,char**) {
    FieldActorAssets a;const auto good=fixture();assert(read(a,good));assert(a.models().size()==1 && a.actors().size()==2);
    const auto& indoors=a.actors()[0];const auto& outdoors=a.actors()[1];
    assert(indoors.visible(12,8,2) && !indoors.visible(12,8,0) && !indoors.visible(12,9,2));
    assert(!indoors.visible(12,std::nullopt,2) && !indoors.visible(2,8,2));
    assert(outdoors.visible(12,10,0) && !outdoors.visible(12,10,2));
    constexpr size_t row=52,stride=120;
    for(auto bad:std::vector<std::pair<size_t,uint32_t>>{{0,0},{8,65},{12,1025},{row,0},{row+8,1},{row+20,2},{row+24,2},{row+28,11},{row+44,0x7fc00000},{row+52,0}}) {
        auto b=good;replace(b,bad.first,bad.second);assert(!read(a,b));assert(a.actors().size()==2 && a.models()[0].asset=="fixture-model");
    }
    {auto b=good;replace(b,row+stride+28,8u);replace(b,row+stride+32,8u);replace(b,row+stride+12,2u);replace(b,row+stride+16,0u);assert(!read(a,b));}
    {auto b=good;b[16]='/';assert(!read(a,b));}
    {auto b=good;b.push_back(0);assert(!read(a,b));}
    {auto b=good;b.pop_back();assert(!read(a,b));}
    assert(!a.load("/missing-field-actor-catalog") && a.actors().size()==2);
    {auto b=good;b[7]='2';
        for(int i=1;i>=0;--i)b.insert(b.begin()+row+i*stride+28,4,0);
        replace(b,row+20,2u);replace(b,row+32,341u);replace(b,row+36,341u);replace(b,row+28,0x20u);
        assert(read(a,b));assert(a.actors()[0].visible(12,8,2,0) && !a.actors()[0].visible(12,341,2,0));
        assert(!a.actors()[0].visible(12,8,2,0x20) && !a.actors()[0].visible(12,std::nullopt,2,0));
        replace(b,row+36,342u);assert(!read(a,b));assert(a.actors()[0].storyHigh==341);
    }
    {auto b=good;b[7]='3';
        // Upgrade the synthetic V1 fixture to V3 without regenerating test logic.
        for(int i=1;i>=0;--i) {
            b.insert(b.begin()+row+i*stride+stride,32,0);
            b.insert(b.begin()+row+i*stride+28,4,0);
        }
        b.insert(b.begin()+row,28,0);
        assert(read(a,b) && a.models()[0].hitMode==0 && a.actors()[0].sourceName.empty());
        auto bad=b;replace(bad,row,2u);assert(!read(a,bad));
        bad=b;replace(bad,row+4,0x7fc00000u);assert(!read(a,bad));
        bad=b;replace(bad,row,1u);assert(!read(a,bad)); // Degenerate bbox.
        assert(a.models()[0].hitMode==0 && a.actors().size()==2);
        b[7]='4';b.insert(b.begin()+row+28,32,0);
        std::memcpy(b.data()+row+28,"FixtureModel",12);
        assert(read(a,b) && a.models()[0].originalName=="FixtureModel");
        bad=b;bad[row+28]='/';assert(!read(a,bad));
        bad=b;bad[row+28]=0;assert(!read(a,bad));
        assert(a.models()[0].originalName=="FixtureModel");
        b[7]='5';b.insert(b.begin()+row+60,4,0);replace(b,row+60,1u);const size_t actors=row+64;
        for(unsigned i=0;i<2;++i) {
            const size_t at=actors+i*196+156;
            b.insert(b.begin()+at,40,0);
            replace(b,at,0x41f00000u);replace(b,at+4,0x41f00000u);
            std::memcpy(b.data()+at+8,"TalkPose",8);
        }
        assert(read(a,b) && a.actors()[0].npcHeight==30 && a.actors()[0].talkPose=="TalkPose");
        bad=b;replace(bad,actors+156+4,0x7fc00000u);assert(!read(a,bad));
        assert(a.actors()[0].npcHeight==30);
        b[7]='6';
        for(unsigned i=0;i<2;++i){const size_t at=actors+i*200+196;b.insert(b.begin()+at,4,0);replace(b,at,0x80603u);}
        assert(read(a,b) && a.actors()[0].npcFlags==0x80603);
        b[7]='7';for(int i=1;i>=0;--i)b.insert(b.begin()+actors+i*200+200,4,0);
        assert(read(a,b) && a.actors()[0].talkRulesBound && !fieldActorTalkAvailable(a.actors()[0],8) && fieldActorTalkKey(a.actors()[0],8).empty());
        std::memcpy(b.data()+actors+92,"gor_02_034_",11);
        replace(b,actors+200,1u);std::vector<uint8_t> rule;
        append(rule,0u);append(rule,int32_t(56));text(rule,"gor_02_034_00");
        b.insert(b.begin()+actors+204,rule.begin(),rule.end());
        assert(read(a,b) && fieldActorTalkKey(a.actors()[0],55)=="gor_02_034_00");
        assert(fieldActorTalkKey(a.actors()[0],56).empty() && fieldActorTalkKey(a.actors()[0],175).empty());
        bad=b;replace(bad,actors+200,65u);assert(!read(a,bad));
        bad=b;replace(bad,actors+204,4u);assert(!read(a,bad));
        bad=b;bad[actors+204+8]='x';assert(!read(a,bad));
        bad=b;bad.pop_back();assert(!read(a,bad));
        std::memcpy(b.data()+actors+92,"tik_04_000_",11);std::memcpy(b.data()+actors+212,"tik_04_000_00",12);
        assert(read(a,b) && fieldActorTalkKey(a.actors()[0],8)=="tik_04_000_00"); // Future room namespace.
        b[7]='8';b.insert(b.end(),188,0);b.insert(b.begin()+actors+244,188,0);replace(b,actors+244,1u);
        assert(read(a,b) && a.actors()[0].sourceTalkPresent && !a.actors()[0].wander.enabled);
        bad=b;replace(bad,actors+244,0u);assert(!read(a,bad)); // Bound rules require original event presence.
        bad=b;replace(bad,actors+244+12,0x7fc00000u);assert(!read(a,bad));
        bad=b;replace(bad,actors+244+4,1u);assert(!read(a,bad)); // Incomplete moving profile rejected atomically.
        bad=b;bad.pop_back();assert(!read(a,bad));
    }
    FieldActorRecord ordered;ordered.talkRulesBound=true;
    ordered.talkRules={{1,175,"gor_02_034_03"},{0,215,"gor_02_034_04"},{0,215,"gor_02_034_05"},{2,401,"gor_02_034_10"}};
    assert(fieldActorTalkKey(ordered,175)=="gor_02_034_03" && fieldActorTalkKey(ordered,214)=="gor_02_034_04");
    assert(fieldActorTalkKey(ordered,215).empty() && fieldActorTalkKey(ordered,401)=="gor_02_034_10");
    ordered.talkRules.push_back({3,0,"gor_02_034_00"});assert(fieldActorTalkKey(ordered,300)=="gor_02_034_00");
    if(argc>1) {
        assert(a.load("romfs/field-actors.pmnpc"));assert(a.actors().size()>=3 && a.models().size()>=2);
        assert(a.models()[0].asset=="field-chu" && a.models()[1].asset=="professor");
        const auto findActor=[&](const char* asset)->const FieldActorRecord&{for(const auto& actor:a.actors())if(a.models()[actor.model].asset==asset)return actor;std::abort();};
        const auto& chu=findActor("field-chu");assert(chu.room==2 && chu.pose=="CHU_S_1");
        assert(chu.npcWidth==30 && chu.npcHeight==30 && chu.talkPose=="CHU_T_1" && a.models()[0].drawMask==1);
        assert(chu.npcFlags==0x80603 && findActor("professor").npcFlags==0x40080603);
        assert((chu.position==std::array<float,3>{150,0,-400}) && chu.yaw==270);
        assert(chu.visible(2,8,0) && chu.visible(2,55,2) && !chu.visible(2,56,0) && !chu.visible(2,std::nullopt,0));
        const auto& saveBlock=findActor("field-saveblock");assert(saveBlock.kind==1 && saveBlock.scale==10 && saveBlock.pose=="S_1");
        assert((saveBlock.position==std::array<float,3>{185,60,-55}));
        assert(saveBlock.visible(0,0,0) && saveBlock.visible(0,400,0) && !saveBlock.visible(0,341,0) && !saveBlock.visible(0,8,0,0x20));
        const auto scaled=ActorRootTransform({0,0,0},0,false,10.f*.01f).point({10,10,10,0,0});
        for(float v:scaled)assert(std::fabs(v-1.f)<.000001f);
        assert(loadCollision("romfs/gor_02.walls") && loadGround("romfs/gor_02.ground") && loadHead("romfs/gor_02.head"));
        assert(loadCameraRoadAssets("romfs"));
        float height=0;assert(groundAt({1.f,-3.7f},0,height) && walkable({1.f,-3.7f},height));
        assert(loadFieldActorCatalog("romfs") && loadHarborStoryAssets("romfs"));
        MessageAssets originals;assert(originals.loadRoom("romfs/messages/gor_02.txt"));
        const auto& darkly=findActor("field-darkly");assert(a.models()[darkly.model].originalName=="c_f_mura");
        assert((darkly.position==std::array<float,3>{-335,0,-335}) && darkly.yaw==270 && darkly.npcFlags==0x40080603);
        assert(darkly.npcWidth==30 && darkly.npcHeight==30 && darkly.talkPose=="T_1");
        assert(darkly.visible(2,8,0) && !darkly.visible(2,56,0));
        Game staticTurn;Progress staticSaved;staticSaved.room=2;staticSaved.originalStory=8;staticSaved.position={1.2f,-4};
        staticTurn.restore(staticSaved);for(int n=0;n<360;++n)staticTurn.tick({});
        staticTurn.progress.position={-3.35f,-3.65f};
        staticTurn.marioPresentation.display=staticTurn.marioPresentation.target=180;staticTurn.tick({});
        const auto* liveStatic=staticTurn.fieldNpcState(darkly.key);
        assert(liveStatic && liveStatic->initialized && liveStatic->presentationReady);
        assert(liveStatic->savedYaw==270 && liveStatic->movement.yaw==0 && (liveStatic->body.state&0x20000000));
        staticTurn.progress.position.x+=.05f;staticTurn.tick({});
        assert(staticTurn.fieldNpcState(darkly.key)->movement.yaw==0); // First admission owns yaw.
        staticTurn.fieldKeysOff=true;staticTurn.progress.position={1.2f,-4};staticTurn.tick({});
        assert(staticTurn.fieldNpcState(darkly.key)->body.state&0x20000000); // Skipped gates retain ownership.
        staticTurn.fieldKeysOff=false;staticTurn.tick({});liveStatic=staticTurn.fieldNpcState(darkly.key);
        assert(!(liveStatic->body.state&0x20000000) && (liveStatic->secondaryFlags&0x10000000) && liveStatic->movement.yaw==0);
        staticTurn.tick({});liveStatic=staticTurn.fieldNpcState(darkly.key);
        assert(!liveStatic->secondaryFlags && liveStatic->movement.yaw==0); // Null regular does not restore yaw.
        assert(fieldActorTalkAvailable(darkly,8) && !fieldActorTalkAvailable(darkly,175));
        assert(fieldActorTalkKey(darkly,8)=="gor_02_034_00" && fieldActorTalkKey(darkly,175).empty());
        ActorAnimationPack darklyArt;assert(darklyArt.load("romfs/field-darkly.pmanim"));
        assert(darklyArt.pose("S_1") && darklyArt.pose("T_1") && darklyArt.pose("Z_1"));
        Game darklyGame;Progress darklySaved;darklySaved.room=2;darklySaved.originalStory=8;darklySaved.position={-3.35f,-3.65f};
        darklyGame.restore(darklySaved);for(int n=0;n<360;++n)darklyGame.tick({});
        Input towardDarkly;towardDarkly.x=1;for(int n=0;n<40;++n)darklyGame.tick(towardDarkly);
        assert(darklyGame.fieldNpcContact()==darkly.key && darklyGame.nearbyFieldActor() && darklyGame.nearbyFieldActor()->key==darkly.key);
        Input darklyPress;darklyPress.confirm=true;darklyGame.tick(darklyPress);darklyGame.tick({});
        assert(darklyGame.fieldActorTalk.message.id()=="gor_02_034_00" && darklyGame.fieldActorTalk.message.active());
        assert(darklyGame.fieldActorTalk.message.rawText()==originals.find("gor_02_034_00") && darklyGame.progress.originalStory==8);
        for(int n=0;n<5000 && darklyGame.fieldActorTalk.message.active();++n){darklyGame.tick(darklyPress);assert(darklyGame.progress.originalStory==8 && !darklyGame.saveRequested);}
        assert(!darklyGame.fieldActorTalk.message.active() && darklyGame.scene==Scene::Explore);
        const int32_t cases[]={-1,55,56,111,112,163,164,214,215,260,261,338,339,372,373,400,401,500};
        for(int32_t story:cases) {
            const auto key=fieldActorTalkKey(chu,story);assert(originals.find(key.c_str()));
            FieldActorTalk talk;assert(talk.begin(chu,story,harborMessageFont(),0));
            assert(talk.message.id()==key && talk.message.rawText()==originals.find(key.c_str()) && talk.actor.key==chu.key);
        }
        Game game;Progress saved;saved.room=2;saved.originalStory=8;saved.position={1.2f,-4.f};game.restore(saved);
        for(int n=0;n<360;++n)game.tick({}); // Native checkpoint warmup and NPC publication.
        assert(game.nearbyFieldActor());Input press;press.confirm=true;game.tick(press);
        assert(!game.fieldActorTalk.message.active() && !game.airborne());
        game.tick({}); // Next NPC invocation owns fbatTalkMode event entry.
        assert(game.fieldActorTalk.message.active() && game.fieldActorTalk.message.id()=="gor_02_031_00" && !game.airborne());
        assert(game.fieldActorTalk.actor.key==chu.key && game.progress.originalStory==8);
        assert(game.fieldActorTalk.actor.yaw==270 && chu.yaw==270); // Talk facing is source world yaw.
        for(int t=0;t<5000 && game.fieldActorTalk.message.active();++t){game.tick(press);assert(game.progress.originalStory==8 && !game.saveRequested);}
        assert(!game.fieldActorTalk.message.active() && game.scene==Scene::Explore);
        Game walking;walking.restore(saved);for(int n=0;n<360;++n)walking.tick({});
        Input toward;toward.x=-1; // Settled original viewYaw180 maps left input to world+X.
        for(int n=0;n<40;++n)walking.tick(toward);
        assert(walking.fieldNpcContact()==chu.key && walking.progress.position.x>1.21f && walking.progress.position.x<1.23f);
        assert(!walking.airborne() && walking.progress.originalStory==8);
        Game scripted;auto professorSaved=saved;professorSaved.position={0,-4};scripted.restore(professorSaved);for(int n=0;n<360;++n)scripted.tick({});
        scripted.professorStory.triggered=true;scripted.professorStory.finished=true;
        auto& professor=scripted.professorStory.actors[2];professor.npcActive=true;
        professor.position={0,0,-400};professor.placementGeneration=1;
        professor.flags=professor.flagMask=0x40000020;scripted.tick({});assert(!scripted.fieldNpcContact());
        professor.flags=0;scripted.tick({});assert(scripted.fieldNpcContact()==findActor("professor").key);
        professor.hidden=true;professor.flags=0x80;professor.flagMask|=0x80;
        scripted.tick({});assert(scripted.fieldNpcContact()==findActor("professor").key); // Draw hiding leaves source collision eligible.
        professor.npcActive=false;scripted.tick({});assert(scripted.fieldNpcContact()!=findActor("professor").key);
        Game history;auto away=saved;away.position={0,-4};history.restore(away);
        for(int n=0;n<360;++n)history.tick({});
        history.professorStory.triggered=true;history.professorStory.finished=true;
        auto& moving=history.professorStory.actors[2];moving.npcActive=true;
        moving.position={100,0,-400};moving.placementGeneration=1;history.tick({});
        moving.position[0]=10;history.tick({}); // Continuous movement keeps the previous root.
        assert(moving.position[0]==100 && !history.fieldNpcContact());
        moving.position[0]=10;++moving.placementGeneration;history.tick({}); // Explicit source placement seeds history.
        assert(moving.position[0]==10 && history.fieldNpcContact()==findActor("professor").key);
        history.progress.position={0,-4};moving.position[0]=100;++moving.placementGeneration;
        moving.flagMask|=3;moving.flags=2;history.tick({});
        moving.position[0]=200;history.tick({}); // Initial flags gate must not publish the end snapshot.
        moving.position[0]=10;moving.flags=3;history.tick({});
        assert(moving.position[0]==100 && !history.fieldNpcContact());
        Game turned;turned.restore(saved);for(int n=0;n<360;++n)turned.tick({});assert(turned.nearbyFieldActor());
        turned.marioPresentation.displayFlags|=1;assert(!turned.nearbyFieldActor());
        turned.marioPresentation.displayFlags&=~1u;
        turned.fieldIdle.mainFlags|=0x01000000;assert(!turned.nearbyFieldActor());turned.fieldIdle.mainFlags&=~0x01000000;
        turned.fieldKeysOff=true;assert(!turned.nearbyFieldActor());turned.fieldKeysOff=false;
        turned.marioPresentation.display=turned.marioPresentation.target=180;
        assert(!turned.nearbyFieldActor());turned.tick(press);
        assert(!turned.airborne() && !turned.fieldActorTalk.message.active()); // Previous NPC candidate still owns this Mario query.
        turned.tick(press);assert(turned.airborne() && !turned.fieldActorTalk.message.active());
        Game outOfRange;saved.position={1.f,-3.7f};outOfRange.restore(saved);outOfRange.tick({});
        assert(!outOfRange.nearbyFieldActor());outOfRange.tick(press);assert(outOfRange.airborne());
        char wallPath[]="/tmp/pm-npc-talk-wall-XXXXXX";const int fd=mkstemp(wallPath);assert(fd>=0);
        FILE* file=fdopen(fd,"wb");assert(file);
        const uint32_t count=0xc0000001;const Rect wall{1.35f,-4.2f,1.35f,-3.8f};
        const Vec2 levels{-1,1};const GroundContact contact{4,99};
        assert(std::fwrite(&count,4,1,file)==1 && std::fwrite(&wall,sizeof(wall),1,file)==1 &&
            std::fwrite(&levels,sizeof(levels),1,file)==1 && std::fwrite(&contact,sizeof(contact),1,file)==1);
        assert(std::fclose(file)==0);clearCollision();assert(loadGround("romfs/gor_02.ground") && loadCollision(wallPath));unlink(wallPath);
        assert(!fieldWallRay({120,27.75f,-400},{1,0,0},30).token); // Wall callback rejects attributes4.
        Game blocked;saved.position={1.2f,-4.f};blocked.restore(saved);for(int n=0;n<360;++n)blocked.tick({});
        assert(!blocked.nearbyFieldActor()); // Null talk callback retains this obstructing polygon.
        assert(loadCollision("romfs/gor_02.walls") && loadHead("romfs/gor_02.head"));
        saved.originalStory.reset();game.restore(saved);assert(!game.nearbyFieldActor());
        for(const auto& model:a.models()) {
            ActorAnimationPack animations;assert(animations.load(("romfs/"+model.asset+".pmanim").c_str()));
            for(const auto& actor:a.actors())if(a.models()[actor.model].asset==model.asset) {
                const auto* pose=animations.pose(actor.pose.c_str());assert(pose);ActorAnimationSample sample;
                for(float frame:{0.f,1.f,60.f,300.f})assert(pose->evaluate(frame,sample) && !sample.vertices.empty() && sample.vertices.size()<12000);
                if(actor.kind==0)assert(!sample.batches.empty() && !sample.colors.empty() && sample.batches[0].cull==3 && sample.batches[0].pass==2);
            }
        }
    }
    std::cout<<"field actor gates, atomic rejection, model namespaces, original animation and talk branch bindings passed\n";
}
