#pragma once
#include <array>
#include <cstdint>
#include <optional>
namespace ttyd {
namespace FieldMapSource {
constexpr uint32_t Init=0x8002af18,Find=0x8001f878,Entry=0x8002a46c;
constexpr uint32_t EntrySub=0x8002a508,Object=0x8002a780,Count=0x8002b120,CountSub=0x8002b16c,Recalculate=0x80028130;
// Complete original room/file/worker entries. Load publishes group count only
// after its entire tail returns; Unload requires all room users to stay frozen.
constexpr uint32_t Load=0x80028d08,LoadEntry=0x80028f18,Unload=0x80028d78;
constexpr uint32_t AllocateTail=0x8002fe60,Free=0x8002fdc4,GetWork=0x8002b438;
constexpr uint32_t ArchiveOpen=0x800782e4,ArchiveContains=0x80078444,ArchiveDelete=0x800783b8;
constexpr uint32_t UnpackPalette=0x80037b6c,TextureGet=0x80037b5c;
constexpr uint32_t Search=0x8001f920,SearchSub=0x8001fd48,SearchPath=0x8001f958;
constexpr uint32_t BuildTexture=0x8002ad08,Bounds=0x8002af5c,DisplayLists=0x8002a228;
constexpr uint32_t FileOpen=0x80144e88,FileRead=0x80144e18,FileLength=0x80144dcc,FileClose=0x80144dd4;
constexpr uint32_t FileWorker=0x80145024,FileCompare=0x80145288,Sort=0x800064fc,SortPartition=0x80006680;
constexpr uint32_t HitEntry=0x80018658,HitEntrySub=0x800186f4,HitObject=0x8001896c,HitVertices=0x800165a8;
constexpr uint32_t HitCount=0x158,HitArray=0x15c,HitObjectBytes=0xe4,HitTriangleBytes=0x54;
constexpr uint32_t Work=0x8041e660,ActiveGroup=0x8041e664,Auxiliary=0x8041e668,PrimaryHeap=0x8041e808;
constexpr uint32_t GlobalsBytes=0x20; // Work through display-list capture flag8041e67c.
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
 bool (*half)(void*,uint32_t,uint16_t&)=nullptr;
};
struct FieldMapSourceCursor {
 FieldMapSourceServices owner;
 bool active=false,queued=false,carry=false;
 uint32_t function=0,pc=0,lr=0,ctr=0,step=0;
 std::array<uint32_t,32> registers{},floats{},paired{};
 // Private binary32 scalar/paired frame; initial GQR0 is unquantized float.
 // Reached hitCalc saves scalar-promoted binary64 and both paired lanes.
 // Arbitrary caller binary64 FPR/FPSCR state is outside this native ABI.
 uint32_t gqr5=0,transfer=0;
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
