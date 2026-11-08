#pragma once
#include "battle_unit_entry_continuation.hpp"
#include "harbor_source_map_pool.hpp"
#include "harbor_tutorial_pose_resources.hpp"
#include "native_field_animation.hpp"
#include <functional>
namespace ttyd {
struct HarborSourceUnitConstructionBinding {
 EventNativeRuntime* vm=nullptr;
 HarborSourceMapPool* pool=nullptr;
 uint32_t battle=0;
 std::shared_ptr<NativeFieldAnimation> animation;
 std::shared_ptr<const HarborTutorialPoseResources> resources;
 std::shared_ptr<const void> lifetime;
 std::function<bool()> current;
};
// One actual source constructor at a time. Caller retains VM/map and retires
// this owner before them; animation resources and shared original-name aliases
// remain owned across source Entry holds. No rendering receipt is produced.
class HarborSourceUnitConstruction {
public:
 static std::unique_ptr<HarborSourceUnitConstruction> create(HarborSourceUnitConstructionBinding);
 ~HarborSourceUnitConstruction();
 std::optional<uint32_t> resume(uint32_t definition);
 // Begin the next distinct source call only after its predecessor completed.
 bool next();
 uint32_t pending() const;
 bool current() const;
private:
 HarborSourceUnitConstruction();
 struct State;std::unique_ptr<State> state_;bool invoking_=false;
};
}
