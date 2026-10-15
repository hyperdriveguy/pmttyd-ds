#include "platform.hpp"
// House PCM owner manages its own update/voice lifetime. Returnfalse when
// NDSP startup is unavailable; the wrapper adds no successful fallback cue.
namespace ttyd {
bool Audio::updateOpeningHousePcm(const OpeningHouseSounds* sounds,bool enabled){return ready && openingHousePcm.update(sounds,enabled);}
}
