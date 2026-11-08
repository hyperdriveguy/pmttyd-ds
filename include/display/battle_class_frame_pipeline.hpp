#pragma once
#include "battle_class_publication.hpp"
#include "battle_class_display.hpp"
#include <functional>
namespace ttyd {
// Native composition owner for one actual class UI allocation. Source tables,
// backend and source contexts must outlive the binding until retire(). Borrowed
// original assets/font/labels must outlive every copied published snapshot too.
// Never replace backend callbacks while bound. RNG is a total owner. Footer
// callbacks are actual caller-supplied retained owners, not success stubs.
class BattleClassFramePipeline {
public:
  BattleClassFramePipeline();
  ~BattleClassFramePipeline();
  BattleClassFramePipeline(const BattleClassFramePipeline&)=delete;
  BattleClassFramePipeline& operator=(const BattleClassFramePipeline&)=delete;
  // A fresh actual allocation token is required, including guest-address reuse.
  // allocationCurrent is a pure lifetime predicate proving the exact live
  // token and B+1c78 ownership; it must not reenter or retire this object.
  // pointer equality/manual saved keys alone cannot establish lifetime. Missing
  // or false ownership/resources remain unavailable. Rebind only after retire.
  bool bind(std::shared_ptr<const uint8_t> owner,uint32_t allocation,
            const BattleClassDisplayServices&,const BattleClassRasterServices&,
            BattleClassBackend&,std::function<bool()> allocationCurrent);
  // Sequence is a monotonically increasing SIMULATION display-pass identity,
  // never a rendering frame. Held keys/record operands cannot be replaced.
  // Repeating the completed key acknowledges it without another source draw.
  // Begin the actual backend once, retain all source/geometry prefixes, publish
  // only after whole queued completion AND backend.complete(). FontString uses
  // the backend's explicit synchronous atomic-admission contract: false must
  // precede RNG/quad effects under immutable font/label and total RNG ownership.
  bool resume(const BattleClassPublicationKey&,uint32_t camera,uint32_t battle);
  bool active() const;
  // Dead allocation ownership exposes no old drawable snapshot/admission.
  const BattleClassPublication& publication() const;
  bool admit(const BattleClassPublicationKey&);
  std::optional<BattleClassPublicationKey> consumeAdmission();
  // A callback may request retirement; native context destruction is deferred
  // until that in-flight call returns, and no later provider/publish executes.
  // Native state/publication retirement only; never fabricates guest UI End,
  // frees original resources or changes guest readiness bytes.
  void retire();
private:
  struct State;
  std::unique_ptr<State> state_;
  // unavailable_ is the stable empty facade returned after ownership loss.
  BattleClassPublication publication_,unavailable_;
  bool invoking_=false,retirePending_=false;
};
}
