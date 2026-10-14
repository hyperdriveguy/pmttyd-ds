#include "battle_audience_sound.hpp"
#include <cassert>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    static constexpr uint32_t B=0x20000,A=B+0x2858,R=A+0x133fc+0x24;
    std::map<uint32_t,uint32_t> words;
    std::map<uint32_t,uint8_t> bytes;
    std::map<int32_t,uint8_t> voices;
    std::vector<uint32_t> keys;
    std::vector<int32_t> stopped;
    int32_t right=0,left=0,next=41;
    bool assigned=false,fail=false,provider=true;
    Fixture(){words[0x8041eb90]=B;}
    void crowd(unsigned count){for(unsigned i=0;i<count;++i)words[A+0x1bc+0x134*i]=1;}
    BattleAudienceSoundServices services() {
        BattleAudienceSoundServices s;s.actors.user=this;s.actors.memory.user=this;s.user=this;
        s.actors.memory.word=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->words[a];};
        s.actors.byte=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->bytes[a];};
        s.actors.memory.storeWord=[](void* p,uint32_t a,uint32_t v){static_cast<Fixture*>(p)->words[a]=v;};
        s.actors.memory.storeByte=[](void* p,uint32_t a,uint8_t v){static_cast<Fixture*>(p)->bytes[a]=v;};
        s.play=[](void* p,uint32_t key,bool assigned,int32_t right,int32_t left)->std::optional<int32_t>{auto& f=*static_cast<Fixture*>(p);
            f.keys.push_back(key);f.assigned=assigned;f.right=right;f.left=left;if(!f.provider)return std::nullopt;if(f.fail)return -1;
            const auto handle=f.next++;f.voices[handle]=73;return handle;};
        s.volume=[](void* p,int32_t handle)->std::optional<uint8_t>{auto& f=*static_cast<Fixture*>(p);assert(f.voices.count(handle));return f.voices[handle];};
        s.stop=[](void* p,int32_t handle){auto& f=*static_cast<Fixture*>(p);f.stopped.push_back(handle);f.voices.erase(handle);f.words[R]=0x81;return true;};return s;
    }
};
int main() {
    constexpr auto R=Fixture::R,A=Fixture::A;
    for(unsigned count:{1u,49u,50u,99u,100u,149u,150u,200u}) {
        Fixture f;f.crowd(count);auto s=f.services();assert(clapBattleAudienceSound(-1,0,s));
        const unsigned category=count<50?1:count<100?2:count<150?3:4;
        constexpr uint32_t keys[]{0x802fa5c4,0x802fa5dc,0x802fa5f4,0x802fa60c};
        assert(f.keys==std::vector<uint32_t>{keys[category-1]} && f.assigned && f.right+f.left==int32_t(count));
        assert(f.bytes[R+8]==category && f.words[R]==1 && f.words[R+4]==41 && f.words[R+0xc]==0xffffffff);
        assert(f.bytes[R+0x1c]==73 && f.bytes[R+0x1d]==73 && f.bytes[R+0x1e]==100);
        assert(stopBattleAudienceSound(1,s) && f.words[R]==0x80 && f.words[R+4]==41 && f.stopped==std::vector<int32_t>{41});
    }
    Fixture filtered;filtered.crowd(200);
    for(unsigned i=0;i<3;++i)filtered.bytes[A+0x1bc+0x134*i+0x19]=i==0?0xc:i==1?0xd:0x11;
    filtered.words[A+0x1bc+0x134*3]=0x101;filtered.words[A+0x1bc+0x134*4]=3;
    auto s=filtered.services();assert(battleAudienceHalfCount(true,s.actors)==100 && battleAudienceHalfCount(false,s.actors)==95);
    Fixture empty;empty.fail=true;empty.words[R+0xc]=99;empty.bytes[R+8]=4;s=empty.services();
    assert(clapBattleAudienceSound(-1,0,s) && !empty.assigned && empty.keys.size()==1 && empty.right==0 && empty.left==0);
    assert(empty.words[R+4]==0xffffffff && empty.words[R]==0 && empty.words[R+0xc]==99 && empty.bytes[R+8]==4);
    assert(stopBattleAudienceSound(1,s) && empty.stopped.empty());
    Fixture absent;absent.provider=false;s=absent.services();assert(!clapBattleAudienceSound(-1,0,s) && absent.words[R+4]==0);
    Fixture ramp;s=ramp.services();
    for(int32_t slot=0;slot<24;++slot){const auto record=battleAudienceSoundRecord(slot,s.actors);ramp.words[record]=0x81;ramp.words[record+4]=7;ramp.bytes[record+0x1e]=83;}
    setAllBattleAudienceVolumes(256,30,s.actors);
    for(int32_t slot=0;slot<24;++slot){const auto record=battleAudienceSoundRecord(slot,s.actors);assert(ramp.words[record]==0x81 && ramp.words[record+4]==7 && ramp.bytes[record+0x1f]==83 && ramp.bytes[record+0x20]==0 && ramp.words[record+0x18]==30 && ramp.words[record+0x10]==0);}
}
