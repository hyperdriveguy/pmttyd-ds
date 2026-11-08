#pragma once
#include <memory>
#include <array>
#include <vector>
#include <optional>
#include "event_address_space.hpp"
namespace ttyd {
class NativeSourceIconFile {
public:
 enum class Kind {Bin,Tpl};
 static std::shared_ptr<const NativeSourceIconFile>load(const char*,Kind);
 NativeSourceIconFile(const NativeSourceIconFile&)=delete;
 NativeSourceIconFile&operator=(const NativeSourceIconFile&)=delete;
 const std::vector<uint8_t>&bytes()const{return bytes_;}
 Kind kind()const{return kind_;}
 // Exact logical file prefix; rounded DVD suffix is separately admitted.
 static constexpr const char*BinPath="icon.bin",*TplPath="icon.tpl";
private:
 NativeSourceIconFile()=default;Kind kind_=Kind::Bin;std::vector<uint8_t>bytes_;
};
// Immutable actual source inputs; no synthetic directory/palette/SDK handles.
class NativeSourceIconAssets {
public:
 static std::shared_ptr<const NativeSourceIconAssets>load(const char*rawDirectory,const char*startupPack);
 const std::shared_ptr<const NativeSourceIconFile>&bin()const{return bin_;}
 const std::shared_ptr<const NativeSourceIconFile>&tpl()const{return tpl_;}
 const std::vector<uint8_t>&roundedBin()const{return roundedBin_;}
 bool owns(uint32_t,uint32_t size=1)const;
 std::optional<uint8_t>byte(uint32_t)const;
 std::optional<uint16_t>half(uint32_t)const;
 std::optional<uint32_t>word(uint32_t)const;
private:
 NativeSourceIconAssets()=default;
 std::shared_ptr<const NativeSourceIconFile>bin_,tpl_;
 std::vector<uint8_t>constants_,startup_,roundedBin_;
};
class NativeSourceIconBinding;
// Process history only. Neither file loading nor memory binding runs original
// iconInit/callbacks or authorizes source completion/object provenance.
class NativeSourceIconMemory:public std::enable_shared_from_this<NativeSourceIconMemory> {
public:
 static std::shared_ptr<NativeSourceIconMemory>create(std::shared_ptr<const void> lifetime,void*user,bool(*current)(void*));
 NativeSourceIconMemory(const NativeSourceIconMemory&)=delete;
 NativeSourceIconMemory&operator=(const NativeSourceIconMemory&)=delete;
 std::unique_ptr<NativeSourceIconBinding>bind(EventAddressSpace&,std::shared_ptr<const void>vmLifetime,void*user,bool(*current)(void*));
 bool current()const;bool captured()const{return captured_;}
 // Actual heap owns backing/capacity; actual DVD owner supplies observed bytes.
 // Reserved but unread/unwritten bytes remain owned and unavailable.
 bool adoptAllocation(uint32_t,uint8_t*,uint32_t,std::shared_ptr<const void>);
 bool installRead(uint32_t,const std::vector<uint8_t>&);
 // Preserve actual source allocator writes when transferring its detached RAM.
 // The unread capacity tail remains owned and unknown.
 bool adoptHeapAllocation(uint32_t,uint8_t*,uint32_t,uint32_t knownPrefix,std::shared_ptr<const void>);
 // One later real DVD delivery overwrites only this captured known prefix.
 bool deliverHeapRead(uint32_t,const std::vector<uint8_t>&);
 bool owns(uint32_t,uint32_t size=1)const;
 std::optional<uint8_t>byte(uint32_t)const;
 std::optional<uint16_t>half(uint32_t)const;
 std::optional<uint32_t>word(uint32_t)const;
 bool storeByte(uint32_t,uint8_t);bool storeHalf(uint32_t,uint16_t);bool storeWord(uint32_t,uint32_t);
 // Must run while ALL borrowed VMs are alive, outside raw borrows.
 void retire();
 static constexpr uint32_t StateBase=0x8041e630,StateSize=32;
private:
 NativeSourceIconMemory()=default;
 struct Allocation{uint32_t base,capacity;uint8_t*backing;std::shared_ptr<const void>owner;std::vector<uint8_t>known;bool installed=false;uint32_t heapKnown=0;};
 std::shared_ptr<const void>lifetime_;void*user_=nullptr;bool(*current_)(void*)=nullptr;
 mutable bool invalid_=false,checking_=false;bool running_=false,captured_=false;
 std::array<uint8_t,StateSize>state_{};std::array<bool,StateSize>known_{};
 std::vector<Allocation>allocations_;std::vector<NativeSourceIconBinding*>bindings_;
 uint8_t*cell(uint32_t,bool requireKnown)const;bool known(uint32_t)const;void mark(uint32_t);
 bool store(uint32_t,uint32_t,uint32_t);bool allBindingsCurrent()const;
 friend class NativeSourceIconBinding;
};
class NativeSourceIconBinding {
public:
 ~NativeSourceIconBinding();bool current()const;void retire();
 NativeSourceIconBinding(const NativeSourceIconBinding&)=delete;
 NativeSourceIconBinding&operator=(const NativeSourceIconBinding&)=delete;
private:
 NativeSourceIconBinding(std::shared_ptr<NativeSourceIconMemory>,EventAddressSpace&);
 std::shared_ptr<NativeSourceIconMemory>owner_;EventAddressSpace*vm_;
 std::shared_ptr<const void>lifetime_;void*user_=nullptr;bool(*current_)(void*)=nullptr;
 std::array<std::optional<uint8_t>,NativeSourceIconMemory::StateSize>originals_{};
 std::vector<uint32_t>mappings_;bool bound_=false;mutable bool stale_=false,checking_=false;
 friend class NativeSourceIconMemory;
};
}
