#include "battle_class_raster_continuation.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
using F = BattleClassRasterService;
enum class P {
  Camera,
  CameraDone,
  Shade,
  ShadeDone,
  Setup,
  SetupDone,
  InitialScale,
  InitialScaleDone,
  InitialTranslationDone,
  InitialRotation,
  InitialRotationDone,
  ComposeFirstDone,
  ComposeSecondDone,
  BubbleTranslationDone,
  CameraConcat,
  CameraConcatDone,
  LoadMatrixDone,
  Texture,
  TextureObjectDone,
  TextureDone,
  Offset,
  CircleBegin,
  CirclePoint,
  CirclePointDone,
  CircleIconTranslation,
  CircleIconTranslationDone,
  CircleIconScaleDone,
  CircleIconRotationDone,
  CircleIdentityDone,
  CircleIconComposeDone,
  CircleIconScaleComposeDone,
  CircleIconTranslateComposeDone,
  BubbleShadowDone,
  BubbleBodyDone,
  BubbleIconTranslationDone,
  BubbleIconScaleDone,
  BubbleIconRotationDone,
  BubbleIconComposeDone,
  BubbleIconTranslationComposeDone,
  BubbleIconScaleComposeDone,
  BubbleIconDone,
  TextTranslationDone,
  TextWidthDone,
  TextScaleDone,
  TextRotationDone,
  TextComposeDone,
  TextTranslationComposeDone,
  TextScaleComposeDone,
  FontStarted,
  FontColorDone,
  Request,
  Scale,
  Translate,
  Rotate,
  Concat,
  Identity,
  Truncate,
  Done,
  Unavailable
};
bool sameMemory(const BattleActorServices &a, const BattleActorServices &b) {
  const auto &x = a.memory;
  const auto &y = b.memory;
  return a.user == b.user && a.byte == b.byte && x.user == y.user &&
         x.word == y.word && x.half == y.half && x.storeWord == y.storeWord &&
         x.storeHalf == y.storeHalf && x.storeByte == y.storeByte &&
         x.flushBlock == y.flushBlock && x.systemCall == y.systemCall;
}
bool sameMath(const BattleActDrawServices &a, const BattleActDrawServices &b) {
  return sameMemory(a.memory, b.memory) && a.user == b.user &&
         a.camera == b.camera && a.center == b.center && a.offset == b.offset &&
         a.interpolate == b.interpolate && a.translation == b.translation &&
         a.rotation == b.rotation && a.scale == b.scale &&
         a.concat == b.concat && a.icons == b.icons &&
         a.partyButton == b.partyButton && a.pinchMark == b.pinchMark;
}
bool same(const BattleClassRasterServices &a,
          const BattleClassRasterServices &b) {
  return sameMemory(a.memory, b.memory) && sameMath(a.math, b.math) &&
         a.user == b.user && a.request == b.request &&
         a.truncate == b.truncate && a.identity == b.identity;
}
float scalar(uint32_t n) {
  float f;
  std::memcpy(&f, &n, 4);
  return f;
}
} // namespace
struct BattleClassRasterCursor::State {
  BattleClassRasterServices owner;
  bool bubble = false;
  uint32_t work = 0, row = 0, b = 0, camera = 0, color = 0, gray = 0, label = 0;
  BattleActMatrix base{}, scaled{}, rotation{}, translation{}, matrix{},
      result{}, lhs{}, rhs{};
  std::array<float, 3> position{}, xyz{};
  float parameter = 0, one = 0, zero = 0, radians = 0, offset = 0, shadowY = 0,
        left = 0, right = 0, number = 0;
  uint32_t value = 0;
  int32_t truncated = 0;
  std::array<float, 32> xy{}, uv{};
  unsigned setup = 0, quad = 0, vertex = 0;
  P phase = P::Camera, next = P::CameraDone;
  BattleClassRasterRequest request{F::Camera, {}, {}, {}};
};
BattleClassRasterCursor::BattleClassRasterCursor() = default;
BattleClassRasterCursor::~BattleClassRasterCursor() = default;
bool BattleClassRasterCursor::active() const { return bool(state_); }
void BattleClassRasterCursor::reset() { state_.reset(); }
struct BattleClassRasterContinuationAccess {
  static bool resume(bool bubble, uint32_t work, uint32_t row,
                     const BattleActMatrix &base,
                     const std::array<float, 3> &position, float parameter,
                     const BattleClassRasterServices &s,
                     BattleClassRasterCursor &cursor) {
    if (cursor.state_) {
      const auto &c = *cursor.state_;
      if (!same(c.owner, s) || c.bubble != bubble || c.work != work ||
          c.row != row ||
          std::memcmp(c.base.data(), base.data(), sizeof(base)) ||
          std::memcmp(c.position.data(), position.data(), sizeof(position)) ||
          std::memcmp(&c.parameter, &parameter, sizeof(parameter)))
        return false;
    }
    if (!s.memory.memory.word || !s.memory.memory.half)
      return false;
    auto word = [&](uint32_t a) {
      return s.memory.memory.word(s.memory.memory.user, a);
    };
    auto real = [&](uint32_t a) { return scalar(word(a)); };
    auto constant = [&](int32_t n) { return real(0x80427440u + uint32_t(n)); };
    if (!cursor.state_) {
      cursor.state_ = std::make_unique<BattleClassRasterCursor::State>();
      auto &c = *cursor.state_;
      c.owner = s;
      c.bubble = bubble;
      c.work = work;
      c.row = row;
      c.base = base;
      c.position = position;
      c.parameter = parameter;
      c.b = currentBattleAddress(s.memory);
    }
    auto &c = *cursor.state_;
    auto request = [&](F f, std::vector<uint32_t> ints,
                       std::vector<float> reals,
                       std::optional<BattleActMatrix> matrix, P next) {
      c.request = {f, std::move(ints), std::move(reals), std::move(matrix)};
      c.next = next;
      c.phase = P::Request;
    };
    auto scale = [&](float x, float y, float z, P next) {
      c.xyz = {x, y, z};
      c.next = next;
      c.phase = P::Scale;
    };
    auto translate = [&](float x, float y, float z, P next) {
      c.xyz = {x, y, z};
      c.next = next;
      c.phase = P::Translate;
    };
    auto rotate = [&](float radians, P next) {
      c.number = radians;
      c.next = next;
      c.phase = P::Rotate;
    };
    auto concat = [&](const BattleActMatrix &a, const BattleActMatrix &b,
                      P next) {
      c.lhs = a;
      c.rhs = b;
      c.next = next;
      c.phase = P::Concat;
    };
    auto cameraMatrix = [&]() {
      BattleActMatrix m;
      for (unsigned i = 0; i < 12; ++i)
        m[i] = real(c.camera + 0x11c + 4 * i);
      return m;
    };
    auto icon = [&]() {
      const auto id = s.memory.memory.half(s.memory.memory.user, c.row + 0xc);
      return id ? uint32_t(id) : 403u;
    };
    for (;;)
      switch (c.phase) {
      case P::Camera:
        request(F::Camera, {8}, {}, {}, P::CameraDone);
        break;
      case P::CameraDone:
        c.camera = c.value;
        if (c.bubble) {
          c.color = word(0x80427440u + uint32_t(-20544));
          c.phase = P::Setup;
        } else
          c.phase = P::Shade;
        break;
      case P::Shade:
        if (!s.truncate)
          return false;
        c.number = constant(-20380) * c.position[2];
        c.next = P::ShadeDone;
        c.phase = P::Truncate;
        break;
      case P::ShadeDone:
        c.gray = (192u - uint32_t(c.truncated)) & 255;
        c.color = word(0x80427440u + uint32_t(-20532));
        c.phase = P::Setup;
        break;
      case P::Setup: {
        static constexpr F functions[]{
            F::Init2D,  F::MaterialColor, F::TevStages, F::ColorOp,
            F::ColorIn, F::AlphaOp,       F::AlphaIn,   F::Order};
        std::vector<uint32_t> args;
        switch (c.setup) {
        case 0:
          break;
        case 1:
          args = {1, c.color};
          break;
        case 2:
          args = {1};
          break;
        case 3:
        case 5:
          args = {0, 0, 0, 0, 1, 0};
          break;
        case 4:
          args = {0, 15, 10, 2, 15};
          break;
        case 6:
          args = {0, 7, 5, 4, 7};
          break;
        case 7:
          args = {0, 0, 0, 4};
          break;
        }
        request(functions[c.setup], std::move(args), {}, {}, P::SetupDone);
        break;
      }
      case P::SetupDone:
        if (++c.setup < 8)
          c.phase = P::Setup;
        else
          c.phase = P::InitialScale;
        break;
      case P::InitialScale:
        if (c.bubble) {
          c.one = constant(-20452);
          c.zero = constant(-20500);
          scale(c.one, c.one, c.one, P::InitialScaleDone);
        } else {
          const float value =
              std::fma(-constant(-20276), c.position[2], constant(-20280));
          scale(value, value, constant(-20452), P::InitialScaleDone);
        }
        break;
      case P::InitialScaleDone:
        c.scaled = c.result;
        if (c.bubble)
          translate(c.zero, constant(-20252), c.zero,
                    P::InitialTranslationDone);
        else
          c.phase = P::InitialRotation;
        break;
      case P::InitialTranslationDone:
        c.translation = c.result;
        c.phase = P::InitialRotation;
        break;
      case P::InitialRotation:
        c.radians = constant(-20472) * c.parameter;
        rotate(c.radians, P::InitialRotationDone);
        break;
      case P::InitialRotationDone:
        c.rotation = c.result;
        concat(c.base, c.rotation, P::ComposeFirstDone);
        break;
      case P::ComposeFirstDone:
        concat(c.result, c.scaled, P::ComposeSecondDone);
        break;
      case P::ComposeSecondDone:
        c.matrix = c.result;
        if (c.bubble)
          concat(c.matrix, c.translation, P::BubbleTranslationDone);
        else
          c.phase = P::CameraConcat;
        break;
      case P::BubbleTranslationDone:
        c.matrix = c.result;
        c.phase = P::CameraConcat;
        break;
      case P::CameraConcat:
        concat(cameraMatrix(), c.matrix, P::CameraConcatDone);
        break;
      case P::CameraConcatDone:
        c.matrix = c.result;
        request(F::LoadMatrix, {0}, {}, c.matrix, P::LoadMatrixDone);
        break;
      case P::LoadMatrixDone:
        c.phase = P::Texture;
        break;
      case P::Texture: {
        const auto owner = word(c.b + 0x163f8),
                   palette = word(word(owner + 0xa0));
        request(F::TextureObject, {palette, c.bubble ? 1u : 0u}, {}, {},
                P::TextureObjectDone);
        break;
      }
      case P::TextureObjectDone:
        request(F::LoadTexture, {c.value, 0}, {}, {}, P::TextureDone);
        break;
      case P::TextureDone:
        c.phase = P::Offset;
        break;
      case P::Offset: {
        std::optional<float> offset;
        const auto zero = constant(-20500), halfTurn = constant(-20488);
        if (c.parameter >= zero && c.parameter <= halfTurn) {
          const float t = (constant(-20272) * c.parameter) / halfTurn,
                      v = constant(-20400) - t;
          offset = c.bubble ? v : v / constant(-20280);
        }
        if (c.parameter >= halfTurn && c.parameter <= constant(-20476)) {
          const float t = (constant(-20272) * (c.parameter - halfTurn)) /
                          halfTurn,
                      v = constant(-20268) + t;
          offset = c.bubble ? v : v / constant(-20280);
        }
        if (!offset) {
          c.phase = P::Unavailable;
          break;
        }
        c.offset = *offset;
        if (c.bubble) {
          const bool mirrored = word(c.work + 0x1c) != 0;
          c.left = constant(mirrored ? -20244 : -20248);
          c.right = constant(mirrored ? -20248 : -20244);
          request(F::Quad, {0, 0, 0, 128},
                  {c.left + c.offset, constant(-20292), c.right + c.offset,
                   constant(-20268)},
                  {}, P::BubbleShadowDone);
        } else {
          for (unsigned i = 0; i < 32; ++i)
            c.xy[i] = real(0x802ef650 + 4 * i);
          for (unsigned i = 0; i < 32; ++i)
            c.uv[i] = real(0x802ef6d0 + 4 * i);
          c.shadowY = constant(-20264);
          c.phase = P::CircleBegin;
        }
        break;
      }
      case P::CircleBegin:
        request(F::Begin, {128, 0, 4}, {}, {}, P::CirclePoint);
        break;
      case P::CirclePoint: {
        const auto i = (c.quad % 4) * 8 + c.vertex * 2;
        const bool shadow = c.quad < 4;
        request(F::Point,
                shadow ? std::vector<uint32_t>{0, 0, 0, 128}
                       : std::vector<uint32_t>{c.gray, c.gray, c.gray, 255},
                {shadow ? c.offset + c.xy[i] : c.xy[i],
                 shadow ? c.shadowY + c.xy[i + 1] : c.xy[i + 1], c.uv[i],
                 c.uv[i + 1]},
                {}, P::CirclePointDone);
        break;
      }
      case P::CirclePointDone:
        if (++c.vertex < 4)
          c.phase = P::CirclePoint;
        else {
          c.vertex = 0;
          if (++c.quad < 8)
            c.phase = P::CircleBegin;
          else
            c.phase = P::CircleIconTranslation;
        }
        break;
      case P::CircleIconTranslation:
        translate(constant(-20500), constant(-20260), constant(-20500),
                  P::CircleIconTranslationDone);
        break;
      case P::CircleIconTranslationDone: {
        c.translation = c.result;
        const float value =
            constant(-20256) *
            std::fma(-constant(-20276), c.position[2], constant(-20280));
        scale(value, value, constant(-20452), P::CircleIconScaleDone);
        break;
      }
      case P::CircleIconScaleDone:
        c.scaled = c.result;
        rotate(c.radians, P::CircleIconRotationDone);
        break;
      case P::CircleIconRotationDone:
        c.next = P::CircleIdentityDone;
        c.phase = P::Identity;
        break;
      case P::CircleIdentityDone:
        c.rotation = c.result;
        concat(c.base, c.rotation, P::CircleIconComposeDone);
        break;
      case P::CircleIconComposeDone:
        concat(c.result, c.scaled, P::CircleIconScaleComposeDone);
        break;
      case P::CircleIconScaleComposeDone:
        concat(c.result, c.translation, P::CircleIconTranslateComposeDone);
        break;
      case P::CircleIconTranslateComposeDone: {
        const auto id = icon();
        const bool enabled = word(c.row + 4) == 1;
        const auto source =
            word(0x80427440u + uint32_t(enabled ? -20528 : -20524));
        const auto color =
            (c.gray << 24) | (c.gray << 16) | (c.gray << 8) | (source & 255);
        request(F::IconColor, {enabled ? 16u : 48u, id, color}, {}, c.result,
                P::Done);
        break;
      }
      case P::BubbleShadowDone:
        request(F::Quad, {255, 255, 255, 255},
                {c.left, constant(-20240), c.right, c.zero}, {},
                P::BubbleBodyDone);
        break;
      case P::BubbleBodyDone:
        translate(constant(-20236), constant(-20468), c.zero,
                  P::BubbleIconTranslationDone);
        break;
      case P::BubbleIconTranslationDone:
        c.translation = c.result;
        scale(c.one, c.one, c.one, P::BubbleIconScaleDone);
        break;
      case P::BubbleIconScaleDone:
        c.scaled = c.result;
        rotate(c.radians, P::BubbleIconRotationDone);
        break;
      case P::BubbleIconRotationDone:
        c.rotation = c.result;
        concat(c.base, c.rotation, P::BubbleIconComposeDone);
        break;
      case P::BubbleIconComposeDone:
        concat(c.result, c.translation, P::BubbleIconTranslationComposeDone);
        break;
      case P::BubbleIconTranslationComposeDone:
        concat(c.result, c.scaled, P::BubbleIconScaleComposeDone);
        break;
      case P::BubbleIconScaleComposeDone: {
        const auto id = icon();
        const bool enabled = word(c.row + 4) == 1;
        request(F::Icon, {enabled ? 16u : 48u, id}, {}, c.result,
                P::BubbleIconDone);
        break;
      }
      case P::BubbleIconDone:
        c.label = word(c.row + 8);
        translate(c.zero, constant(-20232), c.zero, P::TextTranslationDone);
        break;
      case P::TextTranslationDone:
        c.translation = c.result;
        request(F::FontWidth, {c.label}, {}, {}, P::TextWidthDone);
        break;
      case P::TextWidthDone: {
        const float width = float(c.value & 65535), maxWidth = constant(-20480),
                    fontScale = constant(-20228);
        const float x =
            width > maxWidth ? fontScale * (maxWidth / width) : fontScale;
        scale(x, fontScale, fontScale, P::TextScaleDone);
        break;
      }
      case P::TextScaleDone:
        c.scaled = c.result;
        rotate(c.radians, P::TextRotationDone);
        break;
      case P::TextRotationDone:
        c.rotation = c.result;
        concat(c.base, c.rotation, P::TextComposeDone);
        break;
      case P::TextComposeDone:
        concat(c.result, c.translation, P::TextTranslationComposeDone);
        break;
      case P::TextTranslationComposeDone:
        concat(c.result, c.scaled, P::TextScaleComposeDone);
        break;
      case P::TextScaleComposeDone:
        c.matrix = c.result;
        request(F::FontStart, {}, {}, {}, P::FontStarted);
        break;
      case P::FontStarted: {
        const bool enabled = word(c.row + 4) == 1;
        const auto color =
            word(0x80427440u + uint32_t(enabled ? -20540 : -20536));
        request(F::FontColor, {color}, {}, {}, P::FontColorDone);
        break;
      }
      case P::FontColorDone:
        request(F::FontString, {c.label}, {}, c.matrix, P::Done);
        break;
      case P::Request: {
        if (!s.request)
          return false;
        const auto value = s.request(s.user, c.request);
        if (!value)
          return false;
        c.value = *value;
        c.phase = c.next;
        break;
      }
      case P::Scale: {
        if (!s.math.scale)
          return false;
        const auto value =
            s.math.scale(s.math.user, c.xyz[0], c.xyz[1], c.xyz[2]);
        if (!value)
          return false;
        c.result = *value;
        c.phase = c.next;
        break;
      }
      case P::Translate: {
        if (!s.math.translation)
          return false;
        const auto value = s.math.translation(s.math.user, c.xyz);
        if (!value)
          return false;
        c.result = *value;
        c.phase = c.next;
        break;
      }
      case P::Rotate: {
        if (!s.math.rotation)
          return false;
        const auto value = s.math.rotation(s.math.user, c.number, 121);
        if (!value)
          return false;
        c.result = *value;
        c.phase = c.next;
        break;
      }
      case P::Concat: {
        if (!s.math.concat)
          return false;
        const auto value = s.math.concat(s.math.user, c.lhs, c.rhs);
        if (!value)
          return false;
        c.result = *value;
        c.phase = c.next;
        break;
      }
      case P::Identity: {
        if (!s.identity)
          return false;
        const auto value = s.identity(s.user);
        if (!value)
          return false;
        c.result = *value;
        c.phase = c.next;
        break;
      }
      case P::Truncate: {
        if (!s.truncate)
          return false;
        const auto value = s.truncate(s.user, c.number);
        if (!value)
          return false;
        c.truncated = *value;
        c.phase = c.next;
        break;
      }
      case P::Done:
        cursor.reset();
        return true;
      case P::Unavailable:
        return false;
      }
  }
};
bool resumePackedBattleClassCircle(uint32_t row, const BattleActMatrix &base,
                                   const std::array<float, 3> &position,
                                   float parameter,
                                   const BattleClassRasterServices &s,
                                   BattleClassRasterCursor &c) {
  return BattleClassRasterContinuationAccess::resume(false, 0, row, base,
                                                     position, parameter, s, c);
}
bool resumePackedBattleClassBubble(uint32_t work, uint32_t row,
                                   const BattleActMatrix &base, float parameter,
                                   const BattleClassRasterServices &s,
                                   BattleClassRasterCursor &c) {
  return BattleClassRasterContinuationAccess::resume(true, work, row, base, {},
                                                     parameter, s, c);
}
} // namespace ttyd
