#pragma once
#include <array>
#include <cstdint>
#include <vector>
namespace ttyd {
// UV matrix is2x3 affine (6 floats); mesh matrix3x4 (12 floats), row-major.
// Bindings contain destination vertex index and ORIGINAL UV/XYZ values.
// Native sample counts/periods are integer ticks; event activation is separate.
struct UVBinding {uint32_t vertex;float u,v;};
struct UVTrack {
    std::vector<std::array<float,6>> frames;
    std::vector<UVBinding> bindings;
};
struct MeshBinding {uint32_t vertex;float x,y,z;};
struct MeshTrack {
    std::vector<std::array<float,12>> frames;
    std::vector<MeshBinding> bindings;
};
inline std::array<float,3> transformedPosition(const std::array<float,12>& m,const MeshBinding& b) {
    return {m[0]*b.x+m[1]*b.y+m[2]*b.z+m[3],m[4]*b.x+m[5]*b.y+m[6]*b.z+m[7],m[8]*b.x+m[9]*b.y+m[10]*b.z+m[11]};
}
inline std::array<float,2> transformedUV(const std::array<float,6>& m,const UVBinding& b) {
    return {m[0]*b.u+m[1]*b.v+m[2],1-(m[3]*b.u+m[4]*b.v+m[5])};
}
bool loadMeshAnimation(const char* path,uint32_t vertices,std::vector<MeshTrack>& result);
std::array<float,3> animatedPosition(const MeshTrack&,const MeshBinding&,uint64_t ticks);
bool loadUVAnimation(const char* path,uint32_t vertices,std::vector<UVTrack>& result);
std::array<float,2> animatedUV(const UVTrack&,const UVBinding&,uint64_t ticks);
}
