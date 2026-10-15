#pragma once
#include <memory>
#include <optional>
#include <array>
#include <utility>
#include <vector>
#include "event_address_space.hpp"
namespace ttyd {
// Immutable exact original US BFN from the accepted private raw-file pack.
// Runtime FNV is a trusted-pack corruption guard; converter pins SHA-256.
class NativeSourceFontFile {
public:
 static std::shared_ptr<const NativeSourceFontFile> load(const char*);
 NativeSourceFontFile(const NativeSourceFontFile&)=delete;
 NativeSourceFontFile&operator=(const NativeSourceFontFile&)=delete;
 const std::vector<uint8_t>& bytes()const{return bytes_;}
 static constexpr const char* SourcePath="f/papermarioset_US.bfn";
private:
 NativeSourceFontFile()=default;
 std::vector<uint8_t> bytes_;
};
class NativeSourceFontBinding;
// One process owner, capturing real source state rather than initializing it.
// VM bindings must be destroyed BEFORE their borrowed EventAddressSpace,
// outside raw borrows. The caller must not mutate their owned VM mappings.
class NativeSourceFontMemory:public std::enable_shared_from_this<NativeSourceFontMemory> {
public:
 static std::shared_ptr<NativeSourceFontMemory> create();
 NativeSourceFontMemory(const NativeSourceFontMemory&)=delete;
 NativeSourceFontMemory&operator=(const NativeSourceFontMemory&)=delete;
 std::unique_ptr<NativeSourceFontBinding> bind(EventAddressSpace&);
 // Actual heap child supplies live native storage and its strong allocation
 // owner; guest base is explicit native mapping policy, not a recovered heap.
 bool adoptAllocation(uint32_t base,uint8_t* backing,uint32_t capacity,std::shared_ptr<const void> allocationOwner);
 // Actual DVD child supplies exactly its observed bytes. Only that prefix is
 // copied/mapped. Unknown unused capacity is never zero-filled/readable.
 bool installRead(uint32_t base,const std::vector<uint8_t>& actualBytes);
 // Native source-heap transfer ONLY: backing/knownPrefix are actual captured
 // allocator output, including real memset writes. Destination VM RAM has been
 // detached before adoption; no capacity tail is inferred or initialized.
 bool adoptHeapAllocation(uint32_t base,uint8_t* backing,uint32_t capacity,
                          uint32_t knownPrefix,std::shared_ptr<const void> allocationOwner);
 // Genuine later DVD Read may overwrite that captured known heap prefix once.
 // Does not change original unknown-adopt/installRead behavior or map a tail.
 bool deliverHeapRead(uint32_t base,const std::vector<uint8_t>& actualBytes);
 bool captured()const{return bool(snapshot_);}
 // Any intersection claims routing ownership, including unread/unknown tail.
 bool owns(uint32_t address,uint32_t size=1)const;
 std::optional<uint8_t> byte(uint32_t)const;
 std::optional<uint16_t> half(uint32_t)const;
 std::optional<uint32_t> word(uint32_t)const;
 bool storeByte(uint32_t,uint8_t);
 bool storeWord(uint32_t,uint32_t);
 // Original address/byte-size spans: font pointer+GX object/descriptors;
 // initialized palette/spacing/name table; two actual glyph-map BSS cells.
 // bind captures supplied bytes; these extents do not authorize default zeros.
 static constexpr std::array<std::pair<uint32_t,uint32_t>,4> StateSpans{{
  {0x8041e978,0x44},{0x80416010,0x44},{0x80429578,4},{0x80429598,4}}};
private:
 NativeSourceFontMemory()=default;
 struct Snapshot {std::array<std::vector<uint8_t>,4> spans;};
 // capacity reserves address ownership; known is the actual readable DVD
 // prefix. The strong owner pins backing storage even after all VMs unbind.
 struct Allocation {uint32_t base,capacity,known=0;uint8_t* backing;std::shared_ptr<const void> owner;bool heapTransferred=false,readDelivered=false;};
 std::unique_ptr<Snapshot> snapshot_;
 std::vector<Allocation> allocations_;
 std::vector<NativeSourceFontBinding*> bindings_;
 uint8_t* cell(uint32_t)const;
 friend class NativeSourceFontBinding;
};
// RAII alias lease: restore this VM's pre-bind state bytes on destruction,
// while shared process mutations/allocation bytes survive in the owner.
// The borrowed VM must outlive the lease, with mappings unchanged by outsiders.
class NativeSourceFontBinding {
public:
 ~NativeSourceFontBinding();
 NativeSourceFontBinding(const NativeSourceFontBinding&)=delete;
 NativeSourceFontBinding&operator=(const NativeSourceFontBinding&)=delete;
private:
 NativeSourceFontBinding(std::shared_ptr<NativeSourceFontMemory>,EventAddressSpace&);
 std::shared_ptr<NativeSourceFontMemory> owner_;EventAddressSpace* vm_;
 std::array<std::vector<uint8_t>,4> originals_;
 std::vector<uint32_t> payloads_;
 bool bound_=false;
 friend class NativeSourceFontMemory;
};
}
