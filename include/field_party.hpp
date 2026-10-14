#pragma once
#include "field_battle.hpp"
#include "actor_animation.hpp"
#include <array>
#include <cassert>
#include <memory>
#include <cstring>

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
    float scalar(unsigned offset) const {const auto bits=word(offset);float value;std::memcpy(&value,&bits,4);return value;}
    void scalar(unsigned offset,float value) {uint32_t bits;std::memcpy(&bits,&value,4);word(offset,bits);}
};
// Converted original Mario/partner resources and native CPU shape ownership. These
// handles belong to this field registry; GPU/time/ABI equivalence is separate.
struct FieldPartyPose {
    uint32_t flags=1,materialFlags=0,materialLightFlags=0;
    uint32_t sourceResource=0;
    int32_t mode=2,archiveType=0;
    uint64_t enteredMilliseconds=0;
    std::shared_ptr<const ActorAnimationPack> resource;
    ActorAnimationSample shape;
    void paperOff() {flags&=~8u;}
};
struct FieldPartyRegistry {
    std::array<std::unique_ptr<FieldPartyActor>,2> actors;
    std::array<std::unique_ptr<FieldPartyPose>,5> poses; // Used native domain: three Mario owners and two partner owners.
    uint32_t currentDefinition=0; // Original global kind-record pointer view.
    uint32_t goombellaInitWork=0; // Owned view of8041eae0, cleared by the used kuribo_init callback.
    FieldPartyRegistry()=default;
    FieldPartyRegistry(const FieldPartyRegistry& other):currentDefinition(other.currentDefinition),goombellaInitWork(other.goombellaInitWork) {
        for(unsigned i=0;i<actors.size();++i)if(other.actors[i])actors[i]=std::make_unique<FieldPartyActor>(*other.actors[i]);
        for(unsigned i=0;i<poses.size();++i)if(other.poses[i])poses[i]=std::make_unique<FieldPartyPose>(*other.poses[i]);
    }
    FieldPartyRegistry& operator=(const FieldPartyRegistry& other) {
        if(this!=&other){FieldPartyRegistry copy(other);actors=std::move(copy.actors);poses=std::move(copy.poses);currentDefinition=copy.currentDefinition;goombellaInitWork=copy.goombellaInitWork;}return *this;
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
        const char* shape=nullptr;
        switch(sourceResource) {
        case 0x804213e0:shape="PKR_S_1";break;
        case 0x802c18a0:shape="M_S_1";break;
        case 0x802c18a8:shape="M_S_1R";break;
        case 0x802c18b4:shape="M_I_5";break;
        default:return -1;
        }
        if(mode!=2 || !bank)return -1;
        const auto* base=bank->pose(shape);ActorAnimationSample validation;
        if(!base || !base->evaluate(base->start,validation) || validation.vertices.empty())return -1;
        for(unsigned index=0;index<poses.size();++index)if(!poses[index]) {
            auto pose=std::make_unique<FieldPartyPose>();pose->resource=std::move(bank);pose->sourceResource=sourceResource;pose->enteredMilliseconds=milliseconds;
            pose->shape.positions=base->positions;pose->shape.normals=base->normals;pose->shape.nodes=base->nodes;
            pose->shape.visibility=base->visibility;pose->shape.world.resize(base->groups.size());
            pose->shape.vertices.reserve(base->bindings.size()); // Real workspace; no animation is selected before the authored pose request.
            poses[index]=std::move(pose);return int32_t(index);
        }
        return -1; // Explicit native admission failure; original exhausted/negative-handle domain is not repaired.
    }
    FieldPartyPose* pose(int32_t handle) const {
        return handle>=0 && unsigned(handle)<poses.size()?poses[unsigned(handle)].get():nullptr;
    }
    bool initializeMario(FieldMarioBattleState& mario,std::shared_ptr<const ActorAnimationPack> bank,uint64_t milliseconds) {
        constexpr std::array<uint32_t,3> resources{{0x802c18a0,0x802c18a8,0x802c18b4}};
        for(unsigned i=0;i<resources.size();++i) {
            mario.poseHandles[i]=enterPose(resources[i],2,bank,milliseconds);
            if(mario.poseHandles[i]<0)return false;
        }
        return true;
    }
    void reinitializeMario(const FieldMarioBattleState& mario) {
        for(auto handle:mario.poseHandles)if(auto* owner=pose(handle))owner->materialFlags=0;
        for(auto handle:mario.poseHandles)if(auto* owner=pose(handle))owner->materialLightFlags|=2;
    }
    bool requestFreshGoombellaPose(int32_t index,int16_t hp) {
        auto* actor=pointer(index);
        if(!actor || actor->bytes[0x31]!=1 || (actor->word(4)&0x8002) || actor->word(0x18))return false;
        // Fresh Main has a null name. Its following repeat comparison sees
        // the same original string and leaves the admitted request untouched.
        actor->word(0x18,hp<=5?0x802cb52c:0x802cb4dc);
        actor->bytes[0x20]=actor->bytes[0x21]=0;
        actor->word(8,actor->word(8)|0x2000);actor->word(0x1c,0);
        return true; // Borrowed source string/pending request, not animation playback.
    }
    bool requestGoombellaPose(int32_t index,int16_t hp) {
        auto* actor=pointer(index);if(!actor || actor->bytes[0x31]!=1 || (actor->word(4)&0x8000))return false;
        if(actor->word(4)&2)return true;
        const uint32_t old=actor->word(0x18),selected=hp<=5?0x802cb52cu:0x802cb4dcu;
        if(old && old!=0x802cb52c && old!=0x802cb4dc)return false; // Unknown borrowed string requires its comparison provider.
        if(old!=selected) {
            actor->word(0x18,selected);actor->bytes[0x20]=actor->bytes[0x21]=0;
            actor->word(8,actor->word(8)|0x2000);
        }
        return true;
    }
    void kuriboInit(int32_t index) {
        auto* actor=pointer(index);assert(actor);
        actor->bytes[0x33]=1;actor->bytes[0x32]=0;
        actor->word(0xf0,0x41c00000);actor->word(0xf4,0x41d00000); // Original binary32 dimensions24/26.
        goombellaInitWork=0;
    }
};
}
