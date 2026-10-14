#pragma once
#include "battle_act_draw.hpp"
namespace ttyd {
enum class BattleClassRasterService : uint32_t {
  Camera = 0x8000f2fc,
  Init2D = 0x80103a14,
  MaterialColor = 0x802b8ab8,
  TevStages = 0x802b8fcc,
  ColorOp = 0x802b89e8,
  ColorIn = 0x802b8960,
  AlphaOp = 0x802b8a50,
  AlphaIn = 0x802b89a4,
  Order = 0x802b8e30,
  LoadMatrix = 0x802ba2fc,
  TextureObject = 0x80037a80,
  LoadTexture = 0x802b7880,
  Begin = 0x802b5a6c,
  Point = 0x801038e8,
  Quad = 0x801037c4,
  IconColor = 0x8001a4bc,
  Icon = 0x8001a6e0,
  FontWidth = 0x80075528,
  FontStart = 0x80077b4c,
  FontColor = 0x800777e0,
  FontString = 0x80076a10
};
struct BattleClassRasterRequest {
  BattleClassRasterService function;
  std::vector<uint32_t> integers;
  std::vector<float> reals;
  std::optional<BattleActMatrix> matrix;
};
struct BattleClassRasterServices {
  BattleActorServices memory;
  BattleActDrawServices math;
  void *user = nullptr;
  std::optional<uint32_t> (*request)(void *, const BattleClassRasterRequest &) =
      nullptr;
  std::optional<int32_t> (*truncate)(void *, float) = nullptr;
  std::optional<BattleActMatrix> (*identity)(void *) = nullptr;
};
// Whole original circle and speech-bubble local bodies. Required linked SDK,
// GX, texture/icon and font services can stop after prior effects; no rollback.
bool drawPackedBattleClassCircle(uint32_t row, const BattleActMatrix &,
                                 const std::array<float, 3> &, float,
                                 const BattleClassRasterServices &);
bool drawPackedBattleClassBubble(uint32_t work, uint32_t row,
                                 const BattleActMatrix &, float,
                                 const BattleClassRasterServices &);
} // namespace ttyd
