#include "native_source_window_memory.hpp"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <limits>
namespace ttyd {namespace {
bool extent(uint32_t a,uint32_t n){return n&&uint64_t(a)+n<=0x100000000ULL;}
bool overlap(uint32_t a,uint32_t n,uint32_t b,uint32_t m){return uint64_t(a)<uint64_t(b)+m&&uint64_t(b)<uint64_t(a)+n;}
struct Running{bool&v;explicit Running(bool&b):v(b){v=true;}~Running(){v=false;}};
}
std::shared_ptr<const NativeSourceWindowFile>NativeSourceWindowFile::load(const char*path){
 FILE*f=path?std::fopen(path,"rb"):nullptr;if(!f)return {};
 auto result=std::shared_ptr<NativeSourceWindowFile>(new NativeSourceWindowFile);result->bytes_.resize(321984);
 const bool ok=std::fread(result->bytes_.data(),result->bytes_.size(),1,f)==1&&std::fgetc(f)==EOF&&!std::ferror(f);std::fclose(f);if(!ok)return {};
 uint64_t h=14695981039346656037ULL;for(auto b:result->bytes_){h^=b;h*=1099511628211ULL;}return h==0x1dd2c1c8721b5ed4ULL?result:nullptr;
}
std::shared_ptr<NativeSourceWindowMemory>NativeSourceWindowMemory::create(std::shared_ptr<const void>life,void*u,bool(*c)(void*)){
 if(!life||!c)return {};
 auto p=std::shared_ptr<NativeSourceWindowMemory>(new NativeSourceWindowMemory);p->lifetime_=std::move(life);p->user_=u;p->current_=c;return p->current()?p:nullptr;
}
bool NativeSourceWindowMemory::current()const{if(invalid_||checking_||!lifetime_||!current_)return false;
 Running guard(checking_);if(!current_(user_))invalid_=true;return !invalid_;}
NativeSourceWindowBinding::NativeSourceWindowBinding(std::shared_ptr<NativeSourceWindowMemory>o,EventAddressSpace&v):owner_(std::move(o)),vm_(&v){}
NativeSourceWindowBinding::~NativeSourceWindowBinding(){retire();}
bool NativeSourceWindowBinding::current()const{if(!bound_||stale_||checking_||!owner_->current())return false;
 Running guard(checking_);if(!current_||!current_(user_))stale_=true;return !stale_&&owner_->current();}
void NativeSourceWindowBinding::retire(){if(!bound_||checking_||owner_->running_||owner_->checking_)return;for(auto base:mappings_)vm_->unregisterExternalRam(base);for(unsigned i=0;i<originals_.size();++i)if(originals_[i])vm_->registerRam(NativeSourceWindowMemory::StateBase+i,{*originals_[i]});auto&b=owner_->bindings_;b.erase(std::remove(b.begin(),b.end(),this),b.end());bound_=false;stale_=true;}
bool NativeSourceWindowMemory::allBindingsCurrent()const{for(auto*b:bindings_)if(!b->current())return false;return current();}
std::unique_ptr<NativeSourceWindowBinding>NativeSourceWindowMemory::bind(EventAddressSpace&vm,std::shared_ptr<const void>life,void*u,bool(*c)(void*)){
 if(running_||!current()||!life||!c||!c(u))return {};
 Running guard(running_);if(!allBindingsCurrent())return {};for(auto*b:bindings_)if(b->vm_==&vm)return {};
 auto b=std::unique_ptr<NativeSourceWindowBinding>(new NativeSourceWindowBinding(shared_from_this(),vm));b->lifetime_=std::move(life);b->user_=u;b->current_=c;
 std::array<std::optional<uint8_t>,StateSize>candidate{};
 for(unsigned i=0;i<StateSize;++i){const auto a=StateBase+i;uint8_t value;if(vm.readByte(a,value)){if(!vm.contiguousRam(a,1))return {};candidate[i]=value;}else if(!vm.unmappedRange(a,1))return {};}
 for(const auto&a:allocations_)if(!vm.unmappedRange(a.base,a.capacity))return {};
 unsigned extracted=0;for(;extracted<StateSize;++extracted)if(candidate[extracted]){std::vector<uint8_t>old;if(!vm.extractRam(StateBase+extracted,1,old))break;b->originals_[extracted]=old[0];}
 auto rollback=[&]{for(auto base:b->mappings_)vm.unregisterExternalRam(base);for(unsigned i=0;i<StateSize;++i)if(b->originals_[i])vm.registerRam(StateBase+i,{*b->originals_[i]});};
 if(extracted!=StateSize){rollback();return {};}
 // First history bytes are copied only after every extraction can be restored.
 if(!captured_){for(unsigned i=0;i<StateSize;++i)if(candidate[i])state_[i]=*candidate[i];}
 bool ok=true;for(unsigned i=0;i<StateSize&&ok;++i)if(captured_?known_[i]:bool(candidate[i])){ok=vm.registerExternalRam(StateBase+i,&state_[i],1);if(ok)b->mappings_.push_back(StateBase+i);}
 if(ok)for(const auto&a:allocations_){for(uint32_t i=0;i<a.capacity&&ok;){if(!a.known[i]){++i;continue;}const auto begin=i;while(i<a.capacity&&a.known[i])++i;ok=vm.registerExternalRam(a.base+begin,a.backing+begin,i-begin);if(ok)b->mappings_.push_back(a.base+begin);}if(!ok)break;}
 if(!ok||!current()||!c(u)){rollback();return {};}
 if(!captured_){for(unsigned i=0;i<StateSize;++i)known_[i]=bool(candidate[i]);captured_=true;}
 b->bound_=true;bindings_.push_back(b.get());return b;
}
bool NativeSourceWindowMemory::adoptAllocation(uint32_t base,uint8_t*data,uint32_t n,std::shared_ptr<const void>owner){
 if(running_||!current()||!base||!data||!owner||!extent(base,n))return false;
 Running guard(running_);if(!allBindingsCurrent())return false;
 const auto start=reinterpret_cast<uintptr_t>(data);if(n>std::numeric_limits<uintptr_t>::max()-start||overlap(base,n,StateBase,StateSize))return false;
 for(const auto&a:allocations_){const auto p=reinterpret_cast<uintptr_t>(a.backing);if(overlap(base,n,a.base,a.capacity)||(start<p+a.capacity&&p<start+n))return false;}
 for(auto*b:bindings_)if(!b->vm_->unmappedRange(base,n))return false;
 allocations_.push_back({base,n,data,std::move(owner),std::vector<uint8_t>(n,0),false});return true;
}
bool NativeSourceWindowMemory::installRead(uint32_t base,const std::vector<uint8_t>&bytes){
 if(running_||!current()||bytes.empty())return false;
 Running guard(running_);if(!allBindingsCurrent())return false;
 auto a=std::find_if(allocations_.begin(),allocations_.end(),[&](const auto&v){return v.base==base;});if(a==allocations_.end()||a->installed||bytes.size()>a->capacity||std::any_of(a->known.begin(),a->known.end(),[](uint8_t v){return bool(v);}))return false;
 for(auto*b:bindings_)if(!b->vm_->unmappedRange(base,a->capacity))return false;
 size_t done=0;for(;done<bindings_.size();++done)if(!bindings_[done]->vm_->registerExternalRam(base,a->backing,uint32_t(bytes.size())))break;
 if(done!=bindings_.size()){for(size_t j=0;j<done;++j)bindings_[j]->vm_->unregisterExternalRam(base);return false;}
 std::memmove(a->backing,bytes.data(),bytes.size());std::fill_n(a->known.begin(),bytes.size(),1);a->installed=true;for(auto*b:bindings_)b->mappings_.push_back(base);return true;
}
bool NativeSourceWindowMemory::adoptHeapAllocation(uint32_t base,uint8_t*data,uint32_t n,uint32_t prefix,std::shared_ptr<const void>owner){
 if(!prefix||prefix>n||!adoptAllocation(base,data,n,std::move(owner)))return false;
 Running guard(running_);if(!allBindingsCurrent()){allocations_.pop_back();return false;}
 size_t done=0;for(;done<bindings_.size();++done)if(!bindings_[done]->vm_->registerExternalRam(base,data,prefix))break;
 if(done!=bindings_.size()){for(size_t i=0;i<done;++i)bindings_[i]->vm_->unregisterExternalRam(base);allocations_.pop_back();return false;}
 auto&a=allocations_.back();std::fill_n(a.known.begin(),prefix,1);a.heapKnown=prefix;
 for(auto*b:bindings_){b->mappings_.push_back(base);}
 return true;
}
bool NativeSourceWindowMemory::deliverHeapRead(uint32_t base,const std::vector<uint8_t>&bytes){
 if(running_||!current()||bytes.empty())return false;
 Running guard(running_);if(!allBindingsCurrent())return false;
 auto a=std::find_if(allocations_.begin(),allocations_.end(),[&](const auto&v){return v.base==base;});
 if(a==allocations_.end()||!a->heapKnown||a->installed||bytes.size()>a->heapKnown)return false;
 for(auto*b:bindings_)if(!b->vm_->withRawRamAccess([&](auto&ram){return ram.matchesBacking(base,a->heapKnown,a->backing);}))return false;
 std::memmove(a->backing,bytes.data(),bytes.size());a->installed=true;return true;
}
bool NativeSourceWindowMemory::owns(uint32_t a,uint32_t n)const{if(!n)return false;if(overlap(a,n,StateBase,StateSize))return true;for(const auto&p:allocations_)if(overlap(a,n,p.base,p.capacity))return true;return false;}
uint8_t*NativeSourceWindowMemory::cell(uint32_t a,bool requireKnown)const{if(a>=StateBase&&uint64_t(a)<uint64_t(StateBase)+StateSize){if(!captured_)return nullptr;unsigned i=a-StateBase;return !requireKnown||known_[i]?const_cast<uint8_t*>(&state_[i]):nullptr;}for(const auto&p:allocations_)if(a>=p.base&&uint64_t(a)<uint64_t(p.base)+p.capacity)return !requireKnown||p.known[a-p.base]?p.backing+a-p.base:nullptr;return nullptr;}
bool NativeSourceWindowMemory::known(uint32_t a)const{return bool(cell(a,true));}
void NativeSourceWindowMemory::mark(uint32_t a){if(a>=StateBase&&uint64_t(a)<uint64_t(StateBase)+StateSize)known_[a-StateBase]=true;else for(auto&p:allocations_)if(a>=p.base&&uint64_t(a)<uint64_t(p.base)+p.capacity){p.known[a-p.base]=1;break;}}
std::optional<uint8_t>NativeSourceWindowMemory::byte(uint32_t a)const{if(!current())return {};auto*p=cell(a,true);return p?std::optional<uint8_t>(*p):std::nullopt;}
std::optional<uint16_t>NativeSourceWindowMemory::half(uint32_t a)const{if(!extent(a,2))return {};auto x=byte(a),y=byte(a+1);return x&&y?std::optional<uint16_t>(uint16_t(*x)<<8|*y):std::nullopt;}
std::optional<uint32_t>NativeSourceWindowMemory::word(uint32_t a)const{if(!extent(a,4))return {};uint32_t v=0;for(unsigned i=0;i<4;++i){auto b=byte(a+i);if(!b)return {};v=v<<8|*b;}return v;}
bool NativeSourceWindowMemory::store(uint32_t a,uint32_t n,uint32_t value){
 if(running_||!extent(a,n)||!current())return false;
 Running guard(running_);if(!allBindingsCurrent())return false;
 std::array<uint8_t*,4>cells{};for(unsigned i=0;i<n;++i)if(!(cells[i]=cell(a+i,false)))return false;
 std::vector<std::pair<NativeSourceWindowBinding*,uint32_t>>added;
 for(auto*b:bindings_)for(unsigned i=0;i<n;++i)if(!known(a+i)){if(!b->vm_->unmappedRange(a+i,1)||!b->vm_->registerExternalRam(a+i,cells[i],1)){for(auto p:added)p.first->vm_->unregisterExternalRam(p.second);return false;}added.push_back({b,a+i});}
 for(auto p:added)p.first->mappings_.push_back(p.second);
 for(unsigned i=0;i<n;++i){*cells[i]=uint8_t(value>>(8*(n-i-1)));mark(a+i);}return true;
}
bool NativeSourceWindowMemory::storeByte(uint32_t a,uint8_t v){return store(a,1,v);}bool NativeSourceWindowMemory::storeHalf(uint32_t a,uint16_t v){return store(a,2,v);}bool NativeSourceWindowMemory::storeWord(uint32_t a,uint32_t v){return store(a,4,v);}
void NativeSourceWindowMemory::retire(){if(running_||checking_)return;for(auto*b:bindings_)if(b->checking_)return;invalid_=true;while(!bindings_.empty())bindings_.back()->retire();}
}
