#include "harbor_source_startup.hpp"
#include "battle_menu_file_cache.hpp"
#include "battle_event_callbacks.hpp"
#include "fixtures/host_event_cache.hpp"
#include <cassert>
using namespace ttyd;
int main() {
    EventNativeRuntime vm(0x81000000, {});
    test::HostEventCache publication(vm);
    EventScriptAssets original;
    assert(original.load("romfs/harbor-tutorial-events.pmevt") && vm.memory.importScripts(original));
    assert(vm.memory.registerRam(BattleColdPointerCell, std::vector<uint8_t>(4)));
    constexpr uint32_t Information=0x85000000;
    assert(vm.memory.registerRam(Information,std::vector<uint8_t>(16)));
    auto pool=HarborSourceMapPool::bind(vm.memory,vm.memoryBinding.services());assert(pool);
    publication.requireBattlePoolPublication();
    auto files=std::make_shared<BattleMenuFileCache>();
    assert(files->load("romfs/battle-class-common.tpl") && files->bind(vm.memory));
    BattleMenuFileBinding menu(files,nativeBattleActorServices(vm));
    unsigned stageCalls=0,audienceCalls=0;
    HarborSourceStartupBinding binding;binding.vm=&vm;binding.pool=pool.get();binding.menu=menu.services();
    binding.lifetime=std::make_shared<const uint8_t>(0);binding.current=[]{return true;};
    // Explicit test children isolate the actual BreakSlot cold-start suffix.
    // They do not claim source Stage/Audience execution or native readiness.
    binding.coldChild=[&](const BattleColdInitRequest& q)->std::optional<uint32_t>{
        if(q.function==0x80139780){++stageCalls;return 0;}
        if(q.function==0x801a83f8){++audienceCalls;return 0;}
        assert(false);return {};
    };
    auto startup=HarborSourceStartup::create(binding);assert(startup);
    assert(startup->resumeCold(Information) && startup->coldComplete() && startup->pending()==0);
    assert(stageCalls==1 && audienceCalls==1);
    const auto battle=startup->battleAddress();
    auto word=[&](uint32_t at){uint32_t v;assert(vm.memory.readWord(at,v));return v;};
    const auto entry=word(battle+0x163f8);
    std::array<uint32_t,3> addresses{};
    for(unsigned i=0;i<3;++i){
        const auto reel=battle+0x1616c+0x18+i*0x5c;
        const unsigned count=i==2?5:8;
        assert(word(reel)==i && word(reel+8)==0 && word(reel+0x18)==count);
        addresses[i]=word(reel+0x14);assert(pool->allocationLifetime(addresses[i]));
        for(unsigned j=0;j<count;++j)assert(word(addresses[i]+4*j)==word(0x802fe040+32*i+4*j));
    }
    const auto allocations=addresses;
    assert(startup->resumeCold(Information) && stageCalls==1 && audienceCalls==1);
    for(unsigned i=0;i<3;++i)assert(word(battle+0x1616c+0x18+i*0x5c+0x14)==allocations[i]);
    assert(pool->free(pool->root(),addresses[0]));
    assert(!startup->current() && !startup->resumeCold(Information));
    startup.reset();assert(files->release(entry));
}
