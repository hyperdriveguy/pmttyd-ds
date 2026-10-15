#pragma once
#include "event_memory_opcodes.hpp"
#include <optional>
namespace ttyd {
struct FilePoolServices {
  EventMemoryServices memory;
  void* user=nullptr;
  std::optional<uint32_t> (*allocate)(void*,int32_t,uint32_t)=nullptr;
  bool (*clear)(void*,uint32_t,int32_t,uint32_t)=nullptr;
};
// phase1 waits allocation; phase2 retains clear destination after manager
// initialization writes. Caller serializes access and keeps providers stable.
struct FilePoolCursor {
  uint32_t phase=0,destination=0;
  FilePoolServices owner;
};
// Complete fileInit8018b6c0/f0. Exactly1024 entries, original stride0xb0;
// allocation0 is not locally rejected. Fresh initialization abandons old pool
// ownership just as source; caller owns process startup/reinitialization.
bool initializePackedFilePool(const FilePoolServices&,FilePoolCursor&);
void setPackedFileArchiveType(uint32_t,const EventMemoryServices&);
} // namespace ttyd
