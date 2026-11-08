#include "harbor_tutorial.hpp"
#include <cassert>
using namespace ttyd;
int main(){
 assert(loadHarborTutorialAssets("romfs"));
 NativeFacadeRegistration::Proof destroyed;
 std::shared_ptr<const void>oldRuntime;
 {
  HarborTutorial original;assert(!original.nativeGameplayFacadeProof().current());
  assert(original.beginFresh(0,10,10,4));
  const auto first=original.nativeGameplayFacadeProof();assert(first.current());
  oldRuntime=original.nativeGameplayLifetime().lock();assert(oldRuntime);
  HarborTutorial copy=original;
  assert(!copy.nativeGameplayFacadeProof().current()); // Copies cannot mint a birth.
  assert(original.nativeGameplayFacadeProof().current());
  assert(!original.beginFresh(1,10,10,4)); // Already-running source owner refuses.
  assert(first.current());
  original=HarborTutorial{}; // Explicit Game replacement boundary revokes birth.
  assert(!first.current());
  assert(original.beginFresh(1,10,10,4));
  assert(!first.current()); // Old VM can remain alive without its old facade.
  assert(original.nativeGameplayLifetime().lock()!=oldRuntime);
  destroyed=original.nativeGameplayFacadeProof();assert(destroyed.current());
 }
 assert(!destroyed.current()&&oldRuntime);
}
