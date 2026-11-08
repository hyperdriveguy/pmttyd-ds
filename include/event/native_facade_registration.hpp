#pragma once
#include <memory>
namespace ttyd {
// Native C++ facade identity only. This never certifies a reached source call,
// initialized guest memory, or resource readiness. Copies/moves create distinct
// facade identities; no registration is inherited by a checkpoint candidate.
// Mutation/destruction and all proof consumers are single-threaded/quiescent.
class NativeFacadeRegistration {
 struct State {bool active=true;};
public:
 class Proof {
 public:
  bool current()const{const auto s=state_.lock();return s&&s->active;}
 private:
  std::weak_ptr<const State> state_;
  explicit Proof(const std::shared_ptr<State>&s):state_(s){}
  friend class NativeFacadeRegistration;
 public:
  Proof()=default;
 };
 NativeFacadeRegistration():state_(std::make_shared<State>()){}
 ~NativeFacadeRegistration(){retire();}
 NativeFacadeRegistration(const NativeFacadeRegistration&):NativeFacadeRegistration(){}
 NativeFacadeRegistration(NativeFacadeRegistration&&other):NativeFacadeRegistration(){other.retire();}
 NativeFacadeRegistration&operator=(const NativeFacadeRegistration&other){if(this!=&other){retire();state_=std::make_shared<State>();}return *this;}
 NativeFacadeRegistration&operator=(NativeFacadeRegistration&&other){if(this!=&other){retire();other.retire();state_=std::make_shared<State>();}return *this;}
 Proof proof()const{return Proof(state_);}
 // Retirement only removes admission. It performs no guest writes/reset/free.
 void retire(){if(state_)state_->active=false;state_.reset();}
private:
 std::shared_ptr<State> state_;
};
}
