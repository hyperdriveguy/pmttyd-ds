#include "animation_tangent_memory.hpp"
#include "event_address_space.hpp"
#include <algorithm>
#include <cassert>
namespace ttyd {
AnimationTangentMemory::AnimationTangentMemory(std::shared_ptr<BattlePosePool> p,EventAddressSpace& m):pool_(std::move(p)),memory_(m){}
//178 BE float rows at803d9580 encode degrees-89..88 (row89 zero).
// Alias the already produced source bytes only after same-VM pool admission;
// binding never reruns tan or substitutes the detached legacy libm table.
std::unique_ptr<AnimationTangentMemory> AnimationTangentMemory::bind(std::shared_ptr<BattlePosePool> pool,EventAddressSpace& memory){
 if(!pool || !pool->sourceGuard_ || pool->sourceMemory_!=&memory || !pool->sourceArraysCurrent() || pool->tangentOwner_)return {};
 auto result=std::unique_ptr<AnimationTangentMemory>(new AnimationTangentMemory(std::move(pool),memory));auto& b=*result;
 std::vector<uint8_t> original(AnimationTangentTable::Count*4);
 for(unsigned i=0;i<original.size();++i)if(!memory.readByte(Address+i,original[i]))return {};
 // Reattachment requires matching retained live state; never rewind either
 // view to cold constants or silently regenerate using native libm.
 if(b.pool_->tangents_ && !std::equal(original.begin(),original.end(),b.pool_->tangents_->state.begin()))return {};
 std::unique_ptr<AnimationTangentTable> prepared;
 if(!b.pool_->tangents_){prepared=std::make_unique<AnimationTangentTable>();std::copy(original.begin(),original.end(),prepared->state.begin());}
 b.table_=prepared?prepared.get():b.pool_->tangents_.get();
 // Preparation is not publication:only actual RAM extraction plus alias
 // registration installs the retained table/owner. Failed registration rolls
 // source bytes back without adopting the new CPU table.
 if(!memory.extractRam(Address,uint32_t(original.size()),original))return {};
 if(!memory.registerExternalRam(Address,b.table_->state.data(),uint32_t(original.size()))){const bool restored=memory.registerRam(Address,std::move(original));assert(restored);(void)restored;return {};}
 if(prepared)b.pool_->tangents_=std::move(prepared);
 b.pool_->tangentOwner_=&b;b.active_=true;return result;
}
bool AnimationTangentMemory::current() const{return active_ && pool_->tangentOwner_==this && pool_->tangents_.get()==table_ && pool_->sourceGuard_ && pool_->sourceMemory_==&memory_ && pool_->sourceArraysCurrent();}
// Preserve CURRENT table words in both CPU owner and VM RAM. This attachment
// must retire before pool aliases/VM; reattachment requires byte equality.
bool AnimationTangentMemory::detach(){
 if(!active_)return true;
 std::vector<uint8_t> current(table_->state.begin(),table_->state.end());
 if(!memory_.unregisterExternalRam(Address))return false;
 const bool restored=memory_.registerRam(Address,std::move(current));assert(restored);if(!restored)return false;
 pool_->tangentOwner_=nullptr;active_=false;return true;
}
AnimationTangentMemory::~AnimationTangentMemory(){const bool ok=detach();assert(ok);(void)ok;}
}
