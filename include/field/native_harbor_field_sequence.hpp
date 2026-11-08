#pragma once
#include "native_facade_registration.hpp"
#include "native_harbor_completion.hpp"
#include <functional>
#include <memory>
#include <optional>
namespace ttyd {
class EventNativeRuntime;class FieldNpcRegistryOwner;class FieldNpcBattleBinding;
class HarborTutorial;class NativeHarborFieldSequence;class FieldNpcSetupBinding;
class FieldNpcRegularEventHistory;struct BattleSequenceEntryServices;struct EventEntry;
// Sealed actual tutorial facade/runtime identity. Only the real facade can
// produce one; retaining it pins its actual VM, not permission after replacement.
class NativeHarborBattleIdentity {
public:
 NativeHarborBattleIdentity()=default;
 bool current()const{return lifetime_&&current_&&current_();}
 const std::shared_ptr<const void>& lifetime()const{return lifetime_;}
private:
 friend class HarborTutorial;friend class NativeHarborFieldSequence;
 NativeHarborBattleIdentity(std::shared_ptr<const void> p,std::function<bool()> c,std::function<bool(const NativeHarborCompletionReceipt&)> done):lifetime_(std::move(p)),current_(std::move(c)),completed_(std::move(done)){}
 bool completed(const NativeHarborCompletionReceipt&r)const{return current()&&completed_&&completed_(r);}
 std::shared_ptr<const void> lifetime_;std::function<bool()> current_;
 std::function<bool(const NativeHarborCompletionReceipt&)> completed_;
};
// Copy-safe identity, not a result/sequence scalar and not a VM pointer.
class NativeHarborBattleTicket {
public:NativeHarborBattleTicket()=default;
private:
 struct Seal;std::shared_ptr<const Seal> seal_;
 explicit NativeHarborBattleTicket(std::shared_ptr<const Seal>s):seal_(std::move(s)){}
 friend class NativeHarborFieldSequence;
};
// Explicit native frontend sequence policy. Original field start/wait execute
// unchanged. This never acknowledges original Sequence Main Init/Exit callbacks.
class NativeHarborFieldSequence {
public:
 static std::shared_ptr<NativeHarborFieldSequence> prepare(EventNativeRuntime&,
   std::shared_ptr<FieldNpcRegistryOwner>,NativeFacadeRegistration::Proof actualFieldContext);
 ~NativeHarborFieldSequence();
 bool attach(FieldNpcSetupBinding&,const BattleSequenceEntryServices&,
   std::shared_ptr<FieldNpcRegularEventHistory>);
 std::optional<int32_t> invokeSource(uint32_t,EventEntry&,bool first);
 void retire(const EventEntry&);
 uint32_t pending()const;
 std::optional<NativeHarborBattleTicket> request()const;
 bool entered(const NativeHarborBattleTicket&,const NativeHarborBattleIdentity&);
 // Caller has already completed real native teardown, canonical pouch commit
 // and field presentation return. Source wait still owns control/audio/Post.
 bool returned(const NativeHarborBattleTicket&,const NativeHarborCompletionReceipt&);
 bool current()const;
private:
 friend class NativeHarborBattleTicket;
 struct State;std::shared_ptr<State> state_;mutable bool invoking_=false;
 explicit NativeHarborFieldSequence(std::shared_ptr<State>);
 bool finishSource();
};
}
