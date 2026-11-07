#pragma once
#include "battle_submenu_packets.hpp"
#include "battle_class_raster.hpp"
#include "source_gx_inherited_state.hpp"
namespace ttyd {
enum class BattleClassSurfaceRGB { TextureRaster, MaterialRaster };
struct BattleClassSurfaceTextureObject:BattleSubmenuTextureObject {
 // Pure actual owner proof of this exact output address, known32B and source
 // metadata. A live unrelated token alone cannot certify copied object fields.
 bool(*witness)(void*,const BattleClassSurfaceTextureObject&)=nullptr;
};
struct BattleClassSurfaceQueryServices {
 void*user=nullptr;std::shared_ptr<const void>lifetime;
 // Actual common-bank identity is separate from pinned callback/proof userdata.
 std::shared_ptr<const void>sourceResourceOwner;
 bool(*current)(void*)=nullptr;bool(*sourceResourceCurrent)(void*)=nullptr;
 // Actual captured inherited source GX state compatible with this finite path.
 // InitSub alone does not establish fog/write masks/swap/indirect history.
 bool(*ordinaryCurrent)(void*)=nullptr;
 // Capture exact relevant inherited source state AT actual LoadTexture.
 // Required native production route; null preserves explicit legacy fixtures.
 std::shared_ptr<const SourceGxInheritedSnapshot>(*ordinaryCapture)(void*)=nullptr;
 std::optional<uint32_t>(*camera)(void*,std::optional<uint32_t>)=nullptr;
 std::optional<BattleClassSurfaceTextureObject>(*texture)(void*,uint32_t palette,uint32_t index)=nullptr;
};
class BattleClassSurfaceObjectSnapshot;
// Original surfaces0/1 ONLY. Source GX object ownership is independent of the
// resident converted surface pair and of the actual battle common-bank owner.
struct BattleClassSurfacePacket {
 BattleClassSurfaceLease surface;
 BattleSubmenuCameraProof camera;
 BattleClassSurfaceTextureObject object;
 // Created only after the actual load attests its constructor result. Original
 // temporary SDK cells may be reused; this independent immutable copy survives.
 std::shared_ptr<const BattleClassSurfaceObjectSnapshot> objectSnapshot;
 std::shared_ptr<const void> sourceResourceOwner,sourceProofOwner;
 void* sourceResourceUser=nullptr;bool(*sourceResourceCurrent)(void*)=nullptr;
 bool(*ordinaryCurrent)(void*)=nullptr;
 BattleActMatrix matrix{};
 std::array<std::array<float,3>,4> positions{};
 std::array<std::array<float,2>,4> sourceUV{};
 std::array<uint32_t,4> rgba{};
 uint32_t material=0xffffffff;
 BattleClassSurfaceRGB rgb=BattleClassSurfaceRGB::TextureRaster;
 std::shared_ptr<const SourceGxInheritedSnapshot>ordinarySnapshot;
};
using BattleClassSurfacePacketPtr=std::shared_ptr<const BattleClassSurfacePacket>;
struct BattleClassSurfacePacketSink {void*user=nullptr;bool(*append)(void*,BattleClassSurfacePacketPtr)=nullptr;};
std::optional<AnimationTPLImage> readBattleClassSurfaceTexture(const BattleSubmenuMemoryReader&,uint32_t palette,uint32_t index);
std::optional<std::array<BattleClassVertex,4>> battleClassSurfaceVertices(const BattleClassSurfacePacket&);
bool validateBattleClassSurfacePacket(const BattleClassSurfacePacket&,const BattleClassSurfaceGpuResource&);
// Whole finite source GX state/FIFO capture. Only the true TEX output query can
// supply an object handle. Each final FIFO append retains its immutable packet
// on false. Object/C0/source raster colors remain separate, including alpha.
class BattleClassSurfacePacketProvider {
public:
 BattleClassSurfacePacketProvider(const BattleClassSurfaceGpuResource&,BattleSubmenuCameraProof,BattleClassSurfaceQueryServices,BattleClassSurfacePacketSink);
 std::optional<uint32_t> request(const BattleTexturePlaneRequest&);
 bool partial()const{return lanes_!=0;}
private:
 const BattleClassSurfaceGpuResource&resource_;BattleClassSurfaceLease lease_;
 BattleSubmenuCameraProof camera_;BattleClassSurfaceQueryServices query_;BattleClassSurfacePacketSink sink_;
 std::array<std::optional<BattleClassSurfaceTextureObject>,2>objects_{};
 std::optional<BattleClassSurfaceTextureObject>loaded_;std::shared_ptr<const BattleClassSurfaceObjectSnapshot>loadedSnapshot_;std::optional<BattleActMatrix>matrix_;
 std::optional<uint32_t>material_;BattleClassSurfaceRGB rgb_=BattleClassSurfaceRGB::TextureRaster;
 uint32_t state_=0,descriptors_=0,formats_=0,lanes_=0;
 bool invoking_=false,poisoned_=false,ordinaryKnown_=false;
 std::array<std::array<float,3>,4>positions_{};
 std::array<std::array<float,2>,4>uv_{};std::array<uint32_t,4>rgba_{};
 BattleClassSurfacePacketPtr pending_;std::optional<BattleTexturePlaneRequest>pendingLane_;
 bool current();
};
// Original high-level InitSub/Quad/Vertex bridges, not acknowledgement stubs.
// Other class font/icon/camera children return unavailable for their real owner.
// The caller pins this context, source service table and actual word storage
// through every held request. No native/source owner is manufactured here.
class BattleClassSurfaceRaster {
public:
 BattleClassSurfaceRaster(BattleSubmenuSourceServices,BattleClassSurfacePacketProvider&);
 ~BattleClassSurfaceRaster();
 BattleClassSurfaceRaster(const BattleClassSurfaceRaster&)=delete;
 BattleClassSurfaceRaster&operator=(const BattleClassSurfaceRaster&)=delete;
 std::optional<uint32_t> request(const BattleClassRasterRequest&);
 uint32_t pending()const;
private:struct State;std::unique_ptr<State>state_;bool invoking_=false;
};
}
