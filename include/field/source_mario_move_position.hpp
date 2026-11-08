#pragma once
#include "event_manager.hpp"
#include <array>
#include <memory>
#include <optional>
namespace ttyd {
enum class SourceMarioMoveNumberKind {Distance,Angle,Multiply,Divide,Add,SignedWordMinusDouble};
struct SourceMarioMoveNumber {
 SourceMarioMoveNumberKind kind=SourceMarioMoveNumberKind::Add;
 uint32_t function=0,callsite=0;
 std::array<float,4>values{};
 uint32_t word=0;uint64_t doubleBits=0;
};
struct SourceMarioMovePositionServices {
 std::shared_ptr<const void>lifetime;void*user=nullptr;
 bool(*current)(void*)=nullptr;
 std::optional<uint32_t>(*word)(void*,uint32_t)=nullptr;
 std::optional<uint16_t>(*half)(void*,uint32_t)=nullptr;
 std::optional<uint64_t>(*doubleWord)(void*,uint32_t)=nullptr;
 bool(*storeWord)(void*,uint32_t,uint32_t)=nullptr;
 bool(*storeHalf)(void*,uint32_t,uint16_t)=nullptr;
 std::optional<float>(*eventFloat)(void*,EventEntry&,uint32_t raw)=nullptr;
 std::optional<float>(*number)(void*,const SourceMarioMoveNumber&)=nullptr;
 std::optional<int32_t>(*truncate)(void*,float)=nullptr;
 // Actual original marioSetMotion8009455c child; owns any partial work.
 bool(*motion)(void*,int32_t action)=nullptr;
};
// Whole evt_mario_mov_pos2 800e5f50/180 plus whole marioSetForceMove
// 80095e90/d4. SDK/numeric/action services remain their actual owners.
class SourceMarioMovePosition {
public:
 SourceMarioMovePosition();~SourceMarioMovePosition();
 SourceMarioMovePosition(const SourceMarioMovePosition&)=delete;
 SourceMarioMovePosition&operator=(const SourceMarioMovePosition&)=delete;
 std::optional<int32_t>resume(EventEntry&,uint32_t actualEventAddress,bool firstCall,const SourceMarioMovePositionServices&);
 bool complete()const;uint32_t pending()const;
 // Only after whole callback completion (including original result0).
 // Next actual VM visit must then execute a new original poll invocation.
 bool reset();
private:struct State;std::unique_ptr<State>state_;bool invoking_=false;
};
}
