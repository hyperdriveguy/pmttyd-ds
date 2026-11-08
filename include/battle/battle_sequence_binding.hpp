#pragma once
#include "battle_sequence_entry.hpp"
#include "source_sound_flags.hpp"
#include "battle_field_initialize.hpp"
#include "battle_sequence_storage.hpp"
#include "animation_buffer_refresh.hpp"
#include "animation_pool_memory.hpp"
#include <memory>
#include "native_facade_registration.hpp"
#include "battle_sequence_nested_pool.hpp"
// Facade borrows Game/VM; retained process state owns saved-mode BSS/storage.
// Mapping aliases must outlive callbacks; destructor unregisters only its own
// spans, never source storage or another facade's saved game/animation lifetime.
namespace ttyd {
class Game;class EventNativeRuntime;class FieldNpcRegistryOwner;class BattleSequenceCameraReceipt;struct HarborBattleCameraReceipt;class HarborSourceFieldAdmission;struct HarborSourceFieldServices;
// Process BSS8041e968. Init writes this from live GP+c; facades never seed it.
class BattleSequenceNestedPoolEpoch;
struct BattleSequenceProcess {std::array<uint8_t,4> savedMode{};std::shared_ptr<BattleSequenceStorage> storage;
private:
    std::shared_ptr<BattleSequenceNestedPoolEpoch> nestedPoolEpoch_;
    friend class BattleSequenceBinding;friend class BattleSequenceNestedPoolReceipt;
};
class BattleSequenceBinding {
public:
    static std::unique_ptr<BattleSequenceBinding> bind(Game&,EventNativeRuntime&,const char* assetsRoot);
    ~BattleSequenceBinding();
    BattleSequenceBinding(const BattleSequenceBinding&)=delete;
    BattleSequenceBinding& operator=(const BattleSequenceBinding&)=delete;
    bool callback(uint32_t,OriginalSequenceWork&,const BattleSequenceEntryServices&,const SoundFlagServices& = {});
    uint32_t pending() const;
    bool attachStorage(std::shared_ptr<BattleSequenceStorage>);
    std::shared_ptr<const BattleSequenceNestedPoolReceipt> nestedPoolReceipt()const;
    HarborBattleCameraReceipt battleCameraReceipt()const;
    bool bindHarborSourceServices(std::shared_ptr<const HarborSourceFieldServices>);
    std::shared_ptr<HarborSourceFieldAdmission> sourceHarbor()const;
    bool attachAnimationPool(); // Existing actual W/RAM, before sequence entry.
    // Shared primary map arena; compare facade identity before any effects.
    std::optional<uint32_t> allocateMap(uint32_t root,uint32_t bytes,EventAddressSpace&);
    bool ownsMapAllocation(uint32_t payload,uint32_t bytes,const EventAddressSpace&) const;
private:
    BattleSequenceBinding(Game&,EventNativeRuntime&);
    bool admitEntry() const;
    bool facadeCurrent() const;
    friend class BattleSequenceNestedPoolReceipt;
    friend class BattleSequenceCameraReceipt;
    friend class HarborSourceFieldAdmission;
    friend struct HarborSourceFieldBoundaryFixture;
    Game& game_;EventNativeRuntime& vm_;
    std::shared_ptr<BattleSequenceProcess> process_;
    std::shared_ptr<FieldNpcRegistryOwner> npcs_;
    std::unique_ptr<AnimationPoolMemory> animationMemory_;
    bool mapped_=false,entered_=false;
    BattleSequenceEntryCursor init_,main_;
    SoundFlagCursor sound_;
    BattleFieldInitializeCursor fieldInit_;
    AnimationBufferCursor animation_;
    std::vector<uint32_t> storageMappings_;
    std::shared_ptr<BattleSequenceStorage> storage_;
    std::shared_ptr<BattleSequenceNestedPoolReceipt> nestedPool_;
    std::shared_ptr<BattleSequenceCameraReceipt> cameraReceipt_;
    std::shared_ptr<const uint8_t> cameraEpoch_;
    std::shared_ptr<const HarborSourceFieldServices> harborServices_;
    std::weak_ptr<const HarborSourceFieldServices> harborServicesIdentity_;
    std::shared_ptr<HarborSourceFieldAdmission> harborAdmission_;
    NativeFacadeRegistration::Proof gameLife_,vmLife_;
    bool invoking_=false;
    NativeFacadeRegistration facade_;
};
}
