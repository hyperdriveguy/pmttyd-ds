#pragma once
#include "battle_menu_resources.hpp"
#include "event_address_space.hpp"
#include "native_file_cache.hpp"
#include <array>
#include <memory>
#include <vector>
namespace ttyd {
// Common-menu facade over the shared process file owner. Animation consumers
// stage/acquire through files() before VM binding; this facade retains its
// original narrow menu path/type admission and shared source references.
class BattleMenuFileCache {
public:
  BattleMenuFileCache() = default;
  BattleMenuFileCache(const BattleMenuFileCache&) = delete;
  BattleMenuFileCache& operator=(const BattleMenuFileCache&) = delete;
  bool load(const char* originalTpl);
  bool bind(EventAddressSpace&); // Cache must outlive every bound address space.
  std::optional<uint32_t> acquire(const std::string&,uint32_t type);
  bool release(uint32_t entry);
  bool ready() const {return bool(files_);}
  std::shared_ptr<NativeFileCache> files() const {return files_;}
  // Original manager/global/root identities, not native payload storage.
  // The facade intentionally admits only the common TPL path/type4; files()
  // exposes the shared wider process owner rather than another same-address pool.
  static constexpr uint32_t Manager=0x8041ec68,Global=0x80418498,Root=0x8041f494;
private:
  std::shared_ptr<NativeFileCache> files_;
};
// One VM's source-memory binding; retains real bank ownership while the guest
// Init/End cursor waits. Format/root strings are read from that actual VM.
class BattleMenuFileBinding {
public:
  BattleMenuFileBinding(std::shared_ptr<BattleMenuFileCache>,BattleActorServices);
  BattleMenuResourceServices services();
private:
  std::shared_ptr<BattleMenuFileCache> cache_;
  BattleActorServices actors_;
};
} // namespace ttyd
