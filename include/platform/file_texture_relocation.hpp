#pragma once
#include "event_memory_opcodes.hpp"
namespace ttyd {
struct FileTextureRelocationServices {
  EventMemoryServices memory;
  uint8_t (*byte)(void*, uint32_t) = nullptr; // memory.user owns byte reads.
  void* user = nullptr;
  bool (*panic)(void*, uint32_t filename, uint32_t line, uint32_t format) = nullptr;
};
// active retains only the failed-signature panic path. After acknowledged
// panic, continue after the check rather than rereading/repairing signature.
struct FileTextureRelocationCursor {
  bool active = false;
  uint32_t base = 0;
  FileTextureRelocationServices owner;
};
// Complete type4 Set block8018ab88..8018ac7c. Panic may hold; acknowledgement
// resumes AFTER the original signature check. Other memory operations execute
// synchronously in source order, with uncapped live-count/low16-index looping.
// False is only unavailable panic or mismatched owner/base on that retained call.
bool resumePackedFileTextureRelocation(uint32_t base,
    const FileTextureRelocationServices&, FileTextureRelocationCursor&);
} // namespace ttyd
