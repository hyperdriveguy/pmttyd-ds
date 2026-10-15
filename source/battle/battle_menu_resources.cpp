#include "battle_menu_resources.hpp"
namespace ttyd {
bool initializePackedBattleMenuResources(const BattleMenuResourceServices& s,
                                         BattleMenuResourceCursor& c) {
  if(!c.phase){c={};c.battle=currentBattleAddress(s.memory);c.phase=1;}
  if(c.phase==1){
    if(!s.dvdRoot)return false;
    const auto root=s.dvdRoot(s.user);if(!root)return false;
    c.root=*root;c.phase=2;
  }
  if(c.phase==2){
    if(!s.format)return false;
    const auto path=s.format(s.user,0x802ef810,c.root);if(!path)return false;
    c.path=*path;c.phase=3;
  }
  if(c.phase==3){
    if(!s.fileAllocate)return false;
    const auto descriptor=s.fileAllocate(s.user,c.path,4);if(!descriptor)return false;
    c.descriptor=*descriptor;c.phase=4;
  }
  s.memory.memory.storeWord(s.memory.memory.user,c.battle+0x163f8,c.descriptor);
  c={};return true;
}
bool endPackedBattleMenuResources(const BattleMenuResourceServices& s,
                                  BattleMenuResourceCursor& c) {
  if(!c.phase){
    c={};c.battle=currentBattleAddress(s.memory);
    c.descriptor=s.memory.memory.word(s.memory.memory.user,c.battle+0x163f8);
    c.phase=1;
  }
  if(!s.fileRelease || !s.fileRelease(s.user,c.descriptor))return false;
  // Source does not clear or reload the owner after fileFree.
  c={};return true;
}
} // namespace ttyd
