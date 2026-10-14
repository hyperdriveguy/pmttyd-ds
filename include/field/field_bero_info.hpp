#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <string>
namespace ttyd {
struct EventEntry;
class FieldMapFlags;
class FieldBeroExec;
class EventAddressSpace;
struct FieldBeroInfoProgram;
bool loadFieldBeroInfoAssets(const char* root);
// Exact source-selected entry query over original data, with a native GP incoming
// string alias and valid-name get_info publication. Full case/placement children
// and raw EVT callback memory ordering remain separate owners.
class FieldBeroInfo {
public:
 static constexpr uint32_t IncomingAddress=0x85000000;
 void synchronize(uint32_t room);
 void invalidate();
 bool publishRoadIncoming(uint32_t sourceNode,uint32_t gpFlags);
 bool publishIncoming(const std::string& name,uint32_t gpFlags);
 bool initialize(const FieldMapFlags&,FieldBeroExec&); // Actual valid-domain get_info publication, once per area.
 std::optional<uint32_t> tableAddress() const;
 bool registerOriginal(uint32_t table,const FieldMapFlags&,FieldBeroExec&); // Each real get_info request, not once-only admission.
 bool infoByIndex(EventEntry&,int32_t index) const;
 std::optional<float> length(int32_t index) const; // Original float result before evtSetFloat routing.
 bool registered() const;
 void setCurrentNumber(int32_t value){currentNumber_=value;}
 void unknownCurrentNumber(){currentNumber_.reset();}
 std::optional<int32_t> currentNumber() const{return currentNumber_;}
 bool intoInfo(EventEntry&,uint32_t incomingAddress=IncomingAddress) const; // Writes only original LW1..8; unavailable holds.
 bool readWord(uint32_t address,uint32_t&) const;
 bool readByte(uint32_t address,uint8_t&) const;
 bool importScripts(EventAddressSpace&) const;
 bool known() const;
 std::optional<std::array<int32_t,8>> publication() const{return known()?publication_:std::nullopt;}
private:
 std::shared_ptr<const FieldBeroInfoProgram> program_;
 uint32_t room_=UINT32_MAX,flags_=0;
 uint64_t generation_=0;
 bool incomingKnown_=false,registered_=false;
 std::array<uint32_t,16> entries_{};
 std::array<std::optional<int32_t>,16> switches_{};
 std::string incoming_;
 std::optional<std::array<int32_t,8>> publication_;
 std::optional<int32_t> currentNumber_=0; // Original BSS bootstrap; get_info does not reset this word.
};
}
