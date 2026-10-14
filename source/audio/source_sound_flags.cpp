#include "source_sound_flags.hpp"
namespace ttyd {
bool SourceSoundFlags::set(uint32_t mask,const SoundFlagServices& s,SoundFlagCursor& c){
    if(!c.active){
        const uint32_t result=uint32_t(value_)|mask;value_=uint16_t(result);
        c={true,uint8_t(result&1?0:2),0};
    }
    for(;;)switch(c.stage){
    case 0:case 1:
        c.pending=0x800dabac;
        if(!s.fade || !s.fade(s.user,SoundFlagFade::Bgm,c.stage,1000,0))return false;
        ++c.stage;break;
    case 2:
        // Reload after both BGM callbacks: their writes can change the ENV gate.
        c.stage=value_&2?3:5;break;
    case 3:case 4:
        c.pending=0x800d7470;
        if(!s.fade || !s.fade(s.user,SoundFlagFade::Environment,c.stage-3,1000,0))return false;
        ++c.stage;break;
    case 5:c={};return true;
    default:return false;
    }
}
}
