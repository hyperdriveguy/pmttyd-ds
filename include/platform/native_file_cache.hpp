#pragma once
#include "event_address_space.hpp"
#include "file_acquisition.hpp"
#include "file_texture_relocation.hpp"
#include <array>
#include <memory>
#include <string>
#include <vector>
namespace ttyd {
// One process's file manager, shared by menu/animation consumers and VM views.
// Original pool/acquisition/relocation/release operate on these actual BE bytes.
// ROMFS reads and stable backed address tokens are explicit native policy;
// source SDK/smart heap placement, eviction/movement and DVD workers are separate.
class NativeFileCache {
public:
 static std::shared_ptr<NativeFileCache> create();
 NativeFileCache(const NativeFileCache&)=delete;
 NativeFileCache& operator=(const NativeFileCache&)=delete;
 // Catalog is immutable after first successful bind/acquisition. Stage original
 // unrelocated bytes; supported types are0(raw),4(TPL),5(nested MODEL).
 struct Source {std::string path;uint32_t type;std::vector<uint8_t> original;};
 bool stage(const std::string& path,uint32_t type,std::vector<uint8_t> original);
 bool stage(std::vector<Source>); // Atomic catalog addition, including validation.
 bool yieldProvider(void*,bool (*)(void*)); // Configure before first bind/acquire.
 bool bind(EventAddressSpace&); // Owner outlives every borrowing VM.
 std::optional<uint32_t> acquire(const std::string&,uint32_t type);
 bool release(uint32_t entry);
 bool archiveType(uint32_t);
 bool contains(const std::string&,uint32_t) const;
 bool ownsPayload(uint32_t address,uint32_t bytes) const;
 static constexpr uint32_t Manager=0x8041ec68,Global=0x80418498;
private:
 NativeFileCache();
 struct File {
  std::string path;uint32_t type=0,address=0,headerAddress=0,entry=0;
  std::array<uint8_t,12> header{};
  std::vector<uint8_t> original,payload;
  FileTextureRelocationCursor relocation;
 };
 static constexpr uint32_t Pool=0x87000000,FirstPayload=0x87100000,
  Headers=0x87200000,Name=0x803e6e78,Disc=0x87400000,MorePayloads=0x87500000,PayloadLimit=0x88000000;
 std::array<uint8_t,4> global_{};
 std::array<uint8_t,24> manager_{};
 std::array<uint8_t,1024> name_{};
 std::vector<uint8_t> pool_;
 std::vector<std::unique_ptr<File>> files_;
 File* current_=nullptr;
 uint32_t nextPayload_=MorePayloads;
 uint16_t discFlags_=0;
 bool frozen_=false;
 void* yieldUser_=nullptr;bool (*yield_)(void*)=nullptr;
 FileAcquisitionCursor acquisition_;
 uint8_t* span(uint32_t,unsigned);
 uint8_t byte(uint32_t);void byte(uint32_t,uint8_t);
 uint16_t half(uint32_t);void half(uint32_t,uint16_t);
 uint32_t word(uint32_t);void word(uint32_t,uint32_t);
 EventMemoryServices memory();
 FileAcquisitionServices services();
};// Stages complete unchanged original model/TPL/auxiliary files from accepted
// local packs, deduplicated by exact source filename. No acquisition/startup
// acknowledgement; call before binding this same process cache to any VM.
bool stagePrologueAnimationFiles(NativeFileCache&,const char* romfsRoot);
}
