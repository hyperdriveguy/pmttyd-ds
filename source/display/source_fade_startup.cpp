#include "source_fade_startup.hpp"
namespace ttyd {
namespace {
constexpr uint32_t Init = 0x80013b90, Setup = 0x80013aa4,
                   Completion = 0x80013ca4;
constexpr uint32_t Global = 0x8041e620, PaletteOffset = 0x358,
                   ReadyOffset = 0x35c;
constexpr uint32_t RowStride = 0xa8;
constexpr uint32_t RowOffsets[] = {0x74, 0x78, 0x24, 0x28,
                                   0x80, 0x84, 0x98, 0xac};
constexpr uint32_t RowSites[5][8] = {
    {0x80013bc8, 0x80013bd0, 0x80013bd4, 0x80013bd8, 0x80013bdc, 0x80013be0,
     0x80013be4, 0x80013be8},
    {0x80013bf0, 0x80013bf4, 0x80013bf8, 0x80013bfc, 0x80013c00, 0x80013c04,
     0x80013c08, 0x80013c0c},
    {0x80013c14, 0x80013c18, 0x80013c1c, 0x80013c20, 0x80013c24, 0x80013c28,
     0x80013c2c, 0x80013c30},
    {0x80013c38, 0x80013c3c, 0x80013c40, 0x80013c44, 0x80013c48, 0x80013c4c,
     0x80013c50, 0x80013c54},
    {0x80013c5c, 0x80013c60, 0x80013c64, 0x80013c68, 0x80013c6c, 0x80013c70,
     0x80013c74, 0x80013c78}};
constexpr uint32_t RowReads[] = {0x80013bb8, 0x80013bec, 0x80013c10, 0x80013c34,
                                 0x80013c58};
bool same(const SourceFadeStartupServices &a,
          const SourceFadeStartupServices &b) {
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
struct SourceFadeStartupCursor::State {
  SourceFadeStartupServices services;
  uint32_t function = 0, input = 0, pc = 0, pending = 0, value = 0, palette = 0,
           handle = 0, root = 0, length = 0, manager = 0, zero = 0, one = 0,
           row = 0, slot = 0;
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
SourceFadeStartupCursor::SourceFadeStartupCursor() = default;
SourceFadeStartupCursor::~SourceFadeStartupCursor() = default;
bool SourceFadeStartupCursor::active() const { return bool(state_); }
bool SourceFadeStartupCursor::stale() const {
  return state_ && state_->invalid;
}
uint32_t SourceFadeStartupCursor::pending() const {
  return !state_                   ? 0
         : state_->unpack.active() ? state_->unpack.pending()
                                   : state_->pending;
}
bool SourceFadeStartupCursor::reset() {
  if (invoking_)
    return false;
  state_.reset();
  return true;
}
bool resumeSourceFadeEntry(uint32_t fn, uint32_t input,
                           const SourceFadeStartupServices &s,
                           SourceFadeStartupCursor &cursor) {
  if (cursor.invoking_ || !s.objects.gx.lifetime || !s.objects.gx.current ||
      (fn != Init && fn != Setup && fn != Completion))
    return false;
  if (cursor.state_ &&
      (cursor.state_->function != fn || cursor.state_->input != input ||
       !same(cursor.state_->services, s)))
    return false;
  Guard guard(cursor.invoking_);
  if (!cursor.state_) {
    cursor.state_ = std::make_unique<SourceFadeStartupCursor::State>();
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
      if (!c.invoke(0x800300f0, 0x80013ba4, {0, 0x360}, c.manager))
        return false;
      c.pc = 1;
      continue;
      W(1, 0x80013ba8, Global, c.manager, 2);
    case 2:
      if (!c.invoke(0x800050b4, 0x80013bb4, {c.manager, 0, 0x360}, c.value))
        return false;
      c.pc = 3;
      continue;
      R(3, 0x80013bb8, Global, manager, 4);
      R(4, 0x80013bc0, 0x8041f7a8, zero, 5);
      W(5, 0x80013bc8, c.manager + 0x74, 0xffffffffu, 6);
      R(6, 0x80013bcc, 0x8041f7ac, one, 7);
    // The first -1 store precedes the one load. Later rows reuse both loaded
    // binary32 values; each row captures its own live manager pointer.
    case 7:
      c.slot = 1;
      c.pc = 9;
      continue;
    case 8:
      if (!c.read(RowReads[c.row], Global, c.manager))
        return false;
      c.slot = 0;
      c.pc = 9;
      continue;
    case 9: {
      const uint32_t value = c.slot < 2    ? 0xffffffffu
                             : c.slot == 6 ? c.one
                             : c.slot == 7 ? 0
                                           : c.zero;
      if (!c.write(RowSites[c.row][c.slot],
                   c.manager + c.row * RowStride + RowOffsets[c.slot], value))
        return false;
      if (++c.slot < 8)
        continue;
      if (++c.row < 5) {
        c.pc = 8;
        continue;
      }
      c.pc = 10;
      continue;
    }
      R(10, 0x80013c7c, Global, manager, 11);
      W(11, 0x80013c80, c.manager + 0x350, 0xffffffffu, 12);
      R(12, 0x80013c84, Global, manager, 13);
      W(13, 0x80013c88, c.manager + PaletteOffset, 0, 14);
      R(14, 0x80013c8c, Global, manager, 15);
      W(15, 0x80013c90, c.manager + ReadyOffset, 0, 900);
      R(100, 0x80013ac8, Global, manager, 101);
      W(101, 0x80013acc, c.manager + PaletteOffset, 0, 102);
    case 102:
      if (!c.invoke(0x800782e4, 0x80013ad4, {0x802bf79c, 0, 0}, c.palette))
        return false;
      c.pc = 103;
      continue;
      R(103, 0x80013ad8, Global, manager, 104);
      W(104, 0x80013adc, c.manager + PaletteOffset, c.palette, 105);
      R(105, 0x80013ae0, Global, manager, 106);
      R(106, 0x80013ae4, c.manager + PaletteOffset, palette, 107);
    case 107:
      c.pc = c.palette ? 130 : 108;
      continue;
    case 108:
      if (!c.invoke(0x80007388, 0x80013af0, {}, c.root))
        return false;
      c.pc = 109;
      continue;
    case 109:
      if (!c.invoke(0x8026a0b8, 0x80013b08, {0x802bf7a8, c.root}, c.value,
                    true))
        return false;
      c.pc = 110;
      continue;
    case 110:
      if (!c.invoke(0x80144e88, 0x80013b18, {2, 0}, c.handle, false, true))
        return false;
      c.pc = 111;
      continue;
    case 111:
      if (!c.invoke(0x80144dcc, 0x80013b20, {c.handle}, c.value))
        return false;
      c.length = (c.value + 31) & 0xffffffe0u;
      c.pc = 112;
      continue;
    case 112:
      if (!c.invoke(0x800300f0, 0x80013b34, {0, c.length}, c.palette))
        return false;
      c.pc = 113;
      continue;
      R(113, 0x80013b38, Global, manager, 114);
      W(114, 0x80013b48, c.manager + PaletteOffset, c.palette, 115);
      W(115, 0x80013b54, c.handle + 0x6c, c.handle, 116);
      R(116, 0x80013b58, Global, manager, 117);
      R(117, 0x80013b5c, c.manager + PaletteOffset, palette, 118);
    case 118:
      if (!c.invoke(0x80144de4, 0x80013b60,
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
      R(131, 0x80013b6c, Global, manager, 132);
      W(132, 0x80013b74, c.manager + ReadyOffset, 1, 900);
      R(200, 0x80013cb4, Global, manager, 201);
      R(201, 0x80013cb8, c.input + 0x2c, handle, 202);
      R(202, 0x80013cbc, c.manager + PaletteOffset, palette, 203);
    case 203:
      c.pending = 0x80037b6c;
      if (!resumeSourceUnpackTexturePalette(c.palette, c.services, c.unpack) ||
          !c.valid())
        return false;
      c.pc = 204;
      continue;
    case 204:
      if (!c.invoke(0x80144dd4, 0x80013cc8, {c.handle}, c.value))
        return false;
      c.pc = 205;
      continue;
      R(205, 0x80013ccc, Global, manager, 206);
      W(206, 0x80013cd4, c.manager + ReadyOffset, 1, 900);
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
bool resumeSourceFadeInit(const SourceFadeStartupServices &s,
                          SourceFadeStartupCursor &c) {
  return resumeSourceFadeEntry(Init, 0, s, c);
}
bool resumeSourceFadeTextures(const SourceFadeStartupServices &s,
                              SourceFadeStartupCursor &c) {
  return resumeSourceFadeEntry(Setup, 0, s, c);
}
bool resumeSourceFadeCompletion(uint32_t second,
                                const SourceFadeStartupServices &s,
                                SourceFadeStartupCursor &c) {
  return resumeSourceFadeEntry(Completion, second, s, c);
}
} // namespace ttyd
