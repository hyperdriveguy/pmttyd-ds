#include "battle_menu_resources.hpp"
#include <cassert>
#include <map>
#include <vector>
using namespace ttyd;
namespace {
struct Fixture {
  std::map<uint32_t,uint32_t> words{{0x8041eb90,0x10000},{0x10000+0x163f8,7}};
  std::vector<unsigned> calls;
  bool rootReady=false,formatReady=false,allocateReady=false,releaseReady=false;
  uint32_t descriptor=0x30000,expectedRelease=0x30000;
  BattleMenuResourceServices services(){
    BattleMenuResourceServices s;s.user=this;s.memory.memory.user=this;
    s.memory.memory.word=[](void* p,uint32_t a){auto& f=*static_cast<Fixture*>(p);assert(f.words.count(a));return f.words[a];};
    s.memory.memory.storeWord=[](void* p,uint32_t a,uint32_t n){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(4);f.words[a]=n;};
    s.dvdRoot=[](void* p)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);f.calls.push_back(1);if(!f.rootReady)return {};f.words[0x8041eb90]=0x20000;return 0x8041f494;};
    s.format=[](void* p,uint32_t format,uint32_t root)->std::optional<std::string>{auto& f=*static_cast<Fixture*>(p);f.calls.push_back(2);assert(format==0x802ef810 && root==0x8041f494);if(!f.formatReady)return {};return "./battle/common/battle_common.tpl";};
    s.fileAllocate=[](void* p,const std::string& path,int32_t type)->std::optional<uint32_t>{auto& f=*static_cast<Fixture*>(p);f.calls.push_back(3);assert(path=="./battle/common/battle_common.tpl" && type==4);if(!f.allocateReady)return {};return f.descriptor;};
    s.fileRelease=[](void* p,uint32_t entry){auto& f=*static_cast<Fixture*>(p);f.calls.push_back(5);assert(entry==f.expectedRelease);if(!f.releaseReady)return false;f.words[0x8041eb90]=0x50000;return true;};
    return s;
  }
};
}
int main(){
  Fixture f;auto s=f.services();BattleMenuResourceCursor c;
  assert(!initializePackedBattleMenuResources(s,c));
  f.rootReady=true;assert(!initializePackedBattleMenuResources(s,c));
  assert(c.battle==0x10000 && c.root==0x8041f494);
  f.formatReady=true;assert(!initializePackedBattleMenuResources(s,c));
  f.allocateReady=true;assert(initializePackedBattleMenuResources(s,c));
  assert((f.calls==std::vector<unsigned>{1,1,2,2,3,3,4}));
  assert(f.words[0x10000+0x163f8]==f.descriptor && !f.words.count(0x20000+0x163f8));
  assert(!c.phase && c.path.empty());
  f.words[0x8041eb90]=0x10000;assert(!endPackedBattleMenuResources(s,c));
  f.words[0x8041eb90]=0x40000;f.words[0x10000+0x163f8]=9;
  f.releaseReady=true;assert(endPackedBattleMenuResources(s,c));
  assert(f.words[0x10000+0x163f8]==9 && !f.words.count(0x40000+0x163f8));
  Fixture zero;auto z=zero.services();zero.rootReady=zero.formatReady=zero.allocateReady=true;zero.descriptor=zero.expectedRelease=0;
  assert(initializePackedBattleMenuResources(z,c));assert(zero.words[0x10000+0x163f8]==0);
  zero.words[0x8041eb90]=0x10000;zero.releaseReady=true;assert(endPackedBattleMenuResources(z,c));
  assert(zero.words[0x10000+0x163f8]==0); // Source still calls release(null).
}
