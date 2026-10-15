#include "battle_sequence_binding.hpp"
#include "game.hpp"
#include "event_native_runtime.hpp"
#include "event_controls.hpp"
#include "field_npc_registry_owner.hpp"
namespace ttyd {
BattleSequenceBinding::BattleSequenceBinding(Game& game,EventNativeRuntime& vm):game_(game),vm_(vm),process_(game.battleSequenceProcess),npcs_(game.originalFieldNpcs){}
std::unique_ptr<BattleSequenceBinding> BattleSequenceBinding::bind(Game& game,EventNativeRuntime& vm,const char* root){
    // Entry must be connected before the legacy action snapshot creates B.
    if(game.trioAttack.cameras() || !game.originalFieldNpcs || !game.originalFieldNpcs->initialized() || !root)return {};
    EventScriptAssets data;const std::string path=std::string(root)+"/battle-sequence-entry.pmevt";
    if(!data.load(path.c_str()) || !vm.memory.importScripts(data))return {};
    if(!game.battleSequenceProcess)game.battleSequenceProcess=std::make_shared<BattleSequenceProcess>();
    auto result=std::unique_ptr<BattleSequenceBinding>(new BattleSequenceBinding(game,vm));
    if(!vm.memory.registerExternalRam(0x8041e968,result->process_->savedMode.data(),4))return {};
    result->mapped_=true;return result;
}
BattleSequenceBinding::~BattleSequenceBinding(){if(mapped_)vm_.memory.unregisterExternalRam(0x8041e968);}
bool BattleSequenceBinding::admitEntry() const{
    uint32_t gp=0,f=0,n=0,flags=0;
    return !game_.trioAttack.cameras() && vm_.memory.readWord(0x80415fd0,gp) && vm_.memory.readWord(gp+0x168,f) && vm_.memory.readWord(f+8,n) && npcs_->ownsFieldActor(n) && vm_.memory.readWord(n,flags) && (flags&1);
}
uint32_t BattleSequenceBinding::pending() const{
    if(sound_.active)return sound_.pending;
    return init_.active?init_.pending:main_.pending;
}
bool BattleSequenceBinding::callback(uint32_t fn,OriginalSequenceWork& work,const BattleSequenceEntryServices& backend,const SoundFlagServices& sound){
    if((fn!=0x80073f20 && fn!=0x80073b24) || &work!=&game_.originalSequence->state.work || game_.trioAttack.cameras())return false;
    if(fn==0x80073f20 && !init_.active && !admitEntry())return false;
    if(fn==0x80073b24 && !entered_)return false;
    struct Bridge {BattleSequenceBinding& binding;const BattleSequenceEntryServices& backend;const SoundFlagServices& sound;} bridge{*this,backend,sound};
    BattleSequenceEntryServices s;s.user=&bridge;
    s.word=[](void* p,uint32_t a)->std::optional<uint32_t>{uint32_t v;return static_cast<Bridge*>(p)->binding.vm_.memory.readWord(a,v)?std::optional<uint32_t>(v):std::nullopt;};
    s.half=[](void* p,uint32_t a)->std::optional<uint16_t>{uint16_t v;return static_cast<Bridge*>(p)->binding.vm_.memory.readHalf(a,v)?std::optional<uint16_t>(v):std::nullopt;};
    s.store=[](void* p,uint32_t a,uint32_t v){return static_cast<Bridge*>(p)->binding.vm_.memory.writeWord(a,v);};
    s.call=[](void* p,BattleSequenceCall& c){auto& b=*static_cast<Bridge*>(p);auto& binding=b.binding;auto& game=binding.game_;
        switch(c.function){
        case 0x80031260:
            if(c.args[0]!=0 || c.args[1]!=0xf5de0180)return false;
            c.result=uint32_t(readSavedSwitch(SavedSwitchFamily::GlobalByte,0,binding.vm_.interpreterServices().operands));return true;
        case 0x80035c68:stopAllEvents(c.args[0],binding.vm_.creationServices());return true;
        case 0x800dc110:return game.soundFlags->set(c.args[0],b.sound,binding.sound_);
        case 0x8005c2c4:fieldMarioControlOff(game.fieldMarioBattle);game.marioFieldControl=fieldMarioControlOffCheck(game.fieldMarioBattle)==0;return true;
        case 0x800bed44:game.fieldParties.control(false);game.partnerFieldControl=game.fieldParties.controlsEnabled();return true;
        case 0x8002e218:game.originalSequence->request(int32_t(c.args[0]),c.args[1],c.args[2]);return true;
        default:return b.backend.call && b.backend.call(b.backend.user,c);
        }
    };
    if(fn==0x80073f20){const bool done=initializeBattleSequenceEntry(s,init_);if(done)entered_=true;return done;}
    return updateBattleSequenceEntry(work,s,main_);
}
}
