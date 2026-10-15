#pragma once
#include "field_npc_clock.hpp"
#include <array>
namespace ttyd {
// Stack X/Z outputs are semantic locals, never fabricated guest addresses.
// ActorXZ outputs and heading are real aliases and must be written by the
// child.
enum class FieldNpcMoveOutput { None, TemporaryXZ, ActorXZ };
struct FieldNpcMainMoveRequest {
  uint32_t function = 0;
  std::array<uint32_t, 4> args{};
  FieldNpcMoveOutput output = FieldNpcMoveOutput::None;
  float dx = 0, dz = 0, x = 0, z = 0;
};
struct FieldNpcMainMoveResponse {
  uint32_t contact = 0;
  float x = 0, z = 0;
};
struct FieldNpcMainMoveServices {
  FieldNpcClockServices memory; // only word/store/user are used
  // Source paired XY load: both words captured together, or no capture.
  std::optional<std::array<uint32_t, 2>> (*pair)(void *, uint32_t) = nullptr;
  // False has no effects. True publishes all original child effects once.
  bool (*request)(void *, const FieldNpcMainMoveRequest &,
                  FieldNpcMainMoveResponse &) = nullptr; // memory.user
};
enum class FieldNpcMoveFrontier { Pending, AutomaticPose, Detection };
// Complete source region80049e8c..80049fbc, including PSVECSubtract. Neither
// frontier permits advancing the actor loop or claiming Main completion.
struct FieldNpcMainMoveCursor {
  bool active = false;
  FieldNpcMainMoveServices owner;
  uint32_t actor = 0, phase = 0, state = 0, flags = 0, word = 0;
  float dx = 0, dz = 0, x = 0, z = 0, zero = 0;
  std::array<float, 3> current{}, previous{}, delta{};
  FieldNpcMainMoveRequest call;
  FieldNpcMainMoveResponse response;
};
FieldNpcMoveFrontier
advanceFieldNpcMainMovement(uint32_t actor, const FieldNpcMainMoveServices &,
                            FieldNpcMainMoveCursor &);
} // namespace ttyd
