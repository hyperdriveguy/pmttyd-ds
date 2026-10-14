#include "field_door_data.hpp"
#include "event_script_assets.hpp"
#include <cstdio>
#include <cstring>
#include <memory>
namespace ttyd {
namespace {
struct Spec {uint32_t room,source,work,normal,alternate,quest;};
struct Assets {EventScriptAssets scripts;std::vector<Spec> specs;};
std::shared_ptr<const Assets> assets;uint64_t generation=0;
bool names(const EventScriptAssets& data,uint32_t pointer,std::vector<std::string>& result) {
 std::vector<std::string> next;
 for(unsigned n=0;n<64;++n) {
  uint32_t text=0;if(!data.readWord(pointer+4*n,text))return false;
  if(!text){result=std::move(next);return true;}
  std::string name;
  for(unsigned k=0;k<32;++k) {
   uint8_t byte=0;if(!data.readByte(text+k,byte))return false;
   if(!byte)break;name.push_back(char(byte));
  }
  if(name.empty() || name.size()==32)return false;
  next.push_back(std::move(name));
 }
 return false;
}
}
bool loadFieldDoorDataAssets(const char* root) {
 if(!root)return false;
 auto next=std::make_shared<Assets>();const std::string base=root;
 if(!next->scripts.load((base+"/door-data.pmevt").c_str()))return false;
 FILE* f=std::fopen((base+"/door-data.bin").c_str(),"rb");if(!f)return false;
 char magic[8];uint32_t count=0;
 bool ok=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMDRDT01",8) && std::fread(&count,4,1,f)==1 && count<=64;
 for(unsigned n=0;ok && n<count;++n) {
  Spec s{};uint32_t words[18];std::vector<std::string> check;
  ok=std::fread(&s,sizeof(s),1,f)==1 && s.room<=65535 && s.work<32 && s.quest<=255 && !(s.source&3) && s.source;
  for(unsigned w=0;ok && w<18;++w)ok=next->scripts.readWord(s.source+4*w,words[w]);
  ok=ok && words[13]==s.normal && names(next->scripts,s.normal,check) && names(next->scripts,s.alternate,check);
  for(const auto& old:next->specs)if(old.room==s.room && (old.work==s.work || old.source==s.source))ok=false;
  if(ok)next->specs.push_back(s);
 }
 ok=ok && std::fgetc(f)==EOF;std::fclose(f);if(!ok)return false;
 assets=std::move(next);++generation;return true;
}
void FieldDoorDataWorld::synchronize(uint32_t room) {
 if(room_!=room || generation_!=generation) {
  copies_.clear();initialized_=false;room_=room;generation_=generation;
 }
}
bool FieldDoorDataWorld::initialize(uint32_t room,std::optional<uint8_t> quest) {
 synchronize(room);if(!assets)return false;if(initialized_)return true;
 std::vector<FieldDoorDataCopy> next;uint32_t cursor=next_;
 for(const auto& s:assets->specs)if(s.room==room) {
  if(!quest || cursor>0x840fffb8)return false; // Native guest-arena capacity, not original heap policy.
  FieldDoorDataCopy copy;copy.address=cursor;copy.source=s.source;copy.work=s.work;
  for(unsigned w=0;w<18;++w)if(!assets->scripts.readWord(s.source+4*w,copy.words[w]))return false;
  copy.words[13]=*quest==s.quest?s.alternate:s.normal;
  next.push_back(copy);cursor+=72;
 }
 copies_=std::move(next);next_=cursor;initialized_=true;return true;
}
uint32_t FieldDoorDataWorld::workAddress(uint32_t index) const {
 if(generation_!=generation || !initialized_)return 0;
 for(const auto& copy:copies_)if(copy.work==index)return copy.address;
 return 0;
}
bool FieldDoorDataWorld::readWord(uint32_t address,uint32_t& word) const {
 if(!assets || generation_!=generation || (address&3))return false;
 for(const auto& c:copies_)if(address>=c.address && uint64_t(address)<uint64_t(c.address)+72){word=c.words[(address-c.address)/4];return true;}
 return assets->scripts.readWord(address,word);
}
bool FieldDoorDataWorld::setParameter(uint32_t address,int32_t selector,uint32_t value) {
 if(selector!=0)return true; // Original resolves arguments, then performs no access.
 if(generation_!=generation || !initialized_)return false;
 for(auto& c:copies_)if(c.address==address){c.words[13]=value;return true;}
 return false;
}
bool FieldDoorDataWorld::npcNames(uint32_t address,std::vector<std::string>& result) const {
 uint32_t pointer=0;return readWord(address+0x34,pointer) && assets && names(assets->scripts,pointer,result);
}
}
