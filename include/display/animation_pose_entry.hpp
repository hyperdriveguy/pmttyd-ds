#pragma once
#include "battle_sequence_entry.hpp"
#include <string>
namespace ttyd {
// Original stack+8 filename has semantic backing, never a fabricated guest
// stack pointer. Bit i means args[i] uses text instead of an address token.
struct AnimationEntryCall {
 uint32_t function=0,result=0,low=0,stackArguments=0;
 std::array<uint32_t,5> args{};
 std::string text;
};
struct AnimationEntryServices {
 BattleSequenceEntryServices memory;
 bool (*call)(void*,AnimationEntryCall&)=nullptr;
};
struct AnimationEntryCursor {
 bool active=false,queued=false;
 uint32_t pc=0,pending=0,name=0,mode=0,ctr=0,stackRegisters=0,result=0;
 int condition=0;
 std::array<uint32_t,32> r{};
 std::array<uint32_t,2> f{}; // Float load/store bits, no arithmetic in this body.
 std::string stack;
 AnimationEntryCall call;
};
// Complete original animPoseEntry8004176c/4ec, including actual failure paths.
// Unavailable memory primitives have no effects; child providers retain their
// own partial work. Keep cursor/services/mappings. False is unavailable, not
// source -2. Exhaustion reaches the original null access.
bool enterAnimationPose(uint32_t name,uint32_t mode,const AnimationEntryServices&,AnimationEntryCursor&);
}
