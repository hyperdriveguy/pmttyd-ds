#pragma once
#include "battle_audience_aux_callback.hpp"
#include <memory>
namespace ttyd {
enum class BattleTexturePlaneKind {Call,FifoFloat,FifoByte};
enum class BattleTexturePlaneCall : uint32_t {
  DepthLocation=0x802b940c,AlphaCompare=0x802b8d60,BlendMode=0x802b932c,
  DepthMode=0x802b93d8,Cull=0x802b5c8c,ClearDescriptors=0x802b4520,Descriptor=0x802b3c08,
  AttributeFormat=0x802b4558,Channels=0x802b6f3c,ChannelControl=0x802b6f78,
  TextureGenerators=0x802b5084,TextureGenerator=0x802b4e04,
  CurrentCamera=0x8000f2e4,LoadMatrix=0x802ba2fc,
  TextureObject=0x80037a80,LoadTexture=0x802b7880,TextureFormat=0x802b76e4,
  TevStages=0x802b8fcc,ColorOp=0x802b89e8,ColorIn=0x802b8960,
  AlphaOp=0x802b8a50,AlphaIn=0x802b89a4,Order=0x802b8e30,TevOp=0x802b88d4,
  TextureWidth=0x802b76c4,TextureHeight=0x802b76d4,Begin=0x802b5a6c
};
// Kind selects actual SDK/GX call versus one FIFO scalar. An engaged zero
// return can be a valid acknowledgment; only empty optional means unavailable.
struct BattleTexturePlaneRequest {
  BattleTexturePlaneKind kind=BattleTexturePlaneKind::Call;
  BattleTexturePlaneCall function=BattleTexturePlaneCall::Cull;
  std::vector<uint32_t> integers;
  std::vector<float> reals;
  std::optional<BattleActMatrix> matrix;
};
struct BattleTexturePlaneServices {
  BattleActorServices actors;
  void* user=nullptr;
  // Query return: actual camera / opaque texture object / format / dimensions.
  // Other calls acknowledge void with optional0. FIFO kind identifies a single
  // original CC008000 float/byte write; function is unused for FIFO requests.
  // TextureObject arguments are actual palette, index; LoadTexture arguments
  // handle, slot 0; all texture queries receive that retained opaque handle.
  std::optional<uint32_t> (*request)(void*,const BattleTexturePlaneRequest&)=nullptr;
  // Actual SDK matrix calls: Copy8029dd30 / Concat8029dd64 and, for the
  // linked TexPlane wrapper, Scale8029e2b0 / Trans8029e230. False must retain any partial
  // provider effects. The same request object/operands survive suspension.
  bool (*matrix)(void*,BattleAudienceMatrixRequest&)=nullptr;
};
class BattleTexturePlaneCursor {
public:
  BattleTexturePlaneCursor();
  ~BattleTexturePlaneCursor();
  BattleTexturePlaneCursor(const BattleTexturePlaneCursor&)=delete;
  BattleTexturePlaneCursor& operator=(const BattleTexturePlaneCursor&)=delete;
  bool active() const;
  // Caller must separately own cancellation/teardown of already committed effects.
  void reset();
private:
  struct State;
  std::unique_ptr<State> state_;
  static bool resumePart(unsigned,const BattleActMatrix*,uint32_t,
      const uint32_t*,const std::array<uint32_t,4>*,const BattleTexturePlaneServices&,
      BattleTexturePlaneCursor&);
  friend bool resumeBattleTexturePlaneInit(const BattleTexturePlaneServices&,BattleTexturePlaneCursor&);
  friend bool resumePackedBattleTexturePlainGX(uint32_t,const std::array<uint32_t,4>&,
      const BattleTexturePlaneServices&,BattleTexturePlaneCursor&);
  friend bool resumePackedBattleTexturePlane2(const BattleActMatrix&,uint32_t,
      const uint32_t&,const BattleTexturePlaneServices&,BattleTexturePlaneCursor&);
};
// Whole Plane2 8010323c/8c + Init801036e4/e0 + PlainGX801033e4/300.
// Base/RGBA are borrowed by object identity and must outlive this invocation.
// Copy reads base AFTER Init; RGBA reads AFTER LoadMatrix. Does not replace
// current camera, common-bank descriptor/palette, texture metadata or geometry.
// Width/height use fresh original .5 and unsigned-conversion bias reads. FIFO
// requests retain exact scalar order and acknowledge each lane only once.
// Provider tables/owners must remain identical; no guest fields are written.
bool resumePackedBattleTexturePlane2(const BattleActMatrix& base,uint32_t texture,
    const uint32_t& rgba,const BattleTexturePlaneServices&,BattleTexturePlaneCursor&);
// Whole original Init801036e4/e0, independently retained. This does not perform
// InitSub/Rasta, whose source state differs.
bool resumeBattleTexturePlaneInit(const BattleTexturePlaneServices&,BattleTexturePlaneCursor&);
// Whole PlainGX801033e4/300. Four borrowed original RGBA locals are loaded at
// the original byte-load sites, including source read-ahead between FIFO lanes.
// Their object identity and lifetime must survive holds; no matrix/init is done.
bool resumePackedBattleTexturePlainGX(uint32_t texture,
    const std::array<uint32_t,4>& colors,const BattleTexturePlaneServices&,
    BattleTexturePlaneCursor&);
}
