#pragma once
#include "animation_texture.hpp"
#include "battle_class_backend.hpp"
#include "battle_texture_plane.hpp"
#include <string>
#ifdef __3DS__
#include <citro3d.h>
#include <tex3ds.h>
#endif
namespace ttyd {
// Native atlas admission uses PICA RGBA8=0; original bank images are CMPR=14.
struct BattleEnemyHpTextureAdmission {const void* identity=nullptr;uint32_t width=0,height=0,format=0;};
struct BattleEnemyHpTextureServices {
 void* user=nullptr;
 std::optional<BattleEnemyHpTextureAdmission>(*import)(void*,const char*)=nullptr;
 void(*release)(void*,const void*)=nullptr;
};
enum class BattleEnemyHpMaterial {TexturedModulate,TexturedRaster,UntexturedRaster};
struct BattleEnemyHpGpuQuad {
 const void* texture=nullptr;
 // Shared identity token keeps an epoch alive, not the borrowed GPU allocation.
 std::shared_ptr<const uint8_t> generation;
 uint32_t index=0;
 BattleEnemyHpMaterial material=BattleEnemyHpMaterial::TexturedModulate;
 std::array<BattleClassVertex,4> vertices{};
};
struct BattleEnemyHpQuadSink {void* user=nullptr;bool(*append)(void*,const BattleEnemyHpGpuQuad&)=nullptr;};
// Mutable source build queue; copy its packets into immutable publication only
// when the real queued callback completes. No render-driven source mutation.
struct BattleEnemyHpGpuBatch {
 std::vector<BattleEnemyHpGpuQuad> quads;
 BattleEnemyHpQuadSink sink();
};
// Explicit retire is required; this portable resource has no releasing destructor.
// Load/retire/import/release must be externally serialized and non-reentrant.
class BattleEnemyHpGpuResource {
public:
 BattleEnemyHpGpuResource()=default;
 BattleEnemyHpGpuResource(const BattleEnemyHpGpuResource&)=delete;
 BattleEnemyHpGpuResource& operator=(const BattleEnemyHpGpuResource&)=delete;
 bool load(const char* root,const BattleEnemyHpTextureServices&);
 void retire();
 bool ready()const{return admission_.identity!=nullptr;}
 const void* identity()const{return admission_.identity;}
 const std::shared_ptr<const uint8_t>& generation()const{return generation_;}
 const BattleClassRegion* surface(uint32_t index)const;
 bool accepts(const BattleEnemyHpGpuQuad&)const;
 bool quad(uint32_t index,BattleEnemyHpMaterial,const BattleActMatrix&,
     const std::array<std::array<float,3>,4>&,
     const std::array<std::array<float,2>,4>&,const std::array<uint32_t,4>&,
     const BattleEnemyHpQuadSink&)const;
private:
 std::array<BattleClassRegion,2> regions_{};
 BattleEnemyHpTextureAdmission admission_;
 BattleEnemyHpTextureServices owner_;
 std::shared_ptr<const uint8_t> generation_;
};
bool validateBattleEnemyHpBatch(const BattleEnemyHpGpuBatch&,const BattleEnemyHpGpuResource*);
struct BattleEnemyHpMemoryReader {void* user=nullptr;std::optional<uint32_t>(*word)(void*,uint32_t)=nullptr;};
// Actual relocated common TPL, actual live B/FileEntry/a0/pointer-cell chain.
std::optional<AnimationTPLImage> readBattleEnemyHpTexture(const BattleEnemyHpMemoryReader&,uint32_t palette,uint32_t index);
struct BattleEnemyHpQueryServices {
 void* user=nullptr;
 std::optional<uint32_t>(*currentCamera)(void*)=nullptr;
 std::optional<AnimationTPLImage>(*texture)(void*,uint32_t,uint32_t)=nullptr;
};
// Canonical original HP state/GX/FIFO owner. A final lane is acknowledged only
// after its whole quad is copied by the ordered sink. Unavailable admission
// retains all earlier lanes and source state; no partial quad is published.
class BattleEnemyHpGpuProvider {
public:
 BattleEnemyHpGpuProvider(const BattleEnemyHpGpuResource&,BattleEnemyHpQueryServices,BattleEnemyHpQuadSink);
 std::optional<uint32_t> request(const BattleTexturePlaneRequest&);
private:
 const BattleEnemyHpGpuResource& resource_;
 const std::shared_ptr<const uint8_t> generation_;
 BattleEnemyHpQueryServices query_;BattleEnemyHpQuadSink sink_;
 std::array<std::optional<AnimationTPLImage>,2> objects_{};
 std::optional<uint32_t> texture_;
 std::optional<BattleActMatrix> matrix_;
 std::optional<BattleEnemyHpMaterial> material_;
 // Masks track observed setters; generators_=2 is an unconfigured sentinel.
 // channel: 1 solid/2 textured; rgb: 1 raster/2 modulate; alpha and order:
 // 1 solid/2 textured. Zero in these local profile codes means not admitted.
 unsigned fixed_=0,descriptors_=0,attributes_=0,generators_=2,channel_=0;
 bool cull_=false,channels_=false,texgen_=false,tev_=false,colorOp_=false,alphaOp_=false;
 // lanes_=0 is idle; otherwise one plus current scalar ordinal.
 unsigned rgb_=0,alpha_=0,order_=0,lanes_=0,stride_=0;
 std::array<std::array<float,3>,4> positions_{};
 std::array<std::array<float,2>,4> uv_{};
 std::array<uint32_t,4> colors_{};
 bool ready()const;
};
#ifdef __3DS__
// Load outside FrameBegin; explicitly release while C3D lives. Retire all
// published batches first. Whole-batch validation precedes any GPU mutation.
class NativeBattleEnemyHpGpu {
public:
 NativeBattleEnemyHpGpu()=default;
 NativeBattleEnemyHpGpu(const NativeBattleEnemyHpGpu&)=delete;
 NativeBattleEnemyHpGpu& operator=(const NativeBattleEnemyHpGpu&)=delete;
 bool load(const char* root);void release();
 const BattleEnemyHpGpuResource& resource()const{return resource_;}
 bool draw(const BattleEnemyHpGpuBatch&,shaderProgram_s*,int transformUniform,
     const C3D_Mtx& projection,C3D_Tex* parkedSampler);
private:
 BattleEnemyHpGpuResource resource_;C3D_Tex texture_{};Tex3DS_Texture imported_=nullptr;
};
#endif
}
