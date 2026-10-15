#pragma once
#include "battle_actor.hpp"
#include <optional>
#include <string>
// Cursor retains source root/path/guest descriptor across holds; caller
// keeps service bindings fixed (not validated here). Init/End use separate
// invocations; a reset cursor must not replay an already completed fileFree.
namespace ttyd {
// Original battleMenuDispInit/End file ownership. The file service returns a
// guest FileEntry, whose +a0 owns a pointer cell for the actual TPL payload.
// Neither a host FILE nor an atlas image number is that descriptor.
struct BattleMenuResourceServices {
  BattleActorServices memory;
  void* user=nullptr;
  std::optional<uint32_t> (*dvdRoot)(void*)=nullptr;
  // sprintf's destination is a source stack-local char[256]. Its identity is
  // opaque here; the provider formats the original format/root operands.
  std::optional<std::string> (*format)(void*,uint32_t format,uint32_t root)=nullptr;
  std::optional<uint32_t> (*fileAllocate)(void*,const std::string&,int32_t)=nullptr;
  bool (*fileRelease)(void*,uint32_t)=nullptr;
};
struct BattleMenuResourceCursor {
  uint32_t phase=0,battle=0,root=0,descriptor=0;
  std::string path;
};
// Complete local Init80119da0/58 and End80119d74/2c. Preserve the same service
// owners while held. A present file descriptor0 is the source result and is
// still stored, not reinterpreted as an asynchronous wait.
bool initializePackedBattleMenuResources(const BattleMenuResourceServices&,
                                         BattleMenuResourceCursor&);
bool endPackedBattleMenuResources(const BattleMenuResourceServices&,
                                  BattleMenuResourceCursor&);
} // namespace ttyd
