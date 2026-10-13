#pragma once
#include "field_actors.hpp"
#include "save_block_presentation.hpp"
namespace ttyd {
bool loadFieldSaveObjectAssets(const char* root);
struct FieldSaveObject {
    MapObjectState state;
    FieldActorRecord actor;
    std::string pose="S_1";
    uint64_t restartedAt=0;
    float frame=0,ratio=0;
};
// Room-owned instances selected by original model identity, not room number.
// Event/window processing precedes Mario; object/animation processing follows it.
class FieldSaveObjects {
public:
    void configure(uint32_t room,std::optional<int32_t> story,uint32_t interior,uint32_t system,uint64_t now);
    void tickEvents(uint64_t now,const MessageSelectionInput&);
    void tickObjects(uint64_t now,uint32_t headOwner,uint32_t staticHead,int32_t motion);
    bool locked() const{return keys.depth || systemLevel || presentation.active();}
    const FieldSaveObject* find(uint32_t owner) const;
    const std::vector<FieldSaveObject>& instances() const{return objects;}
    MapObjectKeyControl keys;
    int32_t systemLevel=0;
    uint16_t camera8Flags=0; // Separate from the field camera4 owner.
    std::vector<uint32_t> sounds; // Original IDs; no synthetic fallback.
    SaveBlockPresentation presentation;
    SaveBlockSwitches switches;
    bool failed=false;
private:
    std::vector<FieldSaveObject> objects;
    MapObjectHeadCache headCache;
    uint32_t room=UINT32_MAX;
    uint64_t generation=UINT64_MAX,now=0;
    int32_t eventId=0,nextEventId=0;
    FieldSaveObject* mutableFind(uint32_t);
    MapObjectServices services();
    void recover();
};
}
