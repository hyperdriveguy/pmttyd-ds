#include "harbor_native_actor_display.hpp"
#include <cassert>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
struct Fixture {
 bool alive=true;uint32_t retireAt=0;std::map<uint32_t,uint32_t>memory;
 std::vector<uint32_t>reads;
 HarborNativeBodyDisplayServices services(){HarborNativeBodyDisplayServices s;
  s.user=this;s.lifetime=std::make_shared<int>(1);s.current=[](void*p){return static_cast<Fixture*>(p)->alive;};
  s.word=[](void*p,uint32_t a)->std::optional<uint32_t>{auto&f=*static_cast<Fixture*>(p);f.reads.push_back(a);if(a==f.retireAt)f.alive=false;const auto it=f.memory.find(a);if(it==f.memory.end())return {};return it->second;};return s;
 }
};
}
int main(){
 constexpr uint32_t Unit=0x1000,Primary=0x2000,Second=0x3000,Third=0x4000;
 Fixture f;f.memory={{Unit+0x104,0},{Unit+0x14,Primary},{Primary+0x1ac,9}};
 const auto ordinary=harborNativeBodyDisplayAttributes(Unit,f.services());assert(ordinary&&harborNativeBodyDrawAllowed(*ordinary));
 f.memory[Primary+0x1ac]=0x03000009;const auto death=harborNativeBodyDisplayAttributes(Unit,f.services());assert(death&&!harborNativeBodyDrawAllowed(*death));
 assert(harborNativeBodyDrawAllowed({0,0x02000009})); // No-shadow alone is not ordinary hiding.
 assert(!harborNativeBodyDrawAllowed({0x01000000,9}));
 assert(harborNativeBodyDrawAllowed({0,0x04000009})); // Separate additional queue flag.
 f.memory[Primary+0x1ac]=0;f.memory[Primary]=Second;f.memory[Second+0x1ac]=0;f.memory[Second]=Third;f.memory[Third+0x1ac]=1;
 assert(harborNativeBodyDisplayAttributes(Unit,f.services()));
 f.memory[Third+0x1ac]=0;f.memory[Third]=Primary;
 assert(!harborNativeBodyDisplayAttributes(Unit,f.services())); // Bounded native cycle/domain rejection.
 f.memory.erase(Primary+0x1ac);assert(!harborNativeBodyDisplayAttributes(Unit,f.services()));
 f.memory[Primary+0x1ac]=9;f.retireAt=Primary+0x1ac;assert(!harborNativeBodyDisplayAttributes(Unit,f.services()));
 assert(!harborNativeBodyDisplayAttributes(Unit,f.services())&&!harborNativeBodyDisplayAttributes(0,f.services()));
}
