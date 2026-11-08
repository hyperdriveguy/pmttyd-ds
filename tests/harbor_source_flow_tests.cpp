#include <memory>
#include "harbor_source_flow.hpp"
#include "battle_main.hpp"
#include "battle_sequence_startup.hpp"
#include <cassert>
#include <iostream>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Battle=0x1000;
struct F {
 EventNativeRuntime vm{0x81000000,{}};std::shared_ptr<const uint8_t>life=std::make_shared<const uint8_t>(0);std::unique_ptr<HarborSourceFlow>flow;
 std::vector<uint32_t>trace,startup;size_t holdAt=SIZE_MAX;unsigned attempts=0;bool alive=true,reentry=false,holdSetup=false;
 F(){assert(vm.memory.registerRam(Battle,std::vector<uint8_t>(0x40000))&&vm.memory.registerRam(0x8041eb90,std::vector<uint8_t>(4)));w(0x8041eb90,Battle);w(Battle+4,4);
  // The real EventNativeRuntime GP binding supplies this source-global alias;
  // this fixture does not create a second private GP cell.
  assert(w(0x80415fd0)==0x81000000);
  HarborSourceFlowBinding b;b.vm=&vm;b.lifetime=life;b.current=[this]{return alive;};
  b.main=[this](uint32_t fn,uint32_t battle){assert(battle==Battle);if(reentry){assert(!flow->resumeMain());assert(!flow->current());}if(trace.size()==holdAt&&attempts++<2)return false;trace.push_back(fn);return true;};
  b.startup=[this](const BattleFirstActRequest&q)->std::optional<uint32_t>{if(q.function==0x8005e0c0){startup.push_back(q.function);return 0x21000;}if(q.function==0x80141050){startup.push_back(q.function);return 0;}if(q.function==0x800bf1b4){startup.push_back(q.function);return 0;}if(q.function==0x800f8aac){if(holdSetup)return {};startup.push_back(q.function);return 0;}return {};};
  b.end=[this](uint32_t battle){assert(battle==Battle);startup.push_back(0x80215234);w(Battle+4,4);return true;};flow=HarborSourceFlow::create(std::move(b));assert(flow);
 }
 void w(uint32_t a,uint32_t v){assert(vm.memory.writeWord(a,v));}uint32_t w(uint32_t a){uint32_t v;assert(vm.memory.readWord(a,v));return v;}
 void run(){for(unsigned i=0;i<100;++i)if(flow->resumeMain())return;assert(false);}
};
void mainOrder(){F f;f.run();std::vector<uint32_t>expected;for(auto fn:battleMainRequests)if(fn!=0x8011f1ec)expected.push_back(fn);assert(f.trace==expected&&(f.w(Battle+0xef4)&0x80000000));for(size_t i=0;i<expected.size();++i){F held;held.holdAt=i;held.reentry=true;held.run();assert(held.attempts==3&&held.trace==expected);}}
void actualParentComposition(){F f;f.w(Battle+4,0);f.w(Battle+0xf0c,0x01000000);f.holdSetup=true;assert(!f.flow->resumeMain()&&f.flow->active()&&f.flow->pending()==0x800f8aac);const auto prefix=f.startup;assert(!f.flow->resumeMain()&&f.startup==prefix);f.holdSetup=false;f.run();assert(f.startup==std::vector<uint32_t>({0x8005e0c0,0x80141050,0x800bf1b4,0x800f8aac})&&f.w(Battle+0xf0c)==0x01000002&&f.w(Battle+4)==0);
 // Actual manager capture means Init completion never runs FirstAct/Turn during
 // the same dispatch. The next invocation performs real Init-event readiness.
 f.run();assert(f.w(Battle+0xf0c)==0x01000003);assert(!f.flow->resumeMain()&&f.flow->pending()==0x800414e8);const auto mainPrefix=f.trace;assert(!f.flow->resumeMain()&&f.trace==mainPrefix);
 F turn;turn.w(Battle+4,2);turn.w(Battle+0xf14,0x03000000);turn.run();assert(turn.w(Battle+0xf14)==0x03000001&&turn.w(Battle+4)==2); // Real Turn0 increments/yields.
 F end;end.w(Battle+4,3);end.run();assert(end.startup==std::vector<uint32_t>({0x80215234})&&(end.w(Battle+0xef4)&0x80000000));
}
void retirement(){F f;f.holdAt=0;assert(!f.flow->resumeMain());f.alive=false;assert(!f.flow->resumeMain()&&!f.flow->current());f.alive=true;assert(!f.flow->resumeMain()&&f.trace.empty());}
}
int main(){mainOrder();actualParentComposition();retirement();std::cout<<"Harbor actual VM source controller composition/order/holds tests PASS (test external leaves)\n";}
