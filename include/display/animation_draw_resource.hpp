#pragma once
#include <array>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <string>
#include <vector>
namespace ttyd {
struct AnimationDrawGroup {int32_t sibling=-1,child=-1,shape=-1;uint32_t visibility=0,node=0,joint=0;};
// Original serialized MODEL offsets, never executable or relocated CPU pointers.
// Import validation bounds source tables; runtime retains their source order.
class AnimationDrawResource {
public:
    static std::shared_ptr<const AnimationDrawResource> load(const char*);
    bool read(FILE*);
    uint32_t word(uint32_t offset) const;
    float real(uint32_t offset) const;
    uint8_t byte(uint32_t offset) const {return data_.at(offset);}
    bool span(uint32_t offset,uint32_t count,uint32_t stride) const;
    const std::string& modelName() const{return model_;}
    const std::string& textureName() const{return texture_;}
    uint32_t groups() const{return word(0x144);}
    AnimationDrawGroup group(uint32_t) const;
    std::string descriptor(uint32_t) const;
private:
    std::vector<uint8_t> data_;std::string model_,texture_;
};
}
