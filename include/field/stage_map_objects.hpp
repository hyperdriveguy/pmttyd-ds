#pragma once
#include <array>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <vector>
namespace ttyd {
// Native source-address-independent owner of original map names/hierarchy.
// Local matrices/vertex bindings come from the original converted map.
class StageMapObjects {
public:
    using Point=std::array<float,3>;
    using Matrix=std::array<float,12>;
    using Writer=std::function<void(uint32_t,const Point&)>;
    bool load(const char*);
    bool translate(const std::string&,const Point&);
    bool rotate(const std::string&,const Point&);
    bool flag(const std::string&,uint32_t mask,bool on);
    std::optional<uint32_t> flags(const std::string&) const;
    const char* root() const;
    void recalculate(); // Explicit native map-frame owner, never a renderer latch reset.
    void apply(const Writer&) const;
    void visibleRanges(uint32_t first,uint32_t count,const std::function<void(uint32_t,uint32_t)>&) const;
    uint32_t vertices() const{return vertexCount;}
    uint64_t revision() const{return generation;}
    const Matrix* world(const std::string&) const;
private:
    struct Binding {uint32_t index;Point local;};
    struct Object {std::string name;int32_t parent=-1;uint32_t flags=0;Matrix local{},additional{},world{};std::vector<Binding> bindings;};
    std::vector<Object> objects;
    std::vector<uint32_t> owners;
    uint32_t vertexCount=0;
    uint64_t generation=0;
    bool dirty=true;
    mutable bool visibilityDirty=true;
    mutable std::vector<std::array<uint32_t,2>> visible;
    void refreshVisibility() const;
    Object* find(const std::string&);
    const Object* find(const std::string&) const;
    void concatenate(Object&,const Matrix&);
};
}
