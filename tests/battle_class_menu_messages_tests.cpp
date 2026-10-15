#include "battle_class_menu_messages.hpp"
#include "battle_event_callbacks.hpp"
#include "event_native_runtime.hpp"
#include <cassert>
#include <cstdio>
#include <cstring>
#include <string>
#include <unistd.h>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Base = 0x85a00000;
std::string temporary(const std::vector<uint8_t> &bytes) {
  char path[] = "/tmp/pm-class-messages-XXXXXX";
  const auto fd = mkstemp(path);
  assert(fd >= 0);
  FILE *file = fdopen(fd, "wb");
  assert(file);
  assert(std::fwrite(bytes.data(), 1, bytes.size(), file) == bytes.size());
  assert(std::fclose(file) == 0);
  return path;
}
void append(std::vector<uint8_t> &bytes, const char *text) {
  bytes.insert(bytes.end(), text, text + std::strlen(text) + 1);
}
std::shared_ptr<const MessageAssets>
fixture(bool controls = false, bool missing = false, bool ambiguous = false) {
  std::vector<uint8_t> global;
  const auto &keys = originalBattleClassMenuMessageKeys();
  for (size_t i = 0; i < keys.size(); ++i) {
    if (missing && i == 6)
      continue;
    append(global, keys[i].id);
    append(global, controls ? "<icon 334><col ff0000ff>\x82literal</col>"
                            : "Equal text");
  }
  global.push_back(0);
  auto path = temporary(global);
  auto resources = std::make_shared<MessageAssets>();
  assert(resources->loadGlobal(path.c_str()));
  assert(std::remove(path.c_str()) == 0);
  if (controls || ambiguous) {
    std::vector<uint8_t> room;
    append(room, keys[0].id);
    append(room, "<wait 17>Room bytes");
    if (ambiguous) {
      append(room, keys[0].id);
      append(room, "Ambiguous fixture");
    }
    room.push_back(0);
    path = temporary(room);
    assert(resources->loadRoom(path.c_str()));
    assert(std::remove(path.c_str()) == 0);
  }
  return resources;
}
void equalText(const EventAddressSpace &memory, uint32_t address,
               const char *text) {
  assert(text);
  for (size_t i = 0; i <= std::strlen(text); ++i) {
    uint8_t b = 0;
    assert(memory.readByte(address + uint32_t(i), b) && b == uint8_t(text[i]));
  }
}
void testResources(std::shared_ptr<const MessageAssets> resources) {
  EventNativeRuntime vm(0x81000000, {}), other(0x81000000, {});
  auto owner = BattleClassMenuMessages::map(vm.memory, Base, resources);
  assert(owner && owner->base() == Base && owner->size() > 0);
  assert(!BattleClassMenuMessages::map(vm.memory, Base, resources));
  assert(!BattleClassMenuMessages::map(vm.memory, Base + 1, resources));
  const auto &keys = originalBattleClassMenuMessageKeys();
  for (const auto &key : keys) {
    const auto address = owner->lookup(vm.memory, key.source);
    assert(address);
    equalText(vm.memory, *address, resources->find(key.id));
    assert(owner->lookup(vm.memory, key.source) == address);
    assert(!owner->lookup(other.memory, key.source));
  }
  assert(!owner->lookup(vm.memory, 0) && !owner->lookup(vm.memory, 0x802f0019));
  assert(!owner->binding(other.memory));
  // Identical contents at distinct original entries are not
  // content-deduplicated.
  assert(owner->lookup(vm.memory, keys[0].source) !=
         owner->lookup(vm.memory, keys[1].source));
  auto binding = owner->binding(vm.memory);
  assert(binding && binding->allocation && binding->lookup);
  for (const auto &key : keys)
    assert(binding->lookup(binding->user, key.source) ==
           owner->lookup(vm.memory, key.source));
  const auto saved = *owner->lookup(vm.memory, keys[0].source);
  const std::string text = resources->find(keys[0].id);
  std::weak_ptr<const MessageAssets> weak = resources;
  resources.reset();
  owner.reset();
  assert(!weak.expired());
  equalText(vm.memory, saved, text.c_str());
  assert(binding->lookup(binding->user, keys[0].source) == saved);
  binding.reset();
  assert(weak.expired());
  // Actual facade storage survives retirement of all wrapper/resource metadata.
  equalText(vm.memory, saved, text.c_str());
  assert(!vm.memory.unmappedRange(Base, 1));
}
void actualClassIntegration(const char *path) {
  auto resources = std::make_shared<MessageAssets>();
  assert(resources->loadGlobal(path));
  EventNativeRuntime vm(0x81000000, {});
  const uint32_t b = 0x10000, u = 0x40000, p = 0x50000, setup = 0x60000,
                 definition = 0x70000;
  for (const auto span :
       std::array<std::pair<uint32_t, uint32_t>, 7>{{{b, 0x19400},
                                                     {u, 0x400},
                                                     {p, 0x600},
                                                     {setup, 0x20},
                                                     {definition, 0x20},
                                                     {0x8041eb00, 4},
                                                     {0x8041eb90, 4}}})
    assert(
        vm.memory.registerRam(span.first, std::vector<uint8_t>(span.second)));
  auto put = [&](uint32_t a, uint32_t v) { assert(vm.memory.writeWord(a, v)); };
  put(0x8041eb90, b);
  put(0x8041eb00, p);
  put(b + 0x420, 1);
  put(b + 0x24, u);
  put(u + 8, 222);
  put(b + 0x2738, setup);
  put(setup + 12, definition);
  assert(vm.memory.writeByte(p + 0x98, 1) && vm.memory.writeByte(p + 0x99, 1) &&
         vm.memory.writeHalf(p + 0x8c, 1));
  auto owner = BattleClassMenuMessages::map(vm.memory, Base, resources);
  assert(owner);
  struct Context {
    EventNativeRuntime *vm;
    std::shared_ptr<const BattleActionClassMessageBinding> messages;
  } ctx{&vm, owner->binding(vm.memory)};
  BattleActionClassServices s;
  s.memory = nativeBattleActorServices(vm);
  s.user = &ctx;
  s.unit = [](void *p, uint32_t b, int32_t id) -> std::optional<uint32_t> {
    auto &c = *static_cast<Context *>(p);
    return battleRegistryUnit(b, id, nativeBattleActorServices(*c.vm));
  };
  s.pouch = [](void *p) -> std::optional<uint32_t> {
    uint32_t value = 0;
    assert(static_cast<Context *>(p)->vm->memory.readWord(0x8041eb00, value));
    return value;
  };
  s.message = [](void *p, uint32_t key) -> std::optional<uint32_t> {
    auto &c = *static_cast<Context *>(p);
    return c.messages->lookup(c.messages->user, key);
  };
  BattleActionClassTableCursor cursor;
  assert(resumePackedBattleActionClassTable(b, s, cursor) == 5);
  const auto &keys = originalBattleClassMenuMessageKeys();
  const std::array<unsigned, 5> indices{0, 1, 2, 4, 3};
  for (unsigned i = 0; i < 5; ++i) {
    uint32_t label = 0;
    assert(vm.memory.readWord(b + 0x171c + 20 * i + 16, label));
    assert(label == owner->lookup(vm.memory, keys[indices[i]].source));
    equalText(vm.memory, label, resources->find(keys[indices[i]].id));
  }
  // All seven actual original US labels, including the unused Party/Lecture
  // branches, are backed by the same immutable input and real VM allocation.
  for (const auto &key : keys)
    equalText(vm.memory, *owner->lookup(vm.memory, key.source),
              resources->find(key.id));
}
} // namespace
int main(int argc, char **argv) {
  testResources(fixture());
  testResources(fixture(true));
  EventNativeRuntime vm(0x81000000, {});
  auto missing = fixture(false, true), ambiguous = fixture(false, false, true);
  assert(!BattleClassMenuMessages::map(vm.memory, Base, missing));
  assert(vm.memory.unmappedRange(Base, 1024));
  assert(!BattleClassMenuMessages::map(vm.memory, Base, ambiguous));
  assert(vm.memory.unmappedRange(Base, 1024));
  assert(!BattleClassMenuMessages::map(vm.memory, 0, fixture()));
  assert(!BattleClassMenuMessages::map(vm.memory, 0xfffffff0, fixture()));
  assert(!BattleClassMenuMessages::map(vm.memory, Base, {}));
  actualClassIntegration(argc > 1 ? argv[1] : "romfs/messages/global.txt");
  return 0;
}
