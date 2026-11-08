#include "native_logo_startup.hpp"
#include "event_memory_opcodes.hpp"
#include "source_mario_dvd_root.hpp"
namespace ttyd {
namespace {
struct Span {
  uint32_t address, bytes;
  uint64_t hash;
};
constexpr Span Spans[] = {{0x802c3b68, 16, 0xe714c8bac19ca2a3ULL},
                          {0x8030c230, 60, 0x6ea987ed97567b68ULL},
                          {0x8030c26c, 60, 0xc1565c6e3fece79dULL},
                          {0x80416108, 4, 0x145883a97721e910ULL},
                          {0x8041ea80, 32, 0x0c8210784d8af5a5ULL}};
constexpr uint32_t WorkGlobal = 0x80416108, Work = 0x8041ea80, WorkBytes = 0x20,
                   Root = 0x8041f494, Format = 0x802c3b68,
                   FileAllocate = 0x8018a61c, FileFree = 0x8018a260;
struct Guard {
  bool &flag;
  explicit Guard(bool &f) : flag(f) { flag = true; }
  ~Guard() { flag = false; }
};
} // namespace
std::unique_ptr<NativeLogoStartup>
NativeLogoStartup::bind(std::shared_ptr<NativeUiStartup> ui, const char *root) {
  if (!ui || !root || !ui->current() || !ui->fieldProcess())
    return {};
  auto animation = ui->fieldProcess();
  auto &vm = animation->process().vm();
  if (!animation->files()->aliases(vm.memory) ||
      !vm.memory.contiguousRam(Root, 4) ||
      !animation->files()->contains("./mariost.tpl", 4))
    return {};
  EventScriptAssets image;
  if (!image.load((std::string(root) + "/logo-startup/memory.pmevt").c_str()) ||
      image.segments().size() != std::size(Spans))
    return {};
  for (unsigned i = 0; i < std::size(Spans); ++i) {
    const auto s = Spans[i];
    const auto b = image.segments()[i];
    if (b.address != s.address || b.size != s.bytes ||
        !vm.memory.unmappedRange(s.address, s.bytes))
      return {};
    uint64_t h = 14695981039346656037ULL;
    for (uint32_t k = 0; k < s.bytes; ++k) {
      uint8_t v;
      if (!image.readByte(s.address + k, v))
        return {};
      h = (h ^ v) * 1099511628211ULL;
    }
    if (h != s.hash)
      return {};
  }
  if (!vm.memory.importScripts(image))
    return {};
  auto n = std::unique_ptr<NativeLogoStartup>(new NativeLogoStartup);
  n->ui_ = std::move(ui);
  n->animation_ = std::move(animation);
  n->identity_ = std::make_unique<EventAddressSpace>(
      vm.entryAddresses, vm.contextAddresses, vm.runtimeAddresses,
      vm.gpAddresses);
  for (const auto s : Spans)
    if (!vm.memory.shareRamTo(*n->identity_, s.address, s.bytes))
      return {};
  // Borrow the existing source DVD-root backing without importing or resetting.
  if (!vm.memory.shareRamTo(*n->identity_, Root, 4))
    return {};
  return n;
}
bool NativeLogoStartup::current() const {
  if (!ui_ || !ui_->current() || ui_->fieldProcess() != animation_ ||
      !identity_)
    return false;
  auto &m = animation_->process().vm().memory;
  uint32_t work;
  if (!animation_->ready() || !animation_->files()->aliases(m) ||
      !m.readWord(WorkGlobal, work) || work != Work)
    return false;
  for (const auto s : Spans)
    if (!m.sharesRamWith(*identity_, s.address, s.bytes))
      return false;
  if (!m.sharesRamWith(*identity_, Root, 4))
    return false;
  return !children_.lifetime ||
         (children_.current && children_.current(children_.user));
}
bool NativeLogoStartup::configureChildren(NativeLogoChildren children) {
  if (invoking_ || !current() || children_.lifetime || !children.lifetime ||
      !children.current || !children.call || !children.current(children.user))
    return false;
  children_ = std::move(children);
  return true;
}
BattleSequenceEntryServices NativeLogoStartup::services() {
  BattleSequenceEntryServices s;
  s.user = this;
  s.word = [](void *p, uint32_t a) -> std::optional<uint32_t> {
    auto &n = *static_cast<NativeLogoStartup *>(p);
    uint32_t v;
    if (!n.current() || !n.animation_->process().vm().memory.readWord(a, v))
      return {};
    return v;
  };
  s.store = [](void *p, uint32_t a, uint32_t v) {
    auto &n = *static_cast<NativeLogoStartup *>(p);
    return n.current() && n.animation_->process().vm().memory.writeWord(a, v);
  };
  s.call = [](void *p, BattleSequenceCall &q) {
    auto &n = *static_cast<NativeLogoStartup *>(p);
    return n.current() && n.child(q);
  };
  return s;
}
bool NativeLogoStartup::child(BattleSequenceCall &q) {
  auto &vm = animation_->process().vm();
  auto files = animation_->files();
  switch (q.function) {
  case 0x800050b4:
    if (q.args[0] != Work || q.args[1] || q.args[2] != WorkBytes)
      return false;
    eventFillMemory(Work, 0, WorkBytes, vm.memoryBinding.services());
    q.result = Work;
    return true;
  case 0x80007388:
    q.result = sourceMarioDvdRoot(OriginalMarioDvdSourceR2);
    return true;
  case FileAllocate: {
    if (q.args[0] != 4 || q.args[1] != Format ||
        q.args[2] != sourceMarioDvdRoot(OriginalMarioDvdSourceR2))
      return false;
    if (!formatted_) {
      const char expected[] = "%s/mariost.tpl";
      for (unsigned i = 0; i < sizeof(expected); ++i) {
        uint8_t b;
        if (!vm.memory.readByte(Format + i, b) || b != uint8_t(expected[i]))
          return false;
      }
      std::string root;
      for (unsigned i = 0;; ++i) {
        uint8_t b;
        if (i >= 128 || uint64_t(q.args[2]) + i > UINT32_MAX ||
            !vm.memory.readByte(q.args[2] + i, b))
          return false;
        if (!b)
          break;
        root += char(b);
      }
      filename_ = root + "/mariost.tpl";
      if (filename_ != "./mariost.tpl" || !files->publishFilename(filename_))
        return false;
      formatted_ = true;
    }
    const auto result = files->acquire(filename_, 4, true);
    if (!result)
      return false;
    handle_ = q.result = *result;
    formatted_ = false;
    return true;
  }
  case FileFree:
    // Never manufacture a null release to close seq_logoExit. The actual work
    // load must supply the handle acquired by this retained Logo invocation.
    return handle_ && q.args[0] == handle_ && files->release(q.args[0]);
  default: {
    const bool done = children_.call && children_.call(children_.user, q);
    // A terminal child can retire its provider with no later source load.
    // Keep the cursor held until its completed child is still admitted.
    return done && current();
  }
  }
}
bool NativeLogoStartup::initialize() {
  if (invoking_ || !current() || exit_.active || exit_.complete ||
      animation_->entryPending() || animation_->producerPending())
    return false;
  Guard guard(invoking_);
  return initializeSourceLogo(services(), init_);
}
bool NativeLogoStartup::exit() {
  if (invoking_ || !current() || !init_.complete ||
      animation_->producerPending())
    return false;
  Guard guard(invoking_);
  return exitSourceLogo(services(), exit_);
}
} // namespace ttyd
