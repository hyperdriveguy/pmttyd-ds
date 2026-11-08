#pragma once
#include "field_map_source.hpp"
#include <memory>
namespace ttyd {
struct MapGroupFlagRequest {uint32_t callsite=0;unsigned count=0;FieldMapSourceCall source;};
struct MapGroupFlagServices {
 FieldMapSourceServices memory;
 std::shared_ptr<const void> lifetime;
 void* user=nullptr;
 bool (*current)(void*)=nullptr;
 bool (*call)(void*,MapGroupFlagRequest&)=nullptr;
};
class MapGroupFlagCursor {
public:
 struct State;std::shared_ptr<State> state;
 uint32_t pending()const;
 bool complete()const;
};
// Whole original mapGrpFlagOn8001edd0 / Off8001ecb8 parents. Actual source
// MapObj scan/deferred-name queue; deep mapFlg and libc children stay linked.
bool resumePackedMapGroupFlag(bool on,uint32_t name,uint32_t mask,
 const MapGroupFlagServices&,MapGroupFlagCursor&);
}
