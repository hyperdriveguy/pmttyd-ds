#include "battle_vm_message_lookup.hpp"
#include "event_native_runtime.hpp"
#include <cassert>
#include <cstring>
#include <cstdio>
#include <unistd.h>
using namespace ttyd;
namespace {
constexpr uint32_t Key=0x72000000,Cache=0x85b00000,CacheCapacity=0x00400000;
std::vector<uint8_t>resource(std::initializer_list<const char*>strings){std::vector<uint8_t>b;for(const auto*s:strings){const auto n=std::strlen(s)+1;b.insert(b.end(),s,s+n);}b.push_back(0);return b;}
struct Fixture {
 std::shared_ptr<EventNativeRuntime>vm=std::make_shared<EventNativeRuntime>(0x81100000,EventNativePlatform{});
 std::shared_ptr<MessageAssets>assets=std::make_shared<MessageAssets>();std::shared_ptr<BattleVmMessageLookup>lookup;BattleVmMessageLookupCursor cursor;
 std::optional<uint32_t>query(uint32_t key){return lookup->lookup(key,cursor);}
 bool live=true,mapReady=true,reenter=false;unsigned reads=0,maps=0;int missing=-1;uint32_t blockedAddress=0;
 Fixture(){assert(vm->memory.registerRam(Key,resource({"help"})));}
 bool load(const std::vector<uint8_t>&b,bool room=false){
  char path[]="/tmp/ttyd-lookup-XXXXXX";const int fd=mkstemp(path);assert(fd>=0);FILE*f=fdopen(fd,"wb");assert(f);assert(std::fwrite(b.data(),1,b.size(),f)==b.size());std::fclose(f);const bool ok=room?assets->loadRoom(path):assets->loadGlobal(path);std::remove(path);return ok;
 }
 BattleVmMessageLookupBinding binding(){BattleVmMessageLookupBinding b;b.messages=assets;b.vmLifetime=vm;b.addressSpaceIdentity=&vm->memory;b.base=Cache;b.capacity=CacheCapacity;b.maximumKeyBytes=256;
  b.current=[this]{return live;};b.byte=[this](uint32_t a)->std::optional<uint8_t>{++reads;if(reenter&&lookup){assert(!query(Key)&&!lookup->current());}if(a==blockedAddress||int(a-Key)==missing)return {};uint8_t v;return vm->memory.readByte(a,v)?std::optional<uint8_t>(v):std::nullopt;};b.unmapped=[this](uint32_t a,uint64_t n){return vm->memory.unmappedRange(a,n);};b.registerBytes=[this](uint32_t a,std::vector<uint8_t>bytes){++maps;return mapReady&&vm->memory.registerRam(a,std::move(bytes));};return b;
 }
 void create(){lookup=BattleVmMessageLookup::create(binding());assert(lookup);}
 std::string text(uint32_t a){std::string out;for(uint8_t b;;++a){assert(vm->memory.readByte(a,b));if(!b)return out;out.push_back(char(b));}}
};
void exactBytesAndPrecedence(){Fixture f;assert(f.load(resource({"help","<shake>Global\ntext</shake>","other","Other"})));assert(f.load(resource({"help","<col FF0000FF>Room</col>"}),true));f.create();f.reenter=true;auto p=f.query(Key);assert(p&&f.text(*p)=="<col FF0000FF>Room</col>"&&f.maps==1);auto again=f.query(Key);assert(again==p&&f.maps==1);assert(f.lookup->mappedEntries()==1);
 // Actual key pointer reuse must read its current bytes, not memoize address.
 assert(f.vm->memory.writeByte(Key,'o'));assert(f.vm->memory.writeByte(Key+1,'t'));assert(f.vm->memory.writeByte(Key+2,'h'));assert(f.vm->memory.writeByte(Key+3,'e'));assert(f.vm->memory.writeByte(Key+4,'r'));assert(f.vm->memory.writeByte(Key+5,0)); const auto other=f.query(Key);assert(other&&other!=p&&f.text(*other)=="Other"&&f.maps==2);
}
void heldReadsAndMaps(){Fixture f;assert(f.load(resource({"help","A<wait 3>\nB"})));f.create();f.missing=2;assert(!f.query(Key));const auto before=f.reads;assert(!f.query(Key)&&f.reads==before+1);assert(!f.query(Key+1));f.missing=-1;f.mapReady=false;assert(!f.query(Key)&&f.maps==1);const auto complete=f.reads;assert(!f.query(Key)&&f.reads==complete&&f.maps==2);f.mapReady=true;const auto p=f.query(Key);assert(p&&f.text(*p)=="A<wait 3>\nB"&&f.reads==complete&&f.maps==3);}
void everyKeyReadAndLateOverlap(){
 for(int offset=0;offset<5;++offset){Fixture f;assert(f.load(resource({"help","Text"})));f.create();f.missing=offset;assert(!f.query(Key)&&!f.query(Key));f.missing=-1;const auto p=f.query(Key);assert(p&&f.text(*p)=="Text"&&f.reads==7&&f.maps==1);}
 Fixture f;assert(f.load(resource({"help","Text"})));f.create();assert(f.vm->memory.registerRam(Cache,{7}));assert(!f.query(Key)&&f.maps==0);const auto reads=f.reads;assert(!f.query(Key)&&f.reads==reads&&f.maps==0);
 Fixture g;assert(g.load(resource({"help","Text"})));auto b=g.binding();b.registerBytes=[&](uint32_t a,std::vector<uint8_t>bytes){++g.maps;const bool ok=g.vm->memory.registerRam(a,std::move(bytes));g.live=false;return ok;};g.lookup=BattleVmMessageLookup::create(b);assert(g.lookup&&!g.query(Key)&&g.lookup->mappedEntries()==1&&g.maps==1);g.live=true;assert(!g.query(Key)&&g.maps==1);
}
void verifiedExistingAliases(){
 Fixture f;assert(f.load(resource({"help","Same","other","Same"})));constexpr uint32_t Existing=0x85a00000;
 assert(f.vm->memory.registerRam(Existing,resource({"Same"})));auto b=f.binding();b.existing={{"help",Existing}};f.lookup=BattleVmMessageLookup::create(b);assert(f.lookup);const auto pointer=f.query(Key);assert(pointer&&*pointer==Existing&&f.maps==0&&f.lookup->mappedBytes()==0);
 b.existing={{"help",Existing},{"other",Existing}};assert(!BattleVmMessageLookup::create(b));b.existing={{"help",Existing+1}};assert(!BattleVmMessageLookup::create(b));
 assert(f.vm->memory.writeByte(Existing,'X'));b.existing={{"help",Existing}};assert(!BattleVmMessageLookup::create(b));
}
void independentCallerCursors(){
 Fixture f;assert(f.load(resource({"help","Text"})));f.create();f.missing=2;assert(!f.query(Key));
 Fixture other;assert(other.load(resource({"help","Other"})));other.create();assert(!other.lookup->lookup(Key,f.cursor));
 assert(f.vm->memory.registerRam(Key+0x100,resource({"help"})));BattleVmMessageLookupCursor next;const auto pointer=f.lookup->lookup(Key+0x100,next);assert(pointer&&f.text(*pointer)=="Text");
 f.missing=-1;assert(f.query(Key)==pointer&&f.maps==1);
 {BattleVmMessageLookupCursor abandoned;f.blockedAddress=Key+2;assert(!f.lookup->lookup(Key,abandoned));}
 BattleVmMessageLookupCursor replacement;assert(f.lookup->lookup(Key+0x100,replacement)==pointer&&f.maps==1);
}
void snapshotAndMissing(){Fixture f;assert(f.load(resource({"help","Original"})));f.create();assert(f.load(resource({"help","Reloaded"})));const auto p=f.query(Key);assert(p&&f.text(*p)=="Original");f.lookup.reset();assert(f.text(*p)=="Original");
 Fixture absent;assert(absent.load(resource({"other","No substitution"})));absent.create();assert(!absent.query(Key)&&absent.maps==0);
 Fixture duplicate;assert(duplicate.load(resource({"help","One","help","Two"})));duplicate.create();assert(!duplicate.query(Key)&&duplicate.maps==0);
}
void actualOriginalResources(){
 Fixture f;assert(f.assets->loadGlobal("romfs/messages/global.txt"));assert(f.assets->loadRoom("romfs/messages/moo_00.txt"));
 assert(f.vm->memory.registerRam(Key+0x100,std::vector<uint8_t>(256)));f.create();
 for(const char*id:{"btl_hlp_cmd_operation_changeparty","btl_hlp_cmd_operation_charge","btl_hlp_kuriboo","btl_cmd_act_jump","btl_cmd_act_hammer","btl_cmd_act_item"}){const char*original=f.assets->find(id);assert(original);for(size_t i=0;i<=std::strlen(id);++i)assert(f.vm->memory.writeByte(Key+0x100+uint32_t(i),uint8_t(id[i])));const auto value=f.query(Key+0x100);assert(value&&f.text(*value)==original);}
 assert(f.lookup->mappedEntries()==6);
}
void overlapLifetimeAndBound(){Fixture f;assert(f.load(resource({"help","Text"})));assert(f.vm->memory.registerRam(Cache,{7}));assert(!BattleVmMessageLookup::create(f.binding()));
 Fixture g;assert(g.load(resource({"help","Text"})));auto b=g.binding();b.capacity=2;g.lookup=BattleVmMessageLookup::create(b);assert(g.lookup&&!g.query(Key)&&!g.maps);g.live=false;assert(!g.query(Key));g.live=true;assert(!g.lookup->current());
 Fixture h;assert(h.load(resource({"help","Text"})));h.create();h.vm.reset();assert(!h.lookup->current()&&!h.query(Key));
 Fixture shortKey;assert(shortKey.load(resource({"help","Text"})));b=shortKey.binding();b.maximumKeyBytes=3;shortKey.lookup=BattleVmMessageLookup::create(b);assert(shortKey.lookup&&!shortKey.query(Key)&&!shortKey.maps);
}
}
int main(){exactBytesAndPrecedence();heldReadsAndMaps();everyKeyReadAndLateOverlap();verifiedExistingAliases();independentCallerCursors();snapshotAndMissing();actualOriginalResources();overlapLifetimeAndBound();return 0;}
