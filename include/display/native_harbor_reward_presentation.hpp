#pragma once
#include "point_get.hpp"
#include "message_icons.hpp"
#include <functional>
#include <memory>
#include <optional>
namespace ttyd {
// Exact metadata shape of existing original icon404 and PointGet124..131.
bool validNativeHarborRewardArt(const MessageIcon&);
struct NativeHarborRewardIcon {
 std::array<float,3>position{};
 float scale=1;
 uint8_t alpha=0;
 bool hidden=true;
 uint64_t animationTick=0;
};
struct NativeHarborRewardPresentationSource {
 void*user=nullptr;
 std::shared_ptr<const void>lifetime;
 bool(*current)(void*)=nullptr;
 std::optional<uint32_t>(*word)(void*,uint32_t)=nullptr;
};
// Native camera8 presentation policy; owns no original End work or reward writes.
class NativeHarborRewardPresentation {
public:
 static constexpr unsigned TensSlots=10,OnesSlots=9,IconCount=TensSlots+OnesSlots;
 NativeHarborRewardPresentation(NativeHarborRewardPresentationSource);
 bool current()const;
 // Source scalar reads retain successful prefixes through unavailable memory.
 bool admit(int32_t total);
 // Call only after actual victory roots have been created by the coordinator.
 bool enterPointGet();
 bool update(int32_t remaining,uint64_t simulationTick);
 bool hideIcons();
 bool iconsActive()const{return iconsActive_;}
 bool softDeletePointGet();
 bool pointGetReady()const;
 bool pointGetFinished()const;
 bool admitted()const{return admitted_;}
 int32_t total()const{return total_;}
 const std::array<NativeHarborRewardIcon,IconCount>&icons()const{return icons_;}
 const PointGetEffect&pointGet()const{return pointGet_;}
 bool retire();
private:
 NativeHarborRewardPresentationSource source_;
 std::array<float,9>data_{};
 std::array<NativeHarborRewardIcon,IconCount>icons_{};
 std::array<unsigned,TensSlots>phase_{};
 PointGetEffect pointGet_;
 int32_t total_=0,lastRemaining_=0;
 unsigned loaded_=0;
 uint64_t tick_=0;
 mutable bool checking_=false;
 bool iconsActive_=false;
 bool alive_=true,invoking_=false,admitting_=false,admitted_=false,hasTick_=false,entered_=false;
};
}
