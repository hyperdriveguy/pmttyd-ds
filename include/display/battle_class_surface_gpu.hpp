#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#ifdef __3DS__
#include <citro3d.h>
#endif
namespace ttyd {
class BattleClassAssets;
class BattleClassSurfaceGpuResource;
class BattleClassSurfaceGeneration {
public:
    ~BattleClassSurfaceGeneration()=default;
    BattleClassSurfaceGeneration(const BattleClassSurfaceGeneration&)=delete;
    BattleClassSurfaceGeneration& operator=(const BattleClassSurfaceGeneration&)=delete;
private:
    BattleClassSurfaceGeneration()=default;
    bool resident_=false;
    std::shared_ptr<const BattleClassAssets> assets_;
    std::array<const void*,2> textures_{};
    friend class BattleClassSurfaceGpuResource;
    friend struct BattleClassSurfaceLease;
};
// Strong refs pin catalog/epoch identity only. They do not extend GPU pixel
// lifetime; owner retirement invalidates all copied leases before native free.
struct BattleClassSurfaceLease {
    std::shared_ptr<const BattleClassAssets> assets;
    std::shared_ptr<const BattleClassSurfaceGeneration> generation;
    std::array<const void*,2> textures{};
    bool current()const;
};
// Whole-image Tex3DS import facts. Native UVs must be exactly the unrotated
// full image: L0,T1,R1,B0. Format is PICA RGBA8=0, not source TPL format.
struct BattleClassSurfaceImport {
    const void* identity=nullptr;
    uint32_t width=0,height=0,format=0,count=0,subWidth=0,subHeight=0;
    bool rotated=false;
    std::array<float,4> uv{};
};
// Each nonnull imported identity is one acquisition owed a matching release
// at the same slot, including rejected descriptors and an aliased second import.
struct BattleClassSurfaceTextureServices {
    void* user=nullptr;
    std::optional<BattleClassSurfaceImport> (*import)(void*,uint32_t slot,const char*)=nullptr;
    void (*release)(void*,uint32_t slot,const void*)=nullptr;
};
// Load publishes one atomic pair only after exact catalog/import admission.
// Failed second import releases both acquired images. Explicit retirement
// invalidates every copied lease BEFORE either native release callback.
class BattleClassSurfaceGpuResource {
public:
    BattleClassSurfaceGpuResource()=default;
    ~BattleClassSurfaceGpuResource();
    BattleClassSurfaceGpuResource(const BattleClassSurfaceGpuResource&)=delete;
    BattleClassSurfaceGpuResource& operator=(const BattleClassSurfaceGpuResource&)=delete;
    bool load(const char* root,const BattleClassSurfaceTextureServices&);
    void retire();
    bool ready()const;
    BattleClassSurfaceLease lease()const;
    bool accepts(const BattleClassSurfaceLease&)const;
private:
    BattleClassSurfaceLease lease_;
    std::shared_ptr<BattleClassSurfaceGeneration> generation_;
    BattleClassSurfaceTextureServices services_;
    // Callback guard serializes ownership; pending retirement cancels admission.
    bool invoking_=false,retirePending_=false;
};
// Actual import/release require C3D alive and execution outside FrameBegin.
// Host load is unavailable. The retained owner must outlive native callbacks.
class NativeBattleClassSurfaceGpu {
public:
    NativeBattleClassSurfaceGpu();~NativeBattleClassSurfaceGpu();
    NativeBattleClassSurfaceGpu(const NativeBattleClassSurfaceGpu&)=delete;
    NativeBattleClassSurfaceGpu& operator=(const NativeBattleClassSurfaceGpu&)=delete;
    bool load(const char* root);
    void release();
    const BattleClassSurfaceGpuResource& resource()const;
#ifdef __3DS__
    // Read-only lookup after whole-batch preflight; never imports lazily.
    C3D_Tex* texture(uint32_t slot,const BattleClassSurfaceLease&);
#endif
private:struct State;std::unique_ptr<State> state_;
};
}
