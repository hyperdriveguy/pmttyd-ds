#include "tutorial_stage_map_files.hpp"
#include "event_native_runtime.hpp"
#include <cassert>
#include <cstdio>
#include <fstream>
#include <filesystem>
#include <chrono>
#include <iterator>
#include <memory>
using namespace ttyd;
namespace {
constexpr uint32_t Table=0x89000000,WorkerStack=0x8b000000,Path=0x8c000000,CallerStack=0x8d000000,Buffer=0x8e000000,BufferBytes=2*1024*1024;
size_t firstStageRow(){return FieldMapFileOwner::originalHarborCatalog().size();}
struct Fixture {
 EventNativeRuntime vm{0x81000000,{}};std::unique_ptr<FieldMapFileOwner> files;
 FieldMapSourceServices s;std::string root;
 explicit Fixture(std::string directory):root(std::move(directory)){
  assert(vm.memory.registerRam(Path,std::vector<uint8_t>(256))&&vm.memory.registerRam(CallerStack,std::vector<uint8_t>(65536))&&vm.memory.registerRam(Buffer,std::vector<uint8_t>(BufferBytes,0xa5)));
  s.user=this;s.word=[](void*p,uint32_t a,uint32_t&v){return static_cast<Fixture*>(p)->vm.memory.readWord(a,v);};s.half=[](void*p,uint32_t a,uint16_t&v){return static_cast<Fixture*>(p)->vm.memory.readHalf(a,v);};s.byte=[](void*p,uint32_t a,uint8_t&v){return static_cast<Fixture*>(p)->vm.memory.readByte(a,v);};s.store=[](void*p,uint32_t a,uint32_t v){return static_cast<Fixture*>(p)->vm.memory.writeWord(a,v);};s.storeHalf=[](void*p,uint32_t a,uint16_t v){return static_cast<Fixture*>(p)->vm.memory.writeHalf(a,v);};s.storeByte=[](void*p,uint32_t a,uint8_t v){return static_cast<Fixture*>(p)->vm.memory.writeByte(a,v);};
  s.call=[](void*p,FieldMapSourceCall&q){auto&f=*static_cast<Fixture*>(p);return f.files&&f.files->service(q);};
 }
 void text(std::string value){value+='\0';for(unsigned i=0;i<value.size();++i)assert(vm.memory.writeByte(Path+i,uint8_t(value[i])));}
 uint32_t run(uint32_t fn,std::array<uint32_t,5>a={}){FieldMapSourceCursor c;for(unsigned n=0;n<100000;++n){assert(files&&files->current());const auto v=resumeFieldMapSource(fn,a,CallerStack+65536-32,s,c);if(v)return *v;}assert(false);return 0;}
};
void actualTransfers(const char* root){
 Fixture f(root);auto rows=tutorialStageMapFileCatalog();assert(rows.size()==firstStageRow()+4);
 const auto& defaults=FieldMapFileOwner::originalHarborCatalog();
 for(size_t i=0;i<defaults.size();++i){const auto&a=rows[i];const auto&b=defaults[i];assert(a.sourcePath==b.sourcePath&&a.relativePath==b.relativePath&&a.logicalBytes==b.logicalBytes&&a.transferBytes==b.transferBytes);}
 const std::array<FieldMapFileRecord,4> expected{{{"./m/bti_01/d","maps/tutorial-stage/bti_01/d",181042,181056},{"./m/bti_01/t","maps/tutorial-stage/bti_01/t",58976,58976},{"./m/stg_00_0/d","maps/tutorial-stage/stg_00_0/d",129290,129312},{"./m/stg_00_0/t","maps/tutorial-stage/stg_00_0/t",119168,119168}}};
 for(size_t i=0;i<expected.size();++i){const auto&a=rows[firstStageRow()+i];const auto&b=expected[i];assert(a.sourcePath==b.sourcePath&&a.relativePath==b.relativePath&&a.logicalBytes==b.logicalBytes&&a.transferBytes==b.transferBytes);}
 f.files=FieldMapFileOwner::bind(f.vm.memory,Table,WorkerStack,root,rows);assert(f.files);
 const auto owned=rows;rows.clear(); // Source jobs use the owner's immutable copy.
 for(size_t i=0;i<owned.size();++i){const auto&r=owned[i];
  f.text(i%2?"./"+r.sourcePath:r.sourcePath);const auto handle=f.run(FieldMapSource::FileOpen,{Path,2,0});assert(handle>=Table&&handle<Table+FieldMapFileOwner::TableBytes);
  assert(f.run(FieldMapSource::FileLength,{handle})==r.logicalBytes);
  f.run(FieldMapSource::FileRead,{handle,Buffer,r.transferBytes,0});
  std::ifstream file(std::string(root)+"/"+r.relativePath,std::ios::binary);assert(file);const std::vector<uint8_t> original((std::istreambuf_iterator<char>(file)),{});assert(original.size()==r.transferBytes);
  for(unsigned n=0;n<original.size();++n){uint8_t value=0;assert(f.vm.memory.readByte(Buffer+n,value)&&value==original[n]);}
  // The original source Close queues retirement; actual worker owns fclose and
  // slot flags. Do not replace it with a fabricated synchronous manager result.
  f.run(FieldMapSource::FileClose,{handle});for(unsigned n=0;!f.files->pump();++n)assert(n<100000);
  uint16_t flags=1;assert(f.vm.memory.readHalf(handle+0x90,flags)&&!flags);
 }
 // Catalog absence is unavailability, not evidence of original FST absence.
 for(const char*name:{"./m/other/d","../outside","/absolute"}){
  f.text(name);FieldMapSourceCursor pending;std::vector<uint8_t> before(FieldMapFileOwner::TableBytes);for(unsigned i=0;i<before.size();++i)assert(f.vm.memory.readByte(Table+i,before[i]));
  for(unsigned attempt=0;attempt<3;++attempt)assert(!resumeFieldMapSource(FieldMapSource::FileOpen,{Path,2,0},CallerStack+65536-32,f.s,pending));
  assert(pending.queued&&pending.request.function==0x8029fdbc);for(unsigned i=0;i<before.size();++i){uint8_t value=0;assert(f.vm.memory.readByte(Table+i,value)&&value==before[i]);}
 }
}
void rejectionBeforeMapping(const char*root){
 auto good=tutorialStageMapFileCatalog();std::vector<FieldMapFileCatalog> bad;
 auto duplicate=good;auto alias=good[firstStageRow()];alias.sourcePath="./"+alias.sourcePath;alias.relativePath="maps/alias/d";duplicate.push_back(alias);bad.push_back(duplicate);
 for(const auto&path:{"../outside","/absolute","maps/../escape","maps//d","maps/./d"}){auto q=good;q[firstStageRow()].relativePath=path;bad.push_back(q);}
 auto rounded=good;rounded[firstStageRow()].transferBytes=rounded[firstStageRow()].logicalBytes;bad.push_back(rounded);
 auto overflow=good;overflow[firstStageRow()].logicalBytes=UINT32_MAX;overflow[firstStageRow()].transferBytes=0;bad.push_back(overflow);
 auto destination=good;destination[firstStageRow()].relativePath=good[0].relativePath;bad.push_back(destination);
 for(const auto&q:bad){Fixture f(root);assert(!FieldMapFileOwner::bind(f.vm.memory,Table,WorkerStack,root,q));uint32_t word=0;assert(!f.vm.memory.readWord(FieldMapFileOwner::Globals,word)&&!f.vm.memory.readWord(Table,word));}
 Fixture missing(root);auto absent=good;absent[firstStageRow()].relativePath="maps/absent/d";assert(!FieldMapFileOwner::bind(missing.vm.memory,Table,WorkerStack,root,absent));uint32_t word=0;assert(!missing.vm.memory.readWord(Table,word));
}
void knownNativeOpenFailure(const char*root){
 const auto temporary=std::filesystem::temp_directory_path()/("pm-stage-catalog-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
 const auto rows=tutorialStageMapFileCatalog();for(const auto&r:rows){const auto destination=temporary/r.relativePath;std::filesystem::create_directories(destination.parent_path());std::filesystem::copy_file(std::filesystem::path(root)/r.relativePath,destination);}
 {Fixture f(temporary.string());f.files=FieldMapFileOwner::bind(f.vm.memory,Table,WorkerStack,f.root,rows);assert(f.files);std::filesystem::remove(temporary/rows[firstStageRow()].relativePath);f.text(rows[firstStageRow()].sourcePath);assert(f.run(FieldMapSource::FileOpen,{Path,2,0})==0);}
 std::filesystem::remove_all(temporary);
}
void compatibilityAndIdentity(const char*root){
 Fixture original(root);original.files=FieldMapFileOwner::bind(original.vm.memory,Table,WorkerStack,root);assert(original.files);for(const auto& row:FieldMapFileOwner::originalHarborCatalog()){original.text(row.sourcePath);const auto handle=original.run(FieldMapSource::FileOpen,{Path,2,0});assert(handle&&original.run(FieldMapSource::FileLength,{handle})==row.logicalBytes);original.run(FieldMapSource::FileClose,{handle});while(!original.files->pump()){} }
 Fixture replacement(root);replacement.files=FieldMapFileOwner::bind(replacement.vm.memory,Table,WorkerStack,root,tutorialStageMapFileCatalog());assert(replacement.files);std::vector<uint8_t> equal(FieldMapFileOwner::TableBytes);for(unsigned i=0;i<equal.size();++i)assert(replacement.vm.memory.readByte(Table+i,equal[i]));assert(replacement.vm.memory.unregisterExternalRam(Table)&&replacement.vm.memory.registerExternalRam(Table,equal.data(),uint32_t(equal.size())));assert(!replacement.files->current());FieldMapSourceCall q;q.function=0x8029fdbc;q.arguments[0]=Path;assert(!replacement.files->service(q)&&!q.result);
}
}
int main(int argc,char**argv){assert(argc==2);actualTransfers(argv[1]);rejectionBeforeMapping(argv[1]);knownNativeOpenFailure(argv[1]);compatibilityAndIdentity(argv[1]);}
