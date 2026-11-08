#pragma once
#include "event_manager.hpp"
#include <memory>
#include <optional>
namespace ttyd {
struct SourceMarioPoseServices {
 std::shared_ptr<const void>lifetime;void*user=nullptr;bool(*current)(void*)=nullptr;
 std::optional<uint32_t>(*word)(void*,uint32_t)=nullptr;
 std::optional<uint16_t>(*half)(void*,uint32_t)=nullptr;
 bool(*storeWord)(void*,uint32_t,uint32_t)=nullptr;
 std::optional<int32_t>(*eventValue)(void*,EventEntry&,uint32_t)=nullptr;
 std::optional<int32_t>(*compare)(void*,uint32_t left,uint32_t right)=nullptr;
 // Actual toDotMarioPose80058f9c child; engaged zero is real source return.
 std::optional<uint32_t>(*dotPose)(void*,uint32_t requestedPose)=nullptr;
};
// Whole original event wrapper. Selecting a real source pose pointer/flags does
// not certify renderer pose upload, playback, endpoint or original MarioMain.
class SourceMarioSetPose {
public:
 SourceMarioSetPose();~SourceMarioSetPose();
 SourceMarioSetPose(const SourceMarioSetPose&)=delete;
 SourceMarioSetPose&operator=(const SourceMarioSetPose&)=delete;
 std::optional<int32_t>resume(EventEntry&,uint32_t actualEventAddress,const SourceMarioPoseServices&);
 bool complete()const;bool reset();uint32_t pending()const;
private:struct State;std::unique_ptr<State>state_;bool invoking_=false;
};
}
