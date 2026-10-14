#include "source_icon_gx.hpp"
namespace ttyd {
namespace {
struct Op {
  uint32_t function;
  std::array<uint32_t, 8> args{};
  uint32_t colorAddress = 0;
  bool iconColor = false;
};
constexpr Op unlit[] = {{0x802b6f3c, {1}},
                        {0x802b6e54, {4}, 0x8041f868},
                        {0x802b6f78, {4, 0, 0, 0, 0, 0, 2}}};
constexpr Op normal[] = {{0x802b8fcc, {2}},
                         {0x802b8e30, {0, 0, 0, 4}},
                         {0x802b88d4, {0, 0}},
                         {0x802b8c98, {0, 0, 0}},
                         {0x802b8ab8, {1}, 0, true},
                         {0x802b8e30, {1, 255, 255, 255}},
                         {0x802b89e8, {1, 0, 0, 0, 1, 0}},
                         {0x802b8a50, {1, 0, 0, 0, 1, 0}},
                         {0x802b8960, {1, 15, 2, 0, 15}},
                         {0x802b89a4, {1, 7, 1, 0, 7}}};
constexpr Op grayscale[] = {{0x802b8fcc, {3}},
                            {0x802b8ce0, {1, 0, 0, 0, 3}},
                            {0x802b8ce0, {2, 1, 1, 1, 3}},
                            {0x802b8ce0, {3, 2, 2, 2, 3}},
                            {0x802b8c98, {0, 0, 1}},
                            {0x802b8c98, {1, 0, 2}},
                            {0x802b8c98, {2, 0, 3}},
                            {0x802b8ab8, {1}, 0x8041f86c},
                            {0x802b8ab8, {2}, 0, true},
                            {0x802b8e30, {0, 0, 0, 255}},
                            {0x802b89e8, {0, 0, 0, 0, 1, 0}},
                            {0x802b8a50, {0, 0, 0, 0, 1, 0}},
                            {0x802b8960, {0, 15, 2, 8, 15}},
                            {0x802b89a4, {0, 7, 7, 7, 4}},
                            {0x802b8e30, {1, 0, 0, 255}},
                            {0x802b89e8, {1, 0, 0, 0, 1, 0}},
                            {0x802b8a50, {1, 0, 0, 0, 1, 0}},
                            {0x802b8960, {1, 15, 2, 8, 0}},
                            {0x802b89a4, {1, 7, 7, 7, 4}},
                            {0x802b8e30, {2, 0, 0, 255}},
                            {0x802b89e8, {2, 0, 0, 0, 1, 0}},
                            {0x802b8a50, {2, 0, 0, 0, 1, 0}},
                            {0x802b8960, {2, 15, 2, 8, 0}},
                            {0x802b89a4, {2, 7, 2, 4, 7}}};
constexpr Op overlay[] = {{0x802b8ab8, {2}},
                          {0x802b8fcc, {3}},
                          {0x802b8e30, {2, 255, 255, 255}},
                          {0x802b89e8, {2, 0, 0, 0, 1, 0}},
                          {0x802b8a50, {2, 0, 0, 0, 1, 0}},
                          {0x802b8960, {2, 15, 0, 4, 15}},
                          {0x802b89a4, {2, 7, 0, 2, 7}}};
constexpr Op texgen[] = {{0x802b5084, {1}},
                         {0x802b4e04, {0, 1, 4, 60, 0, 125}}};
constexpr Op blend[] = {{0x802b932c, {1, 4, 5, 0}},
                        {0x802b940c, {1}},
                        {0x802b8d60, {7, 0, 0, 7, 0}},
                        {0x802b93d8, {1, 3, 0}}};
constexpr Op opaque[] = {{0x802b932c, {0, 4, 5, 0}},
                         {0x802b940c, {0}},
                         {0x802b8d60, {6, 128, 1, 0, 0}},
                         {0x802b93d8, {1, 3, 1}}};
constexpr Op descriptors[] = {{0x802b4520, {}},
                              {0x802b3c08, {9, 1}},
                              {0x802b3c08, {13, 1}},
                              {0x802b4558, {0, 9, 1, 4, 0}},
                              {0x802b4558, {0, 13, 1, 4, 0}}};
constexpr Op reset[] = {
    {0x802b8c98, {0, 0, 0}}, {0x802b8c98, {1, 0, 0}}, {0x802b8c98, {2, 0, 0}}};
struct Run {
  const SourceIconGXServices &s;
  SourceIconGXCursor &c;
  std::optional<uint32_t> word(uint32_t a) {
    return s.word ? s.word(s.user, a) : std::optional<uint32_t>{};
  }
  std::optional<uint32_t> invoke() {
    c.pending = c.request.function;
    return s.call ? s.call(s.user, c.request) : std::optional<uint32_t>{};
  }
  std::optional<uint32_t> call(uint32_t f, std::array<uint32_t, 8> args = {}) {
    if (!c.ready) {
      c.request = {};
      c.request.function = f;
      c.request.integers = args;
      c.ready = true;
    }
    auto result = invoke();
    if (result)
      c.ready = false;
    return result;
  }
  template <size_t N>
  bool sequence(const Op (&ops)[N], bool overlayColor = false) {
    while (c.step < N) {
      const auto &op = ops[c.step];
      if (!c.ready) {
        c.request = {};
        c.request.function = op.function;
        c.request.integers = op.args;
        if (op.colorAddress) {
          const auto color = word(op.colorAddress);
          if (!color)
            return false;
          c.request.integers[1] = *color;
        }
        if (op.iconColor)
          c.request.integers[1] = c.icon.rgba;
        if (overlayColor && !c.step)
          c.request.integers[1] = c.color;
        c.ready = true;
      }
      if (!invoke())
        return false;
      c.ready = false;
      ++c.step;
    }
    c.step = 0;
    return true;
  }
  bool math(uint32_t function, const BattleActMatrix &left = {},
            const BattleActMatrix &right = {}, std::array<float, 3> xyz = {}) {
    if (!c.mathReady) {
      c.math = {};
      c.math.function = function;
      c.math.left = left;
      c.math.right = right;
      c.math.xyz = xyz;
      c.mathReady = true;
    }
    c.pending = function;
    if (!s.matrix || !s.matrix(s.user, c.math))
      return false;
    c.mathReady = false;
    return true;
  }
};
} // namespace
bool resumeSourceIconGX(const BattleActMatrix &input,
                        const SourceMatrixIcon &icon,
                        const SourceIconGXServices &services,
                        SourceIconGXCursor &c) {
  if (!c.active) {
    c = {};
    c.active = true;
    c.input = input;
    c.icon = icon;
  }
  Run r{services, c};
  const auto flags = c.icon.flags;
  if (c.phase == 0) {
    if (!r.math(0x8029e2b0, {}, {},
                {flags & 4 ? -1.f : 1.f, flags & 8 ? -1.f : 1.f, 1}))
      return false;
    c.scale = c.math.output;
    c.phase = 1;
  }
  if (c.phase == 1) {
    if (!r.math(0x8029dd64, c.input, c.scale))
      return false;
    c.transform = c.math.output;
    c.phase = 2;
  }
  if (c.phase == 2) {
    const auto value = r.call(0x8000f2fc, {8});
    if (!value)
      return false;
    c.camera8 = *value;
    c.phase = 3;
  }
  if (c.phase == 3) {
    const auto value = r.call(0x8000f2e4);
    if (!value)
      return false;
    c.flip = *value != c.camera8 && (flags & 16);
    c.phase = 4;
  }
  if (c.phase == 4) {
    if (c.flip) {
      if (!r.math(0x8029e2b0, {}, {}, {1, -1, 1}))
        return false;
      c.scale = c.math.output;
    }
    c.phase = 5;
  }
  if (c.phase == 5) {
    if (c.flip) {
      if (!r.math(0x8029dd64, c.transform, c.scale))
        return false;
      c.transform = c.math.output;
    }
    c.phase = 6;
  }
  if (c.phase == 6) {
    if (!c.ready) {
      c.request = {};
      c.request.function = 0x802ba2fc;
      c.request.matrix = c.transform;
      c.ready = true;
    }
    if (!r.invoke())
      return false;
    c.ready = false;
    c.phase = 7;
  }
  if (c.phase == 7) {
    if (!r.call(0x802ba39c, {0}))
      return false;
    c.phase = 8;
  }
  if (c.phase == 8) {
    c.pending = 0x800197f8;
    const auto bin = r.word(0x8041e638);
    if (!bin)
      return false;
    c.loaded = false;
    if (*bin) {
      const auto tpl = r.word(0x8041e63c);
      if (!tpl)
        return false;
      c.loaded = *tpl != 0;
    }
    c.phase = 9;
  }
  if (c.phase == 9) {
    if (!c.ready) {
      c.request = {};
      if (c.loaded) {
        const auto palette = r.word(0x8041e630);
        if (!palette)
          return false;
        c.request.function = 0x80037a80;
        c.request.integers = {*palette, c.icon.texture};
      } else {
        c.request.function = 0x802b72b0;
        c.request.integers = {0x80415080, 1, 1, 0, 0, 0, 0};
      }
      c.ready = true;
    }
    const auto token = r.invoke();
    if (!token)
      return false;
    c.texture = *token;
    c.ready = false;
    c.phase = 10;
  }
  if (c.phase == 10) {
    if (c.loaded && !r.call(0x802b7544, {c.texture, 1, 1, 0, 0, 0}))
      return false;
    c.phase = 11;
  }
  if (c.phase == 11) {
    if (!r.call(0x802b7880, {c.texture, 0}))
      return false;
    c.phase = 12;
  }
  if (c.phase == 12) {
    if (flags & 16) {
      if (!r.sequence(unlit))
        return false;
    } else {
      if (!c.ready) {
        c.request = {};
        c.request.function = 0x800208c0;
        c.request.integers = {4};
        c.request.reals = {c.input[3], c.input[7], c.input[11]};
        c.ready = true;
      }
      if (!r.invoke())
        return false;
      c.ready = false;
    }
    c.phase = 13;
  }
  if (c.phase == 13) {
    if (flags & 32) {
      if (!r.sequence(grayscale))
        return false;
      c.phase = 17;
    } else {
      if (!r.sequence(normal))
        return false;
      c.phase = 14;
    }
  }
  if (c.phase == 14) {
    if (flags & (16 | 64 | 128))
      c.phase = 17;
    else {
      const auto map = r.call(0x8002b438);
      if (!map)
        return false;
      c.map = *map;
      c.phase = 15;
    }
  }
  if (c.phase == 15) {
    c.pending = 0x800197f8;
    if (!services.half)
      return false;
    const auto value = services.half(services.user, c.map + 4);
    if (!value)
      return false;
    c.phase = *value & 2 ? 16 : 17;
  }
  if (c.phase == 16) {
    const auto color = r.call(0x8001d968);
    if (!color)
      return false;
    c.color = *color;
    c.phase = 160;
  }
  if (c.phase == 160) {
    if (!r.sequence(overlay, true))
      return false;
    c.phase = 17;
  }
  if (c.phase == 17) {
    if (!r.sequence(texgen))
      return false;
    c.phase = 18;
  }
  if (c.phase == 18) {
    if (flags & 16) {
      if (!c.ready) {
        const auto color = r.word(0x8041f870);
        if (!color)
          return false;
        c.request = {};
        c.request.function = 0x802b8ff4;
        c.request.integers = {0, *color};
        c.ready = true;
      }
      if (!r.invoke())
        return false;
      c.ready = false;
    } else if (!r.call(0x800217c8))
      return false;
    c.phase = 19;
  }
  if (c.phase == 19) {
    if ((c.icon.rgba & 255) < 255) {
      if (!r.sequence(blend))
        return false;
    } else {
      if (!r.sequence(opaque))
        return false;
    }
    c.phase = 20;
  }
  if (c.phase == 20) {
    if ((flags & 16) && !r.call(0x802b93d8, {0, 3, 0}))
      return false;
    c.phase = 21;
  }
  if (c.phase == 21) {
    if (!r.call(0x802b5c8c, {flags & 0x400 ? 2u : flags & 0x800 ? 1u : 0u}))
      return false;
    c.phase = 22;
  }
  if (c.phase == 22) {
    if (!r.sequence(descriptors))
      return false;
    c.phase = 23;
  }
  if (c.phase == 23) {
    if (!c.icon.width && !c.icon.height) {
      const auto value = r.call(0x802b76c4, {c.texture});
      if (!value)
        return false;
      c.width = float(uint16_t(*value));
      c.phase = 24;
    } else {
      c.width = float(c.icon.width);
      c.height = float(c.icon.height);
      c.phase = 25;
    }
  }
  if (c.phase == 24) {
    const auto value = r.call(0x802b76d4, {c.texture});
    if (!value)
      return false;
    c.height = float(uint16_t(*value));
    c.phase = 25;
  }
  if (c.phase == 25) {
    float left = (-c.width) * .5f, top = c.height, width = c.width,
          down = -c.height;
    if (!(flags & 16)) {
      left *= .6f;
      top *= .6f;
      width *= .6f;
      down *= .6f;
      if (c.icon.id == 403) {
        left *= .8f;
        top *= .8f;
        width *= .8f;
        down *= .8f;
      } else if (c.icon.id == 405) {
        left *= .9f;
        top *= .9f;
        width *= .9f;
        down *= .9f;
      }
    }
    const float right = left + width, bottom = top + down;
    c.geometry.positions = {{{left, top, 0},
                             {right, top, 0},
                             {right, bottom, 0},
                             {left, bottom, 0}}};
    c.phase = 26;
  }
  if (c.phase == 26) {
    if (!r.call(0x802b5a6c, {128, 0, 4}))
      return false;
    c.phase = 27;
  }
  if (c.phase == 27) {
    c.pending = 0x800197f8;
    if (!services.quad || !services.quad(services.user, c.geometry))
      return false;
    c.phase = 28;
  }
  if (c.phase == 28) {
    if ((flags & 32) && !r.sequence(reset))
      return false;
    c.active = false;
    c.pending = 0;
    return true;
  }
  return false;
}
} // namespace ttyd
