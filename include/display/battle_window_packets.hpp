#pragma once
#include <memory>
#include "battle_submenu_packets.hpp"
#include "window_primitive_continuation.hpp"
namespace ttyd {
class BattleWindowSurfaceResource;
class BattleWindowSurfaceGeneration {
public:~BattleWindowSurfaceGeneration()=default;
 BattleWindowSurfaceGeneration(const BattleWindowSurfaceGeneration&)=delete;
 BattleWindowSurfaceGeneration&operator=(const BattleWindowSurfaceGeneration&)=delete;
private:BattleWindowSurfaceGeneration()=default;bool resident_=false;const void*texture_=nullptr;
 friend class BattleWindowSurfaceResource;friend struct BattleWindowSurfaceLease;
};
struct BattleWindowSurfaceLease {
 std::shared_ptr<const BattleWindowSurfaceGeneration>generation;const void*texture=nullptr;
 bool current()const;
};
// Whole native128x128RGBA8 atlas, original72x72 descriptor0 at0,0 and entire
// right/bottom/corner padding edge-extruded. Actual native bytes are required.
class BattleWindowSurfaceResource {
public:
 BattleWindowSurfaceResource()=default;~BattleWindowSurfaceResource();
 BattleWindowSurfaceResource(const BattleWindowSurfaceResource&)=delete;
 BattleWindowSurfaceResource&operator=(const BattleWindowSurfaceResource&)=delete;
 bool adopt(const BattleClassSurfaceImport&,const uint8_t*actualTiledABGR,size_t bytes);
 void retire();BattleWindowSurfaceLease lease()const;
 bool accepts(const BattleWindowSurfaceLease&)const;
private:std::shared_ptr<BattleWindowSurfaceGeneration>generation_;
};
struct BattleWindowTextureObject {
 uint32_t address=0;AnimationTPLImage image;std::array<uint32_t,8>bytes{};
 std::shared_ptr<const void>lifetime;void*user=nullptr;bool(*current)(void*)=nullptr;
};
struct BattleWindowPacket {
 BattleWindowSurfaceLease surface;BattleSubmenuCameraProof camera;
 BattleWindowTextureObject source;BattleActMatrix matrix{};uint32_t rgba=0;
 std::array<std::array<float,5>,36>vertices{}; // original XYZ/ST, nine GX quads.
};
using BattleWindowPacketPtr=std::shared_ptr<const BattleWindowPacket>;
struct BattleWindowPacketSink {void*user=nullptr;bool(*append)(void*,BattleWindowPacketPtr)=nullptr;};
struct BattleWindowQueryServices {
 void*user=nullptr;std::shared_ptr<const void>lifetime;
 bool(*current)(void*)=nullptr;
 std::optional<uint32_t>(*camera)(void*)=nullptr;
 // Genuine actual windowTexSetup-created object and resource owner. A bytes
 // reader or synthesized SDK object alone cannot acknowledge this query.
 std::optional<BattleWindowTextureObject>(*texture)(void*,uint32_t object)=nullptr;
};
bool validateBattleWindowPacket(const BattleWindowPacket&,const BattleWindowSurfaceResource&);
CameraProjectionMatrix battleWindowTransform(const BattleWindowPacket&);
class BattleWindowPacketProvider {
public:
 BattleWindowPacketProvider(const BattleWindowSurfaceResource&,BattleSubmenuCameraProof,BattleWindowQueryServices,BattleWindowPacketSink);
 std::optional<uint32_t>request(const WindowPrimitiveRequest&);
 bool partial()const{return lanes_!=0;}
private:
 const BattleWindowSurfaceResource&resource_;BattleWindowSurfaceLease lease_;
 BattleSubmenuCameraProof camera_;BattleWindowQueryServices query_;BattleWindowPacketSink sink_;
 std::optional<BattleWindowTextureObject>texture_;std::optional<BattleActMatrix>matrix_;std::optional<uint32_t>color_;
 uint32_t state_=0,descriptors_=0,formats_=0,lanes_=0;
 std::array<std::array<float,5>,36>vertices_{};BattleWindowPacketPtr pending_;std::optional<WindowPrimitiveRequest>last_;
 bool poisoned_=false,invoking_=false;bool current(bool sourceOnly=false);
};
class NativeBattleWindowSurfaceGpu {
public:
 NativeBattleWindowSurfaceGpu();~NativeBattleWindowSurfaceGpu();
 NativeBattleWindowSurfaceGpu(const NativeBattleWindowSurfaceGpu&)=delete;
 NativeBattleWindowSurfaceGpu&operator=(const NativeBattleWindowSurfaceGpu&)=delete;
 bool load(const char*root);void release();const BattleWindowSurfaceResource&resource()const;
#ifdef __3DS__
 C3D_Tex*texture(const BattleWindowSurfaceLease&)const;
 bool canDraw(const BattleWindowPacket&,const shaderProgram_s*,int,const C3D_Tex*parking)const;
 bool draw(const BattleWindowPacket&,shaderProgram_s*,int,C3D_Tex*parking);
#endif
private:struct State;std::unique_ptr<State>state_;
};
}
