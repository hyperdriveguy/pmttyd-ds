#include "battle_menu_file_cache.hpp"
#include "file_handle_release.hpp"
#include "file_pool.hpp"
#include <algorithm>
#include <cassert>
#include <cstdio>
namespace ttyd {
// One-shot common TPL admission: exact217920 bytes, original TPL magic,
// count97/table12. Candidate pool/file publication succeeds before facade binds;
// this is raw original texture backing, not a converted GPU atlas.
bool BattleMenuFileCache::load(const char* path){
  if(ready() || !path)return false;
  FILE* f=std::fopen(path,"rb");if(!f)return false;
  std::vector<uint8_t> raw(217920);
  const bool ok=std::fread(raw.data(),1,raw.size(),f)==raw.size() && std::fgetc(f)==EOF;
  std::fclose(f);
  const uint8_t header[]{0,0x20,0xaf,0x30,0,0,0,97,0,0,0,12};
  if(!ok || !std::equal(std::begin(header),std::end(header),raw.begin()))return false;
  auto candidate=NativeFileCache::create();
  if(!candidate || !candidate->stage("./battle/common/battle_common.tpl",4,std::move(raw)))return false;
  files_=std::move(candidate);return true;
}
bool BattleMenuFileCache::bind(EventAddressSpace& target){return files_ && files_->bind(target);}
std::optional<uint32_t> BattleMenuFileCache::acquire(const std::string& path,uint32_t type){
  if(!files_ || path!="./battle/common/battle_common.tpl" || type!=4)return {};
  return files_->acquire(path,type);
}
bool BattleMenuFileCache::release(uint32_t entry){return files_ && files_->release(entry);}
BattleMenuFileBinding::BattleMenuFileBinding(std::shared_ptr<BattleMenuFileCache> cache,BattleActorServices actors):cache_(std::move(cache)),actors_(actors){}
BattleMenuResourceServices BattleMenuFileBinding::services(){
  BattleMenuResourceServices s;s.memory=actors_;s.user=this;
  s.dvdRoot=[](void* p)->std::optional<uint32_t>{return static_cast<BattleMenuFileBinding*>(p)->cache_->ready()?std::optional<uint32_t>(BattleMenuFileCache::Root):std::nullopt;};
  // Read actual VM strings, bounded256 bytes as native admission. Accept
  // only original %s/battle/common/battle_common.tpl; do not invent a general
  // printf/path-normalization service. Cache/actor owners outlive these closures.
  s.format=[](void* p,uint32_t format,uint32_t root)->std::optional<std::string>{auto& b=*static_cast<BattleMenuFileBinding*>(p);
    const auto text=[&](uint32_t a)->std::optional<std::string>{std::string t;for(unsigned i=0;i<256;++i){const auto v=b.actors_.byte(b.actors_.user,a+i);if(!v)return t;t+=char(v);}return {};};
    const auto f=text(format),r=text(root);if(!f || !r || *f!="%s/battle/common/battle_common.tpl")return {};
    return *r+"/battle/common/battle_common.tpl";};
  s.fileAllocate=[](void* p,const std::string& path,int32_t type){return static_cast<BattleMenuFileBinding*>(p)->cache_->acquire(path,uint32_t(type));};
  s.fileRelease=[](void* p,uint32_t entry){return static_cast<BattleMenuFileBinding*>(p)->cache_->release(entry);};
  return s;
}
} // namespace ttyd
