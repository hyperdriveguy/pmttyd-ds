#include "opening_sequence.hpp"
#include "camera_road_view.hpp"
namespace ttyd {
bool OpeningSequence::load(const char* base) {
    if(!base || !loadOpeningHouseSoundAssets(base))return false;
    const std::string root=base;MessageResource pendingGlobal;std::array<MessageResource,3> pendingRooms;MessageFont pendingFont;
    const char* names[]={"aaa_00","gor_10","gor_00"};
    if(!pendingGlobal.load((root+"/messages/global.txt").c_str()) || !pendingFont.load((root+"/font.pmfont").c_str()))return false;
    for(unsigned i=0;i<3;++i)if(!pendingRooms[i].load((root+"/messages/"+names[i]+".txt").c_str()))return false;
    const char* ids[]={"pro_00","pro_01","pro_02","pro_03","pro_05","pro_04","mac_0_012","mac_0_013","mac_0_013_01","mac_0_014","mac_0_015"};
    for(unsigned i=0;i<11;++i){MessagePlayback probe;const unsigned room=i<5?0:i<9?1:2;if(!probe.open(ids[i],pendingRooms[room].find(ids[i]),pendingFont,0))return false;}
    CameraRoadAsset pendingCamera;CameraRoadWork road;
    if(!pendingCamera.load((root+"/opening-gor_10.camera").c_str()) || !road.update(pendingCamera,{0,0,0},{0,0,0},0,0))return false;
    const auto frame=cameraRoadFrame(road.current);if(!frame || !road.current.active)return false;
    wakeEye={frame->eye.x*100,frame->eye.y*100,frame->eye.z*100};wakeAt={frame->at.x*100,frame->at.y*100,frame->at.z*100};wakeCamera=std::move(pendingCamera);
    global=std::move(pendingGlobal);rooms=std::move(pendingRooms);font=std::move(pendingFont);ready=true;return true;
}
const char* OpeningSequence::find(const char* id) const {
    const unsigned room=part==OpeningPart::Letter || part==OpeningPart::Wake?1:part==OpeningPart::Arrival?2:0;
    if(rooms[room].ambiguous(id))return nullptr;
    if(const char* raw=rooms[room].find(id))return raw;
    return global.find(id);
}
bool OpeningSequence::print(const char* id,uint32_t who){speaker=who;const char* raw=find(id);if(!raw || !message.open(id,raw,font,now)){failure=std::string(raw?"Unsupported original message: ":"Missing original message: ")+id;failed=true;return false;}message.requireTerminalInput=true;return true;}
bool OpeningSequence::enter(OpeningPart p,uint64_t time){
    part=p;index=0;now=time;waiting=false;message={};enteredAt=time;sceneAdmitted=false;sceneReadyNotification=false;sceneFailure.clear();
    speaker=0;
    if(p==OpeningPart::House)house.begin(time*60/1000);
    if(p==OpeningPart::Prologue){heading.begin(time);headingDisplay.begin(time*60/1000);}
    if(p==OpeningPart::Wake)wake.begin(time);
    if(p==OpeningPart::Arrival)harbor.begin(time*60/1000,globalWork8);
    if(p==OpeningPart::Voyage)voyage.begin(time*60/1000);
    if(p==OpeningPart::MapDisplay)mapDisplay.begin(time*60/1000);
    if(p==OpeningPart::Voyage)mapTransition.enter();
    if(p==OpeningPart::HouseReturn){
        house.interior=true;house.eye={-376,154,65};house.at={-63,75,-247};
        house.actors[0]={{-85,30,-310},90,"M_S_1",nullptr,time*60/1000,true,false};
        house.actors[1]={{-52,30,-275},270,"L_8","L_9",time*60/1000,true,false};
        house.actors[2].visible=false;
    }
    if(p==OpeningPart::Complete)return true;
    if(!ready){failure="Original opening resources unavailable";failed=true;return false;}
    // Native presentation pacing; source service/parallel choreography is not
    // replaced with a claim that these are original measured durations.
    deadline=time+(p==OpeningPart::House?0:p==OpeningPart::Prologue || p==OpeningPart::Wake?0:p==OpeningPart::MapDisplay?0:1000);
    return true;
}
void OpeningSequence::begin(uint64_t time){failed=false;failure.clear();story=0;globalWork8=0;mapBgm.begin(time*60/1000);mapTransition={};enter(OpeningPart::House,time);}
void OpeningSequence::tick(uint64_t time,uint32_t keys){
    if(failed || complete() || time<now)return;
    now=time;mapBgm.advance(now*60/1000);mapTransition.advance(now*60/1000);
    if(sceneServicesBound){
        if(!sceneFailure.empty()){failed=true;failure=sceneFailure;return;}
        if(!sceneAdmitted){
            if(!sceneReadyNotification)return;
            sceneReadyNotification=false;sceneAdmitted=true;deadline+=now-enteredAt;enteredAt=now;
            if(part==OpeningPart::House)house.begin(now*60/1000);
            if(part==OpeningPart::Voyage)voyage.begin(now*60/1000);
            if(part==OpeningPart::Arrival)harbor.begin(now*60/1000,globalWork8);
            if(part==OpeningPart::Wake)wake.begin(now);
        }
    }
    if(part==OpeningPart::House)house.advance(now*60/1000);
    if(part==OpeningPart::Prologue){heading.tick(now,keys);headingDisplay.advance(now*60/1000);}
    if(part==OpeningPart::Wake)wake.tick(now);
    if(part==OpeningPart::Voyage){voyage.advance(now*60/1000,keys);if(voyage.headingRequested)requestBgmOff(6000);}
    if(part==OpeningPart::Arrival){harbor.advance(now*60/1000);globalWork8=harbor.work8;if(harbor.storyPublished)story=3;}
    if(part==OpeningPart::MapDisplay){
        const auto previous=mapDisplay.poseStage;mapDisplay.tick(now*60/1000);
        if(previous!=mapDisplay.poseStage){
            house.actors[1].pose=mapDisplay.poseStage==1?"I_1":"S_1";house.actors[1].talkPose=nullptr;house.actors[1].poseTick=now*60/1000;
            if(mapDisplay.poseStage==2){house.actors[0].pose="M_L_3";house.actors[0].poseTick=now*60/1000;}
        }
    }
    const int wantedMusic=music();if(wantedMusic>=0)mapBgm.on(wantedMusic);
    if(part==OpeningPart::MapDisplay && mapDisplay.musicOffRequested)mapBgm.off();
    if(message.active()){message.tick(now,keys);if(message.active())return;deadline=now;if(part==OpeningPart::House){house.messageComplete(index-1);house.advance(now*60/1000);}
        if(part==OpeningPart::Wake)wake.messageComplete(index-1);
        if(part==OpeningPart::Arrival)harbor.messageComplete(index-1);
    }
    if(now<deadline)return;
    switch(part){
    case OpeningPart::House:{const char* ids[]={"pro_00","pro_01","pro_02","pro_03"};if(index<4 && house.messageReady(index)){print(ids[index],index==0?0x468:0x474);++index;}else if(index==4 && house.complete)enter(OpeningPart::Letter,time);break;}
    case OpeningPart::Letter:if(index==0){print("pro_04",0);++index;}else enter(OpeningPart::HouseReturn,time);break;
    case OpeningPart::HouseReturn:if(index==0){print("pro_05",0x474);++index;}else enter(OpeningPart::MapDisplay,time);break;
    case OpeningPart::MapDisplay:if(mapDisplay.complete){if(!mapTransition.requested)mapTransition.begin();if(mapTransition.exitComplete){story=2;enter(OpeningPart::Voyage,time);}}break;
    case OpeningPart::Voyage:if(voyage.headingReady)enter(OpeningPart::Prologue,time);break;
    case OpeningPart::Prologue:if(heading.complete)enter(OpeningPart::Wake,time);break;
    case OpeningPart::Wake:{const char* ids[]={"mac_0_012","mac_0_013","mac_0_013_01"};if(index<3 && wake.messageReady(index)){print(ids[index],0);++index;}else if(index==3 && wake.complete)enter(OpeningPart::Arrival,time);break;}
    case OpeningPart::Arrival:{const char* ids[]={"mac_0_014","mac_0_015"};if(index<2 && harbor.messageReady(index)){print(ids[index],0);++index;}else if(index==2 && harbor.storyPublished){story=3;enter(OpeningPart::Complete,time);}break;}
    case OpeningPart::Complete:break;
    }
}
const char* OpeningSequence::map() const {
    switch(part){case OpeningPart::House:case OpeningPart::HouseReturn:case OpeningPart::MapDisplay:return "aaa_00";case OpeningPart::Letter:case OpeningPart::Wake:return "gor_10";case OpeningPart::Voyage:return "muj_20";case OpeningPart::Arrival:return "gor_00";default:return nullptr;}
}
int OpeningSequence::music() const {
    if(part==OpeningPart::House || part==OpeningPart::Letter || part==OpeningPart::HouseReturn || part==OpeningPart::MapDisplay)return house.musicStarted?7:-1;
    if(part==OpeningPart::Voyage)return voyage.music();
    return -1;
}
}
