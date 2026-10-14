#include "battle_audience_callback.hpp"
#include "native_shadow_program.hpp"
#include <cassert>
#include <map>
#include <vector>
using namespace ttyd;
struct Fixture {
    uint32_t battle=0x10000;CameraState camera;DisplayRecord entry{4,1};
    std::map<uint32_t,uint32_t> words;std::map<uint32_t,uint8_t> bytes;
    unsigned baseReads=0,cameraReads=0,entryReads=0,flagReads=0;
    int holdStep=-1,holdEntry=-1;std::vector<BattleAudienceGXRequest> acknowledged;
    NativeShadowProgram program;
    Fixture(){program.afterDummy();}
    uint32_t actor(unsigned i)const{return battle+0x2858+0x1bc+0x134*i;}
    BattleAudienceCallbackServices services(){BattleAudienceCallbackServices s;s.user=this;s.actors.user=this;s.actors.memory.user=this;
        s.actors.memory.word=[](void* p,uint32_t addr){auto& f=*static_cast<Fixture*>(p);if(addr==0x8041eb90){++f.baseReads;return f.battle;}++f.flagReads;return f.words[addr];};
        s.actors.byte=[](void* p,uint32_t addr){return static_cast<Fixture*>(p)->bytes[addr];};
        s.camera=[](void* p,uint32_t index)->const CameraState*{auto& f=*static_cast<Fixture*>(p);assert(index==4);++f.cameraReads;return &f.camera;};
        s.currentRecord=[](void* p)->const DisplayRecord*{auto& f=*static_cast<Fixture*>(p);if(int(f.entryReads)==f.holdEntry)return nullptr;++f.entryReads;return &f.entry;};
        s.request=[](void* p,const BattleAudienceGXRequest& r){auto& f=*static_cast<Fixture*>(p);if(int(f.acknowledged.size())==f.holdStep)return false;
            ShadowGXRequest packet;packet.kind=static_cast<ShadowGX>(r.function);packet.values=r.arguments;
            const auto next=f.program.request(packet);assert(next);f.program=*next;f.acknowledged.push_back(r);return true;};return s;}
};
int main(){
    // Genuine all-rejected bank still executes all16 setters and200 entry reads.
    Fixture empty;BattleAudienceCallbackCursor c;assert(resumePackedBattleAudienceCallback(empty.services(),c));
    assert(!c.active && !c.pending && empty.acknowledged.size()==16 && empty.entryReads==200 && empty.baseReads==2 && empty.cameraReads==1 && empty.flagReads==0);
    assert(empty.program.audienceColor && empty.program.alpha==NativeShadowProgram::TextureRaster && !empty.program.material && !empty.program.texture && !empty.program.materialVertex);
    // All200 alpha0 entries pass class2's alpha test; flags are genuinely read.
    Fixture translucent;translucent.entry.drawClass=2;BattleAudienceCallbackCursor t;
    assert(resumePackedBattleAudienceCallback(translucent.services(),t) && translucent.flagReads==200);
    // Failed source setter retains owners/step; GetPtr0 is selected only later.
    Fixture held;held.holdStep=7;BattleAudienceCallbackCursor h;
    assert(!resumePackedBattleAudienceCallback(held.services(),h) && h.step==7 && held.baseReads==1 && held.cameraReads==1 && h.pending==0x802b6f78);
    const auto owner=h.owner;held.battle=0x50000;held.holdStep=-1;held.entry.drawClass=2;held.words[held.actor(199)]=5;
    assert(!resumePackedBattleAudienceCallback(held.services(),h) && h.stage==5 && h.index==199 && h.owner==owner && h.record==held.actor(199));
    assert(held.baseReads==2 && held.cameraReads==1 && held.entryReads==200 && held.acknowledged.size()==16 && h.pending==0x801a38f8);
    held.words[h.record]=0;assert(!resumePackedBattleAudienceCallback(held.services(),h) && held.entryReads==200); // Captured admission cannot be revoked to skip its body.
    // Current-entry hold retries only current slot; changed class remains fresh.
    Fixture entry;entry.holdEntry=47;BattleAudienceCallbackCursor e;
    assert(!resumePackedBattleAudienceCallback(entry.services(),e) && e.index==47 && e.pending==0x80010864);
    entry.holdEntry=-1;entry.entry.drawClass=2;entry.words[entry.actor(47)]=5;entry.bytes[entry.actor(47)+0x44]=254;
    assert(!resumePackedBattleAudienceCallback(entry.services(),e) && e.index==47 && entry.entryReads==48 && entry.flagReads==1 && entry.acknowledged.size()==16);
    // Source flags1+4 mandatory; class2 alpha255 rejects before flags read.
    Fixture skip;skip.entry.drawClass=2;skip.words[skip.actor(0)]=1;skip.words[skip.actor(1)]=4;skip.words[skip.actor(2)]=5;skip.bytes[skip.actor(2)+0x44]=255;
    BattleAudienceCallbackCursor k;assert(resumePackedBattleAudienceCallback(skip.services(),k) && skip.flagReads==199);
    Fixture opaque;opaque.words[opaque.actor(199)]=5;opaque.bytes[opaque.actor(199)+0x44]=255;BattleAudienceCallbackCursor o;
    assert(!resumePackedBattleAudienceCallback(opaque.services(),o) && o.index==199 && opaque.flagReads==1);
    // Dummy resets the audience equation, without inventing a channel material.
    empty.program.afterDummy();assert(!empty.program.audienceColor && empty.program.alpha==NativeShadowProgram::Register && !empty.program.material);
}
