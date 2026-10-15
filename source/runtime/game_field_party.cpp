#include "game.hpp"
namespace ttyd {
// Weak entry/removal leases and the professor's retained party child all
// serialize party mutation. Expiry is lease completion, not a source query.
bool Game::nativePartyPending() const{return !fieldParties.nativeEntryOwner.expired() || !fieldParties.nativeRemovalOwner.expired() || professorStory.nativePartyPending();}
FieldPartyScriptBindings Game::fieldPartyScriptBindings(){
 // Native sessions bind the actual shared source namespace. Legacy standalone
 // scene harnesses retain their explicitly partial projection behavior.
 if(!fieldParties.nativeAnimation)return {};
 FieldPartyScriptBindings b;b.parties=&fieldParties;b.mario=&fieldMarioBattle;b.currentIndex=&fieldPartyCurrentIndex;
 b.pouch=progress.originalPouch;b.root=fieldPlayerPoseAssetRoot();b.appearance=fieldPartyAppearanceServices();
 // Resource bank3 is c_pkr (Goombella), after the three Mario banks. Absence
 // remains an unbound bank; do not substitute a synthesized follower namespace.
 const auto resources=fieldPlayerPoseResources();if(resources[3])b.bank=resources[3]->bank;return b;
}
}
