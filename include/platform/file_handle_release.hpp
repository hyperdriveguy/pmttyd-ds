#pragma once
#include "event_memory_opcodes.hpp"
namespace ttyd {
struct FileHandleReleaseServices {
  EventMemoryServices memory;
  uint8_t (*byte)(void*,uint32_t)=nullptr; // memory.user owns byte reads too.
};
// Complete fileFree8018a260/44. This changes shared entry references/state;
// payload collection, deallocation and pointer clearing are separate owners.
// Null entry, null holder or state!=1 are no-ops. This is not payload free,
// event retirement, collector invocation or a native allocation-lifetime proof.
void releasePackedFileHandle(uint32_t entry,const FileHandleReleaseServices&);
} // namespace ttyd
