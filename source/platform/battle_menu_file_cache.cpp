#include "battle_menu_file_cache.hpp"
#include "file_handle_release.hpp"
#include "file_pool.hpp"
#include <algorithm>
#include <cassert>
#include <cstdio>
namespace ttyd {
uint8_t* BattleMenuFileCache::span(uint32_t a,unsigned n){
  const auto at=[&](uint32_t base,uint8_t* data,size_t size)->uint8_t*{
    return a>=base && uint64_t(a)+n<=uint64_t(base)+size?data+(a-base):nullptr;
  };
  for(auto r:{std::make_pair(Global,std::make_pair(global_.data(),global_.size())),
              std::make_pair(Manager,std::make_pair(manager_.data(),manager_.size())),
              std::make_pair(Header,std::make_pair(pointer_.data(),pointer_.size())),
              std::make_pair(Header+8,std::make_pair(backlink_.data(),backlink_.size())),
              std::make_pair(Name,std::make_pair(name_.data(),name_.size())),
              std::make_pair(Pool,std::make_pair(pool_.data(),pool_.size())),
              std::make_pair(Payload,std::make_pair(payload_.data(),payload_.size()))})
    if(auto* p=at(r.first,r.second.first,r.second.second))return p;
  return nullptr;
}
uint8_t BattleMenuFileCache::byte(uint32_t a){auto* p=span(a,1);assert(p);return *p;}
uint16_t BattleMenuFileCache::half(uint32_t a){return uint16_t(byte(a)<<8|byte(a+1));}
uint32_t BattleMenuFileCache::word(uint32_t a){return (uint32_t(half(a))<<16)|half(a+2);}
void BattleMenuFileCache::byte(uint32_t a,uint8_t n){auto* p=span(a,1);assert(p);*p=n;}
void BattleMenuFileCache::half(uint32_t a,uint16_t n){byte(a,uint8_t(n>>8));byte(a+1,uint8_t(n));}
void BattleMenuFileCache::word(uint32_t a,uint32_t n){half(a,uint16_t(n>>16));half(a+2,uint16_t(n));}
EventMemoryServices BattleMenuFileCache::memory(){
  EventMemoryServices s;s.user=this;
  s.word=[](void* p,uint32_t a){return static_cast<BattleMenuFileCache*>(p)->word(a);};
  s.half=[](void* p,uint32_t a){return static_cast<BattleMenuFileCache*>(p)->half(a);};
  s.storeWord=[](void* p,uint32_t a,uint32_t n){static_cast<BattleMenuFileCache*>(p)->word(a,n);};
  s.storeHalf=[](void* p,uint32_t a,uint16_t n){static_cast<BattleMenuFileCache*>(p)->half(a,n);};
  s.storeByte=[](void* p,uint32_t a,uint8_t n){static_cast<BattleMenuFileCache*>(p)->byte(a,n);};return s;
}
bool BattleMenuFileCache::load(const char* path){
  if(ready() || !path)return false;
  FILE* f=std::fopen(path,"rb");if(!f)return false;
  std::vector<uint8_t> raw(217920);
  const bool ok=std::fread(raw.data(),1,raw.size(),f)==raw.size() && std::fgetc(f)==EOF;
  std::fclose(f);
  const uint8_t header[]{0,0x20,0xaf,0x30,0,0,0,97,0,0,0,12};
  if(!ok || !std::equal(std::begin(header),std::end(header),raw.begin()))return false;
  // Native admission validates every source relocation access before exposing
  // backing to the guest. This is not a replacement for the converter's pinned
  // original-asset hash, or a change to source Set's unchecked memory behavior.
  const auto rawWord=[&](uint32_t a){return uint32_t(raw[a])<<24 |
      uint32_t(raw[a+1])<<16 | uint32_t(raw[a+2])<<8 | raw[a+3];};
  std::vector<std::pair<uint32_t,unsigned>> descriptors;
  for(unsigned i=0;i<97;++i)for(unsigned palette=0;palette<2;++palette){
    const auto descriptor=rawWord(12+i*8+palette*4);
    if(!descriptor)continue;
    const unsigned length=palette?12:36;
    if(descriptor<12+97*8 || uint64_t(descriptor)+length>raw.size())return false;
    for(const auto& previous:descriptors){
      if(descriptor==previous.first && length==previous.second)continue;
      if(uint64_t(descriptor)<uint64_t(previous.first)+previous.second &&
         uint64_t(previous.first)<uint64_t(descriptor)+length)return false;
    }
    descriptors.emplace_back(descriptor,length);
    // Only the original unrelocated bank is admitted. Data must be contained
    // in its owned payload, never an unrelated native address token.
    if(raw[descriptor+(palette?2:35)] || rawWord(descriptor+8)>=raw.size())return false;
  }
  original_=std::move(raw);payload_.resize(original_.size());pool_.resize(0x2c000);
  word(Global,Manager);word(Header,Payload);
  FilePoolServices s;s.memory=memory();s.user=this;
  s.allocate=[](void*,int32_t heap,uint32_t n)->std::optional<uint32_t>{return heap==0 && n==0x2c000?std::optional<uint32_t>(Pool):std::nullopt;};
  s.clear=[](void* p,uint32_t a,int32_t fill,uint32_t n){auto& c=*static_cast<BattleMenuFileCache*>(p);auto* data=c.span(a,n);if(!data || fill)return false;std::fill(data,data+n,0);return true;};
  FilePoolCursor cursor;ready_=initializePackedFilePool(s,cursor);return ready_;
}
bool BattleMenuFileCache::bind(EventAddressSpace& target){
  if(!ready())return false;
  std::vector<uint32_t> registered;
  const std::pair<uint32_t,uint32_t> regions[]{
      {Global,4},{Manager,24},{Pool,uint32_t(pool_.size())},
      {Payload,uint32_t(payload_.size())},{Header,4},{Header+8,4}};
  for(const auto& r:regions){
    if(!target.registerExternalRam(r.first,span(r.first,r.second),r.second)){
      for(auto a:registered)target.unregisterExternalRam(a);
      return false;
    }
    registered.push_back(r.first);
  }
  return true;
}
FileAcquisitionServices BattleMenuFileCache::services(){
  FileAcquisitionServices s;s.memory=memory();s.user=this;
  s.byte=[](void* p,uint32_t a){return static_cast<BattleMenuFileCache*>(p)->byte(a);};
  s.compare=[](void* p,uint32_t a,uint32_t b)->std::optional<int32_t>{auto& c=*static_cast<BattleMenuFileCache*>(p);for(;;++a,++b){const auto x=c.byte(a),y=c.byte(b);if(x!=y)return int32_t(x)-int32_t(y);if(!x)return 0;}};
  s.copy=[](void* p,uint32_t a,uint32_t b){auto& c=*static_cast<BattleMenuFileCache*>(p);for(;;++a,++b){const auto v=c.byte(b);c.byte(a,v);if(!v)return true;}};
  // This native owner mounts no archives; supported content is staged ROMFS.
  s.archive=[](void*,uint32_t,uint32_t,uint32_t)->std::optional<uint32_t>{return 0;};
  s.open=[](void* p,uint32_t,int32_t mode,uint32_t flags)->std::optional<uint32_t>{auto& c=*static_cast<BattleMenuFileCache*>(p);if(mode!=2 || flags || c.entry_)return {};c.discFlags_=0;return Disc;};
  s.length=[](void* p,uint32_t d)->std::optional<uint32_t>{auto& c=*static_cast<BattleMenuFileCache*>(p);return d==Disc?std::optional<uint32_t>(uint32_t(c.original_.size())):std::nullopt;};
  s.allocate=[](void*,uint32_t n,uint32_t flags)->std::optional<uint32_t>{return n==217920 && !flags?std::optional<uint32_t>(Header):std::nullopt;};
  s.read=[](void* p,uint32_t d,uint32_t to,uint32_t n,uint32_t offset)->std::optional<uint32_t>{auto& c=*static_cast<BattleMenuFileCache*>(p);if(d!=Disc || to!=Payload || n!=c.original_.size() || offset)return {};std::copy(c.original_.begin(),c.original_.end(),c.payload_.begin());return n;};
  s.close=[](void* p,uint32_t d){auto& c=*static_cast<BattleMenuFileCache*>(p);if(d!=Disc)return false;c.discFlags_|=8;return true;};
  s.relocate=[](void* p,uint32_t payload,uint32_t type){auto& c=*static_cast<BattleMenuFileCache*>(p);if(type!=4 || payload!=Payload)return false;FileTextureRelocationServices r;r.memory=c.memory();r.byte=[](void* p,uint32_t a){return static_cast<BattleMenuFileCache*>(p)->byte(a);};return resumePackedFileTextureRelocation(payload,r,c.relocation_);};
  return s;
}
std::optional<uint32_t> BattleMenuFileCache::acquire(const std::string& path,uint32_t type){
  if(!ready() || path!="./battle/common/battle_common.tpl" || type!=4 || word(Global)!=Manager)return {};
  if(!acquisition_.phase){std::copy(path.begin(),path.end(),name_.begin());name_[path.size()]=0;}
  uint32_t result=0;
  if(!acquirePackedFile(Name,type,services(),acquisition_,result))return {};
  if(result)entry_=result;
  return result;
}
bool BattleMenuFileCache::release(uint32_t entry){
  if(!ready() || (entry && (entry<Pool || entry>=Pool+0x2c000 || (entry-Pool)%0xb0)))return false;
  FileHandleReleaseServices s;s.memory=memory();s.byte=[](void* p,uint32_t a){return static_cast<BattleMenuFileCache*>(p)->byte(a);};
  releasePackedFileHandle(entry,s);return true;
}
BattleMenuFileBinding::BattleMenuFileBinding(std::shared_ptr<BattleMenuFileCache> cache,BattleActorServices actors):cache_(std::move(cache)),actors_(actors){}
BattleMenuResourceServices BattleMenuFileBinding::services(){
  BattleMenuResourceServices s;s.memory=actors_;s.user=this;
  s.dvdRoot=[](void* p)->std::optional<uint32_t>{return static_cast<BattleMenuFileBinding*>(p)->cache_->ready()?std::optional<uint32_t>(BattleMenuFileCache::Root):std::nullopt;};
  s.format=[](void* p,uint32_t format,uint32_t root)->std::optional<std::string>{auto& b=*static_cast<BattleMenuFileBinding*>(p);
    const auto text=[&](uint32_t a)->std::optional<std::string>{std::string t;for(unsigned i=0;i<256;++i){const auto v=b.actors_.byte(b.actors_.user,a+i);if(!v)return t;t+=char(v);}return {};};
    const auto f=text(format),r=text(root);if(!f || !r || *f!="%s/battle/common/battle_common.tpl")return {};
    return *r+"/battle/common/battle_common.tpl";};
  s.fileAllocate=[](void* p,const std::string& path,int32_t type){return static_cast<BattleMenuFileBinding*>(p)->cache_->acquire(path,uint32_t(type));};
  s.fileRelease=[](void* p,uint32_t entry){return static_cast<BattleMenuFileBinding*>(p)->cache_->release(entry);};
  return s;
}
} // namespace ttyd
