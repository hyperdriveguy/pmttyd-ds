#pragma once
#include <array>
#include <memory>
#include <optional>
#include <cstdint>
namespace ttyd {
enum class SourceMarioForceNumberKind {Multiply,Divide,RoundSingle,FusedMultiplyAdd,FusedNegativeMultiplySubtract};
struct SourceMarioForceNumber {SourceMarioForceNumberKind kind=SourceMarioForceNumberKind::Multiply;uint32_t callsite=0;std::array<float,3>values{};double wide=0;};
struct SourceMarioForceServices {
 std::shared_ptr<const void>lifetime;void*user=nullptr;bool(*current)(void*)=nullptr;
 std::optional<uint32_t>(*word)(void*,uint32_t)=nullptr;
 std::optional<uint16_t>(*half)(void*,uint32_t)=nullptr;
 bool(*storeWord)(void*,uint32_t,uint32_t)=nullptr;
 bool(*storeHalf)(void*,uint32_t,uint16_t)=nullptr;
 // Actual original GetDashSpd80087ce4/GetWalkSpd80087d84 child.
 std::optional<float>(*speed)(void*,uint32_t function)=nullptr;
 // Original sin80272410/cos80271ea8 return double; frsp is separate below.
 std::optional<double>(*trig)(void*,uint32_t function,float radians)=nullptr;
 std::optional<float>(*number)(void*,const SourceMarioForceNumber&)=nullptr;
};
// Whole original marioForceMoveMain80095f64/94. One invocation per actually
// admitted simulation visit AFTER movement/support; no elapsed-time parameter.
class SourceMarioForceMoveMain {
public:
 SourceMarioForceMoveMain();~SourceMarioForceMoveMain();
 SourceMarioForceMoveMain(const SourceMarioForceMoveMain&)=delete;
 SourceMarioForceMoveMain&operator=(const SourceMarioForceMoveMain&)=delete;
 bool resume(const SourceMarioForceServices&);bool complete()const;bool reset();uint32_t pending()const;
private:struct State;std::unique_ptr<State>state_;bool invoking_=false;
};
// Whole original movePos80005de4/a8, used by the ordinary actual forced-step
// branch. Caller owns real branch/field admission; unsupported forms stay held.
class SourceMarioMovePos {
public:
 SourceMarioMovePos();~SourceMarioMovePos();
 SourceMarioMovePos(const SourceMarioMovePos&)=delete;
 SourceMarioMovePos&operator=(const SourceMarioMovePos&)=delete;
 bool resume(uint32_t actualXAddress,uint32_t actualZAddress,float speed,float angle,const SourceMarioForceServices&);
 bool complete()const;bool reset();uint32_t pending()const;
private:struct State;std::unique_ptr<State>state_;bool invoking_=false;
};
}
