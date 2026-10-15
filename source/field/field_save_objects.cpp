#include "field_save_objects.hpp"
#include "actor_animation.hpp"
#include <unordered_map>
namespace ttyd {
namespace {
struct Clips {float idleStart,idleEnd,hitStart,hitEnd;};
std::unordered_map<uint32_t,Clips> clips;
bool ready=false;
}
// Select by original MOBJ_SaveBlock identity. Pin S_1 loop[1, 119],
// A_1 nonloop[0, 15]; do not synthesize timing for an absent/mismatched clip.
bool loadFieldSaveObjectAssets(const char* root) {
    if(!root)return false;
    std::unordered_map<uint32_t,Clips> next;
    const auto& models=fieldActorCatalog().models();
    for(uint32_t i=0;i<models.size();++i)if(models[i].originalName=="MOBJ_SaveBlock") {
        ActorAnimationPack pack;
        if(!pack.load((std::string(root)+"/"+models[i].asset+".pmanim").c_str()))return false;
        const auto* idle=pack.pose("S_1");const auto* hit=pack.pose("A_1");
        if(!idle || !hit || !idle->loop || hit->loop || idle->start!=1 || idle->end!=119 || hit->start!=0 || hit->end!=15)return false;
        next.emplace(i,Clips{idle->start,idle->end,hit->start,hit->end});
    }
    if(!loadSaveBlockPresentationAssets(root))return false;
    clips=std::move(next);ready=true;return true;
}
FieldSaveObject* FieldSaveObjects::mutableFind(uint32_t owner) {
    for(auto& object:objects)if(object.state.owner==owner)return &object;
    return nullptr;
}
const FieldSaveObject* FieldSaveObjects::find(uint32_t owner) const {
    for(const auto& object:objects)if(object.state.owner==owner)return &object;
    return nullptr;
}
// Catalog/room change while locked triggers native recovery and returns;
// it does not install new instances in the same call or emulate source teardown.
void FieldSaveObjects::configure(uint32_t nextRoom,std::optional<int32_t> story,uint32_t interior,uint32_t system,uint64_t time) {
    if(!ready)return;
    now=time;const auto g=fieldActorCatalogGeneration();
    if(room!=nextRoom || generation!=g) {
        if(locked()){recover();return;} // Native owner recovery, not a source teardown claim.
        objects.clear();headCache.previous.clear();room=nextRoom;generation=g;
        for(const auto& actor:fieldActorCatalog().actors())if(actor.room==room && actor.kind==1 && clips.count(actor.model)) {
            FieldSaveObject object;object.actor=actor;object.state.owner=actor.key;
            // 8030b4ec is original save system event; owner key stays catalog identity.
            object.state.systemEvent=0x8030b4ec;object.restartedAt=now;
            objects.push_back(std::move(object));
        }
    }
    for(auto& object:objects) {
        if(object.actor.visible(room,story,interior,system))object.state.flags|=1;
        else object.state.flags&=~1u;
    }
}
// Failure recovery releases native controls/camera200 and presentation;
// it is not original card error handling or a successful story/save result.
void FieldSaveObjects::recover() {
    failed=true;keys={};systemLevel=0;camera8Flags&=~0x200u;
    presentation.message={};presentation.selection={};
    for(auto& object:objects){object.state.stage=0;object.state.flags&=~8u;}
}
// Presentation/window runs before object callbacks. Copy switches only if
// it was active on entry; clear this visit's sound queue before new producers.
void FieldSaveObjects::tickEvents(uint64_t time,const MessageSelectionInput& input) {
    now=time;sounds.clear();
    const bool running=presentation.active();presentation.tick(now,input);
    if(running)switches=presentation.event.switches;
    if(presentation.failed || presentation.event.failed)recover();
}
MapObjectServices FieldSaveObjects::services() {
    MapObjectServices s;s.user=this;
    s.key=[](void* p,bool enabled){auto& self=*static_cast<FieldSaveObjects*>(p);if(enabled)self.keys.on();else self.keys.off();};
    s.system=[](void* p,int32_t mode){static_cast<FieldSaveObjects*>(p)->systemLevel=mode;};
    s.sound=[](void* p,uint32_t,uint32_t id){static_cast<FieldSaveObjects*>(p)->sounds.push_back(id);};
    s.animation=[](void* p,uint32_t owner,const char* pose,bool restart) {
        auto& self=*static_cast<FieldSaveObjects*>(p);auto* object=self.mutableFind(owner);
        if(!object){self.failed=true;return;}
        if(restart || object->pose!=pose){object->pose=pose;object->restartedAt=self.now;object->ratio=0;}
    };
    s.loopRatio=[](void* p,uint32_t owner){const auto* o=static_cast<FieldSaveObjects*>(p)->find(owner);return o?o->ratio:0.f;};
    s.cameraFlag=[](void* p,uint32_t camera,uint16_t mask,bool enabled) {
        auto& self=*static_cast<FieldSaveObjects*>(p);
        if(camera!=8){self.failed=true;return;}
        if(enabled)self.camera8Flags|=mask;else self.camera8Flags&=uint16_t(~mask);
    };
    s.eventCheck=[](void* p,int32_t id)->int32_t {
        auto& self=*static_cast<FieldSaveObjects*>(p);return id && id==self.eventId && self.presentation.active()?1:0;
    };
    // Only original8030b4ec priority30/type26. A failed presentation marks
    // native failure; caller later recovers rather than acknowledging a save.
    s.eventEntry=[](void* p,uint32_t script,uint32_t owner,uint8_t priority,uint8_t type) {
        auto& self=*static_cast<FieldSaveObjects*>(p);
        if(script!=0x8030b4ec || priority!=30 || type!=26 || !self.presentation.begin(self.now,self.switches,owner,self.keys)) {
            self.failed=true;return int32_t(0);
        }
        self.nextEventId=self.nextEventId==INT32_MAX?1:self.nextEventId+1;
        return self.eventId=self.nextEventId;
    };
    return s;
}
// Head-name cache deduplicates semantic HOBJ name, not pointer/owner changes.
// Static head contact replaces cache without triggering a save object.
// Produce hit -> callback -> animation Main is required per-object order;
// ratio is unbounded beyond clip end and uses raw elapsed60Hz frame, not start+X.
void FieldSaveObjects::tickObjects(uint64_t time,uint32_t headOwner,uint32_t staticHead,int32_t motion) {
    if(!ready || failed)return;
    now=time;uint32_t newOwner=0;
    if(const auto* object=find(headOwner)) {
        if(headCache.publish("HOBJ_"+object->actor.sourceName))newOwner=headOwner;
    } else if(staticHead)headCache.publish("static:"+std::to_string(staticHead));
    else headCache.publish({});
    const auto s=services();
    for(auto& object:objects) {
        produceSaveBlockHead(object.state,{newOwner,0,motion,systemLevel});
        advanceSaveBlock(object.state,s);
        // Main publication after callback: raw X, not start+X; PMANIM evaluates
        // clip start/loop/clamp. Ratio remains unbounded after nonloop completion.
        const auto& c=clips.at(object.actor.model);
        const float x=float(60.f*float(now-object.restartedAt)/1000.f);
        const bool hit=object.pose=="A_1";const float start=hit?c.hitStart:c.idleStart,end=hit?c.hitEnd:c.idleEnd;
        object.frame=x;object.ratio=x<start?0.f:(x-start)/(end-start);
    }
    if(failed)recover();
}
}
