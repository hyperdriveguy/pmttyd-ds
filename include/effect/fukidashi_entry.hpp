#pragma once
#include "effect_pool.hpp"
#include "fukidashi_effect.hpp"

namespace ttyd {
struct FukidashiEntryRequest {
 int32_t kind=0,variant=0,duration=0;
 uint32_t attachment=0;
 std::array<float,3> position{};
 float radius=0,angle=0;
};
struct FukidashiEntryServices {
 EffectPoolServices pool;
 void* user=nullptr;
 // Pure synchronous binary32 operation, shared with the Main adapter.
 float (*binary)(void*,FukidashiBinary,float,float)=nullptr;
 // Actual calc_pos80133480; completion owns its computed XYZ/rotation stores.
 bool (*position)(void*,uint32_t payload,int32_t secondArgument)=nullptr;
 std::optional<uint32_t> (*mario)(void*)=nullptr;
 std::optional<uint32_t> (*partyId)(void*,bool extra)=nullptr;
 std::optional<uint32_t> (*partyPointer)(void*,uint32_t id)=nullptr;
 // Original psndSFXOn3D800d9848 accepts numeric93/94 or original string keys.
 // true acknowledges execution, not submission without an audio owner.
 bool (*sound)(void*,uint32_t key,const std::array<float,3>&)=nullptr;
};
// Whole effFukidashiEntry80133164. The real pool/heap, attachment calculation
// and audio remain owned children. Each retry pins original request FLOAT BITS,
// used provider identities and every already reached capture. No payload fill,
// null-allocation repair, invented guest handles or sound replay is supplied.
class FukidashiEntryCursor {
public:
 FukidashiEntryCursor()=default;
 FukidashiEntryCursor(const FukidashiEntryCursor&)=delete;
 FukidashiEntryCursor& operator=(const FukidashiEntryCursor&)=delete;
 bool active()const{return active_;}
 uint32_t pending()const{return pending_;}
 bool reset(); // Drops continuation only; parent owns partial allocation.
private:
 enum class Phase {Acquire,Allocate,Position,ResolveAttachment,Mario,PartyId,PartyPointer,SelectSound,Sound};
 bool active_=false,invoking_=false,extra_=false;
 Phase phase_=Phase::Acquire;
 uint32_t pending_=0,effect_=0,payload_=0,partyId_=0,sound_=0;
 FukidashiEntryRequest request_;
 FukidashiEntryServices owner_;
 std::array<float,3> soundPosition_{};
 friend std::optional<uint32_t> resumeFukidashiEntry(const FukidashiEntryRequest&,const FukidashiEntryServices&,FukidashiEntryCursor&);
};
// Success returns the actual source effect record and resets for a new call.
// An engaged zero is an executed source return; missing children remain held.
std::optional<uint32_t> resumeFukidashiEntry(const FukidashiEntryRequest&,const FukidashiEntryServices&,FukidashiEntryCursor&);
}
