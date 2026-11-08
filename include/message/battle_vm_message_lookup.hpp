#pragma once
#include "message_assets.hpp"
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>
#include <map>
namespace ttyd {
// Private native VM cache, not the original msgdrv sorted-table allocation.
// Actual guest registrations are immutable/permanent until that VM teardown.
struct BattleVmMessageLookupBinding {
 std::shared_ptr<const MessageAssets> messages;
 std::weak_ptr<const void> vmLifetime;
 const void* addressSpaceIdentity=nullptr;
 std::function<bool()> current;
 std::function<std::optional<uint8_t>(uint32_t)> byte;
 std::function<bool(uint32_t,uint64_t)> unmapped;
 std::function<bool(uint32_t,std::vector<uint8_t>)> registerBytes;
 uint32_t base=0,capacity=0;
 // Native admission bound only; never adds a source terminator on exhaustion.
 size_t maximumKeyBytes=0;
 // Existing permanent VM cache entries, verified byte-for-byte against the
 // same immutable resource entry before preserving their alias identity.
 std::vector<std::pair<std::string,uint32_t>>existing;
};
class BattleVmMessageLookup;
// Actual caller-owned native read progress; abandoning a frame discards only
// its private cursor, while immutable per-VM registrations remain resident.
class BattleVmMessageLookupCursor {
public:
 BattleVmMessageLookupCursor()=default;
 BattleVmMessageLookupCursor(const BattleVmMessageLookupCursor&)=delete;
 BattleVmMessageLookupCursor&operator=(const BattleVmMessageLookupCursor&)=delete;
 uint32_t pendingKey()const{return key_.value_or(0);}
private:
 friend class BattleVmMessageLookup;
 std::shared_ptr<const BattleVmMessageLookup>owner_;
 std::optional<uint32_t>key_;std::string bytes_;bool complete_=false,invoking_=false;
};
class BattleVmMessageLookup:public std::enable_shared_from_this<BattleVmMessageLookup> {
public:
 static std::shared_ptr<BattleVmMessageLookup>create(BattleVmMessageLookupBinding);
 std::optional<uint32_t>lookup(uint32_t actualSourceKey,BattleVmMessageLookupCursor&);
 bool current()const;
 size_t mappedEntries()const{return pointers_.size();}
 uint32_t mappedBytes()const{return used_;}
 void retire();
private:
 BattleVmMessageLookup()=default;
 BattleVmMessageLookupBinding binding_;
 std::shared_ptr<const MessageAssets>snapshot_;
 std::map<const char*,uint32_t>pointers_;
 uint32_t used_=0;mutable bool invoking_=false,checking_=false,invalid_=false;
 bool valid()const;
};
}
