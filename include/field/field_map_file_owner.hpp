#pragma once
#include "field_map_source.hpp"
#include "event_address_space.hpp"
#include <cstdio>
#include <memory>
#include <string>
#include <vector>
namespace ttyd {
struct FieldMapFileRecord {
 std::string sourcePath,relativePath;
 uint32_t logicalBytes=0,transferBytes=0;
};
using FieldMapFileCatalog=std::vector<FieldMapFileRecord>;
// Native process adapter for the complete original DVD manager graph. The
// platform bootstrap replaces its GameCube OS thread with an explicitly pumped
// stationary worker. Table/flags/sort scratch are actual shared VM storage;
// only native file IO replaces the physical disc transfer leaf.
class FieldMapFileOwner {
public:
 static constexpr uint32_t Slots=256,SlotBytes=0x98,TableBytes=Slots*SlotBytes;
 static constexpr uint32_t Globals=0x8041ebf0,GlobalsBytes=12,TablePointer=Globals+8;
 static constexpr uint32_t SortScratch=0x803c5360,SortScratchBytes=0x3100;
 static constexpr uint32_t SortGlobals=0x8041e584,SortGlobalsBytes=24;
 static constexpr uint32_t StackBytes=64*1024,CacheLine=32;
 static std::unique_ptr<FieldMapFileOwner> bind(EventAddressSpace&,uint32_t tableAddress,uint32_t stackAddress,const std::string& romfsRoot);
 // The owner copies validated rows before exposing any source manager storage.
 static std::unique_ptr<FieldMapFileOwner> bind(EventAddressSpace&,uint32_t tableAddress,uint32_t stackAddress,const std::string& romfsRoot,const FieldMapFileCatalog&);
 static const FieldMapFileCatalog& originalHarborCatalog();
 ~FieldMapFileOwner();
 FieldMapFileOwner(const FieldMapFileOwner&)=delete;FieldMapFileOwner& operator=(const FieldMapFileOwner&)=delete;
 // Call outside simulation ticks. false retains the source operation. No
 // fallback provider may acknowledge a false result for one of these leaves.
 static bool accepts(uint32_t function);
 bool service(FieldMapSourceCall&);
 bool pump(unsigned visits=4096);
 bool current() const;
 bool pending() const{return worker_.active;}
 uint32_t tableAddress() const{return tableAddress_;}
private:
 FieldMapFileOwner(EventAddressSpace&,uint32_t,uint32_t,const std::string&,const FieldMapFileCatalog&);
 FieldMapSourceServices services();
 bool text(uint32_t,std::string&) const;
 std::optional<unsigned> path(uint32_t) const;
 std::optional<unsigned> slot(uint32_t fileInfo) const;
 EventAddressSpace& memory_;uint32_t tableAddress_,stackAddress_;std::string root_;
 const FieldMapFileCatalog catalog_;
 std::array<uint8_t,GlobalsBytes> globals_{};
 std::array<uint8_t,TableBytes> table_{};
 std::array<uint8_t,SortScratchBytes> sort_{};
 std::array<uint8_t,SortGlobalsBytes> sortGlobals_{};
 std::array<uint8_t,StackBytes> stack_{};
 std::array<uint32_t,5> mappings_{};unsigned mapped_=0;
 std::array<FILE*,Slots> files_{};
 FieldMapSourceCursor worker_;
};
}
