#pragma once
#include <array>
#include <cstdint>
#include <string>
#include <vector>
#include "message_window.hpp"
namespace ttyd {
struct FontGlyph {uint16_t x,y;uint8_t width,height,padding[2];};
class MessageFont {
public:
    bool load(const char*);
    std::array<uint16_t,256> mapping{};
    std::vector<FontGlyph> glyphs;
};
struct MessageRecord {
    uint32_t flags=0;uint16_t code=0;int16_t x=0,y=0;uint16_t extra=0;
    uint32_t schedule=0;float scale=1,value=1;
};
// Opening-message and ordinary placement/style domain; original bytes retained.
// Unsupported controls reject atomically until their native consumers exist.
class MessagePlayback {
public:
    bool open(const char* id,const char* text,const MessageFont&,uint64_t now);
    bool append(const char* text,const MessageFont&,uint64_t now);
    void tick(uint64_t now,uint32_t triggers);
    bool active() const{return state!=0;}
    bool waiting() const{return state==3;}
    bool complete() const{return state==0;}
    bool stopped() const{return state==2;} // Retained window after source once_stop.
    const std::string& id() const{return identity;}
    const std::string& rawText() const{return raw;}
    std::vector<MessageRecord> records;
    size_t cursor=0;
    int32_t scroll=0,scrollTarget=0;
    uint32_t elapsed=0,flags=3;
    uint16_t acceptedKey=0,wait=0,state=0;
    uint8_t alpha=255;
    uint32_t talkCountdown=0;
    bool talkLatched=false,talking=false; // General_Main requests, independent of window state.
    uint64_t serial=0; // Native draw-cache lifetime, independent of message ID.
    uint64_t openedAt=0;
    MessageWindowGeometry geometry;
    int windowType=0;
    int8_t initialChoice=0,cancelChoice=0,choiceCount=0;
    int16_t choiceWidth=0,choiceY=0;
private:
    std::string identity,raw;
    uint64_t lastTime=0,animationTime=0;
    int play(uint64_t,uint32_t);
};
static_assert(sizeof(MessageRecord)==24 && sizeof(FontGlyph)==8,"Native source-derived message record layout");
}
