#include "platform.hpp"
namespace ttyd {
void Audio::updateFrontend(const OpeningSequence& opening,bool enabled){
    if(!ready)return;
    if(!enabled){updateFrontend(-1,false);return;}
    if(opening.music()<0){updateFrontend(-1,true);return;}
    const auto state=opening.mapBgm.snapshot();frontendMusic(state.tracks,state.volumes);
}
}
