#include "animation_group_prefetch.hpp"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <map>
namespace ttyd {
// Source archive1, type 5 group/type 4 texture, flags 0. ag2tg scans32-byte
// strings; matching row's NEXT row supplies the texture bank. Completion token
// -1 is source success and distinct from missing native files(return0/hold).
bool animationGroupBaseAsync(const char* name,const std::vector<uint8_t>& table,const AnimationGroupAsyncServices& s) {
 if(!name || !s.archive || !s.request)return false;
 s.archive(s.user,1);
 const auto group=s.request(s.user,5,0,std::string("a/")+name);
 std::string texture;
 for(size_t offset=0;offset<table.size();offset+=32) {
  // Native validation avoids the original unbounded strcmp/out-of-range row.
  if(table.size()-offset<32)return false;
  const auto* row=reinterpret_cast<const char*>(table.data()+offset);
  const auto* end=static_cast<const char*>(std::memchr(row,0,32));if(!end)return false;
  if(std::string(row,end)==name) {
   if(table.size()-offset<64)return false;
   row+=32;end=static_cast<const char*>(std::memchr(row,0,32));if(!end)return false;
   texture.assign(row,end);break;
  }
 }
 const auto pixels=s.request(s.user,4,0,"a/"+texture+"-");
 return group==-1 || (group!=0 && (pixels==-1 || pixels!=0));
}
// waitCount0/1/100 are private phase markers, not elapsed ticks. Poll p_dokan_x
// then p_dokan_y on separate visits; EVT returns0 yield/2 done.
int32_t fieldBeroReadMarioPera(EventEntry& e,bool first,const AnimationGroupPollServices& s) {
 if(first)e.waitCount=0;
 if(e.waitCount==0) {if(s.poll && s.poll(s.user,"p_dokan_x"))e.waitCount=1;}
 else if(e.waitCount==1) {if(s.poll && s.poll(s.user,"p_dokan_y"))e.waitCount=100;}
 return e.waitCount==100?2:0;
}
struct AnimationGroupPrefetch::Impl {
 // Each cached payload owns stream/storage. Opening yields once; subsequent
 // polls read at most chunk bytes per FILE, not per outer event visit.
 struct File {
  FILE* stream=nullptr;std::vector<uint8_t> bytes;size_t loaded=0;bool ready=false;
  ~File(){if(stream)std::fclose(stream);}
 };
 std::string root;std::vector<uint8_t> table;std::map<std::string,std::unique_ptr<File>> files;
 uint32_t chunk=4096,archive=0;bool failed=false;
 int32_t request(uint32_t type,uint32_t flags,const std::string& path) {
  // Only archive1 mode0 has a bound native source directory. Missing files
  // hold with diagnostics, never become the source -1 completion branch.
  if(archive!=1 || flags || (type!=4 && type!=5) || path.rfind("a/",0)!=0 || path.find("..")!=std::string::npos){failed=true;return 0;}
  auto i=files.find(path);
  if(i==files.end()) {
   auto file=std::make_unique<File>();file->stream=std::fopen((root+"/"+path).c_str(),"rb");
   if(!file->stream){failed=true;files.emplace(path,std::move(file));return 0;}
   if(std::fseek(file->stream,0,SEEK_END)!=0){failed=true;files.emplace(path,std::move(file));return 0;}
   const auto size=std::ftell(file->stream);
   if(size<=0 || size>4*1024*1024 || std::fseek(file->stream,0,SEEK_SET)!=0){failed=true;files.emplace(path,std::move(file));return 0;}
   file->bytes.resize(size);files.emplace(path,std::move(file));return 0;
  }
  auto& file=*i->second;if(file.ready)return 1;
  if(!file.stream || file.bytes.empty())return 0;
  const auto count=std::min<size_t>(chunk,file.bytes.size()-file.loaded);
  if(std::fread(file.bytes.data()+file.loaded,1,count,file.stream)!=count){failed=true;std::fclose(file.stream);file.stream=nullptr;return 0;}
  file.loaded+=count;
  if(file.loaded==file.bytes.size()) {
   if(std::fgetc(file.stream)!=EOF || std::ferror(file.stream)){failed=true;std::fclose(file.stream);file.stream=nullptr;return 0;}
   file.ready=true;std::fclose(file.stream);file.stream=nullptr;
  }
  return file.ready?1:0; // Native completed token, not a guest FileEntry pointer.
 }
};
AnimationGroupPrefetch::AnimationGroupPrefetch()=default;
AnimationGroupPrefetch::~AnimationGroupPrefetch()=default;
// Native caps: chunk<=64KiB, table<=1MiB with 32-byte rows, each payload
// <=4MiB. Completed bytes remain resident; no pose creation/refcount happens here.
bool AnimationGroupPrefetch::open(const char* root,uint32_t chunk) {
 if(impl_ || !root || !chunk || chunk>65536)return false;
 auto next=std::make_unique<Impl>();next->root=root;next->chunk=chunk;
 FILE* f=std::fopen((next->root+"/a/ag2tg.bin").c_str(),"rb");if(!f)return false;
 bool ok=std::fseek(f,0,SEEK_END)==0;const auto size=ok?std::ftell(f):-1;
 ok=ok && size>0 && size<=1024*1024 && size%32==0 && std::fseek(f,0,SEEK_SET)==0;
 if(ok){next->table.resize(size);ok=std::fread(next->table.data(),1,size,f)==size_t(size);}
 std::fclose(f);
 if(!ok)return false;
 for(size_t n=0;n<next->table.size();n+=32)if(!std::memchr(next->table.data()+n,0,32))return false;
 impl_=std::move(next);return true;
}
bool AnimationGroupPrefetch::poll(const char* group) {
 if(!impl_)return false;
 AnimationGroupAsyncServices s;s.user=impl_.get();
 s.archive=[](void* p,uint32_t a){static_cast<Impl*>(p)->archive=a;};
 s.request=[](void* p,uint32_t t,uint32_t f,const std::string& path){return static_cast<Impl*>(p)->request(t,f,path);};
 return animationGroupBaseAsync(group,impl_->table,s);
}
const std::vector<uint8_t>* AnimationGroupPrefetch::resident(const std::string& path) const {
 if(!impl_)return nullptr;
 const auto i=impl_->files.find(path);return i!=impl_->files.end() && i->second->ready?&i->second->bytes:nullptr;
}
uint32_t AnimationGroupPrefetch::requestCount() const{return impl_?uint32_t(impl_->files.size()):0;}
bool AnimationGroupPrefetch::failed() const{return impl_ && impl_->failed;}
}
