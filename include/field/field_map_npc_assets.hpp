#pragma once
#include "field_map_npc.hpp"
#include <vector>
namespace ttyd {
class EventAddressSpace;
// Owns parsed original tag words/strings. No inferred parser/history skip gate.
class FieldMapNpcAssets {
public:
 bool load(const char* path);
 bool mapTo(EventAddressSpace&,uint32_t base) const;
 std::optional<PackedMapNpcInput> input(unsigned index,uint32_t base,uint32_t area) const;
 unsigned count() const{return unsigned(bytes_.size()/392);}
private:std::vector<uint8_t> bytes_;bool loaded_=false;
};
}
