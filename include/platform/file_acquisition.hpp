#pragma once
#include "event_memory_opcodes.hpp"
#include <optional>
namespace ttyd {
struct FileAcquisitionServices {
  EventMemoryServices memory;
  uint8_t (*byte)(void*,uint32_t)=nullptr; // memory.user
  void* user=nullptr;
  std::optional<int32_t> (*compare)(void*,uint32_t,uint32_t)=nullptr;
  bool (*copy)(void*,uint32_t,uint32_t)=nullptr;
  bool (*yield)(void*)=nullptr;
  bool (*garbage)(void*,int32_t)=nullptr;
  std::optional<uint32_t> (*archive)(void*,uint32_t,uint32_t,uint32_t)=nullptr;
  bool (*relocate)(void*,uint32_t,uint32_t)=nullptr;
  std::optional<uint32_t> (*open)(void*,uint32_t,int32_t,uint32_t)=nullptr;
  std::optional<uint32_t> (*length)(void*,uint32_t)=nullptr;
  std::optional<uint32_t> (*allocate)(void*,uint32_t,uint32_t)=nullptr;
  std::optional<uint32_t> (*read)(void*,uint32_t,uint32_t,uint32_t,uint32_t)=nullptr;
  bool (*close)(void*,uint32_t)=nullptr;
};
struct FileAcquisitionCursor {
  uint32_t phase=0,path=0,type=0,entry=0,next=0,payload=0,attempt=0,trials=0;
  uint32_t disc=0,rounded=0,header=0,destination=0;
  FileAcquisitionServices owner;
};
// Whole _fileAlloc8018a2a4/354. Return bool means native provider readiness;
// completed result0 is original allocation failure. Incoming source r5 unused.
// This retains one invocation; callbacks must preserve their own partial work.
bool acquirePackedFile(uint32_t path,uint32_t type,const FileAcquisitionServices&,
                       FileAcquisitionCursor&,uint32_t& result);
} // namespace ttyd
