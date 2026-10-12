# Unit registry reset,setter bounds,and init-event readiness scan

Complete BtlUnit_Init80129694/0x4c,BattleSetUnitPtr800f88b4/0x18,and
BattleCheckEndUnitInitEvent80129994/0x7c,static US G8ME01.
[Verifier](../../../tools/verify_battle_initialization_dispatch.py) includes
these full bodies.[Init sequence](BATTLE_INIT_SEQUENCE_DISPATCH_AND_RESOURCE_READINESS.md)
provides their caller context.No original execution or memory/lifetime proof.

## Reset retains one global B and calls setter64 times

BtlUnit_Init consumes no passed battle argument.Load CURRENT global B from
8041eb90 once;retain it.Start I0.For I0..63,in order,request
BattleSetUnitPtr(retainedB,I,0),increment I,compare signedI<64.After all64
calls,return1 explicitly.No actor/part/resource/event delete or heap free;
no ID-ordering list,snapshot,pouch,current actor,type/state field clear.
Clearing registry membership is not a complete unit destruction protocol.

Complete BattleSetUnitPtr(B,I,V) compares I SIGNED with64.If I>=64,return
without store.If I<64,compute wrap32(B+4*I),store V unchanged at+0x20.
There is NO lower-bound check:negative IDs pass and can address before the
nominal registry.This differs from the
[complete getter](FIRST_ATTACK_PAIR_SELECTION_AND_GETTERS.md),which rejects
negative IDs.No unit-ID rewrite or pointer-null/validity guard inside setter.
Its rawr3 is B+4*I on store path,passedB on rejected path;no explicit success.

Only reset's constrained0..63 inputs establish its64 distinct registry-zero
stores;do not promote them into an invariant for all setter callers.Native
address policies must remain distinct from original accepted-input semantics.

## Readiness visits all64 current slots,including after pending IDs

BattleCheckEndUnitInitEvent retains passedB,sets local READY1 and retained
zeroZ0.Scan I0..63,in order:

1. Request BattleGetUnitPtr(B,I),retain returned U.
2. U0 skips that slot's remaining reads/services.
3. Read CURRENT WORD(U+0x288) as initEventId.Zero skips event query.
4. Nonzero requests evtCheckID(id).Nonzero sets READY0,leaves U288 untouched;
   zero stores WORD(retainedU+0x288)=0.
5. Continue to next I even after READY0.Return READY after all64 slots.

Do not return on first live event,skip completed-ID cleanup in later units,or
cache all registry units before the scan.Event checks can change global/registry/
unit fields;slot queries are fresh,but completed-ID zero store uses the retained
unit pointer without checking whether its ID or registry membership changed.
The input ID is captured before evtCheckID;the clear does not reread/compare
it afterward.No firstCall/event parent/category/constructor/delete operation
occurs in this helper.Original event-ID scheduler lifecycle remains separate.

## Source binding and required native behavior

Scoped aligned DOL search pins BattleInit call800f9010→BtlUnit_Init and
btlseqInit call8011eee4→Btl_UnitSetup exactly.[Existing stage-gate](BATTLE_ACTION_FEEDBACK_AND_STAGE_GATE.md)
places registry reset before snapshot/stage/audience initialization.Registry
publication during [complete unit entry](BATTLE_UNIT_ENTRY_COMPLETE.md) precedes
later init work;this readiness scan therefore matters after setup even when
all allocation calls have already returned.

Implement retained/reset global owner versus passed scan owner,signed setter
upper bound,64 ordered calls,fresh per-slot queries,nonzero ID checks,retained
unit clear,and full scan despite pending events.Reuse original definitions and
VM services;do not fabricate default units/readiness.Next unresolved domains
are actual scheduler/start/end invocation,source/global changes,resources/event
execution,query reachability,Game/platform/original runtime/full-game coverage.
