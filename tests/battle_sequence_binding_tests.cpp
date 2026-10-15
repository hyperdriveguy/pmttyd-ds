#include "battle_sequence_binding.hpp"
#include "game.hpp"
#include "field_npc_registry_owner.hpp"
#include "event_native_runtime.hpp"
#include <cassert>
#include <map>
using namespace ttyd;
int main(){
    auto game=std::make_unique<Game>(false);auto vm=std::make_unique<EventNativeRuntime>(0x81000000,EventNativePlatform{});
    EventScriptAssets data;assert(data.load("romfs/door-data.pmevt") && vm->memory.importScripts(data));
    auto registry=game->initializeOriginalFieldNpcs(vm->memory,0x85000000);assert(registry);
    const auto resource=loadBattlePoseResource("romfs/field-native-c_kuri_h.pose-directory.bin","romfs/field-native-c_kuri_h.pmanim");assert(resource && game->originalFieldNpcs->poses().install(resource));
    std::vector<uint8_t> model(64);for(unsigned i=0;i<64;++i)model[i]=resource->draw->byte(4+i);assert(vm->memory.registerRam(0x86000000,std::move(model)));
    uint32_t name=0;assert(vm->memory.readWord(0x90524f10+0x5c,name));assert(registry->enter(name,0x86000000,{})==0);
    const auto n=*registry->lookup(name);uint32_t gp=0,f=0;assert(vm->memory.readWord(0x80415fd0,gp) && vm->memory.readWord(gp+0x168,f));
    assert(game->originalFieldNpcs->ownsFieldActor(n) && !game->originalFieldNpcs->ownsFieldActor(n+1));
    // Fixture supplies only source caller's field encounter words, not sequence
    // work or an invented BattleWork. No original authored encounter is claimed.
    assert(vm->memory.writeWord(f+8,n) && vm->memory.writeWord(f+0x10,0x800000));
    writeSavedSwitch(SavedSwitchFamily::GlobalByte,0,11,vm->interpreterServices().operands);
    std::array<EventEntry,2> events{};vm->field.capacity=2;vm->field.entries=events.data();events[0].flags=events[1].flags=1;events[0].type=4;events[1].type=2;
    auto binding=game->bindOriginalBattleSequence(*vm,"romfs");assert(binding);
    struct Backend {std::map<uint32_t,unsigned> counts;uint32_t hold=0x800daefc;};Backend backend;
    BattleSequenceEntryServices services;services.user=&backend;services.call=[](void* p,BattleSequenceCall& c){auto& b=*static_cast<Backend*>(p);++b.counts[c.function];return c.function!=b.hold && (c.function==0x800daefc || c.function==0x800db7a0 || c.function==0x800d76b8 || c.function==0x80005f28 || c.function==0x80013368);};
    struct Context {Game& game;EventNativeRuntime& vm;BattleSequenceBinding& binding;BattleSequenceEntryServices services;} context{*game,*vm,*binding,services};
    OriginalSequenceServices sequence;sequence.user=&context;sequence.drawSync=[](void*){return true;};
    sequence.callback=[](void* p,uint32_t fn,OriginalSequenceWork& work){auto& c=*static_cast<Context*>(p);if(fn==0x80007390){uint32_t gp=0,flags=0;assert(c.vm.memory.readWord(0x80415fd0,gp) && c.vm.memory.readWord(gp,flags) && c.vm.memory.writeWord(gp,flags&~2u));return true;}return c.binding.callback(fn,work,c.services);};
    game->originalSequence->request(4,0,0);
    assert(!game->originalSequence->main(sequence) && binding->pending()==0x800daefc && events[0].flags==1 && game->soundFlags->value()==0);
    backend.hold=0x80013368;
    assert(!game->originalSequence->main(sequence) && binding->pending()==0x80013368);
    assert(events[0].flags==3 && events[1].flags==1 && game->soundFlags->value()==0x80 && game->fieldMarioBattleState().controlDepth==1);
    assert(game->originalSequence->state.current==4 && game->originalSequence->state.previous==2 && game->originalSequence->state.work[1]==0);
    backend.hold=0;assert(game->originalSequence->main(sequence) && game->originalSequence->state.work[1]==1 && game->fieldMarioBattleState().controlDepth==1);
    assert(backend.counts[0x800db7a0]==1 && backend.counts[0x80005f28]==2);
    assert(!game->originalSequence->main(sequence) && binding->pending()==0x80007388); // Real unbound resource-root child.
    assert(vm->memory.writeWord(0x8041e968,0x12345678));
    auto retained=game->battleSequenceProcess;binding.reset();uint32_t value=0;assert(!vm->memory.readWord(0x8041e968,value));
    assert((retained && retained->savedMode==std::array<uint8_t,4>{0x12,0x34,0x56,0x78}));
    assert(loadProfessorStoryAssets("romfs") && loadUndergroundStoryAssets("romfs") && loadTrioAttackAssets("romfs") && loadTrioBattleAssets("romfs"));
    ProfessorStory scene;assert(scene.beginTrioBattleIntro(0));TrioBattleCommands menu;assert(menu.begin(scene));menu.input(0x100,0);menu.input(0x100,0);
    assert(game->trioAttack.begin(menu,scene,10) && !game->bindOriginalBattleSequence(*vm,"romfs"));
}
