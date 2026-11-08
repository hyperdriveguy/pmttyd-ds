#pragma once
#include "event_manager.hpp"
#include <memory>
#include <optional>
namespace ttyd {
struct SourceMarioDisplayFlagServices {
 std::shared_ptr<const void>lifetime;void*user=nullptr;bool(*current)(void*)=nullptr;
 std::optional<uint32_t>(*word)(void*,uint32_t)=nullptr;
 bool(*storeWord)(void*,uint32_t,uint32_t)=nullptr;
 std::optional<int32_t>(*eventValue)(void*,EventEntry&,uint32_t)=nullptr;
};
// Whole original evt_mario_dispflag_onoff800e75a8/90. Source+4 mask only;
// no native presentation/readiness or skipped original jump acknowledgment.
class SourceMarioDisplayFlags {
public:
 SourceMarioDisplayFlags();~SourceMarioDisplayFlags();
 SourceMarioDisplayFlags(const SourceMarioDisplayFlags&)=delete;
 SourceMarioDisplayFlags&operator=(const SourceMarioDisplayFlags&)=delete;
 std::optional<int32_t>resume(EventEntry&,uint32_t actualEventAddress,const SourceMarioDisplayFlagServices&);
 bool complete()const;bool reset();uint32_t pending()const;
private:struct State;std::unique_ptr<State>state_;bool invoking_=false;
};
}
