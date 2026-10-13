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
    }
    if(argc>1) {
        assert(a.load("romfs/field-actors.pmnpc"));assert(a.actors().size()>=3 && a.models().size()>=2);
        assert(a.models()[0].asset=="field-chu" && a.models()[1].asset=="professor");
        const auto& chu=a.actors()[0];assert(chu.room==2 && chu.pose=="CHU_S_1");
        assert(chu.npcWidth==30 && chu.npcHeight==30 && chu.talkPose=="CHU_T_1" && a.models()[0].drawMask==1);
        assert((chu.position==std::array<float,3>{150,0,-400}) && chu.yaw==270);
        assert(chu.visible(2,8,0) && chu.visible(2,55,2) && !chu.visible(2,56,0) && !chu.visible(2,std::nullopt,0));
        const auto& saveBlock=a.actors()[3];assert(saveBlock.kind==1 && saveBlock.scale==10 && saveBlock.pose=="S_1");
        assert((saveBlock.position==std::array<float,3>{185,60,-55}));
        assert(saveBlock.visible(0,0,0) && saveBlock.visible(0,400,0) && !saveBlock.visible(0,341,0) && !saveBlock.visible(0,8,0,0x20));
        const auto scaled=ActorRootTransform({0,0,0},0,false,10.f*.01f).point({10,10,10,0,0});
        for(float v:scaled)assert(std::fabs(v-1.f)<.000001f);
        assert(loadCollision("romfs/gor_02.walls") && loadGround("romfs/gor_02.ground") && loadHead("romfs/gor_02.head"));
        assert(loadCameraRoadAssets("romfs"));
        float height=0;assert(groundAt({1.f,-3.7f},0,height) && walkable({1.f,-3.7f},height));
        assert(loadFieldActorCatalog("romfs") && loadHarborStoryAssets("romfs"));
        MessageAssets originals;assert(originals.loadRoom("romfs/messages/gor_02.txt"));
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
