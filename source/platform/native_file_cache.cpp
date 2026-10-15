#include "native_file_cache.hpp"
#include "file_handle_release.hpp"
#include "file_model_relocation.hpp"
#include "file_pool.hpp"
#include <algorithm>
#include <cassert>
#include <cstdio>
namespace ttyd {
namespace {
uint32_t rawWord(const std::vector<uint8_t>& b,uint32_t a){return uint32_t(b[a])<<24|uint32_t(b[a+1])<<16|uint32_t(b[a+2])<<8|b[a+3];}
// Native admission for ordinary unrelocated payloads. This proves all source
// relocation accesses stay inside stable backing, before any VM sees the file.
// It deliberately does not replace source handlers with validated rewrites.
bool admitted(const std::vector<uint8_t>& b,uint32_t type){
 // 0xafffff bounds the later native payload arena, not original file size.
 // Type0 raw has no relocation. Type4 TPL admission below validates the writable
 // descriptor graph; it does not certify texture pixel semantics/GPU ownership.
 if(b.empty() || b.size()>0xafffff)return false;
 if(type==0)return true;
 if(type==4){
  if(b.size()<12 || rawWord(b,0)!=0x0020af30)return false;
  const auto count=rawWord(b,4),table=rawWord(b,8);
  // Source uses a low16 loop index against the full-word live count.
  // Larger native inputs cannot terminate; reject them before file effects.
  if(count>0xffff || table<12 || uint64_t(table)+uint64_t(count)*8>b.size())return false;
  std::vector<std::pair<uint32_t,unsigned>> descriptors;
  for(uint32_t i=0;i<count;++i)for(unsigned palette=0;palette<2;++palette){
   const auto d=rawWord(b,table+i*8+palette*4);if(!d)continue;
   // Image descriptor36 bytes/marker35, palette12 bytes/marker2. Relative
   // data starts must lie inside original bytes; already-set markers are refused.
   // Shared identical descriptors are allowed, conflicting partial overlaps not.
   const unsigned n=palette?12:36;
   if(d<uint64_t(table)+uint64_t(count)*8 || uint64_t(d)+n>b.size())return false;
   descriptors.emplace_back(d,n);
   if(b[d+(palette?2:35)] || rawWord(b,d+8)>=b.size())return false;
  }
  std::sort(descriptors.begin(),descriptors.end());
  for(size_t i=1;i<descriptors.size();++i)if(descriptors[i]!=descriptors[i-1] && descriptors[i].first<uint64_t(descriptors[i-1].first)+descriptors[i-1].second)return false;
  return true;
 }
 // Type5 MODEL has root through0x1ac. Validate only reached writable table/
 // child extents, not every converted unused pointer word. High-bit count is
 // source nonpositive and skips children; ordinary records stride64/child+0x3c.
 if(type!=5 || b.size()<0x1b0)return false;
 // Only actually dereferenced table/child addresses must be contained.
 // Other converted words include original unused sentinels and wrap freely.
 if(rawWord(b,0x14c)>=0x87100000u)return false;
 const auto count=rawWord(b,0x148),table=rawWord(b,0x1ac);
 if(!count || count>0x7fffffff)return true;
 const uint64_t tableEnd=uint64_t(table)+uint64_t(count)*64;
 if(table<0x1b0 || tableEnd>b.size())return false;
 std::vector<uint32_t> children;
 for(uint32_t i=0;i<count;++i){
  const auto c=rawWord(b,table+i*64+0x3c);
  // Ordinary packed MODEL admission excludes aliasing header/record writes.
  // Shared identical children are supported; other source alias domains remain
  // available to the raw handler, not silently certified by this file backend.
  if(c<tableEnd || uint64_t(c)+0x44>b.size())return false;
  children.push_back(c);
 }
 std::sort(children.begin(),children.end());
 for(size_t i=1;i<children.size();++i)if(children[i]!=children[i-1] && uint64_t(children[i])<uint64_t(children[i-1])+0x44)return false;
 return true;
}
}
NativeFileCache::NativeFileCache():pool_(0x2c000){}
std::shared_ptr<NativeFileCache> NativeFileCache::create(){
 auto c=std::shared_ptr<NativeFileCache>(new NativeFileCache);c->word(Global,Manager);
 FilePoolServices s;s.memory=c->memory();s.user=c.get();
 s.allocate=[](void*,int32_t heap,uint32_t n)->std::optional<uint32_t>{return heap==0 && n==0x2c000?std::optional<uint32_t>(Pool):std::nullopt;};
 s.clear=[](void* p,uint32_t a,int32_t fill,uint32_t n){auto& c=*static_cast<NativeFileCache*>(p);auto* bytes=c.span(a,n);if(!bytes || fill)return false;std::fill(bytes,bytes+n,0);return true;};
 FilePoolCursor cursor;if(!initializePackedFilePool(s,cursor))return {};return c;
}
bool NativeFileCache::stage(const std::string& path,uint32_t type,std::vector<uint8_t> bytes){
 std::vector<Source> sources;sources.push_back({path,type,std::move(bytes)});return stage(std::move(sources));
}
// Validate/prepare whole catalog batch before publishing any file/address.
// Exact paths are identity (no canonicalization), path length<128 plus NUL,
// no duplicates; at most1024 files. First payload has a separate1MiB
// window, later ones use the native arena. Payloads round to32 with zero padding.
bool NativeFileCache::stage(std::vector<Source> sources){
 if(frozen_ || sources.empty() || sources.size()>1024-files_.size())return false;
 std::vector<std::unique_ptr<File>> prepared;uint32_t next=nextPayload_;
 for(auto& source:sources){
  const auto& path=source.path;const auto& bytes=source.original;
  if(path.empty() || path.size()>=128 || path.find('\0')!=std::string::npos || !admitted(bytes,source.type))return false;
  for(const auto& f:files_)if(f->path==path)return false;
  for(const auto& f:prepared)if(f->path==path)return false;
  const auto index=files_.size()+prepared.size();const uint32_t rounded=(uint32_t(bytes.size())+31)&~31u;
  const uint32_t address=index?next:FirstPayload,limit=index?PayloadLimit:Headers;
  if(uint64_t(address)+rounded>limit)return false;
  auto f=std::make_unique<File>();f->path=path;f->type=source.type;f->address=address;f->headerAddress=Headers+uint32_t(index)*32;
  f->original=std::move(source.original);f->payload.resize(rounded);for(unsigned i=0;i<4;++i)f->header[i]=uint8_t(address>>(24-8*i));
  if(index)next=address+rounded;
  prepared.push_back(std::move(f));
 }
 files_.reserve(files_.size()+prepared.size());for(auto& f:prepared)files_.push_back(std::move(f));nextPayload_=next;return true;
}
// All views alias the same actual BE byte backing. U64 extent arithmetic
// avoids wrapped admission; byte/half/word access asserts unmapped requests
// instead of supplying fabricated zeros. Staging cannot move backing once frozen.
uint8_t* NativeFileCache::span(uint32_t a,unsigned n){
 const auto at=[&](uint32_t base,uint8_t* data,size_t size)->uint8_t*{return a>=base && uint64_t(a)+n<=uint64_t(base)+size?data+(a-base):nullptr;};
 for(auto r:{std::make_pair(Global,std::make_pair(global_.data(),global_.size())),std::make_pair(Manager,std::make_pair(manager_.data(),manager_.size())),std::make_pair(Name,std::make_pair(name_.data(),name_.size())),std::make_pair(Pool,std::make_pair(pool_.data(),pool_.size()))})if(auto* p=at(r.first,r.second.first,r.second.second))return p;
 for(auto& f:files_){if(auto* p=at(f->headerAddress,f->header.data(),f->header.size()))return p;if(auto* p=at(f->address,f->payload.data(),f->payload.size()))return p;}return nullptr;
}
uint8_t NativeFileCache::byte(uint32_t a){auto* p=span(a,1);assert(p);return *p;}
void NativeFileCache::byte(uint32_t a,uint8_t v){auto* p=span(a,1);assert(p);*p=v;}
uint16_t NativeFileCache::half(uint32_t a){return uint16_t(byte(a)<<8|byte(a+1));}
void NativeFileCache::half(uint32_t a,uint16_t v){byte(a,uint8_t(v>>8));byte(a+1,uint8_t(v));}
uint32_t NativeFileCache::word(uint32_t a){return uint32_t(half(a))<<16|half(a+2);}
void NativeFileCache::word(uint32_t a,uint32_t v){half(a,uint16_t(v>>16));half(a+2,uint16_t(v));}
EventMemoryServices NativeFileCache::memory(){
 EventMemoryServices s;s.user=this;
 s.word=[](void* p,uint32_t a){return static_cast<NativeFileCache*>(p)->word(a);};s.half=[](void* p,uint32_t a){return static_cast<NativeFileCache*>(p)->half(a);};
 s.storeWord=[](void* p,uint32_t a,uint32_t v){static_cast<NativeFileCache*>(p)->word(a,v);};s.storeHalf=[](void* p,uint32_t a,uint16_t v){static_cast<NativeFileCache*>(p)->half(a,v);};s.storeByte=[](void* p,uint32_t a,uint8_t v){static_cast<NativeFileCache*>(p)->byte(a,v);};return s;
}
bool NativeFileCache::yieldProvider(void* user,bool (*yield)(void*)){
 if(frozen_)return false;
 yieldUser_=user;yield_=yield;return true;
}
// Map manager/pool/name and every stable holder/payload into a borrowing VM.
// Failure unregisters only this call's successful prefix. Owner must outlive
// all VM views; mapping does not reset shared refs or relocation. Success freezes
// catalog/yield configuration; caller owns later view retirement.
bool NativeFileCache::bind(EventAddressSpace& vm){
 struct View{uint32_t a;uint8_t* data;uint32_t n;};
 std::vector<View> views={{Global,global_.data(),4},{Manager,manager_.data(),24},{Pool,pool_.data(),uint32_t(pool_.size())},{Name,name_.data(),uint32_t(name_.size())}};
 for(auto& f:files_){views.push_back({f->headerAddress,f->header.data(),12});views.push_back({f->address,f->payload.data(),uint32_t(f->payload.size())});}
 unsigned mapped=0;for(auto v:views){if(!vm.registerExternalRam(v.a,v.data,v.n)){for(unsigned i=0;i<mapped;++i)vm.unregisterExternalRam(views[i].a);return false;}++mapped;}
 frozen_=true;return true;
}
// Serialized staged-ROMFS adapter: real source pool/acquisition/relocation/
// release operate on backed bytes; archives explicitly empty. Disc tokens,
// rounded zero padding and simplified smart headers are native policy. No GC,
// eviction, file movement, original SDK heap or DVD worker is fabricated.
FileAcquisitionServices NativeFileCache::services(){
 FileAcquisitionServices s;s.memory=memory();s.user=this;
 s.byte=[](void* p,uint32_t a){return static_cast<NativeFileCache*>(p)->byte(a);};
 s.compare=[](void* p,uint32_t a,uint32_t b)->std::optional<int32_t>{auto& c=*static_cast<NativeFileCache*>(p);for(;;++a,++b){const auto x=c.byte(a),y=c.byte(b);if(x!=y)return int32_t(x)-int32_t(y);if(!x)return 0;}};
 s.copy=[](void* p,uint32_t a,uint32_t b){auto& c=*static_cast<NativeFileCache*>(p);for(;;++a,++b){auto v=c.byte(b);c.byte(a,v);if(!v)return true;}};
 // Pending reuse needs the explicit configured yield provider. Missing
 // yield or garbage collection remains unavailable; it is not a default success.
 s.yield=[](void* p){auto& c=*static_cast<NativeFileCache*>(p);return c.yield_ && c.yield_(c.yieldUser_);};
 // Explicit native catalog: archives are empty, staged ROMFS replaces disc.
 s.archive=[](void*,uint32_t,uint32_t,uint32_t)->std::optional<uint32_t>{return 0;};
 s.open=[](void* p,uint32_t,int32_t mode,uint32_t flags)->std::optional<uint32_t>{auto& c=*static_cast<NativeFileCache*>(p);if(mode!=2 || flags || !c.current_ || c.current_->entry)return {};c.discFlags_=0;return Disc;};
 s.length=[](void* p,uint32_t d)->std::optional<uint32_t>{auto& c=*static_cast<NativeFileCache*>(p);return d==Disc && c.current_?std::optional<uint32_t>(uint32_t(c.current_->original.size())):std::nullopt;};
 s.allocate=[](void* p,uint32_t n,uint32_t flags)->std::optional<uint32_t>{auto& c=*static_cast<NativeFileCache*>(p);return c.current_ && n==c.current_->payload.size() && !flags?std::optional<uint32_t>(c.current_->headerAddress):std::nullopt;};
 s.read=[](void* p,uint32_t d,uint32_t to,uint32_t n,uint32_t offset)->std::optional<uint32_t>{auto& c=*static_cast<NativeFileCache*>(p);auto* f=c.current_;if(d!=Disc || !f || to!=f->address || n!=f->payload.size() || offset)return {};std::copy(f->original.begin(),f->original.end(),f->payload.begin());return uint32_t(f->original.size());};
 s.close=[](void* p,uint32_t d){auto& c=*static_cast<NativeFileCache*>(p);if(d!=Disc)return false;c.discFlags_|=8;return true;};
 s.relocate=[](void* p,uint32_t payload,uint32_t type){auto& c=*static_cast<NativeFileCache*>(p);if(!c.current_ || payload!=c.current_->address || type!=c.current_->type)return false;
  if(type==0)return true;
  if(type==5){setPackedFileModelPointers(payload,c.memory());return true;}
  if(type!=4)return false;
  FileTextureRelocationServices r;r.memory=c.memory();r.byte=[](void* p,uint32_t a){return static_cast<NativeFileCache*>(p)->byte(a);};return resumePackedFileTextureRelocation(payload,r,c.current_->relocation);};
 return s;
}
// One held path owns current_ and acquisition_ until completion. Freeze even
// if the first admitted acquisition holds. Initial staged type guards only new
// loads; source reuse ignores requested type. c.type still pins held requests.
std::optional<uint32_t> NativeFileCache::acquire(const std::string& path,uint32_t type){
 if(word(Global)!=Manager)return {};
 if(!acquisition_.phase){
  File* file=nullptr;for(auto& f:files_)if(f->path==path){file=f.get();break;}
  // Bound native new-file domain. Cached source reuse still ignores type.
  if(!file || (!file->entry && type!=file->type))return {};
  current_=file;std::copy(path.begin(),path.end(),name_.begin());name_[path.size()]=0;frozen_=true;
 }else if(!current_ || current_->path!=path)return {};
 uint32_t result=0;if(!acquirePackedFile(Name,type,services(),acquisition_,result))return {};
 if(result)current_->entry=result;
 current_=nullptr;return result;
}
// Native entry-range/alignment admission precedes actual fileFree. A true
// return acknowledges the leaf (including its no-op cases), not destruction or
// payload reclamation; state2 retains payload/holder identity for reuse.
bool NativeFileCache::release(uint32_t entry){
 if(word(Global)!=Manager || (entry && (entry<Pool || entry>=Pool+0x2c000 || (entry-Pool)%0xb0)))return false;
 FileHandleReleaseServices s;s.memory=memory();s.byte=[](void* p,uint32_t a){return static_cast<NativeFileCache*>(p)->byte(a);};releasePackedFileHandle(entry,s);return true;
}
bool NativeFileCache::archiveType(uint32_t type){if(word(Global)!=Manager)return false;setPackedFileArchiveType(type,memory());return true;}
// Catalog presence and original-byte extent only (padding excluded): these
// queries do not prove acquired entry state, current content or animation readiness.
bool NativeFileCache::contains(const std::string& path,uint32_t type) const{for(const auto& f:files_)if(f->path==path && f->type==type)return true;return false;}
bool NativeFileCache::ownsPayload(uint32_t address,uint32_t bytes) const{for(const auto& f:files_)if(address>=f->address && uint64_t(address)+bytes<=uint64_t(f->address)+f->original.size())return true;return false;}
// Stage unchanged raw MODEL/TPL/auxiliary products, not PMANIM, pose metadata
// or GPU atlases. Exact source names matter: e_mario/b_mario share a/a_mario-,
// MODEL texture names use trailing hyphen; enemy c_kuribo has its own bank.
// Batch reads/validation finish before publication; no source acquisition occurs.
bool stagePrologueAnimationFiles(NativeFileCache& cache,const char* root){
 if(!root)return false;
 struct Entry{const char* path;uint32_t type;const char* product;};
 const Entry entries[]={
  {"a/a_mario",5,"field-player-a_mario.draw-model.bin"},
  {"a/a_mario_r",5,"field-player-a_mario_r.draw-model.bin"},
  {"a/e_mario",5,"field-player-e_mario.draw-model.bin"},
  {"a/b_mario",5,"trio-attack-mario.draw-model.bin"},
  {"a/c_pkr",5,"field-player-c_pkr.draw-model.bin"},
  {"a/c_kuri_h",5,"field-native-c_kuri_h.draw-model.bin"},
  {"a/c_kuribo",5,"trio-enemy-kuribo.draw-model.bin"},
  {"a/hoshi",5,"field-native-hoshi.draw-model.bin"},
  {"a/c_kanbu_ibt",5,"field-native-c_kanbu_ibt.draw-model.bin"},
  {"a/c_zako_n",5,"field-native-c_zako_n.draw-model.bin"},
  {"a/c_gara",5,"field-native-c_gara.draw-model.bin"},
  {"a/c_suifu_b",5,"field-native-c_suifu_b.draw-model.bin"},
  {"a/a_mario-",4,"field-player-a_mario.draw-textures.bin"},
  {"a/a_mario_r-",4,"field-player-a_mario_r.draw-textures.bin"},
  {"a/c_pkr-",4,"field-player-c_pkr.draw-textures.bin"},
  {"a/c_kuri_h-",4,"field-native-c_kuri_h.draw-textures.bin"},
  {"a/c_kuribo-",4,"trio-enemy-kuribo.draw-textures.bin"},
  // Crump MODEL names c_kanbu as its texture bank; no invented _ibt alias.
  {"a/hoshi-",4,"field-native-hoshi.draw-textures.bin"},
  {"a/c_kanbu-",4,"field-native-c_kanbu_ibt.draw-textures.bin"},
  {"a/c_zako_n-",4,"field-native-c_zako_n.draw-textures.bin"},
  {"a/c_gara-",4,"field-native-c_gara.draw-textures.bin"},
  {"a/c_suifu_b-",4,"field-native-c_suifu_b.draw-textures.bin"},
  {"a/vivian.bin",0,"animation-startup/a/vivian.bin"},
  {"a/ag2tg.bin",0,"animation-startup/a/ag2tg.bin"}};
 std::vector<NativeFileCache::Source> sources;
 for(auto e:entries){
  const auto path=std::string(root)+"/"+e.product;FILE* f=std::fopen(path.c_str(),"rb");if(!f)return false;
  if(std::fseek(f,0,SEEK_END)){std::fclose(f);return false;}
  const auto size=std::ftell(f);if(size<=0 || size>0xafffff){std::fclose(f);return false;}std::rewind(f);
  std::vector<uint8_t> bytes(size_t(size),0);const bool ok=std::fread(bytes.data(),1,bytes.size(),f)==bytes.size();std::fclose(f);if(!ok)return false;
  sources.push_back({e.path,e.type,std::move(bytes)});
 }
 return cache.stage(std::move(sources));
}

}
