#pragma once
#include "field_map_source.hpp"
#include <memory>
namespace ttyd {
struct MapObjectFlagServices {
 FieldMapSourceServices memory;
 std::shared_ptr<const void> lifetime;
 void* user=nullptr;
 bool (*current)(void*)=nullptr;
};
class MapObjectFlagCursor {
public:
 struct State;std::shared_ptr<State> state;
 uint32_t pending()const;
 bool complete()const;
};
// Complete original recursive mapFlgOff8001eee8 / On8001f1fc memory effects.
// Child/sibling traversal is native retained frames, not a guest ISA decoder.
// Budget yields retain the source transfer and do not advance simulation time.
bool resumePackedMapObjectFlag(bool on,uint32_t object,uint32_t mask,int32_t followSiblings,
 const MapObjectFlagServices&,MapObjectFlagCursor&,unsigned transfers=4096);
}
