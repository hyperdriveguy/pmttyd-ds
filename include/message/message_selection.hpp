#pragma once
#include "message_playback.hpp"
namespace ttyd {
// Snapshot words, not repeat generators: trigger masks8/4 and dir1000/2000
// navigate; buttons200/100 cancel/confirm. Producer owns physical mapping/cadence.
struct MessageSelectionInput {uint32_t buttons=0,directions=0,buttonRepeat=0,directionRepeat=0;};
class MessageSelection {
public:
    MessagePlayback message;
    int8_t current=0,cancel=0,count=0;
    // Selection states: 4 complete, 5 opening, 1 input, 7 closing. These
    // differ from MessagePlayback's state meanings; opacity is a 0..255 envelope.
    uint16_t state=4,opacity=0;
    mutable float cursorY=0;
    std::vector<uint32_t> sounds;
    bool open(const char*,const char*,const MessageFont&,uint64_t);
    void tick(const MessageSelectionInput&);
    bool active()const{return state!=4;}
    bool complete()const{return state==4;}
};
}
