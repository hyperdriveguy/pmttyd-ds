#pragma once
#include "battle_class_surface_gpu.hpp"
#include "battle_class_backend.hpp"
namespace ttyd {
class BattleSubmenuSurfaceGpuResource;
class BattleSubmenuSurfaceGeneration {
public:
 ~BattleSubmenuSurfaceGeneration()=default;
 BattleSubmenuSurfaceGeneration(const BattleSubmenuSurfaceGeneration&)=delete;
 BattleSubmenuSurfaceGeneration& operator=(const BattleSubmenuSurfaceGeneration&)=delete;
private:
 BattleSubmenuSurfaceGeneration()=default;bool resident_=false;const void* texture_=nullptr;
 // Immutable rows for original indices 2 and 3; native texture slot stays 0.
 std::array<BattleClassRegion,2> regions_{};
 friend class BattleSubmenuSurfaceGpuResource;friend struct BattleSubmenuSurfaceLease;
};
// Strong immutable catalog/generation pin plus exact live native texture.
// Address equality without the still-resident generation cannot admit reuse.
struct BattleSubmenuSurfaceLease {
 std::shared_ptr<const BattleSubmenuSurfaceGeneration> generation;
 const void* texture=nullptr;
 bool current()const;
 const BattleClassRegion* surface(uint32_t originalIndex)const;
};
// Actual original indices2/3 only. Atomic one-image import, exact immutable
// catalog, full-image native UV validation and invalidation before release.
class BattleSubmenuSurfaceGpuResource {
public:
 BattleSubmenuSurfaceGpuResource()=default;~BattleSubmenuSurfaceGpuResource();
 BattleSubmenuSurfaceGpuResource(const BattleSubmenuSurfaceGpuResource&)=delete;
 BattleSubmenuSurfaceGpuResource& operator=(const BattleSubmenuSurfaceGpuResource&)=delete;
 bool load(const char* root,const BattleClassSurfaceTextureServices&);
 void retire();BattleSubmenuSurfaceLease lease()const;
 bool accepts(const BattleSubmenuSurfaceLease&)const;
private:
 BattleSubmenuSurfaceLease lease_;std::shared_ptr<BattleSubmenuSurfaceGeneration> generation_;
 // Retained callbacks own the actual image until explicit retirement/destruction.
 BattleClassSurfaceTextureServices owner_{};bool invoking_=false,retirePending_=false;
};
// Prepare outside FrameBegin; import/release synchronizes live C3D. Host
// wrapper deliberately stays unavailable; host descriptor services test policy.
class NativeBattleSubmenuSurfaceGpu {
public:
 NativeBattleSubmenuSurfaceGpu();~NativeBattleSubmenuSurfaceGpu();
 NativeBattleSubmenuSurfaceGpu(const NativeBattleSubmenuSurfaceGpu&)=delete;
 NativeBattleSubmenuSurfaceGpu& operator=(const NativeBattleSubmenuSurfaceGpu&)=delete;
 bool load(const char* root);void release();const BattleSubmenuSurfaceGpuResource& resource()const;
#ifdef __3DS__
 C3D_Tex* texture(const BattleSubmenuSurfaceLease&);
#endif
private:struct State;std::unique_ptr<State> state_;
};
}
