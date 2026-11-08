#include "battle_sequence_binding.hpp"
#include "battle_sequence_camera_receipt.hpp"
#include "harbor_source_field_admission.hpp"
#include "harbor_source_display_pass.hpp"
#include "game.hpp"
#include "event_native_runtime.hpp"
#include "event_controls.hpp"
#include "field_npc_registry_owner.hpp"
#include "field_battle_information.hpp"
#include "event_memory_release.hpp"
namespace ttyd {
namespace {bool poolBacking(EventAddressSpace&m,uint32_t address,const uint8_t*bytes,uint32_t size){return m.withRawRamAccess([&](EventAddressSpace::RawRamAccess&r){return r.matchesBacking(address,size,bytes);});}}
BattleSequenceBinding::BattleSequenceBinding(Game& game,EventNativeRuntime& vm):game_(game),vm_(vm),process_(game.battleSequenceProcess),npcs_(game.originalFieldNpcs),gameLife_(game.battleSequenceFacade_.proof()),vmLife_(vm.nativeFacadeProof()){}
bool BattleSequenceBinding::facadeCurrent()const{
 if(!facade_.proof().current()||!gameLife_.current()||!vmLife_.current())return false;
 return process_&&game_.battleSequenceProcess==process_&&game_.originalFieldNpcs==npcs_&&(!storage_||(process_->storage==storage_&&storage_->pouch_==game_.progress.originalPouch&&storage_->aliases(vm_.memory)));
}
std::shared_ptr<const BattleSequenceNestedPoolReceipt> BattleSequenceBinding::nestedPoolReceipt()const{return nestedPool_&&nestedPool_->current()?nestedPool_:nullptr;}
HarborBattleCameraReceipt BattleSequenceBinding::battleCameraReceipt()const{return cameraReceipt_?cameraReceipt_->publication():HarborBattleCameraReceipt{};}
bool BattleSequenceBinding::bindHarborSourceServices(std::shared_ptr<const HarborSourceFieldServices> services){
 if(invoking_||!facadeCurrent()||!services||!services->lifetime||!services->current)return false;
 if(harborServices_){const auto original=harborServicesIdentity_.lock();return original==services&&!original.owner_before(services)&&!services.owner_before(original); }
 harborServicesIdentity_=services;harborServices_=std::make_shared<const HarborSourceFieldServices>(*services);return true;
}
std::shared_ptr<HarborSourceFieldAdmission> BattleSequenceBinding::sourceHarbor()const{return harborAdmission_&&harborAdmission_->current()?harborAdmission_:nullptr;}
// Bind BEFORE legacy snapshot B exists and only with established field NPC
// owner. Failed script import may reuse only a COMPLETE byte-identical source
// view; it does not repair partial/conflicting mappings or reset live state.
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
BattleSequenceBinding::~BattleSequenceBinding(){
 facade_.retire();if(nestedPool_)nestedPool_->retire();
 if(!vmLife_.current())return;
 if(storage_)storage_->unregisterOwnAliases(vm_.memory,storageMappings_);
 if(mapped_&&poolBacking(vm_.memory,0x8041e968,process_->savedMode.data(),4))vm_.memory.unregisterExternalRam(0x8041e968);
}
// Actual field party/NPC shared pool identity must agree. Bind existing W/RAM
// before entry/init; no synthetic empty animation pool replaces missing owners.
bool BattleSequenceBinding::attachAnimationPool(){
    if(!facadeCurrent() || animationMemory_ || entered_ || init_.active || fieldInit_.active || !game_.fieldParties.originalPool || game_.fieldParties.originalPool!=npcs_->poses().sharedPool())return false;
    animationMemory_=AnimationPoolMemory::bind(game_.fieldParties.originalPool,vm_);return bool(animationMemory_);
}
// Five alias spans: heap globals8041e808(20bytes), pouch/battle pointer cells,
// source arena and original pouch. Partial map failure unregisters only completed
// spans; process storage publishes only after all mappings succeed.
bool BattleSequenceBinding::attachStorage(std::shared_ptr<BattleSequenceStorage> owner){
    if(!facadeCurrent() || !owner || owner->pouch_!=game_.progress.originalPouch || !storageMappings_.empty() || fieldInit_.active || (process_->storage && process_->storage!=owner))return false;
    struct Span {uint32_t address;uint8_t* bytes;uint32_t size;};
    const Span spans[]={{0x8041e808,owner->globals_.data(),20},{0x8041eb00,owner->pouchPointer_.data(),4},{0x8041eb90,owner->battlePointer_.data(),4},{owner->arenaAddress_,owner->arena_.data(),uint32_t(owner->arena_.size())},{owner->pouchAddress_,owner->pouch_->bytes.data(),uint32_t(owner->pouch_->bytes.size())}};
    for(const auto& span:spans){if(!vm_.memory.registerExternalRam(span.address,span.bytes,span.size)){for(auto a:storageMappings_)vm_.memory.unregisterExternalRam(a);storageMappings_.clear();return false;}storageMappings_.push_back(span.address);}
    storage_=owner;process_->storage=std::move(owner);return true;
}
// Require exact VM facade, attached owner, actual flush/barrier providers and
// source allocator admission before effects. Allocation is the shared primary
// map arena; a different facade cannot stand in merely by matching addresses.
std::optional<uint32_t> BattleSequenceBinding::allocateMap(uint32_t root,uint32_t bytes,EventAddressSpace& facade){
    if(!facadeCurrent() || &facade!=&vm_.memory || storageMappings_.empty() || !process_->storage || !vm_.platform.flushBlock || !vm_.platform.systemCall || !process_->storage->canAllocate(root,bytes,facade))return {};
    uint32_t primary=0;if(!vm_.memory.readWord(0x8041e808,primary)||root!=primary)return {}; // Nested lease allocations belong only to its adopted owner.
    return eventMapAllocate(root,bytes,vm_.memoryBinding.services());
}
// Walk32-byte-aligned source heap headers: validate full header/payload span,
// live-used bit and monotonic nonoverlapping next. A matching payload address
// alone cannot prove current allocation ownership or sufficient size.
bool BattleSequenceBinding::ownsMapAllocation(uint32_t payload,uint32_t bytes,const EventAddressSpace& facade) const{
    if(!facadeCurrent() || &facade!=&vm_.memory || storageMappings_.empty() || !process_->storage)return false;
    const auto& owner=*process_->storage;uint32_t root=0;if(!facade.readWord(0x8041e808,root))return false;
    const uint64_t end=uint64_t(owner.arenaAddress_)+owner.arena_.size();
    for(uint32_t h=root;;){uint32_t next=0,size=0;uint16_t used=0;
        if((h&31) || h<owner.arenaAddress_ || uint64_t(h)+32>end || !facade.readWord(h,next) || !facade.readWord(h+4,size) || !facade.readHalf(h+8,used) || uint64_t(h)+32+size>end)return false;
        if(uint64_t(h)+32==payload)return used!=0 && size>=bytes;
        if(!next || next<=h || uint64_t(next)<uint64_t(h)+32+size)return false;
        h=next;
    }
}
// Require same optional party/NPC pool, no legacy battle snapshot, actual
// GP+168 field work identity and active owned actor at F+8. This is no-effects
// native admission, not source NPC initialization or completed battle setup.
bool BattleSequenceBinding::admitEntry() const{
    if(!facadeCurrent())return false;
    uint32_t gp=0,f=0,n=0,flags=0;
    return (!game_.fieldParties.originalPool || game_.fieldParties.originalPool==npcs_->poses().sharedPool()) && !game_.trioAttack.cameras() && vm_.memory.readWord(0x80415fd0,gp) && vm_.memory.readWord(gp+0x168,f) && f==npcs_->fieldWorkAddress() && vm_.memory.readWord(f+8,n) && npcs_->ownsFieldActor(n) && vm_.memory.readWord(n,flags) && (flags&1);
}
// Diagnostic priority is sound, animation, field-init, init/main. Zero pending
// does not independently prove all external services/native integration ready.
uint32_t BattleSequenceBinding::pending() const{
    if(sound_.active)return sound_.pending;
    if(animation_.active)return pendingAnimationBufferFunction(animation_);
    if(fieldInit_.active)return fieldInit_.pending;
    return init_.active?init_.pending:main_.pending;
}
// Only source Init80073f20/Main80073b24 on Game's exact sequence work.
// Entered becomes true only after complete Init. Stack-local bridge closures
// are rebuilt per call; retained child cursors belong to this facade, not them.
bool BattleSequenceBinding::callback(uint32_t fn,OriginalSequenceWork& work,const BattleSequenceEntryServices& backend,const SoundFlagServices& sound){
    if(invoking_||!facadeCurrent())return false;
    invoking_=true;struct Invocation{bool&v;~Invocation(){v=false;}}invocation{invoking_};
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
        case 0x8000e95c:{if(c.args[0]!=4 || c.args[1]!=2 || !game.cameraManager)return false;auto* camera=game.cameraManager->camera(4);if(!camera)return false;camera->mode=2;binding.cameraReceipt_=BattleSequenceCameraReceipt::capture(binding);return true;}
        // Actual map pool allocation requires source capacity0x64000 and
        // platform cache/barrier hooks. Camera call admits only source(4, 2);
        // saved-switch call only(0, 0xf5de0180). Others delegate to real backend.
        case 0x80030078:{
            const auto& storage=binding.process_->storage;uint32_t root=0;
            if(!storage || binding.storageMappings_.empty() || !binding.vm_.platform.flushBlock || !binding.vm_.platform.systemCall || !binding.vm_.memory.readWord(0x8041e808,root) || !storage->canAllocate(root,0x64000,binding.vm_.memory))return false;
            auto receipt=BattleSequenceNestedPoolReceipt::prepare(binding);if(!receipt)return false;
            allocateEventBattleMapPool(binding.vm_.memoryBinding.services());
            // The actual void callee completed. Metadata failure must never
            // replay its successful source allocation on a retry.
            (void)receipt->complete();binding.nestedPool_=std::move(receipt);return true;
        }
        case 0x800f8f04:
            if(!binding.harborServices_)break; // Preserve an existing genuine external provider.
            if(!binding.harborAdmission_)binding.harborAdmission_=HarborSourceFieldAdmission::prepare(binding);
            return binding.harborAdmission_&&binding.harborAdmission_->resumeCold(c.args[0]);
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
    // Main intercepts actual field initializer and animation battle-init only.
    // Keep each child cursor across waits; source Main's word/half-only outer
    // table intentionally exposes no byte callbacks. Missing backend stays held,
    // never fake success or activation of the legacy snapshot renderer.
    struct Nested {BattleSequenceBinding& binding;BattleSequenceEntryServices base;} nested{*this,s};
    BattleSequenceEntryServices outer=s;outer.user=&nested;
    outer.word=[](void* p,uint32_t a){auto& n=*static_cast<Nested*>(p);return n.base.word(n.base.user,a);};
    outer.half=[](void* p,uint32_t a){auto& n=*static_cast<Nested*>(p);return n.base.half(n.base.user,a);};
    outer.store=[](void* p,uint32_t a,uint32_t v){auto& n=*static_cast<Nested*>(p);return n.base.store(n.base.user,a,v);};
    auto children=outer;
    children.byte=[](void* p,uint32_t a){auto& n=*static_cast<Nested*>(p);return n.base.byte(n.base.user,a);};
    children.storeByte=[](void* p,uint32_t a,uint8_t v){auto& n=*static_cast<Nested*>(p);return n.base.storeByte(n.base.user,a,v);};
    children.call=[](void* p,BattleSequenceCall& call){auto& n=*static_cast<Nested*>(p);
        if(call.function!=0x80041c58)return n.base.call(n.base.user,call);
        return runAnimationBufferFunction(call.function,0,n.base,n.binding.animation_);
    };
    struct FieldBridge {Nested& nested;BattleSequenceEntryServices children;} fieldBridge{nested,children};
    outer.user=&fieldBridge;
    outer.word=[](void* p,uint32_t a){auto& n=static_cast<FieldBridge*>(p)->nested;return n.base.word(n.base.user,a);};
    outer.half=[](void* p,uint32_t a){auto& n=static_cast<FieldBridge*>(p)->nested;return n.base.half(n.base.user,a);};
    outer.store=[](void* p,uint32_t a,uint32_t v){auto& n=static_cast<FieldBridge*>(p)->nested;return n.base.store(n.base.user,a,v);};
    outer.byte=nullptr;outer.storeByte=nullptr; // Main uses word/half requests only.
    outer.call=[](void* p,BattleSequenceCall& call){auto& b=*static_cast<FieldBridge*>(p);auto& n=b.nested;
        if(call.function!=0x800738ac)return n.base.call(n.base.user,call);
        if(!n.binding.fieldInit_.active && !n.binding.admitEntry())return false;
        if(!initializeFieldBattle(b.children,n.binding.fieldInit_))return false;
        call.result=0;return true;
    };
    return updateBattleSequenceEntry(work,outer,main_);
}
}
