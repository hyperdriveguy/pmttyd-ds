#pragma once
#include <cstdint>
#include <vector>

namespace ttyd {
struct EventScriptSegment {uint32_t address,size,offset;};
class EventScriptAssets {
public:
    // PMEVT001 is a port asset container,not an original VM format. A rejected
    // load retains the last valid pack;original addresses never become host pointers.
    bool load(const char* path);
    bool readByte(uint32_t originalAddress,uint8_t& value) const;
    bool readWord(uint32_t originalAddress,uint32_t& value) const;
    const std::vector<EventScriptSegment>& segments() const {return segments_;}
private:
    const EventScriptSegment* find(uint32_t address,uint32_t bytes) const;
    std::vector<uint8_t> data_;
    std::vector<EventScriptSegment> segments_;
};
}
