#include "native_mario_pose_startup.hpp"
namespace ttyd {
namespace {
constexpr uint32_t MarioGlobal = 0x8041e900, MarioBytes = 0x2f8,
                   ResourceTable = 0x80415eb0, ResourceTableBytes = 12,
                   ResourceNames = 0x802c18a0, ResourceNameBytes = 28,
                   PoseEntry = 0x8004176c;
struct Guard {
  bool &flag;
  explicit Guard(bool &f) : flag(f) { flag = true; }
  ~Guard() { flag = false; }
};
} // namespace
bool NativeMarioPoseStartup::pin(uint32_t address, uint32_t bytes) {
  for (const auto &span : spans_)
    if (span.first == address && span.second == bytes)
      return true;
  auto &m = animation_->process().vm().memory;
  if (!m.shareRamTo(*identity_, address, bytes))
    return false;
  spans_.push_back({address, bytes});
  return true;
}
std::unique_ptr<NativeMarioPoseStartup> NativeMarioPoseStartup::bind(
    std::shared_ptr<NativeFieldAnimation> animation,
    std::array<std::shared_ptr<const BattlePoseResource>, 3> resources) {
  if (!animation || !animation->ready() || animation->entryPending() ||
      animation->producerPending())
    return {};
  const char *const names[] = {"a_mario", "a_mario_r", "e_mario"};
  for (unsigned i = 0; i < resources.size(); ++i)
    if (!resources[i] || resources[i]->modelName != names[i])
      return {};
  auto n = std::unique_ptr<NativeMarioPoseStartup>(new NativeMarioPoseStartup);
  n->animation_ = std::move(animation);
  n->resources_ = std::move(resources);
  auto &vm = n->animation_->process().vm();
  if (!vm.memory.readWord(MarioGlobal, n->mario_) || !n->mario_)
    return {};
  n->identity_ = std::make_unique<EventAddressSpace>(
      vm.entryAddresses, vm.contextAddresses, vm.runtimeAddresses,
      vm.gpAddresses);
  if (!n->pin(MarioGlobal, 4) || !n->pin(n->mario_, MarioBytes) ||
      !n->pin(ResourceTable, ResourceTableBytes) ||
      !n->pin(ResourceNames, ResourceNameBytes))
    return {};
  return n;
}
bool NativeMarioPoseStartup::current() const {
  if (!animation_ || !animation_->ready() || !identity_)
    return false;
  auto &m = animation_->process().vm().memory;
  if (!animation_->files()->aliases(m))
    return false;
  for (const auto &span : spans_)
    if (!m.sharesRamWith(*identity_, span.first, span.second))
      return false;
  // The global's VALUE may legitimately change between original selectors.
  // Before the first source capture it must still select the admitted owner.
  uint32_t actual = 0;
  return cursor_.phase || (m.readWord(MarioGlobal, actual) && actual == mario_);
}
BattleSequenceEntryServices NativeMarioPoseStartup::services() {
  BattleSequenceEntryServices s;
  s.user = this;
  s.word = [](void *p, uint32_t a) -> std::optional<uint32_t> {
    auto &n = *static_cast<NativeMarioPoseStartup *>(p);
    uint32_t v;
    if (!n.current() || !n.animation_->process().vm().memory.readWord(a, v))
      return {};
    return v;
  };
  s.byte = [](void *p, uint32_t a) -> std::optional<uint8_t> {
    auto &n = *static_cast<NativeMarioPoseStartup *>(p);
    uint8_t v;
    if (!n.current())
      return {};
    // Each later selector may refer to another real owner. Pin its whole record
    // before use, without resetting it or changing the initial store owner.
    if (!n.pin(a - 0x3d, MarioBytes) ||
        !n.animation_->process().vm().memory.readByte(a, v))
      return {};
    return v;
  };
  s.store = [](void *p, uint32_t a, uint32_t v) {
    auto &n = *static_cast<NativeMarioPoseStartup *>(p);
    return n.current() && n.animation_->process().vm().memory.writeWord(a, v);
  };
  s.call = [](void *p, BattleSequenceCall &q) {
    auto &n = *static_cast<NativeMarioPoseStartup *>(p);
    if (!n.current() || q.function != PoseEntry || q.args[1] != 2)
      return false;
    auto &m = n.animation_->process().vm().memory;
    std::shared_ptr<const BattlePoseResource> resource;
    for (const auto &r : n.resources_) {
      bool match = true;
      for (unsigned i = 0; i <= r->modelName.size(); ++i) {
        uint8_t b;
        if (uint64_t(q.args[0]) + i > UINT32_MAX ||
            !m.readByte(q.args[0] + i, b) ||
            b != (i < r->modelName.size() ? uint8_t(r->modelName[i]) : 0)) {
          match = false;
          break;
        }
      }
      if (match) {
        resource = r;
        break;
      }
    }
    if (!resource)
      return false;
    BattlePoseClock clock;
    if (!n.animation_->clock(clock))
      return false;
    const auto result = n.animation_->pool()->enterSourceOriginal(
        std::move(resource), 2, clock, q.args[0]);
    if (!result)
      return false;
    q.result = uint32_t(*result);
    return true;
  };
  return s;
}
bool NativeMarioPoseStartup::resume() {
  if (invoking_ || !current() || animation_->producerPending())
    return false;
  Guard guard(invoking_);
  return initializeSourceMarioPoses(services(), cursor_);
}
} // namespace ttyd
