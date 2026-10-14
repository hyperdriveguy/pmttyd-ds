#include "field_map_flags.hpp"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <unordered_map>
#include <unordered_set>
namespace ttyd {
struct FieldMapFlagGeometry {
 struct Node {int32_t parent=-1;uint32_t source=0;bool collision=false;std::string name;};
 uint32_t vertices=0;
 std::vector<Node> nodes;
 std::vector<uint16_t> owners;
};
namespace {
std::unordered_map<uint32_t,std::shared_ptr<const FieldMapFlagGeometry>> assets;
uint64_t generation=0;
std::shared_ptr<const FieldMapFlagGeometry> load(const std::string& path) {
 FILE* f=std::fopen(path.c_str(),"rb");if(!f)return {};
 auto g=std::make_shared<FieldMapFlagGeometry>();char magic[8];uint32_t count=0;
 bool ok=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMFLG001",8) &&
  std::fread(&g->vertices,4,1,f)==1 && g->vertices && g->vertices<=1000000 && !(g->vertices%3) &&
  std::fread(&count,4,1,f)==1 && count && count<=8192;
 std::unordered_set<uint32_t> sources;
 for(uint32_t n=0;ok && n<count;++n) {
  FieldMapFlagGeometry::Node node;char name[64];uint32_t collision=0;
  ok=std::fread(&node.parent,4,1,f)==1 && node.parent>=-1 && (node.parent<0 || uint32_t(node.parent)<n) &&
   std::fread(&node.source,4,1,f)==1 && sources.insert(node.source).second &&
   std::fread(&collision,4,1,f)==1 && collision<=1 && std::fread(name,64,1,f)==1 && name[0] && std::memchr(name,0,64);
  if(ok){node.name=name;node.collision=collision!=0;g->nodes.push_back(std::move(node));}
 }
 if(ok){g->owners.resize(g->vertices);ok=std::fread(g->owners.data(),2,g->vertices,f)==g->vertices;}
 if(ok)for(uint32_t n=0;n<g->vertices;++n) {
  if(g->owners[n]>=count || (n%3 && g->owners[n]!=g->owners[n-n%3])){ok=false;break;}
 }
 ok=ok && std::fgetc(f)==EOF;std::fclose(f);return ok?g:nullptr;
}
}
bool loadFieldMapFlagAssets(const char* root) {
 if(!root)return false;
 const std::string base=root;FILE* f=std::fopen((base+"/field-map-flags.bin").c_str(),"rb");if(!f)return false;
 char magic[8];uint32_t count=0;std::unordered_map<uint32_t,std::shared_ptr<const FieldMapFlagGeometry>> next;
 bool ok=std::fread(magic,8,1,f)==1 && !std::memcmp(magic,"PMFLDS01",8) && std::fread(&count,4,1,f)==1 && count && count<=64;
 for(uint32_t n=0;ok && n<count;++n) {
  uint32_t room=0;char name[64];ok=std::fread(&room,4,1,f)==1 && std::fread(name,64,1,f)==1 && name[0] && std::memchr(name,0,64);
  if(!ok)break;
  for(char c:std::string(name))if(!((c>='a' && c<='z') || (c>='0' && c<='9') || c=='_' || c=='-' || c=='.'))ok=false;
  if(!ok || std::string(name).find("..")!=std::string::npos){ok=false;break;}
  auto g=load(base+"/"+name);ok=g && next.emplace(room,std::move(g)).second;
 }
 ok=ok && std::fgetc(f)==EOF;std::fclose(f);if(!ok)return false;
 assets=std::move(next);++generation;return true;
}
void FieldMapFlags::invalidate() {geometry_.reset();flags_.clear();hitFlags_.clear();disabled_.clear();visible_.clear();dirty_=true;room_=UINT32_MAX;++epoch_;}
void FieldMapFlags::synchronize(uint32_t room) {
 if(room_==room && generation_==generation)return;
 invalidate();room_=room;generation_=generation;
 const auto it=assets.find(room);if(it==assets.end())return;
 geometry_=it->second;flags_.resize(geometry_->nodes.size());hitFlags_.resize(flags_.size());
}
bool FieldMapFlags::ready(uint32_t vertices) const{return geometry_ && generation_==generation && geometry_->vertices==vertices;}
int FieldMapFlags::find(const std::string& name) const {
 if(!geometry_ || generation_!=generation)return -1;
 int found=-1;for(unsigned n=0;n<geometry_->nodes.size();++n)if(geometry_->nodes[n].name==name){if(found>=0)return -1;found=int(n);}return found;
}
bool FieldMapFlags::mapFlag(const std::string& name,uint32_t mask,bool on) {
 const auto target=find(name);if(target<0)return false;
 for(unsigned n=unsigned(target);n<flags_.size();++n) {
  auto parent=int(n);while(parent>=0 && parent!=target)parent=geometry_->nodes[unsigned(parent)].parent;
  if(parent<0)continue;
  const auto value=on?flags_[n]|mask:flags_[n]&~mask;
  if((value^flags_[n])&1)dirty_=true;
  flags_[n]=value;
 }
 return true;
}
bool FieldMapFlags::hitEnabled(const std::string& name,bool enabled) {return hitFlags(name,1,!enabled);}
bool FieldMapFlags::hitFlags(const std::string& name,uint16_t mask,bool on) {
 const auto target=find(name);if(target<0 || !geometry_->nodes[unsigned(target)].collision)return false;
 for(unsigned n=unsigned(target);n<hitFlags_.size();++n) {
  auto parent=int(n);while(parent>=0 && parent!=target)parent=geometry_->nodes[unsigned(parent)].parent;
  if(parent>=0 && geometry_->nodes[n].collision)hitFlags_[n]=on?uint16_t(hitFlags_[n]|mask):uint16_t(hitFlags_[n]&~mask);
 }
 disabled_.clear();for(unsigned n=0;n<hitFlags_.size();++n)if(hitFlags_[n]&0x81)disabled_.push_back(geometry_->nodes[n].source);
 std::sort(disabled_.begin(),disabled_.end());return true;
}
std::optional<uint32_t> FieldMapFlags::mapFlags(const std::string& name) const{const auto n=find(name);return n<0?std::nullopt:std::optional<uint32_t>(flags_[unsigned(n)]);}
std::optional<bool> FieldMapFlags::hitEnabled(const std::string& name) const{const auto n=find(name);return n<0 || !geometry_->nodes[unsigned(n)].collision?std::nullopt:std::optional<bool>(!(hitFlags_[unsigned(n)]&0x81));}
void FieldMapFlags::refreshVisibility() const {
 if(!dirty_)return;
 visible_.clear();uint32_t n=0;
 while(n<geometry_->vertices) {
  while(n<geometry_->vertices && (flags_[geometry_->owners[n]]&1))++n;
  const auto start=n;while(n<geometry_->vertices && !(flags_[geometry_->owners[n]]&1))++n;
  if(n>start)visible_.push_back({start,n-start});
 }
 dirty_=false;
}
std::optional<uint32_t> FieldMapFlags::hitSourceNode(const std::string& name) const {
 const auto n=find(name);return n<0 || !geometry_->nodes[unsigned(n)].collision || (hitFlags_[unsigned(n)]&0x80)?std::nullopt:std::optional<uint32_t>(geometry_->nodes[unsigned(n)].source);
}
uint32_t FieldMapFlags::room() const{return ready(vertices())?room_:UINT32_MAX;}
uint32_t FieldMapFlags::vertices() const{return geometry_?geometry_->vertices:0;}
}
