#pragma once
#include "native_harbor_nice.hpp"
#include "battle_startup_resources.hpp"
#include <functional>
namespace ttyd {
// Source Entry/Draw operands resolved from this battle's genuine GP/data.
// Native playback is a presentation policy, not an original effect-pool record.
struct NativeHarborNiceVisualRequest {
 NativeHarborNiceSpawn effect;uint32_t sourceKey=0,context=0,language=0;
 float modelScale=0,fadeStart=0;
 uint64_t milliseconds=0;
};
struct NativeHarborNiceConsumer {
 void*user=nullptr;std::shared_ptr<const void>lifetime;
 bool(*current)(void*)=nullptr;
 std::optional<uint64_t>(*spawn)(void*,const NativeHarborNiceVisualRequest&)=nullptr;
 bool(*scale)(void*,uint64_t,float)=nullptr;
};
// One finite native instance arena. Budgets are port allocations, not recovered
// effect-pool limits. Import/GPU-buffer/program proof belongs to consumerCurrent.
class NativeHarborNiceVisual {
public:
 static constexpr size_t InstanceCapacity=8,VerticesPerInstance=256;
 struct Instance {
  uint64_t id=0;NativeHarborNiceVisualRequest request;float scale=0,alpha=1;
  bool scaled=false;ActorAnimationSample sample;
 };
 NativeHarborNiceVisual(BattleStartupResources&,std::shared_ptr<const void>,std::function<bool()>consumerCurrent);
 std::optional<uint64_t>spawn(const NativeHarborNiceVisualRequest&);
 bool scale(uint64_t,float);
 bool advance(uint64_t milliseconds);
 bool current()const;
 void retire();
 const std::array<Instance,InstanceCapacity>&instances()const{return instances_;}
private:
 BattleStartupResources*resources_;std::shared_ptr<const void>lifetime_;
 std::function<bool()>consumerCurrent_;std::array<Instance,InstanceCapacity>instances_;
 uint64_t next_=1,last_=0;bool retired_=false,invoking_=false;
};
}
