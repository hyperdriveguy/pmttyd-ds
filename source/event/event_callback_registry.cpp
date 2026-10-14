#include "event_callback_registry.hpp"
#include <cstdlib>

namespace ttyd {
bool EventCallbackRegistry::bind(uint32_t address,void* user,Callback callback) {
    if(!address || !callback || contains(address))return false;
    bindings_.push_back({address,user,callback});return true;
}
bool EventCallbackRegistry::unbind(uint32_t address) {
    for(size_t i=0;i<bindings_.size();++i)if(bindings_[i].address==address) {bindings_.erase(bindings_.begin()+i);return true;}
    return false;
}
bool EventCallbackRegistry::contains(uint32_t address) const {for(const auto& b:bindings_)if(b.address==address)return true;return false;}
bool EventCallbackRegistry::invoke(EventEntry& e,uint32_t address,int32_t first,int32_t& result) const {
    for(const auto& b:bindings_)if(b.address==address) {
        // Copy before invocation:provider may mutate/reallocate this registry.
        const auto callback=b.callback;auto* user=b.user;result=callback(user,e,first);return true;
    }
    return false;
}
int32_t EventCallbackRegistry::dispatch(void* p,EventEntry& e,uint32_t address,int32_t first) {
    int32_t result;if(!p || !static_cast<EventCallbackRegistry*>(p)->invoke(e,address,first,result))std::abort();return result;
}
}
