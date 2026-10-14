#pragma once
#include "harbor_tutorial.hpp"
#include "event_native_runtime.hpp"
#include "battle_event_callbacks.hpp"
#include "battle_damage_application.hpp"
#include "battle_camera_requests.hpp"
#include "battle_packed_generic_jump.hpp"
namespace ttyd {
// Native storage/provider policy. All gameplay fields copied below retain source
// definitions. Packed entry allocation/initialization remains a scoped adapter.
class HarborTutorialPacked {
public:
 static constexpr uint32_t Battle=0x84000000,Units=0x84100000,Parts=0x84200000;
 EventNativeRuntime& vm;EventCallbackRegistry registry;BattleRandom random{0};
 int32_t chosenUnit=-1,chosenPart=0;
 BattlePackedMovePositionServices movement;BattlePackedGenericJumpServices jump;
 BattleCameraRequestServices camera;bool physicalCurrent=false;uint64_t sampledAt=UINT64_MAX;
 void samplePhysical(uint64_t milliseconds,const BattlePadSample*);
 explicit HarborTutorialPacked(EventNativeRuntime& r):vm(r){}
 bool provision();
 bool provisionFreshPouch(const char* originalYoshiName);
 bool provisionFreshFieldInformation(uint32_t configuration,uint32_t expectedGroup,uint32_t expectedStage);
 bool registerActor(HarborTutorialActor&);
 void recordInit(int32_t id,uint32_t event);void recordPhase(int32_t id,uint32_t event);void recordAttack(int32_t id,uint32_t event);
 void publish(const HarborTutorial&);void capture(HarborTutorial&);
 bool invoke(EventEntry&,uint32_t function,int32_t first,int32_t& result);
 bool readWord(uint32_t address,uint32_t& value) const{return vm.memory.readWord(address,value);}
private:
 std::array<bool,4> registered{};
 bool freshPouch=false;uint32_t nextSoundHandle=1;
 std::vector<HarborTutorialSoundRequest> sounds;
 uint32_t requestSound(uint32_t unit,uint32_t key,const std::array<float,3>&);
 uint32_t word(uint32_t) const;void word(uint32_t,uint32_t);
 uint8_t byte(uint32_t) const;void byte(uint32_t,uint8_t);
 void half(uint32_t,int32_t);void real(uint32_t,float);float real(uint32_t) const;
 int32_t raw(EventEntry&,unsigned) const;int32_t value(EventEntry&,unsigned);
 void output(EventEntry&,unsigned,int32_t);uint32_t actor(EventEntry&);
};
}
