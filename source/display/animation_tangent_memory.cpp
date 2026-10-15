#include "animation_tangent_memory.hpp"
#include "event_address_space.hpp"
#include <algorithm>
#include <cassert>
namespace ttyd {
AnimationTangentMemory::AnimationTangentMemory(std::shared_ptr<BattlePosePool> p,EventAddressSpace& m):pool_(std::move(p)),memory_(m){}
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
 if(!memory.extractRam(Address,uint32_t(original.size()),original))return {};
 if(!memory.registerExternalRam(Address,b.table_->state.data(),uint32_t(original.size()))){const bool restored=memory.registerRam(Address,std::move(original));assert(restored);(void)restored;return {};}
 if(prepared)b.pool_->tangents_=std::move(prepared);
 b.pool_->tangentOwner_=&b;b.active_=true;return result;
}
bool AnimationTangentMemory::current() const{return active_ && pool_->tangentOwner_==this && pool_->tangents_.get()==table_ && pool_->sourceGuard_ && pool_->sourceMemory_==&memory_ && pool_->sourceArraysCurrent();}
bool AnimationTangentMemory::detach(){
 if(!active_)return true;
 std::vector<uint8_t> current(table_->state.begin(),table_->state.end());
 if(!memory_.unregisterExternalRam(Address))return false;
 const bool restored=memory_.registerRam(Address,std::move(current));assert(restored);if(!restored)return false;
 pool_->tangentOwner_=nullptr;active_=false;return true;
}
AnimationTangentMemory::~AnimationTangentMemory(){const bool ok=detach();assert(ok);(void)ok;}
}
