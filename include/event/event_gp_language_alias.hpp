#pragma once
#include <cstdint>
#include <functional>
#include <memory>
namespace ttyd {
class EventGpAddresses;
// Actual one registered source GP word, not a language value/cache or wholeGP
// mirror. Strong lifetime pins the source/proof; pure current freezes it through
// one synchronous transaction. Source registration identity prevents ABA reuse.
class EventGpLanguageAlias {
public:
 EventGpLanguageAlias(EventGpAddresses&,uint32_t sourceBase,
                      std::shared_ptr<const void> sourceLifetime,std::function<bool()> current);
 EventGpLanguageAlias(const EventGpLanguageAlias&)=delete;
 EventGpLanguageAlias&operator=(const EventGpLanguageAlias&)=delete;
 bool current()const;
 bool sourceIs(const EventGpAddresses&)const;
 bool read(uint32_t&)const;
 bool modify(uint32_t mask,uint32_t value);
 void retire();
private:
 EventGpAddresses* source_;uint32_t base_;
 std::shared_ptr<const uint8_t> registration_;
 std::shared_ptr<const void> lifetime_;std::function<bool()> current_;
 mutable bool invoking_=false,invalid_=false;
 bool valid()const;
};
}
