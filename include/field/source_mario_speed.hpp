#pragma once
#include <cstdint>
#include <memory>
#include <optional>
namespace ttyd {
struct SourceMarioSpeedServices {
 std::shared_ptr<const void>lifetime;void*user=nullptr;bool(*current)(void*)=nullptr;
 std::optional<uint32_t>(*word)(void*,uint32_t)=nullptr;
 std::optional<uint8_t>(*byte)(void*,uint32_t)=nullptr;
 std::optional<float>(*multiply)(void*,uint32_t sourceCallsite,float,float)=nullptr;
};
// Whole original GetDashSpd80087ce4/a0 or GetWalkSpd80087d84/9c,
// including the reached whole marioBgmodeChk8005c164/10 child.
class SourceMarioSpeed {
public:
 SourceMarioSpeed();~SourceMarioSpeed();
 SourceMarioSpeed(const SourceMarioSpeed&)=delete;
 SourceMarioSpeed&operator=(const SourceMarioSpeed&)=delete;
 std::optional<float>resume(uint32_t function,const SourceMarioSpeedServices&);
 bool complete()const;bool reset();uint32_t pending()const;
private:struct State;std::unique_ptr<State>state_;bool invoking_=false;
};
}
