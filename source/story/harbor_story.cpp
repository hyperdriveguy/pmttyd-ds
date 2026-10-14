#include "harbor_story.hpp"
#include <algorithm>
#include <cmath>
#include <cstdio>
namespace ttyd {
namespace {MessageAssets messages;MessageFont font;bool ready=false;}
bool loadHarborStoryAssets(const char* root) {
    if(!root)return false;
    const std::string base=root;
    MessageAssets m;MessageFont f;
    if(!m.loadGlobal((base+"/messages/global.txt").c_str()) || !m.loadRoom((base+"/messages/gor_00.txt").c_str()) || !f.load((base+"/font.pmfont").c_str()))return false;
    for(int i=16;i<=21;++i){char key[20];std::snprintf(key,sizeof(key),"mac_0_%03d",i);MessagePlayback p;if(!p.open(key,m.find(key),f,0))return false;}
    messages=std::move(m);font=std::move(f);ready=true;return true;
}
const MessageFont& harborMessageFont(){return font;}
void HarborStory::reset(bool available) {
    *this=HarborStory{};enabled=available;
    actors={{{0x14b28,175,15,-300},{0x14b40,225,15,-300},{0x14b5c,255,15,-330},{0x14b6c,280,15,-330}}};
    actors[0].display=180;actors[0].yaw=90;
}
bool HarborStory::print(int id,unsigned actor) {
    char key[20];std::snprintf(key,sizeof(key),"mac_0_%03d",id);speaker=actors[actor].key;
    speakerRoot={actors[actor].x,actors[actor].y,actors[actor].z};
    if(!message.open(key,messages.find(key),font,now)){failed=true;return false;}return true;
}
bool HarborStory::begin(uint64_t time) {
    if(!ready || !enabled || triggered)return false;
    triggered=true;now=time;stage=1;return print(16,0);
}
void HarborStory::move(unsigned actor,int delta,float speed,unsigned delay) {
    auto& a=actors[actor];a.pending=true;a.moveAt=now+delay;a.displacement=delta;a.speed=speed;
}
void HarborStory::advanceActors(uint64_t delta) {
    for(auto& a:actors) {
        uint64_t travel=delta;
        if(a.pending && now>=a.moveAt) {
            // Original get_position float encoding then integer ADD(LW0,-N).
            a.targetX=std::trunc(std::trunc(a.x*1024.f)/1024.f)-a.displacement;
            a.display=a.targetX<a.x?0:180;a.yaw=a.targetX<a.x?270:90;
            a.pending=false;a.moving=true;
            travel=std::min(delta,now-a.moveAt);
        }
        if(a.moving) {
            // Temporary NPC driver adapter: source fifth move parameter treated
            // as engine units/second. Original NPC stepping/collision/poses are
            // still an open inbox dependency; never label this measured timing.
            const float distance=a.speed*float(travel)/1000.f;
            const float diff=a.targetX-a.x;
            if(std::fabs(diff)<=distance){a.x=a.targetX;a.moving=false;}
            else a.x+=std::copysign(distance,diff);
        }
    }
}
void HarborStory::preparePresentation(const NpcAngleTable& angles,const NpcCameraService& camera,const MessagePlayback* text,uint32_t who,uint64_t time) {
    for(unsigned i=0;i<actors.size();++i) {
        auto& a=actors[i];
        const float threshold=i==0?20:50; // Original tribe +3c: Goombella20, Crump/X-Naut50.
        const char* base=i==0?(a.actualSpeed<.10000000149011612f?"PKR_S_1":a.actualSpeed<threshold?"PKR_W_1":"PKR_R_1"):
            a.actualSpeed<.10000000149011612f?"S_1":a.actualSpeed<threshold?"W_1":"R_1";
        a.poseClock.update(base,i==0?"PKR_T_1":"T_1",text && who==a.key && (a.flags&0x200)?text:nullptr,time);
        NpcMatrixInput input;input.root={a.x,a.y,a.z};input.yaw=a.yaw;input.flags=a.flags;
        a.presentationReady=a.presentation.prepare(input,angles,camera);
    }
}
void HarborStory::tick(uint64_t time,uint32_t keys) {
    if(!locked() || failed || time<now)return;
    const uint64_t delta=time-now;now=time;advanceActors(delta);
    if(message.active()){message.tick(now,keys);if(message.active())return;}
    switch(stage) {
    case 1:move(0,100,180,0);move(1,100,120,300);move(2,100,120,350);move(3,100,120,350);deadline=now+1400;stage=2;break;
    case 2:if(now>=deadline){print(17,1);stage=3;}break;
    case 3:print(18,0);stage=4;break;
    case 4:deadline=now+500;stage=5;break;
    case 5:if(now>=deadline){print(19,1);stage=6;}break;
    case 6:print(20,2);stage=7;break;
    case 7:deadline=now+500;stage=8;break;
    case 8:if(now>=deadline){deadline=now+1000;stage=9;}break;
    case 9:if(now>=deadline){move(1,20,120,0);move(2,20,120,100);move(3,20,120,150);deadline=now+50;stage=10;}break;
    case 10:if(now>=deadline){move(0,20,120,0);stage=11;}break;
    case 11:if(!actors[0].pending && !actors[0].moving){print(21,0);stage=12;}break;
    case 12:finished=true;stage=13;break;
    }
}
}
