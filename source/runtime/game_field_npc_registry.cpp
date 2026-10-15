#include "game.hpp"
#include "field_npc_registry_owner.hpp"
#include "battle_sequence_binding.hpp"
namespace ttyd {
// Explicit original owners only;creating a binding does not activate original
// field Main or turn the legacy native Game scene into a sequence callback.
std::unique_ptr<BattleSequenceBinding> Game::bindOriginalBattleSequence(EventNativeRuntime& vm,const char* root){return BattleSequenceBinding::bind(*this,vm,root);}
// One cold registry over existing shared pose pool. Publish Game owner only
// after same-memory binding/initialization succeeds;returned mapping lifetime
// remains caller-owned. Existing Game owner refuses a second cold registry.
std::unique_ptr<FieldNpcRegistryBinding> Game::initializeOriginalFieldNpcs(EventAddressSpace& memory,uint32_t base){
 if(originalFieldNpcs)return {};
 auto owner=FieldNpcRegistryOwner::cold(base,fieldParties.originalPool);auto binding=FieldNpcRegistryBinding::bind(owner,memory);
 if(!binding || !binding->initialize())return {};
 originalFieldNpcs=std::move(owner);return binding;
}
}
