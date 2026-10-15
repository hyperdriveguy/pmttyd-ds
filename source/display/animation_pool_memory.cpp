#include "animation_pool_memory.hpp"
#include "event_address_space.hpp"
#include <algorithm>
#include <cassert>
namespace ttyd {
AnimationPoolMemory::AnimationPoolMemory(std::shared_ptr<BattlePosePool> p,EventAddressSpace& m):pool_(std::move(p)),memory_(m){}
std::unique_ptr<AnimationPoolMemory> AnimationPoolMemory::bind(std::shared_ptr<BattlePosePool> pool,EventAddressSpace& memory){
 if(!pool || pool->sourceGuard_ || !pool->cacheMetadataReady())return {};
 auto result=std::unique_ptr<AnimationPoolMemory>(new AnimationPoolMemory(std::move(pool),memory));auto& b=*result;
 auto word=[&](uint32_t a,uint32_t& v){return memory.readWord(a,v);};uint32_t count=0;
 if(!word(0x80415dc0,b.work_) || !b.work_ || !word(b.work_,b.models_) || !word(b.work_+4,count) || count!=64 || !word(b.work_+8,b.textures_) || !word(b.work_+0xc,count) || count!=64 || !word(b.work_+0x10,b.poses_) || !word(b.work_+0x14,count) || count!=256)return {};
 const uint32_t header[]={b.models_,64,b.textures_,64,b.poses_,256};
 for(unsigned i=0;i<6;++i)for(unsigned lane=0;lane<4;++lane)b.header_[4*i+lane]=uint8_t(header[i]>>(24-8*lane));
 b.spans_={{b.work_,0x110,{}},{b.models_,64*16,{}},{b.textures_,64*12,{}},{b.poses_,256*0x170,{}}};
 for(unsigned i=0;i<b.spans_.size();++i){auto& span=b.spans_[i];const uint64_t end=uint64_t(span.address)+span.size;
  if(!span.address || end>0x100000000ULL || (span.address<0x80415dc4 && end>0x80415dc0))return {};
  for(unsigned j=0;j<i;++j)if(span.address<uint64_t(b.spans_[j].address)+b.spans_[j].size && b.spans_[j].address<end)return {};
  span.saved.resize(span.size);for(unsigned n=0;n<span.size;++n)if(!memory.readByte(span.address+n,span.saved[n]))return {};
 }
 // First source W transfer may supply previously unknown fields. e8 already
 // has typed history and must match; subsequent transfers require full equality.
 const auto& w=b.spans_[0].saved;
 if(b.pool_->workEstablished_){if(!std::equal(w.begin(),w.end(),b.pool_->work_.begin()))return {};}
 else for(unsigned i=0xe8;i<0xec;++i)if(w[i]!=b.pool_->work_[i])return {};
 for(unsigned i=0;i<64;++i){
  if(!std::equal(b.pool_->models_[i].state.begin(),b.pool_->models_[i].state.end(),b.spans_[1].saved.begin()+16*i))return {};
  if(!std::equal(b.pool_->textures_[i].state.begin(),b.pool_->textures_[i].state.end(),b.spans_[2].saved.begin()+12*i))return {};
 }
 for(unsigned i=0;i<256;++i)for(unsigned n=0;n<0x170;++n)if(b.spans_[3].saved[i*0x170+n]!=(b.pool_->poses_[i]?b.pool_->poses_[i]->state[n]:0))return {};
 unsigned extracted=0;
 for(auto& span:b.spans_){std::vector<uint8_t> bytes;if(!memory.extractRam(span.address,span.size,bytes)){
   for(unsigned i=0;i<extracted;++i){const bool restored=memory.registerRam(b.spans_[i].address,std::move(b.spans_[i].saved));assert(restored);(void)restored;}return {};}
  span.saved=std::move(bytes);++extracted;
 }
 // Allocate stable missing backing only after extraction has succeeded.
 // Failed admission must not change even the pool's sparse object identities.
 std::vector<unsigned> prepared;
 for(unsigned i=0;i<256;++i)if(!b.pool_->poses_[i]){b.pool_->poses_[i]=std::make_unique<BattlePose>();prepared.push_back(i);}
 const auto oldWork=b.pool_->work_;const bool established=b.pool_->workEstablished_;
 if(!established)std::copy(b.spans_[0].saved.begin(),b.spans_[0].saved.end(),b.pool_->work_.begin());
 b.rows_.push_back({b.work_,0x110,b.pool_->work_.data()});
 for(unsigned i=0;i<64;++i)b.rows_.push_back({b.models_+16*i,16,b.pool_->models_[i].state.data()});
 for(unsigned i=0;i<64;++i)b.rows_.push_back({b.textures_+12*i,12,b.pool_->textures_[i].state.data()});
 for(unsigned i=0;i<256;++i)b.rows_.push_back({b.poses_+0x170*i,0x170,b.pool_->poses_[i]->state.data()});
 unsigned mapped=0;for(const auto& row:b.rows_){if(!memory.registerExternalRam(row.address,row.bytes,row.size)){
   for(unsigned i=0;i<mapped;++i)memory.unregisterExternalRam(b.rows_[i].address);
   for(auto& span:b.spans_){const bool restored=memory.registerRam(span.address,std::move(span.saved));assert(restored);(void)restored;}
   b.pool_->work_=oldWork;for(auto i:prepared)b.pool_->poses_[i].reset();return {};}++mapped;
 }
 b.pool_->workEstablished_=true;b.active_=true;b.pool_->sourceGuardUser_=&b;b.pool_->sourceMemory_=&memory;
 b.pool_->sourceGuard_=[](const void* p){return static_cast<const AnimationPoolMemory*>(p)->matchesWork();};return result;
}
bool AnimationPoolMemory::matchesWork() const{
 uint32_t root=0;
 return active_ && memory_.readWord(0x80415dc0,root) && root==work_ && memory_.matchesBytes(work_,header_.data(),uint32_t(header_.size()));
}
bool AnimationPoolMemory::detach(){
 if(!active_)return true;
 // Capture from the authoritative backing, including all changes performed
 // since attachment. Old snapshots are only for failed-attachment rollback.
 for(auto& span:spans_)for(const auto& row:rows_)if(row.address>=span.address && uint64_t(row.address)+row.size<=uint64_t(span.address)+span.size)
  std::copy(row.bytes,row.bytes+row.size,span.saved.begin()+(row.address-span.address));
 pool_->sourceGuard_=nullptr;pool_->sourceGuardUser_=nullptr;pool_->sourceMemory_=nullptr;
 bool ok=true;for(const auto& row:rows_)ok=memory_.unregisterExternalRam(row.address) && ok;
 for(auto& span:spans_)ok=memory_.registerRam(span.address,std::move(span.saved)) && ok;
 active_=false;return ok;
}
AnimationPoolMemory::~AnimationPoolMemory(){const bool ok=detach();assert(ok);(void)ok;}
}
