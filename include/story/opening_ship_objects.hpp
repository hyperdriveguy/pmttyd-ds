#pragma once
#include "opening_house_objects.hpp"
namespace ttyd {
class OpeningShipObjects {
public:
 using Point=OpeningHouseObjects::Point;using Writer=OpeningHouseObjects::Writer;
 // Seven converted hierarchy groups, with original local/world/inverse data.
 // Overlap between parent/child vertex groups is intentional. Remap validates
 // index bounds, not uniqueness or arbitrary hierarchy identity; caller owns that.
 bool load(const char*,uint32_t);
 bool remapVertices(const std::vector<uint32_t>& originalToLocal,uint32_t localCount);
 void clear(){objects.clear();childPositions.clear();hasChild.clear();}
 // Const calls reuse mutable vertex scratch, not simulation clocks. Serialize
 // writers; Voyage and harbor use distinct wake Y/yaw policy. Source translations
 // divide10 before composing through original object matrices.
 void applyVoyage(const Point&,float yaw,float rock,const std::array<float,4>& parts,const Writer&) const;
 void apply(float x,float y,float wakeX,float yaw,float rock,const std::array<float,4>& parts,const Writer&) const;
private:
 void applyMatrices(float,float,float,float,float,const std::array<float,4>&,const Writer&,float,float,float) const;
 struct Binding{uint32_t index;Point local,original;};
 struct Object{std::array<float,12> world,inverse;std::vector<Binding> bindings;};
 std::vector<Object> objects;
 mutable std::vector<Point> childPositions;
 std::vector<uint8_t> hasChild;
};
}
