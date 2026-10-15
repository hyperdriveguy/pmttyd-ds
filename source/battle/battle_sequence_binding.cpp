#include "battle_sequence_binding.hpp"
#include "game.hpp"
#include "event_native_runtime.hpp"
#include "event_controls.hpp"
#include "field_npc_registry_owner.hpp"
#include "field_battle_information.hpp"
#include "event_memory_release.hpp"
namespace ttyd {
BattleSequenceBinding::BattleSequenceBinding(Game& game,EventNativeRuntime& vm):game_(game),vm_(vm),process_(game.battleSequenceProcess),npcs_(game.originalFieldNpcs){}
std::unique_ptr<BattleSequenceBinding> BattleSequenceBinding::bind(Game& game,EventNativeRuntime& vm,const char* root){
    // Entry must be connected before the legacy action snapshot creates B.
    if(game.trioAttack.cameras() || !game.originalFieldNpcs || !game.originalFieldNpcs->initialized() || !root)return {};
    EventScriptAssets data;const std::string path=std::string(root)+"/battle-sequence-entry.pmevt";
    if(!data.load(path.c_str()))return {};
    if(!vm.memory.importScripts(data)){
        // Reattaching a facade must preserve VM storage. Reuse only a complete
        // exact original constant view; partial/conflicting views stay rejected.
        for(const auto& segment:data.segments())for(uint32_t i=0;i<segment.size;++i){uint8_t expected=0,actual=0;if(!data.readByte(segment.address+i,expected) || !vm.memory.readByte(segment.address+i,actual) || actual!=expected)return {};}
    }
    if(!game.battleSequenceProcess)game.battleSequenceProcess=std::make_shared<BattleSequenceProcess>();
    auto result=std::unique_ptr<BattleSequenceBinding>(new BattleSequenceBinding(game,vm));
    if(!vm.memory.registerExternalRam(0x8041e968,result->process_->savedMode.data(),4))return {};
    result->mapped_=true;
    if(result->process_->storage && !result->attachStorage(result->process_->storage))return {};
    return result;
}
BattleSequenceBinding::~BattleSequenceBinding(){for(auto a:storageMappings_)vm_.memory.unregisterExternalRam(a);if(mapped_)vm_.memory.unregisterExternalRam(0x8041e968);}
bool BattleSequenceBinding::attachStorage(std::shared_ptr<BattleSequenceStorage> owner){
    if(!owner || owner->pouch_!=game_.progress.originalPouch || !storageMappings_.empty() || fieldInit_.active || (process_->storage && process_->storage!=owner))return false;
    struct Span {uint32_t address;uint8_t* bytes;uint32_t size;};
    const Span spans[]={{0x8041e808,owner->globals_.data(),20},{0x8041eb00,owner->pouchPointer_.data(),4},{0x8041eb90,owner->battlePointer_.data(),4},{owner->arenaAddress_,owner->arena_.data(),uint32_t(owner->arena_.size())},{owner->pouchAddress_,owner->pouch_->bytes.data(),uint32_t(owner->pouch_->bytes.size())}};
    for(const auto& span:spans){if(!vm_.memory.registerExternalRam(span.address,span.bytes,span.size)){for(auto a:storageMappings_)vm_.memory.unregisterExternalRam(a);storageMappings_.clear();return false;}storageMappings_.push_back(span.address);}
    process_->storage=std::move(owner);return true;
}
bool BattleSequenceBinding::admitEntry() const{
    uint32_t gp=0,f=0,n=0,flags=0;
    return !game_.trioAttack.cameras() && vm_.memory.readWord(0x80415fd0,gp) && vm_.memory.readWord(gp+0x168,f) && f==npcs_->fieldWorkAddress() && vm_.memory.readWord(f+8,n) && npcs_->ownsFieldActor(n) && vm_.memory.readWord(n,flags) && (flags&1);
}
uint32_t BattleSequenceBinding::pending() const{
    if(sound_.active)return sound_.pending;
    if(fieldInit_.active)return fieldInit_.pending;
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
    s.byte=[](void* p,uint32_t a)->std::optional<uint8_t>{uint8_t v;return static_cast<Bridge*>(p)->binding.vm_.memory.readByte(a,v)?std::optional<uint8_t>(v):std::nullopt;};
    s.storeByte=[](void* p,uint32_t a,uint8_t v){return static_cast<Bridge*>(p)->binding.vm_.memory.writeByte(a,v);};
    s.call=[](void* p,BattleSequenceCall& c){auto& b=*static_cast<Bridge*>(p);auto& binding=b.binding;auto& game=binding.game_;
        switch(c.function){
        case 0x800d5eb0:
            if(!binding.storageMappings_.empty())return binding.vm_.memory.readWord(0x8041eb00,c.result);
            break;
        case 0x8000e95c:{if(c.args[0]!=4 || c.args[1]!=2 || !game.cameraManager)return false;auto* camera=game.cameraManager->camera(4);if(!camera)return false;camera->mode=2;return true;}
        case 0x80030078:{
            const auto& storage=binding.process_->storage;uint32_t root=0;
            if(!storage || binding.storageMappings_.empty() || !binding.vm_.platform.flushBlock || !binding.vm_.platform.systemCall || !binding.vm_.memory.readWord(0x8041e808,root) || !storage->canAllocate(root,0x64000,binding.vm_.memory))return false;
            allocateEventBattleMapPool(binding.vm_.memoryBinding.services());return true;
        }
        case 0x80114518:initializePackedBattleInformation(c.args[0],binding.vm_.memoryBinding.services());return true;
        case 0x80114598:return binding.vm_.memory.writeWord(c.args[0],c.args[1]);
        case 0x80114590:return binding.vm_.memory.writeWord(c.args[0]+4,c.args[1]);
        case 0x80114588:return binding.vm_.memory.writeWord(c.args[0]+8,c.args[1]);
        case 0x80114510:return binding.vm_.memory.writeWord(c.args[0]+0xc,c.args[1]);
        case 0x80031260:
            if(c.args[0]!=0 || c.args[1]!=0xf5de0180)return false;
            c.result=uint32_t(readSavedSwitch(SavedSwitchFamily::GlobalByte,0,binding.vm_.interpreterServices().operands));return true;
        case 0x80035c68:stopAllEvents(c.args[0],binding.vm_.creationServices());return true;
        case 0x800dc110:return game.soundFlags->set(c.args[0],b.sound,binding.sound_);
        case 0x8005c2c4:fieldMarioControlOff(game.fieldMarioBattle);game.marioFieldControl=fieldMarioControlOffCheck(game.fieldMarioBattle)==0;return true;
        case 0x800bed44:game.fieldParties.control(false);game.partnerFieldControl=game.fieldParties.controlsEnabled();return true;
        case 0x8002e218:game.originalSequence->request(int32_t(c.args[0]),c.args[1],c.args[2]);return true;
        default:break;
        }
        return b.backend.call && b.backend.call(b.backend.user,c);
    };
    if(fn==0x80073f20){const bool done=initializeBattleSequenceEntry(s,init_);if(done)entered_=true;return done;}
    struct Nested {BattleSequenceBinding& binding;BattleSequenceEntryServices base;} nested{*this,s};
    BattleSequenceEntryServices outer=s;outer.user=&nested;
    outer.word=[](void* p,uint32_t a){auto& n=*static_cast<Nested*>(p);return n.base.word(n.base.user,a);};
    outer.half=[](void* p,uint32_t a){auto& n=*static_cast<Nested*>(p);return n.base.half(n.base.user,a);};
    outer.store=[](void* p,uint32_t a,uint32_t v){auto& n=*static_cast<Nested*>(p);return n.base.store(n.base.user,a,v);};
    outer.call=[](void* p,BattleSequenceCall& call){auto& n=*static_cast<Nested*>(p);if(call.function!=0x800738ac)return n.base.call(n.base.user,call);if(!n.binding.fieldInit_.active && !n.binding.admitEntry())return false;if(!initializeFieldBattle(n.base,n.binding.fieldInit_))return false;call.result=0;return true;};
    return updateBattleSequenceEntry(work,outer,main_);
}
}
