#include "battle_vm_message_lookup.hpp"
#include <cstring>
#include <limits>
namespace ttyd {namespace {
constexpr uint64_t GuestAddressSpaceSize=uint64_t{1}<<32;
struct Guard{bool&v;explicit Guard(bool&b):v(b){v=true;}~Guard(){v=false;}};
}
std::shared_ptr<BattleVmMessageLookup>BattleVmMessageLookup::create(BattleVmMessageLookupBinding b){
 if(!b.messages||b.vmLifetime.expired()||!b.addressSpaceIdentity||!b.current||!b.byte||!b.unmapped||!b.registerBytes||!b.base||!b.capacity||!b.maximumKeyBytes||uint64_t(b.base)+b.capacity>GuestAddressSpaceSize)return {};
 const auto lifetime=b.vmLifetime.lock();if(!lifetime||!b.current()||!b.unmapped(b.base,b.capacity)||!b.current())return {};
 auto result=std::shared_ptr<BattleVmMessageLookup>(new BattleVmMessageLookup);
 // Copy before exposing any cache pointer: even a mutable external alias to the
 // caller's resource cannot reload the source bytes behind this VM snapshot.
 result->snapshot_=std::make_shared<const MessageAssets>(*b.messages);
 b.messages.reset();result->binding_=std::move(b);if(!result->current())return {};
 std::map<uint32_t,const char*>aliases;
 for(const auto&entry:result->binding_.existing){
  if(!entry.second||entry.first.find('\0')!=std::string::npos)return {};
  const char*text=result->snapshot_->find(entry.first.c_str());if(!text)return {};
  const size_t length=std::strlen(text)+1;if(uint64_t(entry.second)+length>GuestAddressSpaceSize)return {};
  const auto alias=aliases.find(entry.second);if(alias!=aliases.end()&&alias->second!=text)return {};
  const auto prior=result->pointers_.find(text);if(prior!=result->pointers_.end()&&prior->second!=entry.second)return {};
  for(size_t i=0;i<length;++i){const auto byte=result->binding_.byte(entry.second+uint32_t(i));if(!result->valid()||!byte||*byte!=uint8_t(text[i]))return {};}
  aliases.emplace(entry.second,text);result->pointers_.emplace(text,entry.second);
 }
 result->binding_.existing.clear();return result;
}
bool BattleVmMessageLookup::valid()const{
 if(invalid_||checking_)return false;
 Guard guard(checking_);if(binding_.vmLifetime.expired()||!snapshot_||!binding_.current()){invalid_=true;return false;}return true;
}
bool BattleVmMessageLookup::current()const{if(invoking_)return false;return valid();}
std::optional<uint32_t>BattleVmMessageLookup::lookup(uint32_t actualSourceKey,BattleVmMessageLookupCursor&cursor){
 if(invoking_||cursor.invoking_)return {};
 Guard guard(invoking_);Guard cursorGuard(cursor.invoking_);
 const auto vm=binding_.vmLifetime.lock();if(!vm||!valid()||(cursor.owner_&&cursor.owner_.get()!=this)||(cursor.key_&&*cursor.key_!=actualSourceKey))return {};
 if(!cursor.key_){cursor.key_=actualSourceKey;cursor.owner_=shared_from_this();}
 while(!cursor.complete_){
  if(cursor.bytes_.size()>=binding_.maximumKeyBytes||uint64_t(actualSourceKey)+cursor.bytes_.size()>=GuestAddressSpaceSize)return {};
  const auto next=binding_.byte(actualSourceKey+uint32_t(cursor.bytes_.size()));if(!valid()||!next)return {};
  if(!*next){cursor.complete_=true;break;}
  cursor.bytes_.push_back(char(*next));
 }
 // MessageAssets exactly preserves room-before-global and rejects ambiguous
 // keys. Missing original fallback-table ownership is a hold, never source0.
 const char*text=snapshot_->find(cursor.bytes_.c_str());if(!text)return {};
 const auto known=pointers_.find(text);
 if(known!=pointers_.end()){const auto result=known->second;cursor.key_.reset();cursor.owner_.reset();cursor.bytes_.clear();cursor.complete_=false;return result;}
 const size_t length=std::strlen(text)+1;
 if(length>std::numeric_limits<uint32_t>::max()||uint64_t(used_)+length>binding_.capacity)return {};
 const uint32_t pointer=binding_.base+used_;
 const auto first=reinterpret_cast<const uint8_t*>(text);std::vector<uint8_t>bytes(first,first+length);
 if(!binding_.unmapped(pointer,length)||!valid()||!binding_.registerBytes(pointer,std::move(bytes)))return {};
 // registerBytes is a synchronous atomic actual VM store, not an external
 // source child. It must not retire the VM or call source code after commit.
 pointers_.emplace(text,pointer);used_+=uint32_t(length);cursor.key_.reset();cursor.owner_.reset();cursor.bytes_.clear();cursor.complete_=false;
 return valid()?std::optional<uint32_t>(pointer):std::nullopt;
}
void BattleVmMessageLookup::retire(){invalid_=true;}
}
