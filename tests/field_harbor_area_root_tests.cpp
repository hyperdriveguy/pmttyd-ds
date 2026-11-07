#include "field_area_event.hpp"
#include "event_address_space.hpp"
#include "event_script_assets.hpp"
#include <array>
#include <cassert>
#include <cstring>
#include <map>
#include <string>
using namespace ttyd;
namespace {
struct Trace {
 std::array<unsigned,4> calls{},first{};
 std::array<uint32_t,4> ids{};
 unsigned audio=0;uint32_t root=0;
 static int32_t call(void* user,FieldAreaNativeCall& r){
  auto& t=*static_cast<Trace*>(user);
  if(r.function==0x80136158){
   auto value=[&](unsigned n){uint32_t raw=0;assert(r.memory.readWord(r.event.arguments+4*n,raw));return uint32_t(getEventValue(r.event,int32_t(raw),r.operands));};
   const auto name=value(0);assert(name>=0x90401b80 && name<=0x90401ba4 && (name-0x90401b80)%12==0);
   const auto n=(name-0x90401b80)/12;
   assert(value(1)==0x904017e0 && value(2)==0);
   const auto expected=std::string("pointLight")+char('1'+n);
   for(unsigned i=0;i<=expected.size();++i){uint8_t b=0;assert(r.memory.readByte(name+i,b) && b==(i==expected.size()?0:uint8_t(expected[i])));}
   if(!t.calls[n])t.ids[n]=r.event.id;
   assert(t.ids[n]==r.event.id && r.event.id!=t.root);
   ++t.calls[n];t.first[n]+=r.first?1:0;
   return 0; // Explicit fixture frontier; original callback is ongoing too.
  }
  assert(r.event.id==t.root && r.function==0x801525a4);++t.audio;
  return 0; // Genuine first parent audio provider remains unavailable.
 }
};
}
void harborRoot(const char* root){
 assert(loadFieldAreaEventAssets(root));FieldAreaEvent area;assert(area.begin(1,0));
 FieldAreaGpState state{};const auto id=area.appendAreaInit(state);assert(id);
 Trace trace;trace.root=*id;FieldAreaEventServices services;services.user=&trace;services.call=Trace::call;
 for(unsigned n=1;n<=12;++n)area.tick(17*n,services);
 assert(!area.failed && area.eventAlive(*id) && area.completed().empty() && area.liveEventCount()==5 && trace.audio);
 for(unsigned n=0;n<4;++n){assert(trace.calls[n]>1 && trace.first[n]==1 && area.eventAlive(trace.ids[n]));for(unsigned j=0;j<n;++j)assert(trace.ids[n]!=trace.ids[j]);}
 // Conflicting overlapping packs were merged before the actual VM import;
 // source child and full root now coexist with prior gor02/DOL services.
 EventScriptAssets a,b,c;const std::string base=root;
 assert(a.load((base+"/door-data.pmevt").c_str()) && b.load((base+"/field-npc-creation.pmevt").c_str()) && c.load((base+"/field-harbor-init.pmevt").c_str()));
 assert(a.merge(b) && a.merge(c));uint32_t word=0;
 assert(a.readWord(0x9050ef7c,word) && word==0x6b && a.readWord(0x90505208,word) && word==0x0003005b);
 FieldAreaEvent existing;assert(existing.begin(2,0));
}
int main(){harborRoot("romfs");}
