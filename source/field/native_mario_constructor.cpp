#include "native_mario_constructor.hpp"
#include "native_mario_pose_startup.hpp"
#include "animation_group_request_binding.hpp"
#include "event_heap_allocation.hpp"
#include "event_memory_opcodes.hpp"
#include "message_assets.hpp"
#include <algorithm>
#include <cstring>
namespace ttyd {
namespace {
using namespace SourceMarioConstructor;
struct Span {
  uint32_t address, bytes;
  uint64_t hash;
};
constexpr Span Spans[] = {
    {0x802c18a0, 136, 0x26941cb099dbb89dULL},
    {0x802c1958, 24, 0xea178c6284aa4fa4ULL},
    {0x802c1b24, 8, 0x1d3dad693f156dbdULL},
    {0x802c1b34, 8, 0x149432693a2d6f12ULL},
    {0x802cc728, 12, 0xdf461e3750b6909eULL},
    {0x802fa748, 24, 0x81d23fd7003c2305ULL},
    {0x80309748, 96, 0x3e4a0a5c6d24f891ULL},
    {0x80310030, 12, 0x9ffef991fab8f8daULL},
    {0x803d9848, 96, 0x0243cfa845185aa5ULL},
    {0x803dc228, 104, 0x8d0350be04626145ULL},
    {0x80415eb0, 48, 0xaf41c4d8fa6ce1ecULL},
    {0x8041e8f0, 4, 0x4d25767f9dce13f5ULL},
    {0x8041e900, 4, 0x4d25767f9dce13f5ULL},
    {0x8041ead8, 8, 0xa8c7f832281a39c5ULL},
    {0x8041eb00, 4, 0x4d25767f9dce13f5ULL},
    {0x8041fe88, 136, 0xc42b07891ea19beaULL},
    {0x8041ff90, 32, 0xa625484dd5515429ULL},
    {0x80420004, 8, 0x33a1550bf4e6c02aULL},
    {0x80420088, 4, 0x41e8ec89c40850c4ULL},
    {0x804200b8, 4, 0xc138c894dcbd4d85ULL},
    {0x804200c0, 4, 0xf3b794a61288a15bULL},
    {0x80420a74, 4, 0xc21d92265d7e4f3aULL},
    {0x8042187c, 4, 0x4d25767f9dce13f5ULL},
    {0x80424aa4, 4, 0x4d25767f9dce13f5ULL},
    {0x80424adc, 16, 0xc01ef4a5cd38ada9ULL},
};
// Disjoint native private frame/name tokens, not original heap placement.
constexpr uint32_t Stack = 0x8d080000, StackBytes = 32 * 1024, StackGuard = 32,
                   Name = Stack + StackBytes, NameBytes = 32;
// Original marioStInit publishes1 for the US message/resource domain.
constexpr uint32_t UsMessageLanguage = 1;
constexpr uint32_t Allocate = 0x800300f0, Memset = 0x800050b4,
                   GroupRequest = 0x80039d0c, MessageSearch = 0x80081b8c,
                   StringCopy = 0x8026d5fc, StringCompare = 0x8026d464;
bool coldBss(uint32_t a) {
  return a == MarioGlobal || a == PouchGlobal || a == BlurIndex ||
         a == BlurPositions || a == 0x803dc228 || a == 0x8041ead8;
}
struct Guard {
  bool &flag;
  explicit Guard(bool &f) : flag(f) { flag = true; }
  ~Guard() { flag = false; }
};
} // namespace
struct NativeMarioConstructor::State {
  std::shared_ptr<NativeUiStartup> ui;
  std::shared_ptr<NativeFieldAnimation> animation;
  std::unique_ptr<EventAddressSpace> identity;
  std::vector<std::pair<uint32_t, uint32_t>> backing;
  std::unique_ptr<AnimationGroupRequestBinding> groups;
  FieldMapSourceCursor cursor;
  std::unique_ptr<NativeMarioPoseStartup> poses;
  uint32_t mario = 0, pouch = 0;
  bool marioPublished = false, pouchPublished = false, initialized = false,
       invalid = false;
  EventNativeRuntime &vm() const { return animation->process().vm(); }
  bool pin(uint32_t address, uint32_t bytes) {
    // Shared boot prefixes and our suffixes remain separate actual allocations.
    // Pin each real contiguous backing, without remapping an existing prefix.
    while (bytes) {
      uint32_t run = bytes;
      if (!vm().memory.contiguousRam(address, run)) {
        if (!vm().memory.contiguousRam(address, 1)) return false;
        // Contiguous-prefix admission is monotone at a fixed address. Find
        // the actual fragment boundary without probing every stack byte.
        uint32_t first = 1, last = bytes;
        while (first < last) {
          const auto middle = first + (last - first + 1) / 2;
          if (vm().memory.contiguousRam(address, middle)) first = middle;
          else last = middle - 1;
        }
        run = first;
      }
      if (!vm().memory.shareRamTo(*identity, address, run))
        return false;
      backing.push_back({address, run});
      address += run;
      bytes -= run;
    }
    return true;
  }
  bool current() const {
    if (invalid || !ui || !ui->current() || ui->fieldProcess() != animation ||
        !animation->ready() || !identity ||
        !animation->files()->aliases(vm().memory))
      return false;
    for (const auto s : backing)
      if (!vm().memory.sharesRamWith(*identity, s.first, s.second))
        return false;
    uint32_t actual;
    if (marioPublished &&
        (!vm().memory.readWord(MarioGlobal, actual) || actual != mario))
      return false;
    if (pouchPublished &&
        (!vm().memory.readWord(PouchGlobal, actual) || actual != pouch))
      return false;
    return true;
  }
  bool install(const char *root) {
    EventScriptAssets image;
    if (!image.load(
            (std::string(root) + "/mario-constructor/memory.pmevt").c_str()) ||
        image.segments().size() != std::size(Spans))
      return false;
    struct Fragment {
      uint32_t address;
      std::vector<uint8_t> bytes;
    };
    std::vector<Fragment> missing;
    for (unsigned n = 0; n < std::size(Spans); ++n) {
      const auto s = Spans[n];
      const auto b = image.segments()[n];
      if (b.address != s.address || b.size != s.bytes)
        return false;
      uint64_t hash = 14695981039346656037ULL;
      for (uint32_t i = 0; i < s.bytes; ++i) {
        uint8_t expected, actual;
        if (!image.readByte(s.address + i, expected))
          return false;
        hash = (hash ^ expected) * 1099511628211ULL;
        if (vm().memory.readByte(s.address + i, actual)) {
          if (coldBss(s.address) || actual != expected)
            return false;
        } else {
          if (!vm().memory.unmappedRange(s.address + i, 1))
            return false;
          if (missing.empty() ||
              uint64_t(missing.back().address) + missing.back().bytes.size() !=
                  s.address + i)
            missing.push_back({s.address + i, {}});
          missing.back().bytes.push_back(expected);
        }
      }
      if (hash != s.hash)
        return false;
    }
    if (!vm().memory.unmappedRange(Stack, StackBytes + NameBytes))
      return false;
    // Preserve only the genuine US localized constructor-name source. The
    // whole message catalog/fallback engine is outside this finite adapter.
    MessageResource messages;
    if (!messages.load((std::string(root) + "/messages/global.txt").c_str()))
      return false;
    const char *original = messages.find("name_party3");
    if (!original || std::strlen(original) >= 0x14)
      return false;
    std::vector<uint8_t> name(NameBytes);
    std::memcpy(name.data(), original, std::strlen(original) + 1);
    for (auto &fragment : missing)
      if (!vm().memory.registerRam(fragment.address, std::move(fragment.bytes)))
        return false;
    if (!vm().memory.registerRam(Stack, std::vector<uint8_t>(StackBytes)) ||
        !vm().memory.registerRam(Name, std::move(name)))
      return false;
    for (auto s : Spans)
      if (!pin(s.address, s.bytes))
        return false;
    if (!pin(Stack, StackBytes) || !pin(Name, NameBytes))
      return false;
    groups =
        AnimationGroupRequestBinding::bind(vm().memory, animation->files());
    // Binding owns the genuine empty original literal if absent; pin its actual
    // backing too, rather than inferring its lifetime from file-cache aliases.
    return groups && pin(0x8041fb30, 4);
  }
  bool text(uint32_t a, std::string &out) const {
    out.clear();
    for (unsigned i = 0; i < 128; ++i) {
      uint8_t b;
      if (uint64_t(a) + i > UINT32_MAX || !vm().memory.readByte(a + i, b))
        return false;
      if (!b)
        return true;
      out += char(b);
    }
    return false;
  }
  bool allocate(FieldMapSourceCall &q) {
    const auto &a = q.arguments;
    if (a[0] || (a[1] != MarioBytes && a[1] != PouchBytes))
      return false;
    auto &owned = a[1] == MarioBytes ? mario : pouch;
    if (owned)
      return false;
    const auto address =
        eventHeapAllocate(0, a[1], vm().memoryBinding.services());
    if (!address) {
      q.result = 0;
      return true;
    }
    // Isolate payload backing before source publication. Later heap extraction
    // cannot move this live record; guest address and SDK headers stay genuine.
    std::vector<uint8_t> bytes;
    if (!vm().memory.extractRam(address, a[1], bytes) ||
        !vm().memory.registerRam(address, std::move(bytes)) ||
        !pin(address, a[1])) {
      invalid = true;
      return false;
    }
    owned = address;
    q.result = address;
    return true;
  }
  bool child(FieldMapSourceCall &q) {
    if (!current())
      return false;
    const auto &a = q.arguments;
    auto &m = vm().memory;
    switch (q.function) {
    case Allocate:
      return allocate(q) && current();
    case Memset:
      if (!a[2] || !m.contiguousRam(a[0], a[2]))
        return false;
      q.result = eventFillMemory(a[0], int32_t(a[1]), a[2],
                                 vm().memoryBinding.services());
      return current();
    case GroupRequest: {
      if (a[1] != 2 || a[2])
        return false;
      const auto result = groups->advance(a[0], a[1], a[2]);
      if (!result)
        return false;
      q.result = *result;
      return current();
    }
    case StringCompare: {
      std::string left, right;
      if (!text(a[0], left) || !text(a[1], right))
        return false;
      const auto count = std::min(left.size(), right.size());
      int32_t value = 0;
      for (size_t i = 0; i < count; ++i)
        if (uint8_t(left[i]) != uint8_t(right[i])) {
          value = uint8_t(left[i]) < uint8_t(right[i]) ? -1 : 1;
          break;
        }
      if (!value && left.size() != right.size())
        value = left.size() < right.size() ? -1 : 1;
      q.result = uint32_t(value);
      return current();
    }
    case MessageSearch: {
      std::string key;
      if (ui->language() != UsMessageLanguage || !text(a[0], key) ||
          key != "name_party3")
        return false;
      q.result = Name;
      return current();
    }
    case StringCopy: {
      std::string source;
      if (!pouchPublished || a[0] != pouch + 0x5c0 || a[1] != Name ||
          !text(a[1], source) || source.size() >= 0x14 ||
          !m.contiguousRam(a[0], uint32_t(source.size() + 1)))
        return false;
      // Bounded native C-runtime adapter, ordered bytes through NUL; no
      // optimized GameCube word-overread or whole message catalog receipt is
      // implied.
      for (unsigned i = 0; i <= source.size(); ++i)
        if (!m.writeByte(a[0] + i, i < source.size() ? uint8_t(source[i]) : 0))
          return false;
      q.result = a[0];
      return current();
    }
    default:
      return false; // Actual reached unknown motion/audio providers hold.
    }
  }
  FieldMapSourceServices services() {
    FieldMapSourceServices s;
    s.user = this;
    s.word = [](void *p, uint32_t a, uint32_t &v) {
      auto &n = *static_cast<State *>(p);
      return n.current() && n.vm().memory.readWord(a, v);
    };
    s.byte = [](void *p, uint32_t a, uint8_t &v) {
      auto &n = *static_cast<State *>(p);
      return n.current() && n.vm().memory.readByte(a, v);
    };
    s.half = [](void *p, uint32_t a, uint16_t &v) {
      auto &n = *static_cast<State *>(p);
      return n.current() && n.vm().memory.readHalf(a, v);
    };
    s.store = [](void *p, uint32_t a, uint32_t v) {
      auto &n = *static_cast<State *>(p);
      if (!n.current() || !n.vm().memory.writeWord(a, v))
        return false;
      if (a == MarioGlobal && v == n.mario)
        n.marioPublished = true;
      if (a == PouchGlobal && v == n.pouch)
        n.pouchPublished = true;
      return true;
    };
    s.storeHalf = [](void *p, uint32_t a, uint16_t v) {
      auto &n = *static_cast<State *>(p);
      return n.current() && n.vm().memory.writeHalf(a, v);
    };
    s.storeByte = [](void *p, uint32_t a, uint8_t v) {
      auto &n = *static_cast<State *>(p);
      return n.current() && n.vm().memory.writeByte(a, v);
    };
    s.call = [](void *p, FieldMapSourceCall &q) {
      return static_cast<State *>(p)->child(q);
    };
    return s;
  }
};
NativeMarioConstructor::NativeMarioConstructor() = default;
NativeMarioConstructor::~NativeMarioConstructor() = default;
std::shared_ptr<NativeMarioConstructor>
NativeMarioConstructor::create(std::shared_ptr<NativeUiStartup> ui,
                               const char *root) {
  if (!ui || !root || !ui->current() || !ui->fieldProcess() ||
      !ui->fieldProcess()->ready())
    return {};
  auto n = std::shared_ptr<NativeMarioConstructor>(new NativeMarioConstructor);
  n->state_ = std::make_unique<State>();
  auto &s = *n->state_;
  s.ui = std::move(ui);
  s.animation = s.ui->fieldProcess();
  auto &vm = s.vm();
  s.identity = std::make_unique<EventAddressSpace>(
      vm.entryAddresses, vm.contextAddresses, vm.runtimeAddresses,
      vm.gpAddresses);
  if (!s.install(root) || !s.current())
    return {};
  return n;
}
bool NativeMarioConstructor::current() const {
  return state_ && state_->current();
}
bool NativeMarioConstructor::ready() const {
  return current() && state_->initialized;
}
bool NativeMarioConstructor::busy() const {
  return state_ && (state_->cursor.active || (state_->poses && state_->poses->pending()));
}
uint32_t NativeMarioConstructor::pending() const {
  return state_ ? (state_->poses && state_->poses->pending() ? state_->poses->pendingAddress() : state_->cursor.pc) : 0;
}
uint32_t NativeMarioConstructor::marioAddress() const {
  return current() ? state_->mario : 0;
}
uint32_t NativeMarioConstructor::pouchAddress() const {
  return current() ? state_->pouch : 0;
}
std::shared_ptr<NativeUiStartup> NativeMarioConstructor::ui() const {
  return state_ ? state_->ui : nullptr;
}
bool NativeMarioConstructor::initialize(unsigned visits) {
  if (invoking_ || !current() || state_->animation->entryPending() ||
      state_->animation->producerPending())
    return false;
  auto &s = *state_;
  if (s.initialized)
    return true;
  Guard guard(invoking_);
  const auto result =
      resumeFieldMapSource(Init, {}, Stack + StackBytes - StackGuard,
                           s.services(), s.cursor, visits);
  if (!result || !s.current())
    return false;
  s.initialized = true;
  return true;
}
bool NativeMarioConstructor::initializePoses(
    std::array<std::shared_ptr<const BattlePoseResource>, 3> resources) {
  if (invoking_ || !ready()) return false;
  Guard guard(invoking_);
  if (!state_->poses)
    state_->poses = NativeMarioPoseStartup::bind(state_->animation, std::move(resources));
  return state_->poses && state_->poses->resume();
}
bool NativeMarioConstructor::posesReady() const {
  return ready() && state_->poses && state_->poses->ready();
}
std::array<int32_t, 3> NativeMarioConstructor::poseHandles() const {
  std::array<int32_t, 3> result{{-1, -1, -1}};
  if (!posesReady()) return result;
  for (unsigned i = 0; i < result.size(); ++i) {
    uint32_t raw;
    if (!state_->vm().memory.readWord(state_->mario + MarioPoseHandleOffset + sizeof(uint32_t) * i, raw))
      return {{-1, -1, -1}};
    result[i] = raw <= INT32_MAX ? int32_t(raw) : int32_t(int64_t(raw) - 0x100000000LL);
  }
  return result;
}
bool NativeMarioConstructor::adoptPouchSnapshot(const ProloguePouch &snapshot) {
  if (invoking_ || !ready() || busy() || state_->animation->entryPending() ||
      state_->animation->producerPending())
    return false;
  Guard guard(invoking_);
  auto &m = state_->vm().memory;
  for (unsigned i = 0; i < snapshot.bytes.size(); ++i)
    if (!m.writeByte(state_->pouch + i, snapshot.bytes[i]))
      return false;
  return current();
}
bool NativeMarioConstructor::capturePouchSnapshot(
    ProloguePouch &snapshot) const {
  if (!ready() || busy())
    return false;
  ProloguePouch candidate;
  for (unsigned i = 0; i < candidate.bytes.size(); ++i)
    if (!state_->vm().memory.readByte(state_->pouch + i, candidate.bytes[i]))
      return false;
  snapshot = candidate;
  return true;
}
} // namespace ttyd
