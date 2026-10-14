#include "positional_sfx_start.hpp"
#include "battle_audience_sound_start.hpp"
#include "event_script_assets.hpp"
#include <cassert>
#include <cmath>
#include <cstring>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    PositionalSfxRegistry registry;PositionalSfxRow source;
    int32_t storyValue=11,rateValue=60;unsigned names=0,stories=0,stops=0;
    int matched=5;bool missingName=false,mutateStory=false,canStop=true;
    std::vector<PositionalSfxStopKind> stopped;
    Fixture(){source.bytes[4]=0x20;source.bytes[7]=1;source.bytes[12]=107;source.bytes[13]=64;source.bytes[16]=30;source.bytes[17]=2;source.bytes[18]=0xff;source.bytes[19]=0xfe;}
    PositionalSfxStartServices services(){
        PositionalSfxStartServices s;s.user=this;
        s.row=[](void* p,uint32_t index)->std::optional<PositionalSfxRow>{auto& f=*static_cast<Fixture*>(p);if(index>=4242)return {};auto row=f.source;
            uint32_t name=0x4000+index;for(unsigned i=0;i<4;++i)row.bytes[i]=name>>(24-8*i);if(index==0)row.bytes[4]=row.bytes[7]=0;return row;};
        s.compareName=[](void* p,uint32_t original,uint32_t)->std::optional<int32_t>{auto& f=*static_cast<Fixture*>(p);++f.names;if(f.missingName)return {};return original==uint32_t(0x4000+f.matched)?0:1;};
        s.story=[](void* p)->std::optional<int32_t>{auto& f=*static_cast<Fixture*>(p);++f.stories;if(f.mutateStory)f.registry.globalFlags=16;return f.storyValue;};
        s.rate=[](void* p)->std::optional<int32_t>{return static_cast<Fixture*>(p)->rateValue;};
        s.truncate=[](void*,float v)->std::optional<int32_t>{if(std::isfinite(v) && v>=-2147483648.f && v<2147483648.f)return int32_t(v);return {};};
        s.stop.user=this;s.stop.descriptor=[](void* p,uint32_t)->std::optional<uint32_t>{return static_cast<Fixture*>(p)->source.word(4);};
        s.stop.stop=[](void* p,PositionalSfxStopKind kind,int32_t engine){auto& f=*static_cast<Fixture*>(p);assert(engine==77);++f.stops;f.stopped.push_back(kind);return f.canStop;};return s;
    }
};
int main(){
    Fixture f;auto service=f.services();PositionalSfxStartRequest request;request.key=3;
    // Original last-free selection, low-byte defaults and untouched stale fields.
    f.registry.slots[39].position={7,8,9};f.registry.slots[39].engine=77;
    const auto first=startPositionalSfx(f.registry,request,service);assert(first==0x127);
    auto& slot=f.registry.slots[39];assert(slot.key==3 && slot.flags==0x101 && slot.volume==127 && slot.pan==64);
    assert(slot.rowVolume==107 && slot.priority==30 && slot.delay==2 && slot.variation==65534 && slot.pitch==32000 && slot.distance==16);
    assert((slot.position==std::array<float,3>{7,8,9}) && slot.engine==77 && f.names==0 && f.stories==1);
    request.position=std::array<float,3>{1,2,3};request.volume=0x1ff;request.pan=0x1ff;request.delay=1000;
    assert(startPositionalSfx(f.registry,request,service)==0x226);
    const auto& repeated=f.registry.slots[38];assert(repeated.flags==0x103 && repeated.delay==70 && repeated.retriggerDelay==40 && f.registry.retriggerOffset==8);
    assert(repeated.volume==127 && repeated.pan==64 && repeated.position==*request.position);
    f.registry.allocationGeneration=65535;request.key=4;request.positionMode=1;
    assert(startPositionalSfx(f.registry,request,service)==37 && f.registry.slots[37].generation==0 && (f.registry.slots[37].flags&4));
    // Signed key pointers scan actual ordinal order; miss selects real row0 content gate.
    Fixture named;request={};request.key=int32_t(0x80000000u);
    assert(startPositionalSfx(named.registry,request,named.services())==0x127 && named.names==6 && named.registry.slots[39].key==5);
    Fixture miss;miss.matched=-1;assert(startPositionalSfx(miss.registry,request,miss.services())==-1 && miss.names==4242 && miss.stories==0);
    Fixture unavailable;unavailable.missingName=true;assert(!startPositionalSfx(unavailable.registry,request,unavailable.services()) && unavailable.registry.slots[39].key==-1);
    request={};request.key=3;
    Fixture muted;muted.registry.globalFlags=4;assert(startPositionalSfx(muted.registry,request,muted.services())==-1 && muted.stories==0);
    request.muteOverride=1;assert(startPositionalSfx(muted.registry,request,muted.services())==0x127);
    request={};request.key=91;Fixture cooldown;assert(startPositionalSfx(cooldown.registry,request,cooldown.services())==0x127 && cooldown.registry.cooldown==6);
    assert(startPositionalSfx(cooldown.registry,request,cooldown.services())==-1 && cooldown.stories==1);
    Fixture early;request.key=869;early.storyValue=2;assert(startPositionalSfx(early.registry,request,early.services())==-1);
    // Raw key high bits and live global flag publication survive unchanged.
    Fixture flags;flags.mutateStory=true;flags.source.bytes[14]=1;request.key=0x04070003;
    assert(startPositionalSfx(flags.registry,request,flags.services())==0x127);
    assert(flags.registry.slots[39].key==request.key && flags.registry.slots[39].flags==0xfb1);
    // Equal priority never reclaims; strictly greater reclaims first eligible slot.
    Fixture full;request={};request.key=3;
    for(auto& occupied:full.registry.slots){occupied.key=4;occupied.priority=30;occupied.flags=1;occupied.engine=77;}
    assert(startPositionalSfx(full.registry,request,full.services())==-1);
    full.registry.slots[10].priority=31;
    assert(startPositionalSfx(full.registry,request,full.services())==0x10a && full.stops==0 && full.registry.slots[10].engine==-1);
    for(auto& occupied:full.registry.slots){occupied.key=4;occupied.priority=30;occupied.flags=0;occupied.engine=77;}
    full.registry.slots[0].priority=31;full.canStop=false;
    assert(!startPositionalSfx(full.registry,request,full.services()) && full.registry.slots[0].key==4 && full.stops==1);
    full.canStop=true;assert(startPositionalSfx(full.registry,request,full.services())==0x200 && full.stopped.back()==PositionalSfxStopKind::Effect);
    // Missing conversion retains the already-published prefix, not a successful handle.
    Fixture partial;auto bad=partial.services();bad.truncate=nullptr;
    assert(!startPositionalSfx(partial.registry,request,bad) && partial.registry.slots[39].key==3 && partial.registry.slots[39].flags==0x101 && partial.registry.allocationGeneration==0);
    // Original finite audience volume/unsigned X formulas, including zero still starting sound.
    EventScriptAssets original;assert(original.load("romfs/trio-attack-events.pmevt"));
    BattleAudienceSoundStartServices audience;audience.actors.memory.user=&original;
    audience.actors.memory.word=[](void* p,uint32_t a){uint32_t value;assert(static_cast<EventScriptAssets*>(p)->readWord(a,value));return value;};
    audience.truncate=service.truncate;
    const auto empty=packedBattleAudienceSoundInfo1(0,0,audience);assert(empty && empty->volume==0 && empty->position[0]==250 && empty->position[1]==0);
    assert(packedBattleAudienceSoundInfo1(8,0,audience)->volume==101);
    assert(packedBattleAudienceSoundInfo1(0,8,audience)->position[0]==6);
    assert(packedBattleAudienceSoundInfo1(4,4,audience)->position[0]==0);
    assert(packedBattleAudienceSoundInfo1(100,100,audience)->volume==126);
    std::vector<int> order;audience.user=&order;
    audience.on=[](void* p,uint32_t key,const std::array<float,3>& position)->std::optional<int32_t>{assert(key==42 && position[0]==250);static_cast<std::vector<int>*>(p)->push_back(1);return -1;};
    audience.volume=[](void* p,int32_t handle,uint8_t volume){assert(handle==-1 && volume==0);static_cast<std::vector<int>*>(p)->push_back(2);return true;};
    assert(startPackedBattleAudienceSound1(42,0,0,audience)==-1 && (order==std::vector<int>{1,2}));
    std::map<uint32_t,uint32_t> packed;packed[0x8041eb90]=0x10000;
    const uint32_t actor=0x10000+0x2858+0x1bc+3*0x134;
    for(unsigned i=0;i<3;++i){const float value=std::array<float,3>{13.5f,-4,21}[i];uint32_t bits;std::memcpy(&bits,&value,4);packed[actor+0x48+4*i]=bits;}
    Fixture individual;BattleAudienceSoundStartServices sound2;sound2.user=&individual;sound2.actors.memory.user=&packed;
    sound2.actors.memory.word=[](void* p,uint32_t a){return static_cast<std::map<uint32_t,uint32_t>*>(p)->at(a);};
    sound2.on=[](void* p,uint32_t key,const std::array<float,3>& position)->std::optional<int32_t>{auto& f=*static_cast<Fixture*>(p);PositionalSfxStartRequest request;request.key=int32_t(key);request.position=position;return startPositionalSfx(f.registry,request,f.services());};
    assert(startPackedBattleAudienceSound2(7,3,sound2)==0x127);
    assert((individual.registry.slots[39].position==std::array<float,3>{13.5f,-4,21}) && individual.registry.slots[39].volume==127);

}
