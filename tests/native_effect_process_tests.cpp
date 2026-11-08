#include "native_effect_process.hpp"
#include "native_ui_startup.hpp"
#include <cassert>
#include <cmath>
#include <cstdio>
#include <vector>
using namespace ttyd;
namespace {
constexpr uint32_t Manager=0x8041e908,ManagerPointer=0x80415ee0,RecordBytes=40,PayloadBytes=88;
constexpr uint32_t FukidashiMain=0x80132ea8;
uint32_t word(EventAddressSpace& m,uint32_t a){uint32_t n=0;assert(m.readWord(a,n));return n;}
void put(EventAddressSpace& m,uint32_t a,uint32_t n){assert(m.writeWord(a,n));}
EventNativePlatform platform(){EventNativePlatform p;p.flushBlock=[](void*,uint32_t){};p.systemCall=[](void*){};p.truncate=[](void*,float f){return int32_t(f);};return p;}
struct F {
 std::shared_ptr<NativeFieldAnimation> animation;std::shared_ptr<NativeUiStartup> ui;std::shared_ptr<NativeEffectProcess> effects;
 F(){animation=NativeFieldAnimation::create("romfs",NativeFileCache::create(),platform());assert(animation&&animation->ready());ui=NativeUiStartup::create(animation,"romfs");assert(ui&&ui->resumeFont());effects=NativeEffectProcess::create(ui,"romfs");assert(effects&&effects->current()&&!effects->poolReady());}
 EventAddressSpace& memory(){return animation->process().vm().memory;}
 uint32_t bank(){return word(memory(),Manager+4);}
 uint32_t actor(){auto services=effects->constructionServices();const auto e=effectPoolEntry(services);assert(e);const auto payload=services.heapAllocate(services.user,3,PayloadBytes);assert(payload&&*payload);put(memory(),*e+0xc,*payload);put(memory(),*e+0x10,FukidashiMain);return *e;}
};
void actualInitAndFileDriver(){
 F f;auto& m=f.memory();assert(!f.effects->constructionServices().memory.word);
 const auto gp=word(m,0x80415fd0),heap0=word(m,0x8041e844),heap3=word(m,0x8041e850);
 assert(heap0!=heap3&&word(m,ManagerPointer)==Manager&&word(m,Manager)==0&&word(m,0x8041f494)==0x2e000000);
 assert(f.effects->initialize()&&f.effects->poolReady()&&word(m,Manager)==512&&f.bank()!=0);
 const auto bank=f.bank();for(uint32_t n=0;n<512*RecordBytes;n+=4)assert(word(m,bank+n)==0);
 assert(word(m,Manager+0x14)==0xffffffff&&word(m,0x8041ecd0)==0&&word(m,0x8041ecd4)==0);
 assert(f.effects->initialize()&&f.bank()==bank);assert(!NativeEffectProcess::create(f.ui,"romfs"));
 assert(f.animation->files()->archiveType(2)); // Explicit TEST archive-mode history, distinct from resource type4.
 const auto archive=word(m,NativeFileCache::Manager+4);
 NativeEffectUpdate noCallbacks;assert(f.effects->main(&f,1,noCallbacks));
 assert(word(m,NativeFileCache::Manager+4)==archive);
 assert(word(m,gp+0x16c)==1&&word(m,Manager+0x14)==0xffffffff&&word(m,Manager+0x10)==0&&f.animation->files()->asyncPending());
 assert(!f.effects->pumpFiles(0)&&f.effects->pumpFiles(106880));
 assert(f.effects->main(&f,2,noCallbacks)&&word(m,Manager+0x14)==1&&word(m,NativeFileCache::Manager+4)==archive);const auto handle=word(m,Manager+0x10);assert(handle);
 uint16_t refs=0;assert(m.readHalf(handle+2,refs)&&refs==1);
 assert(f.effects->main(&f,2,noCallbacks)&&m.readHalf(handle+2,refs)&&refs==1);
 assert(!f.effects->main(&f,1,noCallbacks)&&!f.effects->main(&m,3,noCallbacks));
 assert(word(m,Manager+8)==0&&word(m,Manager+0xc)==0); // Primary texture setup is a distinct owner.
}
struct Backend {
 F* f=nullptr;bool ready=false,live=true;unsigned attempts=0;uint32_t first=0,later=0;
 std::vector<uint32_t> completed;
 // TEST-only callback receipt records source driver dispatch, not an actual
 // fukidashi update/display. Record/payload pointers are real source allocations.
 static bool update(void* p,uint32_t callback,uint32_t effect){auto& b=*static_cast<Backend*>(p);++b.attempts;assert(callback==FukidashiMain);
  assert(!b.f->effects->initialize()&&!b.f->effects->retire());
  if(!b.ready)return false;
  b.completed.push_back(effect);
  if(effect==b.first){b.f->animation->process().vm().gp.battleContext=1;b.later=b.f->actor();}
  return true;
 }
};
void heldCallbacksAndExactRelease(){
 F f;assert(f.effects->initialize());auto& m=f.memory();const auto first=f.actor(),other=f.actor();
 Backend b;b.f=&f;b.first=first;auto life=std::make_shared<int>(1);
 NativeEffectUpdate update{&b,life,[](void* p){return static_cast<Backend*>(p)->live;},Backend::update};
 assert(!f.effects->main(&f,1,update)&&f.effects->mainPending()&&f.effects->pending()==FukidashiMain&&b.attempts==1);
 auto changed=update;changed.user=nullptr;assert(!f.effects->main(&f,1,changed)&&!f.effects->main(&f,2,update)&&b.attempts==1);
 b.live=false;assert(!f.effects->main(&f,1,update)&&b.attempts==1);b.live=b.ready=true;
 assert(f.effects->main(&f,1,update)&&!f.effects->mainPending());assert(b.completed==std::vector<uint32_t>({first,b.later})&&b.later==other+RecordBytes);
 assert(f.effects->main(&f,1,update)&&b.completed.size()==2);
 const auto payload=word(m,b.later+0xc);bool drained=false;
 NativeEffectRetirement release{&drained,life,[](void* p){return *static_cast<bool*>(p);}};
 assert(!f.effects->autoRelease(1,release)&&f.effects->pending()==0x8005ecd0&&word(m,b.later)==1);
 assert(!f.effects->main(&f,2,update)&&!f.effects->autoRelease(0,release));
 drained=true;assert(f.effects->autoRelease(1,release)&&word(m,b.later)==0&&word(m,b.later+0xc)==payload&&word(m,first)==1&&word(m,other)==1);
 // Field context0 records remain. Callback dispatch never acknowledged their
 // cleanup; source auto-release frees only exact requested context1 payloads.
 assert(f.effects->retire()&&!f.effects->current()&&!f.effects->main(&f,2,update));assert(!NativeEffectProcess::create(f.ui,"romfs"));
}
void publishedFilenameUsesUnsignedBytes(){
 auto files=NativeFileCache::create();const std::string name="a/"+std::string(1,char(0xe9));
 assert(files->stage(name,0,{1,2,3,4}));EventNativeRuntime vm{0x81000000,{}};assert(files->bind(vm.memory));
 assert(files->publishFilename(name));const auto handle=files->acquire(name,0,true);assert(handle&&*handle);
 // Byte-preserving source filenames must compare equally on signed-char hosts.
 assert(files->publishFilename(name)&&files->request(name,0,0,true)==handle);
}
void unsupportedLanguageRetainsRealFilename(){
 F f;assert(f.effects->initialize());auto& m=f.memory();const auto gp=word(m,0x80415fd0);
 put(m,gp+0x16c,2);NativeEffectUpdate absent;
 assert(!f.effects->main(&f,1,absent)&&f.effects->pending()==0x8018a0e8);
 const auto filename=[&](){std::string value;for(unsigned n=0;n<1024;++n){uint8_t b=0;assert(m.readByte(0x803e6e78+n,b));if(!b)return value;value+=char(b);}assert(false);return value;};
 assert(filename()=="./e/ge/effect_ge.tpl");
 put(m,gp+0x16c,1);assert(!f.effects->main(&f,1,absent)&&filename()=="./e/ge/effect_ge.tpl");
 assert(f.animation->files()->publishFilename("other-owner"));
 assert(!f.effects->main(&f,1,absent)&&filename()=="other-owner"); // No repeated formatter/filename repair across a held backend.
}
void replacedColdStorageAndMissingUpdate(){
 F f;assert(f.effects->initialize());const auto e=f.actor();NativeEffectUpdate absent;
 assert(!f.effects->main(&f,1,absent)&&f.effects->pending()==FukidashiMain&&word(f.memory(),e)==1);
 F replaced;assert(replaced.effects->initialize());auto& m=replaced.memory();std::vector<uint8_t> same(24);for(unsigned n=0;n<same.size();++n)assert(m.readByte(Manager+n,same[n]));
 assert(m.unregisterRam(Manager)&&m.registerRam(Manager,std::move(same)));
 assert(!replaced.effects->current()&&!replaced.effects->initialize()&&!replaced.effects->constructionServices().memory.word);
}
}
int main(){actualInitAndFileDriver();heldCallbacksAndExactRelease();publishedFilenameUsesUnsignedBytes();unsupportedLanguageRetainsRealFilename();replacedColdStorageAndMissingUpdate();}
