#include "battle_class_menu_messages.hpp"
#include "event_address_space.hpp"
#include <cstring>
#include <limits>
#include <map>
namespace ttyd {
const std::array<BattleClassMenuMessageKey, 7> &
originalBattleClassMenuMessageKeys() {
  static constexpr std::array<BattleClassMenuMessageKey, 7> keys{
      {{0x802f0018, "btl_cmd_act_jump"},
       {0x802f0044, "btl_cmd_act_hammer"},
       {0x802eff0c, "btl_cmd_act_item"},
       {0x802f0074, "btl_cmd_act_operation"},
       {0x802f0058, "btl_cmd_act_super_action"},
       {0x802f002c, "btl_cmd_act_tech_party"},
       {0x802f008c, "btl_cmd_act_lecture_end"}}};
  return keys;
}
std::shared_ptr<BattleClassMenuMessages>
BattleClassMenuMessages::map(EventAddressSpace &memory, uint32_t base,
                             std::shared_ptr<const MessageAssets> resources) {
  if (!resources || !base)
    return {};
  auto owner =
      std::shared_ptr<BattleClassMenuMessages>(new BattleClassMenuMessages);
  owner->memory_ = &memory;
  owner->base_ = base;
  owner->resources_ = std::move(resources);
  std::vector<uint8_t> bytes;
  // Preserve original pointer alias identity; identical text at distinct
  // original entries remains distinct. No native label or source-key fallback
  // is supplied.
  std::map<const char *, uint32_t> aliases;
  const auto &keys = originalBattleClassMenuMessageKeys();
  for (size_t i = 0; i < keys.size(); ++i) {
    const auto *text = owner->resources_->find(keys[i].id);
    if (!text)
      return {};
    const auto found = aliases.find(text);
    if (found != aliases.end()) {
      owner->pointers_[i] = found->second;
      continue;
    }
    const auto length = std::strlen(text) + 1;
    if (length > std::numeric_limits<uint32_t>::max() ||
        uint64_t(base) + bytes.size() + length > 0x100000000ULL)
      return {};
    const auto pointer = base + uint32_t(bytes.size());
    owner->pointers_[i] = pointer;
    aliases.emplace(text, pointer);
    const auto *first = reinterpret_cast<const uint8_t *>(text);
    bytes.insert(bytes.end(), first, first + length);
  }
  if (bytes.empty() || bytes.size() > std::numeric_limits<uint32_t>::max())
    return {};
  owner->size_ = uint32_t(bytes.size());
  // Actual facade owns copied bytes after registration, independently of this
  // wrapper/resource metadata lifetime. No destructor unregisters guest
  // storage.
  if (!memory.registerRam(base, std::move(bytes)))
    return {};
  return owner;
}
std::optional<uint32_t>
BattleClassMenuMessages::lookup(const EventAddressSpace &memory,
                                uint32_t key) const {
  if (&memory != memory_)
    return {};
  const auto &keys = originalBattleClassMenuMessageKeys();
  for (size_t i = 0; i < keys.size(); ++i)
    if (keys[i].source == key)
      return pointers_[i];
  return {};
}
std::shared_ptr<const BattleActionClassMessageBinding>
BattleClassMenuMessages::binding(const EventAddressSpace &memory) const {
  if (&memory != memory_)
    return {};
  auto result = std::make_shared<BattleActionClassMessageBinding>();
  result->allocation = shared_from_this();
  result->user = const_cast<BattleClassMenuMessages *>(this);
  result->lookup = [](void *p, uint32_t key) -> std::optional<uint32_t> {
    const auto &owner = *static_cast<BattleClassMenuMessages *>(p);
    return owner.lookup(*owner.memory_, key);
  };
  return result;
}
} // namespace ttyd
