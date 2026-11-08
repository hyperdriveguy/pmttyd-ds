#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include <optional>
namespace ttyd {
// Typed transport for original private-stack values. No guest stack address is
// manufactured. Only live non-private integer arguments appear in integers.
enum class BattleTargetPrivate { None, CursorOutput, ProjectPoint, IconPoint, WindowColor };
struct BattleTargetDisplayRequest {
 uint32_t function=0,callsite=0;
 std::array<uint32_t,3> integers{};
 BattleTargetPrivate transport=BattleTargetPrivate::None;
 std::array<float,3> point{};
 std::array<float,5> scalars{};
 uint32_t color=0;
};
struct BattleTargetDisplayReply {
 // For GetCursorPtr this is the written private output cell, NOT its r3 return.
 uint32_t value=0;
 std::array<float,3> point{};
};
struct BattleTargetDisplayServices {
 void* user=nullptr;
 bool (*current)(void*)=nullptr;
 // Total reads require caller-owned source mappings to remain admitted during
// synchronous read groups. The native owner must prove bounds and lifetime.
 uint32_t (*word)(void*,uint32_t)=nullptr;
 uint16_t (*half)(void*,uint32_t)=nullptr;
 uint8_t (*byte)(void*,uint32_t)=nullptr;
 // nullopt retains this exact request; engaged reply completes the WHOLE child.
 std::optional<BattleTargetDisplayReply> (*request)(void*,const BattleTargetDisplayRequest&)=nullptr;
 // Source fsubs accepts double operands and rounds its result to single.
 // Source fadds receives single operands. Both may hold before completion.
 std::optional<float> (*subtractToSingle)(void*,double,double)=nullptr;
 std::optional<float> (*add)(void*,float,float)=nullptr;
};
class BattleTargetDisplayCursor {
public:
 BattleTargetDisplayCursor();~BattleTargetDisplayCursor();
 BattleTargetDisplayCursor(const BattleTargetDisplayCursor&)=delete;
 BattleTargetDisplayCursor& operator=(const BattleTargetDisplayCursor&)=delete;
 bool active()const;bool stale()const;uint32_t pending()const;
 uint32_t pendingCallsite()const;bool reset();
private:
 struct State;std::unique_ptr<State> state_;bool invoking_=false;
 friend bool resumePackedBattleTargetDisplay(uint32_t,const BattleTargetDisplayServices&,BattleTargetDisplayCursor&);
};
// Whole original 80120a44/0x5a0. Captured battle is r4; original r3 is unused.
// Draw completion includes real final btl_camera_set_mode(0,0). No animation,
// CPU, event or action-command clock may advance while this Draw is held.
bool resumePackedBattleTargetDisplay(uint32_t battle,const BattleTargetDisplayServices&,BattleTargetDisplayCursor&);
}
