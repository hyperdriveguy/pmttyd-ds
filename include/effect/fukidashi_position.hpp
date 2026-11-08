#pragma once
#include "fukidashi_effect.hpp"

namespace ttyd {
enum class FukidashiPositionBinary {Add,Subtract,Multiply,Divide};
struct FukidashiPositionServices {
 EventMemoryServices memory;
 void* user=nullptr;
 // Pure synchronous source single-precision operations, including fused FMA.
 float (*binary)(void*,FukidashiPositionBinary,float,float)=nullptr;
 float (*fused)(void*,float,float,float)=nullptr;
 // Original xoris/4330 signed-word bias conversion followed by fsubs.
 // The actual raw binary64 bias is read from guest memory at each use.
 float (*biasConvert)(void*,uint32_t wrappedWord,uint64_t bias)=nullptr;
 std::optional<uint32_t> (*camera)(void*,int32_t cameraId)=nullptr;
 // Original double sin/cos; caller applies the source frsp return narrowing.
 std::optional<double> (*sine)(void*,float radians)=nullptr;
 std::optional<double> (*cosine)(void*,float radians)=nullptr;
 // Whole PSMTXRotAxisRad, including normalization. Result is row-major3x4.
 // Retain the actual SDK child across nullopt; do not acknowledge a request
 // without its completed matrix. Axis/radians remain captured across retries.
 std::optional<std::array<float,12>> (*rotate)(void*,const std::array<float,3>& axis,float radians)=nullptr;
 std::optional<uint32_t> (*mario)(void*)=nullptr;
 std::optional<uint32_t> (*partyId)(void*,bool extra)=nullptr;
 std::optional<uint32_t> (*partyPointer)(void*,uint32_t id)=nullptr;
};
// Whole eff_fukidashi.o calc_pos80133480. Inline storage and durable local
// matrix/axis identities; no invented guest stack, per-call heap or generic
// screen anchor. Caller owns mapped guest domains and all provider lifetimes.
class FukidashiPositionCursor {
public:
 FukidashiPositionCursor()=default;
 FukidashiPositionCursor(const FukidashiPositionCursor&)=delete;
 FukidashiPositionCursor& operator=(const FukidashiPositionCursor&)=delete;
 bool active()const{return active_;}
 uint32_t pending()const{return pending_;}
 bool reset(); // Drops continuation only; never restores earlier source stores.
private:
 enum class Phase {Camera,PrepareSine,Sine,PrepareCosine,Cosine,PrepareRotation,Rotation,Attachment,Mario,PartyId,PartyPointer,Coordinates,Tail};
 bool active_=false,invoking_=false,extra_=false,attached_=false;
 Phase phase_=Phase::Camera;
 uint32_t pending_=0,payload_=0,camera_=0,argumentProduct_=0,actor_=0,actorPosition_=0,partyId_=0;
 int32_t secondArgument_=0;
 FukidashiPositionServices owner_;
 float trigInput_=0,rotationInput_=0;
 std::array<float,3> axis_{};
 std::array<float,12> matrix_{};
 friend bool resumeFukidashiPosition(uint32_t,int32_t,const FukidashiPositionServices&,FukidashiPositionCursor&);
};
// One actual calculation, used by both Entry and attached Main updates.
// Fixed attachment0 is calculated at Entry only by its source caller; missing
// party preserves old XYZ but still executes the rotation/auxiliary tail.
bool resumeFukidashiPosition(uint32_t payload,int32_t secondArgument,const FukidashiPositionServices&,FukidashiPositionCursor&);
}
