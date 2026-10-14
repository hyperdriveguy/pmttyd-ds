#include "platform.hpp"
namespace ttyd {
bool Audio::updateOpeningHousePcm(const OpeningHouseSounds* sounds,bool enabled){return ready && openingHousePcm.update(sounds,enabled);}
}
