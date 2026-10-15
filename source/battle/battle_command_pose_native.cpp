#include "battle_command_pose_native.hpp"
#include "battle_command_pose.hpp"
#include "battle_event_callbacks.hpp"
#include "battle_packed_status.hpp"
#include "event_native_runtime.hpp"
#include <array>
#include <cstdlib>
namespace ttyd {
bool nativeBattleCommandPose(EventNativeRuntime& runtime,uint32_t unit){
 BattleCommandPoseServices s;s.memory=nativeBattleActorServices(runtime);s.user=&runtime;
 s.status=[](void* p,uint32_t unit,int32_t id)->std::optional<bool>{return checkPackedBattleStatus(unit,id,nativeBattleActorServices(*static_cast<EventNativeRuntime*>(p)))!=0;};
 s.copyName=[](void* p,uint32_t part,uint32_t name){
  auto& memory=static_cast<EventNativeRuntime*>(p)->memory;
  if(!name||uint64_t(part)+0x208>0x100000000ULL)return false;
  const uint32_t destination=part+0x1c4;
  std::array<uint8_t,64> admitted{};unsigned size=0;
  for(;size<admitted.size();++size){
   if(uint64_t(name)+size>0xffffffffu||!memory.readByte(name+size,admitted[size]))return false;
   if(!admitted[size])break;
  }
  if(!size||size==admitted.size())return false;
  const uint64_t end=uint64_t(name)+size+1,outputEnd=uint64_t(destination)+size+1;
  if(uint64_t(name)<outputEnd&&uint64_t(destination)<end)return false;
  if(!memory.contiguousRam(destination,0x44))return false;
  // Preflight guarantees these exact original strcpy operations cannot hold.
  // Actual VM mappings/immutable name backing stay fixed through the call.
  for(unsigned i=0;i<=size;++i){uint8_t byte=0;const bool read=memory.readByte(name+i,byte);if(!read)std::abort();
   const bool wrote=memory.writeByte(destination+i,byte);if(!wrote)std::abort();
  }
  return true;
 };
 return packedBattleCommandPose(unit,s);
}
}
