#include "recovery_effect_draw.hpp"
#include <cstring>
namespace ttyd {
namespace {
struct Op {
  uint32_t function;
  std::array<uint32_t, 8> args{};
};
constexpr Op setup[] = {{0x802b5084, {2}},
                        {0x802b4e04, {0, 1, 4, 60, 0, 125}},
                        {0x802b4e04, {1, 1, 4, 30, 0, 125}},
                        {0x802b6f3c, {0}},
                        {0x802b5c8c, {0}},
                        {0x802b4520, {}},
                        {0x802b3c08, {9, 1}},
                        {0x802b3c08, {13, 1}},
                        {0x802b4558, {0, 9, 1, 4, 0}},
                        {0x802b4558, {0, 13, 1, 4, 0}}};
constexpr Op rootProgram[] = {{0x802b8fcc, {1}},
                              {0x802b8e30, {0, 0, 0, 255}},
                              {0x802b89e8, {0, 0, 0, 0, 1, 0}},
                              {0x802b8a50, {0, 0, 0, 0, 1, 0}},
                              {0x802b8960, {0, 15, 15, 15, 8}},
                              {0x802b89a4, {0, 7, 7, 7, 4}},
                              {0x802b5a6c, {128, 0, 4}}};
constexpr Op particleProgram[] = {{0x802b932c, {0, 1, 0, 0}},
                                  {0x802b940c, {0}},
                                  {0x802b8d60, {6, 128, 1, 0, 0}},
                                  {0x802b93d8, {1, 3, 1}},
                                  {0x802b8fcc, {1}},
                                  {0x802b8e30, {0, 0, 1, 255}},
                                  {0x802b89e8, {0, 0, 0, 0, 1, 0}},
                                  {0x802b8a50, {0, 0, 0, 0, 1, 0}},
                                  {0x802b8960, {0, 15, 2, 8, 15}},
                                  {0x802b89a4, {0, 7, 1, 4, 7}}};
float scalar(uint32_t bits) {
  float f;
  std::memcpy(&f, &bits, 4);
  return f;
}
int32_t sign(uint32_t bits) {
  int32_t n;
  std::memcpy(&n, &bits, 4);
  return n;
}
int32_t sbyte(uint8_t b) { return b < 128 ? b : int32_t(b) - 256; }
bool same(const RecoveryDrawServices &a, const RecoveryDrawServices &b) {
  return a.user == b.user && a.memory.user == b.memory.user &&
         a.memory.word == b.memory.word && a.byte == b.byte &&
         a.multiply == b.multiply && a.matrix == b.matrix && a.call == b.call &&
         a.quad == b.quad && a.number == b.number;
}
bool begin(uint32_t op, int32_t camera, uint32_t effect,
           const RecoveryDrawServices &s, RecoveryDrawCursor &c) {
  if (c.operation)
    return c.operation == op && c.cameraId == uint32_t(camera) &&
           c.effect == effect && same(c.owner, s);
  if (!s.memory.word || !s.byte || !s.multiply)
    return false;
  c = {};
  c.operation = op;
  c.cameraId = uint32_t(camera);
  c.effect = effect;
  c.owner = s;
  return true;
}
struct Run {
  const RecoveryDrawServices &s;
  RecoveryDrawCursor &c;
  uint32_t w(uint32_t a) { return s.memory.word(s.memory.user, a); }
  float f(uint32_t a) { return scalar(w(a)); }
  std::array<float, 3> xyz(uint32_t a) {
    const float x = f(a), y = f(a + 4), z = f(a + 8);
    return {x, y, z};
  }
  BattleActMatrix matrixAt(uint32_t a) {
    BattleActMatrix m{};
    for (unsigned i = 0; i < 12; ++i)
      m[i] = f(a + 4 * i);
    return m;
  }
  void request(uint32_t function, std::array<uint32_t, 8> args = {}) {
    c.request = {};
    c.request.function = function;
    c.request.integers = args;
    c.callReady = true;
  }
  bool call() {
    if (!s.call)
      return false;
    auto r = s.call(s.user, c.request);
    if (!r)
      return false;
    c.result = *r;
    c.callReady = false;
    return true;
  }
  bool call(uint32_t function, std::array<uint32_t, 8> args = {}) {
    if (!c.callReady)
      request(function, args);
    return call();
  }
  template <size_t N> bool table(const Op (&ops)[N]) {
    while (c.step < N) {
      const auto &op = ops[c.step];
      if (!call(op.function, op.args))
        return false;
      ++c.step;
    }
    c.step = 0;
    return true;
  }
  void mathRequest(uint32_t function) {
    c.math = {};
    c.math.function = function;
    c.mathReady = true;
  }
  bool math(unsigned out) {
    if (!s.matrix)
      return false;
    auto next = c.math;
    if (!s.matrix(s.user, next))
      return false;
    c.matrices[out] = next.output;
    c.mathReady = false;
    return true;
  }
  bool translate(unsigned out, uint32_t at) {
    if (!c.mathReady) {
      mathRequest(0x8029e230);
      c.math.xyz = xyz(at);
    }
    return math(out);
  }
  bool translate(unsigned out, float x, float y, float z) {
    if (!c.mathReady) {
      mathRequest(0x8029e230);
      c.math.xyz = {x, y, z};
    }
    return math(out);
  }
  bool concat(unsigned out, unsigned left, unsigned right) {
    if (!c.mathReady) {
      mathRequest(0x8029dd64);
      c.math.left = c.matrices[left];
      c.math.right = c.matrices[right];
    }
    return math(out);
  }
  bool guestConcat(unsigned out, uint32_t left, unsigned right) {
    if (!c.mathReady) {
      mathRequest(0x8029dd64);
      c.math.left = matrixAt(left);
      c.math.right = c.matrices[right];
    }
    return math(out);
  }
  bool rightGuest(unsigned out, unsigned left, uint32_t right) {
    if (!c.mathReady) {
      mathRequest(0x8029dd64);
      c.math.left = c.matrices[left];
      c.math.right = matrixAt(right);
    }
    return math(out);
  }
  bool uniform(unsigned out, uint32_t address) {
    if (!c.mathReady) {
      mathRequest(0x8029e2b0);
      const auto n = f(address);
      c.math.xyz = {n, n, n};
    }
    return math(out);
  }
  bool rotate(unsigned out) {
    if (!c.mathReady) {
      const auto yaw = f(c.camera + 0x114), factor = f(0x80422dc0);
      mathRequest(0x8029dff0);
      c.math.axis = 121;
      c.math.radians = s.multiply(s.user, factor, -yaw);
    }
    return math(out);
  }
  bool loadPosition(unsigned m) {
    if (!c.callReady) {
      request(0x802ba2fc, {0});
      c.request.matrix = c.matrices[m];
    }
    return call();
  }
  bool quad(bool particle) {
    if (!c.geometryReady) {
      // Capture the original twenty FIFO constant loads, in order. Emission
      // itself is atomic in this port; a held renderer cannot consume half a
      // quad.
      const uint32_t lo = particle ? 0x80422e0c : 0x80422e08,
                     hi = particle ? 0x80422e10 : 0x80422de0;
      const uint32_t zero = 0x80422dcc, one = 0x80422dd0,
                     wide = particle ? 0x80422dd8 : one;
      const std::array<std::array<uint32_t, 5>, 4> reads = {
          {{lo, hi, zero, zero, zero},
           {hi, hi, zero, wide, zero},
           {hi, lo, zero, wide, one},
           {lo, lo, zero, zero, one}}};
      for (unsigned i = 0; i < 4; ++i) {
        for (unsigned j = 0; j < 3; ++j)
          c.geometry.positions[i][j] = f(reads[i][j]);
        for (unsigned j = 0; j < 2; ++j)
          c.geometry.uv[i][j] = f(reads[i][3 + j]);
      }
      if (particle) {
        ++c.index;
        c.particle += 0x44;
      }
      c.geometryReady = true;
    }
    if (!s.quad || !s.quad(s.user, c.geometry))
      return false;
    c.geometryReady = false;
    return true;
  }
  bool finish() {
    c = {};
    return true;
  }
};
} // namespace
bool resumeRecoveryEffectDraw(int32_t camera, uint32_t effect,
                              const RecoveryDrawServices &s,
                              RecoveryDrawCursor &c) {
  if (!begin(1, camera, effect, s, c))
    return false;
  Run r{s, c};
  for (;;)
    switch (c.phase) {
    case 0:
      c.payload = r.w(effect + 0xc);
      c.phase = 1;
      break;
    case 1:
      if (!r.call(0x8000f2fc, {uint32_t(camera)}))
        return false;
      c.firstCamera = c.result;
      c.kind = sbyte(s.byte(s.user, c.payload));
      c.phase = 2;
      break;
    case 2:
      if (!r.translate(0, c.payload + 8))
        return false;
      c.phase = 3;
      break;
    case 3:
      if (!r.call(0x8000f2fc, {uint32_t(camera)}))
        return false;
      c.camera = c.result;
      c.phase = 4;
      break;
    case 4:
      if (!r.rotate(1))
        return false;
      c.phase = 5;
      break;
    case 5:
      if (!r.concat(2, 0, 1))
        return false;
      c.phase = 6;
      break;
    case 6:
      if (!r.table(setup))
        return false;
      c.phase = 7;
      break;
    case 7: {
      uint32_t texture = 0;
      switch (c.kind) {
      case 0:
      case 2:
      case 4:
      case 7:
        texture = 18;
        break;
      case 1:
      case 3:
      case 5:
      case 8:
        texture = 19;
        break;
      default:
        break;
      }
      if (!texture) {
        c.phase = 11;
        break;
      }
      if (!r.call(0x8005ed58, {texture}))
        return false;
      c.texture = c.result;
      c.phase = 8;
      break;
    }
    case 8:
      if (!r.call(0x802b7880, {c.texture, 0}))
        return false;
      c.phase = 9;
      break;
    case 9:
      if (!r.call(0x8005ed58, {uint32_t(c.kind == 0 || c.kind == 2 ||
                                                c.kind == 4 || c.kind == 7
                                            ? 20
                                            : 21)}))
        return false;
      c.texture = c.result;
      c.phase = 10;
      break;
    case 10:
      if (!r.call(0x802b7880, {c.texture, 1}))
        return false;
      c.phase = 11;
      break;
    case 11:
      c.payload = r.w(effect + 0xc);
      c.phase = 12;
      break;
    case 12:
      if (!r.translate(0, c.payload + 8))
        return false;
      c.phase = 13;
      break;
    case 13:
      if (!r.call(0x8000f2fc, {uint32_t(camera)}))
        return false;
      c.camera = c.result;
      c.phase = 14;
      break;
    case 14:
      if (!r.rotate(1))
        return false;
      c.phase = 15;
      break;
    case 15:
      if (!r.concat(0, 0, 1))
        return false;
      c.phase = 16;
      break;
    case 16:
      if (!r.uniform(1, 0x80422de4))
        return false;
      c.phase = 17;
      break;
    case 17:
      if (!r.concat(0, 0, 1))
        return false;
      c.phase = 18;
      break;
    case 18:
      if (!c.mathReady) {
        r.mathRequest(0x8029e230);
        const auto zero = r.f(0x80422dcc), y = r.f(0x80422dd8);
        c.math.xyz = {zero, y, zero};
      }
      if (!r.math(1))
        return false;
      c.phase = 19;
      break;
    case 19:
      if (!r.concat(0, 0, 1))
        return false;
      c.phase = 20;
      break;
    case 20:
      if (uint32_t(c.kind) <= 1 || uint32_t(c.kind - 7) <= 1 || c.kind == 4 ||
          c.kind == 5) {
        if (!c.mathReady) {
          const auto maya = r.w(c.payload + 0x38);
          r.mathRequest(0x8029dd64);
          c.math.left = c.matrices[0];
          c.math.right = r.matrixAt(maya + 0x14);
        }
        if (!r.math(0))
          return false;
      }
      c.phase = 21;
      break;
    case 21:
      if (!r.guestConcat(0, c.firstCamera + 0x11c, 0))
        return false;
      c.phase = 22;
      break;
    case 22:
      if (!r.uniform(1, 0x80422e04))
        return false;
      c.phase = 23;
      break;
    case 23:
      if (!r.concat(0, 0, 1))
        return false;
      c.phase = 24;
      break;
    case 24:
      if (!r.loadPosition(0))
        return false;
      c.phase = 25;
      break;
    case 25:
      if (!r.call(0x802ba39c, {0}))
        return false;
      c.phase = 26;
      break;
    case 26:
      if (!r.table(rootProgram))
        return false;
      c.phase = 27;
      break;
    case 27:
      if (!r.quad(false))
        return false;
      if (c.kind == 2 || c.kind == 3)
        return r.finish();
      c.phase = 28;
      break;
    case 28:
      if (!r.table(particleProgram))
        return false;
      c.index = 1;
      c.particle = c.payload + 0x44;
      c.phase = 29;
      break;
    case 29:
      if (sign(r.w(effect + 8)) <= sign(c.index))
        return r.finish();
      c.phase = 30;
      break;
    case 30:
      if (!c.callReady) {
        const auto gp = r.w(0x80415fd0);
        (void)r.w(0x804295e8);
        const auto tick = r.w(gp + 0x1c) + c.index;
        const auto alpha = r.w(c.particle + 4) & 255;
        const auto palette = 0x804179d8 + (tick % 12) * 3;
        const uint8_t red = s.byte(s.user, palette),
                      green = s.byte(s.user, palette + 1),
                      blue = s.byte(s.user, palette + 2);
        r.request(0x802b8ab8,
                  {1, (uint32_t(red) << 24) | (uint32_t(green) << 16) |
                          (uint32_t(blue) << 8) | alpha});
      }
      if (!r.call())
        return false;
      c.phase = 31;
      break;
    case 31:
      if (!r.translate(0, c.particle + 8))
        return false;
      c.phase = 32;
      break;
    case 32:
      if (!c.mathReady) {
        r.mathRequest(0x8029e2b0);
        c.math.xyz = r.xyz(c.particle + 0x20);
      }
      if (!r.math(1))
        return false;
      c.phase = 33;
      break;
    case 33:
      if (!r.concat(0, 0, 1))
        return false;
      c.phase = 34;
      break;
    case 34:
      if (!r.concat(0, 2, 0))
        return false;
      c.phase = 35;
      break;
    case 35:
      if (!r.call(0x8000f2fc, {uint32_t(camera)}))
        return false;
      c.camera = c.result;
      c.phase = 36;
      break;
    case 36:
      if (!r.guestConcat(0, c.camera + 0x11c, 0))
        return false;
      c.phase = 37;
      break;
    case 37:
      if (!r.loadPosition(0))
        return false;
      c.phase = 38;
      break;
    case 38:
      if (!r.call(0x802ba39c, {0}))
        return false;
      c.phase = 39;
      break;
    case 39:
      if (!r.call(0x802b5a6c, {128, 0, 4}))
        return false;
      c.phase = 40;
      break;
    case 40:
      if (!r.quad(true))
        return false;
      c.phase = 29;
      break;
    default:
      return false;
    }
}
bool resumeRecoveryEffectNumberDraw(int32_t camera, uint32_t effect,
                                    const RecoveryDrawServices &s,
                                    RecoveryDrawCursor &c) {
  if (!begin(2, camera, effect, s, c))
    return false;
  Run r{s, c};
  for (;;)
    switch (c.phase) {
    case 0:
      c.payload = r.w(effect + 0xc);
      c.phase = 1;
      break;
    case 1:
      if (!r.call(0x8000f2fc, {uint32_t(camera)}))
        return false;
      c.amount = sbyte(s.byte(s.user, c.payload + 1));
      c.kind = sbyte(s.byte(s.user, c.payload));
      c.phase = 2;
      break;
    case 2:
      if (!r.call(0x8026a0b8, {0x80422df8, uint32_t(c.amount)}))
        return false;
      c.string = c.result;
      c.phase = 3;
      break;
    case 3:
      if (!r.call(0x8026d6b4, {c.string}))
        return false;
      c.length = c.result;
      c.phase = 4;
      break;
    case 4:
      if (!r.translate(0, c.payload + 8))
        return false;
      c.phase = 5;
      break;
    case 5:
      if (!r.call(0x8000f2fc, {uint32_t(camera)}))
        return false;
      c.camera = c.result;
      c.phase = 6;
      break;
    case 6:
      if (!r.rotate(1))
        return false;
      c.phase = 7;
      break;
    case 7:
      if (!r.concat(0, 0, 1))
        return false;
      c.phase = 8;
      break;
    case 8:
      if (!c.mathReady) {
        r.mathRequest(0x8029e230);
        const auto zero = r.f(0x80422dcc), z = r.f(0x80422dd0);
        c.math.xyz = {zero, zero, z};
      }
      if (!r.math(1))
        return false;
      c.phase = 9;
      break;
    case 9:
      if (!r.concat(0, 0, 1))
        return false;
      c.phase = 10;
      break;
    case 10:
      if (!r.uniform(1, 0x80422de4))
        return false;
      c.phase = 11;
      break;
    case 11:
      if (!r.concat(0, 0, 1))
        return false;
      c.phase = 12;
      break;
    case 12:
      if (!c.mathReady) {
        r.mathRequest(0x8029e230);
        const auto zero = r.f(0x80422dcc), y = r.f(0x80422dd8);
        c.math.xyz = {zero, y, zero};
      }
      if (!r.math(1))
        return false;
      c.phase = 13;
      break;
    case 13:
      if (!r.concat(0, 0, 1))
        return false;
      c.phase = 14;
      break;
    case 14:
      if (!c.mathReady) {
        const auto maya = r.w(c.payload + 0x38);
        r.mathRequest(0x8029dd64);
        c.math.left = c.matrices[0];
        c.math.right = r.matrixAt(maya + 0x14);
      }
      if (!r.math(0))
        return false;
      c.phase = 15;
      break;
    case 15:
      if (!r.uniform(1, 0x80422dfc))
        return false;
      c.phase = 16;
      break;
    case 16:
      if (!r.concat(0, 0, 1))
        return false;
      c.phase = 17;
      break;
    case 17:
      if (!c.mathReady) {
        const auto integer = 0u - (c.length - 1u);
        const auto zero = r.f(0x80422dcc);
        const uint64_t biasBits =
            (uint64_t(r.w(0x802f3b68)) << 32) | r.w(0x802f3b6c);
        double bias;
        std::memcpy(&bias, &biasBits, 8);
        const auto width = r.f(0x80422e00), half = r.f(0x80422ddc);
        const uint64_t valueBits =
            0x4330000000000000ull | (integer ^ 0x80000000u);
        double value;
        std::memcpy(&value, &valueBits, 8);
        volatile float converted = float(value - bias);
        const auto x =
            s.multiply(s.user, s.multiply(s.user, width, converted), half);
        r.mathRequest(0x8029e230);
        c.math.xyz = {x, zero, zero};
      }
      if (!r.math(1))
        return false;
      c.phase = 18;
      break;
    case 18:
      if (!r.concat(0, 0, 1))
        return false;
      c.number = {};
      c.number.matrix = c.matrices[0];
      c.number.amount = c.amount;
      c.number.style = (c.kind == 1 || c.kind == 3 || c.kind == 8) ? 1 : 0;
      c.phase = 19;
      break;
    case 19:
      if (c.kind == 0 || c.kind == 1 || c.kind == 2 || c.kind == 3 ||
          c.kind == 7 || c.kind == 8) {
        if (!s.number || !s.number(s.user, c.number))
          return false;
      }
      return r.finish();
    default:
      return false;
    }
}
} // namespace ttyd
