#include "animation_startup_process.hpp"
#include "field_npc_clock.hpp"
#include <cassert>
#include <cstring>
using namespace ttyd;
namespace {
uint32_t word(EventAddressSpace& m,uint32_t a){uint32_t v=0;assert(m.readWord(a,v));return v;}
std::shared_ptr<const BattlePoseResource> resource(const char* root,const char* stem){const auto path=std::string(root)+"/"+stem;return loadBattlePoseResource((path+".pose-directory.bin").c_str(),(path+".pmanim").c_str());}
}
void runAnimationSourceEntryFixture(const char* root){
 auto files=NativeFileCache::create();auto pool=std::make_shared<BattlePosePool>();assert(stagePrologueAnimationFiles(*files,root));
 bool yield=false;assert(files->yieldProvider(&yield,[](void* p){return *static_cast<bool*>(p);}));
 EventNativePlatform platform;platform.flushBlock=[](void*,uint32_t){};platform.systemCall=[](void*){};
 auto process=AnimationStartupProcess::cold(files,pool,platform,(std::string(root)+"/animation-boot/memory.pmevt").c_str(),0x81000000,0x82000000,12*1024*1024);assert(process && process->advance());auto& m=process->vm().memory;
 const auto gp=word(m,0x80415fd0),work=word(m,0x80415dc0),scratch=word(m,work+0x104);
 // Caller-owned source name memory, not fabricated SDK file/model headers.
 assert(m.registerRam(0x81800000,std::vector<uint8_t>(1024)));
 assert(m.registerRam(0x800000f8,{0x09,0xa7,0xec,0x80})); // Explicit native/test bus clock162MHz, not an ISO initial value.
 auto name=[&](uint32_t a,const std::string& s){for(unsigned i=0;i<=s.size();++i)assert(m.writeByte(a+i,i<s.size()?uint8_t(s[i]):0));};
 const auto mario=resource(root,"field-player-a_mario"),rear=resource(root,"field-player-a_mario_r"),event=resource(root,"field-player-e_mario"),battle=resource(root,"trio-attack-mario"),partner=resource(root,"field-player-c_pkr"),goomba=resource(root,"field-native-c_kuri_h");assert(mario && rear && event && battle && partner && goomba);
 name(0x81800000,mario->modelName);name(0x81800040,rear->modelName);name(0x81800080,event->modelName);name(0x818000c0,battle->modelName);name(0x81800100,partner->modelName);name(0x81800140,goomba->modelName);
 assert(m.writeWord(gp+0x40,0) && m.writeWord(gp+0x44,40500000) && m.writeWord(gp+0x38,0xffffffff) && m.writeWord(gp+0x3c,uint32_t(-40500000)));
 // Warm actual original MODEL, then hold its source pending byte and Yield.
 auto file=files->acquire("a/a_mario",5);assert(file && files->release(*file) && m.writeWord(*file+0xac,1));
 const auto rawPose=process->animationMemory()->poseAddress(0);
 assert(m.writeWord(rawPose+0x78,0x1234abcd)); // Entry must retain unmentioned stale fields.
 assert(!process->enter(0x81800000,0,mario) && process->entryPending()==0x8018a61c);
 assert(word(m,rawPose)==1 && pool->models()[0].references()==1 && !pool->models()[0].resource);
 assert(!process->enter(0x81800100,0,partner)); // Cannot replace the held request.
 // Source fileAllocf already captured NAME before Yield; no formatting replay.
 assert(m.writeByte(0x81800000,'X') && m.writeWord(*file+0xac,0));
 assert(!process->enter(0x81800000,0,mario) && process->entryPending()==0x8018a61c);
 yield=true;assert(process->enter(0x81800000,0,mario)==0);name(0x81800000,mario->modelName);
 auto* p=pool->pose(0);assert(p && pool->cacheMetadataReady() && p->word(0x78)==0x1234abcd && p->word(0x10)==0 && p->word(0x14)==0 && p->word(0x120)==0x80415dc4 && p->word(0x48)==scratch && p->word(0x48)!=0 && p->word(0x58)==p->word(0x5c) && p->word(0x68)==p->word(0x6c));
 const auto bus=word(m,0x800000f8),divisor=uint32_t((uint64_t(0x10624dd3)*(bus>>2))>>32)>>6;uint64_t positive=0,negative=0;assert(nativeFieldNpcClockDivide(nullptr,40500000,divisor,positive) && nativeFieldNpcClockDivide(nullptr,uint64_t(-int64_t(40500000)),divisor,negative));
 assert(p->timestamp()==positive && pool->models()[0].file()==*file && pool->models()[0].references()==1 && pool->textures()[0].references()==1);
 // Same model increments MODEL refs only; a different model sharing its TPL
 // increments TEXTURE refs without a second file acquisition reference.
 assert(process->enter(0x81800000,2,mario)==1 && pool->models()[0].references()==2 && pool->textures()[0].references()==1 && pool->pose(1)->timestamp()==negative);
 assert(process->enter(0x81800080,2,event)==2 && process->enter(0x818000c0,1,battle)==3);
 assert(pool->models()[1].texture()==0 && pool->models()[2].texture()==0 && pool->textures()[0].references()==3);
 assert(process->enter(0x81800040,2,rear)==4 && process->enter(0x81800100,2,partner)==5 && process->enter(0x81800140,2,goomba)==6 && pool->cacheMetadataReady());
 for(unsigned h=0;h<7;++h){auto* q=pool->pose(h);assert(q && q->word(0x48) && q->word(0x90)==0xffffffff && q->word(0xf0)==0xffffffff && q->word(0xf4)==0xffffffff && q->word(0x44)==0x3f800000 && q->word(0x124)==0x3f800000 && q->word(0x154)==0 && q->word(0x158)==0);}
 for(unsigned i=0;i<6;++i){const auto& r=pool->models()[i];assert(r.flags() && r.file() && r.resource);const auto t=r.texture();assert(pool->textures()[t].file() && pool->textures()[t].resource);}
 const auto now=BattlePoseClock{positive,uint64_t(negative)};
 assert(pool->selectName(5,"PKR_W_1",true,now) && pool->evaluate(5,{positive,uint64_t(negative)+17}) && !pool->pose(5)->shape.positions.empty());
 assert(word(m,rawPose+0x78)==0x1234abcd && word(m,work+0x104)==scratch && process->ready());
}
namespace {
void failureCases(const char* root){
 for(unsigned failure=0;failure<3;++failure){
  auto files=NativeFileCache::create();auto pool=std::make_shared<BattlePosePool>();assert(stagePrologueAnimationFiles(*files,root));
  EventNativePlatform platform;platform.flushBlock=[](void*,uint32_t){};platform.systemCall=[](void*){};
  auto process=AnimationStartupProcess::cold(files,pool,platform,(std::string(root)+"/animation-boot/memory.pmevt").c_str(),0x81000000,0x82000000,12*1024*1024);assert(process && process->advance());auto& m=process->vm().memory;
  assert(m.registerRam(0x81800000,{'a','_','m','a','r','i','o',0}) && m.registerRam(0x800000f8,{0x09,0xa7,0xec,0x80}));
  struct Fault {
   EventAddressSpace& memory;NativeFileCache& files;unsigned failure;uint32_t model=0,holder=0,payload=0,freeCalls=0,bufferCalls=0;
   std::string text(uint32_t a){std::string s;for(unsigned i=0;i<256;++i){uint8_t b=0;assert(memory.readByte(a+i,b));if(!b)return s;s+=char(b);}assert(false);return {};}
  } f{m,*files,failure};
  AnimationEntryServices services;services.memory.user=&f;
  services.memory.word=[](void* p,uint32_t a)->std::optional<uint32_t>{uint32_t v=0;return static_cast<Fault*>(p)->memory.readWord(a,v)?std::optional<uint32_t>(v):std::nullopt;};
  services.memory.store=[](void* p,uint32_t a,uint32_t v){return static_cast<Fault*>(p)->memory.writeWord(a,v);};
  services.call=[](void* p,AnimationEntryCall& c){auto& f=*static_cast<Fault*>(p);
   switch(c.function){
   case 0x80189d38:return f.files.archiveType(c.args[0]);
   case 0x8018a61c:
    assert(c.args[0]==5);if(f.failure==0){c.result=0;return true;} // Explicit model-load failure injection.
    {auto h=f.files.acquire("a/a_mario",5);assert(h);f.model=*h;f.holder=word(f.memory,*h+0xa0);f.payload=word(f.memory,f.holder);assert(f.payload);if(f.failure==1)assert(f.memory.writeWord(f.holder,0));c.result=*h;return true;}
   case 0x8018a260:++f.freeCalls;return f.files.release(c.args[0]);
   case 0x8026a0b8:assert(c.stackArguments==1);c.text=f.text(c.args[2])+"/"+f.text(c.args[3])+"-";return true;
   case 0x8018a5f8:assert(c.stackArguments==1 && c.text=="a/a_mario-" && c.args[1]==4);c.result=0;return true; // Explicit texture-load failure injection.
   case 0x80264ad8:{uint64_t q=0;assert(nativeFieldNpcClockDivide(nullptr,uint64_t(c.args[0])<<32|c.args[1],c.args[3],q));c.result=uint32_t(q>>32);c.low=uint32_t(q);return true;}
   case 0x80041ee0:++f.bufferCalls;return false; // Observe genuine continuation beyond texture-null; no dummy buffer success.
   default:assert(false);return false;
   }
  };
  AnimationEntryCursor cursor;const bool complete=enterAnimationPose(0x81800000,2,services,cursor);const auto work=word(m,0x80415dc0),pose=word(m,work+0x10),model=word(m,work),texture=word(m,work+8);
  if(failure<2){assert(complete && cursor.result==0xfffffffe && !word(m,pose) && word(m,pose+0x10)==0xfffffffe && !word(m,model) && !word(m,model+4) && f.freeCalls==failure && !f.bufferCalls);if(failure==1){assert(word(m,model+8)==f.model && m.writeWord(f.holder,f.payload));}}
  else{assert(!complete && cursor.pending==0x80041ee0 && f.bufferCalls==1 && !f.freeCalls && word(m,pose)==1 && word(m,pose+0x10)==0 && word(m,model)==1 && word(m,model+4)==1 && word(m,texture)==1 && word(m,texture+4)==1 && word(m,texture+8)==0);assert(!enterAnimationPose(0x81800000,2,services,cursor) && f.bufferCalls==2 && word(m,model+4)==1 && word(m,texture+4)==1);}
 }
}
}

int main(){runAnimationSourceEntryFixture("romfs");failureCases("romfs");}
