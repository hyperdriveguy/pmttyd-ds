#pragma once
#include "field_battle.hpp"
#include "actor_animation.hpp"
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
// Converted original c_pkr resource and native CPU shape ownership. These
// handles belong to this field registry; GPU/time/ABI equivalence is separate.
struct FieldPartyPose {
    uint32_t flags=1,materialFlags=0,materialLightFlags=0;
    int32_t mode=2,archiveType=0;
    uint64_t enteredMilliseconds=0;
    std::shared_ptr<const ActorAnimationPack> resource;
    ActorAnimationSample shape;
    void paperOff() {flags&=~8u;}
};
struct FieldPartyRegistry {
    std::array<std::unique_ptr<FieldPartyActor>,2> actors;
    std::array<std::unique_ptr<FieldPartyPose>,2> poses;
    uint32_t currentDefinition=0; // Original global kind-record pointer view.
    FieldPartyRegistry()=default;
    FieldPartyRegistry(const FieldPartyRegistry& other):currentDefinition(other.currentDefinition) {
        for(unsigned i=0;i<actors.size();++i)if(other.actors[i])actors[i]=std::make_unique<FieldPartyActor>(*other.actors[i]);
        for(unsigned i=0;i<poses.size();++i)if(other.poses[i])poses[i]=std::make_unique<FieldPartyPose>(*other.poses[i]);
    }
    FieldPartyRegistry& operator=(const FieldPartyRegistry& other) {
        if(this!=&other){FieldPartyRegistry copy(other);actors=std::move(copy.actors);poses=std::move(copy.poses);currentDefinition=copy.currentDefinition;}return *this;
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
    int32_t enterPose(uint32_t sourceResource,int32_t mode,std::shared_ptr<const ActorAnimationPack> bank,uint64_t milliseconds) {
        if(sourceResource!=0x804213e0 || mode!=2 || !bank)return -1;
        const auto* base=bank->pose("PKR_S_1");ActorAnimationSample validation;
        if(!base || !base->evaluate(base->start,validation) || validation.vertices.empty())return -1;
        for(unsigned index=0;index<poses.size();++index)if(!poses[index]) {
            auto pose=std::make_unique<FieldPartyPose>();pose->resource=std::move(bank);pose->enteredMilliseconds=milliseconds;
            pose->shape.positions=base->positions;pose->shape.normals=base->normals;pose->shape.nodes=base->nodes;
            pose->shape.visibility=base->visibility;pose->shape.world.resize(base->groups.size());
            pose->shape.vertices.reserve(base->bindings.size()); // Real workspace; no animation is selected before the authored pose request.
            poses[index]=std::move(pose);return int32_t(index);
        }
        return -1; // Explicit native admission failure; original exhausted/negative-handle domain is not repaired.
    }
};
}
