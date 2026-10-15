#include "native_source_font_memory.hpp"
#include <algorithm>
#include <cstring>
#include <limits>
#include <cstdio>
namespace ttyd {namespace {
bool extent(uint32_t base,uint32_t size){return size&&uint64_t(base)+size<=0x100000000ULL;}
bool overlap(uint32_t a,uint32_t n,uint32_t b,uint32_t m){return uint64_t(a)<uint64_t(b)+m&&uint64_t(b)<uint64_t(a)+n;}
}
std::shared_ptr<const NativeSourceFontFile> NativeSourceFontFile::load(const char*path){
 FILE*f=path?std::fopen(path,"rb"):nullptr;if(!f)return {};
 auto file=std::shared_ptr<NativeSourceFontFile>(new NativeSourceFontFile);file->bytes_.resize(66560);
 bool ok=std::fread(file->bytes_.data(),file->bytes_.size(),1,f)==1&&std::fgetc(f)==EOF&&!std::ferror(f);std::fclose(f);
 if(!ok)return {};
 uint64_t hash=14695981039346656037ULL;for(auto b:file->bytes_){hash^=b;hash*=1099511628211ULL;}
 return hash==0xcdfbe0644553b623ULL?file:nullptr;
}
std::shared_ptr<NativeSourceFontMemory> NativeSourceFontMemory::create(){return std::shared_ptr<NativeSourceFontMemory>(new NativeSourceFontMemory);}
NativeSourceFontBinding::NativeSourceFontBinding(std::shared_ptr<NativeSourceFontMemory> owner,EventAddressSpace&vm):owner_(std::move(owner)),vm_(&vm){}
NativeSourceFontBinding::~NativeSourceFontBinding(){if(!bound_)return;
 for(auto base:payloads_)vm_->unregisterExternalRam(base);
 for(size_t i=0;i<originals_.size();++i){vm_->unregisterExternalRam(NativeSourceFontMemory::StateSpans[i].first);vm_->registerRam(NativeSourceFontMemory::StateSpans[i].first,std::move(originals_[i]));}
 auto&bindings=owner_->bindings_;bindings.erase(std::remove(bindings.begin(),bindings.end(),this),bindings.end());
}
std::unique_ptr<NativeSourceFontBinding> NativeSourceFontMemory::bind(EventAddressSpace&vm){
 for(const auto*b:bindings_)if(b->vm_==&vm)return {};
 auto binding=std::unique_ptr<NativeSourceFontBinding>(new NativeSourceFontBinding(shared_from_this(),vm));
 auto candidate=std::make_unique<Snapshot>();
 for(size_t i=0;i<StateSpans.size();++i){auto span=StateSpans[i];if(!vm.contiguousRam(span.first,span.second))return {};candidate->spans[i].resize(span.second);for(uint32_t j=0;j<span.second;++j)if(!vm.readByte(span.first+j,candidate->spans[i][j]))return {};}
 for(const auto&a:allocations_)if(!vm.unmappedRange(a.base,a.capacity))return {};
 auto* target=snapshot_?snapshot_.get():candidate.get();
 size_t extracted=0,registered=0;
 for(;extracted<StateSpans.size();++extracted)if(!vm.extractRam(StateSpans[extracted].first,StateSpans[extracted].second,binding->originals_[extracted]))break;
 if(extracted==StateSpans.size())for(;registered<StateSpans.size();++registered)if(!vm.registerExternalRam(StateSpans[registered].first,target->spans[registered].data(),StateSpans[registered].second))break;
 bool success=extracted==StateSpans.size()&&registered==StateSpans.size();
 if(success)for(const auto&a:allocations_)if(a.known){if(!vm.registerExternalRam(a.base,a.backing,a.known)){success=false;break;}binding->payloads_.push_back(a.base);}
 if(!success){for(auto base:binding->payloads_)vm.unregisterExternalRam(base);for(size_t i=0;i<registered;++i)vm.unregisterExternalRam(StateSpans[i].first);for(size_t i=0;i<extracted;++i)vm.registerRam(StateSpans[i].first,std::move(binding->originals_[i]));return {};}
 if(!snapshot_)snapshot_=std::move(candidate);
 binding->bound_=true;bindings_.push_back(binding.get());return binding;
}
bool NativeSourceFontMemory::adoptAllocation(uint32_t base,uint8_t*backing,uint32_t capacity,std::shared_ptr<const void> owner){
 if(!base||!backing||!owner||!extent(base,capacity))return false;
 const auto start=reinterpret_cast<uintptr_t>(backing);if(capacity>std::numeric_limits<uintptr_t>::max()-start)return false;
 for(auto span:StateSpans)if(overlap(base,capacity,span.first,span.second))return false;
 for(const auto&a:allocations_){const auto other=reinterpret_cast<uintptr_t>(a.backing);if(overlap(base,capacity,a.base,a.capacity)||(start<other+a.capacity&&other<start+capacity))return false;}
 for(auto*b:bindings_)if(!b->vm_->unmappedRange(base,capacity))return false;
 allocations_.push_back({base,capacity,0,backing,std::move(owner)});return true;
}
bool NativeSourceFontMemory::installRead(uint32_t base,const std::vector<uint8_t>&bytes){
 auto i=std::find_if(allocations_.begin(),allocations_.end(),[&](const auto&a){return a.base==base;});
 if(i==allocations_.end()||i->known||bytes.empty()||bytes.size()>i->capacity)return false;
 // Preflight the whole allocation, including unknown capacity, before mapping
 // or writing. No source callbacks/yields may interleave these mapping calls.
 for(auto*b:bindings_)if(!b->vm_->unmappedRange(i->base,i->capacity))return false;
 size_t registered=0;for(;registered<bindings_.size();++registered)if(!bindings_[registered]->vm_->registerExternalRam(i->base,i->backing,uint32_t(bytes.size())))break;
 if(registered!=bindings_.size()){for(size_t j=0;j<registered;++j)bindings_[j]->vm_->unregisterExternalRam(i->base);return false;}
 std::memmove(i->backing,bytes.data(),bytes.size());i->known=uint32_t(bytes.size());for(auto*b:bindings_)b->payloads_.push_back(i->base);return true;
}
bool NativeSourceFontMemory::owns(uint32_t address,uint32_t size)const{if(!extent(address,size))return false;for(auto span:StateSpans)if(overlap(address,size,span.first,span.second))return true;for(const auto&a:allocations_)if(overlap(address,size,a.base,a.capacity))return true;return false;}
uint8_t*NativeSourceFontMemory::cell(uint32_t address)const{
 if(snapshot_)for(size_t i=0;i<StateSpans.size();++i)if(address>=StateSpans[i].first&&uint64_t(address)<uint64_t(StateSpans[i].first)+StateSpans[i].second)return &snapshot_->spans[i][address-StateSpans[i].first];
 for(const auto&a:allocations_)if(address>=a.base&&uint64_t(address)<uint64_t(a.base)+a.known)return a.backing+address-a.base;
 return nullptr;
}
std::optional<uint8_t> NativeSourceFontMemory::byte(uint32_t at)const{auto*p=cell(at);return p?std::optional<uint8_t>(*p):std::nullopt;}
std::optional<uint16_t> NativeSourceFontMemory::half(uint32_t at)const{if(!extent(at,2))return {};auto a=byte(at),b=byte(at+1);return a&&b?std::optional<uint16_t>(uint16_t(*a)<<8|*b):std::nullopt;}
std::optional<uint32_t> NativeSourceFontMemory::word(uint32_t at)const{if(!extent(at,4))return {};uint32_t value=0;for(unsigned i=0;i<4;++i){auto b=byte(at+i);if(!b)return {};value=value<<8|*b;}return value;}
bool NativeSourceFontMemory::storeByte(uint32_t at,uint8_t value){auto*p=cell(at);if(!p)return false;*p=value;return true;}
bool NativeSourceFontMemory::storeWord(uint32_t at,uint32_t value){if(!extent(at,4))return false;std::array<uint8_t*,4> cells;for(unsigned i=0;i<4;++i)if(!(cells[i]=cell(at+i)))return false;for(unsigned i=0;i<4;++i)*cells[i]=uint8_t(value>>(24-8*i));return true;}
}
