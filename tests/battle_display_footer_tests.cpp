#include "battle_display_footer.hpp"
#include <cassert>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    static constexpr uint32_t first=0x1000,script=0x200000;
    std::map<uint32_t,uint8_t> bytes;std::vector<uint32_t> stores,reads;uint32_t missing=0;
    uint32_t word(uint32_t a){uint32_t v=0;for(unsigned i=0;i<4;++i)v=v<<8|bytes[a+i];return v;}
    void word(uint32_t a,uint32_t v){for(unsigned i=0;i<4;++i)bytes[a+i]=uint8_t(v>>(24-8*i));}
    BattleAudienceAnimationServices services(){
        BattleAudienceAnimationServices s;s.user=this;s.actors.user=this;s.actors.memory.user=this;
        s.actors.memory.word=[](void* p,uint32_t a){auto& f=*static_cast<Fixture*>(p);f.reads.push_back(a);return f.word(a);};
        s.actors.memory.storeWord=[](void* p,uint32_t a,uint32_t v){auto& f=*static_cast<Fixture*>(p);f.stores.push_back(a);f.word(a,v);};
        s.actors.byte=[](void* p,uint32_t a){return static_cast<Fixture*>(p)->bytes[a];};
        s.actors.memory.storeByte=[](void* p,uint32_t a,uint8_t v){auto& f=*static_cast<Fixture*>(p);f.stores.push_back(a);f.bytes[a]=v;};
        s.scriptWord=[](void* p,uint32_t a)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);if(a==f.missing || !f.bytes.count(a))return {};return f.word(a);};return s;
    }
    void active(uint32_t r=first){word(r,5);word(r+0x24,script);word(r+0x28,script);}
    void opcode(int32_t op,uint32_t offset=0){word(script+offset,uint32_t(op));}
};
int main(){
    BattleCommandPalette palette;for(unsigned i=0;i<378;++i)palette.step();assert(palette.state==0 && palette.rgba==std::array<uint8_t,4>({255,192,192,207}));
    palette.state=0;palette.rgba[1]=255;palette.step();assert(palette.rgba[1]==0 && palette.state==0);
    palette.state=-1;const auto color=palette.rgba;palette.step();assert(palette.rgba==color);
    Fixture f;DisplayQueue queue;BattleFooterCursor command;palette={};f.word(0x1720,0x111131);
    assert(registerBattleCommandDisplay(0,palette,queue,command,f.services().actors));assert(queue.count()==8 && palette.rgba[1]==193);
    constexpr uint32_t callbacks[]{0x8011fde0,0x8012210c,0x80121ce8,0x80121638,0x80121074,0x80121330,0x80120a44,0x80120844};
    for(unsigned i=0;i<8;++i)assert(queue.record(i)->callback==callbacks[i] && queue.record(i)->camera==8);
    queue.reinitialize();for(unsigned i=0;i<1024;++i)queue.enter(0,0,0,0,0);
    palette={};command={};assert(!registerBattleCommandDisplay(0,palette,queue,command,f.services().actors));assert(palette.rgba[1]==193);
    queue.reinitialize();f.word(0x1720,0);assert(registerBattleCommandDisplay(0,palette,queue,command,f.services().actors) && queue.count()==1 && palette.rgba[1]==193);
    struct Window {DisplayQueue* queue;uint32_t expected;unsigned calls=0;bool visible=false;};
    Window window{&queue,4};BattleAudienceWindowServices ws;ws.user=&window;ws.window=[](void* p)->std::optional<bool>{auto& w=*static_cast<Window*>(p);assert(w.queue->count()==w.expected);++w.calls;return w.visible;};
    queue.reinitialize();BattleFooterCursor audience;assert(registerBattleAudienceDisplay(0,ws,queue,audience) && queue.count()==4 && window.calls==1);
    queue.reinitialize();window.expected=0;window.visible=true;assert(registerBattleAudienceDisplay(0x20000,ws,queue,audience) && queue.count()==1 && queue.record(0)->callback==0x801a35d0 && window.calls==2);
    BattleFooterCursor slot;assert(registerBattleBreakSlotDisplay(queue,slot) && queue.record(1)->callback==0x802036d4);
    BattleAudienceAnimationCursor cursor;f={};assert(processPackedBattleAudienceAnimations(Fixture::first,f.services(),cursor));assert(f.reads.size()==200 && f.stores.empty());
    f.active();f.opcode(-2);f.word(Fixture::script+4,0x123);f.word(Fixture::script+8,5);f.missing=Fixture::script+8;
    assert(!processPackedBattleAudienceAnimations(Fixture::first,f.services(),cursor));assert(f.bytes[Fixture::first+0x1a]==0x23 && f.stores.size()==1);
    f.missing=0;assert(processPackedBattleAudienceAnimations(Fixture::first,f.services(),cursor));assert(f.word(Fixture::first+0x2c)==5 && f.word(Fixture::first+0x28)==Fixture::script+12 && f.stores.size()==3);
    f.stores.clear();f.reads.clear();assert(processPackedBattleAudienceAnimations(Fixture::first,f.services(),cursor));assert(f.word(Fixture::first+0x2c)==4 && f.stores.size()==1 && f.reads[1]==Fixture::first+0x2c && f.reads[2]==Fixture::first+0x28);
    f={};f.active();f.opcode(-6);f.word(Fixture::script+4,2);f.opcode(-2,8);f.word(Fixture::script+12,7);f.word(Fixture::script+16,0);f.opcode(-7,20);f.opcode(-1,24);
    for(unsigned i=0;i<5;++i)assert(processPackedBattleAudienceAnimations(Fixture::first,f.services(),cursor));
    assert(f.bytes[Fixture::first+0x40]==0 && f.word(Fixture::first+0x34)==0 && f.word(Fixture::first+0x28)==Fixture::script+24);
    f={};f.active();f.opcode(-5);f.word(Fixture::script+4,99);f.opcode(-4,8);f.word(Fixture::script+12,99);
    assert(processPackedBattleAudienceAnimations(Fixture::first,f.services(),cursor) && f.word(Fixture::first+0x28)==Fixture::script+8);
    assert(processPackedBattleAudienceAnimations(Fixture::first,f.services(),cursor) && f.word(Fixture::first+0x28)==Fixture::script+16);
    f.opcode(-3,16);assert(processPackedBattleAudienceAnimations(Fixture::first,f.services(),cursor) && f.word(Fixture::first+0x28)==Fixture::script);
    f.opcode(-8);f.word(Fixture::script+4,0xffffffff);assert(processPackedBattleAudienceAnimations(Fixture::first,f.services(),cursor) && f.word(Fixture::first+0x2c)==0xffffffff);
    f={};f.active();f.opcode(-6);f.word(Fixture::script+4,2);f.opcode(-7,8);
    assert(processPackedBattleAudienceAnimations(Fixture::first,f.services(),cursor));
    f.missing=Fixture::script+4;assert(!processPackedBattleAudienceAnimations(Fixture::first,f.services(),cursor));
    assert(f.word(Fixture::first+0x34)==1 && f.word(Fixture::first+0x28)==Fixture::script+4);
    f.missing=0;assert(processPackedBattleAudienceAnimations(Fixture::first,f.services(),cursor));
    assert(f.word(Fixture::first+0x34)==1 && f.word(Fixture::first+0x28)==Fixture::script+8);
    // Frame stores affect later aliased script operands in original order.
    f={};f.active();const auto alias=Fixture::first+0x12;f.word(Fixture::first+0x28,alias);f.word(alias,uint32_t(-2));f.word(alias+4,0x56);f.word(alias+8,0x01020304);
    assert(processPackedBattleAudienceAnimations(Fixture::first,f.services(),cursor) && f.word(Fixture::first+0x2c)==0x56020304);
}
