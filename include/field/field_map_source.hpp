#pragma once
#include <array>
#include <cstdint>
#include <optional>
namespace ttyd {
namespace FieldMapSource {
constexpr uint32_t Init=0x8002af18,Find=0x8001f878,Entry=0x8002a46c;
constexpr uint32_t EntrySub=0x8002a508,Object=0x8002a780,Count=0x8002b120,CountSub=0x8002b16c,Recalculate=0x80028130;
constexpr uint32_t Work=0x8041e660,ActiveGroup=0x8041e664,Auxiliary=0x8041e668,PrimaryHeap=0x8041e808;
constexpr uint32_t GroupBytes=0x2f4,WorkBytes=2*GroupBytes,EntryBytes=0x178,ObjectBytes=0x134;
constexpr uint32_t ObjectCount=0x150,ObjectArray=0x154,Root=0xa8;
constexpr uint32_t Joint=8,World=0x1c,Authored=0x4c,Animated=0x7c,Additional=0xac,Child=0xe4,Sibling=0xe8;
constexpr uint32_t Dirty=0x200,HasAnimation=0x2000,HasAdditional=0x00200000;
constexpr uint32_t SourceR2=0x80427440,SourceR13=0x8041cf20;
}
struct FieldMapSourceCall {
 uint32_t function=0;
 std::array<uint32_t,8> arguments{};
 std::array<uint32_t,3> scalars{}; // Original f1..f3 binary32 input bits.
 std::optional<uint32_t> result,floatingResult;
};
struct FieldMapSourceServices {
 void* user=nullptr;
 bool (*word)(void*,uint32_t,uint32_t&)=nullptr;
 bool (*byte)(void*,uint32_t,uint8_t&)=nullptr;
 bool (*store)(void*,uint32_t,uint32_t)=nullptr;
 bool (*storeHalf)(void*,uint32_t,uint16_t)=nullptr;
 bool (*storeByte)(void*,uint32_t,uint8_t)=nullptr;
 // Exact reached allocator/memory/SDK/strcmp/panic children retain their own
 // effects on false. Acknowledgment publishes only explicit result registers.
 bool (*call)(void*,FieldMapSourceCall&)=nullptr;
};
struct FieldMapSourceCursor {
 FieldMapSourceServices owner;
 bool active=false,queued=false;
 uint32_t function=0,pc=0,lr=0,ctr=0,step=0;
 std::array<uint32_t,32> registers{},floats{};
 std::array<bool,32> condition{};
 FieldMapSourceCall request;
};
// Complete source bodies compiled to a fixed native continuation graph; no
// dynamic code fetch/guest instruction decoder. Stack addresses refer to real
// caller-owned VM scratch, including recursive save areas and SDK temporaries.
// ABI register seeds outside input arguments are a native private-frame policy.
// False/unengaged retains the exact reached primitive; budget yields do not
// advance simulation. Hold cursor/providers/scratch/caller lifetime stationary.
std::optional<uint32_t> resumeFieldMapSource(uint32_t function,
 const std::array<uint32_t,5>& arguments,uint32_t stackTop,
 const FieldMapSourceServices&,FieldMapSourceCursor&,unsigned visits=4096);
}
