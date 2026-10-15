#pragma once
#include "hit_n64_effect_draw.hpp"
#include "hit_n64_texture_resources.hpp"
#include <vector>
namespace ttyd {
struct HitN64GpuVertex {
    std::array<float,3> position{};
    std::array<float,2> uv0{},uv1{};
    uint32_t sourceColor=0; // Retained although source NumChans(0) ignores it.
};
// One six-vertex source triangle pair,with actual reached view/model matrix,
// both independent UV transforms and GX register colors. Preserve callback order.
struct HitN64GpuPacket {
    BattleActMatrix matrix{};
    std::array<HitN64GpuVertex,6> vertices{};
    uint32_t color0=0,color1=0;
};
struct HitN64GpuLifetime {
    std::shared_ptr<const void> owner;
    void* user=nullptr;
    // Must check actual effect allocation AND camera/frame/inherited-state
    // epochs. No implicit process/global lifetime is supplied by this module.
    bool (*current)(void*)=nullptr;
    bool (*textureCurrent)(void*,const std::shared_ptr<const HitN64TextureResources>&,uint64_t)=nullptr;
};
// current() permanently retires on first lifetime/texture loss;restored flags
// or reused allocation cannot revive it. Owner/resource pins are not epoch proof.
class HitN64GpuBatch {
public:
    bool current()const;
    const std::vector<HitN64GpuPacket>& packets()const{return packets_;}
    const std::shared_ptr<const HitN64TextureResources>& resource()const{return resource_;}
    uint64_t generation()const{return generation_;}
private:
    mutable bool retired_=false;
    HitN64GpuLifetime lifetime_;
    std::shared_ptr<const HitN64TextureResources> resource_;
    uint64_t generation_=0;
    std::vector<HitN64GpuPacket> packets_;
    friend class HitN64GpuCapture;
};
// Retains the COMPLETE original callback and captures each six-vertex draw in
// source order (including the three passes). Only camera/GetTexObj queries are
// delegated to source.call. That provider must own genuine source objects.
class HitN64GpuCapture {
public:
    HitN64GpuCapture();~HitN64GpuCapture();
    bool begin(std::shared_ptr<const HitN64TextureResources>,uint64_t,const HitN64GpuLifetime&);
    // Preparation only: true is NOT queued display completion. The outer
    // retained display owner must submit NativeHitN64Gpu::draw before ack.
    bool resume(int32_t camera,uint32_t effect,const HitN64DrawServices&);
    std::shared_ptr<const HitN64GpuBatch> publication()const;
    void discard();uint32_t pending()const;
private:struct State;std::unique_ptr<State> state_;
};
// The shader ABI is hit_n64.v.pica: XYZ, UV0, UV1, constant register0 RGBA.
// The inherited raster owner preflights viewport/projection/blend/depth/alpha
// and parks unused sampler2. apply() cannot fail and must not alter shader/TEV.
// Import, prepare and release are outside FrameBegin while C3D remains alive.
struct HitN64NativeRaster {
    void* user=nullptr;
    bool (*preflight)(void*)=nullptr;
    void (*apply)(void*)=nullptr;
    std::array<float,16> projection{}; // Actual source-view to native clip.
};
// Fixed linear arena capacity is native admission,not a source particle cap.
// Same-frame draws consume distinct ranges;beginFrame syncs before reuse.
// CPU preparation/publication,preflight and actual submission remain distinct.
class NativeHitN64Gpu {
public:
    NativeHitN64Gpu();~NativeHitN64Gpu();
    bool prepare(const uint32_t* shader,uint32_t bytes,size_t maximumPackets);
    // Outside FrameBegin: waits for prior GPU use before resetting the arena.
    bool beginFrame();
    bool preflight(const HitN64GpuBatch&,const NativeHitN64Texture&,const HitN64NativeRaster&)const;
    bool draw(const HitN64GpuBatch&,NativeHitN64Texture&,const HitN64NativeRaster&);
    void release();
private:struct State;std::unique_ptr<State> state_;
};
}
