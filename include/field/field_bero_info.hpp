#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
namespace ttyd {
struct EventEntry;
struct FieldBeroInfoProgram;
bool loadFieldBeroInfoAssets(const char* root);
// Exact source-selected entry query over original data, with a native GP incoming
// string alias. Does not claim get_info HitObj registration or BeroEXEC reset.
class FieldBeroInfo {
public:
 static constexpr uint32_t IncomingAddress=0x85000000;
 void synchronize(uint32_t room);
 void invalidate();
 bool publishRoadIncoming(uint32_t sourceNode,uint32_t gpFlags);
 bool publishIncoming(const std::string& name,uint32_t gpFlags);
 bool intoInfo(EventEntry&) const; // Writes only original LW1..8; unavailable holds.
 bool readWord(uint32_t address,uint32_t&) const;
 bool readByte(uint32_t address,uint8_t&) const;
 bool known() const;
 std::optional<std::array<int32_t,8>> publication() const{return known()?publication_:std::nullopt;}
private:
 std::shared_ptr<const FieldBeroInfoProgram> program_;
 uint32_t room_=UINT32_MAX,flags_=0;
 uint64_t generation_=0;
 bool incomingKnown_=false;
 std::string incoming_;
 std::optional<std::array<int32_t,8>> publication_;
};
}
