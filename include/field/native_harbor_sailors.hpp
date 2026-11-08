#pragma once
#include "event_native_runtime.hpp"
#include "field_map_npc_assets.hpp"
#include "field_npc_registry_owner.hpp"
#include "field_npc_battle_binding.hpp"
#include "animation_group_request_binding.hpp"
#include "native_field_animation.hpp"
namespace ttyd {
// Native story-4/resume constructor policy, not execution of whole field Init.
// VM/registry binding outlive this unique owner. The intrinsic VM proof is
// checked before every borrowed access. Caller routes actual event retirements
// to regularHistory(), including when the regular body holds another provider.
class NativeHarborSailors {
public:
 static std::unique_ptr<NativeHarborSailors> prepare(EventNativeRuntime&,
   FieldNpcRegistryBinding&,std::shared_ptr<FieldNpcRegistryOwner>,
   std::shared_ptr<NativeFieldAnimation>,std::shared_ptr<const EventScriptAssets>,
   const FieldMapNpcAssets&,uint32_t privateInputBase,uint32_t actualAreaName);
 ~NativeHarborSailors();
 bool advance(); // No VM manager/input/time advancement; real file worker only.
 bool ready()const;
 uint32_t pending()const{return pending_;}
 std::shared_ptr<FieldNpcRegularEventHistory> regularHistory()const{return history_;}
 std::optional<uint32_t> actor(unsigned index)const;
 std::optional<uint32_t> regularEvent(unsigned index)const;
private:
 NativeHarborSailors(EventNativeRuntime&,FieldNpcRegistryBinding&,
   std::shared_ptr<FieldNpcRegistryOwner>,std::shared_ptr<NativeFieldAnimation>,
   std::shared_ptr<const EventScriptAssets>);
 bool current()const;bool readable(uint32_t,unsigned)const;
 bool text(uint32_t,std::string&,unsigned=64)const;
 bool word(uint32_t,uint32_t&)const;bool preflight();
 bool capture();bool finishRow();bool aliasModel();
 PackedMapNpcServices tailServices();
 struct Row {PackedMapNpcInput input;uint32_t setup=0,name=0,tribe=0,model=0,script=0,actor=0,event=0;FieldNpcEntryCursor entry;};
 EventNativeRuntime& vm_;NativeFacadeRegistration::Proof vmProof_;
 FieldNpcRegistryBinding& registry_;std::shared_ptr<FieldNpcRegistryOwner> owner_;
 std::shared_ptr<NativeFieldAnimation> animation_;
 std::shared_ptr<const EventScriptAssets> source_;
 std::shared_ptr<FieldNpcRegularEventHistory> history_;
 std::unique_ptr<FieldNpcRegistryBinding> animationRegistry_;
 std::unique_ptr<AnimationGroupRequestBinding> groups_;
 std::array<Row,2> rows_{};unsigned ordinal_=0,phase_=0;
 uint32_t gp_=0,inputBase_=0,pending_=0;bool mapped_=false,invoking_=false;
 std::vector<std::pair<uint32_t,uint32_t>> aliases_;
};
}
