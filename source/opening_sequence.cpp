#include "opening_sequence.hpp"
namespace ttyd {
bool OpeningSequence::load(const char* base) {
    if(!base)return false;
    const std::string root=base;MessageResource pendingGlobal;std::array<MessageResource,3> pendingRooms;MessageFont pendingFont;
    const char* names[]={"aaa_00","gor_10","gor_00"};
    if(!pendingGlobal.load((root+"/messages/global.txt").c_str()) || !pendingFont.load((root+"/font.pmfont").c_str()))return false;
    for(unsigned i=0;i<3;++i)if(!pendingRooms[i].load((root+"/messages/"+names[i]+".txt").c_str()))return false;
    const char* ids[]={"pro_00","pro_01","pro_02","pro_03","pro_05","pro_04","mac_0_012","mac_0_013","mac_0_013_01","mac_0_014","mac_0_015"};
    for(unsigned i=0;i<11;++i){MessagePlayback probe;const unsigned room=i<5?0:i<9?1:2;if(!probe.open(ids[i],pendingRooms[room].find(ids[i]),pendingFont,0))return false;}
    global=std::move(pendingGlobal);rooms=std::move(pendingRooms);font=std::move(pendingFont);ready=true;return true;
}
const char* OpeningSequence::find(const char* id) const {
    const unsigned room=part==OpeningPart::Letter || part==OpeningPart::Wake?1:part==OpeningPart::Arrival?2:0;
    if(rooms[room].ambiguous(id))return nullptr;
    if(const char* raw=rooms[room].find(id))return raw;
    return global.find(id);
}
bool OpeningSequence::print(const char* id,uint32_t who){speaker=who;const char* raw=find(id);if(!raw || !message.open(id,raw,font,now)){failure=std::string(raw?"Unsupported original message: ":"Missing original message: ")+id;failed=true;return false;}return true;}
bool OpeningSequence::enter(OpeningPart p,uint64_t time){
    part=p;index=0;now=time;waiting=false;message={};
    speaker=0;
    if(p==OpeningPart::House)house.begin(time*60/1000);
    if(p==OpeningPart::Complete)return true;
    if(!ready){failure="Original opening resources unavailable";failed=true;return false;}
    // Native presentation pacing; source service/parallel choreography is not
    // replaced with a claim that these are original measured durations.
    deadline=time+(p==OpeningPart::House?0:p==OpeningPart::Prologue?3000:p==OpeningPart::MapDisplay?0:1000);
    return true;
}
void OpeningSequence::begin(uint64_t time){failed=false;failure.clear();story=0;enter(OpeningPart::House,time);}
void OpeningSequence::tick(uint64_t time,uint32_t keys){
    if(failed || complete() || time<now)return;
    now=time;
    if(part==OpeningPart::House)house.advance(now*60/1000);
    if(message.active()){message.tick(now,keys);if(message.active())return;deadline=now;if(part==OpeningPart::House){house.messageComplete(index-1);house.advance(now*60/1000);}}
    if(now<deadline)return;
    switch(part){
    case OpeningPart::House:{const char* ids[]={"pro_00","pro_01","pro_02","pro_03"};if(index<4 && house.messageReady(index)){print(ids[index],index==0?0x468:0x474);++index;}else if(index==4 && house.complete)enter(OpeningPart::Letter,time);break;}
    case OpeningPart::Letter:if(index==0){print("pro_04",0);++index;}else enter(OpeningPart::HouseReturn,time);break;
    case OpeningPart::HouseReturn:if(index==0){print("pro_05",0x474);++index;}else enter(OpeningPart::MapDisplay,time);break;
    case OpeningPart::MapDisplay:if(++index==128){story=2;enter(OpeningPart::Voyage,time);}break;
    case OpeningPart::Voyage:if(!waiting){waiting=true;deadline=time+10000;}else if(keys&0x1100)enter(OpeningPart::Prologue,time);break;
    case OpeningPart::Prologue:enter(OpeningPart::Wake,time);break;
    case OpeningPart::Wake:{const char* ids[]={"mac_0_012","mac_0_013","mac_0_013_01"};if(index<3){print(ids[index],0);++index;}else enter(OpeningPart::Arrival,time);break;}
    case OpeningPart::Arrival:{const char* ids[]={"mac_0_014","mac_0_015"};if(index<2){print(ids[index],0);++index;}else {story=3;enter(OpeningPart::Complete,time);}break;}
    case OpeningPart::Complete:break;
    }
}
const char* OpeningSequence::map() const {
    switch(part){case OpeningPart::House:case OpeningPart::HouseReturn:return "aaa_00";case OpeningPart::Letter:case OpeningPart::Wake:return "gor_10";case OpeningPart::Voyage:return "muj_20";case OpeningPart::Arrival:return "gor_00";default:return nullptr;}
}
int OpeningSequence::music() const {
    if(part==OpeningPart::House)return house.musicStarted?7:-1;
    if(part==OpeningPart::Voyage)return waiting && now>=deadline+5000?9:8;
    return -1;
}
}
