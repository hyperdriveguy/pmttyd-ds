#include "source_effect_texture_setup.hpp"
namespace ttyd {
namespace {
constexpr uint32_t Setup = 0x8005ee0c, N64 = 0x801d7910, Callback = 0x8005efa8,
                   N64Callback = 0x801d7a58;
bool same(const SourceEffectTextureSetupServices &a,
          const SourceEffectTextureSetupServices &b) {
  const auto &x = a.objects.gx;
  const auto &y = b.objects.gx;
  return x.user == y.user && x.lifetime == y.lifetime &&
         !x.lifetime.owner_before(y.lifetime) &&
         !y.lifetime.owner_before(x.lifetime) && x.current == y.current &&
         x.word == y.word && x.byte == y.byte && x.store == y.store &&
         a.request == b.request;
}
struct Guard {
  bool &v;
  explicit Guard(bool &f) : v(f) { v = true; }
  ~Guard() { v = false; }
};
} // namespace
struct SourceEffectTextureSetupCursor::State {
  SourceEffectTextureSetupServices services;
  uint32_t function = 0, input = 0, pc = 0, pending = 0;
  uint32_t manager = 0, palette = 0, value = 0, gp = 0, prefix = 0, root = 0,
           handle = 0, rounded = 0;
  bool invalid = false, n64 = false, callback = false;
  SourceWindowTexturePath path;
  std::optional<SourceWindowTextureSetupCall> call;
  SourceWindowTextureSetupCursor unpack;
  uint32_t global() const { return n64 ? 0x80419a40 : 0x80415ee0; }
  uint32_t offset() const { return n64 ? 0 : 8; }
  uint32_t base() const {
    return callback ? (n64 ? N64Callback : Callback) : (n64 ? N64 : Setup);
  }
  bool valid() {
    if (invalid)
      return false;
    if (!services.objects.gx.current ||
        !services.objects.gx.current(services.objects.gx.user))
      invalid = true;
    return !invalid;
  }
  bool read(uint32_t at, uint32_t &out) {
    if (!valid() || !services.objects.gx.word)
      return false;
    auto v = services.objects.gx.word(services.objects.gx.user, at);
    if (!valid() || !v)
      return false;
    out = *v;
    return true;
  }
  bool write(uint32_t at, uint32_t v) {
    if (!valid() || !services.objects.gx.store)
      return false;
    const bool ok =
        services.objects.gx.store(services.objects.gx.user, at, 4, v);
    return valid() && ok;
  }
  bool invoke(uint32_t fn, uint32_t site, std::initializer_list<uint32_t> args,
              uint32_t &out, bool destination = false,
              bool sourcePath = false) {
    pending = fn;
    if (!valid() || !services.request)
      return false;
    if (!call) {
      SourceWindowTextureSetupCall q;
      q.function = fn;
      q.callsite = site;
      q.count = uint8_t(args.size());
      unsigned i = 0;
      for (auto a : args)
        q.arguments[i++] = a;
      q.destination = destination ? &path : nullptr;
      q.path = sourcePath ? &path : nullptr;
      call = q;
    }
    const auto v = services.request(services.objects.gx.user, *call);
    if (!valid() || !v)
      return false;
    out = *v;
    call.reset();
    return true;
  }
};
SourceEffectTextureSetupCursor::SourceEffectTextureSetupCursor() = default;
SourceEffectTextureSetupCursor::~SourceEffectTextureSetupCursor() = default;
bool SourceEffectTextureSetupCursor::active() const { return bool(state_); }
bool SourceEffectTextureSetupCursor::stale() const {
  return state_ && state_->invalid;
}
uint32_t SourceEffectTextureSetupCursor::pending() const {
  return !state_                   ? 0
         : state_->unpack.active() ? state_->unpack.pending()
                                   : state_->pending;
}
bool SourceEffectTextureSetupCursor::reset() {
  if (invoking_)
    return false;
  state_.reset();
  return true;
}
bool resumeSourceEffectTextureEntry(uint32_t fn, uint32_t input,
                                    const SourceEffectTextureSetupServices &s,
                                    SourceEffectTextureSetupCursor &cursor) {
  if (cursor.invoking_ || !s.objects.gx.lifetime || !s.objects.gx.current ||
      (fn != Setup && fn != Callback && fn != N64Callback))
    return false;
  if (cursor.state_ &&
      (cursor.state_->function != fn || cursor.state_->input != input ||
       !same(cursor.state_->services, s)))
    return false;
  Guard guard(cursor.invoking_);
  if (!cursor.state_) {
    cursor.state_ = std::make_unique<SourceEffectTextureSetupCursor::State>();
    auto &c = *cursor.state_;
    c.services = s;
    c.function = fn;
    c.input = input;
    c.callback = fn != Setup;
    c.n64 = fn == N64Callback;
    c.pc = c.callback ? 100 : 0;
  }
  auto &c = *cursor.state_;
#define R(P, O, A, D, N)                                                       \
  case P:                                                                      \
    c.pending = c.base() + O;                                                  \
    if (!c.read(A, c.D))                                                       \
      return false;                                                            \
    c.pc = N;                                                                  \
    continue
#define W(P, O, A, V, N)                                                       \
  case P:                                                                      \
    c.pending = c.base() + O;                                                  \
    if (!c.write(A, V))                                                        \
      return false;                                                            \
    c.pc = N;                                                                  \
    continue
  for (;;) {
    if (!c.valid())
      return false;
    switch (c.pc) {
      R(0, 0x24, c.global(), manager, 1);
      W(1, 0x28, c.manager + c.offset(), 0, 2);
      R(2, 0x30, c.global(), manager, 3);
      W(3, 0x34, c.manager + c.offset() + 4, 0, 4);
    case 4:
      if (!c.invoke(0x800782e4, c.base() + 0x38,
                    {c.n64 ? 0x802fbafc : 0x802c1f38, 0, 0}, c.palette))
        return false;
      c.pc = 5;
      continue;
      R(5, 0x3c, c.global(), manager, 6);
      W(6, 0x40, c.manager + c.offset(), c.palette, 7);
      R(7, 0x44, c.global(), manager, 8);
      R(8, 0x48, c.manager + c.offset(), palette, 9);
    case 9:
      c.pc = c.palette ? 30 : 10;
      continue;
      R(10, 0x54, 0x80415fd0, gp, 11);
      R(11, 0x5c, c.gp + 0x16c, value, 12);
    case 12:
      c.prefix = c.n64 ? (c.value ? 0x80425b0c : 0x80425b08)
                       : (c.value ? 0x80420184 : 0x80420180);
      c.pc = 13;
      continue;
    case 13:
      if (!c.invoke(0x80007388, c.base() + 0x6c, {}, c.root))
        return false;
      c.pc = 14;
      continue;
    case 14:
      if (!c.invoke(0x8026a0b8, c.base() + 0x88,
                    {c.n64 ? 0x802fbb0c : 0x802c1f44, c.root, c.prefix},
                    c.value, true))
        return false;
      c.pc = 15;
      continue;
    case 15:
      if (!c.invoke(0x80144e88, c.base() + 0x98, {2, 0}, c.handle, false, true))
        return false;
      c.pc = 16;
      continue;
    case 16:
      if (!c.invoke(0x80144dcc, c.base() + 0xa0, {c.handle}, c.value))
        return false;
      c.rounded = (c.value + 31) & 0xffffffe0u;
      c.pc = 17;
      continue;
    case 17:
      if (!c.invoke(0x800300f0, c.base() + 0xb4, {0, c.rounded}, c.palette))
        return false;
      c.pc = 18;
      continue;
      R(18, 0xb8, c.global(), manager, 19);
      W(19, 0xc8, c.manager + c.offset(), c.palette, 20);
      W(20, 0xd4, c.handle + 0x6c, c.handle, 21);
      R(21, 0xd8, c.global(), manager, 22);
      R(22, 0xdc, c.manager + c.offset(), palette, 23);
    case 23:
      if (!c.invoke(0x80144de4, c.base() + 0xe0,
                    {c.handle, c.palette, c.rounded, 0,
                     c.n64 ? N64Callback : Callback},
                    c.value))
        return false;
      c.pc = 40;
      continue;
    // Archive route uses the exact reloaded palette, then a fresh manager for
    // ready.
    case 30:
      c.pending = 0x80037b6c;
      if (!resumeSourceUnpackTexturePalette(c.palette, c.services, c.unpack) ||
          !c.valid())
        return false;
      c.pc = 31;
      continue;
      R(31, 0xec, c.global(), manager, 32);
      W(32, 0xf4, c.manager + c.offset() + 4, 1, 40);
    // Primary MUST execute the whole N64 installer after either route. It
    // shares process history but has a distinct palette/state and independent
    // DVD callback.
    case 40:
      if (!c.n64) {
        c.n64 = true;
        c.pc = 0;
        continue;
      }
      c.pc = 900;
      continue;
      // Original callback loads manager first, captures secondArg+2c handle,
      // then reads palette using that captured manager. No source completion is
      // invented.
      R(100, 0x10, c.global(), manager, 101);
      R(101, 0x14, c.input + 0x2c, handle, 102);
      R(102, 0x18, c.manager + c.offset(), palette, 103);
    case 103:
      c.pending = 0x80037b6c;
      if (!resumeSourceUnpackTexturePalette(c.palette, c.services, c.unpack) ||
          !c.valid())
        return false;
      c.pc = 104;
      continue;
    case 104:
      if (!c.invoke(0x80144dd4, c.base() + 0x24, {c.handle}, c.value))
        return false;
      c.pc = 105;
      continue;
      R(105, 0x28, c.global(), manager, 106);
      W(106, 0x30, c.manager + c.offset() + 4, 1, 900);
    case 900:
      cursor.state_.reset();
      return true;
    default:
      return false;
    }
  }
#undef R
#undef W
}
bool resumeSourceEffectTextureSetup(const SourceEffectTextureSetupServices &s,
                                    SourceEffectTextureSetupCursor &c) {
  return resumeSourceEffectTextureEntry(Setup, 0, s, c);
}
bool resumeSourceEffectTextureCompletion(
    uint32_t second, const SourceEffectTextureSetupServices &s,
    SourceEffectTextureSetupCursor &c) {
  return resumeSourceEffectTextureEntry(Callback, second, s, c);
}
bool resumeSourceEffectTextureN64Completion(
    uint32_t second, const SourceEffectTextureSetupServices &s,
    SourceEffectTextureSetupCursor &c) {
  return resumeSourceEffectTextureEntry(N64Callback, second, s, c);
}
} // namespace ttyd
