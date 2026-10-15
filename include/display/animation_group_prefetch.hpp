#pragma once
#include "event_manager.hpp"
#include <cstdint>
#include <memory>
#include <string>
#include <vector>
namespace ttyd {
// Source request return -1/nonzero is completed; 0 pending. archive selection
// precedes both requests even if a previously completed group is cached.
struct AnimationGroupAsyncServices {
 void* user=nullptr;
 void (*archive)(void*,uint32_t)=nullptr;
 int32_t (*request)(void*,uint32_t type,uint32_t flags,const std::string& path)=nullptr;
};
// Original mode0/flags0 helper. Table is the unchanged ag2tg payload, scanned
// in32-byte steps; matching row's following row supplies the texture name.
bool animationGroupBaseAsync(const char* name,const std::vector<uint8_t>& table,const AnimationGroupAsyncServices&);
struct AnimationGroupPollServices {void* user=nullptr;bool (*poll)(void*,const char*)=nullptr;};
int32_t fieldBeroReadMarioPera(EventEntry&,bool first,const AnimationGroupPollServices&);
// Native cooperative ROMFS adaptation, not the original DVD/cache allocator.
// One open request yields; subsequent polls read at most chunkBytes per file.
// Completed payloads remain owned here, with no pose/refcount side effects.
class AnimationGroupPrefetch {
public:
 AnimationGroupPrefetch();
 ~AnimationGroupPrefetch();
 AnimationGroupPrefetch(const AnimationGroupPrefetch&)=delete;
 AnimationGroupPrefetch& operator=(const AnimationGroupPrefetch&)=delete;
 // chunkBytes4096 is native cooperative I/O policy, not original DVD timing.
 bool open(const char* root,uint32_t chunkBytes=4096);
 bool poll(const char* group);
 const std::vector<uint8_t>* resident(const std::string& path) const;
 uint32_t requestCount() const;
 bool failed() const;
private:
 struct Impl;std::unique_ptr<Impl> impl_;
};
}
