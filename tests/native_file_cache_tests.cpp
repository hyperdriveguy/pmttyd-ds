#include "native_file_cache.hpp"
#include "battle_menu_file_cache.hpp"
#include "animation_initialize.hpp"
#include "event_native_runtime.hpp"
#include <cassert>
#include <cstdio>
#include <map>
using namespace ttyd;
namespace {
std::vector<uint8_t> load(const std::string& p){FILE* f=std::fopen(p.c_str(),"rb");assert(f && !std::fseek(f,0,SEEK_END));auto n=std::ftell(f);assert(n>0);std::rewind(f);std::vector<uint8_t> b(size_t(n),0);assert(std::fread(b.data(),1,b.size(),f)==b.size());std::fclose(f);return b;}
uint32_t rawWord(const std::vector<uint8_t>& b,uint32_t o){return uint32_t(b[o])<<24|uint32_t(b[o+1])<<16|uint32_t(b[o+2])<<8|b[o+3];}
uint32_t word(EventAddressSpace& m,uint32_t a){uint32_t v=0;assert(m.readWord(a,v));return v;}
uint16_t half(EventAddressSpace& m,uint32_t a){uint16_t v=0;assert(m.readHalf(a,v));return v;}
uint8_t byte(EventAddressSpace& m,uint32_t a){uint8_t v=0;assert(m.readByte(a,v));return v;}
uint32_t payload(EventAddressSpace& m,uint32_t h){return word(m,word(m,h+0xa0));}
std::string text(EventAddressSpace& m,uint32_t a){std::string s;for(unsigned i=0;i<128;++i){auto c=byte(m,a+i);if(!c)return s;s+=char(c);}assert(false);return {};}
void invalid(const char* root){
 auto c=NativeFileCache::create();assert(c);
 std::vector<NativeFileCache::Source> batch;batch.push_back({"valid",0,{1}});batch.push_back({"bad",5,{1}});
 assert(!c->stage(std::move(batch)) && c->stage("valid",0,{1}) && !c->stage("valid",0,{2}));
 assert(!c->stage(std::string(128,'a'),0,{1}) && !c->stage(std::string("a\0b",3),0,{1}) && !c->stage("empty",0,{}));
 auto raw=load(std::string(root)+"/field-player-c_pkr.draw-model.bin");
 raw[0x148]=0x7f;assert(!c->stage("bad-model",5,raw));
 std::vector<uint8_t> endless(12+65536*8);endless[1]=0x20;endless[2]=0xaf;endless[3]=0x30;endless[5]=1;endless[11]=12;
 assert(!c->stage("nonterminating-tpl",4,std::move(endless)));
 auto d=NativeFileCache::create();assert(!stagePrologueAnimationFiles(*d,"/no/romfs") && d->stage("a/a_mario",0,{1}));
}
// This fixture supplies cold W, heap return addresses, scalar constants and
// formatting/copy services explicitly. File pool/entries/holders/payloads and
// all acquisition/free/reference effects are the real shared production owner.
struct Cold {
 EventNativeRuntime& vm;std::shared_ptr<NativeFileCache> files;uint32_t next=0x8a000000;
 void run(){auto& m=vm.memory;constexpr uint32_t W=0x803d9470;
  assert(m.registerRam(W,std::vector<uint8_t>(0x110+178*4)) && m.registerRam(0x80415da0,std::vector<uint8_t>(0x24)) && m.writeWord(0x80415dc0,W));
  assert(m.registerRam(0x80305900,std::vector<uint8_t>(0xc00)) && m.writeWord(0x80415db4,0x80305900));
  assert(m.registerRam(0x8041fbb4,std::vector<uint8_t>(4)) && m.writeWord(0x8041fbb4,0x3c8efa33));
  assert(m.registerRam(0x802c1070,std::vector<uint8_t>(8)) && m.writeWord(0x802c1070,0x43300000) && m.writeWord(0x802c1074,0x80000000));
  std::vector<uint8_t> literals(32);const char* names[]={"a/vivian.bin","a/ag2tg.bin"};for(unsigned j=0;j<2;++j)for(unsigned i=0;names[j][i];++i)literals[16*j+i]=names[j][i];assert(m.registerRam(0x802c11d8,std::move(literals)));
  AnimationInitializeServices s;s.memory.user=this;
  s.memory.word=[](void* p,uint32_t a)->std::optional<uint32_t>{uint32_t v=0;return static_cast<Cold*>(p)->vm.memory.readWord(a,v)?std::optional<uint32_t>(v):std::nullopt;};
  s.memory.store=[](void* p,uint32_t a,uint32_t v){return static_cast<Cold*>(p)->vm.memory.writeWord(a,v);};
  s.memory.call=[](void* p,BattleSequenceCall& call){auto& c=*static_cast<Cold*>(p);auto& m=c.vm.memory;
   if(call.function==0x800300f0){assert(call.args[0]==0);call.result=c.next;c.next+=call.args[1]+0x100;return m.registerRam(call.result,std::vector<uint8_t>(call.args[1]));}
   if(call.function==0x8018a61c){assert(call.args[0]==0);auto h=c.files->acquire(text(m,call.args[1]),0);if(!h)return false;call.result=*h;return true;}
   if(call.function==0x8018a260)return c.files->release(call.args[0]);
   if(call.function==0x80189d38)return c.files->archiveType(call.args[0]);
   if(call.function!=0x80005634)return false;
   assert(!(call.args[0]&3) && !(call.args[1]&3) && call.args[2]>=4 && !(call.args[2]&3));
   for(uint32_t o=0;o<call.args[2];o+=4){uint32_t v=0;assert(m.readWord(call.args[1]+o,v) && m.writeWord(call.args[0]+o,v));}return true;};
  s.doubleWord=[](void* p,uint32_t a)->std::optional<uint64_t>{auto& m=static_cast<Cold*>(p)->vm.memory;uint32_t hi=0,lo=0;return m.readWord(a,hi) && m.readWord(a+4,lo)?std::optional<uint64_t>(uint64_t(hi)<<32|lo):std::nullopt;};
  s.tangent=nativeAnimationTangent;AnimationInitializeCursor cursor;assert(initializeAnimation(s,cursor));
 }
};
}
void runNativeFileCacheFixture(const char* root){
 invalid(root);auto menu=std::make_shared<BattleMenuFileCache>();assert(menu->load((std::string(root)+"/battle-class-common.tpl").c_str()));auto files=menu->files();assert(files && stagePrologueAnimationFiles(*files,root));
 bool yieldReady=false;assert(files->yieldProvider(&yieldReady,[](void* p){return *static_cast<bool*>(p);}));
 EventNativeRuntime first(0x81000000,{}),second(0x81000000,{});assert(menu->bind(first.memory) && files->bind(second.memory) && !files->bind(first.memory));auto& m=first.memory;
 assert(!files->stage("late",0,{1}) && !files->acquire("./a/c_pkr",5));
 const auto common=menu->acquire("./battle/common/battle_common.tpl",4);assert(common && payload(m,*common)==0x87100000);
 std::map<std::string,uint32_t> textures;
 const char* models[]={"a_mario","a_mario_r","e_mario","b_mario","c_pkr","c_kuri_h"};
 uint32_t last=0;
 for(auto model:models){
  const auto path=std::string("a/")+model;auto h=files->acquire(path,5);assert(h && *h!=*common && byte(m,*h+1)==5 && half(m,*h+2)==1);auto b=payload(m,*h);assert(b && word(m,b+0x14c)>=b && text(m,b+4)==model);
  if(std::string(model)=="e_mario" || std::string(model)=="b_mario"){
   const auto pack=std::string(model)=="e_mario"?"field-player-e_mario":"trio-attack-mario";auto raw=load(std::string(root)+"/"+pack+".draw-model.bin");unsigned sentinels=0;
   for(unsigned i=0;i<rawWord(raw,0x148);++i){auto c=rawWord(raw,rawWord(raw,0x1ac)+64*i+0x3c);if(rawWord(raw,c+0x30)==0xffbc8fc0u){assert(word(m,b+c+0x30)==uint32_t(b+c+0xffbc8fc0u));++sentinels;}}
   assert(sentinels);
  }
  const auto texture=std::string("a/")+text(m,b+0x44)+"-";auto t=files->acquire(texture,4);assert(t && byte(m,*t+1)==4 && word(m,payload(m,*t))==0x0020af30);
  if(textures.count(texture))assert(textures[texture]==*t);else textures[texture]=*t;
  assert(files->acquire(path,0)==h && half(second.memory,*h+2)==2); // Source filename-only cache hit ignores new type.
  assert(files->release(*h) && files->release(*h) && !half(m,*h+2) && byte(m,*h)==2);last=*h;
 }
 assert(textures.size()==4 && half(m,textures["a/a_mario-"]+2)==3);
 Cold cold{first,files};cold.run();const auto ag=word(m,0x803d9470+0x100);assert(ag && byte(m,ag+1)==0 && half(m,ag+2)==1);
 auto vivian=load(std::string(root)+"/animation-startup/a/vivian.bin");for(unsigned i=0;i<0xc00;++i)assert(byte(m,0x80305900+i)==vivian[i]);for(unsigned i=0;i<16;++i)assert(byte(m,0x80415da0+i)==vivian[0xc00+i]);
 auto agBytes=load(std::string(root)+"/animation-startup/a/ag2tg.bin");auto agPayload=payload(m,ag);for(unsigned i=0;i<agBytes.size();++i)assert(byte(second.memory,agPayload+i)==agBytes[i]);
 auto v=files->acquire("a/vivian.bin",0);assert(v && half(m,*v+2)==1 && files->release(*v) && byte(m,*v)==2);
 assert(m.writeWord(*v+0xac,1) && !files->acquire("a/vivian.bin",0) && !files->acquire("a/ag2tg.bin",0));yieldReady=true;assert(m.writeWord(*v+0xac,0) && files->acquire("a/vivian.bin",0)==v && half(m,*v+2)==1);
 assert(word(m,NativeFileCache::Manager+0xc)==ag && word(m,NativeFileCache::Manager+0x10)==0x87000000+13*0xb0);
 assert(files->archiveType(2) && word(m,NativeFileCache::Manager+4)==2 && files->acquire("a/ag2tg.bin",0)==ag && payload(m,ag)==agPayload && half(m,ag+2)==2);
 assert(m.writeWord(NativeFileCache::Global,0) && !files->acquire("a/c_pkr",5) && !files->release(last) && !files->archiveType(0));assert(m.writeWord(NativeFileCache::Global,NativeFileCache::Manager));
 EventNativeRuntime conflict(0x81000000,{});assert(conflict.memory.registerRam(0x87500000,{7}));assert(!files->bind(conflict.memory));uint32_t missing=0;assert(!conflict.memory.readWord(NativeFileCache::Global,missing) && byte(conflict.memory,0x87500000)==7 && half(second.memory,ag+2)==2);
 assert(files->release(0) && !files->release(ag+1));
}
int main(){runNativeFileCacheFixture("romfs");}
