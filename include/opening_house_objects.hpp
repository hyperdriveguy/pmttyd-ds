#pragma once
#include <array>
#include <cstdint>
#include <functional>
#include <vector>
namespace ttyd {
// Original aaa S_ie_door/S_posuto bindings. Coordinates belong to the converted
// map, not field collision. Recomputed from retained geometry on every update.
class OpeningHouseObjects {
public:
    using Point=std::array<float,3>;
    using Writer=std::function<void(uint32_t,const Point&)>;
    bool load(const char* path,uint32_t vertexCount);
    void apply(float doorParameter,float postboxParameter,const Writer&) const;
    void clear(){objects.clear();}
    size_t size() const{return objects.size();}
    size_t bindings() const;
private:
    struct Binding {uint32_t index;Point local,original;};
    struct Object {uint32_t tag;std::array<float,12> world;std::vector<Binding> bindings;};
    std::vector<Object> objects;
};
}
