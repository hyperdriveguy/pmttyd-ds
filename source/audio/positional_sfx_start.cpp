#include "positional_sfx_start.hpp"
#include <cstring>
namespace ttyd {
uint32_t PositionalSfxRow::word(unsigned o)const{return uint32_t(bytes[o])<<24|uint32_t(bytes[o+1])<<16|uint32_t(bytes[o+2])<<8|bytes[o+3];}
uint16_t PositionalSfxRow::half(unsigned o)const{return uint16_t(bytes[o])<<8|bytes[o+1];}
// Allocation returns a PENDING logical handle, not an audible backend voice.
// nullopt means unavailable and can follow source-visible mutations; this is
// NOT a transaction/resumable cursor. Admit providers before entering rather
// than blindly replaying On after a late failure. -1 is an original rejection.
std::optional<int32_t> startPositionalSfx(PositionalSfxRegistry& registry,const PositionalSfxStartRequest& request,const PositionalSfxStartServices& s){
    if(!s.row)return {};
    uint32_t key=uint32_t(request.key);
    // Negative key is a guest name token: scan4242 names in order, miss ->row0.
    // Nonnegative key low13 selects row; upper bits remain request modifiers.
    if(request.key<0){
        if(!s.compareName)return {};uint32_t index=0;
        for(;index<4242;++index){
            const auto row=s.row(s.user,index);if(!row)return {};
            const auto comparison=s.compareName(s.user,row->word(0),key);if(!comparison)return {};
            if(*comparison==0)break;
        }
        key=index<4242?index:0;
    }
    const auto index=key&0x1fff;const auto row=s.row(s.user,index);if(!row)return {};
    if(row->word(4)==0)return -1;
    if((registry.globalFlags&4) && request.muteOverride==0)return -1;
    // Global mask4 mutes unless overridden. These five source SFX IDs share
    // a6-visit cooldown (decrement belongs to psndMain). Story<=2 rejects869;
    // cooldown publication precedes story admission and can survive a hold.
    if(index==91 || index==2250 || index==446 || index==447 || index==517){
        if(registry.cooldown)return -1;registry.cooldown=6;
    }
    if(!s.story)return {};const auto story=s.story(s.user);if(!story)return {};
    if(*story<=2 && index==869)return -1;
    // First scan retains LAST free slot. Same-key flag100 increments shared
    //16-bit retrigger offset by8 while preference remains0. If full, second
    // scan chooses first slot with priority greater than incoming row priority;
    // successful Off is required before reuse. This is not oldest-voice stealing.
    int selected=-1;uint16_t preference=0;
    for(unsigned i=0;i<registry.slots.size();++i){
        auto& slot=registry.slots[i];
        if(slot.key!=-1 && (uint32_t(slot.key)&0x1fff)==index && (slot.flags&0x100) && preference==0){
            registry.retriggerOffset=uint16_t(registry.retriggerOffset+8);preference=registry.retriggerOffset;
        }
        if(slot.key==-1)selected=int(i); // Original first scan retains LAST free slot.
    }
    if(selected<0){
        for(unsigned i=0;i<registry.slots.size();++i){
            auto& slot=registry.slots[i];
            if(slot.key==-1){selected=int(i);break;}
            if(slot.priority>row->bytes[16]){
                if(!registry.off(int32_t(i),s.stop))return {};
                selected=int(i);break;
            }
        }
    }
    if(selected<0)return -1;
    // Low byte255 means default volume127/default row pan, not clamp-to255.
    // Flags101 start pending/retrigger; global10->10, key04000000->20,
    // special IDs1304/1858->280, row14==1->200, key10000/20000/40000->400/800/80.
    // Preserve original numeric flags without inferring missing backend modes.
    int32_t volume=request.volume,pan=request.pan;
    if(uint8_t(volume)==255)volume=127;
    if(uint8_t(pan)==255)pan=row->bytes[13];
    auto& slot=registry.slots[unsigned(selected)];
    slot.flags=0x101;
    if(registry.globalFlags&0x10)slot.flags|=0x10;
    if(key&0x04000000)slot.flags|=0x20;
    if(index==1304 || index==1858)slot.flags|=0x280;
    if(row->bytes[14]==1)slot.flags|=0x200;
    if(key&0x10000)slot.flags|=0x400;
    if(key&0x20000)slot.flags|=0x800;
    if(key&0x40000)slot.flags|=0x80;
    std::memcpy(&slot.key,&key,4);
    slot.volume=uint8_t(volume);slot.rowVolume=row->bytes[12];slot.pan=uint8_t(pan);
    // Source delay converts U16 milliseconds through1000/live rate, adds row
    // delay and retrigger preference, then caller's truncate. Late unavailability
    // leaves already published slot flags/key/volume. RetriggerDelay=5*preference
    // narrows16 bits; pitch32000 is original default, not this voice's WAV rate.
    if(!s.rate || !s.truncate)return {};
    const auto rate=s.rate(s.user);if(!rate)return {};
    const float period=s.milliseconds/float(*rate);
    const float delay=float(uint16_t(request.delay))/period;
    const float preferred=float(preference)+delay;
    const float adjusted=float(row->bytes[17])+preferred;
    const auto converted=s.truncate(s.user,adjusted);if(!converted)return {};
    slot.delay=uint16_t(*converted);slot.retriggerDelay=uint16_t(5u*preference);
    slot.rowMode=row->bytes[15];slot.priority=row->bytes[16];slot.variation=row->half(18);
    if(request.position){slot.position=*request.position;slot.flags|=request.positionMode==0?2:4;}
    slot.distance=uint16_t(request.distance);slot.pitch=32000;
    // Global generation wraps16 bits; handle=(generation<<8)|slot. Position
    // flags2(mode0)/4(other) and distance U16 are retained for later psndMain.
    registry.allocationGeneration=uint16_t(registry.allocationGeneration+1);
    slot.generation=registry.allocationGeneration;
    registry.lifetimes_[unsigned(selected)]=std::make_shared<const uint8_t>(0);
    return int32_t(uint32_t(registry.allocationGeneration)<<8|unsigned(selected));
}
}
