#pragma once
#include <cstdint>
// Retained low16 flag word and per-call fade cursor. Clearing bits invokes
// no fade. A held set resumes its original publication/children even if the
// caller supplies a different mask; use a fresh cursor for a distinct call.
namespace ttyd {
enum class SoundFlagFade {Bgm,Environment};
struct SoundFlagServices {
    void* user=nullptr;
    bool (*fade)(void*,SoundFlagFade,uint32_t group,int32_t duration,int32_t flags)=nullptr;
};
struct SoundFlagCursor {bool active=false;uint8_t stage=0;uint32_t pending=0;};
// psnd803df6e8+56. Zero is the explicit psndInit halfword store only.
class SourceSoundFlags {
public:
    uint16_t value() const{return value_;}
    void clear(uint32_t mask){value_=uint16_t(uint32_t(value_)&~mask);}
    bool set(uint32_t mask,const SoundFlagServices&,SoundFlagCursor&);
private:
    uint16_t value_=0;
};
}
