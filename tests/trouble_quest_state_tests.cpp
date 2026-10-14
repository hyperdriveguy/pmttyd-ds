#include "trouble_quest_state.hpp"
#include "trouble_quest_services.hpp"
#include "field_actors.hpp"
#include "save.hpp"
#include <cassert>
#include <cstdio>
#include <fstream>
using namespace ttyd;
static void sum(SaveBytes& b,unsigned n) {
 uint32_t c=2166136261u;for(unsigned i=0;i<n;++i){c^=b[i];c*=16777619u;}
 for(unsigned i=0;i<4;++i)b[n+i]=uint8_t(c>>(8*i));
}
int main() {
 Progress p,out;
 for(int32_t f=5374;f<=5421;++f)assert(!troubleQuestFlag(p.originalNpcTalkState,f));
 for(int32_t b=1469;b<=1474;++b)assert(!troubleQuestByte(p.originalNpcTalkState,b));
 assert(!setTroubleQuestFlag(p.originalNpcTalkState,5373,true) && !setTroubleQuestFlag(p.originalNpcTalkState,5422,true));
 assert(!setTroubleQuestByte(p.originalNpcTalkState,1468,1) && !setTroubleQuestByte(p.originalNpcTalkState,1475,1));
 // Preserve unknown knowledge independently for each recovered field.
 for(int32_t f=5374;f<=5421;++f) {
  auto q=p;assert(setTroubleQuestFlag(q.originalNpcTalkState,f,false));auto bytes=encodeSave(q);
  assert(bytes[4]==(f==5376?9:f==5405 || f==5406?7:f==5407?8:10));
  assert(decodeSave(bytes,out));
  for(int32_t other=5374;other<=5421;++other)assert(troubleQuestFlag(out.originalNpcTalkState,other)==(other==f?std::optional<bool>(false):std::nullopt));
  assert(setTroubleQuestFlag(q.originalNpcTalkState,f,true) && decodeSave(encodeSave(q),out));
  assert(troubleQuestFlag(out.originalNpcTalkState,f)==true);
 }
 for(int32_t b=1469;b<=1474;++b) {
  auto q=p;assert(setTroubleQuestByte(q.originalNpcTalkState,b,-1));assert(decodeSave(encodeSave(q),out));
  for(int32_t other=1469;other<=1474;++other)assert(troubleQuestByte(out.originalNpcTalkState,other)==(other==b?std::optional<uint8_t>(255):std::nullopt));
  assert(setTroubleQuestByte(q.originalNpcTalkState,b,256) && decodeSave(encodeSave(q),out));assert(troubleQuestByte(out.originalNpcTalkState,b)==0);
 }
 auto& s=p.originalNpcTalkState;initializeTroubleQuestState(s);
 for(int32_t f=5374;f<=5421;++f)assert(troubleQuestFlag(s,f)==false);
 for(int32_t b=1469;b<=1474;++b)assert(troubleQuestByte(s,b)==0);
 assert(s.quest1420==0 && s.trouble.valid());
 s.flag5376=true;s.flag5405=true;s.flag5406=true;s.flag5407=true;
 for(int32_t f=5374;f<=5421;++f)assert(setTroubleQuestFlag(s,f,true));
 for(int32_t b=1469;b<=1474;++b)setTroubleQuestByte(s,b,b);
 s.quest1420=13;
 assert(startTroubleQuest(troubleQuestSwitchServices(s))==2);
 assert(s.quest1420==13 && s.flag5376==true && troubleQuestFlag(s,5374)==true);
 for(int32_t f=5375;f<=5404;++f)assert(troubleQuestFlag(s,f)==true);
 for(int32_t f=5405;f<=5421;++f)assert(troubleQuestFlag(s,f)==false);
 for(int32_t b=1469;b<=1474;++b)assert(troubleQuestByte(s,b)==0);
 const auto v10=encodeSave(p);assert(v10[4]==10 && decodeSave(v10,out));
 assert(out.originalNpcTalkState.quest1420==13 && out.originalNpcTalkState.flag5376==true);
 auto snapshot=snapshotSaveProgress(p);setTroubleQuestFlag(s,5374,false);assert(troubleQuestFlag(snapshot.originalNpcTalkState,5374)==true);
 // Both checksum damage and rechecksummed structurally invalid payloads fail
 // without replacing a previously restored state.
 const auto retained=out.originalNpcTalkState.trouble.known;
 for(unsigned at:{1536u,1542u,1548u,1549u,1554u,1555u,1564u,1567u}){auto bad=v10;bad[at]^=1;assert(!decodeSave(bad,out) && out.originalNpcTalkState.trouble.known==retained);}
 for(unsigned bit:{2u,31u,32u,33u}){auto bad=v10;bad[1536+bit/8]|=uint8_t(1u<<(bit%8));sum(bad,1564);assert(!decodeSave(bad,out));}
 {auto bad=v10;bad[1536]&=~1u;sum(bad,1564);assert(!decodeSave(bad,out));} // Value without knowledge.
 {auto bad=v10;bad[1548]|=0x80;sum(bad,1564);assert(!decodeSave(bad,out));}
 {auto bad=v10;bad[1548]&=~1u;bad[1549]=1;sum(bad,1564);assert(!decodeSave(bad,out));}
 {auto bad=v10;bad[1555]=1;sum(bad,1564);assert(!decodeSave(bad,out));}
 auto invalid=p;invalid.originalNpcTalkState.trouble.known|=uint64_t(1)<<48;assert(!decodeSave(encodeSave(invalid),out));
 auto indoor=p;indoor.room=2;indoor.interiorMask=4;indoor.originalStory=23;
 assert(decodeSave(encodeSave(indoor),out) && out.interiorMask==4);
 indoor.originalNpcTalkState.trouble={};assert(encodeSave(indoor)[4]==10 && decodeSave(encodeSave(indoor),out));
 indoor.originalStory=22;assert(!decodeSave(encodeSave(indoor),out));
 indoor.originalStory.reset();assert(!decodeSave(encodeSave(indoor),out));
 const char* path="native-trouble-state.bin";
 assert(writeSave(path,p) && loadSave(path,out)==LoadResult::Loaded);
 {std::ifstream file(path,std::ios::binary|std::ios::ate);assert(file.tellg()==1568);}
 assert(writeSave(path,p));
 for(unsigned size:{40u,1536u,1567u,1569u}) {
  std::ofstream file(path,std::ios::binary|std::ios::trunc);file.write(reinterpret_cast<const char*>(v10.data()),size>1568?1568:size);if(size>1568)file.put(0);file.close();
  assert(loadSave(path,out)==LoadResult::Recovered && troubleQuestFlag(out.originalNpcTalkState,5374)==false);
 }
 // Actual v9 disk load retains selected flags but leaves new fields unknown.
 auto legacy=p;legacy.originalNpcTalkState.trouble={};assert(writeSave(path,legacy));
 assert(loadSave(path,out)==LoadResult::Loaded && out.originalNpcTalkState.flag5376==true && !troubleQuestFlag(out.originalNpcTalkState,5374));
 for(int32_t b=1469;b<=1474;++b)assert(!troubleQuestByte(out.originalNpcTalkState,b));
 std::remove(path);std::remove("native-trouble-state.bin.bak");
}
