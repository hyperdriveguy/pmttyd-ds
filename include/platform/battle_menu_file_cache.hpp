#pragma once
#include "battle_menu_resources.hpp"
#include "event_address_space.hpp"
#include "file_acquisition.hpp"
#include "file_texture_relocation.hpp"
#include <array>
#include <memory>
#include <vector>
namespace ttyd {
// Process-owned native backing for the supported original common-menu bank.
// Source fileInit/acquire/relocation/free own entry bytes; native ROMFS replaces
// disc I/O. Native address tokens below are backed objects, not GC heap claims.
class BattleMenuFileCache {
public:
  BattleMenuFileCache() = default;
  BattleMenuFileCache(const BattleMenuFileCache&) = delete;
  BattleMenuFileCache& operator=(const BattleMenuFileCache&) = delete;
  bool load(const char* originalTpl);
  bool bind(EventAddressSpace&); // Cache must outlive every bound address space.
  std::optional<uint32_t> acquire(const std::string&,uint32_t type);
  bool release(uint32_t entry);
  bool ready() const {return ready_;}
  static constexpr uint32_t Manager=0x8041ec68,Global=0x80418498,Root=0x8041f494;
private:
  static constexpr uint32_t Pool=0x87000000,Payload=0x87100000,Header=0x87200000,
                            Name=0x87300000,Disc=0x87400000;
  std::array<uint8_t,4> global_{},pointer_{},backlink_{};
  std::array<uint8_t,24> manager_{};
  std::array<uint8_t,256> name_{};
  std::vector<uint8_t> pool_,original_,payload_;
  uint16_t discFlags_=0;
  uint32_t entry_=0;
  bool ready_=false;
  FileAcquisitionCursor acquisition_;
  FileTextureRelocationCursor relocation_;
  uint8_t* span(uint32_t,unsigned);
  uint32_t word(uint32_t);
  uint16_t half(uint32_t);
  uint8_t byte(uint32_t);
  void word(uint32_t,uint32_t);
  void half(uint32_t,uint16_t);
  void byte(uint32_t,uint8_t);
  EventMemoryServices memory();
  FileAcquisitionServices services();
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
