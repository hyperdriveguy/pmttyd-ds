#include "game.hpp"
#include "field_npc_registry_owner.hpp"
#include "battle_sequence_binding.hpp"
namespace ttyd {
std::unique_ptr<BattleSequenceBinding> Game::bindOriginalBattleSequence(EventNativeRuntime& vm,const char* root){return BattleSequenceBinding::bind(*this,vm,root);}
std::unique_ptr<FieldNpcRegistryBinding> Game::initializeOriginalFieldNpcs(EventAddressSpace& memory,uint32_t base){
 if(originalFieldNpcs)return {};
 auto owner=FieldNpcRegistryOwner::cold(base);auto binding=FieldNpcRegistryBinding::bind(owner,memory);
 if(!binding || !binding->initialize())return {};
 originalFieldNpcs=std::move(owner);return binding;
}
}
