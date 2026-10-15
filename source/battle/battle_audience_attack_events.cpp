#include "battle_audience_attack_events.hpp"
#include "battle_event_callbacks.hpp"
#include "positional_sfx_registry.hpp"
#include <cstring>
#include <unordered_map>
namespace ttyd {
namespace {
float real(uint32_t n) {
  float f;
  std::memcpy(&f, &n, 4);
  return f;
}
int32_t integer(uint32_t n) {
  int32_t v;
  std::memcpy(&v, &n, 4);
  return v;
}
bool same(const BattleAudienceAttackServices &a,
          const BattleAudienceAttackServices &b) {
  const auto &x = a.actors.memory;
  const auto &y = b.actors.memory;
  const auto &v = a.values;
  const auto &w = b.values;
  return a.user == b.user && a.truncate == b.truncate && a.hit == b.hit &&
         a.sound == b.sound && a.pitch == b.pitch &&
         a.actors.user == b.actors.user && a.actors.byte == b.actors.byte &&
         a.actors.audienceLifecycle.user == b.actors.audienceLifecycle.user &&
         a.actors.audienceLifecycle.event == b.actors.audienceLifecycle.event &&
         x.user == y.user && x.word == y.word && x.half == y.half &&
         x.storeWord == y.storeWord && x.storeHalf == y.storeHalf &&
         x.storeByte == y.storeByte && x.flushBlock == y.flushBlock &&
         x.systemCall == y.systemCall && v.user == w.user && v.word == w.word &&
         v.resolve == w.resolve && v.store == w.store &&
         v.resolveFloat == w.resolveFloat && v.storeFloat == w.storeFloat &&
         v.clock == w.clock && v.call == w.call && v.handler == w.handler &&
         v.remove == w.remove;
}
struct Guard {
  bool &flag;
  explicit Guard(bool &f) : flag(f) { flag = true; }
  ~Guard() { flag = false; }
};
} // namespace
struct BattleAudienceAttackEventCursor::State {
  BattleAudienceAttackServices owner;
  EventEntry *entry = nullptr;
  uint64_t lifetime = 0;
  uint32_t id = 0, command = 0, args = 0, function = 0, phase = 0, pending = 0;
  std::array<int32_t, 4> destinations{};
  std::array<float, 3> position{};
  int32_t slot = 0, mode = 0, handle = 0;
  unsigned output = 0;
  float scale = 0;
};
BattleAudienceAttackEventCursor::BattleAudienceAttackEventCursor() = default;
BattleAudienceAttackEventCursor::~BattleAudienceAttackEventCursor() = default;
bool BattleAudienceAttackEventCursor::active() const { return bool(state_); }
bool BattleAudienceAttackEventCursor::reset() {
  if (invoking_)
    return false;
  state_.reset();
  return true;
}
uint32_t BattleAudienceAttackEventCursor::pending() const {
  return state_ ? state_->pending : 0;
}
std::optional<int32_t>
resumeBattleAudienceAttackEvent(EventEntry &e, uint32_t fn,
                                const BattleAudienceAttackServices &s,
                                BattleAudienceAttackEventCursor &cursor) {
  if (cursor.invoking_ || (fn != 0x801465e0 && fn != 0x801466c4))
    return {};
  if (cursor.state_) {
    const auto &c = *cursor.state_;
    if (c.entry != &e || c.lifetime != e.lifetime || c.id != e.id ||
        c.command != e.currentCommand || c.args != e.arguments ||
        c.function != fn || !same(c.owner, s))
      return {};
  }
  const auto &m = s.actors.memory;
  if (!m.word || !m.storeWord)
    return {};
  Guard guard(cursor.invoking_);
  if (!cursor.state_) {
    cursor.state_ = std::make_unique<BattleAudienceAttackEventCursor::State>();
    auto &c = *cursor.state_;
    c.owner = s;
    c.entry = &e;
    c.lifetime = e.lifetime;
    c.id = e.id;
    c.command = e.currentCommand;
    c.args = e.arguments;
    c.function = fn;
  }
  auto &c = *cursor.state_;
  auto word = [&](uint32_t a) { return m.word(m.user, a); };
  auto member = [&](int32_t slot) {
    return currentBattleAddress(s.actors) + 0x2858 + 0x1bc +
           uint32_t(slot) * 0x134;
  };
  auto done = [&]() -> std::optional<int32_t> {
    cursor.state_.reset();
    return 2;
  };
  for (;;)
    switch (c.phase) {
    case 0:
      if (fn == 0x801466c4) {
        for (unsigned i = 0; i < 4; ++i)
          c.destinations[i] = integer(word(c.args + 4 * i));
        c.phase = 2;
      } else {
        if (!s.values.resolve)
          return {};
        c.slot = s.values.resolve(s.values.user, e, integer(word(c.args)));
        c.phase = 1;
      }
      break;
    case 1:
      if (!s.values.resolve)
        return {};
      c.mode = s.values.resolve(s.values.user, e, integer(word(c.args + 4)));
      c.phase = 2;
      break;
    case 2: {
      // GetItemOn2 captures A, resolves its cached member using a fresh global,
      // then independently rereads cache slot and XYZ. No first-eligible scan.
      const auto a = currentBattleAddress(s.actors) + 0x2858;
      const auto cached = integer(word(a + 0x138bc));
      (void)member(cached);
      c.slot = integer(word(a + 0x138bc));
      for (unsigned i = 0; i < 3; ++i)
        c.position[i] = real(word(a + 0x138c0 + 4 * i));
      c.phase = fn == 0x801466c4 ? 10 : 3;
      break;
    }
    case 3: {
      volatile float y = c.position[1] + real(word(0x80422fac));
      c.position[1] = y;
      if (c.mode == 1)
        c.phase = 5;
      else {
        c.scale = real(word(0x80422fb0));
        c.phase = 4;
      }
      break;
    }
    case 4:
      c.pending = 0x8013f1b4;
      if (!s.hit || !s.hit(s.user, 0, 3, c.position, c.scale))
        return {};
      c.phase = 5;
      break;
    case 5: {
      const auto r = member(c.slot);
      for (unsigned i = 0; i < 3; ++i)
        c.position[i] = real(word(r + 0x48 + 4 * i));
      c.phase = c.mode == 1 ? 8 : 6;
      break;
    }
    case 6: {
      c.pending = 0x800d9848;
      if (!s.sound)
        return {};
      const auto h = s.sound(s.user, 0x802f429c, c.position);
      if (!h)
        return {};
      c.handle = *h;
      c.phase = 7;
      break;
    }
    case 7:
      c.pending = 0x800d9744;
      if (!s.pitch || !s.pitch(s.user, c.handle, 1023))
        return {};
      c.phase = 8;
      break;
    case 8: {
      const auto r = member(c.slot);
      m.storeWord(m.user, r, word(r) | 0x40);
      return done();
    }
    case 10:
      while (c.output < 4) {
        const auto destination = c.destinations[c.output];
        if (destination == -250000000) {
          ++c.output;
          continue;
        }
        if (!s.values.store)
          return {};
        int32_t value = c.slot;
        if (c.output) {
          c.pending = 0x801466c4;
          if (!s.truncate)
            return {};
          const auto n = s.truncate(s.user, c.position[c.output - 1]);
          if (!n)
            return {};
          value = *n;
        }
        s.values.store(s.values.user, e, destination, value);
        ++c.output;
      }
      return done();
    default:
      return {};
    }
}
std::optional<bool> setQueuedBattleAudienceAttackPitch(PositionalSfxRegistry &s,
                                                       int32_t handle,
                                                       int32_t pitch) {
  if (handle == -1)
    return true;
  const auto index = uint32_t(handle) & 255;
  if (index >= s.slots.size())
    return {};
  auto &slot = s.slots[index];
  if (slot.key == -1)
    return true;
  if (slot.flags & 1) {
    slot.pitch = uint16_t(pitch);
    return true;
  }
  return {};
}
struct BattleAudienceAttackEvents::State {
  EventNativeRuntime &vm;
  struct Call {
    EventNativeRuntime *vm = nullptr;
    void *user = nullptr;
    decltype(BattleAudienceAttackEvents::hit) hit = nullptr;
    decltype(BattleAudienceAttackEvents::sound) sound = nullptr;
    decltype(BattleAudienceAttackEvents::pitch) pitch = nullptr;
    uint64_t lifetime = 0;
    uint32_t id = 0, command = 0, function = 0;
    BattleAudienceAttackServices services;
    BattleAudienceAttackEventCursor cursor;
  };
  std::unordered_map<EventEntry *, std::unique_ptr<Call>> calls;
  bool invoking = false;
  explicit State(EventNativeRuntime &r) : vm(r) {}
};
BattleAudienceAttackEvents::BattleAudienceAttackEvents(EventNativeRuntime &r)
    : state_(std::make_unique<State>(r)) {}
BattleAudienceAttackEvents::~BattleAudienceAttackEvents() = default;
int32_t BattleAudienceAttackEvents::invoke(EventEntry &e, uint32_t fn) {
  if (state_->invoking)
    return 0;
  Guard guard(state_->invoking);
  auto &slot = state_->calls[&e];
  if (!slot || slot->lifetime != e.lifetime || slot->id != e.id ||
      slot->command != e.currentCommand || slot->function != fn) {
    slot = std::make_unique<State::Call>();
    slot->lifetime = e.lifetime;
    slot->id = e.id;
    slot->command = e.currentCommand;
    slot->function = fn;
    slot->services.actors = nativeBattleActorServices(state_->vm);
    slot->services.values = nativeBattleUnitEventServices(state_->vm).values;
    slot->vm = &state_->vm;
    slot->user = user;
    slot->hit = hit;
    slot->sound = sound;
    slot->pitch = pitch;
    slot->services.user = slot.get();
    slot->services.truncate = [](void *p, float f) -> std::optional<int32_t> {
      auto &v = static_cast<State::Call *>(p)->vm->platform;
      return v.truncate ? std::optional<int32_t>(v.truncate(v.user, f))
                        : std::nullopt;
    };
    slot->services.hit = [](void *p, int32_t a, int32_t b,
                            const std::array<float, 3> &v, float f) {
      auto &c = *static_cast<State::Call *>(p);
      return c.hit && c.hit(c.user, a, b, v, f);
    };
    slot->services.sound =
        [](void *p, uint32_t k,
           const std::array<float, 3> &v) -> std::optional<int32_t> {
      auto &c = *static_cast<State::Call *>(p);
      return c.sound ? c.sound(c.user, k, v) : std::nullopt;
    };
    slot->services.pitch = [](void *p, int32_t h, int32_t v) {
      auto &c = *static_cast<State::Call *>(p);
      return c.pitch && c.pitch(c.user, h, v);
    };
  }
  const auto result =
      resumeBattleAudienceAttackEvent(e, fn, slot->services, slot->cursor);
  if (!result)
    return 0;
  state_->calls.erase(&e);
  return *result;
}
bool BattleAudienceAttackEvents::bind(EventCallbackRegistry &r) {
  if (r.contains(0x801465e0) || r.contains(0x801466c4))
    return false;
  return r.bind(0x801465e0, this, [](void *p, EventEntry &e, int32_t) {
    return static_cast<BattleAudienceAttackEvents *>(p)->invoke(e, 0x801465e0);
  }) && r.bind(0x801466c4, this, [](void *p, EventEntry &e, int32_t) {
    return static_cast<BattleAudienceAttackEvents *>(p)->invoke(e, 0x801466c4);
  });
}
} // namespace ttyd
