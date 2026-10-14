#include "field_area_event.hpp"
#include "field_door_data.hpp"
#include "field_map_flags.hpp"
#include <cassert>
#include <cstring>
#include <filesystem>
#include <fstream>
#include <unistd.h>
using namespace ttyd;
int main(){
 assert(loadFieldDoorDataAssets("romfs") && loadFieldMapFlagAssets("romfs") && loadFieldAreaEventAssets("romfs"));
 FieldDoorDataWorld doors;assert(doors.initialize(2,uint8_t(0)));FieldMapFlags map;map.synchronize(2);
 FieldAreaEvent area;assert(!area.begin(0,0));assert(area.begin(2,0) && area.doorEntryCount()==4);FieldAreaEventServices services;services.doors=&doors;
 std::array<int32_t,16> bank;for(unsigned n=0;n<16;++n)bank[n]=1000+n;
 const auto first=area.appendDoorEntry(0,bank);assert(first);
 area.tick(17,services);assert(!area.failed && area.completed().empty() && area.pendingFunction==0x80038468);
 assert(!area.cases()->active(0));services.map=&map;area.tick(34,services);
 assert(!area.failed && area.completed().size()==1 && area.completed()[0].eventId==*first && area.completed()[0].returned);
 assert(map.mapFlags("S_ie_02_naka")==1 && map.hitEnabled("A_ie_02C")==false);
 const auto* close=area.cases()->active(0);const auto* open=area.cases()->active(1);assert(close && open);
 assert(close->type==9 && close->flags==5 && close->priority==0 && close->locals[15]==8 && close->script==0x80330724);
 assert(open->locals[15]==0 && open->script==0x8032f670 && std::string(close->name.data())=="A_ie_02_doa02");
 bank=area.completed().back().locals;
 for(unsigned kind=1;kind<4;++kind){assert(area.appendDoorEntry(kind,bank));area.tick(34+17*kind,services);assert(!area.failed && area.completed().size()==kind+1);bank=area.completed().back().locals;}
 for(unsigned n=0;n<7;++n)assert(area.cases()->active(n));assert(!area.cases()->active(7));
 assert(std::string(area.cases()->active(2)->name.data())=="A_ie_03_doa_03");
 assert(area.cases()->active(4)->type==0 && std::string(area.cases()->active(4)->name.data())=="A_i03_rol");
 assert(area.cases()->active(5)->script==0x80330724 && area.cases()->active(6)->script==0x8032f670);
 for(unsigned slot=0;slot<7;++slot){const auto& c=*area.cases()->active(slot);for(unsigned n=4;n<15;++n)assert(c.locals[n]==int32_t(1000+n));}
 assert(bank[1]==0 && bank[15]==0); // Actual selector9 tail after snapshots.
 assert(area.cases()->active(5)->locals[1]!=0 && area.cases()->active(6)->locals[1]!=0);
 const auto id=area.cases()->active(0)->script;area.tick(200,services);assert(area.cases()->active(0)->script==id && area.completed().size()==4);
 // A later entry shares the same pool; installer retirement did not reset it.
 assert(area.appendDoorEntry(3,bank));area.tick(217,services);assert(area.completed().size()==5 && area.cases()->active(7) && area.cases()->active(8));
 // Missing map/door state yields before completing source services.
 FieldAreaEvent held;assert(held.begin(2,0) && held.appendDoorEntry(3,bank));held.tick(17,{});assert(!held.failed && held.pendingFunction==0x800e81f4 && held.completed().empty());
 map.synchronize(0);held.tick(34,services);assert(!held.failed && held.pendingFunction==0x80038468 && held.completed().empty());
 map.synchronize(2);held.tick(51,services);assert(!held.failed && held.completed().size()==1);
 assert(!held.appendDoorEntry(4,bank));
 // Source constructors have no exhaustion return; native API admits only a real free slot.
 FieldAreaEvent crowded;assert(crowded.begin(2,0));unsigned roots=0;
 while(crowded.appendDoorEntry(3,bank)){++roots;assert(roots<=256);}
 assert(roots>0);crowded.tick(17,{});assert(!crowded.failed && crowded.completed().empty());
 FieldAreaEvent full;assert(full.begin(2,0));
 for(unsigned n=0;n<32;++n){assert(full.appendDoorEntry(3,bank));full.tick(17*(n+1),services);assert(!full.failed && full.completed().size()==n+1);}
 assert(full.cases()->active(63));assert(full.appendDoorEntry(3,bank));full.tick(600,services);
 assert(!full.failed && full.completed().size()==32 && full.pendingFunction==0x80070110);
 char temporary[]="/tmp/ttyd-area-program-XXXXXX";assert(mkdtemp(temporary));const std::filesystem::path fixture(temporary);
 std::filesystem::copy_file("romfs/door-data.pmevt",fixture/"door-data.pmevt");
 {std::ofstream out(fixture/"field-area-events.bin",std::ios::binary);out.write("PMFAEV02",8);uint32_t count=257;out.write(reinterpret_cast<const char*>(&count),4);}
 assert(!loadFieldAreaEventAssets(temporary));FieldAreaEvent retained;assert(retained.begin(2,0) && retained.doorEntryCount()==4);std::filesystem::remove_all(fixture);


}
