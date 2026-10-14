#include "source_sound_flags.hpp"
#include "game.hpp"
#include <cassert>
#include <vector>
using namespace ttyd;
struct Fixture {
    SourceSoundFlags flags;
    struct Call {SoundFlagFade kind;unsigned group;};
    std::vector<Call> calls;
    unsigned admit=4;
    bool changeAfterBgm=false,clearAfterEnv=false;
    SoundFlagServices services(){
        return {this,[](void* user,SoundFlagFade kind,uint32_t group,int32_t duration,int32_t flags){
            auto& f=*static_cast<Fixture*>(user);assert(duration==1000 && flags==0);
            if(f.calls.size()==f.admit)return false;
            f.calls.push_back({kind,group});
            if(kind==SoundFlagFade::Bgm && group==1 && f.changeAfterBgm){
                f.flags.clear(1);SoundFlagCursor nested;
                // Publish ENV gate before its unavailable fade: source caller's
                // subsequent reload must see this live halfword, not OR result.
                assert(!f.flags.set(2,{},nested));
            }
            if(kind==SoundFlagFade::Environment && group==0 && f.clearAfterEnv)f.flags.clear(2);
            return true;
        }};
    }
};
int main(){
    SourceSoundFlags flags;SoundFlagCursor c;
    assert(flags.value()==0 && flags.set(0x10080,{},c));
    assert(flags.value()==0x80 && !c.active);
    assert(flags.set(0x20,{},c) && flags.value()==0xa0);
    flags.clear(0xffff0020);assert(flags.value()==0x80);
    flags.clear(0);assert(flags.value()==0x80);

    Fixture f;f.admit=0;
    assert(!f.flags.set(1,f.services(),c));
    assert(f.flags.value()==1 && c.pending==0x800dabac && c.stage==0);
    // Clear during a wait does not undo the captured BGM branch or replay OR.
    f.flags.clear(1);f.admit=1;
    assert(!f.flags.set(0x8000,f.services(),c));
    assert(f.flags.value()==0 && f.calls.size()==1 && c.stage==1);
    f.changeAfterBgm=true;f.clearAfterEnv=true;f.admit=3;
    assert(!f.flags.set(0x8000,f.services(),c));
    assert(f.calls.size()==3 && c.pending==0x800d7470 && c.stage==4 && f.flags.value()==0);
    f.admit=4;assert(f.flags.set(0x8000,f.services(),c) && !c.active && c.pending==0);
    assert(f.calls.size()==4 && f.flags.value()==0);
    assert(f.calls[0].kind==SoundFlagFade::Bgm && f.calls[0].group==0);
    assert(f.calls[1].kind==SoundFlagFade::Bgm && f.calls[1].group==1);
    assert(f.calls[2].kind==SoundFlagFade::Environment && f.calls[2].group==0);
    assert(f.calls[3].kind==SoundFlagFade::Environment && f.calls[3].group==1);

    Fixture inherited;
    assert(!inherited.flags.set(3,{},c)); // Both gates survive adding unrelated20.
    c={};assert(inherited.flags.set(0x20,inherited.services(),c));
    assert(inherited.flags.value()==0x23 && inherited.calls.size()==4);
    Fixture envOnly;assert(envOnly.flags.set(2,envOnly.services(),c));
    assert(envOnly.calls.size()==2 && envOnly.calls[0].kind==SoundFlagFade::Environment);

    Game game;auto shared=game.soundFlags;
    assert(game.applyFieldItemStateRequest(ItemGetRequest::SoundFlag80On));
    assert(game.itemSoundFlags()==0x80);
    game.restore(game.progress);assert(game.soundFlags==shared && game.itemSoundFlags()==0x80);
    assert(game.applyFieldItemStateRequest(ItemGetRequest::SoundFlag80Off) && shared->value()==0);
}
