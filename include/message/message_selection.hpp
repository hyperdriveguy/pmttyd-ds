#pragma once
#include "message_playback.hpp"
namespace ttyd {
struct MessageSelectionInput {uint32_t buttons=0,directions=0,buttonRepeat=0,directionRepeat=0;};
class MessageSelection {
public:
    MessagePlayback message;
    int8_t current=0,cancel=0,count=0;
    uint16_t state=4,opacity=0;
    mutable float cursorY=0;
    std::vector<uint32_t> sounds;
    bool open(const char*,const char*,const MessageFont&,uint64_t);
    void tick(const MessageSelectionInput&);
    bool active()const{return state!=4;}
    bool complete()const{return state==4;}
};
}
