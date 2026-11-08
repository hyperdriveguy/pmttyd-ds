#pragma once
#include <array>
#include <cstdint>
#include <optional>
namespace ttyd {
namespace SourceCameraBootstrap {
constexpr uint32_t Init=0x8000fd9c,Entry=0x8000fac8,Perspective=0x8029e7e8,Ortho=0x8029e8b8,RenderMode=0x8002eb30;
// Native bounded CPU contribution; NOT a callable DEMOInit replacement.
constexpr uint32_t SelectedModeCopy=0x8002ed34,SelectedModeEnd=0x8002edb8;
constexpr uint32_t SourceR2=0x80427440,SourceR13=0x8041cf20;
}
struct SourceCameraCall {
 uint32_t function=0;
 std::array<uint32_t,8> arguments{};
 std::array<uint64_t,8> scalars{}; // Actual f1..f8 binary64 bits, no early narrowing.
 std::optional<uint32_t> result;
 std::optional<uint64_t> floatingResult;
};
struct SourceCameraServices {
 void* user=nullptr;
 bool (*word)(void*,uint32_t,uint32_t&)=nullptr;
 bool (*half)(void*,uint32_t,uint16_t&)=nullptr;
 bool (*store)(void*,uint32_t,uint32_t)=nullptr;
 bool (*storeHalf)(void*,uint32_t,uint16_t)=nullptr;
 bool (*call)(void*,SourceCameraCall&)=nullptr;
};
struct SourceCameraCursor {
 SourceCameraServices owner;
 bool active=false,queued=false;
 uint32_t function=0,pc=0,lr=0,step=0,transfer=0;
 std::array<uint32_t,32> registers{},paired{};
 std::array<uint64_t,32> floats{};
 SourceCameraCall input,request;
};
// Complete fixed source bodies; caller owns stationary stack and services.
// The selected-copy contribution seeds r31 from arguments[0] and stops after
// the original final branch, without acknowledging surrounding DEMOInit work.
// Private caller registers/FPSCR are not an arbitrary Gekko execution ABI.
std::optional<uint32_t> resumeSourceCamera(const SourceCameraCall&,uint32_t stackTop,
 const SourceCameraServices&,SourceCameraCursor&,unsigned visits=4096);
}
