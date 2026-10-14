#pragma once
#include "opening_house_sounds.hpp"
#include <3ds.h>
#include <cstddef>
namespace ttyd {
// Native primary-sample presentation, separate from source MusyX/ENV services.
// Channels10..17 avoid both BGMstreams0..1 and the primary eight clips2..9.
class OpeningHousePcmPlayer {
public:
    bool update(const OpeningHouseSounds*,bool enabled);
    void shutdown();
    uint32_t queued=0,failures=0,unboundRequests=0;
private:
    struct Clip {int16_t* data=nullptr;uint32_t rate=0,frames=0;};
    std::array<Clip,4> clips{};
    std::array<ndspWaveBuf,8> voices{};
    const OpeningHouseSounds* owner=nullptr;
    uint64_t epoch=0;
    size_t cursor=0;
    bool ready=false;
    bool load();
    bool play(const OpeningHouseAudioRequest&);
    void clear();
};
}
