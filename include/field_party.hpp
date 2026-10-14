#pragma once
#include "field_battle.hpp"
#include <array>
#include <cassert>
#include <memory>

namespace ttyd {
// Encoded native field-owner identity, distinct from an original heap address.
// Only the bound typed Mario views are available through native services.
constexpr uint32_t FieldMarioOwnerAddress=0x85700000;
// Source-sized, big-endian field actor storage. Allocation/zeroing is only the
// EntryMain prelude; it does not initialize animation, mode or placement.
struct FieldPartyActor {
    std::array<uint8_t,0x188> bytes{};
    uint32_t word(unsigned offset) const {
        assert(offset+4<=bytes.size());
        return uint32_t(bytes[offset])<<24|uint32_t(bytes[offset+1])<<16|uint32_t(bytes[offset+2])<<8|bytes[offset+3];
    }
    void word(unsigned offset,uint32_t value) {
        assert(offset+4<=bytes.size());
        for(unsigned i=0;i<4;++i)bytes[offset+i]=uint8_t(value>>(24-8*i));
    }
};
struct FieldPartyRegistry {
    std::array<std::unique_ptr<FieldPartyActor>,2> actors;
    uint32_t currentDefinition=0; // Original global kind-record pointer view.
    FieldPartyRegistry()=default;
    FieldPartyRegistry(const FieldPartyRegistry& other):currentDefinition(other.currentDefinition) {
        for(unsigned i=0;i<actors.size();++i)if(other.actors[i])actors[i]=std::make_unique<FieldPartyActor>(*other.actors[i]);
    }
    FieldPartyRegistry& operator=(const FieldPartyRegistry& other) {
        if(this!=&other){FieldPartyRegistry copy(other);actors=std::move(copy.actors);currentDefinition=copy.currentDefinition;}return *this;
    }
    FieldPartyRegistry(FieldPartyRegistry&&)=default;
    FieldPartyRegistry& operator=(FieldPartyRegistry&&)=default;
    FieldPartyActor* pointer(int32_t index) const {
        if(index<0)return nullptr;
        assert(index<2); // Native supported domain; the original getter has no upper bound check.
        return actors[unsigned(index)].get();
    }
    int32_t current(const FieldMarioBattleState& mario) const {
        for(const auto raw:mario.partyIndices) {
            const int32_t index=raw<128?int32_t(raw):int32_t(raw)-256;
            const auto* actor=pointer(index);
            if(actor && !(actor->word(0)&8))return index;
        }
        return -1;
    }
    void control(bool enabled) {
        for(auto& actor:actors)if(actor)actor->word(0,enabled?actor->word(0)&~0x20000000u:actor->word(0)|0x20000000u);
    }
    bool controlsEnabled() const {
        for(const auto& actor:actors)if(actor && (actor->word(0)&0x20000000))return false;
        return true;
    }
    int32_t allocateFirst() {
        for(unsigned i=0;i<actors.size();++i)if(!actors[i]) {
            actors[i]=std::make_unique<FieldPartyActor>(); // First zero precedes lookup.
            pointer(int32_t(i))->bytes.fill(0); // Source clears the looked-up actor again.
            return int32_t(i);
        }
        return -1;
    }
    void initializeBeforePose(int32_t index,uint8_t kind,FieldMarioBattleState& mario) {
        auto* actor=pointer(index);assert(actor);
        actor->word(0x164,mario.cameraId);
        actor->word(0x0c,0xffffffff);actor->word(0x10,0xffffffff);actor->word(0x14,0xffffffff);
        actor->word(0,1);actor->bytes[0x2f]=uint8_t(index);
        actor->word(0x160,FieldMarioOwnerAddress);actor->bytes[0x31]=kind;
        if(mario.savedPartyKinds[0]==0)mario.savedPartyKinds[0]=kind;
        if(index==1)actor->word(0,actor->word(0)|4);
        currentDefinition=0x803143e8+uint32_t(kind)*0x28;
        if(mario.flags&0x80000000)actor->word(4,actor->word(4)|0x8000);
    }
};
}
