#pragma once
#include "animation_group_request.hpp"
#include "native_file_cache.hpp"
namespace ttyd {
// Same-VM file holders/AG2TG payload and shared staged-ROMFS cache. Filename
// formatting/strcmp use bounded native byte adapters; source SDK optimized
// string loads, DVD worker cadence and arbitrary C callbacks are separate.
class AnimationGroupRequestBinding {
public:
 static std::unique_ptr<AnimationGroupRequestBinding> bind(EventAddressSpace&,std::shared_ptr<NativeFileCache>);
 std::optional<uint32_t> advance(uint32_t name,uint32_t mode,uint32_t completion=0,unsigned visits=8192);
 bool pending() const{return cursor_.phase!=0;}
private:
 AnimationGroupRequestBinding(EventAddressSpace&,std::shared_ptr<NativeFileCache>);
 AnimationGroupRequestServices services();bool text(uint32_t,std::string&,unsigned limit=128) const;
 // File cache retained,VM borrowed. Both and stable binding outlive pending.
 // Default 8192 visits is native work budget;128-byte text limit is admission.
 EventAddressSpace& memory_;std::shared_ptr<NativeFileCache> files_;AnimationGroupRequestCursor cursor_;
};
}
