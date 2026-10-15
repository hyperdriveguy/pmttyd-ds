#pragma once
#include "battle_submenu_surface_gpu.hpp"
#include "battle_texture_plane.hpp"
#include "camera_depth.hpp"
#include "animation_texture.hpp"
namespace ttyd {
struct BattleSubmenuCameraProof {
 std::shared_ptr<const void> lifetime;
 void* user=nullptr;
 bool (*current)(void*,const BattleSubmenuCameraProof&)=nullptr;
 uint64_t generation=0,pass=0;
 uint32_t camera=0;
 CameraProjectionMatrix projection{};
 std::array<float,6> viewport{};
 std::array<uint32_t,4> scissor{};
};
struct BattleSubmenuTextureObject {
 uint32_t handle=0; // Supplied by the actual typed source-output owner, never the index.
 AnimationTPLImage image;
 std::optional<std::array<uint32_t,8>> bytes; // Whole known source GX object.
 std::shared_ptr<const void> lifetime;
 void* user=nullptr;bool(*current)(void*)=nullptr;
};
struct BattleSubmenuPacket {
 BattleSubmenuSurfaceLease surface;
 BattleSubmenuCameraProof camera;
 AnimationTPLImage originalTexture;
 std::array<uint32_t,8> loadedObject{};
 std::shared_ptr<const void> sourceResourceOwner;
 void* sourceResourceUser=nullptr;
 bool(*sourceResourceCurrent)(void*)=nullptr;
 BattleActMatrix matrix{};
 std::array<std::array<float,3>,4> positions{};
 std::array<std::array<float,2>,4> sourceUV{};
 std::array<uint32_t,4> rgba{};
};
using BattleSubmenuPacketPtr=std::shared_ptr<const BattleSubmenuPacket>;
struct BattleSubmenuPacketSink {void* user=nullptr;bool(*append)(void*,BattleSubmenuPacketPtr)=nullptr;};
struct BattleSubmenuQueryServices {
 void* user=nullptr;
 std::shared_ptr<const void> lifetime;
 bool (*current)(void*)=nullptr;
 bool (*sourceResourceCurrent)(void*)=nullptr;
 std::optional<uint32_t> (*camera)(void*,std::optional<uint32_t>)=nullptr;
 std::optional<BattleSubmenuTextureObject> (*texture)(void*,uint32_t palette,uint32_t index)=nullptr;
};
struct BattleSubmenuMemoryReader {void* user=nullptr;std::optional<uint32_t>(*word)(void*,uint32_t)=nullptr;};
std::optional<AnimationTPLImage> readBattleSubmenuTexture(const BattleSubmenuMemoryReader&,uint32_t palette,uint32_t index);
std::optional<std::array<BattleClassVertex,4>> battleSubmenuVertices(const BattleSubmenuPacket&);
bool validateBattleSubmenuPacket(const BattleSubmenuPacket&,const BattleSubmenuSurfaceGpuResource&);
bool validateBattleSubmenuBatch(const std::vector<BattleSubmenuPacketPtr>&,const BattleSubmenuSurfaceGpuResource&);
// Exact finite GX/CMPR path. No opaque Quad or InitSub acknowledgement: feed
// the original retained source helpers below, or the existing TexturePlane2.
class BattleSubmenuPacketProvider {
public:
 BattleSubmenuPacketProvider(const BattleSubmenuSurfaceGpuResource&,BattleSubmenuCameraProof,BattleSubmenuQueryServices,BattleSubmenuPacketSink);
 std::optional<uint32_t> request(const BattleTexturePlaneRequest&);
 bool partial()const{return lanes_!=0;}
private:
 const BattleSubmenuSurfaceGpuResource& resource_;BattleSubmenuSurfaceLease lease_;
 BattleSubmenuCameraProof camera_;BattleSubmenuQueryServices query_;BattleSubmenuPacketSink sink_;
 std::array<std::optional<BattleSubmenuTextureObject>,2> objects_{};
 std::optional<BattleSubmenuTextureObject> loaded_;
 std::optional<uint32_t> selected_;std::optional<BattleActMatrix> matrix_;
 uint32_t state_=0,descriptors_=0,formats_=0,lanes_=0;
 bool invoking_=false,poisoned_=false;
 std::array<std::array<float,3>,4> positions_{};
 std::array<std::array<float,2>,4> uv_{};std::array<uint32_t,4> rgba_{};
 BattleSubmenuPacketPtr pending_;std::optional<BattleTexturePlaneRequest> pendingLane_;
 bool current();
};
// Whole original InitSub80103a14/134 + InitRasta80103c3c/94 and
// Quad801037c4/124 + Vertex801038e8/30. Exact original constant reads are
// optional-safe, each external operation retained; no fabricated stack alias.
struct BattleSubmenuSourceServices {
 void* user=nullptr;std::shared_ptr<const void> lifetime;
 bool(*current)(void*)=nullptr;
 std::optional<uint32_t>(*word)(void*,uint32_t)=nullptr;
 std::optional<uint32_t>(*request)(void*,const BattleTexturePlaneRequest&)=nullptr;
};
class BattleSubmenuSourceCursor {
public:
 BattleSubmenuSourceCursor();~BattleSubmenuSourceCursor();bool reset();uint32_t pending()const;
 BattleSubmenuSourceCursor(const BattleSubmenuSourceCursor&)=delete;
 BattleSubmenuSourceCursor&operator=(const BattleSubmenuSourceCursor&)=delete;
private:
 struct State;std::unique_ptr<State> state_;bool invoking_=false;
 friend bool resumeBattleSubmenuInit(const BattleSubmenuSourceServices&,BattleSubmenuSourceCursor&);
 friend bool resumeBattleSubmenuQuad(const std::array<float,4>&,const std::array<uint32_t,4>&,const BattleSubmenuSourceServices&,BattleSubmenuSourceCursor&);
 static bool resume(unsigned,const std::array<float,4>&,const std::array<uint32_t,4>&,const BattleSubmenuSourceServices&,BattleSubmenuSourceCursor&);
};
bool resumeBattleSubmenuInit(const BattleSubmenuSourceServices&,BattleSubmenuSourceCursor&);
bool resumeBattleSubmenuQuad(const std::array<float,4>& xy,const std::array<uint32_t,4>& rgba,const BattleSubmenuSourceServices&,BattleSubmenuSourceCursor&);
}
