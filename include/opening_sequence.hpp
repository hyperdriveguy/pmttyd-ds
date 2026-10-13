#pragma once
#include "message_assets.hpp"
#include "message_playback.hpp"
namespace ttyd {
enum class OpeningPart { House, Letter, HouseReturn, MapDisplay, Voyage, Prologue, Wake, Arrival, Complete };
// Sequencing adapter: retains original IDs/tokens and explicit source speakers.
// Scene choreography is provided separately; this does not run REL bytecode.
class OpeningSequence {
public:
    OpeningPart part=OpeningPart::House;
    MessagePlayback message;
    unsigned index=0;
    int32_t story=0;
    uint32_t speaker=0;
    bool failed=false;
    std::string failure;
    uint64_t now=0;
    bool load(const char* root);
    void begin(uint64_t time);
    void tick(uint64_t time,uint32_t keys);
    bool complete() const{return part==OpeningPart::Complete;}
    const char* map() const;
    int music() const;
private:
    MessageResource global;
    std::array<MessageResource,3> rooms;
    bool ready=false;
    const char* find(const char*) const;
    MessageFont font;
    bool waiting=false;
    uint64_t deadline=0;
    bool enter(OpeningPart,uint64_t);
    bool print(const char*,uint32_t);
};
}
