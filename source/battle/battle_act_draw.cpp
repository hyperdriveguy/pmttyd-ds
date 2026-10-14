#include "battle_act_draw.hpp"
#include <cmath>
#include <cstring>
namespace ttyd {
namespace {
int32_t signedWord(uint32_t n) {
  int32_t v;
  std::memcpy(&v, &n, 4);
  return v;
}
float scalar(uint32_t n) {
  float v;
  std::memcpy(&v, &n, 4);
  return v;
}
} // namespace
bool drawPackedBattleActMenu(const BattleActDrawServices &s) {
  const auto &m = s.memory.memory;
  auto word = [&](uint32_t a) { return m.word(m.user, a); };
  auto put = [&](uint32_t a, uint32_t n) { m.storeWord(m.user, a, n); };
  auto f = [&](uint32_t a) { return scalar(word(a)); };
  auto constant = [&](int32_t offset) {
    return f(0x80427440u + uint32_t(offset));
  };
  auto rate = [&]() { return word(word(0x80415fd0) + 4); };
  auto duration = [&](uint32_t n) { return signedWord(rate() * n) / 60; };
  const auto b = currentBattleAddress(s.memory), w = word(b + 0x1c78),
             c = word(w);
  if (!s.camera || !s.camera(s.user, 8))
    return false;
  const auto mode = signedWord(word(w + 0xc));
  auto allStable = [&]() {
    const auto count = word(c + 8);
    uint32_t n = 0;
    for (uint32_t i = 0; signedWord(i) < signedWord(count); ++i)
      if (word(w + 0x34 + i * 4) == 3)
        ++n;
    if (n == count)
      put(w + 0xc, 1);
  };
  if (mode == 0 || mode == 2)
    allStable();
  else if (mode == 10) {
    put(w + 0xc, 1);
    m.storeByte(m.user, w + 0x14, 1);
  } else if (mode == 1) {
    m.storeByte(m.user, w + 0x14, 1);
    const auto liveCursor = word(w), previous = word(w + 0x18);
    const auto delta = word(liveCursor) - previous;
    bool next = delta == 1;
    uint32_t retained = previous;
    if (!next) {
      retained = word(liveCursor + 8) - 1;
      next = delta == 0u - retained;
    }
    const bool reverse = !next && (delta == uint32_t(-1) || delta == retained);
    if (next || reverse) {
      put(w + 0xc, 2);
      m.storeByte(m.user, w + 0x14, 0);
      put(w + 0x18, word(word(w)));
      for (uint32_t i = 0; signedWord(i) < signedWord(word(c + 8)); ++i) {
        const auto a = w + i * 4;
        put(a + 0x34, next ? 4 : 5);
        put(a + 0x48, 0);
        put(a + 0x70, word(a + 0x84));
        put(a + 0x84, word(a + 0x84) + (next ? uint32_t(-1) : 1u));
        if (next) {
          if (word(a + 0x84) == uint32_t(-1))
            put(a + 0x84, word(c + 8) - 1);
        } else if (word(a + 0x84) == word(c + 8))
          put(a + 0x84, 0);
      }
    }
  }
  put(w + 0x1c, 0);
  std::vector<BattleActDrawSlot> slots;
  for (uint32_t i = 0;; ++i) {
    const auto loopCount = word(c + 8);
    if (signedWord(i) >= signedWord(loopCount))
      break;
    slots.emplace_back();
    auto &slot = slots.back();
    const auto a = w + i * 4;
    const auto state = word(a + 0x34);
    if (state > 11)
      continue;
    if (state == 0) {
      put(a + 0x48, word(a + 0x48) - 1);
      if (signedWord(word(a + 0x48)) <= 0) {
        put(a + 0x34, 1);
        put(a + 0x48, 0);
      }
      continue;
    }
    if (state != 1 && state != 3 && state != 4 && state != 5 && state != 10 &&
        state != 11)
      continue;
    auto ringAngle = [&](uint32_t count, uint32_t order) {
      return f(0x803635d8 + count * 20 + order * 4 - 60);
    };
    const float zero = constant(-20500), pi = constant(-20492),
                degrees = constant(-20488);
    auto radians = [&](float angle) { return (pi * angle) / degrees; };
    float angle = zero, entryEndpoint = zero;
    if (state == 1) {
      put(a + 0x20, word(a + 0x20) | 1);
      const auto gp = word(0x80415fd0), count = word(c + 8);
      const auto freshRate = word(gp + 4), order = word(a + 0x84);
      const auto target = ringAngle(count, order);
      const auto counter = word(a + 0x48);
      const float step = constant(-20496) / float(signedWord(freshRate));
      const float endpoint = radians(target);
      entryEndpoint = endpoint;
      angle = std::fma(float(signedWord(counter)), step, zero);
      if (angle >= endpoint)
        angle = endpoint;
    } else if (state == 3 || state == 11)
      angle = radians(
          ringAngle(state == 3 ? loopCount : word(c + 8), word(a + 0x84)));
    else if (state == 4) {
      const auto order = word(a + 0x84), oldOrder = word(a + 0x70);
      auto target = ringAngle(loopCount, order);
      const auto previous = ringAngle(loopCount, oldOrder);
      if (target == zero)
        target = constant(-20476);
      const auto freshRate = rate(), counter = word(a + 0x48);
      const float endpoint = radians(target), start = radians(previous);
      const float step = constant(-20496) / float(signedWord(freshRate));
      angle = std::fma(float(signedWord(counter)), step, start);
      if (angle >= endpoint) {
        angle = endpoint;
        put(a + 0x34, 3);
        put(a + 0x48, uint32_t(-1));
      }
    } else if (state == 5) {
      auto previous = ringAngle(loopCount, word(a + 0x70));
      if (previous == zero)
        previous = constant(-20476);
      const auto gp = word(0x80415fd0), order = word(a + 0x84);
      const auto freshRate = word(gp + 4);
      const auto target = ringAngle(loopCount, order);
      const auto counter = word(a + 0x48);
      const float start = radians(previous), endpoint = radians(target);
      const float step = constant(-20496) / float(signedWord(freshRate));
      angle = std::fma(-float(signedWord(counter)), step, start);
      if (angle <= endpoint) {
        angle = endpoint;
        put(a + 0x34, 3);
        put(a + 0x48, uint32_t(-1));
      }
    } else if (state == 10) {
      put(a + 0x20, word(a + 0x20) | 1);
      put(a + 0x20, word(a + 0x20) | 2);
    }
    if (state == 11) {
      put(a + 0x20, word(a + 0x20) | 1);
      put(a + 0x20, word(a + 0x20) & ~2u);
    }
    if (!s.center || !s.offset)
      return false;
    auto center = s.center(s.user);
    if (!center)
      return false;
    auto offset = s.offset(s.user, angle);
    if (!offset)
      return false;
    (*center)[0] += (*offset)[0];
    (*center)[1] += (*offset)[1];
    (*center)[2] = (*offset)[1];
    slot.position = *center;
    auto translate = [&](std::array<float, 3> pos) -> bool {
      if (!s.translation)
        return false;
      slot.matrix = s.translation(s.user, pos);
      return slot.matrix.has_value();
    };
    if (state == 1) {
      const bool selected = i == word(c);
      const auto gp = word(0x80415fd0), counter = word(a + 0x48);
      const auto count = signedWord(word(gp + 4) * 10) / 60;
      float rotation =
          constant(-20484) +
          (constant(-20480) * float(signedWord(counter))) / float(count);
      if (rotation >= constant(-20476))
        rotation = zero;
      const float rotationRadians = constant(-20472) * rotation;
      if (!s.rotation || !s.concat)
        return false;
      const auto spin = s.rotation(s.user, rotationRadians, 120);
      if (!spin)
        return false;
      auto pos = *center;
      if (selected)
        pos[0] = constant(-20468) + pos[0];
      if (!translate(pos))
        return false;
      slot.matrix = s.concat(s.user, *slot.matrix, *spin);
      if (!slot.matrix)
        return false;
      if (selected)
        put(a + 0x20, word(a + 0x20) | 2);
      slot.parameter = zero;
      if (angle >= entryEndpoint) {
        const auto gp = word(0x80415fd0), now = word(a + 0x48);
        const auto n = signedWord(word(gp + 4) * 10) / 60;
        if (signedWord(now) >= n) {
          put(a + 0x34, 3);
          put(a + 0x48, uint32_t(-1));
        }
      }
      put(a + 0x48, word(a + 0x48) + 1);
    } else if (state == 3) {
      auto pos = *center;
      if (i == word(c)) {
        pos[0] = constant(-20468) + pos[0];
        if (!translate(pos))
          return false;
        put(a + 0x20, word(a + 0x20) | 2);
      } else if (!translate(pos))
        return false;
      slot.parameter = zero;
    } else if (state == 4 || state == 5) {
      if (!translate(*center))
        return false;
      slot.parameter = zero;
      put(a + 0x20, word(a + 0x20) & ~2u);
      put(a + 0x48, word(a + 0x48) + 1);
    } else if (state == 10) {
      if (!s.interpolate)
        return false;
      auto x = s.interpolate(s.user, 4, signedWord(word(a + 0x48)),
                             duration(12), constant(-20464), (*center)[0]);
      if (!x)
        return false;
      (*center)[0] = *x;
      auto y = s.interpolate(s.user, 4, signedWord(word(a + 0x48)),
                             duration(12), constant(-20460), (*center)[1]);
      if (!y)
        return false;
      (*center)[1] = *y;
      slot.position = *center;
      auto pos = *center;
      pos[0] = constant(-20468) + pos[0];
      if (!translate(pos))
        return false;
      auto parameter = s.interpolate(s.user, 0, signedWord(word(a + 0x48)),
                                     duration(12), constant(-20476), zero);
      if (!parameter)
        return false;
      slot.parameter = *parameter;
      put(a + 0x48, word(a + 0x48) + 1);
      if (signedWord(word(a + 0x48)) > duration(12))
        put(a + 0x34, 3);
    } else if (state == 11) {
      const auto n = word(a + 0x48);
      const float scale = constant(-20456) * float(signedWord(n));
      if (scale >= constant(-20452))
        put(a + 0x34, 3);
      if (!s.scale || !s.concat)
        return false;
      const auto grow = s.scale(s.user, scale, scale, constant(-20452));
      if (!grow)
        return false;
      if (!translate(*center))
        return false;
      slot.matrix = s.concat(s.user, *slot.matrix, *grow);
      if (!slot.matrix)
        return false;
      slot.parameter = zero;
      put(a + 0x48, word(a + 0x48) + 1);
    }
  }
  if (!s.icons || !s.icons(s.user, w, word(w + 4), slots))
    return false;
  if (!s.partyButton || !s.partyButton(s.user))
    return false;
  return s.pinchMark && s.pinchMark(s.user);
}
bool drawPackedBattleActIcons(uint32_t w, uint32_t rows,
                              const std::vector<BattleActDrawSlot> &slots,
                              const BattleActIconDrawServices &s) {
  const auto &m = s.memory.memory;
  auto word = [&](uint32_t a) { return m.word(m.user, a); };
  const auto b = currentBattleAddress(s.memory);
  const auto u = battleRegistryUnit(b, signedWord(word(b + 0x420)), s.memory);
  const auto c = b + (word(u + 8) == 222 ? 0x1bb4 : 0x1bc0);
  const auto count = word(c + 8);
  std::vector<float> depths;
  std::vector<uint32_t> order;
  for (uint32_t i = 0; signedWord(i) < signedWord(count); ++i) {
    if (i >= slots.size() || !slots[i].position)
      return false;
    depths.push_back((*slots[i].position)[1]);
    order.push_back(i);
  }
  for (uint32_t i = 0;; ++i) {
    const auto liveCount = word(c + 8);
    if (signedWord(i) >= signedWord(liveCount - 1))
      break;
    for (uint32_t j = i + 1; signedWord(j) < signedWord(liveCount); ++j) {
      if (i >= depths.size() || j >= depths.size())
        return false;
      if (depths[i] < depths[j]) {
        std::swap(depths[i], depths[j]);
        std::swap(order[i], order[j]);
      }
    }
  }
  for (uint32_t i = 0; signedWord(i) < signedWord(word(c + 8)); ++i) {
    if (i >= order.size())
      return false;
    const auto row = order[i], flags = word(w + 0x20 + row * 4);
    if (!(flags & 1))
      continue;
    const auto &slot = slots[row];
    if (!slot.matrix || !slot.parameter)
      return false;
    if (flags & 2) {
      if (!s.bubble ||
          !s.bubble(s.user, w, rows + 20 * row, *slot.matrix, *slot.parameter))
        return false;
    } else {
      if (!slot.position || !s.circle ||
          !s.circle(s.user, w, rows + 20 * row, *slot.matrix, *slot.position,
                    *slot.parameter))
        return false;
    }
  }
  return true;
}

} // namespace ttyd
