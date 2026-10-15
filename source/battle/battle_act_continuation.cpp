#include "battle_act_continuation.hpp"
#include "battle_sequence_startup.hpp"
#include <cstring>
namespace ttyd {
namespace {
int32_t signedWord(uint32_t n) {
  return n < 0x80000000u ? int32_t(n) : int32_t(int64_t(n) - 0x100000000LL);
}
int signedByte(uint8_t n) { return n < 128 ? int(n) : int(n) - 256; }
float scalar(uint32_t n) {
  float f;
  std::memcpy(&f, &n, 4);
  return f;
}
bool same(const BattleActServices &a, const BattleActServices &b) {
  const auto &x = a.actors.memory;
  const auto &y = b.actors.memory;
  return a.user == b.user && a.call == b.call &&
         a.actors.user == b.actors.user && a.actors.byte == b.actors.byte &&
         a.actors.audienceLifecycle.user == b.actors.audienceLifecycle.user &&
         a.actors.audienceLifecycle.event == b.actors.audienceLifecycle.event &&
         x.user == y.user && x.word == y.word && x.half == y.half &&
         x.storeWord == y.storeWord && x.storeHalf == y.storeHalf &&
         x.storeByte == y.storeByte && x.flushBlock == y.flushBlock &&
         x.systemCall == y.systemCall;
}
struct Running {
  bool &v;
  explicit Running(bool &f) : v(f) { v = true; }
  ~Running() { v = false; }
};
} // namespace
struct BattleActCursor::State {
  BattleActServices services;
  uint32_t battle = 0, pc = 0x8011b2f4, unit = 0, event = 0, aggregate = 0,
           pending = 0, operand = 0;
  std::optional<BattleActSequenceRequest> held;
  uint32_t word(uint32_t a) const {
    return services.actors.memory.word(services.actors.memory.user, a);
  }
  uint8_t byte(uint32_t a) const {
    return services.actors.byte(services.actors.user, a);
  }
  void put(uint32_t a, uint32_t v) const {
    services.actors.memory.storeWord(services.actors.memory.user, a, v);
  }
  void putByte(uint32_t a, uint8_t v) const {
    services.actors.memory.storeByte(services.actors.memory.user, a, v);
  }
  void set(uint32_t index) {
    packedBattleSetSequence(battle, 6, 0x06000000 + index,
                            services.actors.memory);
  }
  void inc(uint32_t channel = 6) {
    packedBattleIncrementSequence(battle, channel, services.actors.memory);
  }
  template <class Build>
  std::optional<BattleActOutput> call(BattleActChild child, uint32_t fn,
                                      Build build) {
    pending = fn;
    if (!held) {
      held = BattleActSequenceRequest{};
      held->child = child;
      held->function = fn;
      held->callsite = pc;
      build(*held);
    }
    if (!services.call)
      return {};
    auto out = services.call(services.user, *held);
    if (out) {
      held.reset();
      pending = 0;
    }
    return out;
  }
  std::optional<BattleActOutput> call(BattleActChild child, uint32_t fn,
                                      std::array<uint32_t, 4> args = {}) {
    return call(child, fn, [&](BattleActSequenceRequest &r) { r.arguments = args; });
  }
  std::optional<BattleActOutput> actor(uint32_t site) {
    pc = site;
    return call(BattleActChild::Unit, 0x800f88cc, [&](BattleActSequenceRequest &r) {
      r.arguments = {battle, word(battle + 0x420)};
    });
  }
};
BattleActCursor::BattleActCursor() = default;
BattleActCursor::~BattleActCursor() = default;
bool BattleActCursor::active() const { return bool(state_); }
uint32_t BattleActCursor::pending() const {
  return state_ ? state_->pending : 0;
}
bool BattleActCursor::reset() {
  if (invoking_)
    return false;
  state_.reset();
  return true;
}
bool resumePackedBattleAct(uint32_t battle, const BattleActServices &s,
                           BattleActCursor &cursor) {
  const auto &m = s.actors.memory;
  if (cursor.invoking_ || !m.word || !m.storeWord || !m.storeByte ||
      !s.actors.byte)
    return false;
  if (cursor.state_ &&
      (cursor.state_->battle != battle || !same(cursor.state_->services, s)))
    return false;
  Running run(cursor.invoking_);
  if (!cursor.state_) {
    cursor.state_ = std::make_unique<BattleActCursor::State>();
    cursor.state_->services = s;
    cursor.state_->battle = battle;
  }
  auto &c = *cursor.state_;
  auto finish = [&]() {
    cursor.state_.reset();
    return true;
  };
  using Child = BattleActChild;
  for (;;)
    switch (c.pc) {
    case 0x8011b2f4: {
      const auto index = packedBattleGetSequence(battle, 6, m) - 0x06000000u;
      static constexpr uint32_t targets[]{
          0x8011b324, 0x8011b4d8, 0x8011b500, 0x8011b574, 0x8011b5ac,
          0x8011b538, 0x8011b5e8, 0x8011b7e0, 0x8011b804, 0x8011b838,
          0x8011ba3c, 0x8011ba74, 0x8011ba9c, 0x8011bb20, 0x8011bb5c};
      if (index > 14)
        return finish();
      c.pc = targets[index];
      break;
    }
    case 0x8011b324: {
      auto out = c.actor(c.pc);
      if (!out)
        return false;
      c.unit = out->value;
      c.pc = 0x8011b334;
      break;
    }
    case 0x8011b334:
      if (!c.call(Child::NiceAsync, 0x80144d44, {1}))
        return false;
      c.put(c.unit + 0x284, 0);
      c.put(c.unit + 0x2d0, 0);
      c.pc = 0x8011b344;
      break;
    case 0x8011b344: {
      if (!c.call(Child::AudienceInit, 0x801a82d4))
        return false;
      const auto kind = signedWord(c.word(c.unit + 8));
      c.pc = kind == 222                 ? 0x8011b364
             : kind >= 224 && kind < 231 ? 0x8011b3cc
                                         : 0x8011b420;
      break;
    }
    case 0x8011b364:
    case 0x8011b378:
    case 0x8011b38c:
    case 0x8011b3a0:
    case 0x8011b3cc:
    case 0x8011b3e0:
    case 0x8011b3f4:
    case 0x8011b408: {
      const auto start = c.pc < 0x8011b3cc ? 0x8011b364u : 0x8011b3ccu;
      const auto i = (c.pc - start) / 20;
      static constexpr uint32_t ids[]{1, 2, 9, 27};
      auto out = c.call(Child::Status, 0x80127814, {c.unit, ids[i]});
      if (!out)
        return false;
      c.pc = out->value ? 0x8011b420 : i == 3 ? 0x8011b438 : c.pc + 20;
      break;
    }
    case 0x8011b420:
      c.set(3);
      return finish();
    case 0x8011b438:
      if (c.word(c.unit + 0x104) & 0x10) {
        c.set(4);
        return finish();
      }
      if (c.word(c.unit + 8) == 228)
        c.pc = 0x8011b46c;
      else
        c.pc = 0x8011b494;
      break;
    case 0x8011b46c: {
      auto out = c.call(Child::Mario, 0x800f87a8, {battle});
      if (!out)
        return false;
      if (c.word(out->value + 0x104) & 0x10) {
        c.set(4);
        return finish();
      }
      c.pc = 0x8011b494;
      break;
    }
    case 0x8011b494:
      if (signedByte(c.byte(c.unit + 0x12b))) {
        c.set(5);
        return finish();
      }
      c.pc = 0x8011b4bc;
      break;
    case 0x8011b4bc:
      if (!c.call(Child::CommandInit, 0x80125380, {battle}))
        return false;
      c.set(1);
      return finish();
    case 0x8011b4d8: {
      auto out = c.call(Child::CameraSpeed, 0x8012c0e8);
      if (!out)
        return false;
      if (out->real > scalar(c.word(0x80422584)))
        return finish();
      c.set(2);
      c.pc = 0x8011b500;
      break;
    }
    case 0x8011b500: {
      auto out = c.call(Child::CommandInput, 0x801229d8, {battle});
      if (!out)
        return false;
      if (!out->value)
        return finish();
      c.put(battle + 0xef4, c.word(battle + 0xef4) & ~0x80u);
      c.set(6);
      return finish();
    }
    case 0x8011b538:
    case 0x8011b5ac: {
      auto out = c.actor(c.pc);
      if (!out)
        return false;
      c.unit = out->value;
      c.pc = c.pc == 0x8011b538 ? 0x8011b544 : 0x8011b5b8;
      break;
    }
    case 0x8011b544:
    case 0x8011b5b8: {
      const bool two = c.pc == 0x8011b544;
      auto out = c.call(Child::UnitData, 0x8012733c, {c.unit, two ? 2u : 3u});
      if (!out)
        return false;
      c.put(c.unit + 0x2a0, out->value);
      c.put(c.unit + 0x2ac, two ? 1u : 27u);
      c.set(6);
      return finish();
    }
    case 0x8011b574: {
      auto out = c.actor(c.pc);
      if (!out)
        return false;
      c.unit = out->value;
      c.put(c.unit + 0x2ac, 1);
      c.put(c.unit + 0x1c, c.word(c.unit + 0x1c) | 0x04000000);
      c.set(6);
      return finish();
    }
    case 0x8011b5e8: {
      auto out = c.actor(c.pc);
      if (!out)
        return false;
      c.pc = (c.word(out->value + 0x2ac) & 1) ? 0x8011b604 : 0x8011b5f8;
      break;
    }
    case 0x8011b5f8:
      if (!c.call(Child::WindowOff, 0x800f75cc))
        return false;
      c.pc = 0x8011b604;
      break;
    case 0x8011b604: {
      auto out = c.actor(c.pc);
      if (!out)
        return false;
      c.unit = out->value;
      c.event = 0;
      if (c.word(battle + 0xef4) & 0x40000000)
        if (!(c.word(c.unit + 0x104) & 0x2000)) {
          const auto flags = c.word(c.unit + 0x2ac);
          if (!(flags & 8) && !(flags & 16)) {
            c.pc = 0x8011b64c;
            break;
          }
        }
      c.pc = 0x8011b658;
      break;
    }
    case 0x8011b658: {
      const auto flags = c.word(c.unit + 0x2ac);
      if (flags & 16) {
        c.operand = c.word(c.unit + 0x2a0);
        if (!c.operand) {
          c.pc = 0x8011b79c;
          break;
        }
        c.pc = 0x8011b678;
      } else
        c.pc = (flags & 2) ? 0x8011b698 : 0x8011b6ac;
      break;
    }
    case 0x8011b6ac:
    case 0x8011b6c0:
    case 0x8011b6d4:
    case 0x8011b6e8: {
      const auto i = (c.pc - 0x8011b6ac) / 20;
      static constexpr uint32_t ids[]{1, 2, 9, 27};
      auto out = c.call(Child::Status, 0x80127814, {c.unit, ids[i]});
      if (!out)
        return false;
      if (out->value && (i != 3 || !(c.word(c.unit + 0x104) & 0x10000)))
        c.pc = 0x8011b79c;
      else
        c.pc = i == 3 ? 0x8011b708 : c.pc + 20;
      break;
    }
    case 0x8011b708: {
      auto out = c.call(Child::Status, 0x80127814, {c.unit, 6});
      if (!out)
        return false;
      c.pc = out->value ? 0x8011b718 : 0x8011b748;
      break;
    }
    case 0x8011b718: {
      auto out = c.call(Child::Random, 0x80005da0, {100});
      if (!out)
        return false;
      c.pc = signedWord(out->value) < 0 ? 0x8011b734 : 0x8011b748;
      break;
    }
    case 0x8011b748: {
      auto out = c.call(Child::Status, 0x80127814, {c.unit, 5});
      if (!out)
        return false;
      c.pc = out->value ? 0x8011b758 : 0x8011b794;
      break;
    }
    case 0x8011b758: {
      auto out = c.call(Child::Random, 0x80005da0, {100});
      if (!out)
        return false;
      if (signedWord(out->value) < 50) {
        c.put(c.unit + 0x27c, c.word(c.unit + 0x27c) | 0x10);
        c.pc = 0x8011b77c;
      } else
        c.pc = 0x8011b794;
      break;
    }
    case 0x8011b64c:
    case 0x8011b678:
    case 0x8011b698:
    case 0x8011b734:
    case 0x8011b77c:
    case 0x8011b794: {
      auto out =
          c.call(Child::CreateEvent, 0x800374ac, [&](BattleActSequenceRequest &r) {
            r.arguments = {
                c.pc == 0x8011b64c   ? 0x8034f234u
                : c.pc == 0x8011b734 ? 0x8034e0ccu
                : c.pc == 0x8011b678
                    ? c.operand
                    : c.word(c.unit + (c.pc == 0x8011b77c ? 0x2a4 : 0x2a0)),
                10, 0};
          });
      if (!out)
        return false;
      c.event = out->value;
      c.pc = 0x8011b79c;
      break;
    }
    case 0x8011b79c:
      if (!c.event) {
        c.inc();
        return finish();
      }
      c.put(c.unit + 0x2a8, c.word(c.event + 0x15c));
      c.put(c.event + 0x160, c.word(c.unit));
      c.pc = 0x8011b7b4;
      break;
    case 0x8011b7b4:
      c.operand = c.word(c.unit + 0x290);
      if (!c.operand) {
        c.inc();
        return finish();
      }
      c.pc = 0x8011b7c0;
      break;
    case 0x8011b7c0:
      if (!c.call(Child::DeleteEvent, 0x80036664,
                  [&](BattleActSequenceRequest &r) { r.arguments[0] = c.operand; }))
        return false;
      c.put(c.unit + 0x290, 0);
      c.inc();
      return finish();
    case 0x8011b7e0: {
      auto out = c.call(Child::WaitEvents, 0x8011ace8, {battle});
      if (!out)
        return false;
      if (!out->value)
        return finish();
      c.inc();
      c.pc = 0x8011b7fc;
      break;
    }
    case 0x8011b7fc:
      if (!c.call(Child::AdjustMessages, 0x801958bc, {battle}))
        return false;
      c.pc = 0x8011b804;
      break;
    case 0x8011b804: {
      auto out = c.call(Child::WaitEvents, 0x8011ace8, {battle});
      if (!out)
        return false;
      if (!out->value)
        return finish();
      c.pc = 0x8011b814;
      break;
    }
    case 0x8011b814:
    case 0x8011ba74: {
      auto out = c.call(Child::Messages, 0x801956f8, {battle});
      if (!out)
        return false;
      c.aggregate = out->value;
      c.pc = c.pc == 0x8011b814 ? 0x8011b820 : 0x8011ba80;
      break;
    }
    case 0x8011b820:
    case 0x8011ba80: {
      auto out = c.call(Child::Announcements, 0x80195af8, {battle});
      if (!out)
        return false;
      if (out->value | c.aggregate)
        return finish();
      c.inc();
      c.pc = c.pc == 0x8011b820 ? 0x8011b838 : 0x8011ba9c;
      break;
    }
    case 0x8011b838:
      c.pc = c.word(battle + 0xef4) & 0x30 ? 0x8011b850 : 0x8011b844;
      break;
    case 0x8011b844:
      if (!c.call(Child::WindowOn, 0x800f7624))
        return false;
      c.pc = 0x8011b850;
      break;
    case 0x8011b850: {
      auto out = c.actor(c.pc);
      if (!out)
        return false;
      c.unit = out->value;
      if (!c.unit) {
        c.pc = 0x8011ba1c;
        break;
      }
      c.put(c.unit + 0x1c, c.word(c.unit + 0x1c) & ~0x04000000u);
      c.pc = c.word(c.unit + 0x27c) & 8 ? 0x8011b880 : 0x8011b890;
      break;
    }
    case 0x8011b880:
      if (!c.call(Child::SetStatus, 0x80127bac, {c.unit, 16, 1, uint32_t(-99)}))
        return false;
      c.put(c.unit + 0x27c, c.word(c.unit + 0x27c) & ~8u);
      c.pc = 0x8011b890;
      break;
    case 0x8011b890: {
      if (c.byte(battle + 0x18ff9) == 1)
        c.putByte(battle + 0x18ff9, 0);
      c.put(c.unit + 0x25c, 0);
      const auto state = signedByte(c.byte(c.unit + 0x20));
      if (state == 5)
        c.putByte(c.unit + 0x20, 6);
      else if (state == 6)
        c.putByte(c.unit + 0x20, 7);
      else if (state == 7 || state == 9)
        c.putByte(c.unit + 0x20, 0);
      else if (state == 8)
        c.putByte(c.unit + 0x20, 9);
      const auto now = signedByte(c.byte(c.unit + 0x20));
      if (!now) {
        (void)c.word(battle + 0xef4);
        c.pc = 0x8011b940;
      } else {
        if (now == 3 || now == 4)
          c.putByte(c.unit + 0x20, 0);
        c.pc = 0x8011ba1c;
      }
      break;
    }
    case 0x8011b940: {
      auto out =
          c.call(Child::CheckStatusFlag, 0x801277f8, {c.unit, 0x02000000});
      if (!out)
        return false;
      if (out->value)
        c.pc = 0x8011b954;
      else {
        c.putByte(c.unit + 0x22, uint8_t(c.byte(c.unit + 0x22) - 1));
        c.pc = 0x8011b968;
      }
      break;
    }
    case 0x8011b954:
      if (!c.call(Child::ClearStatusFlag, 0x801277d8, {c.unit, 0x02000000}))
        return false;
      c.pc = 0x8011b968;
      break;
    case 0x8011b968: {
      if (c.word(battle + 0xef4) & 0x2000)
        c.putByte(c.unit + 0x22, 0);
      if (signedByte(c.byte(c.unit + 0x22)) > 0) {
        c.pc = 0x8011ba1c;
        break;
      }
      c.putByte(c.unit + 0x20, 10);
      const auto flags = c.word(c.unit + 0x27c);
      if (flags & 0x2000)
        c.put(c.unit + 0x27c, flags & ~0x2000u);
      else
        c.putByte(c.unit + 0x24, 0);
      c.pc = 0x8011b9b4;
      break;
    }
    case 0x8011b9b4: {
      auto out = c.call(Child::Belong, 0x80128144, {c.unit});
      if (!out)
        return false;
      c.pc = (signedByte(uint8_t(out->value)) == 0 &&
              c.word(c.unit + 0x2c) == 0x04000002)
                 ? 0x8011b9d0
                 : 0x8011ba1c;
      break;
    }
    case 0x8011b9d0: {
      auto out = c.call(Child::PhaseEnd, 0x8011bb88);
      if (!out)
        return false;
      if (!out->value && !(c.word(battle + 0xef4) & 0x2000))
        c.putByte(c.unit + 0x311, 96);
      c.pc = 0x8011ba1c;
      break;
    }
    case 0x8011ba1c:
      if (!c.call(Child::ConsumeItems, 0x800f74b4))
        return false;
      c.put(battle + 0xef4, c.word(battle + 0xef4) & ~4u);
      c.pc = 0x8011ba2c;
      break;
    case 0x8011ba2c:
      if (!c.call(Child::AudiencePerAct, 0x801a7e94))
        return false;
      c.inc();
      c.pc = 0x8011ba3c;
      break;
    case 0x8011ba3c: {
      auto out = c.call(Child::AudienceReaction, 0x801a7e30);
      if (!out)
        return false;
      if (uint8_t(out->value))
        return finish();
      c.pc = 0x8011ba4c;
      break;
    }
    case 0x8011ba4c: {
      auto out = c.call(Child::Concluded, 0x8011aa34, {battle});
      if (!out)
        return false;
      if (out->value) {
        c.inc();
        c.pc = 0x8011ba74;
      } else
        c.pc = 0x8011ba58;
      break;
    }
    case 0x8011ba58: {
      auto out = c.call(Child::BreakSlotReaction, 0x80203394);
      if (!out)
        return false;
      if (uint8_t(out->value))
        return finish();
      c.inc();
      c.pc = 0x8011ba74;
      break;
    }
    case 0x8011ba9c: {
      auto out = c.call(Child::WaitEvents, 0x8011ace8, {battle});
      if (!out)
        return false;
      if (!out->value)
        return finish();
      c.pc = 0x8011bab0;
      break;
    }
    case 0x8011bab0: {
      auto out = c.actor(c.pc);
      if (!out)
        return false;
      c.unit = out->value;
      if (!c.unit) {
        c.inc();
        c.pc = 0x8011bb20;
        break;
      }
      c.put(c.unit + 0x2cc, 0);
      c.pc = 0x8011bac4;
      break;
    }
    case 0x8011bac4:
      c.operand = c.word(c.unit + 0x2c8);
      if (!c.operand) {
        c.inc();
        c.pc = 0x8011bb20;
      } else
        c.pc = 0x8011bad8;
      break;
    case 0x8011bad8: {
      auto out =
          c.call(Child::CreateEvent, 0x800374ac, [&](BattleActSequenceRequest &r) {
            r.arguments = {c.operand, 10, 0};
          });
      if (!out)
        return false;
      c.event = out->value;
      if (!c.event) {
        c.inc();
        c.pc = 0x8011bb20;
        break;
      }
      c.put(c.unit + 0x2cc, c.word(c.event + 0x15c));
      c.put(c.event + 0x160, c.word(c.unit));
      c.pc = 0x8011baf4;
      break;
    }
    case 0x8011baf4:
      c.operand = c.word(c.unit + 0x290);
      if (!c.operand) {
        c.inc();
        c.pc = 0x8011bb20;
      } else
        c.pc = 0x8011bb00;
      break;
    case 0x8011bb00:
      if (!c.call(Child::DeleteEvent, 0x80036664,
                  [&](BattleActSequenceRequest &r) { r.arguments[0] = c.operand; }))
        return false;
      c.put(c.unit + 0x290, 0);
      c.inc();
      c.pc = 0x8011bb20;
      break;
    case 0x8011bb20: {
      auto out = c.actor(c.pc);
      if (!out)
        return false;
      c.unit = out->value;
      c.pc = c.unit ? 0x8011bb2c : 0x8011bb4c;
      break;
    }
    case 0x8011bb2c:
      c.operand = c.word(c.unit + 0x2cc);
      c.pc = c.operand ? 0x8011bb38 : 0x8011bb4c;
      break;
    case 0x8011bb38: {
      auto out =
          c.call(Child::EventExists, 0x80036604,
                 [&](BattleActSequenceRequest &r) { r.arguments[0] = c.operand; });
      if (!out)
        return false;
      if (out->value)
        return finish();
      c.put(c.unit + 0x2cc, 0);
      c.pc = 0x8011bb4c;
      break;
    }
    case 0x8011bb4c:
      c.inc();
      c.pc = 0x8011bb5c;
      break;
    case 0x8011bb5c: {
      auto out = c.call(Child::WaitEvents, 0x8011ace8, {battle});
      if (!out)
        return false;
      if (out->value)
        c.inc(5);
      return finish();
    }
    default:
      return false;
    }
}
} // namespace ttyd
