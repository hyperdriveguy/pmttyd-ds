#pragma once
#include <array>
#include <cstdint>
#include <memory>
#include <optional>
#include <vector>
namespace ttyd {
struct EventEntry;
// Typed native effect allocation: never a guest EffectEntry/payload pointer.
struct NativeHarborNiceSpawn {
 uint32_t type=0;
 std::array<float,3> position{};
};
struct NativeHarborNiceServices {
 void* user=nullptr;std::shared_ptr<const void> lifetime;
 bool(*current)(void*)=nullptr;
 std::optional<uint32_t>(*word)(void*,uint32_t)=nullptr;
 std::optional<int32_t>(*value)(void*,EventEntry&,int32_t)=nullptr;
 std::optional<uint64_t>(*spawn)(void*,const NativeHarborNiceSpawn&)=nullptr;
 bool(*scale)(void*,uint64_t,float)=nullptr;
 std::optional<int32_t>(*sound)(void*,uint32_t,const std::array<float,3>&)=nullptr;
 bool(*pitch)(void*,int32_t,int32_t)=nullptr;
};
class NativeHarborNiceCursor {
public: uint32_t pending()const{return pending_;}
private:
 friend bool resumeNativeHarborNice(EventEntry&,const NativeHarborNiceServices&,NativeHarborNiceCursor&);
 struct Read {uint32_t kind,address,value;};
 NativeHarborNiceServices services_;const EventEntry* event_=nullptr;
 uint32_t id_=0,command_=0,args_=0,pending_=0,stage_=0;
 bool invoking_=false,complete_=false;
 std::vector<Read> reads_;
 NativeHarborNiceSpawn spawn_;std::array<float,3> soundPosition_{};
 uint32_t sound_=0;float scale_=0;uint64_t effect_=0;int32_t handle_=0;
};
// Complete original ACSuccessEffect80106fa0/344 decisions/read order and linked
// entry/scale/PSND/pitch ordering. Spawn is an explicit native visual policy;
// successful vector capture alone is insufficient. Caller retains this cursor.
bool resumeNativeHarborNice(EventEntry&,const NativeHarborNiceServices&,NativeHarborNiceCursor&);
}
