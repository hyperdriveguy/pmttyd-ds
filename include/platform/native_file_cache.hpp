#pragma once
#include "event_address_space.hpp"
#include "file_acquisition.hpp"
#include "file_async.hpp"
#include "file_texture_relocation.hpp"
#include <array>
#include <memory>
#include <string>
#include <vector>
namespace ttyd {
// One process's file manager, shared by menu/animation consumers and VM views.
// Original pool/acquisition/relocation/release operate on these actual BE bytes.
// ROMFS reads and stable backed address tokens are explicit native policy;
// source SDK/smart heap placement, eviction/movement and DVD workers are separate.
class NativeFileCache {
public:
 static std::shared_ptr<NativeFileCache> create();
 NativeFileCache(const NativeFileCache&)=delete;
 NativeFileCache& operator=(const NativeFileCache&)=delete;
 // Catalog is immutable after first successful bind/acquisition. Stage original
 // unrelocated bytes; supported types are0(raw),4(TPL),5(nested MODEL).
 struct Source {std::string path;uint32_t type;std::vector<uint8_t> original;};
 bool stage(const std::string& path,uint32_t type,std::vector<uint8_t> original);
 bool stage(std::vector<Source>); // Atomic catalog addition, including validation.
 bool yieldProvider(void*,bool (*)(void*));
 bool hasYieldProvider() const{return yield_!=nullptr;} // Configure before first bind/acquire.
 bool bind(EventAddressSpace&); // Owner outlives every borrowing VM.
 // One serialized retained acquisition; nullopt means unavailable, engaged0
 // completed source failure. New-file type must match staged bytes, but actual
 // source filename-only hits may reuse them for a different requested type.
 // A finite formatted wrapper publishes the real shared filename first.
 // Publication refuses another held acquisition/request. published=true then
 // consumes that actual buffer without a duplicate copy; it must stay stable.
 bool publishFilename(const std::string&);
 std::optional<uint32_t> acquire(const std::string&,uint32_t type,bool published=false);
 // Original async request over this same file pool. Empty means native hold;
 // engaged0 is source polling, not acquisition. Completion callbacks currently
 // admit only0; unknown function providers must be bound before broader use.
 std::optional<uint32_t> request(const std::string&,uint32_t type,uint32_t completion=0,bool published=false);
 // Cooperative staged-ROMFS worker policy. Copies at most byteBudget actual
 // bytes across queued files, then runs original completion/relocation/close.
 // This is an explicit IO boundary, not GameCube DVD/thread timing.
 bool advanceAsync(uint32_t byteBudget=4096);
 bool asyncPending() const;
 bool aliases(EventAddressSpace&) const; // Exact backed owner identity, not equal bytes.
 bool release(uint32_t entry);
 bool archiveType(uint32_t);
 bool contains(const std::string&,uint32_t) const;
 bool ownsPayload(uint32_t address,uint32_t bytes) const;
 // Exact immutable source identity and live relocated static geometry closure.
 // Timelines/deltas and output buffers are separate consumers. Checks actual
 // VM backing in a synchronous borrow; no detached payload fallback.
 // Synchronous static admission only: immutable identity may be cached weakly,
 // but live bytes/relocated roots are rechecked on every call. No raw lease escapes.
 bool modelGeometryCurrent(EventAddressSpace&,uint32_t,std::shared_ptr<const class AnimationDrawResource>) const;
 static constexpr uint32_t Manager=0x8041ec68,Global=0x80418498;
private:
 NativeFileCache();
 struct File {
  std::string path;uint32_t type=0,address=0,headerAddress=0,entry=0;
  std::array<uint8_t,12> header{};
  std::vector<uint8_t> original,payload;
  FileTextureRelocationCursor relocation;
  uint32_t disc=0,loaded=0;std::array<uint8_t,0xa0> discState{};
  bool queued=false,readDone=false;FileAsyncCompletionCursor completion;
  // Exact original-byte equality was proved for this immutable draw identity.
  // Weak ownership avoids extending resource life or caching live geometry state.
  mutable std::weak_ptr<const class AnimationDrawResource> geometryIdentity;
 };
 // Explicit native backed tokens, not recovered heap placement: pool at
 // 0x87000000; first payload has1MiB before headers; later payloads use
 // [0x87500000,0x88000000). Holders reserve32-byte slots with12 backed bytes.
 // Name/Manager/Global retain original guest identities; Disc is a backend token.
 static constexpr uint32_t Pool=0x87000000,FirstPayload=0x87100000,
  Headers=0x87200000,Name=0x803e6e78,Disc=0x87400000,MorePayloads=0x87500000,PayloadLimit=0x88000000;
 std::array<uint8_t,4> global_{};
 std::array<uint8_t,24> manager_{};
 std::array<uint8_t,1024> name_{};
 std::vector<uint8_t> pool_;
 std::vector<std::unique_ptr<File>> files_;
 File* current_=nullptr;
 uint32_t nextPayload_=MorePayloads;
 uint16_t discFlags_=0;
 bool frozen_=false;
 void* yieldUser_=nullptr;bool (*yield_)(void*)=nullptr;
 FileAcquisitionCursor acquisition_;FileAsyncCursor async_;bool servicing_=false;
 uint8_t* span(uint32_t,unsigned);
 uint8_t byte(uint32_t);void byte(uint32_t,uint8_t);
 uint16_t half(uint32_t);void half(uint32_t,uint16_t);
 uint32_t word(uint32_t);void word(uint32_t,uint32_t);
 bool filenameMatches(const std::string&)const;
 EventMemoryServices memory();
 FileAcquisitionServices services();
 FileAsyncServices asyncServices();
};// Stages complete unchanged original model/TPL/auxiliary files from accepted
// local packs, deduplicated by exact source filename. No acquisition/startup
// acknowledgement; call before binding this same process cache to any VM.
bool stagePrologueAnimationFiles(NativeFileCache&,const char* romfsRoot);
}
