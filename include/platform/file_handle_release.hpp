#pragma once
#include "event_memory_opcodes.hpp"
namespace ttyd {
struct FileHandleReleaseServices {
  EventMemoryServices memory;
  uint8_t (*byte)(void*,uint32_t)=nullptr; // memory.user owns byte reads too.
};
// Complete fileFree8018a260/44. This changes shared entry references/state;
// payload collection, deallocation and pointer clearing are separate owners.
void releasePackedFileHandle(uint32_t entry,const FileHandleReleaseServices&);
} // namespace ttyd
