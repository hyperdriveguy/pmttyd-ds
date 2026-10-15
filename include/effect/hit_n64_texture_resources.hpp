#pragma once
#include <array>
#include <cstdint>
#include <cstdio>
#include <memory>
#include <optional>
namespace ttyd {
// Private native metadata; descriptor/vertices remain original BE bytes.
class HitN64TextureResources {
public:
    static std::shared_ptr<const HitN64TextureResources> load(const char*);
    bool read(FILE*);
    bool ready()const{return ready_;}
    const std::array<uint8_t,36>& descriptor()const{return descriptor_;}
    const std::array<uint8_t,280>& vertices()const{return vertices_;}
    // Only immutable original Hit tables/constants/vertices. No actual pool,
    // camera, state64 globals, palette pointers or BSS storage are manufactured.
    std::optional<uint8_t> byte(uint32_t address)const;
    std::optional<uint32_t> word(uint32_t address)const;
private:
    bool ready_=false;std::array<uint8_t,36> descriptor_{};std::array<uint8_t,280> vertices_{};
    std::array<uint8_t,48> rodata_{};std::array<uint8_t,52> tables_{};std::array<uint8_t,40> scalars_{};
};
// Real 3DS texture ownership only. Host load remains unavailable. Import and
// retirement occur outside FrameBegin; release requires C3D still alive.
class NativeHitN64Texture {
public:
    NativeHitN64Texture();~NativeHitN64Texture();
    NativeHitN64Texture(const NativeHitN64Texture&)=delete;
    NativeHitN64Texture& operator=(const NativeHitN64Texture&)=delete;
    bool load(const char* metadata,const char* texture);
    void release();bool ready()const;
    uint64_t generation()const;
    std::shared_ptr<const HitN64TextureResources> resource()const;
    bool current(const std::shared_ptr<const HitN64TextureResources>&,uint64_t)const;
    // Called only after the whole owning GPU batch has been preflighted.
    bool bind(uint32_t slot,const std::shared_ptr<const HitN64TextureResources>&,uint64_t);
private:struct State;std::unique_ptr<State> state_;
};
}
