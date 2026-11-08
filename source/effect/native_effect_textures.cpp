#include "native_effect_textures.hpp"
#include "event_heap_allocation.hpp"
#include "event_memory_opcodes.hpp"
#include "native_ui_startup.hpp"
#include "source_archive_layout.hpp"
#include "source_effect_texture_setup.hpp"
#include "source_environment_startup.hpp"
#include <algorithm>
#include <array>
#include <cstdio>
#include <string>
namespace ttyd {
namespace {
struct Span {
  uint32_t address, bytes;
  uint64_t hash;
};
constexpr Span Spans[] = {{0x802c1f38, 12, 0xc9c290165cf23912ULL},
                          {0x802c1f44, 20, 0x5bb27e9cc03c8cf8ULL},
                          {0x802fbafc, 16, 0x3e4865cd3984d841ULL},
                          {0x802fbb0c, 24, 0xc9c165346fa3225bULL},
                          {0x80425b08, 8, 0x8bf6ce8f5759b61fULL}};
// Borrow actual source process owners; never install/reset these ranges.
constexpr std::pair<uint32_t, uint32_t> Borrowed[] = {
    {0x80415ee0, 4},
    {0x8041e908, 24},
    {0x80419a40, 4},
    {0x8041ecd0, 8},
    {0x8041f494, 4},
    {0x80420180, 8},
    {SourceArchiveLayout::Descriptors, SourceArchiveLayout::TableBytes}};
constexpr Span EnvironmentSpans[] = {{0x802f9c78, 8, 0x508c98090bcb0be6ULL},
                                     {0x802f9c80, 12, 0x77cbd085e940a32bULL},
                                     {0x803e7278, 480, 0xfcfda4c07d71b8a5ULL},
                                     {0x8041ec90, 8, 0xa8c7f832281a39c5ULL}};
constexpr uint32_t EnvironmentBytes = 37248;
constexpr uint64_t EnvironmentHash = 0x8b95563bd8932bc5ULL;
constexpr uint32_t EntryBytes = 152, EntryStart = 0x60050000,
                   PathCapacity = 128;
constexpr uint32_t Sizes[] = {266784, 235776};
constexpr uint64_t Hashes[] = {0xba8bac8cb699f282ULL, 0xb29a81af3cdde929ULL};
constexpr const char *Names[] = {"effect.tpl", "effect_n64.tpl"};
struct Guard {
  bool &v;
  explicit Guard(bool &f) : v(f) { v = true; }
  ~Guard() { v = false; }
};
} // namespace
struct NativeSourceTextureStartup::State {
  std::shared_ptr<NativeUiStartup> ui;
  std::shared_ptr<NativeFieldAnimation> process;
  std::unique_ptr<EventAddressSpace> identity;
  std::string root;
  SourceEffectTextureSetupCursor setup;
  SourceEnvironmentStartupCursor environmentInit, environmentSetup;
  bool environment = false, initialized = false, archivePinned = false;
  std::vector<Span> spans;
  std::vector<std::pair<uint32_t, uint32_t>> borrowed;
  bool admitted = false, invalid = false;
  uint32_t pending = 0, archiveIndex = 0;
  bool archiveHeld = false;
  struct Job {
    uint32_t handle = 0, palette = 0, bytes = 0, readBytes = 0, delivered = 0;
    FILE *file = nullptr;
    bool queued = false, callback = false, done = false;
    uint64_t hash = 14695981039346656037ULL;
    SourceEffectTextureSetupCursor completion;
    SourceEnvironmentStartupCursor environmentCompletion;
  };
  std::array<Job, 2> jobs;
  ~State() {
    for (auto &j : jobs)
      if (j.file)
        std::fclose(j.file);
  }
  EventNativeRuntime &vm() const { return process->process().vm(); }
  bool current() {
    if (invalid || !ui || !ui->current() || ui->fieldProcess() != process ||
        !identity)
      return false;
    for (auto s : borrowed)
      if (!vm().memory.sharesRamWith(*identity, s.first, s.second)) {
        invalid = true;
        return false;
      }
    for (auto s : spans)
      if (!vm().memory.sharesRamWith(*identity, s.address, s.bytes)) {
        invalid = true;
        return false;
      }
    for (auto &j : jobs) {
      if (j.handle &&
          !vm().memory.sharesRamWith(*identity, j.handle, EntryBytes)) {
        invalid = true;
        return false;
      }
      if (j.palette &&
          !vm().memory.sharesRamWith(*identity, j.palette, j.bytes)) {
        invalid = true;
        return false;
      }
    }
    return true;
  }
  std::optional<uint32_t> word(uint32_t a) {
    uint32_t v;
    if (!current() || !vm().memory.readWord(a, v))
      return {};
    return v;
  }
  bool store(uint32_t a, uint32_t n, uint32_t v) {
    return current() && (n == 1   ? vm().memory.writeByte(a, uint8_t(v))
                         : n == 2 ? vm().memory.writeHalf(a, uint16_t(v))
                         : n == 4 ? vm().memory.writeWord(a, v)
                                  : false);
  }
  bool pin(uint32_t a, uint32_t n) {
    return vm().memory.shareRamTo(*identity, a, n);
  }
  bool text(uint32_t a, std::string &out) {
    out.clear();
    for (uint32_t i = 0; i < PathCapacity; ++i) {
      uint8_t b;
      if (!current() || !vm().memory.readByte(a + i, b))
        return false;
      if (!b)
        return true;
      out += char(b);
    }
    return false;
  }
  uint32_t size(unsigned i) const {
    return environment ? EnvironmentBytes : Sizes[i];
  }
  uint32_t callback(unsigned i) const {
    return environment ? 0x8019e0f0 : i ? 0x801d7a58 : 0x8005efa8;
  }
  const char *name(unsigned i) const {
    return environment ? "env.tpl" : Names[i];
  }
  bool borrowArchive() {
    if (archivePinned)
      return true;
    if (!vm().memory.contiguousRam(SourceArchiveLayout::Descriptors,
                                   SourceArchiveLayout::TableBytes) ||
        !pin(SourceArchiveLayout::Descriptors, SourceArchiveLayout::TableBytes))
      return false;
    borrowed.push_back(
        {SourceArchiveLayout::Descriptors, SourceArchiveLayout::TableBytes});
    archivePinned = true;
    return true;
  }
  unsigned bank(const SourceWindowTextureSetupCall &q) const {
    if (environment)
      return 0;
    return q.callsite >= 0x801d7910 && q.callsite < 0x801d7aa0 ? 1 : 0;
  }
  std::optional<uint32_t> request(const SourceWindowTextureSetupCall &q) {
    if (!current())
      return {};
    const unsigned i = bank(q);
    auto &j = jobs[i];
    switch (q.function) {
    case 0x800050b4: {
      if (!environment || q.count != 3 || q.arguments[1] ||
          q.arguments[2] != 0xf0 ||
          ((q.callsite != 0x8019e0b8 || q.arguments[0] != 0x803e7278) &&
           (q.callsite != 0x8019e0d0 || q.arguments[0] != 0x803e7368)))
        return {};
      eventFillMemory(q.arguments[0], 0, q.arguments[2],
                      vm().memoryBinding.services());
      return q.arguments[0];
    }
    case 0x800782e4: {
      const uint32_t name = environment ? 0x802f9c78
                            : i         ? 0x802fbafc
                                        : 0x802c1f38;
      if (q.count != 3 ||
          q.arguments != std::array<uint32_t, 5>{name, 0, 0, 0, 0})
        return {};
      if (!archivePinned || archiveHeld)
        return {};
      while (archiveIndex < SourceArchiveLayout::DescriptorCount) {
        uint16_t f;
        if (!vm().memory.readHalf(SourceArchiveLayout::Descriptors +
                                      archiveIndex *
                                          SourceArchiveLayout::DescriptorBytes,
                                  f))
          return {};
        if (f & 1) {
          archiveHeld = true;
          return {};
        }
        ++archiveIndex;
      }
      archiveIndex = 0;
      return 0;
    }
    case 0x80007388: {
      if (q.count)
        return {};
      return 0x8041f494;
    }
    case 0x8026a0b8: {
      if (q.count != (environment ? 2 : 3) || !q.destination ||
          q.arguments[0] != (environment ? 0x802f9c80
                             : i         ? 0x802fbb0c
                                         : 0x802c1f44))
        return {};
      std::string format, rootName, prefix;
      if (!text(q.arguments[0], format) ||
          format != (environment ? "%s/env.tpl"
                     : i         ? "%s/e/%s/effect_n64.tpl"
                                 : "%s/e/%s/effect.tpl") ||
          !text(q.arguments[1], rootName) ||
          (!environment && !text(q.arguments[2], prefix)))
        return {};
      const auto path = environment ? rootName + "/env.tpl"
                                    : rootName + "/e/" + prefix + "/" + name(i);
      if (path.size() >= PathCapacity)
        return {};
      auto cells = q.destination->cells;
      for (uint32_t k = 0; k <= path.size(); ++k)
        cells[k] = k == path.size() ? 0 : uint8_t(path[k]);
      q.destination->cells = std::move(cells);
      return uint32_t(path.size());
    }
    case 0x80144e88: {
      if (q.count != 2 || q.arguments[0] != 2 || q.arguments[1] || !q.path ||
          j.handle)
        return {};
      std::string path;
      bool end = false;
      for (uint32_t k = 0; k < PathCapacity; ++k) {
        auto b = q.path->byte(k);
        if (!b)
          return {};
        if (!*b) {
          end = true;
          break;
        }
        path += char(*b);
      }
      if (!end || path != (environment ? "./env.tpl"
                                       : "./e/us/" + std::string(name(i))))
        return {};
      FILE *f = std::fopen(
          (root + (environment ? "/environment-startup/" : "/effect-primary/") +
           name(i))
              .c_str(),
          "rb");
      if (!f)
        return 0;
      if (std::fseek(f, 0, SEEK_END) || std::ftell(f) != long(size(i)) ||
          std::fseek(f, 0, SEEK_SET)) {
        std::fclose(f);
        return {};
      }
      uint32_t handle = 0;
      for (uint32_t k = 0; k < 256; ++k) {
        const auto a = EntryStart + k * EntryBytes;
        if (vm().memory.unmappedRange(a, EntryBytes)) {
          handle = a;
          break;
        }
      }
      if (!handle ||
          !vm().memory.registerRam(handle, std::vector<uint8_t>(EntryBytes))) {
        std::fclose(f);
        return {};
      }
      if (!pin(handle, EntryBytes)) {
        std::fclose(f);
        invalid = true;
        return {};
      }
      j.handle = handle;
      j.file = f;
      for (uint32_t k = 0; k <= path.size(); ++k)
        if (!store(handle + k, 1, k == path.size() ? 0 : uint8_t(path[k])))
          return {};
      if (!store(handle + 0x74, 4, size(i)) ||
          !store(handle + 0x90, 2, 0x8000) || !store(handle + 0x92, 2, 2))
        return {};
      return handle;
    }
    case 0x80144dcc:
      if (q.count != 1 || q.arguments[0] != j.handle)
        return {};
      return word(j.handle + 0x74);
    case 0x800300f0: {
      if (q.count != 2 || q.arguments[0] ||
          q.arguments[1] != ((size(i) + 31) & ~31u) || j.palette)
        return {};
      const auto a =
          eventHeapAllocate(0, q.arguments[1], vm().memoryBinding.services());
      if (!a)
        return 0;
      j.palette = a;
      j.bytes = q.arguments[1];
      if (!pin(a, j.bytes)) {
        invalid = true;
        return {};
      }
      return a;
    }
    case 0x80144de4: {
      const uint32_t callback = this->callback(i);
      if (q.count != 5 || !j.handle || !j.palette || !j.file ||
          q.arguments !=
              std::array<uint32_t, 5>{j.handle, j.palette,
                                      environment ? size(i) : j.bytes, 0,
                                      callback} ||
          word(j.handle + 0x6c) != std::optional<uint32_t>(j.handle))
        return {};
      if (j.queued)
        return 0;
      // Original DVDMgrReadAsync writes destination/remaining/offset, reloads
      // flags twice, then publishes callback and clears progress. No callback
      // runs here.
      uint16_t f;
      if (!store(j.handle + 0x7c, 4, j.palette) ||
          !store(j.handle + 0x80, 4, environment ? size(i) : j.bytes) ||
          !store(j.handle + 0x84, 4, 0) ||
          !vm().memory.readHalf(j.handle + 0x90, f) ||
          !store(j.handle + 0x90, 2, f | 1) ||
          !vm().memory.readHalf(j.handle + 0x90, f) ||
          !store(j.handle + 0x90, 2, f & ~2u) ||
          !store(j.handle + 0x8c, 4, callback) || !store(j.handle + 0x88, 4, 0))
        return {};
      j.readBytes = environment ? size(i) : j.bytes;
      j.queued = true;
      return 0;
    }
    case 0x80144dd4: {
      if (q.count != 1 || q.arguments[0] != j.handle)
        return {};
      uint16_t f;
      if (!vm().memory.readHalf(j.handle + 0x90, f) ||
          !store(j.handle + 0x90, 2, f | 8))
        return {};
      return 0;
    }
    default:
      return {};
    }
  }
  SourceEffectTextureSetupServices services() {
    SourceEffectTextureSetupServices s;
    auto &g = s.objects.gx;
    g.user = this;
    g.lifetime = ui;
    g.current = [](void *p) { return static_cast<State *>(p)->current(); };
    g.word = [](void *p, uint32_t a) {
      return static_cast<State *>(p)->word(a);
    };
    g.byte = [](void *p, uint32_t a) -> std::optional<uint8_t> {
      auto &s = *static_cast<State *>(p);
      uint8_t v;
      if (!s.current() || !s.vm().memory.readByte(a, v))
        return {};
      return v;
    };
    g.store = [](void *p, uint32_t a, uint32_t n, uint32_t v) {
      return static_cast<State *>(p)->store(a, n, v);
    };
    s.request = [](void *p, const SourceWindowTextureSetupCall &q) {
      return static_cast<State *>(p)->request(q);
    };
    return s;
  }
  bool ready() {
    if (!current() || !admitted || !jobs[0].done ||
        (!environment && !jobs[1].done))
      return false;
    if (environment)
      return initialized && word(0x8041ec94) == std::optional<uint32_t>(1);
    auto manager = word(0x80415ee0), n64 = word(0x80419a40);
    return manager && n64 &&
           word(*manager + 0xc) == std::optional<uint32_t>(1) &&
           word(*n64 + 4) == std::optional<uint32_t>(1);
  }
};
NativeSourceTextureStartup::NativeSourceTextureStartup() = default;
NativeSourceTextureStartup::~NativeSourceTextureStartup() = default;
std::unique_ptr<NativeSourceTextureStartup>
NativeSourceTextureStartup::create(std::shared_ptr<NativeUiStartup> ui,
                                   const char *root) {
  return createProfile(std::move(ui), root, false);
}
std::unique_ptr<NativeSourceTextureStartup>
NativeSourceTextureStartup::createEnvironment(
    std::shared_ptr<NativeUiStartup> ui, const char *root) {
  return createProfile(std::move(ui), root, true);
}
std::unique_ptr<NativeSourceTextureStartup>
NativeSourceTextureStartup::createProfile(std::shared_ptr<NativeUiStartup> ui,
                                          const char *root, bool environment) {
  if (!ui || !root || !ui->current() || !ui->fieldProcess())
    return {};
  auto process = ui->fieldProcess();
  auto &vm = process->process().vm();
  std::vector<Span> spans =
      environment ? std::vector<Span>(std::begin(EnvironmentSpans),
                                      std::end(EnvironmentSpans))
                  : std::vector<Span>(std::begin(Spans), std::end(Spans));
  std::vector<std::pair<uint32_t, uint32_t>> borrowed =
      environment ? std::vector<std::pair<uint32_t, uint32_t>>{{0x8041f494, 4}}
                  : std::vector<std::pair<uint32_t, uint32_t>>(
                        std::begin(Borrowed), std::end(Borrowed));
  for (auto span : borrowed)
    if (!vm.memory.contiguousRam(span.first, span.second))
      return {};
  EventScriptAssets image;
  if (!image.load((std::string(root) +
                   (environment ? "/environment-startup/memory.pmevt"
                                : "/effect-primary/memory.pmevt"))
                      .c_str()) ||
      image.segments().size() != spans.size())
    return {};
  for (unsigned i = 0; i < spans.size(); ++i) {
    auto s = spans[i];
    auto b = image.segments()[i];
    if (s.address != b.address || s.bytes != b.size ||
        !vm.memory.unmappedRange(s.address, s.bytes))
      return {};
    uint64_t hash = 14695981039346656037ULL;
    for (uint32_t k = 0; k < s.bytes; ++k) {
      uint8_t v;
      if (!image.readByte(s.address + k, v))
        return {};
      hash = (hash ^ v) * 1099511628211ULL;
    }
    if (hash != s.hash)
      return {};
  }
  if (!vm.memory.importScripts(image))
    return {};
  auto out = std::unique_ptr<NativeSourceTextureStartup>(
      new NativeSourceTextureStartup);
  out->state_ = std::make_unique<State>();
  auto &s = *out->state_;
  s.ui = std::move(ui);
  s.process = std::move(process);
  s.root = root;
  s.environment = environment;
  s.spans = std::move(spans);
  s.borrowed = std::move(borrowed);
  s.archivePinned = !environment;
  s.identity = std::make_unique<EventAddressSpace>(
      vm.entryAddresses, vm.contextAddresses, vm.runtimeAddresses,
      vm.gpAddresses);
  for (auto span : s.spans)
    if (!s.pin(span.address, span.bytes))
      return {};
  for (auto span : s.borrowed)
    if (!s.pin(span.first, span.second))
      return {};
  return out;
}
bool NativeSourceTextureStartup::current() const {
  return state_ && state_->current();
}
bool NativeSourceTextureStartup::initializeEnvironment() {
  if (invoking_ || !current() || !state_->environment)
    return false;
  Guard guard(invoking_);
  auto &s = *state_;
  if (s.initialized)
    return true;
  if (!resumeSourceEnvironmentInit(s.services(), s.environmentInit)) {
    s.pending = s.environmentInit.pending();
    return false;
  }
  s.initialized = true;
  s.pending = 0;
  return true;
}
bool NativeSourceTextureStartup::begin() {
  if (invoking_ || !current())
    return false;
  Guard guard(invoking_);
  auto &s = *state_;
  if (s.admitted)
    return true;
  if (s.environment && (!s.initialized || !s.borrowArchive()))
    return false;
  const bool done =
      s.environment
          ? resumeSourceEnvironmentTextures(s.services(), s.environmentSetup)
          : resumeSourceEffectTextureSetup(s.services(), s.setup);
  if (!done) {
    s.pending =
        s.environment ? s.environmentSetup.pending() : s.setup.pending();
    return false;
  }
  s.admitted = true;
  s.pending = 0;
  return true;
}
bool NativeSourceTextureStartup::pump(uint32_t budget) {
  if (invoking_ || !current())
    return false;
  Guard guard(invoking_);
  auto &s = *state_;
  for (unsigned i = 0; i < (s.environment ? 1u : 2u); ++i) {
    auto &j = s.jobs[i];
    if (!j.queued || j.done)
      continue;
    const auto remaining = s.word(j.handle + 0x80),
               destination = s.word(j.handle + 0x7c),
               progress = s.word(j.handle + 0x88);
    if (!remaining || !destination || !progress ||
        *remaining != j.readBytes - j.delivered || *progress != j.delivered ||
        *destination != j.palette + j.delivered)
      return false;
    const uint32_t n = std::min(budget, *remaining);
    if (n) {
      if (!j.file)
        return false;
      std::vector<uint8_t> bytes(n);
      if (std::fread(bytes.data(), 1, n, j.file) != n) {
        s.invalid = true;
        return false;
      }
      if (!s.current() || !s.vm().memory.withRawRamAccess([&](auto &ram) {
            if (!ram.bind(*destination, n))
              return false;
            uint32_t k = 0;
            for (; k + 4 <= n; k += 4)
              if (!ram.writeWord(
                      *destination + k,
                      uint32_t(bytes[k]) << 24 | uint32_t(bytes[k + 1]) << 16 |
                          uint32_t(bytes[k + 2]) << 8 | bytes[k + 3]))
                return false;
            for (; k < n; ++k)
              if (!ram.writeByte(*destination + k, bytes[k]))
                return false;
            return true;
          }))
        return false;
      for (auto b : bytes)
        j.hash = (j.hash ^ b) * 1099511628211ULL;
      j.delivered += n;
      budget -= n;
      if (!s.store(j.handle + 0x80, 4, *remaining - n) ||
          !s.store(j.handle + 0x88, 4, *progress + n) ||
          !s.store(j.handle + 0x7c, 4, *destination + n))
        return false;
    }
    if (j.delivered != j.readBytes)
      continue;
    if (j.hash != (s.environment ? EnvironmentHash : Hashes[i])) {
      s.invalid = true;
      return false;
    }
    if (!j.callback) {
      uint16_t f;
      if (!s.vm().memory.readHalf(j.handle + 0x90, f) ||
          !s.store(j.handle + 0x90, 2, f & ~1u) ||
          !s.vm().memory.readHalf(j.handle + 0x90, f) ||
          !s.store(j.handle + 0x90, 2, f | 2))
        return false;
      j.callback = true;
    }
    const uint32_t callback = s.callback(i);
    if (s.word(j.handle + 0x8c) != std::optional<uint32_t>(callback))
      return false;
    const auto services = s.services();
    const bool done =
        s.environment ? resumeSourceEnvironmentCompletion(
                            j.handle + 0x40, services, j.environmentCompletion)
        : i ? resumeSourceEffectTextureN64Completion(j.handle + 0x40, services,
                                                     j.completion)
            : resumeSourceEffectTextureCompletion(j.handle + 0x40, services,
                                                  j.completion);
    if (!done) {
      s.pending = s.environment ? j.environmentCompletion.pending()
                                : j.completion.pending();
      return false;
    }
    uint16_t flags;
    if (!s.vm().memory.readHalf(j.handle + 0x90, flags) || !(flags & 8))
      return false;
    std::fclose(j.file);
    j.file = nullptr;
    j.done = true;
  }
  s.pending = 0;
  return s.ready();
}
bool NativeSourceTextureStartup::ready() const {
  return state_ && state_->ready();
}
bool NativeSourceTextureStartup::busy() const {
  return state_ &&
         (state_->setup.active() || state_->environmentInit.active() ||
          state_->environmentSetup.active() ||
          std::any_of(state_->jobs.begin(), state_->jobs.end(),
                      [](const auto &j) { return j.queued && !j.done; }));
}
uint32_t NativeSourceTextureStartup::pending() const {
  return state_ ? state_->pending : 0;
}
} // namespace ttyd
