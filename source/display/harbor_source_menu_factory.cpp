#include "harbor_source_menu_factory.hpp"
#include "battle_event_callbacks.hpp"
#include <cstdlib>
#include <limits>
#include <map>
namespace ttyd {
namespace {
constexpr uint32_t BattleGlobal = 0x8041eb90, UiOffset = 0x1c78,
                   CommonOffset = 0x163f8;
constexpr uint32_t ClassCallback = 0x8012210c, WeaponCallback = 0x80121ce8;
constexpr uint32_t ClassViewBase = 0x60010000, WeaponViewBase = 0x60014000;
constexpr uint32_t MessageCacheBase = 0x85b00000,
                   MessageCacheCapacity = 4u * 1024u * 1024u;
constexpr size_t MaximumKeyBytes = 4096, MaximumLabelBytes = 16384;
struct Guard {
  bool &b;
  Guard(bool &v) : b(v) { b = true; }
  ~Guard() { b = false; }
};
} // namespace
struct HarborSourceMenuFactory::State {
  HarborSourceMenuFactoryBinding binding;
  mutable bool checking = false, invalid = false;
  mutable std::map<uint32_t, std::string> labels;
  mutable std::shared_ptr<BattleVmMessageLookup> lookup;
  std::weak_ptr<BattleWeaponSourceMemory> memory;
  std::shared_ptr<EventGpLanguageAlias> language;
  bool current() const {
    if (checking || invalid)
      return false;
    Guard g(checking);
    const auto runtime = binding.runtime.lock();
    if (!runtime) {
      invalid = true;
      return false;
    }
    uint32_t b = 0;
    if (!binding.current() || !binding.vm->memory.readWord(BattleGlobal, b) ||
        b != binding.battle) {
      invalid = true;
      return false;
    }
    const auto display = binding.display.lock();
    return display && display->runtimeIs(runtime) && display->current();
  }
  bool ui(uint32_t w, const std::shared_ptr<const uint8_t> &token) const {
    if (!current())
      return false;
    const auto runtime = binding.runtime.lock();
    if (!runtime)
      return false;
    uint32_t actual = 0;
    return binding.uiLifetime(w) == token &&
           binding.vm->memory.readWord(binding.battle + UiOffset, actual) &&
           actual == w;
  }
  std::optional<uint32_t> word(uint32_t a) const {
    if (!current())
      return {};
    const auto runtime = binding.runtime.lock();
    if (!runtime)
      return {};
    uint32_t v = 0;
    if (binding.vm->memory.readWord(a, v))
      return v;
    if (!binding.vm->memory.unmappedRange(a, 4))
      return {};
    return binding.vm->gpAddresses.readWord(a, v) ? std::optional<uint32_t>(v)
                                                  : std::nullopt;
  }
  bool resource(uint32_t descriptor, uint32_t cell, uint32_t palette) const {
    if (!current() || !binding.files->ready())
      return false;
    const auto runtime = binding.runtime.lock();
    const auto files = binding.files->files();
    uint8_t state = 0, type = 0;
    uint16_t references = 0;
    if (!runtime || !files || !files->aliases(binding.vm->memory) ||
        !files->ownsPayload(palette, 12) ||
        !binding.vm->memory.readByte(descriptor, state) || state != 1 ||
        !binding.vm->memory.readByte(descriptor + 1, type) || type != 4 ||
        !binding.vm->memory.readHalf(descriptor + 2, references) || !references)
      return false;
    const auto d = word(binding.battle + CommonOffset),
               c = word(descriptor + 0xa0), p = word(cell);
    return d && *d == descriptor && c && *c == cell && p && *p == palette;
  }
};
std::shared_ptr<HarborSourceMenuFactory>
HarborSourceMenuFactory::create(HarborSourceMenuFactoryBinding b) {
  if (!b.vm || !b.battle || b.runtime.expired() || b.display.expired() ||
      !b.files || !b.messages || !b.current || !b.uiLifetime || !b.random)
    return {};
  auto out =
      std::shared_ptr<HarborSourceMenuFactory>(new HarborSourceMenuFactory);
  out->state_ = std::make_shared<State>();
  out->state_->binding = std::move(b);
  return out;
}
bool HarborSourceMenuFactory::resumeDisplay() const {
  const auto s=state_;const auto runtime=s?s->binding.runtime.lock():nullptr;
  const auto display=s?s->binding.display.lock():nullptr;
  return runtime&&s->current()&&display&&display->resume();
}
bool HarborSourceMenuFactory::current() const {
  return state_ && state_->current();
}
std::optional<TrioClassMenuFrameBinding> HarborSourceMenuFactory::classBinding(
    const BattleActDrawServices &linked) const {
  const auto s = state_;
  if (!s || !s->current())
    return {};
  const auto runtime = s->binding.runtime.lock();
  if (!runtime)
    return {};
  const auto descriptor = s->word(s->binding.battle + CommonOffset);
  if (!descriptor || !*descriptor)
    return {};
  const auto cell = s->word(*descriptor + 0xa0);
  if (!cell || !*cell)
    return {};
  const auto palette = s->word(*cell);
  if (!palette || !*palette || !s->resource(*descriptor, *cell, *palette))
    return {};
  const auto value = s->word(s->binding.battle + UiOffset);
  if (!value || !*value)
    return {};
  const auto w = *value;
  const auto token = s->binding.uiLifetime(w);
  const auto display = s->binding.display.lock();
  if (!token || !display)
    return {};
  const uint64_t generation =
      uint64_t(reinterpret_cast<uintptr_t>(token.get()));
  TrioClassMenuFrameBinding out;
  out.native.allocationOwner = token;
  out.native.allocation = w;
  out.native.generation = generation;
  out.native.cameras = display->cameraManager();
  out.native.actors = nativeBattleActorServices(*s->binding.vm);
  out.native.addresses = {ClassViewBase, [s](uint32_t a, uint32_t n) {
                            const auto runtime = s->binding.runtime.lock();
                            return runtime && s->current() &&
                                   s->binding.vm->memory.unmappedRange(a, n);
                          }};
  out.native.allocationCurrent = [s, w, token, d = *descriptor, c = *cell,
                                  p = *palette] {
    return s->ui(w, token) && s->resource(d, c, p);
  };
  const auto producerGeneration = display->generation();
  out.native.producerCurrent = [s, generation,
                                producerGeneration](BattleClassCameraEpoch e) {
    const auto display = s->binding.display.lock();
    return s->current() && display && e.generation == generation &&
           display->cameraMainCurrent({producerGeneration, e.pass});
  };
  out.display.draw = linked;
  out.display.draw.memory = out.native.actors;
  out.display.user = s.get();
  out.display.cursorLookup = [](void *p, uint32_t menu, uint32_t type,
                                std::optional<uint32_t> &output) {
    auto &s = *static_cast<State *>(p);
    const auto runtime = s.binding.runtime.lock();
    if (!runtime || !s.current())
      return false;
    uint32_t value = 0;
    if (findPackedCommandCursor(
            menu, type, value,
            nativeBattlePackedSelectionServices(*s.binding.vm)))
      output = value;
    return s.current();
  };
  out.label = [s](uint32_t address) -> const char * {
    const auto runtime = s->binding.runtime.lock();
    if (!runtime || !s->current())
      return nullptr;
    std::string value;
    for (size_t i = 0; i < MaximumLabelBytes; ++i) {
      if (uint64_t(address) + i > UINT32_MAX)
        return nullptr;
      uint8_t byte;
      if (!s->binding.vm->memory.readByte(address + uint32_t(i), byte))
        return nullptr;
      if (!byte) {
        auto [at, inserted] = s->labels.emplace(address, value);
        return inserted || at->second == value ? at->second.c_str() : nullptr;
      }
      value.push_back(char(byte));
    }
    return nullptr;
  };
  out.random = [s](uint32_t bound) {
    const auto runtime = s->binding.runtime.lock();
    if (!runtime || !s->current())
      std::abort();
    return s->binding.random(bound);
  };
  out.resourceWord = [s](uint32_t a) { return s->word(a); };
  return out;
}
std::optional<TrioWeaponMenuFrameBinding>
HarborSourceMenuFactory::weaponBinding(
    const BattleWeaponDrawServices &linked) const {
  const auto base = classBinding({});
  if (!base)
    return {};
  const auto s = state_;
  const auto d = s->word(s->binding.battle + CommonOffset);
  if (!d || !*d)
    return {};
  const auto c = s->word(*d + 0xa0);
  if (!c || !*c)
    return {};
  const auto p = s->word(*c);
  if (!p || !*p || !s->resource(*d, *c, *p))
    return {};
  TrioWeaponMenuFrameBinding out;
  out.native.allocationOwner = base->native.allocationOwner;
  out.native.sourceResourceOwner = s;
  out.native.allocation = base->native.allocation;
  out.native.battle = s->binding.battle;
  out.native.generation = base->native.generation;
  out.native.cameras = base->native.cameras;
  out.native.actors = base->native.actors;
  out.native.addresses = base->native.addresses;
  out.native.addresses.base = WeaponViewBase;
  out.native.allocationCurrent = base->native.allocationCurrent;
  out.native.producerCurrent = base->native.producerCurrent;
  out.native.sourceResourceCurrent = [s, d = *d, c = *c, p = *p] {
    return s->resource(d, c, p);
  };
  out.source = linked;
  out.source.math.memory = out.native.actors;
  out.random = base->random;
  out.resourceWord = base->resourceWord;
  const auto cursor = std::make_shared<BattleVmMessageLookupCursor>();
  out.messageLookup = [s, cursor](uint32_t key) -> std::optional<uint32_t> {
    const auto runtime = s->binding.runtime.lock();
    if (!runtime || !s->current())
      return {};
    if (!s->lookup) {
      const std::weak_ptr<State> weak = s;
      BattleVmMessageLookupBinding b;
      b.messages = s->binding.messages;
      b.vmLifetime = s->binding.runtime;
      b.addressSpaceIdentity = &s->binding.vm->memory;
      b.base = MessageCacheBase;
      b.capacity = MessageCacheCapacity;
      b.maximumKeyBytes = MaximumKeyBytes;
      b.existing = s->binding.existingMessages;
      b.current = [weak] {
        const auto s = weak.lock();
        return s && s->current();
      };
      b.byte = [weak](uint32_t a) -> std::optional<uint8_t> {
        const auto s = weak.lock();
        const auto runtime = s ? s->binding.runtime.lock() : nullptr;
        uint8_t v;
        return runtime && s->current() && s->binding.vm->memory.readByte(a, v)
                   ? std::optional<uint8_t>(v)
                   : std::nullopt;
      };
      b.unmapped = [weak](uint32_t a, uint64_t n) {
        const auto s = weak.lock();
        const auto runtime = s ? s->binding.runtime.lock() : nullptr;
        return runtime && s->current() &&
               s->binding.vm->memory.unmappedRange(a, n);
      };
      b.registerBytes = [weak](uint32_t a, std::vector<uint8_t> v) {
        const auto s = weak.lock();
        const auto runtime = s ? s->binding.runtime.lock() : nullptr;
        return runtime && s->current() &&
               s->binding.vm->memory.registerRam(a, std::move(v));
      };
      s->lookup = BattleVmMessageLookup::create(std::move(b));
      if (!s->lookup)
        return {};
    }
    return s->lookup->lookup(key, *cursor);
  };
  return out;
}
std::shared_ptr<BattleWeaponSourceMemory>
HarborSourceMenuFactory::weaponMemory(std::shared_ptr<NativeUiStartup> startup,
                                      const char *image) {
  const auto s = state_;
  if (!s || !s->current() || !startup || !startup->current() ||
      !startup->fontObject())
    return {};
  const auto runtime = s->binding.runtime.lock();
  if (!runtime)
    return {};
  if (const auto held = s->memory.lock())
    return held->startup() == startup && held->current() ? held : nullptr;
  const auto process = startup->fieldProcess();
  if (!process)
    return {};
  auto alias = s->language;
  if (alias) {
    if (!alias->sourceIs(process->process().vm().gpAddresses) ||
        !alias->current())
      return {};
  } else {
    uint32_t gp = 0;
    if (!process->process().vm().memory.readWord(0x80415fd0, gp))
      return {};
    alias = std::make_shared<EventGpLanguageAlias>(
        process->process().vm().gpAddresses, gp, startup->lifetime(),
        [startup] { return startup->current(); });
  }
  const std::weak_ptr<State> weak = s;
  auto memory = BattleWeaponSourceMemory::create(
      *s->binding.vm, startup, alias, runtime,
      [weak] {
        const auto s = weak.lock();
        return s && s->current();
      },
      image);
  if (memory) {
    s->language = std::move(alias);
    s->memory = memory;
  }
  return memory;
}
bool HarborSourceMenuFactory::resumeClass(
    const HarborSourceDisplayInvocation &q, BattleClassNativeFrame &frame,
    BattleClassNativeSubmission submission) const {
  const auto s = state_;
  const auto runtime = s ? s->binding.runtime.lock() : nullptr;
  const auto display = s ? s->binding.display.lock() : nullptr;
  return runtime && s->current() && display && display->acceptsInvocation(q) &&
         q.callback == ClassCallback && q.battle == s->binding.battle &&
         frame.resume(q.pass, q.main, q.draw, q.queue, q.camera, q.battle,
                      submission);
}
bool HarborSourceMenuFactory::resumeWeapon(
    const HarborSourceDisplayInvocation &q,
    BattleWeaponNativeFrame &frame) const {
  const auto s = state_;
  const auto runtime = s ? s->binding.runtime.lock() : nullptr;
  const auto display = s ? s->binding.display.lock() : nullptr;
  return runtime && s->current() && display && display->acceptsInvocation(q) &&
         q.callback == WeaponCallback && q.battle == s->binding.battle &&
         frame.resume(q.pass, q.main, q.draw, q.queue, q.camera, q.battle);
}
} // namespace ttyd
