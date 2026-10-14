#pragma once
#include <array>
#include <cstdint>
#include <optional>
#include <string>
#include <vector>
namespace ttyd {
class FieldMapFlags;
bool loadFieldDoorDataAssets(const char* root);
struct FieldDoorDataCopy {
 uint32_t address=0,source=0,work=0;
 std::array<uint32_t,18> words{}; // Original 72-byte record, pointers unre-based.
};
// Native per-area original-address RAM view. Guest addresses identify actual
// copied records, never case IDs/interior masks. No per-event destruction.
class FieldDoorDataWorld {
public:
 void invalidate(){copies_.clear();initialized_=false;room_=UINT32_MAX;mapPhase_=0;}
 void synchronize(uint32_t room);
 bool initialize(uint32_t room,std::optional<uint8_t> quest1420);
 uint32_t workAddress(uint32_t index) const;
 bool readWord(uint32_t address,uint32_t&) const;
 bool setParameter(uint32_t address,int32_t selector,uint32_t value);
 bool initializeHouse05Map(FieldMapFlags&); // Original inline prefix before case snapshots.
 bool npcNames(uint32_t address,std::vector<std::string>&) const;
 const std::vector<FieldDoorDataCopy>& copies() const{return copies_;}
private:
 uint32_t room_=UINT32_MAX,next_=0x84000000;
 uint64_t generation_=0,mapEpoch_=0;
 unsigned mapPhase_=0;
 bool initialized_=false;
 std::vector<FieldDoorDataCopy> copies_;
};
}
