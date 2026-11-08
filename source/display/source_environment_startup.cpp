#include "source_environment_startup.hpp"
namespace ttyd {
namespace {
constexpr uint32_t Init = 0x8019e09c, Setup = 0x8019df84,
                   Completion = 0x8019e0f0;
constexpr uint32_t Palette = 0x8041ec90, Ready = 0x8041ec94;
bool same(const SourceEnvironmentStartupServices &a,
          const SourceEnvironmentStartupServices &b) {
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
struct SourceEnvironmentStartupCursor::State {
  SourceEnvironmentStartupServices services;
  uint32_t function = 0, input = 0, pc = 0, pending = 0, value = 0, palette = 0,
           handle = 0, root = 0, length = 0;
  bool invalid = false;
  SourceWindowTexturePath path;
  std::optional<SourceWindowTextureSetupCall> call;
  SourceWindowTextureSetupCursor unpack;
  bool valid() {
    if (invalid)
      return false;
    if (!services.objects.gx.current ||
        !services.objects.gx.current(services.objects.gx.user))
      invalid = true;
    return !invalid;
  }
  bool read(uint32_t site, uint32_t at, uint32_t &out) {
    pending = site;
    if (!valid() || !services.objects.gx.word)
      return false;
    auto v = services.objects.gx.word(services.objects.gx.user, at);
    if (!valid() || !v)
      return false;
    out = *v;
    return true;
  }
  bool write(uint32_t site, uint32_t at, uint32_t v) {
    pending = site;
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
SourceEnvironmentStartupCursor::SourceEnvironmentStartupCursor() = default;
SourceEnvironmentStartupCursor::~SourceEnvironmentStartupCursor() = default;
bool SourceEnvironmentStartupCursor::active() const { return bool(state_); }
bool SourceEnvironmentStartupCursor::stale() const {
  return state_ && state_->invalid;
}
uint32_t SourceEnvironmentStartupCursor::pending() const {
  return !state_                   ? 0
         : state_->unpack.active() ? state_->unpack.pending()
                                   : state_->pending;
}
bool SourceEnvironmentStartupCursor::reset() {
  if (invoking_)
    return false;
  state_.reset();
  return true;
}
bool resumeSourceEnvironmentEntry(uint32_t fn, uint32_t input,
                                  const SourceEnvironmentStartupServices &s,
                                  SourceEnvironmentStartupCursor &cursor) {
  if (cursor.invoking_ || !s.objects.gx.lifetime || !s.objects.gx.current ||
      (fn != Init && fn != Setup && fn != Completion))
    return false;
  if (cursor.state_ &&
      (cursor.state_->function != fn || cursor.state_->input != input ||
       !same(cursor.state_->services, s)))
    return false;
  Guard guard(cursor.invoking_);
  if (!cursor.state_) {
    cursor.state_ = std::make_unique<SourceEnvironmentStartupCursor::State>();
    auto &c = *cursor.state_;
    c.services = s;
    c.function = fn;
    c.input = input;
    c.pc = fn == Init ? 0 : fn == Setup ? 100 : 200;
  }
  auto &c = *cursor.state_;
#define W(P, S, A, V, N)                                                       \
  case P:                                                                      \
    if (!c.write(S, A, V))                                                     \
      return false;                                                            \
    c.pc = N;                                                                  \
    continue
#define R(P, S, A, D, N)                                                       \
  case P:                                                                      \
    if (!c.read(S, A, c.D))                                                    \
      return false;                                                            \
    c.pc = N;                                                                  \
    continue
  for (;;) {
    if (!c.valid())
      return false;
    switch (c.pc) {
    case 0:
      if (!c.invoke(0x800050b4, 0x8019e0b8, {0x803e7278, 0, 0xf0}, c.value))
        return false;
      c.pc = 1;
      continue;
    case 1:
      if (!c.invoke(0x800050b4, 0x8019e0d0, {0x803e7368, 0, 0xf0}, c.value))
        return false;
      c.pc = 2;
      continue;
      W(2, 0x8019e0d8, Palette, 0, 3);
      W(3, 0x8019e0dc, Ready, 0, 900);
      // Setup preserves Ready. Branch consumes captured archive return, not a
      // reload of Palette; whole Unpack receives that retained return on a hit.
      W(100, 0x8019dfac, Palette, 0, 101);
    case 101:
      if (!c.invoke(0x800782e4, 0x8019dfb0, {0x802f9c78, 0, 0}, c.palette))
        return false;
      c.pc = 102;
      continue;
      W(102, 0x8019dfb8, Palette, c.palette, 103);
    case 103:
      c.pc = c.palette ? 130 : 104;
      continue;
    case 104:
      if (!c.invoke(0x80007388, 0x8019dfc0, {}, c.root))
        return false;
      c.pc = 105;
      continue;
    case 105:
      if (!c.invoke(0x8026a0b8, 0x8019dfd8, {0x802f9c80, c.root}, c.value,
                    true))
        return false;
      c.pc = 106;
      continue;
    case 106:
      if (!c.invoke(0x80144e88, 0x8019dfe8, {2, 0}, c.handle, false, true))
        return false;
      c.pc = 107;
      continue;
    case 107:
      if (!c.invoke(0x80144dcc, 0x8019dff0, {c.handle}, c.length))
        return false;
      c.pc = 108;
      continue;
    case 108:
      if (!c.invoke(0x800300f0, 0x8019e008, {0, (c.length + 31) & 0xffffffe0u},
                    c.palette))
        return false;
      c.pc = 109;
      continue;
      W(109, 0x8019e00c, Palette, c.palette, 110);
      W(110, 0x8019e01c, c.handle + 0x6c, c.handle, 111);
      R(111, 0x8019e028, Palette, palette, 112);
    case 112:
      if (!c.invoke(0x80144de4, 0x8019e02c,
                    {c.handle, c.palette, c.length, 0, Completion}, c.value))
        return false;
      c.pc = 900;
      continue;
    case 130:
      c.pending = 0x80037b6c;
      if (!resumeSourceUnpackTexturePalette(c.palette, c.services, c.unpack) ||
          !c.valid())
        return false;
      c.pc = 131;
      continue;
      W(131, 0x8019e03c, Ready, 1, 900);
      R(200, 0x8019e100, c.input + 0x2c, handle, 201);
      R(201, 0x8019e104, Palette, palette, 202);
    case 202:
      c.pending = 0x80037b6c;
      if (!resumeSourceUnpackTexturePalette(c.palette, c.services, c.unpack) ||
          !c.valid())
        return false;
      c.pc = 203;
      continue;
    case 203:
      if (!c.invoke(0x80144dd4, 0x8019e110, {c.handle}, c.value))
        return false;
      c.pc = 204;
      continue;
      W(204, 0x8019e118, Ready, 1, 900);
    case 900:
      cursor.state_.reset();
      return true;
    default:
      return false;
    }
  }
#undef W
#undef R
}
bool resumeSourceEnvironmentInit(const SourceEnvironmentStartupServices &s,
                                 SourceEnvironmentStartupCursor &c) {
  return resumeSourceEnvironmentEntry(Init, 0, s, c);
}
bool resumeSourceEnvironmentTextures(const SourceEnvironmentStartupServices &s,
                                     SourceEnvironmentStartupCursor &c) {
  return resumeSourceEnvironmentEntry(Setup, 0, s, c);
}
bool resumeSourceEnvironmentCompletion(
    uint32_t second, const SourceEnvironmentStartupServices &s,
    SourceEnvironmentStartupCursor &c) {
  return resumeSourceEnvironmentEntry(Completion, second, s, c);
}
} // namespace ttyd
