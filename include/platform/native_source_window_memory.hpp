#pragma once
#include <memory>
#include <array>
#include <vector>
#include <optional>
#include "event_address_space.hpp"
namespace ttyd {
class NativeSourceWindowFile {
public:
 static std::shared_ptr<const NativeSourceWindowFile>load(const char*);
 NativeSourceWindowFile(const NativeSourceWindowFile&)=delete;
 NativeSourceWindowFile&operator=(const NativeSourceWindowFile&)=delete;
 const std::vector<uint8_t>&bytes()const{return bytes_;}
 static constexpr const char*SourcePath="f/msgWindow.tpl";
private:
 NativeSourceWindowFile()=default;std::vector<uint8_t>bytes_;
};
class NativeSourceWindowBinding;
// Process history only. Neither file loading nor memory binding runs original
// windowTexSetup or authorizes source completion/object provenance.
class NativeSourceWindowMemory:public std::enable_shared_from_this<NativeSourceWindowMemory> {
public:
 static std::shared_ptr<NativeSourceWindowMemory>create(std::shared_ptr<const void> lifetime,void*user,bool(*current)(void*));
 NativeSourceWindowMemory(const NativeSourceWindowMemory&)=delete;
 NativeSourceWindowMemory&operator=(const NativeSourceWindowMemory&)=delete;
 std::unique_ptr<NativeSourceWindowBinding>bind(EventAddressSpace&,std::shared_ptr<const void>vmLifetime,void*user,bool(*current)(void*));
 bool current()const;bool captured()const{return captured_;}
 // Actual heap owns backing/capacity; actual DVD owner supplies observed bytes.
 // Reserved but unread/unwritten bytes remain owned and unavailable.
 bool adoptAllocation(uint32_t,uint8_t*,uint32_t,std::shared_ptr<const void>);
 bool installRead(uint32_t,const std::vector<uint8_t>&);
 bool owns(uint32_t,uint32_t size=1)const;
 std::optional<uint8_t>byte(uint32_t)const;
 std::optional<uint16_t>half(uint32_t)const;
 std::optional<uint32_t>word(uint32_t)const;
 bool storeByte(uint32_t,uint8_t);bool storeHalf(uint32_t,uint16_t);bool storeWord(uint32_t,uint32_t);
 // Must run while ALL borrowed VMs are alive, outside raw borrows.
 void retire();
 static constexpr uint32_t StateBase=0x8041ea18,StateSize=16;
private:
 NativeSourceWindowMemory()=default;
 struct Allocation{uint32_t base,capacity;uint8_t*backing;std::shared_ptr<const void>owner;std::vector<uint8_t>known;bool installed=false;};
 std::shared_ptr<const void>lifetime_;void*user_=nullptr;bool(*current_)(void*)=nullptr;
 mutable bool invalid_=false,checking_=false;bool running_=false,captured_=false;
 std::array<uint8_t,StateSize>state_{};std::array<bool,StateSize>known_{};
 std::vector<Allocation>allocations_;std::vector<NativeSourceWindowBinding*>bindings_;
 uint8_t*cell(uint32_t,bool requireKnown)const;bool known(uint32_t)const;void mark(uint32_t);
 bool store(uint32_t,uint32_t,uint32_t);bool allBindingsCurrent()const;
 friend class NativeSourceWindowBinding;
};
class NativeSourceWindowBinding {
public:
 ~NativeSourceWindowBinding();bool current()const;void retire();
 NativeSourceWindowBinding(const NativeSourceWindowBinding&)=delete;
 NativeSourceWindowBinding&operator=(const NativeSourceWindowBinding&)=delete;
private:
 NativeSourceWindowBinding(std::shared_ptr<NativeSourceWindowMemory>,EventAddressSpace&);
 std::shared_ptr<NativeSourceWindowMemory>owner_;EventAddressSpace*vm_;
 std::shared_ptr<const void>lifetime_;void*user_=nullptr;bool(*current_)(void*)=nullptr;
 std::array<std::optional<uint8_t>,NativeSourceWindowMemory::StateSize>originals_{};
 std::vector<uint32_t>mappings_;bool bound_=false;mutable bool stale_=false,checking_=false;
 friend class NativeSourceWindowMemory;
};
}
