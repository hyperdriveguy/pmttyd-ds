#pragma once
#include "event_manager.hpp"
#include <vector>

namespace ttyd {
class EventCallbackRegistry {
public:
    using Callback=int32_t (*)(void*,EventEntry&,int32_t firstCall);
    // Native registration policy:one nonzero original address per live binding.
    // Provider state must outlive registration. Addresses are never host pointers.
    bool bind(uint32_t originalAddress,void* user,Callback);
    bool unbind(uint32_t originalAddress);
    bool contains(uint32_t originalAddress) const;
    bool invoke(EventEntry&,uint32_t originalAddress,int32_t firstCall,int32_t& result) const;
    // For EventNativePlatform.call. An unbound callback terminates as an explicit
    // unsupported native domain,including release builds;it never returns success.
    static int32_t dispatch(void*,EventEntry&,uint32_t originalAddress,int32_t firstCall);
private:
    struct Binding {uint32_t address;void* user;Callback callback;};
    std::vector<Binding> bindings_;
};
}
