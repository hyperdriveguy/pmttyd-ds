#pragma once
#include "battle_audience_matrix.hpp"
#include "source_icon_record.hpp"
namespace ttyd {
// Original function identity, 8 integer argument lanes, 4 float lanes and
// affine3x4 matrix payload. Values are guest/source bits, never native pointers.
struct SourceIconGXCall {
  uint32_t function = 0;
  std::array<uint32_t, 8> integers{};
  std::array<float, 4> reals{};
  BattleActMatrix matrix{};
};
struct SourceIconGXQuad {
  std::array<std::array<float, 3>, 4> positions{};
  std::array<std::array<float, 2>, 4> uv{{{0, 0}, {1, 0}, {1, 1}, {0, 1}}};
};
struct SourceIconGXServices {
  void *user = nullptr;
  std::optional<uint32_t> (*word)(void *, uint32_t) = nullptr;
  std::optional<uint16_t> (*half)(void *, uint32_t) = nullptr;
  bool (*matrix)(void *, BattleAudienceMatrixRequest &) = nullptr;
  // Void services acknowledge with any value. Queries return source values;
  // texture construction returns a retained opaque object token. No original
  // address is cast to a host pointer. mapGetBlend returns its copied RGBA.
  std::optional<uint32_t> (*call)(void *, const SourceIconGXCall &) = nullptr;
  // Atomic acknowledgement of the four direct FIFO XYZ/ST vertices after
  // the original GXBegin. A false result must not consume partial geometry.
  bool (*quad)(void *, const SourceIconGXQuad &) = nullptr;
};
// Request/math latches and table step prevent already acknowledged setters
// from replaying. Numeric phases are private continuation labels, not source PC.
struct SourceIconGXCursor {
  bool active = false, ready = false, mathReady = false, loaded = false,
       flip = false;
  uint32_t phase = 0, step = 0, pending = 0, camera8 = 0, texture = 0, map = 0,
           color = 0;
  float width = 0, height = 0;
  SourceMatrixIcon icon;
  BattleActMatrix input{}, transform{}, scale{};
  BattleAudienceMatrixRequest math;
  SourceIconGXCall request;
  SourceIconGXQuad geometry;
};
// Whole local original iconGX800197f8. Linked resources, current-map services,
// SDK math and actual GPU work remain explicit. Retains acknowledged effects.
bool resumeSourceIconGX(const BattleActMatrix &, const SourceMatrixIcon &,
                        const SourceIconGXServices &, SourceIconGXCursor &);
} // namespace ttyd
