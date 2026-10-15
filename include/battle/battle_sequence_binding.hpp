#pragma once
#include "battle_sequence_entry.hpp"
#include "source_sound_flags.hpp"
#include "battle_field_initialize.hpp"
#include "battle_sequence_storage.hpp"
#include <memory>
namespace ttyd {
class Game;class EventNativeRuntime;class FieldNpcRegistryOwner;
// Process BSS8041e968. Init writes this from live GP+c; facades never seed it.
struct BattleSequenceProcess {std::array<uint8_t,4> savedMode{};std::shared_ptr<BattleSequenceStorage> storage;};
class BattleSequenceBinding {
public:
    static std::unique_ptr<BattleSequenceBinding> bind(Game&,EventNativeRuntime&,const char* assetsRoot);
    ~BattleSequenceBinding();
    BattleSequenceBinding(const BattleSequenceBinding&)=delete;
    BattleSequenceBinding& operator=(const BattleSequenceBinding&)=delete;
    bool callback(uint32_t,OriginalSequenceWork&,const BattleSequenceEntryServices&,const SoundFlagServices& = {});
    uint32_t pending() const;
    bool attachStorage(std::shared_ptr<BattleSequenceStorage>);
private:
    BattleSequenceBinding(Game&,EventNativeRuntime&);
    bool admitEntry() const;
    Game& game_;EventNativeRuntime& vm_;
    std::shared_ptr<BattleSequenceProcess> process_;
    std::shared_ptr<FieldNpcRegistryOwner> npcs_;
    bool mapped_=false,entered_=false;
    BattleSequenceEntryCursor init_,main_;
    SoundFlagCursor sound_;
    BattleFieldInitializeCursor fieldInit_;
    std::vector<uint32_t> storageMappings_;
};
}
