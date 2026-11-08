#include "native_facade_registration.hpp"
#include <cassert>
#include <iostream>
using namespace ttyd;
int main(){
 NativeFacadeRegistration::Proof dead;
 {NativeFacadeRegistration a;const auto pa=a.proof();assert(pa.current());NativeFacadeRegistration b=a;const auto pb=b.proof();assert(pa.current()&&pb.current());
  b=a;assert(pa.current()&&!pb.current()&&b.proof().current());auto same=b.proof();auto* alias=&b;b=*alias;assert(same.current());b=std::move(*alias);assert(same.current());
  const auto moved=a.proof();NativeFacadeRegistration c=std::move(a);assert(!moved.current()&&c.proof().current());
  const auto old=c.proof();c=std::move(b);assert(!old.current()&&!same.current()&&c.proof().current());dead=c.proof();c.retire();assert(!dead.current());}
 assert(!dead.current());NativeFacadeRegistration::Proof scope;{NativeFacadeRegistration a;scope=a.proof();assert(scope.current());}assert(!scope.current());
 std::cout<<"native facade registration copy/move/retirement tests PASS\n";return 0;
}
