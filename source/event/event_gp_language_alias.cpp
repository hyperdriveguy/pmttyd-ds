#include "event_gp_language_alias.hpp"
#include "event_gp_addresses.hpp"
namespace ttyd {namespace {
struct Guard{bool&v;explicit Guard(bool&b):v(b){v=true;}~Guard(){v=false;}};
}
EventGpLanguageAlias::EventGpLanguageAlias(EventGpAddresses&source,uint32_t base,std::shared_ptr<const void>lifetime,std::function<bool()>current):source_(&source),base_(base),registration_(source.languageRegistration(base)),lifetime_(std::move(lifetime)),current_(std::move(current)){}
bool EventGpLanguageAlias::valid()const{
 if(invalid_)return false;
 if(!lifetime_||!current_||!current_()){invalid_=true;return false;}
 if(invalid_)return false;
 if(!registration_||!source_->languageSourceCurrent(base_,registration_)){invalid_=true;return false;}
 return true;
}
bool EventGpLanguageAlias::current()const{if(invoking_)return false;Guard guard(invoking_);return valid();}
bool EventGpLanguageAlias::sourceIs(const EventGpAddresses&source)const{return source_==&source;}
bool EventGpLanguageAlias::read(uint32_t&value)const{if(invoking_)return false;Guard guard(invoking_);if(!valid())return false;uint32_t word;if(!source_->readWord(base_+0x16c,word)){invalid_=true;return false;}value=word;return true;}
bool EventGpLanguageAlias::modify(uint32_t mask,uint32_t value){if(invoking_)return false;Guard guard(invoking_);if(!valid())return false;uint32_t old;if(!source_->readWord(base_+0x16c,old)||!source_->writeWord(base_+0x16c,(old&~mask)|(value&mask))){invalid_=true;return false;}return true;}
void EventGpLanguageAlias::retire(){invalid_=true;if(invoking_)return;registration_.reset();lifetime_.reset();current_={};}
}

namespace ttyd {
std::shared_ptr<const uint8_t> EventGpAddresses::languageRegistration(uint32_t base)const{for(const auto&r:regions_)if(r.base==base)return r.registration;return {};}
bool EventGpAddresses::languageSourceCurrent(uint32_t base,const std::shared_ptr<const uint8_t>&token)const{
 if(languageInvoking_||!token||current_!=base)return false;
 for(const auto&r:regions_)if(r.base==base)return !r.language&&r.registration==token&&!r.registration.owner_before(token)&&!token.owner_before(r.registration);
 return false;
}
bool EventGpAddresses::bindLanguageAlias(uint32_t base,std::shared_ptr<EventGpLanguageAlias>alias){
 if(languageInvoking_||!alias||alias->sourceIs(*this))return false;
 Guard guard(languageInvoking_);
 for(auto&r:regions_)if(r.base==base){if(r.language||!alias->current())return false;r.language=std::move(alias);return true;}
 return false;
}
bool EventGpAddresses::languageAliasIs(uint32_t base,const std::shared_ptr<EventGpLanguageAlias>&alias)const{
 if(languageInvoking_||!alias)return false;
 for(const auto&r:regions_)if(r.base==base)return r.language==alias&&!r.language.owner_before(alias)&&!alias.owner_before(r.language);
 return false;
}
bool EventGpAddresses::languageAliasSpan(uint32_t a,uint64_t n)const{
 if(!n)return false;
 const uint64_t end=n>0x100000000ULL-a?0x100000000ULL:uint64_t(a)+n;
 for(const auto&r:regions_)if(r.language&&uint64_t(a)<uint64_t(r.base)+0x170&&uint64_t(r.base)+0x16c<end)return true;
 return false;
}
bool EventGpAddresses::readAliasSpan(uint32_t a,unsigned n,uint32_t&value)const{
 if(languageInvoking_||(n!=1&&n!=2&&n!=4)||uint64_t(a)+n>0x100000000ULL)return false;
 Guard guard(languageInvoking_);
 for(const auto&r:regions_)if(r.language&&uint64_t(a)<uint64_t(r.base)+0x170&&uint64_t(r.base)+0x16c<uint64_t(a)+n){
  uint32_t language;if(!r.language->read(language))return false;uint32_t out=0;
  for(unsigned i=0;i<n;++i){const auto at=a+i;uint8_t b;if(at>=r.base+0x16c&&at<r.base+0x170)b=uint8_t(language>>(24-8*(at-r.base-0x16c)));else if(!readByteLocal(at,b))return false;out=out<<8|b;}
  value=out;return true;
 }
 return false;
}
bool EventGpAddresses::writeAliasSpan(uint32_t a,unsigned n,uint32_t value){
 if(languageInvoking_||(n!=1&&n!=2&&n!=4)||uint64_t(a)+n>0x100000000ULL)return false;
 Guard guard(languageInvoking_);
 for(auto&r:regions_)if(r.language&&uint64_t(a)<uint64_t(r.base)+0x170&&uint64_t(r.base)+0x16c<uint64_t(a)+n){
  uint32_t mask=0,insert=0;
  for(unsigned i=0;i<n;++i){const auto at=a+i;uint8_t before;if(!readByteLocal(at,before))return false;if(at>=r.base+0x16c&&at<r.base+0x170){const unsigned shift=24-8*(at-r.base-0x16c);mask|=0xffu<<shift;insert|=uint32_t(uint8_t(value>>(8*(n-1-i))))<<shift;}}
  // Current proof executes before this genuine source-word mutation. Source
  // and destination maps are frozen through the noncallback commit below.
  if(!r.language->modify(mask,insert))return false;
  for(unsigned i=0;i<n;++i){const auto at=a+i;if(at>=r.base+0x16c&&at<r.base+0x170)continue;(void)writeByteLocal(at,uint8_t(value>>(8*(n-1-i))));}
  return true;
 }
 return false;
}
bool EventGpAddresses::readByte(uint32_t a,uint8_t&value)const{
 if(languageInvoking_)return false;
 if(languageAliasSpan(a,1)){uint32_t v;if(!readAliasSpan(a,1,v))return false;value=uint8_t(v);return true;}
 return readByteLocal(a,value);
}
bool EventGpAddresses::writeByte(uint32_t a,uint8_t value){
 if(languageInvoking_)return false;
 if(languageAliasSpan(a,1))return writeAliasSpan(a,1,value);
 return writeByteLocal(a,value);
}
}
